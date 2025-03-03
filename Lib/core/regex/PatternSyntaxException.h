//
// Created by bruns on 13/10/2024.
//

#ifndef CORE24_PATTERNSYNTAXEXCEPTION_H
#define CORE24_PATTERNSYNTAXEXCEPTION_H

#include <core/lang/IllegalArgumentException.h>

namespace core {
    namespace regex {
        /**
         * Unchecked exception thrown to indicate a syntax error in a
         * regular-expression pattern.
         */
        class PatternSyntaxException : public virtual IllegalArgumentException {
            String desc;
            String regex;
            gint index;

        public:
            /**
             * Constructs a new instance of this class.
             *
             * @param  desc
             *         A description of the error
             *
             * @param  regex
             *         The erroneous pattern
             *
             * @param  index
             *         The approximate index in the pattern of the error,
             *         or {@code -1} if the index is not known
             */
            CORE_EXPLICIT PatternSyntaxException(const String &desc, const String &regex, gint index);

            /**
             * Retrieves the error index.
             *
             * @return  The approximate index in the pattern of the error,
             *         or {@code -1} if the index is not known
             */
            gint getIndex() const;

            /**
             * Retrieves the description of the error.
             *
             * @return  The description of the error
             */
            String description() const;

            /**
             * Retrieves the erroneous regular-expression pattern.
             *
             * @return  The erroneous pattern
             */
            String pattern() const;

            /**
             * Returns a multi-line string containing the description of the syntax
             * error and its index, the erroneous regular-expression pattern, and a
             * visual indication of the error index within the pattern.
             *
             * @return  The full detail message
             */
            String message() const override;

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // regex
} // core

#endif //CORE24_PATTERNSYNTAXEXCEPTION_H
