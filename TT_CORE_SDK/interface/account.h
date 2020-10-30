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

namespace ttsdk {

    class Account : public shared_base
    {
    public:
        virtual ~Account() {};
        virtual uint64_t GetAccountId() const = 0;
        virtual const char* GetName() const = 0;
        virtual uint64_t GetCompanyId() const = 0;
        virtual uint64_t GetBrokerId() const = 0;
        virtual bool IsTradingAllowed() const = 0;

    protected:
        Account() {};

        Account(Account&&) = delete;
        Account& operator= (Account&&) = delete;
    };
    using AccountPtr = shared_ptr<const Account>;

    class AccountCollection : public shared_base
    {
    public:
        virtual ~AccountCollection() {};
        virtual size_t GetCount() const = 0;
        virtual AccountPtr GetAccount(const uint32_t index) const = 0;
        virtual AccountPtr GetAccount(const char* const name) const = 0;

    protected:
        AccountCollection() {};
        AccountCollection(const AccountCollection&) = delete;
        AccountCollection& operator= (const AccountCollection&) = delete;
        AccountCollection(AccountCollection&&) = delete;
        AccountCollection& operator= (AccountCollection&&) = delete;
    };
    using AccountCollectionPtr = shared_ptr<const AccountCollection>;




}
