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
        SGXGIFT        = 17,
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
        // ESPEED has been deprecated. Please use NFI
        ESPEED         = 98,
        NFI            = 98,
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
        CMEBTEC        = 152,
        TADAWUL        = 153,
        CEDX           = 154,
        JPX            = 155,
        OPRA           = 156,
        LMENTP         = 157,
        NASDAQNED      = 158,
        PME            = 600,
        DV             = 601,
        TT             = 602,
        EUREXDEV       = 802,
        LSEDEV         = 804,
        CMEUC          = 807,
        SGXGIFTDEV     = 817,
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
        // ESPEEDDEV has been deprecated. Please use NFIDEV
        ESPEEDDEV      = 841,
        NFIDEV         = 841,
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
        CEDXDEV        = 928,
        JPXDEV         = 929,
        ALGO           = 1001,
        BANKALGO       = 1004,
        SDKALGO        = 1005,
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
        case MarketId::SGXGIFT:
            return "SGXGIFT";
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
        case MarketId::NFI:
            return "NFI";
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
        case MarketId::CMEBTEC:
            return "CMEBTEC";
        case MarketId::TADAWUL:
            return "TADAWUL";
        case MarketId::CEDX:
            return "CEDX";
        case MarketId::JPX:
            return "JPX";
        case MarketId::OPRA:
            return "OPRA";
        case MarketId::LMENTP:
            return "LMENTP";
        case MarketId::NASDAQNED:
            return "NASDAQNED";
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
        case MarketId::SGXGIFTDEV:
            return "SGXGIFTDEV";
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
        case MarketId::NFIDEV:
            return "NFIDEV";
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
        case MarketId::CEDXDEV:
            return "CEDXDEV";
        case MarketId::JPXDEV:
            return "JPXDEV";
        case MarketId::ALGO:
            return "ALGO";
        case MarketId::BANKALGO:
            return "BANKALGO";
        case MarketId::SDKALGO:
            return "SDKALGO";
        case MarketId::INVALID:
            return "INVALID";
        default:
            return "Unknown";
        }
    }
    inline const char* ToTTName(const MarketId value)
    {
        switch (value)
        {
        case MarketId::NotSet:
            return "";
        case MarketId::ASE:
            return "TT_MARKET_ID_SYNTHETIC";
        case MarketId::XETRA:
            return "TT_MARKET_ID_XETRA";
        case MarketId::EUREX:
            return "TT_MARKET_ID_EUREX";
        case MarketId::NYSELIFFE:
            return "TT_MARKET_ID_NYSE_LIFFE";
        case MarketId::LSE:
            return "TT_MARKET_ID_LSE";
        case MarketId::OM:
            return "TT_MARKET_ID_OM";
        case MarketId::CBOT:
            return "TT_MARKET_ID_CBOT";
        case MarketId::CME:
            return "TT_MARKET_ID_CME";
        case MarketId::CFFE:
            return "TT_MARKET_ID_CFFE";
        case MarketId::SEHK:
            return "TT_MARKET_ID_SEHK";
        case MarketId::ENXTFUT:
            return "TT_MARKET_ID_ENXTFUT";
        case MarketId::ENXTOPT:
            return "TT_MARKET_ID_ENXTOPT";
        case MarketId::SYCOM:
            return "TT_MARKET_ID_SYCOM";
        case MarketId::NQLX:
            return "TT_MARKET_ID_NQLX";
        case MarketId::EPEX:
            return "TT_MARKET_ID_EPEX";
        case MarketId::GARBAN:
            return "TT_MARKET_ID_GARBAN";
        case MarketId::LIFFEEO:
            return "TT_MARKET_ID_LIFFE_EO";
        case MarketId::SGXGIFT:
            return "TT_MARKET_ID_SGX_GIFT";
        case MarketId::ISE:
            return "TT_MARKET_ID_ISE";
        case MarketId::INSTINET:
            return "TT_MARKET_ID_INSTINET";
        case MarketId::NASDAQ:
            return "TT_MARKET_ID_NASDAQ";
        case MarketId::BTECFUTURES:
            return "TT_MARKET_ID_BTECFUTURES";
        case MarketId::BTEC:
            return "TT_MARKET_ID_BTEC";
        case MarketId::ISLAND:
            return "TT_MARKET_ID_ISLAND";
        case MarketId::ASX:
            return "TT_MARKET_ID_ASX";
        case MarketId::EUREXUS:
            return "TT_MARKET_ID_EUREXUS";
        case MarketId::CBOE:
            return "TT_MARKET_ID_CBOE";
        case MarketId::ARCHIPELAGO:
            return "TT_MARKET_ID_ARCHIPELAGO";
        case MarketId::IB:
            return "TT_MARKET_ID_IB";
        case MarketId::EURONEXT:
            return "TT_MARKET_ID_EURONEXT";
        case MarketId::ICE:
            return "TT_MARKET_ID_ICE";
        case MarketId::COREEX:
            return "TT_MARKET_ID_COREEX";
        case MarketId::NYMEX:
            return "TT_MARKET_ID_NYMEX";
        case MarketId::HOTSPOT:
            return "TT_MARKET_ID_HOTSPOT";
        case MarketId::MONTREAL:
            return "TT_MARKET_ID_MONTREAL";
        case MarketId::SGX:
            return "TT_MARKET_ID_SGX";
        case MarketId::IDEM:
            return "TT_MARKET_ID_IDEM";
        case MarketId::MEFF:
            return "TT_MARKET_ID_MEFF";
        case MarketId::TFX:
            return "TT_MARKET_ID_TFX";
        case MarketId::TOCOM:
            return "TT_MARKET_ID_TOCOM";
        case MarketId::TSE:
            return "TT_MARKET_ID_TSE";
        case MarketId::HKFE:
            return "TT_MARKET_ID_HKFE";
        case MarketId::OSE:
            return "TT_MARKET_ID_OSE";
        case MarketId::MEXDER:
            return "TT_MARKET_ID_MEXDER";
        case MarketId::KRX:
            return "TT_MARKET_ID_KRX";
        case MarketId::CCX:
            return "TT_MARKET_ID_CCX";
        case MarketId::ECX:
            return "TT_MARKET_ID_ECX";
        case MarketId::CCFE:
            return "TT_MARKET_ID_CCFE";
        case MarketId::NSE:
            return "TT_MARKET_ID_NSE";
        case MarketId::LME:
            return "TT_MARKET_ID_LME";
        case MarketId::FXMS:
            return "TT_MARKET_ID_FXMS";
        case MarketId::USFE:
            return "TT_MARKET_ID_USFE";
        case MarketId::DME:
            return "TT_MARKET_ID_DME";
        case MarketId::EBS:
            return "TT_MARKET_ID_EBS";
        case MarketId::SSE:
            return "TT_MARKET_ID_SSE";
        case MarketId::MCX:
            return "TT_MARKET_ID_MCX";
        case MarketId::SIMEX:
            return "TT_MARKET_ID_SIMEX";
        case MarketId::TMX:
            return "TT_MARKET_ID_TMX";
        case MarketId::BVMF:
            return "TT_MARKET_ID_BVMF";
        case MarketId::MX:
            return "TT_MARKET_ID_MX";
        case MarketId::ICEL:
            return "TT_MARKET_ID_ICE_L";
        case MarketId::CFE:
            return "TT_MARKET_ID_CFE";
        case MarketId::ERIS:
            return "TT_MARKET_ID_ERIS";
        case MarketId::NASDAQOMXEU:
            return "TT_MARKET_ID_NASDAQ_OMX_EU";
        case MarketId::AGGREGATOR:
            return "TT_MARKET_ID_AGGREGATOR";
        case MarketId::TTEX:
            return "TT_MARKET_ID_TTEX";
        case MarketId::NFI:
            return "TT_MARKET_ID_NFI";
        case MarketId::KCG:
            return "TT_MARKET_ID_KCG";
        case MarketId::EEX:
            return "TT_MARKET_ID_EEX";
        case MarketId::CMEMDP:
            return "TT_MARKET_ID_CME_MDP";
        case MarketId::NFX:
            return "TT_MARKET_ID_NFX";
        case MarketId::NLX:
            return "TT_MARKET_ID_NLX";
        case MarketId::NORDIC:
            return "TT_MARKET_ID_NORDIC";
        case MarketId::GMEX:
            return "TT_MARKET_ID_GMEX";
        case MarketId::NDAQEU:
            return "TT_MARKET_ID_NDAQ_EU";
        case MarketId::HKEX:
            return "TT_MARKET_ID_HKEX";
        case MarketId::FEX:
            return "TT_MARKET_ID_FEX";
        case MarketId::ASX2:
            return "TT_MARKET_ID_ASX2";
        case MarketId::OTCME:
            return "TT_MARKET_ID_OT_CME";
        case MarketId::OTDGCX:
            return "TT_MARKET_ID_OT_DGCX";
        case MarketId::DGCX:
            return "TT_MARKET_ID_DGCX";
        case MarketId::FIXGW2:
            return "TT_MARKET_ID_FIX_GW2";
        case MarketId::FIXGW3:
            return "TT_MARKET_ID_FIX_GW3";
        case MarketId::FIXGW4:
            return "TT_MARKET_ID_FIX_GW4";
        case MarketId::FIXGW5:
            return "TT_MARKET_ID_FIX_GW5";
        case MarketId::FIXGW6:
            return "TT_MARKET_ID_FIX_GW6";
        case MarketId::FIXGW7:
            return "TT_MARKET_ID_FIX_GW7";
        case MarketId::FIXGW8:
            return "TT_MARKET_ID_FIX_GW8";
        case MarketId::FIXGW9:
            return "TT_MARKET_ID_FIX_GW9";
        case MarketId::DIRECTMATCH:
            return "TT_MARKET_ID_DIRECTMATCH";
        case MarketId::FENICS:
            return "TT_MARKET_ID_FENICS";
        case MarketId::NODAL:
            return "TT_MARKET_ID_NODAL";
        case MarketId::BIST:
            return "TT_MARKET_ID_BIST";
        case MarketId::TFEX:
            return "TT_MARKET_ID_TFEX";
        case MarketId::GDAX:
            return "TT_MARKET_ID_GDAX";
        case MarketId::SAFEX:
            return "TT_MARKET_ID_SAFEX";
        case MarketId::WSE:
            return "TT_MARKET_ID_WSE";
        case MarketId::CURVEGLOBAL:
            return "TT_MARKET_ID_CURVEGLOBAL";
        case MarketId::COINFLOOREX:
            return "TT_MARKET_ID_COINFLOOREX";
        case MarketId::KRAKEN:
            return "TT_MARKET_ID_KRAKEN";
        case MarketId::BITMEX:
            return "TT_MARKET_ID_BITMEX";
        case MarketId::MOEX:
            return "TT_MARKET_ID_MOEX";
        case MarketId::TAIFEX:
            return "TT_MARKET_ID_TAIFEX";
        case MarketId::ATHEX:
            return "TT_MARKET_ID_ATHEX";
        case MarketId::COINFLOORUK:
            return "TT_MARKET_ID_COINFLOORUK";
        case MarketId::INE:
            return "TT_MARKET_ID_INE";
        case MarketId::DCE:
            return "TT_MARKET_ID_DCE";
        case MarketId::ZCE:
            return "TT_MARKET_ID_ZCE";
        case MarketId::SHFE:
            return "TT_MARKET_ID_SHFE";
        case MarketId::JUMP:
            return "TT_MARKET_ID_JUMP";
        case MarketId::GRAYSTONE:
            return "TT_MARKET_ID_GRAYSTONE";
        case MarketId::CHFFE:
            return "TT_MARKET_ID_CHFFE";
        case MarketId::BITSO:
            return "TT_MARKET_ID_BITSO";
        case MarketId::DERIBIT:
            return "TT_MARKET_ID_DERIBIT";
        case MarketId::MYSTEEL:
            return "TT_MARKET_ID_MYSTEEL";
        case MarketId::XBTG:
            return "TT_MARKET_ID_XBTG";
        case MarketId::CMEBTEC:
            return "TT_MARKET_ID_CME_BTEC";
        case MarketId::TADAWUL:
            return "TT_MARKET_ID_TADAWUL";
        case MarketId::CEDX:
            return "TT_MARKET_ID_CEDX";
        case MarketId::JPX:
            return "TT_MARKET_ID_JPX";
        case MarketId::OPRA:
            return "TT_MARKET_ID_OPRA";
        case MarketId::LMENTP:
            return "TT_MARKET_ID_LME_NTP";
        case MarketId::NASDAQNED:
            return "TT_MARKET_ID_NASDAQ_NED";
        case MarketId::PME:
            return "TT_MARKET_ID_PME";
        case MarketId::DV:
            return "TT_MARKET_ID_DV";
        case MarketId::TT:
            return "TT_MARKET_ID_TT";
        case MarketId::EUREXDEV:
            return "TT_MARKET_ID_EUREX_DEV";
        case MarketId::LSEDEV:
            return "TT_MARKET_ID_LSE_DEV";
        case MarketId::CMEUC:
            return "TT_MARKET_ID_CME_UC";
        case MarketId::SGXGIFTDEV:
            return "TT_MARKET_ID_SGX_GIFT_DEV";
        case MarketId::BTECDEV:
            return "TT_MARKET_ID_BTEC_DEV";
        case MarketId::BITMEXDEV:
            return "TT_MARKET_ID_BITMEX_DEV";
        case MarketId::ASXDEV:
            return "TT_MARKET_ID_ASX_DEV";
        case MarketId::EURONEXTDEV:
            return "TT_MARKET_ID_EURONEXT_DEV";
        case MarketId::ICEDEV:
            return "TT_MARKET_ID_ICE_DEV";
        case MarketId::ASEDEV:
            return "TT_MARKET_ID_ASE_DEV";
        case MarketId::BISTDEV:
            return "TT_MARKET_ID_BIST_DEV";
        case MarketId::CLEARPORTDEV:
            return "TT_MARKET_ID_CLEARPORT_DEV";
        case MarketId::CMEDEV:
            return "TT_MARKET_ID_CME_DEV";
        case MarketId::DGCXDEV:
            return "TT_MARKET_ID_DGCX_DEV";
        case MarketId::DIRECTMATCHDEV:
            return "TT_MARKET_ID_DIRECTMATCH_DEV";
        case MarketId::EEXDEV:
            return "TT_MARKET_ID_EEX_DEV";
        case MarketId::ERISDEV:
            return "TT_MARKET_ID_ERIS_DEV";
        case MarketId::NFIDEV:
            return "TT_MARKET_ID_NFDEV";
        case MarketId::FENICSDEV:
            return "TT_MARKET_ID_FENICS_DEV";
        case MarketId::FEXDEV:
            return "TT_MARKET_ID_FEX_DEV";
        case MarketId::GDAXDEV:
            return "TT_MARKET_ID_GDAX_DEV";
        case MarketId::HKEXDEV:
            return "TT_MARKET_ID_HKEX_DEV";
        case MarketId::IDEMDEV:
            return "TT_MARKET_ID_IDEM_DEV";
        case MarketId::ISEDEV:
            return "TT_MARKET_ID_ISE_DEV";
        case MarketId::KRXDEV:
            return "TT_MARKET_ID_KRX_DEV";
        case MarketId::MCXDEV:
            return "TT_MARKET_ID_MCX_DEV";
        case MarketId::MEXDERDEV:
            return "TT_MARKET_ID_MEXDER_DEV";
        case MarketId::NDAQEUDEV:
            return "TT_MARKET_ID_NDAQ_EU_DEV";
        case MarketId::NFXDEV:
            return "TT_MARKET_ID_NFX_DEV";
        case MarketId::NODALDEV:
            return "TT_MARKET_ID_NODAL_DEV";
        case MarketId::NSEDEV:
            return "TT_MARKET_ID_NSE_DEV";
        case MarketId::SAFEXDEV:
            return "TT_MARKET_ID_SAFEX_DEV";
        case MarketId::SEHKDEV:
            return "TT_MARKET_ID_SEHK_DEV";
        case MarketId::TFEXDEV:
            return "TT_MARKET_ID_TFEX_DEV";
        case MarketId::TSEDEV:
            return "TT_MARKET_ID_TSE_DEV";
        case MarketId::WSEDEV:
            return "TT_MARKET_ID_WSE_DEV";
        case MarketId::CURVEGLOBALDEV:
            return "TT_MARKET_ID_CURVEGLOBAL_DEV";
        case MarketId::COINFLOOREXDEV:
            return "TT_MARKET_ID_COINFLOOREX_DEV";
        case MarketId::KRAKENDEV:
            return "TT_MARKET_ID_KRAKEN_DEV";
        case MarketId::SGXDEV:
            return "TT_MARKET_ID_SGX_DEV";
        case MarketId::MEFFDEV:
            return "TT_MARKET_ID_MEFF_DEV";
        case MarketId::TFXDEV:
            return "TT_MARKET_ID_TFX_DEV";
        case MarketId::TOCOMDEV:
            return "TT_MARKET_ID_TOCOM_DEV";
        case MarketId::OSEDEV:
            return "TT_MARKET_ID_OSE_DEV";
        case MarketId::LMEDEV:
            return "TT_MARKET_ID_LME_DEV";
        case MarketId::BVMFDEV:
            return "TT_MARKET_ID_BVMF_DEV";
        case MarketId::MXDEV:
            return "TT_MARKET_ID_MX_DEV";
        case MarketId::ICELDEV:
            return "TT_MARKET_ID_ICE_L_DEV";
        case MarketId::CFEDEV:
            return "TT_MARKET_ID_CFE_DEV";
        case MarketId::KCGDEV:
            return "TT_MARKET_ID_KCG_DEV";
        case MarketId::NLXDEV:
            return "TT_MARKET_ID_NLX_DEV";
        case MarketId::ASX2DEV:
            return "TT_MARKET_ID_ASX2_DEV";
        case MarketId::MOEXDEV:
            return "TT_MARKET_ID_MOEX_DEV";
        case MarketId::TAIFEXDEV:
            return "TT_MARKET_ID_TAIFEX_DEV";
        case MarketId::ATHEXDEV:
            return "TT_MARKET_ID_ATHEX_DEV";
        case MarketId::COINFLOORUKDEV:
            return "TT_MARKET_ID_COINFLOORUK_DEV";
        case MarketId::INEDEV:
            return "TT_MARKET_ID_INE_DEV";
        case MarketId::DCEDEV:
            return "TT_MARKET_ID_DCE_DEV";
        case MarketId::ZCEDEV:
            return "TT_MARKET_ID_ZCE_DEV";
        case MarketId::SHFEDEV:
            return "TT_MARKET_ID_SHFE_DEV";
        case MarketId::JUMPDEV:
            return "TT_MARKET_ID_JUMP_DEV";
        case MarketId::GRAYSTONEDEV:
            return "TT_MARKET_ID_GRAYSTONE_DEV";
        case MarketId::CHFFEDEV:
            return "TT_MARKET_ID_CHFFE_DEV";
        case MarketId::DERIBITDEV:
            return "TT_MARKET_ID_DERIBIT_DEV";
        case MarketId::TFEXDEV2:
            return "TT_MARKET_ID_TFEX_DEV2";
        case MarketId::MYSTEELDEV:
            return "TT_MARKET_ID_MYSTEEL_DEV";
        case MarketId::CBOEDEV:
            return "TT_MARKET_ID_CBOE_DEV";
        case MarketId::CEDXDEV:
            return "TT_MARKET_ID_CEDX_DEV";
        case MarketId::JPXDEV:
            return "TT_MARKET_ID_JPX_DEV";
        case MarketId::ALGO:
            return "TT_MARKET_ID_ALGO_INSTRUMENT";
        case MarketId::BANKALGO:
            return "TT_MARKET_ID_BANK_ALGO";
        case MarketId::SDKALGO:
            return "TT_MARKET_ID_SDK_ALGO";
        case MarketId::INVALID:
            return "TT_MARKET_ID_INVALID";
        default:
            return "";
        }
    }
}
