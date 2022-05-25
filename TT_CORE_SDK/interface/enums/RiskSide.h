/***************************************************************************
*                                                                           
*                  Unpublished Work Copyright (c) 2019-2021                 
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
#include "OrderSide.h"

namespace ttsdk
{
    enum class RiskSide
    { 
        NotSet  = 0,
        Buy     = 1,
        Sell    = 2,
    };

    inline const char* ToString(const RiskSide value)
    {
        switch (value)
        {
        case RiskSide::Buy:
            return "Buy";
        case RiskSide::Sell:
            return "Sell";
        default:
            return "NotSet";
        }
    }

    inline RiskSide ToRiskSide(const OrderSide value)
    {
        switch (value)
        {
        case OrderSide::Buy:
        case OrderSide::BuyMinus:
            return RiskSide::Buy;
        case OrderSide::Sell:
        case OrderSide::SellPlus:
        case OrderSide::SellShort:
        case OrderSide::SellShortExempt:
            return RiskSide::Sell;
        default:
            return RiskSide::NotSet;
        }
    }
}
