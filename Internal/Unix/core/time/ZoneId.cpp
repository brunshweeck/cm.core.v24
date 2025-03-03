//
// Created by admin on 08/01/25.
//

#include <native/System.h>
#include <core/time/ZoneId.h>

#include <core/time/ZoneOffset.h>
#include <core/util/Optional.h>

namespace core {
  using namespace util;

  namespace time {
    ZoneId const& ZoneId::systemZone() {
      static OptionalZone defaultZone;

      if (defaultZone.isPresent())
        return defaultZone.get();

      CHAR tmp[256] = { };
      LPSTR lpBuffer = tmp;
      DWORD len = 0;
      try {
        len = GetLocalTimeZone(lpBuffer, 256);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }


      try {
        if (len == 0) {
          LOCALTIME lt = GetLocalTime();
          defaultZone = ZoneOffset::ofTotalSeconds(lt.dwOffset);
        } else {
          defaultZone = of(lpBuffer);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }

      return defaultZone.get();
    }
  }
}
