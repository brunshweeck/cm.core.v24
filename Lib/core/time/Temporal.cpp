//
// Created by brunshweeck on 3 août 2024.
//

#include "Temporal.h"

#include <core/UnsupportedOperationException.h>
#include <core/time/TemporalQuery.h>
#include <core/util/Optional.h>

namespace core {
    namespace time {
        gbool Temporal::isSupported(ChronoUnit unit) const {
            return false;
        }

        gbool Temporal::isSupported(ChronoField field) const {
            return false;
        }

        gint Temporal::get(ChronoField field) const {
            if (!isSupported(field))
                TemporalException("Unsupported field: "_S + toString(field)).throws($ftrace());
            glong value;
            try {
                value = getLong(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (value < Integer::MIN_VALUE || Integer::MAX_VALUE < value)
                DateTimeException(String()).throws($ftrace());
            return CORE_CAST(gint, value);
        }

        glong Temporal::getLong(ChronoField field) const {
            if (!isSupported(field))
                TemporalException("Unsupported field: "_S + toString(field))
                        .throws($ftrace());
            UnsupportedOperationException().throws($ftrace());
        }

        glong Temporal::until(Temporal const& endExclusive, ChronoUnit unit) const {
            UnsupportedOperationException().throws($ftrace());
        }

        util::Optional<> Temporal::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::ZONE_ID ||
                query == TemporalQuery::CHRONOLOGY ||
                query == TemporalQuery::PRECISION) {
                return util::Optional<>();
            }
            try { return query.queryFrom(*this); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Temporal::checkValue(gint value, ChronoField field) {
            glong value2 = checkValue(CORE_CAST(glong, value), field);
            return CORE_CAST(gint, value2);
        }

        glong Temporal::checkValue(glong value, ChronoField field) {
            String fieldName;
            glong min = 0LL;
            glong max = 0LL;
            glong max2 = -1LL;
            switch (field) {
                case NANO_OF_SECOND:
                    min = 0;
                    max = 999999999LL;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case NANO_OF_DAY:
                    min = 0;
                    max = 86400LL * 1000000000LL - 1;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case MICRO_OF_SECOND:
                    min = 0;
                    max = 999999LL;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case MICRO_OF_DAY:
                    min = 0;
                    max = 86400LL * 1000000LL - 1;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case MILLI_OF_SECOND:
                    min = 0;
                    max = 999LL;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case MILLI_OF_DAY:
                    min = 0;
                    max = 86400LL * 1000LL - 1;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case SECOND_OF_MINUTE:
                    min = 0;
                    max = 59LL;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case SECOND_OF_DAY:
                    min = 0;
                    max = 86400LL - 1;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case MINUTE_OF_HOUR:
                    min = 0;
                    max = 59LL;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case MINUTE_OF_DAY:
                    min = 0;
                    max = 24 * 60 - 1;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case HOUR_OF_AMPM:
                    min = 0;
                    max = 11;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case CLOCK_HOUR_OF_AMPM:
                    min = 1;
                    max = 12;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case HOUR_OF_DAY:
                    min = 0;
                    max = 23;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case CLOCK_HOUR_OF_DAY:
                    min = 1;
                    max = 24;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case AMPM_OF_DAY:
                    min = 0;
                    max = 1;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case DAY_OF_WEEK:
                case ALIGNED_DAY_OF_WEEK_IN_MONTH:
                case ALIGNED_DAY_OF_WEEK_IN_YEAR:
                    min = 1;
                    max = 7;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case DAY_OF_MONTH:
                    min = 1;
                    max = 31;
                    max2 = 28;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case DAY_OF_YEAR:
                    min = 1;
                    max = 366;
                    max2 = 365;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case EPOCH_DAY:
                    min = -365243219162LL;
                    max = 365241780471LL;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case ALIGNED_WEEK_OF_MONTH:
                    min = 1;
                    max = 5;
                    max2 = 4;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case ALIGNED_WEEK_OF_YEAR:
                    min = 1;
                    max = 53;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case MONTH_OF_YEAR:
                    min = 1;
                    max = 12;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case PROLEPTIC_MONTH:
                    min = -999999999LL * 12LL;
                    max = +999999999LL * 12LL + 11;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case YEAR_OF_ERA:
                    min = 1;
                    max = 999999999LL + 1;
                    max2 = 999999999LL;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case YEAR:
                    min = -999999999LL;
                    max = +999999999LL;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case ERA:
                    min = 0;
                    max = 1;
                    if (value >= min && value <= max)
                        return value;
                    break;
                case INSTANT_SECONDS:
                    // min = Long::MIN_VALUE;
                    // max = Long::MAX_VALUE;
                    return value;
                case OFFSET_SECONDS:
                    min = -18LL * 3600;
                    max = +18LL * 3600;
                    if (value >= min && value <= max)
                        return value;
                    break;
            }
            if (max2 == -1)
                DateTimeException("Invalid value for "_S + toString(field) + " (valid values: "_S + min + " - "_S
                    + max + ")").throws($ftrace());
            DateTimeException("Invalid value for "_S + toString(field) + " (valid values: "_S + min + " - "_S
                + max2 + "/"_S + max + ")").throws($ftrace());
        }

        String Temporal::toString(ChronoUnit unit) {
            switch (unit) {
                case NANOS: return "Nanos"_S;
                case MICROS: return "Micros"_S;
                case MILLIS: return "Millis"_S;
                case SECONDS: return "Seconds"_S;
                case MINUTES: return "Minutes"_S;
                case HOURS: return "Hours"_S;
                case HALF_DAYS: return "HalfDays"_S;
                case DAYS: return "Days"_S;
                case WEEKS: return "Weeks"_S;
                case MONTHS: return "Months"_S;
                case YEARS: return "Years"_S;
                case DECADES: return "Decades"_S;
                case CENTURIES: return "Centuries"_S;
                case MILLENNIA: return "Millenia"_S;
                case ERAS: return "Eras"_S;
                case FOREVER: return "Forever"_S;
            }
            TemporalException("Unknown unit"_S).throws($ftrace());
        }

        String Temporal::toString(ChronoField field) {
            switch (field) {
                case NANO_OF_SECOND: return "NanoOfSecond"_S;
                case NANO_OF_DAY: return "NanoOfDay"_S;
                case MICRO_OF_SECOND: return "MicroOfSecond"_S;
                case MICRO_OF_DAY: return "MicroOfDay"_S;
                case MILLI_OF_SECOND: return "MilliOfSecond"_S;
                case MILLI_OF_DAY: return "MilliOfDay"_S;
                case SECOND_OF_MINUTE: return "SecondOfMinute"_S;
                case SECOND_OF_DAY: return "SecondOfDay"_S;
                case MINUTE_OF_HOUR: return "MinuteOfHour"_S;
                case MINUTE_OF_DAY: return "MinuteOfDay"_S;
                case HOUR_OF_AMPM: return "HourOfAmPm"_S;
                case CLOCK_HOUR_OF_AMPM: return "ClockHourOfAmPm"_S;
                case HOUR_OF_DAY: return "HourOfDay"_S;
                case CLOCK_HOUR_OF_DAY: return "ClockHourOfDay"_S;
                case AMPM_OF_DAY: return "AmPmOfDay"_S;
                case DAY_OF_WEEK: return "DayOfWeeck"_S;
                case ALIGNED_DAY_OF_WEEK_IN_MONTH: return "AlignedDayOfWeekInMonth"_S;
                case ALIGNED_DAY_OF_WEEK_IN_YEAR: return "AlignedDayOfWeekInYear"_S;
                case DAY_OF_MONTH: return "DayOfMonth"_S;
                case DAY_OF_YEAR: return "DayOfYear"_S;
                case EPOCH_DAY: return "EpochDay"_S;
                case ALIGNED_WEEK_OF_MONTH: return "AlignedWeekOfMonth"_S;
                case ALIGNED_WEEK_OF_YEAR: return "AlignedWeekOfYear"_S;
                case MONTH_OF_YEAR: return "MonthOfYear"_S;
                case PROLEPTIC_MONTH: return "ProlepticMonth"_S;
                case YEAR_OF_ERA: return "YearOfEra"_S;
                case YEAR: return "Year"_S;
                case ERA: return "Era"_S;
                case INSTANT_SECONDS: return "InstantSeconds"_S;
                case OFFSET_SECONDS: return "OffsetSeconds"_S;
            }
            TemporalException("Unknown field"_S).throws($ftrace());
        }

        String Temporal::displayMonth(gint value) {
            switch (value) {
                case 1: return "January"_Sl;
                case 2: return "February"_Sl;
                case 3: return "March"_Sl;
                case 4: return "April"_Sl;
                case 5: return "May"_Sl;
                case 6: return "June"_Sl;
                case 7: return "July"_Sl;
                case 8: return "August"_Sl;
                case 9: return "September"_Sl;
                case 10: return "October"_Sl;
                case 11: return "November"_Sl;
                case 12: return "December"_Sl;
                default: return String();
            }
        }

        gbool Temporal::isTimeBased(ChronoUnit unit) {
            return unit < DAYS;
        }

        gbool Temporal::isTimeBased(ChronoField field) {
            return field < DAY_OF_WEEK;
        }

        gbool Temporal::isDateBased(ChronoUnit unit) {
            return unit >= DAYS && unit != FOREVER;
        }

        gbool Temporal::isDateBased(ChronoField field) {
            return field >= DAY_OF_WEEK && field <= ERA;
        }
    } // time
} // core
