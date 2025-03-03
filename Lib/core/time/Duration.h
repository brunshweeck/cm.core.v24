//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_DURATION_H
#define CORE24_DURATION_H

#include <core/time/LocalTime.h>
#include <core/time/TemporalAmount.h>

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
    class Duration final : public virtual TemporalAmount,
                           public virtual Comparable<Duration> {
      /**
       * The number of seconds in the duration.
       */
      glong seconds_;

      /**
       * The number of nanoseconds in the duration, expressed as a fraction of the
       * number of seconds. This is always positive, and never exceeds 999,999,999.
       */
      gint nanos_;

      /**
       * Constructs an instance of @c Duration using seconds and nanoseconds.
       *
       * @param seconds  the length of the duration in seconds, positive or negative
       * @param nanos  the nanoseconds within the second, from 0 to 999,999,999
       */
      CORE_EXPLICIT Duration(glong seconds, gint nanos);

    public:
      /**
       * Constant for a duration of zero.
       */
      static Duration const ZERO;

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
       *  Duration.ofSeconds(4, -999_999_999);
       *  Duration.ofSeconds(2, 1000_000_001);
       * @endcode
       *
       * @param seconds  the number of seconds, positive or negative
       * @param nanoAdjustment  the nanosecond adjustment to the number of seconds, positive or negative
       * @return a @c Duration, not null
       * @throws ArithmeticException if the adjustment causes the seconds to exceed the capacity of @c Duration
       */
      static Duration ofSeconds(glong seconds, glong nanoAdjustment);


      /**
       * Obtains a @c Duration representing a number of milliseconds.
       * <p>
       * The seconds and nanoseconds are extracted from the specified milliseconds.
       * </p>
       * @param millis  the number of milliseconds, positive or negative
       * @return a @c Duration, not null
       */
      static Duration ofMillis(glong millis);


      /**
       * Obtains a @c Duration representing a number of nanoseconds.
       * <p>
       * The seconds and nanoseconds are extracted from the specified nanoseconds.
       * </p>
       * @param nanos  the number of nanoseconds, positive or negative
       * @return a @c Duration, not null
       */
      static Duration ofNanos(glong nanos);


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
       * be @b TemporalUnit::DAYS which is treated as 24 hours. Other units throw an exception.
       * </p>
       * @param amount  the amount of the duration, measured in terms of the unit, positive or negative
       * @param unit  the unit that the duration is measured in, must have an exact duration, not null
       * @return a @c Duration, not null
       * @throws DateTimeException if the unit has an estimated duration
       * @throws ArithmeticException if a numeric overflow occurs
       */
      static Duration of(glong amount, TemporalUnit unit);

      /**
       * Obtains an instance of @c Duration from a temporal amount.
       * <p>
       * This obtains a duration based on the specified amount.
       * A @c TemporalAmount represents an  amount of time, which may be
       * date-based or time-based, which this factory extracts to a duration.
       * <p>
       * The conversion loops around the set of units from the amount and uses
       * the @em duration of the unit to calculate the total @c Duration.
       * Only a subset of units is accepted by this method. The unit must either
       * have an <em>exact duration</em>
       * or be @b TemporalUnit::DAYS which is treated as 24 hours.
       * If any other units are found then an exception is thrown.
       *
       * @param amount  the temporal amount to convert, not null
       * @return the equivalent duration, not null
       * @throws DateTimeException if unable to convert to a @c Duration
       * @throws ArithmeticException if numeric overflow occurs
       */
      static Duration from(TemporalAmount const& amount);

      /**
       * Obtains a @c Duration from a text string such as @c PnDTnHnMn.nS.
       * <p>
       * This will parse a textual representation of a duration, including the
       * string produced by @c toString(). The formats accepted are based
       * on the ISO-8601 duration format @c PnDTnHnMn.nS with days
       * considered to be exactly 24 hours.
       * <p>
       * The string starts with an optional sign, denoted by the ASCII negative
       * or positive symbol. If negative, the whole duration is negated.
       * The ASCII letter "P" is next in upper or lower case.
       * There are then four sections, each consisting of a number and a suffix.
       * The sections have suffixes in ASCII of "D", "H", "M" and "S" for
       * days, hours, minutes and seconds, accepted in upper or lower case.
       * The suffixes must occur in order. The ASCII letter "T" must occur before
       * the first occurrence, if any, of an hour, minute or second section.
       * At least one of the four sections must be present, and if "T" is present
       * there must be at least one section after the "T".
       * The number part of each section must consist of one or more ASCII digits.
       * The number may be prefixed by the ASCII negative or positive symbol.
       * The number of days, hours and minutes must parse to a @c long.
       * The number of seconds must parse to a @c long with optional fraction.
       * The decimal point may be either a dot or a comma.
       * The fractional part may have from zero to 9 digits.
       * <p>
       * The leading plus/minus sign, and negative values for other units are
       * not part of the ISO-8601 standard.
       * <p>
       * Examples:
       * @code
       *    "PT20.345S" -- parses as "20.345 seconds"
       *    "PT15M"     -- parses as "15 minutes" (where a minute is 60 seconds)
       *    "PT10H"     -- parses as "10 hours" (where an hour is 3600 seconds)
       *    "P2D"       -- parses as "2 days" (where a day is 24 hours or 86400 seconds)
       *    "P2DT3H4M"  -- parses as "2 days, 3 hours and 4 minutes"
       *    "PT-6H3M"    -- parses as "-6 hours and +3 minutes"
       *    "-PT6H3M"    -- parses as "-6 hours and -3 minutes"
       *    "-PT-6H+3M"  -- parses as "+6 hours and -3 minutes"
       * @endcode
       * </p>
       * @param text  the text to parse, not null
       * @return the parsed duration, not null
       * @throws DateTimeParseException if the text cannot be parsed to a duration
       */
      static Duration parse(CharSequence const& text);

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
      glong get(TemporalUnit unit) const override;

      /**
       * Gets the set of units supported by this duration.
       * <p>
       * The supported units are @b SECONDS,
       * and @b NANOS.
       * They are returned in the order seconds, nanos.
       * <p>
       * This set can be used in conjunction with @b get(TemporalUnit)
       * to access the entire state of the duration.
       *
       * @return a list containing the seconds and nanos units, not null
       */
      UnitList& getUnits() const override;

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
       * be @b TemporalUnit::DAYS which is treated as 24 hours. Other units throw an exception.
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
      Duration plus(glong amountToAdd, TemporalUnit unit) const;


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
       * be @b TemporalUnit::DAYS which is treated as 24 hours. Other units throw an exception.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param amountToSubtract  the amount to subtract, measured in terms of the unit, positive or negative
       * @param unit  the unit that the amount is measured in, must have an exact duration, not null
       * @return a @c Duration based on this duration with the specified duration subtracted, not null
       * @throws ArithmeticException if numeric overflow occurs
       */
      Duration minus(glong amountToSubtract, TemporalUnit unit) const;


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

      /**
       * Returns a copy of this duration with the length negated.
       * <p>
       * This method swaps the sign of the total length of this duration.
       * For example, @c PT1.3S will be returned as @c PT-1.3S.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @return a @c Duration based on this duration with the amount negated, not null
       * @throws ArithmeticException if numeric overflow occurs
       */
      Temporal& addTo(const Temporal& temporal) const override;

      /**
       * Subtracts this duration from the specified temporal object.
       * <p>
       * This returns a temporal object of the same observable type as the input
       * with this duration subtracted.
       * <p>
       * In most cases, it is clearer to reverse the calling pattern by using
       * @b Temporal::minus(TemporalAmount).
       * @code
       *   // these two lines are equivalent, but the second approach is recommended
       *   dateTime = thisDuration.subtractFrom(dateTime);
       *   dateTime = dateTime.minus(thisDuration);
       * @endcode
       * <p>
       * The calculation will subtract the seconds, then nanos.
       * Only non-zero amounts will be added.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param temporal  the temporal object to adjust, not null
       * @return an object of the same type with the adjustment made, not null
       * @throws DateTimeException if unable to subtract
       * @throws ArithmeticException if numeric overflow occurs
       */
      Temporal& subtractFrom(const Temporal& temporal) const override;


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
       * @em time-based units on @c TemporalUnit
       * and @b DAYS. Other TemporalUnits throw an exception.
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
      Duration truncatedTo(TemporalUnit unit) const;

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

      /**
       * Return shadow copy of this object
       */
      Object& clone() const override;

    private:
      static gint CORE_FAST MINUTES_PER_HOUR = LocalTime::MINUTES_PER_HOUR;
      static gint CORE_FAST NANOS_PER_MILLI = LocalTime::NANOS_PER_MILLI;
      static gint CORE_FAST NANOS_PER_SECOND = LocalTime::NANOS_PER_SECOND;
      static gint CORE_FAST SECONDS_PER_DAY = LocalTime::SECONDS_PER_DAY;
      static gint CORE_FAST SECONDS_PER_HOUR = LocalTime::SECONDS_PER_HOUR;
      static gint CORE_FAST SECONDS_PER_MINUTE = LocalTime::SECONDS_PER_MINUTE;

      /**
       * Obtains an instance of @c Duration using seconds and nanoseconds.
       *
       * @param seconds  the length of the duration in seconds, positive or negative
       * @param nanoAdjustment  the nanosecond adjustment within the second, from 0 to 999,999,999
       */
      static Duration create(glong seconds, gint nanoAdjustment);
    };
  } // time
} // core

#endif //CORE24_DURATION_H
