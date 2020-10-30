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
    enum class TradeIndicator
    {
        NotSet           = -2,
        Unknown          = -1,
        New              = 0,
        Bust             = 1,
        Correct          = 2,
        Cross            = 3,
        Open             = 4,
        Close            = 5,
        NonRegularSettle = 6,
        InterMktSweep    = 7,
        OutOfSequence    = 8,
        ExchGenPrice     = 9,
        Block            = 10,
        ExtendedHours    = 11,
        AvgPrice         = 12,
        PriceVariation   = 13,
        StockOption      = 14,
        ReOpen           = 15,
        BunchedSold      = 16,
        CapElection      = 17,
        AutoExecution    = 18,
        Regular          = 19,
    };

    inline const char* ToString(const TradeIndicator value)
    {
        switch (value)
        {
        case TradeIndicator::NotSet:
            return "NotSet";
        case TradeIndicator::Unknown:
            return "Unknown";
        case TradeIndicator::New:
            return "New";
        case TradeIndicator::Bust:
            return "Bust";
        case TradeIndicator::Correct:
            return "Correct";
        case TradeIndicator::Cross:
            return "Cross";
        case TradeIndicator::Open:
            return "Open";
        case TradeIndicator::Close:
            return "Close";
        case TradeIndicator::NonRegularSettle:
            return "NonRegularSettle";
        case TradeIndicator::InterMktSweep:
            return "InterMktSweep";
        case TradeIndicator::OutOfSequence:
            return "OutOfSequence";
        case TradeIndicator::ExchGenPrice:
            return "ExchGenPrice";
        case TradeIndicator::Block:
            return "Block";
        case TradeIndicator::ExtendedHours:
            return "ExtendedHours";
        case TradeIndicator::AvgPrice:
            return "AvgPrice";
        case TradeIndicator::PriceVariation:
            return "PriceVariation";
        case TradeIndicator::StockOption:
            return "StockOption";
        case TradeIndicator::ReOpen:
            return "ReOpen";
        case TradeIndicator::BunchedSold:
            return "BunchedSold";
        case TradeIndicator::CapElection:
            return "CapElection";
        case TradeIndicator::AutoExecution:
            return "AutoExecution";
        case TradeIndicator::Regular:
            return "Regular";
        default:
            return "Unknown";
        }
    }
}
