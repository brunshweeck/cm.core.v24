//
// Created by bruns on 22/03/2024.
//

#include "Event.h"

#include <errno.h>
#include <core/time/Chrono.h>
#include <core/time/LocalDateTime.h>

namespace core {
    namespace misc {
        Event::Event(): mutex(), cond() {
            mutex = {};
            cond = PTHREAD_COND_INITIALIZER;

            pthread_condattr_t attr;
            pthread_condattr_init(&attr);

            pthread_cond_init(&cond, &attr);
        }

        void Event::waitFor(glong millis) const {
            if (millis == 0) {
                do {
                    pthread_cond_wait(&cond, &mutex);
                } while (errno != EINTR);
            }else {
                time::LocalDateTime ldt = time::LocalDateTime::now().plus(millis, time::Temporal::MILLIS);
                timespec ts = {};
                ts.tv_sec = ldt.toEpochSecond();
                ts.tv_nsec = ldt.nano();
                clock_gettime(CLOCK_MONOTONIC, &ts);
                do {
                    pthread_cond_timedwait(&cond, &mutex, &ts);
                }while (errno == EINTR);
            }
        }

        void Event::notify() const {
            pthread_cond_signal(&cond);
        }

        Event::~Event() {
            notify();
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
