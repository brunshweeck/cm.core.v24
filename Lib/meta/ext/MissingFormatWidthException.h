//
// Created by admin on 03/12/24.
//

#ifndef MISSINGFORMATWIDTHEXCEPTION_H
#define MISSINGFORMATWIDTHEXCEPTION_H
#include "core/util/IllegalFormatException.h"

namespace core {
    namespace ext {
        /**
         * Unchecked exception thrown when the format width is required.
         *
         * <p> Unless otherwise specified, passing a {@code null} argument to any
         * method or constructor in this class will cause a {@link
         * NullPointerException} to be thrown.
         */
        class MissingFormatWidthException : public util::IllegalFormatException {
            String s;
        public:

            /**
             * Constructs an instance of this class with the specified format
             * specifier.
             *
             * @param  s
             *         The format specifier which does not have a width
             */
            explicit MissingFormatWidthException(String const& s);

            /**
             * Returns the format specifier which does not have a width.
             *
             * @return  The format specifier which does not have a width
             */
            String getFormatSpecifier() const { return s; }

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //MISSINGFORMATWIDTHEXCEPTION_H
