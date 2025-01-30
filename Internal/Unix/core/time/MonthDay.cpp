//
// Created by admin on 08/01/25.
//

#include <core/Unix.h>
#include <core/time/MonthDay.h>

namespace core {
    namespace time {
        MonthDay MonthDay::now() {
            try {
                LocalDate ld = LocalDate::now();
                return MonthDay(ld.month(), ld.dayOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        MonthDay MonthDay::now(ZoneId const& zone) {
            try {
                LocalDate ld = LocalDate::now(zone);
                return MonthDay(ld.month(), ld.dayOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    }
}
