//
// Created by admin on 03/12/24.
//

#include "IllegalFormatConversionException.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace ext {
        IllegalFormatConversionException::IllegalFormatConversionException(gchar c, Object const& obj)
            : c(c), arg(typeName(obj)) {}

        gchar IllegalFormatConversionException::getConversion() const { return c; }

        String IllegalFormatConversionException::getArgumentClass() const { return arg; }

        String IllegalFormatConversionException::message() const {
            return String::format("%c != %s", c, arg);
        }

        Object& IllegalFormatConversionException::clone() const {
            try {
                return UNSAFE::newInstance<IllegalFormatConversionException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void IllegalFormatConversionException::selfThrow() const {
            throw IllegalFormatConversionException(*this);
        }
    } // ext
} // core
