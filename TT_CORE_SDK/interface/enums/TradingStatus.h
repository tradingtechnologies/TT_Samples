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
* Generated from messaging\edge_server\edge_server_messages.proto
* Package tt.messaging.edge_server
*                                                                           
***************************************************************************/
#pragma once

namespace ttsdk
{
    enum class TradingStatus
    {
        NotSet                  = -1,
        Halted                  = 0,
        Suspended               = 1,
        Reserved                = 2,
        Mktclose                = 3,
        Mktopen                 = 4,
        Lateopen                = 5,
        Auction                 = 6,
        Preopen                 = 7,
        Slowbid                 = 8,
        Slowask                 = 9,
        Slowbidask              = 10,
        Gapslowbidask           = 11,
        Securitydeleted         = 12,
        Shorestriction          = 13,
        Quotingonly             = 14,
        Opendelayimbalance      = 15,
        Openingrotation         = 16,
        Fastmarket              = 17,
        Precspa                 = 18,
        Cspa                    = 19,
        Adjust                  = 20,
        Adjuston                = 21,
        Purgeorders             = 22,
        Sysmaintain             = 23,
        Clclose                 = 24,
        Clstart                 = 25,
        Openalloc               = 26,
        Preopenalloc            = 27,
        Premktact               = 28,
        Shohaltrestriction      = 29,
        Filter                  = 30,
        Shoquotingrestriction   = 31,
        Closesessionroll        = 32,
        Volatilitypause         = 33,
        Shovolatilitypause      = 34,
        Auctionopen             = 35,
        Auctionintra            = 36,
        Auctionclose            = 37,
        Atlast                  = 38,
        Auctionvola             = 39,
        Precautionaryhalt       = 40,
        Luld                    = 41,
        Auctionexp              = 42,
        Auctionvol              = 43,
        Mwcbhalt1               = 44,
        Mwcbhalt2               = 45,
        Mwcbhalt3               = 46,
        Crossing                = 47,
        Shopremktactrestriction = 48,
        Shosuspendedrestriction = 49,
        Haltorderallowed        = 50,
        Precross                = 51,
        Nocancel                = 52,
        Postclose               = 53,
        Openingiop              = 54,
        Sessionclose            = 55,
        Impliedstart            = 56,
        Impliedend              = 57,
        Uncrosscall             = 58,
        Preopennottrading       = 59,
        Auctionsurplus          = 60,
        Unknown                 = 61,
        Shocloserestriction     = 62,
        Pretrading              = 63,
        Posttrading             = 64,
    };

    inline const char* ToString(const TradingStatus value)
    {
        switch (value)
        {
        case TradingStatus::NotSet:
            return "NotSet";
        case TradingStatus::Halted:
            return "Halted";
        case TradingStatus::Suspended:
            return "Suspended";
        case TradingStatus::Reserved:
            return "Reserved";
        case TradingStatus::Mktclose:
            return "Mktclose";
        case TradingStatus::Mktopen:
            return "Mktopen";
        case TradingStatus::Lateopen:
            return "Lateopen";
        case TradingStatus::Auction:
            return "Auction";
        case TradingStatus::Preopen:
            return "Preopen";
        case TradingStatus::Slowbid:
            return "Slowbid";
        case TradingStatus::Slowask:
            return "Slowask";
        case TradingStatus::Slowbidask:
            return "Slowbidask";
        case TradingStatus::Gapslowbidask:
            return "Gapslowbidask";
        case TradingStatus::Securitydeleted:
            return "Securitydeleted";
        case TradingStatus::Shorestriction:
            return "Shorestriction";
        case TradingStatus::Quotingonly:
            return "Quotingonly";
        case TradingStatus::Opendelayimbalance:
            return "Opendelayimbalance";
        case TradingStatus::Openingrotation:
            return "Openingrotation";
        case TradingStatus::Fastmarket:
            return "Fastmarket";
        case TradingStatus::Precspa:
            return "Precspa";
        case TradingStatus::Cspa:
            return "Cspa";
        case TradingStatus::Adjust:
            return "Adjust";
        case TradingStatus::Adjuston:
            return "Adjuston";
        case TradingStatus::Purgeorders:
            return "Purgeorders";
        case TradingStatus::Sysmaintain:
            return "Sysmaintain";
        case TradingStatus::Clclose:
            return "Clclose";
        case TradingStatus::Clstart:
            return "Clstart";
        case TradingStatus::Openalloc:
            return "Openalloc";
        case TradingStatus::Preopenalloc:
            return "Preopenalloc";
        case TradingStatus::Premktact:
            return "Premktact";
        case TradingStatus::Shohaltrestriction:
            return "Shohaltrestriction";
        case TradingStatus::Filter:
            return "Filter";
        case TradingStatus::Shoquotingrestriction:
            return "Shoquotingrestriction";
        case TradingStatus::Closesessionroll:
            return "Closesessionroll";
        case TradingStatus::Volatilitypause:
            return "Volatilitypause";
        case TradingStatus::Shovolatilitypause:
            return "Shovolatilitypause";
        case TradingStatus::Auctionopen:
            return "Auctionopen";
        case TradingStatus::Auctionintra:
            return "Auctionintra";
        case TradingStatus::Auctionclose:
            return "Auctionclose";
        case TradingStatus::Atlast:
            return "Atlast";
        case TradingStatus::Auctionvola:
            return "Auctionvola";
        case TradingStatus::Precautionaryhalt:
            return "Precautionaryhalt";
        case TradingStatus::Luld:
            return "Luld";
        case TradingStatus::Auctionexp:
            return "Auctionexp";
        case TradingStatus::Auctionvol:
            return "Auctionvol";
        case TradingStatus::Mwcbhalt1:
            return "Mwcbhalt1";
        case TradingStatus::Mwcbhalt2:
            return "Mwcbhalt2";
        case TradingStatus::Mwcbhalt3:
            return "Mwcbhalt3";
        case TradingStatus::Crossing:
            return "Crossing";
        case TradingStatus::Shopremktactrestriction:
            return "Shopremktactrestriction";
        case TradingStatus::Shosuspendedrestriction:
            return "Shosuspendedrestriction";
        case TradingStatus::Haltorderallowed:
            return "Haltorderallowed";
        case TradingStatus::Precross:
            return "Precross";
        case TradingStatus::Nocancel:
            return "Nocancel";
        case TradingStatus::Postclose:
            return "Postclose";
        case TradingStatus::Openingiop:
            return "Openingiop";
        case TradingStatus::Sessionclose:
            return "Sessionclose";
        case TradingStatus::Impliedstart:
            return "Impliedstart";
        case TradingStatus::Impliedend:
            return "Impliedend";
        case TradingStatus::Uncrosscall:
            return "Uncrosscall";
        case TradingStatus::Preopennottrading:
            return "Preopennottrading";
        case TradingStatus::Auctionsurplus:
            return "Auctionsurplus";
        case TradingStatus::Unknown:
            return "Unknown";
        case TradingStatus::Shocloserestriction:
            return "Shocloserestriction";
        case TradingStatus::Pretrading:
            return "Pretrading";
        case TradingStatus::Posttrading:
            return "Posttrading";
        default:
            return "Unknown";
        }
    }
}
