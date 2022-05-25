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
#include <sdkalgo.h>
#include <atomic>
#include <map>

class BaseStrategy
{
public:
    BaseStrategy() {};
    virtual ~BaseStrategy() {};

    // handle the requests from the TT SDK
    virtual bool Start(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req) = 0;
    virtual bool Recover(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req) = 0;
    virtual void Update(ttsdk::SDKAlgoRequestPtr req) = 0;
    virtual void Stop(ttsdk::SDKAlgoRequestPtr req) = 0;

    virtual void OnRiskReserved(const uint64_t instrumentId, const uint64_t accountId, const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code) {};
    virtual void OnRiskReleased(const uint64_t instrumentId, const uint64_t accountId, const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code) {};

    // stop the algo from running
    virtual void InternalStop() = 0;

protected:

};

