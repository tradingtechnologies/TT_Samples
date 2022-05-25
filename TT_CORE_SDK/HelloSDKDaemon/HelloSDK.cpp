#include <iostream>
#include <iomanip>
#include <atomic>
#include <sstream>
#include <string.h>
#include <tt_cplus_sdk.h>
#include <condition_variable>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

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
        ttsdk::TTLogInfo("SDKEventHandler::OnStatus (%d)", (uint32_t)status);
    };
    virtual void OnAccountStatus(const ttsdk::AccountConnectionStatus& status) override
    {
        ttsdk::TTLogInfo("Account connections status changed for account_id=%llu", status.account_id);
        std::ostringstream ss;
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
        ttsdk::TTLogInfo(ss.str().c_str());
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
        ttsdk::TTLogInfo("---Received ExecutionReport for TTOrderId=%s ExecType=%s OrderStatus=%s", order->GetOrderId() , 
                    ttsdk::ToString(execRpt->GetExecType()), ttsdk::ToString(execRpt->GetOrderStatus()));
    };
    virtual void OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp) override
    {
        ttsdk::TTLogInfo("---Received reject response for TTOrderId=%s", rejResp->GetOrderId());
    }
    virtual void OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const SendCode code) override
    {
        ttsdk::TTLogInfo("---Order TTOrderId=%s failed to send (SendCode:%d)", order->GetOrderId(), code);
    }
    virtual void OnUnsubscribed(const char* orderId) override
    {
        ttsdk::TTLogInfo("---OnUnsubscribe completed. Safe to delete this object now.");
    }

    // account and book sync events are sent on the position thread so they are sequence
    // properly with the position update events
    virtual void OnOrderBookDownloadEnd() override
    {
        ttsdk::TTLogInfo("---All assigned accounts are ready to use.");
        std::lock_guard<std::mutex> lock(mutex);
        sdkReadyCondition.notify_one();
    };    
    // the SDK synchronizes the order book and positions by account so any given acccount 
    // can be able to trade so the user does not need to wait for all accounts to be 
    // ready before trading
    virtual void OnAccountDownloadEnd(const uint64_t accountId) override
    {
        ttsdk::TTLogInfo("---Account accountId:%llu is synchronized and ready to use.", accountId);
    };
    virtual void OnAccountDownloadFailed(const uint64_t accountId, const char* msg) override
    {
        ttsdk::TTLogInfo("---Account accountId:%llu download failed detaila: %s", accountId,  msg);
    };
    // all position events are received on the SDK position processing thread. holding up processing
    // for a given position event will impact all subsequent position updates
    virtual void OnPositionUpdate(const ttsdk::Position& updatedPosition) override
    {
        ttsdk::TTLogInfo("---OnPositionUpdate for accountId:%llu Instrument: %s", updatedPosition.account_id, updatedPosition.instrument->GetAlias());
        ttsdk::TTLogInfo(" SOD:%f@%f Buys:%f@%f Sells:%f@%f Net:%f P/L:%f AvgPrc:%f", updatedPosition.sod_quantity, updatedPosition.sod_price 
            ,updatedPosition.buy_quantity, updatedPosition.buy_average_price 
            ,updatedPosition.sell_quantity, updatedPosition.sell_average_price
            ,updatedPosition.net_position  ,updatedPosition.native_currency_pnl ,updatedPosition.open_average_price);
    }

};


bool Daemonize()
{
    pid_t pid, sid;

    // Fork off the parent process
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // If we got a good PID, then return true to indicate this is the parent process.
    if (pid > 0)
    {
        return true;
    }

    // Change the file mode mask
    umask(0);

    // Create a new SID for the child process
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Change the current working directory:
    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    int fd = open("/dev/null",O_RDWR, 0);

    if (fd != -1)
    {
        dup2 (fd, STDIN_FILENO);
        dup2 (fd, STDOUT_FILENO);
        dup2 (fd, STDERR_FILENO);
    }
    else
    {
        close (fd);
        throw std::runtime_error(
            std::string("Failed to redirect standard output to /dev/null: ") +
            strerror(errno));
    }

    close (fd);

    return false;
}


void HandleStandardShutdownSignal(int signal)
{
    std::lock_guard<std::mutex> lock(mutex);
    sdkReadyCondition.notify_one();
}

int main(int argc, char* argv[])
{
    // Handle signals that are meant to shutdown the process.
     if (signal(SIGINT, HandleStandardShutdownSignal) == SIG_ERR  ||
         signal(SIGTERM, HandleStandardShutdownSignal) == SIG_ERR)
    {
        return EXIT_FAILURE;
    }

    bool daemonize = true;

    // this example is loading the app key secret from the command line or environment rather than hard coding.
    ttsdk::Environment env = ttsdk::Environment::UatCert;
    std::string sEnv("UatCert");
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
                sEnv = argv[i];
                if (sEnv == "ProdLive") env = ttsdk::Environment::ProdLive;
                else if (sEnv == "ProdSim") env = ttsdk::Environment::ProdSim;
                else if (sEnv == "ProdDelayed") env = ttsdk::Environment::ProdDelayed;
                else if (sEnv == "UatCert") env = ttsdk::Environment::UatCert;
                else if (sEnv == "DevCert")env = ttsdk::Environment::DevCert;
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
        else if ((arg == "-c") || (arg == "--console")) 
        {
            daemonize = false; 
        }
   }
    if (app_key.empty())
    {
        auto env_app_key = std::getenv("TT_APP_KEY");
        if (!env_app_key || strcmp(env_app_key, "") == 0)
        {
            std::cerr << "app_key is invalid. " << std::endl;
            return 1;
        }
        app_key = env_app_key;
    }
    if (daemonize && Daemonize())
    {
        return EXIT_SUCCESS;
    }

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
        std::cerr << "Unable to initialize SDK!" << std::endl;
        return -1;
    }  

    {
        std::unique_lock<std::mutex> lock(mutex);
        if (sdkReadyCondition.wait_for(lock, std::chrono::seconds(300)) == std::cv_status::timeout)
        {
            ttsdk::TTLogInfo("Timeout waiting for SDK to sync the order book!");
            return -1;
        }
    }


    ttsdk::TTLogInfo("<<<<< Hello world. TT CORE SDK is initialized, orderbook downloaded and all accounts ready for trading. >>>>>");
    ttsdk::TTLogInfo("<<<<< Orderbook and position events will be output to the log while the application is running. >>>>>");

    {
        std::unique_lock<std::mutex> lock(mutex);
        sdkReadyCondition.wait(lock);
    }
    ttsdk::TTLogInfo("<<<<< Initiating shutdown process. >>>>>");
    ttsdk::Shutdown();
}