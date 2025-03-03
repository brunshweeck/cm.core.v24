//
// Created by admin on 07/01/25.
//

#ifndef CORE24_OFFSETTIME_H
#define CORE24_OFFSETTIME_H

#include <core/time/LocalTime.h>
#include <core/time/ZoneOffset.h>

namespace core {
  namespace time {
    /**
     * A time with an offset from UTC/Greenwich in the ISO-8601 calendar system,
     * such as @c 10:15:30+01:00.
     * <p>
     * @c OffsetTime is an immutable date-time object that represents a time, often
     * viewed as hour-minute-second-offset.
     * This class stores all time fields, to a precision of nanoseconds,
     * as well as a zone offset.
     * For example, the value "13:45:30.123456789+02:00" can be stored
     * in an @c OffsetTime.
     * </p>
     */
    class OffsetTime final : public virtual TemporalBase<OffsetTime>,
                             public virtual TemporalAdjuster,
                             public virtual Comparable<OffsetTime> {
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, Optional);

    public:
      /**
       * The minimum supported @c OffsetTime, '00:00:00+18:00'.
       * This is the time of midnight at the start of the day in the maximum offset
       * (larger offsets are earlier on the time-line).
       * This combines @b LocalTime#MIN and @b ZoneOffset::MAX.
       * This could be used by an application as a "far past" date.
       */
      static OffsetTime const MIN;

      /**
       * The maximum supported @c OffsetTime, '23:59:59.999999999-18:00'.
       * This is the time just before midnight at the end of the day in the minimum offset
       * (larger negative offsets are later on the time-line).
       * This combines @b LocalTime#MAX and @b ZoneOffset#MIN.
       * This could be used by an application as a "far future" date.
       */
      static OffsetTime const MAX;

    private:
      /**
       * The local date-time.
       */
      LocalTime time;

      /**
       * The offset from UTC/Greenwich.
       */
      ZoneOffset offset;

      /**
       * Constructor.
       *
       * @param time  the local time, not null
       * @param offset  the zone offset, not null
       */
      OffsetTime(LocalTime const& time, ZoneOffset const& offset);

    public:
      /**
       * Obtains the current time from the system clock in the default time-zone.
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       * </p>
       * @return the current time using the system clock and default time-zone
       */
      static OffsetTime now();

      /**
       * Obtains the current time from the system clock in the specified time-zone.
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       * </p>
       * @param zone  the zone ID to use
       * @return the current time using the system clock
       */
      static OffsetTime now(ZoneId const& zone);

      /**
       * Obtains an instance of @c OffsetTime from a local time and an offset.
       *
       * @param time  the local time
       * @param offset  the zone offset
       * @return the offset time
       */
      static OffsetTime of(LocalTime const& time, ZoneOffset const& offset);

      /**
       * Obtains an instance of @c OffsetTime from an hour, minute, second and nanosecond.
       * <p>
       * This creates an offset time with the four specified fields.
       * </p>
       * <p>
       * This method exists primarily for writing test cases.
       * Non test-code will typically use other methods to create an offset time.
       * @c LocalTime has two additional convenience variants of the
       * equivalent factory method taking fewer arguments.
       * They are not provided here to reduce the footprint of the API.
       * </p>
       * @param hour  the hour-of-day to represent, from 0 to 23
       * @param minute  the minute-of-hour to represent, from 0 to 59
       * @param second  the second-of-minute to represent, from 0 to 59
       * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
       * @param offset  the zone offset
       * @return the offset time
       * @throws DateTimeException if the value of any field is out of range
       */
      static OffsetTime of(gint hour, gint minute, gint second, gint nanoOfSecond, ZoneOffset const& offset);

      /**
       * Obtains an instance of @c OffsetTime from an @c Instant and zone ID.
       * <p>
       * This creates an offset time with the same instant as that specified.
       * Finding the offset from UTC/Greenwich is simple as there is only one valid
       * offset for each instant.
       * </p>
       * <p>
       * The date component of the instant is dropped during the conversion.
       * This means that the conversion can never fail due to the instant being
       * out of the valid range of dates.
       * </p>
       * @param instant  the instant to create the time from, not null
       * @param zone  the time-zone, which may be an offset, not null
       * @return the offset time, not null
       */
      static OffsetTime ofInstant(Instant const& instant, ZoneId const& zone);

      /**
       * Obtains an instance of @c OffsetTime from a temporal object.
       * <p>
       * This obtains an offset time based on the specified temporal.
       * A @c Temporal represents an arbitrary set of date and time information,
       * which this factory converts to an instance of @c OffsetTime.
       * </p>
       * <p>
       * The conversion extracts and combines the @c ZoneOffset and the
       * @c LocalTime from the temporal object.
       * Implementations are permitted to perform optimizations such as accessing
       * those fields that are equivalent to the relevant objects.
       * </p>
       * <p>
       * This method matches the signature of the functional interface @b TemporalQuery
       * allowing it to be used as a query via method reference, @c OffsetTime::from.
       * </p>
       * @param temporal  the temporal object to convert
       * @return the offset time
       * @throws DateTimeException if unable to convert to an @c OffsetTime
       */
      static OffsetTime from(TemporalAccessor const& temporal);

      /**
       * Obtains an instance of @c OffsetTime from a text string such as @c 10:15:30+01:00.
       * <p>
       * The string must represent a valid time and is parsed using
       * @b DateTimeFormatter::ISO_OFFSET_TIME.
       *
       * @param text  the text to parse such as "10:15:30+01:00", not null
       * @return the parsed local time, not null
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static OffsetTime parse(CharSequence const& text);

      /**
       * Obtains an instance of @c OffsetTime from a text string using a specific formatter.
       * <p>
       * The text is parsed using the formatter, returning a time.
       *
       * @param text  the text to parse, not null
       * @param formatter  the formatter to use, not null
       * @return the parsed offset time, not null
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static OffsetTime parse(CharSequence const& text, DateTimeFormatter const& formatter);

      /**
       * Checks if the specified field is supported.
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
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
       * @li @c OFFSET_SECONDS
       * </p>
       * All other @c TemporalField instances will return false.
       *
       * @param field  the field to check, null returns false
       * @return true if the field is supported on this time, false if not
       */
      gbool isSupported(TemporalField field) const override;

      /**
       * Checks if the specified unit is supported.
       * <p>
       * If the unit is a @b TemporalUnit then the query is implemented here.
       * The supported units are:
       *
       * @li @c NANOS
       * @li @c MICROS
       * @li @c MILLIS
       * @li @c SECONDS
       * @li @c MINUTES
       * @li @c HOURS
       * @li @c HALF_DAYS
       * </p>
       * All other @c TemporalUnit instances will return false.
       *
       * @param unit  the unit to check, null returns false
       * @return true if the unit can be added/subtracted, false if not
       */
      gbool isSupported(TemporalUnit unit) const override;

      /**
       * Gets the range of valid values for the specified field.
       * <p>
       * The range object expresses the minimum and maximum valid values for a field.
       * This time is used to enhance the accuracy of the returned range.
       * If it is not possible to return the range, because the field is not supported
       * or for some other reason, an exception is thrown.
       *
       * @param field  the field to query the range for, not null
       * @return the range of valid values for the field, not null
       * @throws DateTimeException if the range for the field cannot be obtained
       * @throws UnsupportedTemporalTypeException if the field is not supported
       */
      ValueRange range(TemporalField field) const override;

      /**
       * Gets the value of the specified field from this time as an @c int.
       * <p>
       * This queries this time for the value of the specified field.
       * The returned value will always be within the valid range of values for the field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
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
       * Gets the value of the specified field from this time as a @c long.
       * <p>
       * This queries this time for the value of the specified field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
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
       * Gets the zone offset, such as '+01:00'.
       * <p>
       * This is the offset of the local time from UTC/Greenwich.
       * </p>
       * @return the zone offset
       */
      ZoneOffset toOffset() const;

      /**
       * Returns a copy of this @c OffsetTime with the specified offset ensuring
       * that the result has the same local time.
       * <p>
       * This method returns an object with the same @c LocalTime and the specified @c ZoneOffset.
       * No calculation is needed or performed.
       * For example, if this time represents @c 10:30+02:00 and the offset specified is
       * @c +03:00, then this method will return @c 10:30+03:00.
       * </p>
       * <p>
       * To take into account the difference between the offsets, and adjust the time fields,
       * use @b withOffsetSameInstant.
       * </p>
       *
       * @param offset  the zone offset to change to
       * @return an @c OffsetTime based on this time with the requested offset
       */
      OffsetTime withOffsetSameLocal(ZoneOffset const& offset) const;

      /**
       * Returns a copy of this @c OffsetTime with the specified offset ensuring
       * that the result is at the same instant on an implied day.
       * <p>
       * This method returns an object with the specified @c ZoneOffset and a @c LocalTime
       * adjusted by the difference between the two offsets.
       * This will result in the old and new objects representing the same instant on an implied day.
       * This is useful for finding the local time in a different offset.
       * For example, if this time represents @c 10:30+02:00 and the offset specified is
       * @c +03:00, then this method will return @c 11:30+03:00.
       * </p>
       * <p>
       * To change the offset without adjusting the local time use @b withOffsetSameLocal.
       * </p>
       * @param offset  the zone offset to change to
       * @return an @c OffsetTime based on this time with the requested offset
       */
      OffsetTime withOffsetSameInstant(ZoneOffset const& offset) const;

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
       * Returns an adjusted copy of this time.
       * <p>
       * This returns an {@code OffsetTime}, based on this one, with the time adjusted.
       * The adjustment takes place using the specified adjuster strategy object.
       * Read the documentation of the adjuster to understand what adjustment will be made.
       * <p>
       * A simple adjuster might simply set the one of the fields, such as the hour field.
       * A more complex adjuster might set the time to the last hour of the day.
       * <p>
       * The classes {@link LocalTime} and {@link ZoneOffset} implement {@code TemporalAdjuster},
       * thus this method can be used to change the time or offset:
       * <pre>
       *  result = offsetTime.with(time);
       *  result = offsetTime.with(offset);
       * </pre>
       * <p>
       * The result of this method is obtained by invoking the
       * {@link TemporalAdjuster#adjustInto(Temporal)} method on the
       * specified adjuster passing {@code this} as the argument.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param adjuster the adjuster to use, not null
       * @return an {@code OffsetTime} based on {@code this} with the adjustment made, not null
       * @throws DateTimeException if the adjustment cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetTime with(TemporalAdjuster const& adjuster) const override;

      /**
       * Returns a copy of this time with the specified field set to a new value.
       * <p>
       * This returns an @c OffsetTime, based on this one, with the value
       * for the specified field changed.
       * This can be used to change any supported field, such as the hour, minute or second.
       * If it is not possible to set the value, because the field is not supported or for
       * some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the adjustment is implemented here.
       * </p>
       * <p>
       * The @c OFFSET_SECONDS field will return a time with the specified offset.
       * The local time is unaltered. If the new offset value is outside the valid range
       * then a @c DateTimeException will be thrown.
       * </p>
       * <p>
       * The other supported fields will behave as per
       * the matching method on @b LocalTime.
       * In this case, the offset is not part of the calculation and will be unchanged.
       * </p>
       * <p>
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       * @param field  the field to set in the result
       * @param newValue  the new value of the field in the result
       * @return an @c OffsetTime based on @c this with the specified field set
       * @throws DateTimeException if the field cannot be set
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetTime with(TemporalField field, glong newValue) const override;

      /**
       * Returns a copy of this @c OffsetTime with the hour-of-day altered.
       * <p>
       * The offset does not affect the calculation and will be the same in the result.
       * </p>
       * @param hour  the hour-of-day to set in the result, from 0 to 23
       * @return an @c OffsetTime based on this time with the requested hour
       * @throws DateTimeException if the hour value is invalid
       */
      OffsetTime withHour(gint hour) const;

      /**
       * Returns a copy of this @c OffsetTime with the minute-of-hour altered.
       * <p>
       * The offset does not affect the calculation and will be the same in the result.
       * </p>
       * @param minute  the minute-of-hour to set in the result, from 0 to 59
       * @return an @c OffsetTime based on this time with the requested minute
       * @throws DateTimeException if the minute value is invalid
       */
      OffsetTime withMinute(gint minute) const;

      /**
       * Returns a copy of this @c OffsetTime with the second-of-minute altered.
       * <p>
       * The offset does not affect the calculation and will be the same in the result.
       * </p>
       * @param second  the second-of-minute to set in the result, from 0 to 59
       * @return an @c OffsetTime based on this time with the requested second
       * @throws DateTimeException if the second value is invalid
       */
      OffsetTime withSecond(gint second) const;

      /**
       * Returns a copy of this @c OffsetTime with the nano-of-second altered.
       * <p>
       * The offset does not affect the calculation and will be the same in the result.
       * </p>
       * @param nanoOfSecond  the nano-of-second to set in the result, from 0 to 999,999,999
       * @return an @c OffsetTime based on this time with the requested nanosecond
       * @throws DateTimeException if the nanos value is invalid
       */
      OffsetTime withNano(gint nanoOfSecond) const;

      /**
       * Returns a copy of this @c OffsetTime with the time truncated.
       * <p>
       * Truncation returns a copy of the original time with fields
       * smaller than the specified unit set to zero.
       * For example, truncating with the @b minutes unit
       * will set the second-of-minute and nano-of-second field to zero.
       * </p>
       * <p>
       * The unit must have a @em duration
       * that divides into the length of a standard day without remainder.
       * This includes all supplied time units on @b TemporalUnit and
       * @b DAYS. Other units throw an exception.
       * </p>
       * <p>
       * The offset does not affect the calculation and will be the same in the result.
       * </p>
       * @param unit  the unit to truncate to
       * @return an @c OffsetTime based on this time with the time truncated
       * @throws DateTimeException if unable to truncate
       * @throws TemporalException if the unit is not supported
       */
      OffsetTime truncatedTo(TemporalUnit unit) const;

      /**
       * Returns a copy of this time with the specified amount added.
       * <p>
       * This returns an @c OffsetTime, based on this one, with the specified amount added.
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
       *
       * @param amountToAdd  the amount to add, not null
       * @return an @c OffsetTime based on this time with the addition made, not null
       * @throws DateTimeException if the addition cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetTime plus(TemporalAmount const& amountToAdd) const override;

      /**
       * Returns a copy of this time with the specified amount added.
       * <p>
       * This returns an @c OffsetTime, based on this one, with the amount
       * in terms of the unit added. If it is not possible to add the amount, because the
       * unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalUnit then the addition is implemented by
       * @b LocalTime::plus(long, TemporalUnit).
       * The offset is not part of the calculation and will be unchanged in the result.
       * </p>
       * @param amountToAdd  the amount of the unit to add to the result, may be negative
       * @param unit  the unit of the amount to add
       * @return an @c OffsetTime based on this time with the specified amount added
       * @throws DateTimeException if the addition cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetTime plus(glong amountToAdd, TemporalUnit unit) const override;

      /**
       * Returns a copy of this @c OffsetTime with the specified number of hours added.
       * <p>
       * This adds the specified number of hours to this time, returning a new time.
       * The calculation wraps around midnight.
       * </p>
       * @param hours  the hours to add, may be negative
       * @return an @c OffsetTime based on this time with the hours added
       */
      OffsetTime plusHours(glong hours) const;

      /**
       * Returns a copy of this @c OffsetTime with the specified number of minutes added.
       * <p>
       * This adds the specified number of minutes to this time, returning a new time.
       * The calculation wraps around midnight.
       * </p>
       * @param minutes  the minutes to add, may be negative
       * @return an @c OffsetTime based on this time with the minutes added
       */
      OffsetTime plusMinutes(glong minutes) const;

      /**
       * Returns a copy of this @c OffsetTime with the specified number of seconds added.
       * <p>
       * This adds the specified number of seconds to this time, returning a new time.
       * The calculation wraps around midnight.
       * </p>
       * @param seconds  the seconds to add, may be negative
       * @return an @c OffsetTime based on this time with the seconds added
       */
      OffsetTime plusSeconds(glong seconds) const;

      /**
       * Returns a copy of this @c OffsetTime with the specified number of nanoseconds added.
       * <p>
       * This adds the specified number of nanoseconds to this time, returning a new time.
       * The calculation wraps around midnight.
       * </p>
       * @param nanos  the nanos to add, may be negative
       * @return an @c OffsetTime based on this time with the nanoseconds added
       */
      OffsetTime plusNanos(glong nanos) const;

      /**
       * Returns a copy of this time with the specified amount subtracted.
       * <p>
       * This returns an @c OffsetTime, based on this one, with the specified amount subtracted.
       * The amount is typically @b Duration but may be any other type implementing
       * the @b TemporalAmount interface.
       * </p>
       * <p>
       * The calculation is delegated to the amount object by calling
       * @b TemporalAmount::subtractFrom(Temporal). The amount implementation is free
       * to implement the subtraction in any way it wishes, however it typically
       * calls back to @b minus(long, TemporalUnit). Consult the documentation
       * of the amount implementation to determine if it can be successfully subtracted.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param amountToSubtract  the amount to subtract, not null
       * @return an @c OffsetTime based on this time with the subtraction made, not null
       * @throws DateTimeException if the subtraction cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetTime minus(TemporalAmount const& amountToSubtract) const override;

      /**
       * Returns a copy of this time with the specified amount subtracted.
       * <p>
       * This returns an @c OffsetTime, based on this one, with the amount
       * in terms of the unit subtracted. If it is not possible to subtract the amount,
       * because the unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * This method is equivalent to @b plus(long, TemporalUnit) with the amount negated.
       * See that method for a full description of how addition, and thus subtraction, works.
       * </p>
       * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
       * @param unit  the unit of the amount to subtract
       * @return an @c OffsetTime based on this time with the specified amount subtracted
       * @throws DateTimeException if the subtraction cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetTime minus(glong amountToSubtract, TemporalUnit unit) const override;

      /**
       * Returns a copy of this @c OffsetTime with the specified number of hours subtracted.
       * <p>
       * This subtracts the specified number of hours from this time, returning a new time.
       * The calculation wraps around midnight.
       * </p>
       * @param hours  the hours to subtract, may be negative
       * @return an @c OffsetTime based on this time with the hours subtracted
       */
      OffsetTime minusHours(glong hours) const;

      /**
       * Returns a copy of this @c OffsetTime with the specified number of minutes subtracted.
       * <p>
       * This subtracts the specified number of minutes from this time, returning a new time.
       * The calculation wraps around midnight.
       * </p>
       * @param minutes  the minutes to subtract, may be negative
       * @return an @c OffsetTime based on this time with the minutes subtracted
       */
      OffsetTime minusMinutes(glong minutes) const;

      /**
       * Returns a copy of this @c OffsetTime with the specified number of seconds subtracted.
       * <p>
       * This subtracts the specified number of seconds from this time, returning a new time.
       * The calculation wraps around midnight.
       * </p>
       * @param seconds  the seconds to subtract, may be negative
       * @return an @c OffsetTime based on this time with the seconds subtracted
       */
      OffsetTime minusSeconds(glong seconds) const;

      /**
       * Returns a copy of this @c OffsetTime with the specified number of nanoseconds subtracted.
       * <p>
       * This subtracts the specified number of nanoseconds from this time, returning a new time.
       * The calculation wraps around midnight.
       * </p>
       * @param nanos  the nanos to subtract, may be negative
       * @return an @c OffsetTime based on this time with the nanoseconds subtracted
       */
      OffsetTime minusNanos(gint nanos) const;

      /**
       * Queries this time using the specified query.
       * <p>
       * This queries this time using the specified query strategy object.
       * The @c TemporalQuery object defines the logic to be used to
       * obtain the result. Read the documentation of the query to understand
       * what the result of this method will be.
       * </p>
       * <p>
       * The result of this method is obtained by invoking the
       * @b TemporalQuery::queryFrom(Temporal) method on the
       * specified query passing @c this as the argument.
       * </p>
       * @param query  the query to invoke
       * @return the query result, null may be returned (defined by the query)
       * @throws DateTimeException if unable to query (defined by the query)
       * @throws ArithmeticException if numeric overflow occurs (defined by the query)
       */
      Optional query(TemporalQuery const& query) const override;

      /**
       * Adjusts the specified temporal object to have the same offset and time
       * as this object.
       * <p>
       * This returns a temporal object of the same observable type as the input
       * with the offset and time changed to be the same as this.
       * <p>
       * The adjustment is equivalent to using {@link Temporal#with(TemporalField, long)}
       * twice, passing {@link ChronoField#NANO_OF_DAY} and
       * {@link ChronoField#OFFSET_SECONDS} as the fields.
       * <p>
       * In most cases, it is clearer to reverse the calling pattern by using
       * {@link Temporal#with(TemporalAdjuster)}:
       * <pre>
       *   // these two lines are equivalent, but the second approach is recommended
       *   temporal = thisOffsetTime.adjustInto(temporal);
       *   temporal = temporal.with(thisOffsetTime);
       * </pre>
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
       * Calculates the amount of time until another time in terms of the specified unit.
       * <p>
       * This calculates the amount of time between two @c OffsetTime
       * objects in terms of a single @c TemporalUnit.
       * The start and end points are @c this and the specified time.
       * The result will be negative if the end is before the start.
       * For example, the amount in hours between two times can be calculated
       * using @c startTime.until(endTime, HOURS).
       * </p>
       * <p>
       * The @c Temporal passed to this method is converted to a
       * @c OffsetTime using @b from(Temporal).
       * If the offset differs between the two times, then the specified
       * end time is normalized to have the same offset as this time.
       * </p>
       * <p>
       * The calculation returns a whole number, representing the number of
       * complete units between the two times.
       * For example, the amount in hours between 11:30Z and 13:29Z will only
       * be one hour as it is one minute short of two hours.
       * </p>
       * <p>
       * The calculation is implemented in this method for @b TemporalUnit.
       * The units @c NANOS, @c MICROS, @c MILLIS, @c SECONDS,
       * @c MINUTES, @c HOURS and @c HALF_DAYS are supported.
       * Other @c TemporalUnit values will throw an exception.
       * </p>
       * @param endExclusive  the end time, exclusive, which is converted to an @c OffsetTime
       * @param unit  the unit to measure the amount in
       * @return the amount of time between this time and the end time
       * @throws DateTimeException if the amount cannot be calculated, or the end
       *  temporal cannot be converted to an @c OffsetTime
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong until(Temporal const& endExclusive, TemporalUnit unit) const override;

      /**
       * Formats this time using the specified formatter.
       * <p>
       * This time will be passed to the formatter to produce a string.
       *
       * @param formatter  the formatter to use, not null
       * @return the formatted time string, not null
       * @throws DateTimeException if an error occurs during printing
       */
      String format(DateTimeFormatter const& formatter) const;

      /**
       * Combines this time with a date to create an @c OffsetDateTime.
       * <p>
       * This returns an @c OffsetDateTime formed from this time and the specified date.
       * All possible combinations of date and time are valid.
       * </p>
       * @param date  the date to combine with
       * @return the offset date-time formed from this time and the specified date
       */
      OffsetDateTime atDate(LocalDate const& date) const;

      /**
       * Converts this @c OffsetTime to the number of seconds since the epoch
       * of 1970-01-01T00:00:00Z.
       * <p>
       * This combines this offset time with the specified date to calculate the
       * epoch-second value, which is the number of elapsed seconds from
       * 1970-01-01T00:00:00Z.
       * Instants on the time-line after the epoch are positive, earlier
       * are negative.
       * </p>
       * @param date the localdate
       * @return the number of seconds since the epoch of 1970-01-01T00:00:00Z, may be negative
       */
      glong toEpochSecond(LocalDate const& date) const;

      /**
       * Compares this @c OffsetTime to another time.
       * <p>
       * The comparison is based first on the UTC equivalent instant, then on the local time.
       * It is "consistent with equals", as defined by @b Comparable.
       * </p>
       * <p>
       * For example, the following is the comparator order:
       *
       * @li @c 10:30+01:00
       * @li @c 11:00+01:00
       * @li @c 12:00+02:00
       * @li @c 11:30+01:00
       * @li @c 12:00+01:00
       * @li @c 12:30+01:00
       *
       * Values #2 and #3 represent the same instant on the time-line.
       * When two values represent the same instant, the local time is compared
       * to distinguish them. This step is needed to make the ordering
       * consistent with @c equals().
       * </p>
       * <p>
       * To compare the underlying local time of two @c Temporal instances,
       * use @b TemporalField#NANO_OF_DAY as a comparator.
       * </p>
       * @param other  the other time to compare to
       * @return the comparator value, that is the comparison of the UTC equivalent @c other instant,
       *          if they are not equal, and if the UTC equivalent @c other instant is equal,
       *          the comparison of this local time with @c other local time
       * @see isBefore
       * @see isAfter
       */
      gint compareTo(OffsetTime const& other) const override;

      /**
       * Checks if the instant of this @c OffsetTime is after that of the
       * specified time applying both times to a common date.
       * <p>
       * This method differs from the comparison in @b compareTo in that it
       * only compares the instant of the time. This is equivalent to converting both
       * times to an instant using the same date and comparing the instants.
       * </p>
       * @param other  the other time to compare to
       * @return true if this is after the instant of the specified time
       */
      gbool isAfter(OffsetTime const& other) const;

      /**
       * Checks if the instant of this @c OffsetTime is before that of the
       * specified time applying both times to a common date.
       * <p>
       * This method differs from the comparison in @b compareTo in that it
       * only compares the instant of the time. This is equivalent to converting both
       * times to an instant using the same date and comparing the instants.
       * </p>
       * @param other  the other time to compare to
       * @return true if this is before the instant of the specified time
       */
      gbool isBefore(OffsetTime const& other) const;

      /**
       * Checks if this time is equal to another time.
       * <p>
       * The comparison is based on the local-time and the offset.
       * To compare for the same instant on the time-line, use @b isEqual(OffsetTime).
       * </p>
       * <p>
       * Only objects of type @c OffsetTime are compared, other types return false.
       * To compare the underlying local time of two @c Temporal instances,
       * use @b TemporalField#NANO_OF_DAY as a comparator.
       * </p>
       * @param other  the object to check, null returns false
       * @return true if this is equal to the other time
       */
      gbool equals(Object const& other) const override;

      /**
       * A hash code for this time.
       *
       * @return a suitable hash code
       */
      gint hash() const override;

      /**
       * Outputs this time as a @c String, such as @c 10:15:30+01:00.
       * <p>
       * The output will be one of the following ISO-8601 formats:
       *
       * @li @c HH:mmXXXXX
       * @li @c HH:mm:ssXXXXX
       * @li @c HH:mm:ss.SSSXXXXX
       * @li @c HH:mm:ss.SSSSSSXXXXX
       * @li @c HH:mm:ss.SSSSSSSSSXXXXX
       * </p>
       * The format used will be the shortest that outputs the full value of
       * the time when the omitted parts are implied to be zero.
       *
       * @return a string representation of this time
       */
      String toString() const override;

      /**
       * Return the shadow copy of this object.
       *
       * @return the shadow copy of this object
       */
      Object& clone() const override;

    private:
      static CORE_FAST gint SECONDS_PER_DAY = LocalTime::SECONDS_PER_DAY;
      static CORE_FAST gint NANOS_PER_MILLI = LocalTime::NANOS_PER_MILLI;
      static CORE_FAST glong NANOS_PER_SECOND = LocalTime::NANOS_PER_SECOND;
      static CORE_FAST glong NANOS_PER_MINUTE = LocalTime::NANOS_PER_MINUTE;
      static CORE_FAST glong NANOS_PER_HOUR = LocalTime::NANOS_PER_HOUR;

      /**
       * Returns a new time based on this one, returning {@code this} where possible.
       *
       * @param time  the time to create with, not null
       * @param offset  the zone offset to create with, not null
       */
      OffsetTime with(LocalTime const& time, ZoneOffset const& offset) const;

      /**
       * Converts this time to epoch nanos based on 1970-01-01Z.
       *
       * @return the epoch nanos value
       */
      glong toEpochNano() const;
    };
  } // time
} // core

#endif //CORE24_OFFSETTIME_H
