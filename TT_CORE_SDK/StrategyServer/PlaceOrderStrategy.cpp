#include "PlaceOrderStrategy.h"
#include "StrategyManager.h"
#include <sstream>

using namespace ttsample;


PlaceOrderStrategy::PlaceOrderStrategy(StrategyManager& mgr, const std::string id,
    const ttsdk::MarketId market, const std::string product,
    const ttsdk::ProductType productType, const std::string alias, const uint64_t acctId, const double qty)
    : mgr_(mgr), id_(id), market_(market), product_(product), productType_(productType), alias_(alias),
    running_(false), shuttingDown_(false), subId_(0)
{
    profile_.account_id = acctId;
    profile_.quantity = qty;
    profile_.side = ttsdk::OrderSide::Buy;
    profile_.type = ttsdk::OrderType::Limit;
    profile_.tif = ttsdk::TimeInForce::Day;
};
PlaceOrderStrategy::~PlaceOrderStrategy()
{
    Stop();
};

void  PlaceOrderStrategy::Stop()
{
    if (shuttingDown_) return;

    shuttingDown_ = true;
    if (running_)
    {
        // cancel order if working
        if (order_)
        {
            auto execRpt = order_->GetCurrentState();
            if (execRpt->IsWorking())
                SendCancel();
            else
                order_->Unsubscribe();
        }
    }
    else
    {
        ttsdk::UnsubscribePrices(subId_);
    }
};

void  PlaceOrderStrategy::OnOrderCancelled()
{
    order_->Unsubscribe();
};

void PlaceOrderStrategy::Start()
{
    ttsdk::Instrument::ResponseCode respCode;
    instrument_ = ttsdk::GetInstrument(market_, product_.c_str(), productType_, alias_.c_str(), respCode);
    if (!instrument_)
    {
        std::stringstream stream;
        stream << "Unable to get instrument market=" << ttsdk::ToString(market_) << 
                        "type=" << ttsdk::ToString(productType_) << " alias=" << alias_ << ". Stopping strategy." << std::endl;
        SendStatusMessage(stream.str().c_str());
        mgr_.OnStopped(this);
        return;
    }
    subId_ = ttsdk::SubscribePrices(instrument_, 10, true, true, false, this);
    SendStatusMessage("Started");

}

void PlaceOrderStrategy::SendStatusMessage(const char* status)
{
    mgr_.OnStrategyStatus(this, status);
}
 
void PlaceOrderStrategy::OnUnsubscribed(const uint64_t subId)
{
    running_ = false;
    mgr_.OnStopped(this);
}

void PlaceOrderStrategy::OnDisconnect(const uint64_t subId, const ttsdk::InstrumentPtr&, const ttsdk::PriceEventType)
{
    // potentially cancel the orders if we lose the price stream 
}

void PlaceOrderStrategy::OnPriceUpdate(const uint64_t, const ttsdk::InstrumentPtr&, const ttsdk::PriceSnap& snap)
{
    if (!order_)
        PlaceOrder(snap);
    else
    {
        if (snap.asks.count == 0)
        {
            SendCancel();
            SendStatusMessage("No ask market, cancelling order.");
            return;
        }
        double newPrice = instrument_->TickPriceDown(snap.asks.levels[0].price, 1);
        if (!double_eq(newPrice, profile_.price))
        {
            // calculated price changed so update order
            profile_.price = newPrice;
            profile_.request_id++;
            order_->SendChange(profile_);
            SendStatusMessage("Send order change to follow ask.");
        }

    }
};
void PlaceOrderStrategy::OnError(const uint64_t subId, const ttsdk::InstrumentPtr& instrument,
    const ttsdk::SubscriptionError code, const char* error)
{
    std::stringstream stream;
    stream << "Error occurred subscribing for prices for instr=" << instrument->GetName()
        << " code=" << (int)code << " error=" << error << " subId=" << subId << std::endl;
    SendStatusMessage(stream.str().c_str());

};

void PlaceOrderStrategy::PlaceOrder(const ttsdk::PriceSnap& snap)
{
    // don't place order until there is an ask market
    if (snap.asks.count == 0)
        return;

    PlaceOrder(instrument_->TickPriceDown(snap.asks.levels[0].price, 1));
}

void PlaceOrderStrategy::PlaceOrder(const double price)
{
    order_ = ttsdk::CreateOrderFromPriceSub(subId_);
    if (!order_)
    {
        ttsdk::TTLogError("Order creation failed. Stopping strategy for instrument market=%s type=%s alias=%s ",
            ttsdk::ToString(market_), ttsdk::ToString(productType_), alias_.c_str());
        SendStatusMessage("Order creation failed.Stopping strategy.");
        return;
    }
    std::stringstream stream;
    stream << "Placing order TTOrderId:" << order_->GetOrderId() << " Buy " << profile_.quantity << 
                "@" << price << std::endl;
    SendStatusMessage(stream.str().c_str());

    running_ = true;
    order_->Subscribe(*this);

    profile_.request_id++;
    profile_.price = price;
    order_->SendNew(profile_);
}


void PlaceOrderStrategy::SendCancel()
{
    if (order_)
    {
        profile_.request_id++;
        order_->SendCancel(profile_);
    }
    else
    {
        SendStatusMessage("Order not working.");
    }
    
}

void PlaceOrderStrategy::SendChange(const double qty)
{
    if (order_)
    {
        profile_.quantity = qty;
        profile_.request_id++;
        order_->SendChange(profile_);
    }
    else
    {
        SendStatusMessage("Order not working.");
    }
}

void PlaceOrderStrategy::OnUnsubscribed(const char*)
{
    ttsdk::UnsubscribePrices(subId_);
    order_.reset();
    instrument_.reset();
}

void PlaceOrderStrategy::OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt)
{
    std::stringstream stream;
    switch (execRpt->GetExecType())
    {
    case ttsdk::ExecType::New:
        stream << "Order successfully entered in market. RequestId=" << execRpt->GetRequestId() <<
            " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << std::endl;
        break;
    case ttsdk::ExecType::Canceled:
        stream << "Order successfully cancelled. RequestId=" << execRpt->GetRequestId() <<
            " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << " [external=" << execRpt->IsExternalAction() <<
            "]" << std::endl;
        OnOrderCancelled();
        break;
    case ttsdk::ExecType::Replaced:
        stream << "Order replaced. RequestId=" << execRpt->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) <<
            " [external=" << execRpt->IsExternalAction() << "]" << std::endl;
        break;
    case ttsdk::ExecType::Trade:
        stream << "Order filled. RequestId=" << execRpt->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << std::endl;
        // clear the order variable so a new order will be placed
        order_.reset();
        PlaceOrder(instrument_->TickPriceDown(execRpt->GetLastFillPrice(), 1));
        break;
    case ttsdk::ExecType::Rejected:
        stream << "Order rejected. RequestId=" << execRpt->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << std::endl;
        break;
    default:
        stream << "Unhandled event. RequestId=" << execRpt->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) <<
            " ExecutionType=" << ttsdk::ToString(execRpt->GetExecType()) << std::endl;
    }
    SendStatusMessage(stream.str().c_str());
}
void PlaceOrderStrategy::OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp)
{
    std::stringstream stream;
    stream << "OnReject event. RequestId=" << rejResp->GetRequestId() <<
        " TTOrderId=" << order->GetOrderId() << " RejectReason=" << ttsdk::ToString(rejResp->GetRejectReason()) <<
        " Text=" << rejResp->GetText() << std::endl;
    order_.reset();
    SendStatusMessage(stream.str().c_str());
}
void PlaceOrderStrategy::OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const ttsdk::IOrderEventHandler::SendCode code)
{
    std::stringstream stream;
    stream << "Order send failed. RequestId=" << profile.request_id <<
        " TTOrderId=" << order->GetOrderId() << " SendCode=" << (int)code << std::endl;
    SendStatusMessage(stream.str().c_str());
}

