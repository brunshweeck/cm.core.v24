//
// Created by brunshweeck on 3 août 2024.
//

#include <core/lang/Enum.h>
#include <core/misc/Unsafe.h>
#include <core/time/Chrono.h>
#include <core/time/Duration.h>
#include <core/time/TemporalQuery.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  namespace time {
    using namespace util;

    CORE_ALIAS(Units, TemporalUtils::Units);

    Chrono::Chrono(gbool autoStart) : Chrono() {
      if (autoStart) start();
    }

    gbool Chrono::isStarted() const {
      return seconds != -1;
    }

    glong Chrono::elapsedTime() const {
      if (!isStarted())
        return -1;
      Chrono chrono = Chrono(true);
      glong s = chrono.seconds - seconds;
      gint ns = chrono.nanos - nanos;
      glong result = Math::multiplyExact(s, 1000) + Math::floorDiv(ns, 1000000);
      return result < 0 ? Long::MAX_VALUE : result;
    }

    Duration Chrono::duration() const {
      if (!isStarted())
        return Duration::ZERO;
      Chrono chrono = Chrono(true);
      glong s = chrono.seconds - seconds;
      gint ns = chrono.nanos - nanos;
      return Duration::ofSeconds(s, ns);
    }

    glong Chrono::elapsedTime(TemporalUnit unit) const {
      if (!isStarted())
        return -1;
      Chrono const chrono = Chrono(true);
      glong s = chrono.seconds - seconds;
      gint ns = chrono.nanos - nanos;
      glong result = 0;
      switch (unit) {
        case TemporalUnit::NANOS:
          if (Math::multiplyHigh(s, 1000000000) < 0)
            return Long::MAX_VALUE;
          result = Math::multiplyExact(s, 1000000000) + ns;
          break;
        case TemporalUnit::MICROS:
          if (Math::multiplyHigh(s, 1000000) < 0)
            return Long::MAX_VALUE;
          result = Math::multiplyExact(s, 1000000) + Math::floorDiv(ns, 1000);
          break;
        case TemporalUnit::MILLIS:
          if (Math::multiplyHigh(s, 1000) < 0)
            return Long::MAX_VALUE;
          result = Math::multiplyExact(s, 1000) + Math::floorDiv(ns, 1000000);
          break;
        case TemporalUnit::SECONDS:
          result = s + Math::floorDiv(ns, 1000000000);
          break;
        case TemporalUnit::MINUTES:
          result = Math::floorDiv(s, 60);
          break;
        case TemporalUnit::HOURS:
          result = Math::floorDiv(s, 3600);
          break;
        case TemporalUnit::HALF_DAYS:
          result = Math::floorDiv(s, 12 * 3600);
          break;
        case TemporalUnit::DAYS:
          result = Math::floorDiv(s, 24 * 3600);
          break;
        case TemporalUnit::WEEKS:
          result = Math::floorDiv(s, 7 * 24 * 3600);
          break;
        default:
          TemporalException("Unsupported unit: "_S + Units::toString(unit)).throws($ftrace());
      }
      return result < 0 ? Long::MAX_VALUE : result;
    }

    gbool Chrono::hasExpired(glong time) const {
      return isStarted() && elapsedTime() >= time;
    }

    gbool Chrono::hasExpired(glong time, TemporalUnit unit) const {
      return isStarted() && elapsedTime(unit) >= time;
    }

    gbool Chrono::hasExpired(Duration const& d) const {
      return isStarted() && duration().compareTo(d) >= 0;
    }

    gint Chrono::hash() const {
      return Long::hash(seconds) ^ Integer::hash(nanos);
    }

    gbool Chrono::equals(const Object& o) const {
      if (this == &o)
        return true;
      if (!Class<Chrono>::hasInstance(o))
        return false;
      Chrono const& other = CORE_XCAST(Chrono const, o);
      return seconds == other.seconds && nanos == other.nanos;
    }

    String Chrono::toString() const {
      return "Chrono[elapsed="_S + elapsedTime() + "ms]"_S;
    }

    Object& Chrono::clone() const {
      try {
        return UNSAFE::newInstance<Chrono>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
  } // time
} // core
