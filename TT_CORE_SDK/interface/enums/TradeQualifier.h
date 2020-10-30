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
    enum class TradeQualifier
    {
        Regular         = 0,
        ImpliedTrade    = 1,
        LegTrade        = 12,
        Auction         = 20,
        Unknown         = 255,
    };

    inline const char* ToString(const TradeQualifier value)
    {
        switch (value)
        {
        case TradeQualifier::Regular:
            return "Regular";
        case TradeQualifier::ImpliedTrade:
            return "ImpliedTrade";
        case TradeQualifier::LegTrade:
            return "LegTrade";
        case TradeQualifier::Auction:
            return "Auction";
         default:
            return "Unknown";
        }
    }
}
