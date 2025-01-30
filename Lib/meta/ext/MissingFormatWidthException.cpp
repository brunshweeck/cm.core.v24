//
// Created by admin on 03/12/24.
//

#include "MissingFormatWidthException.h"

#include "core/misc/Unsafe.h"

namespace core {
    namespace ext {
        MissingFormatWidthException::MissingFormatWidthException(String const& s): s(s) {}

        String MissingFormatWidthException::message() const {
            return s;
        }

        Object& MissingFormatWidthException::clone() const {
            try {
                return UNSAFE::newInstance<MissingFormatWidthException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void MissingFormatWidthException::selfThrow() const {
            throw MissingFormatWidthException(*this);
        }
    } // ext
} // core
