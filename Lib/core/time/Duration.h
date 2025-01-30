//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_DURATION_H
#define CORE24_DURATION_H

#include <core/time/Temporal.h>

namespace core {
    namespace time {
        /**
         * A time-based amount of time, such as '34.5 seconds'.
         * <p>
         * This class models a quantity or amount of time in terms of seconds and nanoseconds.
         * It can be accessed using other duration-based units, such as minutes and hours.
         * In addition, the @b DAYS unit can be used and is treated as
         * exactly equal to 24 hours, thus ignoring daylight savings effects.
         * </p>
         * <p>
         * A physical duration could be of infinite length.
         * For practicality, the duration is stored with constraints similar to @b Instant.
         * The duration uses nanosecond resolution with a maximum value of the seconds that can
         * be held in a @c long. This is greater than the current estimated age of the universe.
         * </p>
         * <p>
         * The range of a duration requires the storage of a number larger than a @c long.
         * To achieve this, the class stores a @c long representing seconds and an @c int
         * representing nanosecond-of-second, which will always be between 0 and 999,999,999.
         * The model is of a directed duration, meaning that the duration may be negative.
         * </p>
         * <p>
         * The duration is measured in "seconds", but these are not necessarily identical to
         * the scientific "SI second" definition based on atomic clocks.
         * This difference only impacts durations measured near a leap-second and should not affect
         * most applications.
         * See @b Instant for a discussion as to the meaning of the second and time-scales.
         * </p>
         * <p>
         * This is a @em value-based class; programmers should treat instances that are
         * @em equal as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * The @c equals method should be used for comparisons.
         * </p>
         * @note
         * This class is immutable and thread-safe.
         */
        class Duration final : public virtual Temporal, public virtual Comparable<Duration> {
        public:
            /**
             * Constant for a duration of zero.
             */
            static Duration const ZERO;

        private:
            /**
             * The number of seconds in the duration.
             */
            glong secs;

            /**
             * The number of nanoseconds in the duration, expressed as a fraction of the
             * number of seconds. This is always positive, and never exceeds 999,999,999.
             */
            gint ns;

        public:
            //-----------------------------------------------------------------------
            /**
             * Obtains a @c Duration representing a number of standard 24 hours days.
             * <p>
             * The seconds are calculated based on the standard definition of a day,
             * where each day is 86400 seconds which implies a 24 hours day.
             * The nanosecond in second field is set to zero.
             * </p>
             * @param days  the number of days, positive or negative
             * @return a @c Duration, not null
             * @throws ArithmeticException if the input days exceeds the capacity of @c Duration
             */
            static Duration ofDays(glong days);

            /**
             * Obtains a @c Duration representing a number of standard hours.
             * <p>
             * The seconds are calculated based on the standard definition of an hour,
             * where each hour is 3600 seconds.
             * The nanosecond in second field is set to zero.
             * </p>
             * @param hours  the number of hours, positive or negative
             * @return a @c Duration, not null
             * @throws ArithmeticException if the input hours exceeds the capacity of @c Duration
             */
            static Duration ofHours(glong hours);

            /**
             * Obtains a @c Duration representing a number of standard minutes.
             * <p>
             * The seconds are calculated based on the standard definition of a minute,
             * where each minute is 60 seconds.
             * The nanosecond in second field is set to zero.
             * </p>
             * @param minutes  the number of minutes, positive or negative
             * @return a @c Duration, not null
             * @throws ArithmeticException if the input minutes exceeds the capacity of @c Duration
             */
            static Duration ofMinutes(glong minutes);

            //-----------------------------------------------------------------------
            /**
             * Obtains a @c Duration representing a number of seconds.
             * <p>
             * The nanosecond in second field is set to zero.
             * </p>
             * @param seconds  the number of seconds, positive or negative
             * @return a @c Duration, not null
             */
            static Duration ofSeconds(glong seconds);

            /**
             * Obtains a @c Duration representing a number of seconds and an
             * adjustment in nanoseconds.
             * <p>
             * This method allows an arbitrary number of nanoseconds to be passed in.
             * The factory will alter the values of the second and nanosecond in order
             * to ensure that the stored nanosecond is in the range 0 to 999,999,999.
             * For example, the following will result in exactly the same duration:
             * @code
             *  Duration.ofSeconds(3, 1);
             *  Duration.ofSeconds(4, -999999999);
             *  Duration.ofSeconds(2, 1000000001);
             * @endcode
             * </p>
             * @param seconds  the number of seconds, positive or negative
             * @param nanoAdjustment  the nanosecond adjustment to the number of seconds, positive or negative
             * @return a @c Duration, not null
             * @throws ArithmeticException if the adjustment causes the seconds to exceed the capacity of @c Duration
             */
            CORE_EXPLICIT Duration(glong seconds, gint nanoAdjustment);

            //-----------------------------------------------------------------------
            /**
             * Obtains a @c Duration representing a number of milliseconds.
             * <p>
             * The seconds and nanoseconds are extracted from the specified milliseconds.
             * </p>
             * @param millis  the number of milliseconds, positive or negative
             * @return a @c Duration, not null
             */
            static Duration ofMillis(glong millis);

            //-----------------------------------------------------------------------
            /**
             * Obtains a @c Duration representing a number of nanoseconds.
             * <p>
             * The seconds and nanoseconds are extracted from the specified nanoseconds.
             * </p>
             * @param nanos  the number of nanoseconds, positive or negative
             * @return a @c Duration, not null
             */
            static Duration ofNanos(glong nanos);

            //-----------------------------------------------------------------------
            /**
             * Obtains a @c Duration representing an amount in the specified unit.
             * <p>
             * The parameters represent the two parts of a phrase like '6 Hours'. For example:
             * @code
             *  Duration.of(3, SECONDS);
             *  Duration.of(465, HOURS);
             * @endcode
             * Only a subset of units is accepted by this method.
             * The unit must either have an <em> exact duration</em> or
             * be @b ChronoUnit::DAYS which is treated as 24 hours. Other units throw an exception.
             * </p>
             * @param amount  the amount of the duration, measured in terms of the unit, positive or negative
             * @param unit  the unit that the duration is measured in, must have an exact duration, not null
             * @return a @c Duration, not null
             * @throws DateTimeException if the unit has an estimated duration
             * @throws ArithmeticException if a numeric overflow occurs
             */
            static Duration of(glong amount, ChronoUnit unit);

            //-----------------------------------------------------------------------
            /**
             * Obtains a @c Duration representing the duration between two temporal objects.
             * <p>
             * This calculates the duration between two temporal objects. If the objects
             * are of different types, then the duration is calculated based on the type
             * of the first object. For example, if the first argument is a @c LocalTime
             * then the second argument is converted to a @c LocalTime.
             * </p>
             * <p>
             * The specified temporal objects must support the @b SECONDS unit.
             * For full accuracy, either the @b NANOS unit or the
             * @b NANO_OF_SECOND field should be supported.
             * </p>
             * <p>
             * The result of this method can be a negative duration if the end is before the start.
             * To guarantee to obtain a positive duration call @b abs() on the result.
             * </p>
             * @param startInclusive  the start instant, inclusive, not null
             * @param endExclusive  the end instant, exclusive, not null
             * @return a @c Duration, not null
             * @throws DateTimeException if the seconds between the temporal cannot be obtained
             * @throws ArithmeticException if the calculation exceeds the capacity of @c Duration
             */
            static Duration between(Temporal const& startInclusive, Temporal const& endExclusive);

            //-----------------------------------------------------------------------
            /**
             * Gets the value of the requested unit.
             * <p>
             * This returns a value for each of the two supported units,
             * @b SECONDS and @b NANOS.
             * All other units throw an exception.
             * </p>
             * @param unit the @c TemporalUnit for which to return the value
             * @return the long value of the unit
             * @throws DateTimeException if the unit is not supported
             * @throws TemporalException if the unit is not supported
             */
            glong get(ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Checks if this duration is positive, excluding zero.
             * <p>
             * A @c Duration represents a directed distance between two points on
             * the time-line and can therefore be positive, zero or negative.
             * This method checks whether the length is greater than zero.
             * </p>
             * @return true if this duration has a total length greater than zero
             * @since 18
             */
            gbool isPositive() const;

            /**
             * Checks if this duration is zero length.
             * <p>
             * A @c Duration represents a directed distance between two points on
             * the time-line and can therefore be positive, zero or negative.
             * This method checks whether the length is zero.
             * </p>
             * @return true if this duration has a total length equal to zero
             */
            gbool isZero() const;

            /**
             * Checks if this duration is negative, excluding zero.
             * <p>
             * A @c Duration represents a directed distance between two points on
             * the time-line and can therefore be positive, zero or negative.
             * This method checks whether the length is less than zero.
             * </p>
             * @return true if this duration has a total length less than zero
             */
            gbool isNegative() const;

            //-----------------------------------------------------------------------
            /**
             * Gets the number of seconds in this duration.
             * <p>
             * The length of the duration is stored using two fields - seconds and nanoseconds.
             * The nanoseconds part is a value from 0 to 999,999,999 that is an adjustment to
             * the length in seconds.
             * The total duration is defined by calling this method and @b nano().
             * </p>
             * <p>
             * A @c Duration represents a directed distance between two points on the time-line.
             * A negative duration is expressed by the negative sign of the seconds part.
             * A duration of -1 nanosecond is stored as -1 seconds plus 999,999,999 nanoseconds.
             * </p>
             * @return the whole seconds part of the length of the duration, positive or negative
             */
            glong seconds() const;

            /**
             * Gets the number of nanoseconds within the second in this duration.
             * <p>
             * The length of the duration is stored using two fields - seconds and nanoseconds.
             * The nanoseconds part is a value from 0 to 999,999,999 that is an adjustment to
             * the length in seconds.
             * The total duration is defined by calling this method and @b seconds().
             * </p>
             * <p>
             * A @c Duration represents a directed distance between two points on the time-line.
             * A negative duration is expressed by the negative sign of the seconds part.
             * A duration of -1 nanosecond is stored as -1 seconds plus 999,999,999 nanoseconds.
             * </p>
             * @return the nanoseconds within the second part of the length of the duration, from 0 to 999,999,999
             */
            gint nanos() const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this duration with the specified amount of seconds.
             * <p>
             * This returns a duration with the specified seconds, retaining the
             * nano-of-second part of this duration.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param seconds  the seconds to represent, may be negative
             * @return a @c Duration based on this duration with the requested seconds, not null
             */
            Duration withSeconds(glong seconds) const;

            /**
             * Returns a copy of this duration with the specified nano-of-second.
             * <p>
             * This returns a duration with the specified nano-of-second, retaining the
             * seconds part of this duration.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @return a @c Duration based on this duration with the requested nano-of-second, not null
             * @throws DateTimeException if the nano-of-second is invalid
             */
            Duration withNanos(gint nanoOfSecond) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this duration with the specified duration added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param duration  the duration to add, positive or negative, not null
             * @return a @c Duration based on this duration with the specified duration added, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plus(Duration const& duration) const;

            /**
             * Returns a copy of this duration with the specified duration added.
             * <p>
             * The duration amount is measured in terms of the specified unit.
             * Only a subset of units is accepted by this method.
             * The unit must either have an <em> exact duration</em> or
             * be @b ChronoUnit::DAYS which is treated as 24 hours. Other units throw an exception.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param amountToAdd  the amount to add, measured in terms of the unit, positive or negative
             * @param unit  the unit that the amount is measured in, must have an exact duration, not null
             * @return a @c Duration based on this duration with the specified duration added, not null
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plus(glong amountToAdd, ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this duration with the specified duration in standard 24 hours days added.
             * <p>
             * The number of days is multiplied by 86400 to obtain the number of seconds to add.
             * This is based on the standard definition of a day as 24 hours.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param daysToAdd  the days to add, positive or negative
             * @return a @c Duration based on this duration with the specified days added, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusDays(glong daysToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in hours added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param hoursToAdd  the hours to add, positive or negative
             * @return a @c Duration based on this duration with the specified hours added, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusHours(glong hoursToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in minutes added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param minutesToAdd  the minutes to add, positive or negative
             * @return a @c Duration based on this duration with the specified minutes added, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusMinutes(glong minutesToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in seconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param secondsToAdd  the seconds to add, positive or negative
             * @return a @c Duration based on this duration with the specified seconds added, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusSeconds(glong secondsToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in milliseconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param millisToAdd  the milliseconds to add, positive or negative
             * @return a @c Duration based on this duration with the specified milliseconds added, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusMillis(glong millisToAdd) const;

            /**
             * Returns a copy of this duration with the specified duration in nanoseconds added.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanosToAdd  the nanoseconds to add, positive or negative
             * @return a @c Duration based on this duration with the specified nanoseconds added, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration plusNanos(glong nanosToAdd) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this duration with the specified duration subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param duration  the duration to subtract, positive or negative, not null
             * @return a @c Duration based on this duration with the specified duration subtracted, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minus(Duration const& duration) const;

            /**
             * Returns a copy of this duration with the specified duration subtracted.
             * <p>
             * The duration amount is measured in terms of the specified unit.
             * Only a subset of units is accepted by this method.
             * The unit must either have an <em> exact duration</em> or
             * be @b ChronoUnit::DAYS which is treated as 24 hours. Other units throw an exception.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param amountToSubtract  the amount to subtract, measured in terms of the unit, positive or negative
             * @param unit  the unit that the amount is measured in, must have an exact duration, not null
             * @return a @c Duration based on this duration with the specified duration subtracted, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minus(glong amountToSubtract, ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this duration with the specified duration in standard 24 hours days subtracted.
             * <p>
             * The number of days is multiplied by 86400 to obtain the number of seconds to subtract.
             * This is based on the standard definition of a day as 24 hours.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param daysToSubtract  the days to subtract, positive or negative
             * @return a @c Duration based on this duration with the specified days subtracted, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusDays(glong daysToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in hours subtracted.
             * <p>
             * The number of hours is multiplied by 3600 to obtain the number of seconds to subtract.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param hoursToSubtract  the hours to subtract, positive or negative
             * @return a @c Duration based on this duration with the specified hours subtracted, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusHours(glong hoursToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in minutes subtracted.
             * <p>
             * The number of hours is multiplied by 60 to obtain the number of seconds to subtract.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param minutesToSubtract  the minutes to subtract, positive or negative
             * @return a @c Duration based on this duration with the specified minutes subtracted, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusMinutes(glong minutesToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in seconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param secondsToSubtract  the seconds to subtract, positive or negative
             * @return a @c Duration based on this duration with the specified seconds subtracted, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusSeconds(glong secondsToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in milliseconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param millisToSubtract  the milliseconds to subtract, positive or negative
             * @return a @c Duration based on this duration with the specified milliseconds subtracted, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusMillis(glong millisToSubtract) const;

            /**
             * Returns a copy of this duration with the specified duration in nanoseconds subtracted.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param nanosToSubtract  the nanoseconds to subtract, positive or negative
             * @return a @c Duration based on this duration with the specified nanoseconds subtracted, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration minusNanos(glong nanosToSubtract) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this duration multiplied by the scalar.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param multiplicand  the value to multiply the duration by, positive or negative
             * @return a @c Duration based on this duration multiplied by the specified scalar, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration multipliedBy(glong multiplicand) const;

            /**
             * Returns a copy of this duration divided by the specified value.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param divisor  the value to divide the duration by, positive or negative, not zero
             * @return a @c Duration based on this duration divided by the specified divisor, not null
             * @throws ArithmeticException if the divisor is zero or if numeric overflow occurs
             */
            Duration dividedBy(glong divisor) const;

            /**
             * Returns number of whole times a specified Duration occurs within this Duration.
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param divisor the value to divide the duration by, positive or negative, not null
             * @return number of whole times, rounded toward zero, a specified
             *         @c Duration occurs within this Duration, may be negative
             * @throws ArithmeticException if the divisor is zero, or if numeric overflow occurs
             *
             */
            glong dividedBy(Duration const& divisor) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this duration with the length negated.
             * <p>
             * This method swaps the sign of the total length of this duration.
             * For example, @c PT1.3S will be returned as @c PT-1.3S.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return a @c Duration based on this duration with the amount negated, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration negated() const;

            /**
             * Returns a copy of this duration with a positive length.
             * <p>
             * This method returns a positive duration by effectively removing the sign from any negative total length.
             * For example, @c PT-1.3S will be returned as @c PT1.3S.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return a @c Duration based on this duration with an absolute length, not null
             * @throws ArithmeticException if numeric overflow occurs
             */
            Duration abs() const;

            //-----------------------------------------------------------------------
            /**
             * Gets the number of days in this duration.
             * <p>
             * This returns the total number of days in the duration by dividing the
             * number of seconds by 86400.
             * This is based on the standard definition of a day as 24 hours.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the number of days in the duration, may be negative
             */
            glong toDays() const;

            /**
             * Gets the number of hours in this duration.
             * <p>
             * This returns the total number of hours in the duration by dividing the
             * number of seconds by 3600.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the number of hours in the duration, may be negative
             */
            glong toHours() const;

            /**
             * Gets the number of minutes in this duration.
             * <p>
             * This returns the total number of minutes in the duration by dividing the
             * number of seconds by 60.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the number of minutes in the duration, may be negative
             */
            glong toMinutes() const;

            /**
             * Gets the number of seconds in this duration.
             * <p>
             * This returns the total number of whole seconds in the duration.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the whole seconds part of the length of the duration, positive or negative
             *
             */
            glong toSeconds() const;

            /**
             * Converts this duration to the total length in milliseconds.
             * <p>
             * If this duration is too large to fit in a @c long milliseconds, then an
             * exception is thrown.
             * </p>
             * <p>
             * If this duration has greater than millisecond precision, then the conversion
             * will drop any excess precision information as though the amount in nanoseconds
             * was subject to integer division by one million.
             * </p>
             * @return the total length of the duration in milliseconds
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong toMillis() const;

            /**
             * Converts this duration to the total length in nanoseconds expressed as a @c long.
             * <p>
             * If this duration is too large to fit in a @c long nanoseconds, then an
             * exception is thrown.
             * </p>
             * @return the total length of the duration in nanoseconds
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong toNanos() const;

            /**
             * Extracts the number of days in the duration.
             * <p>
             * This returns the total number of days in the duration by dividing the
             * number of seconds by 86400.
             * This is based on the standard definition of a day as 24 hours.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @note
             * This method behaves exactly the same way as @b toDays().
             *
             * @return the number of days in the duration, may be negative
             *
             */
            glong toDaysPart() const;

            /**
             * Extracts the number of hours part in the duration.
             * <p>
             * This returns the number of remaining hours when dividing @b toHours
             * by hours in a day.
             * This is based on the standard definition of a day as 24 hours.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the number of hours part in the duration, may be negative
             *
             */
            gint toHoursPart() const;

            /**
             * Extracts the number of minutes part in the duration.
             * <p>
             * This returns the number of remaining minutes when dividing @b toMinutes
             * by minutes in an hour.
             * This is based on the standard definition of an hour as 60 minutes.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the number of minutes parts in the duration, may be negative
             *
             */
            gint toMinutesPart() const;

            /**
             * Extracts the number of seconds part in the duration.
             * <p>
             * This returns the remaining seconds when dividing @b toSeconds
             * by seconds in a minute.
             * This is based on the standard definition of a minute as 60 seconds.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the number of seconds parts in the duration, may be negative
             *
             */
            gint toSecondsPart() const;

            /**
             * Extracts the number of milliseconds part of the duration.
             * <p>
             * This returns the milliseconds part by dividing the number of nanoseconds by 1,000,000.
             * The length of the duration is stored using two fields - seconds and nanoseconds.
             * The nanoseconds part is a value from 0 to 999,999,999 that is an adjustment to
             * the length in seconds.
             * The total duration is defined by calling @b nano() and @b seconds().
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the number of milliseconds part of the duration.
             *
             */
            gint toMillisPart() const;

            /**
             * Get the nanoseconds part within seconds of the duration.
             * <p>
             * The length of the duration is stored using two fields - seconds and nanoseconds.
             * The nanoseconds part is a value from 0 to 999,999,999 that is an adjustment to
             * the length in seconds.
             * The total duration is defined by calling @b nano() and @b seconds().
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @return the nanoseconds within the second part of the length of the duration, from 0 to 999,999,999
             *
             */
            gint toNanosPart() const;


            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c Duration truncated to the specified unit.
             * <p>
             * Truncating the duration returns a copy of the original with conceptual fields
             * smaller than the specified unit set to zero.
             * For example, truncating with the @b MINUTES unit will
             * round down towards zero to the nearest minute, setting the seconds and
             * nanoseconds to zero.
             * </p>
             * <p>
             * The unit must have a @em duration
             * that divides into the length of a standard day without remainder.
             * This includes all
             * @em time-based units on @c ChronoUnit
             * and @b DAYS. Other ChronoUnits throw an exception.
             * </p>
             * <p>
             * This instance is immutable and unaffected by this method call.
             * </p>
             * @param unit the unit to truncate to, not null
             * @return a @c Duration based on this duration with the time truncated, not null
             * @throws DateTimeException if the unit is invalid for truncation
             * @throws TemporalException if the unit is not supported
             *
             */
            Duration truncateTo(ChronoUnit unit) const;

            util::Optional<> query(TemporalQuery const& query) const override;

            //-----------------------------------------------------------------------
            /**
             * Compares this duration to the specified @c Duration.
             * <p>
             * The comparison is based on the total length of the durations.
             * It is "consistent with equals", as defined by @b Comparable.
             * </p>
             * @param otherDuration the other duration to compare to, not null
             * @return the comparator value, that is less than zero if this duration is less than @c otherDuration,
             *          zero if they are equal, greater than zero if this duration is greater than @c otherDuration
             */
            gint compareTo(const Duration& otherDuration) const override;

            //-----------------------------------------------------------------------
            /**
             * Checks if this duration is equal to the specified @c Duration.
             * <p>
             * The comparison is based on the total length of the durations.
             * </p>
             * @param other the other duration, null returns false
             * @return true if the other duration is equal to this one
             */
            gbool equals(const Object& other) const override;

            /**
             * A hash code for this duration.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            //-----------------------------------------------------------------------
            /**
             * A string representation of this duration using ISO-8601 seconds
             * based representation, such as @c PT8H6M12.345S.
             * <p>
             * The format of the returned string will be @c PTnHnMnS, where n is
             * the relevant hours, minutes or seconds part of the duration.
             * Any fractional seconds are placed after a decimal point in the seconds section.
             * If a section has a zero value, it is omitted.
             * The hours, minutes and seconds will all have the same sign.
             * </p>
             * <p>
             * Examples:
             * @code
             *    "20.345 seconds"                 -- "PT20.345S
             *    "15 minutes" (15 * 60 seconds)   -- "PT15M"
             *    "10 hours" (10 * 3600 seconds)   -- "PT10H"
             *    "2 days" (2 * 86400 seconds)     -- "PT48H"
             * @endcode
             * Note that multiples of 24 hours are not output as days to avoid confusion
             * with @c Period.
             * </p>
             * @return an ISO-8601 representation of this duration, not null
             */
            String toString() const override;

            glong until(const Temporal& endExclusive, ChronoUnit unit) const override;

            Object& clone() const override;

            ~Duration() override;

            gbool isSupported(ChronoUnit unit) const override;
        };
    } // time
} // core

#endif //CORE24_DURATION_H
