//
// Created by brunshweeck on 8 sept. 2024.
//

#ifndef CORE24_HKSCS_H
#define CORE24_HKSCS_H

#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        class HKSCS final : public Object {
        public:
            static CORE_FAST gchar UNMAPPABLE_DECODING = u'\uFFFD';
            static CORE_FAST gint UNMAPPABLE_ENCODING = 0xFFFD;

            class Decoder : public virtual DoubleByte::Decoder {
                CharArray2D& b2cBmp;
                CharArray2D& b2cSupp;
                DoubleByte::Decoder& big5Dec;

            protected:
                CharArray2D b2c = {};
                CharArray b2cSB = {};

                static gint CORE_FAST b2Min = 0x40;
                static gint CORE_FAST b2Max = 0x40;

            public:
                // CORE_EXPLICIT Decoder();

                CORE_EXPLICIT Decoder(Charset const& cs,
                                      DoubleByte::Decoder& big5Dec,
                                      CharArray2D& b2cBmp,
                                      CharArray2D& b2cSupp);

                gchar decodeSingle(gint b) const override;

                gchar decodeBig5(gint b1, gint b2) const;

                gchar decodeDouble(gint b1, gint b2) const override;

                virtual gchar decodeDoubleEx(gint b1, gint b2) const;

                gint decode(const ByteArray& src, gint sp, gint len, CharArray& dst) override;

                CoderResult decode0(io::ByteBuffer& src, io::CharBuffer& dst) override;

                static void initB2C(CharArray2D& b2c, StringArray& b2cStr);

            protected:
                CoderResult decodeArray(io::ByteBuffer& src, io::CharBuffer& dst) override;
                CoderResult decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst) override;
            };

            class Encoder : public virtual DoubleByte::Encoder {
                DoubleByte::Encoder& big5Enc;
                CharArray2D& c2bBmp;
                CharArray2D& c2bSupp;

                ByteArray repl = replacement();

                // ignored fields;
            public:
                CharArray c2b;
                CharArray c2bIndex;

                CORE_EXPLICIT Encoder(Charset const& cs,
                                      DoubleByte::Encoder& big5Enc,
                                      CharArray2D& c2bBmp,
                                      CharArray2D& c2bSupp);

                gint encodeBig5(gchar ch) const;

                gint encodeChar(gchar ch) override;

                virtual gint encodeSupp(gint cp) const;

                gbool canEncode(gchar c) override;

                gint encode(const CharArray& src, gint sp, gint len, ByteArray& dst) override;

                gint encodeFromUTF16(const ByteArray& src, gint sp, gint len, ByteArray& dst) override;

                static CharArray C2B_UNMAPPABLE;

                static void initC2B(CharArray2D &c2b, StringArray &b2cStr, String &pua);

            protected:
                CoderResult encodeArray(io::CharBuffer& src, io::ByteBuffer& dst) override;

                CoderResult encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst) override;

                CoderResult encode0(io::CharBuffer& src, io::ByteBuffer& dst) override;

                void replaceWith0(const ByteArray& newReplacement) override;
            };
        };
    } // charset
} // core

#endif //CORE24_HKSCS_H
