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
    enum class SynthStatus
    {
        NotSet         = -2,
        Unknown        = -1,
        Starting       = 0,
        Waiting        = 1,
        PendingTrigger = 2,
        Working        = 3,
        Paused         = 4,
        Held           = 5,
        Failed         = 6,
        Canceled       = 7,
        Recovering     = 8,
        Finished       = 9,
        Pausing        = 10,
        Stopping       = 11,
        Resuming       = 12,
        RecoveryFailed = 13,
        Expired        = 14,
        Holding        = 15,
        Unmanaged      = 16,
        PendingCleanup = 17,
    };

    inline const char* ToString(const SynthStatus value)
    {
        switch (value)
        {
        case SynthStatus::NotSet:
            return "NotSet";
        case SynthStatus::Unknown:
            return "Unknown";
        case SynthStatus::Starting:
            return "Starting";
        case SynthStatus::Waiting:
            return "Waiting";
        case SynthStatus::PendingTrigger:
            return "PendingTrigger";
        case SynthStatus::Working:
            return "Working";
        case SynthStatus::Paused:
            return "Paused";
        case SynthStatus::Held:
            return "Held";
        case SynthStatus::Failed:
            return "Failed";
        case SynthStatus::Canceled:
            return "Canceled";
        case SynthStatus::Recovering:
            return "Recovering";
        case SynthStatus::Finished:
            return "Finished";
        case SynthStatus::Pausing:
            return "Pausing";
        case SynthStatus::Stopping:
            return "Stopping";
        case SynthStatus::Resuming:
            return "Resuming";
        case SynthStatus::RecoveryFailed:
            return "RecoveryFailed";
        case SynthStatus::Expired:
            return "Expired";
        case SynthStatus::Holding:
            return "Holding";
        case SynthStatus::Unmanaged:
            return "Unmanaged";
        case SynthStatus::PendingCleanup:
            return "PendingCleanup";
        default:
            return "Unknown";
        }
    }
}
