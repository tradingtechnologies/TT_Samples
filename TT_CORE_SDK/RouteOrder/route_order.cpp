#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <string.h>
#include <tt_cplus_sdk.h>
#include "enums/OrderSide.h"
#include "enums/TimeInForce.h"
#include "enums/OrderType.h"
#include "enums/ExecType.h"
#include "enums/ProductType.h"
#include <iomanip>
#include <sstream>
#include <condition_variable>

std::mutex mutex;
std::condition_variable sdkReadyCondition;


class PlaceOrderStrategy : public ttsdk::IOrderEventHandler,
                           public ttsdk::IPriceEventHandler
{
public:
    PlaceOrderStrategy() 
        : market_(ttsdk::MarketId::CME), product_("GE"), 
          productType_(ttsdk::ProductType::Future), alias_("GE Sep28"),
          running_(false), shuttingDown_(false), requoteCnt_(0)
    {
    };
    virtual ~PlaceOrderStrategy()
    {
        Stop();
    };

    static bool double_eq(double p1, double p2)
    {
        return (std::isnan(p1) && std::isnan(p2)) ||
            (std::isinf(p1) && std::isinf(p2)) ||
            std::abs(p1 - p2) <= 0.000000001;
    }

    void Stop()
    {
        if (shuttingDown_) return;

        shuttingDown_ = true;
        if (running_)
        {
            // cancelorder if working
            auto execRpt = order_->GetCurrentState();
            if (execRpt->IsWorking())
                SendCancel();
            order_->Unsubscribe();
        }
        else
        {
            ttsdk::UnsubscribePrices(subId_);
        }
        
    };

    void Start()
    {
        ttsdk::TTLogInfo("Starting strategy");
        ttsdk::Instrument::ResponseCode respCode;
        instrument_ = ttsdk::GetInstrument(market_, product_.c_str(), productType_, alias_.c_str(), respCode);
        if (!instrument_)
        {
            ttsdk::TTLogError("Unable to get instrument market=%s type=%s alias=%s. Stopping strategy.",
                        ttsdk::ToString(market_), ttsdk::ToString(productType_), alias_.c_str());
            std::cout << "Unable to get instrument market=" << ttsdk::ToString(market_) << 
                        "type=" << ttsdk::ToString(productType_) << " alias=" << alias_ << ". Stopping strategy." << std::endl;
            return;
        }
        std::cout << "Subscribing for prices for instrument market=" << ttsdk::ToString(market_) << 
                        "type=" << ttsdk::ToString(productType_) << " alias=" << alias_ << std::endl;
        subId_ = ttsdk::SubscribePrices(instrument_, 10, true, true, false, this);
    }

    virtual void OnUnsubscribed(const uint64_t subId)
    {
        running_ = false;
        std::cout << "OnUnsubscribed from prices. subId=" << subId << std::endl;
    };
    virtual void OnDisconnect(const uint64_t subId, const ttsdk::InstrumentPtr& , const ttsdk::PriceEventType )
    {
       std::cout << "OnDisconnected from prices. subId=" << subId << std::endl;
    };
    virtual void OnPriceUpdate(const uint64_t , const ttsdk::InstrumentPtr& , const ttsdk::PriceSnap& snap)
    {
        if (!order_)
            PlaceOrder(snap);
        else
            FollowMarket(snap);
    };
    virtual void OnError(const uint64_t subId, const ttsdk::InstrumentPtr& instrument,
                         const ttsdk::SubscriptionError code, const char* error)
    {
        std::cout << "Error occurred subscribing for prices for instr=" << instrument->GetName()
                  << " code=" << (int)code << " error=" << error << " subId=" << subId<< std::endl;
    };
    inline void PlaceOrder(const ttsdk::PriceSnap& snap)
    {
        if (snap.bids.count == 0)
        {
            // don't place order until there is a bid market
            return;
        }

        order_ = ttsdk::CreateOrderFromPriceSub(subId_);
        if (!order_)
        {
            ttsdk::TTLogError("Order creation failed. Stopping strategy for instrument market=%s type=%s alias=%s ", 
                              ttsdk::ToString(market_), ttsdk::ToString(productType_), alias_.c_str());
            std::cout << "Order creation failed. Stopping strategy." << std::endl;
            return;
        }
        std::cout << "Placing order." << std::endl;

        running_ = true;
        order_->Subscribe(*this);

        profile_.request_id++;
        profile_.account_id = 98829;
        profile_.side = ttsdk::OrderSide::Buy;
        profile_.price = instrument_->TickPriceDown(snap.bids.levels[0].price, 2);
        profile_.quantity = 5;
        profile_.type = ttsdk::OrderType::Limit;
        profile_.tif = ttsdk::TimeInForce::Day;
        order_->SendNew(profile_);
    }
    inline void FollowMarket(const ttsdk::PriceSnap& snap)
    {
        if (snap.bids.count == 0)
        {
            std::cout << "Waiting for a bid price to follow on " << ttsdk::ToString(market_) << " " << alias_ << std::endl;
            return;
        }

        if ((snap.bids.count == 1) && (snap.bids.levels[0].quantity == profile_.quantity))
        {
            SendCancel();
            std::cout << "No other bid market, cancelling order." << std::endl;
            return;
        }        
        double newPrice = instrument_->TickPriceDown(snap.bids.levels[0].price, 2);
        if (!double_eq(newPrice, profile_.price))
        {
            if (requoteCnt_ >= 3)
            {
                // cross the market in an attempt to be filled
                if (snap.asks.count > 0)
                {
                    profile_.price = snap.asks.levels[0].price;
                    profile_.request_id++;
                    order_->SendChange(profile_);
                    std::cout << "Sent order change to cross the market." << std::endl;
                    requoteCnt_ = 0;
                }
            }
            else
            {
               // calculated price changed so update order
                profile_.price = newPrice;
                profile_.request_id++;
                order_->SendChange(profile_);
                std::cout << "Send order change to follow bid." << std::endl;
                 ++requoteCnt_;
            }
            return;
        }      
    }

    void SendCancel()
    {
        profile_.request_id++;
        order_->SendCancel(profile_);
    }

    virtual void OnUnsubscribed(const char*) override
    {
        ttsdk::TTLogInfo("Order event handler unsubscribed");
        ttsdk::UnsubscribePrices(subId_);
        order_.reset();
        instrument_.reset();
     }

    virtual void OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt) override
    {
        switch(execRpt->GetExecType())
        {
            case ttsdk::ExecType::New :
                std::cout << "Order successfully entered in market. RequestId=" << execRpt->GetRequestId() << 
                                " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << std::endl;
                break;
            case ttsdk::ExecType::Canceled :
                std::cout << "Order successfully cancelled. RequestId=" << execRpt->GetRequestId() <<
                    " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << " [external=" << execRpt->IsExternalAction() <<
                    "]" << std::endl;
                order_.reset();
                break;
            case ttsdk::ExecType::Replaced :
                std::cout << "Order replaced. RequestId=" << execRpt->GetRequestId() <<
                    " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) <<
                    " [external=" << execRpt->IsExternalAction() << "]" << std::endl;
                break;
            case ttsdk::ExecType::Trade :
                std::cout << "Order filled. RequestId=" << execRpt->GetRequestId() <<
                    " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << std::endl;
                // clear the order variable so a new order will be placed
                order_.reset();
                break;
            case ttsdk::ExecType::Rejected :
                std::cout << "Order filled. RequestId=" << execRpt->GetRequestId() <<
                    " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << std::endl;
                break;
            default:
                std::cout << "Unhandled event. RequestId=" << execRpt->GetRequestId() <<
                    " TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << 
                    " ExecutionType=" << ttsdk::ToString(execRpt->GetExecType()) << std::endl;
         }
    }
    virtual void OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp) override
    {
        std::cout << "OnReject event. RequestId=" << rejResp->GetRequestId() <<
            " TTOrderId=" << order->GetOrderId() << " RejectReason=" << ttsdk::ToString(rejResp->GetRejectReason()) << 
            " Text=" << rejResp->GetText() << std::endl;
        order_.reset();
    }
    virtual void OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const ttsdk::IOrderEventHandler::SendCode code) override
    {
        std::cout << "Order send failed. RequestId=" << profile.request_id<<
            " TTOrderId=" << order->GetOrderId() << " SendCode=" << (int)code << std::endl;
    }

    const ttsdk::MarketId market_;
    const std::string product_;
    const ttsdk::ProductType productType_;
    const std::string alias_;

    ttsdk::OrderProfile profile_;
    ttsdk::InstrumentPtr instrument_;
    ttsdk::OrderPtr order_;
    std::atomic<bool> running_;
    std::atomic<bool> shuttingDown_;
    uint16_t requoteCnt_;
    uint64_t subId_;
};


class SDKEventHandler : public ttsdk::IEventHandler
{
public:
    SDKEventHandler() : IEventHandler() {};
    virtual ~SDKEventHandler() {};
    virtual void OnStatus(const ttsdk::IEventHandler::Status status)
    {
        if (status == ttsdk::IEventHandler::Status::INITIALIZED)
        {
            std::cout << "---SDK is initialize. Prices are ready to use." << std::endl;
            std::lock_guard<std::mutex> lock(mutex);
            sdkReadyCondition.notify_one();
        }
        else
        {
            std::cout << "SDKEventHandler::OnStatus (" << (uint32_t)status << ")" << std::endl;
        }
    };
};


int main()
{
    // this example is loading the app key secret from the environment rather than hard coding.
    auto app_key = std::getenv("TT_APP_KEY");
    if (!app_key || strcmp(app_key, "") == 0)
    {
        std::cout << "app_key is invalid. " << std::endl;
        return 1;
    }
    std::cout << "using app_key: " << app_key << std::endl;


    // these are the options for the SDK
    ttsdk::TTSDKOptions options;
    // set the environment the app needs to run (where your machine is located)
    options.environment = ttsdk::Environment::DevCert;
    // set your app secret Key here. It looks like: 00000000-0000-0000-0000-000000000000:00000000-0000-0000-0000-000000000000
    options.app_key_secret = app_key;
    // the number of threads to use for orders and prices
    options.num_order_threads = 2;
    options.num_price_threads = 2;


    SDKEventHandler sdkObserver;
    if (!ttsdk::Initialize(options, &sdkObserver, nullptr))
    {
        std::cout << "Unable to initialize SDK!" << std::endl;
        return -1;
    }
    // wait for the SDK to be initialized as ready to handle price requests
    std::unique_lock<std::mutex> lock(mutex);
    if (sdkReadyCondition.wait_for(lock, std::chrono::seconds(300)) == std::cv_status::timeout)
    {
        std::cout << "Timeout waiting for SDK to initialize!" << std::endl;
        return -1;
    }


    std::cout << "Stategy starting... " << std::endl;
    PlaceOrderStrategy myStrategy;
    myStrategy.Start();
    std::cout << "Enter q to quit. " << std::endl;
    std::string command;
    while (std::cin >> command)
    {
        if (command == "q")
        {
            std::cout << "Quitting...\n";
            break;
        }
    }
    bool running(true);
    while (running)
    {
        running = false;
        if (myStrategy.running_)
        {
            myStrategy.Stop();
            running = true;
        }
        if (running)
        {
            std::cout << "Waiting for running strategy "  << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }

    ttsdk::Shutdown();
}