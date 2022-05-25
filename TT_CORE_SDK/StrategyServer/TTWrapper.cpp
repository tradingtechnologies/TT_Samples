#include <iostream>
#include <iomanip>
#include <atomic>
#include <sstream>
#include <string.h>
#include "TTWrapper.h"

#include <connection.h>

void TTWrapper::OnStatus(const ttsdk::IEventHandler::Status status)
{
    if (status == ttsdk::IEventHandler::Status::DOWN)
    {
        // send message to client app the SDK is down
    }
};
void TTWrapper::OnAccountStatus(const ttsdk::AccountConnectionStatus& status)
{
};
void TTWrapper::OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt)
{
};
void TTWrapper::OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp)
{
}
void TTWrapper::OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const ttsdk::IOrderBookEventHandler::SendCode code)
{
}
void TTWrapper::OnUnsubscribed(const char* orderId)
{
}
void TTWrapper::OnAccountDownloadEnd(const uint64_t accountId)
{
};
void TTWrapper::OnAccountDownloadFailed(const uint64_t accountId, const char* msg)
{
};
void TTWrapper::OnOrderBookDownloadEnd()
{
    std::lock_guard<std::mutex> lock(mutex);
    sdkReadyCondition.notify_one();
};
void TTWrapper::OnPositionUpdate(const ttsdk::Position& updatedPosition)
{
}

bool TTWrapper::Start(const char* appkey)
{
    ttsdk::TTSDKOptions options;
    options.environment = ttsdk::Environment::DevCert;
    options.app_key_secret = appkey;
    if (!ttsdk::Initialize(options, this, this, nullptr))
    {
        return false;
    }

    std::unique_lock<std::mutex> lock(mutex);
    if (sdkReadyCondition.wait_for(lock, std::chrono::seconds(5000)) == std::cv_status::timeout)
    {
        return false;
    }
    return true;
}

bool TTWrapper::Stop()
{
    ttsdk::Shutdown();
    return true;
}



