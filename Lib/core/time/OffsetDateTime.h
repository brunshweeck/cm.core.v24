//
// Created by admin on 07/01/25.
//

#ifndef CORE24_OFFSETDATETIME_H
#define CORE24_OFFSETDATETIME_H

#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>

namespace core {
  namespace time {
    /**
     * A date-time with an offset from UTC/Greenwich in the ISO-8601 calendar system,
     * such as @c 2007-12-03T10:15:30+01:00.
     * <p>
     * @c OffsetDateTime is an immutable representation of a date-time with an offset.
     * This class stores all date and time fields, to a precision of nanoseconds,
     * as well as the offset from UTC/Greenwich. For example, the value
     * "2nd October 2007 at 13:45:30.123456789 +02:00" can be stored in an @c OffsetDateTime.
     * </p>
     * <p>
     * @c OffsetDateTime and @b ZonedDateTime all store an instant
     * on the time-line to nanosecond precision.
     * @c OffsetDateTime adds to the instant the offset from UTC/Greenwich, which allows
     * the local date-time to be obtained.
     * @c ZonedDateTime adds full time-zone rules.
     * </p>
     */
    class OffsetDateTime final : public virtual TemporalBase<OffsetDateTime>,
                                 public virtual TemporalAdjuster,
                                 public virtual Comparable<OffsetDateTime> {
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, Optional);
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, OptionalDate);
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, OptionalTime);

    public:
      CORE_ALIAS(Comparator, util::Comparator<OffsetDateTime>);

      /**
       * The minimum supported @c OffsetDateTime, '-999999999-01-01T00:00:00+18:00'.
       * This is the local date-time of midnight at the start of the minimum date
       * in the maximum offset (larger offsets are earlier on the time-line).
       * This combines @b LocalDateTime::MIN and @b ZoneOffset::MAX.
       * This could be used by an application as a "far past" date-time.
       */
      static OffsetDateTime const MIN;

      /**
       * The maximum supported @c OffsetDateTime, '+999999999-12-31T23:59:59.999999999-18:00'.
       * This is the local date-time just before midnight at the end of the maximum date
       * in the minimum offset (larger negative offsets are later on the time-line).
       * This combines @b LocalDateTime::MAX and @b ZoneOffset::MIN.
       * This could be used by an application as a "far future" date-time.
       */
      static OffsetDateTime const MAX;

    private:
      /**
       * The local date-time.
       */
      LocalDateTime dateTime;

      /**
       * The offset from UTC/Greenwich.
       */
      ZoneOffset offset;

      /**
       * Constructor.
       *
       * @param dateTime  the local date-time, not null
       * @param offset  the zone offset, not null
       */
      OffsetDateTime(LocalDateTime const& dateTime, ZoneOffset const& offset);

    public:
      /**
       * Gets a comparator that compares two @c OffsetDateTime instances
       * based solely on the instant.
       * <p>
       * This method differs from the comparison in @b compareTo in that it
       * only compares the underlying instant.
       *
       * @return a comparator that compares in time-line order
       *
       * @see #isAfter
       * @see #isBefore
       * @see #isEqual
       */
      static Comparator& timeLineOrder();

      /**
       * Obtains the current date-time from the system clock in the default time-zone.
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       * </p>
       * @return the current date-time using the system clock
       */
      static OffsetDateTime now();

      /**
       * Obtains the current date-time from the system clock in the specified time-zone.
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       * </p>
       * @param zone  the zone ID to use
       * @return the current date-time using the system clock
       */
      static OffsetDateTime now(ZoneId const& zone);

      /**
       * Obtains an instance of @c OffsetDateTime from a date, time and offset.
       * <p>
       * This creates an offset date-time with the specified local date, time and offset.
       * </p>
       * @param date  the local date
       * @param time  the local time
       * @param offset  the zone offset
       * @return the offset date-time
       */
      static OffsetDateTime of(LocalDate const& date, LocalTime const& time, ZoneOffset const& offset);

      /**
       * Obtains an instance of @c OffsetDateTime from a date-time and offset.
       * <p>
       * This creates an offset date-time with the specified local date-time and offset.
       * </p>
       * @param dateTime  the local date-time
       * @param offset  the zone offset
       * @return the offset date-time
       */
      static OffsetDateTime of(LocalDateTime const& dateTime, ZoneOffset const& offset);

      /**
       * Obtains an instance of @c OffsetDateTime from a year, month, day,
       * hour, minute, second, nanosecond and offset.
       * <p>
       * This creates an offset date-time with the seven specified fields.
       * </p>
       * <p>
       * This method exists primarily for writing test cases.
       * Non test-code will typically use other methods to create an offset time.
       * @c LocalDateTime has five additional convenience variants of the
       * equivalent factory method taking fewer arguments.
       * They are not provided here to reduce the footprint of the API.
       * </p>
       * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
       * @param dayOfMonth  the day-of-month to represent, from 1 to 31
       * @param hour  the hour-of-day to represent, from 0 to 23
       * @param minute  the minute-of-hour to represent, from 0 to 59
       * @param second  the second-of-minute to represent, from 0 to 59
       * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
       * @param offset  the zone offset
       * @return the offset date-time
       * @throws DateTimeException if the value of any field is out of range, or
       *  if the day-of-month is invalid for the month-year
       */
      static OffsetDateTime of(gint year, gint month, gint dayOfMonth,
                               gint hour, gint minute, gint second, gint nanoOfSecond,
                               ZoneOffset const& offset);

      /**
       * Obtains an instance of @c OffsetDateTime from an @c Instant and zone ID.
       * <p>
       * This creates an offset date-time with the same instant as that specified.
       * Finding the offset from UTC/Greenwich is simple as there is only one valid
       * offset for each instant.
       *
       * @param instant  the instant to create the date-time from, not null
       * @param zone  the time-zone, which may be an offset, not null
       * @return the offset date-time, not null
       * @throws DateTimeException if the result exceeds the supported range
       */
      static OffsetDateTime ofInstant(Instant const& instant, ZoneId const& zone);

      /**
       * Obtains an instance of @c OffsetDateTime from a temporal object.
       * <p>
       * This obtains an offset date-time based on the specified temporal.
       * A @c Temporal represents an arbitrary set of date and time information,
       * which this factory converts to an instance of @c OffsetDateTime.
       * </p>
       * <p>
       * The conversion will first obtain a @c ZoneOffset from the temporal object.
       * It will then try to obtain a @c LocalDateTime, falling back to an @c Instant if necessary.
       * The result will be the combination of @c ZoneOffset with either
       * with @c LocalDateTime or @c Instant.
       * Implementations are permitted to perform optimizations such as accessing
       * those fields that are equivalent to the relevant objects.
       * </p>
       * <p>
       * This method matches the signature of the functional interface @b TemporalQuery
       * allowing it to be used as a query via method reference, @c OffsetDateTime::from.
       * </p>
       * @param temporal  the temporal object to convert
       * @return the offset date-time
       * @throws DateTimeException if unable to convert to an @c OffsetDateTime
       */
      static OffsetDateTime from(TemporalAccessor const& temporal);

      /**
       * Obtains an instance of @c OffsetDateTime from a text string
       * such as @c 2007-12-03T10:15:30+01:00.
       * <p>
       * The string must represent a valid date-time and is parsed using
       * @b DateTimeFormatter::ISO_OFFSET_DATE_TIME.
       * </p>
       * @param text  the text to parse such as "2007-12-03T10:15:30+01:00", not null
       * @return the parsed offset date-time, not null
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static OffsetDateTime parse(CharSequence const& text);

      /**
       * Obtains an instance of @c OffsetDateTime from a text string using a specific formatter.
       * <p>
       * The text is parsed using the formatter, returning a date-time.
       * </p>
       * @param text  the text to parse, not null
       * @param formatter  the formatter to use, not null
       * @return the parsed offset date-time, not null
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static OffsetDateTime parse(CharSequence const& text, DateTimeFormatter const& formatter);

      /**
       * Checks if the specified field is supported.
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The supported standard fields are: <br/>
       *
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
       * - @c DAY_OF_WEEK <br/>
       * - @c ALIGNED_DAY_OF_WEEK_IN_MONTH <br/>
       * - @c ALIGNED_DAY_OF_WEEK_IN_YEAR <br/>
       * - @c DAY_OF_MONTH <br/>
       * - @c DAY_OF_YEAR <br/>
       * - @c EPOCH_DAY <br/>
       * - @c ALIGNED_WEEK_OF_MONTH <br/>
       * - @c ALIGNED_WEEK_OF_YEAR <br/>
       * - @c MONTH_OF_YEAR <br/>
       * - @c PROLEPTIC_MONTH <br/>
       * - @c YEAR_OF_ERA <br/>
       * - @c YEAR <br/>
       * - @c ERA <br/>
       * - @c INSTANT_SECONDS <br/>
       * - @c OFFSET_SECONDS <br/>
       * </p>
       * All other @c TemporalField instances will return false.
       * @param field  the field to check
       * @return true if the field is supported on this date-time, false if not
       */
      gbool isSupported(TemporalField field) const override;

      /**
       * Checks if the specified unit is supported.
       * <p>
       * This checks if the specified unit can be added to, or subtracted from, this date-time.
       * If false, then calling the @b plus(long, TemporalUnit) and
       * @b minus(long, TemporalUnit) methods will throw an exception.
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
       * - @c WEEKS <br/>
       * - @c MONTHS <br/>
       * - @c YEARS <br/>
       * - @c DECADES <br/>
       * - @c CENTURIES <br/>
       * - @c MILLENNIA <br/>
       * - @c ERAS <br/>
       * </p>
       * All other @c TemporalUnit instances will return false.
       * @param unit  the unit to check
       * @return true if the unit can be added/subtracted, false if not
       */
      gbool isSupported(TemporalUnit unit) const override;

      /**
       * Gets the range of valid values for the specified field.
       * <p>
       * The range object expresses the minimum and maximum valid values for a field.
       * This date-time is used to enhance the accuracy of the returned range.
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
       * Gets the value of the specified field from this date-time as an @c int.
       * <p>
       * This queries this date-time for the value of the specified field.
       * The returned value will always be within the valid range of values for the field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The supported fields will return valid
       * values based on this date-time, except @c NANO_OF_DAY, @c MICRO_OF_DAY,
       * @c EPOCH_DAY, @c PROLEPTIC_MONTH and @c INSTANT_SECONDS which are too
       * large to fit in an @c int and throw an @c TemporalException.
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
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
       * Gets the value of the specified field from this date-time as a @c long.
       * <p>
       * This queries this date-time for the value of the specified field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The supported fields will return valid
       * values based on this date-time.
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
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
       * This is the offset of the local date-time from UTC/Greenwich.
       * </p>
       * @return the zone offset
       */
      ZoneOffset toOffset() const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified offset ensuring
       * that the result has the same local date-time.
       * <p>
       * This method returns an object with the same @c LocalDateTime and the specified @c ZoneOffset.
       * No calculation is needed or performed.
       * For example, if this time represents @c 2007-12-03T10:30+02:00 and the offset specified is
       * @c +03:00, then this method will return @c 2007-12-03T10:30+03:00.
       * </p>
       * <p>
       * To take into account the difference between the offsets, and adjust the time fields,
       * use @b withOffsetSameInstant.
       * </p>
       *
       * @param offset  the zone offset to change to
       * @return an @c OffsetDateTime based on this date-time with the requested offset
       */
      OffsetDateTime withOffsetSameLocal(ZoneOffset const& offset) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified offset ensuring
       * that the result is at the same instant.
       * <p>
       * This method returns an object with the specified @c ZoneOffset and a @c LocalDateTime
       * adjusted by the difference between the two offsets.
       * This will result in the old and new objects representing the same instant.
       * This is useful for finding the local time in a different offset.
       * For example, if this time represents @c 2007-12-03T10:30+02:00 and the offset specified is
       * @c +03:00, then this method will return @c 2007-12-03T11:30+03:00.
       * </p>
       * <p>
       * To change the offset without adjusting the local time use @b withOffsetSameLocal.
       * </p>
       * @param offset  the zone offset to change to
       * @return an @c OffsetDateTime based on this date-time with the requested offset
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime withOffsetSameInstant(ZoneOffset const& offset) const;

      /**
       * Gets the @c LocalDateTime part of this date-time.
       * <p>
       * This returns a @c LocalDateTime with the same year, month, day and time
       * as this date-time.
       * </p>
       * @return the local date-time part of this date-time
       */
      LocalDateTime toLocalDateTime() const;

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
       *
       * @return the month-of-year
       */
      Month month() const;

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
       * Additional information can be obtained from the @c DayOfWeek.
       * This includes textual names of the values.
       * </p>
       * @return the day-of-week
       */
      DayOfWeek dayOfWeek() const;

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
       * Returns an adjusted copy of this date-time.
       * <p>
       * This returns an {@code OffsetDateTime}, based on this one, with the date-time adjusted.
       * The adjustment takes place using the specified adjuster strategy object.
       * Read the documentation of the adjuster to understand what adjustment will be made.
       * <p>
       * A simple adjuster might simply set the one of the fields, such as the year field.
       * A more complex adjuster might set the date to the last day of the month.
       * A selection of common adjustments is provided in
       * {@link java.time.temporal.TemporalAdjusters TemporalAdjusters}.
       * These include finding the "last day of the month" and "next Wednesday".
       * Key date-time classes also implement the {@code TemporalAdjuster} interface,
       * such as {@link java.time.MonthDay MonthDay}.
       * The adjuster is responsible for handling special cases, such as the varying
       * lengths of month and leap years.
       * <p>
       * For example this code returns a date on the last day of July:
       * <pre>
       *  import static java.time.Month.*;
       *  import static java.time.temporal.TemporalAdjusters.*;
       *
       *  result = offsetDateTime.with(JULY).with(lastDayOfMonth());
       * </pre>
       * <p>
       * The classes {@link LocalDate}, {@link LocalTime} and {@link ZoneOffset} implement
       * {@code TemporalAdjuster}, thus this method can be used to change the date, time or offset:
       * <pre>
       *  result = offsetDateTime.with(date);
       *  result = offsetDateTime.with(time);
       *  result = offsetDateTime.with(offset);
       * </pre>
       * <p>
       * The result of this method is obtained by invoking the
       * {@link TemporalAdjuster#adjustInto(Temporal)} method on the
       * specified adjuster passing {@code this} as the argument.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param adjuster the adjuster to use, not null
       * @return an {@code OffsetDateTime} based on {@code this} with the adjustment made, not null
       * @throws DateTimeException if the adjustment cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetDateTime with(TemporalAdjuster const& adjuster) const override;

      /**
       * Returns a copy of this date-time with the specified field set to a new value.
       * <p>
       * This returns an @c OffsetDateTime, based on this one, with the value
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
       * If the field is a @b TemporalField then the adjustment is implemented here.
       * </p>
       * <p>
       * The @c INSTANT_SECONDS field will return a date-time with the specified instant.
       * The offset and nano-of-second are unchanged.
       * If the new instant value is outside the valid range then a @c DateTimeException will be thrown.
       * </p>
       * <p>
       * The @c OFFSET_SECONDS field will return a date-time with the specified offset.
       * The local date-time is unaltered. If the new offset value is outside the valid range
       * then a @c DateTimeException will be thrown.
       * </p>
       * <p>
       * The other supported fields will behave as per
       * the matching method on @b LocalDateTime.
       * In this case, the offset is not part of the calculation and will be unchanged.
       * </p>
       * <p>
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       * @param field  the field to set in the result
       * @param newValue  the new value of the field in the result
       * @return an @c OffsetDateTime based on @c this with the specified field set
       * @throws DateTimeException if the field cannot be set
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetDateTime with(TemporalField field, glong newValue) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the year altered.
       * <p>
       * The time and offset do not affect the calculation and will be the same in the result.
       * If the day-of-month is invalid for the year, it will be changed to the last valid day of the month.
       * </p>
       * @param year  the year to set in the result, from MIN_YEAR to MAX_YEAR
       * @return an @c OffsetDateTime based on this date-time with the requested year
       * @throws DateTimeException if the year value is invalid
       */
      OffsetDateTime withYear(gint year) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the month-of-year altered.
       * <p>
       * The time and offset do not affect the calculation and will be the same in the result.
       * If the day-of-month is invalid for the year, it will be changed to the last valid day of the month.
       * </p>
       * @param month  the month-of-year to set in the result, from 1 (January) to 12 (December)
       * @return an @c OffsetDateTime based on this date-time with the requested month
       * @throws DateTimeException if the month-of-year value is invalid
       */
      OffsetDateTime withMonth(gint month) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the day-of-month altered.
       * <p>
       * If the resulting @c OffsetDateTime is invalid, an exception is thrown.
       * The time and offset do not affect the calculation and will be the same in the result.
       * </p>
       * @param dayOfMonth  the day-of-month to set in the result, from 1 to 28-31
       * @return an @c OffsetDateTime based on this date-time with the requested day
       * @throws DateTimeException if the day-of-month value is invalid,
       *  or if the day-of-month is invalid for the month-year
       */
      OffsetDateTime withDayOfMonth(gint dayOfMonth) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the day-of-year altered.
       * <p>
       * The time and offset do not affect the calculation and will be the same in the result.
       * If the resulting @c OffsetDateTime is invalid, an exception is thrown.
       * </p>
       * @param dayOfYear  the day-of-year to set in the result, from 1 to 365-366
       * @return an @c OffsetDateTime based on this date with the requested day
       * @throws DateTimeException if the day-of-year value is invalid,
       *  or if the day-of-year is invalid for the year
       */
      OffsetDateTime withDayOfYear(gint dayOfYear) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the hour-of-day altered.
       * <p>
       * The date and offset do not affect the calculation and will be the same in the result.
       * </p>
       * @param hour  the hour-of-day to set in the result, from 0 to 23
       * @return an @c OffsetDateTime based on this date-time with the requested hour
       * @throws DateTimeException if the hour value is invalid
       */
      OffsetDateTime withHour(gint hour) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the minute-of-hour altered.
       * <p>
       * The date and offset do not affect the calculation and will be the same in the result.
       * </p>
       * @param minute  the minute-of-hour to set in the result, from 0 to 59
       * @return an @c OffsetDateTime based on this date-time with the requested minute
       * @throws DateTimeException if the minute value is invalid
       */
      OffsetDateTime withMinute(gint minute) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the second-of-minute altered.
       * <p>
       * The date and offset do not affect the calculation and will be the same in the result.
       * </p>
       * @param second  the second-of-minute to set in the result, from 0 to 59
       * @return an @c OffsetDateTime based on this date-time with the requested second
       * @throws DateTimeException if the second value is invalid
       */
      OffsetDateTime withSecond(gint second) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the nano-of-second altered.
       * <p>
       * The date and offset do not affect the calculation and will be the same in the result.
       * </p>
       * @param nanoOfSecond  the nano-of-second to set in the result, from 0 to 999,999,999
       * @return an @c OffsetDateTime based on this date-time with the requested nanosecond
       * @throws DateTimeException if the nano value is invalid
       */
      OffsetDateTime withNano(gint nanoOfSecond) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the time truncated.
       * <p>
       * Truncation returns a copy of the original date-time with fields
       * smaller than the specified unit set to zero.
       * For example, truncating with the @em minutes unit
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
       * @return an @c OffsetDateTime based on this date-time with the time truncated
       * @throws DateTimeException if unable to truncate
       * @throws TemporalException if the unit is not supported
       */
      OffsetDateTime truncatedTo(TemporalUnit unit) const;

      /**
       * Returns a copy of this date-time with the specified amount added.
       * <p>
       * This returns an @c OffsetDateTime, based on this one, with the specified amount added.
       * The amount is typically @b Period or @b Duration but may be
       * any other type implementing the @b TemporalAmount interface.
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
       * @param amountToAdd  the amount to add, not null
       * @return an @c OffsetDateTime based on this date-time with the addition made, not null
       * @throws DateTimeException if the addition cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetDateTime plus(TemporalAmount const& amountToAdd) const override;

      /**
       * Returns a copy of this date-time with the specified amount added.
       * <p>
       * This returns an @c OffsetDateTime, based on this one, with the amount
       * in terms of the unit added. If it is not possible to add the amount, because the
       * unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalUnit then the addition is implemented by
       * @b LocalDateTime::plus(long, TemporalUnit).
       * The offset is not part of the calculation and will be unchanged in the result.
       * </p>
       * @param amountToAdd  the amount of the unit to add to the result, may be negative
       * @param unit  the unit of the amount to add
       * @return an @c OffsetDateTime based on this date-time with the specified amount added
       * @throws DateTimeException if the addition cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetDateTime plus(glong amountToAdd, TemporalUnit unit) const override;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of years added.
       * <p>
       * This method adds the specified amount to the years field in three steps: <br/>
       *
       * - Add the input years to the year field <br/>
       * - Check if the resulting date would be invalid <br/>
       * - Adjust the day-of-month to the last valid day if necessary <br/>
       * </p>
       * <p>
       * For example, 2008-02-29 (leap year) plus one year would result in the
       * invalid date 2009-02-29 (standard year). Instead of returning an invalid
       * result, the last valid day of the month, 2009-02-28, is selected instead.
       * </p>
       * @param years  the years to add, may be negative
       * @return an @c OffsetDateTime based on this date-time with the years added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime plusYears(glong years) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of months added.
       * <p>
       * This method adds the specified amount to the months field in three steps: <br/>
       *
       * - Add the input months to the month-of-year field <br/>
       * - Check if the resulting date would be invalid <br/>
       * - Adjust the day-of-month to the last valid day if necessary <br/>
       * </p>
       * <p>
       * For example, 2007-03-31 plus one month would result in the invalid date
       * 2007-04-31. Instead of returning an invalid result, the last valid day
       * of the month, 2007-04-30, is selected instead.
       * </p>
       * @param months  the months to add, may be negative
       * @return an @c OffsetDateTime based on this date-time with the months added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime plusMonths(glong months) const;

      /**
       * Returns a copy of this OffsetDateTime with the specified number of weeks added.
       * <p>
       * This method adds the specified amount in weeks to the days field incrementing
       * the month and year fields as necessary to ensure the result remains valid.
       * The result is only invalid if the maximum/minimum year is exceeded.
       * </p>
       * <p>
       * For example, 2008-12-31 plus one week would result in 2009-01-07.
       * </p>
       * @param weeks  the weeks to add, may be negative
       * @return an @c OffsetDateTime based on this date-time with the weeks added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime plusWeeks(glong weeks) const;

      /**
       * Returns a copy of this OffsetDateTime with the specified number of days added.
       * <p>
       * This method adds the specified amount to the days field incrementing the
       * month and year fields as necessary to ensure the result remains valid.
       * The result is only invalid if the maximum/minimum year is exceeded.
       * </p>
       * <p>
       * For example, 2008-12-31 plus one day would result in 2009-01-01.
       * </p>
       * @param days  the days to add, may be negative
       * @return an @c OffsetDateTime based on this date-time with the days added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime plusDays(glong days) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of hours added.
       *
       * @param hours  the hours to add, may be negative
       * @return an @c OffsetDateTime based on this date-time with the hours added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime plusHours(glong hours) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of minutes added.
       *
       * @param minutes  the minutes to add, may be negative
       * @return an @c OffsetDateTime based on this date-time with the minutes added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime plusMinutes(glong minutes) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of seconds added.
       *
       * @param seconds  the seconds to add, may be negative
       * @return an @c OffsetDateTime based on this date-time with the seconds added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime plusSeconds(glong seconds) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of nanoseconds added.
       *
       * @param nanos  the nanos to add, may be negative
       * @return an @c OffsetDateTime based on this date-time with the nanoseconds added
       * @throws DateTimeException if the unit cannot be added to this type
       */
      OffsetDateTime plusNanos(glong nanos) const;

      /**
       * Returns a copy of this date-time with the specified amount subtracted.
       * <p>
       * This returns an @c OffsetDateTime, based on this one, with the specified amount subtracted.
       * The amount is typically @b Period or @b Duration but may be
       * any other type implementing the @b TemporalAmount interface.
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
       * </p>
       *
       * @param amountToSubtract  the amount to subtract, not null
       * @return an @c OffsetDateTime based on this date-time with the subtraction made, not null
       * @throws DateTimeException if the subtraction cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetDateTime minus(TemporalAmount const& amountToSubtract) const override;

      /**
       * Returns a copy of this date-time with the specified amount subtracted.
       * <p>
       * This returns an @c OffsetDateTime, based on this one, with the amount
       * in terms of the unit subtracted. If it is not possible to subtract the amount,
       * because the unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * This method is equivalent to @b plus(long, TemporalUnit) with the amount negated.
       * See that method for a full description of how addition, and thus subtraction, works.
       * </p>
       * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
       * @param unit  the unit of the amount to subtract
       * @return an @c OffsetDateTime based on this date-time with the specified amount subtracted
       * @throws DateTimeException if the subtraction cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      OffsetDateTime minus(glong amountToSubtract, TemporalUnit unit) const override;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of years subtracted.
       * <p>
       * This method subtracts the specified amount from the years field in three steps: <br/>
       *
       * - Subtract the input years from the year field <br/>
       * - Check if the resulting date would be invalid <br/>
       * - Adjust the day-of-month to the last valid day if necessary <br/>
       * </p>
       * <p>
       * For example, 2008-02-29 (leap year) minus one year would result in the
       * invalid date 2007-02-29 (standard year). Instead of returning an invalid
       * result, the last valid day of the month, 2007-02-28, is selected instead.
       * </p>
       * @param years  the years to subtract, may be negative
       * @return an @c OffsetDateTime based on this date-time with the years subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime minusYears(glong years) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of months subtracted.
       * <p>
       * This method subtracts the specified amount from the months field in three steps: <br/>
       *
       * - Subtract the input months from the month-of-year field <br/>
       * - Check if the resulting date would be invalid <br/>
       * - Adjust the day-of-month to the last valid day if necessary <br/>
       * </p>
       * <p>
       * For example, 2007-03-31 minus one month would result in the invalid date
       * 2007-02-31. Instead of returning an invalid result, the last valid day
       * of the month, 2007-02-28, is selected instead.
       * </p>
       * @param months  the months to subtract, may be negative
       * @return an @c OffsetDateTime based on this date-time with the months subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime minusMonths(glong months) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of weeks subtracted.
       * <p>
       * This method subtracts the specified amount in weeks from the days field decrementing
       * the month and year fields as necessary to ensure the result remains valid.
       * The result is only invalid if the maximum/minimum year is exceeded.
       * </p>
       * <p>
       * For example, 2009-01-07 minus one week would result in 2008-12-31.
       * </p>
       * @param weeks  the weeks to subtract, may be negative
       * @return an @c OffsetDateTime based on this date-time with the weeks subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime minusWeeks(glong weeks) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of days subtracted.
       * <p>
       * This method subtracts the specified amount from the days field decrementing the
       * month and year fields as necessary to ensure the result remains valid.
       * The result is only invalid if the maximum/minimum year is exceeded.
       * </p>
       * <p>
       * For example, 2009-01-01 minus one day would result in 2008-12-31.
       * </p>
       * @param days  the days to subtract, may be negative
       * @return an @c OffsetDateTime based on this date-time with the days subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime minusDays(glong days) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of hours subtracted.
       *
       * @param hours  the hours to subtract, may be negative
       * @return an @c OffsetDateTime based on this date-time with the hours subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime minusHours(glong hours) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of minutes subtracted.
       *
       * @param minutes  the minutes to subtract, may be negative
       * @return an @c OffsetDateTime based on this date-time with the minutes subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime minusMinutes(glong minutes) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of seconds subtracted.
       *
       * @param seconds  the seconds to subtract, may be negative
       * @return an @c OffsetDateTime based on this date-time with the seconds subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime minusSeconds(glong seconds) const;

      /**
       * Returns a copy of this @c OffsetDateTime with the specified number of nanoseconds subtracted.
       *
       * @param nanos  the nanos to subtract, may be negative
       * @return an @c OffsetDateTime based on this date-time with the nanoseconds subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      OffsetDateTime minusNanos(glong nanos) const;

      /**
       * Queries this date-time using the specified query.
       * <p>
       * This queries this date-time using the specified query strategy object.
       * The @c TemporalQuery object defines the logic to be used to
       * obtain the result. Read the documentation of the query to understand
       * what the result of this method will be.
       * </p>
       *
       * @param query  the query to invoke
       * @return the query result, null may be returned (defined by the query)
       * @throws DateTimeException if unable to query (defined by the query)
       * @throws ArithmeticException if numeric overflow occurs (defined by the query)
       */
      Optional query(TemporalQuery const& query) const override;

      /**
       * Adjusts the specified temporal object to have the same offset, date
       * and time as this object.
       * <p>
       * This returns a temporal object of the same observable type as the input
       * with the offset, date and time changed to be the same as this.
       * <p>
       * The adjustment is equivalent to using {@link Temporal#with(TemporalField, long)}
       * three times, passing {@link ChronoField#EPOCH_DAY},
       * {@link ChronoField#NANO_OF_DAY} and {@link ChronoField#OFFSET_SECONDS} as the fields.
       * <p>
       * In most cases, it is clearer to reverse the calling pattern by using
       * {@link Temporal#with(TemporalAdjuster)}:
       * <pre>
       *   // these two lines are equivalent, but the second approach is recommended
       *   temporal = thisOffsetDateTime.adjustInto(temporal);
       *   temporal = temporal.with(thisOffsetDateTime);
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
       * Calculates the amount of time until another date-time in terms of the specified unit.
       * <p>
       * This calculates the amount of time between two @c OffsetDateTime
       * objects in terms of a single @c TemporalUnit.
       * The start and end points are @c this and the specified date-time.
       * The result will be negative if the end is before the start.
       * For example, the amount in days between two date-times can be calculated
       * using @c startDateTime.until(endDateTime, DAYS).
       * </p>
       * <p>
       * The @c Temporal passed to this method is converted to a
       * @c OffsetDateTime using @b from(Temporal).
       * If the offset differs between the two date-times, the specified
       * end date-time is normalized to have the same offset as this date-time.
       * </p>
       * <p>
       * The calculation returns a whole number, representing the number of
       * complete units between the two date-times.
       * For example, the amount in months between 2012-06-15T00:00Z and 2012-08-14T23:59Z
       * will only be one month as it is one minute short of two months.
       * </p>
       * <p>
       * The calculation is implemented in this method for @b TemporalUnit.
       * The units @c NANOS, @c MICROS, @c MILLIS, @c SECONDS,
       * @c MINUTES, @c HOURS and @c HALF_DAYS, @c DAYS,
       * @c WEEKS, @c MONTHS, @c YEARS, @c DECADES,
       * @c CENTURIES, @c MILLENNIA and @c ERAS are supported.
       * Other @c TemporalUnit values will throw an exception.
       * </p>
       * @param endExclusive  the end date, exclusive, which is converted to an @c OffsetDateTime
       * @param unit  the unit to measure the amount in
       * @return the amount of time between this date-time and the end date-time
       * @throws DateTimeException if the amount cannot be calculated, or the end
       *  temporal cannot be converted to an @c OffsetDateTime
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong until(Temporal const& endExclusive, TemporalUnit unit) const override;

      /**
       * Formats this date-time using the specified formatter.
       * <p>
       * This date-time will be passed to the formatter to produce a string.
       *
       * @param formatter  the formatter to use, not null
       * @return the formatted date-time string, not null
       * @throws DateTimeException if an error occurs during printing
       */
      String format(DateTimeFormatter const& formatter) const;

      /**
       * Combines this date-time with a time-zone to create a @c ZonedDateTime
       * ensuring that the result has the same instant.
       * <p>
       * This returns a @c ZonedDateTime formed from this date-time and the specified time-zone.
       * This conversion will ignore the visible local date-time and use the underlying instant instead.
       * This avoids any problems with local time-line gaps or overlaps.
       * The result might have different values for fields such as hour, minute an even day.
       * </p>
       * <p>
       * To attempt to retain the values of the fields, use @b atZoneSimilarLocal(ZoneId).
       * To use the offset as the zone ID, use @b toZonedDateTime().
       * </p>
       * @param zone  the time-zone to use
       * @return the zoned date-time formed from this date-time
       */
      ZonedDateTime atZoneSameInstant(ZoneId const& zone) const;

      /**
       * Combines this date-time with a time-zone to create a @c ZonedDateTime
       * trying to keep the same local date and time.
       * <p>
       * This returns a @c ZonedDateTime formed from this date-time and the specified time-zone.
       * Where possible, the result will have the same local date-time as this object.
       * </p>
       * <p>
       * Time-zone rules, such as daylight savings, mean that not every time on the
       * local time-line exists. If the local date-time is in a gap or overlap according to
       * the rules then a resolver is used to determine the resultant local time and offset.
       * This method uses @b ZonedDateTime::ofLocal(LocalDateTime, ZoneId, ZoneOffset)
       * to retain the offset from this instance if possible.
       * </p>
       * <p>
       * Finer control over gaps and overlaps is available in two ways.
       * If you simply want to use the later offset at overlaps then call
       * @b ZonedDateTime::withLaterOffsetAtOverlap() immediately after this method.
       * </p>
       * <p>
       * To create a zoned date-time at the same instant irrespective of the local time-line,
       * use @b atZoneSameInstant(ZoneId).
       * To use the offset as the zone ID, use @b toZonedDateTime().
       * </p>
       * @param zone  the time-zone to use
       * @return the zoned date-time formed from this date and the earliest valid time for the zone
       */
      ZonedDateTime atZoneSimilarLocal(ZoneId const& zone) const;

      /**
       * Converts this date-time to an @c OffsetTime.
       * <p>
       * This returns an offset time with the same local time and offset.
       * </p>
       * @return an OffsetTime representing the time and offset
       */
      OffsetTime toOffsetTime() const;

      /**
       * Converts this date-time to a @c ZonedDateTime using the offset as the zone ID.
       * <p>
       * This creates the simplest possible @c ZonedDateTime using the offset
       * as the zone ID.
       * </p>
       * <p>
       * To control the time-zone used, see @b atZoneSameInstant(ZoneId) and
       * @b atZoneSimilarLocal(ZoneId).
       * </p>
       * @return a zoned date-time representing the same local date-time and offset
       */
      ZonedDateTime toZonedDateTime() const;

      /**
       * Converts this date-time to an @c Instant.
       * <p>
       * This returns an @c Instant representing the same point on the
       * time-line as this date-time.
       *
       * @return an @c Instant representing the same instant, not null
       */
      Instant toInstant() const;

      /**
       * Converts this date-time to the number of seconds from the epoch of 1970-01-01T00:00:00Z.
       * <p>
       * This allows this date-time to be converted to a value of the
       * @b epoch-seconds field. This is primarily
       * intended for low-level conversions rather than general application usage.
       * </p>
       * @return the number of seconds from the epoch of 1970-01-01T00:00:00Z
       */
      glong toEpochSecond() const;

      /**
       * Compares this date-time to another date-time.
       * <p>
       * The comparison is based on the instant then on the local date-time.
       * It is "consistent with equals", as defined by @b Comparable.
       * </p>
       * <p>
       * For example, the following is the comparator order: <br/>
       *
       * - @c 2008-12-03T10:30+01:00 <br/>
       * - @c 2008-12-03T11:00+01:00 <br/>
       * - @c 2008-12-03T12:00+02:00 <br/>
       * - @c 2008-12-03T11:30+01:00 <br/>
       * - @c 2008-12-03T12:00+01:00 <br/>
       * - @c 2008-12-03T12:30+01:00 <br/>
       * </p>
       * Values #2 and #3 represent the same instant on the time-line.
       * When two values represent the same instant, the local date-time is compared
       * to distinguish them. This step is needed to make the ordering
       * consistent with @c equals().
       *
       * @param other  the other date-time to compare to
       * @return the comparator value, that is the comparison with the @c other's instant, if they are not equal;
       *          and if equal to the @c other's instant, the comparison of the @c other's local date-time
       * @see isBefore
       * @see isAfter
       */
      gint compareTo(OffsetDateTime const& other) const override;

      /**
       * Checks if the instant of this date-time is after that of the specified date-time.
       * <p>
       * This method differs from the comparison in @b compareTo and @b equals in that it
       * only compares the instant of the date-time.
       * </p>
       * @param other  the other date-time to compare to
       * @return true if this is after the instant of the specified date-time
       */
      gbool isAfter(OffsetDateTime const& other) const;

      /**
       * Checks if the instant of this date-time is before that of the specified date-time.
       * <p>
       * This method differs from the comparison in @b compareTo in that it
       * only compares the instant of the date-time.
       * </p>
       * @param other  the other date-time to compare to
       * @return true if this is before the instant of the specified date-time
       */
      gbool isBefore(OffsetDateTime const& other) const;

      /**
       * Checks if this date-time is equal to another date-time.
       * <p>
       * The comparison is based on the local date-time and the offset.
       * </p>
       * @param other  the object to check
       * @return true if this is equal to the other date-time
       */
      gbool equals(Object const& other) const override;

      /**
       * A hash code for this date-time.
       *
       * @return a suitable hash code
       */
      gint hash() const override;

      /**
       * Outputs this date-time as a @c String, such as @c 2007-12-03T10:15:30+01:00.
       * <p>
       * The output will be one of the following formats: <br/>
       *
       * - @c uuuu-MM-dd'T'HH:mmXXXXX <br/>
       * - @c uuuu-MM-dd'T'HH:mm:ssXXXXX <br/>
       * - @c uuuu-MM-dd'T'HH:mm:ss.SSSXXXXX <br/>
       * - @c uuuu-MM-dd'T'HH:mm:ss.SSSSSSXXXXX <br/>
       * - @c uuuu-MM-dd'T'HH:mm:ss.SSSSSSSSSXXXXX <br/>
       * </p>
       * The format used will be the shortest that outputs the full value of
       * the time when the omitted parts are implied to be zero. The output
       * is compatible with ISO 8601 if the seconds in the offset are zero.
       *
       * @return a string representation of this date-time
       */
      String toString() const override;

      /**
       * Return the shadow copy of this object.
       *
       * @return the shadow copy of this object
       */
      Object& clone() const override;

    private:
      /**
       * Compares this @c OffsetDateTime to another date-time.
       * The comparison is based on the instant.
       *
       * @param datetime1  the first date-time to compare, not null
       * @param datetime2  the other date-time to compare to, not null
       * @return the comparator value, that is less than zero if @c datetime1 is before @c datetime2,
       *          zero if they are equal, greater than zero if @c datetime1 is after @c datetime2
       */
      static gint compareInstant(OffsetDateTime const& datetime1, OffsetDateTime const& datetime2);

      /**
       * Returns a new date-time based on this one, returning @c this where possible.
       *
       * @param dateTime  the date-time to create with, not null
       * @param offset  the zone offset to create with, not null
       */
      OffsetDateTime with(LocalDateTime const& dateTime, ZoneOffset const& offset) const;
    };
  } // time
} // core

#endif //CORE24_OFFSETDATETIME_H
