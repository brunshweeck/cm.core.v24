//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_TEMPORAL_H
#define CORE24_TEMPORAL_H

#include <core/time/TemporalException.h>

namespace core {
    namespace time {
        /**
         * Framework-level interface defining read-write access to a temporal object,
         * such as a date, time, offset or some combination of these.
         * <p>
         * This is the base interface type for date, time and offset objects that
         * are complete enough to be manipulated using plus and minus.
         * It is implemented by those classes that can provide and manipulate information
         * as @em fields.
         * </p>
         * <p>
         * Most date and time information can be represented as a number.
         * These are modeled using @c Temporal::ChronoField with the number held using
         * a @c glong to handle large values. Year, month and day-of-month are
         * simple examples of fields, but they also include instant and offsets.
         * See @c Temporal::ChronoField for the standard set of fields.
         * </p>
         * <p>
         * Two pieces of date/time information cannot be represented by numbers,
         * the @em chronology and the @em time-zone.
         * </p>
         * <p>
         * This interface is a framework-level interface that should not be widely
         * used in application code. Instead, applications should create and pass
         * around instances of concrete types, such as @c LocalDate.
         * There are many reasons for this, part of which is that implementations
         * of this interface may be in calendar systems other than ISO.
         * </p>
         * <h2>When to implement</h2>
         * <p>
         * A class should implement this interface if it meets three criteria:
         * - it provides access to date/time/offset information
         * - the set of fields are contiguous from the largest to the smallest
         * - the set of fields are complete, such that no other field is needed to define the
         *  valid range of values for the fields that are represented
         * </p>
         * <p>
         * Two examples make this clear:
         * - @c LocalDate implements this interface as it represents a set of fields
         *  that are contiguous from days to forever and require no external information to determine
         *  the validity of each date. It is therefore able to implement plus/minus correctly.
         * - @c LocalTime implements this interface as it represents a set of fields
         *  that are contiguous from nanos to within days and require no external information to determine
         *  validity. It is able to implement plus/minus correctly, by wrapping around the day.
         *
         * @note
         * This interface places no restrictions on the mutability of implementations,
         * however immutability is strongly recommended.
         * All implementations must be @c Comparable.
         */
        class Temporal : public virtual Object {
        protected:
            Temporal() = default;

        public:
            /**
             * A standard set of fields.
             * <p>
             * This set of fields provide field-based access to manipulate a date, time or date-time.
             * The standard set of fields have been extended by others temporal fields.
             * </p>
             * <p>
             * These fields are intended to be applicable in multiple calendar systems.
             * For example, most non-ISO calendar systems define dates as a year, m and day,
             * just with slightly different rules.
             * The documentation of each field explains how i operates.
             * </p>
             */
            enum ChronoField {
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
                OFFSET_SECONDS
            };

            /**
             * A standard set of date periods units.
             * <p>
             * This set of units provide unit-based access to manipulate a date, time or date-time.
             * </p>
             * <p>
             * These units are intended to be applicable in multiple calendar systems.
             * For example, most non-ISO calendar systems define units of years, months and days,
             * just with slightly different rules.
             * The documentation of each unit explains how it operates.
             * </p>
             */
            enum ChronoUnit {
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
                 * This is primarily used with @c ChronoField to represent unbounded fields
                 * such as the year or era.
                 * The estimated duration of this unit is artificially defined as the largest duration
                 * supported by @c Duration.
                 */
                FOREVER,
            };

            /**
             * Enumeration of the style of a localized date, time or date-time formatter.
             * <p>
             * These styles are used when obtaining a date-time style from configuration.
             * See @em DateTimeFormatter for usage.
             * </p>
             */
            enum FormatStyle {
                // ordered from large to small

                /**
                 * Full text style, with the most detail.
                 * For example, the format might be <em>'Saturday, April 12, 1952 AD'</em> or <em>'3:30:42pm PST'</em>.
                 */
                FULL,
                /**
                 * Long text style, with lots of detail.
                 * For example, the format might be 'January 12, 1952'.
                 */
                LONG,
                /**
                 * Medium text style, with some detail.
                 * For example, the format might be 'Jan 12, 1952'.
                 */
                MEDIUM,
                /**
                 * Short text style, typically numeric.
                 * For example, the format might be '12.13.52' or '3:30pm'.
                 */
                SHORT
            };

            enum Constant {
                MAX_YEAR           = +999999999,
                MIN_YEAR           = -999999999,
                MAX_MONTH          = 12,
                MIN_MONTH          = 1,
                MAX_DAY_OF_YEAR    = 366,
                MIN_DAY_OF_YEAR    = 1,
                MAX_DAY_OF_MONTH   = 31,
                MIN_DAY_OF_MONTH   = 1,
                MAX_DAY_OF_WEEK    = 7,
                MIN_DAY_OF_WEEK    = 1,
                MAX_HOUR           = 23,
                MIN_HOUR           = 0,
                MAX_MINUTE         = 59,
                MIN_MINUTE         = 0,
                MAX_SECOND         = 59,
                MIN_SECOND         = 0,
                MAX_MILLI          = 999,
                MIN_MILLI          = 0,
                MAX_MICRO          = 999999,
                MIN_MICRO          = 0,
                MAX_NANOSECOND     = 999999999,
                MIN_NANOSECOND     = 0,
                MAX_OFFSET_SECONDS = +18 * 24 * 60 * 60,
                MIN_OFFSET_SECONDS = -18 * 24 * 60 * 60,
            };

            /**
             * Checks if the specified unit is supported.
             * <p>
             * This checks if the specified unit can be added to, or subtracted from, this date-time.
             * If false, then calling the @em plus and @em minus methods will throw an exception.
             *
             * @param unit  the unit to check
             * @return true if the unit can be added/subtracted, false if not
             */
            virtual gbool isSupported(ChronoUnit unit) const;

            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if the date-time can be queried for the specified field.
             * If false, then calling the @em get method will throw an exception.
             * </p>
             *
             * @param field  the field to check
             * @return true if this date-time can be queried for the field, false if not
             */
            virtual gbool isSupported(ChronoField field) const;

            /**
             * Gets the value of the specified field as a @c gint.
             * <p>
             * This queries the date-time for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If the date-time cannot return the value, because the field is unsupported or for
             * some other reason, an exception will be thrown.
             * </p>
             * @note
             * Implementations must check and handle all fields defined in @c ChronoField.
             * If the field is supported and has a @c gint range, then the value of
             * the field must be returned.
             * If unsupported, then an @c TemporalException must be thrown.
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             * </p>
             *
             * @param field  the field to get, not null
             * @return the value for the field, within the valid range of values
             * @throws DateTimeException if a value for the field cannot be obtained or
             *         the value is outside the range of valid values for the field
             * @throws TemporalException if the field is not supported or
             *         the range of values exceeds a @c gint
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual gint get(ChronoField field) const;

            /**
             * Gets the value of the specified field as a @c glong.
             * <p>
             * This queries the date-time for the value of the specified field.
             * The returned value may be outside the valid range of values for the field.
             * If the date-time cannot return the value, because the field is unsupported or for
             * some other reason, an exception will be thrown.
             * </p>
             * @note
             * Implementations must check and handle all fields defined in @c ChronoField.
             * If the field is supported, then the value of the field must be returned.
             * If unsupported, then an @c TemporalException must be thrown.
             *
             * @param field  the field to get, not null
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual glong getLong(ChronoField field) const;

            /**
             * Calculates the amount of time until another temporal in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two temporal objects
             * in terms of a single @c ChronoUnit.
             * The start and end points are @c this and the specified temporal.
             * The end point is converted to be of the same type as the start point if different.
             * The result will be negative if the end is before the start.
             * For example, the amount in hours between two temporal objects can be
             * calculated using @c startTime.until(endTime,HOURS).
             * </p>
             * <p>
             * The calculation returns a whole number, representing the number of
             * complete units between the two temporal.
             * For example, the amount in hours between the times 11:30 and 13:29
             * will only be one hour as it is one minute short of two hours.
             * </p>
             *
             * @note
             * Implementations must begin by checking to ensure that the input temporal
             * object is of the same observable type as the implementation.
             * They must then perform the calculation for all instances of @em ChronoUnit.
             * An @c TemporalException must be thrown for @c ChronoUnit
             * instances that are unsupported.
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             * </p>
             * @param endExclusive  the end temporal, exclusive, converted to be of the
             *  same type as this object, not null
             * @param unit  the unit to measure the amount in, not null
             * @return the amount of time between this temporal object and the specified one
             *  in terms of the unit; positive if the specified object is later than this one,
             *  negative if it is earlier than this one
             * @throws DateTimeException if the amount cannot be calculated, or the end
             *  temporal cannot be converted to the same type as this temporal
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual glong until(Temporal const& endExclusive, ChronoUnit unit) const;

            /**
             * Queries this date-time.
             * <p>
             * This queries this date-time using the specified query strategy object.
             * </p>
             * <p>
             * Queries are a key tool for extracting information from date-times.
             * They exist to externalize the process of querying, permitting different
             * approaches, as per the strategy design pattern.
             * Examples might be a query that checks if the date is the day before February 29th
             * in a leap year, or calculates the number of days to your next birthday.
             * </p>
             * <p>
             * The most common query implementations are method references, such as
             * @c LocalDate::from and @c ZoneId::from.
             * Additional implementations are provided as static methods on @b TemporalQuery.
             * </p>
             * @note
             * The default implementation must behave equivalent to this code:
             * @code
             *  if (query == TemporalQuery::ZONE_ID ||
             *        query == TemporalQueries::CHRONOLOGY || query == TemporalQuery::PRECISION) {
             *    return clone();
             *  }
             *  return {};
             * @endcode
             * Future versions are permitted to add further queries to the if statement.
             *
             * <p>
             * If the implementation can supply a value for one of the queries listed in the
             * if statement of the default implementation, then it must do so.
             * For example, an application-defined @c HourMin class storing the hour
             * and minute must override this method as follows:
             * @code
             *  if (query == TemporalQuery::PRECISION) {
             *    return Enum<ChronoUnit>(MINUTES);
             *  }
             *  return Temporal.super.query(query);
             * @endcode
             * </p>
             * <p>
             * Implementations must ensure that no observable state is altered when this
             * read-only method is invoked.
             * </p>
             * @param query  the query to invoke
             * @return the query result, no option may be returned (defined by the query)
             * @throws DateTimeException if unable to query
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual util::Optional<> query(TemporalQuery const& query) const = 0;

            /**
             * Check if the specified value is into range defined by specified field.
             *
             * @param value the except value
             * @param field the except field
             * @return the given value if it's correct.
             * @throws DateTimeException if value out of range defined by specified field
             * @see ChronoField for fields ranges
             */
            static gint checkValue(gint value, ChronoField field);

            /**
             * Check if the specified value is into range defined by specified field.
             *
             * @param value the except value
             * @param field the except field
             * @return the given value if it's correct.
             * @throws DateTimeException if value out of range defined by specified field
             * @see ChronoField for fields ranges
             */
            static glong checkValue(glong value, ChronoField field);


            static String toString(ChronoUnit unit);

            static String toString(ChronoField field);

            static String displayMonth(gint value);

            /**
             * Checks if this unit is a time unit.
             * <p>
             * All units from nanos to half-days inclusive are time-based.
             * Date-based units and @c FOREVER return false.
             * </p>
             * @return true if a time unit, false if a date unit
             */
            static gbool isTimeBased(ChronoUnit unit);

            /**
             * Checks if this field represents a component of a time.
             * <p>
             * Fields from nano-of-second to am-pm-of-day are time-based.
             * </p>
             * @return true if it is a component of a time
             */
            static gbool isTimeBased(ChronoField field);

            /**
             * Checks if this unit is a date unit.
             * <p>
             * All units from days to eras inclusive are date-based.
             * Time-based units and @c FOREVER return false.
             * </p>
             * @return true if a date unit, false if a time unit
             */
            static gbool isDateBased(ChronoUnit unit);

            /**
             * Checks if this field represents a component of a date.
             * <p>
             * Fields from day-of-week to era are date-based.
             * </p>
             * @return true if it is a component of a date
             */
            static gbool isDateBased(ChronoField field);
        };
    } // time
} // core

#endif //CORE24_TEMPORAL_H
