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
****************************************************************************/

#pragma once

namespace ttsdk
{
    enum UserDataType {
        UserDataTypeDouble,
        UserDataTypePrice,
        UserDataTypeInteger,
        UserDataTypeUnsignedInteger,
        UserDataTypeBool,
        UserDataTypeString,
        UserDataTypeTimestamp,
        UserDataTypeEmpty,
    };

    inline const char* ToString(const UserDataType value)
    {
        switch (value)
        {
        case UserDataType::UserDataTypeDouble:
            return "Double";
        case UserDataType::UserDataTypePrice:
            return "Price";
        case UserDataType::UserDataTypeInteger:
            return "Integer";
        case UserDataType::UserDataTypeUnsignedInteger:
            return "UnsignedInteger";
        case UserDataType::UserDataTypeBool:
            return "Bool";
        case UserDataType::UserDataTypeString:
            return "String";
        case UserDataType::UserDataTypeTimestamp:
            return "Timestamp";
        default:
            return "Empty";
        }
    }

    enum ExportValueType {
        ExportValueTypeDouble,
        ExportValueTypeInteger,
        ExportValueTypeeBool,
        ExportValueTypeString,
        ExportValueTypeTimestamp,
        ExportValueTypeEmpty,
    };

    inline const char* ToString(const ExportValueType value)
    {
        switch (value)
        {
        case ExportValueType::ExportValueTypeDouble:
            return "Double";
        case ExportValueType::ExportValueTypeInteger:
            return "Integer";
        case ExportValueType::ExportValueTypeeBool:
            return "Bool";
        case ExportValueType::ExportValueTypeString:
            return "String";
        case ExportValueType::ExportValueTypeTimestamp:
            return "Timestamp";
        default:
            return "Empty";
        }
    }
}
