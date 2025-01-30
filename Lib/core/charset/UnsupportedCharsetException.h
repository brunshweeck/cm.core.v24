//
// Created by brunshweeck on 2 sept. 2024.
//

#ifndef CORE24_UNSUPPORTEDCHARSETEXCEPTION_H
#define CORE24_UNSUPPORTEDCHARSETEXCEPTION_H

#include <core/IllegalArgumentException.h>

namespace core {
    namespace charset {
        /**
         * Unchecked exception thrown when no support is available
         * for a requested charset.
         */
        class UnsupportedCharsetException : public virtual IllegalArgumentException {
            /**
             * The name of the unsupported charset.
             */
            String charset;

        public:
            /**
             * Constructs an instance of this class.
             *
             * @param  charsetName
             *         The name of the unsupported charset
             */
            CORE_EXPLICIT UnsupportedCharsetException(const String& charsetName);

            /**
             * Retrieves the name of the unsupported charset.
             *
             * @return  The name of the unsupported charset
             */
            String charsetName() const;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // charset
} // core

#endif //CORE24_UNSUPPORTEDCHARSETEXCEPTION_H
