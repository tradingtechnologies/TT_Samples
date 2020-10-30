#pragma once
#include <tt_cplus_sdk.h>
#include <string>
#include <atomic>
#include <cmath>

namespace ttsample {

    class StrategyManager;

    /* a simple strategy that listens to prices for a given contract. when an
       ask market is present, it places a buy order one tick below the inside ask.
       when the ask market moves away, the order will follow. when the order is
       filled, it will place another buy order one tick away from the fill price
       of the previous order
    */
    class PlaceOrderStrategy : public ttsdk::IOrderEventHandler,
                               public ttsdk::IPriceEventHandler
    {
    public:
        PlaceOrderStrategy(StrategyManager& mgr, const std::string id, 
            const ttsdk::MarketId market, const std::string product,
            const ttsdk::ProductType productType, const std::string alias, 
            const uint64_t acctId, const double qty);
        virtual ~PlaceOrderStrategy();

        void Start();
        bool IsRunning() { return running_; }
        const std::string& Id() { return id_; }
        void SendChange(const double qty);
        void SendCancel();
        void Stop();

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
        static bool double_eq(double p1, double p2)
        {
            return (std::isnan(p1) && std::isnan(p2)) ||
                (std::isinf(p1) && std::isinf(p2)) ||
                std::abs(p1 - p2) <= 0.000000001;
        };        
        
        // sends a message to the manager class about what is happening
        // with strategy
        void SendStatusMessage(const char* status);         
        void PlaceOrder(const ttsdk::PriceSnap& snap);
        void PlaceOrder(const double price);
        void OnOrderCancelled();
        
        StrategyManager& mgr_;

        const std::string id_;
        const ttsdk::MarketId market_;
        const std::string product_;
        const ttsdk::ProductType productType_;
        const std::string alias_;

        ttsdk::OrderProfile profile_;
        ttsdk::InstrumentPtr instrument_;
        ttsdk::OrderPtr order_;
        std::atomic<bool> running_;
        std::atomic<bool> shuttingDown_;
        uint64_t subId_;

    private:
        PlaceOrderStrategy();

    };

}