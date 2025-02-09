//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_LOCALTIME_H
#define CORE24_LOCALTIME_H

#include <core/time/Temporal.h>

namespace core {
    namespace time {
        /**
         * A time without a time-zone in the ISO-8601 calendar system,
         * such as @c 10:15:30.
         * <p>
         * @c LocalTime is an immutable date-time object that represents a time,
         * often viewed as hour-minute-second.
         * Time is represented to nanosecond precision.
         * For example, the value "13:45.30.123456789" can be stored in a @c LocalTime.
         * </p>
         * <p>
         * This class does not store or represent a date or time-zone.
         * Instead, it is a description of the local time as seen on a wall clock.
         * It cannot represent an instant on the time-line without additional information
         * such as an offset or time-zone.
         * </p>
         * <p>
         * The ISO-8601 calendar system is the modern civil calendar system used today
         * in most of the world. This API assumes that all calendar systems use the same
         * representation, this class, for time-of-day.
         * </p>
         * <p>
         * This is a <b>value-based</b> class; programmers should treat instances that are
         * @em equal as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * The @c equals method should be used for comparisons.
         * </p>
         * @note This class is immutable and thread-safe.
         */
        class LocalTime final : public virtual Temporal, public virtual Comparable<LocalTime> {
        public:
            /**
             * The minimum supported @c LocalTime, '00:00'.
             * This is the time of midnight at the start of the day.
             */
            static LocalTime const MIN;

            /**
             * The maximum supported @c LocalTime, '23:59:59.999999999'.
             * This is the time just before midnight at the end of the day.
             */
            static LocalTime const MAX;

            /**
             * The time of midnight at the start of the day, '00:00'.
             */
            static LocalTime const MIDNIGHT;

            /**
             * The time of noon in the middle of the day, '12:00'.
             */
            static LocalTime const NOON;

            /**
             * Hours per day.
             */
            static CORE_FAST gint HOURS_PER_DAY = 24;

            /**
             * Minutes per hour.
             */
            static CORE_FAST gint MINUTES_PER_HOUR = 60;
            /**
             * Minutes per day.
             */
            static CORE_FAST gint MINUTES_PER_DAY = MINUTES_PER_HOUR * HOURS_PER_DAY;
            /**
             * Seconds per minute.
             */
            static CORE_FAST gint SECONDS_PER_MINUTE = 60;

            /**
             * Seconds per hour.
             */
            static CORE_FAST gint SECONDS_PER_HOUR = SECONDS_PER_MINUTE * MINUTES_PER_HOUR;

            /**
             * Seconds per day.
             */
            static CORE_FAST gint SECONDS_PER_DAY = SECONDS_PER_HOUR * HOURS_PER_DAY;

            /**
             * Milliseconds per second.
             */
            static CORE_FAST glong MILLIS_PER_SECOND = 1000L;

            /**
             * Milliseconds per day.
             */
            static CORE_FAST glong MILLIS_PER_DAY = MILLIS_PER_SECOND * SECONDS_PER_DAY;

            /**
             * Microseconds per second.
             */
            static CORE_FAST glong MICROS_PER_SECOND = 1000000L;

            /**
             * Microseconds per day.
             */
            static CORE_FAST glong MICROS_PER_DAY = MICROS_PER_SECOND * SECONDS_PER_DAY;

            /**
             * Nanos per millisecond.
             */
            static CORE_FAST glong NANOS_PER_MILLI = 1000000L;

            /**
             * Nanos per second.
             */
            static CORE_FAST glong NANOS_PER_SECOND = 1000000000L;

            /**
             * Nanos per minute.
             */
            static CORE_FAST glong NANOS_PER_MINUTE = NANOS_PER_SECOND * SECONDS_PER_MINUTE;

            /**
             * Nanos per hour.
             */
            static CORE_FAST glong NANOS_PER_HOUR = NANOS_PER_MINUTE * MINUTES_PER_HOUR;

            /**
             * Nanos per day.
             */
            static CORE_FAST glong NANOS_PER_DAY = NANOS_PER_HOUR * HOURS_PER_DAY;

        private:
            /**
             * The hour (5 bits), The minute (6 bits), The second(6 bits), The nanosecond (30 bits) = 47 bits.
             *
             * <p> 23:59:59.999999999 = 0b10111 111011 111011 111011100110101100100111111111 </p>
             * <p> 00:00:00.000000000 = 0b00000 000000 000000 000000000000000000000000000000 </p>
             */
            glong hms = 0;

            enum {
                HOUR_OFFSET       = 42,
                MINUTE_OFFSET     = 36,
                SECOND_OFFSET     = 30,
                NANOSECOND_OFFSET = 0,
                HOUR_MASK         = 0x7C0000000000,
                MINUTE_MASK       = 0x3F000000000,
                SECOND_MASK       = 0xFC0000000,
                NANOSECOND_MASK   = 0x3FFFFFFF,
            };

        public:
            //-----------------------------------------------------------------------
            /**
             * Obtains the current time from the system clock in the default time-zone.
             * <p>
             * This will query the <b>system clock</b> in the default
             * time-zone to obtain the current time.
             * </p>
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             * </p>
             *
             * @return the current time using the system clock and default time-zone
             */
            static LocalTime now();

            /**
             * Obtains the current time from the system clock in the specified time-zone.
             * <p>
             * This will query the <b> system clock</b> to obtain the current time.
             * Specifying the time-zone avoids dependence on the default time-zone.
             * </p>
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             * </p>
             * @param zone  the zone ID to use.
             * @return the current time using the system clock.
             */
            static LocalTime now(ZoneId const& zone);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of @c LocalTime from a second-of-day value.
             * <p>
             * This returns a @c LocalTime with the specified second-of-day.
             * The nanosecond field will be set to zero.
             * </p>
             * @param secondOfDay  the second-of-day, from @c 0 to @code 24 * 60 * 60 - 1 @endcode
             * @return the local time
             * @throws DateTimeException if the second-of-day value is invalid
             */
            static LocalTime ofSecondOfDay(glong secondOfDay);

            /**
             * Obtains an instance of @c LocalTime from a nanos-of-day value.
             * <p>
             * This returns a @c LocalTime with the specified nanosecond-of-day.
             * </p>
             * @param nanoOfDay  the nano of day, from @c 0 to @code 24 * 60 * 60 * 1,000,000,000 - 1 @endcode
             * @return the local time
             * @throws DateTimeException if the nanos of day value is invalid
             */
            static LocalTime ofNanoOfDay(glong nanoOfDay);

            //-----------------------------------------------------------------------
            /**
             * Obtains an instance of @c LocalTime from an hour and minute.
             * <p>
             * This returns a @c LocalTime with the specified hour and minute.
             * The second and nanosecond fields will be set to zero.
             * </p>
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @return the local time
             * @throws DateTimeException if the value of any field is out of range
             */
            CORE_EXPLICIT LocalTime(gint hour, gint minute);

            /**
             * Obtains an instance of @c LocalTime from an hour, minute and second.
             * <p>
             * This returns a @c LocalTime with the specified hour, minute and second.
             * The nanosecond field will be set to zero.
             * </p>
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @return the local time
             * @throws DateTimeException if the value of any field is out of range
             */
            CORE_EXPLICIT LocalTime(gint hour, gint minute, gint second);

            /**
             * Obtains an instance of @c LocalTime from an hour, minute, second and nanosecond.
             * <p>
             * This returns a @c LocalTime with the specified hour, minute, second and nanosecond.
             * </p>
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return the local time
             * @throws DateTimeException if the value of any field is out of range
             */
            CORE_EXPLICIT LocalTime(gint hour, gint minute, gint second, gint nanoOfSecond);

            //-----------------------------------------------------------------------
            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this time can be queried for the specified field.
             * If false, then calling the @b range, @b get and @b with(TemporalField, long)
             * methods will throw an exception.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The supported fields are:
             * </p>
             * - @c NANO_OF_SECOND <br/>
             * - @c NANO_OF_DAY <br/>
             * - @c MICRO_OF_SECOND <br/>
             * - @c MICRO_OF_DAY <br/>
             * - @c MILLI_OF_SECOND <br/>
             * - @c MILLI_OF_DAY <br/>
             * - @c SECOND_OF_MINUTE <br/>
             * - @c SECOND_OF_DAY <br/>
             * - @c MINUTE_OF_HOUR <br/>
             * - @c MINUTE_OF_DAY <br/>
             * - @c HOUR_OF_AMPM <br/>
             * - @c CLOCK_HOUR_OF_AMPM <br/>
             * - @c HOUR_OF_DAY <br/>
             * - @c CLOCK_HOUR_OF_DAY <br/>
             * - @c AMPM_OF_DAY <br/>
             *
             * All other @c ChronoField instances will return false.
             *
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this time, false if not
             */
            gbool isSupported(ChronoField field) const override;

            /**
             * Checks if the specified unit is supported.
             * <p>
             * This checks if the specified unit can be added to, or subtracted from, this time.
             * If false, then calling the <b> plus(long, ChronoUnit) </b> and
             * @b minus methods will throw an exception.
             * </p>
             * <p>
             * If the unit is a @b ChronoUnit then the query is implemented here.
             * The supported units are:
             * </p>
             * - @c NANOS <br/>
             * - @c MICROS <br/>
             * - @c MILLIS <br/>
             * - @c SECONDS <br/>
             * - @c MINUTES <br/>
             * - @c HOURS <br/>
             * - @c HALF_DAYS <br/>
             *
             * All other @c ChronoUnit instances will return false.
             *
             * @param unit  the unit to check, null returns false
             * @return true if the unit can be added/subtracted, false if not
             */
            gbool isSupported(ChronoUnit unit) const override;

            /**
             * Gets the value of the specified field from this time as a @c gint.
             * <p>
             * This queries this time for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The <b> supported fields</b> will return valid
             * values based on this time, except @c NANO_OF_DAY and @c MICRO_OF_DAY
             * which are too large to fit in an @c int and throw an @c TemporalException.
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
             * Gets the value of the specified field from this time as a @c glong.
             * <p>
             * This queries this time for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The <b> supported fields</b> will return valid
             * values based on this time.
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
             * Returns a copy of this time with the specified field set to a new value.
             * <p>
             * This returns a @c LocalTime, based on this one, with the value
             * for the specified field changed.
             * This can be used to change any supported field, such as the hour, minute or second.
             * If it is not possible to set the value, because the field is not supported or for
             * some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the adjustment is implemented here.
             * The supported fields behave as follows:
             *
             * - @c NANO_OF_SECOND -
             *  Returns a @c LocalTime with the specified nano-of-second.
             *  The hour, minute and second will be unchanged. <br/>
             * - @c NANO_OF_DAY -
             *  Returns a @c LocalTime with the specified nano-of-day.
             *  This completely replaces the time and is equivalent to @b ofNanoOfDay(long). <br/>
             * - @c MICRO_OF_SECOND -
             *  Returns a @c LocalTime with the nano-of-second replaced by the specified
             *  micro-of-second multiplied by 1,000.
             *  The hour, minute and second will be unchanged. <br/>
             * - @c MICRO_OF_DAY -
             *  Returns a @c LocalTime with the specified micro-of-day.
             *  This completely replaces the time and is equivalent to using @b ofNanoOfDay(long)
             *  with the micro-of-day multiplied by 1,000. <br/>
             * - @c MILLI_OF_SECOND -
             *  Returns a @c LocalTime with the nano-of-second replaced by the specified
             *  milli-of-second multiplied by 1,000,000.
             *  The hour, minute and second will be unchanged. <br/>
             * - @c MILLI_OF_DAY -
             *  Returns a @c LocalTime with the specified milli-of-day.
             *  This completely replaces the time and is equivalent to using @b ofNanoOfDay(long)
             *  with the milli-of-day multiplied by 1,000,000. <br/>
             * - @c SECOND_OF_MINUTE -
             *  Returns a @c LocalTime with the specified second-of-minute.
             *  The hour, minute and nano-of-second will be unchanged. <br/>
             * - @c SECOND_OF_DAY -
             *  Returns a @c LocalTime with the specified second-of-day.
             *  The nano-of-second will be unchanged. <br/>
             * - @c MINUTE_OF_HOUR -
             *  Returns a @c LocalTime with the specified minute-of-hour.
             *  The hour, second-of-minute and nano-of-second will be unchanged. <br/>
             * - @c MINUTE_OF_DAY -
             *  Returns a @c LocalTime with the specified minute-of-day.
             *  The second-of-minute and nano-of-second will be unchanged. <br/>
             * - @c HOUR_OF_AMPM -
             *  Returns a @c LocalTime with the specified hour-of-am-pm.
             *  The AM/PM, minute-of-hour, second-of-minute and nano-of-second will be unchanged. <br/>
             * - @c CLOCK_HOUR_OF_AMPM -
             *  Returns a @c LocalTime with the specified clock-hour-of-am-pm.
             *  The AM/PM, minute-of-hour, second-of-minute and nano-of-second will be unchanged. <br/>
             * - @c HOUR_OF_DAY -
             *  Returns a @c LocalTime with the specified hour-of-day.
             *  The minute-of-hour, second-of-minute and nano-of-second will be unchanged. <br/>
             * - @c CLOCK_HOUR_OF_DAY -
             *  Returns a @c LocalTime with the specified clock-hour-of-day.
             *  The minute-of-hour, second-of-minute and nano-of-second will be unchanged. <br/>
             * - @c AMPM_OF_DAY -
             *  Returns a @c LocalTime with the specified AM/PM.
             *  The hour-of-am-pm, minute-of-hour, second-of-minute and nano-of-second will be unchanged. <br/>
             *
             * </p>
             * <p>
             * In all cases, if the new value is outside the valid range of values for the field
             * then a @c DateTimeException will be thrown.
             * </p>
             * <p>
             * All other @c ChronoField instances will throw an @c TemporalException.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param field  the field to set in the result
             * @param newValue  the new value of the field in the result
             * @return a @c LocalTime based on @c this with the specified field set
             * @throws DateTimeException if the field cannot be set
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalTime with(ChronoField field, glong newValue) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalTime with the hour-of-day altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param hour  the hour-of-day to set in the result, from 0 to 23
             * @return a @c LocalTime based on this time with the requested hour
             * @throws DateTimeException if the hour value is invalid
             */
            LocalTime withHour(gint hour) const;

            /**
             * Returns a copy of this @c LocalTime with the minute-of-hour altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param minute  the minute-of-hour to set in the result, from 0 to 59
             * @return a @c LocalTime based on this time with the requested minute
             * @throws DateTimeException if the minute value is invalid
             */
            LocalTime withMinute(gint minute) const;

            /**
             * Returns a copy of this @c LocalTime with the second-of-minute altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param second  the second-of-minute to set in the result, from 0 to 59
             * @return a @c LocalTime based on this time with the requested second
             * @throws DateTimeException if the second value is invalid
             */
            LocalTime withSecond(gint second) const;

            /**
             * Returns a copy of this @c LocalTime with the nano-of-second altered.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanoOfSecond  the nano-of-second to set in the result, from 0 to 999,999,999
             * @return a @c LocalTime based on this time with the requested nanosecond
             * @throws DateTimeException if the nanos value is invalid
             */
            LocalTime withNano(gint nanoOfSecond) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalTime with the time truncated.
             * <p>
             * Truncation returns a copy of the original time with fields
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
             * @return a @c LocalTime based on this time with the time truncated
             * @throws DateTimeException if unable to truncate
             * @throws TemporalException if the unit is not supported
             */
            LocalTime truncateTo(ChronoUnit unit) const;

            /**
             * Returns a copy of this time with the specified amount added.
             * <p>
             * This returns a @c LocalTime, based on this one, with the amount
             * in terms of the unit added. If it is not possible to add the amount, because the
             * unit is not supported or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoUnit then the addition is implemented here.
             * The supported fields behave as follows:
             *
             * - @c NANOS -
             *  Returns a @c LocalTime with the specified number of nanoseconds added.
             *  This is equivalent to @b plusNanos(glong). <br/>
             * - @c MICROS -
             *  Returns a @c LocalTime with the specified number of microseconds added.
             *  This is equivalent to @b plusNanos(glong) with the amount
             *  multiplied by 1,000. <br/>
             * - @c MILLIS -
             *  Returns a @c LocalTime with the specified number of milliseconds added.
             *  This is equivalent to @b plusNanos(glong) with the amount
             *  multiplied by 1,000,000. <br/>
             * - @c SECONDS -
             *  Returns a @c LocalTime with the specified number of seconds added.
             *  This is equivalent to @b plusSeconds(glong). <br/>
             * - @c MINUTES -
             *  Returns a @c LocalTime with the specified number of minutes added.
             *  This is equivalent to @b plusMinutes(glong). <br/>
             * - @c HOURS -
             *  Returns a @c LocalTime with the specified number of hours added.
             *  This is equivalent to @b plusHours(glong). <br/>
             * - @c HALF_DAYS -
             *  Returns a @c LocalTime with the specified number of half-days added.
             *  This is equivalent to @b plusHours(glong) with the amount
             *  multiplied by 12. <br/>
             *
             * </p>
             * <p>
             * All other @c ChronoUnit instances will throw an @c TemporalException.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param amountToAdd  the amount of the unit to add to the result, may be negative
             * @param unit  the unit of the amount to add
             * @return a @c LocalTime based on this time with the specified amount added
             * @throws DateTimeException if the addition cannot be made
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalTime plus(glong amountToAdd, ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalTime with the specified number of hours added.
             * <p>
             * This adds the specified number of hours to this time, returning a new time.
             * The calculation wraps around midnight.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param hoursToAdd  the hours to add, may be negative
             * @return a @c LocalTime based on this time with the hours added
             */
            LocalTime plusHours(glong hoursToAdd) const;

            /**
             * Returns a copy of this @c LocalTime with the specified number of minutes added.
             * <p>
             * This adds the specified number of minutes to this time, returning a new time.
             * The calculation wraps around midnight.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param minutesToAdd  the minutes to add, may be negative
             * @return a @c LocalTime based on this time with the minutes added
             */
            LocalTime plusMinutes(glong minutesToAdd) const;

            /**
             * Returns a copy of this @c LocalTime with the specified number of seconds added.
             * <p>
             * This adds the specified number of seconds to this time, returning a new time.
             * The calculation wraps around midnight.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param secondsToAdd  the seconds to add, may be negative
             * @return a @c LocalTime based on this time with the seconds added
             */
            LocalTime plusSeconds(glong secondsToAdd) const;

            /**
             * Returns a copy of this @c LocalTime with the specified number of nanoseconds added.
             * <p>
             * This adds the specified number of nanoseconds to this time, returning a new time.
             * The calculation wraps around midnight.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanosToAdd  the nanos to add, may be negative
             * @return a @c LocalTime based on this time with the nanoseconds added
             */
            LocalTime plusNanos(glong nanosToAdd) const;

            /**
             * Returns a copy of this time with the specified amount subtracted.
             * <p>
             * This returns a @c LocalTime, based on this one, with the amount
             * in terms of the unit subtracted. If it is not possible to subtract the amount,
             * because the unit is not supported or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * This method is equivalent to <b> plus(long, ChronoUnit) </b> with the amount negated.
             * See that method for a full description of how addition, and thus subtraction, works.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
             * @param unit  the unit of the amount to subtract
             * @return a @c LocalTime based on this time with the specified amount subtracted
             * @throws DateTimeException if the subtraction cannot be made
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalTime minus(glong amountToSubtract, ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalTime with the specified number of hours subtracted.
             * <p>
             * This subtracts the specified number of hours from this time, returning a new time.
             * The calculation wraps around midnight.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param hoursToSubtract  the hours to subtract, may be negative
             * @return a @c LocalTime based on this time with the hours subtracted
             */
            LocalTime minusHours(glong hoursToSubtract) const;

            /**
             * Returns a copy of this @c LocalTime with the specified number of minutes subtracted.
             * <p>
             * This subtracts the specified number of minutes from this time, returning a new time.
             * The calculation wraps around midnight.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param minutesToSubtract  the minutes to subtract, may be negative
             * @return a @c LocalTime based on this time with the minutes subtracted
             */
            LocalTime minusMinutes(glong minutesToSubtract) const;

            /**
             * Returns a copy of this @c LocalTime with the specified number of seconds subtracted.
             * <p>
             * This subtracts the specified number of seconds from this time, returning a new time.
             * The calculation wraps around midnight.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param secondsToSubtract  the seconds to subtract, may be negative
             * @return a @c LocalTime based on this time with the seconds subtracted
             */
            LocalTime minusSeconds(glong secondsToSubtract) const;

            /**
             * Returns a copy of this @c LocalTime with the specified number of nanoseconds subtracted.
             * <p>
             * This subtracts the specified number of nanoseconds from this time, returning a new time.
             * The calculation wraps around midnight.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanosToSubtract  the nanos to subtract, may be negative
             * @return a @c LocalTime based on this time with the nanoseconds subtracted
             */
            LocalTime minusNanos(glong nanosToSubtract) const;

            /**
             * Calculates the amount of time until another time in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two @c LocalTime
             * objects in terms of a single @c ChronoUnit.
             * The start and end points are @c this and the specified time.
             * The result will be negative if the end is before the start.
             * For example, the amount in hours between two times can be calculated
             * using @code startTime.until(endTime, HOURS) @endcode .
             * </p>
             * <p>
             * The calculation returns a whole number, representing the number of
             * complete units between the two times.
             * For example, the amount in hours between 11:30 and 13:29 will only
             * be one hour as it is one minute short of two hours.
             * </p>
             * <p>
             * The calculation is implemented in this method for @b ChronoUnit.
             * The units @c NANOS, @c MICROS, @c MILLIS, @c SECONDS,
             * @c MINUTES, @c HOURS and @c HALF_DAYS are supported.
             * Other @c ChronoUnit values will throw an exception.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param endExclusive  the end time, exclusive, which is converted to a @c LocalTime
             * @param unit  the unit to measure the amount in
             * @return the amount of time between this time and the end time
             * @throws DateTimeException if the amount cannot be calculated, or the end
             *  temporal cannot be converted to a @c LocalTime
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong until(const Temporal& endExclusive, ChronoUnit unit) const override;

            //-----------------------------------------------------------------------
            /**
             * Combines this time with a date to create a @c LocalDateTime.
             * <p>
             * This returns a @c LocalDateTime formed from this time at the specified date.
             * All possible combinations of date and time are valid.
             * </p>
             * @param date  the date to combine with
             * @return the local date-time formed from this time and the specified date
             */
            LocalDateTime atDate(LocalDate const& date) const;

            //-----------------------------------------------------------------------
            /**
             * Extracts the time as seconds of day,
             * from @c 0 to @code 24 * 60 * 60 - 1 @endcode.
             *
             * @return the second-of-day equivalent to this time
             */
            gint toSecondOfDay() const;

            /**
             * Extracts the time as nanos of day,
             * from @c 0 to @code 24 * 60 * 60 * 1,000,000,000 - 1 @endcode.
             *
             * @return the nano of day equivalent to this time
             */
            glong toNanoOfDay() const;

            /**
             * Converts this @c LocalTime to the number of seconds since the epoch
             * of 1970-01-01T00:00:00Z.
             * <p>
             * This combines this local time with the specified date to calculate
             * the epoch-second value, which is the number of elapsed seconds from
             * 1970-01-01T00:00:00Z.
             * Instants on the time-line after the epoch are positive, earlier
             * are negative.
             * </p>
             * @param date the local date
             * @return the number of seconds since the epoch of 1970-01-01T00:00:00Z, may be negative
             */
            glong toEpochSecond(LocalDate const& date) const;

            /**
             * Converts this @c LocalTime to the number of seconds since the epoch
             * of 1970-01-01T00:00:00Z.
             * <p>
             * This combines this local time with the specified date and
             * offset to calculate the epoch-second value, which is the
             * number of elapsed seconds from 1970-01-01T00:00:00Z.
             * Instants on the time-line after the epoch are positive, earlier
             * are negative.
             * </p>
             * @param date the local date
             * @param offset the zone offset
             * @return the number of seconds since the epoch of 1970-01-01T00:00:00Z, may be negative
             */
            glong toEpochSecond(LocalDate const& date, ZoneOffset const& offset) const;

            //-----------------------------------------------------------------------
            /**
             * Compares this time to another time.
             * <p>
             * The comparison is based on the time-line position of the local times within a day.
             * It is "consistent with equals", as defined by @b Comparable.
             * </p>
             * @param other  the other time to compare to
             * @return the comparator value, that is less than zero if this is before @c other,
             *          zero if they are equal, or greater than zero if this is after @c other
             * @see isBefore
             * @see isAfter
             */
            gint compareTo(const LocalTime& other) const override;

            /**
             * Checks if this time is after the specified time.
             * <p>
             * The comparison is based on the time-line position of the time within a day.
             * </p>
             * @param other  the other time to compare to
             * @return true if this is after the specified time
             */
            gbool isAfter(LocalTime const& other) const;

            /**
             * Checks if this time is before the specified time.
             * <p>
             * The comparison is based on the time-line position of the time within a day.
             * </p>
             * @param other  the other time to compare to
             * @return true if this point is before the specified time
             */
            gbool isBefore(LocalTime const& other) const;

            /**
             * Queries this time using the specified query.
             * <p>
             * This queries this time using the specified query strategy object.
             * The @c TemporalQuery object defines the logic to be used to
             * obtain the result. Read the documentation of the query to understand
             * what the result of this method will be.
             * <p>
             * The result of this method is obtained by invoking the
             * @b TemporalQuery::queryFrom(Temporal) method on the
             * specified query passing @c this as the argument.
             *
             * @param query  the query to invoke
             * @return the query result, null may be returned (defined by the query)
             * @throws DateTimeException if unable to query (defined by the query)
             * @throws ArithmeticException if numeric overflow occurs (defined by the query)
             */
            util::Optional<> query(TemporalQuery const& query) const override;

            /**
             * Obtains an instance of @c LocalTime from a temporal object.
             * <p>
             * This obtains a local time based on the specified temporal.
             * A @c Temporal represents an arbitrary set of date and time information,
             * which this factory converts to an instance of @c LocalTime.
             * </p>
             * <p>
             * The conversion uses the @b TemporalQuery::LOCAL_TIME query, which relies
             * on extracting the @b NANO_OF_DAY field.
             * </p>
             * <p>
             * This method matches the signature of the functional interface @b TemporalQuery
             * allowing it to be used as a query via method reference, @c LocalTime::from.
             * </p>
             * @param temporal  the temporal object to convert
             * @return the local time
             * @throws DateTimeException if unable to convert to a @c LocalTime
             */
            static LocalTime from(Temporal const& temporal);

            //-----------------------------------------------------------------------
            /**
             * Checks if this time is equal to another time.
             * <p>
             * The comparison is based on the time-line position of the time within a day.
             * </p>
             * <p>
             * Only objects of type @c LocalTime are compared, other types return false.
             * </p>
             * @param obj  the object to check, null returns false
             * @return true if this is equal to the other time
             */
            gbool equals(const Object& obj) const override;

            /**
             * A hash code for this time.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            //-----------------------------------------------------------------------
            /**
             * Outputs this time as a @c String, such as @c 10:15.
             * <p>
             * The output will be one of the following ISO-8601 formats: <br/>
             *
             * - @c HH:mm <br/>
             * - @c HH:mm:ss <br/>
             * - @c HH:mm:ss.SSS <br/>
             * - @c HH:mm:ss.SSSSSS <br/>
             * - @c HH:mm:ss.SSSSSSSSS <br/>
             *
             * The format used will be the shortest that outputs the full value of
             * the time when the omitted parts are implied to be zero.
             * </p>
             * @return a string representation of this time
             */
            String toString() const override;

            Object& clone() const override;

        private:
            static glong timeToCompactInt(gint hour, gint minute, gint second, gint nanoOfSecond);
        };
    } // time
} // core

#endif //CORE24_LOCALTIME_H
