//
// Created by brunshweeck on 1 sept. 2024.
//

#ifndef CORE24_CHARACTERCODINGEXCEPTION_H
#define CORE24_CHARACTERCODINGEXCEPTION_H

#include <core/io/IOException.h>

namespace core {
    namespace charset {


        /**
         * Checked exception thrown when a character encoding
         * or decoding error occurs.
         */
        class CharacterCodingException: public virtual io::IOException {
        public:
            /**
             * Constructs an instance of this class.
             */
            CORE_IMPLICIT CharacterCodingException() = default;

            Object & clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // text
} // core

#endif //CORE24_CHARACTERCODINGEXCEPTION_H
