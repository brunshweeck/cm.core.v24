//
// Created by brunshweeck on 24 juil. 2024.
//

#include <core/misc/Unsafe.h>
#include <core/util/MissingKeyException.h>

namespace core {
    namespace util {
        MissingKeyException::MissingKeyException(String const& message)
            : Throwable(message) {}

        MissingKeyException::MissingKeyException(Object const& key)
            : Throwable(String::valueOf(key)) {}

        MissingKeyException::MissingKeyException(String const& message, Throwable const& cause)
            : Throwable(message, cause) {}

        MissingKeyException::MissingKeyException(Object const& key, Throwable const& cause)
            : Throwable(String::valueOf(key), cause) {}

        Object& MissingKeyException::clone() const {
            try {
                return UNSAFE::newInstance<MissingKeyException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
         }

        void MissingKeyException::selfThrow() const {
            throw MissingKeyException(*this);
        }
    } // util
} // core
