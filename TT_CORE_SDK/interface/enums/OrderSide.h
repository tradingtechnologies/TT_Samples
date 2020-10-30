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
    enum class OrderSide
    {
        NotSet           = 0,
        Buy              = 1,
        Sell             = 2,
        BuyMinus         = 3,
        SellPlus         = 4,
        SellShort        = 5,
        SellShortExempt  = 6,
        Undisclosed      = 7,
        Cross            = 8,
        CrossShort       = 9,
        CrossShortExempt = 10,
        AsDefined        = 11,
        Opposite         = 12,
        Subscribe        = 13,
        Redeem           = 14,
        Lend             = 15,
        Borrow           = 16,
        Invalid          = 17,
        Held             = 18,
    };

    inline const char* ToString(const OrderSide value)
    {
        switch (value)
        {
        case OrderSide::NotSet:
            return "NotSet";
        case OrderSide::Buy:
            return "Buy";
        case OrderSide::Sell:
            return "Sell";
        case OrderSide::BuyMinus:
            return "BuyMinus";
        case OrderSide::SellPlus:
            return "SellPlus";
        case OrderSide::SellShort:
            return "SellShort";
        case OrderSide::SellShortExempt:
            return "SellShortExempt";
        case OrderSide::Undisclosed:
            return "Undisclosed";
        case OrderSide::Cross:
            return "Cross";
        case OrderSide::CrossShort:
            return "CrossShort";
        case OrderSide::CrossShortExempt:
            return "CrossShortExempt";
        case OrderSide::AsDefined:
            return "AsDefined";
        case OrderSide::Opposite:
            return "Opposite";
        case OrderSide::Subscribe:
            return "Subscribe";
        case OrderSide::Redeem:
            return "Redeem";
        case OrderSide::Lend:
            return "Lend";
        case OrderSide::Borrow:
            return "Borrow";
        case OrderSide::Invalid:
            return "Invalid";
        case OrderSide::Held:
            return "Held";
        default:
            return "Unknown";
        }
    }
}
