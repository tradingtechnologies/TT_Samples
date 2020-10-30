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
***************************************************************************/
#pragma once

namespace ttsdk
{
    enum class TradeType
    {
        NotSet  = 0,
        Take    = 16,
        Hit     = 32,
        Rest    = 64,
        Unknown = 255,
    };

    inline const char* ToString(const TradeType value)
    {
        switch (value)
        {
        case TradeType::NotSet:
            return "NotSet";
        case TradeType::Hit:
            return "Hit";
        case TradeType::Take:
            return "Take";
        case TradeType::Rest:
            return "Rest";
        default:
            return "Unknown";
        }
    }
}
