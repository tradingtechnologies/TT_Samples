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

#include "enums/SODPriceType.h"
#include <math.h>


namespace ttsdk
{

    struct Position
    {
    public:
        uint64_t account_id = 0;
        uint64_t instrument_id = 0;
        InstrumentPtr instrument = nullptr;

        double   buy_quantity       = NAN;
        double   sell_quantity      = NAN;
        double   net_position       = NAN;
        double   buy_average_price  = NAN;
        double   sell_average_price = NAN;
        double   open_average_price = NAN;

        double   pnl          = NAN;
        double   realized_pnl = NAN;
        double   open_pnl     = NAN;

        double   native_currency_pnl = NAN;
        double   native_currency_realized_pnl = NAN;
        double   native_currency_open_pnl = NAN;

        SODPriceType sod_type = SODPriceType::None;
        double   sod_quantity = NAN;
        double   sod_price    = NAN;

        double   contract_price = NAN;

    };

    struct PositionReserveBucket
    {
        uint64_t account_id = 0;
        uint64_t instrument_id = 0;
        InstrumentPtr instrument = nullptr;

        struct BucketSide
        {
            double positionLimit = -1.0;    // -1 indicated no position has been reserved
            double maxClipSize = -1.0;      // -1 indicated no position has been reserved
            double workingPosition = 0.0;   // qty currently working in the market covered by this bucket
            double filledQty = 0.0;         // qty filled that was included in this bucket
        };
        BucketSide buySide;
        BucketSide sellSide;
        char parent_id[38];
    };

}
