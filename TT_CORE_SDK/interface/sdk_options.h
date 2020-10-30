/***************************************************************************
 *
 *                  Unpublished Work Copyright (c) 2019-2020
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

#include "environment.h"
#include "enums/ProfitLossCalculationType.h"

namespace ttsdk {

    struct TTSDKOptions 
    {
    public:
        Environment environment        = Environment::NotSet;
        const char* app_key_secret     = nullptr;
        bool        use_onload         = true;
        int32_t     lbm_affinity       = -1;
        bool        log_to_console     = false;
        uint8_t     num_price_threads  = 2;
        uint8_t     num_order_threads  = 2;
        bool        enable_pnl         = true;
        bool        use_indicative_pnl = false;
        bool        spin_price_thread  = true;
        int32_t     spin_price_core    = 1;
        int32_t     price_wait_ms      = 0;
        bool        price_config_check = false;
        ProfitLossCalculationType  pnl_calc_type = ProfitLossCalculationType::RiskWaterfall;
        bool        filter_accounts    = false;
    };

}
