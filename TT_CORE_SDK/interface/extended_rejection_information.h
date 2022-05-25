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

#include "enums/RejectCode.h"
#include "enums/OrderRejectReason.h"
#include "shared_ptr.h"
#include "consts.h"
#include <math.h>

namespace ttsdk
{

    struct ExtendedRejectionInformation
    {
        struct ExceedsMaxOrderSize
        {
            double size = NAN;
            double max_size = NAN;
        };
        struct ExceedsMaxFamilyPosition
        {
             double position = NAN;
             double max_position = NAN;
        };
        struct ExceedsMaxProductPosition
        {
             double position = NAN;
             double max_position = NAN;
        };
        struct ExceedsMaxInstrumentPosition
        {
             double position = NAN;
             double max_position = NAN;
        };
        struct ExceedsLongShort
        {
             double long_short = NAN;
             double max_long_short = NAN;
        };
        struct ExceedsMaxCredit
        {
             double max_credit = NAN;
             double cost = NAN;
        };
        struct ExceedsPriceReasonability
        {
             double deviation = NAN;
             double max_deviation = NAN;
             double percent_deviation = NAN;
             double max_percent_deviation = NAN;
        };
        enum NoConnectionReason
        {
            NONE = 0, // Not set
            UNSUPPORTED_MARKET = 1, // Routing to this market is not supported
            NO_MATCHING_TYPE = 2, // No valid connection type could be found
            UNAVAILABLE = 3, // A valid connection was found, but is currently unavailable
            NO_RESPONSE_SOURCE = 4 // A valid connection was found, but source/sender doesn't exist
        };
        enum RejectIdType
        {
            REJECT_ID_NONE = 0, // Not set
            REJECT_ID_TYPE_ACCOUNT = 1,
            REJECT_ID_TYPE_USER = 2,
            REJECT_ID_TYPE_USER_GROUP = 3,
            // Reject was on an account-user relationship field. reject_id will be the
            // account id and secondary_reject_id will be the user id.
            REJECT_ID_TYPE_ACCOUNT_USER = 4
        };

        // tells what kind of reject this was
        ttsdk::RejectCode code =  ttsdk::RejectCode::NotSet;

        // these fields are set if the reject is because of the limits on a given
        // user/user group/account
        RejectIdType reject_id_type = RejectIdType::REJECT_ID_NONE;
        uint64_t reject_id = U64NAN;
        uint64_t secondary_reject_id = U64NAN;

        // Identifies the leg which violated the limit (spread contracts only).
        // This is sort of a tricky one. It is only set on contract-level leg
        // limit violations with one exception for inter-product spreads. If there
        // is a product-level limit violation for an inter-product spread, then
        // this field will be populated with the leg who's product limit was
        // violated.
        uint64_t leg_instrument_id = U64NAN;

        // these fields are set for certain codes to provide additional information
        // on the reject, the name of the field corresponds to the code name
        ExceedsMaxOrderSize exceeds_max_order_size;
        ExceedsMaxFamilyPosition exceeds_max_family_position;
        ExceedsMaxProductPosition exceeds_max_product_position;
        ExceedsMaxInstrumentPosition exceeds_max_instrument_position;
        ExceedsMaxCredit exceeds_max_credit;
        ExceedsLongShort exceeds_long_short;
        ExceedsPriceReasonability exceeds_price_reasonability;
        NoConnectionReason no_connection = NoConnectionReason::NONE;
    };


}
