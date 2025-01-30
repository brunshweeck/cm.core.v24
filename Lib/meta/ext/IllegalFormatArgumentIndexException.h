//
// Created by admin on 03/12/24.
//

#ifndef ILLEGALFORMATARGUMENTINDEXEXCEPTION_H
#define ILLEGALFORMATARGUMENTINDEXEXCEPTION_H
#include "core/util/IllegalFormatException.h"

namespace core {
    namespace ext {
        /**
         * Unchecked exception thrown when the argument index is not within the valid
         * range of supported argument index values. If an index value isn't
         * representable by an {@code int} type, then the value
         * {@code Integer.MIN_VALUE} will be used in the exception.
         *
         * @since 16
         */
        class IllegalFormatArgumentIndexException : public util::IllegalFormatException {
            gint index;

        public:
            CORE_EXPLICIT IllegalFormatArgumentIndexException(gint index);

            gint getIndex() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //ILLEGALFORMATARGUMENTINDEXEXCEPTION_H
