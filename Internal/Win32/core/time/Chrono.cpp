//
// Created by brunshweeck on 15 août 2024.
//

#include <core/lang/Windows.h>
#include <core/lang/Error.h>
#include <core/time/Chrono.h>

namespace core {
    namespace time {
        static gint DEFAULT_CLOCK = 0x1234;
        static gbool RESOLVED_FREQ = false;
        static glong FREQUENCY = {};

        void Chrono::start0() {
            if (DEFAULT_CLOCK == 0x1234) {
                if (!RESOLVED_FREQ) {
                    LARGE_INTEGER frequency = {};
                    const BOOL success = QueryPerformanceFrequency(&frequency);
                    RESOLVED_FREQ = success != 0;
                    if (RESOLVED_FREQ) {
                        DEFAULT_CLOCK = PERFORMANCE;
                        FREQUENCY = frequency.QuadPart;
                    } else {
                        FREQUENCY = 0;
                    }
                }
            PERFORMANCE_EVALUATION: {
                    if (RESOLVED_FREQ && FREQUENCY != 0) {
                        LARGE_INTEGER counter = {};
                        const BOOL success = QueryPerformanceCounter(&counter);
                        if (success != 0) {
                            ULONGLONG s = counter.QuadPart / FREQUENCY;
                            ULONGLONG ns = counter.QuadPart % FREQUENCY;
                            while (ns >= 1000000000) {
                                s += 1;
                                ns -= 1000000000;
                            }
                            seconds = s;
                            nanos = ns;
                        }
                        if (DEFAULT_CLOCK == PERFORMANCE) {
                            if (success == 0)
                                Error().throws($ftrace());
                            return;
                        }
                    }

                    DEFAULT_CLOCK = TICK;
                }
            TICK_EVALUATION: {
                    const ULONGLONG tick = GetTickCount64();
                    if (tick != 0) {
                        seconds = CORE_CAST(glong, tick / 1000);
                        nanos = CORE_CAST(gint, tick % 1000) * 1000000;
                        if (seconds >= 0 && nanos >= 0)
                            return;
                    }

                    if (DEFAULT_CLOCK == TICK) {
                        if (tick == 0)
                            Error().throws($ftrace());
                        return;
                    }

                    DEFAULT_CLOCK = SYSTEM;
                }
            SYSTEM_EVALUATION: {
                    SYSTEMTIME time1 = {};
                    GetSystemTime(&time1);
                    FILETIME time2 = {};
                    SystemTimeToFileTime(&time1, &time2);
                    ULONGLONG time = CORE_CAST(ULONGLONG, time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
                    seconds = CORE_CAST(glong, time / 1000);
                    nanos = CORE_CAST(gint, time % 1000) * 1000000;
                    if (seconds >= 0 && nanos >= 0)
                        return;
                    if (DEFAULT_CLOCK == SYSTEM) {
                        if (time == 0) {
                            Error().throws($ftrace());
                        }
                    }
                }
            } else {
                switch (DEFAULT_CLOCK) {
                    case SYSTEM:
                        goto SYSTEM_EVALUATION;
                    case PERFORMANCE:
                        goto PERFORMANCE_EVALUATION;
                    case TICK:
                        goto TICK_EVALUATION;
                    default:
                        Error().throws($ftrace());
                }
            }
        }

        Chrono::Clock Chrono::clock0() const {
            if (DEFAULT_CLOCK == 0x1234)
                return SYSTEM;
            return CORE_CAST(Clock, DEFAULT_CLOCK);
        }
    }
}
