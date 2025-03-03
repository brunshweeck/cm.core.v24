//
// Created by brunshweeck on 1 sept. 2024.
//

#include <core/charset/IllegalCharsetNameException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace charset {
        IllegalCharsetNameException::IllegalCharsetNameException(String charsetName)
                : Throwable(charsetName), IllegalArgumentException(charsetName),
                  illegalCharsetName(UNSAFE::moveInstance(charsetName)) {}

        String IllegalCharsetNameException::charsetName() const { return illegalCharsetName; }

        Object &IllegalCharsetNameException::clone() const {
            try {
                return UNSAFE::newInstance<IllegalCharsetNameException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void IllegalCharsetNameException::selfThrow() const {
            throw IllegalCharsetNameException(*this);
        }
    } // io
} // core
