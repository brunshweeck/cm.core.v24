//
// Created by brunshweeck on 3 août 2024.
//

#include <core/time/TemporalException.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    TemporalException::TemporalException(String const& message)
      : DateTimeException(message), Throwable(message) {}

    TemporalException::TemporalException(String const& message, Throwable const& cause)
      : DateTimeException(message, cause), Throwable(message, cause) {}

    Object& TemporalException::clone() const {
      try {
        return UNSAFE::newInstance<DateTimeException>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void TemporalException::selfThrow() const {
      throw TemporalException(*this);
    }
  } // time
} // core
