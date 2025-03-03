//
// Created by brunshweeck on 7 sept. 2024.
//

#ifndef CORE24_ARRAYDECODER_H
#define CORE24_ARRAYDECODER_H

#include <core/lang/ByteArray.h>
#include <core/lang/CharArray.h>

namespace core {
    namespace charset {
        /*
         * FastPath ByteArray -> CharArray decoder, REPLACE on malformed or
         * unmappable input.
         *
         * FastPath encoded ByteArray -> "String Latin1 coding" ByteArray decoder for use when
         * charset is always decodable to the internal String Latin1 coding ByteArray, i.e. all mappings <=0xff
         */
        class ArrayDecoder : public virtual Object {
        public:
            virtual gint decode(ByteArray const &src, gint sp, gint len, CharArray &dst) = 0;

            virtual gbool isASCIICompatible() const;

            // Is always decodable to internal String Latin1 coding, i.e. all mappings <= 0xff
            virtual gbool isLatin1Decodable() const;

            // Decode to internal String Latin1 coding ByteArray fast-path for when isLatin1Decodable == true
            virtual gint decodeToLatin1(ByteArray const &src, gint sp, gint len, ByteArray &dst);
        };
    } // charset
} // core

#endif //CORE24_ARRAYDECODER_H
