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
    enum class OrderType
    {
        NotSet                           = 0,
        Market                           = 1,
        Limit                            = 2,
        Stop                             = 3,
        StopLimit                        = 4,
        MarketWithLeftOverAsLimit        = 20,
        MarketLimitMarketLeftOverAsLimit = 21,
        StopMarketToLimit                = 30,
        IfTouchedMarket                  = 31,
        IfTouchedLimit                   = 32,
        IfTouchedMarketToLimit           = 33,
        LimitPostOnly                    = 37,
        MarketCloseToday                 = 38,
        LimitCloseToday                  = 39,
        LimitReduceOnly                  = 40,
        MarketReduceOnly                 = 41,
        UserDefinedBlock                 = 42,
        Unknown                          = 100,
    };

    inline const char* ToString(const OrderType value)
    {
        switch (value)
        {
        case OrderType::NotSet:
            return "NotSet";
        case OrderType::Market:
            return "Market";
        case OrderType::Limit:
            return "Limit";
        case OrderType::Stop:
            return "Stop";
        case OrderType::StopLimit:
            return "StopLimit";
        case OrderType::MarketWithLeftOverAsLimit:
            return "MarketWithLeftOverAsLimit";
        case OrderType::MarketLimitMarketLeftOverAsLimit:
            return "MarketLimitMarketLeftOverAsLimit";
        case OrderType::StopMarketToLimit:
            return "StopMarketToLimit";
        case OrderType::IfTouchedMarket:
            return "IfTouchedMarket";
        case OrderType::IfTouchedLimit:
            return "IfTouchedLimit";
        case OrderType::IfTouchedMarketToLimit:
            return "IfTouchedMarketToLimit";
        case OrderType::LimitPostOnly:
            return "LimitPostOnly";
        case OrderType::MarketCloseToday:
            return "MarketCloseToday";
        case OrderType::LimitCloseToday:
            return "LimitCloseToday";
        case OrderType::LimitReduceOnly:
            return "LimitReduceOnly";
        case OrderType::MarketReduceOnly:
            return "MarketReduceOnly";
        case OrderType::UserDefinedBlock:
            return "UserDefinedBlock";
        case OrderType::Unknown:
            return "Unknown";
        default:
            return "Unknown";
        }
    }
}
