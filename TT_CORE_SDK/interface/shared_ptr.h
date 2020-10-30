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
#include "shared_base.h"

namespace ttsdk
{

    template<typename T>
    class shared_ptr
    {
    public:
        typedef T         element_type;
        typedef T *       pointer;
        typedef const T * const_pointer;
        typedef T&        reference;
        typedef const T&  const_reference;

        shared_ptr()
        :object(nullptr)
        {}

        shared_ptr(pointer p)
        :object(nullptr)
        {
            reset(p);
        }

        ~shared_ptr()
        {
            dec_ref(object);
        }

        // Copy constructor
        shared_ptr(const shared_ptr<T>& cc)
        :object(nullptr)
        {
            reset(cc.object);
        }

        // Copy from a derived type
        template<typename U>
        shared_ptr(const shared_ptr<U>& cc)
        :object(nullptr)
        {
            reset(cc.object);
        }

        shared_ptr& operator=(const shared_ptr& rhs)
        {
            reset(rhs.object);
            return *this;
        }

        // assignment from a derived type
        template<typename U>
        shared_ptr& operator=(const shared_ptr<U>& rhs)
        {
            reset(rhs.object);
            return *this;
        }

        // true if not null
        operator bool() {return object != nullptr;}
        operator bool() const {return object != nullptr;}

        // Cast to type U, usually used to downcast to derived type
        // example:
        //       shared_ptr<base_class> base;
        //       shared_ptr<derived_class> derived = base.cast<derived_class>()
        template<typename U>
        shared_ptr<U> cast()
        {
            return shared_ptr<U>(static_cast<U *>(object));
        }

        pointer operator->()
        {
            return object;
        }

        const_pointer operator->() const
        {
            return object;
        }

        reference operator *()
        {
            return *object;
        }

        const_reference operator *() const
        {
            return *object;
        }

        void reset(pointer p)
        {
            dec_ref(object);
            object = p;
            inc_ref(object);
        }
        void reset()
        {
            dec_ref(object);
            object = nullptr;
        }

        pointer get()
        {
            return object;
        }

        const_pointer get() const
        {
            return object;
        }

    private:
        void dec_ref(const T * p)
        {
            if (p)
            {
                if (p->dec_ref() == 0)
                {
                    //delete p;
                    p->del_ref();
                }
            }
        }
       void inc_ref(const T * p)
        {
            if (p)
                p->inc_ref();
        }
        template<typename U>
        friend class shared_ptr;
        mutable T *object;
    };
}
