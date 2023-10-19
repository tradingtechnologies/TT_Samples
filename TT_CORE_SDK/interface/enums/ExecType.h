/***************************************************************************
*                                                                           
*                  Unpublished Work Copyright (c) 2019-2020                 
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
****************************************************************************
*                                                                           
* This file is auto generated - do not modify by hand.                      
* Run the auto generation against the message template files.               
* Generated from messaging\order\enums.proto
* Package tt.messaging.order.enums
*                                                                           
***************************************************************************/
#pragma once

namespace ttsdk
{
    enum class ExecType
    {
        NotSet                                            = -1,
        Unknown                                           = 0,
        New                                               = 1,
        DoneForDay                                        = 2,
        Canceled                                          = 3,
        Replaced                                          = 4,
        PendingCancel                                     = 5,
        Stopped                                           = 6,
        Rejected                                          = 7,
        Suspended                                         = 8,
        PendingNew                                        = 9,
        Calculated                                        = 10,
        Expired                                           = 11,
        Restated                                          = 12,
        PendingReplace                                    = 13,
        Trade                                             = 14,
        TradeCorrect                                      = 15,
        TradeCancel                                       = 16,
        OrderStatus                                       = 17,
        TradeInAClearingHold                              = 18,
        TradeHasBeenReleasedToClearing                    = 19,
        TriggeredOrActivatedBySystem                      = 20,
        PositionReserveCancelReject                       = 21,
        PositionReserveReplaceReject                      = 22,
        AlgoMessage                                       = 23,
        ExpiredOrderbook                                  = 24,
        SquareOff                                         = 25,
        ObpStateChange                                    = 26,
        StageOrderApprovalOfPendingAccepted               = 27,
        StageOrderRejectionOfPendingAccepted              = 28,
        Ordertag                                          = 29,
        CancelledByStp                                    = 30,
        OrderCancelledDueToCodMechanism                   = 31,
        OrderCancelledDueToPotentialTradeOutsideFspLimits = 32,
        OrderCancelledDueToMarketMakerProtection          = 33,
        OrderCancelledByClearingRiskManager               = 34,
        OrderCancelledDueToTradePriceValidation           = 35,
        EliminatedByCorporateEvent                        = 36,
        CancelledByMemberRiskManager                      = 37,
        OrderCancelledByMarketOperations                  = 38,
        CancelledDueToKillCommand                         = 39,
        RemainingQuantityKilled                           = 40,
        BeginningOfPakoPeriod                             = 41,
        RfqPartiallyOrFullyMatchedWithOtherCounterparts   = 42,
    };

    inline const char* ToString(const ExecType value)
    {
        switch (value)
        {
        case ExecType::NotSet:
            return "NotSet";
        case ExecType::Unknown:
            return "Unknown";
        case ExecType::New:
            return "New";
        case ExecType::DoneForDay:
            return "DoneForDay";
        case ExecType::Canceled:
            return "Canceled";
        case ExecType::Replaced:
            return "Replaced";
        case ExecType::PendingCancel:
            return "PendingCancel";
        case ExecType::Stopped:
            return "Stopped";
        case ExecType::Rejected:
            return "Rejected";
        case ExecType::Suspended:
            return "Suspended";
        case ExecType::PendingNew:
            return "PendingNew";
        case ExecType::Calculated:
            return "Calculated";
        case ExecType::Expired:
            return "Expired";
        case ExecType::Restated:
            return "Restated";
        case ExecType::PendingReplace:
            return "PendingReplace";
        case ExecType::Trade:
            return "Trade";
        case ExecType::TradeCorrect:
            return "TradeCorrect";
        case ExecType::TradeCancel:
            return "TradeCancel";
        case ExecType::OrderStatus:
            return "OrderStatus";
        case ExecType::TradeInAClearingHold:
            return "TradeInAClearingHold";
        case ExecType::TradeHasBeenReleasedToClearing:
            return "TradeHasBeenReleasedToClearing";
        case ExecType::TriggeredOrActivatedBySystem:
            return "TriggeredOrActivatedBySystem";
        case ExecType::PositionReserveCancelReject:
            return "PositionReserveCancelReject";
        case ExecType::PositionReserveReplaceReject:
            return "PositionReserveReplaceReject";
        case ExecType::AlgoMessage:
            return "AlgoMessage";
        case ExecType::ExpiredOrderbook:
            return "ExpiredOrderbook";
        case ExecType::SquareOff:
            return "SquareOff";
        case ExecType::ObpStateChange:
            return "ObpStateChange";
        case ExecType::StageOrderApprovalOfPendingAccepted:
            return "StageOrderApprovalOfPendingAccepted";
        case ExecType::StageOrderRejectionOfPendingAccepted:
            return "StageOrderRejectionOfPendingAccepted";
        case ExecType::Ordertag:
            return "Ordertag";
        case ExecType::CancelledByStp:
            return "CancelledByStp";
        case ExecType::OrderCancelledDueToCodMechanism:
            return "OrderCancelledDueToCodMechanism";
        case ExecType::OrderCancelledDueToPotentialTradeOutsideFspLimits:
            return "OrderCancelledDueToPotentialTradeOutsideFspLimits";
        case ExecType::OrderCancelledDueToMarketMakerProtection:
            return "OrderCancelledDueToMarketMakerProtection";
        case ExecType::OrderCancelledByClearingRiskManager:
            return "OrderCancelledByClearingRiskManager";
        case ExecType::OrderCancelledDueToTradePriceValidation:
            return "OrderCancelledDueToTradePriceValidation";
        case ExecType::EliminatedByCorporateEvent:
            return "EliminatedByCorporateEvent";
        case ExecType::CancelledByMemberRiskManager:
            return "CancelledByMemberRiskManager";
        case ExecType::OrderCancelledByMarketOperations:
            return "OrderCancelledByMarketOperations";
        case ExecType::CancelledDueToKillCommand:
            return "CancelledDueToKillCommand";
        case ExecType::RemainingQuantityKilled:
            return "RemainingQuantityKilled";
        case ExecType::BeginningOfPakoPeriod:
            return "BeginningOfPakoPeriod";
        case ExecType::RfqPartiallyOrFullyMatchedWithOtherCounterparts:
            return "RfqPartiallyOrFullyMatchedWithOtherCounterparts";
        default:
            return "Unknown";
        }
    }
}
