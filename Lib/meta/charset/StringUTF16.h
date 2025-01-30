//
// Created by brunshweeck on 8 sept. 2024.
//

#ifndef CORE24_STRINGUTF16_H
#define CORE24_STRINGUTF16_H

#include <core/Object.h>

namespace core {
    namespace charset {
        class StringUTF16 final : public Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

        public:
            static gchar getChar(ByteArray const &val, gint index);
        };
    } // charset
}     // core

#endif //CORE24_STRINGUTF16_H
