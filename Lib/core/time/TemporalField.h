//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_TEMPORALFIELD_H
#define CORE24_CORE_24_TEMPORALFIELD_H

#include <core/lang/String.h>

namespace core {
  namespace time {
    /**
     * A field of date-time, such as month-of-year or minute-of-hour.
     * <p>
     * Date and time is expressed using fields which partition the time-line into something
     * meaningful for humans. Implementations of this interface represent those fields.
     * </p>
     * @see TemporalUtils
     */
    enum class TemporalField {
      // Standard Fields

      /**
       * The nano-of-second.
       * <p>
       * This counts the nanosecond within the second, from 0 to 999,999,999.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * This field is used to represent the nano-of-second handling any fraction of the second.
       * Implementations of @a accessors should provide a value for this field if
       * they can return a value for @c SECOND_OF_MINUTE, @c SECOND_OF_DAY or
       * @c INSTANT_SECONDS filling unknown precision with zero.
       * </p>
       * <p>
       * When this field is used for setting a value, it should set as much precision as the
       * object stores, using integer division to remove excess precision.
       * For example, if the @a accessors stores time to millisecond precision,
       * then the nano-of-second must be divided by 1,000,000 before replacing the milli-of-second.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The field is resolved in combination with @c MILLI_OF_SECOND and @c MICRO_OF_SECOND.
       * </p>
       */
      NANO_OF_SECOND,

      /**
       * The nano-of-day.
       * <p>
       * This counts the nanosecond within the day, from 0 to (24 * 60 * 60 * 1,000,000,000) - 1.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * This field is used to represent the nano-of-day handling any fraction of the second.
       * Implementations of @a accessors should provide a value for this field if
       * they can return a value for @c SECOND_OF_DAY filling unknown precision with zero.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The value is split to form @c NANO_OF_SECOND, @c SECOND_OF_MINUTE,
       * @c MINUTE_OF_HOUR and @c HOUR_OF_DAY fields.
       * </p>
       */
      NANO_OF_DAY,

      /**
       * The micro-of-second.
       * <p>
       * This counts the microsecond within the second, from 0 to 999,999.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * This field is used to represent the micro-of-second handling any fraction of the second.
       * Implementations of @a accessors should provide a value for this field if
       * they can return a value for @c SECOND_OF_MINUTE, @c SECOND_OF_DAY or
       * @c INSTANT_SECONDS filling unknown precision with zero.
       * </p>
       * <p>
       * When this field is used for setting a value, it should behave in the same way as
       * setting @c NANO_OF_SECOND with the value multiplied by 1,000.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The field is resolved in combination with @c MILLI_OF_SECOND to produce
       * @c NANO_OF_SECOND.
       * </p>
       */
      MICRO_OF_SECOND,

      /**
       * The micro-of-day.
       * <p>
       * This counts the microsecond within the day, from 0 to (24 * 60 * 60 * 1,000,000) - 1.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * This field is used to represent the micro-of-day handling any fraction of the second.
       * Implementations of @a accessors should provide a value for this field if
       * they can return a value for @c SECOND_OF_DAY filling unknown precision with zero.
       * </p>
       * <p>
       * When this field is used for setting a value, it should behave in the same way as
       * setting @c NANO_OF_DAY with the value multiplied by 1,000.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The value is split to form @c MICRO_OF_SECOND, @c SECOND_OF_MINUTE,
       * @c MINUTE_OF_HOUR and @c HOUR_OF_DAY fields.
       * </p>
       */
      MICRO_OF_DAY,

      /**
       * The milli-of-second.
       * <p>
       * This counts the millisecond within the second, from 0 to 999.
       * This field has the same meaning for all calendar systems.
       * <p>
       * This field is used to represent the milli-of-second handling any fraction of the second.
       * Implementations of @a accessors should provide a value for this field if
       * they can return a value for @c SECOND_OF_MINUTE, @c SECOND_OF_DAY or
       * @c INSTANT_SECONDS filling unknown precision with zero.
       * </p>
       * <p>
       * When this field is used for setting a value, it should behave in the same way as
       * setting @c NANO_OF_SECOND with the value multiplied by 1,000,000.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The field is resolved in combination with @c MICRO_OF_SECOND to produce
       * @c NANO_OF_SECOND.
       * </p>
       */
      MILLI_OF_SECOND,

      /**
       * The milli-of-day.
       * <p>
       * This counts the millisecond within the day, from 0 to (24 * 60 * 60 * 1,000) - 1.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * This field is used to represent the milli-of-day handling any fraction of the second.
       * Implementations of @a accessors should provide a value for this field if
       * they can return a value for @c SECOND_OF_DAY filling unknown precision with zero.
       * </p>
       * <p>
       * When this field is used for setting a value, it should behave in the same way as
       * setting @c NANO_OF_DAY with the value multiplied by 1,000,000.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The value is split to form @c MILLI_OF_SECOND, @c SECOND_OF_MINUTE,
       * @c MINUTE_OF_HOUR and @c HOUR_OF_DAY fields.
       * </p>
       */
      MILLI_OF_DAY,

      /**
       * The second-of-minute.
       * <p>
       * This counts the second within the minute, from 0 to 59.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * </p>
       */
      SECOND_OF_MINUTE,

      /**
       * The second-of-day.
       * <p>
       * This counts the second within the day, from 0 to (24 * 60 * 60) - 1.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The value is split to form @c SECOND_OF_MINUTE, @c MINUTE_OF_HOUR
       * and @c HOUR_OF_DAY fields.
       * </p>
       */
      SECOND_OF_DAY,

      /**
       * The minute-of-hour.
       * <p>
       * This counts the minute within the hour, from 0 to 59.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * </p>
       */
      MINUTE_OF_HOUR,

      /**
       * The minute-of-day.
       * <p>
       * This counts the minute within the day, from 0 to (24 * 60) - 1.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The value is split to form @c MINUTE_OF_HOUR and @c HOUR_OF_DAY fields.
       * </p>
       */
      MINUTE_OF_DAY,

      /**
       * The hour-of-am-pm.
       * <p>
       * This counts the hour within the AM/PM, from 0 to 11.
       * This is the hour that would be observed on a standard 12-hour digital clock.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated from 0 to 11 in strict and smart mode.
       * In lenient mode the value is not validated. It is combined with
       * @c AMPM_OF_DAY to form @c HOUR_OF_DAY by multiplying
       * the @c AMPM_OF_DAY value by 12.
       * </p>
       * <p>
       * See @c CLOCK_HOUR_OF_AMPM for the related field that counts hours from 1 to 12.
       * </p>
       */
      HOUR_OF_AMPM,

      /**
       * The clock-hour-of-am-pm.
       * <p>
       * This counts the hour within the AM/PM, from 1 to 12.
       * This is the hour that would be observed on a standard 12-hour analog wall clock.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated from 1 to 12 in strict mode and from
       * 0 to 12 in smart mode. In lenient mode the value is not validated.
       * The field is converted to an @c HOUR_OF_AMPM with the same value,
       * unless the value is 12, in which case it is converted to 0.
       * </p>
       * <p>
       * See @em HOUR_OF_AMPM for the related field that counts hours from 0 to 11.
       * </p>
       */
      CLOCK_HOUR_OF_AMPM,

      /**
       * The hour-of-day.
       * <p>
       * This counts the hour within the day, from 0 to 23.
       * This is the hour that would be observed on a standard 24-hour digital clock.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated in strict and smart mode but not in lenient mode.
       * The field is combined with @c MINUTE_OF_HOUR, @c SECOND_OF_MINUTE and
       * @c NANO_OF_SECOND to produce a @c LocalTime.
       * In lenient mode, any excess days are added to the parsed date.
       * </p>
       * <p>
       * See @em CLOCK_HOUR_OF_DAY for the related field that counts hours from 1 to 24.
       * </p>
       */
      HOUR_OF_DAY,

      /**
       * The clock-hour-of-day.
       * <p>
       * This counts the hour within the day, from 1 to 24.
       * This is the hour that would be observed on a 24-hour analog wall clock.
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated from 1 to 24 in strict mode and from
       * 0 to 24 in smart mode. In lenient mode the value is not validated.
       * The field is converted to an @c HOUR_OF_DAY with the same value,
       * unless the value is 24, in which case it is converted to 0.
       * </p>
       * <p>
       * See @em HOUR_OF_DAY for the related field that counts hours from 0 to 23.
       * </p>
       */
      CLOCK_HOUR_OF_DAY,

      /**
       * The am-pm-of-day.
       * <p>
       * This counts the AM/PM within the day, from 0 (AM) to 1 (PM).
       * This field has the same meaning for all calendar systems.
       * </p>
       * <p>
       * When parsing this field it behaves equivalent to the following:
       * The value is validated from 0 to 1 in strict and smart mode.
       * In lenient mode the value is not validated. It is combined with
       * @c HOUR_OF_AMPM (if not present, it defaults to '6') to form
       * @c HOUR_OF_DAY by multiplying the @c AMPM_OF_DAY value
       * by 12.
       * </p>
       */
      AMPM_OF_DAY,

      /**
       * The day-of-week, such as Tuesday.
       * <p>
       * This represents the standard concept of the day of the week.
       * In the default ISO calendar system, this has values from Monday (1) to Sunday (7).
       * The @em DayOfWeek class can be used to interpret the result.
       * </p>
       * <p>
       * Most non-ISO calendar systems also define a seven-day week that aligns with ISO.
       * Those calendar systems must also use the same numbering system, from Monday (1) to
       * Sunday (7), which allows @c DayOfWeek to be used.
       * </p>
       * <p>
       * Calendar systems that do not have a standard seven-day week should implement this field
       * if they have a similar concept of named or numbered days within a period similar
       * to a week. It is recommended that the numbering starts from 1.
       * </p>
       */
      DAY_OF_WEEK,

      /**
       * The aligned day-of-week within a month.
       * <p>
       * This represents concept of the count of days within the period of a week
       * when the weeks are aligned to the start of the month.
       * This field is typically used with @em ALIGNED_WEEK_OF_MONTH.
       * </p>
       * <p>
       * For example, in a calendar systems with a seven-day week, the first aligned-week-of-month
       * starts on day-of-month 1, the second aligned-week starts on day-of-month 8, and so on.
       * Within each of these aligned-weeks, the days are numbered from 1 to 7 and returned
       * as the value of this field.
       * As such, day-of-month 1 to 7 will have aligned-day-of-week values from 1 to 7.
       * And day-of-month 8 to 14 will repeat this with aligned-day-of-week values from 1 to 7.
       * </p>
       * <p>
       * Calendar systems that do not have a seven-day week should typically implement this
       * field in the same way, but using the alternate week length.
       * </p>
       */
      ALIGNED_DAY_OF_WEEK_IN_MONTH,

      /**
       * The aligned day-of-week within a year.
       * <p>
       * This represents concept of the count of days within the period of a week
       * when the weeks are aligned to the start of the year.
       * This field is typically used with @em ALIGNED_WEEK_OF_YEAR.
       * </p>
       * <p>
       * For example, in a calendar systems with a seven-day week, the first aligned-week-of-year
       * starts on day-of-year 1, the second aligned-week starts on day-of-year 8, and so on.
       * Within each of these aligned-weeks, the days are numbered from 1 to 7 and returned
       * as the value of this field.
       * As such, day-of-year 1 to 7 will have aligned-day-of-week values from 1 to 7.
       * And day-of-year 8 to 14 will repeat this with aligned-day-of-week values from 1 to 7.
       * </p>
       * <p>
       * Calendar systems that do not have a seven-day week should typically implement this
       * field in the same way, but using the alternate week length.
       * </p>
       */
      ALIGNED_DAY_OF_WEEK_IN_YEAR,

      /**
       * The day-of-month.
       * <p>
       * This represents the concept of the day within the month.
       * In the default ISO calendar system, this has values from 1 to 31 in most months.
       * April, June, September, November have days from 1 to 30, while February has days
       * from 1 to 28, or 29 in a leap year.
       * </p>
       * <p>
       * Non-ISO calendar systems should implement this field using the most recognized
       * day-of-month values for users of the calendar system.
       * Normally, this is a count of days from 1 to the length of the month.
       * </p>
       */
      DAY_OF_MONTH,

      /**
       * The day-of-year.
       * <p>
       * This represents the concept of the day within the year.
       * In the default ISO calendar system, this has values from 1 to 365 in standard
       * years and 1 to 366 in leap years.
       * </p>
       * <p>
       * Non-ISO calendar systems should implement this field using the most recognized
       * day-of-year values for users of the calendar system.
       * Normally, this is a count of days from 1 to the length of the year.
       * </p>
       * <p>
       * Note that a non-ISO calendar system may have year numbering system that changes
       * at a different point to the natural reset in the month numbering. An example
       * of this is the Japanese calendar system where a change of era, which resets
       * the year number to 1, can happen on any date. The era and year reset also cause
       * the day-of-year to be reset to 1, but not the month-of-year or day-of-month.
       * </p>
       */
      DAY_OF_YEAR,

      /**
       * The epoch-day, based on the Java epoch of 1970-01-01 (ISO).
       * <p>
       * This field is the sequential count of days when 1970-01-01 (ISO) is zero.
       * Note that this uses the <i>local</i> time-line, ignoring offset and time-zone.
       * </p>
       * <p>
       * This field is strictly defined to have the same meaning in all calendar systems.
       * This is necessary to ensure interoperation between calendars.
       * </p>
       * <p>
       * Range of EpochDay is between (LocalDate::MIN.toEpochDay(), LocalDate::MAX.toEpochDay())
       * both inclusive.
       * </p>
       */
      EPOCH_DAY,

      /**
       * The aligned week within a month.
       * <p>
       * This represents concept of the count of weeks within the period of a month
       * when the weeks are aligned to the start of the month.
       * This field is typically used with @em ALIGNED_DAY_OF_WEEK_IN_MONTH.
       * </p>
       * <p>
       * For example, in a calendar systems with a seven-day week, the first aligned-week-of-month
       * starts on day-of-month 1, the second aligned-week starts on day-of-month 8, and so on.
       * Thus, day-of-month values 1 to 7 are in aligned-week 1, while day-of-month values
       * 8 to 14 are in aligned-week 2, and so on.
       * </p>
       * <p>
       * Calendar systems that do not have a seven-day week should typically implement this
       * field in the same way, but using the alternate week length.
       */
      ALIGNED_WEEK_OF_MONTH,

      /**
       * The aligned week within a year.
       * <p>
       * This represents concept of the count of weeks within the period of a year
       * when the weeks are aligned to the start of the year.
       * This field is typically used with @em ALIGNED_DAY_OF_WEEK_IN_YEAR.
       * </p>
       * <p>
       * For example, in a calendar systems with a seven-day week, the first aligned-week-of-year
       * starts on day-of-year 1, the second aligned-week starts on day-of-year 8, and so on.
       * Thus, day-of-year values 1 to 7 are in aligned-week 1, while day-of-year values
       * 8 to 14 are in aligned-week 2, and so on.
       * </p>
       * <p>
       * Calendar systems that do not have a seven-day week should typically implement this
       * field in the same way, but using the alternate week length.
       * </p>
       */
      ALIGNED_WEEK_OF_YEAR,

      /**
       * The month-of-year, such as March.
       * <p>
       * This represents the concept of the month within the year.
       * In the default ISO calendar system, this has values from January (1) to December (12).
       * </p>
       * <p>
       * Non-ISO calendar systems should implement this field using the most recognized
       * month-of-year values for users of the calendar system.
       * Normally, this is a count of months starting from 1.
       * </p>
       */
      MONTH_OF_YEAR,

      /**
       * The proleptic-month based, counting months sequentially from year 0.
       * <p>
       * This field is the sequential count of months when the first month
       * in proleptic-year zero has the value zero.
       * Later months have increasingly larger values.
       * Earlier months have increasingly small values.
       * There are no gaps or breaks in the sequence of months.
       * Note that this uses the <i>local</i> time-line, ignoring offset and time-zone.
       * </p>
       * <p>
       * In the default ISO calendar system, June 2012 would have the value
       * @code (2012 * 12 + 6 - 1) @endcode . This field is primarily for internal use.
       * </p>
       * <p>
       * Non-ISO calendar systems must implement this field as per the definition above.
       * It is just a simple zero-based count of elapsed months from the start of proleptic-year 0.
       * All calendar systems with a full proleptic-year definition will have a year zero.
       * If the calendar system has a minimum year that excludes year zero, then one must
       * be extrapolated in order for this method to be defined.
       * </p>
       */
      PROLEPTIC_MONTH,

      /**
       * The year within the era.
       * <p>
       * This represents the concept of the year within the era.
       * This field is typically used with @em ERA.
       * </p>
       * <p>
       * The standard mental model for a date is based on three concepts - year, month and day.
       * These map onto the @c YEAR, @c MONTH_OF_YEAR and @c DAY_OF_MONTH fields.
       * Note that there is no reference to eras.
       * The full model for a date requires four concepts - era, year, month and day. These map onto
       * the @c ERA, @c YEAR_OF_ERA, @c MONTH_OF_YEAR and @c DAY_OF_MONTH fields.
       * Whether this field or @c YEAR is used depends on which mental model is being used.
       * </p>
       * <p>
       * In the default ISO calendar system, there are two eras defined, 'BCE' and 'CE'.
       * The era 'CE' is the one currently in use and year-of-era runs from 1 to the maximum value.
       * The era 'BCE' is the previous era, and the year-of-era runs backwards.
       * </p>
       * <p>
       * For example, subtracting a year each time yield the following:<br>
       * - year-proleptic 2  = 'CE' year-of-era 2<br>
       * - year-proleptic 1  = 'CE' year-of-era 1<br>
       * - year-proleptic 0  = 'BCE' year-of-era 1<br>
       * - year-proleptic -1 = 'BCE' year-of-era 2<br>
       * </p>
       * <p>
       * Note that the ISO-8601 standard does not actually define eras.
       * Note also that the ISO eras do not align with the well-known AD/BC eras due to the
       * change between the Julian and Gregorian calendar systems.
       * </p>
       * <p>
       * Non-ISO calendar systems should implement this field using the most recognized
       * year-of-era value for users of the calendar system.
       * Since most calendar systems have only two eras, the year-of-era numbering approach
       * will typically be the same as that used by the ISO calendar system.
       * The year-of-era value should typically always be positive, however this is not required.
       * </p>
       */
      YEAR_OF_ERA,

      /**
       * The proleptic year, such as 2012.
       * <p>
       * This represents the concept of the year, counting sequentially and using negative numbers.
       * The proleptic year is not interpreted in terms of the era.
       * See @em YEAR_OF_ERA for an example showing the mapping from proleptic year to year-of-era.
       * </p>
       * <p>
       * The standard mental model for a date is based on three concepts - year, month and day.
       * These map onto the @c YEAR, @c MONTH_OF_YEAR and @c DAY_OF_MONTH fields.
       * Note that there is no reference to eras.
       * The full model for a date requires four concepts - era, year, month and day. These map onto
       * the @c ERA, @c YEAR_OF_ERA, @c MONTH_OF_YEAR and @c DAY_OF_MONTH fields.
       * Whether this field or @c YEAR_OF_ERA is used depends on which mental model is being used.
       * </p>
       * <p>
       * Non-ISO calendar systems should implement this field as follows.
       * If the calendar system has only two eras, before and after a fixed date, then the
       * proleptic-year value must be the same as the year-of-era value for the later era,
       * and increasingly negative for the earlier era.
       * If the calendar system has more than two eras, then the proleptic-year value may be
       * defined with any appropriate value, although defining it to be the same as ISO may be
       * the best option.
       * </p>
       */
      YEAR,

      /**
       * The era.
       * <p>
       * This represents the concept of the era, which is the largest division of the time-line.
       * This field is typically used with @em YEAR_OF_ERA.
       * </p>
       * <p>
       * In the default ISO calendar system, there are two eras defined, 'BCE' and 'CE'.
       * The era 'CE' is the one currently in use and year-of-era runs from 1 to the maximum value.
       * The era 'BCE' is the previous era, and the year-of-era runs backwards.
       * See @em YEAR_OF_ERA for a full example.
       * </p>
       * <p>
       * Non-ISO calendar systems should implement this field to define eras.
       * The value of the era that was active on 1970-01-01 (ISO) must be assigned the value 1.
       * Earlier eras must have sequentially smaller values.
       * Later eras must have sequentially larger values,
       * </p>
       */
      ERA,

      /**
       * The instant epoch-seconds.
       * <p>
       * This represents the concept of the sequential count of seconds where
       * 1970-01-01T00:00Z (ISO) is zero.
       * This field may be used with @c NANO_OF_SECOND to represent the fraction of the second.
       * </p>
       * <p>
       * This field is strictly defined to have the same meaning in all calendar systems.
       * This is necessary to ensure interoperation between calendars.
       * </p>
       */
      INSTANT_SECONDS,

      /**
       * The offset from UTC/Greenwich.
       * <p>
       * This represents the concept of the offset in seconds of local time from UTC/Greenwich.
       * </p>
       * <p>
       * A @em ZoneOffset represents the period of time that local time differs from UTC/Greenwich.
       * This is usually a fixed number of hours and minutes.
       * It is equivalent to the <em>total amount</em> of the offset in seconds.
       * For example, during the winter Paris has an offset of @c +01:00, which is 3600 seconds.
       * </p>
       * <p>
       * This field is strictly defined to have the same meaning in all calendar systems.
       * This is necessary to ensure interoperation between calendars.
       * </p>
       */
      OFFSET_SECONDS,

      // ISO - Fields

      /**
       * The field that represents the day-of-quarter.
       * <p>
       * This field allows the day-of-quarter value to be queried and set.
       * The day-of-quarter has values from 1 to 90 in Q1 of a standard year, from 1 to 91
       * in Q1 of a leap year, from 1 to 91 in Q2 and from 1 to 92 in Q3 and Q4.
       * </p>
       * <p>
       * The day-of-quarter can only be calculated if the day-of-year, month-of-year and year
       * are available.
       * </p>
       * <p>
       * When setting this field, the value is allowed to be partially lenient, taking any
       * value from 1 to 92. If the quarter has less than 92 days, then day 92, and
       * potentially day 91, is in the following quarter.
       * </p>
       * <p>
       * In the resolving phase of parsing, a date can be created from a year,
       * quarter-of-year and day-of-quarter.
       * </p>
       * <p>
       * In <em>strict mode</em>, all three fields are
       * validated against their range of valid values. The day-of-quarter field
       * is validated from 1 to 90, 91 or 92 depending on the year and quarter.
       * </p>
       * <p>
       * In <em>smart mode</em>, all three fields are
       * validated against their range of valid values. The day-of-quarter field is
       * validated between 1 and 92, ignoring the actual range based on the year and quarter.
       * If the day-of-quarter exceeds the actual range by one day, then the resulting date
       * is one day later. If the day-of-quarter exceeds the actual range by two days,
       * then the resulting date is two days later.
       * </p>
       * <p>
       * In <em>lenient mode</em>, only the year is validated
       * against the range of valid values. The resulting date is calculated equivalent to
       * the following three stage approach. First, create a date on the first of January
       * in the requested year. Then take the quarter-of-year, subtract one, and add the
       * amount in quarters to the date. Finally, take the day-of-quarter, subtract one,
       * and add the amount in days to the date.
       * </p>
       * <p>
       * This unit is an immutable and thread-safe singleton.
       * </p>
       */
      DAY_OF_QUARTER,

      /**
       * The field that represents the quarter-of-year.
       * <p>
       * This field allows the quarter-of-year value to be queried and set.
       * The quarter-of-year has values from 1 to 4.
       * </p>
       * <p>
       * The quarter-of-year can only be calculated if the month-of-year is available.
       * </p>
       * <p>
       * In the resolving phase of parsing, a date can be created from a year,
       * quarter-of-year and day-of-quarter. See @b DAY_OF_QUARTER for details.
       * </p>
       * <p>
       * This unit is an immutable and thread-safe singleton.
       * </p>
       */
      QUARTER_OF_YEAR,

      /**
       * The field that represents the week-of-week-based-year.
       * <p>
       * This field allows the week of the week-based-year value to be queried and set.
       * The week-of-week-based-year has values from 1 to 52, or 53 if the
       * week-based-year has 53 weeks.
       * </p>
       * <p>
       * In the resolving phase of parsing, a date can be created from a
       * week-based-year, week-of-week-based-year and day-of-week.
       * </p>
       * <p>
       * In <em>strict mode</em>, all three fields are
       * validated against their range of valid values. The week-of-week-based-year
       * field is validated from 1 to 52 or 53 depending on the week-based-year.
       * </p>
       * <p>
       * In <em>smart mode</em>, all three fields are
       * validated against their range of valid values. The week-of-week-based-year
       * field is validated between 1 and 53, ignoring the week-based-year.
       * If the week-of-week-based-year is 53, but the week-based-year only has
       * 52 weeks, then the resulting date is in week 1 of the following week-based-year.
       * </p>
       * <p>
       * In <em>lenient mode</em>, only the week-based-year
       * is validated against the range of valid values. If the day-of-week is outside
       * the range 1 to 7, then the resulting date is adjusted by a suitable number of
       * weeks to reduce the day-of-week to the range 1 to 7. If the week-of-week-based-year
       * value is outside the range 1 to 52, then any excess weeks are added or subtracted
       * from the resulting date.
       * </p>
       * <p>
       * This unit is an immutable and thread-safe singleton.
       * </p>
       */
      WEEK_OF_WEEK_BASED_YEAR,

      /**
       * The field that represents the week-based-year.
       * <p>
       * This field allows the week-based-year value to be queried and set.
       * </p>
       * <p>
       * The field has a range that matches @b LocalDate::MAX and @b LocalDate::MIN.
       * </p>
       * <p>
       * In the resolving phase of parsing, a date can be created from a
       * week-based-year, week-of-week-based-year and day-of-week.
       * See @b WEEK_OF_WEEK_BASED_YEAR for details.
       * </p>
       * <p>
       * This unit is an immutable and thread-safe singleton.
       * </p>
       */
      WEEK_BASED_YEAR,

      // Julian Fields

      /**
       * Julian Day field.
       * <p>
       * This is an integer-based version of the Julian Day Number.
       * Julian Day is a well-known system that represents the count of whole days since day 0,
       * which is defined to be January 1, 4713 BCE in the Julian calendar, and -4713-11-24 Gregorian.
       * The field  has "JulianDay" as 'name', and 'DAYS' as 'baseUnit'.
       * The field always refers to the local date-time, ignoring the offset or zone.
       * </p>
       * <p>
       * For date-times, 'JULIAN_DAY.getFrom()' assumes the same value from
       * midnight until just before the next midnight.
       * When 'JULIAN_DAY.adjustInto()' is applied to a date-time, the time of day portion remains unaltered.
       * 'JULIAN_DAY.adjustInto()' and 'JULIAN_DAY.getFrom()' only apply to @c Temporal objects that
       * can be converted into @b TemporalField::EPOCH_DAY.
       * An @b TemporalException is thrown for any other type of object.
       * </p>
       * <p>
       * In the resolving phase of parsing, a date can be created from a Julian Day field.
       * In <em>strict mode</em> and <em>smart mode</em>
       * the Julian Day value is validated against the range of valid values.
       * In <em>lenient mode</em> no validation occurs.
       * </p>
       * <h3>Astronomical and Scientific Notes</h3>
       * The standard astronomical definition uses a fraction to indicate the time-of-day,
       * where each day is counted from midday to midday. For example,
       * a fraction of 0 represents midday, a fraction of 0.25
       * represents 18:00, a fraction of 0.5 represents midnight and a fraction
       * of 0.75 represents 06:00.
       * <p>
       * By contrast, this implementation has no fractional part, and counts
       * days from midnight to midnight.
       * This implementation uses an integer and days starting at midnight.
       * The integer value for the Julian Day Number is the astronomical Julian Day value at midday
       * of the date in question.
       * This amounts to the astronomical Julian Day, rounded to an integer
       * @code JDN = floor(JD + 0.5) @endcode.
       *
       * @code
       *  | ISO date          |  Julian Day Number | Astronomical Julian Day |
       *  | 1970-01-01T00:00  |         2,440,588  |         2,440,587.5     |
       *  | 1970-01-01T06:00  |         2,440,588  |         2,440,587.75    |
       *  | 1970-01-01T12:00  |         2,440,588  |         2,440,588.0     |
       *  | 1970-01-01T18:00  |         2,440,588  |         2,440,588.25    |
       *  | 1970-01-02T00:00  |         2,440,589  |         2,440,588.5     |
       *  | 1970-01-02T06:00  |         2,440,589  |         2,440,588.75    |
       *  | 1970-01-02T12:00  |         2,440,589  |         2,440,589.0     |
       * @endcode
       * <p>
       * Julian Days are sometimes taken to imply Universal Time or UTC, but this
       * implementation always uses the Julian Day number for the local date,
       * regardless of the offset or time-zone.
       * </p>
       */
      JULIAN_DAY,

      /**
       * Modified Julian Day field.
       * <p>
       * This is an integer-based version of the Modified Julian Day Number.
       * Modified Julian Day (MJD) is a well-known system that counts days continuously.
       * It is defined relative to astronomical Julian Day as  @code MJD = JD - 2400000.5 @endcode.
       * Each Modified Julian Day runs from midnight to midnight.
       * The field always refers to the local date-time, ignoring the offset or zone.
       * </p>
       * <p>
       * For date-times, 'MODIFIED_JULIAN_DAY.getFrom()' assumes the same value from
       * midnight until just before the next midnight.
       * When 'MODIFIED_JULIAN_DAY.adjustInto()' is applied to a date-time, the time of day portion remains unaltered.
       * 'MODIFIED_JULIAN_DAY.adjustInto()' and 'MODIFIED_JULIAN_DAY.getFrom()' only apply to @c Temporal objects
       * that can be converted into @b TemporalField::EPOCH_DAY.
       * An @b TemporalException is thrown for any other type of object.
       * </p>
       * <p>
       * This implementation is an integer version of MJD with the decimal part rounded to floor.
       * </p>
       * <p>
       * In the resolving phase of parsing, a date can be created from a Modified Julian Day field.
       * In <em>strict mode</em> and <em>smart mode</em>
       * the Modified Julian Day value is validated against the range of valid values.
       * In <em>lenient mode</em> no validation occurs.
       * </p>
       *
       * <h3>Astronomical and Scientific Notes</h3>
       * @code
       *  | ISO date          | Modified Julian Day |      Decimal MJD |
       *  | 1970-01-01T00:00  |             40,587  |       40,587.0   |
       *  | 1970-01-01T06:00  |             40,587  |       40,587.25  |
       *  | 1970-01-01T12:00  |             40,587  |       40,587.5   |
       *  | 1970-01-01T18:00  |             40,587  |       40,587.75  |
       *  | 1970-01-02T00:00  |             40,588  |       40,588.0   |
       *  | 1970-01-02T06:00  |             40,588  |       40,588.25  |
       *  | 1970-01-02T12:00  |             40,588  |       40,588.5   |
       * @endcode
       *
       * Modified Julian Days are sometimes taken to imply Universal Time or UTC, but this
       * implementation always uses the Modified Julian Day for the local date,
       * regardless of the offset or time-zone.
       */
      MODIFIED_JULIAN_DAY,

      /**
       * Rata Die field.
       * <p>
       * Rata Die counts whole days continuously starting day 1 at midnight at the beginning of 0001-01-01 (ISO).
       * The field always refers to the local date-time, ignoring the offset or zone.
       * </p>
       * <p>
       * In the resolving phase of parsing, a date can be created from a Rata Die field.
       * In <em>strict mode</em> and <em>smart mode</em>
       * the Rata Die value is validated against the range of valid values.
       * In <em>lenient mode</em> no validation occurs.
       * </p>
       */
      RATA_DIE,
    };
  } // time

  inline namespace literals {
    extern String operator+(String const& text, time::TemporalField field);

    extern String operator+(time::TemporalField field, String const& text);
  }
} // core

#endif //CORE24_CORE_24_TEMPORALFIELD_H
