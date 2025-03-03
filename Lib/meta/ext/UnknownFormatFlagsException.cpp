//
// Created by admin on 03/12/24.
//

#include "UnknownFormatFlagsException.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace ext {
        UnknownFormatFlagsException::UnknownFormatFlagsException(String const& flags): flags(flags) {}

        String const& UnknownFormatFlagsException::getFlags() { return flags; }

        String UnknownFormatFlagsException::message() const { return "Flags = " + flags; }

        Object& UnknownFormatFlagsException::clone() const {
            try {
                return UNSAFE::newInstance<UnknownFormatFlagsException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void UnknownFormatFlagsException::selfThrow() const {
            throw UnknownFormatFlagsException(*this);
        }
    } // ext
} // core
