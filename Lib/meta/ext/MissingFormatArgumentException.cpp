//
// Created by admin on 03/12/24.
//

#include "MissingFormatArgumentException.h"

#include "core/misc/Unsafe.h"

namespace core {
    namespace ext {
        MissingFormatArgumentException::MissingFormatArgumentException(String const& s): s(s) {}

        String MissingFormatArgumentException::getFormatSpecifier() const { return s; }

        String MissingFormatArgumentException::message() const {
            return "Format specifier '" + s + "'";
        }

        Object& MissingFormatArgumentException::clone() const {
            try {
                return UNSAFE::newInstance<MissingFormatArgumentException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void MissingFormatArgumentException::selfThrow() const {
            throw MissingFormatArgumentException(*this);
        }
    } // ext
} // core
