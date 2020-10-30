#include <iostream>
#include <iomanip>
#include <atomic>
#include <sstream>
#include <string.h>
#include <tt_cplus_sdk.h>
#include <condition_variable>


std::mutex mutex;
std::condition_variable sdkReadyCondition;

class SDKEventHandler : public ttsdk::IEventHandler
{
public:
    SDKEventHandler() : IEventHandler() {};
    virtual ~SDKEventHandler() {};
    // received on an SDK thread managed thread that can be used for processing
    // however, time comsuming tasks can delay the delivery of another status event.
    virtual void OnStatus(const ttsdk::IEventHandler::Status status) override
    {
        std::cout << "SDKEventHandler::OnStatus (" << (uint32_t)status << ")" << std::endl;
    };
    virtual void OnAccountStatus(const ttsdk::AccountConnectionStatus& status) override
    {
        std::cout << "Account connections status changed: " << std::endl;
        std::ostringstream ss;
        ss << "account_id=" << status.account_id
            << " conn count=" << status.count
            << '\n';
        if (status.count > 0)
        {
            ss << "  market  |  connId  |  status\n";
        }
        for (size_t i = status.count; i > 0; --i)
        {
            const auto& info = status.connections[i-1];
            ss << std::setw(10) << ttsdk::ToString(info.market)
                << "|"
                << std::setw(10) << info.id
                << "|"
                << std::setw(10) << ttsdk::ToString(info.status) << "\n";
        }
        std::cout << ss.str() << std::endl;
    };
};


class MyOrderBookHandler : public ttsdk::IOrderBookEventHandler
{
public:
    MyOrderBookHandler() : IOrderBookEventHandler() {};
    virtual ~MyOrderBookHandler() {};
    // call back is received on a TTSDK thread that can be used for processing of the event
    // each order will be sequenced on a given thread from the sdk's collection of order thread
    virtual void OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt) override
    {
        std::cout << "---Received ExecutionReport for TTOrderId=" << order->GetOrderId() 
                  << " ExecType=" << ttsdk::ToString(execRpt->GetExecType())
                  << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << std::endl;
    };
    virtual void OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp) override
    {
        std::cout << "---Received reject response for TTOrderId=" << rejResp->GetOrderId() << std::endl;
    }
    virtual void OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const SendCode code) override
    {
        std::cout << "---Order TTOrderId=" << order->GetOrderId() << "failed to send (SendCode:" << code << ")" << std::endl;
    }
    virtual void OnUnsubscribed(const char* orderId) override
    {
        std::cout << "---OnUnsubscribe completed. Safe to delete this object now." << std::endl;
    }

    // account and book sync events are sent on the position thread so they are sequence
    // properly with the position update events
    virtual void OnOrderBookDownloadEnd() override
    {
        std::cout << "---All assigned accounts are ready to use." << std::endl;
        std::lock_guard<std::mutex> lock(mutex);
        sdkReadyCondition.notify_one();
    };    
    // the SDK synchronizes the order book and positions by account so any given acccount 
    // can be able to trade so the user does not need to wait for all accounts to be 
    // ready before trading
    virtual void OnAccountDownloadEnd(const uint64_t accountId) override
    {
        std::cout << "---Account accountId:" << accountId << " is synchronized and ready to use." << std::endl;
    };
    virtual void OnAccountDownloadFailed(const uint64_t accountId, const char* msg) override
    {
        std::cout << "---Account accountId:" << accountId <<" download failed details: " << msg << std::endl;
    };
    // all position events are received on the SDK position processing thread. holding up processing
    // for a given position event will impact all subsequent position updates
    virtual void OnPositionUpdate(const ttsdk::Position& updatedPosition) override
    {
        std::cout << "---OnPositionUpdate for AccountId:" << updatedPosition.account_id << " Instrument:" << updatedPosition.instrument->GetAlias() << std::endl;
        std::cout 
            << " SOD:" << updatedPosition.sod_quantity << "@" << updatedPosition.sod_price 
            << " Buys:" <<  updatedPosition.buy_quantity << "@" << updatedPosition.buy_average_price 
            << " Sells:" << updatedPosition.sell_quantity << "@" << updatedPosition.sell_average_price
            << " Net:" << updatedPosition.net_position 
            << " P/L:" << updatedPosition.native_currency_pnl 
            << " AvgPrc:" << updatedPosition.open_average_price << std::endl;
    }

};



int main(int argc, char* argv[])
{
    // this example is loading the app key secret from the command line or environment rather than hard coding.
    ttsdk::Environment env = ttsdk::Environment::DevCert;
    std::string app_key;
   for (int i = 1; i < argc; ++i) 
   {
        std::string arg = argv[i];
        if ((arg == "-k") || (arg == "--key")) 
        {
            if (i + 1 < argc) 
            { 
                i++;
                app_key = argv[i]; 
            } 
            else 
            { 
                std::cerr << "--key option requires one argument." << std::endl;
                return 1;
            }  
        } 
        else if ((arg == "-e") || (arg == "--env")) 
        {
            if (i + 1 < argc) 
            { 
                i++;
                auto sEnv = argv[i];
                if (::strcmp(sEnv, "ProdLive") == 0) env = ttsdk::Environment::ProdLive;
                else if (::strcmp(sEnv, "ProdSim") == 0) env = ttsdk::Environment::ProdSim;
                else if (::strcmp(sEnv, "ProdDelayed") == 0) env = ttsdk::Environment::ProdDelayed;
                else if (::strcmp(sEnv, "UatCert") == 0) env = ttsdk::Environment::UatCert;
                else if (::strcmp(sEnv, "DevCert") == 0) env = ttsdk::Environment::DevCert;
                else {
                    std::cerr << "Invalid env argument: " << sEnv << std::endl;
                    return 1;                    
                }
            } 
            else 
            { 
                std::cerr << "--env option requires one argument." << std::endl;
                return 1;
            } 
        }   
    }
    if (app_key.empty())
    {
        auto env_app_key = std::getenv("TT_APP_KEY");
        if (!env_app_key || strcmp(env_app_key, "") == 0)
        {
            std::cout << "app_key is invalid. " << std::endl;
            return 1;
        }
        app_key = env_app_key;
    }
    std::cout << "using app_key: " << app_key << std::endl;   

    SDKEventHandler myObserver;
    MyOrderBookHandler myOrderObserver;
    // these are the options for the SDK
    ttsdk::TTSDKOptions options;
    // set the environment the app needs to run (where your machine is located)
    options.environment = env;
    // set your app secret Key here. It looks like: 00000000-0000-0000-0000-000000000000:00000000-0000-0000-0000-000000000000
    options.app_key_secret = app_key.c_str();
    // the number of threads to use for orders and prices
    options.num_order_threads = 3; 
    options.num_price_threads = 3; 

    if (!ttsdk::Initialize(options, &myObserver, &myOrderObserver))
    {
        std::cout << "Unable to initialize SDK!" << std::endl;
        return -1;
    }  

    std::unique_lock<std::mutex> lock(mutex);
    if (sdkReadyCondition.wait_for(lock, std::chrono::seconds(300)) == std::cv_status::timeout)
    {
        std::cout << "Timeout waiting for SDK to sync the order book!" << std::endl;
        return -1;
    }

    std::cout << std::endl;
    std::cout << "<<<<< Hello world. TT CORE SDK is initialized, orderbook downloaded and all accounts ready for trading. >>>>>" << std::endl;
    std::cout << "<<<<< Orderbook and position events will be output to the console while the application is running. >>>>>" << std::endl;
    std::cout << std::endl << "Press q to exit....." << std::endl;
    std::string command;
    while (std::cin >> command)
    {
        if (command == "q")
        {
            std::cout << "Quitting...\n";
            break;
        }
    }
    std::cout << "Exiting..." << std::endl;
    ttsdk::Shutdown();
}