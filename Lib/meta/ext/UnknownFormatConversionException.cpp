//
// Created by admin on 03/12/24.
//

#include "UnknownFormatConversionException.h"

#include "core/misc/Unsafe.h"

namespace core {
    namespace ext {
        UnknownFormatConversionException::UnknownFormatConversionException(String const& s): s(s) {}

        String UnknownFormatConversionException::getConversion() const { return s; }

        String UnknownFormatConversionException::message() const {
            return String::format("Conversion = '%s'", s);
        }

        Object& UnknownFormatConversionException::clone() const {
            try {
                return UNSAFE::newInstance<UnknownFormatConversionException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void UnknownFormatConversionException::selfThrow() const {
            throw UnknownFormatConversionException(*this);
        }
    } // ext
} // core
