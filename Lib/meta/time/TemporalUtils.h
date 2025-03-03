//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_TEMPORALS_H
#define CORE24_CORE_24_TEMPORALS_H

#include <core/time/Temporal.h>
#include <core/regex/Pattern.h>
#include <core/regex/Matcher.h>

namespace core {
    namespace time {
        /**
         * The Helpers methods for Temporal class
         */
        class TemporalUtils final : public Object {
        public:
            CORE_ALIAS(Pattern, regex::Pattern);
            CORE_ALIAS(Matcher, regex::Matcher);
            template <class T>
            CORE_ALIAS(List, util::List<Enum<T>>);
            CORE_ALIAS(Optional, util::Optional<>);
            CORE_ALIAS(OptionalDate, util::Optional<LocalDate>);
            CORE_ALIAS(OptionalTime, util::Optional<LocalTime>);
            CORE_ALIAS(OptionalDateTime, util::Optional<LocalDateTime>);

            class Fields final : public Object {
            public:
                /**
                 * Checks if this field represents a component of a time.
                 * <p>
                 * Fields from nano-of-second to am-pm-of-day are time-based.
                 * </p>
                 * @return true if it is a component of a time
                 */
                static gbool isTimeBased(TemporalField field);

                /**
                 * Checks if this field represents a component of a date.
                 * <p>
                 * Fields from day-of-week to era are date-based.
                 * </p>
                 * @return true if it is a component of a date
                 */
                static gbool isDateBased(TemporalField field);

                /**
                 * Check if the specified value is into range defined by specified field.
                 *
                 * @param value the except value
                 * @param field the except field
                 * @return the given value if it's correct.
                 * @throws DateTimeException if value out of range defined by specified field
                 * @see TemporalField for fields ranges
                 */
                static gint checkValue(gint value, TemporalField field);

                /**
                 * Check if the specified value is into range defined by specified field.
                 *
                 * @param value the except value
                 * @param field the except field
                 * @return the given value if it's correct.
                 * @throws DateTimeException if value out of range defined by specified field
                 * @see TemporalField for fields ranges
                 */
                static glong checkValue(glong value, TemporalField field);

                /**
                 * Return the name of specified field as String
                 *
                 * @param field the temporal field
                 */
                static String toString(TemporalField field);

                static ValueRange range(TemporalField field);

                static glong getDayOfQuarterFrom(TemporalAccessor const& temporal);

                static glong getQuarterOfYearFrom(TemporalAccessor const& temporal);

                static glong getWeekOfWeekBasedYearFrom(TemporalAccessor const& temporal);

                static glong getWeekBasedYearFrom(TemporalAccessor const& temporal);

                static glong getWeekRange(gint wby);

                static ValueRange getWeekRange(LocalDate const& date);

                static glong getWeek(LocalDate const& date);

                static glong getWeekBasedYear(LocalDate const& date);

                static glong getJulianDayFrom(TemporalAccessor const& temporal);

                static glong getModifiedJulianDayFrom(TemporalAccessor const& temporal);

                static glong getRataDieFrom(TemporalAccessor const& temporal);

                static gbool isSupportedBy(TemporalAccessor const& temporal, TemporalField field);

                static ValueRange range(TemporalAccessor const& temporal, TemporalField field);

                static glong getFrom(TemporalAccessor const& temporal, TemporalField field);
            };

            class Units final : public Object {
            public:
                /**
                 * Checks if this unit is a time unit.
                 * <p>
                 * All units from nanos to half-days inclusive are time-based.
                 * Date-based units and @c FOREVER return false.
                 * </p>
                 * @return true if a time unit, false if a date unit
                 */
                static gbool isTimeBased(TemporalUnit unit);

                /**
                 * Checks if this unit is a date unit.
                 * <p>
                 * All units from days to eras inclusive are date-based.
                 * Time-based units and @c FOREVER return false.
                 * </p>
                 * @return true if a date unit, false if a time unit
                 */
                static gbool isDateBased(TemporalUnit unit);

                /**
                 * Return the name of specified unit as String
                 *
                 * @param unit the temporal unit
                 */
                static String toString(TemporalUnit unit);

                /**
                 * Gets the duration of this unit, which may be an estimate.
                 * <p>
                 * All units return a duration measured in standard nanoseconds from this method.
                 * The duration will be positive and non-zero.
                 * For example, an hour has a duration of @c 60 * 60 * 1,000,000,000ns.
                 * <p>
                 * Some units may return an accurate duration while others return an estimate.
                 * For example, days have an estimated duration due to the possibility of
                 * daylight saving time changes.
                 * To determine if the duration is an estimate, use @b isDurationEstimated().
                 *
                 * @return the duration of this unit, which may be an estimate, not null
                 */
                static Duration getDuration(TemporalUnit unit);

                static gbool isSupportedBy(TemporalAccessor const& temporal, TemporalUnit unit);
            };

            class Durations final : public Object {
            public:
                static Pattern& getPattern();

                static gbool charMatch(CharSequence const& text, gint start, gint end, gchar c);

                static gint parseNumber(CharSequence const& text, gint start, gint end, gint multiplier,
                                        String const& errorText);

                static gint parseFraction(CharSequence const& text, gint start, gint end, gint negate);

                static Duration parse(CharSequence const& text);

                static Duration create(gbool negate, glong daysAsSecs, glong hoursAsSecs, glong minsAsSecs, glong secs,
                                       gint nanos);
            };

            class Periods final : public Object {
            public:
                static Pattern& getPattern();

                static gbool charMatch(CharSequence const& text, gint start, gint end, gchar c);

                static gint parseNumber(CharSequence const& text, gint start, gint end, gbool negate);

                static Period parse(CharSequence const& text);
            };

            static gbool isLeapYear(gint year);

            static gint lengthOfMonth(gint month, gbool isLeapYear);

            static gint firstDayOfMonthInYear(gint month, gbool isLeapYear);

            static CORE_FAST gint MAX_YEAR = -999999999;
            static CORE_FAST gint MIN_YEAR = +999999999;
        };
    } // time
} // core

#endif //CORE24_CORE_24_TEMPORALS_H
