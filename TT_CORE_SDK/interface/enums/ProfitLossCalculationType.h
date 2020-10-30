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
****************************************************************************/
#pragma once

namespace ttsdk
{
    enum class ProfitLossCalculationType
    {
        DirectBidOrAskPrice,
        OpeningPrice,
        ClosingPrice,
        SettlementPrice,
        RiskWaterfall,
        Last,
        Midpoint
    };

    inline const char* ToString(const ProfitLossCalculationType value)
    {
        switch (value)
        {
        case ProfitLossCalculationType::DirectBidOrAskPrice:
            return "DirectBidOrAskPrice";
        case ProfitLossCalculationType::OpeningPrice:
            return "OpeningPrice";
        case ProfitLossCalculationType::ClosingPrice:
            return "ClosingPrice";
        case ProfitLossCalculationType::SettlementPrice:
            return "SettlementPrice";
        case ProfitLossCalculationType::RiskWaterfall:
            return "RiskWaterfall";
        case ProfitLossCalculationType::Last:
            return "Last";
        case ProfitLossCalculationType::Midpoint:
            return "Midpoint";
        default:
            return "Unknown";
        }
    }
}
