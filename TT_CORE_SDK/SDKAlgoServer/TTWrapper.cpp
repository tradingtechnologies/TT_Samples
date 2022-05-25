#include <iostream>
#include <iomanip>
#include <atomic>
#include <sstream>
#include <string.h>
#include "TTWrapper.h"

#include <connection.h>

TTWrapper::TTWrapper() : ttsdk::IEventHandler(), sdkComplete_(false) 
{
    stopping_.store(false);
};

void TTWrapper::OnStatus(const ttsdk::IEventHandler::Status status)
{
    std::cout << "SDKEventHandler::OnStatus (" << (uint32_t)status << ")" << std::endl;
    if (status == ttsdk::IEventHandler::Status::DOWN && stopping_.load())
    {
        sdkComplete_ = true;
        // send message to client app the SDK is down
        sdkFinishedCondition_.notify_one();
    }
    else if (status == ttsdk::IEventHandler::Status::SDKALGO_CONNECTED)
    {
        std::cout << "SDK Algo Ready" << std::endl;
        sdkReadyCondition_.notify_one();
    }
    else if (status == ttsdk::IEventHandler::Status::SDKALGO_NOT_CONNECTED)
    {
        std::cout << "SDK Algo Not Connected!" << std::endl;
    }
};

void TTWrapper::OnAccountStatus(const ttsdk::AccountConnectionStatus& status)
{
    std::ostringstream ss;
    ss << "Account connections status changed account_id=" << status.account_id
        << " conn count=" << status.count;
    std::cout << ss.str() << std::endl;
};


bool TTWrapper::Start(const std::string& appkey, const ttsdk::Environment env)
{
    algoManager_ = std::make_unique<SDKAlgoManager>();

    ttsdk::TTSDKOptions options;
    options.environment = env;
    options.app_key_secret = appkey.c_str();
    // has to be unique and remain for the same for this application
    // instance across process runs
    options.sdk_instance_id = 1; 
    if (!ttsdk::Initialize(options, this, algoManager_.get(), algoManager_.get()))
    {
        return false;
    }

    std::unique_lock<std::mutex> lock(mutex);
    if (sdkReadyCondition_.wait_for(lock, std::chrono::seconds(5000)) == std::cv_status::timeout)
    {
        return false;
    }
    return true;
}

bool TTWrapper::Stop()
{
   stopping_ = true;
    ttsdk::Shutdown();
    // wait for the shutdown to be completed
    std::unique_lock<std::mutex> lock(mutex);
    while (!sdkComplete_)
    {
        if (sdkFinishedCondition_.wait_for(lock, std::chrono::seconds(5000)) == std::cv_status::timeout)
        {
            return false;
        }
    }

    if (algoManager_)
        algoManager_.release();
    return true;
}



