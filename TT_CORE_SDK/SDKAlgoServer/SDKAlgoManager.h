/***************************************************************************
 *
 *                  Unpublished Work Copyright (c) 2021
 *                  Trading Technologies International, Inc.
 *                       All Rights Reserved Worldwide
 *
 *          * * *   S T R I C T L Y   P R O P R I E T A R Y   * * *
 *
 * WARNING:  This program (or document) is unpublished, proprietary property
 * of Trading Technologies International, Inc. and is to be maintained in
 * strict confidence. Unauthorized reproduction, distribution or disclosure
 * of this program (or document), or any program (or document) derived from
 * it is prohibited by State and Federal law, and by local law outside of
 * the U.S.
 *
 ***************************************************************************/
#pragma once
#include <string.h>
#include <map>
#include <vector>
#include <mutex>
#include <memory>
#include <tt_cplus_sdk.h>
#include "BaseStrategy.h"

class SDKAlgoManager : public ttsdk::ISDKAlgoManager, public ttsdk::IOrderBookEventHandler
{
public:
    SDKAlgoManager();
    virtual ~SDKAlgoManager() {};
    // ISDKAlgoManager callbacks
    virtual const char* GetMarkets() override
    {
        return markets_.c_str();
    }
    virtual bool RecoverSDKAlgos() const override { return false; }

    virtual void OnRecoverAlgo(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req);
    virtual void OnRecoveryEnd();
    virtual void OnRecoveryFailed();
    virtual void OnStartRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req);
    virtual void OnUpdateRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req);
    virtual void OnStopRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req);
    virtual void OnPauseRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req);
    virtual void OnResumeRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req);
    virtual void OnAlgoCompleted(ttsdk::SDKAlgoPtr algoOrder);
    virtual void OnScheduledEvent(ttsdk::SDKAlgoPtr algoOrder, unsigned int eventId, void* eventUserData);
    virtual void OnRiskReserved(ttsdk::SDKAlgoPtr algoOrder, const uint64_t instrumentId, const uint64_t accountId,
        const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code);
    virtual void OnRiskReleased(ttsdk::SDKAlgoPtr algoOrder, const uint64_t instrumentId, const uint64_t accountId,
        const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code);

    void StrategyStopped(const std::string& algoId);


    // IOrderBookEventHandler callbacks
    virtual void OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt) override;
    virtual void OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp) override;
    virtual void OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const ttsdk::IOrderBookEventHandler::SendCode code) override;
    virtual void OnUnsubscribed(const char* orderId) override;
    virtual void OnAccountDownloadEnd(const uint64_t accountId) override;
    virtual void OnAccountDownloadFailed(const uint64_t accountId, const char* msg) override;
    virtual void OnOrderBookDownloadEnd() override;
    virtual void OnPositionUpdate(const ttsdk::Position& updatedPosition) override;

protected:
    void RecoveryCompleted();
protected:
    mutable std::mutex mutexStrategies_;
    std::map< std::string, std::shared_ptr<BaseStrategy> > strategies_;
    std::atomic<bool> downloadEnd_;

    std::string markets_ = "7,32";

};
