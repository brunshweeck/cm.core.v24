//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_TEMPORALEXCEPTION_H
#define CORE24_TEMPORALEXCEPTION_H
#include <core/time/DateTimeException.h>

namespace core {
    namespace time {
        /**
         * thrown to indicates that a ChronoField or ChronoUnit is
         * not supported for a Temporal class.
         *
         * @note
         * This class is intended for use in a single thread.
         */
        class TemporalException final : public virtual DateTimeException  {
        public:

            /**
             * Constructs a new TemporalException with the specified message.
             *
             * @param message  the message to use for this exception, may be null
             */
            CORE_EXPLICIT TemporalException(String const &message);

            /**
             * Constructs a new TemporalException with the specified message and cause.
             *
             * @param message  the message to use for this exception, may be null
             * @param cause  the cause of the exception, may be null
             */
            CORE_EXPLICIT TemporalException(String const &message, Throwable const &cause);

            /**
             * Return shadow copy of this object
             */
            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // time
} // core

#endif //CORE24_TEMPORALEXCEPTION_H
