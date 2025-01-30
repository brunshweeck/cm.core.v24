//
// Created by brunshweeck on 3 août 2024.
//

#include <core/Enum.h>
#include <core/misc/Unsafe.h>
#include <core/time/Chrono.h>
#include <core/time/Duration.h>
#include <core/time/TemporalQuery.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;
    namespace time {
        Chrono::Chrono() {
            seconds = -1;
            nanos = 0;
        }

        Chrono::Chrono(gbool autoStart): Chrono() {
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
            return Duration(s, ns);
        }

        glong Chrono::elapsedTime(ChronoUnit unit) const {
            if (!isStarted())
                return -1;
            Chrono const chrono = Chrono(true);
            glong s = chrono.seconds - seconds;
            gint ns = chrono.nanos - nanos;
            glong result = 0;
            switch (unit) {
                case NANOS:
                    if (Math::multiplyHigh(s, 1000000000) < 0)
                        return Long::MAX_VALUE;
                    result = Math::multiplyExact(s, 1000000000) + ns;
                    break;
                case MICROS:
                    if (Math::multiplyHigh(s, 1000000) < 0)
                        return Long::MAX_VALUE;
                    result = Math::multiplyExact(s, 1000000) + Math::floorDiv(ns, 1000);
                    break;
                case MILLIS:
                    if (Math::multiplyHigh(s, 1000) < 0)
                        return Long::MAX_VALUE;
                    result = Math::multiplyExact(s, 1000) + Math::floorDiv(ns, 1000000);
                    break;
                case SECONDS:
                    result = s + Math::floorDiv(ns, 1000000000);
                    break;
                case MINUTES:
                    result = Math::floorDiv(s, 60);
                    break;
                case HOURS:
                    result = Math::floorDiv(s, 3600);
                    break;
                case HALF_DAYS:
                    result = Math::floorDiv(s, 12 * 3600);
                    break;
                case DAYS:
                    result = Math::floorDiv(s, 24 * 3600);
                    break;
                case WEEKS:
                    result = Math::floorDiv(s, 7 * 24 * 3600);
                    break;
                default:
                    TemporalException("Unsupported unit: "_S + Temporal::toString(unit)).throws($ftrace());
            }
            return result < 0 ? Long::MAX_VALUE : result;
        }

        gbool Chrono::hasExpired(glong time) const {
            return isStarted() && elapsedTime() >= time;
        }

        gbool Chrono::hasExpired(glong time, ChronoUnit unit) const {
            return isStarted() && elapsedTime(unit) >= time;
        }

        gbool Chrono::hasExpired(Duration const& d) const {
            return isStarted() && duration().compareTo(d) >= 0;
        }

        gbool Chrono::isSupported(ChronoUnit unit) const {
            return unit <= WEEKS;
        }

        gint Chrono::hash() const {
            return Long::hash(seconds) ^ Integer::hash(nanos);
        }

        gbool Chrono::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<Chrono>::hasInstance(o))
                return false;
            Chrono const &other = CORE_XCAST(Chrono const, o);
            return seconds == other.seconds && nanos == other.nanos;
        }

        String Chrono::toString() const {
            return "Chrono[elapsed="_S + elapsedTime() + "ms]"_S;
        }

        Object &Chrono::clone() const {
            try {
                return UNSAFE::newInstance<Chrono>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        Optional<> Chrono::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::PRECISION)
                return Optional<Enum<ChronoUnit>>(NANOS);
            return Optional<>();
        }
    } // time
} // core
