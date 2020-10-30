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

#include "execution_report.h"
#include "enums/CxlRejReason.h"

namespace ttsdk
{
    class RejectResponse : public shared_base
    {
    public:
        explicit RejectResponse() {};
        ~RejectResponse() noexcept {};

        virtual bool IsExternalAction() const noexcept = 0;
        //!@{ Order status
        virtual const char* GetOrderId() const noexcept = 0;
        virtual uint64_t GetRequestId() const noexcept = 0;
        virtual ttsdk::OrdStatus GetOrderStatus() const noexcept = 0;
        virtual ttsdk::OrderType GetOrderType() const noexcept = 0;
        virtual ttsdk::OrderSide GetSide() const noexcept = 0;
        virtual ttsdk::TimeInForce GetTimeInForce() const noexcept = 0;
         //!@}

        //!@{ User information
        virtual uint64_t GetUserId() const noexcept = 0;
        virtual uint64_t GetCurrentUserId() const noexcept = 0;
        virtual uint64_t GetAccountId() const noexcept = 0;
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
        virtual ttsdk::CxlRejReason GetRejectReason() const noexcept = 0;
        virtual void GetExtendedRejectInfo(ttsdk::ExtendedRejectionInformation& info) const noexcept = 0;
        virtual const char* GetText() const noexcept = 0;
        //!@}


    private:
        RejectResponse (const RejectResponse&) = default;
        RejectResponse& operator= (const RejectResponse&) = default;
        RejectResponse (RejectResponse&&) = default;
        RejectResponse& operator= (RejectResponse&&) = default;
    };


    using RejectResponsePtr = shared_ptr<const RejectResponse>;


}
