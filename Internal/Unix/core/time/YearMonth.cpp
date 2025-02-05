//
// Created by admin on 08/01/25.
//

#include <core/time/YearMonth.h>
#include <native/System.h>

namespace core {
    namespace time {
        YearMonth YearMonth::now() {
            try {
                LocalDate ld = LocalDate::now();
                return YearMonth(ld.year(), ld.month());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        YearMonth YearMonth::now(ZoneId const& zone) {
            try {
                LocalDate ld = LocalDate::now(zone);
                return YearMonth(ld.year(), ld.month());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    }
}
