//
// Created by admin on 03/12/24.
//

#ifndef ILLEGALFORMATFLAGSEXCEPTION_H
#define ILLEGALFORMATFLAGSEXCEPTION_H
#include <core/util/IllegalFormatException.h>

namespace core {
    namespace ext {
        class IllegalFormatFlagsException : public util::IllegalFormatException {
            String flags;

        public:

            /**
             * Constructs an instance of this class with the specified flags.
             *
             * @param  f
             *         The set of format flags which contain an illegal combination
             */
            CORE_EXPLICIT IllegalFormatFlagsException(String const& f);

            /**
             * Returns the set of flags which contains an illegal combination.
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

#endif //ILLEGALFORMATFLAGSEXCEPTION_H
