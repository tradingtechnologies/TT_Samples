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
    enum class OrdStatus
    {
        NotSet             = 0,
        New                = 1,
        PartiallyFilled    = 2,
        Filled             = 3,
        DoneForDay         = 4,
        Canceled           = 5,
        PendingCancel      = 6,
        Stopped            = 7,
        Rejected           = 8,
        Suspended          = 9,
        PendingNew         = 10,
        Calculated         = 11,
        Expired            = 12,
        AcceptedForBidding = 13,
        PendingReplace     = 14,
        Unknown            = 16,
        Inactive           = 17,
        Planned            = 18,
    };

    inline const char* ToString(const OrdStatus value)
    {
        switch (value)
        {
        case OrdStatus::NotSet:
            return "NotSet";
        case OrdStatus::New:
            return "New";
        case OrdStatus::PartiallyFilled:
            return "PartiallyFilled";
        case OrdStatus::Filled:
            return "Filled";
        case OrdStatus::DoneForDay:
            return "DoneForDay";
        case OrdStatus::Canceled:
            return "Canceled";
        case OrdStatus::PendingCancel:
            return "PendingCancel";
        case OrdStatus::Stopped:
            return "Stopped";
        case OrdStatus::Rejected:
            return "Rejected";
        case OrdStatus::Suspended:
            return "Suspended";
        case OrdStatus::PendingNew:
            return "PendingNew";
        case OrdStatus::Calculated:
            return "Calculated";
        case OrdStatus::Expired:
            return "Expired";
        case OrdStatus::AcceptedForBidding:
            return "AcceptedForBidding";
        case OrdStatus::PendingReplace:
            return "PendingReplace";
        case OrdStatus::Unknown:
            return "Unknown";
        case OrdStatus::Inactive:
            return "Inactive";
        case OrdStatus::Planned:
            return "Planned";
        default:
            return "Unknown";
        }
    }
}
