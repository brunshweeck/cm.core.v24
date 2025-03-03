//
// Created by brunshweeck on 22/02/25.
//

#ifndef CORE24_DATETIMEPARSEEXCEPTION_H
#define CORE24_DATETIMEPARSEEXCEPTION_H

#include <core/time/DateTimeException.h>

namespace core {
    namespace time {
        /**
         * An exception thrown when an error occurs during parsing.
         * <p>
         * This exception includes the text being parsed and the error index.
         * </p>
         * @note
         * This class is intended for use in a single thread.
         */
        class DateTimeParseException : public DateTimeException {
            /**
             * The text that was being parsed.
             */
            String text;

            /**
             * The error index in the text.
             */
            gint index;

        public:
            /**
             * Constructs a new exception with the specified message.
             *
             * @param message  the message to use for this exception
             * @param parsedData  the parsed text
             * @param errorIndex  the index in the parsed string that was invalid, should be a valid index
             */
            CORE_EXPLICIT DateTimeParseException(String const& message, CharSequence const& parsedData,
                                                 gint errorIndex);

            /**
             * Constructs a new exception with the specified message and cause.
             *
             * @param message  the message to use for this exception
             * @param parsedData  the parsed text
             * @param errorIndex  the index in the parsed string that was invalid, should be a valid index
             * @param cause  the cause exception
             */
            CORE_EXPLICIT DateTimeParseException(String const& message, CharSequence const& parsedData,
                                                 gint errorIndex, Throwable const& cause);

            /**
             * Returns the string that was being parsed.
             *
             * @return the string that was being parsed.
             */
            String parsedString() const;

            /**
             * Returns the index where the error was found.
             *
             * @return the index in the parsed string that was invalid, should be a valid index
             */
            gint errorIndex() const;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // time
} // core

#endif //CORE24_DATETIMEPARSEEXCEPTION_H
