//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_LOCALDATE_H
#define CORE24_LOCALDATE_H

#include <core/time/LocalTime.h>
#include <core/time/TemporalAdjuster.h>

namespace core {
  namespace time {
    /**
     * A date without a time-zone in the ISO-8601 calendar system,
     * such as @c 2007-12-03.
     * <p>
     * @c LocalDate is an immutable date-time object that represents a date,
     * often viewed as year-month-day. Other date fields, such as day-of-year,
     * day-of-week and week-of-year, can also be accessed.
     * For example, the value "2nd October 2007" can be stored in a @c LocalDate.
     * </p>
     * <p>
     * This class does not store or represent a time or time-zone.
     * Instead, it is a description of the date, as used for birthdays.
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
     * This is a @a value-based class; programmers should treat instances that are
     * @em equals as interchangeable and should not
     * use instances for synchronization, or unpredictable behavior may
     * occur. For example, in a future release, synchronization may fail.
     * The @c equals method should be used for comparisons.
     * </p>
     * @note
     * This class is immutable and thread-safe.
     */
    class LocalDate final : public virtual TemporalBase<LocalDate>,
                            public virtual TemporalAdjuster,
                            public virtual Comparable<LocalDate> {
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, Optional);
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, OptionalDate);

    public:
      /**
       * The minimum supported @c LocalDate, '-999999999-01-01'.
       * This could be used by an application as a "far past" date.
       */
      static LocalDate const MIN;

      /**
       * The maximum supported @c LocalDate, '+999999999-12-31'.
       * This could be used by an application as a "far future" date.
       */
      static LocalDate const MAX;

      /**
       * The epoch year @c LocalDate, '1970-01-01'.
       *
       * @since 9
       */
      static LocalDate const EPOCH;

      /**
       * The number of days in a 400 years cycle.
       */
      static CORE_FAST gint DAYS_PER_CYCLE = 146097;

      /**
       * The number of days from year zero to year 1970.
       * There are five 400 year cycles from year zero to 2000.
       * There are 7 leap years from 1970 to 2000.
       */
      static CORE_FAST glong DAYS_0000_TO_1970 = (DAYS_PER_CYCLE * 5L) - (30L * 365L + 7L);

      /**
       * The offset from Julian to EPOCH DAY.
       */
      static CORE_FAST glong JULIAN_DAY_OFFSET = 2440588L;

    private:
      /**
       * The year.
       */
      gint year_;

      /**
       * The month-of-year.
       */
      gshort month_;

      /**
       * The day-of-month.
       */
      gshort day_;

      /**
       * Constructor, previously validated.
       *
       * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent
       * @param dayOfMonth  the day-of-month to represent, valid for year-month, from 1 to 31
       */
      LocalDate(gint year, gint month, gint dayOfMonth);

    public:
      /**
       * Obtains the current date from the system clock in the default time-zone.
       * <p>
       * This will query the <b> system clock</b> in the default
       * time-zone to obtain the current date.
       * </p>
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       * </p>
       * @return the current date using the system clock and default time-zone
       */
      static LocalDate now();

      /**
       * Obtains the current date from the system clock in the specified time-zone.
       * <p>
       * This will query the <b> system clock</b> to obtain the current date.
       * Specifying the time-zone avoids dependence on the default time-zone.
       * </p>
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       * </p>
       * @param zone  the zone ID to use
       * @return the current date using the system clock
       */
      static LocalDate now(ZoneId const& zone);


      /**
       * Obtains an instance of @c LocalDate from a year, month and day.
       * <p>
       * This returns a @c LocalDate with the specified year, month and day-of-month.
       * The day must be valid for the year and month, otherwise an exception will be thrown.
       * </p>
       * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent
       * @param dayOfMonth  the day-of-month to represent, from 1 to 31
       * @return the local date
       * @throws DateTimeException if the value of any field is out of range,
       *  or if the day-of-month is invalid for the month-year
       */
      static LocalDate of(gint year, Month month, gint dayOfMonth);

      /**
       * Obtains an instance of @c LocalDate from a year, month and day.
       * <p>
       * This returns a @c LocalDate with the specified year, month and day-of-month.
       * The day must be valid for the year and month, otherwise an exception will be thrown.
       * </p>
       * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
       * @param dayOfMonth  the day-of-month to represent, from 1 to 31
       * @return the local date
       * @throws DateTimeException if the value of any field is out of range,
       *  or if the day-of-month is invalid for the month-year
       */
      static LocalDate of(gint year, gint month, gint dayOfMonth);


      /**
       * Obtains an instance of @c LocalDate from a year and day-of-year.
       * <p>
       * This returns a @c LocalDate with the specified year and day-of-year.
       * The day-of-year must be valid for the year, otherwise an exception will be thrown.
       * </p>
       * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
       * @param dayOfYear  the day-of-year to represent, from 1 to 366
       * @return the local date
       * @throws DateTimeException if the value of any field is out of range,
       *  or if the day-of-year is invalid for the year
       */
      static LocalDate ofYearDay(gint year, gint dayOfYear);

      /**
       * Obtains an instance of @c LocalDate from an @c Instant and zone ID.
       * <p>
       * This creates a local date based on the specified instant.
       * First, the offset from UTC/Greenwich is obtained using the zone ID and instant,
       * which is simple as there is only one valid offset for each instant.
       * Then, the instant and offset are used to calculate the local date.
       * </p>
       * @param instant  the instant to create the date from
       * @param zone  the time-zone, which may be an offset
       * @return the local date
       * @throws DateTimeException if the result exceeds the supported range
       */
      static LocalDate ofInstant(Instant const& instant, ZoneId const& zone);


      /**
       * Obtains an instance of @c LocalDate from the epoch day count.
       * <p>
       * This returns a @c LocalDate with the specified epoch-day.
       * The @b EPOCH_DAY is a simple incrementing count
       * of days when day 0 is 1970-01-01. Negative numbers represent earlier days.
       * </p>
       * @param epochDay  the Epoch Day to convert, based on the epoch 1970-01-01
       * @return the local date
       * @throws DateTimeException if the epoch day exceeds the supported date range
       */
      static LocalDate ofEpochDay(glong epochDay);

      /**
       * Obtains an instance of @c LocalDate from a temporal object.
       * <p>
       * This obtains a local date based on the specified temporal.
       * A @c TemporalAccessor represents an arbitrary set of date and time information,
       * which this factory converts to an instance of @c LocalDate.
       * <p>
       * The conversion uses the @b TemporalQuery::localDate() query, which relies
       * on extracting the @b EPOCH_DAY field.
       * <p>
       * This method matches the signature of the functional interface @b TemporalQuery
       * allowing it to be used as a query via method reference, @c LocalDate::from.
       *
       * @param temporal  the temporal object to convert
       * @return the local date
       * @throws DateTimeException if unable to convert to a @c LocalDate
       */
      static LocalDate from(TemporalAccessor const& temporal);

      /**
       * Obtains an instance of @c LocalDate from a text string such as @c 2007-12-03.
       * <p>
       * The string must represent a valid date and is parsed using
       * @b DateTimeFormatter::ISO_LOCAL_DATE.
       *
       * @param text  the text to parse such as "2007-12-03"
       * @return the parsed local date
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static LocalDate parse(CharSequence const& text);

      /**
       * Obtains an instance of @c LocalDate from a text string using a specific formatter.
       * <p>
       * The text is parsed using the formatter, returning a date.
       *
       * @param text  the text to parse
       * @param formatter  the formatter to use
       * @return the parsed local date
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static LocalDate parse(CharSequence const& text, DateTimeFormatter const& formatter);


      /**
       * Checks if the specified field is supported.
       * <p>
       * This checks if this date can be queried for the specified field.
       * If false, then calling the @b get and @b with(TemporalField,long)
       * methods will throw an exception.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The supported fields are: <br/>
       *
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
       *
       * All other @c TemporalField instances will return false.
       * </p>
       * @param field  the field to check, null returns false
       * @return true if the field is supported on this date, false if not
       */
      gbool isSupported(TemporalField field) const override;

      /**
       * Checks if the specified unit is supported.
       * <p>
       * This checks if the specified unit can be added to, or subtracted from, this date.
       * If false, then calling the <b> plus(long, TemporalUnit)</b> and
       * <b>minus(long, TemporalUnit) </b> methods will throw an exception.
       * </p>
       * <p>
       * If the unit is a @b TemporalUnit then the query is implemented here.
       * The supported units are: <br/>
       *
       * - @c DAYS <br/>
       * - @c WEEKS <br/>
       * - @c MONTHS <br/>
       * - @c YEARS <br/>
       * - @c DECADES <br/>
       * - @c CENTURIES <br/>
       * - @c MILLENNIA <br/>
       * - @c ERAS
       *
       * All other @c TemporalUnit instances will return false.
       * </p>
       * @param unit  the unit to check, null returns false
       * @return true if the unit can be added/subtracted, false if not
       */
      gbool isSupported(TemporalUnit unit) const override;

      /**
       * Gets the range of valid values for the specified field.
       * <p>
       * The range object expresses the minimum and maximum valid values for a field.
       * This date is used to enhance the accuracy of the returned range.
       * If it is not possible to return the range, because the field is not supported
       * or for some other reason, an exception is thrown.
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The @b supported fields will return
       * appropriate range instances.
       * All other @c TemporalField instances will throw an @c TemporalException.
       *
       * @param field  the field to query the range for
       * @return the range of valid values for the field
       * @throws DateTimeException if the range for the field cannot be obtained
       * @throws TemporalException if the field is not supported
       */
      ValueRange range(TemporalField field) const override;

      /**
       * Gets the value of the specified field from this date as an @c int.
       * <p>
       * This queries this date for the value of the specified field.
       * The returned value will always be within the valid range of values for the field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The <b> supported fields</b> will return valid
       * values based on this date, except @c EPOCH_DAY and @c PROLEPTIC_MONTH
       * which are too large to fit in an @c int and throw an @c TemporalException.
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
       * Gets the value of the specified field from this date as a @c long.
       * <p>
       * This queries this date for the value of the specified field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The <b> supported fields</b> will return valid
       * values based on this date.
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
       * provides the <b> int value</b>.
       * </p>
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
      DayOfWeek dayOfWeek() const;


      /**
       * Checks if the year is a leap year, according to the ISO proleptic
       * calendar system rules.
       * <p>
       * This method applies the current rules for leap years across the whole time-line.
       * In general, a year is a leap year if it is divisible by four without
       * remainder. However, years divisible by 100, are not leap years, except for
       *  years divisible by 400 which are.
       * </p>
       * <p>
       * For example, 1904 is a leap year it is divisible by 4.
       * 1900 was not a leap year as it is divisible by 100, however 2000 was a
       * leap year as it is divisible by 400.
       * </p>
       * <p>
       * The calculation is proleptic - applying the same rules into the far future and far past.
       * This is historically inaccurate, but is correct for the ISO-8601 standard.
       * </p>
       * @return true if the year is leap, false otherwise
       */
      gbool isLeapYear() const;

      /**
       * Returns the length of the month represented by this date.
       * <p>
       * This returns the length of the month in days.
       * For example, a date in January would return 31.
       * </p>
       * @return the length of the month in days
       */
      gint lengthOfMonth() const;

      /**
       * Returns the length of the month represented by this date.
       * <p>
       * This returns the length of the month in days.
       * For example, a date in January would return 31.
       * </p>
       * @return the length of the month in days
       */
      gint lengthOfYear() const;

      /**
       * Returns an adjusted copy of this date.
       * <p>
       * This returns a @c LocalDate, based on this one, with the date adjusted.
       * The adjustment takes place using the specified adjuster strategy object.
       * Read the documentation of the adjuster to understand what adjustment will be made.
       * <p>
       * A simple adjuster might simply set the one of the fields, such as the year field.
       * A more complex adjuster might set the date to the last day of the month.
       * <p>
       * A selection of common adjustments is provided in
       * @b TemporalAdjuster.
       * These include finding the "last day of the month" and "next Wednesday".
       * Key date-time classes also implement the @c TemporalAdjuster interface,
       * such as @b MonthDay.
       * The adjuster is responsible for handling special cases, such as the varying
       * lengths of month and leap years.
       * <p>
       * For example this code returns a date on the last day of July:
       * @code
       *  result = localDate.with(TemporalAdjuster::ofMonth(Month::JULY))
       *                    .with(TemporalAdjuster::lastDayOfMonth());
       * @endcode
       * <p>
       * The result of this method is obtained by invoking the
       * @b TemporalAdjuster::adjustInto(Temporal) method on the
       * specified adjuster passing @c this as the argument.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param adjuster the adjuster to use
       * @return a @c LocalDate based on @c this with the adjustment made
       * @throws DateTimeException if the adjustment cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      LocalDate with(TemporalAdjuster const& adjuster) const override;

      /**
       * Returns a copy of this date with the specified field set to a new value.
       * <p>
       * This returns a @c LocalDate, based on this one, with the value
       * for the specified field changed.
       * This can be used to change any supported field, such as the year, month or day-of-month.
       * If it is not possible to set the value, because the field is not supported or for
       * some other reason, an exception is thrown.
       * </p>
       * <p>
       * In some cases, changing the specified field can cause the resulting date to become invalid,
       * such as changing the month from 31st January to February would make the day-of-month invalid.
       * In cases like this, the field is responsible for resolving the date. Typically, it will choose
       * the previous valid date, which would be the last valid day of February in this example.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the adjustment is implemented here.
       * The supported fields behave as follows:
       *
       * - @c DAY_OF_WEEK -
       *  Returns a @c LocalDate with the specified day-of-week.
       *  The date is adjusted up to 6 days forward or backward within the boundary
       *  of a Monday to Sunday week. <br/>
       * - @c ALIGNED_DAY_OF_WEEK_IN_MONTH -
       *  Returns a @c LocalDate with the specified aligned-day-of-week.
       *  The date is adjusted to the specified month-based aligned-day-of-week.
       *  Aligned weeks are counted such that the first week of a given month starts
       *  on the first day of that month.
       *  This may cause the date to be moved up to 6 days into the following month. <br/>
       * - @c ALIGNED_DAY_OF_WEEK_IN_YEAR -
       *  Returns a @c LocalDate with the specified aligned-day-of-week.
       *  The date is adjusted to the specified year-based aligned-day-of-week.
       *  Aligned weeks are counted such that the first week of a given year starts
       *  on the first day of that year.
       *  This may cause the date to be moved up to 6 days into the following year. <br/>
       * - @c DAY_OF_MONTH -
       *  Returns a @c LocalDate with the specified day-of-month.
       *  The month and year will be unchanged. If the day-of-month is invalid for the
       *  year and month, then a @c DateTimeException is thrown. <br/>
       * - @c DAY_OF_YEAR -
       *  Returns a @c LocalDate with the specified day-of-year.
       *  The year will be unchanged. If the day-of-year is invalid for the
       *  year, then a @c DateTimeException is thrown. <br/>
       * - @c EPOCH_DAY -
       *  Returns a @c LocalDate with the specified epoch-day.
       *  This completely replaces the date and is equivalent to @b ofEpochDay(long). <br/>
       * - @c ALIGNED_WEEK_OF_MONTH -
       *  Returns a @c LocalDate with the specified aligned-week-of-month.
       *  Aligned weeks are counted such that the first week of a given month starts
       *  on the first day of that month.
       *  This adjustment moves the date in whole week chunks to match the specified week.
       *  The result will have the same day-of-week as this date.
       *  This may cause the date to be moved into the following month. <br/>
       * - @c ALIGNED_WEEK_OF_YEAR -
       *  Returns a @c LocalDate with the specified aligned-week-of-year.
       *  Aligned weeks are counted such that the first week of a given year starts
       *  on the first day of that year.
       *  This adjustment moves the date in whole week chunks to match the specified week.
       *  The result will have the same day-of-week as this date.
       *  This may cause the date to be moved into the following year. <br/>
       * - @c MONTH_OF_YEAR -
       *  Returns a @c LocalDate with the specified month-of-year.
       *  The year will be unchanged. The day-of-month will also be unchanged,
       *  unless it would be invalid for the new month and year. In that case, the
       *  day-of-month is adjusted to the maximum valid value for the new month and year. <br/>
       * - @c PROLEPTIC_MONTH -
       *  Returns a @c LocalDate with the specified proleptic-month.
       *  The day-of-month will be unchanged, unless it would be invalid for the new month
       *  and year. In that case, the day-of-month is adjusted to the maximum valid value
       *  for the new month and year. <br/>
       * - @c YEAR_OF_ERA -
       *  Returns a @c LocalDate with the specified year-of-era.
       *  The era and month will be unchanged. The day-of-month will also be unchanged,
       *  unless it would be invalid for the new month and year. In that case, the
       *  day-of-month is adjusted to the maximum valid value for the new month and year. <br/>
       * - @c YEAR -
       *  Returns a @c LocalDate with the specified year.
       *  The month will be unchanged. The day-of-month will also be unchanged,
       *  unless it would be invalid for the new month and year. In that case, the
       *  day-of-month is adjusted to the maximum valid value for the new month and year. <br/>
       * - @c ERA -
       *  Returns a @c LocalDate with the specified era.
       *  The year-of-era and month will be unchanged. The day-of-month will also be unchanged,
       *  unless it would be invalid for the new month and year. In that case, the
       *  day-of-month is adjusted to the maximum valid value for the new month and year. <br/>
       *
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
       * @param field  the field to set in the result
       * @param newValue  the new value of the field in the result
       * @return a @c LocalDate based on @c this with the specified field set
       * @throws DateTimeException if the field cannot be set
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      LocalDate with(TemporalField field, glong newValue) const override;


      /**
       * Returns a copy of this @c LocalDate with the year altered.
       * <p>
       * If the day-of-month is invalid for the year, it will be changed to the last valid day of the month.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param year  the year to set in the result, from MIN_YEAR to MAX_YEAR
       * @return a @c LocalDate based on this date with the requested year
       * @throws DateTimeException if the year value is invalid
       */
      LocalDate withYear(gint year) const;

      /**
       * Returns a copy of this @c LocalDate with the month-of-year altered.
       * <p>
       * If the day-of-month is invalid for the year, it will be changed to the last valid day of the month.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param month  the month-of-year to set in the result, from 1 (January) to 12 (December)
       * @return a @c LocalDate based on this date with the requested month
       * @throws DateTimeException if the month-of-year value is invalid
       */
      LocalDate withMonth(gint month) const;

      /**
       * Returns a copy of this @c LocalDate with the day-of-month altered.
       * <p>
       * If the resulting date is invalid, an exception is thrown.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param dayOfMonth  the day-of-month to set in the result, from 1 to 28-31
       * @return a @c LocalDate based on this date with the requested day
       * @throws DateTimeException if the day-of-month value is invalid,
       *  or if the day-of-month is invalid for the month-year
       */
      LocalDate withDayOfMonth(gint dayOfMonth) const;

      /**
       * Returns a copy of this @c LocalDate with the day-of-year altered.
       * <p>
       * If the resulting date is invalid, an exception is thrown.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param dayOfYear  the day-of-year to set in the result, from 1 to 365-366
       * @return a @c LocalDate based on this date with the requested day
       * @throws DateTimeException if the day-of-year value is invalid,
       *  or if the day-of-year is invalid for the year
       */
      LocalDate withDayOfYear(gint dayOfYear) const;

      /**
       * Returns a copy of this date with the specified amount added.
       * <p>
       * This returns a @c LocalDate, based on this one, with the specified amount added.
       * The amount is typically @b Period but may be any other type implementing
       * the @b TemporalAmount interface.
       * <p>
       * The calculation is delegated to the amount object by calling
       * @b TemporalAmount::addTo(Temporal). The amount implementation is free
       * to implement the addition in any way it wishes, however it typically
       * calls back to @b plus(long, TemporalUnit). Consult the documentation
       * of the amount implementation to determine if it can be successfully added.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param amountToAdd  the amount to add
       * @return a @c LocalDate based on this date with the addition made
       * @throws DateTimeException if the addition cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      LocalDate plus(TemporalAmount const& amountToAdd) const override;

      /**
       * Returns a copy of this date with the specified amount added.
       * <p>
       * This returns a @c LocalDate, based on this one, with the amount
       * in terms of the unit added. If it is not possible to add the amount, because the
       * unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * In some cases, adding the amount can cause the resulting date to become invalid.
       * For example, adding one month to 31st January would result in 31st February.
       * In cases like this, the unit is responsible for resolving the date.
       * Typically, it will choose the previous valid date, which would be the last valid
       * day of February in this example.
       * </p>
       * <p>
       * If the field is a @b TemporalUnit then the addition is implemented here.
       * The supported fields behave as follows: <br/>
       *
       * - @c DAYS -
       *  Returns a @c LocalDate with the specified number of days added.
       *  This is equivalent to @b plusDays(long). <br/>
       * - @c WEEKS -
       *  Returns a @c LocalDate with the specified number of weeks added.
       *  This is equivalent to @b plusWeeks(long) and uses a 7 days week. <br/>
       * - @c MONTHS -
       *  Returns a @c LocalDate with the specified number of months added.
       *  This is equivalent to @b plusMonths(long).
       *  The day-of-month will be unchanged unless it would be invalid for the new
       *  month and year. In that case, the day-of-month is adjusted to the maximum
       *  valid value for the new month and year. <br/>
       * - @c YEARS -
       *  Returns a @c LocalDate with the specified number of years added.
       *  This is equivalent to @b plusYears(long).
       *  The day-of-month will be unchanged unless it would be invalid for the new
       *  month and year. In that case, the day-of-month is adjusted to the maximum
       *  valid value for the new month and year. <br/>
       * - @c DECADES -
       *  Returns a @c LocalDate with the specified number of decades added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 10.
       *  The day-of-month will be unchanged unless it would be invalid for the new
       *  month and year. In that case, the day-of-month is adjusted to the maximum
       *  valid value for the new month and year. <br/>
       * - @c CENTURIES -
       *  Returns a @c LocalDate with the specified number of centuries added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 100.
       *  The day-of-month will be unchanged unless it would be invalid for the new
       *  month and year. In that case, the day-of-month is adjusted to the maximum
       *  valid value for the new month and year. <br/>
       * - @c MILLENNIA -
       *  Returns a @c LocalDate with the specified number of millennia added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 1,000.
       *  The day-of-month will be unchanged unless it would be invalid for the new
       *  month and year. In that case, the day-of-month is adjusted to the maximum
       *  valid value for the new month and year. <br/>
       * - @c ERAS -
       *  Returns a @c LocalDate with the specified number of eras added.
       *  Only two eras are supported so the amount must be one, zero or minus one.
       *  If the amount is non-zero then the year is changed such that the year-of-era
       *  is unchanged.
       *  The day-of-month will be unchanged unless it would be invalid for the new
       *  month and year. In that case, the day-of-month is adjusted to the maximum
       *  valid value for the new month and year. <br/>
       *
       * </p>
       * <p>
       * All other @c TemporalUnit instances will throw an @c TemporalException.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param amountToAdd  the amount of the unit to add to the result, may be negative
       * @param unit  the unit of the amount to add
       * @return a @c LocalDate based on this date with the specified amount added
       * @throws DateTimeException if the addition cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      LocalDate plus(glong amountToAdd, TemporalUnit unit) const override;


      /**
       * Returns a copy of this @c LocalDate with the specified number of years added.
       * <p>
       * This method adds the specified amount to the years field in three steps: <br/>
       *
       * - Add the input years to the year field <br/>
       * - Check if the resulting date would be invalid <br/>
       * - Adjust the day-of-month to the last valid day if necessary <br/>
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
       * @param yearsToAdd  the years to add, may be negative
       * @return a @c LocalDate based on this date with the years added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      LocalDate plusYears(glong yearsToAdd) const;

      /**
       * Returns a copy of this @c LocalDate with the specified number of months added.
       * <p>
       * This method adds the specified amount to the months field in three steps: <br/>
       *
       * - Add the input months to the month-of-year field <br/>
       * - Check if the resulting date would be invalid <br/>
       * - Adjust the day-of-month to the last valid day if necessary <br/>
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
       * @param monthsToAdd  the months to add, may be negative
       * @return a @c LocalDate based on this date with the months added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      LocalDate plusMonths(glong monthsToAdd) const;

      /**
       * Returns a copy of this @c LocalDate with the specified number of weeks added.
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
       * @param weeksToAdd  the weeks to add, may be negative
       * @return a @c LocalDate based on this date with the weeks added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      LocalDate plusWeeks(glong weeksToAdd) const;

      /**
       * Returns a copy of this @c LocalDate with the specified number of days added.
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
       * @param daysToAdd  the days to add, may be negative
       * @return a @c LocalDate based on this date with the days added
       * @throws DateTimeException if the result exceeds the supported date range
       */
      LocalDate plusDays(glong daysToAdd) const;

      /**
       * Returns a copy of this date with the specified amount subtracted.
       * <p>
       * This returns a @c LocalDate, based on this one, with the specified amount subtracted.
       * The amount is typically @b Period but may be any other type implementing
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
       * @return a @c LocalDate based on this date with the subtraction made
       * @throws DateTimeException if the subtraction cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      LocalDate minus(TemporalAmount const& amountToSubtract) const override;

      /**
       * Returns a copy of this date with the specified amount subtracted.
       * <p>
       * This returns a @c LocalDate, based on this one, with the amount
       * in terms of the unit subtracted. If it is not possible to subtract the amount,
       * because the unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * This method is equivalent to <b>plus(long, TemporalUnit)</b> with the amount negated.
       * See that method for a full description of how addition, and thus subtraction, works.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
       * @param unit  the unit of the amount to subtract
       * @return a @c LocalDate based on this date with the specified amount subtracted
       * @throws DateTimeException if the subtraction cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      LocalDate minus(glong amountToSubtract, TemporalUnit unit) const override;


      /**
       * Returns a copy of this @c LocalDate with the specified number of years subtracted.
       * <p>
       * This method subtracts the specified amount from the years field in three steps: <br/>
       *
       * - Subtract the input years from the year field <br/>
       * - Check if the resulting date would be invalid <br/>
       * - Adjust the day-of-month to the last valid day if necessary <br/>
       *
       * </p>
       * <p>
       * For example, 2008-02-29 (leap year) minus one year would result in the
       * invalid date 2007-02-29 (standard year). Instead of returning an invalid
       * result, the last valid day of the month, 2007-02-28, is selected instead.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param yearsToSubtract  the years to subtract, may be negative
       * @return a @c LocalDate based on this date with the years subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      LocalDate minusYears(glong yearsToSubtract) const;

      /**
       * Returns a copy of this @c LocalDate with the specified number of months subtracted.
       * <p>
       * This method subtracts the specified amount from the months field in three steps: <br/>
       *
       * - Subtract the input months from the month-of-year field <br/>
       * - Check if the resulting date would be invalid <br/>
       * - Adjust the day-of-month to the last valid day if necessary <br/>
       *
       * </p>
       * <p>
       * For example, 2007-03-31 minus one month would result in the invalid date
       * 2007-02-31. Instead of returning an invalid result, the last valid day
       * of the month, 2007-02-28, is selected instead.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param monthsToSubtract  the months to subtract, may be negative
       * @return a @c LocalDate based on this date with the months subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      LocalDate minusMonths(glong monthsToSubtract) const;

      /**
       * Returns a copy of this @c LocalDate with the specified number of weeks subtracted.
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
       * @param weeksToSubtract  the weeks to subtract, may be negative
       * @return a @c LocalDate based on this date with the weeks subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      LocalDate minusWeeks(glong weeksToSubtract) const;

      /**
       * Returns a copy of this @c LocalDate with the specified number of days subtracted.
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
       * @param daysToSubtract  the days to subtract, may be negative
       * @return a @c LocalDate based on this date with the days subtracted
       * @throws DateTimeException if the result exceeds the supported date range
       */
      LocalDate minusDays(glong daysToSubtract) const;

      /**
       * Queries this date using the specified query.
       * <p>
       * This queries this date using the specified query strategy object.
       * The @c TemporalQuery object defines the logic to be used to
       * obtain the result. Read the documentation of the query to understand
       * what the result of this method will be.
       * </p>
       * <p>
       * The result of this method is obtained by invoking the
       * @em TemporalQuery::queryFrom(Temporal) method on the
       * specified query passing @c this as the argument.
       * </p>
       * @param query  the query to invoke
       * @return the query result, no option may be returned (defined by the query)
       * @throws DateTimeException if unable to query (defined by the query)
       * @throws ArithmeticException if numeric overflow occurs (defined by the query)
       */
      Optional query(TemporalQuery const& query) const override;

      /**
       * Adjusts the specified temporal object to have the same date as this object.
       * <p>
       * This returns a temporal object of the same observable type as the input
       * with the date changed to be the same as this.
       * </p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param temporal  the target object to be adjusted
       * @return the adjusted object
       * @throws DateTimeException if unable to make the adjustment
       * @throws ArithmeticException if numeric overflow occurs
       */
      Temporal& adjustInto(Temporal const& temporal) const override;

      /**
       * Calculates the amount of time until another date in terms of the specified unit.
       * <p>
       * This calculates the amount of time between two @c LocalDate
       * objects in terms of a single @c TemporalUnit.
       * The start and end points are @c this and the specified date.
       * The result will be negative if the end is before the start.
       * </p>
       * <p>
       * The calculation returns a whole number, representing the number of
       * complete units between the two dates.
       * For example, the amount in months between 2012-06-15 and 2012-08-14
       * will only be one month as it is one day short of two months.
       * </p>
       * <p>
       * The calculation is implemented in this method for @b TemporalUnit.
       * The units @c DAYS, @c WEEKS, @c MONTHS, @c YEARS,
       * @c DECADES, @c CENTURIES, @c MILLENNIA and @c ERAS
       * are supported. Other @c TemporalUnit values will throw an exception.
       * </p>
       * <p>
       * This instance is immutable and unaffected by this method call.
       * </p>
       * @param endExclusive  the end date, exclusive, which is converted to a @c LocalDate
       * @param unit  the unit to measure the amount in
       * @return the amount of time between this date and the end date
       * @throws DateTimeException if the amount cannot be calculated, or the end
       *  temporal cannot be converted to a @c LocalDate
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong until(const Temporal& endExclusive, TemporalUnit unit) const override;

      /**
       * Calculates the period between this date and another date as a @c Period.
       * <p>
       * This calculates the period between two dates in terms of years, months and days.
       * The start and end points are @c this and the specified date.
       * The result will be negative if the end is before the start.
       * The negative sign will be the same in each of year, month and day.
       * </p>
       * <p>
       * The calculation is performed using the ISO calendar system.
       * If necessary, the input date will be converted to ISO.
       * </p>
       * <p>
       * The start date is included, but the end date is not.
       * The period is calculated by removing complete months, then calculating
       * the remaining number of days, adjusting to ensure that both have the same sign.
       * The number of months is then normalized into years and months based on a 12 month year.
       * A month is considered to be complete if the end day-of-month is greater
       * than or equal to the start day-of-month.
       * For example, from @c 2010-01-15 to @c 2011-03-18 is "1 year, 2 months and 3 days".
       * </p>
       * <p>
       * There are two equivalent ways of using this method.
       * The first is to invoke this method.
       * The second is to use @b Period::between(LocalDate, LocalDate):
       * @code
       *   // these two lines are equivalent
       *   period = start.until(end);
       *   period = Period.between(start, end);
       * @endcode
       * The choice should be made based on which makes the code more readable.
       * </p>
       * @param endDateExclusive  the end date, exclusive, which may be in any chronology
       * @return the period between this date and the end date
       */
      Period until(LocalDate const& endDateExclusive) const;

      /**
       * Formats this date using the specified formatter.
       * <p>
       * This date will be passed to the formatter to produce a string.
       *
       * @param formatter  the formatter to use
       * @return the formatted date string
       * @throws DateTimeException if an error occurs during printing
       */
      String format(DateTimeFormatter const& formatter) const;

      /**
       * Combines this date with a time to create a @c LocalDateTime.
       * <p>
       * This returns a @c LocalDateTime formed from this date at the specified time.
       * All possible combinations of date and time are valid.
       * </p>
       * @param time  the time to combine with
       * @return the local date-time formed from this date and the specified time
       */
      LocalDateTime atTime(LocalTime const& time) const;

      /**
       * Combines this date with a time to create a @c LocalDateTime.
       * <p>
       * This returns a @c LocalDateTime formed from this date at the
       * specified hour and minute.
       * The seconds and nanosecond fields will be set to zero.
       * The individual time fields must be within their valid range.
       * All possible combinations of date and time are valid.
       * </p>
       * @param hour  the hour-of-day to use, from 0 to 23
       * @param minute  the minute-of-hour to use, from 0 to 59
       * @return the local date-time formed from this date and the specified time
       * @throws DateTimeException if the value of any field is out of range
       */
      LocalDateTime atTime(gint hour, gint minute) const;

      /**
       * Combines this date with a time to create a @c LocalDateTime.
       * <p>
       * This returns a @c LocalDateTime formed from this date at the
       * specified hour, minute and second.
       * The nanosecond field will be set to zero.
       * The individual time fields must be within their valid range.
       * All possible combinations of date and time are valid.
       * </p>
       * @param hour  the hour-of-day to use, from 0 to 23
       * @param minute  the minute-of-hour to use, from 0 to 59
       * @param second  the second-of-minute to represent, from 0 to 59
       * @return the local date-time formed from this date and the specified time
       * @throws DateTimeException if the value of any field is out of range
       */
      LocalDateTime atTime(gint hour, gint minute, gint second) const;

      /**
       * Combines this date with a time to create a @c LocalDateTime.
       * <p>
       * This returns a @c LocalDateTime formed from this date at the
       * specified hour, minute, second and nanosecond.
       * The individual time fields must be within their valid range.
       * All possible combinations of date and time are valid.
       * </p>
       * @param hour  the hour-of-day to use, from 0 to 23
       * @param minute  the minute-of-hour to use, from 0 to 59
       * @param second  the second-of-minute to represent, from 0 to 59
       * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
       * @return the local date-time formed from this date and the specified time
       * @throws DateTimeException if the value of any field is out of range
       */
      LocalDateTime atTime(gint hour, gint minute, gint second, gint nanoOfSecond) const;

      /**
       * Combines this date with the time of midnight to create a @c LocalDateTime
       * at the start of this date.
       * <p>
       * This returns a @c LocalDateTime formed from this date at the time of
       * midnight, 00:00, at the start of this date.
       * </p>
       * @return the local date-time of midnight at the start of this date
       */
      LocalDateTime atStartOfDay() const;

      /**
       * Returns a zoned date-time from this date at the earliest valid time according
       * to the rules in the time-zone.
       * <p>
       * Time-zone rules, such as daylight savings, mean that not every local date-time
       * is valid for the specified zone, thus the local date-time may not be midnight.
       * </p>
       * <p>
       * In most cases, there is only one valid offset for a local date-time.
       * In the case of an overlap, there are two valid offsets, and the earlier one is used,
       * corresponding to the first occurrence of midnight on the date.
       * In the case of a gap, the zoned date-time will represent the instant just after the gap.
       * </p>
       * <p>
       * If the zone ID is a @b ZoneOffset, then the result always has a time of midnight.
       * </p>
       * <p>
       * To convert to a specific time in a given time-zone call @b atTime(LocalTime)
       * followed by @b LocalDateTime#atZone(ZoneId).
       * </p>
       * @param zone  the zone ID to use
       * @return the zoned date-time formed from this date and the earliest valid time for the zone
       */
      ZonedDateTime atStartOfDay(ZoneId const& zone) const;

      /**
       * Converts this date to the Epoch Day.
       * <p>
       * The <b>Epoch Day count</b> is a simple
       * incrementing count of days when day 0 is 1970-01-01 (ISO).
       * This definition is the same for all chronologies, enabling conversion.
       * </p>
       * <p>
       * This default implementation queries the @c EPOCH_DAY field.
       * </p>
       * @return the Epoch Day equivalent to this date
       */
      glong toEpochDay() const;

      /**
       * Converts this @c LocalDate to the number of seconds since the epoch
       * of 1970-01-01T00:00:00Z.
       * <p>
       * This combines this local date with the specified time and
       * offset to calculate the epoch-second value, which is the
       * number of elapsed seconds from 1970-01-01T00:00:00Z.
       * Instants on the time-line after the epoch are positive, earlier
       * are negative.
       * </p>
       * @param time the local time
       * @param offset the zone offset
       * @return the number of seconds since the epoch of 1970-01-01T00:00:00Z, may be negative
       */
      glong toEpochSecond(LocalTime const& time, ZoneOffset const& offset) const;

      /**
       * Compares this date to another date.
       * <p>
       * The comparison is primarily based on the date, from earliest to latest.
       * It is "consistent with equals", as defined by @b Comparable.
       * </p>
       * <p>
       * If all the dates being compared are instances of @c LocalDate,
       * then the comparison will be entirely based on the date.
       * If some dates being compared are in different chronologies, then the
       * chronology is also considered, see @b ChronoLocalDate::compareTo.
       * </p>
       * @param other  the other date to compare to
       * @return the comparator value, that is the comparison of this local date with
       *          the @c other local date and this chronology with the @c other chronology,
       *          in order, returning the first non-zero result, and otherwise returning zero
       * @see isBefore
       * @see isAfter
       */
      gint compareTo(const LocalDate& other) const override;

      /**
       * Checks if this date is after the specified date.
       * <p>
       * This checks to see if this date represents a point on the
       * local time-line after the other date.
       * @code
       *   LocalDate a = LocalDate(2012, 6, 30);
       *   LocalDate b = LocalDate(2012, 7, 1);
       *   a.isAfter(b) == false
       *   a.isAfter(a) == false
       *   b.isAfter(a) == true
       * @endcode
       * </p>
       * <p>
       * This method only considers the position of the two dates on the local time-line.
       * It does not take into account the chronology, or calendar system.
       * This is different from the comparison in @b compareTo(ChronoLocalDate),
       * but is the same approach as @b ChronoLocalDate::timeLineOrder().
       * </p>
       * @param other  the other date to compare to
       * @return true if this date is after the specified date
       */
      gbool isAfter(LocalDate const& other) const;

      /**
       * Checks if this date is before the specified date.
       * <p>
       * This checks to see if this date represents a point on the
       * local time-line before the other date.
       * @code
       *   LocalDate a = LocalDate(2012, 6, 30);
       *   LocalDate b = LocalDate(2012, 7, 1);
       *   a.isBefore(b) == true
       *   a.isBefore(a) == false
       *   b.isBefore(a) == false
       * @endcode
       * </p>
       * <p>
       * This method only considers the position of the two dates on the local time-line.
       * It does not take into account the chronology, or calendar system.
       * This is different from the comparison in @b compareTo(ChronoLocalDate),
       * but is the same approach as @b ChronoLocalDate::timeLineOrder().
       * </p>
       * @param other  the other date to compare to
       * @return true if this date is before the specified date
       */
      gbool isBefore(LocalDate const& other) const;

      /**
       * Checks if this date is equal to another date.
       * <p>
       * Compares this @c LocalDate with another ensuring that the date is the same.
       * </p>
       * <p>
       * Only objects of type @c LocalDate are compared, other types return false.
       * </p>
       * @param obj  the object to check, null returns false
       * @return true if this is equal to the other date
       */
      gbool equals(const Object& obj) const override;

      /**
       * A hash code for this date.
       *
       * @return a suitable hash code
       */
      gint hash() const override;


      /**
       * Outputs this date as a @c String, such as @c 2007-12-03.
       * <p>
       * The output will be in the ISO-8601 format @c uuuu-MM-dd.
       * </p>
       * @return a string representation of this date
       */
      String toString() const override;

      Object& clone() const override;

    private:
      static CORE_FAST gint SECONDS_PER_DAY = LocalTime::SECONDS_PER_DAY;

      /**
       * Creates a local date from the year, month and day fields.
       *
       * @param year  the year to represent, validated from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent, from 1 to 12, validated
       * @param dayOfMonth  the day-of-month to represent, validated from 1 to 31
       * @return the local date
       * @throws DateTimeException if the day-of-month is invalid for the month-year
       */
      static LocalDate create(gint year, gint month, gint dayOfMonth);

      /**
       * Resolves the date, resolving days past the end of month.
       *
       * @param year  the year to represent, validated from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent, validated from 1 to 12
       * @param day  the day-of-month to represent, validated from 1 to 31
       * @return the resolved date
       */
      static LocalDate resolve(gint year, gint month, gint day);

      gint prolepticMonth() const;

      glong daysUntil(LocalDate const& end) const;

      glong monthsUntil(LocalDate const& end) const;

      CORE_ADD_AS_FRIEND(LocalDateTime);
    };
  } // util
} // core

#endif //CORE24_LOCALDATE_H
