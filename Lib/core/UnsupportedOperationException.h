//
// Created by brunshweeck on 6 juin 2024.
//

#ifndef CORE24_UNSUPPORTEDOPERATIONEXCEPTION_H
#define CORE24_UNSUPPORTEDOPERATIONEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    class UnsupportedOperationException : public virtual RuntimeException {
    public:
        UnsupportedOperationException() = default;

        CORE_EXPLICIT UnsupportedOperationException(String const& message);

        CORE_EXPLICIT UnsupportedOperationException(String const& message, Throwable const& cause);

        CORE_EXPLICIT UnsupportedOperationException(Throwable const& cause);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_UNSUPPORTEDOPERATIONEXCEPTION_H
