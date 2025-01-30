//
// Created by brunshweeck on 2 sept. 2024.
//

#include <core/charset/UnsupportedCharsetException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace charset {
        UnsupportedCharsetException::UnsupportedCharsetException(const String &charsetName)
                : Throwable(charsetName), IllegalArgumentException(charsetName) {}

        String UnsupportedCharsetException::charsetName() const { return charset; }

        Object &UnsupportedCharsetException::clone() const {
            try {
                return UNSAFE::newInstance<UnsupportedCharsetException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void UnsupportedCharsetException::selfThrow() const { throw UnsupportedCharsetException(*this); }
    } // charset
} // core
