//
// Created by admin on 29/12/24.
//

#ifndef CORE24_ZONERULESEXCEPTION_H
#define CORE24_ZONERULESEXCEPTION_H

#include <core/time/DateTimeException.h>

namespace core {
    namespace time {
        /**
         * Thrown to indicate a problem with time-zone configuration.
         * <p>
         * This exception is used to indicate a problems with the configured
         * time-zone rules.
         * </p>
         * @note
         * This class is intended for use in a single thread.
         */
        class ZoneRulesException : public virtual DateTimeException {
        public:
            /**
             * Constructs a new date-time exception with the specified message.
             *
             * @param message  the message to use for this exception
             */
            CORE_EXPLICIT ZoneRulesException(String const& message);

            /**
             * Constructs a new date-time exception with the specified message and cause.
             *
             * @param message  the message to use for this exception
             * @param cause  the cause of the exception
             */
            CORE_EXPLICIT ZoneRulesException(String const& message, Throwable const& cause);

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // time
} // core

#endif //CORE24_ZONERULESEXCEPTION_H
