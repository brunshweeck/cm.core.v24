//
// Created by brunshweeck on 26 août 2024.
//

#ifndef CORE24_INVALIDMARKEXCEPTION_H
#define CORE24_INVALIDMARKEXCEPTION_H
#include <core/lang/IllegalStateException.h>

namespace core {
    namespace io {
        /**
         * Unchecked exception thrown when an attempt is made to reset a buffer
         * when its mark is not defined.
         */
        class InvalidMarkException final : public virtual IllegalStateException {
        public:
            /**
             * Constructs an instance of this class.
             */
            CORE_IMPLICIT InvalidMarkException() = default;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // io
} // core

#endif //CORE24_INVALIDMARKEXCEPTION_H
