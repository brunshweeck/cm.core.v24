//
// Created by brunshweeck on 11/02/25.
//

#include "Instant.h"

#include <core/lang/Enum.h>
#include <core/time/DateTimeException.h>
#include <core/time/Duration.h>
#include <core/time/OffsetDateTime.h>
#include <core/time/TemporalAmount.h>
#include <core/time/TemporalException.h>
#include <core/time/TemporalQuery.h>
#include <core/time/TemporalUnit.h>
#include <core/time/ZonedDateTime.h>
#include <core/time/ValueRange.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>
#include <native/System.h>

namespace core {
    namespace time {
        CORE_ALIAS(Fields, TemporalUtils::Fields);
        CORE_ALIAS(Units, TemporalUtils::Units);

        Instant const Instant::EPOCH = Instant(0, 0);

        Instant const Instant::MIN = Instant::ofEpochSecond(MIN_SECOND, 0);

        Instant const Instant::MAX = Instant::ofEpochSecond(MAX_SECOND, 999999999);

        Instant::Instant(glong epochSecond, gint nanos) : seconds(epochSecond), nanos(nanos) {}

        Instant Instant::now() {
          TICK_COUNT timestamp = GetTimeStamp();
          return Instant::ofEpochSecond(timestamp.dwSeconds, timestamp.dwNano);
        }

        Instant Instant::ofEpochSecond(glong epochSecond) {
            try {
                return create(epochSecond, 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::ofEpochSecond(glong epochSecond, glong nanoAdjustment) {
            try {
                glong seconds = Math::addExact(epochSecond, (glong) Math::floorDiv(nanoAdjustment, NANOS_PER_SECOND));
                gint nanos = Math::floorMod(nanoAdjustment, NANOS_PER_SECOND);

                return create(seconds, nanos);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::ofEpochMilli(glong epochMilli) {
            try {
                glong seconds = Math::floorDiv(epochMilli, MILLIS_PER_SECOND);
                gint millis = Math::floorMod(epochMilli, MILLIS_PER_SECOND);

                return create(seconds, millis * NANOS_PER_MILLI);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::from(const TemporalAccessor& temporal) {
            if (Class<Instant>::hasInstance(temporal))
                return CORE_XCAST(Instant const, temporal);
            try {
                glong seconds = temporal.getLong(TemporalField::INSTANT_SECONDS);
                gint nanos = temporal.get(TemporalField::NANO_OF_SECOND);

                return Instant::ofEpochSecond(seconds, nanos);
            } catch (Throwable const& ex) {
                DateTimeException("Unable to obtain Instant from TemporalAccessor: "_Sl +
                                  temporal + " of type " + typeName(temporal), ex).throws($ftrace());
            }
        }

        gbool Instant::isSupported(TemporalField field) const {
            return field == TemporalField::INSTANT_SECONDS ||
                    field == TemporalField::NANO_OF_SECOND ||
                    field == TemporalField::MICRO_OF_SECOND ||
                    field == TemporalField::MILLI_OF_SECOND;
        }

        gbool Instant::isSupported(TemporalUnit unit) const {
            return Units::isTimeBased(unit) || unit == TemporalUnit::DAYS;
        }

        ValueRange Instant::range(TemporalField field) const {
            if (isSupported(field))
                return Fields::range(field);
            TemporalException("Unsupported field"_Sl + field).throws($ftrace());
        }

        gint Instant::get(TemporalField field) const {
            switch (field) {
                case TemporalField::NANO_OF_SECOND:
                    return nanos;
                case TemporalField::MICRO_OF_SECOND:
                    return nanos / 1000;
                case TemporalField::MILLI_OF_SECOND:
                    return nanos / NANOS_PER_MILLI;
                case TemporalField::INSTANT_SECONDS:
                    if (seconds != (gint) seconds)
                        return (gint) seconds;
                default:
                    TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
            }
        }

        glong Instant::getLong(TemporalField field) const {
            switch (field) {
                case TemporalField::NANO_OF_SECOND:
                    return nanos;
                case TemporalField::MICRO_OF_SECOND:
                    return nanos / 1000;
                case TemporalField::MILLI_OF_SECOND:
                    return nanos / NANOS_PER_MILLI;
                case TemporalField::INSTANT_SECONDS:
                    return seconds;
                default:
                    TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
            }
        }

        glong Instant::epochSecond() const {
            return seconds;
        }

        gint Instant::nano() const {
            return nanos;
        }

        Instant Instant::with(TemporalAdjuster const& adjuster) const {
            try {
                Temporal& t = adjuster.adjustInto(*this);
                Instant instant = CORE_XCAST(Instant, t);
                if (&t != this)
                    UNSAFE::deleteRegInstance(t);
                return instant;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::with(TemporalField field, glong newValue) const {
            try {
                Fields::checkValue(newValue, field);
                switch (field) {
                    case TemporalField::NANO_OF_SECOND:
                        return create(seconds, (gint) newValue);
                    case TemporalField::MICRO_OF_SECOND:
                        return create(seconds, (gint) newValue * 1000);
                    case TemporalField::MILLI_OF_SECOND:
                        return create(seconds, (gint) newValue * NANOS_PER_MILLI);
                    case TemporalField::INSTANT_SECONDS:
                        return create(newValue, nanos);
                    default:
                        break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
        }

        Instant Instant::truncatedTo(TemporalUnit unit) const {
            switch (unit) {
                case TemporalUnit::NANOS:
                    return *this;
                case TemporalUnit::MICROS:
                    return create(seconds, nanos / 1000);
                case TemporalUnit::MILLIS:
                    return create(seconds, nanos / NANOS_PER_MILLI);
                case TemporalUnit::SECONDS:
                    return create(seconds, 0);
                case TemporalUnit::MINUTES:
                    return create((seconds / SECONDS_PER_MINUTE) * SECONDS_PER_MINUTE, 0);
                case TemporalUnit::HOURS:
                    return create((seconds / SECONDS_PER_HOUR) * SECONDS_PER_HOUR, 0);
                case TemporalUnit::HALF_DAYS:
                    return create((seconds / (SECONDS_PER_DAY / 2)) * (SECONDS_PER_DAY / 2), 0);
                case TemporalUnit::DAYS:
                    return create((seconds / SECONDS_PER_DAY) * SECONDS_PER_DAY, 0);
                default:
                    TemporalException("Unit is too large to be used for truncation"_Sl).throws($ftrace());
            }
        }

        Instant Instant::plus(const TemporalAmount& amountToAdd) const {
            try {
                Temporal& r = amountToAdd.addTo(*this);
                Instant i = CORE_XCAST(Instant, r);
                if (this != &r)
                    UNSAFE::deleteRegInstance(r);

                return i;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::plus(glong amountToAdd, TemporalUnit unit) const {
            try {
                switch (unit) {
                    case TemporalUnit::NANOS: return plusNanos(amountToAdd);
                    case TemporalUnit::MICROS: return plusSeconds(amountToAdd / MICROS_PER_SECOND)
                                .plusNanos((amountToAdd % MICROS_PER_SECOND) * 1000);
                    case TemporalUnit::MILLIS: return plusSeconds(amountToAdd / 1000)
                                .plusNanos((amountToAdd % 1000) * NANOS_PER_MILLI);
                    case TemporalUnit::SECONDS: return plusSeconds(amountToAdd);
                    case TemporalUnit::MINUTES:
                        return plusSeconds(Math::multiplyExact(amountToAdd, SECONDS_PER_MINUTE));
                    case TemporalUnit::HOURS:
                        return plusSeconds(Math::multiplyExact(amountToAdd, SECONDS_PER_HOUR));
                    case TemporalUnit::HALF_DAYS:
                        return plusSeconds(Math::multiplyExact(amountToAdd, SECONDS_PER_HOUR * 12));
                    case TemporalUnit::DAYS:
                        return plusSeconds(Math::multiplyExact(amountToAdd, SECONDS_PER_DAY));
                    default: break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
        }

        Instant Instant::plusSeconds(glong secondsToAdd) const {
            try {
                return plus(secondsToAdd, 0L);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::plusMillis(glong millisToAdd) const {
            try {
                gint secondsToAdd = Math::floorDiv(millisToAdd, MILLIS_PER_SECOND);
                gint nanosToAdd = Math::floorMod(millisToAdd, MILLIS_PER_SECOND);
                return plus(secondsToAdd, nanosToAdd * NANOS_PER_MILLI);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::plusNanos(glong nanosToAdd) const {
            try {
                return plus(0L, nanosToAdd);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::minus(TemporalAmount const& amountToSubtract) const {
            try {
                Temporal& r = amountToSubtract.subtractFrom(*this);
                Instant i = CORE_XCAST(Instant, r);
                if (this != &r)
                    UNSAFE::deleteRegInstance(r);

                return i;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::minus(glong amountToSubtract, TemporalUnit unit) const {
            try {
                return (amountToSubtract == Long::MIN_VALUE)
                           ? plus(Long::MAX_VALUE, unit).plus(1L, unit)
                           : plus(-amountToSubtract, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::minusSeconds(glong secondsToSubtract) const {
            try {
                return (secondsToSubtract == Long::MIN_VALUE)
                           ? plusSeconds(Long::MAX_VALUE).plusSeconds(1L)
                           : plusSeconds(-secondsToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::minusMillis(glong millisToSubtract) const {
            try {
                return (millisToSubtract == Long::MIN_VALUE)
                           ? plusMillis(Long::MAX_VALUE).plusMillis(1L)
                           : plusMillis(-millisToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Instant Instant::minusNanos(glong nanosToSubtract) const {
            try {
                return (nanosToSubtract == Long::MIN_VALUE)
                           ? plusNanos(Long::MAX_VALUE).plusNanos(1L)
                           : plusNanos(-nanosToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Temporal::Optional Instant::query(const TemporalQuery& query) const {
            if (query == TemporalQuery::precision())
                return (Enum<TemporalUnit>) TemporalUnit::NANOS;
            if (query == TemporalQuery::localTime() ||
                query == TemporalQuery::localDate() ||
                query == TemporalQuery::zone() ||
                query == TemporalQuery::zoneId() ||
                query == TemporalQuery::offset())
                return Optional::empty();

            try {
                return query.queryFrom(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Temporal& Instant::adjustInto(Temporal const& temporal) const {
            try {
                Temporal& t1 = adjustFieldTo(TemporalField::INSTANT_SECONDS, seconds, temporal);
                Temporal& t2 = adjustFieldTo(TemporalField::NANO_OF_SECOND, nanos, t1);
                if (&t1 != &t2 && &t1 != &temporal)
                    UNSAFE::deleteRegInstance(t1);
                return t2;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Instant::until(const Temporal& endExclusive, TemporalUnit unit) const {
            try {
                Instant end = Instant::from(endExclusive);
                switch (unit) {
                    case TemporalUnit::NANOS: return nanosUntil(end);
                    case TemporalUnit::MICROS: return microsUntil(end);
                    case TemporalUnit::MILLIS: return millisUntil(end);
                    case TemporalUnit::SECONDS: return secondsUntil(end);
                    case TemporalUnit::MINUTES: return secondsUntil(end) / SECONDS_PER_MINUTE;
                    case TemporalUnit::HOURS: return secondsUntil(end) / SECONDS_PER_HOUR;
                    case TemporalUnit::HALF_DAYS: return secondsUntil(end) / (SECONDS_PER_HOUR * 12L);
                    case TemporalUnit::DAYS: return secondsUntil(end) / SECONDS_PER_DAY;
                    default: break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
        }

        Duration Instant::until(const Instant& endExclusive) const {
            try {
                gint secondsUntil = Math::subtractExact(endExclusive.seconds, seconds);
                gint nanosUntil = endExclusive.nanos - nanos;
                return Duration::ofSeconds(secondsUntil, nanosUntil);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetDateTime Instant::atOffset(const ZoneOffset& offset) const {
            try {
                return OffsetDateTime::ofInstant(*this, offset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime Instant::atZone(const ZoneId& zone) const {
            try {
                return ZonedDateTime::ofInstant(*this, zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Instant::toEpochMilli() const {
            try {
                if (seconds < 0 && nanos > 0) {
                    glong millis = Math::multiplyExact(seconds + 1, MILLIS_PER_SECOND);
                    glong adjustment = nanos / NANOS_PER_MILLI - MILLIS_PER_SECOND;
                    return Math::addExact(millis, adjustment);
                } else {
                    glong millis = Math::multiplyExact(seconds, MILLIS_PER_SECOND);
                    return Math::addExact(millis, (glong) nanos / NANOS_PER_MILLI);
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Instant::compareTo(const Instant& other) const {
            gint result = Long::compare(seconds, other.seconds);
            if (result == 0)
                result = nanos - other.nanos;
            return result;
        }

        gbool Instant::isAfter(const Instant& otherInstant) const {
            return compareTo(otherInstant) > 0;
        }

        gbool Instant::isBefore(const Instant& otherInstant) const {
            return compareTo(otherInstant) < 0;
        }

        gbool Instant::equals(const Object& other) const {
            return this == &other ||
                    (Class<Instant>::hasInstance(other) && compareTo(CORE_XCAST(Instant const, other)) == 0);
        }

        gint Instant::hash() const {
            return ((gint) (seconds ^ ((seconds | 0U) >> 32))) + 51 * nanos;
        }

        String Instant::toString() const {
            return LocalDateTime::ofInstant(*this, ZoneOffset::UTC).toString() + ZoneOffset::UTC.toString();
        }

        Object& Instant::clone() const {
            return UNSAFE::newInstance<Instant>(*this);
        }

        Instant Instant::create(glong seconds, gint nanoOfSecond) {
            if ((seconds | nanoOfSecond) == 0) {
                return EPOCH;
            }
            if (seconds < MIN_SECOND || seconds > MAX_SECOND) {
                DateTimeException("Instant exceeds minimum or maximum instant"_Sl).throws($ftrace());
            }
            return Instant(seconds, nanoOfSecond);
        }

        Instant Instant::plus(glong secondsToAdd, glong nanosToAdd) const {
            try {
                if ((secondsToAdd | nanosToAdd) == 0) {
                    return *this;
                }
                glong epochSec = Math::addExact(seconds, secondsToAdd);
                epochSec = Math::addExact(epochSec, nanosToAdd / NANOS_PER_SECOND);
                nanosToAdd = nanosToAdd % NANOS_PER_SECOND;
                glong nanoAdjustment = nanos + nanosToAdd; // safe int+NANOS_PER_SECOND
                return Instant::ofEpochSecond(epochSec, nanoAdjustment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Instant::nanosUntil(Instant const& end) const {
            try {
                glong secondsUntil = Math::subtractExact(end.seconds, seconds);
                glong toNanos = Math::multiplyExact(secondsUntil, NANOS_PER_SECOND);
                return Math::addExact(toNanos, (glong) (end.nanos - nanos));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Instant::microsUntil(Instant const& end) const {
            try {
                glong secondsUntil = Math::subtractExact(end.seconds, seconds);
                glong toMicros = Math::multiplyExact(secondsUntil, MICROS_PER_SECOND);
                glong nanosDiff = end.nanos - nanos;
                if (toMicros > 0 && nanosDiff < 0) {
                    toMicros -= MICROS_PER_SECOND;
                    nanosDiff += NANOS_PER_SECOND;
                } else if (toMicros < 0 && nanosDiff > 0) {
                    toMicros += MICROS_PER_SECOND;
                    nanosDiff -= NANOS_PER_SECOND;
                }
                return Math::addExact(toMicros, nanosDiff / 1000);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Instant::millisUntil(Instant const& end) const {
            try {
                glong secondsUntil = Math::subtractExact(end.seconds, seconds);
                glong toMillis = Math::multiplyExact(secondsUntil, MILLIS_PER_SECOND);
                glong nanosDiff = end.nanos - nanos;
                if (toMillis > 0 && nanosDiff < 0) {
                    toMillis -= MILLIS_PER_SECOND;
                    nanosDiff += NANOS_PER_SECOND;
                } else if (toMillis < 0 && nanosDiff > 0) {
                    toMillis += MILLIS_PER_SECOND;
                    nanosDiff -= NANOS_PER_SECOND;
                }
                return Math::addExact(toMillis, nanosDiff / NANOS_PER_MILLI);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Instant::secondsUntil(Instant const& end) const {
            try {
                glong secondsUntil = Math::subtractExact(end.seconds, seconds);
                glong nanosDiff = end.nanos - nanos;
                if (secondsUntil > 0 && nanosDiff < 0) {
                    secondsUntil -= 1;
                } else if (secondsUntil < 0 && nanosDiff > 0) {
                    secondsUntil += 1;
                }
                return secondsUntil;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // time
} // core
