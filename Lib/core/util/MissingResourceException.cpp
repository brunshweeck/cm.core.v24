//
// Created by brunshweeck on 11 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <core/util/MissingResourceException.h>

namespace core {
    namespace util {
        MissingResourceException::MissingResourceException(String const& message, String className, String key)
            : Throwable(message), RuntimeException(message),
              resourceClass(UNSAFE::moveInstance(className)),
              resourceKey(UNSAFE::moveInstance(key)) {}

        String MissingResourceException::className() const { return resourceClass; }

        String MissingResourceException::key() const { return resourceKey; }

        Object& MissingResourceException::clone() const {
            try {
                return UNSAFE::newInstance<MissingResourceException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void MissingResourceException::selfThrow() const {
            throw MissingResourceException(*this);
        }

        MissingResourceException::MissingResourceException(String const& message, String className, String key,
                                                           Throwable const& cause)
            : Throwable(message, cause), RuntimeException(message, cause),
              resourceClass(UNSAFE::moveInstance(className)),
              resourceKey(UNSAFE::moveInstance(key)) {}
    } // util
} // core
