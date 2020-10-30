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

namespace ttsdk
{
    class shared_base_impl;
    class shared_base
    {
    public:
        shared_base();
        virtual ~shared_base();
        int dec_ref() const;
        void inc_ref() const;
        virtual void del_ref() const;

    private:
        shared_base_impl* impl_;
    };
}
