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
    enum class ProductType
    {
        NotSet                                         = 0,
        AssetBackedSecurities                          = 1,
        AmendedRestated                                = 2,
        OtherAnticipationNotes                         = 3,
        BankersAcceptance                              = 4,
        BankNotes                                      = 5,
        BillOfExchanges                                = 6,
        BradyBond                                      = 7,
        BridgeLoan                                     = 8,
        BuySellback                                    = 9,
        ConvertibleBond                                = 10,
        CertificateOfDeposit                           = 11,
        CallLoans                                      = 12,
        CorpMortgageBackedSecurities                   = 13,
        CollateralizedMortgageObligation               = 14,
        CertificateOfObligation                        = 15,
        CertificateOfParticipation                     = 16,
        CorporateBond                                  = 17,
        CommercialPaper                                = 18,
        CorporatePrivatePlacement                      = 19,
        CommonStock                                    = 20,
        Defaulted                                      = 21,
        DebtorInPossession                             = 22,
        DepositNotes                                   = 23,
        DualCurrency                                   = 24,
        EuroCertificateOfDeposit                       = 25,
        EuroCorporateBond                              = 26,
        EuroCommercialPaper                            = 27,
        EuroSovereigns                                 = 28,
        EuroSupranationalCoupons                       = 29,
        FederalAgencyCoupon                            = 30,
        FederalAgencyDiscountNote                      = 31,
        ForeignExchangeContract                        = 32,
        Forward                                        = 33,
        Future                                         = 34,
        GeneralObligationBonds                         = 35,
        IoetteMortgage                                 = 36,
        LetterOfCredit                                 = 37,
        LiquidityNote                                  = 38,
        Matured                                        = 39,
        MortgageBackedSecurities                       = 40,
        MutualFund                                     = 41,
        MortgageInterestOnly                           = 42,
        MultilegInstrument                             = 43,
        MortgagePrincipalOnly                          = 44,
        MortgagePrivatePlacement                       = 45,
        MiscellaneousPassThrough                       = 46,
        MandatoryTender                                = 47,
        MediumTermNotes                                = 48,
        NoSecurityType                                 = 49,
        Overnight                                      = 50,
        Option                                         = 51,
        PrivateExportFunding                           = 52,
        Pfandbriefe                                    = 53,
        PromissoryNote                                 = 54,
        PreferredStock                                 = 55,
        PlazosFijos                                    = 56,
        RevenueAnticipationNote                        = 57,
        Replaced                                       = 58,
        Repurchase                                     = 59,
        Retired                                        = 60,
        RevenueBonds                                   = 61,
        RevolverLoan                                   = 62,
        RevolverTermLoan                               = 63,
        SecuritiesLoan                                 = 64,
        SecuritiesPledge                               = 65,
        SpecialAssessment                              = 66,
        SpecialObligation                              = 67,
        SpecialTax                                     = 68,
        ShortTermLoanNote                              = 69,
        StructuredNotes                                = 70,
        UsdSupranationalCoupons                        = 71,
        SwingLineFacility                              = 72,
        TaxAnticipationNote                            = 73,
        TaxAllocation                                  = 74,
        ToBeAnnounced                                  = 75,
        UsTreasuryBillTbill                            = 76,
        UsTreasuryBond                                 = 77,
        PrincipalStripOfACallableBondOrNote            = 78,
        TimeDeposit                                    = 79,
        TaxExemptCommercialPaper                       = 80,
        TermLoan                                       = 81,
        InterestStripFromAnyBondOrNote                 = 82,
        TreasuryInflationProtectedSecurities           = 83,
        UsTreasuryNoteTnote                            = 84,
        PrincipalStripFromANonCallableBondOrNote       = 85,
        TaxRevenueAnticipationNote                     = 86,
        UsTreasuryNoteUst                              = 87,
        UsTreasuryBillUstb                             = 88,
        VariableRateDemandNote                         = 89,
        Warrant                                        = 90,
        Withdrawn                                      = 91,
        WildcardEntryForUseOnSecurityDefinitionRequest = 92,
        ExtendedCommNote                               = 93,
        IndexedLinked                                  = 94,
        YankeeCorporateBond                            = 95,
        YankeeCertificateOfDeposit                     = 96,
        OptionsOnPhysical                              = 97,
        OptionsOnFutures                               = 98,
        Cash                                           = 99,
        OptionsOnCombo                                 = 100,
        InterestRateSwap                               = 101,
        BankDepositoryNote                             = 102,
        CanadianMoneyMarkets                           = 103,
        CanadianTreasuryNotes                          = 104,
        CanadianTreasuryBills                          = 105,
        CreditDefaultSwap                              = 106,
        CanadianMortgageBonds                          = 107,
        EuroCorporateFloatingRateNotes                 = 108,
        UsCorporateFloatingRateNotes                   = 109,
        CanadianProvincialBonds                        = 110,
        SecuredLiquidityNote                           = 111,
        TreasuryBill                                   = 112,
        TermLiquidityNote                              = 113,
        TaxableMunicipalCp                             = 114,
        NonDeliverableForward                          = 115,
        FxSpot                                         = 116,
        FxForward                                      = 117,
        FxSwap                                         = 118,
        Index                                          = 119,
        OptionStrategy                                 = 200,
        Emission                                       = 201,
        Spot                                           = 202,
        Currencies                                     = 203,
        Synthetic                                      = 1000,
        Invalid                                        = 134217726,
    };

    inline const char* ToString(const ProductType value)
    {
        switch (value)
        {
        case ProductType::NotSet:
            return "NotSet";
        case ProductType::AssetBackedSecurities:
            return "AssetBackedSecurities";
        case ProductType::AmendedRestated:
            return "AmendedRestated";
        case ProductType::OtherAnticipationNotes:
            return "OtherAnticipationNotes";
        case ProductType::BankersAcceptance:
            return "BankersAcceptance";
        case ProductType::BankNotes:
            return "BankNotes";
        case ProductType::BillOfExchanges:
            return "BillOfExchanges";
        case ProductType::BradyBond:
            return "BradyBond";
        case ProductType::BridgeLoan:
            return "BridgeLoan";
        case ProductType::BuySellback:
            return "BuySellback";
        case ProductType::ConvertibleBond:
            return "ConvertibleBond";
        case ProductType::CertificateOfDeposit:
            return "CertificateOfDeposit";
        case ProductType::CallLoans:
            return "CallLoans";
        case ProductType::CorpMortgageBackedSecurities:
            return "CorpMortgageBackedSecurities";
        case ProductType::CollateralizedMortgageObligation:
            return "CollateralizedMortgageObligation";
        case ProductType::CertificateOfObligation:
            return "CertificateOfObligation";
        case ProductType::CertificateOfParticipation:
            return "CertificateOfParticipation";
        case ProductType::CorporateBond:
            return "CorporateBond";
        case ProductType::CommercialPaper:
            return "CommercialPaper";
        case ProductType::CorporatePrivatePlacement:
            return "CorporatePrivatePlacement";
        case ProductType::CommonStock:
            return "CommonStock";
        case ProductType::Defaulted:
            return "Defaulted";
        case ProductType::DebtorInPossession:
            return "DebtorInPossession";
        case ProductType::DepositNotes:
            return "DepositNotes";
        case ProductType::DualCurrency:
            return "DualCurrency";
        case ProductType::EuroCertificateOfDeposit:
            return "EuroCertificateOfDeposit";
        case ProductType::EuroCorporateBond:
            return "EuroCorporateBond";
        case ProductType::EuroCommercialPaper:
            return "EuroCommercialPaper";
        case ProductType::EuroSovereigns:
            return "EuroSovereigns";
        case ProductType::EuroSupranationalCoupons:
            return "EuroSupranationalCoupons";
        case ProductType::FederalAgencyCoupon:
            return "FederalAgencyCoupon";
        case ProductType::FederalAgencyDiscountNote:
            return "FederalAgencyDiscountNote";
        case ProductType::ForeignExchangeContract:
            return "ForeignExchangeContract";
        case ProductType::Forward:
            return "Forward";
        case ProductType::Future:
            return "Future";
        case ProductType::GeneralObligationBonds:
            return "GeneralObligationBonds";
        case ProductType::IoetteMortgage:
            return "IoetteMortgage";
        case ProductType::LetterOfCredit:
            return "LetterOfCredit";
        case ProductType::LiquidityNote:
            return "LiquidityNote";
        case ProductType::Matured:
            return "Matured";
        case ProductType::MortgageBackedSecurities:
            return "MortgageBackedSecurities";
        case ProductType::MutualFund:
            return "MutualFund";
        case ProductType::MortgageInterestOnly:
            return "MortgageInterestOnly";
        case ProductType::MultilegInstrument:
            return "MultilegInstrument";
        case ProductType::MortgagePrincipalOnly:
            return "MortgagePrincipalOnly";
        case ProductType::MortgagePrivatePlacement:
            return "MortgagePrivatePlacement";
        case ProductType::MiscellaneousPassThrough:
            return "MiscellaneousPassThrough";
        case ProductType::MandatoryTender:
            return "MandatoryTender";
        case ProductType::MediumTermNotes:
            return "MediumTermNotes";
        case ProductType::NoSecurityType:
            return "NoSecurityType";
        case ProductType::Overnight:
            return "Overnight";
        case ProductType::Option:
            return "Option";
        case ProductType::PrivateExportFunding:
            return "PrivateExportFunding";
        case ProductType::Pfandbriefe:
            return "Pfandbriefe";
        case ProductType::PromissoryNote:
            return "PromissoryNote";
        case ProductType::PreferredStock:
            return "PreferredStock";
        case ProductType::PlazosFijos:
            return "PlazosFijos";
        case ProductType::RevenueAnticipationNote:
            return "RevenueAnticipationNote";
        case ProductType::Replaced:
            return "Replaced";
        case ProductType::Repurchase:
            return "Repurchase";
        case ProductType::Retired:
            return "Retired";
        case ProductType::RevenueBonds:
            return "RevenueBonds";
        case ProductType::RevolverLoan:
            return "RevolverLoan";
        case ProductType::RevolverTermLoan:
            return "RevolverTermLoan";
        case ProductType::SecuritiesLoan:
            return "SecuritiesLoan";
        case ProductType::SecuritiesPledge:
            return "SecuritiesPledge";
        case ProductType::SpecialAssessment:
            return "SpecialAssessment";
        case ProductType::SpecialObligation:
            return "SpecialObligation";
        case ProductType::SpecialTax:
            return "SpecialTax";
        case ProductType::ShortTermLoanNote:
            return "ShortTermLoanNote";
        case ProductType::StructuredNotes:
            return "StructuredNotes";
        case ProductType::UsdSupranationalCoupons:
            return "UsdSupranationalCoupons";
        case ProductType::SwingLineFacility:
            return "SwingLineFacility";
        case ProductType::TaxAnticipationNote:
            return "TaxAnticipationNote";
        case ProductType::TaxAllocation:
            return "TaxAllocation";
        case ProductType::ToBeAnnounced:
            return "ToBeAnnounced";
        case ProductType::UsTreasuryBillTbill:
            return "UsTreasuryBillTbill";
        case ProductType::UsTreasuryBond:
            return "UsTreasuryBond";
        case ProductType::PrincipalStripOfACallableBondOrNote:
            return "PrincipalStripOfACallableBondOrNote";
        case ProductType::TimeDeposit:
            return "TimeDeposit";
        case ProductType::TaxExemptCommercialPaper:
            return "TaxExemptCommercialPaper";
        case ProductType::TermLoan:
            return "TermLoan";
        case ProductType::InterestStripFromAnyBondOrNote:
            return "InterestStripFromAnyBondOrNote";
        case ProductType::TreasuryInflationProtectedSecurities:
            return "TreasuryInflationProtectedSecurities";
        case ProductType::UsTreasuryNoteTnote:
            return "UsTreasuryNoteTnote";
        case ProductType::PrincipalStripFromANonCallableBondOrNote:
            return "PrincipalStripFromANonCallableBondOrNote";
        case ProductType::TaxRevenueAnticipationNote:
            return "TaxRevenueAnticipationNote";
        case ProductType::UsTreasuryNoteUst:
            return "UsTreasuryNoteUst";
        case ProductType::UsTreasuryBillUstb:
            return "UsTreasuryBillUstb";
        case ProductType::VariableRateDemandNote:
            return "VariableRateDemandNote";
        case ProductType::Warrant:
            return "Warrant";
        case ProductType::Withdrawn:
            return "Withdrawn";
        case ProductType::WildcardEntryForUseOnSecurityDefinitionRequest:
            return "WildcardEntryForUseOnSecurityDefinitionRequest";
        case ProductType::ExtendedCommNote:
            return "ExtendedCommNote";
        case ProductType::IndexedLinked:
            return "IndexedLinked";
        case ProductType::YankeeCorporateBond:
            return "YankeeCorporateBond";
        case ProductType::YankeeCertificateOfDeposit:
            return "YankeeCertificateOfDeposit";
        case ProductType::OptionsOnPhysical:
            return "OptionsOnPhysical";
        case ProductType::OptionsOnFutures:
            return "OptionsOnFutures";
        case ProductType::Cash:
            return "Cash";
        case ProductType::OptionsOnCombo:
            return "OptionsOnCombo";
        case ProductType::InterestRateSwap:
            return "InterestRateSwap";
        case ProductType::BankDepositoryNote:
            return "BankDepositoryNote";
        case ProductType::CanadianMoneyMarkets:
            return "CanadianMoneyMarkets";
        case ProductType::CanadianTreasuryNotes:
            return "CanadianTreasuryNotes";
        case ProductType::CanadianTreasuryBills:
            return "CanadianTreasuryBills";
        case ProductType::CreditDefaultSwap:
            return "CreditDefaultSwap";
        case ProductType::CanadianMortgageBonds:
            return "CanadianMortgageBonds";
        case ProductType::EuroCorporateFloatingRateNotes:
            return "EuroCorporateFloatingRateNotes";
        case ProductType::UsCorporateFloatingRateNotes:
            return "UsCorporateFloatingRateNotes";
        case ProductType::CanadianProvincialBonds:
            return "CanadianProvincialBonds";
        case ProductType::SecuredLiquidityNote:
            return "SecuredLiquidityNote";
        case ProductType::TreasuryBill:
            return "TreasuryBill";
        case ProductType::TermLiquidityNote:
            return "TermLiquidityNote";
        case ProductType::TaxableMunicipalCp:
            return "TaxableMunicipalCp";
        case ProductType::NonDeliverableForward:
            return "NonDeliverableForward";
        case ProductType::FxSpot:
            return "FxSpot";
        case ProductType::FxForward:
            return "FxForward";
        case ProductType::FxSwap:
            return "FxSwap";
        case ProductType::Index:
            return "Index";
        case ProductType::OptionStrategy:
            return "OptionStrategy";
        case ProductType::Emission:
            return "Emission";
        case ProductType::Spot:
            return "Spot";
        case ProductType::Currencies:
            return "Currencies";
        case ProductType::Synthetic:
            return "Synthetic";
        case ProductType::Invalid:
            return "Invalid";
        default:
            return "Unknown";
        }
    }
}
