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
#include <string.h>
#include "enums/UserDataType.h"

namespace ttsdk {

    struct UserParameter
    {
    public:
        UserParameter();
        UserParameter(const UserParameter& other);
        ~UserParameter();
        UserParameter& operator= (const UserParameter& other);

        char name[75] = { 0 };
        UserDataType type = UserDataType::UserDataTypeEmpty;
        double v_double = NAN;
        double v_price = NAN;
        int32_t v_int = -2147483648LL;
        uint32_t v_uint = 0;
        uint64_t v_timestamp = 0;
        bool v_bool = false;
        // must be allocated on the heap. Object will take
        // ownership of memory assigned to v_string variable
        char* v_string = nullptr;
    };


    inline UserParameter::UserParameter()
    {
    }

    inline UserParameter::UserParameter(const ttsdk::UserParameter& other)
    {
        *this = other;
    }

    inline UserParameter& ttsdk::UserParameter::operator=(const ttsdk::UserParameter& other)
    {
        strncpy(name, other.name, sizeof(name));
        type = other.type;
        v_double = other.v_double;
        v_int = other.v_int;
        v_uint = other.v_uint;
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

    inline UserParameter::~UserParameter() {
        if (v_string)
            delete[] v_string;
    }


}
