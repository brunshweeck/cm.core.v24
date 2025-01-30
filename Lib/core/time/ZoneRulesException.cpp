//
// Created by admin on 29/12/24.
//

#include "ZoneRulesException.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace time {
        ZoneRulesException::ZoneRulesException(String const &message)
                : Throwable(message), DateTimeException(message) {}

        ZoneRulesException::ZoneRulesException(String const &message, Throwable const &cause)
                : Throwable(message, cause), DateTimeException(message, cause) {}

        Object &ZoneRulesException::clone() const {
            try {
                return UNSAFE::newInstance<ZoneRulesException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void ZoneRulesException::selfThrow() const { throw ZoneRulesException(*this); }
    } // time
} // core
