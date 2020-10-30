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

#include <stddef.h>
#include "enums/ImpliedsSource.h"
#include "enums/TradingStatus.h"
#include "enums/TradeIndicator.h"
#include "enums/ExchSupportedOTCTypes.h"
#include "enums/TradeType.h"
#include "enums/TradeQualifier.h"
#include "instrument.h"
#include <math.h>

namespace ttsdk {

    enum class PriceEventType
    {
        DIRECT,
        CALCULATED_IMPLIEDS,
        SIMULATED,
        UNKNOWN,
    };
    struct PriceSnap
    {
        static constexpr size_t MAX_DEPTH_LEVEL = 10; //!< Max supported levels of depth

        enum class SnapStatus
        {
            NOT_REQUESTED,
            RECEIVED,
            NOT_RECEIVED,
            ERROR,
            UNKNOWN,
        };
        bool all_snaps_ready;                       //!< helper bool indicates all requested data has been received and merged
        SnapStatus direct = SnapStatus::UNKNOWN;    //!< status of the direct subscription
        SnapStatus implied = SnapStatus::UNKNOWN;   //!< status of the implied subscription
        SnapStatus simulated = SnapStatus::UNKNOWN; //!< status of the simulated subscription
        ImpliedsSource implieds_source = ImpliedsSource::Noimplieds;

        //! \struct depth_snap
        //! \brief Snapshot of the market depth
        struct DepthSnap
        {
            //! \struct depth_level
            //! \brief A single level of the total depth
            struct DepthLevel
            {
                double price;            //!< Price
                int32_t order_count;     //!< Number of orders that make up the total quantity
                double quantity;         //!< Total quantity at this level
                double implied_quantity; //!< Implied quantity contributing to the total quantity
            } levels[MAX_DEPTH_LEVEL];

            size_t count; //!< Number of available price depth levels
        };

        uint64_t instrument_id; //!< Instrument Id for the trade
        TradingStatus market_state; //!< Market State

        //!@{ Price feed depth
        DepthSnap asks; //!< Ask side depth
        DepthSnap bids; //!< Bid side depth
        //!@}

        //!@{ Price feed properties
        double open_price = NAN;          //!< Open price (NAN when unavailable)
        double close_price = NAN;         //!< Close price (NAN when unavailable)
        double high_price = NAN;          //!< High price (NAN when unavailable)
        double low_price = NAN;           //!< Low price (NAN when unavailable)
        double last_trade_price = NAN;    //!< Last traded price (NAN when unavailable)
        double last_trade_quantity = NAN; //!< Last traded quantity (NAN when unavailable)
        double volume = NAN;              //!< Total contract volume
        double cum_last_trade_quantity = NAN; //!<Cum Last traded quantity (NAN when unavailable)
        double settlement_price = NAN;    //!< Settlement price (NAN when unavailable)
        double open_interest = NAN;       //!< Open interest
        uint64_t epoch_transact_time_nano = 0;
        //!@}

        //!@{ Indicative values
        double ind_settle = NAN;          //!< Indicative Settle price (NAN when unavailable)
        double ind_price = NAN;           //!< Indicative price (NAN when unavailable)
        double ind_quantity = NAN;        //!< Indicative quantity (NAN when unavailable)
        double ind_bid = NAN;             //!< Indicative Bid price (NAN when unavailable)
        double ind_ask = NAN;             //!< Indicative Ask Price (NAN when unavailable)
        //!@}


        //!@{ Price snapshot properties
        PriceEventType event_type;  //!< the data type that triggered this event
        uint64_t seq_no;            //!< Price snap sequence number
        uint64_t conflation_no;     //!< Price conflation count
        uint64_t exchange_time;     //!< Exchange timestamp
        uint64_t server_time;       //!< Price Server timestamp (mdrc_recv_time);
        uint64_t client_time;       //!< Client timestamp (client_recv_time);
        uint64_t sdk_time;          //!< User listener timestamp (order_stimulus_received_algo)
        uint64_t exchange_seq_no;
        //!@}
    };

    //! \struct TimeSales
    //! \brief Time and Sales update data
    struct TimeSales
    {
        static constexpr size_t MAX_EVENTS = 10; //!< Max supported trades in one update
        uint64_t instrument_id;                  //!< Instrument Id for the trade
        PriceEventType event_type;
        size_t count;                            //!< Number of available T&S events

        struct Trade
        {
            //!@{ Time and Sales feed properties
            double last_trade_price = NAN;           //!< Last Traded Price (NAN when unavailable)
            double last_trade_quantity = NAN;        //!< Last traded quantity (NAN when unavailable)
            ttsdk::TradeType trade_side;             //!< Trade Side
            ttsdk::TradeQualifier qualifier;         //!< Trade Qualifier
            ttsdk::TradeIndicator  indicator;
            ttsdk::ExchSupportedOTCTypes otc_type;
            uint32_t bid_counterparty_id;
            uint32_t ask_counterparty_id;
            //!@}

            //!@{ Alternate trade information
            double   alternate_last_traded_price = NAN;
            double   alternate_last_traded_quantity = NAN;
            uint64_t alternate_epoch_transact_time_nano = 0;
            //!@}

            //!@{ Time and Sales trade properties
            uint64_t epoch_transact_time_nano;       //!< Transaction Time Since Epoch (ns)
            uint64_t exchange_transact_time_ns;
            //!@}
        };
        Trade trades[MAX_EVENTS];

    };


    enum class SubscriptionError {
        UNKNOWN_CLIENT,
        DUPLICATE_SUBSCRIPTION,
        INVALID_INSTRUMENT,
        INVALID_INSTRUMENT_SYMBOL,
        PARTYID_NOT_SUPPORTED,
        INSTRUMENT_ID_MISMATCH,
        INACTIVE_INSTRUMENT,
        DUPLICATE_SUBSCRIPTION_ID,
        UNKNOWN_SUBSCRIPTION_ID,
        INVALID_SUBSCRIPTION_PARAMETERS,
        NOT_PERMISSIONED,
        NOT_INITIALIZED,
        INVALID_CONFIGURATION,
        NOTTRADABLE_INSTRUMENT,
        NO_CONNECTIVITY,
        EXCEEDED_MAX_SUBS_ALLOWED,
        UNKNOWN
    };

    //! \class IPriceEventHandler
    //! \brief Interface for listening to price subscription events
    //! \warning Events are delivered on a thread managed by the SDK. The number of
    //!          price delivery threads is set in the SDK options when initializing
    //!          the SDK. Users can do work on this thread since they control how many
    //!          threads there are and the threads are only for price events.
    class IPriceEventHandler
    {
    public:
        virtual ~IPriceEventHandler(void) noexcept = default;

        //! \brief Callback delivering price updates
        virtual void OnPriceUpdate(const uint64_t subId, const InstrumentPtr& instrument, const PriceSnap& snap) = 0;

        //! \brief Callback fired when an error occurs fulfilling a price subscription request
        virtual void OnError(const uint64_t subId, const InstrumentPtr& instrument,
                             const SubscriptionError code, const char* error) = 0;

        //! \brief Callback fired when an disconnection event happens
        virtual void OnDisconnect(const uint64_t subId, const InstrumentPtr& instrument, const PriceEventType type) = 0;

        //! \brief Callback fired when the unsubscribe request is complete and it is safe to
        //         destroy the handler object (sdk will no longer use it)
        virtual void OnUnsubscribed(const uint64_t subId) = 0;
    };
    using IPriceEventHandlerPtr = IPriceEventHandler*;

    //! \class ITimeSalesEventHandler
    //! \brief Interface for listening to time and sales subscription events
    //! \warning Events are delivered on a thread managed by the SDK.
    class ITimeSalesEventHandler
    {
    public:
        virtual ~ITimeSalesEventHandler(void) noexcept = default;

        //! \brief Callback indicating
        virtual void OnTimeSalesUpdate(const uint64_t subId, const InstrumentPtr& instrument, const TimeSales& ts) = 0;

        //! \brief Callback fired when an error occurs fulfilling a price subscription request
        virtual void OnError(const uint64_t subId, const InstrumentPtr& instrument,
                             const SubscriptionError code, const char* error) = 0;

        //! \brief Callback fired when an disconnection event happens
        virtual void OnDisconnect(const uint64_t subId, const InstrumentPtr& instrument, const PriceEventType type) = 0;

        //! \brief Callback fired when the unsubscribe request is complete and it is safe to
        //         destroy the handler object (sdk will no longer use it)
        virtual void OnUnsubscribed(const uint64_t subId) = 0;
    };
    using ITimeSalesEventHandlerPtr = ITimeSalesEventHandler*;



}
