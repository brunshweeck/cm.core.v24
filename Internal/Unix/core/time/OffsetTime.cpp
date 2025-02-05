//
// Created by admin on 08/01/25.
//

#include <native/System.h>
#include <core/time/OffsetDateTime.h>
#include <core/time/OffsetTime.h>

namespace core {
    namespace time {
        OffsetTime OffsetTime::now() {
            try {
                return OffsetDateTime::now().toOffsetTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        OffsetTime OffsetTime::now(ZoneId const& zone) {
            try {
                return OffsetDateTime::now(zone).toOffsetTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    }
}
