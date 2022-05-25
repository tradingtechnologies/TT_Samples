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
    enum class OrderRejectReason
    {
        NotSet                                            = -1,
        Unknown                                           = 0,
        Broker                                            = 1,
        UnknownSymbol                                     = 2,
        InvalidInvestorId                                 = 3,
        UnsupportedOrderCharacteristic                    = 4,
        SurveillenceOption                                = 5,
        IncorrectQuantity                                 = 6,
        IncorrectAllocatedQuantity                        = 7,
        UnknownAccount                                    = 8,
        ExchangeClosed                                    = 9,
        OrderExceedsLimit                                 = 10,
        TooLateToEnter                                    = 11,
        UnknownOrder                                      = 12,
        DuplicateOrder                                    = 13,
        DuplicateOfAVerballyCommunicatedOrder             = 14,
        StaleOrder                                        = 15,
        TradeAlongRequired                                = 16,
        Other                                             = 17,
        InvalidPriceIncrement                             = 18,
        PriceExceedsCurrentPriceBand                      = 19,
        MessagePending                                    = 20,
        RoutingError                                      = 21,
        OrderRetransmitted                                = 22,
        TooLateToCancel                                   = 23,
        OrderAlreadyInPendingCancelOrPendingReplaceStatus = 24,
        UnableToProcessOrderMassCancelRequest             = 25,
        Origordmodtime                                    = 26,
        DuplicateClordid                                  = 27,
        PriceExceedsCurrentPrice                          = 28,
        UnableToReduceForInternalization                  = 29,
        OrderExceedsThrottleLimit                         = 30,
        CannotForceDeleteUnknownOrder                     = 31,
        MarketClosed                                      = 32,
        FixFieldMissingOrIncorrect                        = 33,
        RequiredFieldMissing                              = 34,
        FixFieldIncorrect                                 = 35,
        PriceMustBeGreaterThanZero                        = 36,
        InvalidOrderQualifier                             = 37,
        UserNotAuthorized                                 = 38,
        MarketOrdersNotSupportedByOpposite                = 39,
        InvalidExpireDate                                 = 40,
        OrderNotInBook                                    = 41,
        OrderNotInBook2                                   = 42,
        DisclosedQtyCannotBeGreater                       = 43,
        UnknownContract                                   = 44,
        CancelWithDifferentSenderCompId                   = 45,
        ClordidDifferentThanCorrelationclordid            = 46,
        ClordidDifferentThanOriginalclordid               = 47,
        DifferentSide                                     = 48,
        DifferentGroup                                    = 49,
        DifferentSecurityType                             = 50,
        DifferentAccount                                  = 51,
        DifferentQty                                      = 52,
        CancelWithDifferentTraderId                       = 53,
        StopPriceMustBeGreater                            = 54,
        StopPriceMustBeSmaller                            = 55,
        SellStopPriceMustBeBelowLtp                       = 56,
        BuyStopPriceMustBeAboveLtp                        = 57,
        DifferentProduct                                  = 58,
        DifferentInflightFillMitigation                   = 59,
        ModifyWithDifferentSenderCompId                   = 60,
        ModifyWithDifferentTraderId                       = 61,
        OrderQtyOutsideAllowableRange                     = 62,
        InvalidOrderTypeForPcp                            = 63,
        OrderPriceOutsideLimits                           = 64,
        OrderPriceOutsideBands                            = 65,
        InvalidOrderTypeForGroup                          = 66,
        InstrumentCrossRequestInProgress                  = 67,
        OrderQtyTooLow                                    = 68,
        OrderRejected                                     = 69,
        MarketInNoCancel                                  = 70,
        InvalidOrderTypeForReservedMarket                 = 71,
        OrderSessionDateInPast                            = 72,
        DisclosedQtyCannotBeSmaller                       = 73,
        MarketMakerProtectionHasTripped                   = 74,
        EngineDidNotRespond                               = 75,
        ContractNotGtcGtdEligible                         = 76,
        ContractPastExpiration                            = 77,
        MaxContractWorkingQtyExceeded                     = 78,
        ModifyWithDifferentSide                           = 79,
        ContractNotGtcGtdEligible2                        = 80,
        NoTradingCalendarForExpireDate                    = 81,
        ExpireDateBeyondInstrumentExpiration              = 82,
        ExpireDateBeyondLegInstrumentExpiration           = 83,
        TechnicalErrorFunctionNotPerformed                = 84,
        DuplicateQuote                                    = 85,
        PendingReplace                                    = 91,
        PendingCancel                                     = 92,
        CleanupPendingAction                              = 93,
        PendingOrderRejectedDueToResend                   = 94,
        OrderStagedOrderCancelRejectedByClaimant          = 95,
        OrderStagedOrderReplaceRejectedByClaimant         = 96,
        OrderStaleStagedOrderReplaceAcceptedByClaimant    = 97,
        InvalidRequest                                    = 98,
        BeforeSendHandlerReject                           = 99,
        ActionNotSupported                                = 100,
        SendError                                         = 101,
        AotcCrossesOrderInSameAccount                     = 102,
        PreventDuplicateOrder                             = 103,
        NoPriceSubForDeviationCalc                        = 104,
        ServerOverload                                    = 105,
        PendingNew                                        = 106,
        ReviewStatusInvalidTransition                     = 107,
        OrderTimedOut                                     = 108,
        ApiNotReady                                       = 200,
    };

    inline const char* ToString(const OrderRejectReason value)
    {
        switch (value)
        {
        case OrderRejectReason::NotSet:
            return "NotSet";
        case OrderRejectReason::Unknown:
            return "Unknown";
        case OrderRejectReason::Broker:
            return "Broker";
        case OrderRejectReason::UnknownSymbol:
            return "UnknownSymbol";
        case OrderRejectReason::InvalidInvestorId:
            return "InvalidInvestorId";
        case OrderRejectReason::UnsupportedOrderCharacteristic:
            return "UnsupportedOrderCharacteristic";
        case OrderRejectReason::SurveillenceOption:
            return "SurveillenceOption";
        case OrderRejectReason::IncorrectQuantity:
            return "IncorrectQuantity";
        case OrderRejectReason::IncorrectAllocatedQuantity:
            return "IncorrectAllocatedQuantity";
        case OrderRejectReason::UnknownAccount:
            return "UnknownAccount";
        case OrderRejectReason::ExchangeClosed:
            return "ExchangeClosed";
        case OrderRejectReason::OrderExceedsLimit:
            return "OrderExceedsLimit";
        case OrderRejectReason::TooLateToEnter:
            return "TooLateToEnter";
        case OrderRejectReason::UnknownOrder:
            return "UnknownOrder";
        case OrderRejectReason::DuplicateOrder:
            return "DuplicateOrder";
        case OrderRejectReason::DuplicateOfAVerballyCommunicatedOrder:
            return "DuplicateOfAVerballyCommunicatedOrder";
        case OrderRejectReason::StaleOrder:
            return "StaleOrder";
        case OrderRejectReason::TradeAlongRequired:
            return "TradeAlongRequired";
        case OrderRejectReason::Other:
            return "Other";
        case OrderRejectReason::InvalidPriceIncrement:
            return "InvalidPriceIncrement";
        case OrderRejectReason::PriceExceedsCurrentPriceBand:
            return "PriceExceedsCurrentPriceBand";
        case OrderRejectReason::MessagePending:
            return "MessagePending";
        case OrderRejectReason::RoutingError:
            return "RoutingError";
        case OrderRejectReason::OrderRetransmitted:
            return "OrderRetransmitted";
        case OrderRejectReason::TooLateToCancel:
            return "TooLateToCancel";
        case OrderRejectReason::OrderAlreadyInPendingCancelOrPendingReplaceStatus:
            return "OrderAlreadyInPendingCancelOrPendingReplaceStatus";
        case OrderRejectReason::UnableToProcessOrderMassCancelRequest:
            return "UnableToProcessOrderMassCancelRequest";
        case OrderRejectReason::Origordmodtime:
            return "Origordmodtime";
        case OrderRejectReason::DuplicateClordid:
            return "DuplicateClordid";
        case OrderRejectReason::PriceExceedsCurrentPrice:
            return "PriceExceedsCurrentPrice";
        case OrderRejectReason::UnableToReduceForInternalization:
            return "UnableToReduceForInternalization";
        case OrderRejectReason::OrderExceedsThrottleLimit:
            return "OrderExceedsThrottleLimit";
        case OrderRejectReason::CannotForceDeleteUnknownOrder:
            return "CannotForceDeleteUnknownOrder";
        case OrderRejectReason::MarketClosed:
            return "MarketClosed";
        case OrderRejectReason::FixFieldMissingOrIncorrect:
            return "FixFieldMissingOrIncorrect";
        case OrderRejectReason::RequiredFieldMissing:
            return "RequiredFieldMissing";
        case OrderRejectReason::FixFieldIncorrect:
            return "FixFieldIncorrect";
        case OrderRejectReason::PriceMustBeGreaterThanZero:
            return "PriceMustBeGreaterThanZero";
        case OrderRejectReason::InvalidOrderQualifier:
            return "InvalidOrderQualifier";
        case OrderRejectReason::UserNotAuthorized:
            return "UserNotAuthorized";
        case OrderRejectReason::MarketOrdersNotSupportedByOpposite:
            return "MarketOrdersNotSupportedByOpposite";
        case OrderRejectReason::InvalidExpireDate:
            return "InvalidExpireDate";
        case OrderRejectReason::OrderNotInBook:
            return "OrderNotInBook";
        case OrderRejectReason::OrderNotInBook2:
            return "OrderNotInBook2";
        case OrderRejectReason::DisclosedQtyCannotBeGreater:
            return "DisclosedQtyCannotBeGreater";
        case OrderRejectReason::UnknownContract:
            return "UnknownContract";
        case OrderRejectReason::CancelWithDifferentSenderCompId:
            return "CancelWithDifferentSenderCompId";
        case OrderRejectReason::ClordidDifferentThanCorrelationclordid:
            return "ClordidDifferentThanCorrelationclordid";
        case OrderRejectReason::ClordidDifferentThanOriginalclordid:
            return "ClordidDifferentThanOriginalclordid";
        case OrderRejectReason::DifferentSide:
            return "DifferentSide";
        case OrderRejectReason::DifferentGroup:
            return "DifferentGroup";
        case OrderRejectReason::DifferentSecurityType:
            return "DifferentSecurityType";
        case OrderRejectReason::DifferentAccount:
            return "DifferentAccount";
        case OrderRejectReason::DifferentQty:
            return "DifferentQty";
        case OrderRejectReason::CancelWithDifferentTraderId:
            return "CancelWithDifferentTraderId";
        case OrderRejectReason::StopPriceMustBeGreater:
            return "StopPriceMustBeGreater";
        case OrderRejectReason::StopPriceMustBeSmaller:
            return "StopPriceMustBeSmaller";
        case OrderRejectReason::SellStopPriceMustBeBelowLtp:
            return "SellStopPriceMustBeBelowLtp";
        case OrderRejectReason::BuyStopPriceMustBeAboveLtp:
            return "BuyStopPriceMustBeAboveLtp";
        case OrderRejectReason::DifferentProduct:
            return "DifferentProduct";
        case OrderRejectReason::DifferentInflightFillMitigation:
            return "DifferentInflightFillMitigation";
        case OrderRejectReason::ModifyWithDifferentSenderCompId:
            return "ModifyWithDifferentSenderCompId";
        case OrderRejectReason::ModifyWithDifferentTraderId:
            return "ModifyWithDifferentTraderId";
        case OrderRejectReason::OrderQtyOutsideAllowableRange:
            return "OrderQtyOutsideAllowableRange";
        case OrderRejectReason::InvalidOrderTypeForPcp:
            return "InvalidOrderTypeForPcp";
        case OrderRejectReason::OrderPriceOutsideLimits:
            return "OrderPriceOutsideLimits";
        case OrderRejectReason::OrderPriceOutsideBands:
            return "OrderPriceOutsideBands";
        case OrderRejectReason::InvalidOrderTypeForGroup:
            return "InvalidOrderTypeForGroup";
        case OrderRejectReason::InstrumentCrossRequestInProgress:
            return "InstrumentCrossRequestInProgress";
        case OrderRejectReason::OrderQtyTooLow:
            return "OrderQtyTooLow";
        case OrderRejectReason::OrderRejected:
            return "OrderRejected";
        case OrderRejectReason::MarketInNoCancel:
            return "MarketInNoCancel";
        case OrderRejectReason::InvalidOrderTypeForReservedMarket:
            return "InvalidOrderTypeForReservedMarket";
        case OrderRejectReason::OrderSessionDateInPast:
            return "OrderSessionDateInPast";
        case OrderRejectReason::DisclosedQtyCannotBeSmaller:
            return "DisclosedQtyCannotBeSmaller";
        case OrderRejectReason::MarketMakerProtectionHasTripped:
            return "MarketMakerProtectionHasTripped";
        case OrderRejectReason::EngineDidNotRespond:
            return "EngineDidNotRespond";
        case OrderRejectReason::ContractNotGtcGtdEligible:
            return "ContractNotGtcGtdEligible";
        case OrderRejectReason::ContractPastExpiration:
            return "ContractPastExpiration";
        case OrderRejectReason::MaxContractWorkingQtyExceeded:
            return "MaxContractWorkingQtyExceeded";
        case OrderRejectReason::ModifyWithDifferentSide:
            return "ModifyWithDifferentSide";
        case OrderRejectReason::ContractNotGtcGtdEligible2:
            return "ContractNotGtcGtdEligible2";
        case OrderRejectReason::NoTradingCalendarForExpireDate:
            return "NoTradingCalendarForExpireDate";
        case OrderRejectReason::ExpireDateBeyondInstrumentExpiration:
            return "ExpireDateBeyondInstrumentExpiration";
        case OrderRejectReason::ExpireDateBeyondLegInstrumentExpiration:
            return "ExpireDateBeyondLegInstrumentExpiration";
        case OrderRejectReason::TechnicalErrorFunctionNotPerformed:
            return "TechnicalErrorFunctionNotPerformed";
        case OrderRejectReason::DuplicateQuote:
            return "DuplicateQuote";
        case OrderRejectReason::PendingReplace:
            return "PendingReplace";
        case OrderRejectReason::PendingCancel:
            return "PendingCancel";
        case OrderRejectReason::CleanupPendingAction:
            return "CleanupPendingAction";
        case OrderRejectReason::PendingOrderRejectedDueToResend:
            return "PendingOrderRejectedDueToResend";
        case OrderRejectReason::OrderStagedOrderCancelRejectedByClaimant:
            return "OrderStagedOrderCancelRejectedByClaimant";
        case OrderRejectReason::OrderStagedOrderReplaceRejectedByClaimant:
            return "OrderStagedOrderReplaceRejectedByClaimant";
        case OrderRejectReason::OrderStaleStagedOrderReplaceAcceptedByClaimant:
            return "OrderStaleStagedOrderReplaceAcceptedByClaimant";
        case OrderRejectReason::InvalidRequest:
            return "InvalidRequest";
        case OrderRejectReason::BeforeSendHandlerReject:
            return "BeforeSendHandlerReject";
        case OrderRejectReason::ActionNotSupported:
            return "ActionNotSupported";
        case OrderRejectReason::SendError:
            return "SendError";
        case OrderRejectReason::AotcCrossesOrderInSameAccount:
            return "AotcCrossesOrderInSameAccount";
        case OrderRejectReason::PreventDuplicateOrder:
            return "PreventDuplicateOrder";
        case OrderRejectReason::NoPriceSubForDeviationCalc:
            return "NoPriceSubForDeviationCalc";
        case OrderRejectReason::ServerOverload:
            return "ServerOverload";
        case OrderRejectReason::PendingNew:
            return "PendingNew";
        case OrderRejectReason::ReviewStatusInvalidTransition:
            return "ReviewStatusInvalidTransition";
        case OrderRejectReason::OrderTimedOut:
            return "OrderTimedOut";
        case OrderRejectReason::ApiNotReady:
            return "ApiNotReady";
        default:
            return "Unknown";
        }
    }
}
