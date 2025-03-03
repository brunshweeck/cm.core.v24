//
// Created by admin on 03/12/24.
//

#ifndef UNKNOWNFORMATFLAGSEXCEPTION_H
#define UNKNOWNFORMATFLAGSEXCEPTION_H
#include <core/util/IllegalFormatException.h>

namespace core {
    namespace ext {
        /**
         * Unchecked exception thrown when an unknown flag is given.
         *
         * <p> Unless otherwise specified, passing a {@code null} argument to any
         * method or constructor in this class will cause a {@link
         * NullPointerException} to be thrown.
         */
        class UnknownFormatFlagsException: public util::IllegalFormatException {
            String flags;

        public:
            CORE_EXPLICIT UnknownFormatFlagsException(String const &flags);

            String const &getFlags();

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //UNKNOWNFORMATFLAGSEXCEPTION_H
