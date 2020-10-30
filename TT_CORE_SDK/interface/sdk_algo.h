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

#include "consts.h"
#include "instrument.h"
#include "shared_ptr.h"
#include "enums/OrdStatus.h"
#include "enums/ExecType.h"
#include "enums/OrderRejectReason.h"

namespace ttsdk
{
    //! \struct SDKAlgoResponse
    //! \brief Definition of a response to a sdk algo request
    struct SDKAlgoResponse
    {
        OrdStatus order_status = OrdStatus::NotSet;
        ExecType exec_type = ExecType::NotSet;
        OrderRejectReason ord_rej_reason = OrderRejectReason::NotSet;
        SynthStatus synth_status = SynthStatus::NotSet;

        double price = NAN;
        double order_qty = NAN;
        double leaves_qty = NAN;
        double cum_qty = NAN;

        double last_px = NAN;
        double last_qty = NAN;
        char exec_id[128] = { 0 };

        /*
        maybe BUT we would have to expose setting/getting in the regular order API
        optional UserParameters user_parameters = 46[(visibility) = "public"];
        optional string vendor_defined_field_1 = 305[(visibility) = "public", (fixtag) = 17001];
        optional string vendor_defined_field_2 = 306[(visibility) = "public", (fixtag) = 17002];
        optional string vendor_defined_field_3 = 307[(visibility) = "public", (fixtag) = 17003];
        optional string vendor_defined_field_4 = 308[(visibility) = "public", (fixtag) = 17004];
        optional string vendor_defined_field_5 = 309[(visibility) = "public", (fixtag) = 17005];
        */
    };

    class SDKAlgo;
    using SDKAlgoPtr = shared_ptr<SDKAlgo>;

    //! \class ISDKAlgoManager
    //! \brief Interface for listening to sdk algo events
    class ISDKAlgoManager
    {
    public:
        virtual ~ISDKAlgoManager() noexcept = default;

        virtual void OnDownloadedSDKAlgo(SDKAlgoPtr order) = 0;
        virtual void OnNewOrderRequest(SDKAlgoPtr order, NewOrderSinglePtr req) = 0;
        virtual void OnChangeOrderRequest(SDKAlgoPtr order, CancelReplaceRequestPtr req) = 0;
        virtual void OnCancelOrderRequest(SDKAlgoPtr order, CancelRequestPtr req) = 0;

    };
    using ISDKAlgoManagerPtr = ISDKAlgoManager*;


    //! \class SyntheticOrder
    //! \brief an interface to interact with the synthetic order.
    class SDKAlgo : public shared_base
    {
    public:
        explicit SDKAlgo() {};
        ~SDKAlgo() {};

        virtual const char* GetOrderId() const noexcept = 0;
        virtual InstrumentPtr GetInstrument() const noexcept = 0;
        virtual ExecutionReportPtr GetCurrentState() const noexcept = 0;
        virtual void SendResponse(SDKAlgoResponse& response) noexcept = 0;
        // when PR order support is added
        // virtual void ReserveRisk() noexcept = 0;
        // virtual void ReleaseReservedRisk() noexcept = 0;

    private:
        SDKAlgo(const SDKAlgo&) = delete;
        SDKAlgo& operator=(SDKAlgo&) = delete;
        SDKAlgo(SDKAlgo&&) = delete;
        SDKAlgo& operator=(SDKAlgo&&) = delete;
    };


    void RegisterSDKAlgoManager(ISDKAlgoManager handler, const uint64_t instanceId);
}
