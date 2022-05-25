#pragma once
#include <tt_cplus_sdk.h>
#include <condition_variable>
#include "SDKAlgoManager.h"

class TTWrapper : public ttsdk::IEventHandler
{
public:
    TTWrapper();
    virtual ~TTWrapper() {};

    // IEventHandler callbacks
    virtual void OnStatus(const ttsdk::IEventHandler::Status status) override;
    virtual void OnAccountStatus(const ttsdk::AccountConnectionStatus& status) override;


    bool Start(const std::string& appkey, const ttsdk::Environment env);
    bool Stop();

protected:

    std::mutex mutex;
    std::condition_variable sdkReadyCondition_;
    std::condition_variable sdkFinishedCondition_;
    bool sdkComplete_;
    std::atomic<bool> stopping_;
    
    std::unique_ptr<SDKAlgoManager> algoManager_;

};




