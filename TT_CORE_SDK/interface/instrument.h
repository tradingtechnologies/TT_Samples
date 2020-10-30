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

#include <stddef.h>
#include "shared_ptr.h"
#include "enums/MarketId.h"
#include "enums/CurrencyCode.h"
#include "enums/ProductType.h"

/*
#ifdef _AFXDLL

#pragma warning(disable: 4275)  // deriving exported class from non-exported

#pragma warning(disable: 4251)  // using non-exported as public in exported

#endif

*/
namespace ttsdk {

    class InstrumentLeg : public shared_base
    {
    public:
        virtual uint64_t GetInstrumentId() const noexcept = 0;
        virtual const char* GetName() const noexcept = 0;
        virtual MarketId GetMarket() const noexcept = 0;
        virtual int32_t GetRatio() const noexcept = 0;
        virtual float GetStrike() const noexcept = 0;
        InstrumentLeg() {};
    private:
        InstrumentLeg(InstrumentLeg&&) = delete;
        InstrumentLeg& operator= (InstrumentLeg&&) = delete;
    };
    using InstrumentLegPtr = shared_ptr<const InstrumentLeg>;
    template class shared_ptr<const InstrumentLeg>; 


    class Instrument : public shared_base
    {
    public:
        enum class ResponseCode
        {
            SUCCESS = 0,
            FAILED = 1,
            INVALID_REQUEST = 2,
            NOT_ALLOWED = 3,
            THROTTLE_EXCEEDED = 4,
            NOT_FOUND = 5,
        };
        enum State
        {
            DELETED = 1,
            INACTIVE = 2,
            ACTIVE = 4
        };
        enum Rounding
        {
            NONE,   //!< Do not round the price
            UP,     //!< Round the price up to the next tradable price
            DOWN,   //!< Round the price down to the next tradable price
            NEAREST //!< Round the price to the nearest tradable price, whether up or down
        };
        // Ticking methods
        //! \brief Returns the tradable price a number of ticks from the specified price
        //! \details You can use this function to find a price a number of price levels
        //!          away from a specified price.  For example, if this price represents
        //!          the last-traded price, you could use this method to get the price two
        //!          ticks higher (or lower).
        //!          <br>
        //!          This function applies the offset value to the specified price and
        //!          then applies the rounding method to arrive at the tradable price.
        //!          For example, suppose you specify a base price that is not tradable
        //!          with an offset of -3 and want to round the price up.  The function
        //!          first drops the price by three ticks and then rounds the new price
        //!          up to the next tradable tick.
        //! \param [in] price Base price
        //! \param [in] offset Number of ticks away from the base price
        //! \param [in] round Rounding method to use
        //! \returns A valid tradable price
        virtual double OffsetPrice (double price, int32_t offset, Rounding round) const noexcept = 0;

        //! \brief Returns the tradable price a number of ticks above the specified price
        //! \param [in] price Base price
        //! \param [in] offset Number of ticks above the base price
        //! \returns A valid tradable price
        virtual double TickPriceUp (double price, int32_t offset) const noexcept = 0;

        //! \brief Returns the tradable price a number of ticks below the specified price
        //! \param [in] price Base price
        //! \param [in] offset Number of ticks below the base price
        //! \returns A valid tradable price
        virtual double TickPriceDown (double price, int32_t offset) const noexcept = 0;

        //! \brief Returns the specified price rounded to a tradable tick
        //! \param [in] price Base price (to round to a tradable tick)
        //! \param [in] round Rounding method to use
        //! \returns A valid tradable price
        virtual double RoundPriceToTick (double price, Rounding round) const noexcept = 0;

        //! \brief Returns the minimum tick size
        //! \details If the instruments supports variable ticking, the result may
        //!          differ from the tick_size property.
        //! \returns Minimum tick size for this instrument
        virtual double GetMinTickSize () const noexcept = 0;

        //! \brief Returns if the specified price is on a tradable tick
        //! \param [in] price Base price
        virtual bool IsPriceOnTick (const double price) const noexcept = 0;

        //! \brief Returns the specified price in tradable ticks
        //! \param [in] price Base price
        //! \note If price does not fall on a tradable tick, it will be rounded to a
        //!       tradable tick (rounded down for positive prices and up for negative
        //!       prices).  Invalid input will return 0.
        //! \returns Number of ticks for the provided price
        virtual int32_t PriceToTicks (const double price) const noexcept = 0;

        // Data members
        virtual State GetState() const noexcept = 0;
        virtual uint64_t GetInstrumentId() const noexcept = 0;
        virtual const char* GetName() const noexcept = 0;
        virtual const char* GetAlias() const noexcept = 0;
        virtual const char* GetSecurityId() const noexcept = 0;
        virtual uint32_t GetSecurityExchangeId() const noexcept = 0;
        virtual ProductType GetProductType() const noexcept = 0;
        virtual MarketId GetMarket() const noexcept = 0;
        virtual CurrencyCode GetCurrencyCode() const noexcept = 0;
        virtual const char* GetProductName() const noexcept = 0;

        virtual double  GetTickValue() const noexcept = 0;
        virtual double  GetTickSize() const noexcept = 0;
        virtual double GetTickSize(const double price) const noexcept = 0;
        virtual int64_t GetTickNum() const noexcept = 0;
        virtual int64_t GetTickDenom() const noexcept = 0;
        virtual double GetPointValue() const noexcept = 0;
        virtual double GetDisplayFactor() const noexcept = 0;

        virtual uint64_t GetStartingDate() const noexcept = 0;
        virtual uint64_t GetLastTradingDate() const noexcept = 0;
        virtual uint64_t GetFirstDeliveryDate() const noexcept = 0;
        virtual uint64_t GetMaturityDate() const noexcept = 0;

        virtual size_t GetNumberLegs() const noexcept = 0;
        virtual InstrumentLegPtr GetLeg(const uint32_t index) const noexcept = 0;

        Instrument() {};
    private:
        Instrument(Instrument&&) = delete;
        Instrument& operator= (Instrument&&) = delete;
    };

    using InstrumentPtr = shared_ptr<const Instrument>;




}
