//
// Created by brunshweeck on 18 août 2024.
//

#include <core/time/LocalDateTime.h>
#include <core/lang/Windows.h>

namespace core {
    namespace time {
        LocalDateTime LocalDateTime::systemDateTime() {
            SYSTEMTIME time = {};
            GetSystemTime(&time);
            return LocalDateTime(time.wYear, time.wMonth, time.wDay,
                                 time.wHour, time.wMinute, time.wSecond, time.wMilliseconds * 1000000);
        }
    }
}
