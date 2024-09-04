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
    enum class CxlRejReason
    {
        NotSet                                            = -1,
        Unknown                                           = 0,
        TooLateToCancel                                   = 1,
        UnknownOrder                                      = 2,
        Broker                                            = 3,
        OrderAlreadyInPendingCancelOrPendingReplaceStatus = 4,
        UnableToProcessOrderMassCancelRequest             = 5,
        Origordmodtime                                    = 6,
        DuplicateClordid                                  = 7,
        Other                                             = 8,
        InvalidPriceIncrement                             = 9,
        PriceExceedsCurrentPrice                          = 10,
        PriceExceedsCurrentPriceBand                      = 11,
        UnableToReduceForInternalization                  = 12,
        UnsupportedOrderCharacteristic                    = 13,
        IncorrectQuantity                                 = 14,
        IncorrectAllocatedQuantity                        = 15,
        UnknownAccount                                    = 16,
        ExchangeClosed                                    = 17,
        OrderExceedsLimit                                 = 18,
        StaleOrder                                        = 19,
        MessagePending                                    = 20,
        RoutingError                                      = 21,
        OrderRetransmitted                                = 22,
        InvalidInvestorId                                 = 23,
        SurveillenceOption                                = 24,
        OrderNotInBookLongEnough                          = 25,
        CannotForceDeleteUnknownOrder                     = 26,
        MarketClosed                                      = 27,
        FixFieldMissingOrIncorrect                        = 28,
        RequiredFieldMissing                              = 29,
        FixFieldIncorrect                                 = 30,
        PriceMustBeGreaterThanZero                        = 31,
        InvalidOrderQualifier                             = 32,
        UserNotAuthorized                                 = 33,
        MarketOrdersNotSupportedByOpposite                = 34,
        InvalidExpireDate                                 = 35,
        OrderNotInBook                                    = 36,
        OrderNotInBook2                                   = 37,
        DisclosedQtyCannotBeGreater                       = 38,
        UnknownContract                                   = 39,
        CancelWithDifferentSenderCompId                   = 40,
        ClordidDifferentThanCorrelationclordid            = 41,
        ClordidDifferentThanOriginalclordid               = 42,
        DifferentSide                                     = 43,
        DifferentGroup                                    = 44,
        DifferentSecurityType                             = 45,
        DifferentAccount                                  = 46,
        DifferentQty                                      = 47,
        CancelWithDifferentTraderId                       = 48,
        StopPriceMustBeGreater                            = 49,
        StopPriceMustBeSmaller                            = 50,
        SellStopPriceMustBeBelowLtp                       = 51,
        BuyStopPriceMustBeAboveLtp                        = 52,
        DifferentProduct                                  = 53,
        DifferentInflightFillMitigation                   = 54,
        ModifyWithDifferentSenderCompId                   = 55,
        ModifyWithDifferentTraderId                       = 56,
        OrderQtyOutsideAllowableRange                     = 57,
        InvalidOrderTypeForPcp                            = 58,
        OrderPriceOutsideLimits                           = 59,
        OrderPriceOutsideBands                            = 60,
        InvalidOrderTypeForGroup                          = 61,
        InstrumentCrossRequestInProgress                  = 62,
        OrderQtyTooLow                                    = 63,
        OrderRejected                                     = 64,
        MarketInNoCancel                                  = 65,
        InvalidOrderTypeForReservedMarket                 = 66,
        OrderSessionDateInPast                            = 67,
        DisclosedQtyCannotBeSmaller                       = 68,
        MarketMakerProtectionHasTripped                   = 69,
        EngineDidNotRespond                               = 70,
        ContractNotGtcGtdEligible                         = 71,
        ContractPastExpiration                            = 72,
        MaxContractWorkingQtyExceeded                     = 73,
        ModifyWithDifferentSide                           = 74,
        ContractNotGtcGtdEligible2                        = 75,
        NoTradingCalendarForExpireDate                    = 76,
        ExpireDateBeyondInstrumentExpiration              = 77,
        ExpireDateBeyondLegInstrumentExpiration           = 78,
        TechnicalErrorFunctionNotPerformed                = 79,
        EuronextUnknownOrder                              = 80,
        EuronextOther                                     = 81,
        CompIdProblem                                     = 82,
        LogonProblem                                      = 83,
        NoRouterForSecurityGroup                          = 84,
        RouterNotAvailableOrConnected                     = 85,
        InvalidPrice                                      = 86,
        InvalidOrdqty                                     = 87,
        InvalidOrdtype                                    = 88,
        InvalidSide                                       = 89,
        FullyFilled                                       = 90,
        PendingReplace                                    = 91,
        PendingCancel                                     = 92,
        CleanupPendingAction                              = 93,
        PendingOrderRejectedDueToResend                   = 94,
        StagedOrderCancelRejectedByClaimant               = 95,
        StagedOrderReplaceRejectedByClaimant              = 96,
        StaleStagedOrderReplaceAcceptedByClaimant         = 97,
        StagedOrderNewPendingRequestArrived               = 98,
        AotcProcessingAlreadyInProgress                   = 99,
        AotcCrossesOrderInSameAccount                     = 100,
        AotcCrossesOrderThatCannotBeCancelled             = 101,
        LbmSendFailure                                    = 102,
        NoPriceSubForDeviationCalc                        = 103,
        ServerOverload                                    = 104,
        PendingNew                                        = 105,
        OrderExceedsThrottleLimit                         = 106,
        TtsmpChangeNotSupported                           = 107,
        InvalidSecurityId                                 = 109,
        UnknownMarketSegmentId                            = 110,
        FieldMissingOrIncorrect                           = 111,
    };

    inline const char* ToString(const CxlRejReason value)
    {
        switch (value)
        {
        case CxlRejReason::NotSet:
            return "NotSet";
        case CxlRejReason::Unknown:
            return "Unknown";
        case CxlRejReason::TooLateToCancel:
            return "TooLateToCancel";
        case CxlRejReason::UnknownOrder:
            return "UnknownOrder";
        case CxlRejReason::Broker:
            return "Broker";
        case CxlRejReason::OrderAlreadyInPendingCancelOrPendingReplaceStatus:
            return "OrderAlreadyInPendingCancelOrPendingReplaceStatus";
        case CxlRejReason::UnableToProcessOrderMassCancelRequest:
            return "UnableToProcessOrderMassCancelRequest";
        case CxlRejReason::Origordmodtime:
            return "Origordmodtime";
        case CxlRejReason::DuplicateClordid:
            return "DuplicateClordid";
        case CxlRejReason::Other:
            return "Other";
        case CxlRejReason::InvalidPriceIncrement:
            return "InvalidPriceIncrement";
        case CxlRejReason::PriceExceedsCurrentPrice:
            return "PriceExceedsCurrentPrice";
        case CxlRejReason::PriceExceedsCurrentPriceBand:
            return "PriceExceedsCurrentPriceBand";
        case CxlRejReason::UnableToReduceForInternalization:
            return "UnableToReduceForInternalization";
        case CxlRejReason::UnsupportedOrderCharacteristic:
            return "UnsupportedOrderCharacteristic";
        case CxlRejReason::IncorrectQuantity:
            return "IncorrectQuantity";
        case CxlRejReason::IncorrectAllocatedQuantity:
            return "IncorrectAllocatedQuantity";
        case CxlRejReason::UnknownAccount:
            return "UnknownAccount";
        case CxlRejReason::ExchangeClosed:
            return "ExchangeClosed";
        case CxlRejReason::OrderExceedsLimit:
            return "OrderExceedsLimit";
        case CxlRejReason::StaleOrder:
            return "StaleOrder";
        case CxlRejReason::MessagePending:
            return "MessagePending";
        case CxlRejReason::RoutingError:
            return "RoutingError";
        case CxlRejReason::OrderRetransmitted:
            return "OrderRetransmitted";
        case CxlRejReason::InvalidInvestorId:
            return "InvalidInvestorId";
        case CxlRejReason::SurveillenceOption:
            return "SurveillenceOption";
        case CxlRejReason::OrderNotInBookLongEnough:
            return "OrderNotInBookLongEnough";
        case CxlRejReason::CannotForceDeleteUnknownOrder:
            return "CannotForceDeleteUnknownOrder";
        case CxlRejReason::MarketClosed:
            return "MarketClosed";
        case CxlRejReason::FixFieldMissingOrIncorrect:
            return "FixFieldMissingOrIncorrect";
        case CxlRejReason::RequiredFieldMissing:
            return "RequiredFieldMissing";
        case CxlRejReason::FixFieldIncorrect:
            return "FixFieldIncorrect";
        case CxlRejReason::PriceMustBeGreaterThanZero:
            return "PriceMustBeGreaterThanZero";
        case CxlRejReason::InvalidOrderQualifier:
            return "InvalidOrderQualifier";
        case CxlRejReason::UserNotAuthorized:
            return "UserNotAuthorized";
        case CxlRejReason::MarketOrdersNotSupportedByOpposite:
            return "MarketOrdersNotSupportedByOpposite";
        case CxlRejReason::InvalidExpireDate:
            return "InvalidExpireDate";
        case CxlRejReason::OrderNotInBook:
            return "OrderNotInBook";
        case CxlRejReason::OrderNotInBook2:
            return "OrderNotInBook2";
        case CxlRejReason::DisclosedQtyCannotBeGreater:
            return "DisclosedQtyCannotBeGreater";
        case CxlRejReason::UnknownContract:
            return "UnknownContract";
        case CxlRejReason::CancelWithDifferentSenderCompId:
            return "CancelWithDifferentSenderCompId";
        case CxlRejReason::ClordidDifferentThanCorrelationclordid:
            return "ClordidDifferentThanCorrelationclordid";
        case CxlRejReason::ClordidDifferentThanOriginalclordid:
            return "ClordidDifferentThanOriginalclordid";
        case CxlRejReason::DifferentSide:
            return "DifferentSide";
        case CxlRejReason::DifferentGroup:
            return "DifferentGroup";
        case CxlRejReason::DifferentSecurityType:
            return "DifferentSecurityType";
        case CxlRejReason::DifferentAccount:
            return "DifferentAccount";
        case CxlRejReason::DifferentQty:
            return "DifferentQty";
        case CxlRejReason::CancelWithDifferentTraderId:
            return "CancelWithDifferentTraderId";
        case CxlRejReason::StopPriceMustBeGreater:
            return "StopPriceMustBeGreater";
        case CxlRejReason::StopPriceMustBeSmaller:
            return "StopPriceMustBeSmaller";
        case CxlRejReason::SellStopPriceMustBeBelowLtp:
            return "SellStopPriceMustBeBelowLtp";
        case CxlRejReason::BuyStopPriceMustBeAboveLtp:
            return "BuyStopPriceMustBeAboveLtp";
        case CxlRejReason::DifferentProduct:
            return "DifferentProduct";
        case CxlRejReason::DifferentInflightFillMitigation:
            return "DifferentInflightFillMitigation";
        case CxlRejReason::ModifyWithDifferentSenderCompId:
            return "ModifyWithDifferentSenderCompId";
        case CxlRejReason::ModifyWithDifferentTraderId:
            return "ModifyWithDifferentTraderId";
        case CxlRejReason::OrderQtyOutsideAllowableRange:
            return "OrderQtyOutsideAllowableRange";
        case CxlRejReason::InvalidOrderTypeForPcp:
            return "InvalidOrderTypeForPcp";
        case CxlRejReason::OrderPriceOutsideLimits:
            return "OrderPriceOutsideLimits";
        case CxlRejReason::OrderPriceOutsideBands:
            return "OrderPriceOutsideBands";
        case CxlRejReason::InvalidOrderTypeForGroup:
            return "InvalidOrderTypeForGroup";
        case CxlRejReason::InstrumentCrossRequestInProgress:
            return "InstrumentCrossRequestInProgress";
        case CxlRejReason::OrderQtyTooLow:
            return "OrderQtyTooLow";
        case CxlRejReason::OrderRejected:
            return "OrderRejected";
        case CxlRejReason::MarketInNoCancel:
            return "MarketInNoCancel";
        case CxlRejReason::InvalidOrderTypeForReservedMarket:
            return "InvalidOrderTypeForReservedMarket";
        case CxlRejReason::OrderSessionDateInPast:
            return "OrderSessionDateInPast";
        case CxlRejReason::DisclosedQtyCannotBeSmaller:
            return "DisclosedQtyCannotBeSmaller";
        case CxlRejReason::MarketMakerProtectionHasTripped:
            return "MarketMakerProtectionHasTripped";
        case CxlRejReason::EngineDidNotRespond:
            return "EngineDidNotRespond";
        case CxlRejReason::ContractNotGtcGtdEligible:
            return "ContractNotGtcGtdEligible";
        case CxlRejReason::ContractPastExpiration:
            return "ContractPastExpiration";
        case CxlRejReason::MaxContractWorkingQtyExceeded:
            return "MaxContractWorkingQtyExceeded";
        case CxlRejReason::ModifyWithDifferentSide:
            return "ModifyWithDifferentSide";
        case CxlRejReason::ContractNotGtcGtdEligible2:
            return "ContractNotGtcGtdEligible2";
        case CxlRejReason::NoTradingCalendarForExpireDate:
            return "NoTradingCalendarForExpireDate";
        case CxlRejReason::ExpireDateBeyondInstrumentExpiration:
            return "ExpireDateBeyondInstrumentExpiration";
        case CxlRejReason::ExpireDateBeyondLegInstrumentExpiration:
            return "ExpireDateBeyondLegInstrumentExpiration";
        case CxlRejReason::TechnicalErrorFunctionNotPerformed:
            return "TechnicalErrorFunctionNotPerformed";
        case CxlRejReason::EuronextUnknownOrder:
            return "EuronextUnknownOrder";
        case CxlRejReason::EuronextOther:
            return "EuronextOther";
        case CxlRejReason::CompIdProblem:
            return "CompIdProblem";
        case CxlRejReason::LogonProblem:
            return "LogonProblem";
        case CxlRejReason::NoRouterForSecurityGroup:
            return "NoRouterForSecurityGroup";
        case CxlRejReason::RouterNotAvailableOrConnected:
            return "RouterNotAvailableOrConnected";
        case CxlRejReason::InvalidPrice:
            return "InvalidPrice";
        case CxlRejReason::InvalidOrdqty:
            return "InvalidOrdqty";
        case CxlRejReason::InvalidOrdtype:
            return "InvalidOrdtype";
        case CxlRejReason::InvalidSide:
            return "InvalidSide";
        case CxlRejReason::FullyFilled:
            return "FullyFilled";
        case CxlRejReason::PendingReplace:
            return "PendingReplace";
        case CxlRejReason::PendingCancel:
            return "PendingCancel";
        case CxlRejReason::CleanupPendingAction:
            return "CleanupPendingAction";
        case CxlRejReason::PendingOrderRejectedDueToResend:
            return "PendingOrderRejectedDueToResend";
        case CxlRejReason::StagedOrderCancelRejectedByClaimant:
            return "StagedOrderCancelRejectedByClaimant";
        case CxlRejReason::StagedOrderReplaceRejectedByClaimant:
            return "StagedOrderReplaceRejectedByClaimant";
        case CxlRejReason::StaleStagedOrderReplaceAcceptedByClaimant:
            return "StaleStagedOrderReplaceAcceptedByClaimant";
        case CxlRejReason::StagedOrderNewPendingRequestArrived:
            return "StagedOrderNewPendingRequestArrived";
        case CxlRejReason::AotcProcessingAlreadyInProgress:
            return "AotcProcessingAlreadyInProgress";
        case CxlRejReason::AotcCrossesOrderInSameAccount:
            return "AotcCrossesOrderInSameAccount";
        case CxlRejReason::AotcCrossesOrderThatCannotBeCancelled:
            return "AotcCrossesOrderThatCannotBeCancelled";
        case CxlRejReason::LbmSendFailure:
            return "LbmSendFailure";
        case CxlRejReason::NoPriceSubForDeviationCalc:
            return "NoPriceSubForDeviationCalc";
        case CxlRejReason::ServerOverload:
            return "ServerOverload";
        case CxlRejReason::PendingNew:
            return "PendingNew";
        case CxlRejReason::OrderExceedsThrottleLimit:
            return "OrderExceedsThrottleLimit";
        case CxlRejReason::TtsmpChangeNotSupported:
            return "TtsmpChangeNotSupported";
        case CxlRejReason::InvalidSecurityId:
            return "InvalidSecurityId";
        case CxlRejReason::UnknownMarketSegmentId:
            return "UnknownMarketSegmentId";
        case CxlRejReason::FieldMissingOrIncorrect:
            return "FieldMissingOrIncorrect";
        default:
            return "Unknown";
        }
    }
}
