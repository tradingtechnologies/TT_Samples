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
        uint8_t     num_algo_threads   = 2;
        bool        enable_pnl         = false;
        bool        use_indicative_pnl = false;
        bool        enable_synthetics  = false;
        bool        spin_price_thread  = true;
        int32_t     spin_price_core    = 1;
        int32_t     price_wait_ms      = 0;
        bool        price_config_check = false;
        ProfitLossCalculationType  pnl_calc_type = ProfitLossCalculationType::RiskWaterfall;
        bool        filter_accounts    = false;
        bool        enable_epiq        = false;
        // unique identifier of this sdk instance - this value should remain
        // the same through all runs of this instance since it identifies the application that
        // is managing the orders and algos within the system, allowing working algos to be recovered in the
        // case of the algo server application shutting down with working algos running. This 
        // value also allows the sdk to identify exchange orders that were managed by the application
        // which have leave on restart set to false so they can be deleted by the sdk on restart
        // 0 indicates not to use this feature
        uint64_t    sdk_instance_id    = 0;
        // enables extra order logging - note this will be done on the critical path processing
        // so only enable it if you need to debug an issue with additional logging around
        // the order event callbacks
        bool        order_debug_logging = false;
    };

}
