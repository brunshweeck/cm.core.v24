//
// Created by brunshweeck on 25 mai 2024.
//

#include "Exception.h"
#include <core/misc/Unsafe.h>

namespace core {
  Exception::Exception(String const& message) CORE_NOTHROW: Throwable(message) {}

  Exception::Exception(String const& message, const Throwable& cause) : Throwable(message, cause) {}

  Exception::Exception(const Throwable& cause) : Throwable(cause.toString(), cause) {}

  Object& Exception::clone() const {
    try {
      return UNSAFE::newInstance<Exception>(*this);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  void Exception::selfThrow() const {
    throw Exception(*this);
  }
} // core
