//
// Created by admin on 08/01/25.
//

#include <core/Unix.h>
#include <core/time/LocalDateTime.h>
#include <core/time/LocalTime.h>

namespace core {
    namespace time {
        LocalTime LocalTime::now() {
            try {
                return LocalDateTime::now().toLocalTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalTime LocalTime::now(ZoneId const& zone) {
            try {
                return LocalDateTime::now(zone).toLocalTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    }
}
