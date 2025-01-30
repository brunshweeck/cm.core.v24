//
// Created by brunshweeck on 6 sept. 2024.
//

#ifndef CORE24_US_ASCII_H
#define CORE24_US_ASCII_H

#include <meta/charset/Surrogate.h>
#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>

namespace core {
    namespace charset {
        class US_ASCII final : public Charset {
        public:

            CORE_IMPLICIT US_ASCII();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &newDecoder() const override;

            CharsetEncoder &newEncoder() const override;

            /**
             * Decodes ASCII from the source byte array into the destination
             * char array. Used from UTF_8 and other charset decoders.
             *
             * @return the number of bytes successfully decoded, at most len
             */
            static gint decodeASCII(ByteArray const &src, gint srcOff, CharArray &dst, gint dstOff, gint len);

            static gint encodeASCII(CharArray const &src, gint srcOff, ByteArray &dst, gint dstOff, gint len);

            static void inflateBytesToChars(ByteArray const &src, gint srcOff, CharArray &dst, gint dstOff, gint len);

            static void inflateCharsToBytes(CharArray const&src, gint srcOff, ByteArray &dst, gint dstOff, gint len);

        private:
            class Decoder : public CharsetDecoder {
            public:
                CORE_EXPLICIT Decoder(Charset const &cs);

            protected:
                CoderResult decode0(io::ByteBuffer &in, io::CharBuffer &out) override;

            private:
                virtual CoderResult decodeArray(io::ByteBuffer &src, io::CharBuffer &dst);

                virtual CoderResult decodeBuffer(io::ByteBuffer &src, io::CharBuffer &dst);
            };

            class Encoder final : public CharsetEncoder {

                Surrogate::Parser sgp = {};

            public:
                CORE_EXPLICIT Encoder(Charset const &cs);

                gbool isLegalReplacement(const ByteArray &repl) const override;

                gbool canEncode(gchar c) override;

            protected:
                CoderResult encode0(io::CharBuffer &in, io::ByteBuffer &out) override;

            private:
                CoderResult encodeArray(io::CharBuffer &src, io::ByteBuffer &dst);

                CoderResult encodeBuffer(io::CharBuffer &src, io::ByteBuffer &dst);
            };
        };
    } // charset
} // core

#endif //CORE24_US_ASCII_H
