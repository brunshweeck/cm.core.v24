//
// Created by brunshweeck on 3 août 2024.
//

#include <core/time/DateTimeException.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    DateTimeException::DateTimeException(String const& message)
      : Throwable(message), RuntimeException(message) {}

    DateTimeException::DateTimeException(String const& message, Throwable const& cause)
      : Throwable(message, cause), RuntimeException(message, cause) {}

    Object& DateTimeException::clone() const {
      try {
        return UNSAFE::newInstance<DateTimeException>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); };
    }

    void DateTimeException::selfThrow() const {
      throw DateTimeException(*this);
    }

    //
  } // time
} // core
