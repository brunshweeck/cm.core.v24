//
// Created by admin on 03/12/24.
//

#include "IllegalFormatCodePointException.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace ext {
        IllegalFormatCodePointException::IllegalFormatCodePointException(gint codePoint): codePoint(codePoint) {}

        gint IllegalFormatCodePointException::getCodePoint() const { return codePoint; }

        String IllegalFormatCodePointException::message() const {
            return String::format("Code point = %#x", codePoint);
        }

        Object& IllegalFormatCodePointException::clone() const {
            try {
                return UNSAFE::newInstance<IllegalFormatCodePointException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void IllegalFormatCodePointException::selfThrow() const {
            throw IllegalFormatCodePointException(*this);
        }
    } // ext
} // core
