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


#ifndef DEPRECATE_SUPER_STRING
    #define DEPRECATE_SUPER_STRING
#endif
#ifndef SUPER_STRING_IS_STD_STRING
    #define SUPER_STRING_IS_STD_STRING
#endif 


#include <sys/types.h>
#include "consts.h"
#include "sdk_options.h"
#include "account.h"
#include "instrument.h"
#include "instrument_search.h"
#include "prices.h"
#include "order.h"
#include "fill.h"
#include "position.h"
#include "connection.h"
#include "enums/MarketId.h"
#include "sdkalgo.h"


namespace ttsdk {


    class IEventHandler
    {
    public:
        enum class Status
        {
            INITIALIZED,      // SDK has been initialized but it not fully ready (some aspects of the SDK might still be negotiating)
            READY,            // SDK is fully functional
            UNAUTHORIZED,     // SDK is no longer permissioned
            RECOMMEND_UPDATE, // SDK should be upgraded
            REQUIRES_UPDATE,  // SDK must be upgraded
            FORCED_SHUTDOWN,  // SDK is being forced to shutdown due to error
            SDKALGO_CONNECTED,  // SDK Algo support is connected and ready
            EDGE_NOT_CONNECTED,  // SDK could not connect to the edge for synthetic and position data
            SDKALGO_NOT_CONNECTED,  // SDK could not connect to the edge for SDK Algo support
            DOWN,                // SDK is down
            INVALID_PRICE_CONFIG,     // Invalid Price Config file was downloaded
            ORDER_THROTTLE_LIMIT_EXCEEDED,  // Number of orders sent in 1 second exceeded the limit
        };

        IEventHandler() {};
        virtual ~IEventHandler() {};
        virtual void OnStatus(const Status status) = 0;
        virtual void OnAccountStatus(const AccountConnectionStatus& connectionStatus) {};
        virtual void OnInstrumentUpdated(InstrumentPtr /*oldInstrument*/, InstrumentPtr /*newInstrument*/) {};
        virtual void OnInstrumentDeleted(InstrumentPtr /*deletedInstrument*/) {};
    };
    using IEventHandlerPtr = IEventHandler*;


    //! \brief Initialize the TT CoreSDK shared library
    //! \details The initialize function is a required call to setup the CoreSDK shared
    //!          library according to the values within the provided configuration file.
    //!          Once an application initializes the shared library, all other applications
    //!          will use the same instance, so any subsequent calls will be a no-op.
    //!          Note the handler object to receive notifications about sdk events and the
    //!          order book event handler must have a lifespan that is valid for the life 
    //!          of the SDK (until shutdown is completed).
    bool Initialize(const TTSDKOptions& options, IEventHandlerPtr sdkEventObserver, 
                    IOrderBookEventHandlerPtr orderHandler, ISDKAlgoManagerPtr algoManager = nullptr) noexcept;

    //! \brief Shutdown the TT CoreSDK shared library
    //! \details Invoke a graceful shutdown for the library.  Note that once called, any
    //!          subsequent library calls will result in undefined behavior.
    void Shutdown() noexcept;

    //! \brief Returns the version of the SDK
    //! \details Version is returned as a string in the format of major.minor.build.revision
    const char* Version() noexcept;

    //----------------------------------------------------------------------------

    typedef enum
    {
        LogDebug = 0,
        LogInfo = 10,
        LogWarning = 20,
        LogError = 30,
        LogCritical = 40,
        LogAlways = 100      //  this level is used for messages which must be logged regardless of the minimal logging level
    } LogLevel;

#ifdef _WINDOWS
    //! \brief Logs an info level message to the core sdk log file.
    //! \details Formats the arguments into the format statement and logs the resulting string
    //           in the TT log file as an INFO level message.
    void TTLogInfo(const char *format, ...);
    //! \brief Logs a warning level message to the core sdk log file.
    //! \details Formats the arguments into the format statement and logs the resulting string
    //           in the TT log file as an WARNING level message.
    void TTLogWarning(const char *format, ...);
    //! \brief Logs an error level message to the core sdk log file.
    //! \details Formats the arguments into the format statement and logs the resulting string
    //           in the TT log file as an ERROR level message.
    void TTLogError(const char *format, ...);
    //! \brief Logs an given level message to the core sdk log file.
    //! \details Formats the arguments into the format statement and logs the resulting string
    //           in the TT log file as the specified level message.
    void TTLog(const LogLevel level, const char* category, const char* msg);
#else
    void TTLogInfo(const char *format, ...) __attribute__((format(printf, 1, 2)));
    void TTLogWarning(const char *format, ...) __attribute__((format(printf, 1, 2)));
    void TTLogError(const char *format, ...) __attribute__((format(printf, 1, 2))); 
    void TTLog(const LogLevel level, const char* category, const char *msg) ;
#endif
    //-----------------------------------------------------------------------------------

    //! \brief Returns the user accounts
    //! \details Returns an object through which the user accounts can be iterated and searched
    AccountCollectionPtr GetAccounts();

    //! \brief Gets the instrument definition (active only).
    //! \details Returns an instrument for the given id. Returns nullptr if not found.
    InstrumentPtr GetInstrument(const uint64_t instrumentId, Instrument::ResponseCode& code);

    //! \brief Gets the instrument definition (active only).
    //! \details Returns an instrument for the given market, product, type and alias. Returns nullptr if not found.
    InstrumentPtr GetInstrument(const MarketId market, const char* product, const ProductType type, const char* alias, Instrument::ResponseCode& code);

    //! \brief Searches for instrument (active only).
    //! \details Returns a object contains the results of the instrument search. Markets
    //!          is an comma delimited, no spaces list of market ids within which to search. Pass
    //!          an empty string if market is not to be specified
    //!          Max of 100 results are returned.
    InstrumentSearchResultsPtr SearchInstruments(const char* query, const char* markets, Instrument::ResponseCode& code);


    OrderPtr CreateOrderFromPriceSub(uint64_t priceSubscriptionId);
    OrderPtr CreateOrder(InstrumentPtr instrument);
    OrderPtr CreateOrder(uint64_t instrumentId);
    
    bool DownloadFills(IFillDownloadCallbackHandlerPtr obj,
                       const uint64_t accountId = 0,
                       const ttsdk::MarketId marketId = ttsdk::MarketId::INVALID,
                       const uint64_t minTimestamp = 0,
                       const uint64_t maxTimestamp = 0,
                       const char* orderId = nullptr);


    uint64_t SubscribePrices(InstrumentPtr instrument, const uint16_t depth,
                             const bool implieds, const bool allowTTImplieds,
                             const bool includeSimPrices,
                             IPriceEventHandlerPtr listener) noexcept;
    uint64_t SubscribePrices(uint64_t instrumentId, const uint16_t depth,
                             const bool implieds, const bool allowTTImplieds,
                             const bool includeSimPrices,
                             IPriceEventHandlerPtr listener) noexcept;
    void UnsubscribePrices(const uint64_t subscriptionId) noexcept;

    uint64_t SubscribeTimeAndSales(InstrumentPtr instrument, const bool includeSimPrices,
                                   ITimeSalesEventHandlerPtr listener) noexcept;
    uint64_t SubscribeTimeAndSales(uint64_t instrumentId, const bool includeSimPrices,
                                    ITimeSalesEventHandlerPtr listener) noexcept;
    void UnsubscribeTimeAndSales(const uint64_t subscriptionId) noexcept;

    //! \brief starts orders,fills and positions for the given account.
    //! \details Used in conjunction with the filter_accounts flag in the SDK options,
    //! this method starts the order/fill/positions subscriptions for the given account. If
    //! filter_accounts is false, this method has not affect.
    void StartAccount(const uint64_t accountId);


    // Position Reserve Order support 
    //! \brief Reserves a quantity of risk for the given instrumentId/accountId/side to be used to by this app to avoid the quantity checks in bouncer
    //!        when an order is sent.
    //!        maxClipSize indicates the max lot size for orders sent. This value can not exceed any max order qty values defined in TT Setup 
    bool ReserveRisk(InstrumentPtr instrument, const uint64_t accountId, const RiskSide side, 
                     const double quantity, const double maxClipSize) noexcept;
    //! \brief Releases the previously allocated quantity of risk for the given instrumentId/accountId/side.
    bool ReleaseRisk(InstrumentPtr instrument, const uint64_t accountId, const RiskSide side) noexcept;
    //! \brief Returns the current position reserve bucket for the instrument and account
    PositionReserveBucket GetRiskBucket(InstrumentPtr instrument, const uint64_t accountId) noexcept;



}
