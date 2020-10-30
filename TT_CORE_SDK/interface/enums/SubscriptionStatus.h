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
* Generated from messaging\subscription_status.proto
* Package tt.messaging
*                                                                           
***************************************************************************/
#pragma once
namespace core_wrapper
{
    private enum class SubscriptionStatus
    {
        NotSet                      = -1,
        Unknown                     = 0,
        Ok                          = 1,
        Duplicate                   = 2,
        NoConnectivity              = 3,
        InvalidId                   = 4,
        NoEntitlement               = 5,
        TransportFail               = 6,
        SubscriptionNotFound        = 7,
        UnknownFail                 = 8,
        MiddlewareContextFail       = 9,
        SessionLimitExceeded        = 10,
        Terminated                  = 11,
        PositionDownloadFail        = 12,
        NeedsMarketdataAgreement    = 13,
        PricesInactiveInstrument    = 14,
        PricesDeletedInstrument     = 15,
        PricesNottradableInstrument = 16,
        Closed                      = 17,
        FeatureNotEnabled           = 18,
        PricesCollision             = 19,
        PricesPartyMismatch         = 20,
        NotReady                    = 21,
        NoProxySession              = 22,
        PricesConflictingUpdate     = 23,
        Restart                     = 24,
        SubscriptionLimitExceeded   = 25,
    };
}
