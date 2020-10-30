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
* Generated from messaging\pds\pds_enums.proto
* Package tt.messaging.pds.enums
*                                                                           
***************************************************************************/
#pragma once

namespace ttsdk
{
    enum class ExchSupportedOTCTypes
    {
        NotSet                        = -2,
        Unknown                       = -1,
        ExchSupportedOtcNone          = 0,
        ExchSupportedOtcBlock         = 6,
        ExchSupportedOtcCross         = 7,
        ExchSupportedOtcCommitted     = 22,
        ExchSupportedOtcBasis         = 23,
        ExchSupportedOtcGcross        = 24,
        ExchSupportedOtcAssetalloc    = 25,
        ExchSupportedOtcProf          = 26,
        ExchSupportedOtcOnesided      = 27,
        ExchSupportedOtcCombination   = 28,
        ExchSupportedOtcAgainstactual = 29,
        ExchSupportedOtcVola          = 30,
        ExchSupportedOtcEfp           = 31,
        ExchSupportedOtcEfpi          = 32,
        ExchSupportedOtcEfpfi         = 33,
        ExchSupportedOtcEfs           = 34,
        ExchSupportedOtcFlexfut       = 35,
        ExchSupportedOtcFlexopt       = 36,
        ExchSupportedOtcOther         = 50,
    };

    inline const char* ToString(const ExchSupportedOTCTypes value)
    {
        switch (value)
        {
        case ExchSupportedOTCTypes::NotSet:
            return "NotSet";
        case ExchSupportedOTCTypes::Unknown:
            return "Unknown";
        case ExchSupportedOTCTypes::ExchSupportedOtcNone:
            return "ExchSupportedOtcNone";
        case ExchSupportedOTCTypes::ExchSupportedOtcBlock:
            return "ExchSupportedOtcBlock";
        case ExchSupportedOTCTypes::ExchSupportedOtcCross:
            return "ExchSupportedOtcCross";
        case ExchSupportedOTCTypes::ExchSupportedOtcCommitted:
            return "ExchSupportedOtcCommitted";
        case ExchSupportedOTCTypes::ExchSupportedOtcBasis:
            return "ExchSupportedOtcBasis";
        case ExchSupportedOTCTypes::ExchSupportedOtcGcross:
            return "ExchSupportedOtcGcross";
        case ExchSupportedOTCTypes::ExchSupportedOtcAssetalloc:
            return "ExchSupportedOtcAssetalloc";
        case ExchSupportedOTCTypes::ExchSupportedOtcProf:
            return "ExchSupportedOtcProf";
        case ExchSupportedOTCTypes::ExchSupportedOtcOnesided:
            return "ExchSupportedOtcOnesided";
        case ExchSupportedOTCTypes::ExchSupportedOtcCombination:
            return "ExchSupportedOtcCombination";
        case ExchSupportedOTCTypes::ExchSupportedOtcAgainstactual:
            return "ExchSupportedOtcAgainstactual";
        case ExchSupportedOTCTypes::ExchSupportedOtcVola:
            return "ExchSupportedOtcVola";
        case ExchSupportedOTCTypes::ExchSupportedOtcEfp:
            return "ExchSupportedOtcEfp";
        case ExchSupportedOTCTypes::ExchSupportedOtcEfpi:
            return "ExchSupportedOtcEfpi";
        case ExchSupportedOTCTypes::ExchSupportedOtcEfpfi:
            return "ExchSupportedOtcEfpfi";
        case ExchSupportedOTCTypes::ExchSupportedOtcEfs:
            return "ExchSupportedOtcEfs";
        case ExchSupportedOTCTypes::ExchSupportedOtcFlexfut:
            return "ExchSupportedOtcFlexfut";
        case ExchSupportedOTCTypes::ExchSupportedOtcFlexopt:
            return "ExchSupportedOtcFlexopt";
        case ExchSupportedOTCTypes::ExchSupportedOtcOther:
            return "ExchSupportedOtcOther";
        default:
            return "Unknown";
        }
    }
}
