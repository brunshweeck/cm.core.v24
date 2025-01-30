//
// Created by brunshweeck on 3 août 2024.
//

#include "LocalDateTime.h"

#include <core/Enum.h>
#include <core/misc/Unsafe.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZoneOffset.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;

    namespace time {
        LocalDateTime const LocalDateTime::MIN = LocalDateTime(-999999999, 1, 1, 0, 0);
        LocalDateTime const LocalDateTime::MAX = LocalDateTime(+999999999, 12, 31, 23, 59, 59, 999999999);
        LocalDateTime const LocalDateTime::EPOCH = LocalDateTime(1970, 01, 01, 0, 0, 0, 0);

        LocalDateTime::LocalDateTime(gint year, LocalDate::Month month, gint dayOfMonth, gint hour, gint minute)
            : date(year, month, dayOfMonth), time(hour, minute) {}

        LocalDateTime::LocalDateTime(gint year, LocalDate::Month month, gint dayOfMonth,
                                     gint hour, gint minute, gint second)
            : date(year, month, dayOfMonth), time(hour, minute, second) {}

        LocalDateTime::LocalDateTime(gint year, LocalDate::Month month, gint dayOfMonth,
                                     gint hour, gint minute, gint second, gint nanoOfSecond)
            : date(year, month, dayOfMonth), time(hour, minute, second, nanoOfSecond) {}

        LocalDateTime::LocalDateTime(gint year, gint month, gint dayOfMonth,
                                     gint hour, gint minute)
            : date(year, month, dayOfMonth), time(hour, minute) {}

        LocalDateTime::LocalDateTime(gint year, gint month, gint dayOfMonth,
                                     gint hour, gint minute, gint second)
            : date(year, month, dayOfMonth), time(hour, minute, second) {}

        LocalDateTime::LocalDateTime(gint year, gint month, gint dayOfMonth,
                                     gint hour, gint minute, gint second, gint nanoOfSecond)
            : date(year, month, dayOfMonth), time(hour, minute, second, nanoOfSecond) {}

        LocalDateTime::LocalDateTime(LocalDate const& date, LocalTime const& time)
            : date(date), time(time) {}

        LocalDateTime LocalDateTime::ofEpochSecond(glong epochSecond, gint nanoOfSecond) {
            checkValue(nanoOfSecond, NANO_OF_SECOND);
            glong localSecond = epochSecond;
            glong localEpochDay = Math::floorDiv(localSecond, LocalTime::SECONDS_PER_DAY);
            glong secsOfDay = Math::floorMod(localSecond, LocalTime::SECONDS_PER_DAY);
            try {
                LocalDate date = LocalDate::ofEpochDay(localEpochDay);
                LocalTime time = LocalTime::ofNanoOfDay(secsOfDay * LocalTime::NANOS_PER_SECOND + nanoOfSecond);
                return LocalDateTime(date, time);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::ofEpochSecond(glong epochSecond, gint nanoOfSecond, ZoneOffset const& offset) {
            checkValue(nanoOfSecond, NANO_OF_SECOND);
            glong localSecond = epochSecond + offset.totalSeconds(); // overflow caught later
            glong localEpochDay = Math::floorDiv(localSecond, LocalTime::SECONDS_PER_DAY);
            gint secsOfDay = Math::floorMod(localSecond, LocalTime::SECONDS_PER_DAY);
            try {
                LocalDate date = LocalDate::ofEpochDay(localEpochDay);
                LocalTime time = LocalTime::ofNanoOfDay(secsOfDay * LocalTime::NANOS_PER_SECOND + nanoOfSecond);
                return LocalDateTime(date, time);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool LocalDateTime::isSupported(ChronoField field) const {
            return isTimeBased(field) || isDateBased(field) || field == INSTANT_SECONDS;
        }

        gbool LocalDateTime::isSupported(ChronoUnit unit) const {
            return isTimeBased(unit) || isDateBased(unit);
        }

        gint LocalDateTime::get(ChronoField field) const {
            if (field == INSTANT_SECONDS)
                TemporalException("Value of field "_S + Temporal::toString(field)
                    + " exceed implementation limit"_S).throws($ftrace());
            try {
                if (field == OFFSET_SECONDS)
                    return 0;
                return time.isSupported(field) ? time.get(field) : date.get(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong LocalDateTime::getLong(ChronoField field) const {
            try {
                return field == INSTANT_SECONDS
                           ? toEpochSecond()
                           : field == OFFSET_SECONDS
                                 ? 0
                                 : time.isSupported(field)
                                       ? time.getLong(field)
                                       : date.getLong(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDateTime::toLocalDate() const {
            return date;
        }

        gint LocalDateTime::year() const {
            return date.year();
        }

        LocalDate::Month LocalDateTime::month() const {
            return date.month();
        }

        gint LocalDateTime::dayOfMonth() const {
            return date.dayOfMonth();
        }

        gint LocalDateTime::dayOfYear() const {
            return date.dayOfYear();
        }

        LocalDate::DayOfWeek LocalDateTime::dayOfWeek() const {
            return date.dayOfWeek();
        }

        LocalTime LocalDateTime::toLocalTime() const {
            return time;
        }

        gint LocalDateTime::hour() const {
            return time.hour();
        }

        gint LocalDateTime::minute() const {
            return time.minute();
        }

        gint LocalDateTime::second() const {
            return time.second();
        }

        gint LocalDateTime::nano() const {
            return time.nano();
        }

        LocalDateTime LocalDateTime::with(ChronoField field, glong newValue) const {
            try {
                return time.isSupported(field)
                           ? date.atTime(time.with(field, newValue))
                           : time.atDate(date.with(field, newValue));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::withYear(gint year) const {
            try { return time.atDate(date.withYear(year)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::withMonth(gint month) const {
            try { return time.atDate(date.withMonth(month)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::withDayOfMonth(gint dayOfMonth) const {
            try {
                return time.atDate(date.withDayOfMonth(dayOfMonth));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::withDayOfYear(gint dayOfYear) const {
            try {
                return time.atDate(date.withDayOfYear(dayOfYear));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::withHour(gint hour) const {
            try {
                return date.atTime(time.withHour(hour));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::withMinute(gint minute) const {
            try {
                return date.atTime(time.withMinute(minute));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::withSecond(gint second) const {
            try {
                return date.atTime(time.withSecond(second));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::withNano(gint nanoOfSecond) const {
            try {
                return date.atTime(time.withNano(nanoOfSecond));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::truncateTo(ChronoUnit unit) const {
            try {
                return date.atTime(time.truncateTo(unit));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::plus(glong amountToAdd, ChronoUnit unit) const {
            switch (unit) {
                case NANOS:
                    try { return plusNanos(amountToAdd); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MICROS:
                    try {
                        return plusDays(amountToAdd / LocalTime::MICROS_PER_DAY)
                                .plusNanos(amountToAdd % LocalTime::MICROS_PER_DAY * 1000);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MILLIS:
                    try {
                        return plusDays(amountToAdd / LocalTime::MILLIS_PER_DAY)
                                .plusNanos(amountToAdd % LocalTime::MICROS_PER_DAY * LocalTime::NANOS_PER_MILLI);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case SECONDS:
                    try { return plusSeconds(amountToAdd); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MINUTES:
                    try { return plusMinutes(amountToAdd); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case HOURS:
                    try { return plusHours(amountToAdd); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case HALF_DAYS:
                    try {
                        return plusDays(amountToAdd / 256).plusHours(amountToAdd % 256 * 12);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                default:
                    try {
                        return time.atDate(date.plus(amountToAdd, unit));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        }

        LocalDateTime LocalDateTime::plusYears(glong years) const {
            try {
                return time.atDate(date.plusYears(years));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::plusMonths(glong months) const {
            try {
                return time.atDate(date.plusMonths(months));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::plusWeeks(glong weeks) const {
            try {
                return time.atDate(date.plusWeeks(weeks));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::plusDays(glong days) const {
            try {
                return time.atDate(date.plusDays(days));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::plusHours(glong hours) const {
            glong hoursCount = hour() + hours;
            gint daysToAdd = Math::floorDiv(hoursCount, LocalTime::HOURS_PER_DAY);
            gint newHour = Math::floorMod(hoursCount, LocalTime::HOURS_PER_DAY);
            try {
                return time.withHour(newHour).atDate(date.plusDays(daysToAdd));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::plusMinutes(glong minutes) const {
            glong daysToAdd = Math::floorDiv(minutes, LocalTime::MINUTES_PER_DAY);
            minutes = Math::floorMod(minutes, LocalTime::MINUTES_PER_DAY);

            glong minutesCount = hour() * 60 + (minute() + minutes);
            daysToAdd += Math::floorDiv(minutesCount, LocalTime::MINUTES_PER_DAY);
            minutesCount = Math::floorMod(minutesCount, LocalTime::MINUTES_PER_DAY);
            gint newHour = Math::floorDiv(minutesCount, LocalTime::MINUTES_PER_HOUR);
            gint newMinute = Math::floorMod(minutesCount, LocalTime::MINUTES_PER_HOUR);
            try {
                return date.plusDays(daysToAdd).atTime(newHour, newMinute, time.second(), time.nano());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::plusSeconds(glong seconds) const {
            glong daysToAdd = Math::floorDiv(seconds, LocalTime::SECONDS_PER_DAY);
            seconds = Math::floorMod(seconds, LocalTime::SECONDS_PER_DAY);

            glong secondsCount = time.toSecondOfDay() + seconds;
            daysToAdd += Math::floorDiv(secondsCount, LocalTime::SECONDS_PER_DAY);
            secondsCount = Math::floorMod(secondsCount, LocalTime::SECONDS_PER_DAY);

            gint newHour = Math::floorDiv(secondsCount, LocalTime::SECONDS_PER_HOUR);
            gint newMinute = Math::floorDiv(secondsCount, LocalTime::SECONDS_PER_MINUTE) % LocalTime::MINUTES_PER_HOUR;
            gint newSecond = Math::floorMod(secondsCount, LocalTime::SECONDS_PER_MINUTE);

            try {
                return date.plusDays(daysToAdd).atTime(newHour, newMinute, newSecond, time.nano());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::plusNanos(glong nanos) const {
            glong daysToAdd = Math::floorDiv(nanos, LocalTime::NANOS_PER_DAY);
            nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_DAY);

            glong nanosCount = time.toNanoOfDay() + nanos;
            daysToAdd += Math::floorDiv(nanosCount, LocalTime::NANOS_PER_DAY);
            nanosCount = Math::floorMod(nanosCount, LocalTime::NANOS_PER_DAY);

            gint newHour = CORE_CAST(gint, Math::floorDiv(nanosCount, LocalTime::NANOS_PER_HOUR));
            gint newMinute = CORE_CAST(gint, Math::floorDiv(nanosCount, LocalTime::NANOS_PER_MINUTE)) %
                    LocalTime::MINUTES_PER_HOUR;
            gint newSecond = CORE_CAST(gint, Math::floorDiv(nanosCount, LocalTime::NANOS_PER_SECOND)) %
                    LocalTime::SECONDS_PER_MINUTE;
            gint newNano = CORE_CAST(gint, Math::floorMod(nanosCount, LocalTime::NANOS_PER_SECOND));

            try {
                return date.plusDays(daysToAdd).atTime(newHour, newMinute, newSecond, newNano);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minus(glong amountToSubtract, ChronoUnit unit) const {
            try {
                return amountToSubtract == Long::MIN_VALUE
                           ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                           : plus(-amountToSubtract, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minusYears(glong years) const {
            try {
                return years == Long::MIN_VALUE ? plusYears(Long::MAX_VALUE).plusYears(1) : plusYears(-years);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minusMonths(glong months) const {
            try {
                return months == Long::MIN_VALUE ? plusMonths(Long::MAX_VALUE).plusMonths(1) : plusMonths(-months);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minusWeeks(glong weeks) const {
            try {
                return weeks == Long::MIN_VALUE ? plusWeeks(Long::MAX_VALUE).plusWeeks(1) : plusWeeks(-weeks);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minusDays(glong days) const {
            try {
                return days == Long::MIN_VALUE ? plusDays(Long::MAX_VALUE).plusDays(1) : plusDays(-days);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minusHours(glong hours) const {
            try {
                return hours == Long::MIN_VALUE ? plusHours(Long::MAX_VALUE).plusHours(1) : plusHours(-hours);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minusMinutes(glong minutes) const {
            try {
                return minutes == Long::MIN_VALUE
                           ? plusMinutes(Long::MAX_VALUE).plusMinutes(1)
                           : plusMinutes(-minutes);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minusSeconds(glong seconds) const {
            try {
                return seconds == Long::MIN_VALUE
                           ? plusSeconds(Long::MAX_VALUE).plusSeconds(1)
                           : plusSeconds(-seconds);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::minusNanos(glong nanos) const {
            try {
                return nanos == Long::MIN_VALUE ? plusNanos(Long::MAX_VALUE).plusNanos(1) : plusNanos(-nanos);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong LocalDateTime::until(const Temporal& endExclusive, ChronoUnit unit) const {
            LocalDateTime end = MIN;
            try {
                end = from(endExclusive);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (isTimeBased(unit)) {
                glong daysCount = end.date.toEpochDay() - date.toEpochDay();
                glong nanosCount = end.time.toNanoOfDay() - time.toNanoOfDay();

                if (daysCount > 0) {
                    daysCount -= 1;
                    nanosCount += LocalTime::NANOS_PER_DAY;
                } else if (daysCount < 0) {
                    daysCount += 1;
                    nanosCount -= LocalTime::NANOS_PER_DAY;
                }
                switch (unit) {
                    case NANOS:
                        daysCount = Math::multiplyExact(daysCount, LocalTime::NANOS_PER_DAY);
                        break;
                    case MICROS:
                        daysCount = Math::multiplyExact(daysCount, LocalTime::MICROS_PER_DAY);
                        nanosCount = nanosCount / 1000;
                        break;
                    case MILLIS:
                        daysCount = Math::multiplyExact(daysCount, LocalTime::MILLIS_PER_DAY);
                        nanosCount = nanosCount / 1000000;
                        break;
                    case SECONDS:
                        daysCount = Math::multiplyExact(daysCount, LocalTime::SECONDS_PER_DAY);
                        nanosCount = nanosCount / LocalTime::NANOS_PER_SECOND;
                        break;
                    case MINUTES:
                        daysCount = Math::multiplyExact(daysCount, LocalTime::MINUTES_PER_DAY);
                        nanosCount = nanosCount / LocalTime::NANOS_PER_MINUTE;
                        break;
                    case HOURS:
                        daysCount = Math::multiplyExact(daysCount, LocalTime::HOURS_PER_DAY);
                        nanosCount = nanosCount / LocalTime::NANOS_PER_HOUR;
                        break;
                    case HALF_DAYS:
                        daysCount = Math::multiplyExact(daysCount, 2);
                        nanosCount = nanosCount / (LocalTime::NANOS_PER_HOUR * 12);
                        break;
                    default: break;
                }
                try {
                    return Math::addExact(daysCount, nanosCount);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
            LocalDate endDate = end.date;
            if (endDate.isAfter(date) && end.time.isBefore(time))
                endDate = endDate.minusDays(1);
            else if (endDate.isBefore(date) && end.time.isAfter(time))
                endDate = endDate.plusDays(1);
            try {
                return date.until(endDate, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong LocalDateTime::toEpochSecond() const {
            try {
                return date.toEpochDay() * LocalTime::SECONDS_PER_DAY + toLocalTime().toSecondOfDay();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong LocalDateTime::toEpochSecond(ZoneOffset const& offset) const {
            try {
                return date.toEpochDay() * LocalTime::SECONDS_PER_DAY + time.toSecondOfDay() - offset.totalSeconds();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint LocalDateTime::compareTo(const LocalDateTime& other) const {
            gint r = date.compareTo(other.toLocalDate());
            if (r == 0)
                r = time.compareTo(other.toLocalTime());
            return r;
        }

        gbool LocalDateTime::isAfter(LocalDateTime const& other) const {
            return compareTo(other) > 0;
        }

        gbool LocalDateTime::isBefore(LocalDateTime const& other) const {
            return compareTo(other) < 0;
        }

        Optional<> LocalDateTime::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::LOCAL_DATE)
                return toLocalDate();
            if (query == TemporalQuery::LOCAL_TIME)
                return toLocalTime();
            if (query == TemporalQuery::PRECISION)
                return Optional<Enum<ChronoUnit>>(DAYS);
            if (query == TemporalQuery::ZONE || query == TemporalQuery::OFFSET)
                return Optional<>();

            try {
                return query.queryFrom(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime LocalDateTime::from(Temporal const& temporal) {
            Optional<LocalDate> date;
            Optional<LocalTime> time;
            try {
                date = (Optional<LocalDate>) temporal.query(TemporalQuery::LOCAL_DATE);
                time = (Optional<LocalTime>) temporal.query(TemporalQuery::LOCAL_TIME);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (date.isEmpty() || time.isEmpty()) {
                if (date.isPresent())
                    UNSAFE::deleteRegInstance(date.get());
                if (time.isPresent())
                    UNSAFE::deleteRegInstance(time.get());
                DateTimeException("Unable to obtain LocalDateTime from Temporal: "_Sl + temporal
                    + " of type " + typeName(temporal)).throws($ftrace());
            }
            LocalDate localD = date.get();
            LocalTime localT = time.get();
            UNSAFE::deleteRegInstance(date.get());
            UNSAFE::deleteRegInstance(time.get());

            return LocalDateTime(localD, localT);
        }

        gbool LocalDateTime::equals(const Object& obj) const {
            return this == &obj ||
                    Class<LocalDateTime>::hasInstance(obj) && compareTo(CORE_XCAST(LocalDateTime const, obj)) == 0;
        }

        gint LocalDateTime::hash() const {
            return date.hash() ^ time.hash();
        }

        String LocalDateTime::toString() const {
            return date.toString() + "T"_S + time.toString();
        }

        Object& LocalDateTime::clone() const {
            try {
                return UNSAFE::newInstance<LocalDateTime>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // time
} // core
