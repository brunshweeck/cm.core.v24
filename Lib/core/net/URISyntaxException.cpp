//
// Created by brunshweeck on 15 sept. 2024.
//

#include "URISyntaxException.h"

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace net {
    URISyntaxException::URISyntaxException(String input, String reason, gint index)
      : Exception(UNSAFE::moveInstance(reason)),
        errorInput(UNSAFE::moveInstance(input)),
        errorIndex(index) {
      if (index < -1)
        IllegalArgumentException().throws($ftrace());
    }

    URISyntaxException::URISyntaxException(String input, String reason)
      : URISyntaxException(UNSAFE::moveInstance(input), UNSAFE::moveInstance(reason), -1) {}

    String URISyntaxException::input() const { return errorInput; }

    String URISyntaxException::reason() const { return Throwable::message(); }

    gint URISyntaxException::index() const { return errorIndex; }

    String URISyntaxException::message() const {
      XString xs = XString(255);
      xs.append(reason());
      if (errorIndex > -1) {
        xs.append(" at index ");
        xs.append(errorIndex);
      }
      xs.append(": ");
      xs.append(errorInput);
      return xs.toString();
    }

    Object& URISyntaxException::clone() const {
      try {
        return UNSAFE::newInstance<URISyntaxException>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void URISyntaxException::selfThrow() const { throw URISyntaxException(*this); }
  } // net
} // core
