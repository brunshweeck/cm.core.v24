//
// Created by admin on 07/01/25.
//

#include "OffsetDateTime.h"

#include <core/Enum.h>
#include <core/time/OffsetTime.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZonedDateTime.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;

    namespace time {
        OffsetDateTime const OffsetDateTime::MAX = OffsetDateTime(MAX_YEAR, MAX_MONTH, MAX_DAY_OF_MONTH,
                                                                  MAX_HOUR, MAX_MINUTE, MAX_SECOND, MAX_NANOSECOND,
                                                                  ZoneOffset(MIN_OFFSET_SECONDS));

        OffsetDateTime const OffsetDateTime::MIN = OffsetDateTime(MIN_YEAR, MIN_MONTH, MIN_DAY_OF_MONTH,
                                                                  MIN_HOUR, MIN_MINUTE, MIN_SECOND, MIN_NANOSECOND,
                                                                  ZoneOffset(MAX_OFFSET_SECONDS));

        OffsetDateTime::OffsetDateTime(LocalDate const& date, LocalTime const& time, ZoneOffset const& offset)
            : dateTime(date, time), zOffset(offset) {}

        OffsetDateTime::OffsetDateTime(LocalDateTime const& dateTime, ZoneOffset const& offset)
            : dateTime(dateTime), zOffset(offset) {}

        OffsetDateTime::OffsetDateTime(gint year, gint month, gint dayOfMonth,
                                       gint hour, gint minute,
                                       ZoneOffset const& offset)
            : dateTime(year, month, dayOfMonth, hour, minute), zOffset(offset) {}

        OffsetDateTime::OffsetDateTime(gint year, gint month, gint dayOfMonth,
                                       gint hour, gint minute, gint second,
                                       ZoneOffset const& offset)
            : dateTime(year, month, dayOfMonth, hour, minute, second), zOffset(offset) {}

        OffsetDateTime::OffsetDateTime(gint year, gint month, gint dayOfMonth,
                                       gint hour, gint minute, gint second, gint nanoOfSecond,
                                       ZoneOffset const& offset)
            : dateTime(year, month, dayOfMonth, hour, minute, second, nanoOfSecond), zOffset(offset) {}

        OffsetDateTime OffsetDateTime::from(Temporal const& temporal) {
            if (Class<OffsetDateTime>::hasInstance(temporal))
                return CORE_XCAST(OffsetDateTime const, temporal);
            try {
                ZoneOffset offset = ZoneOffset::from(temporal);
                Optional<LocalDate> localDate = (Optional<LocalDate>) temporal.query(TemporalQuery::LOCAL_DATE);
                Optional<LocalTime> localTime = (Optional<LocalTime>) temporal.query(TemporalQuery::LOCAL_TIME);
                if (localDate.isPresent() && localTime.isPresent()) {
                    LocalDate date = localDate.get();
                    LocalTime time = localTime.get();
                    UNSAFE::deleteRegInstance(localDate.get());
                    UNSAFE::deleteRegInstance(localTime.get());

                    return OffsetDateTime(date, time, offset);
                }
                if (localDate.isPresent())
                    UNSAFE::deleteRegInstance(localDate.get());
                if (localTime.isPresent())
                    UNSAFE::deleteRegInstance(localTime.get());

                return OffsetDateTime(LocalDateTime(
                                          temporal.get(YEAR),
                                          temporal.get(MONTH_OF_YEAR),
                                          temporal.get(DAY_OF_MONTH),
                                          temporal.get(HOUR_OF_DAY),
                                          temporal.get(MINUTE_OF_HOUR),
                                          temporal.get(SECOND_OF_MINUTE),
                                          temporal.get(NANO_OF_SECOND)
                                      ), offset);
            } catch (DateTimeException const& ex) { ex.throws($ftrace()); }
        }

        gbool OffsetDateTime::isSupported(ChronoField field) const {
            return true;
        }

        gbool OffsetDateTime::isSupported(ChronoUnit unit) const {
            return unit != FOREVER;
        }

        gint OffsetDateTime::get(ChronoField field) const {
            if (field == INSTANT_SECONDS)
                TemporalException("Value out of range").throws($ftrace());
            if (field == OFFSET_SECONDS)
                return zOffset.totalSeconds();
            try {
                return dateTime.get(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong OffsetDateTime::getLong(ChronoField field) const {
            if (field == INSTANT_SECONDS)
                return toEpochSecond();
            if (field == OFFSET_SECONDS)
                return zOffset.totalSeconds();
            try {
                return dateTime.get(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneOffset OffsetDateTime::offset() const {
            return zOffset;
        }

        OffsetDateTime OffsetDateTime::withOffsetSameLocal(ZoneOffset const& offset) const {
            return OffsetDateTime(dateTime, offset);
        }

        OffsetDateTime OffsetDateTime::withOffsetSameInstant(ZoneOffset const& offset) const {
            if (zOffset == offset)
                return *this;
            gint difference = -zOffset.totalSeconds() + offset.totalSeconds();
            LocalDateTime adjusted = dateTime.plusSeconds(difference);
            return OffsetDateTime(adjusted, offset);
        }

        LocalDateTime OffsetDateTime::toLocalDateTime() const {
            return dateTime;
        }

        LocalDate OffsetDateTime::toLocalDate() const {
            return dateTime.toLocalDate();
        }

        gint OffsetDateTime::year() const {
            return dateTime.year();
        }

        LocalDate::Month OffsetDateTime::month() const {
            return dateTime.month();
        }

        gint OffsetDateTime::dayOfMonth() const {
            return dateTime.dayOfMonth();
        }

        gint OffsetDateTime::dayOfYear() const {
            return dateTime.dayOfYear();
        }

        LocalDate::DayOfWeek OffsetDateTime::dayOfWeek() const {
            return dateTime.dayOfWeek();
        }

        LocalTime OffsetDateTime::toLocalTime() const {
            return dateTime.toLocalTime();
        }

        gint OffsetDateTime::hour() const {
            return dateTime.hour();
        }

        gint OffsetDateTime::minute() const {
            return dateTime.minute();
        }

        gint OffsetDateTime::second() const {
            return dateTime.second();
        }

        gint OffsetDateTime::nano() const {
            return dateTime.nano();
        }

        OffsetDateTime OffsetDateTime::with(ChronoField field, glong newValue) const {
            if (field == INSTANT_SECONDS)
                TemporalException("Value out of range").throws($ftrace());
            try {
                if (field == OFFSET_SECONDS)
                    return OffsetDateTime(dateTime, ZoneOffset(checkValue(newValue, OFFSET_SECONDS)));
                return OffsetDateTime(dateTime.with(field, newValue), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::withYear(gint year) const {
            try {
                return OffsetDateTime(dateTime.withYear(year), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::withMonth(gint month) const {
            try {
                return OffsetDateTime(dateTime.withMonth(month), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::withDayOfMonth(gint dayOfMonth) const {
            try {
                return OffsetDateTime(dateTime.withDayOfMonth(dayOfMonth), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::withDayOfYear(gint dayOfYear) const {
            try {
                return OffsetDateTime(dateTime.withDayOfYear(dayOfYear), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::withHour(gint hour) const {
            try {
                return OffsetDateTime(dateTime.withHour(hour), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::withMinute(gint minute) const {
            try {
                return OffsetDateTime(dateTime.withMinute(minute), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::withSecond(gint second) const {
            try {
                return OffsetDateTime(dateTime.withSecond(second), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::withNano(gint nano) const {
            try {
                return OffsetDateTime(dateTime.withNano(nano), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::truncateTo(ChronoUnit unit) const {
            try {
                return OffsetDateTime(dateTime.truncateTo(unit), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plus(glong amountToAdd, ChronoUnit unit) const {
            try {
                return OffsetDateTime(dateTime.plus(amountToAdd, unit), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plusYears(gint years) const {
            try {
                return OffsetDateTime(dateTime.plusYears(years), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plusMonths(gint months) const {
            try {
                return OffsetDateTime(dateTime.plusMonths(months), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plusWeeks(gint weeks) const {
            try {
                return OffsetDateTime(dateTime.plusWeeks(weeks), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plusDays(gint days) const {
            try {
                return OffsetDateTime(dateTime.plusDays(days), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plusHours(gint hours) const {
            try {
                return OffsetDateTime(dateTime.plusHours(hours), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plusMinutes(gint minutes) const {
            try {
                return OffsetDateTime(dateTime.plusMinutes(minutes), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plusSeconds(gint seconds) const {
            try {
                return OffsetDateTime(dateTime.plusSeconds(seconds), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::plusNanos(gint nanos) const {
            try {
                return OffsetDateTime(dateTime.plusNanos(nanos), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minus(glong amountToSubtract, ChronoUnit unit) const {
            try {
                return amountToSubtract == Long::MIN_VALUE
                           ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                           : plus(-amountToSubtract, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minusYears(gint years) const {
            try {
                return years == Long::MIN_VALUE ? plusYears(Long::MAX_VALUE).plusYears(1) : plusYears(-years);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minusMonths(gint months) const {
            try {
                return months == Long::MIN_VALUE ? plusMonths(Long::MAX_VALUE).plusMonths(1) : plusMonths(-months);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minusWeeks(gint weeks) const {
            try {
                return weeks == Long::MIN_VALUE ? plusWeeks(Long::MAX_VALUE).plusWeeks(1) : plusWeeks(-weeks);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minusDays(gint days) const {
            try {
                return days == Long::MIN_VALUE ? plusDays(Long::MAX_VALUE).plusDays(1) : plusDays(-days);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minusHours(gint hours) const {
            try {
                return hours == Long::MIN_VALUE ? plusHours(Long::MAX_VALUE).plusHours(1) : plusHours(-hours);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minusMinutes(gint minutes) const {
            try {
                return minutes == Long::MIN_VALUE ? plusMinutes(Long::MAX_VALUE).plusMinutes(1) : plusMinutes(-minutes);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minusSeconds(gint seconds) const {
            try {
                return seconds == Long::MIN_VALUE ? plusSeconds(Long::MAX_VALUE).plusSeconds(1) : plusSeconds(-seconds);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime OffsetDateTime::minusNanos(gint nanos) const {
            try {
                return nanos == Long::MIN_VALUE ? plusNanos(Long::MAX_VALUE).plusNanos(1) : plusNanos(-nanos);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Optional<> OffsetDateTime::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::OFFSET || query == TemporalQuery::ZONE)
                return Optional<ZoneOffset>(zOffset);
            if (query == TemporalQuery::LOCAL_DATE)
                return Optional<LocalDate>(dateTime.toLocalDate());
            if (query == TemporalQuery::LOCAL_TIME)
                return Optional<LocalTime>(dateTime.toLocalTime());
            if (query == TemporalQuery::PRECISION)
                return Optional<Enum<ChronoUnit>>(NANOS);
            if (query == TemporalQuery::ZONE_ID)
                return Optional<>();

            try {
                return query.queryFrom(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong OffsetDateTime::until(Temporal const& endExclusive, ChronoUnit unit) const {
            OffsetDateTime end = MIN;
            try {
                end = from(endExclusive);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            OffsetDateTime start = *this;
            try {
                end = end.withOffsetSameInstant(zOffset);
            } catch (DateTimeException const& _) {
                // end may be out of valid range. Adjust to end's offset.
                try {
                    start = withOffsetSameInstant(end.zOffset);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
            try {
                return start.dateTime.until(end.dateTime, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime OffsetDateTime::atZoneSameInstant(ZoneId const& region) const {
            try {
                return ZonedDateTime::ofInstant(dateTime, zOffset, region);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime OffsetDateTime::atZoneSimilarLocal(ZoneId const& region) const {
            try {
                return ZonedDateTime::ofLocal(dateTime, region, zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetDateTime::toOffsetTime() const {
            try {
                return OffsetTime(dateTime.toLocalTime(), zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime OffsetDateTime::toZonedDateTime() const {
            return ZonedDateTime(dateTime, zOffset);
        }

        glong OffsetDateTime::toEpochSecond() const {
            return dateTime.toEpochSecond(zOffset);
        }

        gint OffsetDateTime::compareTo(OffsetDateTime const& other) const {
            if (zOffset == other.zOffset)
                return dateTime.compareTo(other.dateTime);
            gint cmp = Long::compare(toEpochSecond(), other.toEpochSecond());
            if (cmp == 0)
                cmp = dateTime.toLocalTime().nano() - other.dateTime.toLocalTime().nano();
            return cmp;
        }

        gbool OffsetDateTime::isAfter(OffsetDateTime const& other) const {
            return compareTo(other) > 0;
        }

        gbool OffsetDateTime::isBefore(OffsetDateTime const& other) const {
            return compareTo(other) < 0;
        }

        gbool OffsetDateTime::equals(Object const& other) const {
            if (this == &other)
                return true;
            if (!Class<OffsetDateTime>::hasInstance(other))
                return false;
            OffsetDateTime const& otherDT = CORE_XCAST(OffsetDateTime const, other);
            return zOffset == otherDT.zOffset && dateTime == otherDT.dateTime;
        }

        gint OffsetDateTime::hash() const {
            return dateTime.hash() ^ zOffset.hash();
        }

        String OffsetDateTime::toString() const {
            return dateTime.toString() + zOffset.toString();
        }

        Object& OffsetDateTime::clone() const {
            try {
                return UNSAFE::newInstance<OffsetDateTime>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // time
} // core
