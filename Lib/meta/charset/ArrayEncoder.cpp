//
// Created by brunshweeck on 7 sept. 2024.
//

#include <meta/charset/ArrayEncoder.h>

namespace core {
    namespace charset {
        gint ArrayEncoder::encode(CharArray const& src, gint off, gint len, ByteArray& dst) { return 0; }

        gint ArrayEncoder::encodeFromLatin1(ByteArray const &src, gint sp, gint len, ByteArray &dst) { return -1; }

        gint ArrayEncoder::encodeFromUTF16(ByteArray const &src, gint sp, gint len, ByteArray &dst) { return -1; }

        gbool ArrayEncoder::isASCIICompatible() const { return false; }
    } // charset
} // core
