//
// Created by bruns on 22/03/2024.
//

#ifndef CORE24_EVENT_H
#define CORE24_EVENT_H

#include <core/misc/Unsafe.h>

namespace core {
    namespace misc {
        class Event final : public Object {
            glong handle;

        public:
            Event();

            void waitFor(glong millis) const;

            void notify() const;

            ~Event() override;

            static void park(Object& thread, glong parkBlockerOffset, glong parkEventOffset, glong millis);

            static void unpark(Object& thread, glong parkBlockerOffset, glong parkEventOffset);

            class EventBlocker: public Object {
                ~EventBlocker() override = default;
            };
        };
    }
} // core

#endif //CORE24_EVENT_H
