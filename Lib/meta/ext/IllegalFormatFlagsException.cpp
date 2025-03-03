//
// Created by admin on 03/12/24.
//

#include "IllegalFormatFlagsException.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace ext {
        IllegalFormatFlagsException::IllegalFormatFlagsException(String const& flags): flags(flags) {}

        String IllegalFormatFlagsException::getFlags() const { return flags; }

        String IllegalFormatFlagsException::message() const {
            return "Flags = '" + flags + "'";
        }

        Object& IllegalFormatFlagsException::clone() const {
            try {
                return UNSAFE::newInstance<IllegalFormatFlagsException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void IllegalFormatFlagsException::selfThrow() const {
            throw IllegalFormatFlagsException(*this);
        }
    } // ext
} // core
