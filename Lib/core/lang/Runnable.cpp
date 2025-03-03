//
// Created by bruns on 04/10/2024.
//

#include "Runnable.h"
#include <core/misc/Unsafe.h>

namespace core {
    Runnable::Runnable(const Runnable &other) {
        if (other.executor) {
            executor = other.executor;
            executor->acquire();
        }
    }

    Runnable::~Runnable() {
        if (executor && executor->release() == 0)
            delete executor;
    }

    Runnable::Runnable(Runnable &&other) noexcept {
        UNSAFE::swapValues(executor, other.executor);
    }

    Runnable & Runnable::operator=(const Runnable &other) {
        if (this != &other) {
            if (executor && executor->release() == 0)
                delete executor;
            executor = other.executor;
            executor->acquire();
        }
        return *this;
    }

    Runnable & Runnable::operator=(Runnable &&other) noexcept {
        if (this != &other)
            UNSAFE::swapValues(executor, other.executor);
        return *this;
    }

    void Runnable::run() const {
        if (executor)
            executor->execute();
    }

    gbool Runnable::equals(const Object &other) const {
        return this == &other ||
               Class<Runnable>::hasInstance(other) &&
               executor == CORE_XCAST(Runnable const, other).executor;
    }

    Object & Runnable::clone() const {
        try {
            return UNSAFE::newInstance<Runnable>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
}
