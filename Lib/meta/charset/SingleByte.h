//
// Created by brunshweeck on 6 sept. 2024.
//

#ifndef CORE24_SINGLEBYTE_H
#define CORE24_SINGLEBYTE_H

#include <meta/charset/ArrayDecoder.h>
#include <meta/charset/ArrayEncoder.h>
#include <meta/charset/Surrogate.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CoderResult.h>

namespace core {
    namespace charset {
        class SingleByte final : public Object {
        public:
            static CORE_FAST gchar UNMAPPABLE_DECODING = u'\uFFFD';
            static CORE_FAST gint  UNMAPPABLE_ENCODING = 0xFFFD;

            class Decoder : public virtual CharsetDecoder, public virtual ArrayDecoder {
                CharArray &b2c;
                gbool      asciiCompatible;
                gbool      latin1Decodable;

                gchar repl = u'\uFFFD';

            public:
                CORE_EXPLICIT Decoder(Charset const &cs, CharArray &b2c);

                CORE_EXPLICIT Decoder(Charset const &cs, CharArray &b2c,
                                      gbool          isASCIICompatible);

                CORE_EXPLICIT Decoder(Charset const &cs, CharArray &b2c,
                                      gbool          isASCIICompatible,
                                      gbool          isLatin1Decodable);

                gchar decode(gint b) const;

                gint decodeToLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) override;

                gint decode(const ByteArray &src, gint sp, gint len, CharArray &dst) override;

                gbool isASCIICompatible() const override;

                gbool isLatin1Decodable() const override;

            protected:
                CoderResult decode0(io::ByteBuffer &in, io::CharBuffer &out) override;

                void replaceWith0(const String &newReplacement) override;

            private:
                CoderResult decodeArray(io::ByteBuffer &src, io::CharBuffer &dst);

                CoderResult decodeBuffer(io::ByteBuffer &src, io::CharBuffer &dst);
            };

            class Encoder : public virtual CharsetEncoder, public virtual ArrayEncoder {
                Surrogate::Parser sgp = {};
                CharArray &       c2b;
                CharArray &       c2bIndex;
                gbool             asciiCompatible;

                gbyte repl = CORE_CAST(gbyte, '?');

            public:
                CORE_EXPLICIT Encoder(Charset const &cs, CharArray &c2b, CharArray &c2bIndex, gbool isASCIICompatible);

                gbool canEncode(gchar c) override;

                gbool isLegalReplacement(const ByteArray &repl) const override;

                gint encode(const CharArray &src, gint sp, gint len, ByteArray &dst) override;

                gint encode(gchar ch) const;

                gint encodeFromLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) override;

                gint encodeFromUTF16(const ByteArray &src, gint sp, gint len, ByteArray &dst) override;

                gbool isASCIICompatible() const override;

            protected:
                CoderResult encode0(io::CharBuffer &in, io::ByteBuffer &out) override;

                void replaceWith0(ByteArray const &newReplacement) override;

            private:
                CoderResult encodeArray(io::CharBuffer &src, io::ByteBuffer &dst);

                CoderResult encodeBuffer(io::CharBuffer &src, io::ByteBuffer &dst);
            };

            // init the c2b and c2bIndex tables from b2c.
            static void initC2B(CharArray &b2c, CharArray &c2bNR,
                                CharArray &c2b, CharArray &c2bIndex);

        private:
            static CoderResult withResult(CoderResult cr,
                                          io::Buffer &src, gint sp,
                                          io::Buffer &dst, gint dp);
        };
    } // charset
}     // core

#endif //CORE24_SINGLEBYTE_H
