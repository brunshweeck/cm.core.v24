//
// Created by brunshweeck on 7 sept. 2024.
//

#ifndef CORE24_ARRAYENCODER_H
#define CORE24_ARRAYENCODER_H

#include <core/ByteArray.h>
#include <core/CharArray.h>

namespace core {
    namespace charset {
        /*
         * FastPath CharArray/ByteArray -> ByteArray encoder, REPLACE on malformed input or
         * unmappable input.
         */
        class ArrayEncoder : public virtual Object {
        public:
            //  is only used by ZipCoder for utf-8
            virtual gint encode(CharArray const &src, gint off, gint len, ByteArray &dst) = 0;

            virtual gint encodeFromLatin1(ByteArray const &src, gint sp, gint len, ByteArray &dst);

            virtual gint encodeFromUTF16(ByteArray const &src, gint sp, gint len, ByteArray &dst);

            virtual gbool isASCIICompatible() const;
        };
    } // charset
} // core

#endif //CORE24_ARRAYENCODER_H
