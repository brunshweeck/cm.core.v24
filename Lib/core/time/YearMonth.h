//
// Created by admin on 06/01/25.
//

#ifndef CORE24_YEARMONTH_H
#define CORE24_YEARMONTH_H

#include <core/time/LocalDate.h>

namespace core {
  namespace time {
    /**
     * A year-month in the ISO-8601 calendar system, such as @c 2007-12.
     * <p>
     * @c YearMonth is an immutable date-time object that represents the combination
     * of a year and month. Any field that can be derived from a year and month, such as
     * quarter-of-year, can be obtained.
     * </p>
     * <p>
     * This class does not store or represent a day, time or time-zone.
     * For example, the value "October 2007" can be stored in a @c YearMonth.
     * </p>
     * <p>
     * The ISO-8601 calendar system is the modern civil calendar system used today
     * in most of the world. It is equivalent to the proleptic Gregorian calendar
     * system, in which today's rules for leap years are applied for all time.
     * For most applications written today, the ISO-8601 rules are entirely suitable.
     * However, any application that makes use of historical dates, and requires them
     * to be accurate will find the ISO-8601 approach unsuitable.
     * </p>
     */
    class YearMonth final : public virtual TemporalBase<YearMonth>,
                            public virtual TemporalAdjuster,
                            public virtual Comparable<YearMonth> {
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, Optional);

      /**
       * The year.
       */
      gint year_;

      /**
       * The month-of-year.
       */
      gint month_;

      /**
       * Constructor.
       *
       * @param year  the year to represent, validated from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent, validated from 1 (January) to 12 (December)
       */
      YearMonth(gint year, gint month);

    public:
      /**
       * Obtains the current year-month from the system clock in the default time-zone.
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       * </p>
       * @return the current year-month using the system clock and default time-zone, not null
       */
      static YearMonth now();

      /**
       * Obtains the current year-month from the system clock in the specified time-zone.
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       * </p>
       * @param zone  the zone ID to use, not null
       * @return the current year-month using the system clock, not null
       */
      static YearMonth now(ZoneId const& zone);

      /**
       * Obtains an instance of @c YearMonth from a year and month.
       *
       * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent, not null
       * @return the year-month, not null
       * @throws DateTimeException if the year value is invalid
       */
      static YearMonth of(gint year, Month month);

      /**
       * Obtains an instance of @c YearMonth from a year and month.
       *
       * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
       * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
       * @return the year-month, not null
       * @throws DateTimeException if either field value is invalid
       */
      static YearMonth of(gint year, gint month);

      /**
       * Obtains an instance of @c YearMonth from a temporal object.
       * <p>
       * This obtains a year-month based on the specified temporal.
       * A @c Temporal represents an arbitrary set of date and time information,
       * which this factory converts to an instance of @c YearMonth.
       * </p>
       * <p>
       * The conversion extracts the @b YEAR and @b MONTH_OF_YEAR fields.
       * The extraction is only permitted if the temporal object has an ISO
       * chronology, or can be converted to a @c LocalDate.
       * </p>
       * <p>
       * This method matches the signature of the functional interface @b TemporalQuery
       * allowing it to be used as a query via method reference, @c YearMonth::from.
       * </p>
       * @param temporal  the temporal object to convert, not null
       * @return the year-month, not null
       * @throws DateTimeException if unable to convert to a @c YearMonth
       */
      static YearMonth from(TemporalAccessor const& temporal);

      /**
       * Obtains an instance of {@code YearMonth} from a text string such as {@code 2007-12}.
       * <p>
       * The string must represent a valid year-month.
       * The format must be {@code uuuu-MM}.
       * Years outside the range 0000 to 9999 must be prefixed by the plus or minus symbol.
       *
       * @param text  the text to parse such as "2007-12", not null
       * @return the parsed year-month, not null
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static YearMonth parse(CharSequence const& text);

      /**
       * Obtains an instance of {@code YearMonth} from a text string using a specific formatter.
       * <p>
       * The text is parsed using the formatter, returning a year-month.
       *
       * @param text  the text to parse, not null
       * @param formatter  the formatter to use, not null
       * @return the parsed year-month, not null
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static YearMonth parse(CharSequence const& text, DateTimeFormatter const& formatter);

      /**
       * Checks if the specified field is supported.
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The supported fields are:
       *
       * @li @c MONTH_OF_YEAR
       * @li @c PROLEPTIC_MONTH
       * @li @c YEAR_OF_ERA
       * @li @c YEAR
       * @li @c ERA
       * </p>
       * All other @c TemporalField instances will return false.
       * @param field  the field to check, null returns false
       * @return true if the field is supported on this year-month, false if not
       */
      gbool isSupported(TemporalField field) const override;

      /**
       * Checks if the specified unit is supported.
       * <p>
       * If the unit is a @b TemporalUnit then the query is implemented here.
       * The supported units are:
       *
       * @li @c MONTHS
       * @li @c YEARS
       * @li @c DECADES
       * @li @c CENTURIES
       * @li @c MILLENNIA
       * @li @c ERAS
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
       * This year-month is used to enhance the accuracy of the returned range.
       * If it is not possible to return the range, because the field is not supported
       * or for some other reason, an exception is thrown.
       * <p>
       * If the field is a {@link ChronoField} then the query is implemented here.
       * The {@link #isSupported(TemporalField) supported fields} will return
       * appropriate range instances.
       * All other {@code ChronoField} instances will throw an {@code UnsupportedTemporalTypeException}.
       * <p>
       * If the field is not a {@code ChronoField}, then the result of this method
       * is obtained by invoking {@code TemporalField.rangeRefinedBy(TemporalAccessor)}
       * passing {@code this} as the argument.
       * Whether the range can be obtained is determined by the field.
       *
       * @param field  the field to query the range for, not null
       * @return the range of valid values for the field, not null
       * @throws DateTimeException if the range for the field cannot be obtained
       * @throws UnsupportedTemporalTypeException if the field is not supported
       */
      ValueRange range(TemporalField field) const override;

      /**
       * Gets the value of the specified field from this year-month as an @c int.
       * <p>
       * This queries this year-month for the value of the specified field.
       * The returned value will always be within the valid range of values for the field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The @b supported fields will return valid
       * values based on this year-month, except @c PROLEPTIC_MONTH which is too
       * large to fit in an @c int and throw a @c DateTimeException.
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       * @param field  the field to get, not null
       * @return the value for the field
       * @throws DateTimeException if a value for the field cannot be obtained or
       *         the value is outside the range of valid values for the field
       * @throws TemporalException if the field is not supported or
       *         the range of values exceeds an @c int
       * @throws ArithmeticException if numeric overflow occurs
       */
      gint get(TemporalField field) const override;

      /**
       * Gets the value of the specified field from this year-month as a @c long.
       * <p>
       * This queries this year-month for the value of the specified field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The supported fields will return valid
       * values based on this year-month.
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       * @param field  the field to get, not null
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
       * </p>
       * @return the year, from MIN_YEAR to MAX_YEAR
       */
      gint year() const;

      /**
       * Gets the month-of-year field using the @c Month enum.
       *
       * @return the month-of-year, not null
       * @see #getMonthValue()
       */
      Month month() const;

      /**
       * Checks if the year is a leap year, according to the ISO proleptic
       * calendar system rules.
       * <p>
       * This method applies the current rules for leap years across the whole time-line.
       * In general, a year is a leap year if it is divisible by four without
       * remainder. However, years divisible by 100, are not leap years, except
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
       * Checks if the day-of-month is valid for this year-month.
       * <p>
       * This method checks whether this year and month and the input day form
       * a valid date.
       * </p>
       * @param dayOfMonth  the day-of-month to validate, from 1 to 31, invalid value returns false
       * @return true if the day is valid for this year-month
       */
      gbool isValidDay(gint dayOfMonth) const;

      /**
       * Returns the length of the month, taking account of the year.
       * <p>
       * This returns the length of the month in days.
       * For example, a date in January would return 31.
       * </p>
       * @return the length of the month in days, from 28 to 31
       */
      gint lengthOfMonth() const;

      /**
       * Returns the length of the year.
       * <p>
       * This returns the length of the year in days, either 365 or 366.
       * </p>
       * @return 366 if the year is leap, 365 otherwise
       */
      gint lengthOfYear() const;

      /**
       * Returns an adjusted copy of this year-month.
       * <p>
       * This returns a {@code YearMonth}, based on this one, with the year-month adjusted.
       * The adjustment takes place using the specified adjuster strategy object.
       * Read the documentation of the adjuster to understand what adjustment will be made.
       * <p>
       * A simple adjuster might simply set the one of the fields, such as the year field.
       * A more complex adjuster might set the year-month to the next month that
       * Halley's comet will pass the Earth.
       * <p>
       * The result of this method is obtained by invoking the
       * {@link TemporalAdjuster#adjustInto(Temporal)} method on the
       * specified adjuster passing {@code this} as the argument.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param adjuster the adjuster to use, not null
       * @return a {@code YearMonth} based on {@code this} with the adjustment made, not null
       * @throws DateTimeException if the adjustment cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      YearMonth with(TemporalAdjuster const& adjuster) const override;

      /**
       * Returns a copy of this year-month with the specified field set to a new value.
       * <p>
       * This returns a @c YearMonth, based on this one, with the value
       * for the specified field changed.
       * This can be used to change any supported field, such as the year or month.
       * If it is not possible to set the value, because the field is not supported or for
       * some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalField then the adjustment is implemented here.
       * The supported fields behave as follows:
       *
       * @li @c MONTH_OF_YEAR -
       *  Returns a @c YearMonth with the specified month-of-year.
       *  The year will be unchanged.
       * @li @c PROLEPTIC_MONTH -
       *  Returns a @c YearMonth with the specified proleptic-month.
       *  This completely replaces the year and month of this object.
       * @li @c YEAR_OF_ERA -
       *  Returns a @c YearMonth with the specified year-of-era
       *  The month and era will be unchanged.
       * @li @c YEAR -
       *  Returns a @c YearMonth with the specified year.
       *  The month will be unchanged.
       * @li @c ERA -
       *  Returns a @c YearMonth with the specified era.
       *  The month and year-of-era will be unchanged.
       * </p>
       * <p>
       * In all cases, if the new value is outside the valid range of values for the field
       * then a @c DateTimeException will be thrown.
       * </p>
       * <p>
       * All other @c TemporalField instances will throw an @c TemporalException.
       * </p>
       * @param field  the field to set in the result, not null
       * @param newValue  the new value of the field in the result
       * @return a @c YearMonth based on @c this with the specified field set, not null
       * @throws DateTimeException if the field cannot be set
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      YearMonth with(TemporalField field, glong newValue) const;

      /**
       * Returns a copy of this @c YearMonth with the year altered.
       *
       * @param year  the year to set in the returned year-month, from MIN_YEAR to MAX_YEAR
       * @return a @c YearMonth based on this year-month with the requested year, not null
       * @throws DateTimeException if the year value is invalid
       */
      YearMonth withYear(gint year) const;

      /**
       * Returns a copy of this @c YearMonth with the month-of-year altered.
       *
       * @param month  the month-of-year to set in the returned year-month, from 1 (January) to 12 (December)
       * @return a @c YearMonth based on this year-month with the requested month, not null
       * @throws DateTimeException if the month-of-year value is invalid
       */
      YearMonth withMonth(gint month) const;

      /**
       * Returns a copy of this year-month with the specified amount added.
       * <p>
       * This returns a {@code YearMonth}, based on this one, with the specified amount added.
       * The amount is typically {@link Period} but may be any other type implementing
       * the {@link TemporalAmount} interface.
       * <p>
       * The calculation is delegated to the amount object by calling
       * {@link TemporalAmount#addTo(Temporal)}. The amount implementation is free
       * to implement the addition in any way it wishes, however it typically
       * calls back to {@link #plus(long, TemporalUnit)}. Consult the documentation
       * of the amount implementation to determine if it can be successfully added.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param amountToAdd  the amount to add, not null
       * @return a {@code YearMonth} based on this year-month with the addition made, not null
       * @throws DateTimeException if the addition cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      YearMonth plus(TemporalAmount const& amountToAdd) const override;

      /**
       * Returns a copy of this year-month with the specified amount added.
       * <p>
       * This returns a @c YearMonth, based on this one, with the amount
       * in terms of the unit added. If it is not possible to add the amount, because the
       * unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * If the field is a @b TemporalUnit then the addition is implemented here.
       * The supported fields behave as follows:
       *
       * @li @c MONTHS -
       *  Returns a @c YearMonth with the specified number of months added.
       *  This is equivalent to @b plusMonths(long).
       * @li @c YEARS -
       *  Returns a @c YearMonth with the specified number of years added.
       *  This is equivalent to @b plusYears(long).
       * @li @c DECADES -
       *  Returns a @c YearMonth with the specified number of decades added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 10.
       * @li @c CENTURIES -
       *  Returns a @c YearMonth with the specified number of centuries added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 100.
       * @li @c MILLENNIA -
       *  Returns a @c YearMonth with the specified number of millennia added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 1,000.
       * @li @c ERAS -
       *  Returns a @c YearMonth with the specified number of eras added.
       *  Only two eras are supported so the amount must be one, zero or minus one.
       *  If the amount is non-zero then the year is changed such that the year-of-era
       *  is unchanged.
       *
       * </p>
       * <p>
       * All other @c TemporalUnit instances will throw an @c TemporalException.
       * </p>
       * @param amountToAdd  the amount of the unit to add to the result, may be negative
       * @param unit  the unit of the amount to add, not null
       * @return a @c YearMonth based on this year-month with the specified amount added, not null
       * @throws DateTimeException if the addition cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      YearMonth plus(glong amountToAdd, TemporalUnit unit) const;

      /**
       * Returns a copy of this @c YearMonth with the specified number of years added.
       *
       * @param yearsToAdd  the years to add, may be negative
       * @return a @c YearMonth based on this year-month with the years added, not null
       * @throws DateTimeException if the result exceeds the supported range
       */
      YearMonth plusYears(glong yearsToAdd) const;

      /**
       * Returns a copy of this @c YearMonth with the specified number of months added.
       *
       * @param monthsToAdd  the months to add, may be negative
       * @return a @c YearMonth based on this year-month with the months added, not null
       * @throws DateTimeException if the result exceeds the supported range
       */
      YearMonth plusMonths(glong monthsToAdd) const;

      /**
       * Returns a copy of this year-month with the specified amount subtracted.
       * <p>
       * This returns a {@code YearMonth}, based on this one, with the specified amount subtracted.
       * The amount is typically {@link Period} but may be any other type implementing
       * the {@link TemporalAmount} interface.
       * <p>
       * The calculation is delegated to the amount object by calling
       * {@link TemporalAmount#subtractFrom(Temporal)}. The amount implementation is free
       * to implement the subtraction in any way it wishes, however it typically
       * calls back to {@link #minus(long, TemporalUnit)}. Consult the documentation
       * of the amount implementation to determine if it can be successfully subtracted.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param amountToSubtract  the amount to subtract, not null
       * @return a {@code YearMonth} based on this year-month with the subtraction made, not null
       * @throws DateTimeException if the subtraction cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      YearMonth minus(TemporalAmount const& amountToSubtract) const override;

      /**
       * Returns a copy of this year-month with the specified amount subtracted.
       * <p>
       * This returns a @c YearMonth, based on this one, with the amount
       * in terms of the unit subtracted. If it is not possible to subtract the amount,
       * because the unit is not supported or for some other reason, an exception is thrown.
       * </p>
       * <p>
       * This method is equivalent to @b plus(long, TemporalUnit) with the amount negated.
       * See that method for a full description of how addition, and thus subtraction, works.
       * </p>
       * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
       * @param unit  the unit of the amount to subtract, not null
       * @return a @c YearMonth based on this year-month with the specified amount subtracted, not null
       * @throws DateTimeException if the subtraction cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      YearMonth minus(glong amountToSubtract, TemporalUnit unit) const;

      /**
       * Returns a copy of this @c YearMonth with the specified number of years subtracted.
       *
       * @param yearsToSubtract  the years to subtract, may be negative
       * @return a @c YearMonth based on this year-month with the years subtracted, not null
       * @throws DateTimeException if the result exceeds the supported range
       */
      YearMonth minusYears(glong yearsToSubtract) const;

      /**
       * Returns a copy of this @c YearMonth with the specified number of months subtracted.
       *
       * @param monthsToSubtract  the months to subtract, may be negative
       * @return a @c YearMonth based on this year-month with the months subtracted, not null
       * @throws DateTimeException if the result exceeds the supported range
       */
      YearMonth minusMonths(glong monthsToSubtract) const;

      /**
       * Queries this year-month using the specified query.
       * <p>
       * This queries this year-month using the specified query strategy object.
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
       * @return the query result, null may be returned (defined by the query)
       * @throws DateTimeException if unable to query (defined by the query)
       * @throws ArithmeticException if numeric overflow occurs (defined by the query)
       */
      Optional query(TemporalQuery const& query) const override;

      /**
       * Adjusts the specified temporal object to have this year-month.
       * <p>
       * This returns a temporal object of the same observable type as the input
       * with the year and month changed to be the same as this.
       * <p>
       * The adjustment is equivalent to using {@link Temporal#with(TemporalField, long)}
       * passing {@link ChronoField#PROLEPTIC_MONTH} as the field.
       * If the specified temporal object does not use the ISO calendar system then
       * a {@code DateTimeException} is thrown.
       * <p>
       * In most cases, it is clearer to reverse the calling pattern by using
       * {@link Temporal#with(TemporalAdjuster)}:
       * <pre>
       *   // these two lines are equivalent, but the second approach is recommended
       *   temporal = thisYearMonth.adjustInto(temporal);
       *   temporal = temporal.with(thisYearMonth);
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
       * Calculates the amount of time until another year-month in terms of the specified unit.
       * <p>
       * This calculates the amount of time between two @c YearMonth
       * objects in terms of a single @c TemporalUnit.
       * The start and end points are @c this and the specified year-month.
       * The result will be negative if the end is before the start.
       * The @c Temporal passed to this method is converted to a
       * @c YearMonth using @b from(Temporal).
       * For example, the amount in years between two year-months can be calculated
       * using @c startYearMonth.until(endYearMonth, YEARS).
       * </p>
       * <p>
       * The calculation returns a whole number, representing the number of
       * complete units between the two year-months.
       * For example, the amount in decades between 2012-06 and 2032-05
       * will only be one decade as it is one month short of two decades.
       * </p>
       * <p>
       * The calculation is implemented in this method for @b TemporalUnit.
       * The units @c MONTHS, @c YEARS, @c DECADES,
       * @c CENTURIES, @c MILLENNIA and @c ERAS are supported.
       * Other @c TemporalUnit values will throw an exception.
       * </p>
       * @param endExclusive  the end date, exclusive, which is converted to a @c YearMonth, not null
       * @param unit  the unit to measure the amount in, not null
       * @return the amount of time between this year-month and the end year-month
       * @throws DateTimeException if the amount cannot be calculated, or the end
       *  temporal cannot be converted to a @c YearMonth
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong until(Temporal const& endExclusive, TemporalUnit unit) const override;

      /**
       * Formats this year-month using the specified formatter.
       * <p>
       * This year-month will be passed to the formatter to produce a string.
       *
       * @param formatter  the formatter to use, not null
       * @return the formatted year-month string, not null
       * @throws DateTimeException if an error occurs during printing
       */
      String format(DateTimeFormatter const& formatter) const;

      /**
       * Combines this year-month with a day-of-month to create a @c LocalDate.
       * <p>
       * This returns a @c LocalDate formed from this year-month and the specified day-of-month.
       * </p>
       * <p>
       * The day-of-month value must be valid for the year-month.
       * </p>
       * @param dayOfMonth  the day-of-month to use, from 1 to 31
       * @return the date formed from this year-month and the specified day, not null
       * @throws DateTimeException if the day is invalid for the year-month
       * @see #isValidDay(int)
       */
      LocalDate atDay(gint dayOfMonth) const;

      /**
       * Returns a @c LocalDate at the end of the month.
       * <p>
       * This returns a @c LocalDate based on this year-month.
       * The day-of-month is set to the last valid day of the month, taking
       * into account leap years.
       * </p>
       *
       * @return the last valid date of this year-month, not null
       */
      LocalDate atEndOfMonth() const;

      /**
       * Compares this year-month to another year-month.
       * <p>
       * The comparison is based first on the value of the year, then on the value of the month.
       * It is "consistent with equals", as defined by @b Comparable.
       * </p>
       * @param other  the other year-month to compare to, not null
       * @return the comparator value, that is less than zero if this is before @c other,
       *          zero if they are equal, greater than zero if this is after @c other
       * @see isBefore
       * @see isAfter
       */
      gint compareTo(YearMonth const& other) const override;

      /**
       * Checks if this year-month is after the specified year-month.
       *
       * @param other  the other year-month to compare to, not null
       * @return true if this is after the specified year-month
       */
      gbool isAfter(YearMonth const& other) const;

      /**
       * Checks if this year-month is before the specified year-month.
       *
       * @param other  the other year-month to compare to, not null
       * @return true if this point is before the specified year-month
       */
      gbool isBefore(YearMonth const& other) const;

      /**
       * Checks if this year-month is equal to another year-month.
       * <p>
       * The comparison is based on the time-line position of the year-months.
       * </p>
       * @param other  the object to check, null returns false
       * @return true if this is equal to the other year-month
       */
      gbool equals(Object const& other) const override;

      /**
       * A hash code for this year-month.
       *
       * @return a suitable hash code
       */
      gint hash() const override;

      /**
       * Outputs this year-month as a @c String, such as @c 2007-12.
       * <p>
       * The output will be in the format @c uuuu-MM:
       * </p>
       * @return a string representation of this year-month, not null
       */
      String toString() const override;

      Object& clone() const override;

    private:
      /**
       * Returns a copy of this year-month with the new year and month, checking
       * to see if a new object is in fact required.
       *
       * @param newYear  the year to represent, validated from MIN_YEAR to MAX_YEAR
       * @param newMonth  the month-of-year to represent, validated not null
       * @return the year-month, not null
       */
      YearMonth with(gint newYear, gint newMonth) const;

      glong prolepticMonth() const;
    };
  } // time
} // core

#endif //CORE24_YEARMONTH_H
