//
// Created by brunshweeck on 21/02/25.
//

#ifndef CORE24_TEMPORALADJUSTER_H
#define CORE24_TEMPORALADJUSTER_H

#include <core/time/DayOfWeek.h>
#include <core/time/Month.h>

namespace core {
  namespace time {
    /**
     * Strategy for adjusting a temporal object.
     * <p>
     * Adjusters are a key tool for modifying temporal objects.
     * They exist to externalize the process of adjustment, permitting different
     * approaches, as per the strategy design pattern.
     * Examples might be an adjuster that sets the date avoiding weekends, or one that
     * sets the date to the last day of the month.
     * <p>
     * There are two equivalent ways of using a @c TemporalAdjuster.
     * The first is to invoke the method on this interface directly.
     * The second is to use @b Temporal::with(TemporalAdjuster):
     * @code
     *   // these two lines are equivalent, but the second approach is recommended
     *   temporal = thisAdjuster.adjustInto(temporal);
     *   temporal = temporal.with(thisAdjuster);
     * @endcode
     * It is recommended to use the second approach, @c with(TemporalAdjuster),
     * as it is a lot clearer to read in code.
     * <p>
     * The @b TemporalAdjuster class contains a standard set of adjusters,
     * available as static methods.
     * These include: <br/>
     *
     * - finding the first or last day of the month <br/>
     * - finding the first day of next month <br/>
     * - finding the first or last day of the year <br/>
     * - finding the first day of next year <br/>
     * - finding the first or last day-of-week within a month, such as "first Wednesday in June" <br/>
     * - finding the next or previous day-of-week, such as "next Thursday" <br/>
     *
     * @note
     * This interface places no restrictions on the mutability of implementations,
     * however immutability is strongly recommended.
     */
    class TemporalAdjuster : public virtual Object {
    public:
      CORE_ALIAS(Optional, util::Optional<>);
      CORE_ALIAS(OptionalDate, util::Optional<LocalDate>);
      CORE_ALIAS(OptionalTime, util::Optional<LocalTime>);
      CORE_ALIAS(OptionalZone, util::Optional<ZoneId>);
      CORE_ALIAS(OptionalOffset, util::Optional<ZoneOffset>);
      CORE_ALIAS(OptionalDateTime, util::Optional<LocalDateTime>);
      CORE_ALIAS(OptionalOffsetTime, util::Optional<OffsetTime>);
      CORE_ALIAS(OptionalOffsetDateTime, util::Optional<OffsetDateTime>);
      CORE_ALIAS(LocalDateOperator, function::UnaryOperator<LocalDate>);

      /**
       * Adjusts the specified temporal object.
       * <p>
       * This adjusts the specified temporal object using the logic
       * encapsulated in the implementing class.
       * Examples might be an adjuster that sets the date avoiding weekends, or one that
       * sets the date to the last day of the month.
       * <p>
       * There are two equivalent ways of using this method.
       * The first is to invoke this method directly.
       * The second is to use @b Temporal::with(TemporalAdjuster):
       * @code
       *   // these two lines are equivalent, but the second approach is recommended
       *   temporal = thisAdjuster.adjustInto(temporal);
       *   temporal = temporal.with(thisAdjuster);
       * @endcode
       * It is recommended to use the second approach, @c with(TemporalAdjuster),
       * as it is a lot clearer to read in code.
       *
       * @implSpec
       * The implementation must take the input object and adjust it.
       * The implementation defines the logic of the adjustment and is responsible for
       * documenting that logic. It may use any method on @c Temporal to
       * query the temporal object and perform the adjustment.
       * The returned object must have the same observable type as the input object
       * <p>
       * The input object must not be altered.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable temporal objects.
       * <p>
       * The input temporal object may be in a calendar system other than ISO.
       * Implementations may choose to document compatibility with other calendar systems,
       * or reject non-ISO temporal objects by @b querying the chronology.
       * <p>
       * This method may be called from multiple threads in parallel.
       * It must be thread-safe when invoked.
       *
       * @param temporal  the temporal object to adjust, not null
       * @return an object of the same observable type with the adjustment made, not null
       * @throws DateTimeException if unable to make the adjustment
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Temporal& adjustInto(Temporal const& temporal) const = 0;

      /**
       * Obtains a @c TemporalAdjuster that wraps a date adjuster.
       * <p>
       * The @c TemporalAdjuster is based on the low level @c Temporal interface.
       * This method allows an adjustment from @c LocalDate to @c LocalDate
       * to be wrapped to match the temporal-based interface.
       * This is provided for convenience to make user-written adjusters simpler.
       * <p>
       * In general, user-written adjusters should be static constants:
       * @code
       *  static auto& TWO_DAYS_LATER =
       *       TemporalAdjuster::ofDateAdjuster([](LocalDate const& date) { return date.plusDays(2); });
       * @endcode
       *
       * @param dateBasedAdjuster  the date-based adjuster, not null
       * @return the temporal adjuster wrapping on the date adjuster, not null
       */
      static TemporalAdjuster& ofDateAdjuster(LocalDateOperator const& dateBasedAdjuster);

      /**
       * Returns the "first day of month" adjuster, which returns a new date set to
       * the first day of the current month.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 will return 2011-01-01.<br>
       * The input 2011-02-15 will return 2011-02-01.
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It is equivalent to:
       * @code
       *  temporal.with(TemporalField::DAY_OF_MONTH, 1);
       * @endcode
       *
       * @return the first day-of-month adjuster, not null
       */
      static TemporalAdjuster& firstDayOfMonth();

      /**
       * Returns the "last day of month" adjuster, which returns a new date set to
       * the last day of the current month.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 will return 2011-01-31.<br>
       * The input 2011-02-15 will return 2011-02-28.<br>
       * The input 2012-02-15 will return 2012-02-29 (leap year).<br>
       * The input 2011-04-15 will return 2011-04-30.
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It is equivalent to:
       * @code
       *  long lastDay = temporal.range(TemporalField::DAY_OF_MONTH).getMaximum();
       *  temporal.with(TemporalField::DAY_OF_MONTH, lastDay);
       * @endcode
       *
       * @return the last day-of-month adjuster, not null
       */
      static TemporalAdjuster& lastDayOfMonth();

      /**
       * Returns the "first day of next month" adjuster, which returns a new date set to
       * the first day of the next month.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 will return 2011-02-01.<br>
       * The input 2011-02-15 will return 2011-03-01.
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It is equivalent to:
       * @code
       *  temporal.with(TemporalField::DAY_OF_MONTH, 1).plus(1, TemporalUnits::MONTHS);
       * @endcode
       *
       * @return the first day of next month adjuster, not null
       */
      static TemporalAdjuster& firstDayOfNextMonth();

      /**
       * Returns the "first day of year" adjuster, which returns a new date set to
       * the first day of the current year.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 will return 2011-01-01.<br>
       * The input 2011-02-15 will return 2011-01-01.<br>
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It is equivalent to:
       * @code
       *  temporal.with(DAY_OF_YEAR, 1);
       * @endcode
       *
       * @return the first day-of-year adjuster, not null
       */
      static TemporalAdjuster& firstDayOfYear();

      /**
       * Returns the "last day of year" adjuster, which returns a new date set to
       * the last day of the current year.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 will return 2011-12-31.<br>
       * The input 2011-02-15 will return 2011-12-31.<br>
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It is equivalent to:
       * @code
       *  long lastDay = temporal.range(DAY_OF_YEAR).getMaximum();
       *  temporal.with(DAY_OF_YEAR, lastDay);
       * @endcode
       *
       * @return the last day-of-year adjuster, not null
       */
      static TemporalAdjuster& lastDayOfYear();

      /**
       * Returns the "first day of next year" adjuster, which returns a new date set to
       * the first day of the next year.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 will return 2012-01-01.
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It is equivalent to:
       * @code
       *  temporal.with(DAY_OF_YEAR, 1).plus(1, YEARS);
       * @endcode
       *
       * @return the first day of next month adjuster, not null
       */
      static TemporalAdjuster& firstDayOfNextYear();

      /**
       * Returns the first in month adjuster, which returns a new date
       * in the same month with the first matching day-of-week.
       * This is used for expressions like 'first Tuesday in March'.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-12-15 for (MONDAY) will return 2011-12-05.<br>
       * The input 2011-12-15 for (FRIDAY) will return 2011-12-02.<br>
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It uses the @c DAY_OF_WEEK and @c DAY_OF_MONTH fields
       * and the @c DAYS unit, and assumes a seven-day week.
       *
       * @param dayOfWeek  the day-of-week, not null
       * @return the first in month adjuster, not null
       */
      static TemporalAdjuster& firstInMonth(DayOfWeek dayOfWeek);

      /**
       * Returns the last in month adjuster, which returns a new date
       * in the same month with the last matching day-of-week.
       * This is used for expressions like 'last Tuesday in March'.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-12-15 for (MONDAY) will return 2011-12-26.<br>
       * The input 2011-12-15 for (FRIDAY) will return 2011-12-30.<br>
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It uses the @c DAY_OF_WEEK and @c DAY_OF_MONTH fields
       * and the @c DAYS unit, and assumes a seven day week.
       *
       * @param dayOfWeek  the day-of-week, not null
       * @return the first in month adjuster, not null
       */
      static TemporalAdjuster& lastInMonth(DayOfWeek dayOfWeek);

      /**
       * Returns the day-of-week in month adjuster, which returns a new date
       * with the ordinal day-of-week based on the month.
       * This is used for expressions like the 'second Tuesday in March'.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-12-15 for (1,TUESDAY) will return 2011-12-06.<br>
       * The input 2011-12-15 for (2,TUESDAY) will return 2011-12-13.<br>
       * The input 2011-12-15 for (3,TUESDAY) will return 2011-12-20.<br>
       * The input 2011-12-15 for (4,TUESDAY) will return 2011-12-27.<br>
       * The input 2011-12-15 for (5,TUESDAY) will return 2012-01-03.<br>
       * The input 2011-12-15 for (-1,TUESDAY) will return 2011-12-27 (last in month).<br>
       * The input 2011-12-15 for (-4,TUESDAY) will return 2011-12-06 (3 weeks before last in month).<br>
       * The input 2011-12-15 for (-5,TUESDAY) will return 2011-11-29 (4 weeks before last in month).<br>
       * The input 2011-12-15 for (0,TUESDAY) will return 2011-11-29 (last in previous month).<br>
       * <p>
       * For a positive or zero ordinal, the algorithm is equivalent to finding the first
       * day-of-week that matches within the month and then adding a number of weeks to it.
       * For a negative ordinal, the algorithm is equivalent to finding the last
       * day-of-week that matches within the month and then subtracting a number of weeks to it.
       * The ordinal number of weeks is not validated and is interpreted leniently
       * according to this algorithm. This definition means that an ordinal of zero finds
       * the last matching day-of-week in the previous month.
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It uses the @c DAY_OF_WEEK and @c DAY_OF_MONTH fields
       * and the @c DAYS unit, and assumes a seven day week.
       *
       * @param ordinal  the week within the month, unbounded but typically from -5 to 5
       * @param dayOfWeek  the day-of-week, not null
       * @return the day-of-week in month adjuster, not null
       */
      static TemporalAdjuster& dayOfWeekInMonth(gint ordinal, DayOfWeek dayOfWeek);

      /**
       * Returns the next day-of-week adjuster, which adjusts the date to the
       * first occurrence of the specified day-of-week after the date being adjusted.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 (a Saturday) for parameter (MONDAY) will return 2011-01-17 (two days later).<br>
       * The input 2011-01-15 (a Saturday) for parameter (WEDNESDAY) will return 2011-01-19 (four days later).<br>
       * The input 2011-01-15 (a Saturday) for parameter (SATURDAY) will return 2011-01-22 (seven days later).
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It uses the @c DAY_OF_WEEK field and the @c DAYS unit,
       * and assumes a seven day week.
       *
       * @param dayOfWeek  the day-of-week to move the date to, not null
       * @return the next day-of-week adjuster, not null
       */
      static TemporalAdjuster& next(DayOfWeek dayOfWeek);

      /**
       * Returns the next-or-same day-of-week adjuster, which adjusts the date to the
       * first occurrence of the specified day-of-week after the date being adjusted
       * unless it is already on that day in which case the same object is returned.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 (a Saturday) for parameter (MONDAY) will return 2011-01-17 (two days later).<br>
       * The input 2011-01-15 (a Saturday) for parameter (WEDNESDAY) will return 2011-01-19 (four days later).<br>
       * The input 2011-01-15 (a Saturday) for parameter (SATURDAY) will return 2011-01-15 (same as input).
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It uses the @c DAY_OF_WEEK field and the @c DAYS unit,
       * and assumes a seven day week.
       *
       * @param dayOfWeek  the day-of-week to check for or move the date to, not null
       * @return the next-or-same day-of-week adjuster, not null
       */
      static TemporalAdjuster& nextOrSame(DayOfWeek dayOfWeek);

      /**
       * Returns the previous day-of-week adjuster, which adjusts the date to the
       * first occurrence of the specified day-of-week before the date being adjusted.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 (a Saturday) for parameter (MONDAY) will return 2011-01-10 (five days earlier).<br>
       * The input 2011-01-15 (a Saturday) for parameter (WEDNESDAY) will return 2011-01-12 (three days earlier).<br>
       * The input 2011-01-15 (a Saturday) for parameter (SATURDAY) will return 2011-01-08 (seven days earlier).
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It uses the @c DAY_OF_WEEK field and the @c DAYS unit,
       * and assumes a seven day week.
       *
       * @param dayOfWeek  the day-of-week to move the date to, not null
       * @return the previous day-of-week adjuster, not null
       */
      static TemporalAdjuster& previous(DayOfWeek dayOfWeek);

      /**
       * Returns the previous-or-same day-of-week adjuster, which adjusts the date to the
       * first occurrence of the specified day-of-week before the date being adjusted
       * unless it is already on that day in which case the same object is returned.
       * <p>
       * The ISO calendar system behaves as follows:<br>
       * The input 2011-01-15 (a Saturday) for parameter (MONDAY) will return 2011-01-10 (five days earlier).<br>
       * The input 2011-01-15 (a Saturday) for parameter (WEDNESDAY) will return 2011-01-12 (three days earlier).<br>
       * The input 2011-01-15 (a Saturday) for parameter (SATURDAY) will return 2011-01-15 (same as input).
       * <p>
       * The behavior is suitable for use with most calendar systems.
       * It uses the @c DAY_OF_WEEK field and the @c DAYS unit,
       * and assumes a seven day week.
       *
       * @param dayOfWeek  the day-of-week to check for or move the date to, not null
       * @return the previous-or-same day-of-week adjuster, not null
       */
      static TemporalAdjuster& previousOrSame(DayOfWeek dayOfWeek);

      static TemporalAdjuster& ofMonth(Month month);

      static TemporalAdjuster& ofDayOfWeek(DayOfWeek dayOfWeek);

    protected:
      static Temporal& adjustFieldTo(TemporalField field, glong newValue, Temporal const& temporal);

      static Temporal& adjustFieldTo(TemporalAdjuster const& adjuster, Temporal const& temporal);
    };
  } // time
} // core

#endif //CORE24_TEMPORALADJUSTER_H
