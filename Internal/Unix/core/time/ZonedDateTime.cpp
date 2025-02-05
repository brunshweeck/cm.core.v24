//
// Created by admin on 08/01/25.
//

#include <native/System.h>
#include <core/time/ZonedDateTime.h>
#include <core/time/ZoneRules.h>

namespace core {
    namespace time {
        ZonedDateTime ZonedDateTime::now() {
            LOCALTIME lt = { };
            try {
                lt = GetLocalTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            LocalDateTime dateTime = LocalDateTime(lt.dwYear, lt.dwMonth, lt.dwDayOfMonth,
                                                   lt.dwHour, lt.dwMinute, lt.dwSecond, lt.dwNano);
            ZoneOffset offset = ZoneOffset(lt.dwOffset);

            try {
                return ZonedDateTime(dateTime, offset, ZoneId::getDefault());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZonedDateTime ZonedDateTime::now(ZoneId const& zone) {
            LOCALTIME lt = { };
            try {
                lt = GetLocalTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            LocalDateTime dateTime = LocalDateTime(lt.dwYear, lt.dwMonth, lt.dwDayOfMonth,
                                                   lt.dwHour, lt.dwMinute, lt.dwSecond, lt.dwNano);
            ZoneOffset offset = ZoneOffset(lt.dwOffset);

            INT offset2 = 0;
            if (Class<ZoneOffset>::hasInstance(zone)) {
                offset2 = CORE_XCAST(ZoneOffset const, zone).totalSeconds();
            } else {
                ZoneRules rules = zone.getRules();
                offset2 = rules.offset(dateTime).totalSeconds();
            }

            dateTime = dateTime.plusSeconds(offset2 - offset.totalSeconds());
            offset = ZoneOffset(offset2);

            try {
                return ZonedDateTime(dateTime, zone);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    }
}
