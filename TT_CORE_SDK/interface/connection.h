/***************************************************************************
 *
 *                    Unpublished Work Copyright (c) 2020
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

namespace ttsdk {

    struct AccountConnectionStatus
    {
        enum class ConnectionStatus
        {
            UNKNOWN = 0,
            UP = 1,
            RISK_DOWN = 2,
            ORDER_CONNECTOR_DOWN = 3,
            SYNTHETIC_ENGINE_DOWN = 4,
            NO_CONNECTIVITY = 5,
        };       

        static constexpr size_t MAX_CONNECTIONS = 56; 
        uint64_t account_id = 0;

        struct Connection
        {
            uint64_t id;
            MarketId market;
            ConnectionStatus status;
        } connections[MAX_CONNECTIONS];

        size_t count; 

    };

    inline const char* ToString(const AccountConnectionStatus::ConnectionStatus status)
    {
        if (status == AccountConnectionStatus::ConnectionStatus::UP) return "Up";
        if (status == AccountConnectionStatus::ConnectionStatus::RISK_DOWN) return "Risk Down";
        if (status == AccountConnectionStatus::ConnectionStatus::ORDER_CONNECTOR_DOWN) return "Order Connector Down";
        if (status == AccountConnectionStatus::ConnectionStatus::SYNTHETIC_ENGINE_DOWN) return "Synthetic Engine Down";
        if (status == AccountConnectionStatus::ConnectionStatus::NO_CONNECTIVITY) return "No Connectivity";
        return "Unknown";
    }
}