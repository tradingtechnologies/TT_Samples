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
    enum class PriceDisplayType
    {
        NotSet                                          = -2,
        Unknown                                         = -1,
        TtPriceDisplayNormal                            = 0,
        TtPriceDisplay32                                = 1,
        TtPriceDisplayYield1                            = 2,
        TtPriceDisplayYield2                            = 3,
        TtPriceDisplayDiscount                          = 4,
        TtPriceDisplayDecimal                           = 5,
        TtPriceDisplayBasis                             = 6,
        TtPriceDisplayRoll                              = 7,
        TtPriceDisplaySpread                            = 8,
        TtPriceDisplay512th                             = 9,
        TtPriceDisplayHalves                            = 10,
        TtPriceDisplayFourths                           = 11,
        TtPriceDisplayModifiedFourths                   = 12,
        TtPriceDisplayEighths                           = 13,
        TtPriceDisplayModifiedDecimalFourths            = 14,
        TtPriceDisplayThirtySeconds                     = 15,
        TtPriceDisplaySixtyFourths                      = 16,
        TtPriceDisplayHalfThirtySeconds                 = 17,
        TtPriceDisplayQuarterThirtySeconds              = 18,
        TtPriceDisplayHalfSixtyFourths                  = 19,
        TtPriceDisplayFives                             = 20,
        TtPriceDisplayHalfPoints                        = 21,
        TtPriceDisplayFiveOneThousandths                = 22,
        TtPriceDisplayOneHundredths                     = 23,
        TtPriceDisplayTenths                            = 24,
        TtPriceDisplayTenHundredths                     = 25,
        TtPriceDisplayNoDecimals                        = 26,
        TtPriceDisplayModifiedOneFortieths              = 27,
        TtPriceDisplayEighthThirtySecondsNotImplemented = 28,
        TtPriceDisplayIslandNotImplemented              = 29,
        TtPriceDisplayBtec164                           = 30,
        TtPriceDisplayBtec28                            = 31,
        TtPriceDisplayBtec28Of132                       = 32,
        TtPriceDisplayBtec18Of132                       = 33,
        TtPriceDisplayBtec1100And18                     = 34,
        TtPriceDisplayBtecNormal                        = 35,
        TtPriceDisplayBtec14Of164                       = 36,
        TtPriceDisplayBtecWhole18Of132                  = 37,
        TtPriceDisplayQuarterThirtySeconds4x            = 38,
        TtPriceDisplayQuarterThirtySeconds2x            = 39,
        TtPriceDisplayBtecYield                         = 40,
        TtPriceDisplayHalves2x                          = 41,
        TtPriceDisplayNormal2x                          = 42,
        TtPriceDisplayNormal5x                          = 43,
        TtPriceDisplayNormal100x                        = 44,
        TtPriceDisplayFiveOneThousandths2x              = 45,
        TtPriceDisplayFiveOneThousandths5x              = 46,
        TtPriceDisplayFiveOneThousandths10x             = 47,
        TtPriceDisplayTenHundredths2x                   = 48,
        TtPriceDisplayTenHundredths5x                   = 49,
        TtPriceDisplayTenHundredths10x                  = 50,
        TtPriceDisplayQuarterOneHundredths              = 51,
        TtPriceDisplayNormal10x                         = 52,
        TtPriceDisplayNormal50x                         = 53,
        TtPriceDisplayNormal500x                        = 54,
        TtPriceDisplayNormal25x                         = 55,
        TtPriceDisplayBtecInverted                      = 56,
        TtPriceDisplay18Of132                           = 57,
        TtPriceDisplay116Of132                          = 58,
    };

    inline const char* ToString(const PriceDisplayType value)
    {
        switch (value)
        {
        case PriceDisplayType::NotSet:
            return "NotSet";
        case PriceDisplayType::Unknown:
            return "Unknown";
        case PriceDisplayType::TtPriceDisplayNormal:
            return "TtPriceDisplayNormal";
        case PriceDisplayType::TtPriceDisplay32:
            return "TtPriceDisplay32";
        case PriceDisplayType::TtPriceDisplayYield1:
            return "TtPriceDisplayYield1";
        case PriceDisplayType::TtPriceDisplayYield2:
            return "TtPriceDisplayYield2";
        case PriceDisplayType::TtPriceDisplayDiscount:
            return "TtPriceDisplayDiscount";
        case PriceDisplayType::TtPriceDisplayDecimal:
            return "TtPriceDisplayDecimal";
        case PriceDisplayType::TtPriceDisplayBasis:
            return "TtPriceDisplayBasis";
        case PriceDisplayType::TtPriceDisplayRoll:
            return "TtPriceDisplayRoll";
        case PriceDisplayType::TtPriceDisplaySpread:
            return "TtPriceDisplaySpread";
        case PriceDisplayType::TtPriceDisplay512th:
            return "TtPriceDisplay512th";
        case PriceDisplayType::TtPriceDisplayHalves:
            return "TtPriceDisplayHalves";
        case PriceDisplayType::TtPriceDisplayFourths:
            return "TtPriceDisplayFourths";
        case PriceDisplayType::TtPriceDisplayModifiedFourths:
            return "TtPriceDisplayModifiedFourths";
        case PriceDisplayType::TtPriceDisplayEighths:
            return "TtPriceDisplayEighths";
        case PriceDisplayType::TtPriceDisplayModifiedDecimalFourths:
            return "TtPriceDisplayModifiedDecimalFourths";
        case PriceDisplayType::TtPriceDisplayThirtySeconds:
            return "TtPriceDisplayThirtySeconds";
        case PriceDisplayType::TtPriceDisplaySixtyFourths:
            return "TtPriceDisplaySixtyFourths";
        case PriceDisplayType::TtPriceDisplayHalfThirtySeconds:
            return "TtPriceDisplayHalfThirtySeconds";
        case PriceDisplayType::TtPriceDisplayQuarterThirtySeconds:
            return "TtPriceDisplayQuarterThirtySeconds";
        case PriceDisplayType::TtPriceDisplayHalfSixtyFourths:
            return "TtPriceDisplayHalfSixtyFourths";
        case PriceDisplayType::TtPriceDisplayFives:
            return "TtPriceDisplayFives";
        case PriceDisplayType::TtPriceDisplayHalfPoints:
            return "TtPriceDisplayHalfPoints";
        case PriceDisplayType::TtPriceDisplayFiveOneThousandths:
            return "TtPriceDisplayFiveOneThousandths";
        case PriceDisplayType::TtPriceDisplayOneHundredths:
            return "TtPriceDisplayOneHundredths";
        case PriceDisplayType::TtPriceDisplayTenths:
            return "TtPriceDisplayTenths";
        case PriceDisplayType::TtPriceDisplayTenHundredths:
            return "TtPriceDisplayTenHundredths";
        case PriceDisplayType::TtPriceDisplayNoDecimals:
            return "TtPriceDisplayNoDecimals";
        case PriceDisplayType::TtPriceDisplayModifiedOneFortieths:
            return "TtPriceDisplayModifiedOneFortieths";
        case PriceDisplayType::TtPriceDisplayEighthThirtySecondsNotImplemented:
            return "TtPriceDisplayEighthThirtySecondsNotImplemented";
        case PriceDisplayType::TtPriceDisplayIslandNotImplemented:
            return "TtPriceDisplayIslandNotImplemented";
        case PriceDisplayType::TtPriceDisplayBtec164:
            return "TtPriceDisplayBtec164";
        case PriceDisplayType::TtPriceDisplayBtec28:
            return "TtPriceDisplayBtec28";
        case PriceDisplayType::TtPriceDisplayBtec28Of132:
            return "TtPriceDisplayBtec28Of132";
        case PriceDisplayType::TtPriceDisplayBtec18Of132:
            return "TtPriceDisplayBtec18Of132";
        case PriceDisplayType::TtPriceDisplayBtec1100And18:
            return "TtPriceDisplayBtec1100And18";
        case PriceDisplayType::TtPriceDisplayBtecNormal:
            return "TtPriceDisplayBtecNormal";
        case PriceDisplayType::TtPriceDisplayBtec14Of164:
            return "TtPriceDisplayBtec14Of164";
        case PriceDisplayType::TtPriceDisplayBtecWhole18Of132:
            return "TtPriceDisplayBtecWhole18Of132";
        case PriceDisplayType::TtPriceDisplayQuarterThirtySeconds4x:
            return "TtPriceDisplayQuarterThirtySeconds4x";
        case PriceDisplayType::TtPriceDisplayQuarterThirtySeconds2x:
            return "TtPriceDisplayQuarterThirtySeconds2x";
        case PriceDisplayType::TtPriceDisplayBtecYield:
            return "TtPriceDisplayBtecYield";
        case PriceDisplayType::TtPriceDisplayHalves2x:
            return "TtPriceDisplayHalves2x";
        case PriceDisplayType::TtPriceDisplayNormal2x:
            return "TtPriceDisplayNormal2x";
        case PriceDisplayType::TtPriceDisplayNormal5x:
            return "TtPriceDisplayNormal5x";
        case PriceDisplayType::TtPriceDisplayNormal100x:
            return "TtPriceDisplayNormal100x";
        case PriceDisplayType::TtPriceDisplayFiveOneThousandths2x:
            return "TtPriceDisplayFiveOneThousandths2x";
        case PriceDisplayType::TtPriceDisplayFiveOneThousandths5x:
            return "TtPriceDisplayFiveOneThousandths5x";
        case PriceDisplayType::TtPriceDisplayFiveOneThousandths10x:
            return "TtPriceDisplayFiveOneThousandths10x";
        case PriceDisplayType::TtPriceDisplayTenHundredths2x:
            return "TtPriceDisplayTenHundredths2x";
        case PriceDisplayType::TtPriceDisplayTenHundredths5x:
            return "TtPriceDisplayTenHundredths5x";
        case PriceDisplayType::TtPriceDisplayTenHundredths10x:
            return "TtPriceDisplayTenHundredths10x";
        case PriceDisplayType::TtPriceDisplayQuarterOneHundredths:
            return "TtPriceDisplayQuarterOneHundredths";
        case PriceDisplayType::TtPriceDisplayNormal10x:
            return "TtPriceDisplayNormal10x";
        case PriceDisplayType::TtPriceDisplayNormal50x:
            return "TtPriceDisplayNormal50x";
        case PriceDisplayType::TtPriceDisplayNormal500x:
            return "TtPriceDisplayNormal500x";
        case PriceDisplayType::TtPriceDisplayNormal25x:
            return "TtPriceDisplayNormal25x";
        case PriceDisplayType::TtPriceDisplayBtecInverted:
            return "TtPriceDisplayBtecInverted";
        case PriceDisplayType::TtPriceDisplay18Of132:
            return "TtPriceDisplay18Of132";
        case PriceDisplayType::TtPriceDisplay116Of132:
            return "TtPriceDisplay116Of132";
        default:
            return "Unknown";
        }
    }
}
