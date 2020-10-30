#include "StrategyManager.h"
#include <sstream>
#include "nlohmann/json.hpp"

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/conf.h>

using namespace ttsample;

 
EventQueue::EventQueue()
{
    thread_ = std::thread(&EventQueue::ThreadFunc, this);
    quit_ = false;
}
EventQueue::~EventQueue()
{
    Stop();  
}
void EventQueue::Stop()
{    
    // signal it's time to wrap up
    quit_ = true;
    cv_.notify_all();
    // wait for thread to finish before we exit
    if (thread_.joinable())
    {
        thread_.join();
    }
}
void EventQueue::Dispatch(const EventTask& op)
{
    std::unique_lock<std::mutex> lock(lock_);
    q_.push(op);
    // manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cv_.notify_all();
}
void EventQueue::Dispatch(EventTask&& op)
{
    std::unique_lock<std::mutex> lock(lock_);
    q_.push(std::move(op));

    // manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lock.unlock();
    cv_.notify_all();
}
void EventQueue::ThreadFunc(void)
{
    std::unique_lock<std::mutex> lock(lock_);
    do {
        //Wait until we have data or a quit signal
        cv_.wait(lock, [this] {
            return (q_.size() || quit_);
            });

        //after wait, we own the lock
        if (!quit_ && q_.size())
        {
            auto task = std::move(q_.front());
            q_.pop();
            //unlock now that we're done messing with the queue
            lock.unlock();
            task();
            lock.lock();
        }
    } while (!quit_);
}


void StrategyManager::OnStrategyStatus(PlaceOrderStrategy* strategy, const char* status)
{ 
    assert(strategy);
    std::string msg("{\"status\": {\"id\":\"");
    msg.append(strategy->Id());
    msg.append("\", \"message\":\"");
    msg.append(status);
    msg.append("\"}}");
    SendResponse(msg);
}

inline void StrategyManager::SendErrorMessage(const char* id, const char* status)
{
    std::string msg("{\"status\":");
    if (id)
    {
        msg.append("\"id\":\"");
        msg.append(id);
        msg.append("\", ");
    }
    msg.append("\"message\":\"ERROR: ");
    msg.append(status);
    msg.append("\"}}");
    SendResponse(msg);
}
/*
{"start": {"id": "order1", "instrument": {"mrkId":7, "product": "GE", "typeId": 34, "alias": "GE Sep20"}, "qty": 1, "acctId":98829}}
{"change": {"id": "order1", "qty": 1})
{"stop": {"id": "order1"}}
*/
bool StrategyManager::ProcessRequest(const std::string& msg)
{
    try
    {
        nlohmann::json jt;
        jt = nlohmann::json::parse(msg);
        if (!jt.is_object())
        {
            SendErrorMessage(nullptr, "Invalid command message");
            return false;
        }
        std::string sCommand = jt.begin().key();
        Command command = Command::Undefined;
        if (sCommand == "start") command = Command::Start;
        else if (sCommand == "stop") command = Command::Stop;
        else if (sCommand == "change") command = Command::Change;
        auto commandNode = jt.begin().value();

        std::string id;
        auto node = commandNode.find("id");
        if (node != commandNode.end())
            id = node.value().get<std::string>();
        double qty = 1.0;
        node = commandNode.find("qty");
        if (node != commandNode.end())
            qty = node.value().get<double>();
        uint64_t acctId(0);
        int mrkId(-1);
        std::string product;
        int productType(-1);
        std::string alias;
        bool hasInstrument = true;
        if (command == Command::Start)
        {
            node = commandNode.find("acctId");
            if (node != commandNode.end())
                acctId = node.value().get<uint64_t>();

            node = commandNode.find("instrument");
            if (node != commandNode.end())
            {
                auto instAttr = node.value().find("mrkId");
                if (instAttr != node.value().end())
                    mrkId = instAttr.value().get<int>();
                instAttr = node.value().find("product");
                if (instAttr != node.value().end())
                    product = instAttr.value().get<std::string>();
                instAttr = node.value().find("typeId");
                if (instAttr != node.value().end())
                    productType = instAttr.value().get<int>();
                instAttr = node.value().find("alias");
                if (instAttr != node.value().end())
                    alias = instAttr.value().get<std::string>();
            }
            else
                hasInstrument = false;
        }


        if (id.empty() || (command == Command::Undefined) ||
            (command == Command::Start && (!hasInstrument || (mrkId == -1) || (productType == -1) || alias.empty() || product.empty())))
        {
            std::string error("Missing required field(s) in request:");
            if (id.empty()) error.append(" id ");
            if (command == Command::Undefined) error.append(" action ");
            if (command == Command::Start)
            {
                if (!hasInstrument) error.append(" instrument ");
                if (mrkId == -1) error.append(" instrument.mrkId ");
                if (product.empty()) error.append(" instrument.product ");
                if (productType == -1) error.append(" instrument.typeId ");
                if (alias.empty()) error.append(" instrument.alias ");
            }
            SendErrorMessage(nullptr, error.c_str());
            return false;
        }

        std::unique_lock<std::mutex> lock(mutexStrategies_);
        auto iStrat = strategies_.find(id);
        if (command == Command::Start)
        {
            if (iStrat != strategies_.end())
            {
                lock.unlock();          
                SendErrorMessage(id.c_str(), "Strategy already exists");
                return false;
            }
            auto strat = std::make_shared<PlaceOrderStrategy>(*this, id,
                (ttsdk::MarketId)mrkId, product, (ttsdk::ProductType)productType, alias, 
                acctId, qty);
            strategies_.insert(std::make_pair(id, strat));
            lock.unlock();
            strat->Start();
        }
        else
        {
            if (iStrat == strategies_.end())
            {
                lock.unlock();
                SendErrorMessage(id.c_str(), "Strategy does not exist");
                return false;
            }
            std::shared_ptr<PlaceOrderStrategy> strat = iStrat->second;
            lock.unlock();
            if (command == Command::Stop)
            {
                strat->Stop();
            }
            else if (command == Command::Change)
            {
                strat->SendChange(qty);
            }
        }

    }
    catch (nlohmann::json::parse_error& e)
    {
        std::string error("{\"Error\":\"Unable to parse json request. }");
        error.append(e.what());
        error.append("\"}");
        SendResponse(error);
        return false;
    }
    return true;
}

int StrategyManager::OnDataReceived(struct lws* wsi, std::string&& msg)
{
    if (wsi != wsi_)
        return 2;

    std::cout << "Received message: " << msg << std::endl;
    eventQueue_.Dispatch([=] { ProcessRequest(msg); });

    return 0;
}

int StrategyManager::OnDataWritable(struct lws* wsi)
{
    if (wsi != wsi_)
        return 2;

    std::string request;
    {
        std::unique_lock<std::mutex> lock(mutexMsgs_);
        if (!queueMgs_.empty())
        {
            request = queueMgs_.front();
            queueMgs_.pop();
        }
    }
    if (!request.empty())
    {
        Write((const unsigned char*)request.c_str(), request.length(), LWS_WRITE_TEXT);
        lws_callback_on_writable(wsi_);
    }
    return 0;
}

 
bool StrategyManager::Start(const std::string& ipAddress, const int port)
{
    // init open ssl since lws is compiled with it
    ::SSL_library_init();
    ::SSL_load_error_strings();
    ::OpenSSL_add_all_algorithms();

    memset(&contextInfo_, 0, sizeof(contextInfo_));
    contextInfo_.port = port;
    contextInfo_.iface = ipAddress.c_str();
    contextInfo_.protocols = protocols_.data();
    contextInfo_.user = (void*)this;
    contextInfo_.extensions = nullptr;
    contextInfo_.gid = -1;
    contextInfo_.uid = -1;
    contextInfo_.options = 0;
    contextInfo_.ssl_cert_filepath = nullptr;
    contextInfo_.ssl_private_key_filepath = nullptr;

    //* create libwebsocket context. */
    context_ = lws_create_context(&contextInfo_);
    if (context_ == NULL) {
        return false;
    }

     socketThread_ =
        std::thread([&]()
            {
                // infinite loop, to end this server send SIGTERM. (CTRL+C)
                while (running_) {
                    lws_service(context_, 500);
                    // libwebsocket_service will process all waiting events with
                    // their callback functions and then wait 500 ms.
                    // (this is a single threaded web server and this will keep our
                    // server from generating load while there are not
                    // requests to process)
                }

                lws_context_destroy(context_);
            });

    running_ = true;

    return true;
}

void StrategyManager::Stop()
{
    if (running_)
    {
        running_ = false;
        size_t running(0);
        socketThread_.join();
        {
            std::unique_lock<std::mutex> lock(this->mutexStrategies_);
            for (auto s : strategies_)
            {
                s.second->Stop();
            }
            running = strategies_.size();
        }
        while (running > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
            std::unique_lock<std::mutex> lock(this->mutexStrategies_);
            running = strategies_.size();
        }
    }

    // clean up open ssl 
    ::ERR_free_strings();
    ::ERR_remove_thread_state(nullptr);
    ::EVP_cleanup();
    ::CRYPTO_cleanup_all_ex_data();
    ::CONF_modules_unload(1);

    eventQueue_.Stop();
}

void  StrategyManager::OnStopped(PlaceOrderStrategy* strat)
{
    assert(strat);

    // avoid locking on calling thread
    eventQueue_.Dispatch([=] {
        std::unique_lock<std::mutex> lock(this->mutexStrategies_);
        this->strategies_.erase(strat->Id());
        });

    OnStrategyStatus(strat, "Stopped");
}

int StrategyManager::Write(const unsigned char* buf, size_t len, lws_write_protocol writeProtocol)
{
    size_t totalSize = len
        + LWS_SEND_BUFFER_PRE_PADDING
        + LWS_SEND_BUFFER_POST_PADDING;
    const size_t cMaxLocalBufSize = 2048;
    if (totalSize < cMaxLocalBufSize)
    {
        unsigned char paddedBuf[cMaxLocalBufSize];
        memcpy(paddedBuf + LWS_SEND_BUFFER_PRE_PADDING, buf, len);
        return lws_write(
            wsi_,
            paddedBuf + LWS_SEND_BUFFER_PRE_PADDING,
            len,
            writeProtocol);
    }
    else
    {
        // Only 1 write can be outstanding with the library.
        // This call can only be executed within OnDataWritable!
        largeWriteBuf_.reset(new unsigned char[totalSize]);
        memcpy(largeWriteBuf_.get() + LWS_SEND_BUFFER_PRE_PADDING, buf, len);
        return lws_write(
            wsi_,
            largeWriteBuf_.get() + LWS_SEND_BUFFER_PRE_PADDING,
            len,
            writeProtocol);
    }
}

