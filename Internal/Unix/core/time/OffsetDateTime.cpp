//
// Created by admin on 08/01/25.
//

#include <native/System.h>
#include <core/time/OffsetDateTime.h>

#include "core/time/ZoneRules.h"

namespace core {
    namespace time {
        OffsetDateTime OffsetDateTime::now() {
            LOCALTIME lt = { };
            try {
                lt = GetLocalTime();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            LocalDateTime dateTime = LocalDateTime(lt.dwYear, lt.dwMonth, lt.dwDayOfMonth,
                                                   lt.dwHour, lt.dwMinute, lt.dwSecond, lt.dwNano);
            ZoneOffset offset = ZoneOffset(lt.dwOffset);
            return OffsetDateTime(dateTime, offset);
        }

        OffsetDateTime OffsetDateTime::now(ZoneId const& zone) {
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

            return OffsetDateTime(dateTime, offset);
        }
    }
}
