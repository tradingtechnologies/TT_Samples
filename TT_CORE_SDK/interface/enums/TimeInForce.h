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
    enum class TimeInForce
    {
        NotSet                = -1,
        Unknown               = 0,
        Day                   = 1,
        GoodTillCancel        = 2,
        AtTheOpening          = 3,
        ImmediateOrCancel     = 4,
        FillOrKill            = 5,
        GoodTillCrossing      = 6,
        GoodTillDate          = 7,
        AtTheClose            = 8,
        GoodThroughCrossing   = 9,
        AtCrossing            = 10,
        Auction               = 13,
        GoodInSession         = 14,
        DayPlus               = 15,
        GoodTillCancelPlus    = 16,
        GoodTillDatePlus      = 17,
        GoodTillTime          = 18,
        ClosingPriceCross     = 19,
        ImmediateOrCancelPlus = 20,
        FillOrKillPlus        = 21,
        MorningAtTheClose     = 22,
        AfternoonAtTheClose   = 23,
    };

    inline const char* ToString(const TimeInForce value)
    {
        switch (value)
        {
        case TimeInForce::NotSet:
            return "NotSet";
        case TimeInForce::Unknown:
            return "Unknown";
        case TimeInForce::Day:
            return "Day";
        case TimeInForce::GoodTillCancel:
            return "GoodTillCancel";
        case TimeInForce::AtTheOpening:
            return "AtTheOpening";
        case TimeInForce::ImmediateOrCancel:
            return "ImmediateOrCancel";
        case TimeInForce::FillOrKill:
            return "FillOrKill";
        case TimeInForce::GoodTillCrossing:
            return "GoodTillCrossing";
        case TimeInForce::GoodTillDate:
            return "GoodTillDate";
        case TimeInForce::AtTheClose:
            return "AtTheClose";
        case TimeInForce::GoodThroughCrossing:
            return "GoodThroughCrossing";
        case TimeInForce::AtCrossing:
            return "AtCrossing";
        case TimeInForce::Auction:
            return "Auction";
        case TimeInForce::GoodInSession:
            return "GoodInSession";
        case TimeInForce::DayPlus:
            return "DayPlus";
        case TimeInForce::GoodTillCancelPlus:
            return "GoodTillCancelPlus";
        case TimeInForce::GoodTillDatePlus:
            return "GoodTillDatePlus";
        case TimeInForce::GoodTillTime:
            return "GoodTillTime";
        case TimeInForce::ClosingPriceCross:
            return "ClosingPriceCross";
        case TimeInForce::ImmediateOrCancelPlus:
            return "ImmediateOrCancelPlus";
        case TimeInForce::FillOrKillPlus:
            return "FillOrKillPlus";
        case TimeInForce::MorningAtTheClose:
            return "MorningAtTheClose";
        case TimeInForce::AfternoonAtTheClose:
            return "AfternoonAtTheClose";
        default:
            return "Unknown";
        }
    }
}
