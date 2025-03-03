//
// Created by brunshweeck on 24/02/25.
//

#ifndef CORE24_YEAR_H
#define CORE24_YEAR_H

#include "TemporalBase.h"

namespace core {
  namespace time {
    /**
     * A year in the ISO-8601 calendar system, such as @c 2007.
     * <p>
     * @c Year is an immutable date-time object that represents a year.
     * Any field that can be derived from a year can be obtained.
     * </p>
     * <p>
     * <b>Note that years in the ISO chronology only align with years in the
     * Gregorian-Julian system for modern years. Parts of Russia did not switch to the
     * modern Gregorian/ISO rules until 1920.
     * As such, historical years must be treated with caution.</b>
     * </p>
     * <p>
     * This class does not store or represent a month, day, time or time-zone.
     * For example, the value "2007" can be stored in a @c Year.
     * </p>
     * <p>
     * Years represented by this class follow the ISO-8601 standard and use
     * the proleptic numbering system. Year 1 is preceded by year 0, then by year -1.
     * <p>
     * The ISO-8601 calendar system is the modern civil calendar system used today
     * in most of the world. It is equivalent to the proleptic Gregorian calendar
     * system, in which today's rules for leap years are applied for all time.
     * For most applications written today, the ISO-8601 rules are entirely suitable.
     * However, any application that makes use of historical dates, and requires them
     * to be accurate will find the ISO-8601 approach unsuitable.
     * </p>
     */
    class Year final : public virtual TemporalBase<Year>,
                       public virtual TemporalAdjuster,
                       public virtual Comparable<Year> {
      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, Optional);

      /**
       * The year being represented.
       */
      gint year;

      /**
       * Constructor.
       *
       * @param year  the year to represent
       */
      Year(gint year);

    public:
      /**
       * The minimum supported year, '-999,999,999'.
       */
      static CORE_FAST gint MIN_VALUE = -999999999;

      /**
       * The maximum supported year, '+999,999,999'.
       */
      static CORE_FAST gint MAX_VALUE = +999999999;
      /**
       * Obtains the current year from the system clock in the default time-zone.
       * <p>
       * This will query the <b>system clock</b> in the default
       * time-zone to obtain the current year.
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       *
       * @return the current year using the system clock and default time-zone
       */
      static Year now();

      /**
       * Obtains the current year from the system clock in the specified time-zone.
       * <p>
       * This will query the <b>system clock</b> to obtain the current year.
       * Specifying the time-zone avoids dependence on the default time-zone.
       * <p>
       * Using this method will prevent the ability to use an alternate clock for testing
       * because the clock is hard-coded.
       *
       * @param zone  the zone ID to use
       * @return the current year using the system clock
       */
      static Year now(ZoneId const& zone);

      /**
       * Obtains an instance of @c Year.
       * <p>
       * This method accepts a year value from the proleptic ISO calendar system.
       * <p>
       * The year 2AD/CE is represented by 2.<br>
       * The year 1AD/CE is represented by 1.<br>
       * The year 1BC/BCE is represented by 0.<br>
       * The year 2BC/BCE is represented by -1.<br>
       *
       * @param isoYear  the ISO proleptic year to represent, from @c MIN_VALUE to @c MAX_VALUE
       * @return the year
       * @throws DateTimeException if the field is invalid
       */
      static Year of(gint isoYear);

      /**
       * Obtains an instance of @c Year from a temporal object.
       * <p>
       * This obtains a year based on the specified temporal.
       * A @c TemporalAccessor represents an arbitrary set of date and time information,
       * which this factory converts to an instance of @c Year.
       * <p>
       * The conversion extracts the @b year field.
       * The extraction is only permitted if the temporal object has an ISO
       * chronology, or can be converted to a @c LocalDate.
       * <p>
       * This method matches the signature of the functional interface @b TemporalQuery
       * allowing it to be used as a query via method reference, @c Year::from.
       *
       * @param temporal  the temporal object to convert
       * @return the year
       * @throws DateTimeException if unable to convert to a @c Year
       */
      static Year from(TemporalAccessor const& temporal);

      /**
       * Obtains an instance of @c Year from a text string such as @c 2019.
       * <p>
       * The string must represent a valid year.
       *
       * @param text  the text to parse such as "2007"
       * @return the parsed year
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static Year parse(CharSequence const& text);

      /**
       * Obtains an instance of @c Year from a text string using a specific formatter.
       * <p>
       * The text is parsed using the formatter, returning a year.
       *
       * @param text  the text to parse
       * @param formatter  the formatter to use
       * @return the parsed year
       * @throws DateTimeParseException if the text cannot be parsed
       */
      static Year parse(CharSequence const& text, DateTimeFormatter const& formatter);

      /**
       * Checks if the year is a leap year, according to the ISO proleptic
       * calendar system rules.
       * <p>
       * This method applies the current rules for leap years across the whole time-line.
       * In general, a year is a leap year if it is divisible by four without
       * remainder. However, years divisible by 100, are not leap years, with
       * the exception of years divisible by 400 which are.
       * <p>
       * For example, 1904 is a leap year it is divisible by 4.
       * 1900 was not a leap year as it is divisible by 100, however 2000 was a
       * leap year as it is divisible by 400.
       * <p>
       * The calculation is proleptic - applying the same rules into the far future and far past.
       * This is historically inaccurate, but is correct for the ISO-8601 standard.
       *
       * @param year  the year to check
       * @return true if the year is leap, false otherwise
       */
      static gbool isLeap(gint year);

      /**
       * Gets the year value.
       * <p>
       * The year returned by this method is proleptic as per @c get(YEAR).
       *
       * @return the year, @c MIN_VALUE to @c MAX_VALUE
       */
      gint get() const;

      /**
       * Checks if the specified field is supported.
       * <p>
       * This checks if this year can be queried for the specified field.
       * If false, then calling the @b range,
       * @b get and @b with(TemporalField, long)
       * methods will throw an exception.
       *
       * @param field  the field to check, null returns false
       * @return true if the field is supported on this year, false if not
       */
      gbool isSupported(TemporalField field) const override;

      /**
       * Checks if the specified unit is supported.
       * <p>
       * This checks if the specified unit can be added to, or subtracted from, this year.
       * If false, then calling the @b plus(long, TemporalUnit) and
       * @b minus(long, TemporalUnit) methods will throw an exception.
       *
       * @param unit  the unit to check, null returns false
       * @return true if the unit can be added/subtracted, false if not
       */
      gbool isSupported(TemporalUnit unit) const override;

      /**
       * Gets the range of valid values for the specified field.
       * <p>
       * The range object expresses the minimum and maximum valid values for a field.
       * This year is used to enhance the accuracy of the returned range.
       * If it is not possible to return the range, because the field is not supported
       * or for some other reason, an exception is thrown.
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The <b>supported fields</b> will return
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
       * Gets the value of the specified field from this year as an @c int.
       * <p>
       * This queries this year for the value of the specified field.
       * The returned value will always be within the valid range of values for the field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The <b>supported fields</b> will return valid
       * values based on this year.
       * All other @c TemporalField instances will throw an @c TemporalException.
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
       * Gets the value of the specified field from this year as a @c long.
       * <p>
       * This queries this year for the value of the specified field.
       * If it is not possible to return the value, because the field is not supported
       * or for some other reason, an exception is thrown.
       * <p>
       * If the field is a @b TemporalField then the query is implemented here.
       * The <b>supported fields</b> will return valid
       * values based on this year.
       * All other @c TemporalField instances will throw an @c TemporalException.
       *
       * @param field  the field to get
       * @return the value for the field
       * @throws DateTimeException if a value for the field cannot be obtained
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong getLong(TemporalField field) const override;

      /**
       * Checks if the year is a leap year, according to the ISO proleptic
       * calendar system rules.
       * <p>
       * This method applies the current rules for leap years across the whole time-line.
       * In general, a year is a leap year if it is divisible by four without
       * remainder. However, years divisible by 100, are not leap years, with
       * the exception of years divisible by 400 which are.
       * <p>
       * For example, 1904 is a leap year it is divisible by 4.
       * 1900 was not a leap year as it is divisible by 100, however 2000 was a
       * leap year as it is divisible by 400.
       * <p>
       * The calculation is proleptic - applying the same rules into the far future and far past.
       * This is historically inaccurate, but is correct for the ISO-8601 standard.
       *
       * @return true if the year is leap, false otherwise
       */
      gbool isLeap() const;

      /**
       * Checks if the month-day is valid for this year.
       * <p>
       * This method checks whether this year and the input month and day form
       * a valid date.
       *
       * @param monthDay  the month-day to validate, null returns false
       * @return true if the month and day are valid for this year
       */
      gbool isValidMonthDay(MonthDay const& monthDay) const;

      /**
       * Gets the length of this year in days.
       *
       * @return the length of this year in days, 365 or 366
       */
      gint length() const;

      /**
       * Returns an adjusted copy of this year.
       * <p>
       * This returns a @c Year, based on this one, with the year adjusted.
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
       * @return a @c Year based on @c this with the adjustment made
       * @throws DateTimeException if the adjustment cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      Year with(TemporalAdjuster const& adjuster) const override;

      /**
       * Returns a copy of this year with the specified field set to a new value.
       * <p>
       * This returns a @c Year, based on this one, with the value
       * for the specified field changed.
       * If it is not possible to set the value, because the field is not supported or for
       * some other reason, an exception is thrown.
       * <p>
       * If the field is a @b TemporalField then the adjustment is implemented here.
       * The supported fields behave as follows:
       * <ul>
       * <li>@c YEAR_OF_ERA -
       *  Returns a @c Year with the specified year-of-era
       *  The era will be unchanged.
       * <li>@c YEAR -
       *  Returns a @c Year with the specified year.
       *  This completely replaces the date and is equivalent to @b of(int).
       * <li>@c ERA -
       *  Returns a @c Year with the specified era.
       *  The year-of-era will be unchanged.
       * </ul>
       * <p>
       * In all cases, if the new value is outside the valid range of values for the field
       * then a @c DateTimeException will be thrown.
       * <p>
       * All other @c TemporalField instances will throw an @c TemporalException.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param field  the field to set in the result
       * @param newValue  the new value of the field in the result
       * @return a @c Year based on @c this with the specified field set
       * @throws DateTimeException if the field cannot be set
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      Year with(TemporalField field, glong newValue) const override;

      /**
       * Returns a copy of this year with the specified amount added.
       * <p>
       * This returns a @c Year, based on this one, with the specified amount added.
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
       * @return a @c Year based on this year with the addition made
       * @throws DateTimeException if the addition cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      Year plus(TemporalAmount const& amountToAdd) const override;

      /**
       * Returns a copy of this year with the specified amount added.
       * <p>
       * This returns a @c Year, based on this one, with the amount
       * in terms of the unit added. If it is not possible to add the amount, because the
       * unit is not supported or for some other reason, an exception is thrown.
       * <p>
       * If the field is a @b TemporalUnit then the addition is implemented here.
       * The supported fields behave as follows:
       * <ul>
       * <li>@c YEARS -
       *  Returns a @c Year with the specified number of years added.
       *  This is equivalent to @b plusYears(long).
       * <li>@c DECADES -
       *  Returns a @c Year with the specified number of decades added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 10.
       * <li>@c CENTURIES -
       *  Returns a @c Year with the specified number of centuries added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 100.
       * <li>@c MILLENNIA -
       *  Returns a @c Year with the specified number of millennia added.
       *  This is equivalent to calling @b plusYears(long) with the amount
       *  multiplied by 1,000.
       * <li>@c ERAS -
       *  Returns a @c Year with the specified number of eras added.
       *  Only two eras are supported so the amount must be one, zero or minus one.
       *  If the amount is non-zero then the year is changed such that the year-of-era
       *  is unchanged.
       * </ul>
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param amountToAdd  the amount of the unit to add to the result, may be negative
       * @param unit  the unit of the amount to add
       * @return a @c Year based on this year with the specified amount added
       * @throws DateTimeException if the addition cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      Year plus(glong amountToAdd, TemporalUnit unit) const override;

      /**
       * Returns a copy of this @c Year with the specified number of years added.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param yearsToAdd  the years to add, may be negative
       * @return a @c Year based on this year with the years added
       * @throws DateTimeException if the result exceeds the supported range
       */
      Year plusYears(glong yearsToAdd) const;

      /**
       * Returns a copy of this year with the specified amount subtracted.
       * <p>
       * This returns a @c Year, based on this one, with the specified amount subtracted.
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
       * @return a @c Year based on this year with the subtraction made
       * @throws DateTimeException if the subtraction cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      Year minus(TemporalAmount const& amountToSubtract) const override;

      /**
       * Returns a copy of this year with the specified amount subtracted.
       * <p>
       * This returns a @c Year, based on this one, with the amount
       * in terms of the unit subtracted. If it is not possible to subtract the amount,
       * because the unit is not supported or for some other reason, an exception is thrown.
       * <p>
       * This method is equivalent to @b plus(long, TemporalUnit) with the amount negated.
       * See that method for a full description of how addition, and thus subtraction, works.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
       * @param unit  the unit of the amount to subtract
       * @return a @c Year based on this year with the specified amount subtracted
       * @throws DateTimeException if the subtraction cannot be made
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      Year minus(glong amountToSubtract, TemporalUnit unit) const override;

      /**
       * Returns a copy of this @c Year with the specified number of years subtracted.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param yearsToSubtract  the years to subtract, may be negative
       * @return a @c Year based on this year with the year subtracted
       * @throws DateTimeException if the result exceeds the supported range
       */
      Year minusYears(glong yearsToSubtract) const;

      /**
       * Queries this year using the specified query.
       * <p>
       * This queries this year using the specified query strategy object.
       * The @c TemporalQuery object defines the logic to be used to
       * obtain the result. Read the documentation of the query to understand
       * what the result of this method will be.
       *
       *
       * @param query  the query to invoke
       * @return the query result, null may be returned (defined by the query)
       * @throws DateTimeException if unable to query (defined by the query)
       * @throws ArithmeticException if numeric overflow occurs (defined by the query)
       */
      Optional query(const TemporalQuery& query) const override;

      /**
       * Adjusts the specified temporal object to have this year.
       * <p>
       * This returns a temporal object of the same observable type as the input
       * with the year changed to be the same as this.
       * <p>
       * The adjustment is equivalent to using @b Temporal::with(TemporalField, long)
       * passing @b TemporalField::YEAR as the field.
       * If the specified temporal object does not use the ISO calendar system then
       * a @c DateTimeException is thrown.
       * <p>
       * In most cases, it is clearer to reverse the calling pattern by using
       * @b Temporal::with(TemporalAdjuster):
       * @code
       *   // these two lines are equivalent, but the second approach is recommended
       *   temporal = thisYear.adjustInto(temporal);
       *   temporal = temporal.with(thisYear);
       * @endcode
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param temporal  the target object to be adjusted
       * @return the adjusted object
       * @throws DateTimeException if unable to make the adjustment
       * @throws ArithmeticException if numeric overflow occurs
       */
      Temporal& adjustInto(Temporal const& temporal) const override;

      /**
       * Calculates the amount of time until another year in terms of the specified unit.
       * <p>
       * This calculates the amount of time between two @c Year
       * objects in terms of a single @c TemporalUnit.
       * The start and end points are @c this and the specified year.
       * The result will be negative if the end is before the start.
       * The @c Temporal passed to this method is converted to a
       * @c Year using @b from(TemporalAccessor).
       * For example, the amount in decades between two year can be calculated
       * using @c startYear.until(endYear, DECADES).
       * <p>
       * The calculation returns a whole number, representing the number of
       * complete units between the two years.
       * For example, the amount in decades between 2012 and 2031
       * will only be one decade as it is one year short of two decades.
       * <p>
       * The calculation is implemented in this method for @b TemporalUnit.
       * The units @c YEARS, @c DECADES, @c CENTURIES,
       * @c MILLENNIA and @c ERAS are supported.
       * Other @c TemporalUnit values will throw an exception.
       * <p>
       * This instance is immutable and unaffected by this method call.
       *
       * @param endExclusive  the end date, exclusive, which is converted to a @c Year
       * @param unit  the unit to measure the amount in
       * @return the amount of time between this year and the end year
       * @throws DateTimeException if the amount cannot be calculated, or the end
       *  temporal cannot be converted to a @c Year
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      glong until(Temporal const& endExclusive, TemporalUnit unit) const override;

      /**
       * Formats this year using the specified formatter.
       * <p>
       * This year will be passed to the formatter to produce a string.
       *
       * @param formatter  the formatter to use
       * @return the formatted year string
       * @throws DateTimeException if an error occurs during printing
       */
      String format(DateTimeFormatter const& formatter) const;

      /**
       * Combines this year with a day-of-year to create a @c LocalDate.
       * <p>
       * This returns a @c LocalDate formed from this year and the specified day-of-year.
       * <p>
       * The day-of-year value 366 is only valid in a leap year.
       *
       * @param dayOfYear  the day-of-year to use, from 1 to 365-366
       * @return the local date formed from this year and the specified date of year
       * @throws DateTimeException if the day of year is zero or less, 366 or greater or equal
       *  to 366 and this is not a leap year
       */
      LocalDate atDay(gint dayOfYear) const;

      /**
       * Combines this year with a month to create a @c YearMonth.
       * <p>
       * This returns a @c YearMonth formed from this year and the specified month.
       * All possible combinations of year and month are valid.
       * <p>
       * This method can be used as part of a chain to produce a date:
       * @code
       *  LocalDate date = year.atMonth(month).atDay(day);
       * @endcode
       *
       * @param month  the month-of-year to use
       * @return the year-month formed from this year and the specified month
       */
      YearMonth atMonth(Month month) const;

      /**
       * Combines this year with a month to create a @c YearMonth.
       * <p>
       * This returns a @c YearMonth formed from this year and the specified month.
       * All possible combinations of year and month are valid.
       * <p>
       * This method can be used as part of a chain to produce a date:
       * @code
       *  LocalDate date = year.atMonth(month).atDay(day);
       * @endcode
       *
       * @param month  the month-of-year to use, from 1 (January) to 12 (December)
       * @return the year-month formed from this year and the specified month
       * @throws DateTimeException if the month is invalid
       */
      YearMonth atMonth(gint month) const;

      /**
       * Combines this year with a month-day to create a @c LocalDate.
       * <p>
       * This returns a @c LocalDate formed from this year and the specified month-day.
       * <p>
       * A month-day of February 29th will be adjusted to February 28th in the resulting
       * date if the year is not a leap year.
       *
       * @param monthDay  the month-day to use
       * @return the local date formed from this year and the specified month-day
       */
      LocalDate atMonthDay(MonthDay const& monthDay) const;

      /**
       * Compares this year to another year.
       * <p>
       * The comparison is based on the value of the year.
       * It is "consistent with equals", as defined by @b Comparable.
       *
       * @param other  the other year to compare to
       * @return the comparator value, that is less than zero if this is before @c other,
       *          zero if they are equal, or greater than zero if this is after @c other
       * @see #isBefore
       * @see #isAfter
       */
      gint compareTo(Year const& other) const override;

      /**
       * Checks if this year is after the specified year.
       *
       * @param other  the other year to compare to
       * @return true if this is after the specified year
       */
      gbool isAfter(Year const& other) const;

      /**
       * Checks if this year is before the specified year.
       *
       * @param other  the other year to compare to
       * @return true if this point is before the specified year
       */
      gbool isBefore(Year const& other) const;

      /**
       * Checks if this year is equal to another year.
       * <p>
       * The comparison is based on the time-line position of the years.
       *
       * @param other  the object to check, null returns false
       * @return true if this is equal to the other year
       */
      gbool equals(Object const& other) const override;

      /**
       * A hash code for this year.
       *
       * @return a suitable hash code
       */
      gint hash() const override;

      /**
       * Outputs this year as a @c String.
       *
       * @return a string representation of this year
       */
      String toString() const override;
    };
  } // time
} // core

#endif //CORE24_YEAR_H
