/***************************************************************************
 *
 *                  Unpublished Work Copyright (c) 2020
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
#include <string>

namespace ttsdk {

    //! BETA FEATURE - This is part of the beta support of Algos in the CORE SDK and is subject to 
    //! change based on user feedback. Please make note of this if you choose to use it.
    struct ExportValues
    {
    public:
        static constexpr size_t MAX_EVENTS = 10; //!< Max supported export values in one update
        size_t count;
        
        struct ExportValue
        {
        public:

            ExportValue();
            ExportValue(const ExportValue& other);
            ~ExportValue();
            ExportValue& operator= (const ExportValue& other);

            std::string id;
            double v_double = NAN;
            int32_t v_int = -2147483648LL;
            uint64_t v_timestamp = 0;
            bool v_bool = false;
            char* v_string = nullptr;

        };
    
        ExportValue exportValues[MAX_EVENTS];
    };

    inline ExportValues::ExportValue::ExportValue()
    {
    }

    inline ExportValues::ExportValue::ExportValue(const ExportValue& other)
    {
        *this = other;
    }

    inline ExportValues::ExportValue& ExportValues::ExportValue::operator= (const ExportValue& other)
    {
        id = other.id;
        v_double = other.v_double;
        v_int = other.v_int;
        v_timestamp = other.v_timestamp;
        v_bool = other.v_bool;
        v_string = nullptr;
        if (other.v_string)
        {
            size_t len = strlen(other.v_string);
            v_string = new char[len + 1];
            strcpy(v_string, other.v_string);
        }
        return *this;
    }

    inline ExportValues::ExportValue::~ExportValue()
    {
        if (v_string)
            delete[] v_string;
    }
}