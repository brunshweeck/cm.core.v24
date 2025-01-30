//
// Created by brunshweeck on 26 août 2024.
//

#ifndef CORE24_BUFFEROVERFLOWEXCEPTION_H
#define CORE24_BUFFEROVERFLOWEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace io {
        /**
         * Unchecked exception thrown when a relative <i>put</i> operation reaches
         * the target buffer's limit.
         */
        class BufferOverflowException final : public virtual RuntimeException {
        public:
            /**
             * Constructs an instance of this class.
             */
            CORE_IMPLICIT BufferOverflowException() = default;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // io
} // core

#endif //CORE24_BUFFEROVERFLOWEXCEPTION_H
