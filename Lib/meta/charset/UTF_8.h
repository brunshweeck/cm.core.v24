//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_UTF_8_H
#define CORE24_UTF_8_H
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <meta/charset/Surrogate.h>
#include <meta/charset/Unicode.h>

namespace core {
    namespace charset {
        /* Legal UTF-8 Byte Sequences

           #    Code Points      Bits   Bit/Byte pattern
           1                     7      0xxxxxxx
                U+0000..U+007F          00..7F

           2                     11     110xxxxx    10xxxxxx
                U+0080..U+07FF          C2..DF      80..BF

           3                     16     1110xxxx    10xxxxxx    10xxxxxx
                U+0800..U+0FFF          E0          A0..BF      80..BF
                U+1000..U+FFFF          E1..EF      80..BF      80..BF

           4                     21     11110xxx    10xxxxxx    10xxxxxx    10xxxxxx
               U+10000..U+3FFFF         F0          90..BF      80..BF      80..BF
               U+40000..U+FFFFF         F1..F3      80..BF      80..BF      80..BF
              U+100000..U10FFFF         F4          80..8F      80..BF      80..BF

         */
        class UTF_8 final : public Unicode {
        public:

            CORE_IMPLICIT UTF_8();

            CharsetDecoder& newDecoder() const override;

            CharsetEncoder& newEncoder() const override;

            static void updatePositions(io::Buffer& src, int sp,
                                        io::Buffer& dst, int dp);

        private:
            class Decoder : public CharsetDecoder {
            public:
                CORE_EXPLICIT Decoder(Charset const& cs);

            protected:
                CoderResult decode0(io::ByteBuffer& in, io::CharBuffer& out) override;

            private:
                static gbool isNotContinuation(gint b);

                //  [E0]     [A0..BF] [80..BF]
                //  [E1..EF] [80..BF] [80..BF]
                static gbool isMalformed3(gint b1, gint b2, gint b3);

                // only used when there is only one byte left in src buffer
                static gbool isMalformed3_2(gint b1, gint b2);

                //  [F0]     [90..BF] [80..BF] [80..BF]
                //  [F1..F3] [80..BF] [80..BF] [80..BF]
                //  [F4]     [80..8F] [80..BF] [80..BF]
                //  only check 80-be range here, the [0xf0,0x80...] and [0xf4,0x90-...]
                //  will be checked by Character.isSupplementaryCodePoint(uc)
                static gbool isMalformed4(gint b2, gint b3, gint b4);

                // only used when there is less than 4 bytes left in src buffer.
                // both b1 and b2 should be "& 0xff" before passed in.
                static gbool isMalformed4_2(gint b1, gint b2);

                // tests if b1 and b2 are malformed as the first 2 bytes of a
                // legal`4-byte utf-8 byte sequence.
                // only used when there is less than 4 bytes left in src buffer,
                // after isMalformed4_2 has been invoked.
                static gbool isMalformed4_3(gint b3);

                static CoderResult malformedN(io::ByteBuffer& src, gint nb);

                static CoderResult malformed(io::ByteBuffer& src, gint sp, io::CharBuffer& dst, gint dp, gint nb);

                static CoderResult malformed(io::ByteBuffer& src, gint mark, gint nb);

                static CoderResult malformedForLength(io::ByteBuffer& src, gint sp,
                                                      io::CharBuffer& dst, gint dp, gint malformedNB);

                static CoderResult malformedForLength(io::ByteBuffer& src, gint mark, gint malformedNB);

                static CoderResult xflow(io::Buffer& src, gint sp, gint sl, io::Buffer& dst, gint dp, gint nb);

                static CoderResult xflow(io::Buffer& src, gint mark, gint nb);

                virtual CoderResult decodeArray(io::ByteBuffer& src, io::CharBuffer& dst);

                virtual CoderResult decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst);
            };

            class Encoder : public CharsetEncoder {
                Surrogate::Parser sgp = {};

            public:
                CORE_EXPLICIT Encoder(Charset const& cs);

                gbool canEncode(gchar c) override;

                gbool isLegalReplacement(const ByteArray& repl) const override;

            protected:
                CoderResult encode0(io::CharBuffer& src, io::ByteBuffer& dst) override;

            private:
                static CoderResult overflow(io::CharBuffer& src, gint sp, io::ByteBuffer& dst, gint dp);

                static CoderResult overflow(io::CharBuffer& src, gint mark);

                CoderResult encodeArray(io::CharBuffer& src, io::ByteBuffer& dst);

                CoderResult encodeArraySlow(io::CharBuffer& src, CharArray& sa, gint sp, gint sl,
                                            io::ByteBuffer& dst, ByteArray& da, gint dp, gint dl);

                CoderResult encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst);
            };
        };
    } // charset
} // core

#endif //CORE24_UTF_8_H
