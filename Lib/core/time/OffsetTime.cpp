//
// Created by admin on 07/01/25.
//

#include "OffsetTime.h"

#include <core/util/Optional.h>
#include "OffsetDateTime.h"
#include "TemporalQuery.h"

namespace core {
    using namespace util;

    namespace time {
        OffsetTime const OffsetTime::MAX = OffsetTime(MAX_HOUR, MAX_MINUTE, MAX_SECOND, MAX_NANOSECOND,
                                                      ZoneOffset(MIN_OFFSET_SECONDS));
        OffsetTime const OffsetTime::MIN = OffsetTime(MAX_HOUR, MAX_MINUTE, MAX_SECOND, MAX_NANOSECOND,
                                                      ZoneOffset(MAX_OFFSET_SECONDS));


        OffsetTime::OffsetTime(LocalTime const& time, ZoneOffset const& offset)
            : time(time), zone(offset) {}

        OffsetTime::OffsetTime(gint hour, gint minute, ZoneOffset const& offset)
            : time(hour, minute), zone(offset) {}

        OffsetTime::OffsetTime(gint hour, gint minute, gint second, ZoneOffset const& offset)
            : time(hour, minute, second), zone(offset) {}

        OffsetTime::OffsetTime(gint hour, gint minute, gint second, gint nanoOfSecond, ZoneOffset const& offset)
            : time(hour, minute, second, nanoOfSecond), zone(offset) {}

        OffsetTime OffsetTime::from(Temporal const& temporal) {
            if (Class<OffsetTime>::hasInstance(temporal))
                return CORE_XCAST(OffsetTime const, temporal);
            try {
                LocalTime time = LocalTime::from(temporal);
                ZoneOffset zone = ZoneOffset::from(temporal);
                return OffsetTime(time, zone);
            } catch (DateTimeException const& ex) {
                DateTimeException("Unable to obtain OffsetTime from Temporal "_Sl +
                                  temporal + " of type " + typeName(temporal), ex).throws($ftrace());
            }
        }

        gbool OffsetTime::isSupported(ChronoField field) const {
            return field == OFFSET_SECONDS || time.isSupported(field);
        }

        gbool OffsetTime::isSupported(ChronoUnit unit) const {
            return time.isSupported(unit);
        }

        gint OffsetTime::get(ChronoField field) const {
            try {
                return Temporal::get(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong OffsetTime::getLong(ChronoField field) const {
            if (field == OFFSET_SECONDS)
                return zone.totalSeconds();
            try {
                return time.getLong(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneOffset OffsetTime::offset() const {
            return zone;
        }

        OffsetTime OffsetTime::withOffsetSameLocal(ZoneOffset const& offset) const {
            return OffsetTime(time, offset);
        }

        OffsetTime OffsetTime::withOffsetSameInstant(ZoneOffset const& offset) const {
            if (zone == offset)
                return *this;
            gint difference = -zone.totalSeconds() + offset.totalSeconds();
            LocalTime adjusted = time.plusSeconds(difference);
            return OffsetTime(adjusted, offset);
        }

        LocalTime OffsetTime::toLocalTime() const {
            return time;
        }

        gint OffsetTime::hour() const {
            return time.hour();
        }

        gint OffsetTime::minute() const {
            return time.minute();
        }

        gint OffsetTime::second() const {
            return time.second();
        }

        gint OffsetTime::nano() const {
            return time.nano();
        }

        OffsetTime OffsetTime::with(ChronoField field, glong newValue) const {
            try {
                if (field == OFFSET_SECONDS)
                    return OffsetTime(time, ZoneOffset(checkValue(newValue, field)));

                return OffsetTime(time.with(field, newValue), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::withHour(gint hour) const {
            try {
                return OffsetTime(time.withHour(hour), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::withMinute(gint minute) const {
            try {
                return OffsetTime(time.withMinute(minute), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::withSecond(gint second) const {
            try {
                return OffsetTime(time.withSecond(second), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::withNano(gint nanoOfSecond) const {
            try {
                return OffsetTime(time.withNano(nanoOfSecond), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::truncateTo(ChronoUnit unit) const {
            try {
                return OffsetTime(time.truncateTo(unit), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::plus(glong amountToAdd, ChronoUnit unit) const {
            try {
                return OffsetTime(time.plus(amountToAdd, unit), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::plusHours(glong hours) const {
            try {
                return OffsetTime(time.plusHours(hours), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::plusMinutes(glong minutes) const {
            try {
                return OffsetTime(time.plusMinutes(minutes), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::plusSeconds(glong seconds) const {
            try {
                return OffsetTime(time.plusSeconds(seconds), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::plusNanos(gint nanos) const {
            try {
                return OffsetTime(time.plusNanos(nanos), zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::minus(glong amountToSubtract, ChronoUnit unit) const {
            try {
                return amountToSubtract == Long::MIN_VALUE
                           ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                           : plus(-amountToSubtract, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::minusHours(glong hours) const {
            try {
                return hours == Long::MIN_VALUE ? plusHours(Long::MAX_VALUE).plusHours(1) : plusHours(-hours);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::minusMinutes(glong minutes) const {
            try {
                return minutes == Long::MIN_VALUE ? plusMinutes(Long::MAX_VALUE).plusMinutes(1) : plusMinutes(-minutes);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::minusSeconds(glong seconds) const {
            try {
                return seconds == Long::MIN_VALUE ? plusSeconds(Long::MAX_VALUE).plusSeconds(1) : plusSeconds(-seconds);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::minusNanos(gint nanos) const {
            try {
                return nanos == Long::MIN_VALUE ? plusSeconds(Long::MAX_VALUE).plusNanos(1) : plusSeconds(-nanos);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Optional<> OffsetTime::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::OFFSET || query == TemporalQuery::ZONE)
                return zone;
            if (query == TemporalQuery::ZONE_ID)
                return Optional<>();
            if (query == TemporalQuery::LOCAL_TIME)
                return toLocalTime();
            if (query == TemporalQuery::LOCAL_DATE)
                return Optional<>();
            return query.queryFrom(*this);
        }

        glong OffsetTime::until(Temporal const& endExclusive, ChronoUnit unit) const {
            OffsetTime end = MIN;
            try {
                end = from(endExclusive);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            glong nanoUntil = (end.time.toNanoOfDay() - end.zone.totalSeconds() * LocalTime::NANOS_PER_SECOND)
                    - (time.toNanoOfDay() - zone.totalSeconds() * LocalTime::NANOS_PER_SECOND);
            switch (unit) {
                case NANOS: return nanoUntil;
                case MICROS: return nanoUntil / 1000;
                case MILLIS: return nanoUntil / LocalTime::NANOS_PER_MILLI;
                case SECONDS: return nanoUntil / LocalTime::NANOS_PER_SECOND;
                case MINUTES: return nanoUntil / LocalTime::NANOS_PER_MINUTE;
                case HOURS: return nanoUntil / LocalTime::NANOS_PER_HOUR;
                case HALF_DAYS: return nanoUntil / (12 * LocalTime::NANOS_PER_HOUR);
                default: TemporalException("Unsupported unit: " + Temporal::toString(unit)).throws($ftrace());
            }
        }

        OffsetDateTime OffsetTime::atDate(LocalDate const& date) const {
            return OffsetDateTime(date, time, zone);
        }

        glong OffsetTime::toEpochSecond(LocalDate const& date) const {
            glong epochDay = date.toEpochDay();
            glong seconds = epochDay * 86400 + time.toSecondOfDay();
            seconds -= zone.totalSeconds();
            return seconds;
        }

        gint OffsetTime::compareTo(OffsetTime const& other) const {
            if (zone == other.zone)
                return time.compareTo(other.time);
            gint cmp = Long::compare(time.toNanoOfDay() - zone.totalSeconds(),
                                     other.time.toNanoOfDay() - other.zone.totalSeconds());
            if (cmp == 0)
                return time.compareTo(other.time);
            return cmp;
        }

        gbool OffsetTime::isAfter(OffsetTime const& other) const {
            return compareTo(other) > 0;
        }

        gbool OffsetTime::isBefore(OffsetTime const& other) const {
            return compareTo(other) < 0;
        }

        gbool OffsetTime::equals(Object const& other) const {
            if (this == &other)
                return true;
            if (!Class<OffsetTime>::hasInstance(other))
                return false;
            OffsetTime const& otherOffset = CORE_XCAST(OffsetTime const, other);
            return time.equals(otherOffset.time) && zone.equals(otherOffset.zone);
        }

        gint OffsetTime::hash() const {
            return time.hash() ^ zone.hash();
        }

        String OffsetTime::toString() const {
            return time.toString() + zone.toString();
        }

        Object& OffsetTime::clone() const {
            try {
                return UNSAFE::newInstance<OffsetTime>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // time
} // core
