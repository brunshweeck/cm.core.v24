//
// Created by admin on 07/01/25.
//

#include "ZonedDateTime.h"

#include <core/Enum.h>
#include <core/time/Duration.h>
#include <core/time/OffsetDateTime.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZoneOffsetTransition.h>
#include <core/time/ZoneRules.h>
#include <core/util/List.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;

    namespace time {
        ZonedDateTime::ZonedDateTime(LocalDate const& date, LocalTime const& time, ZoneId const& zone)
            : ZonedDateTime(LocalDateTime(date, time), zone) {}

        ZonedDateTime::ZonedDateTime(LocalDateTime const& dateTime, ZoneId const& zone)
            : ZonedDateTime(ofLocal(dateTime, zone)) {}

        ZonedDateTime::ZonedDateTime(gint year, gint month, gint dayOfMonth,
                                     gint hour, gint minute,
                                     ZoneId const& zone)
            : ZonedDateTime(LocalDateTime(year, month, dayOfMonth, hour, minute), zone) {}

        ZonedDateTime::ZonedDateTime(gint year, gint month, gint dayOfMonth,
                                     gint hour, gint minute, gint second,
                                     ZoneId const& zone)
            : ZonedDateTime(LocalDateTime(year, month, dayOfMonth, hour, minute, second), zone) {}

        ZonedDateTime::ZonedDateTime(gint year, gint month, gint dayOfMonth,
                                     gint hour, gint minute, gint second, gint nanoOfSecond,
                                     ZoneId const& zone)
            : ZonedDateTime(LocalDateTime(year, month, dayOfMonth, hour, minute, second, nanoOfSecond), zone) {}

        ZonedDateTime::~ZonedDateTime() {
            if (Class<ZoneOffset>::hasInstance(zone())) {
                delete zId;
            }
            zId = null;
        }

        ZonedDateTime ZonedDateTime::ofLocal(LocalDateTime const& dateTime, ZoneId const& zone,
                                             ZoneOffset const& preferredOffset) {
            if (Class<ZoneOffset>::hasInstance(zone))
                return ZonedDateTime(dateTime, CORE_XCAST(ZoneOffset const, zone), zone);
            LocalDateTime localDT = dateTime;
            ZoneRules rules = zone.getRules();
            List<ZoneOffset>& validOffsets = rules.validOffsets(localDT);
            ZoneOffset offset = ZoneOffset::UTC;
            if (validOffsets.size() == 1)
                offset = validOffsets.get(0);
            else if (validOffsets.size() == 0) {
                ZoneOffsetTransition trans = rules.transition(localDT).get();
                localDT = localDT.plusSeconds(trans.duration().seconds());
                offset = trans.offsetAfter();
            } else {
                if (validOffsets.contains(preferredOffset))
                    offset = preferredOffset;
                else
                    offset = validOffsets.get(0);
            }
            return ZonedDateTime(localDT, offset, zone);
        }

        ZonedDateTime ZonedDateTime::ofInstant(LocalDateTime const& dateTime, ZoneOffset const& offset,
                                               ZoneId const& zone) {
            if (zone.getRules().isValidOffset(dateTime, offset))
                return ZonedDateTime(dateTime, offset, zone);

            return create(dateTime.toEpochSecond(offset), dateTime.nano(), zone);
        }

        ZonedDateTime ZonedDateTime::ofStrict(LocalDateTime const& dateTime, ZoneOffset const& offset,
                                              ZoneId const& zone) {
            ZoneRules rules = zone.getRules();
            if (rules.isValidOffset(dateTime, offset) == false) {
                auto trans = rules.transition(dateTime);
                if (trans.isPresent() && trans.get().isGap()) {
                    // error message says daylight savings for simplicity
                    // even though there are other kinds of gaps
                    DateTimeException("LocalDateTime '"_Sl + dateTime + "' does not exist in zone '"_Sl + zone +
                        "' due to a gap in the local time-line, typically "
                        "caused by daylight savings"_Sl).throws($ftrace());
                }
                DateTimeException("ZoneOffset '"_Sl + offset + "' is not valid for LocalDateTime '"_Sl +
                    dateTime + "' in zone '" + zone + "'").throws($ftrace());
            }
            return ZonedDateTime(dateTime, offset, zone);
        }

        ZonedDateTime ZonedDateTime::ofLenient(LocalDateTime const& dateTime, ZoneOffset const& offset,
                                               ZoneId const& zone) {
            if (Class<ZoneOffset>::hasInstance(zone) && offset.equals(zone) == false) {
                IllegalArgumentException("ZoneId must match ZoneOffset").throws($ftrace());
            }
            return ZonedDateTime(dateTime, offset, zone);
        }


        ZonedDateTime ZonedDateTime::from(Temporal const& temporal) {
            if (Class<ZonedDateTime>::hasInstance(temporal))
                return CORE_XCAST(ZonedDateTime const, temporal);
            try {
                ZoneId const& zone = ZoneId::from(temporal);
                if (temporal.isSupported(INSTANT_SECONDS)) {
                    glong epochSecond = temporal.getLong(INSTANT_SECONDS);
                    gint nanoOfSecond = temporal.get(NANO_OF_SECOND);

                    return create(epochSecond, nanoOfSecond, zone);
                }
                LocalDate localD = LocalDate::from(temporal);
                LocalTime localT = LocalTime::from(localD);
                return ZonedDateTime(localD, localT, zone);
            } catch (DateTimeException const& ex) {
                DateTimeException("Unable to obtain ZonedDateTime from Temporal: "_Sl +
                                  temporal + " of type " + typeName(temporal), ex).throws($ftrace());
            }
        }

        gbool ZonedDateTime::isSupported(ChronoField field) const {
            return true;
        }

        gbool ZonedDateTime::isSupported(ChronoUnit unit) const {
            return unit != FOREVER;
        }

        gint ZonedDateTime::get(ChronoField field) const {
            if (field == INSTANT_SECONDS)
                TemporalException("Invalid field 'InstantSeconds' for get() method, "
                    "use getLong() instead"_Sl).throws($ftrace());
            if (field == OFFSET_SECONDS)
                return zOffset.totalSeconds();
            try {
                return dateTime.get(field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong ZonedDateTime::getLong(ChronoField field) const {
            try {
                switch (field) {
                    case INSTANT_SECONDS: return toEpochSecond();
                    case OFFSET_SECONDS: return zOffset.totalSeconds();
                    default: return dateTime.getLong(field);
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneOffset ZonedDateTime::offset() const {
            return zOffset;
        }

        ZonedDateTime ZonedDateTime::withEarlierOffsetAtOverlap() const {
            Optional<ZoneOffsetTransition> trans = zone().getRules().transition(dateTime);
            if (trans.isPresent() && trans.get().isOverlap()) {
                ZoneOffset earlierOffset = trans.get().offsetBefore();
                if (earlierOffset.equals(zOffset) == false)
                    return ZonedDateTime(dateTime, earlierOffset, zone());
            }
            return *this;
        }

        ZonedDateTime ZonedDateTime::withLaterOffsetAtOverlap() const {
            Optional<ZoneOffsetTransition> trans = zone().getRules().transition(toLocalDateTime());
            if (trans.isPresent()) {
                ZoneOffset laterOffset = trans.get().offsetAfter();
                if (laterOffset.equals(zOffset) == false)
                    return ZonedDateTime(dateTime, laterOffset, zone());
            }
            return *this;
        }

        ZoneId const& ZonedDateTime::zone() const {
            return *zId;
        }

        ZonedDateTime ZonedDateTime::withZoneSameLocal(ZoneId const& zone) const {
            if (zone == ZonedDateTime::zone())
                return *this;
            try {
                return ofLocal(dateTime, zone, zOffset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::withZoneSameInstant(ZoneId const& zone) const {
            if (zone == ZonedDateTime::zone())
                return *this;
            return create(dateTime.toEpochSecond(zOffset), dateTime.nano(), zone);
        }

        ZonedDateTime ZonedDateTime::withFixedOffsetZone() const {
            if (zone().equals(zOffset))
                return *this;
            return ZonedDateTime(dateTime, zOffset, zOffset);
        }

        LocalDateTime ZonedDateTime::toLocalDateTime() const {
            return dateTime;
        }

        LocalDate ZonedDateTime::toLocalDate() const {
            return dateTime.toLocalDate();
        }

        gint ZonedDateTime::year() const {
            return dateTime.year();
        }

        LocalDate::Month ZonedDateTime::month() const {
            return dateTime.month();
        }

        gint ZonedDateTime::dayOfMonth() const {
            return dateTime.dayOfMonth();
        }

        gint ZonedDateTime::dayOfYear() const {
            return dateTime.dayOfYear();
        }

        LocalDate::DayOfWeek ZonedDateTime::dayOfWeek() const {
            return dateTime.dayOfWeek();
        }

        LocalTime ZonedDateTime::toLocalTime() const {
            return dateTime.toLocalTime();
        }

        gint ZonedDateTime::hour() const {
            return dateTime.hour();
        }

        gint ZonedDateTime::minute() const {
            return dateTime.minute();
        }

        gint ZonedDateTime::second() const {
            return dateTime.second();
        }

        gint ZonedDateTime::nano() const {
            return dateTime.nano();
        }

        ZonedDateTime ZonedDateTime::with(ChronoField field, glong newValue) const {
            try {
                switch (field) {
                    case INSTANT_SECONDS: return create(newValue, nano(), zone());
                    case OFFSET_SECONDS: return resolveOffset(ZoneOffset(checkValue(newValue, field)));
                    default: return resolveLocal(dateTime.with(field, newValue));
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::withYear(gint year) const {
            try {
                return resolveLocal(dateTime.withYear(year));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::withMonth(gint month) const {
            try {
                return resolveLocal(dateTime.withMonth(month));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::withDayOfMonth(gint dayOfMonth) const {
            try {
                return resolveLocal(dateTime.withDayOfMonth(dayOfMonth));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::withHour(gint hour) const {
            try {
                return resolveLocal(dateTime.withHour(hour));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::withMinute(gint minute) const {
            try {
                return resolveLocal(dateTime.withMinute(minute));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::withSecond(gint second) const {
            try {
                return resolveLocal(dateTime.withSecond(second));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::withNano(gint nanoOfSecond) const {
            try {
                return resolveLocal(dateTime.withNano(nanoOfSecond));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::truncateTo(ChronoUnit unit) const {
            try {
                return resolveLocal(dateTime.truncateTo(unit));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plus(glong amountToAdd, ChronoUnit unit) const {
            try {
                if (isDateBased(unit))
                    return resolveLocal(dateTime.plus(amountToAdd, unit));
                return resolveInstant(dateTime.plus(amountToAdd, unit));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plusYears(gint years) const {
            try {
                return resolveLocal(dateTime.plusYears(years));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plusMonths(gint months) const {
            try {
                return resolveLocal(dateTime.plusMonths(months));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plusWeeks(gint weeks) const {
            try {
                return resolveLocal(dateTime.plusWeeks(weeks));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plusDays(gint days) const {
            try {
                return resolveLocal(dateTime.plusDays(days));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plusHours(gint hours) const {
            try {
                return resolveInstant(dateTime.plusHours(hours));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plusMinutes(gint minutes) const {
            try {
                return resolveInstant(dateTime.plusMinutes(minutes));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plusSeconds(gint seconds) const {
            try {
                return resolveInstant(dateTime.plusSeconds(seconds));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::plusNanos(gint nanos) const {
            try {
                return resolveInstant(dateTime.plusNanos(nanos));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minus(glong amountToSubtract, ChronoUnit unit) const {
            try {
                return amountToSubtract == Long::MIN_VALUE
                           ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                           : plus(-amountToSubtract, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minusYears(gint years) const {
            try {
                return years == Long::MIN_VALUE ? plusYears(Long::MAX_VALUE).plusYears(1) : plusYears(-years);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minusMonths(gint months) const {
            try {
                return months == Long::MIN_VALUE ? plusMinutes(Long::MAX_VALUE).plusMinutes(1) : plusMinutes(-months);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minusWeeks(gint weeks) const {
            try {
                return weeks == Long::MIN_VALUE ? plusWeeks(Long::MAX_VALUE).plusWeeks(1) : plusWeeks(-weeks);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minusDays(gint days) const {
            try {
                return days == Long::MIN_VALUE ? plusDays(Long::MAX_VALUE).plusDays(1) : plusDays(-days);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minusHours(gint hours) const {
            try {
                return hours == Long::MIN_VALUE ? plusHours(Long::MAX_VALUE).plusHours(1) : plusHours(-hours);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minusMinutes(gint minutes) const {
            try {
                return minutes == Long::MIN_VALUE ? plusMinutes(Long::MAX_VALUE).plusMinutes(1) : plusMinutes(-minutes);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minusSeconds(gint seconds) const {
            try {
                return seconds == Long::MIN_VALUE ? plusSeconds(Long::MAX_VALUE).plusSeconds(1) : plusSeconds(-seconds);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::minusNanos(gint nanos) const {
            try {
                return nanos == Long::MIN_VALUE ? plusNanos(Long::MAX_VALUE).plusNanos(1) : plusNanos(-nanos);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Optional<> ZonedDateTime::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::LOCAL_DATE)
                return toLocalDate();
            if (query == TemporalQuery::LOCAL_TIME)
                return toLocalTime();
            if (query == TemporalQuery::ZONE || query == TemporalQuery::ZONE_ID)
                return zone();
            if (query == TemporalQuery::OFFSET)
                return zOffset;
            if (query == TemporalQuery::PRECISION)
                return Optional<Enum<ChronoUnit>>(NANOS);

            try {
                return query.queryFrom(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong ZonedDateTime::until(Temporal const& endExclusive, ChronoUnit unit) const {
            try {
                ZonedDateTime end = from(endExclusive);
                ZonedDateTime start = *this;
                try {
                    end = end.withZoneSameInstant(zone());
                } catch (DateTimeException const& _) {
                    // end may be out of valid range. Adjust to end's zone.
                    start = start.withZoneSameInstant(end.zone());
                }
                if (isDateBased(unit))
                    return start.dateTime.until(end.dateTime, unit);
                return start.toOffsetDateTime().until(end.toOffsetDateTime(), unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong ZonedDateTime::toEpochSecond() const {
            glong epochDay = toLocalDate().toEpochDay();
            glong secs = epochDay * 86400 + toLocalTime().toSecondOfDay();
            secs -= zOffset.totalSeconds();
            return secs;
        }

        OffsetDateTime ZonedDateTime::toOffsetDateTime() const {
            return OffsetDateTime(dateTime, zOffset);
        }

        gint ZonedDateTime::compareTo(ZonedDateTime const& other) const {
            gint cmp = Long::compare(toEpochSecond(), other.toEpochSecond());
            if (cmp == 0) {
                cmp = dateTime.nano() - other.dateTime.nano();
                if (cmp == 0) {
                    cmp = dateTime.compareTo(other.dateTime);
                    if (cmp == 0) {
                        cmp = zone().getId().compareTo(other.zone().getId());
                    }
                }
            }
            return cmp;
        }

        gbool ZonedDateTime::isAfter(ZonedDateTime const& other) const {
            glong thisEpochSecond = toEpochSecond();
            glong otherEpochSecond = other.toEpochSecond();
            return thisEpochSecond > otherEpochSecond ||
                    thisEpochSecond == otherEpochSecond && dateTime.nano() > other.dateTime.nano();
        }

        gbool ZonedDateTime::isBefore(ZonedDateTime const& other) const {
            glong thisEpochSecond = toEpochSecond();
            glong otherEpochSecond = other.toEpochSecond();
            return thisEpochSecond < otherEpochSecond ||
                    thisEpochSecond == otherEpochSecond && dateTime.nano() < other.dateTime.nano();
        }

        gbool ZonedDateTime::equals(Object const& other) const {
            if (this == &other)
                return true;
            if (!Class<ZonedDateTime>::hasInstance(other))
                return false;
            ZonedDateTime const& otherDT = CORE_XCAST(ZonedDateTime const, other);
            return dateTime.equals(otherDT.dateTime) &&
                    zOffset.equals(otherDT.zOffset) &&
                    zone().equals(otherDT.zone());
        }

        gint ZonedDateTime::hash() const {
            return dateTime.hash() ^ zOffset.hash() ^ Integer::rotateLeft(zone().hash(), 3);
        }

        String ZonedDateTime::toString() const {
            XString str = XString(40);
            str.append(dateTime)
               .append(zOffset);
            if (zOffset != zone())
                str.append('[').append(zone()).append(']');
            return str.toString();
        }

        Object& ZonedDateTime::clone() const {
            try {
                ZonedDateTime& clone = UNSAFE::newInstance<ZonedDateTime>(*this);
                if (Class<ZoneOffset>::hasInstance(clone.zone()))
                    clone.zId = new ZoneOffset(CORE_XCAST(ZoneOffset const, clone.zone()));
                return clone;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::create(glong epochSecond, gint nanoOfSecond, ZoneId const& zone) {
            // nanoOfSecond is in a range that'll not affect epochSecond, validated
            // by LocalDateTime.ofEpochSecond
            ZoneOffset offset = zone.getOffset(epochSecond);
            LocalDateTime localDT = LocalDateTime::ofEpochSecond(epochSecond, nanoOfSecond, offset);

            return ZonedDateTime(localDT, offset, zone);
        }

        ZonedDateTime::ZonedDateTime(LocalDateTime const& dateTime, ZoneOffset const& offset, ZoneId const& zone)
            : dateTime(dateTime), zOffset(offset) {
            if (Class<ZoneOffset>::hasInstance(zone))
                zId = new ZoneOffset(CORE_XCAST(ZoneOffset const, zone));
            else
                zId = &(ZoneId&) zone;
        }

        ZonedDateTime ZonedDateTime::ofLocal(LocalDateTime const& dateTime, ZoneId const& zone) {
            if (Class<ZoneOffset>::hasInstance(zone))
                return ZonedDateTime(dateTime, CORE_XCAST(ZoneOffset const, zone), zone);
            LocalDateTime localDT = dateTime;
            ZoneRules rules = zone.getRules();
            List<ZoneOffset>& validOffsets = rules.validOffsets(localDT);
            ZoneOffset offset = ZoneOffset::UTC;
            if (validOffsets.size() == 1)
                offset = validOffsets.get(0);
            else if (validOffsets.size() == 0) {
                ZoneOffsetTransition trans = rules.transition(localDT).get();
                localDT = localDT.plusSeconds(trans.duration().seconds());
                offset = trans.offsetAfter();
            } else {
                offset = validOffsets.get(0);
            }
            return ZonedDateTime(localDT, offset, zone);
        }

        ZonedDateTime ZonedDateTime::resolveOffset(ZoneOffset const& offset) const {
            if (offset.equals(zOffset) == false && zone().getRules().isValidOffset(dateTime, offset)) {
                return ZonedDateTime(dateTime, zOffset, zone());
            }
            return *this;
        }

        ZonedDateTime ZonedDateTime::resolveLocal(LocalDateTime const& newDateTime) const {
            return ofLocal(newDateTime, zone(), zOffset);
        }

        ZonedDateTime ZonedDateTime::resolveInstant(LocalDateTime const& newDateTime) const {
            return ofInstant(newDateTime, zOffset, zone());
        }
    } // time
} // core
