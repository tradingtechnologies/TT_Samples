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
#include "algo_definition.h"
#include "execution_report.h"
#include "sdkalgo_request.h"
#include "position.h"
#include "shared_ptr.h"
#include "enums/MarketId.h"
#include "enums/OrdStatus.h"
#include "enums/ExecType.h"
#include "enums/OrderRejectReason.h"
#include "enums/AlgoResponseCode.h"
#include "enums/RiskSide.h"

namespace ttsdk
{
    class SDKAlgo;
    using SDKAlgoPtr = shared_ptr<SDKAlgo>;

    //! \class ISDKAlgoManager
    //! \brief Interface for listening to sdk algo events
    class ISDKAlgoManager
    {
    public:
        virtual ~ISDKAlgoManager() noexcept = default;

        //! \brief Returns the preferred markets for this instance in a comma separated list of market ids.
        //! The returns string should not be created on the stack in the method since it needs to exist 
        //! outside the method call - it should be member or static data.         
        //! A null or empty return value indicates any market can be routed to this instance.
        //! A hard coded example for CME would be "7". For CME and ICE, "7,32"
        virtual const char* GetMarkets() = 0;

        //! \brief Returns whether or not the SDK algos created in prior instance of the application should
        //! be recovered. If true, the reserve position will be recovered and OnRecovery will be called
        //! for each SDK algo order previosuly managed by this application. If false, all reserved risk
        //! for previous SDK algos will be cancelled and the SDK algo order will be failed by the SDK.
        //! In the no recovery case, OnRecovery will be called for the SDK algo orders so any working 
        //! child orders can be attached but the SDK will fail the algo regardless of what is returned 
        //! by the application.
        //! Default is false to Fail all the SDK algo orders on restart.
        virtual bool RecoverSDKAlgos() const { return false; }


        //! \brief Callback when a working algo is downloaded on startup and need  to be recovered
        virtual void OnRecoverAlgo(SDKAlgoPtr algoOrder, SDKAlgoRequestPtr req) = 0;
        //! \brief Callback when all algos have been downloaded and OnRecoverAlgo called for each of them.
        virtual void OnRecoveryEnd() = 0;
        //! \brief Callback when the previous algos can not be downloaded and recovered.
        virtual void OnRecoveryFailed() = 0;

        //! \brief Callback to start a new algo instance.
        virtual void OnStartRequest(SDKAlgoPtr algoOrder, SDKAlgoRequestPtr req) = 0;
        //! \brief Callback to stop an existiong algo instance.
        virtual void OnStopRequest(SDKAlgoPtr algoOrder, SDKAlgoRequestPtr req) = 0;
        //! \brief Callback when an update request is made for an existing algo.
        virtual void OnUpdateRequest(SDKAlgoPtr algoOrder, SDKAlgoRequestPtr req) = 0;
        //! \brief Callback when a request to pause an algo is received. When an algo is in paused state,
        //         no fills or user response messages may be generated and set to the users.
        virtual void OnPauseRequest(SDKAlgoPtr algoOrder, SDKAlgoRequestPtr req) = 0;
        //! \brief Callback when a request to restart a paused algo is received.
        virtual void OnResumeRequest(SDKAlgoPtr algoOrder, SDKAlgoRequestPtr req) = 0;

        //! \brief Callback for an event scheduled by the user.An event is scheduled programmatically 
        //         through the ScheduleEvent() method.
        virtual void OnScheduledEvent(SDKAlgoPtr algoOrder, unsigned int eventId, void* eventUserData) = 0;


        //! \brief Callback for a reserve risk request. Orders sent before a PR is acked will be rejected.
        virtual void OnRiskReserved(SDKAlgoPtr algoOrder, const uint64_t instrumentId, const uint64_t accountId, 
            const ttsdk::RiskSide side, const AlgoResponseCode code) {};
        //! \brief Callback for a release reserve risk request.
        virtual void OnRiskReleased(SDKAlgoPtr algoOrder, const uint64_t instrumentId, const uint64_t accountId, 
            const ttsdk::RiskSide side, const AlgoResponseCode code) {};



        //! \brief Callback when an algo is finished and should no longer be used. This will be called
        //         when a stop request is successful, a full fill is sent, the algo fails or the 
        //         start request fails. The user should properly shutdown the algo and clean up
        //         the resources it is using when this event is received.
        virtual void OnAlgoCompleted(SDKAlgoPtr algoOrder) = 0;

    };
    using ISDKAlgoManagerPtr = ISDKAlgoManager*;


    //! \class SDKAlgo
    //! \brief an interface to interact with the sdk managed algo.
    //         The algo order lifespan begins with new order single request. Once that algo is running,
    //         the sdk algo will exist until either:
    //         1) a cancel request is issued by the user and the OnStopRequest request return ok
    //         2) the application fails the algo using the FailAlgo method
    //         3) the order is fully filled which is indicated by the application calling GenerateSyntheticFill
    //            and the fill qty becomes equal or greater than the algo order qty
    class SDKAlgo : public shared_base
    {
    public:
        explicit SDKAlgo() {};
        ~SDKAlgo() {};

        //! \brief Returns the unique id of this algo
        virtual const char* GetOrderId() const noexcept = 0;
        //! \brief Returns the instrument referenced by the algo. Might be null if the user did not send
        //         an instrument in the tradition manner in the algo requests.
        virtual InstrumentPtr GetInstrument() const noexcept = 0;
        //! \brief Returns the algo definition Id for the algo definition.
        virtual AlgoDefinitionPtr GetAlgoDefinition() const noexcept = 0;
        //! \brief Returns the current state of the algo.
        virtual ExecutionReportPtr GetCurrentState() const noexcept = 0;

        //! \brief Generates and sends a fill message for the given qty and price. If the fill qty exceeds the currently
        //         working qty of the algo, a full fill will be sent and the algo will be complete, triggering
        //         the ISDKAlgoManager::OnAlgoCompleted callback. Fills sent before the algo is started will be sent
        //         after the start is completed.
        virtual bool GenerateSyntheticFill(const double fillPrc, const double fillQty) noexcept = 0;
        //! \brief Generates and sends a restatement message to the user contains the given information. Note that
        //         if there is a pending action, this data will be collapsed into the pending action reqponse message.
        //         In addition, these messages are throttled so a limited number are sent per second to over constant
        //         sending of restatement messages. Throttle messages will be collapsed the lastest sent.
        virtual bool GenerateUserResponse(const char* msg, const ttsdk::UserParameter params[], const size_t numParams) noexcept = 0;

        //! \brief Sets this algo as failed and send a message to user indicating failed status. This will not remove 
        //!        the order from the order book but will indicate to the end user the algo has stopped working and they
        //!        need to cancel it.
        virtual bool FailAlgo(const char* message) noexcept = 0;
        //! \brief Sets this algo as finished and send a message to user indicating finished status.
        virtual bool StopAlgo(const char* message) noexcept = 0;

        //! \brief This needs to be called when a request from the user is completed. If the algo request action
        //!        was successful, pass AlgoResponseCode::ok otherwise pass in a code that describes the reason for failure
        virtual void OnPendingRequestCompleted(const AlgoResponseCode code, const char* message = nullptr) noexcept = 0;

        //! \brief Schedules an OnScheduleEvent callback to be triggered for this algo with the given id and user data.
        //  eventId: id to identify the event
        //  eventUserData: user provided data to be passed to the callback
        virtual void ScheduledEvent(unsigned int eventId, void* eventUserData) = 0;

        // Position Reserve Order support 

        //! \brief Reserves a quantity of risk for the given instrumentId/accountId/side to be used to by this algo to avoid the quantity checks in bouncer
        //!        when an order is sent.
        //!        Must be called before the algo places any orders on that instrument/account/side. If orders already exists for the instrument/account/side
        //!        or there already is risk allocated for it, this method will return false.
        //!        maxClipSize indicates the max lot size for orders sent. This value can not exceed any max order qty values defined in TT Setup
        virtual bool ReserveRisk(InstrumentPtr instrument, const uint64_t accountId, const RiskSide side, 
                                 const double quantity, const double maxClipSize) noexcept = 0;
        //! \brief Releases the previously allocated quantity of risk for the given instrumentId/accountId/side. 
        virtual bool ReleaseRisk(InstrumentPtr instrument, const uint64_t accountId, const RiskSide side) noexcept = 0;
        //! \brief Returns the current position reserve bucket for the instrument and account
        virtual PositionReserveBucket GetRiskBucket(InstrumentPtr instrument, const uint64_t accountId) noexcept = 0;


    private:
        SDKAlgo(const SDKAlgo&) = delete;
        SDKAlgo& operator=(SDKAlgo&) = delete;
        SDKAlgo(SDKAlgo&&) = delete;
        SDKAlgo& operator=(SDKAlgo&&) = delete;
    };


}
