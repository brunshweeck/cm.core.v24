//
// Created by admin on 03/12/24.
//

#include "IllegalFormatWidthException.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace ext {
        IllegalFormatWidthException::IllegalFormatWidthException(gint width): width(width) {}

        gint IllegalFormatWidthException::getWidth() const { return width; }

        String IllegalFormatWidthException::message() const {
            return Integer::toString(width);
        }

        Object& IllegalFormatWidthException::clone() const {
            try {
                return UNSAFE::newInstance<IllegalFormatWidthException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void IllegalFormatWidthException::selfThrow() const {
            throw IllegalFormatWidthException(*this);
        }
    } // ext
} // core
