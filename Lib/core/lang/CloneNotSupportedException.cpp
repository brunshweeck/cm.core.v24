//
// Created by brunshweeck on 8 juin 2024.
//

#include "CloneNotSupportedException.h"
#include <core/misc/Unsafe.h>

namespace core {
  CloneNotSupportedException::CloneNotSupportedException(String const& message) CORE_NOTHROW:
    RuntimeException(message), Throwable(message) {}

  Object& CloneNotSupportedException::clone() const {
    try {
      return UNSAFE::newInstance<CloneNotSupportedException>(*this);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  void CloneNotSupportedException::selfThrow() const {
    throw CloneNotSupportedException(*this);
  }
} // core
