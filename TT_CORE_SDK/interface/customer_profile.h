/***************************************************************************
 *
 *                  Unpublished Work Copyright (c) 2022-2023
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

namespace ttsdk 
{    
    class CustomerProfile : public shared_base
    {
    public:

        virtual ~CustomerProfile() = default;

        virtual uint64_t GetId() const = 0;
        virtual uint64_t GetCompanyId() const = 0;
        virtual const char* GetName() const = 0;

    protected:

        CustomerProfile() = default;

        CustomerProfile(CustomerProfile&&) = delete;
        CustomerProfile& operator= (CustomerProfile&&) = delete;
    }; 

    using CustomerProfilePtr = shared_ptr<const CustomerProfile>;

    class CustomerProfileCollection : public shared_base
    {
    public:

        virtual ~CustomerProfileCollection() = default;

        virtual size_t GetCount() const = 0;
        virtual CustomerProfilePtr GetCustomerProfile(const uint32_t index) const = 0;
        virtual CustomerProfilePtr GetCustomerProfile(const char* const name) const = 0;

    protected:

        CustomerProfileCollection() = default;
        
        CustomerProfileCollection(const CustomerProfileCollection&) = delete;
        CustomerProfileCollection& operator=(const CustomerProfileCollection&) = delete;
        
        CustomerProfileCollection(CustomerProfileCollection&&) = delete;
        CustomerProfileCollection& operator=(CustomerProfileCollection&&) = delete;
    };
    
    using CustomerProfileCollectionPtr = shared_ptr<const CustomerProfileCollection>;
}
