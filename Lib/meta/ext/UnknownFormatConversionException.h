//
// Created by admin on 03/12/24.
//

#ifndef UNKNOWNFORMATCONVERSIONEXCEPTION_H
#define UNKNOWNFORMATCONVERSIONEXCEPTION_H
#include <core/util/IllegalFormatException.h>

namespace core {
    namespace ext {

        /**
         * Unchecked exception thrown when an unknown conversion is given.
         *
         * <p> Unless otherwise specified, passing a {@code null} argument to
         * any method or constructor in this class will cause a {@link
         * NullPointerException} to be thrown.
         */
        class UnknownFormatConversionException: public util::IllegalFormatException {
            String s;

        public:

            /**
             * Constructs an instance of this class with the unknown conversion.
             *
             * @param  s
             *         Unknown conversion
             */
            CORE_EXPLICIT UnknownFormatConversionException(String const& s);

            /**
             * Returns the unknown conversion.
             *
             * @return  The unknown conversion.
             */
            String getConversion() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //UNKNOWNFORMATCONVERSIONEXCEPTION_H
