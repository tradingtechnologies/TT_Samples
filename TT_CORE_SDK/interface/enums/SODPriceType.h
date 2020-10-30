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
* Generated from messaging\juno_up\sod_price_type.proto
* Package tt.messaging.juno_up
*                                                                           
***************************************************************************/
#pragma once

namespace ttsdk
{
    enum class SODPriceType
    {
        NotSet  = -2,
        Unknown = -1,
        None    = 0,
        Settle  = 1,
        UserSet = 2,
    };

    inline const char* ToString(const SODPriceType value)
    {
        switch (value)
        {
        case SODPriceType::NotSet:
            return "NotSet";
        case SODPriceType::Unknown:
            return "Unknown";
        case SODPriceType::None:
            return "None";
        case SODPriceType::Settle:
            return "Settle";
        case SODPriceType::UserSet:
            return "UserSet";
        default:
            return "Unknown";
        }
    }
}
