#include "TrailingCrossStrategy.h"
#include "SDKAlgoManager.h"
#include <sstream>
#include <assert.h>

TrailingCrossStrategy::TrailingCrossStrategy(SDKAlgoManager& mgr) : mgr_(mgr), shuttingDown_(false), failed_(false),
replacing_(false), subId_(0)
{
}

TrailingCrossStrategy::~TrailingCrossStrategy()
{
    ttsdk::TTLogInfo("~TrailingCrossStrategy for algoId: %s", GetOrderId());
}

void  TrailingCrossStrategy::InternalStop()
{
    if (shuttingDown_) return;
    ttsdk::TTLogInfo("Stopping TrailingCrossStrategy for algoId: %s", GetOrderId());

    shuttingDown_ = true;
    // chain of events -- if there is a working  order, send a cancel for the order
    //   when the cancel ack is received, the order will be unsubscribed
    //   if not, unsubscribe to order
    //   when the ack for the order unsub is received, the prices will be unsubscribed
    //   if no order, unsubscribe prices
    //   when the ack for the price unsub is received, the algo is finished working
    //   so tell our algo tracking manager, this instance has stopped 
    //   that call will remove the algo from its collection of working algos and
    //   this object will be destroyed (nothing should be holding a reference to it)
    if (order_)
    {
        auto execRpt = order_->GetCurrentState();
        if (execRpt && execRpt->IsWorking())
        {
            if (failed_)
            {
                // leave the order in the market unmanaged
                order_->SetParent(nullptr);
                order_->Unsubscribe();
            }
            else
                SendCancel();
        }
        else
        {
            order_->Unsubscribe();
        }
    }
    else
    {
        ReleaseRisk();
    }
};

void  TrailingCrossStrategy::ReleaseRisk()
{
    ttsdk::RiskSide side = ttsdk::RiskSide::Buy;
    if (profile_.side == ttsdk::OrderSide::Sell)
        side = ttsdk::RiskSide::Buy;
    if (!algoOrder_->ReleaseRisk(instrument_, profile_.account_id, side))
    {
        // there was no risk to release so continue with the shutdown process
        OnRiskReleased(instrument_->GetInstrumentId(), profile_.account_id, side, ttsdk::AlgoResponseCode::Ok);
    }
}

void  TrailingCrossStrategy::OnOrderCancelled()
{
    order_->Unsubscribe();

};

ttsdk::AlgoResponseCode TrailingCrossStrategy::InitializeStrategy(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req, std::string& error)
{
    algoOrder_ = algoOrder;
    instrument_ = algoOrder->GetInstrument();

    ttsdk::AlgoResponseCode code = ttsdk::AlgoResponseCode::Ok;
    if (!instrument_)
    {
        error = "No instrument specified.";
        code = ttsdk::AlgoResponseCode::UnknownSymbol;
    }
    else if (std::isnan(req->GetQuantity()))
    {
        error = "Quantity is required but is missing.";
        code = ttsdk::AlgoResponseCode::InvalidQty;
    }
    else if (req->GetAccountId() == 0)
    {
        error = "AccountId is required but is missing";
        code = ttsdk::AlgoResponseCode::AccountNotSet;
    }
    else
    {
        lastReq_ = req;
        profile_.account_id = req->GetAccountId();
        profile_.quantity = req->GetQuantity();
        profile_.side = req->GetSide();
        profile_.type = ttsdk::OrderType::Limit;
        profile_.tif = ttsdk::TimeInForce::Day;
        // pull out the user parameters
        auto count = req->GetUserParameterCount();
        for (uint32_t i = 0; i < count; ++i)
        {
            auto param = req->GetUserParameter(i);
            if (strncmp(param.name, "TicksAway", sizeof(param.name)) == 0)
                ticksAway_ = param.v_int;
            else if (strncmp(param.name, "MaxRequotes", sizeof(param.name)) == 0)
                maxRequotes_ = param.v_int;
            else if (strncmp(param.name, "Requoted", sizeof(param.name)) == 0)
                requotes_ = param.v_int;
        }
        if (ticksAway_ == 0)
        {
            error = "TicksAway must be greater than 0";
            code = ttsdk::AlgoResponseCode::InvalidParameters;
        }
    }
    return code;
}

bool TrailingCrossStrategy::Recover(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req)
{
    if (!mgr_.RecoverSDKAlgos())
        failed_ = true;
    algoOrder_ = algoOrder;
    instrument_ = algoOrder->GetInstrument();
    std::string error;
    ttsdk::AlgoResponseCode code = InitializeStrategy(algoOrder, req, error);
    if (code != ttsdk::AlgoResponseCode::Ok)
    {
        ttsdk::TTLogInfo("Algo: %s %s. Unable to recover strategy.", algoOrder_->GetOrderId(), error.c_str());
        algoOrder->OnPendingRequestCompleted(code, error.c_str());
        return false;
    }
    if (req->GetChildOrderCount() >= 1)
    {
        // this algo manages at most one order
        assert(req->GetChildOrderCount() == 1);
        // reattach the order to this strategy and hook up the callbacks
        order_ = req->GetChildOrder(0);
        order_->Subscribe(*this);
        ttsdk::TTLogInfo("Algo: %s recovered working child order %s", algoOrder_->GetOrderId(), order_->GetOrderId());
        // initialize the profile
        profile_.price = order_->GetCurrentState()->GetPrice();
        // don't change the qty
        profile_.quantity = std::numeric_limits<double>::quiet_NaN();
    }
    else
    {
        profile_.quantity = req->GetQuantity() - algoOrder_->GetCurrentState()->GetCumQty();
    }
    subId_ = ttsdk::SubscribePrices(instrument_, 10, true, true, false, this);
    if (subId_ <= 0)
        algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::PriceSubFailed, "Unable to subscribe to prices.");
    else
        algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::Ok);
    return true;
}

bool TrailingCrossStrategy::Start(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req)
{
    algoOrder_ = algoOrder;
    instrument_ = algoOrder->GetInstrument();
    std::string error;
    ttsdk::AlgoResponseCode code = InitializeStrategy(algoOrder, req, error);

    if (code != ttsdk::AlgoResponseCode::Ok)
    {
        ttsdk::TTLogInfo("Algo: %s %s. Unable to start strategy.", algoOrder_->GetOrderId(), error.c_str());
        algoOrder->OnPendingRequestCompleted(code, error.c_str());
        return false;
    }
    ttsdk::RiskSide side = ttsdk::RiskSide::Buy;
    if (profile_.side == ttsdk::OrderSide::Sell)
        side = ttsdk::RiskSide::Buy;

    algoOrder_->ReserveRisk(instrument_, profile_.account_id, side, profile_.quantity,  profile_.quantity);
    algoOrder_->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::Ok);
    return true;
}

void TrailingCrossStrategy::OnRiskReserved(const uint64_t instrumentId, const uint64_t accountId, const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code)
{
    if (code != ttsdk::AlgoResponseCode::Ok)
    {
        ttsdk::TTLogWarning("TrailingCrossStrategy failed to reserve risk for algoId: %s [instrumentId: %llu accountId: %llu side: %s]", GetOrderId(), instrumentId, accountId, ttsdk::ToString(side));
        algoOrder_->FailAlgo("Position reserve request failed.");
    }
    else
    {
        ttsdk::TTLogInfo("TrailingCrossStrategy reserved risk for algoId: %s [instrumentId: %llu accountId: %llu side: %s]", GetOrderId(), instrumentId, accountId, ttsdk::ToString(side));
        subId_ = ttsdk::SubscribePrices(instrument_, 10, true, true, false, this);
        if (subId_ <= 0)
            algoOrder_->FailAlgo("Unable to subscribe to prices.");
    }
}

void TrailingCrossStrategy::OnUnsubscribed(const char*)
{
    order_.reset();
    requotes_ = 0;
    if (shuttingDown_)
    {
        ReleaseRisk();
    }
    else
    {
        if (!std::isnan(mktPrice_))
        {
            assert(!std::isnan(profile_.price));
            PlaceOrder(profile_.price);
        }
    }
}

void TrailingCrossStrategy::OnRiskReleased(const uint64_t instrumentId, const uint64_t accountId, const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code)
{
    if (code != ttsdk::AlgoResponseCode::Ok)
    {
        ttsdk::TTLogWarning("TrailingCrossStrategy failed to release risk for algoId: %s [instrumentId: %llu accountId: %llu side: %s]", GetOrderId(), instrumentId, accountId, ttsdk::ToString(side));
        algoOrder_->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::RiskRejectReceived, "Position release request failed.");
    }
    else
    {
        ttsdk::TTLogInfo("TrailingCrossStrategy released risk for algoId: %s [instrumentId: %llu accountId: %llu side: %s]", GetOrderId(), instrumentId, accountId, ttsdk::ToString(side));
        if (subId_ != 0)
            ttsdk::UnsubscribePrices(subId_);
        else
            OnUnsubscribed(subId_);
        subId_ = 0;
        instrument_.reset();
    }
}
void TrailingCrossStrategy::OnUnsubscribed(const uint64_t subId)
{
    if (shuttingDown_)
    {
        // this was a requested cancellation of the algo so need to respond to the pending cancel request
        algoOrder_->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::Ok);
    }
    // this algo has finished working and cleaned up all the subscription
    // so it is safe for the object to go away
    mgr_.StrategyStopped(algoOrder_->GetOrderId());
}




void  TrailingCrossStrategy::OnOrderReplaced()
{
    bool isExternalAction = order_->GetCurrentState()->IsExternalAction();
    if (replacing_ && !isExternalAction)
    {
        replacing_ = false;
        algoOrder_->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::Ok);
    }
};

void TrailingCrossStrategy::Update(ttsdk::SDKAlgoRequestPtr req)
{
    uint32_t orgTicksAway = ticksAway_;
    uint32_t orgMaxRequotes = maxRequotes_;
    auto count = req->GetUserParameterCount();
    for (uint32_t i = 0; i < count; ++i)
    {
        auto param = req->GetUserParameter(i);
        if (strncmp(param.name, "TicksAway", sizeof(param.name)) == 0)
            ticksAway_ = param.v_int;
        else if (strncmp(param.name, "MaxRequotes", sizeof(param.name)) == 0)
            maxRequotes_ = param.v_int;
    }
    if (ticksAway_ == 0)
    {
        ticksAway_ = orgTicksAway;
        maxRequotes_ = orgMaxRequotes;
        algoOrder_->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::InvalidParameters);
        return;
    }

    if (req->GetAccountId() != 0)
        profile_.account_id = req->GetAccountId();
    if (!std::isnan(req->GetQuantity()))
        profile_.quantity = req->GetQuantity();
    // reset the requote counter
    requotes_ = 0;
    if (!std::isnan(mktPrice_))
    {
        double newPrice = 0;
        if (profile_.side == ttsdk::OrderSide::Buy)
        {
            newPrice = instrument_->TickPriceDown(mktPrice_, ticksAway_);
        }
        else
        {
            newPrice = instrument_->TickPriceUp(mktPrice_, ticksAway_);
        }
        if (!double_eq(newPrice, profile_.price))
        {
            // calculated price changed so update order
            profile_.price = newPrice;
            replacing_ = true;
            order_->SendChange(profile_);
            // order change was sent - update callback response on the ack of the order request
            return;
        }
    }
    algoOrder_->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::Ok);

}

void TrailingCrossStrategy::Stop(ttsdk::SDKAlgoRequestPtr req)
{
    InternalStop();
}

void TrailingCrossStrategy::SendCancel()
{
    if (order_)
    {
        profile_.request_id++;
        order_->SendCancel(profile_);
    }
}

void TrailingCrossStrategy::SendChange(const double qty)
{
    if (order_)
    {
        profile_.request_id++;
        order_->SendChange(profile_);
    }
}



void TrailingCrossStrategy::OnDisconnect(const uint64_t subId, const ttsdk::InstrumentPtr&, const ttsdk::PriceEventType)
{
    // potentially cancel the orders if we lose the price stream 
}

void TrailingCrossStrategy::OnPriceUpdate(const uint64_t, const ttsdk::InstrumentPtr&, const ttsdk::PriceSnap& snap)
{
    if (shuttingDown_ || failed_) return;

    if (!order_)
    {
        PlaceOrder(snap);
    }
    else
    {
        double newPrice = 0;
        if (profile_.side == ttsdk::OrderSide::Buy)
        {
            if (snap.asks.count == 0)
            {
                mktPrice_ = NAN;
                SendCancel();
                ttsdk::TTLogInfo("Algo: %s No market, cancelling order: %s.", algoOrder_->GetOrderId(),
                    order_->GetOrderId());
                return;
            }
            mktPrice_ = snap.asks.levels[0].price;
            newPrice = instrument_->TickPriceDown(mktPrice_, ticksAway_);
        }
        else
        {
            if (snap.bids.count == 0)
            {
                mktPrice_ = NAN;
                SendCancel();
                ttsdk::TTLogInfo("Algo: %s No market, cancelling order: %s.", algoOrder_->GetOrderId(),
                    order_->GetOrderId());
                return;
            }
            mktPrice_ = snap.bids.levels[0].price;
            newPrice = instrument_->TickPriceUp(mktPrice_, ticksAway_);
        }
        if (!double_eq(newPrice, profile_.price))
        {
            ++requotes_;
            char msg[256];
            // cross the market if the max requotes limit is reached
            if (requotes_ >= maxRequotes_)
            {
                sprintf(msg, "requoting order at mkt price [%d requotes]", requotes_);
                newPrice = mktPrice_;
            }
            else
            {
                sprintf(msg, "requoted order [%d requotes]", requotes_);
            }

            // calculated price changed so update order
            profile_.price = newPrice;
            order_->SendChange(profile_);
            ttsdk::TTLogInfo("Algo: %s Send change request for order: %s.", algoOrder_->GetOrderId(),
                order_->GetOrderId());
            // update the user parameters
            ttsdk::UserParameter params[3];
            strcpy(params[0].name, "TicksAway");
            params[0].type = ttsdk::UserDataType::UserDataTypeInteger;
            params[0].v_int = ticksAway_;
            strcpy(params[1].name, "MaxRequotes");
            params[1].type = ttsdk::UserDataType::UserDataTypeInteger;
            params[1].v_int = maxRequotes_;
            strcpy(params[2].name, "Requoted");
            params[2].type = ttsdk::UserDataType::UserDataTypeInteger;
            params[2].v_int = requotes_;
            algoOrder_->GenerateUserResponse(msg, params, 3);

            ttsdk::TTLogInfo("Send order change to follow market.");
        }

    }
};
void TrailingCrossStrategy::OnError(const uint64_t subId, const ttsdk::InstrumentPtr& instrument,
    const ttsdk::SubscriptionError code, const char* error)
{
    std::stringstream stream;
    stream << "Algo: " << algoOrder_->GetOrderId()
        << "Error occurred subscribing for prices for instr=" << instrument->GetName()
        << " code=" << (int)code << " error=" << error << " subId=" << subId;
    ttsdk::TTLogError(stream.str().c_str());
};

void TrailingCrossStrategy::PlaceOrder(const ttsdk::PriceSnap& snap)
{
    // don't place order until there is an market
    double price = 0;
    if (profile_.side == ttsdk::OrderSide::Buy)
    {
        if (snap.asks.count == 0) return;
        price = instrument_->TickPriceDown(snap.asks.levels[0].price, ticksAway_);
    }
    else
    {
        if (snap.bids.count == 0) return;
        price = instrument_->TickPriceUp(snap.bids.levels[0].price, ticksAway_);
    }
    mktPrice_ = price;
    PlaceOrder(price);
}

void TrailingCrossStrategy::PlaceOrder(const double price)
{
    order_ = ttsdk::CreateOrderFromPriceSub(subId_);
    if (!order_)
    {
        std::string message("Order creation failed. Stopping strategy for algo order :");
        message += algoOrder_->GetOrderId();
        ttsdk::TTLogError(message.c_str());
        algoOrder_->FailAlgo(message.c_str());
        return;
    }
    order_->Subscribe(*this);
    profile_.request_id++;
    profile_.price = price;
    order_->SetParent(algoOrder_);
    order_->SendNew(profile_);

    std::stringstream stream;
    stream << "Algo:" << algoOrder_->GetOrderId() << " placed order TTOrderId:" << order_->GetOrderId();
    if (profile_.side == ttsdk::OrderSide::Buy)
        stream << " Buy ";
    else
        stream << " Sell ";
    stream << profile_.quantity << "@" << price;
    ttsdk::TTLogInfo(stream.str().c_str());
}


void TrailingCrossStrategy::OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt)
{
    std::stringstream stream;
    switch (execRpt->GetExecType())
    {
    case ttsdk::ExecType::New:
        stream << "Algo:" << algoOrder_->GetOrderId()
            << " Order successfully entered in market. RequestId=" << execRpt->GetRequestId() <<
            " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus());
        break;
    case ttsdk::ExecType::Canceled:
        stream << "Algo:" << algoOrder_->GetOrderId()
            << " Order successfully cancelled. RequestId=" << execRpt->GetRequestId() <<
            " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << " [external=" << execRpt->IsExternalAction() << "]";
        OnOrderCancelled();
        break;
    case ttsdk::ExecType::Replaced:
        stream << "Algo:" << algoOrder_->GetOrderId()
            << " Order replaced. RequestId=" << execRpt->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) <<
            " [external=" << execRpt->IsExternalAction() << "]";
        OnOrderReplaced();
        break;
    case ttsdk::ExecType::Trade:
        stream << "Algo:" << algoOrder_->GetOrderId()
            << " Order filled. RequestId=" << execRpt->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus());
        algoOrder_->GenerateSyntheticFill(execRpt->GetLastFillPrice(), execRpt->GetLastFillQty());
        order_->Unsubscribe();
        break;
    case ttsdk::ExecType::Rejected:
        stream << "Algo:" << algoOrder_->GetOrderId()
            << " Order rejected. RequestId=" << execRpt->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus());
        // rejection on new
        algoOrder_->FailAlgo(stream.str().c_str());
        break;
    default:
        stream << "Algo:" << algoOrder_->GetOrderId()
            << " Unhandled event. RequestId=" << execRpt->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) <<
            " ExecutionType=" << ttsdk::ToString(execRpt->GetExecType());
    }
    ttsdk::TTLogInfo(stream.str().c_str());
    ttsdk::PositionReserveBucket bucket = algoOrder_->GetRiskBucket(instrument_, profile_.account_id);
    ttsdk::TTLogInfo("Algo PR Bucket: %llu %llu: BUY P:%f W:%f F:%f SELL P:%f W:%f F:%f ", instrument_->GetInstrumentId(), profile_.account_id,
        bucket.buySide.positionLimit, bucket.buySide.workingPosition, bucket.buySide.filledQty,
        bucket.sellSide.positionLimit, bucket.sellSide.workingPosition, bucket.sellSide.filledQty);


}
void TrailingCrossStrategy::OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp)
{
    std::stringstream stream;
    stream << "OnReject event. RequestId=" << rejResp->GetRequestId() <<
        " Algo: " << algoOrder_->GetOrderId() <<
        " TTOrderId=" << order->GetOrderId() << " RejectReason=" << ttsdk::ToString(rejResp->GetRejectReason()) <<
        " Text=" << rejResp->GetText();
    ttsdk::TTLogInfo(stream.str().c_str());
    bool isExternalAction = order_->GetCurrentState()->IsExternalAction();
    if (replacing_ && !isExternalAction)
    {
        algoOrder_->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::Other, stream.str().c_str());
    }
    else
    {
        algoOrder_->FailAlgo(stream.str().c_str());
        InternalStop();
    }
}
void TrailingCrossStrategy::OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const ttsdk::IOrderEventHandler::SendCode code)
{
    std::stringstream stream;
    stream << "Order send failed. RequestId=" << profile.request_id <<
        " Algo: " << algoOrder_->GetOrderId() <<
        " TTOrderId=" << order->GetOrderId() << " SendCode=" << (int)code;
    ttsdk::TTLogInfo(stream.str().c_str());
    algoOrder_->FailAlgo(stream.str().c_str());
    InternalStop();
}

