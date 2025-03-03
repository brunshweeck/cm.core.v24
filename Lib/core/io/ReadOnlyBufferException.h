//
// Created by brunshweeck on 27 août 2024.
//

#ifndef CORE24_READONLYBUFFEREXCEPTION_H
#define CORE24_READONLYBUFFEREXCEPTION_H

#include <core/lang/UnsupportedOperationException.h>

namespace core {
    namespace io {
        /**
         * Unchecked exception thrown when a content-mutation method such as
         * @c put or @c compact is invoked upon a read-only buffer.
         */
        class ReadOnlyBufferException final : public UnsupportedOperationException {
        public:
            /**
             * Constructs an instance of this class.
             */
            ReadOnlyBufferException() = default;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // io
} // core

#endif //CORE24_READONLYBUFFEREXCEPTION_H
