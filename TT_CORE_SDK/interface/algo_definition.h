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
#include "shared_ptr.h"
#include "enums/MarketId.h"
#include "enums/AlgoType.h"
#include "enums/UserDataType.h"

namespace ttsdk {

    //! BETA FEATURE - This is part of the beta support of Algos in the CORE SDK and is subject to 
    //! change based on user feedback. Please make note of this if you choose to use it.
    class UserParamDefinition : public shared_base
    {
    public:
        virtual const char* GetDisplayName() const noexcept = 0;
        virtual const char* GetName() const noexcept = 0;
        virtual UserDataType GetDataType() const noexcept = 0;

        explicit UserParamDefinition() {};
        ~UserParamDefinition() {};
    private:
        UserParamDefinition(const UserParamDefinition&) = delete;
        UserParamDefinition& operator= (UserParamDefinition&) = delete;
        UserParamDefinition(UserParamDefinition&&) = delete;
        UserParamDefinition& operator= (UserParamDefinition&&) = delete;
    };
    using UserParamDefinitionPtr = shared_ptr<const UserParamDefinition>;

    class ExportValueDefinition : public shared_base
    {
    public:
        virtual const char* GetDisplayName() const noexcept = 0;
        virtual const char* GetId() const noexcept = 0;
        virtual const char* GetBlockName() const noexcept = 0;
        virtual const char* GetConnector() const noexcept = 0;
        virtual ExportValueType GetDataType() const noexcept = 0;

        explicit ExportValueDefinition() {};
        ~ExportValueDefinition() {};
    private:
        ExportValueDefinition(const ExportValueDefinition&) = delete;
        ExportValueDefinition& operator= (ExportValueDefinition&) = delete;
        ExportValueDefinition(ExportValueDefinition&&) = delete;
        ExportValueDefinition& operator= (ExportValueDefinition&&) = delete;
    };
    using ExportValueDefinitionPtr = shared_ptr<const ExportValueDefinition>;

    class AlgoDefinition : public shared_base
    {
    public:
        enum class ResponseCode
        {
            SUCCESS = 0,
            FAILED = 1,
            INVALID_REQUEST = 2,
            NOT_ALLOWED = 3,
            THROTTLE_EXCEEDED = 4,
            NOT_FOUND = 5,
        };
        
        // Data members
        virtual uint64_t GetAlgoDefinitionId() const noexcept = 0;
        virtual const char* GetName() const noexcept = 0;
        virtual AlgoType GetAlgoType() const noexcept = 0;
        virtual const char* GetOpaqueData() const noexcept = 0;
        virtual const char* GetAlgoVersion() const noexcept = 0;

        virtual uint32_t GetParameterCount() const noexcept = 0;
        virtual ttsdk::UserParamDefinitionPtr GetParameterDefinition(const uint32_t index) const noexcept = 0;

        virtual uint32_t GetExportValueCount() const noexcept = 0;
        virtual ttsdk::ExportValueDefinitionPtr GetExportValueDefinition(const uint32_t index) const noexcept = 0;

        explicit AlgoDefinition() {};
        ~AlgoDefinition() {};
    private:
        AlgoDefinition(const AlgoDefinition&) = delete;
        AlgoDefinition& operator= (AlgoDefinition&) = delete;
        AlgoDefinition(AlgoDefinition&&) = delete;
        AlgoDefinition& operator= (AlgoDefinition&&) = delete;
    };

    using AlgoDefinitionPtr = shared_ptr<const AlgoDefinition>;
}
