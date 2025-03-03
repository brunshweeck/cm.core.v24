//
// Created by brunshweeck on 12/02/25.
//

#include "TemporalUtils.h"

#include <core/lang/ArithmeticException.h>
#include <core/lang/NumberFormatException.h>
#include <core/time/DateTimeParseException.h>
#include <core/time/DayOfWeek.h>
#include <core/time/Duration.h>
#include <core/time/Instant.h>
#include <core/time/LocalDate.h>
#include <core/time/Period.h>
#include <core/time/TemporalException.h>
#include <core/time/TemporalUnit.h>
#include <core/time/ValueRange.h>
#include <core/util/Optional.h>

#include <core/lang/Enum.h>

namespace core {
    namespace time {
        gbool TemporalUtils::Fields::isTimeBased(TemporalField field) {
            switch (field) {
                case TemporalField::NANO_OF_SECOND:
                case TemporalField::NANO_OF_DAY:
                case TemporalField::MICRO_OF_SECOND:
                case TemporalField::MICRO_OF_DAY:
                case TemporalField::MILLI_OF_SECOND:
                case TemporalField::MILLI_OF_DAY:
                case TemporalField::SECOND_OF_MINUTE:
                case TemporalField::SECOND_OF_DAY:
                case TemporalField::MINUTE_OF_HOUR:
                case TemporalField::MINUTE_OF_DAY:
                case TemporalField::HOUR_OF_AMPM:
                case TemporalField::CLOCK_HOUR_OF_AMPM:
                case TemporalField::HOUR_OF_DAY:
                case TemporalField::CLOCK_HOUR_OF_DAY:
                case TemporalField::AMPM_OF_DAY: return true;
                default: return false;
            }
        }

        gbool TemporalUtils::Fields::isDateBased(TemporalField field) {
            switch (field) {
                case TemporalField::DAY_OF_WEEK:
                case TemporalField::ALIGNED_DAY_OF_WEEK_IN_MONTH:
                case TemporalField::ALIGNED_DAY_OF_WEEK_IN_YEAR:
                case TemporalField::DAY_OF_MONTH:
                case TemporalField::DAY_OF_YEAR:
                case TemporalField::EPOCH_DAY:
                case TemporalField::ALIGNED_WEEK_OF_MONTH:
                case TemporalField::ALIGNED_WEEK_OF_YEAR:
                case TemporalField::MONTH_OF_YEAR:
                case TemporalField::PROLEPTIC_MONTH:
                case TemporalField::YEAR_OF_ERA:
                case TemporalField::YEAR:
                case TemporalField::ERA:
                case TemporalField::DAY_OF_QUARTER:
                case TemporalField::QUARTER_OF_YEAR:
                case TemporalField::WEEK_OF_WEEK_BASED_YEAR:
                case TemporalField::WEEK_BASED_YEAR:
                case TemporalField::JULIAN_DAY:
                case TemporalField::MODIFIED_JULIAN_DAY:
                case TemporalField::RATA_DIE: return true;
                default: return false;
            }
        }

        gint TemporalUtils::Fields::checkValue(gint value, TemporalField field) {
            if (range(field).isValidValue(value))
                return value;
            TemporalException("Invalid value for field "_Sl + field).throws($ftrace());
        }

        glong TemporalUtils::Fields::checkValue(glong value, TemporalField field) {
            if (range(field).isValidValue(value))
                return value;
            TemporalException("Invalid value for field "_Sl + field).throws($ftrace());
        }

        String TemporalUtils::Fields::toString(TemporalField field) {}

        ValueRange TemporalUtils::Fields::range(TemporalField field) {
            switch (field) {
                case TemporalField::NANO_OF_SECOND: return ValueRange::of(0, 999999999L);
                case TemporalField::NANO_OF_DAY: return ValueRange::of(0, 86400L * 1000000000L - 1);
                case TemporalField::MICRO_OF_SECOND: return ValueRange::of(0, 999999L);
                case TemporalField::MICRO_OF_DAY: return ValueRange::of(0, 86400L * 1000000L - 1);
                case TemporalField::MILLI_OF_SECOND: return ValueRange::of(0, 999L);
                case TemporalField::MILLI_OF_DAY: return ValueRange::of(0, 86400L * 1000L - 1);
                case TemporalField::SECOND_OF_MINUTE: return ValueRange::of(0, 59);
                case TemporalField::SECOND_OF_DAY: return ValueRange::of(0, 86400L - 1);
                case TemporalField::MINUTE_OF_HOUR: return ValueRange::of(0, 59);
                case TemporalField::MINUTE_OF_DAY: return ValueRange::of(0, (24 * 60) - 1);
                case TemporalField::HOUR_OF_AMPM: return ValueRange::of(0, 11);
                case TemporalField::CLOCK_HOUR_OF_AMPM: return ValueRange::of(1, 12);
                case TemporalField::HOUR_OF_DAY: return ValueRange::of(0, 23);
                case TemporalField::CLOCK_HOUR_OF_DAY: return ValueRange::of(1, 24);
                case TemporalField::AMPM_OF_DAY: return ValueRange::of(0, 1);
                case TemporalField::DAY_OF_WEEK: return ValueRange::of(1, 7);
                case TemporalField::ALIGNED_DAY_OF_WEEK_IN_MONTH: return ValueRange::of(1, 7);
                case TemporalField::ALIGNED_DAY_OF_WEEK_IN_YEAR: return ValueRange::of(1, 7);
                case TemporalField::DAY_OF_MONTH: return ValueRange::of(1, 28, 31);
                case TemporalField::DAY_OF_YEAR: return ValueRange::of(1, 365, 366);
                case TemporalField::EPOCH_DAY: return ValueRange::of(-365243219162L, 365241780471L);
                case TemporalField::ALIGNED_WEEK_OF_MONTH: return ValueRange::of(1, 4, 5);
                case TemporalField::ALIGNED_WEEK_OF_YEAR: return ValueRange::of(1, 53);
                case TemporalField::MONTH_OF_YEAR: return ValueRange::of(1, 12);
                case TemporalField::PROLEPTIC_MONTH: ValueRange::of(MIN_YEAR * 12L, MAX_YEAR * 12L + 11);
                case TemporalField::YEAR_OF_ERA: return ValueRange::of(1, MAX_YEAR, MAX_YEAR + 1);
                case TemporalField::YEAR: return ValueRange::of(MIN_YEAR, MAX_YEAR);
                case TemporalField::ERA: return ValueRange::of(0, 1);
                case TemporalField::INSTANT_SECONDS: return ValueRange::of(Instant::MIN.epochSecond(),
                                                                           Instant::MAX.epochSecond());
                case TemporalField::OFFSET_SECONDS: return ValueRange::of(-18 * 3600, 18 * 3600);
                case TemporalField::DAY_OF_QUARTER: return ValueRange::of(1, 90, 92);
                case TemporalField::QUARTER_OF_YEAR: return ValueRange::of(1, 4);
                case TemporalField::WEEK_OF_WEEK_BASED_YEAR: return ValueRange::of(1, 52, 53);
                case TemporalField::WEEK_BASED_YEAR: return range(TemporalField::YEAR);
                case TemporalField::JULIAN_DAY: return ValueRange::of(-365243219162L + 2440588L,
                                                                      365241780471L + 2440588L);
                case TemporalField::MODIFIED_JULIAN_DAY: return ValueRange::of(-365243219162L + 40587L,
                                                                               365241780471L + 40587L);
                case TemporalField::RATA_DIE: return ValueRange::of(-365243219162L + 719163L, 365241780471L + 719163L);
                default: return ValueRange::of(0, 0);
            }
        }

        static CORE_FAST gint QUARTER_DAYS[] = {0, 90, 181, 273, 0, 91, 182, 274};

        glong TemporalUtils::Fields::getDayOfQuarterFrom(TemporalAccessor const& temporal) {
            if (temporal.isSupported(TemporalField::DAY_OF_QUARTER))
                TemporalException("Unsupported field: DAY_OF_QUARTER").throws($ftrace());
            gint doy = temporal.get(TemporalField::DAY_OF_YEAR);
            gint moy = temporal.get(TemporalField::MONTH_OF_YEAR);
            glong year = temporal.get(TemporalField::YEAR);
            return doy - QUARTER_DAYS[((moy - 1) / 3) + (isLeapYear(year) ? 4 : 0)];
        }

        glong TemporalUtils::Fields::getQuarterOfYearFrom(TemporalAccessor const& temporal) {
            if (temporal.isSupported(TemporalField::QUARTER_OF_YEAR))
                TemporalException("Unsupported field: QUARTER_OF_YEAR").throws($ftrace());
            glong moy = temporal.get(TemporalField::MONTH_OF_YEAR);
            return (moy + 2) / 3;
        }

        glong TemporalUtils::Fields::getWeekOfWeekBasedYearFrom(TemporalAccessor const& temporal) {
            if (temporal.isSupported(TemporalField::WEEK_OF_WEEK_BASED_YEAR))
                TemporalException("Unsupported field: WEEK_OF_WEEK_BASED_YEAR").throws($ftrace());
            try {
                return getWeek(LocalDate::from(temporal));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong TemporalUtils::Fields::getWeekBasedYearFrom(TemporalAccessor const& temporal) {
            if (temporal.isSupported(TemporalField::WEEK_BASED_YEAR))
                TemporalException("Unsupported field: WEEK_BASED_YEAR").throws($ftrace());
            try {
                return getWeekBasedYear(LocalDate::from(temporal));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong TemporalUtils::Fields::getWeekRange(gint wby) {
            LocalDate date = LocalDate::of(wby, 1, 1);
            // 53 weeks if standard year starts on Thursday, or Wed in a leap year
            if (date.dayOfWeek() == DayOfWeek::THURSDAY ||
                (date.dayOfWeek() == DayOfWeek::WEDNESDAY && date.isLeapYear())) {
                return 53;
            }
            return 52;
        }

        ValueRange TemporalUtils::Fields::getWeekRange(LocalDate const& date) {
            int wby = getWeekBasedYear(date);
            return ValueRange::of(1, getWeekRange(wby));
        }

        glong TemporalUtils::Fields::getWeek(LocalDate const& date) {
            gint dow0 = (gint) date.dayOfWeek();
            gint doy0 = date.dayOfYear() - 1;
            gint doyThu0 = doy0 + (3 - dow0); // adjust to mid-week Thursday (which is 3 indexed from zero)
            gint alignedWeek = doyThu0 / 7;
            gint firstThuDoy0 = doyThu0 - (alignedWeek * 7);
            gint firstMonDoy0 = firstThuDoy0 - 3;
            if (firstMonDoy0 < -3) {
                firstMonDoy0 += 7;
            }
            if (doy0 < firstMonDoy0) {
                return (gint) getWeekRange(date.withDayOfYear(180).minusYears(1)).maximum();
            }
            gint week = ((doy0 - firstMonDoy0) / 7) + 1;
            if (week == 53) {
                if ((firstMonDoy0 == -3 || (firstMonDoy0 == -2 && date.isLeapYear())) == false) {
                    week = 1;
                }
            }
            return week;
        }

        glong TemporalUtils::Fields::getWeekBasedYear(LocalDate const& date) {
            gint year = date.year();
            gint doy = date.dayOfYear();
            if (doy <= 3) {
                gint dow = (gint) date.dayOfWeek();
                if (doy - dow < -2) {
                    year--;
                }
            } else if (doy >= 363) {
                gint dow = (gint) date.dayOfWeek();
                doy = doy - 363 - (date.isLeapYear() ? 1 : 0);
                if (doy - dow >= 0) {
                    year++;
                }
            }
            return year;
        }

        glong TemporalUtils::Fields::getJulianDayFrom(TemporalAccessor const& temporal) {
            return temporal.getLong(TemporalField::EPOCH_DAY) + 2440588L;
        }

        glong TemporalUtils::Fields::getModifiedJulianDayFrom(TemporalAccessor const& temporal) {
            return temporal.getLong(TemporalField::EPOCH_DAY) + 40587L;
        }

        glong TemporalUtils::Fields::getRataDieFrom(TemporalAccessor const& temporal) {
            return temporal.getLong(TemporalField::EPOCH_DAY) + 719163L;
        }

        gbool TemporalUtils::Fields::isSupportedBy(TemporalAccessor const& temporal, TemporalField field) {
            switch (field) {
                case TemporalField::DAY_OF_QUARTER:
                    return temporal.isSupported(TemporalField::DAY_OF_YEAR)
                            && temporal.isSupported(TemporalField::MONTH_OF_YEAR)
                            && temporal.isSupported(TemporalField::YEAR); // && Chronology::from(temporal).isISO();
                case TemporalField::QUARTER_OF_YEAR:
                    return temporal.isSupported(TemporalField::MONTH_OF_YEAR); // && Chronology::from(temporal).isISO();
                case TemporalField::WEEK_OF_WEEK_BASED_YEAR:
                    return temporal.isSupported(TemporalField::EPOCH_DAY); // && Chronology::from(temporal).isISO();
                case TemporalField::WEEK_BASED_YEAR:
                    return temporal.isSupported(TemporalField::EPOCH_DAY); // && Chronology::from(temporal).isISO();
                case TemporalField::JULIAN_DAY:
                case TemporalField::MODIFIED_JULIAN_DAY:
                case TemporalField::RATA_DIE:
                    return temporal.isSupported(TemporalField::EPOCH_DAY);
                default: return false;
            }
        }

        ValueRange TemporalUtils::Fields::range(TemporalAccessor const& temporal, TemporalField field) {
            try {
                if (temporal.isSupported(field))
                    return range(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
        }

        glong TemporalUtils::Fields::getFrom(TemporalAccessor const& temporal, TemporalField field) {
            if (temporal.isSupported(field)) {
                try {
                    switch (field) {
                        case TemporalField::DAY_OF_QUARTER: return getDayOfQuarterFrom(temporal);
                        case TemporalField::QUARTER_OF_YEAR: return getQuarterOfYearFrom(temporal);
                        case TemporalField::WEEK_OF_WEEK_BASED_YEAR: return getWeekOfWeekBasedYearFrom(temporal);
                        case TemporalField::WEEK_BASED_YEAR: return getWeekBasedYearFrom(temporal);
                        case TemporalField::JULIAN_DAY: return getJulianDayFrom(temporal);
                        case TemporalField::MODIFIED_JULIAN_DAY: return getModifiedJulianDayFrom(temporal);
                        case TemporalField::RATA_DIE: return getRataDieFrom(temporal);
                        default: break;
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                RuntimeException("Undefined operation for field: "_Sl + field).throws($ftrace());
            }
            TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
        }

        gbool TemporalUtils::Units::isTimeBased(TemporalUnit unit) {
            switch (unit) {
                case TemporalUnit::NANOS:
                case TemporalUnit::MICROS:
                case TemporalUnit::MILLIS:
                case TemporalUnit::SECONDS:
                case TemporalUnit::MINUTES:
                case TemporalUnit::HOURS:
                case TemporalUnit::HALF_DAYS: return true;
                default: return false;
            }
        }

        gbool TemporalUtils::Units::isDateBased(TemporalUnit unit) {
            switch (unit) {
                case TemporalUnit::DAYS:
                case TemporalUnit::WEEKS:
                case TemporalUnit::MONTHS:
                case TemporalUnit::YEARS:
                case TemporalUnit::DECADES:
                case TemporalUnit::CENTURIES:
                case TemporalUnit::MILLENNIA:
                case TemporalUnit::ERAS:
                case TemporalUnit::FOREVER:
                case TemporalUnit::WEEK_BASED_YEARS:
                case TemporalUnit::QUARTER_YEARS: return true;
                default: return false;
            }
        }

        String TemporalUtils::Units::toString(TemporalUnit unit) {
            return ((Enum<TemporalUnit>) unit).toString();
        }

        Duration TemporalUtils::Units::getDuration(TemporalUnit unit) {
            switch (unit) {
                case TemporalUnit::NANOS: return Duration::ofNanos(1);
                case TemporalUnit::MICROS: return Duration::ofNanos(1000);
                case TemporalUnit::MILLIS: return Duration::ofNanos(1000000);
                case TemporalUnit::SECONDS: return Duration::ofSeconds(1);
                case TemporalUnit::MINUTES: return Duration::ofSeconds(60);
                case TemporalUnit::HOURS: return Duration::ofSeconds(3600);
                case TemporalUnit::HALF_DAYS: return Duration::ofSeconds(43200);
                case TemporalUnit::DAYS: return Duration::ofSeconds(86400);
                case TemporalUnit::WEEKS: return Duration::ofSeconds(7 * 86400);
                case TemporalUnit::MONTHS: return Duration::ofSeconds(31556952L / 12);
                case TemporalUnit::YEARS: return Duration::ofSeconds(31556952L);
                case TemporalUnit::DECADES: return Duration::ofSeconds(31556952L * 10L);
                case TemporalUnit::CENTURIES: return Duration::ofSeconds(31556952L * 100L);
                case TemporalUnit::MILLENNIA: return Duration::ofSeconds(31556952L * 1000L);
                case TemporalUnit::ERAS: return Duration::ofSeconds(31556952L * 1000000000L);
                case TemporalUnit::FOREVER: return Duration::ofSeconds(Long::MAX_VALUE, 999999999);
                case TemporalUnit::WEEK_BASED_YEARS: return Duration::ofSeconds(31556952L);
                case TemporalUnit::QUARTER_YEARS: return Duration::ofSeconds(31556952L / 4);
                default: TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
            }
        }

        gbool TemporalUtils::Units::isSupportedBy(TemporalAccessor const& temporal, TemporalUnit unit) {
            switch (unit) {
                case TemporalUnit::WEEK_BASED_YEARS:
                case TemporalUnit::QUARTER_YEARS:
                    return temporal.isSupported(TemporalField::EPOCH_DAY); // && Chronology::from(temporal).isISO();
                default:
                    return false;
            }
        }

        TemporalUtils::Pattern& TemporalUtils::Durations::getPattern() {
            try {
                static Pattern PATTERN = Pattern::compile(
                    "([-+]?)P(?:([-+]?[0-9]+)D)?"
                    "(T(?:([-+]?[0-9]+)H)?(?:([-+]?[0-9]+)M)?(?:([-+]?[0-9]+)(?:[.,]([0-9]{0,9}))?S)?)?"_Sl,
                    Pattern::CASE_INSENSITIVE);

                return PATTERN;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool TemporalUtils::Durations::charMatch(CharSequence const& text, gint start, gint end, gchar c) {
            return (start >= 0 && end == start + 1 && text.charAt(start) == c);
        }

        gint TemporalUtils::Durations::parseNumber(CharSequence const& text, gint start, gint end, gint multiplier,
                                                   String const& errorText) {
            // regex limits to [-+]?[0-9]+
            if (start < 0 || end < 0) {
                return 0;
            }
            try {
                glong val = Long::parseLong(text, start, end, 10);
                return Math::multiplyExact(val, multiplier);
            } catch (Throwable const& ex) {
                DateTimeParseException("Text cannot be parsed to a Duration: "_Sl +
                                       errorText, text, 0, ex).throws($ftrace());
            }
        }

        gint TemporalUtils::Durations::parseFraction(CharSequence const& text, gint start, gint end, gint negate) {
            // regex limits to [0-9]{0,9}
            if (start < 0 || end < 0 || end - start == 0) {
                return 0;
            }
            try {
                gint fraction = Integer::parseInt(text, start, end, 10);

                // for number strings smaller than 9 digits, interpret as if there
                // were trailing zeros
                for (gint i = end - start; i < 9; i++) {
                    fraction *= 10;
                }
                return fraction * negate;
            } catch (Throwable const& ex) {
                DateTimeParseException("Text cannot be parsed to a Duration: fraction"_Sl,
                                       text, 0, ex).throws($ftrace());
            }
        }

        Duration TemporalUtils::Durations::parse(CharSequence const& text) {
            Matcher matcher = getPattern().matcher(text);
            if (matcher.matches()) {
                // check for letter T but no time sections
                if (!charMatch(text, matcher.start(3), matcher.end(3), 'T')) {
                    gbool negate = charMatch(text, matcher.start(1), matcher.end(1), '-');

                    gint dayStart = matcher.start(2), dayEnd = matcher.end(2);
                    gint hourStart = matcher.start(4), hourEnd = matcher.end(4);
                    gint minuteStart = matcher.start(5), minuteEnd = matcher.end(5);
                    gint secondStart = matcher.start(6), secondEnd = matcher.end(6);
                    gint fractionStart = matcher.start(7), fractionEnd = matcher.end(7);

                    if (dayStart >= 0 || hourStart >= 0 || minuteStart >= 0 || secondStart >= 0) {
                        try {
                            glong daysAsSecs = parseNumber(text, dayStart, dayEnd, LocalTime::SECONDS_PER_DAY, "days");
                            glong hoursAsSecs = parseNumber(text, hourStart, hourEnd, LocalTime::SECONDS_PER_HOUR,
                                                            "hours");
                            glong minsAsSecs = parseNumber(text, minuteStart, minuteEnd, LocalTime::SECONDS_PER_MINUTE,
                                                           "minutes");
                            glong seconds = parseNumber(text, secondStart, secondEnd, 1, "seconds");
                            gbool negativeSecs = secondStart >= 0 && text.charAt(secondStart) == '-';
                            gint nanos = parseFraction(text, fractionStart, fractionEnd, negativeSecs ? -1 : 1);
                            return create(negate, daysAsSecs, hoursAsSecs, minsAsSecs, seconds, nanos);
                        } catch (Throwable const& ex) {
                            DateTimeParseException("Text cannot be parsed to a Duration: overflow"_Sl,
                                                   text, 0, ex).throws($ftrace());
                        }
                    }
                }
            }
            DateTimeParseException("Text cannot be parsed to a Duration"_Sl, text, 0).throws($ftrace());
        }

        Duration TemporalUtils::Durations::create(gbool negate, glong daysAsSecs, glong hoursAsSecs, glong minsAsSecs,
                                                  glong secs, gint nanos) {
            try {
                glong seconds = Math::addExact(daysAsSecs,
                                               Math::addExact(hoursAsSecs,
                                                              Math::addExact(minsAsSecs, secs)));
                if (negate) {
                    return Duration::ofSeconds(seconds, nanos).negated();
                }
                return Duration::ofSeconds(seconds, nanos);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        TemporalUtils::Pattern& TemporalUtils::Periods::getPattern() {
            try {
                static Pattern PATTERN = Pattern::compile(
                    "([-+]?)P(?:([-+]?[0-9]+)Y)?(?:([-+]?[0-9]+)M)?(?:([-+]?[0-9]+)W)?(?:([-+]?[0-9]+)D)?"_Sl,
                    Pattern::CASE_INSENSITIVE);

                return PATTERN;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool TemporalUtils::Periods::charMatch(CharSequence const& text, gint start, gint end, gchar c) {
            return (start >= 0 && end == start + 1 && text.charAt(start) == c);
        }

        gint TemporalUtils::Periods::parseNumber(CharSequence const& text, gint start, gint end, gbool negate) {
            if (start < 0 || end < 0) {
                return 0;
            }
            gint val = Integer::parseInt(text, start, end, 10);
            try {
                return Math::multiplyExact(val, negate);
            } catch (ArithmeticException const& ex) {
                DateTimeParseException("Text cannot be parsed to a Period", text, 0, ex).throws($ftrace());
            }
        }

        Period TemporalUtils::Periods::parse(CharSequence const& text) {
            Matcher matcher = getPattern().matcher(text);
            if (matcher.matches()) {
                gint negate = (charMatch(text, matcher.start(1), matcher.end(1), '-') ? -1 : 1);
                gint yearStart = matcher.start(2), yearEnd = matcher.end(2);
                gint monthStart = matcher.start(3), monthEnd = matcher.end(3);
                gint weekStart = matcher.start(4), weekEnd = matcher.end(4);
                gint dayStart = matcher.start(5), dayEnd = matcher.end(5);
                if (yearStart >= 0 || monthStart >= 0 || weekStart >= 0 || dayStart >= 0) {
                    try {
                        gint years = parseNumber(text, yearStart, yearEnd, negate);
                        gint months = parseNumber(text, monthStart, monthEnd, negate);
                        gint weeks = parseNumber(text, weekStart, weekEnd, negate);
                        gint days = parseNumber(text, dayStart, dayEnd, negate);
                        days = Math::addExact(days, Math::multiplyExact(weeks, 7));
                        return Period::of(years, months, days);
                    } catch (NumberFormatException const& ex) {
                        DateTimeParseException("Text cannot be parsed to a Period", text, 0, ex).throws($ftrace());
                    }
                }
            }
            DateTimeParseException("Text cannot be parsed to a Period", text, 0).throws($ftrace());
        }

        gbool TemporalUtils::isLeapYear(gint year) {
            // A year that is a multiple of 100, 200 and 300 is not divisible by 16, but 400 is.
            // So for a year that's divisible by 4, checking that it's also divisible by 16
            // is sufficient to determine it must be a leap year.
            return (year & 15) == 0 ? (year & 3) == 0 : (year & 3) == 0 && year % 100 != 0;
        }

        gint TemporalUtils::lengthOfMonth(gint month, gbool isLeapYear) {
            switch (month) {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 12: return 31;
                case 2: return isLeapYear ? 29 : 28;
                default: return 30;
            }
        }

        gint TemporalUtils::firstDayOfMonthInYear(gint month, gbool isLeapYear) {
            gint adjustment = isLeapYear ? 1 : 0;
            switch (month) {
                case 1: return 1;
                case 2: return 32;
                case 3: return 60 + adjustment;
                case 4: return 91 + adjustment;
                case 5: return 121 + adjustment;
                case 6: return 152 + adjustment;
                case 7: return 182 + adjustment;
                case 8: return 213 + adjustment;
                case 9: return 244 + adjustment;
                case 10: return 374 + adjustment;
                case 11: return 305 + adjustment;
                case 12: return 335 + adjustment;
                default: return 0;
            }
        }
    } // time
} // core
