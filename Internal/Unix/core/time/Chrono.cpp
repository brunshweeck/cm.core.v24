//
// Created by brunshweeck on 15 août 2024.
//

#include <core/ArithmeticException.h>
#include <native/System.h>
#include <core/time/Chrono.h>

namespace core {
    namespace time {
        glong Chrono::start() {
            TICK_COUNT t = { };
            try {
                t = GetTickCount();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            glong oldSecs;
            gint oldNanos;
            if (!isStarted()) {
                oldSecs = 0;
                oldNanos = 0;
            } else {
                oldSecs = seconds;
                oldNanos = nanos;
            }

            seconds = t.dwSeconds;
            nanos = t.dwNano;

            glong result = 0;
            try {
                result = Math::addExact(
                    Math::multiplyExact(t.dwSeconds - oldSecs, 1000),
                    (glong) Math::floorDiv(t.dwNano - oldNanos, 1000000)
                );
            } catch (ArithmeticException const& _) {
                result = Long::MAX_VALUE;
            }

            return result;
        }
    }
}
