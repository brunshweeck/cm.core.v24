//
// Created by brunshweeck on 26 août 2024.
//

#ifndef CORE24_BUFFERUNDERFLOWEXCEPTION_H
#define CORE24_BUFFERUNDERFLOWEXCEPTION_H
#include <core/lang/RuntimeException.h>

namespace core {
    namespace io {
        /**
         * Unchecked exception thrown when a relative <i>get</i> operation reaches
         * the source buffer's limit.
         */
        class BufferUnderflowException final : public virtual RuntimeException {
        public:
            /**
             * Constructs an instance of this class.
             */
            CORE_IMPLICIT BufferUnderflowException() = default;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // io
} // core

#endif //CORE24_BUFFERUNDERFLOWEXCEPTION_H
