//
// Created by brunshweeck on 7 sept. 2024.
//

#include <meta/charset/ArrayDecoder.h>

namespace core {
    namespace charset {
        gint ArrayDecoder::decode(ByteArray const& src, gint sp, gint len, CharArray& dst) { return 0; }

        gbool ArrayDecoder::isASCIICompatible() const { return false; }

        gbool ArrayDecoder::isLatin1Decodable() const { return false; }

        gint ArrayDecoder::decodeToLatin1(ByteArray const &src, gint sp, gint len, ByteArray &dst) { return 0; }
    } // charset
} // core
