//
// Created by brunshweeck on 22/02/25.
//

#include "DateTimeParseException.h"

#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    DateTimeParseException::DateTimeParseException(String const& message, CharSequence const& parsedData,
                                                   gint errorIndex)
      : Throwable(message), DateTimeException(message), text(parsedData.toString()), index(errorIndex) {}

    DateTimeParseException::DateTimeParseException(String const& message, CharSequence const& parsedData,
                                                   gint errorIndex, Throwable const& cause)
      : Throwable(message, cause), DateTimeException(message, cause),
        text(parsedData.toString()), index(errorIndex) {}

    String DateTimeParseException::parsedString() const {
      return text;
    }

    gint DateTimeParseException::errorIndex() const {
      return index;
    }

    Object& DateTimeParseException::clone() const {
      return UNSAFE::newInstance<DateTimeParseException>(*this);
    }

    void DateTimeParseException::selfThrow() const {
      throw DateTimeParseException(*this);
    }
  } // time
} // core
