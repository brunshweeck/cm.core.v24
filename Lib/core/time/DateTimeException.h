//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_DATETIMEEXCEPTION_H
#define CORE24_DATETIMEEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    namespace time {

        /**
         * Exception used to indicate a problem while calculating a date-time.
         * <p>
         * This exception is used to indicate problems with creating, querying
         * and manipulating date-time objects.
         * </p>
         * @note
         * This class is intended for use in a single thread.
         */
        class DateTimeException : public virtual RuntimeException {
        public:
            /**
             * Constructs a new date-time exception with the specified message.
             *
             * @param message  the message to use for this exception
             */
            CORE_EXPLICIT DateTimeException(String const &message);

            /**
             * Constructs a new date-time exception with the specified message and cause.
             *
             * @param message  the message to use for this exception
             * @param cause  the cause of the exception
             */
            CORE_EXPLICIT DateTimeException(String const &message, Throwable const &cause);

            /**
             * Return shadow copy of this object
             */
            Object & clone() const override;

        protected:
            /**
             * thrown itself.
             */
            void selfThrow() const override;
        };
    } // time
} // core

#endif //CORE24_DATETIMEEXCEPTION_H
