//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_LOCALDATE_H
#define CORE24_LOCALDATE_H

#include <core/ArithmeticException.h>
#include <core/time/Temporal.h>

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
        class LocalDate final : public virtual Temporal, public virtual Comparable<LocalDate> {
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

        private:
            /**
             * The year (1 + 30 bits), The month-of-year (4 bits), The day-of-month. (5 bits) = 40 bits
             * <p> +999999999-12-31 = 0..111011100110101100100111111111 1100 11111 </p>
             * <p> -999999999-01-01 = 1..111011100110101100100111111111 0001 00001 </p>
             */
            glong ymd = 0;

            enum {
                YEAR_OFFSET  = 9,
                MONTH_OFFSET = 5,
                DAY_OFFSET   = 0,
                YEAR_MASK    = 0x7FFFFFFE00,
                MONTH_MASK   = 0x1E0,
                DAY_MASK     = 0x1F,
            };

        public:
            /**
             * A month-of-year, such as 'July'.
             * <p>
             * @c Month is an enum representing the 12 months of the year -
             * January, February, March, April, May, June, July, August, September, October,
             * November and December.
             * </p>
             * <p>
             * In addition to the textual enum name, each month-of-year has an @c int value.
             * The @c int value follows normal usage and the ISO-8601 standard,
             * from 1 (January) to 12 (December). It is recommended that applications use the enum
             * rather than the @c int value to ensure code clarity.
             * </p>
             * <p>
             * This enum represents a common concept that is found in many calendar systems.
             * As such, this enum may be used by any calendar system that has the month-of-year
             * concept defined exactly equivalent to the ISO-8601 calendar system.
             * </p>
             * @note
             * This is an immutable and thread-safe enum.
             */
            enum Month {
                /**
                 * The singleton instance for the month of January with 31 days.
                 * This has the numeric value of @c 1.
                 */
                JANUARY = 1,
                /**
                 * The singleton instance for the month of February with 28 days, or 29 in a leap year.
                 * This has the numeric value of @c 2.
                 */
                FEBRUARY,
                /**
                 * The singleton instance for the month of March with 31 days.
                 * This has the numeric value of @c 3.
                 */
                MARCH,
                /**
                 * The singleton instance for the month of April with 30 days.
                 * This has the numeric value of @c 4.
                 */
                APRIL,
                /**
                 * The singleton instance for the month of May with 31 days.
                 * This has the numeric value of @c 5.
                 */
                MAY,
                /**
                 * The singleton instance for the month of June with 30 days.
                 * This has the numeric value of @c 6.
                 */
                JUNE,
                /**
                 * The singleton instance for the month of July with 31 days.
                 * This has the numeric value of @c 7.
                 */
                JULY,
                /**
                 * The singleton instance for the month of August with 31 days.
                 * This has the numeric value of @c 8.
                 */
                AUGUST,
                /**
                 * The singleton instance for the month of September with 30 days.
                 * This has the numeric value of @c 9.
                 */
                SEPTEMBER,
                /**
                 * The singleton instance for the month of October with 31 days.
                 * This has the numeric value of @c 10.
                 */
                OCTOBER,
                /**
                 * The singleton instance for the month of November with 30 days.
                 * This has the numeric value of @c 11.
                 */
                NOVEMBER,
                /**
                 * The singleton instance for the month of December with 31 days.
                 * This has the numeric value of @c 12.
                 */
                DECEMBER,
            };

            /**
             * A day-of-week, such as 'Tuesday'.
             * <p>
             * @c DayOfWeek is an enum representing the 7 days of the week -
             * Monday, Tuesday, Wednesday, Thursday, Friday, Saturday and Sunday.
             * </p>
             * <p>
             * In addition to the textual enum name, each day-of-week has an @c int value.
             * The @c int value follows the ISO-8601 standard, from 1 (Monday) to 7 (Sunday).
             * It is recommended that applications use the enum rather than the @c int value
             * to ensure code clarity.
             * </p>
             * <p>
             * This enum provides access to the localized textual form of the day-of-week.
             * Some locales also assign different numeric values to the days, declaring
             * Sunday to have the value 1, however this class provides no support for this.
             * See @b WeekFields for localized week-numbering.
             * </p>
             * <p>
             * This enum represents a common concept that is found in many calendar systems.
             * As such, this enum may be used by any calendar system that has the day-of-week
             * concept defined exactly equivalent to the ISO calendar system.
             * </p>
             * @note
             * This is an immutable and thread-safe enum.
             */
            enum DayOfWeek {
                /**
                 * The singleton instance for the day-of-week of Monday.
                 * This has the numeric value of @c 1.
                 */
                MONDAY = 1,
                /**
                 * The singleton instance for the day-of-week of Tuesday.
                 * This has the numeric value of @c 2.
                 */
                TUESDAY,
                /**
                 * The singleton instance for the day-of-week of Wednesday.
                 * This has the numeric value of @c 3.
                 */
                WEDNESDAY,
                /**
                 * The singleton instance for the day-of-week of Thursday.
                 * This has the numeric value of @c 4.
                 */
                THURSDAY,
                /**
                 * The singleton instance for the day-of-week of Friday.
                 * This has the numeric value of @c 5.
                 */
                FRIDAY,
                /**
                 * The singleton instance for the day-of-week of Saturday.
                 * This has the numeric value of @c 6.
                 */
                SATURDAY,
                /**
                 * The singleton instance for the day-of-week of Sunday.
                 * This has the numeric value of @c 7.
                 */
                SUNDAY,
            };

            //-----------------------------------------------------------------------
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

            //-----------------------------------------------------------------------
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
            CORE_EXPLICIT LocalDate(gint year, Month month, gint dayOfMonth);

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
            CORE_EXPLICIT LocalDate(gint year, gint month, gint dayOfMonth);

            //-----------------------------------------------------------------------
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
            CORE_EXPLICIT LocalDate(gint year, gint dayOfYear);

            //-----------------------------------------------------------------------
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

            //-----------------------------------------------------------------------
            /**
             * Checks if the specified field is supported.
             * <p>
             * This checks if this date can be queried for the specified field.
             * If false, then calling the @b get and @b with(ChronoField,long)
             * methods will throw an exception.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The supported fields are:
             *
             * @li @c DAY_OF_WEEK
             * @li @c ALIGNED_DAY_OF_WEEK_IN_MONTH
             * @li @c ALIGNED_DAY_OF_WEEK_IN_YEAR
             * @li @c DAY_OF_MONTH
             * @li @c DAY_OF_YEAR
             * @li @c EPOCH_DAY
             * @li @c ALIGNED_WEEK_OF_MONTH
             * @li @c ALIGNED_WEEK_OF_YEAR
             * @li @c MONTH_OF_YEAR
             * @li @c PROLEPTIC_MONTH
             * @li @c YEAR_OF_ERA
             * @li @c YEAR
             * @li @c ERA
             *
             * All other @c ChronoField instances will return false.
             * </p>
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this date, false if not
             */
            gbool isSupported(ChronoField field) const override;

            /**
             * Checks if the specified unit is supported.
             * <p>
             * This checks if the specified unit can be added to, or subtracted from, this date.
             * If false, then calling the <b> plus(long, ChronoUnit)</b> and
             * <b>minus(long, ChronoUnit) </b> methods will throw an exception.
             * </p>
             * <p>
             * If the unit is a @b ChronoUnit then the query is implemented here.
             * The supported units are:
             *
             * @li @c DAYS
             * @li @c WEEKS
             * @li @c MONTHS
             * @li @c YEARS
             * @li @c DECADES
             * @li @c CENTURIES
             * @li @c MILLENNIA
             * @li @c ERAS
             *
             * All other @c ChronoUnit instances will return false.
             * </p>
             * @param unit  the unit to check, null returns false
             * @return true if the unit can be added/subtracted, false if not
             */
            gbool isSupported(ChronoUnit unit) const override;

            /**
             * Gets the value of the specified field from this date as an @c int.
             * <p>
             * This queries this date for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The <b> supported fields</b> will return valid
             * values based on this date, except @c EPOCH_DAY and @c PROLEPTIC_MONTH
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
             * Gets the value of the specified field from this date as a @c long.
             * <p>
             * This queries this date for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The <b> supported fields</b> will return valid
             * values based on this date.
             * All other @c ChronoField instances will throw an @c TemporalException.
             * </p>
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong getLong(ChronoField field) const override;

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

            //-----------------------------------------------------------------------
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
             * If the field is a @b ChronoField then the adjustment is implemented here.
             * The supported fields behave as follows:
             *
             * @li @c DAY_OF_WEEK -
             *  Returns a @c LocalDate with the specified day-of-week.
             *  The date is adjusted up to 6 days forward or backward within the boundary
             *  of a Monday to Sunday week.
             * @li @c ALIGNED_DAY_OF_WEEK_IN_MONTH -
             *  Returns a @c LocalDate with the specified aligned-day-of-week.
             *  The date is adjusted to the specified month-based aligned-day-of-week.
             *  Aligned weeks are counted such that the first week of a given month starts
             *  on the first day of that month.
             *  This may cause the date to be moved up to 6 days into the following month.
             * @li @c ALIGNED_DAY_OF_WEEK_IN_YEAR -
             *  Returns a @c LocalDate with the specified aligned-day-of-week.
             *  The date is adjusted to the specified year-based aligned-day-of-week.
             *  Aligned weeks are counted such that the first week of a given year starts
             *  on the first day of that year.
             *  This may cause the date to be moved up to 6 days into the following year.
             * @li @c DAY_OF_MONTH -
             *  Returns a @c LocalDate with the specified day-of-month.
             *  The month and year will be unchanged. If the day-of-month is invalid for the
             *  year and month, then a @c DateTimeException is thrown.
             * @li @c DAY_OF_YEAR -
             *  Returns a @c LocalDate with the specified day-of-year.
             *  The year will be unchanged. If the day-of-year is invalid for the
             *  year, then a @c DateTimeException is thrown.
             * @li @c EPOCH_DAY -
             *  Returns a @c LocalDate with the specified epoch-day.
             *  This completely replaces the date and is equivalent to @b ofEpochDay(long).
             * @li @c ALIGNED_WEEK_OF_MONTH -
             *  Returns a @c LocalDate with the specified aligned-week-of-month.
             *  Aligned weeks are counted such that the first week of a given month starts
             *  on the first day of that month.
             *  This adjustment moves the date in whole week chunks to match the specified week.
             *  The result will have the same day-of-week as this date.
             *  This may cause the date to be moved into the following month.
             * @li @c ALIGNED_WEEK_OF_YEAR -
             *  Returns a @c LocalDate with the specified aligned-week-of-year.
             *  Aligned weeks are counted such that the first week of a given year starts
             *  on the first day of that year.
             *  This adjustment moves the date in whole week chunks to match the specified week.
             *  The result will have the same day-of-week as this date.
             *  This may cause the date to be moved into the following year.
             * @li @c MONTH_OF_YEAR -
             *  Returns a @c LocalDate with the specified month-of-year.
             *  The year will be unchanged. The day-of-month will also be unchanged,
             *  unless it would be invalid for the new month and year. In that case, the
             *  day-of-month is adjusted to the maximum valid value for the new month and year.
             * @li @c PROLEPTIC_MONTH -
             *  Returns a @c LocalDate with the specified proleptic-month.
             *  The day-of-month will be unchanged, unless it would be invalid for the new month
             *  and year. In that case, the day-of-month is adjusted to the maximum valid value
             *  for the new month and year.
             * @li @c YEAR_OF_ERA -
             *  Returns a @c LocalDate with the specified year-of-era.
             *  The era and month will be unchanged. The day-of-month will also be unchanged,
             *  unless it would be invalid for the new month and year. In that case, the
             *  day-of-month is adjusted to the maximum valid value for the new month and year.
             * @li @c YEAR -
             *  Returns a @c LocalDate with the specified year.
             *  The month will be unchanged. The day-of-month will also be unchanged,
             *  unless it would be invalid for the new month and year. In that case, the
             *  day-of-month is adjusted to the maximum valid value for the new month and year.
             * @li @c ERA -
             *  Returns a @c LocalDate with the specified era.
             *  The year-of-era and month will be unchanged. The day-of-month will also be unchanged,
             *  unless it would be invalid for the new month and year. In that case, the
             *  day-of-month is adjusted to the maximum valid value for the new month and year.
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
             * @return a @c LocalDate based on @c this with the specified field set
             * @throws DateTimeException if the field cannot be set
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalDate with(ChronoField field, glong newValue) const;

            //-----------------------------------------------------------------------
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
             * If the field is a @b ChronoUnit then the addition is implemented here.
             * The supported fields behave as follows:
             *
             * @li @c DAYS -
             *  Returns a @c LocalDate with the specified number of days added.
             *  This is equivalent to @b plusDays(long).
             * @li @c WEEKS -
             *  Returns a @c LocalDate with the specified number of weeks added.
             *  This is equivalent to @b plusWeeks(long) and uses a 7 days week.
             * @li @c MONTHS -
             *  Returns a @c LocalDate with the specified number of months added.
             *  This is equivalent to @b plusMonths(long).
             *  The day-of-month will be unchanged unless it would be invalid for the new
             *  month and year. In that case, the day-of-month is adjusted to the maximum
             *  valid value for the new month and year.
             * @li @c YEARS -
             *  Returns a @c LocalDate with the specified number of years added.
             *  This is equivalent to @b plusYears(long).
             *  The day-of-month will be unchanged unless it would be invalid for the new
             *  month and year. In that case, the day-of-month is adjusted to the maximum
             *  valid value for the new month and year.
             * @li @c DECADES -
             *  Returns a @c LocalDate with the specified number of decades added.
             *  This is equivalent to calling @b plusYears(long) with the amount
             *  multiplied by 10.
             *  The day-of-month will be unchanged unless it would be invalid for the new
             *  month and year. In that case, the day-of-month is adjusted to the maximum
             *  valid value for the new month and year.
             * @li @c CENTURIES -
             *  Returns a @c LocalDate with the specified number of centuries added.
             *  This is equivalent to calling @b plusYears(long) with the amount
             *  multiplied by 100.
             *  The day-of-month will be unchanged unless it would be invalid for the new
             *  month and year. In that case, the day-of-month is adjusted to the maximum
             *  valid value for the new month and year.
             * @li @c MILLENNIA -
             *  Returns a @c LocalDate with the specified number of millennia added.
             *  This is equivalent to calling @b plusYears(long) with the amount
             *  multiplied by 1,000.
             *  The day-of-month will be unchanged unless it would be invalid for the new
             *  month and year. In that case, the day-of-month is adjusted to the maximum
             *  valid value for the new month and year.
             * @li @c ERAS -
             *  Returns a @c LocalDate with the specified number of eras added.
             *  Only two eras are supported so the amount must be one, zero or minus one.
             *  If the amount is non-zero then the year is changed such that the year-of-era
             *  is unchanged.
             *  The day-of-month will be unchanged unless it would be invalid for the new
             *  month and year. In that case, the day-of-month is adjusted to the maximum
             *  valid value for the new month and year.
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
             * @return a @c LocalDate based on this date with the specified amount added
             * @throws DateTimeException if the addition cannot be made
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            LocalDate plus(glong amountToAdd, ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDate with the specified number of years added.
             * <p>
             * This method adds the specified amount to the years field in three steps:
             *
             * @li Add the input years to the year field
             * @li Check if the resulting date would be invalid
             * @li Adjust the day-of-month to the last valid day if necessary
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
             * This method adds the specified amount to the months field in three steps:
             *
             * @li Add the input months to the month-of-year field
             * @li Check if the resulting date would be invalid
             * @li Adjust the day-of-month to the last valid day if necessary
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

            //-----------------------------------------------------------------------

            /**
             * Returns a copy of this date with the specified amount subtracted.
             * <p>
             * This returns a @c LocalDate, based on this one, with the amount
             * in terms of the unit subtracted. If it is not possible to subtract the amount,
             * because the unit is not supported or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * This method is equivalent to <b>plus(long, ChronoUnit)</b> with the amount negated.
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
            LocalDate minus(glong amountToSubtract, ChronoUnit unit) const;

            //-----------------------------------------------------------------------
            /**
             * Returns a copy of this @c LocalDate with the specified number of years subtracted.
             * <p>
             * This method subtracts the specified amount from the years field in three steps:
             *
             * @li Subtract the input years from the year field
             * @li Check if the resulting date would be invalid
             * @li Adjust the day-of-month to the last valid day if necessary
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
             * This method subtracts the specified amount from the months field in three steps:
             *
             * @li Subtract the input months from the month-of-year field
             * @li Check if the resulting date would be invalid
             * @li Adjust the day-of-month to the last valid day if necessary
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
            LocalDate minusMonths(gint monthsToSubtract) const;

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
            LocalDate minusWeeks(gint weeksToSubtract) const;

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
             * Calculates the amount of time until another date in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two @c LocalDate
             * objects in terms of a single @c ChronoUnit.
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
             * The calculation is implemented in this method for @b ChronoUnit.
             * The units @c DAYS, @c WEEKS, @c MONTHS, @c YEARS,
             * @c DECADES, @c CENTURIES, @c MILLENNIA and @c ERAS
             * are supported. Other @c ChronoUnit values will throw an exception.
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
            glong until(const Temporal& endExclusive, ChronoUnit unit) const override;

            //-----------------------------------------------------------------------
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
            LocalDateTime atStartOfDay(ZoneId const& zone) const;

            //-----------------------------------------------------------------------
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

            //-----------------------------------------------------------------------
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
             * Queries this date using the specified query.
             * <p>
             * This queries this date using the specified query strategy object.
             * The {@code TemporalQuery} object defines the logic to be used to
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
            util::Optional<> query(TemporalQuery const& query) const override;

            /**
             * Obtains an instance of @c LocalDate from a temporal object.
             * <p>
             * This obtains a local date based on the specified temporal.
             * A {@code Temporal} represents an arbitrary set of date and time information,
             * which this factory converts to an instance of @c LocalDate.
             * <p>
             * The conversion uses the @em TemporalQuery::LOCAL_DATE query, which relies
             * on extracting the @b EPOCH_DAY field.
             * <p>
             * This method matches the signature of the functional interface @a TemporalQuery
             * allowing it to be used as a query via method reference, @c LocalDate::from.
             *
             * @param temporal  the temporal object to convert
             * @return the local date
             * @throws DateTimeException if unable to convert to a @c LocalDate
             */
            static LocalDate from(Temporal const& temporal);

            //-----------------------------------------------------------------------
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

            //-----------------------------------------------------------------------
            /**
             * Outputs this date as a @c String, such as @c 2007-12-03.
             * <p>
             * The output will be in the ISO-8601 format @c uuuu-MM-dd.
             * </p>
             * @return a string representation of this date
             */
            String toString() const override;

            Object& clone() const override;

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
             * @param prolepticYear  the ISO proleptic year to check
             * @return true if the year is leap, false otherwise
             */
            static gbool isLeapYear(gint prolepticYear);

            /**
             * Gets the length of specified month in days.
             * <p>
             * This takes a flag to determine whether to return the length for a leap year or not.
             * <p>
             * February has 28 days in a standard year and 29 days in a leap year.
             * April, June, September and November have 30 days.
             * All other months have 31 days.
             *
             * @param month the given month
             * @param leapYear  true if the length is required for a leap year
             * @return the length of this month in days, from 28 to 31
             */
            static gint lengthOfMonth(Month month, gbool leapYear);

        private:
            static glong dateToCompactInt(gint year, gint month, gint dayOfMonth);
        };
    } // util
} // core

#endif //CORE24_LOCALDATE_H
