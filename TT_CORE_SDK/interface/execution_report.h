/***************************************************************************
 *
 *                  Unpublished Work Copyright (c) 2020
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
#include "enums/SynthStatus.h"
#include "enums/UserDisconnectAction.h"
#include "user_parameter.h"
#include "extended_rejection_information.h"
#include "shared_ptr.h"
#include "consts.h"
#include <math.h>

namespace ttsdk
{
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
        virtual uint64_t GetTransactionTime() const noexcept = 0;

        //!@{ Order status
        virtual const char* GetOrderId() const noexcept = 0;
        virtual uint64_t GetRequestId() const noexcept = 0;
        virtual const char* GetExecId() const noexcept = 0;
        virtual const char* GetUniqueExecId() const noexcept = 0;
        virtual ttsdk::OrdStatus GetOrderStatus() const noexcept = 0;
        virtual ttsdk::ExecType GetExecType() const noexcept = 0;
        virtual ttsdk::OrderType GetOrderType() const noexcept = 0;
        virtual ttsdk::OrderSide GetSide() const noexcept = 0;
        virtual ttsdk::TimeInForce GetTimeInForce() const noexcept = 0;
        virtual ttsdk::SynthStatus GetSynthStatus() const noexcept = 0;
        virtual double GetPrice() const noexcept = 0;
        virtual double GetTriggerPrice() const noexcept = 0;
        virtual double GetOrderQty() const noexcept = 0;
        virtual double GetLeavesQty() const noexcept = 0;
        virtual double GetCumQty() const noexcept = 0;
        virtual double GetDisplayQty() const noexcept = 0;
        virtual double GetRefreshQty() const noexcept = 0;
        virtual double GetMinimumQty() const noexcept = 0;
        virtual uint64_t GetOrderSequence() const noexcept = 0;
        virtual uint32_t GetClientIp() const noexcept = 0;
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
        virtual const char* GetSenderLocation() const noexcept = 0; 
        virtual const char* GetConnection() const noexcept = 0;
        //!@}
        //!@{ Prent order information
        virtual const char* GetParentOrderId() const noexcept = 0;
        virtual bool IsChildOrder() const noexcept = 0;
        //!@}

        //!@{ Instrument information
        virtual uint64_t GetInstrumentId() const noexcept = 0;
        virtual ttsdk::MarketId GetMarket() const noexcept = 0;
        //!@}
       
        virtual uint32_t GetUserParameterCount() const noexcept = 0;
        virtual ttsdk::UserParameter GetUserParameter(const uint32_t index) const noexcept = 0;

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

        //!@{ Synthetic/Algo related fields
        virtual ttsdk::MarketId GetColocation() const noexcept = 0;
        virtual ttsdk::UserDisconnectAction GetUserDisconnectAction() const noexcept = 0;
        //!@}


    private:
        ExecutionReport(const ExecutionReport&) = default;
        ExecutionReport& operator= (const ExecutionReport&) = default;
        ExecutionReport(ExecutionReport&&) = default;
        ExecutionReport& operator= (ExecutionReport&&) = default;
    };


    using ExecutionReportPtr = shared_ptr<const ExecutionReport>;


}
