//
// Created by admin on 03/12/24.
//

#include "IllegalFormatPrecisionException.h"

#include "core/misc/Unsafe.h"

namespace core {
    namespace ext {
        IllegalFormatPrecisionException::IllegalFormatPrecisionException(gint precision): precision(precision) {}

        gint IllegalFormatPrecisionException::getPrecision() const { return precision; }

        String IllegalFormatPrecisionException::message() const {
            return Integer::toString(precision);
        }

        Object& IllegalFormatPrecisionException::clone() const {
            try {
                return UNSAFE::newInstance<IllegalFormatPrecisionException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void IllegalFormatPrecisionException::selfThrow() const {
            throw IllegalFormatPrecisionException(*this);
        }
    } // ext
} // core
