//
// Created by admin on 03/01/25.
//

#ifndef CORE24_MONTHDAY_H
#define CORE24_MONTHDAY_H

#include <core/time/LocalDate.h>

namespace core {
    namespace time {
        /**
         * A month-day in the ISO-8601 calendar system, such as @c --12-03.
         * <p>
         * @c MonthDay is an immutable date-time object that represents the combination
         * of a month and day-of-month. Any field that can be derived from a month and day,
         * such as quarter-of-year, can be obtained.
         * <p>
         * This class does not store or represent a year, time or time-zone.
         * For example, the value "December 3rd" can be stored in a @c MonthDay.
         * <p>
         * Since a @c MonthDay does not possess a year, the leap day of
         * February 29th is considered valid.
         * <p>
         * This class implements @b Temporal rather than @b Temporal.
         * This is because it is not possible to define whether February 29th is valid or not
         * without external information, preventing the implementation of plus/minus.
         * Related to this, @c MonthDay only provides access to query and set the fields
         * @c MONTH_OF_YEAR and @c DAY_OF_MONTH.
         * <p>
         * The ISO-8601 calendar system is the modern civil calendar system used today
         * in most of the world. It is equivalent to the proleptic Gregorian calendar
         * system, in which today's rules for leap years are applied for all time.
         * For most applications written today, the ISO-8601 rules are entirely suitable.
         * However, any application that makes use of historical dates, and requires them
         * to be accurate will find the ISO-8601 approach unsuitable.
         */
        class MonthDay final : public virtual Temporal, public virtual Comparable<MonthDay> {
            /**
             * The month-of-year (4 bits), The day-of-month. (5 bits) = 9 bits
             * <p> December-31 = 1100 11111 </p>
             * <p> January-01 = 0001 00001 </p>
             */
            gshort md = 0;

            enum {
                MONTH_OFFSET = 5,
                DAY_OFFSET   = 0,
                MONTH_MASK   = 0x1E0,
                DAY_MASK     = 0x1F,
            };

        public:
            /**
             * Obtains the current month-day from the system clock in the default time-zone.
             * <p>
             * This will query the <b>system clock</b> in the default
             * time-zone to obtain the current month-day.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @return the current month-day using the system clock and default time-zone, not null
             */
            static MonthDay now();

            /**
             * Obtains the current month-day from the system clock in the specified time-zone.
             * <p>
             * This will query the <b>system clock</b> to obtain the current month-day.
             * Specifying the time-zone avoids dependence on the default time-zone.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             *
             * @param zone  the zone ID to use, not null
             * @return the current month-day using the system clock, not null
             */
            static MonthDay now(const ZoneId& zone);

            /**
             * Obtains an instance of @c MonthDay.
             * <p>
             * The day-of-month must be valid for the month within a leap year.
             * Hence, for February, day 29 is valid.
             * <p>
             * For example, passing in April and day 31 will throw an exception, as
             * there can never be April 31st in any year. By contrast, passing in
             * February 29th is permitted, as that month-day can sometimes be valid.
             *
             * @param month  the month-of-year to represent, not null
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @return the month-day, not null
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-month is invalid for the month
             */
            CORE_EXPLICIT MonthDay(LocalDate::Month month, gint dayOfMonth);

            /**
             * Obtains an instance of @c MonthDay.
             * <p>
             * The day-of-month must be valid for the month within a leap year.
             * Hence, for month 2 (February), day 29 is valid.
             * <p>
             * For example, passing in month 4 (April) and day 31 will throw an exception, as
             * there can never be April 31st in any year. By contrast, passing in
             * February 29th is permitted, as that month-day can sometimes be valid.
             *
             * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @return the month-day, not null
             * @throws DateTimeException if the value of any field is out of range,
             *  or if the day-of-month is invalid for the month
             */
            CORE_EXPLICIT MonthDay(gint month, gint dayOfMonth);

            /**
             * Obtains an instance of @c MonthDay from a temporal object.
             * <p>
             * This obtains a month-day based on the specified temporal.
             * A @c Temporal represents an arbitrary set of date and time information,
             * which this factory converts to an instance of @c MonthDay.
             * <p>
             * The conversion extracts the b MONTH_OF_YEAR and
             * @b DAY_OF_MONTH fields.
             * The extraction is only permitted if the temporal object has an ISO
             * chronology, or can be converted to a @c LocalDate.
             * <p>
             * This method matches the signature of the functional interface @b TemporalQuery
             * allowing it to be used as a query via method reference, @c MonthDay::from.
             *
             * @param temporal  the temporal object to convert, not null
             * @return the month-day, not null
             * @throws DateTimeException if unable to convert to a @c MonthDay
             */
            static MonthDay from(Temporal const& temporal);

            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this month-day can be queried for the specified field.
             * If false, then calling the
             * @b get methods will throw an exception.
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The supported fields are:
             * <ul>
             * <li> @c MONTH_OF_YEAR
             * <li> @c YEAR
             * </ul>
             * All other @c ChronoField instances will return false.
             *
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this month-day, false if not
             */
            gbool isSupported(ChronoField field) const override;

            /**
             * Gets the value of the specified field from this month-day as an @c int.
             * <p>
             * This queries this month-day for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The <b> supported fields</b> will return valid
             * values based on this month-day.
             * All other @c ChronoField instances will throw an @c TemporalException.
             *
             * @param field  the field to get, not null
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained or
             *         the value is outside the range of valid values for the field
             * @throws TemporalException if the field is not supported or
             *         the range of values exceeds an @c int
             * @throws ArithmeticException if numeric overflow occurs
             */
            gint get(ChronoField field) const override;

            /**
             * Gets the value of the specified field from this month-day as a @c long.
             * <p>
             * This queries this month-day for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The <b> supported fields</b> will return valid
             * values based on this month-day.
             * All other @c ChronoField instances will throw an @c TemporalException.
             *
             * @param field  the field to get, not null
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong getLong(ChronoField field) const override;

            /**
             * Gets the month-of-year field using the @c Month enum.
             * <p>
             * This method returns the enum @b Month for the month.
             * This avoids confusion as to what @c int values mean.
             *
             * @return the month-of-year, not null
             * @see #getMonthValue()
             */
            LocalDate::Month month() const;

            /**
             * Gets the day-of-month field.
             * <p>
             * This method returns the primitive @c int value for the day-of-month.
             *
             * @return the day-of-month, from 1 to 31
             */
            gint dayOfMonth() const;

            /**
             * Checks if the year is valid for this month-day.
             * <p>
             * This method checks whether this month and day and the input year form
             * a valid date. This can only return false for February 29th.
             *
             * @param year  the year to validate
             * @return true if the year is valid for this month-day
             * @see Year#isValidMonthDay(MonthDay)
             */
            gbool isValidYear(gint year) const;

            /**
             * Returns a copy of this @c MonthDay with the month-of-year altered.
             * <p>
             * This returns a month-day with the specified month.
             * If the day-of-month is invalid for the specified month, the day will
             * be adjusted to the last valid day-of-month.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param month  the month-of-year to set in the returned month-day, from 1 (January) to 12 (December)
             * @return a @c MonthDay based on this month-day with the requested month, not null
             * @throws DateTimeException if the month-of-year value is invalid
             */
            MonthDay withMonth(gint month) const;

            /**
             * Returns a copy of this @c MonthDay with the month-of-year altered.
             * <p>
             * This returns a month-day with the specified month.
             * If the day-of-month is invalid for the specified month, the day will
             * be adjusted to the last valid day-of-month.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param month  the month-of-year to set in the returned month-day, not null
             * @return a @c MonthDay based on this month-day with the requested month, not null
             */
            MonthDay withMonth(LocalDate::Month month) const;

            /**
             * Returns a copy of this @c MonthDay with the day-of-month altered.
             * <p>
             * This returns a month-day with the specified day-of-month.
             * If the day-of-month is invalid for the month, an exception is thrown.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param dayOfMonth  the day-of-month to set in the return month-day, from 1 to 31
             * @return a @c MonthDay based on this month-day with the requested day, not null
             * @throws DateTimeException if the day-of-month value is invalid,
             *  or if the day-of-month is invalid for the month
             */
            MonthDay withDayOfMonth(gint dayOfMonth) const;

            /**
             * Queries this month-day using the specified query.
             * <p>
             * This queries this month-day using the specified query strategy object.
             * The @c TemporalQuery object defines the logic to be used to
             * obtain the result. Read the documentation of the query to understand
             * what the result of this method will be.
             * <p>
             * The result of this method is obtained by invoking the
             * @b TemporalQuery::queryFrom(Temporal) method on the
             * specified query passing @c this as the argument.
             *
             * @param query  the query to invoke, not null
             * @return the query result, null may be returned (defined by the query)
             * @throws DateTimeException if unable to query (defined by the query)
             * @throws ArithmeticException if numeric overflow occurs (defined by the query)
             */
            util::Optional<> query(TemporalQuery const& query) const override;

            /**
             * Combines this month-day with a year to create a @c LocalDate.
             * <p>
             * This returns a @c LocalDate formed from this month-day and the specified year.
             * <p>
             * A month-day of February 29th will be adjusted to February 28th in the resulting
             * date if the year is not a leap year.
             * <p>
             * This instance is immutable and unaffected by this method call.
             *
             * @param year  the year to use, from MIN_YEAR to MAX_YEAR
             * @return the local date formed from this month-day and the specified year, not null
             * @throws DateTimeException if the year is outside the valid range of years
             */
            LocalDate atYear(gint year) const;

            /**
             * Compares this month-day to another month-day.
             * <p>
             * The comparison is based first on value of the month, then on the value of the day.
             * It is "consistent with equals", as defined by @b Comparable.
             *
             * @param other  the other month-day to compare to, not null
             * @return the comparator value, that is less than zero if this is before @c other,
             *          zero if they are equal, greater than zero if this is after @c other
             * @see #isBefore
             * @see #isAfter
             */
            gint compareTo(MonthDay const& other) const override;

            /**
             * Checks if this month-day is after the specified month-day.
             *
             * @param other  the other month-day to compare to, not null
             * @return true if this is after the specified month-day
             */
            gbool isAfter(MonthDay const& other) const;

            /**
             * Checks if this month-day is before the specified month-day.
             *
             * @param other  the other month-day to compare to, not null
             * @return true if this point is before the specified month-day
             */
            gbool isBefore(MonthDay const& other) const;

            /**
             * Checks if this month-day is equal to another month-day.
             * <p>
             * The comparison is based on the time-line position of the month-day within a year.
             *
             * @param other  the object to check, null returns false
             * @return true if this is equal to the other month-day
             */
            gbool equals(Object const& other) const override;

            /**
             * A hash code for this month-day.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            /**
             * Outputs this month-day as a @c String, such as @c --12-03.
             * <p>
             * The output will be in the format @c --MM-dd:
             *
             * @return a string representation of this month-day, not null
             */
            String toString() const override;

            Object& clone() const override;
        };
    } // time
} // core

#endif //CORE24_MONTHDAY_H
