//
// Created by admin on 03/12/24.
//

#include "DuplicateFormatFlagsException.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace ext {
        DuplicateFormatFlagsException::DuplicateFormatFlagsException(String const& f): flags(f) {}

        String DuplicateFormatFlagsException::getFlags() const { return flags; }

        String DuplicateFormatFlagsException::message() const {
            return String::format("Flags = '%s'", flags);
        }

        Object& DuplicateFormatFlagsException::clone() const {
            try {
                return UNSAFE::newInstance<DuplicateFormatFlagsException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void DuplicateFormatFlagsException::selfThrow() const {
            throw DuplicateFormatFlagsException(*this);
        }
    } // ext
} // core
