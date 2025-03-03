//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/StringUTF16.h>

namespace core {
    namespace charset {
        gchar StringUTF16::getChar(ByteArray const &val, gint index) {
            return UNSAFE::getChar(val,
                                   UNSAFE::ARRAY_BYTE_BASE_OFFSET + UNSAFE::ARRAY_BYTE_INDEX_SCALE * index * 2LL);
        }
    } // charset
}     // core
