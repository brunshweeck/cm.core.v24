//
// Created by brunshweeck on 15 sept. 2024.
//

#ifndef CORE24_URISYNTAXEXCEPTION_H
#define CORE24_URISYNTAXEXCEPTION_H

#include <core/Exception.h>

namespace core {
    namespace net {
        /**
         * Checked exception thrown to indicate that a string could not be parsed as a
         * URI reference.
         *
         * @see URI
         */
        class URISyntaxException : public Exception {
            /**
             * The input string.
             */
            String errorInput;

            /**
             * The index at which the parse error occurred,
             * or @c -1 if the index is not known.
             */
            gint errorIndex;

        public:
            /**
             * Constructs an instance from the given input string, reason, and error
             * index.
             *
             * @param  input   The input string
             * @param  reason  A string explaining why the input could not be parsed
             * @param  index   The index at which the parse error occurred,
             *                 or @c -1 if the index is not known
             *
             * @throws  IllegalArgumentException
             *          If the error index is less than @c -1
             */
            CORE_EXPLICIT URISyntaxException(String input, String reason, gint index);

            /**
             * Constructs an instance from the given input string and reason.  The
             * resulting object will have an error index of @c -1.
             *
             * @param  input   The input string
             * @param  reason  A string explaining why the input could not be parsed
             */
            CORE_EXPLICIT URISyntaxException(String input, String reason);

            /**
             * Returns the input string.
             *
             * @return  The input string
             */
            String input() const;

            /**
             * Returns a string explaining why the input string could not be parsed.
             *
             * @return  The reason string
             */
            String reason() const;

            /**
             * Returns an index into the input string of the position at which the
             * parse error occurred, or @c -1 if this position is not known.
             *
             * @return  The error index
             */
            gint index() const;

            /**
             * Returns a string describing the parse error.  The resulting string
             * consists of the reason string followed by a colon character
             * (@c ':'), a space, and the input string.  If the error index is
             * defined then the string @c " at index " followed by the index, in
             * decimal, is inserted after the reason string and before the colon
             * character.
             *
             * @return  A string describing the parse error
             */
            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // net
} // core

#endif //CORE24_URISYNTAXEXCEPTION_H
