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
* Generated from messaging\order\reject_reason.proto
* Package tt.messaging.order
*                                                                           
***************************************************************************/
#pragma once

namespace ttsdk
{
    enum class RejectCode
    {
        NotSet                                    = -1,
        Invalid                                   = 0,
        UnknownOrder                              = 1,
        UnknownProduct                            = 2,
        UnknownInstrument                         = 3,
        InvalidQty                                = 4,
        NotAllowed                                = 5,
        NoConnection                              = 6,
        DuplicateOrder                            = 7,
        NoLimit                                   = 8,
        ExceedsMaxOrderSize                       = 9,
        ExceedsMaxProductPosition                 = 10,
        ExceedsMaxInstrumentPosition              = 11,
        ExceedsCreditLimit                        = 12,
        IncorrectInstrumentType                   = 13,
        ExceedsLongShort                          = 14,
        TifForbidden                              = 15,
        DeleteForbidden                           = 16,
        SubmitForbidden                           = 17,
        GtcGtdForbidden                           = 18,
        MarketOrderForbidden                      = 19,
        AlgoOrderForbidden                        = 20,
        WholesaleOrderForbidden                   = 21,
        OutrightForbidden                         = 22,
        SpreadForbidden                           = 23,
        TradingDisabled                           = 24,
        ExceedsPriceReasonability                 = 25,
        AseOrderForbidden                         = 26,
        UnsupportedSecurityType                   = 27,
        InvalidPrice                              = 28,
        ManualFillSubmitForbidden                 = 29,
        MalformedRequest                          = 30,
        MobileForbidden                           = 31,
        UnsupportedMarket                         = 32,
        CrossedOrder                              = 33,
        UnsupportedRequest                        = 34,
        ExceedsReserveQty                         = 35,
        NotTradableInstrument                     = 36,
        CrossedOrderNoTtint                       = 37,
        CrossedOrderMultiLeg                      = 38,
        ChildOrderQtyChangeForbidden              = 39,
        LbmSendFailure                            = 40,
        IncorrectBrokerId                         = 41,
        InvalidCounterpartyId                     = 42,
        SubmitStagedForbidden                     = 43,
        ClaimStagedForbidden                      = 44,
        CancelReplaceStagedForbidden              = 45,
        StagedChildForbidden                      = 46,
        StagedAlreadyInPending                    = 47,
        RfqDisabled                               = 48,
        PositionNotOffSetting                     = 49,
        ProductMarginNotSet                       = 50,
        DuplicateQuote                            = 51,
        InterproductForbidden                     = 52,
        TradingDisabledAutoLiquidate              = 53,
        ExceedsMaxFamilyPosition                  = 54,
        StagedOrderCancelRejectedByClaimant       = 55,
        StagedOrderReplaceRejectedByClaimant      = 56,
        StaleStagedOrderReplaceAcceptedByClaimant = 57,
        StagedOrderNewPendingRequestArrived       = 58,
        PreventDuplicateOrder                     = 59,
        TooLateToAmend                            = 60,
        ModifyForbidden                           = 61,
        TradingDisabledMic                        = 62,
        ReviewStatusInvalidTransition             = 63,
        StagedOrderCancelRejectedChildNosUnacked  = 64,
        ClearingAccountModifyForbidden            = 65,
        OtdRequestTimedOut                        = 66,
    };

    inline const char* ToString(const RejectCode value)
    {
        switch (value)
        {
        case RejectCode::NotSet:
            return "NotSet";
        case RejectCode::Invalid:
            return "Invalid";
        case RejectCode::UnknownOrder:
            return "UnknownOrder";
        case RejectCode::UnknownProduct:
            return "UnknownProduct";
        case RejectCode::UnknownInstrument:
            return "UnknownInstrument";
        case RejectCode::InvalidQty:
            return "InvalidQty";
        case RejectCode::NotAllowed:
            return "NotAllowed";
        case RejectCode::NoConnection:
            return "NoConnection";
        case RejectCode::DuplicateOrder:
            return "DuplicateOrder";
        case RejectCode::NoLimit:
            return "NoLimit";
        case RejectCode::ExceedsMaxOrderSize:
            return "ExceedsMaxOrderSize";
        case RejectCode::ExceedsMaxProductPosition:
            return "ExceedsMaxProductPosition";
        case RejectCode::ExceedsMaxInstrumentPosition:
            return "ExceedsMaxInstrumentPosition";
        case RejectCode::ExceedsCreditLimit:
            return "ExceedsCreditLimit";
        case RejectCode::IncorrectInstrumentType:
            return "IncorrectInstrumentType";
        case RejectCode::ExceedsLongShort:
            return "ExceedsLongShort";
        case RejectCode::TifForbidden:
            return "TifForbidden";
        case RejectCode::DeleteForbidden:
            return "DeleteForbidden";
        case RejectCode::SubmitForbidden:
            return "SubmitForbidden";
        case RejectCode::GtcGtdForbidden:
            return "GtcGtdForbidden";
        case RejectCode::MarketOrderForbidden:
            return "MarketOrderForbidden";
        case RejectCode::AlgoOrderForbidden:
            return "AlgoOrderForbidden";
        case RejectCode::WholesaleOrderForbidden:
            return "WholesaleOrderForbidden";
        case RejectCode::OutrightForbidden:
            return "OutrightForbidden";
        case RejectCode::SpreadForbidden:
            return "SpreadForbidden";
        case RejectCode::TradingDisabled:
            return "TradingDisabled";
        case RejectCode::ExceedsPriceReasonability:
            return "ExceedsPriceReasonability";
        case RejectCode::AseOrderForbidden:
            return "AseOrderForbidden";
        case RejectCode::UnsupportedSecurityType:
            return "UnsupportedSecurityType";
        case RejectCode::InvalidPrice:
            return "InvalidPrice";
        case RejectCode::ManualFillSubmitForbidden:
            return "ManualFillSubmitForbidden";
        case RejectCode::MalformedRequest:
            return "MalformedRequest";
        case RejectCode::MobileForbidden:
            return "MobileForbidden";
        case RejectCode::UnsupportedMarket:
            return "UnsupportedMarket";
        case RejectCode::CrossedOrder:
            return "CrossedOrder";
        case RejectCode::UnsupportedRequest:
            return "UnsupportedRequest";
        case RejectCode::ExceedsReserveQty:
            return "ExceedsReserveQty";
        case RejectCode::NotTradableInstrument:
            return "NotTradableInstrument";
        case RejectCode::CrossedOrderNoTtint:
            return "CrossedOrderNoTtint";
        case RejectCode::CrossedOrderMultiLeg:
            return "CrossedOrderMultiLeg";
        case RejectCode::ChildOrderQtyChangeForbidden:
            return "ChildOrderQtyChangeForbidden";
        case RejectCode::LbmSendFailure:
            return "LbmSendFailure";
        case RejectCode::IncorrectBrokerId:
            return "IncorrectBrokerId";
        case RejectCode::InvalidCounterpartyId:
            return "InvalidCounterpartyId";
        case RejectCode::SubmitStagedForbidden:
            return "SubmitStagedForbidden";
        case RejectCode::ClaimStagedForbidden:
            return "ClaimStagedForbidden";
        case RejectCode::CancelReplaceStagedForbidden:
            return "CancelReplaceStagedForbidden";
        case RejectCode::StagedChildForbidden:
            return "StagedChildForbidden";
        case RejectCode::StagedAlreadyInPending:
            return "StagedAlreadyInPending";
        case RejectCode::RfqDisabled:
            return "RfqDisabled";
        case RejectCode::PositionNotOffSetting:
            return "PositionNotOffSetting";
        case RejectCode::ProductMarginNotSet:
            return "ProductMarginNotSet";
        case RejectCode::DuplicateQuote:
            return "DuplicateQuote";
        case RejectCode::InterproductForbidden:
            return "InterproductForbidden";
        case RejectCode::TradingDisabledAutoLiquidate:
            return "TradingDisabledAutoLiquidate";
        case RejectCode::ExceedsMaxFamilyPosition:
            return "ExceedsMaxFamilyPosition";
        case RejectCode::StagedOrderCancelRejectedByClaimant:
            return "StagedOrderCancelRejectedByClaimant";
        case RejectCode::StagedOrderReplaceRejectedByClaimant:
            return "StagedOrderReplaceRejectedByClaimant";
        case RejectCode::StaleStagedOrderReplaceAcceptedByClaimant:
            return "StaleStagedOrderReplaceAcceptedByClaimant";
        case RejectCode::StagedOrderNewPendingRequestArrived:
            return "StagedOrderNewPendingRequestArrived";
        case RejectCode::PreventDuplicateOrder:
            return "PreventDuplicateOrder";
        case RejectCode::TooLateToAmend:
            return "TooLateToAmend";
        case RejectCode::ModifyForbidden:
            return "ModifyForbidden";
        case RejectCode::TradingDisabledMic:
            return "TradingDisabledMic";
        case RejectCode::ReviewStatusInvalidTransition:
            return "ReviewStatusInvalidTransition";
        case RejectCode::StagedOrderCancelRejectedChildNosUnacked:
            return "StagedOrderCancelRejectedChildNosUnacked";
        case RejectCode::ClearingAccountModifyForbidden:
            return "ClearingAccountModifyForbidden";
        case RejectCode::OtdRequestTimedOut:
            return "OtdRequestTimedOut";
        default:
            return "Unknown";
        }
    }
}
