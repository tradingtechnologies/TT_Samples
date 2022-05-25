/***************************************************************************
 *
 *                  Unpublished Work Copyright (c) 2018-2020
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

#include "consts.h"
#include "enums/OrderType.h"
#include "enums/OrderSide.h"
#include "enums/TimeInForce.h"
#include "instrument.h"
#include "execution_report.h"
#include "reject_response.h"
#include "position.h"
#include "shared_ptr.h"
#include "sdkalgo.h"

 // Example computing the expire date in C++ for December 19, 2026.
 // timespec now;
 // clock_gettime(CLOCK_REALTIME, &now);
 // tm utc, local;
 // gmtime_r(&now.tv_sec, &utc);
 // localtime_r(&now.tv_sec, &local);
 // utc.tm_isdst = -1;  // This assumes time_t unit is seconds.  True
 // time_t bias = now.tv_sec - mktime(&utc); // on all known systems.
 // Bias is the offset in seconds from local time to UTC time.
 // tm gtdate;
 // gtdate.tm_isdst = -1;              // Let local system decide.
 // gtdate.tm_sec = gtdate.tm_min = gtdate.tm_hour = 0;
 // gtdate.tm_mday = 19;               // Day of the month (1-31).
 // gtdate.tm_mon = 11;                // Month of the year (0-11).
 // gtdate.tm_year = 2026 - 1900;      // Year since 1900.
 // time_t seconds = mktime(&gtdate) + bias;  // Seconds since epoch UTC.
 // The mktime function will take the local time into account.
 // In order to convert to UTC add the bias.
 // new_order.set_expire_date(seconds * 1000000000ULL);  // Seconds to nanoseconds.

namespace ttsdk
{
    //! \struct OrderProfile
    //! \brief Definition of an order to provide when submitting
    struct OrderProfile
    {
        uint64_t request_id = 0;
        uint64_t price_subscription_id = 0;
        OrderType type = OrderType::NotSet;
        uint64_t expire_date = 0;   // nanoseconds since the epoch UTC
        OrderSide side = OrderSide::NotSet;
        TimeInForce tif = TimeInForce::NotSet;
        uint64_t account_id = 0;
        char clearing_acct_override[128] = {0};
        double price = NAN;
        double trigger_price = NAN;
        double quantity = NAN;
        double display_quantity = NAN;
        double minimum_quantity = NAN;
        char text[128] = { 0 };
        char text_a[128] = { 0 };
        char text_b[128] = { 0 };
        char text_c[128] = { 0 };
        char text_tt[128] = { 0 };
        char sender_sub_id[128] = { 0 };
        //!< indicator to the SDK whether or not to cancel this
        //!  order if it is working when the SDK restarts. 
        //!  the TTSDKOptions.sdk_instance_id needs to remain the same from
        //!  run to run to remove the orders placed by the previous run
        bool leave_on_restart = true;

        //!< Time variables which can be populated to store on the outbound order
        //! for performance measuring of order reactions to price updates
        uint64_t server_price_time = 0;   //!< Price Server timestamp (mdrc_recv_time);
        uint64_t order_stimulus_received = 0; //!< User listener timestamp (order_stimulus_receieved_oc)
    };

    //! \struct OrderPrcQtyProfile
    //! \brief Definition of an order price/qty change
    struct OrderPrcQtyProfile
    {
        uint64_t request_id = 0;
        uint64_t price_subscription_id = 0;
        double price = NAN;
        double quantity = NAN;
        //!< Time variables which can be populated to store on the outbound order
        //! for performance measuring of order reactions to price updates
        uint64_t server_price_time = 0;   //!< Price Server timestamp (mdrc_recv_time);
        uint64_t order_stimulus_received = 0; //!< User listener timestamp (order_stimulus_receieved_oc)
    };
    

    class Order;
    using OrderPtr = shared_ptr<Order>;

    //! \class IOrderEventHandler
    //! \brief Interface for listening to order events
    //! \warning Events are delivered on a thread managed by the SDK. The number of
    //!          order delivery threads is set in the SDK options when initializing
    //!          the SDK. Users can do work on this thread since they control how many
    //!          threads there are and the threads are only for order events.
    class IOrderEventHandler
    {
    public:
        enum SendCode
        {
            SUCCESS,
            UNKNOWN,
            TIMEOUT,
            INVALID_ACCOUNT_FOR_MARKET,
            INVALID_INSTRUMENT,
            NETWORK_FAILURE,
            SDK_NOT_INITIALIZED,
            ALREADY_IN_MARKET,
            CHANGE_AFTER_CANCEL,
            ORDER_NOT_WORKING,
            PRICE_SUBSCRIPTION_REQUIRED,
            ORDER_MISSING_REQUIRED_DATA,
            MALFORMED,
            ORDERBOOK_NOT_SYNCHRONIZED,
            RISK_NOT_READY,
            PRICE_SUBSCRIPTION_NOT_READY,
            SYNTHETIC_ROUTING_NOT_AVAILABLE,
            ORDER_MISSING_MARKET_DATA,
            INVALID_ACCOUNT_ID,
            RESTRICTED_ACCOUNT,
            EXCEEDED_PREALLOCATED_RISK,
            ORDER_THROTTLE_EXCEEDED,
            CHANGE_ACCOUNT_ID_NOT_ALLOWED,
            EXCEEDED_PREALLOCATED_RISK_CLIP_SIZE,
            INVALID_CLIP_SIZE,
        };

        virtual ~IOrderEventHandler() noexcept = default;

        //! \brief Callback delivering execution report messages
        virtual void OnExecutionReport(OrderPtr order, ExecutionReportPtr execRpt) = 0;

        virtual void OnEpiqUpdate(OrderPtr order, double epiq) {};

        //! \brief Callback delivering order reject messages
        virtual void OnReject(OrderPtr order, RejectResponsePtr rejResp) = 0;

        //! \brief Callback fired when a request delivery surpasses the timeout threshold
        virtual void OnSendFailed(OrderPtr order, const OrderProfile& profile, const SendCode code) = 0;

        //! \brief Callback fired when the unsubscribe request is complete and it is safe to
        //         destroy the handler object (sdk will no longer use it)
        //         orderId is null when unsubscribed from all order events
        virtual void OnUnsubscribed(const char* orderId) = 0;

    };
    using IOrderEventHandlerPtr = IOrderEventHandler*;


    //! \class Order
    //! \brief an interface to interact with the order.
    class Order : public shared_base
    {
    public:
        explicit Order() {};
        ~Order() {};

        virtual const char* GetOrderId() const noexcept = 0;
        virtual const double GetEpiq() const noexcept = 0;
        virtual void Subscribe(IOrderEventHandler& listener) noexcept = 0;
        virtual void Unsubscribe() noexcept = 0;
        virtual ExecutionReportPtr GetCurrentState() const noexcept = 0;
        virtual InstrumentPtr GetInstrument() const noexcept = 0;

        virtual void SendNew(const OrderProfile& profile) noexcept = 0;
        virtual void SendChange(const OrderProfile& profile) noexcept = 0;
        virtual void SendChange(const OrderPrcQtyProfile& profile) noexcept = 0;
        virtual void SendCancel(const OrderProfile& profile) noexcept = 0;

        virtual void SetParent(SDKAlgoPtr parent = nullptr) noexcept = 0;
        //! \brief Method which indicates if this order is a SDK Algo order that
        //!        is/was being managed by this application instance
        virtual bool IsAppsSDKAlgoOrder() noexcept = 0;

    private:
        Order(const Order&) = delete;
        Order& operator=(Order&) = delete;
        Order(Order&&) = delete;
        Order& operator=(Order&&) = delete;
    };


    //! \class IOrderBookEventHandler
    //! \brief Interface for listening to order and position events. There is
    //!        one OrderBookEventHandler set in the SDK. All order events
    //!        will go to this handler unless a specific handler is set for
    //!        specific orders on an order by order basis. 
    //! \warning It is assumed this object will exist for the life of the SDK.
    class IOrderBookEventHandler : public IOrderEventHandler
    {
    public: 
        virtual ~IOrderBookEventHandler() noexcept = default;

        //! \brief Callback delivering position updates
        virtual void OnPositionUpdate(const Position& updatedPosition) {};

        //! \brief Indicates the given account have been synchronized with the realtime
        //!        streams, orders and positions have been downloaded.
        virtual void OnAccountDownloadEnd(const uint64_t accountId) {};

        //! \brief Indicates the given account orders and positions downloads
        //!        have failed and the account if not usable. This is a critical 
        //!        failure.
        virtual void OnAccountDownloadFailed(const uint64_t accountId, const char* message) {};

        //! \brief Indicates all orders and positions for all accounts are
        //!        downloaded and synchronized with the realtime stream.
        virtual void OnOrderBookDownloadEnd() {};


        //! \brief Callback for a reserve risk request. Orders sent before a PR is acked will be rejected.
        virtual void OnRiskReserved(const uint64_t instrumentId, const uint64_t accountId,
            const RiskSide side, const bool successful) {};
        //! \brief Callback for a release reserve risk request.
        virtual void OnRiskReleased(const uint64_t instrumentId, const uint64_t accountId,
            const RiskSide side, const bool successful) {};


    };
    using IOrderBookEventHandlerPtr = IOrderBookEventHandler*;

}
