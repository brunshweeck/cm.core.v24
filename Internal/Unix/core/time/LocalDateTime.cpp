//
// Created by brunshweeck on 18 août 2024.
//

#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneRules.h>
#include <native/System.h>

namespace core {
    namespace time {
        LocalDateTime LocalDateTime::now() {
            LOCALTIME lt = { };
            try {
                lt = GetLocalTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            LocalDateTime ldt1 = LocalDateTime(
                lt.dwYear,
                lt.dwMonth,
                lt.dwDayOfMonth,
                lt.dwHour,
                lt.dwMinute,
                lt.dwSecond,
                lt.dwNano);
            return ldt1;
        }

        LocalDateTime LocalDateTime::now(ZoneId const& zone) {
            LOCALTIME lt = { };
            try {
                lt = GetLocalTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            LocalDateTime ldt1 = LocalDateTime(
                lt.dwYear,
                lt.dwMonth,
                lt.dwDayOfMonth,
                lt.dwHour,
                lt.dwMinute,
                lt.dwSecond,
                lt.dwNano);
            INT offset = 0;
            if (Class<ZoneOffset>::hasInstance(zone)) {
                offset = CORE_XCAST(ZoneOffset const, zone).totalSeconds();
            } else {
                ZoneRules rules = zone.getRules();
                offset = rules.offset(ldt1).totalSeconds();
            }

            offset -= lt.dwOffset;
            return ldt1.plusSeconds(offset);
        }
    }
}
