/***************************************************************************
 *
 *                  Unpublished Work Copyright (c) 2018-2020
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
 ***************************************************************************/
#pragma once

#include <stddef.h>
#include "enums/ProductType.h"
#include "enums/MarketId.h"
#include "shared_ptr.h"

namespace ttsdk {

    class InstrumentResult : public shared_base
    {
    public:
        virtual ~InstrumentResult() {};
        virtual const char* GetName() const noexcept = 0;
        virtual const char* GetAlias() const noexcept = 0;
        virtual uint64_t GetInstrumentId() const noexcept = 0;
        virtual ProductType GetProductType() const noexcept = 0;
        virtual const char* GetProductName() const noexcept = 0;
        virtual const char* GetProductSymbol() const noexcept = 0;
        virtual MarketId GetMarketId() const noexcept = 0;
        virtual uint64_t GetMaturityDate() const noexcept = 0;
        virtual const char* GetSeriesKey() const noexcept = 0;

    protected:
        InstrumentResult() {};
    };
    using InstrumentResultPtr = shared_ptr<const InstrumentResult>;

    class InstrumentSearchResults : public shared_base
    {
    public:
        virtual ~InstrumentSearchResults() {};
        virtual size_t GetNumResults() const = 0;
        virtual InstrumentResultPtr GetResult(const uint32_t index) const = 0;

    protected:
        InstrumentSearchResults() {};
    };

    using InstrumentSearchResultsPtr = shared_ptr<InstrumentSearchResults>;

}
