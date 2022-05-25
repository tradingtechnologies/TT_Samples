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
#include "SDKAlgoManager.h"
#include "TrailingCrossStrategy.h"
#include <iostream>
#include <cassert>

SDKAlgoManager::SDKAlgoManager() : ttsdk::ISDKAlgoManager(), ttsdk::IOrderBookEventHandler(), downloadEnd_(false) 
{
}

void SDKAlgoManager::OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt)
{
    if (downloadEnd_)
    {
        // realtime order receove for the shared order book that is not going directly to an algo listener
        // so we don't really care about it enough to save it or do anything with it
        std::cout << "OnExecutionReport TTOrderId=" << order->GetOrderId() << " OrderStatus=" << ttsdk::ToString(execRpt->GetOrderStatus()) << std::endl;
    }
    else
    {
    }
};
void SDKAlgoManager::OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp)
{
    std::cout << "OnReject" << std::endl;
}
void SDKAlgoManager::OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const ttsdk::IOrderBookEventHandler::SendCode code)
{
    std::cout << "OnSendFailed" << std::endl;
}
void SDKAlgoManager::OnUnsubscribed(const char* orderId)
{
    std::cout << "OnUnsubscribed" << std::endl;
}
void SDKAlgoManager::OnAccountDownloadEnd(const uint64_t accountId)
{
    std::cout << "---OnAccountDownloadEnd accountId:" << accountId << std::endl;
};
void SDKAlgoManager::OnAccountDownloadFailed(const uint64_t accountId, const char* msg)
{
    std::cout << "---OnAccountDownloadFailed accountId:" << accountId << " details: " << msg << std::endl;
};
void SDKAlgoManager::OnOrderBookDownloadEnd()
{
    std::cout << "------OnOrderBookDownloadEnd------" << std::endl;
    downloadEnd_.store(true);
};
void SDKAlgoManager::OnPositionUpdate(const ttsdk::Position& updatedPosition)
{
}



void SDKAlgoManager::OnRecoverAlgo(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req)
{
    std::cout << "SDKAlgoManager::OnRecoverAlgo " << algoOrder->GetOrderId() << 
                    " OrderStatus=" << ttsdk::ToString(algoOrder->GetCurrentState()->GetOrderStatus()) << std::endl;
    auto algo = algoOrder->GetAlgoDefinition();
    if (!algo)
    {
        ttsdk::TTLogInfo("Algo: %s Unknown algo instrument - unable to recover.", algoOrder->GetOrderId());
        algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::InvalidAlgoInstrument, "Unknown algo instrument - unable to recover.");
        return;
    }

    if (strncmp(algo->GetName(), "TrailingXSDKAlgo", sizeof(algo->GetName())) == 0)
    {
        std::shared_ptr<BaseStrategy> strategy(new TrailingCrossStrategy(*this));
        std::string parentId = algoOrder->GetOrderId();
        if (strategy->Recover(algoOrder, req))
        {
            {
                std::unique_lock<std::mutex> lock(mutexStrategies_);
                strategies_.insert(std::make_pair(req->GetOrderId(), strategy));
            }
            ttsdk::TTLogInfo("Algo: %s recovered and started.", algoOrder->GetOrderId());
        }
        else
        {
            ttsdk::TTLogInfo("Algo: %s unable to recover algo.", algoOrder->GetOrderId());
        }
    }
};
void SDKAlgoManager::OnRecoveryEnd()
{
    std::cout << "SDKAlgoManager::OnRecoveryEnd " << std::endl;
    RecoveryCompleted();
}
void SDKAlgoManager::OnRecoveryFailed()
{
    // something went wrong
    std::cout << "SDKAlgoManager::OnRecoveryFailed " << std::endl;
    RecoveryCompleted();
}
void SDKAlgoManager::RecoveryCompleted()
{
    std::cout << "SDKAlgoManager::RecoveryCompleted " << std::endl;
}

void SDKAlgoManager::OnStartRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req)
{
    auto algo = algoOrder->GetAlgoDefinition();
    if (!algo)
    {
        ttsdk::TTLogInfo("Algo: %s No strategy specified. Unable to start.", algoOrder->GetOrderId());
        algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::InvalidAlgoInstrument, "No algo specified.");
        return;
    }
    std::cout << "SDKAlgoManager::OnStartRequest " << algo->GetName() << " id:" << req->GetOrderId() << std::endl;
    if (strncmp(algo->GetName(), "TrailingXSDKAlgo", sizeof(algo->GetName())) == 0)
    {
        std::shared_ptr<BaseStrategy> strategy(new TrailingCrossStrategy(*this));
        if (strategy->Start(algoOrder, req))
        {
            std::unique_lock<std::mutex> lock(mutexStrategies_);
            strategies_.insert(std::make_pair(req->GetOrderId(), strategy));
        }
        // let the strategy decide when it is started and send the callback to the TT SDK
    }
    else
    {
        algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::InvalidAlgoInstrument, "Unsupported algo requested.");
    }
}
void SDKAlgoManager::OnUpdateRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req)
{
    std::cout << "SDKAlgoManager::OnUpdateRequest " << req->GetOrderId() << " " << req->GetQuantity() << "@" << req->GetPrice() << std::endl;
    std::shared_ptr<BaseStrategy> strategy(nullptr);
    {
        std::unique_lock<std::mutex> lock(this->mutexStrategies_);
        auto iter = strategies_.find(algoOrder->GetOrderId());
        if (iter != strategies_.end())
        {
            strategy = iter->second;
        }
    }
    if (strategy)
    {
        return strategy->Update(req);
    }
    algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::UnknownOrder);
}
void SDKAlgoManager::OnStopRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req)
{
    std::cout << "SDKAlgoManager::OnStopRequest " << req->GetOrderId() << std::endl;
    std::shared_ptr<BaseStrategy> strategy(nullptr);
    {
        std::unique_lock<std::mutex> lock(this->mutexStrategies_);
        auto iter = strategies_.find(algoOrder->GetOrderId());
        if (iter != strategies_.end())
        {
            strategy = iter->second;
        }
    }
    if (strategy)
    {
        return strategy->Stop(req);
    }
    algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::UnknownOrder, "Unable to find order. Please use force delete.");
}


void SDKAlgoManager::OnPauseRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req)
{
    std::cout << "SDKAlgoManager::OnPauseRequest " << req->GetOrderId() << std::endl;
    algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::NotSupportedAction, "Unsupported action");
}    
void SDKAlgoManager::OnResumeRequest(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req)
{
    std::cout << "SDKAlgoManager::OnResumeRequest " << req->GetOrderId() << std::endl;
    algoOrder->OnPendingRequestCompleted(ttsdk::AlgoResponseCode::NotSupportedAction, "Unsupported action");
}


void SDKAlgoManager::OnAlgoCompleted(ttsdk::SDKAlgoPtr algoOrder)
{
    std::cout << "SDKAlgoManager::OnAlgoCompleted " << algoOrder->GetOrderId() << std::endl;
    // tell the algo to clean up
    std::shared_ptr<BaseStrategy> strategy(nullptr);
    {
        std::unique_lock<std::mutex> lock(this->mutexStrategies_);
        auto iter = strategies_.find(algoOrder->GetOrderId());
        if (iter != strategies_.end())
        {
            strategy = iter->second;
        }
    }
    if (strategy)
    {
        return strategy->InternalStop();
    }
}

void SDKAlgoManager::OnScheduledEvent(ttsdk::SDKAlgoPtr algoOrder, unsigned int eventId, void* eventUserData)
{
    std::cout << "SDKAlgoManager::OnScheduledEvent " << algoOrder->GetOrderId() <<  " eventId:" << eventId << std::endl;
}


void SDKAlgoManager::OnRiskReserved(ttsdk::SDKAlgoPtr algoOrder, const uint64_t instrumentId, const uint64_t accountId,
    const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code)
{
    std::cout << "SDKAlgoManager::OnRiskReserved " << algoOrder->GetOrderId() << std::endl;
    // tell the algo to clean up
    std::shared_ptr<BaseStrategy> strategy(nullptr);
    {
        std::unique_lock<std::mutex> lock(this->mutexStrategies_);
        auto iter = strategies_.find(algoOrder->GetOrderId());
        if (iter != strategies_.end())
        {
            strategy = iter->second;
        }
    }
    if (strategy)
    {
        return strategy->OnRiskReserved(instrumentId, accountId, side, code);
    }
}
void SDKAlgoManager::OnRiskReleased(ttsdk::SDKAlgoPtr algoOrder, const uint64_t instrumentId, const uint64_t accountId,
    const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code)
{
    std::cout << "SDKAlgoManager::OnRiskReleased " << algoOrder->GetOrderId() << std::endl;
    // tell the algo to clean up
    std::shared_ptr<BaseStrategy> strategy(nullptr);
    {
        std::unique_lock<std::mutex> lock(this->mutexStrategies_);
        auto iter = strategies_.find(algoOrder->GetOrderId());
        if (iter != strategies_.end())
        {
            strategy = iter->second;
        }
    }
    if (strategy)
    {
        return strategy->OnRiskReleased(instrumentId, accountId, side, code);
    }
}


void SDKAlgoManager::StrategyStopped(const std::string& algoId)
{
    std::cout << "SDKAlgoManager::StrategyStopped " << algoId << std::endl;
    std::unique_lock<std::mutex> lock(mutexStrategies_);
    strategies_.erase(algoId);
}