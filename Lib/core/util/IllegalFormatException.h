//
// Created by admin on 03/12/24.
//

#ifndef ILLEGALFORMATEXCEPTION_H
#define ILLEGALFORMATEXCEPTION_H
#include "core/IllegalArgumentException.h"

namespace core {
    namespace util {
        class IllegalFormatException: public IllegalArgumentException {
        public:
            IllegalFormatException() = default;

            Object& clone() const override = 0;

        protected:
            void selfThrow() const override = 0;
        };
    } // util
} // core

#endif //ILLEGALFORMATEXCEPTION_H
