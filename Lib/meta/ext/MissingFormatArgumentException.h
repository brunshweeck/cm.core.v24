//
// Created by admin on 03/12/24.
//

#ifndef MISSINGFORMATARGUMENTEXCEPTION_H
#define MISSINGFORMATARGUMENTEXCEPTION_H
#include <core/util/IllegalFormatException.h>

namespace core {
    namespace ext {
        /**
         * Unchecked exception thrown when there is a format specifier which does not
         * have a corresponding argument or if an argument index refers to an argument
         * that does not exist.
         *
         * <p> Unless otherwise specified, passing a {@code null} argument to any
         * method or constructor in this class will cause a {@link
         * NullPointerException} to be thrown.
         */
        class MissingFormatArgumentException : public util::IllegalFormatException {
            String s;

        public:

            /**
             * Constructs an instance of this class with the unmatched format
             * specifier.
             *
             * @param  s
             *         Format specifier which does not have a corresponding argument
             */
            CORE_EXPLICIT MissingFormatArgumentException(String const& s);

            /**
             * Returns the unmatched format specifier.
             *
             * @return  The unmatched format specifier
             */
            String getFormatSpecifier() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //MISSINGFORMATARGUMENTEXCEPTION_H
