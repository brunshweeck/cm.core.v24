//
// Created by admin on 03/12/24.
//

#ifndef ILLEGALFORMATCONVERSIONEXCEPTION_H
#define ILLEGALFORMATCONVERSIONEXCEPTION_H
#include <core/util/IllegalFormatException.h>

namespace core {
    namespace ext {

        /**
         * Unchecked exception thrown when the argument corresponding to the format
         * specifier is of an incompatible type.
         *
         * <p> Unless otherwise specified, passing a {@code null} argument to any
         * method or constructor in this class will cause a {@link
         * NullPointerException} to be thrown.
         */
        class IllegalFormatConversionException: public util::IllegalFormatException {
            gchar c;
            String arg;

        public:

            /**
             * Constructs an instance of this class with the mismatched conversion and
             * the corresponding argument class.
             *
             * @param  c
             *         Inapplicable conversion
             *
             * @param  arg
             *         instance of the mismatched argument
             */
            IllegalFormatConversionException(gchar c, Object const& arg);

            /**
             * Returns the inapplicable conversion.
             *
             * @return  The inapplicable conversion
             */
            gchar getConversion() const;

            /**
             * Returns the class of the mismatched argument.
             *
             * @return   The class of the mismatched argument
             */
            String getArgumentClass() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //ILLEGALFORMATCONVERSIONEXCEPTION_H
