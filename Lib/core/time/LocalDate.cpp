//
// Created by brunshweeck on 3 août 2024.
//

#include "LocalDate.h"

#include <core/Enum.h>
#include <core/XString.h>
#include <core/misc/Unsafe.h>
#include <core/time/LocalDateTime.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZoneId.h>
#include <core/time/ZoneOffsetTransition.h>
#include <core/time/ZoneRules.h>
#include <core/util/Optional.h>

static gint FIRST_DAY_OF_MONTH_IN_YEAR[] = {0, 1, 32, 60, 91, 121, 152, 182, 213, 244, 305, 355};

static gint FIRST_DAY_OF_MONTH_IN_LEAP_YEAR[] = {0, 1, 32, 61, 92, 122, 153, 183, 214, 245, 306, 356};

static gint MONTH_LENGTH_IN_LEAP_YEAR[] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static gint MONTH_LENGTH_IN_YEAR[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

namespace core {
    using namespace util;

    namespace time {
        LocalDate const LocalDate::MIN = LocalDate(MIN_YEAR, JANUARY, 01);
        LocalDate const LocalDate::MAX = LocalDate(MAX_YEAR, DECEMBER, 31);
        LocalDate const LocalDate::EPOCH = LocalDate(1970, JANUARY, 01);

        LocalDate::LocalDate(gint year, Month month, gint dayOfMonth) {
            try {
                ymd = dateToCompactInt(
                    checkValue(year, YEAR),
                    checkValue(month, DAY_OF_MONTH),
                    checkValue(dayOfMonth, DAY_OF_MONTH)
                );
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate::LocalDate(gint year, gint month, gint dayOfMonth) {
            try {
                ymd = dateToCompactInt(
                    checkValue(year, YEAR),
                    checkValue(month, DAY_OF_MONTH),
                    checkValue(dayOfMonth, DAY_OF_MONTH)
                );
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate::LocalDate(gint year, gint dayOfYear) {
            try { checkValue(year, YEAR); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            try { checkValue(dayOfYear, DAY_OF_YEAR); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gbool isLeap = isLeapYear(year);
            if (dayOfYear == 366 && !isLeap) {
                DateTimeException("Invalid date 'DayOfYear 366' as '"_Sl +
                    year + "' is not a leap year").throws($ftrace());
            }
            gint monthOfYear = (dayOfYear - 1) / 31 + 1;
            gint monthEnd = 0;
            gint firstDayOfMonth = 0;
            gint monthLength = 0;
            if (isLeap) {
                firstDayOfMonth = FIRST_DAY_OF_MONTH_IN_LEAP_YEAR[monthOfYear];
                monthLength = MONTH_LENGTH_IN_LEAP_YEAR[monthOfYear];
            } else {
                firstDayOfMonth = FIRST_DAY_OF_MONTH_IN_YEAR[monthOfYear];
                monthLength = MONTH_LENGTH_IN_YEAR[monthOfYear];
            }
            monthEnd = firstDayOfMonth + monthLength - 1;
            if (dayOfYear > monthEnd) {
                monthOfYear = monthOfYear == 12 ? 1 : monthOfYear + 1;
                if (isLeap)
                    firstDayOfMonth = FIRST_DAY_OF_MONTH_IN_LEAP_YEAR[monthOfYear];
                else
                    firstDayOfMonth = FIRST_DAY_OF_MONTH_IN_YEAR[monthOfYear];
            }
            gint dayOfMonth = dayOfYear - firstDayOfMonth + 1;

            try {
                ymd = dateToCompactInt(year, monthOfYear, dayOfMonth);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::ofEpochDay(glong epochDay) {
            checkValue(epochDay, EPOCH_DAY);
            glong zeroDay = epochDay + DAYS_0000_TO_1970;
            // find the march-based year
            zeroDay -= 60; // adjust to 0000-03-01 so leap day is at end of four years cycle
            glong adjust = 0;
            if (zeroDay < 0) {
                // adjust negative years to positive for calculation
                glong adjustCycles = (zeroDay + 1) / DAYS_PER_CYCLE - 1;
                adjust = adjustCycles * 400;
                zeroDay += -adjustCycles * DAYS_PER_CYCLE;
            }
            glong yearEst = (400 * zeroDay + 591) / DAYS_PER_CYCLE;
            glong doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
            if (doyEst < 0) {
                // fix estimate
                yearEst--;
                doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
            }
            yearEst += adjust; // reset any negative year
            gint marchDoy0 = CORE_CAST(gint, doyEst);

            // convert march-based values back to january-based
            gint marchMonth0 = (marchDoy0 * 5 + 2) / 153;
            gint month = marchMonth0 + 3;
            if (month > 12) {
                month -= 12;
            }
            gint dom = marchDoy0 - (marchMonth0 * 306 + 5) / 10 + 1;
            if (marchDoy0 >= 306) {
                yearEst++;
            }

            return LocalDate(yearEst, month, dom);
        }

        gbool LocalDate::isSupported(ChronoField field) const {
            return isDateBased(field);
        }

        gbool LocalDate::isSupported(ChronoUnit unit) const {
            return isDateBased(unit);
        }

        gint LocalDate::get(ChronoField field) const {
            switch (field) {
                case DAY_OF_WEEK: return dayOfWeek();
                case ALIGNED_DAY_OF_WEEK_IN_MONTH: return (dayOfMonth() - 1) % 7 + 1;
                case ALIGNED_DAY_OF_WEEK_IN_YEAR: return (dayOfYear() - 1) % 7 + 1;
                case DAY_OF_MONTH: return dayOfMonth();
                case DAY_OF_YEAR: return dayOfYear();
                case EPOCH_DAY: goto RESULT_OVERFLOW;
                case ALIGNED_WEEK_OF_MONTH: return (dayOfMonth() - 1) / 7 + 1;
                case ALIGNED_WEEK_OF_YEAR: return (dayOfYear() - 1) / 7 + 1;
                case MONTH_OF_YEAR: return month();
                case PROLEPTIC_MONTH: goto RESULT_OVERFLOW;
                case YEAR_OF_ERA: return year() >= 1 ? year() : 1 - year();
                case YEAR: return year();
                case ERA: return year() >= 1 ? 1 : 0;
                default: break;
            }
            TemporalException("Unsupported field "_S + Temporal::toString(field)).throws($ftrace());
        RESULT_OVERFLOW:
            TemporalException("Value of field "_S + Temporal::toString(field)
                + " exceed implementation limit"_S).throws($ftrace());
        }

        glong LocalDate::getLong(ChronoField field) const {
            switch (field) {
                case DAY_OF_WEEK: return dayOfWeek();
                case ALIGNED_DAY_OF_WEEK_IN_MONTH: return (dayOfMonth() - 1) % 7 + 1;
                case ALIGNED_DAY_OF_WEEK_IN_YEAR: return (dayOfYear() - 1) % 7 + 1;
                case DAY_OF_MONTH: return dayOfMonth();
                case DAY_OF_YEAR: return dayOfYear();
                case EPOCH_DAY: return toEpochDay();
                case ALIGNED_WEEK_OF_MONTH: return (dayOfMonth() - 1) / 7 + 1;
                case ALIGNED_WEEK_OF_YEAR: return (dayOfYear() - 1) / 7 + 1;
                case MONTH_OF_YEAR: return month();
                case PROLEPTIC_MONTH: return year() * 12 + month() - 1;
                case YEAR_OF_ERA: return year() >= 1 ? year() : 1 - year();
                case YEAR: return year();
                case ERA: return year() >= 1 ? 1 : 0;
                default: break;
            }
            TemporalException("Unsupported field "_S + Temporal::toString(field)).throws($ftrace());
        }

        gint LocalDate::year() const {
            return (ymd < 0 ? -1 : 1) * CORE_CAST(gint, (ymd & YEAR_MASK) >> YEAR_OFFSET);
        }

        LocalDate::Month LocalDate::month() const {
            return (Month) ((ymd & MONTH_MASK) >> MONTH_OFFSET);
        }

        gint LocalDate::dayOfMonth() const {
            return (gint) (ymd & DAY_MASK) >> DAY_OFFSET;
        }

        gint LocalDate::dayOfYear() const {
            if (isLeapYear())
                return FIRST_DAY_OF_MONTH_IN_LEAP_YEAR[month()] + dayOfMonth() - 1;
            return FIRST_DAY_OF_MONTH_IN_YEAR[month()] + dayOfMonth() - 1;
        }

        LocalDate::DayOfWeek LocalDate::dayOfWeek() const {
            return (DayOfWeek) Math::floorMod(toEpochDay() + 3, 7);
        }

        gbool LocalDate::isLeapYear() const {
            return isLeapYear(year());
        }

        gint LocalDate::lengthOfMonth() const {
            return lengthOfMonth(month(), isLeapYear());
        }

        gint LocalDate::lengthOfYear() const {
            return isLeapYear() ? 366 : 365;
        }

        LocalDate LocalDate::with(ChronoField field, glong newValue) const {
            checkValue(newValue, field);
            switch (field) {
                case DAY_OF_WEEK: return plusDays(newValue - dayOfWeek());
                case ALIGNED_DAY_OF_WEEK_IN_MONTH: return plusDays(newValue - getLong(ALIGNED_DAY_OF_WEEK_IN_MONTH));
                case ALIGNED_DAY_OF_WEEK_IN_YEAR: return plusDays(newValue - getLong(ALIGNED_DAY_OF_WEEK_IN_YEAR));
                case DAY_OF_MONTH: return withDayOfMonth(CORE_CAST(gint, newValue));
                case DAY_OF_YEAR: return withDayOfYear(CORE_CAST(gint, newValue));
                case EPOCH_DAY: return ofEpochDay(newValue);
                case ALIGNED_WEEK_OF_MONTH: return plusWeeks(newValue - getLong(ALIGNED_WEEK_OF_MONTH));
                case ALIGNED_WEEK_OF_YEAR: return plusWeeks(newValue - getLong(ALIGNED_WEEK_OF_YEAR));
                case MONTH_OF_YEAR: return withMonth(CORE_CAST(gint, newValue));
                case PROLEPTIC_MONTH: return plusMonths(newValue - getLong(PROLEPTIC_MONTH));
                case YEAR_OF_ERA: return withYear(CORE_CAST(gint, year() >= 1 ? newValue : 1 - newValue));
                case YEAR: return withYear(CORE_CAST(gint, newValue));
                case ERA: return getLong(ERA) == newValue ? *this : withYear(1 - year());
                default: break;
            }
            TemporalException("Unsupported field: "_S + Temporal::toString(field)).throws($ftrace());
        }

        LocalDate LocalDate::withYear(gint year) const {
            try {
                return LocalDate(checkValue(year, YEAR), month(), dayOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::withMonth(gint month) const {
            try {
                return LocalDate(year(), checkValue(month, MONTH_OF_YEAR), dayOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::withDayOfMonth(gint dayOfMonth) const {
            try {
                return LocalDate(year(), month(), checkValue(dayOfMonth, DAY_OF_MONTH));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::withDayOfYear(gint dayOfYear) const {
            try {
                return LocalDate(year(), checkValue(dayOfYear, DAY_OF_YEAR));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::plus(glong amountToAdd, ChronoUnit unit) const {
            switch (unit) {
                case DAYS:
                    try { return plusDays(amountToAdd); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case WEEKS:
                    try { return plusWeeks(amountToAdd); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MONTHS:
                    try { return plusMonths(amountToAdd); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case YEARS:
                    try { return plusYears(amountToAdd); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case DECADES:
                    try {
                        return plusYears(Math::multiplyExact(amountToAdd, 10));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case CENTURIES:
                    try {
                        return plusYears(Math::multiplyExact(amountToAdd, 100));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MILLENNIA:
                    try {
                        return plusYears(Math::multiplyExact(amountToAdd, 1000));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case ERAS:
                    try {
                        return with(ERA, Math::addExact(getLong(ERA), amountToAdd));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                default:
                    TemporalException("Unsupported unit: "_S + Temporal::toString(unit)).throws($ftrace());
            }
        }

        LocalDate LocalDate::plusYears(glong yearsToAdd) const {
            const glong newYear = year() + yearsToAdd;
            checkValue(newYear, YEAR);
            return LocalDate(CORE_CAST(gint, newYear), month(), dayOfMonth());
        }

        LocalDate LocalDate::plusMonths(glong monthsToAdd) const {
            glong months = year() * 12LL + month() - 1 + monthsToAdd;
            gint newYear = year() + Math::floorDiv(months, 12);
            gint newMonth = year() + Math::floorMod(months, 12);
            try {
                return LocalDate(newYear, newMonth, dayOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::plusWeeks(glong weeksToAdd) const {
            try {
                return plusDays(Math::multiplyExact(weeksToAdd, 7));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::plusDays(glong daysToAdd) const {
            if (daysToAdd == 0)
                return *this;
            glong dom = dayOfMonth() + daysToAdd;
            if (dom > 0) {
                if (dom <= 28)
                    return LocalDate(year(), month(), (gint) dom);
                if (dom <= 59) {
                    // 59th Jan is 28th Feb, 59th Feb is 31st Mar
                    glong monthLen = lengthOfMonth();
                    try {
                        if (dom <= monthLen)
                            return LocalDate(year(), month(), CORE_CAST(gint, dom));
                        if (month() < 12)
                            return LocalDate(year(), month() + 1, CORE_CAST(gint, dom - monthLen));
                        checkValue(year() + 1, YEAR);
                        return LocalDate(year() + 1, 1, CORE_CAST(gint, dom - monthLen));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            }

            try {
                glong mjDay = Math::addExact(toEpochDay(), daysToAdd);
                return ofEpochDay(mjDay);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::minus(glong amountToSubtract, ChronoUnit unit) const {
            try {
                return amountToSubtract == Long::MIN_VALUE
                           ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                           : plus(-amountToSubtract, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::minusYears(glong yearsToSubtract) const {
            try {
                return yearsToSubtract == Long::MIN_VALUE
                           ? plusYears(Long::MAX_VALUE).plusYears(1)
                           : plusYears(-yearsToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::minusMonths(gint monthsToSubtract) const {
            try {
                return monthsToSubtract == Long::MIN_VALUE
                           ? plusMonths(Long::MAX_VALUE).plusMonths(1)
                           : plusMonths(-monthsToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::minusWeeks(gint weeksToSubtract) const {
            try {
                return weeksToSubtract == Long::MIN_VALUE
                           ? plusWeeks(Long::MAX_VALUE).plusWeeks(1)
                           : plusWeeks(-weeksToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::minusDays(glong daysToSubtract) const {
            try {
                return daysToSubtract == Long::MIN_VALUE
                           ? plusDays(Long::MAX_VALUE).plusDays(1)
                           : plusDays(-daysToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong LocalDate::until(const Temporal& endExclusive, ChronoUnit unit) const {
            LocalDate end = MIN;
            try {
                end = from(endExclusive);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
            switch (unit) {
                case DAYS: return end.toEpochDay() - toEpochDay();
                case WEEKS: return (end.toEpochDay() - toEpochDay()) / 7;
                case MONTHS: {
                    glong months1 = (year() * 12LL + month() - 1) * 32LL + dayOfMonth();
                    glong months2 = (end.year() * 12LL + end.month() - 1) * 32LL + end.dayOfMonth();
                    return months2 - months1;
                }
                case YEARS: {
                    glong months1 = (year() * 12LL + month() - 1) * 32LL + dayOfMonth();
                    glong months2 = (end.year() * 12LL + end.month() - 1) * 32LL + end.dayOfMonth();
                    return (months2 - months1) / 12;
                }
                case DECADES: {
                    glong months1 = (year() * 12LL + month() - 1) * 32LL + dayOfMonth();
                    glong months2 = (end.year() * 12LL + end.month() - 1) * 32LL + end.dayOfMonth();
                    return (months2 - months1) / 120;
                }
                case CENTURIES: {
                    glong months1 = (year() * 12LL + month() - 1) * 32LL + dayOfMonth();
                    glong months2 = (end.year() * 12LL + end.month() - 1) * 32LL + end.dayOfMonth();
                    return (months2 - months1) / 1200;
                }
                case MILLENNIA: {
                    glong months1 = (year() * 12LL + month() - 1) * 32LL + dayOfMonth();
                    glong months2 = (end.year() * 12LL + end.month() - 1) * 32LL + end.dayOfMonth();
                    return (months2 - months1) / 12000;
                }
                case ERAS: return end.getLong(ERA) - getLong(ERA);
                default:
                    TemporalException("Unsupported unit: "_S + Temporal::toString(unit)).throws($ftrace());
            }
        }

        LocalDateTime LocalDate::atTime(LocalTime const& time) const {
            return LocalDateTime(*this, time);
        }

        LocalDateTime LocalDate::atTime(gint hour, gint minute) const {
            try {
                return LocalDateTime(*this, LocalTime(hour, minute));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDate::atTime(gint hour, gint minute, gint second) const {
            try {
                return LocalDateTime(*this, LocalTime(hour, minute, second));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDate::atTime(gint hour, gint minute, gint second, gint nanoOfSecond) const {
            try {
                return LocalDateTime(*this, LocalTime(hour, minute, second, nanoOfSecond));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDate::atStartOfDay() const {
            try {
                return LocalDateTime(*this, LocalTime::MIDNIGHT);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDate::atStartOfDay(ZoneId const& zone) const {
            try {
                LocalDateTime localDT = atTime(LocalTime::MIDNIGHT);
                if (!Class<ZoneOffset>::hasInstance(zone)) {
                    ZoneRules rules = zone.getRules();
                    Optional<ZoneOffsetTransition> trans = rules.transition(localDT);
                    if (trans.isPresent() && trans.get().isGap())
                        localDT = trans.get().dateTimeAfter();
                }
                return localDT;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong LocalDate::toEpochDay() const {
            glong y = year();
            glong m = month();
            glong total = 0;
            total += 365 * y;
            if (y >= 0) {
                total += (y + 3) / 4 - (y + 99) / 100 + (y + 399) / 400;
            } else {
                total -= y / -4 - y / -100 + y / -400;
            }
            total += (367 * m - 362) / 12;
            total += dayOfMonth() - 1;
            if (m > 2) {
                total--;
                if (isLeapYear() == false) {
                    total--;
                }
            }
            return total - DAYS_0000_TO_1970;
        }

        glong LocalDate::toEpochSecond(LocalTime const& time, ZoneOffset const& offset) const {
            return toEpochDay() * LocalTime::SECONDS_PER_DAY + time.toSecondOfDay()  - offset.totalSeconds();
        }

        gint LocalDate::compareTo(const LocalDate& other) const {
            int r = year() - other.year();
            if (r == 0) {
                r = month() - other.month();
                if (r == 0)
                    r = dayOfMonth() - other.dayOfMonth();
            }
            return r;
        }

        gbool LocalDate::isAfter(LocalDate const& other) const {
            return compareTo(other) > 0;
        }

        gbool LocalDate::isBefore(LocalDate const& other) const {
            return compareTo(other) < 0;
        }

        Optional<> LocalDate::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::LOCAL_DATE)
                return *this;
            if (query == TemporalQuery::PRECISION)
                return Optional<Enum<ChronoUnit>>(DAYS);
            return Optional<>();
        }

        LocalDate LocalDate::from(Temporal const& temporal) {
            Optional<LocalDate> date;
            try {
                date = (Optional<LocalDate>) temporal.query(TemporalQuery::LOCAL_DATE);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (date.isEmpty())
                DateTimeException("Unable to obtain LocalDate from Temporal: "_Sl +
                    temporal + " of type " + typeName(temporal)).throws($ftrace());
            return date.get();
        }

        gbool LocalDate::equals(const Object& obj) const {
            return this == &obj || Class<LocalDate>::hasInstance(obj) && ymd == CORE_XCAST(LocalDate const, obj).ymd;
        }

        gint LocalDate::hash() const {
            return Long::hash(ymd);
        }

        String LocalDate::toString() const {
            gint year = LocalDate::year();
            gint month = LocalDate::month();
            gint day = dayOfMonth();
            gint absYear = Math::abs(year);
            XString str = XString(10);
            if (absYear < 1000)
                if (year < 0)
                    str.append(year - 10000).deleteCharAt(1);
                else
                    str.append(year + 10000).deleteCharAt(0);
            else {
                if (year > 9999)
                    str.append('+');
                str.append(year);
            }
            return str.append(month < 10 ? "-0"_S : "-"_S)
                      .append(month)
                      .append(day < 10 ? "-0"_S : "-"_S)
                      .append(day)
                      .toString();
        }

        Object& LocalDate::clone() const {
            return UNSAFE::newInstance<LocalDate>(*this);
        }

        // LocalDate::LocalDate() {
        //     ymd = dateToCompactInt(1970, JANUARY, 01);
        // }

        glong LocalDate::dateToCompactInt(gint year, gint month, gint dayOfMonth) {
            if (dayOfMonth > 28) {
                gint dom = 0;
                switch (month) {
                    case 2: dom = isLeapYear(year) ? 29 : 28;
                        break;
                    case 4:
                    case 6:
                    case 9:
                    case 11: dom = 30;
                        break;
                    default: dom = 31;
                        break;
                }
                if (dayOfMonth > dom) {
                    if (dom == 29)
                        DateTimeException("Invalid date 'February 29' as '"_Sl
                            + year + "' is not a leap year").throws($ftrace());
                    DateTimeException("Invalid date '"_Sl + displayMonth(month)
                        + " " + dayOfMonth + "'").throws($ftrace());
                }
            }
            glong date = 0;
            date |= (glong) Math::absExact(year) << YEAR_OFFSET & YEAR_MASK;
            date |= month << MONTH_OFFSET & MONTH_MASK;
            date |= dayOfMonth << DAY_OFFSET & DAY_MASK;
            return year < 0 ? -date : date;
        }

        gbool LocalDate::isLeapYear(gint prolepticYear) {
            return (prolepticYear & 3) == 0 && (prolepticYear % 100 != 0 || prolepticYear % 400 == 0);
        }

        gint LocalDate::lengthOfMonth(Month month, gbool leapYear) {
            switch (month) {
                case FEBRUARY: return leapYear ? 29 : 28;
                case APRIL:
                case JUNE:
                case SEPTEMBER:
                case NOVEMBER: return 30;
                default: return 31;
            }
        }
    } // util
} // core
