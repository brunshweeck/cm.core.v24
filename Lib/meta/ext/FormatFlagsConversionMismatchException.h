//
// Created by admin on 03/12/24.
//

#ifndef FORMATFLAGSCONVERSIONMISMATCHEXCEPTION_H
#define FORMATFLAGSCONVERSIONMISMATCHEXCEPTION_H
#include "core/util/IllegalFormatException.h"

namespace core {
    using namespace util;
    namespace ext {

        /**
         * Unchecked exception thrown when a conversion and flag are incompatible.
         *
         * <p> Unless otherwise specified, passing a {@code null} argument to any
         * method or constructor in this class will cause a {@link
         * NullPointerException} to be thrown.
         *
         * @since 1.5
         */
        class FormatFlagsConversionMismatchException final: public IllegalFormatException {
            String flags;
            gchar c;

        public:

            /**
             * Constructs an instance of this class with the specified flag
             * and conversion.
             *
             * @param  f
             *         The flag
             *
             * @param  c
             *         The conversion
             */
            FormatFlagsConversionMismatchException(String const& f, gchar c);

            /**
             * Returns the incompatible flag.
             *
             * @return  The flag
             */
            String getFlags() const;

            /**
             * Returns the incompatible conversion.
             *
             * @return  The conversion
             */
            gchar getConversion() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //FORMATFLAGSCONVERSIONMISMATCHEXCEPTION_H
