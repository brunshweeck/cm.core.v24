//
// Created by admin on 03/12/24.
//

#ifndef ILLEGALFORMATPRECISIONEXCEPTION_H
#define ILLEGALFORMATPRECISIONEXCEPTION_H
#include <core/util/IllegalFormatException.h>

namespace core {
    namespace ext {
        /**
         * Unchecked exception thrown when the precision is a negative value other than
         * {@code -1}, the conversion does not support a precision, or the value is
         * otherwise unsupported. If the precision is not representable by an
         * {@code int} type, then the value {@code Integer.MIN_VALUE} will be used
         * in the exception.
         */
        class IllegalFormatPrecisionException : public util::IllegalFormatException {
            gint precision;

        public:
            /**
             * Constructs an instance of this class with the specified precision.
             *
             * @param  p
             *         The precision
             */
            CORE_EXPLICIT IllegalFormatPrecisionException(gint p);

            /**
             * Returns the precision. If the precision isn't representable by an
             * {@code int}, then will return {@code Integer.MIN_VALUE}.
             *
             * @return  The precision
             */
            gint getPrecision() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //ILLEGALFORMATPRECISIONEXCEPTION_H
