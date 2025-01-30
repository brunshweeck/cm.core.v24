//
// Created by admin on 03/12/24.
//

#include "IllegalFormatArgumentIndexException.h"

#include "core/misc/Unsafe.h"

namespace core {
    namespace ext {
        IllegalFormatArgumentIndexException::IllegalFormatArgumentIndexException(gint index): index(index) {}

        gint IllegalFormatArgumentIndexException::getIndex() const { return index; }

        String IllegalFormatArgumentIndexException::message() const {
            int index = getIndex();

            if (index == Integer::MIN_VALUE) {
                return "Format argument index: (not representable as int)";
            }

            return String::format("Illegal format argument index = %d", getIndex());
        }

        Object& IllegalFormatArgumentIndexException::clone() const {
            try {
                return UNSAFE::newInstance<IllegalFormatArgumentIndexException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void IllegalFormatArgumentIndexException::selfThrow() const {
            throw IllegalFormatArgumentIndexException(*this);
        }
    } // ext
} // core
