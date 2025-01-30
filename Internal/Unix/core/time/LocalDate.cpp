//
// Created by admin on 08/01/25.
//

#include <core/Unix.h>
#include <core/time/LocalDate.h>
#include <core/time/LocalDateTime.h>

namespace core {
    namespace time {
        LocalDate LocalDate::now() {
            try {
                return LocalDateTime::now().toLocalDate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate LocalDate::now(ZoneId const& zone) {
            try {
                return LocalDateTime::now(zone).toLocalDate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    }
}
