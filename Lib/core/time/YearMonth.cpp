//
// Created by admin on 06/01/25.
//

#include "YearMonth.h"

#include <core/Enum.h>
#include <core/time/TemporalQuery.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;

    namespace time {
        YearMonth::YearMonth(gint year, LocalDate::Month month) {
            try {
                ym = toCompactInt(checkValue(year, YEAR), checkValue(month, MONTH_OF_YEAR));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth::YearMonth(gint year, gint month) {
            try {
                ym = toCompactInt(checkValue(year, YEAR), checkValue(month, MONTH_OF_YEAR));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth YearMonth::from(Temporal const& temporal) {
            if (Class<YearMonth>::hasInstance(temporal))
                return CORE_XCAST(YearMonth const, temporal);

            try {
                LocalDate date = LocalDate::from(temporal);
                return YearMonth(date.year(), date.month());
            } catch (DateTimeException const& ex) {
                DateTimeException("Unable to obtain MonthDay from Temporal: " +
                                  temporal + " of type " + typeName(temporal), ex).throws($ftrace());
            }
        }

        gbool YearMonth::isSupported(ChronoField field) const {
            return field == YEAR || field == MONTH_OF_YEAR ||
                    field == PROLEPTIC_MONTH || field == YEAR_OF_ERA || field == ERA;
        }

        gbool YearMonth::isSupported(ChronoUnit unit) const {
            return unit == MONTHS || unit == YEARS ||
                    unit == DECADES || unit == CENTURIES || unit == MILLENNIA || unit == ERAS;
        }

        gint YearMonth::get(ChronoField field) const {
            glong result = 0;
            try { result = getLong(field); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (result < Integer::MIN_VALUE || result > Integer::MAX_VALUE)
                DateTimeException("Value out of range").throws($ftrace());
            return (gint) result;
        }

        glong YearMonth::getLong(ChronoField field) const {
            switch (field) {
                case YEAR: return year();
                case MONTH_OF_YEAR: return month();
                case PROLEPTIC_MONTH: return year() * 12 + month() - 1;
                case YEAR_OF_ERA: return year();
                case ERA: return year() < 1 ? 0 : 1;
                default: break;
            }
            TemporalException("Unsupported field: "_Sl + Temporal::toString(field)).throws($ftrace());
        }

        gint YearMonth::year() const {
            return (gint) (ym >> YEAR_OFFSET & YEAR_MASK);
        }

        LocalDate::Month YearMonth::month() const {
            return (LocalDate::Month) (ym >> MONTH_OFFSET & MONTH_MASK);
        }

        gbool YearMonth::isLeapYear() const {
            return LocalDate::isLeapYear(year());
        }

        gbool YearMonth::isValidDay(gint dayOfMonth) const {
            return dayOfMonth >= 1 && dayOfMonth <= lengthOfMonth();
        }

        gint YearMonth::lengthOfMonth() const {
            return LocalDate::lengthOfMonth(month(), isLeapYear());
        }

        gint YearMonth::lengthOfYear() const {
            return isLeapYear() ? 366 : 365;
        }

        YearMonth YearMonth::with(ChronoField field, glong newValue) const {
            try {
                checkValue(newValue, field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            switch (field) {
                case MONTH_OF_YEAR: return withMonth((gint) newValue);
                case PROLEPTIC_MONTH: return withMonth(newValue - (year() * 12 + month() - 1));
                case YEAR_OF_ERA: return withYear((gint) (year() < 1 ? 1 - newValue : newValue));
                case YEAR: return withYear((gint) newValue);
                case ERA: return getLong(ERA) == newValue ? *this : withYear(1 - year());
                default: break;
            }
            TemporalException("Unsupported field: "_Sl + Temporal::toString(field)).throws($ftrace());
        }

        YearMonth YearMonth::withYear(gint year) const {
            try {
                checkValue(year, YEAR);
                return YearMonth(year, month());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth YearMonth::withMonth(gint month) const {
            try {
                checkValue(month, MONTH_OF_YEAR);
                return YearMonth(year(), month);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth YearMonth::plus(glong amountToAdd, ChronoUnit unit) const {
            try {
                switch (unit) {
                    case MONTHS: return plusMonths(amountToAdd);
                    case YEARS: return plusYears(amountToAdd);
                    case DECADES: return plusYears(Math::multiplyExact(amountToAdd, 10));
                    case CENTURIES: return plusYears(Math::multiplyExact(amountToAdd, 100));
                    case MILLENNIA: return plusYears(Math::multiplyExact(amountToAdd, 1000));
                    default: break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            TemporalException("Unsupported unit: "_Sl + Temporal::toString(unit)).throws($ftrace());
        }

        YearMonth YearMonth::plusYears(glong yearsToAdd) const {
            if (yearsToAdd == 0)
                return *this;
            try {
                gint newYear = checkValue(year() + yearsToAdd, YEAR);
                return YearMonth(newYear, month());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth YearMonth::plusMonths(glong monthsToAdd) const {
            if (monthsToAdd == 0)
                return *this;
            glong months = year() * 12 + month() - 1;
            glong cals = months + monthsToAdd;
            try {
                gint newYear = checkValue(Math::floorDiv(cals, 12), YEAR);
                gint newMonth = checkValue(Math::floorMod(cals, 12) + 1, MONTH_OF_YEAR);
                return YearMonth(newYear, newMonth);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth YearMonth::minus(glong amountToSubtract, ChronoUnit unit) const {
            try {
                return amountToSubtract == Long::MIN_VALUE
                           ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                           : plus(-amountToSubtract, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth YearMonth::minusYears(gint yearsToSubtract) const {
            try {
                return yearsToSubtract == Long::MIN_VALUE
                           ? plusYears(Long::MAX_VALUE).plusYears(1)
                           : plusYears(-yearsToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth YearMonth::minusMonths(gint monthsToSubtract) const {
            try {
                return monthsToSubtract == Long::MIN_VALUE
                           ? plusMonths(Long::MAX_VALUE).plusMonths(1)
                           : plusMonths(-monthsToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Optional<> YearMonth::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::PRECISION)
                return (Enum<ChronoUnit>) MONTHS;
            return Optional<>();
        }

        glong YearMonth::until(Temporal const& endExclusive, ChronoUnit unit) const {
            try {
                YearMonth end = from(endExclusive);
                gint monthUntil = (end.year() - year()) * 12 + (end.month() - month());
                switch (unit) {
                    case MONTHS: return monthUntil;
                    case YEARS: return monthUntil / 12;
                    case DECADES: return monthUntil / 120;
                    case CENTURIES: return monthUntil / 1200;
                    case MILLENNIA: return monthUntil / 12000;
                    case ERA: return end.getLong(ERA) - getLong(ERA);
                    default: break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            TemporalException("Unsupported unit: "_Sl + Temporal::toString(unit)).throws($ftrace());
        }

        LocalDate YearMonth::atDay(gint dayOfMonth) const {
            try {
                checkValue(dayOfMonth, DAY_OF_MONTH);
                return LocalDate(year(), month(), dayOfMonth);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate YearMonth::atEndOfMonth() const {
            try {
                return LocalDate(year(), month(), lengthOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint YearMonth::compareTo(YearMonth const& other) const {
            gint r = year() - other.year();
            if (r != 0)
                return r;
            return month() - other.month();
        }

        gbool YearMonth::isAfter(YearMonth const& other) const {
            return compareTo(other) > 0;
        }

        gbool YearMonth::isBefore(YearMonth const& other) const {
            return compareTo(other) < 0;
        }

        gbool YearMonth::equals(Object const& other) const {
            return this == &other ||
                    Class<YearMonth>::hasInstance(other) && ym == CORE_XCAST(YearMonth const, other).ym;
        }

        gint YearMonth::hash() const {
            return Long::hash(ym);
        }

        String YearMonth::toString() const {
            gint y = year();
            int absYear = Math::abs(y);
            XString buf = XString(9);
            if (absYear < 1000) {
                if (y < 0) {
                    buf.append(y - 10000).deleteCharAt(1);
                } else {
                    buf.append(y + 10000).deleteCharAt(0);
                }
            } else {
                buf.append(y);
            }
            gint m = month();
            return buf.append(m < 10 ? "-0" : "-")
                      .append(m)
                      .toString();
        }

        Object& YearMonth::clone() const {
            try {
                return UNSAFE::newInstance<YearMonth>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong YearMonth::toCompactInt(gint year, gint month) {
            glong date = 0;
            date |= (glong) Math::absExact(year) << YEAR_OFFSET & YEAR_MASK;
            date |= month << MONTH_OFFSET & MONTH_MASK;
            return year < 0 ? -date : date;
        }
    } // time
} // core
