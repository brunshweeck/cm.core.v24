//
// Created by admin on 03/12/24.
//

#ifndef ILLEGALFORMATWIDTHEXCEPTION_H
#define ILLEGALFORMATWIDTHEXCEPTION_H
#include "core/util/IllegalFormatException.h"

namespace core {
    namespace ext {

        /**
         * Unchecked exception thrown when the format width is a negative value other
         * than {@code -1} or is otherwise unsupported. If a given format width is not
         * representable by an {@code int} type, then the value
         * {@code Integer.MIN_VALUE} will be used in the exception.
         */
        class IllegalFormatWidthException : public util::IllegalFormatException {
            gint width;

        public:

            /**
             * Constructs an instance of this class with the specified width.
             *
             * @param  w
             *         The width
             */
            CORE_EXPLICIT IllegalFormatWidthException(gint w);

            /**
             * Returns the width. If the width is not representable by an {@code int},
             * then returns {@code Integer.MIN_VALUE}.
             *
             * @return  The width
             */
            gint getWidth() const;

            String message() const override;

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // ext
} // core

#endif //ILLEGALFORMATWIDTHEXCEPTION_H
