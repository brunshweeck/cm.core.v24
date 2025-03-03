//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_ISO_8859_1_H
#define CORE24_ISO_8859_1_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <meta/charset/Surrogate.h>

namespace core {
    namespace charset {
        class ISO_8859_1 final : public Charset {
        public:

            CORE_IMPLICIT ISO_8859_1();

            gbool contains(const Charset& cs) const override;

            CharsetDecoder& newDecoder() const override;

            CharsetEncoder& newEncoder() const override;

        private:
            class Decoder : public CharsetDecoder {
            public:
                CORE_EXPLICIT Decoder(Charset const& cs);

            protected:
                CoderResult decode0(io::ByteBuffer& src, io::CharBuffer& dst) override;

            private:
                CoderResult decodeArray(io::ByteBuffer& src, io::CharBuffer& dst);
                CoderResult decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst);
            };

            class Encoder : public CharsetEncoder {
                Surrogate::Parser sgp = {};

            public:
                CORE_EXPLICIT Encoder(Charset const& cs);

                gbool isLegalReplacement(const ByteArray& repl) const override;

                gbool canEncode(gchar c) override;

            protected:
                CoderResult encode0(io::CharBuffer& src, io::ByteBuffer& dst) override;

            private:
                CoderResult encodeArray(io::CharBuffer& src, io::ByteBuffer& dst);

                CoderResult encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst);

                static gint encodeISOArray(CharArray const& sa, gint sp, ByteArray& da, gint dp, gint len);

                static gint encodeISOArray0(CharArray const& sa, gint sp, ByteArray& da, gint dp, gint len);

                static void encodeISOArrayCheck(CharArray const& sa, gint sp, ByteArray& da, gint dp, gint len);
            };
        };
    } // charset
} // core

#endif //CORE24_ISO_8859_1_H
