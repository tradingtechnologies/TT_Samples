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
#include "instrument.h"
#include "execution_report.h"
#include "shared_ptr.h"


namespace ttsdk
{
    //! \class Fill
    //! \brief an interface around the fill execution report
    class Fill : public shared_base
    {
    public:
        explicit Fill() {};
        ~Fill() {};

        virtual const char* GetOrderId() const noexcept = 0;
        virtual uint64_t GetTimestamp() const noexcept = 0;
        virtual ExecutionReportPtr GetExecutionReport() const noexcept = 0;
        virtual InstrumentPtr GetInstrument() const noexcept = 0;

    private:
        Fill(const Fill&) = delete;
        Fill& operator=(Fill&) = delete;
        Fill(Fill&&) = delete;
        Fill& operator=(Fill&&) = delete;
    };
    using FillPtr = shared_ptr<const Fill>;

    class HistoricalFillCollection : public shared_base
    {
    public:
        HistoricalFillCollection() {};
        virtual ~HistoricalFillCollection() {};
        virtual size_t GetCount() const = 0;
        virtual FillPtr GetFill(const uint32_t index) const = 0;
    };
    using HistoricalFillCollectionPtr = shared_ptr<const HistoricalFillCollection>;

    class IFillDownloadCallbackHandler
    {
    public:
        enum DownloadResult
        {
            SUCCESS,
            EXCEEDED_THROTTLE,
            FAILED
        };
        IFillDownloadCallbackHandler() {};
        virtual ~IFillDownloadCallbackHandler() {};
        virtual void OnDownloadComplete(const DownloadResult code,
                                        HistoricalFillCollectionPtr orders, const char* message) = 0;
    };
    using IFillDownloadCallbackHandlerPtr = IFillDownloadCallbackHandler*;


}
