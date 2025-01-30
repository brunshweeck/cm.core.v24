//
// Created by brunshweeck on 3 août 2024.
//

#include "LocalTime.h"

#include <core/Enum.h>
#include <core/XString.h>
#include <core/misc/Unsafe.h>
#include <core/time/LocalDateTime.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZoneOffset.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;

    namespace time {
        LocalTime const LocalTime::MIN = LocalTime(MIN_HOUR, MIN_MINUTE, MIN_SECOND, MIN_NANOSECOND);
        LocalTime const LocalTime::MAX = LocalTime(MAX_HOUR, MAX_MINUTE, MAX_SECOND, MAX_NANOSECOND);
        LocalTime const LocalTime::MIDNIGHT = LocalTime(00, 00);
        LocalTime const LocalTime::NOON = LocalTime(12, 00);

        LocalTime LocalTime::ofSecondOfDay(glong secondOfDay) {
            try { checkValue(secondOfDay, SECOND_OF_DAY); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint hours = Math::floorDiv(secondOfDay, SECONDS_PER_HOUR);
            secondOfDay = Math::floorMod(secondOfDay, SECONDS_PER_HOUR);
            gint minutes = Math::floorDiv(secondOfDay, SECONDS_PER_MINUTE);
            secondOfDay = Math::floorMod(secondOfDay, SECONDS_PER_MINUTE);
            gint seconds = (gint) secondOfDay;
            return LocalTime(hours, minutes, seconds, 000000000);
        }

        LocalTime LocalTime::ofNanoOfDay(glong nanoOfDay) {
            try { checkValue(nanoOfDay, NANO_OF_DAY); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint hours = (gint) Math::floorDiv(nanoOfDay, NANOS_PER_HOUR);
            nanoOfDay = Math::floorMod(nanoOfDay, NANOS_PER_HOUR);
            gint minutes = (gint) Math::floorDiv(nanoOfDay, NANOS_PER_MINUTE);
            nanoOfDay = Math::floorMod(nanoOfDay, NANOS_PER_MINUTE);
            gint seconds = (gint) Math::floorDiv(nanoOfDay, NANOS_PER_SECOND);
            nanoOfDay = Math::floorMod(nanoOfDay, NANOS_PER_SECOND);
            gint nanos = (gint) nanoOfDay;
            return LocalTime(hours, minutes, seconds, nanos);
        }

        LocalTime::LocalTime(gint hour, gint minute): LocalTime(hour, minute, 00, 000000000) {}

        LocalTime::LocalTime(gint hour, gint minute, gint second): LocalTime(hour, minute, second, 000000000) {}

        LocalTime::LocalTime(gint hour, gint minute, gint second, gint nanoOfSecond) {
            try {
                hms = timeToCompactInt(
                    checkValue(hour, HOUR_OF_DAY),
                    checkValue(minute, MINUTE_OF_HOUR),
                    checkValue(second, SECOND_OF_MINUTE),
                    checkValue(nanoOfSecond, NANO_OF_SECOND)
                );
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool LocalTime::isSupported(ChronoField field) const {
            return field < DAY_OF_WEEK;
        }

        gbool LocalTime::isSupported(ChronoUnit unit) const {
            return unit < DAYS;
        }

        gint LocalTime::get(ChronoField field) const {
            switch (field) {
                case NANO_OF_SECOND: return nano();
                case NANO_OF_DAY: goto RESULT_OVERFLOW;
                case MICRO_OF_SECOND: return nano() / 1000;
                case MICRO_OF_DAY: goto RESULT_OVERFLOW;
                case MILLI_OF_SECOND: return (gint) (nano() / NANOS_PER_MILLI);
                case MILLI_OF_DAY: return (gint) (toNanoOfDay() / NANOS_PER_MILLI);
                case SECOND_OF_MINUTE: return second();
                case SECOND_OF_DAY: return toSecondOfDay();
                case MINUTE_OF_HOUR: return minute();
                case MINUTE_OF_DAY: return hour() * 60 + minute();
                case HOUR_OF_AMPM: return hour() % 12;
                case CLOCK_HOUR_OF_AMPM: {
                    gint am = hour() % 12;
                    return am % 12 == 0 ? 12 : am;
                }
                case HOUR_OF_DAY: return hour();
                case CLOCK_HOUR_OF_DAY: return hour() == 0 ? 24 : hour();
                case AMPM_OF_DAY: return hour() / 12;
                default:
                    TemporalException("Unsupported field "_S + Temporal::toString(field)).throws($ftrace());
            }
        RESULT_OVERFLOW:
            TemporalException("Value of field "_S + Temporal::toString(field)
                + " exceed implentation limit."_S).throws($ftrace());
        }

        glong LocalTime::getLong(ChronoField field) const {
            switch (field) {
                case NANO_OF_SECOND: return nano();
                case NANO_OF_DAY: return toNanoOfDay();
                case MICRO_OF_SECOND: return nano() / 1000;
                case MICRO_OF_DAY: return toNanoOfDay() / 1000;
                case MILLI_OF_SECOND: return (gint) (nano() / NANOS_PER_MILLI);
                case MILLI_OF_DAY: return (gint) (toNanoOfDay() / NANOS_PER_MILLI);
                case SECOND_OF_MINUTE: return second();
                case SECOND_OF_DAY: return toSecondOfDay();
                case MINUTE_OF_HOUR: return minute();
                case MINUTE_OF_DAY: return hour() * 60 + minute();
                case HOUR_OF_AMPM: return hour() % 12;
                case CLOCK_HOUR_OF_AMPM: {
                    gint am = hour() % 12;
                    return am % 12 == 0 ? 12 : am;
                }
                case HOUR_OF_DAY: return hour();
                case CLOCK_HOUR_OF_DAY: return hour() == 0 ? 24 : hour();
                case AMPM_OF_DAY: return hour() / 12;
                default:
                    TemporalException("Unsupported field "_S + Temporal::toString(field)).throws($ftrace());
            }
        }

        gint LocalTime::hour() const {
            return (gint) ((hms & HOUR_MASK) >> HOUR_OFFSET);
        }

        gint LocalTime::minute() const {
            return (gint) ((hms & MINUTE_MASK) >> MINUTE_OFFSET);
        }

        gint LocalTime::second() const {
            return (gint) ((hms & SECOND_MASK) >> SECOND_OFFSET);
        }

        gint LocalTime::nano() const {
            return (gint) ((hms & NANOSECOND_MASK) >> NANOSECOND_OFFSET);
        }

        LocalTime LocalTime::with(ChronoField field, glong newValue) const {
            try { checkValue(newValue, field); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            switch (field) {
                case NANO_OF_SECOND:
                    try {
                        return withNano((gint) newValue);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case NANO_OF_DAY:
                    try { return ofNanoOfDay(newValue); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MICRO_OF_SECOND:
                    try {
                        return withNano((gint) newValue * 1000);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MICRO_OF_DAY:
                    try { return ofNanoOfDay(newValue * 1000); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MILLI_OF_SECOND:
                    try {
                        return withNano((gint) newValue * NANOS_PER_MILLI);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MILLI_OF_DAY:
                    try {
                        return ofNanoOfDay(newValue * NANOS_PER_MILLI);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case SECOND_OF_MINUTE:
                    try {
                        return withSecond((gint) newValue);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case SECOND_OF_DAY:
                    try {
                        return plusSeconds(newValue - toSecondOfDay());
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MINUTE_OF_HOUR:
                    try {
                        return withMinute((gint) newValue);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case MINUTE_OF_DAY:
                    try {
                        return plusMinutes(newValue - (hour() * 60 + minute()));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case HOUR_OF_AMPM:
                    try {
                        return plusHours(newValue - hour() % 12);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case CLOCK_HOUR_OF_AMPM:
                    try {
                        return plusHours((newValue == 12 ? 0 : newValue) - hour() % 12);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case HOUR_OF_DAY:
                    try {
                        return withHour((gint) newValue);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case CLOCK_HOUR_OF_DAY:
                    try {
                        return withHour((gint) (newValue == 24 ? 0 : newValue));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case AMPM_OF_DAY:
                    try {
                        return plusHours((newValue - hour() / 12) * 12);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                default:
                    TemporalException("Unsupported field "_S + Temporal::toString(field)).throws($ftrace());
            }
        }

        LocalTime LocalTime::withHour(gint hour) const {
            try {
                return LocalTime(checkValue(hour, HOUR_OF_DAY), minute(), second(), nano());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalTime LocalTime::withMinute(gint minute) const {
            try {
                return LocalTime(hour(), checkValue(minute, MINUTE_OF_HOUR), second(), nano());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalTime LocalTime::withSecond(gint second) const {
            try {
                return LocalTime(hour(), minute(), checkValue(second, SECOND_OF_MINUTE), nano());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalTime LocalTime::withNano(gint nanoOfSecond) const {
            try {
                return LocalTime(hour(), minute(), second(), checkValue(nanoOfSecond, NANO_OF_SECOND));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalTime LocalTime::truncateTo(ChronoUnit unit) const {
            switch (unit) {
                case NANOS: return *this;
                case MICROS: return ofNanoOfDay(toNanoOfDay() / 1000 * 1000);
                case MILLIS: return ofNanoOfDay(toNanoOfDay() / NANOS_PER_MILLI * NANOS_PER_MILLI);
                case SECONDS: return LocalTime(hour(), minute(), second(), 000000000);
                case MINUTES: return LocalTime(hour(), minute(), 00, 000000000);
                case HOURS: return LocalTime(hour(), 00, 00, 000000000);
                case HALF_DAYS:
                    TemporalException("Unit `HalfDays` is too large to be used for truncation"_S).throws($ftrace());
                default:
                    TemporalException("Unsupported unit "_S + Temporal::toString(unit)).throws($ftrace());
            }
        }

        LocalTime LocalTime::plus(glong amountToAdd, ChronoUnit unit) const {
            switch (unit) {
                case NANOS: return plusNanos(amountToAdd % NANOS_PER_DAY);
                case MICROS: return plusNanos(amountToAdd % MICROS_PER_DAY * 1000);
                case MILLIS: return plusNanos(amountToAdd % MILLIS_PER_DAY * NANOS_PER_MILLI);
                case SECONDS: return plusSeconds(amountToAdd % SECONDS_PER_DAY);
                case MINUTES: return plusMinutes(amountToAdd % MICROS_PER_DAY);
                case HOURS: return plusHours(amountToAdd % HOURS_PER_DAY);
                default:
                    TemporalException("Unsupported unit "_S + Temporal::toString(unit)).throws($ftrace());
            }
        }

        LocalTime LocalTime::plusHours(glong hoursToAdd) const {
            if (hoursToAdd == 0)
                return *this;
            gint newHour = (gint) (hoursToAdd % HOURS_PER_DAY + hour() + HOURS_PER_DAY) % HOURS_PER_DAY;
            return LocalTime(newHour, minute(), second(), nano());
        }

        LocalTime LocalTime::plusMinutes(glong minutesToAdd) const {
            if (minutesToAdd == 0)
                return *this;
            gint minuteOfDay = hour() * MINUTES_PER_HOUR + minute();
            gint newMinuteOfDay = (gint) (minutesToAdd % MINUTES_PER_DAY + minuteOfDay + MINUTES_PER_DAY);
            if (minuteOfDay == newMinuteOfDay)
                return *this;
            gint newHour = newMinuteOfDay / MINUTES_PER_HOUR;
            gint newMinute = newMinuteOfDay % MINUTES_PER_HOUR;
            return LocalTime(newHour, newMinute, second(), nano());
        }

        LocalTime LocalTime::plusSeconds(glong secondsToAdd) const {
            if (secondsToAdd == 0)
                return *this;
            gint secondOfDay = hour() * SECONDS_PER_HOUR + minute() * SECONDS_PER_MINUTE + second();
            gint newSecondOfDay = (gint) (secondsToAdd % SECONDS_PER_DAY + secondOfDay + SECONDS_PER_DAY);
            if (secondOfDay == newSecondOfDay)
                return *this;
            gint newHour = newSecondOfDay / SECONDS_PER_HOUR;
            gint newMinute = newSecondOfDay / SECONDS_PER_MINUTE % SECONDS_PER_MINUTE;
            gint newSecond = newSecondOfDay % SECONDS_PER_MINUTE;
            return LocalTime(newHour, newMinute, newSecond, nano());
        }

        LocalTime LocalTime::plusNanos(glong nanosToAdd) const {
            if (nanosToAdd == 0)
                return *this;
            glong nanoOfDay = toNanoOfDay();
            glong newNanoOfDay = (nanosToAdd % NANOS_PER_DAY + nanoOfDay + NANOS_PER_DAY) % NANOS_PER_DAY;
            if (nanoOfDay == newNanoOfDay)
                return *this;
            gint newHour = (gint) (newNanoOfDay / NANOS_PER_HOUR);
            gint newMinute = (gint) (newNanoOfDay / NANOS_PER_MILLI % NANOS_PER_MINUTE);
            gint newSecond = (gint) (newNanoOfDay / NANOS_PER_SECOND % NANOS_PER_SECOND);
            gint newNano = (gint) (newNanoOfDay % NANOS_PER_SECOND);

            return LocalTime(newHour, newMinute, newSecond, newNano);
        }

        LocalTime LocalTime::minus(glong amountToSubtract, ChronoUnit unit) const {
            return amountToSubtract == Long::MIN_VALUE
                       ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                       : plus(-amountToSubtract, unit);
        }

        LocalTime LocalTime::minusHours(glong hoursToSubtract) const {
            return plusHours(-(hoursToSubtract % HOURS_PER_DAY));
        }

        LocalTime LocalTime::minusMinutes(glong minutesToSubtract) const {
            return plusMinutes(-(minutesToSubtract % MICROS_PER_DAY));
        }

        LocalTime LocalTime::minusSeconds(glong secondsToSubtract) const {
            return plusSeconds(-(secondsToSubtract % SECONDS_PER_DAY));
        }

        LocalTime LocalTime::minusNanos(glong nanosToSubtract) const {
            return plusNanos(-(nanosToSubtract % NANOS_PER_DAY));
        }

        glong LocalTime::until(const Temporal& endExclusive, ChronoUnit unit) const {
            LocalTime end = MIN;
            try {
                end = from(endExclusive);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            glong nanosUntil = end.toNanoOfDay() - toNanoOfDay();
            switch (unit) {
                case NANOS: return nanosUntil;
                case MICROS: return nanosUntil / 1000;
                case MILLIS: return nanosUntil / NANOS_PER_MILLI;
                case SECONDS: return nanosUntil / NANOS_PER_SECOND;
                case MINUTES: return nanosUntil / NANOS_PER_MINUTE;
                case HOURS: return nanosUntil / NANOS_PER_HOUR;
                case HALF_DAYS: return nanosUntil / (12 * NANOS_PER_HOUR);
                default:
                    TemporalException("Unsupported unit "_S + Temporal::toString(unit)).throws($ftrace());
            }
        }

        LocalDateTime LocalTime::atDate(LocalDate const& date) const {
            return LocalDateTime(date, *this);
        }

        gint LocalTime::toSecondOfDay() const {
            return hour() * SECONDS_PER_HOUR + minute() * SECONDS_PER_MINUTE + second();
        }

        glong LocalTime::toNanoOfDay() const {
            return hour() * NANOS_PER_HOUR + minute() * NANOS_PER_MINUTE + second() * NANOS_PER_SECOND + nano();
        }

        glong LocalTime::toEpochSecond(LocalDate const& date) const {
            return date.toEpochDay() * SECONDS_PER_DAY + toSecondOfDay();
        }

        glong LocalTime::toEpochSecond(LocalDate const& date, ZoneOffset const& offset) const {
            return date.toEpochDay() * SECONDS_PER_DAY + toSecondOfDay() + offset.totalSeconds();
        }

        gint LocalTime::compareTo(const LocalTime& other) const {
            return Long::compare(hms, other.hms);
        }

        gbool LocalTime::isAfter(LocalTime const& other) const {
            return hms > other.hms;
        }

        gbool LocalTime::isBefore(LocalTime const& other) const {
            return hms < other.hms;
        }

        Optional<> LocalTime::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::LOCAL_TIME)
                return clone();
            if (query == TemporalQuery::PRECISION)
                return Optional<Enum<ChronoUnit>>(NANOS);
            return Optional<>();
        }

        LocalTime LocalTime::from(Temporal const& temporal) {
            Optional<> time = temporal.query(TemporalQuery::LOCAL_DATE);
            if (time.isEmpty())
                DateTimeException("Unable to obtain LocalTime from Temporal: "_Sl +
                    temporal + " of type " + typeName(temporal)).throws($ftrace());
            LocalTime& localT = ((Optional<LocalTime>) time).get();
            LocalTime lt = localT;
            UNSAFE::deleteRegInstance(localT);
            return lt;
        }

        gbool LocalTime::equals(const Object& obj) const {
            return this == &obj ||
                    Class<LocalTime>::hasInstance(obj) && hms == CORE_XCAST(LocalTime const, obj).hms;
        }

        gint LocalTime::hash() const {
            return Long::hash(hms);
        }

        String LocalTime::toString() const {
            gint hour = LocalTime::hour();
            gint minute = LocalTime::minute();
            gint second = LocalTime::second();
            gint nano = LocalTime::nano();
            XString str = XString(18);
            str.append(hour < 10 ? "0"_S : ""_S).append(hour)
               .append(minute < 10 ? ":0"_S : ":"_S).append(minute);
            if (second > 0 || nano > 0) {
                str.append(second < 10 ? ":0"_S : ":"_S).append(second);
                if (nano > 0) {
                    str.append('.');
                    if (nano % 1000000 == 0)
                        str.append(Integer::toString(nano / 1000000 + 1000).subString(1));
                    else if (nano % 1000 == 0)
                        str.append(Integer::toString(nano / 1000 + 1000000).subString(1));
                    else
                        str.append(Integer::toString(nano + 1000000000).subString(1));
                }
            }
            return str.toString();
        }

        Object& LocalTime::clone() const {
            return UNSAFE::newInstance<LocalTime>(*this);
        }

        glong LocalTime::timeToCompactInt(gint hour, gint minute, gint second, gint nanoOfSecond) {
            glong time = 0;
            time |= (glong) hour << HOUR_OFFSET & HOUR_MASK;
            time |= (glong) minute << MINUTE_OFFSET & MINUTE_MASK;
            time |= (glong) second << SECOND_OFFSET & SECOND_MASK;
            time |= nanoOfSecond << NANOSECOND_OFFSET & NANOSECOND_MASK;
            return time;
        }
    } // time
} // core
