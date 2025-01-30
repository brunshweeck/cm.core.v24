//
// Created by brunshweeck on 3 août 2024.
//

#include "Duration.h"

#include <core/ArithmeticException.h>
#include <core/XString.h>
#include <core/misc/Unsafe.h>
#include <core/time/LocalTime.h>
#include <core/util/Optional.h>

namespace core {
    namespace time {
        using namespace util;

        Duration const Duration::ZERO = Duration(0, 0);

        Duration Duration::ofDays(glong days) {
            try {
                return Duration(Math::multiplyExact(days, LocalTime::SECONDS_PER_DAY), 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::ofHours(glong hours) {
            try {
                return Duration(Math::multiplyExact(hours, LocalTime::SECONDS_PER_HOUR), 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::ofMinutes(glong minutes) {
            try {
                return Duration(Math::multiplyExact(minutes, LocalTime::SECONDS_PER_MINUTE), 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::ofSeconds(glong seconds) {
            return Duration(seconds, 0);
        }

        Duration::Duration(glong seconds, gint nanoAdjustment) {
            try {
                secs = Math::addExact(
                    seconds, Math::floorDiv(CORE_CAST(glong, nanoAdjustment), LocalTime::NANOS_PER_SECOND));
                ns = CORE_CAST(gint, Math::floorMod(CORE_CAST(glong, nanoAdjustment), LocalTime::NANOS_PER_SECOND));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::ofMillis(glong millis) {
            glong s = Math::floorDiv(millis, 1000);
            gint ms = CORE_CAST(gint, Math::floorMod(millis, 1000));
            if (ms < 0) {
                ms += 1000;
                s -= 1;
            }
            return Duration(s, ms * 1000000);
        }

        Duration Duration::ofNanos(glong nanos) {
            glong s = Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND);
            gint ns = CORE_CAST(gint, Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND));
            if (ns < 0) {
                ns += LocalTime::NANOS_PER_SECOND;
                s -= 1;
            }
            return Duration(s, ns);
        }

        Duration Duration::of(glong amount, ChronoUnit unit) {
            try {
                return ZERO.plus(amount, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::between(Temporal const& startInclusive, Temporal const& endExclusive) {
            glong secs = startInclusive.until(endExclusive, SECONDS);
            if (secs == 0) {
                // We don't know which Temporal is earlier, so the adjustment below would not work.
                // But we do know that there's no danger of until(NANOS) overflowing in that case.
                return ofNanos(startInclusive.until(endExclusive, NANOS));
            }
            glong nanos = 0;
            try {
                nanos = endExclusive.getLong(NANO_OF_SECOND) -
                        startInclusive.getLong(NANO_OF_SECOND);
            } catch (DateTimeException const&) {}

            if (nanos < 0 && secs > 0)
                // ofSeconds will subtract one even though until(SECONDS) already gave the correct
                // number of seconds. So compensate. Similarly, for the secs < 0 case below.
                secs++;
            else if (nanos > 0 && secs < 0)
                secs--;
            return Duration(secs, CORE_CAST(gint, nanos));
        }

        glong Duration::get(ChronoUnit unit) const {
            if (unit == SECONDS)
                return secs;
            if (unit == NANOS)
                return ns;
            TemporalException("Unsupported unit: "_Sl + Temporal::toString(unit)).throws($ftrace());
        }

        gbool Duration::isPositive() const {
            return (secs | ns) > 0;
        }

        gbool Duration::isZero() const {
            return (secs | ns) == 0;
        }

        gbool Duration::isNegative() const {
            return secs < 0;
        }

        glong Duration::seconds() const {
            return secs;
        }

        gint Duration::nanos() const {
            return ns;
        }

        Duration Duration::withSeconds(glong seconds) const {
            return Duration(seconds, ns);
        }

        Duration Duration::withNanos(gint nanoOfSecond) const {
            try {
                checkValue(nanoOfSecond, NANO_OF_SECOND);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return Duration(secs, nanoOfSecond);
        }

        Duration Duration::plus(Duration const& duration) const {
            glong seconds = duration.seconds();
            glong nanos = ns + duration.nanos();
            try {
                seconds = Math::addExact(seconds, secs);
                seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);
            return Duration(seconds, CORE_CAST(gint, nanos));
        }

        Duration Duration::plus(glong amountToAdd, ChronoUnit unit) const {
            try {
                switch (unit) {
                    case NANOS:
                        return plusNanos(amountToAdd);
                    case MICROS:
                        return plusSeconds(Math::floorDiv(amountToAdd, LocalTime::MICROS_PER_SECOND))
                                .plusNanos(Math::floorMod(amountToAdd, LocalTime::MICROS_PER_SECOND) * 1000);
                    case MILLIS:
                        return plusMillis(amountToAdd);
                    case SECONDS:
                        return plusSeconds(amountToAdd);
                    case MINUTES:
                        return plusSeconds(Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_MINUTE));
                    case HOURS:
                        return plusSeconds(
                            Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_HOUR));
                    case HALF_DAYS:
                        return plusSeconds(
                            Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_DAY >> 1));
                    case DAYS:
                        return plusSeconds(Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_DAY));
                    case WEEKS:
                        return plusSeconds(Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_DAY));
                    default:
                        break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            TemporalException("Unsupported unit "_Sl + Temporal::toString(unit)).throws($ftrace());
        }

        Duration Duration::plusDays(glong daysToAdd) const {
            try {
                return plusSeconds(Math::multiplyExact(daysToAdd, LocalTime::SECONDS_PER_DAY));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::plusHours(glong hoursToAdd) const {
            try {
                return plusSeconds(Math::multiplyExact(hoursToAdd, LocalTime::SECONDS_PER_HOUR));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::plusMinutes(glong minutesToAdd) const {
            try {
                return plusSeconds(Math::multiplyExact(minutesToAdd, LocalTime::SECONDS_PER_MINUTE));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::plusSeconds(glong secondsToAdd) const {
            try {
                return withSeconds(Math::addExact(secs, secondsToAdd));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::plusMillis(glong millisToAdd) const {
            try {
                glong seconds = Math::addExact(secs, Math::floorDiv(millisToAdd, LocalTime::MILLIS_PER_SECOND));
                glong nanos = ns + Math::floorMod(millisToAdd, LocalTime::MILLIS_PER_SECOND) *
                        LocalTime::NANOS_PER_MILLI;

                seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
                nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);
                return Duration(seconds, CORE_CAST(gint, nanos));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::plusNanos(glong nanosToAdd) const {
            try {
                glong seconds = Math::addExact(secs, Math::floorDiv(nanosToAdd, LocalTime::NANOS_PER_SECOND));
                glong nanos = ns + Math::floorMod(nanosToAdd, LocalTime::NANOS_PER_SECOND);

                seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
                nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);

                return Duration(seconds, CORE_CAST(gint, nanos));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::minus(Duration const& duration) const {
            glong seconds = duration.seconds();
            if (seconds == Long::MIN_VALUE) {
                try {
                    seconds = Math::addExact(secs, -Long::MAX_VALUE);
                    seconds = Math::addExact(seconds, -1LL);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
            glong nanos = ns - duration.nanos();

            try {
                seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
                nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return Duration(seconds, CORE_CAST(gint, nanos));
        }

        Duration Duration::minus(glong amountToSubtract, ChronoUnit unit) const {
            try {
                return amountToSubtract == Long::MIN_VALUE
                           ? plus(-Long::MAX_VALUE, unit).plus(-1, unit)
                           : plus(-amountToSubtract, unit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::minusDays(glong daysToSubtract) const {
            try {
                return daysToSubtract == Long::MIN_VALUE
                           ? plusDays(-Long::MAX_VALUE).plusDays(-1)
                           : plusDays(-daysToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::minusHours(glong hoursToSubtract) const {
            try {
                return hoursToSubtract == Long::MIN_VALUE
                           ? plusHours(-Long::MAX_VALUE).plusDays(-1)
                           : plusHours(-hoursToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::minusMinutes(glong minutesToSubtract) const {
            try {
                return minutesToSubtract == Long::MIN_VALUE
                           ? plusMinutes(-Long::MAX_VALUE).plusHours(-1)
                           : plusMinutes(-minutesToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::minusSeconds(glong secondsToSubtract) const {
            try {
                return secondsToSubtract == Long::MIN_VALUE
                           ? plusSeconds(-Long::MAX_VALUE).plusSeconds(-1)
                           : plusSeconds(-secondsToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::minusMillis(glong millisToSubtract) const {
            try {
                return millisToSubtract == Long::MIN_VALUE
                           ? plusMillis(-Long::MAX_VALUE).plusMillis(-1)
                           : plusMillis(-millisToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::minusNanos(glong nanosToSubtract) const {
            try {
                return nanosToSubtract == Long::MIN_VALUE
                           ? plusNanos(-Long::MAX_VALUE).plusNanos(-1)
                           : plusNanos(-nanosToSubtract);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::multipliedBy(glong multiplicand) const {
            if (multiplicand == 0)
                return ZERO;
            if (multiplicand == 1)
                return *this;
            try {
                glong seconds = Math::multiplyExact(secs, multiplicand);
                glong nanos = Math::multiplyExact(CORE_CAST(glong, ns), multiplicand);

                seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
                nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);

                return Duration(seconds, CORE_CAST(gint, nanos));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::dividedBy(glong divisor) const {
            if (divisor == 0)
                ArithmeticException("Cannot divide by zero"_Sl).throws($ftrace());
            if (divisor == 1)
                return *this;

            try {
                const glong seconds = Math::floorDiv(secs, divisor);
                const glong nanos = Math::floorDiv(CORE_CAST(glong, ns), divisor);

                return Duration(seconds, CORE_CAST(gint, nanos));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Duration::dividedBy(Duration const& divisor) const {
            if (divisor.isZero())
                ArithmeticException("Cannot divide by zero"_Sl).throws($ftrace());
            if (isZero())
                return 0;

            const glong seconds = divisor.seconds();
            const glong nanos = divisor.nanos();

            gdouble x = CORE_CAST(gdouble, secs) * LocalTime::NANOS_PER_SECOND + CORE_CAST(gdouble, ns);
            gdouble y = CORE_CAST(gdouble, seconds) * LocalTime::NANOS_PER_SECOND + CORE_CAST(gdouble, nanos);

            return Math::round(x / y);
        }

        Duration Duration::negated() const {
            try {
                return multipliedBy(-1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration Duration::abs() const {
            try {
                return Duration(Math::abs(secs), ns);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Duration::toDays() const {
            return Math::floorDiv(secs, LocalTime::SECONDS_PER_DAY);
        }

        glong Duration::toHours() const {
            return Math::floorDiv(secs, LocalTime::SECONDS_PER_HOUR);
        }

        glong Duration::toMinutes() const {
            return Math::floorDiv(secs, LocalTime::SECONDS_PER_MINUTE);
        }

        glong Duration::toSeconds() const {
            return secs;
        }

        glong Duration::toMillis() const {
            glong tempSeconds = secs;
            glong tempNanos = ns;
            if (tempSeconds < 0) {
                // change the seconds and nano value to
                // handle Long.MIN_VALUE case
                tempSeconds = tempSeconds + 1;
                tempNanos = tempNanos - LocalTime::NANOS_PER_SECOND;
            }
            try {
                glong millis = Math::multiplyExact(tempSeconds, 1000);
                millis = Math::addExact(millis, tempNanos / LocalTime::NANOS_PER_MILLI);
                return millis;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Duration::toNanos() const {
            glong tempSeconds = secs;
            glong tempNanos = ns;
            if (tempSeconds < 0) {
                // change the seconds and nano value to
                // handle Long.MIN_VALUE case
                tempSeconds = tempSeconds + 1;
                tempNanos = tempNanos - LocalTime::NANOS_PER_SECOND;
            }
            try {
                glong totalNanos = Math::multiplyExact(tempSeconds, LocalTime::NANOS_PER_SECOND);
                totalNanos = Math::addExact(totalNanos, tempNanos);
                return totalNanos;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong Duration::toDaysPart() const {
            return secs / LocalTime::SECONDS_PER_DAY;
        }

        gint Duration::toHoursPart() const {
            return CORE_CAST(gint, toHours() % LocalTime::HOURS_PER_DAY);
        }

        gint Duration::toMinutesPart() const {
            return CORE_CAST(gint, toMinutes() % LocalTime::MINUTES_PER_HOUR);
        }

        gint Duration::toSecondsPart() const {
            return CORE_CAST(gint, toSeconds() % LocalTime::SECONDS_PER_MINUTE);
        }

        gint Duration::toMillisPart() const {
            return CORE_CAST(gint, ns % LocalTime::NANOS_PER_MILLI);
        }

        gint Duration::toNanosPart() const {
            return ns;
        }

        Duration Duration::truncateTo(ChronoUnit unit) const {
            try {
                switch (unit) {
                    case NANOS:
                        return *this;
                    case MICROS:
                        return ofSeconds(secs).plusNanos(ns * 1000LL);
                    case MILLIS:
                        return ofSeconds(secs).plusNanos(ns * LocalTime::NANOS_PER_MILLI);
                    case SECONDS:
                        return ofSeconds(secs);
                    case MINUTES:
                        return ofMinutes(secs / LocalTime::SECONDS_PER_MINUTE);
                    case HOURS:
                        return ofHours(secs / LocalTime::SECONDS_PER_HOUR);
                    case HALF_DAYS:
                        return ofHours(secs / (LocalTime::SECONDS_PER_DAY >> 1));
                    case DAYS:
                        return ofDays(secs / LocalTime::SECONDS_PER_DAY);
                    default:
                        break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            TemporalException("Unsupported unit: "_Sl + Temporal::toString(unit)).throws($ftrace());
        }

        Optional<> Duration::query(TemporalQuery const& query) const {
            return util::Optional<>();
        }

        gint Duration::compareTo(const Duration& otherDuration) const {
            const gint r = Long::compare(secs, otherDuration.secs);
            if (r != 0)
                return r;
            return Integer::compare(ns, otherDuration.ns);
        }

        gbool Duration::equals(const Object& other) const {
            if (this == &other)
                return true;
            if (!Class<Duration>::hasInstance(other))
                return false;
            Duration const& duration = CORE_XCAST(Duration const, other);
            return secs == duration.secs && ns == duration.ns;
        }

        gint Duration::hash() const {
            return Long::hash(secs) ^ Integer::hash(ns);
        }

        String Duration::toString() const {
            if (*this == ZERO) {
                return "PT0S"_Sl;
            }
            glong effectiveTotalSecs = secs;
            if (secs < 0 && ns > 0) {
                effectiveTotalSecs++;
            }
            glong hours = effectiveTotalSecs / LocalTime::SECONDS_PER_HOUR;
            gint minutes = (gint)(effectiveTotalSecs % LocalTime::SECONDS_PER_HOUR / LocalTime::SECONDS_PER_MINUTE);
            gint seconds = (gint)(effectiveTotalSecs % LocalTime::SECONDS_PER_MINUTE);
            XString buf = XString(24);
            buf.append("PT"_Sl);
            if (hours != 0) {
                buf.append(hours).append('H');
            }
            if (minutes != 0) {
                buf.append(minutes).append('M');
            }
            if (seconds == 0 && ns == 0 && buf.length() > 2) {
                return buf.toString();
            }
            if (secs < 0 && ns > 0) {
                if (seconds == 0) {
                    buf.append("-0"_Sl);
                } else {
                    buf.append(seconds);
                }
            } else {
                buf.append(seconds);
            }
            if (ns > 0) {
                gint pos = buf.length();
                if (secs < 0) {
                    buf.append(2 * LocalTime::NANOS_PER_SECOND - ns);
                } else {
                    buf.append(ns + LocalTime::NANOS_PER_SECOND);
                }
                while (buf.charAt(buf.length() - 1) == '0') {
                    buf.setLength(buf.length() - 1);
                }
                buf.setCharAt(pos, '.');
            }
            buf.append('S');
            return buf.toString();
        }

        glong Duration::until(const Temporal& endExclusive, ChronoUnit unit) const {
            if (!Class<Duration>::hasInstance(endExclusive)) {
                DateTimeException("Could not obtain duration"_Sl).throws($ftrace());
            }
            if (!isSupported(unit))
                TemporalException("Unsupported unit"_Sl + Temporal::toString(unit)).throws($ftrace());
            Duration const& end = CORE_XCAST(Duration const, endExclusive);
            Duration const duration = end.plus(*this);
            try {
                switch (unit) {
                    case NANOS:
                        return duration.toNanos();
                    case MICROS:
                        return duration.toNanos() / 1000;
                    case MILLIS:
                        return duration.toNanos() / 1000000;
                    case SECONDS:
                        return duration.toSeconds();
                    case MINUTES:
                        return duration.toMinutes();
                    case HOURS:
                        return duration.toHours();
                    case HALF_DAYS:
                        return duration.toHours() / 12;
                    case DAYS:
                        return duration.toDays();
                    default:
                        return 0;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Object& Duration::clone() const {
            try {
                return UNSAFE::newInstance<Duration>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Duration::~Duration() {
            secs = ns = 0;
        }

        gbool Duration::isSupported(ChronoUnit unit) const {
            return unit <= WEEKS;
        }
    } // time

    using namespace time;

    inline namespace literals {
        gbool operator<(Duration const& lhs, Duration const& rhs) {
            return lhs.compareTo(rhs) < 0;
        }

        gbool operator<=(Duration const& duration, Duration const& duration1) {
            return !(duration1 < duration);
        }

        gbool operator>(Duration const& duration, Duration const& duration1) {
            return duration1 < duration;
        }

        gbool operator>=(Duration const& duration, Duration const& duration1) {
            return !(duration < duration1);
        }

        gbool operator==(Duration const& lhs, Duration const& rhs) {
            return lhs.compareTo(rhs) == 0;
        }

        gbool operator!=(Duration const& duration, Duration const& duration1) {
            return !(duration == duration1);
        }
    }
} // core
