//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_TEMPORALUNIT_H
#define CORE24_CORE_24_TEMPORALUNIT_H

#include <core/lang/String.h>

namespace core {
  namespace time {
    /**
     * A unit of date-time, such as Days or Hours.
     * <p>
     * Measurement of time is built on units, such as years, months, days, hours, minutes and seconds.
     * Implementations of this interface represent those units.
     * </p>
     * <p>
     * An instance of this interface represents the unit itself, rather than an amount of the unit.
     * See @b Period for a class that represents an amount in terms of the common units.
     * </p>
     */
    enum class TemporalUnit {
      // Chrono Units

      /**
       * Unit that represents the concept of a nanosecond, the smallest supported unit of time.
       * For the ISO calendar system, it is equal to the 1,000,000,000th part of the second unit.
       */
      NANOS,

      /**
       * Unit that represents the concept of a microsecond.
       * For the ISO calendar system, it is equal to the 1,000,000th part of the second unit.
       */
      MICROS,

      /**
       * Unit that represents the concept of a millisecond.
       * For the ISO calendar system, it is equal to the 1000th part of the second unit.
       */
      MILLIS,

      /**
       * Unit that represents the concept of a second.
       * For the ISO calendar system, it is equal to the second in the SI system
       * of units, except around a leap-second.
       */
      SECONDS,

      /**
       * Unit that represents the concept of a minute.
       * For the ISO calendar system, it is equal to 60 seconds.
       */
      MINUTES,

      /**
       * Unit that represents the concept of an hour.
       * For the ISO calendar system, it is equal to 60 minutes.
       */
      HOURS,

      /**
       * Unit that represents the concept of half a day, as used in AM/PM.
       * For the ISO calendar system, it is equal to 12 hours.
       */
      HALF_DAYS,

      /**
       * Unit that represents the concept of a day.
       * For the ISO calendar system, it is the standard day from midnight to midnight.
       * The estimated duration of a day is @c 24 Hours.
       * <p>
       * When used with other calendar systems it must correspond to the day defined by
       * the rising and setting of the Sun on Earth. It is not required that days begin
       * at midnight - when converting between calendar systems, the date should be
       * equivalent at midday.
       * </p>
       */
      DAYS,

      /**
       * Unit that represents the concept of a week.
       * For the ISO calendar system, it is equal to 7 days.
       * <p>
       * When used with other calendar systems it must correspond to an integral number of days.
       * </p>
       */
      WEEKS,

      /**
       * Unit that represents the concept of a month.
       * For the ISO calendar system, the length of the month varies by month-of-year.
       * The estimated duration of a month is one twelfth of <em>365.2425 Days</em>.
       * <p>
       * When used with other calendar systems it must correspond to an integral number of days.
       * </p>
       */
      MONTHS,

      /**
       * Unit that represents the concept of a year.
       * For the ISO calendar system, it is equal to 12 months.
       * The estimated duration of a year is <em>365.2425 Days</em>.
       * <p>
       * When used with other calendar systems it must correspond to an integral number of days
       * or months roughly equal to a year defined by the passage of the Earth around the Sun.
       * </p>
       */
      YEARS,

      /**
       * Unit that represents the concept of a decade.
       * For the ISO calendar system, it is equal to 10 years.
       * <p>
       * When used with other calendar systems it must correspond to an integral number of days
       * and is normally an integral number of years.
       * </p>
       */
      DECADES,

      /**
       * Unit that represents the concept of a century.
       * For the ISO calendar system, it is equal to 100 years.
       * <p>
       * When used with other calendar systems it must correspond to an integral number of days
       * and is normally an integral number of years.
       * </p>
       */
      CENTURIES,

      /**
       * Unit that represents the concept of a millennium.
       * For the ISO calendar system, it is equal to 1000 years.
       * <p>
       * When used with other calendar systems it must correspond to an integral number of days
       * and is normally an integral number of years.
       * </p>
       */
      MILLENNIA,

      /**
       * Unit that represents the concept of an era.
       * The ISO calendar system doesn't have eras thus it is impossible to add
       * an era to a date or date-time.
       * The estimated duration of the era is artificially defined as <em>1,000,000,000 Years</em>.
       * <p>
       * When used with other calendar systems there are no restrictions on the unit.
       * </p>
       */
      ERAS,

      /**
       * Artificial unit that represents the concept of forever.
       * This is primarily used with @c TemporalField to represent unbounded fields
       * such as the year or era.
       * The estimated duration of this unit is artificially defined as the largest duration
       * supported by @c Duration.
       */
      FOREVER,

      // ISO Units

      /**
       * The unit that represents week-based-years for the purpose of addition and subtraction.
       * <p>
       * This allows a number of week-based-years to be added to, or subtracted from, a date.
       * The unit is equal to either 52 or 53 weeks.
       * The estimated duration of a week-based-year is the same as that of a standard ISO
       * year at @c 365.2425 Days.
       * </p>
       * <p>
       * The rules for addition add the number of week-based-years to the existing value
       * for the week-based-year field. If the resulting week-based-year only has 52 weeks,
       * then the date will be in week 1 of the following week-based-year.
       * </p>
       * <p>
       * This unit is an immutable and thread-safe singleton.
       * </p>
       */
      WEEK_BASED_YEARS,

      /**
       * Unit that represents the concept of a quarter-year.
       * For the ISO calendar system, it is equal to 3 months.
       * The estimated duration of a quarter-year is one quarter of @c 365.2425 Days.
       * <p>
       * This unit is an immutable and thread-safe singleton.
       * </p>
       */
      QUARTER_YEARS,
    };
  } // time

  inline namespace literals {
    extern String operator+(String const& text, time::TemporalUnit unit);

    extern String operator+(time::TemporalUnit unit, String const& text);
  }
} // core

#endif //CORE24_CORE_24_TEMPORALUNIT_H
