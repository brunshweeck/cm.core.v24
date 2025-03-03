//
// Created by admin on 03/12/24.
//

#ifndef ILLEGALFORMATCODEPOINTEXCEPTION_H
#define ILLEGALFORMATCODEPOINTEXCEPTION_H

#include <core/util/IllegalFormatException.h>

namespace core {
    namespace ext {

        /**
         * Unchecked exception thrown when a character with an invalid Unicode code
         * point as defined by {@link Character#isValidCodePoint} is passed to the
         * {@link Formatter}.
         *
         * <p> Unless otherwise specified, passing a {@code null} argument to any
         * method or constructor in this class will cause a {@link
         * NullPointerException} to be thrown.
         *
         * @since 1.5
         */
        class IllegalFormatCodePointException: public util::IllegalFormatException {
            gint codePoint;

        public:
            CORE_EXPLICIT IllegalFormatCodePointException(gint codePoint);

            /**
             * Returns the illegal code point as defined by {@link
             * Character#isValidCodePoint}.
             *
             * @return  The illegal Unicode code point
             */
            gint getCodePoint() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //ILLEGALFORMATCODEPOINTEXCEPTION_H
