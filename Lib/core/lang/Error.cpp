//
// Created by brunshweeck on 25 mai 2024.
//

#include "Error.h"
#include <core/misc/Unsafe.h>

namespace core {
  Error::Error(String const& message) CORE_NOTHROW: Throwable(message) {}

  Error::Error(String const& message, const Throwable& cause) : Throwable(message, cause) {}

  Error::Error(const Throwable& cause) : Throwable(cause.toString(), cause) {}

  Object& Error::clone() const {
    try {
      return UNSAFE::newInstance<Error>(*this);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  void Error::selfThrow() const {
    throw Error(*this);
  }
} // core
