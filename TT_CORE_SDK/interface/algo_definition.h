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

 /*
#ifdef _AFXDLL

#pragma warning(disable: 4275)  // deriving exported class from non-exported

#pragma warning(disable: 4251)  // using non-exported as public in exported

#endif

*/

namespace ttsdk {

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

        AlgoDefinition() {};
    private:
        AlgoDefinition(AlgoDefinition&&) = delete;
        AlgoDefinition& operator= (AlgoDefinition&&) = delete;
    };

    using AlgoDefinitionPtr = shared_ptr<const AlgoDefinition>;
}
