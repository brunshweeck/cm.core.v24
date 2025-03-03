//
// Created by brunshweeck on 11/02/25.
//

#ifndef CORE24_CORE_24_INSTANT_H
#define CORE24_CORE_24_INSTANT_H

#include <core/time/LocalTime.h>
#include <core/time/TemporalAdjuster.h>
#include <core/time/TemporalBase.h>

namespace core {
  namespace time {
    /**
     * An instantaneous point on the time-line.
     * <p>
     * This class models a single instantaneous point on the time-line.
     * This might be used to record event time-stamps in the application.
     * </p>
     * <p>
     * The range of an instant requires the storage of a number larger than a @c long.
     * To achieve this, the class stores a @c long representing epoch-seconds and an
     * @c int representing nanosecond-of-second, which will always be between 0 and 999,999,999.
     * The epoch-seconds are measured from the standard Java epoch of @c 1970-01-01T00:00:00Z
     * where instants after the epoch have positive values, and earlier instants have negative values.
     * For both the epoch-second and nanosecond parts, a larger value is always later on the time-line
     * than a smaller value.
     * </p>
     * <h3>Time-scale</h3>
     * <p>
     * The length of the solar day is the standard way that humans measure time.
     * This has traditionally been subdivided into 24 hours of 60 minutes of 60 seconds,
     * forming a 86400 second day.
     * </p>
     * <p>
     * Modern timekeeping is based on atomic clocks which precisely define an SI second
     * relative to the transitions of a Caesium atom. The length of an SI second was defined
     * to be very close to the 86400th fraction of a day.
     * </p>
     * <p>
     * Unfortunately, as the Earth rotates the length of the day varies.
     * In addition, over time the average length of the day is getting longer as the Earth slows.
     * As a result, the length of a solar day in 2012 is slightly longer than 86400 SI seconds.
     * The actual length of any given day and the amount by which the Earth is slowing
     * are not predictable and can only be determined by measurement.
     * The UT1 time-scale captures the accurate length of day, but is only available some
     * time after the day has completed.
     * </p>
     * <p>
     * The UTC time-scale is a standard approach to bundle up all the additional fractions
     * of a second from UT1 into whole seconds, known as <i>leap-seconds</i>.
     * A leap-second may be added or removed depending on the Earth's rotational changes.
     * As such, UTC permits a day to have 86399 SI seconds or 86401 SI seconds where
     * necessary in order to keep the day aligned with the Sun.
     * </p>
     * <p>
     * The modern UTC time-scale was introduced in 1972, introducing the concept of whole leap-seconds.
     * Between 1958 and 1972, the definition of UTC was complex, with minor sub-second leaps and
     * alterations to the length of the notional second. As of 2012, discussions are underway
     * to change the definition of UTC again, with the potential to remove leap seconds or
     * introduce other changes.
     * </p>
     * <p>
     * Given the complexity of accurate timekeeping described above, this Java API defines
     * its own time-scale, the <i>Java Time-Scale</i>.
     * </p>
     * <p>
     * The Java Time-Scale divides each calendar day into exactly 86400
     * subdivisions, known as seconds.  These seconds may differ from the
     * SI second.  It closely matches the de facto international civil time
     * scale, the definition of which changes from time to time.
     * </p>
     * <p>
     * The Java Time-Scale has slightly different definitions for different
     * segments of the time-line, each based on the consensus international
     * time scale that is used as the basis for civil time. Whenever the
     * internationally-agreed time scale is modified or replaced, a new
     * segment of the Java Time-Scale must be defined for it.  Each segment
     * must meet these requirements: <br/>
     * - the Java Time-Scale shall closely match the underlying international
     *  civil time scale;<br/>
     * - the Java Time-Scale shall exactly match the international civil
     *  time scale at noon each day;<br/>
     * - the Java Time-Scale shall have a precisely-defined relationship to
     *  the international civil time scale.<br/>
     *
     * There are currently, as of 2013, two segments in the Java time-scale.
     * </p>
     * <p>
     * For the segment from 1972-11-03 (exact boundary discussed below) until
     * further notice, the consensus international time scale is UTC (with
     * leap seconds).  In this segment, the Java Time-Scale is identical to
     * <a href="http://www.cl.cam.ac.uk/~mgk25/time/utc-sls/">UTC-SLS</a>.
     * This is identical to UTC on days that do not have a leap second.
     * On days that do have a leap second, the leap second is spread equally
     * over the last 1000 seconds of the day, maintaining the appearance of
     * exactly 86400 seconds per day.
     * </p>
     * <p>
     * For the segment prior to 1972-11-03, extending back arbitrarily far,
     * the consensus international time scale is defined to be UT1, applied
     * proleptically, which is equivalent to the (mean) solar time on the
     * prime meridian (Greenwich). In this segment, the Java Time-Scale is
     * identical to the consensus international time scale. The exact
     * boundary between the two segments is the instant where UT1 = UTC
     * between 1972-11-03T00:00 and 1972-11-04T12:00.
     * </p>
     * <p>
     * Implementations of the Java time-scale using the JSR-310 API are not
     * required to provide any clock that is sub-second accurate, or that
     * progresses monotonically or smoothly. Implementations are therefore
     * not required to actually perform the UTC-SLS slew or to otherwise be
     * aware of leap seconds. JSR-310 does, however, require that
     * implementations must document the approach they use when defining a
     * clock representing the current instant.
     * </p>
     * <p>
     * The Java time-scale is used for all date-time classes.
     * This includes @c Instant, @c LocalDate, @c LocalTime, @c OffsetDateTime,
     * @c ZonedDateTime and @c Duration.
     * </p>
     * @implSpec
     * This class is immutable and thread-safe.
     */
    class Instant final : public virtual TemporalBase<Instant>,
                          public virtual TemporalAdjuster,
                          public virtual Comparable<Instant> {
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, Optional);

      // Static Imports From LocalTime class

      static CORE_FAST gint MICROS_PER_SECOND = LocalTime::MICROS_PER_SECOND;
      static CORE_FAST gint MILLIS_PER_SECOND = LocalTime::MILLIS_PER_SECOND;
      static CORE_FAST gint NANOS_PER_SECOND = LocalTime::NANOS_PER_SECOND;
      static CORE_FAST gint SECONDS_PER_DAY = LocalTime::SECONDS_PER_DAY;
      static CORE_FAST gint SECONDS_PER_HOUR = LocalTime::SECONDS_PER_HOUR;
      static CORE_FAST gint SECONDS_PER_MINUTE = LocalTime::SECONDS_PER_MINUTE;
      static CORE_FAST gint NANOS_PER_MILLI = LocalTime::NANOS_PER_MILLI;

      /**
       * The minimum supported epoch second.
       */
      static CORE_FAST glong MIN_SECOND = -31557014167219200L;

      /**
       * The maximum supported epoch second.
       */
      static CORE_FAST glong MAX_SECOND = 31556889864403199L;

      /**
       * The number of seconds from the epoch of 1970-01-01T00:00:00Z.
       */
      glong seconds = 0;

      /**
       * The number of nanoseconds, later along the time-line, from the seconds field.
       * This is always positive, and never exceeds 999,999,999.
       */
      gint nanos = 0;

      /**
       * Constructs an instance of @c Instant using seconds from the epoch of
       * 1970-01-01T00:00:00Z and nanosecond fraction of second.
       *
       * @param epochSecond  the number of seconds from 1970-01-01T00:00:00Z
       * @param nanos  the nanoseconds within the second, must be positive
       */
      CORE_EXPLICIT Instant(glong epochSecond, gint nanos);

    public:
      /**
       * Constant for the 1970-01-01T00:00:00Z epoch instant.
       */
      static const Instant EPOCH;

      /**
       * The minimum supported @c Instant, '-1000000000-01-01T00:00Z'.
       * This could be used by an application as a "far past" instant.
       * <p>
       * This is one year earlier than the minimum @c LocalDateTime.
       * This provides sufficient values to handle the range of @c ZoneOffset
       * which affect the instant in addition to the local date-time.
       * The value is also chosen such that the value of the year fits in
       * an @c int.
       * </p>
       */
      static const Instant MIN;

      /**
       * The maximum supported @c Instant, '1000000000-12-31T23:59:59.999999999Z'.
       * This could be used by an application as a "far future" instant.
       * <p>
       * This is one year later than the maximum @c LocalDateTime.
       * This provides sufficient values to handle the range of @c ZoneOffset
       * which affect the instant in addition to the local date-time.
       * The value is also chosen such that the value of the year fits in
       * an @c int.
       * </p>
       */
      static const Instant MAX;

      /**
       * Obtains the current instant from the system clock.
       * <p>
       * Using this method will prevent the ability to use an alternate time-source for
       * testing because the clock is effectively hard-coded.
       * </p>
       * @return the current instant using the system clock
       */
      static Instant now();

      /**
       * Obtains an instance of @c Instant using seconds from the
       * epoch of 1970-01-01T00:00:00Z.
       * <p>
       * The nanosecond field is set to zero.
       * </p>
       * @param epochSecond  the number of seconds from 1970-01-01T00:00:00Z
       * @return an instant
       * @throws DateTimeException if the instant exceeds the maximum or minimum instant
       */
      static Instant ofEpochSecond(glong epochSecond);

      /**
       * Obtains an instance of @c Instant using seconds from the
       * epoch of 1970-01-01T00:00:00Z and nanosecond fraction of second.
       * <p>
       * This method allows an arbitrary number of nanoseconds to be passed in.
       * The factory will alter the values of the second and nanosecond in order
       * to ensure that the stored nanosecond is in the range 0 to 999,999,999.
       * For example, the following will result in exactly the same instant:
       * @code
       *  Instant::ofEpochSecond(3, 1);
       *  Instant::ofEpochSecond(4, -999_999_999);
       *  Instant::ofEpochSecond(2, 1000_000_001);
       * @endcode
       * </p>
       * @param epochSecond  the number of seconds from 1970-01-01T00:00:00Z
       * @param nanoAdjustment  the nanosecond adjustment to the number of seconds, positive or negative
       * @return an instant
       * @throws DateTimeException if the instant exceeds the maximum or minimum instant
       * @throws ArithmeticException if numeric overflow occurs
       */
      static Instant ofEpochSecond(glong epochSecond, glong nanoAdjustment);

      /**
       * Obtains an instance of @c Instant using milliseconds from the
       * epoch of 1970-01-01T00:00:00Z.
       * <p>
       * The seconds and nanoseconds are extracted from the specified milliseconds.
       * </p>
       * @param epochMilli  the number of milliseconds from 1970-01-01T00:00:00Z
       * @return an instant
       */
      static Instant ofEpochMilli(glong epochMilli);

      /**
       * Obtains an instance of @c Instant from a temporal object.
       * <p>
       * This obtains an instant based on the specified temporal.
       * A @c TemporalAccessor represents an arbitrary set of date and time information,
       * which this factory converts to an instance of @c Instant.
       * </p>
       * <p>
       * The conversion extracts the @b INSTANT_SECONDS
       * and @b NANO_OF_SECOND fields.
       * </p>
       * <p>
       * This method matches the signature of the functional interface @b TemporalQuery
       * allowing it to be used as a query via method reference, @c Instant::from.
       * </p>
       * @param temporal  the temporal object to convert
       * @return the instant
       * @throws DateTimeException if unable to convert to an @c Instant
       */
      static Instant from(TemporalAccessor const& temporal);

      /**
       * Checks if the specified field is supported.
       * <p>
       * This checks if this instant can be queried for the specified field.
       * If false, then calling the @b range,
       * @b get and @b with methods will throw an exception.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The supported fields are: <br/>
       * - @c NANO_OF_SECOND <br/>
       * - @c MICRO_OF_SECOND <br/>
       * - @c MILLI_OF_SECOND <br/>
       * - @c INSTANT_SECONDS <br/>
       * All other @c TemporalField instances will return false.
       * </p>
       * @param field  the field to check, null returns false
       * @return true if the field is supported on this instant, false if not
       */
      gbool isSupported(TemporalField field) const override;

      /**
       * Checks if the specified unit is supported.
       * <p>
       * This checks if the specified unit can be added to, or subtracted from, this date-time.
       * If false, then calling the @b plus and
       * @b minus minus methods will throw an exception.
       * </p>
       * <p>
       * If the unit is a @b TemporalUnit then the query is implemented here.
       * The supported units are: <br/>
       *
       * - @c NANOS <br/>
       * - @c MICROS <br/>
       * - @c MILLIS <br/>
       * - @c SECONDS <br/>
       * - @c MINUTES <br/>
       * - @c HOURS <br/>
       * - @c HALF_DAYS <br/>
       * - @c DAYS <br/>
       *
       * All other @c TemporalUnit instances will return false.
       * </p>
       *
       * @param unit  the unit to check, null returns false
       * @return true if the unit can be added/subtracted, false if not
       */
      gbool isSupported(TemporalUnit unit) const override;

      /**
       * Gets the range of valid values for the specified field.
       * <p>
       * The range object expresses the minimum and maximum valid values for a field.
       * This instant is used to enhance the accuracy of the returned range.
       * If it is not possible to return the range, because the field is not supported
       * or for some other reason, an exception is thrown.
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The @b supported fields will return
       * appropriate range instances.
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       * @param field  the field to query the range for
       * @return the range of valid values for the field
       * @throws DateTimeException if the range for the field cannot be obtained
       * @throws TemporalException if the field is not supported
       */
      ValueRange range(TemporalField field) const override;

      /**
       * Gets the value of the specified field from this instant as an @c int.
       * <p>
       * This queries this instant for the value of the specified field.
       * The returned value will always be within the valid range of values for the field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The <b>supported fields</b> will return valid
       * values based on this date-time, except @c INSTANT_SECONDS which is too
       * large to fit in an @c int and throws a @c DateTimeException.
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       *
       * @param field  the field to get
       * @return the value for the field
       * @throws DateTimeException if a value for the field cannot be obtained or
       *         the value is outside the range of valid values for the field
       * @throws TemporalException if the field is not supported or
       *         the range of values exceeds an @c int
       * @throws ArithmeticException if numeric overflow occurs
       */
      gint get(TemporalField field) const override;

      /**
       * Gets the value of the specified field from this instant as a @c long.
       * <p>
       * This queries this instant for the value of the specified field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The <b>supported fields</b> will return valid
       * values based on this date-time.
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       *
       * @param field  the field to get
       * @return the value for the field
       * @throws DateTimeException if a value for the field cannot be obtained
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong getLong(TemporalField field) const override;

      /**
       * Gets the number of seconds from the Java epoch of 1970-01-01T00:00:00Z.
       * <p>
       * The epoch second count is a simple incrementing count of seconds where
       * second 0 is 1970-01-01T00:00:00Z.
       * The nanosecond part is returned by @b nano.
       * </p>
       *
       * @return the seconds from the epoch of 1970-01-01T00:00:00Z
       */
      glong epochSecond() const;

      /**
       * Gets the number of nanoseconds, later along the time-line, from the start
       * of the second.
       * <p>
       * The nanosecond-of-second value measures the total number of nanoseconds from
       * the second returned by @b epochSecond.
       * </p>
       *
       * @return the nanoseconds within the second, always positive, never exceeds 999,999,999
       */
      gint nano() const;

      /**
       * Returns an adjusted copy of this instant.
       * <p>
       * This returns an @c Instant, based on this one, with the instant adjusted.
       * The adjustment takes place using the specified adjuster strategy object.
       * Read the documentation of the adjuster to understand what adjustment will be made.
       * <p>
       * The result of this method is obtained by invoking the
       * @b TemporalAdjuster::adjustInto(Temporal) method on the
       * specified adjuster passing @c this as the argument.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param adjuster the adjuster to use
       * @return an @c Instant based on @c this with the adjustment made
       * @throws DateTimeException if the adjustment cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant with(TemporalAdjuster const& adjuster) const override;

      /**
       * Returns a copy of this instant with the specified field set to a new value.
       * <p>
       * This returns an @c Instant, based on this one, with the value
       * for the specified field changed.
       * If it is not possible to set the value, because the field is not supported or for
       * some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the adjustment is implemented here.
       * The supported fields behave as follows: <br/>
       *
       * - @c NANO_OF_SECOND -
       *  Returns an @c Instant with the specified nano-of-second.
       *  The epoch-second will be unchanged. <br/>
       * - @c MICRO_OF_SECOND -
       *  Returns an @c Instant with the nano-of-second replaced by the specified
       *  micro-of-second multiplied by 1,000. The epoch-second will be unchanged. <br/>
       * - @c MILLI_OF_SECOND -
       *  Returns an @c Instant with the nano-of-second replaced by the specified
       *  milli-of-second multiplied by 1,000,000. The epoch-second will be unchanged. <br/>
       * - @c INSTANT_SECONDS -
       *  Returns an @c Instant with the specified epoch-second.
       *  The nano-of-second will be unchanged. <br/>
       * </p>
       * <p>
       * In all cases, if the new value is outside the valid range of values for the field
       * then a @c DateTimeException will be thrown.
       * </p>
       * <p>
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param field  the field to set in the result
       * @param newValue  the new value of the field in the result
       * @return an @c Instant based on @c this with the specified field set
       * @throws DateTimeException if the field cannot be set
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant with(TemporalField field, glong newValue) const override;

      /**
       * Returns a copy of this @c Instant truncated to the specified unit.
       * <p>
       * Truncating the instant returns a copy of the original with fields
       * smaller than the specified unit set to zero.
       * The fields are calculated on the basis of using a UTC offset as seen
       * in @c toString.
       * For example, truncating with the @b MINUTES unit will
       * round down to the nearest minute, setting the seconds and nanoseconds to zero.
       * </p>
       * <p>
       * The unit must have a @em duration
       * that divides into the length of a standard day without remainder.
       * This includes all supplied time units on @b TemporalUnit and
       * @b DAYS. Other units throw an exception.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param unit  the unit to truncate to
       * @return an @c Instant based on this instant with the time truncated
       * @throws DateTimeException if the unit is invalid for truncation
       * @throws TemporalException if the unit is not supported
       */
      Instant truncatedTo(TemporalUnit unit) const;

      /**
       * Returns a copy of this instant with the specified amount added.
       * <p>
       * This returns an @c Instant, based on this one, with the specified amount added.
       * The amount is typically @b Duration but may be any other type implementing
       * the @b TemporalAmount interface.
       * </p>
       * <p>
       * The calculation is delegated to the amount object by calling
       * @b TemporalAmount::addTo(Temporal). The amount implementation is free
       * to implement the addition in any way it wishes, however it typically
       * calls back to @b plus(long, TemporalUnit). Consult the documentation
       * of the amount implementation to determine if it can be successfully added.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param amountToAdd  the amount to add
       * @return an @c Instant based on this instant with the addition made
       * @throws DateTimeException if the addition cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant plus(TemporalAmount const& amountToAdd) const override;

      /**
       * Returns a copy of this instant with the specified amount added.
       * <p>
       * This returns an @c Instant, based on this one, with the amount
       * in terms of the unit added. If it is not possible to add the amount, because the
       * unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalUnit then the addition is implemented here.
       * The supported fields behave as follows: <br/>
       *
       * - @c NANOS -
       *  Returns an @c Instant with the specified number of nanoseconds added.
       *  This is equivalent to @b plusNanos(long). <br/>
       * - @c MICROS -
       *  Returns an @c Instant with the specified number of microseconds added.
       *  This is equivalent to @b plusNanos(long) with the amount
       *  multiplied by 1,000. <br/>
       * - @c MILLIS -
       *  Returns an @c Instant with the specified number of milliseconds added.
       *  This is equivalent to @b plusNanos(long) with the amount
       *  multiplied by 1,000,000. <br/>
       * - @c SECONDS -
       *  Returns an @c Instant with the specified number of seconds added.
       *  This is equivalent to @b plusSeconds(long). <br/>
       * - @c MINUTES -
       *  Returns an @c Instant with the specified number of minutes added.
       *  This is equivalent to @b plusSeconds(long) with the amount
       *  multiplied by 60. <br/>
       * - @c HOURS -
       *  Returns an @c Instant with the specified number of hours added.
       *  This is equivalent to @b plusSeconds(long) with the amount
       *  multiplied by 3,600. <br/>
       * - @c HALF_DAYS -
       *  Returns an @c Instant with the specified number of half-days added.
       *  This is equivalent to @b plusSeconds(long) with the amount
       *  multiplied by 43,200 (12 hours). <br/>
       * - @c DAYS -
       *  Returns an @c Instant with the specified number of days added.
       *  This is equivalent to @b plusSeconds(long) with the amount
       *  multiplied by 86,400 (24 hours). <br/>
       *
       * <p>
       * All other @c TemporalUnit instances will throw an @c TemporalException.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param amountToAdd  the amount of the unit to add to the result, may be negative
       * @param unit  the unit of the amount to add
       * @return an @c Instant based on this instant with the specified amount added
       * @throws DateTimeException if the addition cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant plus(glong amountToAdd, TemporalUnit unit) const override;

      /**
       * Returns a copy of this instant with the specified duration in seconds added.
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param secondsToAdd  the seconds to add, positive or negative
       * @return an @c Instant based on this instant with the specified seconds added
       * @throws DateTimeException if the result exceeds the maximum or minimum instant
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant plusSeconds(glong secondsToAdd) const;

      /**
       * Returns a copy of this instant with the specified duration in milliseconds added.
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param millisToAdd  the milliseconds to add, positive or negative
       * @return an @c Instant based on this instant with the specified milliseconds added
       * @throws DateTimeException if the result exceeds the maximum or minimum instant
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant plusMillis(glong millisToAdd) const;

      /**
       * Returns a copy of this instant with the specified duration in nanoseconds added.
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param nanosToAdd  the nanoseconds to add, positive or negative
       * @return an @c Instant based on this instant with the specified nanoseconds added
       * @throws DateTimeException if the result exceeds the maximum or minimum instant
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant plusNanos(glong nanosToAdd) const;

      /**
       * Returns a copy of this instant with the specified amount subtracted.
       * <p>
       * This returns an @c Instant, based on this one, with the specified amount subtracted.
       * The amount is typically @b Duration but may be any other type implementing
       * the @b TemporalAmount interface.
       * <p>
       * The calculation is delegated to the amount object by calling
       * @b TemporalAmount::subtractFrom(Temporal). The amount implementation is free
       * to implement the subtraction in any way it wishes, however it typically
       * calls back to @b minus(long, TemporalUnit). Consult the documentation
       * of the amount implementation to determine if it can be successfully subtracted.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param amountToSubtract  the amount to subtract
       * @return an @c Instant based on this instant with the subtraction made
       * @throws DateTimeException if the subtraction cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant minus(TemporalAmount const& amountToSubtract) const override;

      /**
       * Returns a copy of this instant with the specified amount subtracted.
       * <p>
       * This returns an @c Instant, based on this one, with the amount
       * in terms of the unit subtracted. If it is not possible to subtract the amount,
       * because the unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * This method is equivalent to @b plus(long, TemporalUnit) with the amount negated.
       * See that method for a full description of how addition, and thus subtraction, works.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
       * @param unit  the unit of the amount to subtract
       * @return an @c Instant based on this instant with the specified amount subtracted
       * @throws DateTimeException if the subtraction cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant minus(glong amountToSubtract, TemporalUnit unit) const override;

      /**
       * Returns a copy of this instant with the specified duration in seconds subtracted.
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param secondsToSubtract  the seconds to subtract, positive or negative
       * @return an @c Instant based on this instant with the specified seconds subtracted
       * @throws DateTimeException if the result exceeds the maximum or minimum instant
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant minusSeconds(glong secondsToSubtract) const;

      /**
       * Returns a copy of this instant with the specified duration in milliseconds subtracted.
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param millisToSubtract  the milliseconds to subtract, positive or negative
       * @return an @c Instant based on this instant with the specified milliseconds subtracted
       * @throws DateTimeException if the result exceeds the maximum or minimum instant
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant minusMillis(glong millisToSubtract) const;

      /**
       * Returns a copy of this instant with the specified duration in nanoseconds subtracted.
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param nanosToSubtract  the nanoseconds to subtract, positive or negative
       * @return an @c Instant based on this instant with the specified nanoseconds subtracted
       * @throws DateTimeException if the result exceeds the maximum or minimum instant
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant minusNanos(glong nanosToSubtract) const;

      /**
       * Queries this instant using the specified query.
       * <p>
       * This queries this instant using the specified query strategy object.
       * The @c TemporalQuery object defines the logic to be used to
       * obtain the result. Read the documentation of the query to understand
       * what the result of this method will be.
       * <p>
       * The result of this method is obtained by invoking the
       * @b TemporalQuery::queryFrom(TemporalAccessor) method on the
       * specified query passing @c this as the argument.
       *
       * @param query  the query to invoke
       * @return the query result, null may be returned (defined by the query)
       * @throws DateTimeException if unable to query (defined by the query)
       * @throws ArithmeticException if numeric overflow occurs (defined by the query)
       */
      Optional query(const TemporalQuery& query) const override;

      /**
       * Adjusts the specified temporal object to have this instant.
       * <p>
       * This returns a temporal object of the same observable type as the input
       * with the instant changed to be the same as this.
       * <p>
       * The adjustment is equivalent to using @b Temporal::with(TemporalField, long)
       * twice, passing @b TemporalField::INSTANT_SECONDS and
       * @b TemporalField::NANO_OF_SECOND as the fields.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param temporal  the target object to be adjusted, not null
       * @return the adjusted object, not null
       * @throws DateTimeException if unable to make the adjustment
       * @throws ArithmeticException if numeric overflow occurs
       */
      Temporal& adjustInto(Temporal const& temporal) const override;

      /**
       * Calculates the amount of time until another instant in terms of the specified unit.
       * <p>
       * This calculates the amount of time between two @c Instant
       * objects in terms of a single @c TemporalUnit.
       * The start and end points are @c this and the specified instant.
       * The result will be negative if the end is before the start.
       * The calculation returns a whole number, representing the number of
       * complete units between the two instants.
       * The @c Temporal passed to this method is converted to a
       * @c Instant using @b from(TemporalAccessor).
       * For example, the amount in seconds between two dates can be calculated
       * using @code startInstant.until(endInstant, SECONDS) @endcode.
       * </p>
       * <p>
       * The calculation is implemented in this method for @b TemporalUnit.
       * The units @c NANOS, @c MICROS, @c MILLIS, @c SECONDS,
       * @c MINUTES, @c HOURS, @c HALF_DAYS and @c DAYS
       * are supported. Other @c TemporalUnit values will throw an exception.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param endExclusive  the end date, exclusive, which is converted to an @c Instant
       * @param unit  the unit to measure the amount in
       * @return the amount of time between this instant and the end instant
       * @throws DateTimeException if the amount cannot be calculated, or the end
       *  temporal cannot be converted to an @c Instant
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong until(const Temporal& endExclusive, TemporalUnit unit) const override;

      /**
       * Calculates the @c Duration until another @c Instant.
       * <p>
       * The start and end points are @c this and the specified instant.
       * The result will be negative if the end is before the start. Calling
       * this method is equivalent to
       * @b Duration::between(this, endExclusive).
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param endExclusive the end @c Instant, exclusive
       * @return the @c Duration from this @c Instant until the
       *      specified @c endExclusive @c Instant
       * @see Duration::between(Temporal, Temporal)
       */
      Duration until(Instant const& endExclusive) const;

      /**
       * Combines this instant with an offset to create an @c OffsetDateTime.
       * <p>
       * This returns an @c OffsetDateTime formed from this instant at the
       * specified offset from UTC/Greenwich. An exception will be thrown if the
       * instant is too large to fit into an offset date-time.
       * </p>
       * <p>
       * This method is equivalent to @b OffsetDateTime::ofInstant(this, offset).
       * </p>
       *
       * @param offset  the offset to combine with
       * @return the offset date-time formed from this instant and the specified offset
       * @throws DateTimeException if the result exceeds the supported range
       */
      OffsetDateTime atOffset(ZoneOffset const& offset) const;

      /**
       * Combines this instant with a time-zone to create a @c ZonedDateTime.
       * <p>
       * This returns an @c ZonedDateTime formed from this instant at the
       * specified time-zone. An exception will be thrown if the instant is too
       * large to fit into a zoned date-time.
       * </p>
       * <p>
       * This method is equivalent to @b ZonedDateTime::ofInstant(this, zone).
       * </p>
       *
       * @param zone  the zone to combine with
       * @return the zoned date-time formed from this instant and the specified zone
       * @throws DateTimeException if the result exceeds the supported range
       */
      ZonedDateTime atZone(ZoneId const& zone) const;

      /**
       * Converts this instant to the number of milliseconds from the epoch
       * of 1970-01-01T00:00:00Z.
       * <p>
       * If this instant represents a point on the time-line too far in the future
       * or past to fit in a @c long milliseconds, then an exception is thrown.
       * </p>
       * <p>
       * If this instant has greater than millisecond precision, then the conversion
       * will drop any excess precision information as though the amount in nanoseconds
       * was subject to integer division by one million.
       * </p>
       *
       * @return the number of milliseconds since the epoch of 1970-01-01T00:00:00Z
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong toEpochMilli() const;

      /**
       * Compares this instant to the specified instant.
       * <p>
       * The comparison is based on the time-line position of the instants.
       * It is "consistent with equals", as defined by @b Comparable.
       * </p>
       *
       * @param otherInstant  the other instant to compare to
       * @return the comparator value, that is less than zero if this instant is before @c otherInstant,
       *          zero if they are equal, or greater than zero if this instant is after @c otherInstant
       *
       * @see isBefore
       * @see isAfter
       */
      gint compareTo(const Instant& otherInstant) const override;

      /**
       * Checks if this instant is after the specified instant.
       * <p>
       * The comparison is based on the time-line position of the instants.
       * </p>
       *
       * @param otherInstant  the other instant to compare to
       * @return true if this instant is after the specified instant
       *
       */
      gbool isAfter(const Instant& otherInstant) const;

      /**
       * Checks if this instant is before the specified instant.
       * <p>
       * The comparison is based on the time-line position of the instants.
       * </p>
       *
       * @param otherInstant  the other instant to compare to
       * @return true if this instant is before the specified instant
       *
       */
      gbool isBefore(const Instant& otherInstant) const;

      /**
       * Checks if this instant is equal to the specified instant.
       * <p>
       * The comparison is based on the time-line position of the instants.
       * </p>
       *
       * @param other  the other instant, null returns false
       * @return true if the other instant is equal to this one
       */
      gbool equals(const Object& other) const override;

      /**
       * Returns a hash code for this instant.
       *
       * @return a suitable hash code
       */
      gint hash() const override;

      /**
       * A string representation of this instant using ISO-8601 representation.
       * <p>
       * The format used is the same as @b DateTimeFormatter::ISO_INSTANT.
       * </p>
       *
       * @return an ISO-8601 representation of this instant
       */
      String toString() const override;

      /**
       * Return shadow copy of this object
       *
       * @return The shadow copy of this object
       */
      Object& clone() const override;

    private:
      // Helpers

      /**
       * Obtains an instance of @c Instant using seconds and nanoseconds.
       *
       * @param seconds  the length of the duration in seconds
       * @param nanoOfSecond  the nano-of-second, from 0 to 999,999,999
       * @throws DateTimeException if the instant exceeds the maximum or minimum instant
       */
      static Instant create(glong seconds, gint nanoOfSecond);

      /**
       * Returns a copy of this instant with the specified duration added.
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       *
       * @param secondsToAdd  the seconds to add, positive or negative
       * @param nanosToAdd  the nanos to add, positive or negative
       * @return an @c Instant based on this instant with the specified seconds added
       * @throws DateTimeException if the result exceeds the maximum or minimum instant
       * @throws ArithmeticException if numeric overflow occurs
       */
      Instant plus(glong secondsToAdd, glong nanosToAdd) const;

      glong nanosUntil(Instant const& end) const;

      glong microsUntil(Instant const& end) const;

      glong millisUntil(Instant const& end) const;

      glong secondsUntil(Instant const& end) const;
    };
  } // time
} // core

#endif //CORE24_CORE_24_INSTANT_H
