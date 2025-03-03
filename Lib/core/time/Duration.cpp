//
// Created by brunshweeck on 3 août 2024.
//

#include "Duration.h"

#include <core/lang/ArithmeticException.h>
#include <core/lang/Enum.h>
#include <core/lang/XString.h>
#include <core/misc/Unsafe.h>
#include <core/time/LocalTime.h>
#include <core/time/TemporalException.h>
#include <core/util/List.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  namespace time {
    using namespace util;

    Duration const Duration::ZERO = Duration(0, 0);

    Duration::Duration(glong seconds, gint nanos): seconds_(seconds), nanos_(nanos) {}

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

    Duration Duration::ofSeconds(glong seconds, glong nanoAdjustment) {
      try {
        glong secs = Math::addExact(seconds, (glong) Math::floorDiv(nanoAdjustment, NANOS_PER_SECOND));
        gint ns = Math::floorMod(nanoAdjustment, NANOS_PER_SECOND);
        return create(secs, ns);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Duration Duration::ofMillis(glong millis) {
      glong s = Math::floorDiv(millis, 1000);
      gint ms = Math::floorMod(millis, 1000);
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

    Duration Duration::of(glong amount, TemporalUnit unit) {
      try {
        return ZERO.plus(amount, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Duration Duration::from(TemporalAmount const& amount) {
      Duration duration = ZERO;
      for (TemporalUnit unit : amount.getUnits()) {
        Duration d = duration.plus(amount.get(unit), unit);

        (glong&) duration.seconds_ = d.seconds_;
        (gint&) duration.nanos_ = d.nanos_;
      }
      return duration;
    }

    Duration Duration::parse(CharSequence const& text) {
      try {
        return TemporalUtils::Durations::parse(text);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Duration Duration::between(Temporal const& startInclusive, Temporal const& endExclusive) {
      glong secs = startInclusive.until(endExclusive, TemporalUnit::SECONDS);
      if (secs == 0) {
        // We don't know which Temporal is earlier, so the adjustment below would not work.
        // But we do know that there's no danger of until(NANOS) overflowing in that case.
        return ofNanos(startInclusive.until(endExclusive, TemporalUnit::NANOS));
      }
      glong nanos = 0;
      try {
        nanos = endExclusive.getLong(TemporalField::NANO_OF_SECOND) -
            startInclusive.getLong(TemporalField::NANO_OF_SECOND);
      } catch (DateTimeException const&) {}

      if (nanos < 0 && secs > 0)
        // ofSeconds will subtract one even though until(SECONDS) already gave the correct
        // number of seconds. So compensate. Similarly, for the secs < 0 case below.
        secs++;
      else if (nanos > 0 && secs < 0)
        secs--;
      return Duration(secs, CORE_CAST(gint, nanos));
    }

    glong Duration::get(TemporalUnit unit) const {
      if (unit == TemporalUnit::SECONDS)
        return seconds_;
      if (unit == TemporalUnit::NANOS)
        return nanos_;
      TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
    }

    List<TemporalAmount::UnitEnum>& Duration::getUnits() const {
      static UnitList& units = UnitList::of(TemporalUnit::SECONDS, TemporalUnit::NANOS);
      return units;
    }

    gbool Duration::isPositive() const {
      return (seconds_ | nanos_) > 0;
    }

    gbool Duration::isZero() const {
      return (seconds_ | nanos_) == 0;
    }

    gbool Duration::isNegative() const {
      return seconds_ < 0;
    }

    glong Duration::seconds() const {
      return seconds_;
    }

    gint Duration::nanos() const {
      return nanos_;
    }

    Duration Duration::withSeconds(glong seconds) const {
      return Duration(seconds, nanos_);
    }

    Duration Duration::withNanos(gint nanoOfSecond) const {
      try {
        TemporalUtils::Fields::checkValue(nanoOfSecond, TemporalField::NANO_OF_SECOND);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      return Duration(seconds_, nanoOfSecond);
    }

    Duration Duration::plus(Duration const& duration) const {
      glong seconds = duration.seconds();
      glong nanos = nanos_ + duration.nanos();
      try {
        seconds = Math::addExact(seconds, seconds_);
        seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);
      return Duration(seconds, CORE_CAST(gint, nanos));
    }

    Duration Duration::plus(glong amountToAdd, TemporalUnit unit) const {
      try {
        switch (unit) {
          case TemporalUnit::NANOS:
            return plusNanos(amountToAdd);
          case TemporalUnit::MICROS:
            return plusSeconds(Math::floorDiv(amountToAdd, LocalTime::MICROS_PER_SECOND))
                .plusNanos(Math::floorMod(amountToAdd, LocalTime::MICROS_PER_SECOND) * 1000);
          case TemporalUnit::MILLIS:
            return plusMillis(amountToAdd);
          case TemporalUnit::SECONDS:
            return plusSeconds(amountToAdd);
          case TemporalUnit::MINUTES:
            return plusSeconds(Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_MINUTE));
          case TemporalUnit::HOURS:
            return plusSeconds(
              Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_HOUR));
          case TemporalUnit::HALF_DAYS:
            return plusSeconds(
              Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_DAY >> 1));
          case TemporalUnit::DAYS:
            return plusSeconds(Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_DAY));
          case TemporalUnit::WEEKS:
            return plusSeconds(Math::multiplyExact(amountToAdd, LocalTime::SECONDS_PER_DAY));
          default:
            break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit "_Sl + unit).throws($ftrace());
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
        return withSeconds(Math::addExact(seconds_, secondsToAdd));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Duration Duration::plusMillis(glong millisToAdd) const {
      try {
        glong seconds = Math::addExact(seconds_, Math::floorDiv(millisToAdd, LocalTime::MILLIS_PER_SECOND));
        glong nanos = nanos_ + Math::floorMod(millisToAdd, LocalTime::MILLIS_PER_SECOND) *
            LocalTime::NANOS_PER_MILLI;

        seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
        nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);
        return Duration(seconds, CORE_CAST(gint, nanos));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Duration Duration::plusNanos(glong nanosToAdd) const {
      try {
        glong seconds = Math::addExact(seconds_, Math::floorDiv(nanosToAdd, LocalTime::NANOS_PER_SECOND));
        glong nanos = nanos_ + Math::floorMod(nanosToAdd, LocalTime::NANOS_PER_SECOND);

        seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
        nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);

        return Duration(seconds, CORE_CAST(gint, nanos));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Duration Duration::minus(Duration const& duration) const {
      glong seconds = duration.seconds();
      if (seconds == Long::MIN_VALUE) {
        try {
          seconds = Math::addExact(seconds_, -Long::MAX_VALUE);
          seconds = Math::addExact(seconds, -1LL);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      glong nanos = nanos_ - duration.nanos();

      try {
        seconds = Math::addExact(seconds, Math::floorDiv(nanos, LocalTime::NANOS_PER_SECOND));
        nanos = Math::floorMod(nanos, LocalTime::NANOS_PER_SECOND);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }

      return Duration(seconds, CORE_CAST(gint, nanos));
    }

    Duration Duration::minus(glong amountToSubtract, TemporalUnit unit) const {
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
        glong seconds = Math::multiplyExact(seconds_, multiplicand);
        glong nanos = Math::multiplyExact(CORE_CAST(glong, nanos_), multiplicand);

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
        const glong seconds = Math::floorDiv(seconds_, divisor);
        const glong nanos = Math::floorDiv(CORE_CAST(glong, nanos_), divisor);

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

      gdouble x = CORE_CAST(gdouble, seconds_) * LocalTime::NANOS_PER_SECOND + CORE_CAST(gdouble, nanos_);
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
        return Duration(Math::abs(seconds_), nanos_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& Duration::addTo(const Temporal& temporal) const {
      OptionalTemporal t1, t2;
      try {
        if (seconds_ != 0)
          t1 = addAmountTo(seconds_, TemporalUnit::SECONDS, temporal);
        if (nanos_ != 0) {
          if (t1.isPresent())
            t2 = addAmountTo(nanos_, TemporalUnit::NANOS, t1.get());
          else
            t2 = addAmountTo(nanos_, TemporalUnit::NANOS, temporal);
        }

        if (t2.isPresent()) {
          if (t1.isPresent())
            if (&t1.get() != &t2.get() && &t1.get() != &temporal)
              UNSAFE::deleteRegInstance(t1.get());
          return t2.get();
        }
        if (t1.isPresent())
          return t1.get();
        return CORE_XCAST(Temporal, temporal.clone());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& Duration::subtractFrom(const Temporal& temporal) const {
      OptionalTemporal t1, t2;
      try {
        if (seconds_ != 0)
          t1 = subtractAmountFrom(seconds_, TemporalUnit::SECONDS, temporal);
        if (nanos_ != 0) {
          if (t1.isPresent())
            t2 = subtractAmountFrom(nanos_, TemporalUnit::NANOS, t1.get());
          else
            t2 = subtractAmountFrom(nanos_, TemporalUnit::NANOS, temporal);
        }

        if (t2.isPresent()) {
          if (t1.isPresent())
            if (&t1.get() != &t2.get() && &t1.get() != &temporal)
              UNSAFE::deleteRegInstance(t1.get());
          return t2.get();
        }
        if (t1.isPresent())
          return t1.get();
        return CORE_XCAST(Temporal, temporal.clone());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong Duration::toDays() const {
      return Math::floorDiv(seconds_, LocalTime::SECONDS_PER_DAY);
    }

    glong Duration::toHours() const {
      return Math::floorDiv(seconds_, LocalTime::SECONDS_PER_HOUR);
    }

    glong Duration::toMinutes() const {
      return Math::floorDiv(seconds_, LocalTime::SECONDS_PER_MINUTE);
    }

    glong Duration::toSeconds() const {
      return seconds_;
    }

    glong Duration::toMillis() const {
      glong tempSeconds = seconds_;
      glong tempNanos = nanos_;
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
      glong tempSeconds = seconds_;
      glong tempNanos = nanos_;
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
      return seconds_ / LocalTime::SECONDS_PER_DAY;
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
      return CORE_CAST(gint, nanos_ % LocalTime::NANOS_PER_MILLI);
    }

    gint Duration::toNanosPart() const {
      return nanos_;
    }

    Duration Duration::truncatedTo(TemporalUnit unit) const {
      try {
        switch (unit) {
          case TemporalUnit::NANOS:
            return *this;
          case TemporalUnit::MICROS:
            return ofSeconds(seconds_).plusNanos(nanos_ * 1000LL);
          case TemporalUnit::MILLIS:
            return ofSeconds(seconds_).plusNanos(nanos_ * LocalTime::NANOS_PER_MILLI);
          case TemporalUnit::SECONDS:
            return ofSeconds(seconds_);
          case TemporalUnit::MINUTES:
            return ofMinutes(seconds_ / LocalTime::SECONDS_PER_MINUTE);
          case TemporalUnit::HOURS:
            return ofHours(seconds_ / LocalTime::SECONDS_PER_HOUR);
          case TemporalUnit::HALF_DAYS:
            return ofHours(seconds_ / (LocalTime::SECONDS_PER_DAY >> 1));
          case TemporalUnit::DAYS:
            return ofDays(seconds_ / LocalTime::SECONDS_PER_DAY);
          default:
            break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
    }

    gint Duration::compareTo(const Duration& otherDuration) const {
      const gint r = Long::compare(seconds_, otherDuration.seconds_);
      if (r != 0)
        return r;
      return Integer::compare(nanos_, otherDuration.nanos_);
    }

    gbool Duration::equals(const Object& other) const {
      if (this == &other)
        return true;
      if (!Class<Duration>::hasInstance(other))
        return false;
      Duration const& duration = CORE_XCAST(Duration const, other);
      return seconds_ == duration.seconds_ && nanos_ == duration.nanos_;
    }

    gint Duration::hash() const {
      return Long::hash(seconds_) ^ Integer::hash(nanos_);
    }

    String Duration::toString() const {
      if (*this == ZERO) {
        return "PT0S"_Sl;
      }
      glong effectiveTotalSecs = seconds_;
      if (seconds_ < 0 && nanos_ > 0) {
        effectiveTotalSecs++;
      }
      glong hours = effectiveTotalSecs / LocalTime::SECONDS_PER_HOUR;
      gint minutes = (gint) (effectiveTotalSecs % LocalTime::SECONDS_PER_HOUR / LocalTime::SECONDS_PER_MINUTE);
      gint seconds = (gint) (effectiveTotalSecs % LocalTime::SECONDS_PER_MINUTE);
      XString buf = XString(24);
      buf.append("PT"_Sl);
      if (hours != 0) {
        buf.append(hours).append('H');
      }
      if (minutes != 0) {
        buf.append(minutes).append('M');
      }
      if (seconds == 0 && nanos_ == 0 && buf.length() > 2) {
        return buf.toString();
      }
      if (seconds_ < 0 && nanos_ > 0) {
        if (seconds == 0) {
          buf.append("-0"_Sl);
        } else {
          buf.append(seconds);
        }
      } else {
        buf.append(seconds);
      }
      if (nanos_ > 0) {
        gint pos = buf.length();
        if (seconds_ < 0) {
          buf.append(2 * LocalTime::NANOS_PER_SECOND - nanos_);
        } else {
          buf.append(nanos_ + LocalTime::NANOS_PER_SECOND);
        }
        while (buf.charAt(buf.length() - 1) == '0') {
          buf.setLength(buf.length() - 1);
        }
        buf.setCharAt(pos, '.');
      }
      buf.append('S');
      return buf.toString();
    }

    Object& Duration::clone() const {
      try {
        return UNSAFE::newInstance<Duration>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Duration Duration::create(glong seconds, gint nanoAdjustment) {
      if ((seconds | nanoAdjustment) == 0)
        return ZERO;
      return Duration(seconds, nanoAdjustment);
    }
  } // time
} // core
