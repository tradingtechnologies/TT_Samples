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
****************************************************************************/

#pragma once

namespace ttsdk
{
    enum class AlgoResponseCode
    {
        Ok                    = 0,
        UnknownSymbol         = 1,
        QtyNotSet             = 2,
        PriceNotSet           = 3,
        TypeNotSet            = 4,
        TIFNotSet             = 5,
        SideNotSet            = 6,
        AccountNotSet         = 7,
        MissingParameters     = 8,
        InvalidParameters     = 9,
        UnknownOrder          = 10,
        DuplicateOrder        = 11,
        RiskRejectReceived    = 12,
        InvalidAlgoInstrument = 13,
        RoutingError          = 14,
        AlgoNotWorking        = 15,
        InvalidPrice          = 16,
        InvalidQty            = 17,
        UserNotAuthorized     = 18,
        PendingAction         = 19,
        NotSupportedAction    = 20,
        InvalidRequest        = 21,
        PriceSubFailed        = 22,
        Other                 = 23,
        AlgoRecoveryDisabled  = 24,
    };

    inline const char* ToString(const AlgoResponseCode value)
    {
        switch (value)
        {
        case AlgoResponseCode::Ok:
            return "NotSet";
        case AlgoResponseCode::UnknownSymbol:
            return "UnknownSymbol";
        case AlgoResponseCode::QtyNotSet:
            return "QtyNotSet";
        case AlgoResponseCode::PriceNotSet:
            return "PriceNotSet";
        case AlgoResponseCode::TypeNotSet:
            return "TypeNotSet";
        case AlgoResponseCode::TIFNotSet:
            return "TIFNotSet";
        case AlgoResponseCode::SideNotSet:
            return "SideNotSet";
        case AlgoResponseCode::AccountNotSet:
            return "AccountNotSet";
        case AlgoResponseCode::MissingParameters:
            return "MissingParameters";
        case AlgoResponseCode::InvalidParameters:
            return "InvalidParameters";
        case AlgoResponseCode::UnknownOrder:
            return "UnknownOrder";
        case AlgoResponseCode::DuplicateOrder:
            return "DuplicateOrder";
        case AlgoResponseCode::RiskRejectReceived:
            return "RiskRejectReceived";
        case AlgoResponseCode::InvalidAlgoInstrument:
            return "InvalidAlgoInstrument";
        case AlgoResponseCode::RoutingError:
            return "RoutingError";
        case AlgoResponseCode::AlgoNotWorking:
            return "AlgoNotWorking";
        case AlgoResponseCode::InvalidPrice:
            return "InvalidPrice";
        case AlgoResponseCode::InvalidQty:
            return "InvalidQty";
        case AlgoResponseCode::UserNotAuthorized:
            return "UserNotAuthorized";
        case AlgoResponseCode::NotSupportedAction:
            return "NotSupportedAction";
        case AlgoResponseCode::InvalidRequest:
            return "InvalidRequest";
        case AlgoResponseCode::Other:
            return "Other";
        default:
            return "Unknown";
        }
    }
}
