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

#include "shared_ptr.h"
#include "user_parameter.h"
#include "enums/OrderType.h"
#include "enums/OrderSide.h"
#include "enums/TimeInForce.h"
#include "enums/MarketId.h"

namespace ttsdk
{
    class Order;
    using OrderPtr = shared_ptr<Order>;

    //! \class SDKAlgoRequest
    //! \brief Inbound SDK algo request message data
    class SDKAlgoRequest : public shared_base
    {
    public:
        explicit SDKAlgoRequest() {};
        ~SDKAlgoRequest() noexcept {};

        virtual const char* GetOrderId() const noexcept = 0;
        virtual ttsdk::OrderType GetOrderType() const noexcept = 0;
        virtual ttsdk::OrderSide GetSide() const noexcept = 0;
        virtual ttsdk::TimeInForce GetTimeInForce() const noexcept = 0;
        virtual double GetPrice() const noexcept = 0;
        virtual double GetQuantity() const noexcept = 0;

        virtual uint64_t GetUserId() const noexcept = 0;
        virtual uint64_t GetCurrentUserId() const noexcept = 0;
        virtual uint64_t GetAccountId() const noexcept = 0;
        virtual const char* GetClearingAccount() const noexcept = 0;
        virtual uint64_t GetInstrumentId() const noexcept = 0;
        virtual uint64_t GetAlgoDefinitionId() const noexcept = 0;

        virtual const char* GetText() const noexcept = 0;
        virtual const char* GetTextA() const noexcept = 0;
        virtual const char* GetTextB() const noexcept = 0;
        virtual const char* GetTextC() const noexcept = 0;
        virtual const char* GetTextTT() const noexcept = 0;

        virtual uint32_t GetUserParameterCount() const noexcept = 0;
        virtual UserParameter GetUserParameter(const uint32_t index) const noexcept = 0;

        //! \brief only valid when the request is sent on for recovery
        virtual uint32_t GetChildOrderCount() const noexcept = 0;
        virtual OrderPtr GetChildOrder(const uint32_t index) const noexcept = 0;

    private:
        SDKAlgoRequest(const SDKAlgoRequest&) = default;
        SDKAlgoRequest& operator= (const SDKAlgoRequest&) = default;
        SDKAlgoRequest(SDKAlgoRequest&&) = default;
        SDKAlgoRequest& operator= (SDKAlgoRequest&&) = default;
    };


    using SDKAlgoRequestPtr = shared_ptr<const SDKAlgoRequest>;


}
