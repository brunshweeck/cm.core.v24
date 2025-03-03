//
// Created by admin on 03/12/24.
//

#ifndef DUPLICATEFORMATFLAGSEXCEPTION_H
#define DUPLICATEFORMATFLAGSEXCEPTION_H

#include <core/util/IllegalFormatException.h>

namespace core {
    using namespace util;

    namespace ext {

        /**
         * Unchecked exception thrown when duplicate flags are provided in the format
         * specifier.
         *
         * <p> Unless otherwise specified, passing a {@code null} argument to any
         * method or constructor in this class will cause a {@link
         * NullPointerException} to be thrown.
         *
         * @since 1.5
         */
        class DuplicateFormatFlagsException final : public IllegalFormatException {
            String flags;

        public:
            /**
             * Constructs an instance of this class with the specified flags.
             *
             * @param  f
             *         The set of format flags which contain a duplicate flag.
             */
            CORE_EXPLICIT DuplicateFormatFlagsException(String const& f);

            /**
             * Returns the set of flags which contains a duplicate flag.
             *
             * @return  The flags
             */
            String getFlags() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //DUPLICATEFORMATFLAGSEXCEPTION_H
