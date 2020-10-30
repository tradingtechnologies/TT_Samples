#pragma once
#include <sys/types.h>
#include <tt_cplus_sdk.h>

#ifdef  __linux
#include <sys/syscall.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <string.h>
#include <cassert>
#include <vector>
#include <map>
#include <set>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <queue>
#include <iostream>
#include <sstream>
#include <memory>
#include <condition_variable>
#ifdef _WINDOWS
#include "libwebsockets.h"
#else
#include "libwebsockets7.h"
#endif

#include <thread>
#include <functional>

#include "PlaceOrderStrategy.h"

namespace ttsample {

    class EventQueue {
        typedef std::function<void(void)> EventTask;
    public:
        EventQueue();
        ~EventQueue();

        void Stop();
        void Dispatch(const EventTask& op);
        void Dispatch(EventTask&& op);

    private:
        std::mutex lock_;
        std::thread thread_;
        std::queue<EventTask> q_;
        std::condition_variable cv_;
        std::atomic<bool> quit_;

        void ThreadFunc(void);

        // deleted operations
        EventQueue(const EventQueue& rhs) = delete;
        EventQueue& operator=(const EventQueue& rhs) = delete;
        EventQueue(EventQueue&& rhs) = delete;
        EventQueue& operator=(EventQueue&& rhs) = delete;
    };

    /* Class which manages the running strategies and communicates with the remote controller to 
       start, update and stop the various algorithms available to runn on the SDK application.
       this application only supports one strategy - PlaceOrderStrategy. Any message protocol could 
       be used, this happens to use json, example commands are below. 
        {"start": {"id": "algo1", "instrument": {"mrkId":7, "product": "GE", "typeId": 34, "alias": "GE Sep20"}, "qty": 1, "acctId":98829}}
        {"change": {"id": "algo1", "qty": 1})
        {"stop": {"id": "algo1"}}
       responses will be in the following format:
        {"status": {"id": "algo1", "message":"status message saying something about the strategy"}}
    */
    class StrategyManager
    {
    public:
        enum class Command
        {
            Undefined = -1,
            Start = 0,
            Change = 1,
            Stop = 2
        };
        StrategyManager();
        virtual ~StrategyManager();
        bool Start(const std::string& ipAddress, const int port);
        void Stop();

        static int ServiceHttp(struct lws* wsi, enum lws_callback_reasons reason,
            void* user, void* in, size_t len);
        static int ServiceJson(struct lws* wsi, enum lws_callback_reasons reason,
            void* user, void* in, size_t len);

        void OnStopped(PlaceOrderStrategy* strat);  
        void OnStrategyStatus(PlaceOrderStrategy* strategy, const char* status);

    protected:
        void SendResponse(std::string msg);
        bool ProcessRequest(const std::string& msg);
        int Write(const unsigned char* buf, size_t len, lws_write_protocol writeProtocol);
        void SendErrorMessage(const char* id, const char* status);

        void OnConnectionEstablished(struct lws* wsi);
        void OnConnectionClosed();
        int OnDataReceived(struct lws* wsi, std::string&&);
        int OnDataWritable(struct lws* wsi);

    protected:
        struct lws_context_creation_info contextInfo_;
        struct lws_context* context_;
        std::vector<lws_protocols> protocols_;

        std::thread  socketThread_;
        std::atomic<bool> running_;

        std::unique_ptr<unsigned char[]> largeWriteBuf_;
        struct lws* wsi_;

        mutable std::mutex mutexMsgs_;
        std::queue<std::string> queueMgs_;

        EventQueue eventQueue_;
        mutable std::mutex mutexStrategies_;
        std::map< std::string, std::shared_ptr<PlaceOrderStrategy> > strategies_;


    };

 
    inline StrategyManager::StrategyManager() : context_(nullptr), wsi_(nullptr)
    {
        running_ = false;
        protocols_.push_back({ "http-only", StrategyManager::ServiceHttp, 0, 0, 0, nullptr });
        protocols_.push_back({ "json1", StrategyManager::ServiceJson, 0, 0, 0, nullptr });
        protocols_.push_back({ nullptr, nullptr, 0, 0, 0, nullptr });
    }

    inline StrategyManager::~StrategyManager()
    {
        Stop();
    }

    inline int StrategyManager::ServiceHttp(struct lws* wsi,
        enum lws_callback_reasons reason,
        void* user,
        void* in,
        size_t len)
    {
        return 0;
    }

    inline int StrategyManager::ServiceJson(struct lws* wsi,
        enum lws_callback_reasons reason,
        void* user,
        void* in,
        size_t len)
    {
        switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
        {
            auto mgr = (StrategyManager*)lws_context_user(lws_get_context(wsi));
            assert(mgr);
            mgr->OnConnectionEstablished(wsi);
            break;
        }
        case LWS_CALLBACK_SERVER_WRITEABLE:
        {
            auto mgr = (StrategyManager*)lws_context_user(lws_get_context(wsi));
            assert(mgr);
            return mgr->OnDataWritable(wsi);
            break;
        }
        case LWS_CALLBACK_RECEIVE:
        {
            auto mgr = (StrategyManager*)lws_context_user(lws_get_context(wsi));
            if (!mgr) // does anyone understand the protocol requested by the client?
            {
                return -1;
            }

            std::string data((char*)in, len);
            // entire message received in one callback
            mgr->OnDataReceived(wsi, std::move(data));
            break;
        }
        default:
            break;
        }
        return 0;
    }

    inline void StrategyManager::OnConnectionEstablished(struct lws* wsi)
    {
        std::cout << "Connection established " << std::endl;
        struct lws* oldWsi = wsi_;
        wsi_ = wsi;
        if (oldWsi)
        {
            // close the old connection
            std::cout << "Closing previous client connection " << std::endl;
            lws_callback_on_writable(oldWsi);
        }
    }
    inline void StrategyManager::OnConnectionClosed()
    {
        std::cout << "Connection closed " << std::endl;
        wsi_ = nullptr;
    }



    inline void StrategyManager::SendResponse(std::string msg)
    {
        std::cout << "Sending message: " << msg << std::endl;
        std::unique_lock<std::mutex> lock(mutexMsgs_);
        queueMgs_.push(msg);
        lws_callback_on_writable(wsi_);
    }


}