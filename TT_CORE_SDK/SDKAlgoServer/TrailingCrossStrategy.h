#pragma once
#include <tt_cplus_sdk.h>
#include <string>
#include <atomic>
#include <cmath>
#include "BaseStrategy.h"

class SDKAlgoManager;
/* a simple strategy that listens to prices for a given contract. when an
    ask market is present, it places a buy order one tick below the inside ask.
    when the ask market moves away, the order will follow. when the order is
    filled, it will place another buy order one tick away from the fill price
    of the previous order
*/
class TrailingCrossStrategy : public BaseStrategy,
    public ttsdk::IOrderEventHandler,
    public ttsdk::IPriceEventHandler
{
public:
    TrailingCrossStrategy(SDKAlgoManager& mgr);
    virtual ~TrailingCrossStrategy();

    virtual bool Start(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req);
    virtual bool Recover(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req);
    virtual void Update(ttsdk::SDKAlgoRequestPtr req);
    virtual void Stop(ttsdk::SDKAlgoRequestPtr req);
    virtual void InternalStop();

    virtual void OnRiskReserved(const uint64_t instrumentId, const uint64_t accountId, const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code);
    virtual void OnRiskReleased(const uint64_t instrumentId, const uint64_t accountId, const ttsdk::RiskSide side, const ttsdk::AlgoResponseCode code);


    // price callbacks
    virtual void OnUnsubscribed(const uint64_t subId);
    virtual void OnDisconnect(const uint64_t subId, const ttsdk::InstrumentPtr&, const ttsdk::PriceEventType);
    virtual void OnPriceUpdate(const uint64_t, const ttsdk::InstrumentPtr&, const ttsdk::PriceSnap& snap);
    virtual void OnError(const uint64_t subId, const ttsdk::InstrumentPtr& instrument,
        const ttsdk::SubscriptionError code, const char* error);

    // order callbacks
    virtual void OnUnsubscribed(const char*);
    virtual void OnExecutionReport(ttsdk::OrderPtr order, ttsdk::ExecutionReportPtr execRpt);
    virtual void OnReject(ttsdk::OrderPtr order, ttsdk::RejectResponsePtr rejResp);
    virtual void OnSendFailed(ttsdk::OrderPtr order, const ttsdk::OrderProfile& profile, const ttsdk::IOrderEventHandler::SendCode code);

protected:
    void  ReleaseRisk();
    ttsdk::AlgoResponseCode InitializeStrategy(ttsdk::SDKAlgoPtr algoOrder, ttsdk::SDKAlgoRequestPtr req, std::string& error);
    static bool double_eq(double p1, double p2)
    {
        return (std::isnan(p1) && std::isnan(p2)) ||
            (std::isinf(p1) && std::isinf(p2)) ||
            std::abs(p1 - p2) <= 0.000000001;
    };
    inline const char* GetOrderId() const;

    void PlaceOrder(const ttsdk::PriceSnap& snap);
    void PlaceOrder(const double price);
    void SendCancel();
    void SendChange(const double qty);
    void OnOrderCancelled();
    void OnOrderReplaced();

    SDKAlgoManager& mgr_;
    ttsdk::OrderProfile profile_;
    ttsdk::InstrumentPtr instrument_;
    ttsdk::OrderPtr order_;
    std::atomic<bool> shuttingDown_;
    std::atomic<bool> replacing_;
    std::atomic<bool> failed_;
    uint64_t subId_;
    ttsdk::SDKAlgoPtr algoOrder_;
    ttsdk::SDKAlgoRequestPtr lastReq_;
    double mktPrice_ = NAN;
    uint32_t ticksAway_ = 1;
    uint32_t maxRequotes_ = 3;
    uint32_t requotes_ = 0;

private:

};


inline const char* TrailingCrossStrategy::GetOrderId() const
{
    if (algoOrder_) return algoOrder_->GetOrderId();
    return "";
}
