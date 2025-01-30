//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_LOCALDATETIME_H
#define CORE24_LOCALDATETIME_H

#include <core/time/LocalDate.h>
#include <core/time/LocalTime.h>

namespace core {
    namespace time {
        /**
         * A date-time without a time-zone in the ISO-8601 calendar system,
         * such as @c 2007-12-03T10:15:30.
         * <p>
         * @c LocalDateTime is an immutable date-time object that represents a date-time,
         * often viewed as year-month-day-hour-minute-second. Other date and time fields,
         * such as day-of-year, day-of-week and week-of-year, can also be accessed.
         * Time is represented to nanosecond precision.
         * For example, the value "2nd October 2007 at 13:45.30.123456789" can be
         * stored in a @c LocalDateTime.
         * </p>
         * <p>
         * This class does not store or represent a time-zone.
         * Instead, it is a description of the date, as used for birthdays, combined with
         * the local time as seen on a wall clock.
         * It cannot represent an instant on the time-line without additional information
         * such as an offset or time-zone.
         * </p>
         * <p>
         * The ISO-8601 calendar system is the modern civil calendar system used today
         * in most of the world. It is equivalent to the proleptic Gregorian calendar
         * system, in which today's rules for leap years are applied for all time.
         * For most applications written today, the ISO-8601 rules are entirely suitable.
         * However, any application that makes use of historical dates, and requires them
         * to be accurate will find the ISO-8601 approach unsuitable.
         * </p>
         * <p>
         * This is a <em>value-based</em>
         * class; programmers should treat instances that are
         * @em equal as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * The @c equals method should be used for comparisons.
         * </p>
         * @note
         * This class is immutable and thread-safe.
         */
        class LocalDateTime final : public Temporal, public Comparable<LocalDateTime> {
        public:
            /**
             * The minimum supported @c LocalDateTime, '-999999999-01-01T00:00:00'.
             * This is the local date-time of midnight at the start of the minimum date.
             * This combines @b LocalDate::MIN and @b LocalTime#MIN.
             * This could be used by an application as a "far past" date-time.
             */
            static LocalDateTime const MIN;

            /**
             * The maximum supported @c LocalDateTime, '+999999999-12-31T23:59:59.999999999'.
             * This is the local date-time just before midnight at the end of the maximum date.
             * This combines @b LocalDate::MAX and @b LocalTime::MAX.
             * This could be used by an application as a "far future" date-time.
             */
            static LocalDateTime const MAX;

            /**
             * The epoch year @c LocalDateTime, '1970-01-01T00:00:00:00Z'.
             */
            static LocalDateTime const EPOCH;

        private:
            /**
             * The date part.
             */
            LocalDate date;

            /**
             * The time part.
             */
            LocalTime time;

        public:
            //-----------------------------------------------------------------------
            /**
             * Obtains the current date-time from the system clock in the default time-zone.
             * <p>
             * This will query the <b> system clock</b> in the default
             * time-zone to obtain the current date-time.
             * </p>
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             * </p>
             * @return the current date-time using the system clock and default time-zone
             */
            static LocalDateTime now();

            /**
             * Obtains the current date-time from the system clock in the specified time-zone.
             * <p>
             * This will query the <b> system clock</b> to obtain the current date-time.
             * Specifying the time-zone avoids dependence on the default time-zone.
             * </p>
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             * </p>
             * @param zone  the zone ID to use
             * @return the current date-time using the system clock
             */
            static LocalDateTime now(ZoneId const& zone);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of @c LocalDateTime from year, month,
             * day, hour and minute, setting the second and nanosecond to zero.
             * <p>
             * This returns a @c LocalDateTime with the specified year, month,
             * day-of-month, hour and minute.
             * The day must be valid for the year and month, otherwise an exception will be thrown.
             * The second and nanosecond fields will be set to zero.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT LocalDateTime(gint year, LocalDate::Month month, gint dayOfMonth,
                                        gint hour, gint minute);

            /**
             * Obtains an instance of @c LocalDateTime from year, month,
             * day, hour, minute and second, setting the nanosecond to zero.
             * <p>
             * This returns a @c LocalDateTime with the specified year, month,
             * day-of-month, hour, minute and second.
             * The day must be valid for the year and month, otherwise an exception will be thrown.
             * The nanosecond field will be set to zero.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT LocalDateTime(gint year, LocalDate::Month month, gint dayOfMonth,
                                        gint hour, gint minute, gint second);

            /**
             * Obtains an instance of @c LocalDateTime from year, month,
             * day, hour, minute, second and nanosecond.
             * <p>
             * This returns a @c LocalDateTime with the specified year, month,
             * day-of-month, hour, minute, second and nanosecond.
             * The day must be valid for the year and month, otherwise an exception will be thrown.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT LocalDateTime(gint year, LocalDate::Month month, gint dayOfMonth,
                                        gint hour, gint minute, gint second, gint nanoOfSecond);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of @c LocalDateTime from year, month,
             * day, hour and minute, setting the second and nanosecond to zero.
             * <p>
             * This returns a @c LocalDateTime with the specified year, month,
             * day-of-month, hour and minute.
             * The day must be valid for the year and month, otherwise an exception will be thrown.
             * The second and nanosecond fields will be set to zero.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT LocalDateTime(gint year, gint month, gint dayOfMonth,
                                        gint hour, gint minute);

            /**
             * Obtains an instance of @c LocalDateTime from year, month,
             * day, hour, minute and second, setting the nanosecond to zero.
             * <p>
             * This returns a @c LocalDateTime with the specified year, month,
             * day-of-month, hour, minute and second.
             * The day must be valid for the year and month, otherwise an exception will be thrown.
             * The nanosecond field will be set to zero.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT LocalDateTime(gint year, gint month, gint dayOfMonth,
                                        gint hour, gint minute, gint second);

            /**
             * Obtains an instance of @c LocalDateTime from year, month,
             * day, hour, minute, second and nanosecond.
             * <p>
             * This returns a @c LocalDateTime with the specified year, month,
             * day-of-month, hour, minute, second and nanosecond.
             * The day must be valid for the year and month, otherwise an exception will be thrown.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return the local date-time
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT LocalDateTime(gint year, gint month, gint dayOfMonth,
                                        gint hour, gint minute, gint second, gint nanoOfSecond);

            /**
             * Obtains an instance of @c LocalDateTime from a date and time.
             *
             * @param date  the local date
             * @param time  the local time
             * @return the local date-time
             */
            CORE_EXPLICIT LocalDateTime(LocalDate const& date, LocalTime const& time);

            /**
             * Obtains an instance of @c LocalDateTime using seconds from the
             * epoch of 1970-01-01T00:00:00Z.
             * <p>
             * This allows the @b epoch-second field
             * to be converted to a local date-time. This is primarily intended for
             * low-level conversions rather than general application usage.
             * </p>
             * @param epochSecond  the number of seconds from the epoch of 1970-01-01T00:00:00Z
             * @param nanoOfSecond  the nanosecond within the second, from 0 to 999,999,999
             * @return the local date-time
             * @throws DateTimeException if the result exceeds the supported range,
             *  or if the nano-of-second is invalid
             */
            static LocalDateTime ofEpochSecond(glong epochSecond, gint nanoOfSecond);

            /**
             * Obtains an instance of @c LocalDateTime using seconds from the
             * epoch of 1970-01-01T00:00:00Z.
             * <p>
             * This allows the @b epoch-second field
             * to be converted to a local date-time. This is primarily intended for
             * low-level conversions rather than general application usage.
             * </p>
             * @param epochSecond  the number of seconds from the epoch of 1970-01-01T00:00:00Z
             * @param nanoOfSecond  the nanosecond within the second, from 0 to 999,999,999
             * @param offset  the zone offset
             * @return the local date-time
             * @throws DateTimeException if the result exceeds the supported range,
             *  or if the nano-of-second is invalid
             */
            static LocalDateTime ofEpochSecond(glong epochSecond, gint nanoOfSecond, ZoneOffset const& offset);

            //-----------------------------------------------------------------------
            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this date-time can be queried for the specified field.
             * If false, then calling the @b get and <b> with(ChronoField, long) </b>
             * methods will throw an exception.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The supported fields are:
             *
             * @li @c NANO_OF_SECOND
             * @li @c NANO_OF_DAY
             * @li @c MICRO_OF_SECOND
             * @li @c MICRO_OF_DAY
             * @li @c MILLI_OF_SECOND
             * @li @c MILLI_OF_DAY
             * @li @c SECOND_OF_MINUTE
             * @li @c SECOND_OF_DAY
             * @li @c MINUTE_OF_HOUR
             * @li @c MINUTE_OF_DAY
             * @li @c HOUR_OF_AMPM
             * @li @c CLOCK_HOUR_OF_AMPM
             * @li @c HOUR_OF_DAY
             * @li @c CLOCK_HOUR_OF_DAY
             * @li @c AMPM_OF_DAY
             * @li @c DAY_OF_WEEK
             * @li @c ALIGNED_DAY_OF_WEEK_IN_MONTH
             * @li @c ALIGNED_DAY_OF_WEEK_IN_YEAR
             * @li @c DAY_OF_MONTH
             * @li @c DAY_OF_YEAR
             * @li @c EPOCH_DAY
             * @li @c ALIGNED_WEEK_OF_MONTH
             * @li @c ALIGNED_WEEK_OF_YEAR
             * @li @c MONTH_OF_YEAR
             * @li @c PROLEPTIC_MONTH
             * @li @c YEAR_OF_ERA
             * @li @c YEAR
             * @li @c ERA
             * All other @c ChronoField instances will return false.
             * </p>
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this date-time, false if not
             */
            gbool isSupported(ChronoField field) const override;

            /**
             * Checks if the specified unit is supported.
             * <p>
             * This checks if the specified unit can be added to, or subtracted from, this date-time.
             * If false, then calling the <b> plus(long, ChronoUnit)</b> and
             * <b> minus(long, ChronoUnit) </b> methods will throw an exception.
             * </p>
             * <p>
             * If the unit is a @b ChronoUnit then the query is implemented here.
             * The supported units are:
             *
             * @li @c NANOS
             * @li @c MICROS
             * @li @c MILLIS
             * @li @c SECONDS
             * @li @c MINUTES
             * @li @c HOURS
             * @li @c HALF_DAYS
             * @li @c DAYS
             * @li @c WEEKS
             * @li @c MONTHS
             * @li @c YEARS
             * @li @c DECADES
             * @li @c CENTURIES
             * @li @c MILLENNIA
             * @li @c ERAS
             *
             * All other @c ChronoUnit instances will return false
             * </p>
             * @param unit  the unit to check, null returns false
             * @return true if the unit can be added/subtracted, false if not
             */
            gbool isSupported(ChronoUnit unit) const override;

            /**
             * Gets the value of the specified field from this date-time as an @c int.
             * <p>
             * This queries this date-time for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The <b> supported fields</b> will return valid
             * values based on this date-time, except @c NANO_OF_DAY, @c MICRO_OF_DAY,
             * @c EPOCH_DAY and @c PROLEPTIC_MONTH which are too large to fit in
             * an @c int and throw an @c TemporalException.
             * All other @c ChronoField instances will throw an @c TemporalException.
             * </p>
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained or
             *         the value is outside the range of valid values for the field
             * @throws TemporalException if the field is not supported or
             *         the range of values exceeds an @c int
             * @throws ArithmeticException if numeric overflow occurs
             */
            gint get(ChronoField field) const override;

            /**
             * Gets the value of the specified field from this date-time as a @c long.
             * <p>
             * This queries this date-time for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The <b> supported fields</b> will return valid
             * values based on this date-time.
             * All other @c ChronoField instances will throw an @c TemporalException.
             * </p>
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong getLong(ChronoField field) const override;

            //-----------------------------------------------------------------------
            /**
             * Gets the @c LocalDate part of this date-time.
             * <p>
             * This returns a @c LocalDate with the same year, month and day
             * as this date-time.
             * </p>
             * @return the date part of this date-time
             */
            LocalDate toLocalDate() const;

            /**
             * Gets the year field.
             * <p>
             * This method returns the primitive @c int value for the year.
             * </p>
             * <p>
             * The year returned by this method is proleptic as per @c get(YEAR).
             * To obtain the year-of-era, use @c get(YEAR_OF_ERA).
             * </p>
             * @return the year, from MIN_YEAR to MAX_YEAR
             */
            gint year() const;

            /**
             * Gets the month-of-year field using the @c Month enum.
             * <p>
             * This method returns the enum @b Month for the month.
             * This avoids confusion as to what @c int values mean.
             * If you need access to the primitive @c int value then the enum
             * provides the <b> int value </b>.
             * </p>
             * @return the month-of-year
             */
            LocalDate::Month month() const;

            /**
             * Gets the day-of-month field.
             * <p>
             * This method returns the primitive @c int value for the day-of-month.
             * </p>
             * @return the day-of-month, from 1 to 31
             */
            gint dayOfMonth() const;

            /**
             * Gets the day-of-year field.
             * <p>
             * This method returns the primitive @c int value for the day-of-year.
             * </p>
             * @return the day-of-year, from 1 to 365, or 366 in a leap year
             */
            gint dayOfYear() const;

            /**
             * Gets the day-of-week field, which is an enum @c DayOfWeek.
             * <p>
             * This method returns the enum @b DayOfWeek for the day-of-week.
             * This avoids confusion as to what @c int values mean.
             * If you need access to the primitive @c int value then the enum
             * provides the <b> int value</b>.
             * </p>
             * <p>
             * Additional information can be obtained from the @c DayOfWeek.
             * This includes textual names of the values.
             * </p>
             * @return the day-of-week
             */
            LocalDate::DayOfWeek dayOfWeek() const;

            //-----------------------------------------------------------------------
            /**
             * Gets the @c LocalTime part of this date-time.
             * <p>
             * This returns a @c LocalTime with the same hour, minute, second and
             * nanosecond as this date-time.
             * </p>
             * @return the time part of this date-time
             */
            LocalTime toLocalTime() const;

            /**
             * Gets the hour-of-day field.
             *
             * @return the hour-of-day, from 0 to 23
             */
            gint hour() const;

            /**
             * Gets the minute-of-hour field.
             *
             * @return the minute-of-hour, from 0 to 59
             */
            gint minute() const;

            /**
             * Gets the second-of-minute field.
             *
             * @return the second-of-minute, from 0 to 59
             */
            gint second() const;

            /**
             * Gets the nano-of-second field.
             *
             * @return the nano-of-second, from 0 to 999,999,999
             */
            gint nano() const;

            /**
             * Returns a copy of this date-time with the specified field set to a new value.
             * <p>
             * This returns a @c LocalDateTime, based on this one, with the value
             * for the specified field changed.
             * This can be used to change any supported field, such as the year, month or day-of-month.
             * If it is not possible to set the value, because the field is not supported or for
             * some other reason, an exception is thrown.
             * </p>
             * <p>
             * In some cases, changing the specified field can cause the resulting date-time to become invalid,
             * such as changing the month from 31st January to February would make the day-of-month invalid.
             * In cases like this, the field is responsible for resolving the date. Typically, it will choose
             * the previous valid date, which would be the last valid day of February in this example.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the adjustment is implemented here.
             * The <b> supported fields</b> will behave as per
             * the matching method on @b LocalDate or @b LocalTime.
             * All other @c ChronoField instances will throw an @c TemporalException.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param field  the field to set in the result
             * @param newValue  the new value of the field in the result
             * @return a @c LocalDateTime based on @c this with the specified field set
             * @throws DateTimeException if the field cannot be set
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalDateTime with(ChronoField field, glong newValue) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDateTime with the year altered.
             * <p>
             * The time does not affect the calculation and will be the same in the result.
             * If the day-of-month is invalid for the year, it will be changed to the last valid day of the month.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param year  the year to set in the result, from MIN_YEAR to MAX_YEAR
             * @return a @c LocalDateTime based on this date-time with the requested year
             * @throws DateTimeException if the year value is invalid
             */
            LocalDateTime withYear(gint year) const;

            /**
             * Returns a copy of this @c LocalDateTime with the month-of-year altered.
             * <p>
             * The time does not affect the calculation and will be the same in the result.
             * If the day-of-month is invalid for the year, it will be changed to the last valid day of the month.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param month  the month-of-year to set in the result, from 1 (January) to 12 (December)
             * @return a @c LocalDateTime based on this date-time with the requested month
             * @throws DateTimeException if the month-of-year value is invalid
             */
            LocalDateTime withMonth(gint month) const;

            /**
             * Returns a copy of this @c LocalDateTime with the day-of-month altered.
             * <p>
             * If the resulting date-time is invalid, an exception is thrown.
             * The time does not affect the calculation and will be the same in the result.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param dayOfMonth  the day-of-month to set in the result, from 1 to 28-31
             * @return a @c LocalDateTime based on this date-time with the requested day
             * @throws DateTimeException if the day-of-month value is invalid,
             *  or if the day-of-month is invalid for the month-year
             */
            LocalDateTime withDayOfMonth(gint dayOfMonth) const;

            /**
             * Returns a copy of this @c LocalDateTime with the day-of-year altered.
             * <p>
             * If the resulting date-time is invalid, an exception is thrown.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param dayOfYear  the day-of-year to set in the result, from 1 to 365-366
             * @return a @c LocalDateTime based on this date with the requested day
             * @throws DateTimeException if the day-of-year value is invalid,
             *  or if the day-of-year is invalid for the year
             */
            LocalDateTime withDayOfYear(gint dayOfYear) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDateTime with the hour-of-day altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param hour  the hour-of-day to set in the result, from 0 to 23
             * @return a @c LocalDateTime based on this date-time with the requested hour
             * @throws DateTimeException if the hour value is invalid
             */
            LocalDateTime withHour(gint hour) const;

            /**
             * Returns a copy of this @c LocalDateTime with the minute-of-hour altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param minute  the minute-of-hour to set in the result, from 0 to 59
             * @return a @c LocalDateTime based on this date-time with the requested minute
             * @throws DateTimeException if the minute value is invalid
             */
            LocalDateTime withMinute(gint minute) const;

            /**
             * Returns a copy of this @c LocalDateTime with the second-of-minute altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param second  the second-of-minute to set in the result, from 0 to 59
             * @return a @c LocalDateTime based on this date-time with the requested second
             * @throws DateTimeException if the second value is invalid
             */
            LocalDateTime withSecond(gint second) const;

            /**
             * Returns a copy of this @c LocalDateTime with the nano-of-second altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanoOfSecond  the nano-of-second to set in the result, from 0 to 999,999,999
             * @return a @c LocalDateTime based on this date-time with the requested nanosecond
             * @throws DateTimeException if the nano value is invalid
             */
            LocalDateTime withNano(gint nanoOfSecond) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDateTime with the time truncated.
             * <p>
             * Truncation returns a copy of the original date-time with fields
             * smaller than the specified unit set to zero.
             * For example, truncating with the @b minutes unit
             * will set the second-of-minute and nano-of-second field to zero.
             * </p>
             * <p>
             * The unit must have a @em duration
             * that divides into the length of a standard day without remainder.
             * This includes all supplied time units on @b ChronoUnit and
             * @b DAYS. Other units throw an exception.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param unit  the unit to truncate to
             * @return a @c LocalDateTime based on this date-time with the time truncated
             * @throws DateTimeException if unable to truncate
             * @throws TemporalException if the unit is not supported
             */
            LocalDateTime truncateTo(ChronoUnit unit) const;

            /**
             * Returns a copy of this date-time with the specified amount added.
             * <p>
             * This returns a @c LocalDateTime, based on this one, with the amount
             * in terms of the unit added. If it is not possible to add the amount, because the
             * unit is not supported or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoUnit then the addition is implemented here.
             * Date units are added as per <b> LocalDate::plus(long, ChronoUnit)</b>.
             * Time units are added as per <b> LocalTime::plus(long, ChronoUnit)</b> with
             * any overflow in days added equivalent to using @b #plusDays(long).
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param amountToAdd  the amount of the unit to add to the result, may be negative
             * @param unit  the unit of the amount to add
             * @return a @c LocalDateTime based on this date-time with the specified amount added
             * @throws DateTimeException if the addition cannot be made
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalDateTime plus(glong amountToAdd, ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDateTime with the specified number of years added.
             * <p>
             * This method adds the specified amount to the years field in three steps:
             *
             * @li Add the input years to the year field
             * @li Check if the resulting date would be invalid
             * @li Adjust the day-of-month to the last valid day if necessary
             *
             * </p>
             * <p>
             * For example, 2008-02-29 (leap year) plus one year would result in the
             * invalid date 2009-02-29 (standard year). Instead of returning an invalid
             * result, the last valid day of the month, 2009-02-28, is selected instead.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param years  the years to add, may be negative
             * @return a @c LocalDateTime based on this date-time with the years added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime plusYears(glong years) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of months added.
             * <p>
             * This method adds the specified amount to the months field in three steps:
             *
             * @li Add the input months to the month-of-year field
             * @li Check if the resulting date would be invalid
             * @li Adjust the day-of-month to the last valid day if necessary
             *
             * </p>
             * <p>
             * For example, 2007-03-31 plus one month would result in the invalid date
             * 2007-04-31. Instead of returning an invalid result, the last valid day
             * of the month, 2007-04-30, is selected instead.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param months  the months to add, may be negative
             * @return a @c LocalDateTime based on this date-time with the months added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime plusMonths(glong months) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of weeks added.
             * <p>
             * This method adds the specified amount in weeks to the days field incrementing
             * the month and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * </p>
             * <p>
             * For example, 2008-12-31 plus one week would result in 2009-01-07.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param weeks  the weeks to add, may be negative
             * @return a @c LocalDateTime based on this date-time with the weeks added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime plusWeeks(glong weeks) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of days added.
             * <p>
             * This method adds the specified amount to the days field incrementing the
             * month and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * </p>
             * <p>
             * For example, 2008-12-31 plus one day would result in 2009-01-01.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param days  the days to add, may be negative
             * @return a @c LocalDateTime based on this date-time with the days added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime plusDays(glong days) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDateTime with the specified number of hours added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param hours  the hours to add, may be negative
             * @return a @c LocalDateTime based on this date-time with the hours added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime plusHours(glong hours) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of minutes added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param minutes  the minutes to add, may be negative
             * @return a @c LocalDateTime based on this date-time with the minutes added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime plusMinutes(glong minutes) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of seconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param seconds  the seconds to add, may be negative
             * @return a @c LocalDateTime based on this date-time with the seconds added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime plusSeconds(glong seconds) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of nanoseconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanos  the nanos to add, may be negative
             * @return a @c LocalDateTime based on this date-time with the nanoseconds added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime plusNanos(glong nanos) const;

            /**
             * Returns a copy of this date-time with the specified amount subtracted.
             * <p>
             * This returns a @c LocalDateTime, based on this one, with the amount
             * in terms of the unit subtracted. If it is not possible to subtract the amount,
             * because the unit is not supported or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * This method is equivalent to <b> plus(long, ChronoUnit)</b> with the amount negated.
             * See that method for a full description of how addition, and thus subtraction, works.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
             * @param unit  the unit of the amount to subtract
             * @return a @c LocalDateTime based on this date-time with the specified amount subtracted
             * @throws DateTimeException if the subtraction cannot be made
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalDateTime minus(glong amountToSubtract, ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDateTime with the specified number of years subtracted.
             * <p>
             * This method subtracts the specified amount from the years field in three steps:
             *
             * @li Subtract the input years from the year field
             * @li Check if the resulting date would be invalid
             * @li Adjust the day-of-month to the last valid day if necessary
             * </p>
             * <p>
             * For example, 2008-02-29 (leap year) minus one year would result in the
             * invalid date 2007-02-29 (standard year). Instead of returning an invalid
             * result, the last valid day of the month, 2007-02-28, is selected instead.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param years  the years to subtract, may be negative
             * @return a @c LocalDateTime based on this date-time with the years subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime minusYears(glong years) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of months subtracted.
             * <p>
             * This method subtracts the specified amount from the months field in three steps:
             *
             * @li Subtract the input months from the month-of-year field
             * @li Check if the resulting date would be invalid
             * @li Adjust the day-of-month to the last valid day if necessary
             * </p>
             * <p>
             * For example, 2007-03-31 minus one month would result in the invalid date
             * 2007-02-31. Instead of returning an invalid result, the last valid day
             * of the month, 2007-02-28, is selected instead.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param months  the months to subtract, may be negative
             * @return a @c LocalDateTime based on this date-time with the months subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime minusMonths(glong months) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of weeks subtracted.
             * <p>
             * This method subtracts the specified amount in weeks from the days field decrementing
             * the month and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * </p>
             * <p>
             * For example, 2009-01-07 minus one week would result in 2008-12-31.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param weeks  the weeks to subtract, may be negative
             * @return a @c LocalDateTime based on this date-time with the weeks subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime minusWeeks(glong weeks) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of days subtracted.
             * <p>
             * This method subtracts the specified amount from the days field decrementing the
             * month and year fields as necessary to ensure the result remains valid.
             * The result is only invalid if the maximum/minimum year is exceeded.
             * </p>
             * <p>
             * For example, 2009-01-01 minus one day would result in 2008-12-31.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param days  the days to subtract, may be negative
             * @return a @c LocalDateTime based on this date-time with the days subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime minusDays(glong days) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDateTime with the specified number of hours subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param hours  the hours to subtract, may be negative
             * @return a @c LocalDateTime based on this date-time with the hours subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime minusHours(glong hours) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of minutes subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param minutes  the minutes to subtract, may be negative
             * @return a @c LocalDateTime based on this date-time with the minutes subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime minusMinutes(glong minutes) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of seconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param seconds  the seconds to subtract, may be negative
             * @return a @c LocalDateTime based on this date-time with the seconds subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime minusSeconds(glong seconds) const;

            /**
             * Returns a copy of this @c LocalDateTime with the specified number of nanoseconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanos  the nanos to subtract, may be negative
             * @return a @c LocalDateTime based on this date-time with the nanoseconds subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            LocalDateTime minusNanos(glong nanos) const;

            /**
             * Calculates the amount of time until another date-time in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two @c LocalDateTime
             * objects in terms of a single @c ChronoUnit.
             * The start and end points are @c this and the specified date-time.
             * For example, the amount in days between two date-times can be calculated
             * using @code startDateTime.until(endDateTime, DAYS) @endcode .
             * </p>
             * <p>
             * The calculation returns a whole number, representing the number of
             * complete units between the two date-times.
             * For example, the amount in months between 2012-06-15T00:00 and 2012-08-14T23:59
             * will only be one month as it is one minute short of two months.
             * </p>
             * <p>
             * The calculation is implemented in this method for @b ChronoUnit.
             * The units @c NANOS, @c MICROS, @c MILLIS, @c SECONDS,
             * @c MINUTES, @c HOURS and @c HALF_DAYS, @c DAYS,
             * @c WEEKS, @c MONTHS, @c YEARS, @c DECADES,
             * @c CENTURIES, @c MILLENNIA and @c ERAS are supported.
             * Other @c ChronoUnit values will throw an exception.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param endExclusive  the end date, exclusive, which is converted to a @c LocalDateTime
             * @param unit  the unit to measure the amount in
             * @return the amount of time between this date-time and the end date-time
             * @throws DateTimeException if the amount cannot be calculated, or the end
             *  temporal cannot be converted to a @c LocalDateTime
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong until(const Temporal& endExclusive, ChronoUnit unit) const override;

            /**
             * Converts this date-time to the number of seconds from the epoch
             * of 1970-01-01T00:00:00Z.
             * <p>
             * This combines this local date-time and the specified offset to calculate the
             * epoch-second value, which is the number of elapsed seconds from 1970-01-01T00:00:00Z.
             * Instants on the time-line after the epoch are positive, earlier are negative.
             * </p>
             * <p>
             * This default implementation calculates from the epoch-day of the date and the
             * second-of-day of the time.
             * </p>
             * @return the number of seconds from the epoch of 1970-01-01T00:00:00Z
             */
            glong toEpochSecond() const;

            /**
             * Converts this date-time to the number of seconds from the epoch
             * of 1970-01-01T00:00:00Z.
             * <p>
             * This combines this local date-time and the specified offset to calculate the
             * epoch-second value, which is the number of elapsed seconds from 1970-01-01T00:00:00Z.
             * Instants on the time-line after the epoch are positive, earlier are negative.
             * </p>
             * <p>
             * This default implementation calculates from the epoch-day of the date and the
             * second-of-day of the time.
             * </p>
             * @param offset  the offset to use for the conversion, not null
             * @return the number of seconds from the epoch of 1970-01-01T00:00:00Z
             */
            glong toEpochSecond(ZoneOffset const& offset) const;

            //-----------------------------------------------------------------------
            /**
             * Compares this date-time to another date-time.
             * <p>
             * The comparison is primarily based on the date-time, from earliest to latest.
             * It is "consistent with equals", as defined by @b Comparable.
             * </p>
             * <p>
             * If all the date-times being compared are instances of @c LocalDateTime,
             * then the comparison will be entirely based on the date-time.
             * If some dates being compared are in different chronologies, then the
             * chronology is also considered, see @b ChronoLocalDateTime::compareTo.
             * </p>
             * @param other  the other date-time to compare to
             * @return the comparator value, that is the comparison of this local date-time with
             *          the @c other local date-time and this chronology with the @c other chronology,
             *          in order, returning the first non-zero result, and otherwise returning zero
             * @see isBefore
             * @see isAfter
             */
            gint compareTo(const LocalDateTime& other) const override;

            /**
             * Checks if this date-time is after the specified date-time.
             * <p>
             * This checks to see if this date-time represents a point on the
             * local time-line after the other date-time.
             * @code
             *   LocalDateTime a = LocalDateTime(2012, 6, 30, 12, 00);
             *   LocalDateTime b = LocalDateTime(2012, 7, 1, 12, 00);
             *   a.isAfter(b) == false
             *   a.isAfter(a) == false
             *   b.isAfter(a) == true
             * @endcode
             * </p>
             * <p>
             * This method only considers the position of the two date-times on the local time-line.
             * It does not take into account the chronology, or calendar system.
             * This is different from the comparison in @b compareTo(ChronoLocalDateTime),
             * but is the same approach as @b ChronoLocalDateTime::timeLineOrder().
             * </p>
             * @param other  the other date-time to compare to
             * @return true if this date-time is after the specified date-time
             */
            gbool isAfter(LocalDateTime const& other) const;

            /**
             * Checks if this date-time is before the specified date-time.
             * <p>
             * This checks to see if this date-time represents a point on the
             * local time-line before the other date-time.
             * @code
             *   LocalDateTime a = LocalDateTime.of(2012, 6, 30, 12, 00);
             *   LocalDateTime b = LocalDateTime.of(2012, 7, 1, 12, 00);
             *   a.isBefore(b) == true
             *   a.isBefore(a) == false
             *   b.isBefore(a) == false
             * @endcode
             * </p>
             * <p>
             * This method only considers the position of the two date-times on the local time-line.
             * It does not take into account the chronology, or calendar system.
             * This is different from the comparison in @b compareTo(ChronoLocalDateTime),
             * but is the same approach as @b ChronoLocalDateTime::timeLineOrder().
             * </p>
             * @param other  the other date-time to compare to
             * @return true if this date-time is before the specified date-time
             */
            gbool isBefore(LocalDateTime const& other) const;

            /**
             * Queries this date-time using the specified query.
             * <p>
             * This queries this date-time using the specified query strategy object.
             * The @c TemporalQuery object defines the logic to be used to
             * obtain the result. Read the documentation of the query to understand
             * what the result of this method will be.
             * </p>
             * <p>
             * The result of this method is obtained by invoking the
             * @b TemporalQuery::queryFrom(Temporal) method on the
             * specified query passing @c this as the argument.
             * </p>
             * @param query  the query to invoke, not null
             * @return the query result, no option may be returned (defined by the query)
             * @throws DateTimeException if unable to query (defined by the query)
             * @throws ArithmeticException if numeric overflow occurs (defined by the query)
             */
            util::Optional<> query(TemporalQuery const& query) const override;

            /**
             * Obtains an instance of @c LocalDateTime from a temporal object.
             * <p>
             * This obtains a local date-time based on the specified temporal.
             * A @c Temporal represents an arbitrary set of date and time information,
             * which this factory converts to an instance of @c LocalDateTime.
             * </p>
             * <p>
             * The conversion extracts and combines the @c LocalDate and the
             * @c LocalTime from the temporal object.
             * Implementations are permitted to perform optimizations such as accessing
             * those fields that are equivalent to the relevant objects.
             * </p>
             * <p>
             * This method matches the signature of the functional interface @b TemporalQuery
             * allowing it to be used as a query via method reference, @c LocalDateTime::from.
             * </p>
             * @param temporal  the temporal object to convert, not null
             * @return the local date-time, not null
             * @throws DateTimeException if unable to convert to a @c LocalDateTime
             */
            static LocalDateTime from(Temporal const& temporal);

            //-----------------------------------------------------------------------
            /**
             * Checks if this date-time is equal to another date-time.
             * <p>
             * Compares this @c LocalDateTime with another ensuring that the date-time is the same.
             * Only objects of type @c LocalDateTime are compared, other types return false.
             * </p>
             * @param obj  the object to check, null returns false
             * @return true if this is equal to the other date-time
             */
            gbool equals(const Object& obj) const override;

            /**
             * A hash code for this date-time.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            //-----------------------------------------------------------------------
            /**
             * Outputs this date-time as a @c String, such as @c 2007-12-03T10:15:30.
             * <p>
             * The output will be one of the following ISO-8601 formats:
             *
             * @li @c uuuu-MM-dd'T'HH:mm
             * @li @c uuuu-MM-dd'T'HH:mm:ss
             * @li @c uuuu-MM-dd'T'HH:mm:ss.SSS
             * @li @c uuuu-MM-dd'T'HH:mm:ss.SSSSSS
             * @li @c uuuu-MM-dd'T'HH:mm:ss.SSSSSSSSS
             *
             * The format used will be the shortest that outputs the full value of
             * the time when the omitted parts are implied to be zero.
             * </p>
             * @return a string representation of this date-time
             */
            String toString() const override;

            Object& clone() const override;
        };
    } // time
} // core

#endif //CORE24_LOCALDATETIME_H
