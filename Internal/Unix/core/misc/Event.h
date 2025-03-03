//
// Created by bruns on 22/03/2024.
//

#ifndef CORE24_UNIX_EVENT_H
#define CORE24_UNIX_EVENT_H

#include <core/misc/Unsafe.h>
#include <pthread.h>

namespace core {
    namespace misc {
        class Event final : public Object {
            pthread_mutex_t mutable mutex;
            pthread_cond_t mutable cond;

        public:
            Event();

            void waitFor(glong millis) const;

            void notify() const;

            ~Event() override;

            static void park(Object &thread, glong parkBlockerOffset, glong parkEventOffset, glong millis);

            static void unpark(Object &thread, glong parkBlockerOffset, glong parkEventOffset);

            class EventBlocker : public Object {
                ~EventBlocker() override = default;
            };
        };
    }
} // core

#endif //CORE24_UNIX_EVENT_H
