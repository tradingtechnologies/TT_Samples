#pragma once
#include <tt_cplus_sdk.h>
#include <condition_variable>

class TTWrapper : public ttsdk::IEventHandler, public ttsdk::IOrderBookEventHandler
{
public:
    TTWrapper() : ttsdk::IEventHandler(), ttsdk::IOrderBookEventHandler() {};
    virtual ~TTWrapper() {};

    // IEventHandler callbacks
    virtual void OnStatus(const ttsdk::IEventHandler::Status status) override;
    virtual void OnAccountStatus(const ttsdk::AccountConnectionStatus& status) override;

    // IOrderBookEventHandler callbacks
    virtual void OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt) override;
    virtual void OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp) override;
    virtual void OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const ttsdk::IOrderBookEventHandler::SendCode code) override;
    virtual void OnUnsubscribed(const char* orderId) override;
    virtual void OnAccountDownloadEnd(const uint64_t accountId) override;
    virtual void OnAccountDownloadFailed(const uint64_t accountId, const char* msg) override;
    virtual void OnOrderBookDownloadEnd() override;
    virtual void OnPositionUpdate(const ttsdk::Position& updatedPosition) override;

    bool Start(const char* appkey);
    bool Stop();

protected:

    std::mutex mutex;
    std::condition_variable sdkReadyCondition;

};




