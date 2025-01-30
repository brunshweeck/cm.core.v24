//
// Created by bruns on 13/10/2024.
//

#ifndef CORE24_EOFEXCEPTION_H
#define CORE24_EOFEXCEPTION_H

#include <core/io/IOException.h>

namespace core {
    namespace io {
        /**
         * Signals that an end of file or end of stream has been reached
         * unexpectedly during input.
         * <p>
         * This exception is mainly used by data input streams to signal end of
         * stream. Note that many other input operations return a special value on
         * end of stream rather than throwing an exception.
         * </p>
         * @see io::DataInputStream
         * @see io::IOException
         */
        class EOFException final : public virtual IOException {
        public:
            /**
             * Constructs an @c EOFException with empty string
             * as its error detail message.
             */
            CORE_IMPLICIT EOFException() = default;

            /**
             * Constructs an @c EOFException with the specified detail
             * message. The string @c message may later be retrieved by the
             * @b Throwable::message method of class @b Throwable.
             *
             * @param message   the detail message.
             */
            CORE_EXPLICIT EOFException(String const& message);

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // io
} // core

#endif //CORE24_EOFEXCEPTION_H
