//
// Created by brunshweeck on 2 juil. 2024.
//

#ifndef CORE24_CONCURRENTMODIFICATIONEXCEPTION_H
#define CORE24_CONCURRENTMODIFICATIONEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace util {
        class ConcurrentModificationException : public virtual RuntimeException {
        public:
            ConcurrentModificationException() = default;

            CORE_EXPLICIT ConcurrentModificationException(String const& message);

            CORE_EXPLICIT ConcurrentModificationException(String const& message, Throwable const& cause);

            CORE_EXPLICIT ConcurrentModificationException(Throwable const& cause);

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // util
} // core

#endif //CORE24_CONCURRENTMODIFICATIONEXCEPTION_H
