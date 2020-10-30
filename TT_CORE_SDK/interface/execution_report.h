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

#include "enums/RejectCode.h"
#include "enums/MultiLegReportingType.h"
#include "enums/OrdStatus.h"
#include "enums/ExecType.h"
#include "enums/OrderType.h"
#include "enums/OrderSide.h"
#include "enums/TimeInForce.h"
#include "enums/MarketId.h"
#include "enums/OrderRejectReason.h"
#include "shared_ptr.h"
#include "consts.h"
#include <math.h>

namespace ttsdk
{

    struct ExtendedRejectionInformation
    {
        struct ExceedsMaxOrderSize
        {
            double size = NAN;
            double max_size = NAN;
        };
        struct ExceedsMaxFamilyPosition
        {
             double position = NAN;
             double max_position = NAN;
        };
        struct ExceedsMaxProductPosition
        {
             double position = NAN;
             double max_position = NAN;
        };
        struct ExceedsMaxInstrumentPosition
        {
             double position = NAN;
             double max_position = NAN;
        };
        struct ExceedsLongShort
        {
             double long_short = NAN;
             double max_long_short = NAN;
        };
        struct ExceedsMaxCredit
        {
             double max_credit = NAN;
             double cost = NAN;
        };
        struct ExceedsPriceReasonability
        {
             double deviation = NAN;
             double max_deviation = NAN;
             double percent_deviation = NAN;
             double max_percent_deviation = NAN;
        };
        enum NoConnectionReason
        {
            NONE = 0, // Not set
            UNSUPPORTED_MARKET = 1, // Routing to this market is not supported
            NO_MATCHING_TYPE = 2, // No valid connection type could be found
            UNAVAILABLE = 3, // A valid connection was found, but is currently unavailable
            NO_RESPONSE_SOURCE = 4 // A valid connection was found, but source/sender doesn't exist
        };
        enum RejectIdType
        {
            REJECT_ID_NONE = 0, // Not set
            REJECT_ID_TYPE_ACCOUNT = 1,
            REJECT_ID_TYPE_USER = 2,
            REJECT_ID_TYPE_USER_GROUP = 3,
            // Reject was on an account-user relationship field. reject_id will be the
            // account id and secondary_reject_id will be the user id.
            REJECT_ID_TYPE_ACCOUNT_USER = 4
        };

        // tells what kind of reject this was
        ttsdk::RejectCode code =  ttsdk::RejectCode::NotSet;

        // these fields are set if the reject is because of the limits on a given
        // user/user group/account
        RejectIdType reject_id_type = RejectIdType::REJECT_ID_NONE;
        uint64_t reject_id = U64NAN;
        uint64_t secondary_reject_id = U64NAN;

        // Identifies the leg which violated the limit (spread contracts only).
        // This is sort of a tricky one. It is only set on contract-level leg
        // limit violations with one exception for inter-product spreads. If there
        // is a product-level limit violation for an inter-product spread, then
        // this field will be populated with the leg who's product limit was
        // violated.
        uint64_t leg_instrument_id = U64NAN;

        // these fields are set for certain codes to provide additional information
        // on the reject, the name of the field corresponds to the code name
        ExceedsMaxOrderSize exceeds_max_order_size;
        ExceedsMaxFamilyPosition exceeds_max_family_position;
        ExceedsMaxProductPosition exceeds_max_product_position;
        ExceedsMaxInstrumentPosition exceeds_max_instrument_position;
        ExceedsMaxCredit exceeds_max_credit;
        ExceedsLongShort exceeds_long_short;
        ExceedsPriceReasonability exceeds_price_reasonability;
        NoConnectionReason no_connection = NoConnectionReason::NONE;
    };



    //! \class ExecutionReport
    //! \brief Outbound order message receipt
    //! \details Conforms to the FIX specification, execution reports are delivered
    //!          in response to an outbound message.  Content can include confirmation
    //!          of the order (including changes), order status information, fill
    //!          receipts, and order rejects.  Not all "Getter" fields will be populated
    //!          for any given message, so requesting a value will be returned with a
    //!          tuple containing a boolean value representing if that field was present
    //!          on the message, and the value if present.
    class ExecutionReport : public shared_base
    {
    public:
        explicit ExecutionReport() {};
        ~ExecutionReport() noexcept {};

        virtual bool IsExternalAction() const noexcept = 0;
        virtual bool IsFill() const noexcept = 0;
        virtual bool IsFullFill() const noexcept = 0;
        virtual bool IsDelete() const noexcept = 0;
        virtual bool IsReject() const noexcept = 0;
        virtual bool IsTriggerActivated() const noexcept = 0;
        virtual bool IsWorking() const noexcept = 0;

        //!@{ Order status
        virtual const char* GetOrderId() const noexcept = 0;
        virtual uint64_t GetRequestId() const noexcept = 0;
        virtual const char* GetExecId() const noexcept = 0;
        virtual ttsdk::OrdStatus GetOrderStatus() const noexcept = 0;
        virtual ttsdk::ExecType GetExecType() const noexcept = 0;
        virtual ttsdk::OrderType GetOrderType() const noexcept = 0;
        virtual ttsdk::OrderSide GetSide() const noexcept = 0;
        virtual ttsdk::TimeInForce GetTimeInForce() const noexcept = 0;
        virtual double GetPrice() const noexcept = 0;
        virtual double GetTriggerPrice() const noexcept = 0;
        virtual double GetOrderQty() const noexcept = 0;
        virtual double GetLeavesQty() const noexcept = 0;
        virtual double GetCumQty() const noexcept = 0;
        virtual double GetDisplayQty() const noexcept = 0;
        virtual double GetRefreshQty() const noexcept = 0;
        virtual double GetMinimumQty() const noexcept = 0;
        virtual uint64_t GetOrderSequence() const noexcept = 0;
        //!@}

        //!@{ Fill information
        virtual double GetLastFillPrice() const noexcept = 0;
        virtual double GetLastFillQty() const noexcept = 0;
        virtual ttsdk::MultiLegReportingType GetMultiLegReportingType() const noexcept = 0;
        virtual uint64_t GetReceivedFromExchange() const noexcept = 0;
        //!@}

        //!@{ User information
        virtual uint64_t GetUserId() const noexcept = 0;
        virtual uint64_t GetCurrentUserId() const noexcept = 0;
        virtual uint64_t GetAccountId() const noexcept = 0;
        virtual const char* GetClearingAccount() const noexcept = 0;
        virtual uint64_t GetBrokerId() const noexcept = 0;
        virtual const char* GetSenderSubId() const noexcept = 0;
        //!@}

        //!@{ Instrument information
        virtual uint64_t GetInstrumentId() const noexcept = 0;
        virtual ttsdk::MarketId GetMarket() const noexcept = 0;
        //!@}

        //!@{ Reject details
        virtual bool IsExchangeReject() const noexcept = 0;
        virtual bool IsRiskReject() const noexcept = 0;
        virtual ttsdk::OrderRejectReason GetRejectReason() const noexcept = 0;
        virtual void GetExtendedRejectInfo(ttsdk::ExtendedRejectionInformation& info) const noexcept = 0;
        //!@}

         //!@{ Order text fields
        virtual const char* GetText() const noexcept = 0;
        virtual const char* GetTextA() const noexcept = 0;
        virtual const char* GetTextB() const noexcept = 0;
        virtual const char* GetTextC() const noexcept = 0;
        virtual const char* GetTextTT() const noexcept = 0;
        //!@}



    private:
        ExecutionReport(const ExecutionReport&) = default;
        ExecutionReport& operator= (const ExecutionReport&) = default;
        ExecutionReport(ExecutionReport&&) = default;
        ExecutionReport& operator= (ExecutionReport&&) = default;
    };


    using ExecutionReportPtr = shared_ptr<const ExecutionReport>;


}
