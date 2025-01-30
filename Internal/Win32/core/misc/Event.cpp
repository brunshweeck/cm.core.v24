//
// Created by bruns on 22/03/2024.
//

#include <thread>
#include <Windows.h>
#include <core/misc/Event.h>

#include "core/time/Chrono.h"

namespace core {
    namespace misc {
        Event::Event(): handle(0) {
            HANDLE pHandle = CreateEvent(null, 1, 0, null);
            if (pHandle == INVALID_HANDLE_VALUE || pHandle == null) {
                gint error = CORE_CAST(gint, GetLastError());
                Error(String::valueOf(error)).throws($ftrace());
            }
            handle = CORE_CAST(glong, pHandle);
        }

        void Event::waitFor(glong millis) const {
            switch (WaitForSingleObject(CORE_CAST(HANDLE, handle), millis)) {
                case WAIT_OBJECT_0:
                case WAIT_TIMEOUT:
                    ResetEvent(CORE_CAST(HANDLE, handle));
                    return;
                default:
                    Error("Event Wait Loop Failed ["_S + GetLastError() + "]"_S).throws($ftrace());
            }
        }

        void Event::notify() const {
            SetEvent(CORE_CAST(HANDLE, handle));
        }

        Event::~Event() {
            notify();
            CloseHandle(CORE_CAST(HANDLE, handle));
            handle = 0;
        }

        void Event::park(Object &thread, glong parkBlockerOffset, glong parkEventOffset, glong millis) {
            if (millis < 0 || parkBlockerOffset <= 0 || parkEventOffset <= 0 || thread == null)
                return;
            gbool blockedByEvent = false;
            Object &blocker = Unsafe::getReferenceOpaque(null, parkBlockerOffset);
            Object &event = Unsafe::getReferenceOpaque(null, parkEventOffset);
            if (blocker == null) {
                EventBlocker &newBlocker = *new EventBlocker();
                Unsafe::putReferenceOpaque(null, parkBlockerOffset, newBlocker);
                blockedByEvent = true;
            } else {
                if (Class<EventBlocker>::hasInstance(blocker)) {
                    // It's already blocked by Event.
                    Unsafe::unpark(thread);
                    Unsafe::putReferenceOpaque(null, parkBlockerOffset, blocker);
                    blockedByEvent = true;
                } else {
                    // It'll be blocked by Synchronizer
                }
            }

            time::Chrono chronometer;
            if (event == null) {
                chronometer.start();
                Event &newEvent = *new Event();
                gbool b;
                do {
                    b = Unsafe::compareAndSetReference(null, parkEventOffset, null, newEvent);
                    if (b) newEvent.waitFor(millis);
                } while (!b);
            } else {
                chronometer.start();
                Event &oldEvent = CORE_XCAST(Event, event);
                oldEvent.waitFor(millis);
            }

            if (blockedByEvent && chronometer.hasExpired(millis)) {
                return;
            }

            chronometer.start();
        }

        void Event::unpark(Object &thread, glong parkBlockerOffset, glong parkEventOffset) {
            if (parkBlockerOffset <= 0 || parkEventOffset <= 0 || thread == null)
                return;
            Object &blocker = Unsafe::getReferenceOpaque(null, parkBlockerOffset);
            Object &event = Unsafe::getReferenceOpaque(null, parkEventOffset);
            gbool blockedByEvent = blocker != null && Class<EventBlocker>::hasInstance(blocker);

            if (event == null) {
                if (blockedByEvent) {
                    Unsafe::putReferenceOpaque(null, parkBlockerOffset, null);
                    delete &blocker;
                }
            } else {
                Event &oldEvent = CORE_XCAST(Event, event);
                oldEvent.notify();
                if (blockedByEvent) {
                    Unsafe::putReferenceOpaque(null, parkBlockerOffset, null);
                    delete &blocker;
                }
            }

            //
        }

        //
    } // misc
} // core
