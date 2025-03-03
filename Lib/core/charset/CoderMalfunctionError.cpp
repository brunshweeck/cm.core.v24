//
// Created by brunshweeck on 1 sept. 2024.
//

#include <core/charset/CoderMalfunctionError.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace charset {
    CoderMalfunctionError::CoderMalfunctionError(Exception const& cause)
      : Throwable(cause.toString(), cause), Error(cause) {}

    Object& CoderMalfunctionError::clone() const {
      try {
        return UNSAFE::newInstance<CoderMalfunctionError>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void CoderMalfunctionError::selfThrow() const { throw CoderMalfunctionError(*this); }
  } // charset
} // core
