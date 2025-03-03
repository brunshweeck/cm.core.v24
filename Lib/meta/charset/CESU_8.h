//
// Created by brunshweeck on 6 sept. 2024.
//

#ifndef CORE24_CESU_8_H
#define CORE24_CESU_8_H

#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <meta/charset/ArrayDecoder.h>
#include <meta/charset/ArrayEncoder.h>
#include <meta/charset/Surrogate.h>
#include <meta/charset/Unicode.h>

namespace core {
    namespace charset {
        //
        // Legal CESU-8 Byte Sequences
        //
        // #    Code Points      Bits   Bit/Byte pattern
        // 1                     7      0xxxxxxx
        //      U+0000..U+007F          00..7F
        //
        // 2                     11     110xxxxx    10xxxxxx
        //      U+0080..U+07FF          C2..DF      80..BF
        //
        // 3                     16     1110xxxx    10xxxxxx    10xxxxxx
        //      U+0800..U+0FFF          E0          A0..BF      80..BF
        //      U+1000..U+FFFF          E1..EF      80..BF      80..BF
        //
        //
        class CESU_8 final : public Unicode {
        public:
            CORE_IMPLICIT CESU_8();

            CharsetDecoder &newDecoder() const override;

            CharsetEncoder &newEncoder() const override;

        private:
            static void updatePositions(io::Buffer &src, gint sp, io::Buffer &dst, gint dp);

            class Decoder : public virtual CharsetDecoder, public virtual ArrayDecoder {
            public:
                CORE_EXPLICIT Decoder(Charset const &cs);

                // returns -1 if there is/are malformed byte(s) and the
                // "action" for malformed input is not REPLACE.
                gint decode(ByteArray const &sa, gint sp, gint len, CharArray &da) override;


            protected:
                CoderResult decode0(io::ByteBuffer &in, io::CharBuffer &out) override;

            private:
                static gbool isNotContinuation(gint b);

                //  [E0]     [A0..BF] [80..BF]
                //  [E1..EF] [80..BF] [80..BF]
                static gbool isMalformed3(gint b1, gint b2, gint b3);

                // only used when there is only one byte left in src buffer
                static gbool isMalformed3_2(gint b1, gint b2);

                static CoderResult malformedN(io::ByteBuffer &src, gint nb);

                static CoderResult malformed(io::ByteBuffer &src, gint sp, io::CharBuffer &dst, gint dp, gint nb);

                static CoderResult malformed(io::ByteBuffer &src, gint mark, gint nb);

                static CoderResult malformedForLength(io::ByteBuffer &src, gint sp,
                                                      io::CharBuffer &dst, gint dp, gint malformedNB);

                static CoderResult malformedForLength(io::ByteBuffer &src, gint mark, gint malformedNB);

                static CoderResult xflow(io::Buffer &src, gint sp, gint sl, io::Buffer &dst, gint dp, gint nb);

                static CoderResult xflow(io::Buffer &src, gint mark, gint nb);

                virtual CoderResult decodeArray(io::ByteBuffer &src, io::CharBuffer &dst);

                virtual CoderResult decodeBuffer(io::ByteBuffer &src, io::CharBuffer &dst);
            };

            class Encoder : public virtual CharsetEncoder, public virtual ArrayEncoder {

                Surrogate::Parser sgp = {};

            public:
                CORE_EXPLICIT Encoder(Charset const &cs);

                gbool canEncode(gchar c) override;

                gbool isLegalReplacement(const ByteArray &repl) const override;

                // returns -1 if there is malformed char(s) and the
                // "action" for malformed input is not REPLACE.
                gint encode(CharArray const &sa, gint sp, gint len, ByteArray& da) override;

            protected:
                CoderResult encode0(io::CharBuffer &in, io::ByteBuffer &out) override;

            private:
                static CoderResult overflow(io::CharBuffer &src, gint sp, io::ByteBuffer &dst, gint dp);

                static CoderResult overflow(io::CharBuffer &src, gint mark);

                static void to3Bytes(ByteArray &da, gint dp, gchar c);

                static void to3Bytes(io::ByteBuffer &dst, gchar c);

                CoderResult encodeArray(io::CharBuffer &src, io::ByteBuffer &dst);

                CoderResult encodeBuffer(io::CharBuffer &src, io::ByteBuffer &dst);
            };
        };
    } // charset
} // core

#endif //CORE24_CESU_8_H
