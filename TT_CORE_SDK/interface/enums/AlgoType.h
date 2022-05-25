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
    enum class AlgoType
    {
        NotSet               = -1,
        Adl                  = 0,
        Sse                  = 1,
        CustomLibrary        = 2,
        Vendor               = 3,
        TtCoreSdk            = 4,
        Liquidate            = 5,
        AutoLiquidate        = 6,
        Aggregator           = 7,
        Autospreader         = 8,
        OrderCrossPrevention = 9,
        Sniper               = 10,
        Unknown              = 11,
        Sdk                  = 12,
    };

    inline const char* ToString(const AlgoType value)
    {
        switch (value)
        {
        case AlgoType::NotSet:
            return "NotSet";
        case AlgoType::Adl:
            return "Adl";
        case AlgoType::Sse:
            return "Sse";
        case AlgoType::CustomLibrary:
            return "CustomLibrary";
        case AlgoType::Vendor:
            return "Vendor";
        case AlgoType::TtCoreSdk:
            return "TtCoreSdk";
        case AlgoType::Liquidate:
            return "Liquidate";
        case AlgoType::AutoLiquidate:
            return "AutoLiquidate";
        case AlgoType::Aggregator:
            return "Aggregator";
        case AlgoType::Autospreader:
            return "Autospreader";
        case AlgoType::OrderCrossPrevention:
            return "OrderCrossPrevention";
        case AlgoType::Sniper:
            return "Sniper";
        case AlgoType::Unknown:
            return "Unknown";
        case AlgoType::Sdk:
            return "Sdk";
        default:
            return "Unknown";
        }
    }
}
