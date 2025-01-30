//
// Created by brunshweeck on 24 juil. 2024.
//

#ifndef CORE24_MISSINGKEYEXCEPTION_H
#define CORE24_MISSINGKEYEXCEPTION_H

#include <core/util/NoSuchElementException.h>


namespace core {
    namespace util {
        /**
         * Thrown by various map accessor methods to indicate that the key
         * being requested is not mapped.
         */
        class MissingKeyException : public NoSuchElementException {
        public:
            /**
             * Construct new MissingKeyException without detail message
             */
            MissingKeyException() = default;


            /**
             * Construct new MissingKeyException with detail message
             * @param message the detail message
             */
            CORE_EXPLICIT MissingKeyException(String const &message);

            /**
             * Construct new MissingKeyException with requested key as message
             * @param key the requested key.
             */
            CORE_EXPLICIT MissingKeyException(Object const &key);

            /**
             * Construct new MissingKeyException with requested detail message
             * and cause
             * @param message the detail message key.
             * @param cause the cause.
             */
            CORE_EXPLICIT MissingKeyException(String const &message, Throwable const &cause);

            /**
             * Construct new MissingKeyException with requested key as message,
             * and cause
             * @param key the requested key.
             * @param cause the cause
             */
            CORE_EXPLICIT MissingKeyException(Object const &key, Throwable const &cause);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // util
} // core

#endif //CORE24_MISSINGKEYEXCEPTION_H
