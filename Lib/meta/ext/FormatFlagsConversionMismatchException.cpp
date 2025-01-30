//
// Created by admin on 03/12/24.
//

#include "FormatFlagsConversionMismatchException.h"

#include "core/misc/Unsafe.h"

namespace core {
    namespace ext {
        FormatFlagsConversionMismatchException::FormatFlagsConversionMismatchException(String const& flags, gchar c)
            : flags(flags),
              c(c) {}

        String FormatFlagsConversionMismatchException::getFlags() const { return flags; }

        gchar FormatFlagsConversionMismatchException::getConversion() const { return c; }

        String FormatFlagsConversionMismatchException::message() const {
            return "Conversion = "_Sl + c + ", Flags = " + flags;
        }

        Object& FormatFlagsConversionMismatchException::clone() const {
            try {
                return UNSAFE::newInstance<FormatFlagsConversionMismatchException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void FormatFlagsConversionMismatchException::selfThrow() const {
            throw FormatFlagsConversionMismatchException(*this);
        }
    } // ext
} // core
