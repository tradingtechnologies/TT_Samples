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
    enum class UserDisconnectAction
    {
        NotSet  = -2,
        Unknown = -1,
        Leave   = 0,
        Pause   = 1,
        Cancel  = 2,
    };

    inline const char* ToString(const UserDisconnectAction value)
    {
        switch (value)
        {
        case UserDisconnectAction::NotSet:
            return "NotSet";
        case UserDisconnectAction::Unknown:
            return "Unknown";
        case UserDisconnectAction::Leave:
            return "Leave";
        case UserDisconnectAction::Pause:
            return "Pause";
        case UserDisconnectAction::Cancel:
            return "Cancel";
        default:
            return "Unknown";
        }
    }
}
