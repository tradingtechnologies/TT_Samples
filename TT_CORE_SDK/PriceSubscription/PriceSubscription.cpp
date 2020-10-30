#include <iostream>
#include <iomanip>
#include <atomic>
#include <chrono>
#include <string.h>
#include <tt_cplus_sdk.h>
#include <condition_variable>

std::mutex mutex;
std::condition_variable sdkReadyCondition;


const char* to_string(const ttsdk::PriceEventType& type)
{
    if (type == ttsdk::PriceEventType::DIRECT)
        return "direct event";
    else if (type == ttsdk::PriceEventType::CALCULATED_IMPLIEDS)
        return "calulcated implieds event";
    else if (type == ttsdk::PriceEventType::SIMULATED)
        return "simulated event";
    else
        return "unknown";
}

std::string to_string(const ttsdk::PriceSnap& snap)
{
    std::ostringstream ss;
    if (snap.event_type == ttsdk::PriceEventType::DIRECT)
        ss << "PriceSnap:     [direct event]\n";
    else if (snap.event_type == ttsdk::PriceEventType::CALCULATED_IMPLIEDS)
        ss << "PriceSnap:     [calulcated implieds event]\n";
    else if (snap.event_type == ttsdk::PriceEventType::SIMULATED)
        ss << "PriceSnap:     [simulated event]\n";
    else
        ss << "PriceSnap:     [unknown event]\n";
    ss << "market_state=" << (int)snap.market_state
        << " asks.count=" << snap.asks.count
        << " bids.count=" << snap.bids.count
        << '\n'
        << "open=" << snap.open_price
        << " close=" << snap.close_price
        << " high=" << snap.high_price
        << " low=" << snap.low_price
        << " settle=" << snap.settlement_price
        << '\n'
        << "ltp=" << snap.last_trade_price
        << " ltq=" << snap.last_trade_quantity
        << " volume=" << snap.volume
        << '\n';

    if (snap.asks.count > 0 || snap.bids.count > 0)
    {
        ss << "  #order   imp Q       Q  |     price     |       Q   imp Q  #order\n";
    }
    for (int i = snap.asks.count - 1; i >= 0; --i)
    {
        const auto& level = snap.asks.levels[i];
        ss << "                          |"
            << std::setw(15) << std::setprecision(6) << level.price
            << "|"
            << std::setw(8) << level.quantity
            << std::setw(8) << level.implied_quantity
            << std::setw(8) << level.order_count << '\n';
    }
    for (int i = 0; i < snap.bids.count; ++i)
    {
        const auto& level = snap.bids.levels[i];
        ss << std::setw(8) << level.order_count
            << std::setw(8) << level.implied_quantity
            << std::setw(8) << level.quantity
            << "  |"
            << std::setw(15) << std::setprecision(6) << level.price << "|\n";
    }

    return ss.str();
}

std::string to_string(const ttsdk::TimeSales& ts)
{
    std::ostringstream ss;
    if (ts.event_type == ttsdk::PriceEventType::DIRECT)
        ss << "TimeSales:     [direct event]\n";
    else if (ts.event_type == ttsdk::PriceEventType::CALCULATED_IMPLIEDS)
        ss << "TimeSales:     [calulcated implieds event]\n";
    else if (ts.event_type == ttsdk::PriceEventType::SIMULATED)
        ss << "TimeSales:     [simulated event]\n";
    else
        ss << "TimeSales:     [unknown event]\n";

    for (size_t i=0; i< ts.count;++i) 
    {
        ss <<  "side=" << (int)ts.trades[i].trade_side << '\n'
        << "ltp=" << ts.trades[i].last_trade_price << '\n'
        << "ltq=" << ts.trades[i].last_trade_quantity << '\n'
        << "trade_indicator=" << (int)ts.trades[i].indicator << '\n'
        << "trade_qualifier=" << (int)ts.trades[i].qualifier << '\n'
        << "otc_type=" << (int)ts.trades[i].otc_type << '\n';
   }        

    
    return ss.str();
}


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

class PriceObserver : public ttsdk::IPriceEventHandler
{
public:
    PriceObserver() {};

    virtual void OnPriceUpdate(const uint64_t subId, const ttsdk::InstrumentPtr& instrument, const ttsdk::PriceSnap& snap)
	{
		std::cout << "OnPriceUpdate for instrument:" << instrument->GetAlias() << " subId=" << subId << std::endl;
        std::cout << to_string(snap) << std::endl;
	};
    virtual void OnError(const uint64_t subId, const ttsdk::InstrumentPtr& instrument, 
                         const ttsdk::SubscriptionError code, const char* error)
	{
		std::cout << "Error [" << error  << "] occurred for instrument:" << instrument->GetAlias() << " subId=" << subId 
                  << " code=" << (int)code << std::endl;
	};
    virtual void OnDisconnect(const uint64_t subId, const ttsdk::InstrumentPtr& instrument, const ttsdk::PriceEventType type)
    {
       std::cout << "Prices disconnected for subscription for instrument:" << instrument->GetAlias() << " subid=" << subId << std::endl;
    };
    virtual void OnUnsubscribed(const uint64_t subId) 
    {
 		std::cout << "Price observer for subId=" << subId << " has been successfully unsubscriped. " << std::endl;
    }        
};
class TSObserver : public ttsdk::ITimeSalesEventHandler
{
public:
    TSObserver() {};
    virtual void OnTimeSalesUpdate(const uint64_t subId, const ttsdk::InstrumentPtr& instrument, const ttsdk::TimeSales& ts)
	{
		std::cout << "OnTimeSalesUpdate for instrument:" << instrument->GetAlias() << " subId=" << subId << std::endl;
        std::cout << to_string(ts) << std::endl;
	};
    virtual void OnError(const uint64_t subId, const ttsdk::InstrumentPtr& instrument, 
                         const ttsdk::SubscriptionError code, const char* error)
	{
		std::cout << "Error [" << error  << "] occurred for instrument:" << instrument->GetAlias() << " subId=" << subId 
                  << " code=" << (int)code << std::endl;
	};
    virtual void OnDisconnect(const uint64_t subId, const ttsdk::InstrumentPtr& instrument, const ttsdk::PriceEventType type)
    {
        std::cout << "TimeAndSales disconnected for subscription for instrument:" << instrument->GetAlias() << " subid=" << subId << std::endl;
    };
    virtual void OnUnsubscribed(const uint64_t subId) 
    {
 		std::cout << "TimeAndSales observer for subId=" << subId << " has been successfully unsubscriped." << std::endl;
    }  
};

void displayHelp()
{
    // Use the following commands to control the application
    std::cout << "Commands: [q] Quit"
        << "\n          [sp]  Subscription to prices"
        << "\n          [st]  Subscription to trades"
        << "\n          [un]  Unsubscribe"
        << "\n          [h]   Display help"
        << std::endl;    
}

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


    // these are the options for the SDK
    ttsdk::TTSDKOptions options;
    // set the environment the app needs to run (where your machine is located)
    options.environment = env;
    // set your app secret Key here. It looks like: 00000000-0000-0000-0000-000000000000:00000000-0000-0000-0000-000000000000
    options.app_key_secret = app_key.c_str();
    // the number of threads to use for orders and prices
    options.num_order_threads = 1; 
    options.num_price_threads = 3; 

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

    PriceObserver prcObserver;
    TSObserver tsObserver;

    displayHelp();

    std::string command;
    while (std::cin >> command)
    {
        if (command == "h")
        {
            displayHelp();
        }  
        else if (command == "q")
        {
            std::cout << "Quitting...\n";
            break;
        }
        if ((command == "st") || (command == "sp"))
        {
            uint32_t mrkId;
            uint32_t typeId;
            std::string tmp;
            std::string prodName;
            std::string instrName;
            std::cout << "Enter Market Id:";
            std::cin >> mrkId;
            std::getline(std::cin, tmp);
            std::cout << "Enter Product Name:";
            std::getline(std::cin, prodName);
            std::cout << "Enter Product Type (34 for Future, 43 for MultilegInstrument, 1000 for ASSynthetic):";
            std::cin >> typeId;
            std::getline(std::cin, tmp);
            std::cout << "Enter Instrument Alias:";
            std::getline(std::cin, instrName);
            ttsdk::Instrument::ResponseCode respCode;
            ttsdk::MarketId ttMrkId = ttsdk::MarketId(mrkId);
            ttsdk::ProductType ttTypeId = ttsdk::ProductType(typeId);
            ttsdk::InstrumentPtr instrument = ttsdk::GetInstrument(ttMrkId, prodName.c_str(), ttTypeId, instrName.c_str(), respCode);
            if (!instrument)
            {
               std::cout << "Unable to find instrument " << instrName << " in market " << ttsdk::ToString(ttMrkId) << std::endl;
            }
            else
            {
                std::cout << "Found instrument " << ttsdk::ToString(ttMrkId)  << " " << instrument->GetAlias() 
                         << " id= " << instrument->GetInstrumentId() << " [" << instrument->GetName() << "]" << std::endl;
                uint64_t subId = 0;
                if (command[1] == 't')
                    subId = ttsdk::SubscribeTimeAndSales(instrument, false, &tsObserver);
                else
                    subId = ttsdk::SubscribePrices(instrument, 10, true, true, false, &prcObserver);

                std::cout << "Subscription id=" << subId << std::endl; 
            }	            
        }
        else if (command == "un")
        {
            std::string tmp;
            uint32_t subId;
            std::getline(std::cin, tmp);
            std::cout << "Enter subscription id:";
            std::cin >> subId;
            ttsdk::UnsubscribePrices(subId);
            std::cout << "Unsubscribed subscription id=" << subId << std::endl;    
        }        

    }

    std::cout<< "Exiting...\n";
	ttsdk::Shutdown();
}