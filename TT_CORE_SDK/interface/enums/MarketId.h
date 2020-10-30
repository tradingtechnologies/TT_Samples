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
    enum class MarketId
    {
        NotSet         = -1,
        ASE            = 0,
        XETRA          = 1,
        EUREX          = 2,
        NYSELIFFE      = 3,
        LSE            = 4,
        OM             = 5,
        CBOT           = 6,
        CME            = 7,
        CFFE           = 8,
        SEHK           = 9,
        ENXTFUT        = 10,
        ENXTOPT        = 11,
        SYCOM          = 12,
        NQLX           = 13,
        EPEX           = 14,
        GARBAN         = 15,
        LIFFEEO        = 16,
        ISE            = 18,
        INSTINET       = 19,
        NASDAQ         = 20,
        BTECFUTURES    = 21,
        BTEC           = 22,
        ISLAND         = 23,
        ASX            = 24,
        EUREXUS        = 25,
        CBOE           = 26,
        ARCHIPELAGO    = 28,
        IB             = 29,
        EURONEXT       = 30,
        ICE            = 32,
        COREEX         = 43,
        NYMEX          = 61,
        HOTSPOT        = 62,
        MONTREAL       = 63,
        SGX            = 64,
        IDEM           = 65,
        MEFF           = 66,
        TFX            = 67,
        TOCOM          = 68,
        TSE            = 69,
        HKFE           = 70,
        OSE            = 71,
        MEXDER         = 72,
        KRX            = 73,
        CCX            = 74,
        ECX            = 75,
        CCFE           = 76,
        NSE            = 77,
        LME            = 79,
        FXMS           = 80,
        USFE           = 81,
        DME            = 82,
        EBS            = 83,
        SSE            = 85,
        MCX            = 86,
        SIMEX          = 87,
        TMX            = 89,
        BVMF           = 90,
        MX             = 91,
        ICEL           = 92,
        CFE            = 93,
        ERIS           = 94,
        NASDAQOMXEU    = 95,
        AGGREGATOR     = 96,
        TTEX           = 97,
        ESPEED         = 98,
        KCG            = 99,
        EEX            = 100,
        CMEMDP         = 101,
        NFX            = 102,
        NLX            = 103,
        NORDIC         = 104,
        GMEX           = 105,
        NDAQEU         = 106,
        HKEX           = 107,
        FEX            = 108,
        ASX2           = 111,
        OTCME          = 112,
        OTDGCX         = 113,
        DGCX           = 114,
        FIXGW2         = 115,
        FIXGW3         = 116,
        FIXGW4         = 117,
        FIXGW5         = 118,
        FIXGW6         = 119,
        FIXGW7         = 120,
        FIXGW8         = 121,
        FIXGW9         = 122,
        DIRECTMATCH    = 123,
        FENICS         = 124,
        NODAL          = 125,
        BIST           = 126,
        TFEX           = 127,
        GDAX           = 128,
        SAFEX          = 129,
        WSE            = 130,
        CURVEGLOBAL    = 131,
        COINFLOOREX    = 132,
        KRAKEN         = 133,
        BITMEX         = 134,
        MOEX           = 136,
        TAIFEX         = 137,
        ATHEX          = 138,
        COINFLOORUK    = 139,
        INE            = 140,
        DCE            = 141,
        ZCE            = 142,
        SHFE           = 143,
        JUMP           = 145,
        GRAYSTONE      = 146,
        CHFFE          = 147,
        BITSO          = 148,
        DERIBIT        = 149,
        MYSTEEL        = 150,
        XBTG           = 151,
        PME            = 600,
        DV             = 601,
        TT             = 602,
        EUREXDEV       = 802,
        LSEDEV         = 804,
        CMEUC          = 807,
        BTECDEV        = 822,
        BITMEXDEV      = 823,
        ASXDEV         = 824,
        EURONEXTDEV    = 830,
        ICEDEV         = 832,
        ASEDEV         = 833,
        BISTDEV        = 834,
        CLEARPORTDEV   = 835,
        CMEDEV         = 836,
        DGCXDEV        = 837,
        DIRECTMATCHDEV = 838,
        EEXDEV         = 839,
        ERISDEV        = 840,
        ESPEEDDEV      = 841,
        FENICSDEV      = 842,
        FEXDEV         = 843,
        GDAXDEV        = 844,
        HKEXDEV        = 845,
        IDEMDEV        = 846,
        ISEDEV         = 847,
        KRXDEV         = 848,
        MCXDEV         = 849,
        MEXDERDEV      = 850,
        NDAQEUDEV      = 851,
        NFXDEV         = 852,
        NODALDEV       = 853,
        NSEDEV         = 854,
        SAFEXDEV       = 855,
        SEHKDEV        = 856,
        TFEXDEV        = 857,
        TSEDEV         = 858,
        WSEDEV         = 859,
        CURVEGLOBALDEV = 860,
        COINFLOOREXDEV = 861,
        KRAKENDEV      = 862,
        SGXDEV         = 864,
        MEFFDEV        = 866,
        TFXDEV         = 867,
        TOCOMDEV       = 868,
        OSEDEV         = 871,
        LMEDEV         = 879,
        BVMFDEV        = 890,
        MXDEV          = 891,
        ICELDEV        = 892,
        CFEDEV         = 893,
        KCGDEV         = 899,
        NLXDEV         = 903,
        ASX2DEV        = 911,
        MOEXDEV        = 912,
        TAIFEXDEV      = 913,
        ATHEXDEV       = 914,
        COINFLOORUKDEV = 915,
        INEDEV         = 916,
        DCEDEV         = 917,
        ZCEDEV         = 918,
        SHFEDEV        = 919,
        JUMPDEV        = 921,
        GRAYSTONEDEV   = 922,
        CHFFEDEV       = 923,
        DERIBITDEV     = 924,
        TFEXDEV2       = 925,
        MYSTEELDEV     = 926,
        CBOEDEV        = 927,
        ALGO           = 1001,
        BANKALGO       = 1004,
        INVALID        = 134217726,
    };

    inline const char* ToString(const MarketId value)
    {
        switch (value)
        {
        case MarketId::NotSet:
            return "NotSet";
        case MarketId::ASE:
            return "ASE";
        case MarketId::XETRA:
            return "XETRA";
        case MarketId::EUREX:
            return "EUREX";
        case MarketId::NYSELIFFE:
            return "NYSELIFFE";
        case MarketId::LSE:
            return "LSE";
        case MarketId::OM:
            return "OM";
        case MarketId::CBOT:
            return "CBOT";
        case MarketId::CME:
            return "CME";
        case MarketId::CFFE:
            return "CFFE";
        case MarketId::SEHK:
            return "SEHK";
        case MarketId::ENXTFUT:
            return "ENXTFUT";
        case MarketId::ENXTOPT:
            return "ENXTOPT";
        case MarketId::SYCOM:
            return "SYCOM";
        case MarketId::NQLX:
            return "NQLX";
        case MarketId::EPEX:
            return "EPEX";
        case MarketId::GARBAN:
            return "GARBAN";
        case MarketId::LIFFEEO:
            return "LIFFEEO";
        case MarketId::ISE:
            return "ISE";
        case MarketId::INSTINET:
            return "INSTINET";
        case MarketId::NASDAQ:
            return "NASDAQ";
        case MarketId::BTECFUTURES:
            return "BTECFUTURES";
        case MarketId::BTEC:
            return "BTEC";
        case MarketId::ISLAND:
            return "ISLAND";
        case MarketId::ASX:
            return "ASX";
        case MarketId::EUREXUS:
            return "EUREXUS";
        case MarketId::CBOE:
            return "CBOE";
        case MarketId::ARCHIPELAGO:
            return "ARCHIPELAGO";
        case MarketId::IB:
            return "IB";
        case MarketId::EURONEXT:
            return "EURONEXT";
        case MarketId::ICE:
            return "ICE";
        case MarketId::COREEX:
            return "COREEX";
        case MarketId::NYMEX:
            return "NYMEX";
        case MarketId::HOTSPOT:
            return "HOTSPOT";
        case MarketId::MONTREAL:
            return "MONTREAL";
        case MarketId::SGX:
            return "SGX";
        case MarketId::IDEM:
            return "IDEM";
        case MarketId::MEFF:
            return "MEFF";
        case MarketId::TFX:
            return "TFX";
        case MarketId::TOCOM:
            return "TOCOM";
        case MarketId::TSE:
            return "TSE";
        case MarketId::HKFE:
            return "HKFE";
        case MarketId::OSE:
            return "OSE";
        case MarketId::MEXDER:
            return "MEXDER";
        case MarketId::KRX:
            return "KRX";
        case MarketId::CCX:
            return "CCX";
        case MarketId::ECX:
            return "ECX";
        case MarketId::CCFE:
            return "CCFE";
        case MarketId::NSE:
            return "NSE";
        case MarketId::LME:
            return "LME";
        case MarketId::FXMS:
            return "FXMS";
        case MarketId::USFE:
            return "USFE";
        case MarketId::DME:
            return "DME";
        case MarketId::EBS:
            return "EBS";
        case MarketId::SSE:
            return "SSE";
        case MarketId::MCX:
            return "MCX";
        case MarketId::SIMEX:
            return "SIMEX";
        case MarketId::TMX:
            return "TMX";
        case MarketId::BVMF:
            return "BVMF";
        case MarketId::MX:
            return "MX";
        case MarketId::ICEL:
            return "ICEL";
        case MarketId::CFE:
            return "CFE";
        case MarketId::ERIS:
            return "ERIS";
        case MarketId::NASDAQOMXEU:
            return "NASDAQOMXEU";
        case MarketId::AGGREGATOR:
            return "AGGREGATOR";
        case MarketId::TTEX:
            return "TTEX";
        case MarketId::ESPEED:
            return "ESPEED";
        case MarketId::KCG:
            return "KCG";
        case MarketId::EEX:
            return "EEX";
        case MarketId::CMEMDP:
            return "CMEMDP";
        case MarketId::NFX:
            return "NFX";
        case MarketId::NLX:
            return "NLX";
        case MarketId::NORDIC:
            return "NORDIC";
        case MarketId::GMEX:
            return "GMEX";
        case MarketId::NDAQEU:
            return "NDAQEU";
        case MarketId::HKEX:
            return "HKEX";
        case MarketId::FEX:
            return "FEX";
        case MarketId::ASX2:
            return "ASX2";
        case MarketId::OTCME:
            return "OTCME";
        case MarketId::OTDGCX:
            return "OTDGCX";
        case MarketId::DGCX:
            return "DGCX";
        case MarketId::FIXGW2:
            return "FIXGW2";
        case MarketId::FIXGW3:
            return "FIXGW3";
        case MarketId::FIXGW4:
            return "FIXGW4";
        case MarketId::FIXGW5:
            return "FIXGW5";
        case MarketId::FIXGW6:
            return "FIXGW6";
        case MarketId::FIXGW7:
            return "FIXGW7";
        case MarketId::FIXGW8:
            return "FIXGW8";
        case MarketId::FIXGW9:
            return "FIXGW9";
        case MarketId::DIRECTMATCH:
            return "DIRECTMATCH";
        case MarketId::FENICS:
            return "FENICS";
        case MarketId::NODAL:
            return "NODAL";
        case MarketId::BIST:
            return "BIST";
        case MarketId::TFEX:
            return "TFEX";
        case MarketId::GDAX:
            return "GDAX";
        case MarketId::SAFEX:
            return "SAFEX";
        case MarketId::WSE:
            return "WSE";
        case MarketId::CURVEGLOBAL:
            return "CURVEGLOBAL";
        case MarketId::COINFLOOREX:
            return "COINFLOOREX";
        case MarketId::KRAKEN:
            return "KRAKEN";
        case MarketId::BITMEX:
            return "BITMEX";
        case MarketId::MOEX:
            return "MOEX";
        case MarketId::TAIFEX:
            return "TAIFEX";
        case MarketId::ATHEX:
            return "ATHEX";
        case MarketId::COINFLOORUK:
            return "COINFLOORUK";
        case MarketId::INE:
            return "INE";
        case MarketId::DCE:
            return "DCE";
        case MarketId::ZCE:
            return "ZCE";
        case MarketId::SHFE:
            return "SHFE";
        case MarketId::JUMP:
            return "JUMP";
        case MarketId::GRAYSTONE:
            return "GRAYSTONE";
        case MarketId::CHFFE:
            return "CHFFE";
        case MarketId::BITSO:
            return "BITSO";
        case MarketId::DERIBIT:
            return "DERIBIT";
        case MarketId::MYSTEEL:
            return "MYSTEEL";
        case MarketId::XBTG:
            return "XBTG";
        case MarketId::PME:
            return "PME";
        case MarketId::DV:
            return "DV";
        case MarketId::TT:
            return "TT";
        case MarketId::EUREXDEV:
            return "EUREXDEV";
        case MarketId::LSEDEV:
            return "LSEDEV";
        case MarketId::CMEUC:
            return "CMEUC";
        case MarketId::BTECDEV:
            return "BTECDEV";
        case MarketId::BITMEXDEV:
            return "BITMEXDEV";
        case MarketId::ASXDEV:
            return "ASXDEV";
        case MarketId::EURONEXTDEV:
            return "EURONEXTDEV";
        case MarketId::ICEDEV:
            return "ICEDEV";
        case MarketId::ASEDEV:
            return "ASEDEV";
        case MarketId::BISTDEV:
            return "BISTDEV";
        case MarketId::CLEARPORTDEV:
            return "CLEARPORTDEV";
        case MarketId::CMEDEV:
            return "CMEDEV";
        case MarketId::DGCXDEV:
            return "DGCXDEV";
        case MarketId::DIRECTMATCHDEV:
            return "DIRECTMATCHDEV";
        case MarketId::EEXDEV:
            return "EEXDEV";
        case MarketId::ERISDEV:
            return "ERISDEV";
        case MarketId::ESPEEDDEV:
            return "ESPEEDDEV";
        case MarketId::FENICSDEV:
            return "FENICSDEV";
        case MarketId::FEXDEV:
            return "FEXDEV";
        case MarketId::GDAXDEV:
            return "GDAXDEV";
        case MarketId::HKEXDEV:
            return "HKEXDEV";
        case MarketId::IDEMDEV:
            return "IDEMDEV";
        case MarketId::ISEDEV:
            return "ISEDEV";
        case MarketId::KRXDEV:
            return "KRXDEV";
        case MarketId::MCXDEV:
            return "MCXDEV";
        case MarketId::MEXDERDEV:
            return "MEXDERDEV";
        case MarketId::NDAQEUDEV:
            return "NDAQEUDEV";
        case MarketId::NFXDEV:
            return "NFXDEV";
        case MarketId::NODALDEV:
            return "NODALDEV";
        case MarketId::NSEDEV:
            return "NSEDEV";
        case MarketId::SAFEXDEV:
            return "SAFEXDEV";
        case MarketId::SEHKDEV:
            return "SEHKDEV";
        case MarketId::TFEXDEV:
            return "TFEXDEV";
        case MarketId::TSEDEV:
            return "TSEDEV";
        case MarketId::WSEDEV:
            return "WSEDEV";
        case MarketId::CURVEGLOBALDEV:
            return "CURVEGLOBALDEV";
        case MarketId::COINFLOOREXDEV:
            return "COINFLOOREXDEV";
        case MarketId::KRAKENDEV:
            return "KRAKENDEV";
        case MarketId::SGXDEV:
            return "SGXDEV";
        case MarketId::MEFFDEV:
            return "MEFFDEV";
        case MarketId::TFXDEV:
            return "TFXDEV";
        case MarketId::TOCOMDEV:
            return "TOCOMDEV";
        case MarketId::OSEDEV:
            return "OSEDEV";
        case MarketId::LMEDEV:
            return "LMEDEV";
        case MarketId::BVMFDEV:
            return "BVMFDEV";
        case MarketId::MXDEV:
            return "MXDEV";
        case MarketId::ICELDEV:
            return "ICELDEV";
        case MarketId::CFEDEV:
            return "CFEDEV";
        case MarketId::KCGDEV:
            return "KCGDEV";
        case MarketId::NLXDEV:
            return "NLXDEV";
        case MarketId::ASX2DEV:
            return "ASX2DEV";
        case MarketId::MOEXDEV:
            return "MOEXDEV";
        case MarketId::TAIFEXDEV:
            return "TAIFEXDEV";
        case MarketId::ATHEXDEV:
            return "ATHEXDEV";
        case MarketId::COINFLOORUKDEV:
            return "COINFLOORUKDEV";
        case MarketId::INEDEV:
            return "INEDEV";
        case MarketId::DCEDEV:
            return "DCEDEV";
        case MarketId::ZCEDEV:
            return "ZCEDEV";
        case MarketId::SHFEDEV:
            return "SHFEDEV";
        case MarketId::JUMPDEV:
            return "JUMPDEV";
        case MarketId::GRAYSTONEDEV:
            return "GRAYSTONEDEV";
        case MarketId::CHFFEDEV:
            return "CHFFEDEV";
        case MarketId::DERIBITDEV:
            return "DERIBITDEV";
        case MarketId::TFEXDEV2:
            return "TFEXDEV2";
        case MarketId::MYSTEELDEV:
            return "MYSTEELDEV";
        case MarketId::CBOEDEV:
            return "CBOEDEV";
        case MarketId::ALGO:
            return "ALGO";
        case MarketId::BANKALGO:
            return "BANKALGO";
        case MarketId::INVALID:
            return "INVALID";
        default:
            return "Unknown";
        }
    }
}
