//
// Created by bruns on 01/10/2024.
//

#include <core/InterruptedException.h>
#include <core/misc/Unsafe.h>

namespace core {

    InterruptedException::InterruptedException(const String& s)
        : Exception(s), Throwable(s) {
    }

    Object & InterruptedException::clone() const {
        try {
            return UNSAFE::newInstance<InterruptedException>(*this);
        }catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void InterruptedException::selfThrow() const {
        throw InterruptedException(*this);
    }
} // core