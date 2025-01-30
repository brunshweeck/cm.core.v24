//
// Created by brunshweeck on 6 sept. 2024.
//

#ifndef CORE24_DOUBLEBYTE_H
#define CORE24_DOUBLEBYTE_H

#include <core/CharArray2D.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <meta/charset/ArrayDecoder.h>
#include <meta/charset/ArrayEncoder.h>
#include <meta/charset/DelegatableDecoder.h>
#include <meta/charset/Surrogate.h>

namespace core {
    namespace charset {

        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        /*
         * Four types of "DoubleByte" charsets are implemented in this class
         *
         * (1)DoubleByte
         *    The "mostly widely used" multibyte charset, a combination of
         *    a singlebyte character set (usually the ASCII charset) and a
         *    doublebyte character set. The codepoint values of singlebyte
         *    and doublebyte don't overlap. Microsoft's multibyte charsets
         *    and IBM's "DBCS_ASCII" charsets, such as IBM1381, 942, 943,
         *    948, 949 and 950 are such charsets.
         *
         * (2)DoubleByte_EBCDIC
         *    IBM EBCDIC Mix multibyte charset. Use SO and SI to shift (switch)
         *    in and out between the singlebyte character set and doublebyte
         *    character set.
         *
         * (3)DoubleByte_SIMPLE_EUC
         *    It's a "simple" form of EUC encoding scheme, only have the
         *    singlebyte character set G0 and one doublebyte character set
         *    G1 are defined, G2 (with SS2) and G3 (with SS3) are not used.
         *    So it is actually the same as the "typical" type (1) mentioned
         *    above, except it return "malformed" for the SS2 and SS3 when
         *    decoding.
         *
         * (4)DoubleByte ONLY
         *    A "pure" doublebyte only character set. From implementation
         *    point of view, this is the type (1) with "decodeSingle" always
         *    returns unmappable.
         *
         * For simplicity, all implementations share the same decoding and
         * encoding data structure.
         *
         * Decoding:
         *
         *    CharArray2D b2c;
         *    CharArray b2cSB;
         *    gint b2Min, b2Max
         *
         *    public gchar decodeSingle(int b) {
         *        return b2cSB[b];
         *    }
         *
         *    public char decodeDouble(int b1, int b2) {
         *        if (b2 < b2Min || b2 > b2Max)
         *            return UNMAPPABLE_DECODING;
         *         return b2c[b1][b2 - b2Min];
         *    }
         *
         *    (1)b2Min, b2Max are the corresponding min and max value of the
         *       low-half of the double-byte.
         *    (2)The high 8-bit/b1 of the double-byte are used to indexed into
         *       b2c array.
         *
         * Encoding:
         *
         *    CharArray c2b;
         *    CharArray c2bIndex;
         *
         *    public int encodeChar(char ch) {
         *        return c2b[c2bIndex[ch >> 8] + (ch & 0xff)];
         *    }
         *
         */
        class DoubleByte final : public Object {
            static CORE_FAST gchar UNMAPPABLE_DECODING = u'\uFFFD';
            static CORE_FAST gint UNMAPPABLE_ENCODING = 0xFFFD;

        public:
            static CharArray B2C_UNMAPPABLE;

            class Decoder : public virtual CharsetDecoder,
                            public virtual DelegatableDecoder, public virtual ArrayDecoder {
            protected:
                CharArray2D& b2c;
                CharArray& b2cSB;
                gint b2Min;
                gint b2Max;
                gbool asciiCompatible;

            public:

                CORE_EXPLICIT Decoder(Charset const& cs, gfloat averageCharsPerByte, gfloat maxCharsPerByte,
                                      CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max,
                                      gbool isAsciiCompatible);

                CORE_EXPLICIT Decoder(Charset const& cs, CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max,
                                      gbool isAsciiCompatible);

                CORE_EXPLICIT Decoder(Charset const& cs, CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max);

                CoderResult flush0(io::CharBuffer& out) override;

                void reset0() override;

                CoderResult decode0(io::ByteBuffer& src, io::CharBuffer& dst) override;

                gint decode(ByteArray const& src, gint sp, gint len, CharArray& dst) override;

                gbool isASCIICompatible() const override;

                // decode loops are not using decodeSingle/Double() for performance
                // reason.
                virtual gchar decodeSingle(gint b) const;

                virtual gchar decodeDouble(gint b1, gint b2) const;

            protected:
                // for SimpleEUC override
                virtual CoderResult crMalformedOrUnderFlow(gint b);
                virtual CoderResult crMalformedOrUnmappable(gint b1, gint b2);

                virtual CoderResult decodeArray(io::ByteBuffer& src, io::CharBuffer& dst);
                virtual CoderResult decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst);
            };

             // IBM_EBCDIC_DBCS
             class DecoderEBCDIC : public virtual Decoder {
                 static CORE_FAST gint SBCS = 0;
                 static CORE_FAST gint DBCS = 1;
                 static CORE_FAST gint SO   = 0x0E;
                 static CORE_FAST gint SI   = 0x0F;

                 gint state = -1;

             public:
                 CORE_EXPLICIT DecoderEBCDIC(Charset const &cs,
                                             CharArray2D &b2c, CharArray &b2cSB, gint b2Min, gint b2Max,
                                             gbool isAsciiCompatible);

                 CORE_EXPLICIT DecoderEBCDIC(Charset const &cs,
                                             CharArray2D &b2c, CharArray &b2cSB, gint b2Min, gint b2Max);

                 void reset0() override;

                 gint decode(ByteArray const &src, gint sp, gint len, CharArray &dst) override;

             protected:
                 CoderResult decodeArray(io::ByteBuffer &src, io::CharBuffer &dst) override;
                 CoderResult decodeBuffer(io::ByteBuffer &src, io::CharBuffer &dst) override;

             private:
                 // Check validity of dbcs ebcdic byte pair values
                 //
                 // First byte : 0x41 -- 0xFE
                 // Second byte: 0x41 -- 0xFE
                 // Doublebyte blank: 0x4040
                 //
                 // The validation implementation in "old" DBCS_IBM_EBCDIC and sun.io
                 // as
                 //            if ((b1 != 0x40 || b2 != 0x40) &&
                 //                (b2 < 0x41 || b2 > 0xfe)) {...}
                 // is not correct/complete (range check for b1)
                 //
                 static gbool isDoubleByte(gint b1, gint b2);
             };

             // DBCS_ONLY
             class DecoderDBCS_ONLY : public virtual Decoder {
             public:
                 static CharArray B2CSB_UNMAPPABLE;

                 CORE_EXPLICIT DecoderDBCS_ONLY(Charset const &cs,
                                                CharArray2D &b2c, CharArray &b2cSB, gint b2Min, gint b2Max,
                                                gbool isAsciiCompatible);

                 CORE_EXPLICIT DecoderDBCS_ONLY(Charset const &cs,
                                                CharArray2D &b2c, CharArray &b2cSB, gint b2Min, gint b2Max);

             protected:
                 // always returns unmappableForLenth(2) for doublebyte_only
                 CoderResult crMalformedOrUnmappable(gint b1, gint b2) override;
             };

             // EUC_SIMPLE
             // The only thing we need to "override" is to check SS2/SS3 and
             // return "malformed" if found
             class DecoderEUC_SIM : public virtual Decoder {
                 gint SS2 = 0x8E;
                 gint SS3 = 0x8F;

             public:
                 CORE_EXPLICIT DecoderEUC_SIM(Charset const &cs,
                                              CharArray2D &b2c, CharArray &b2cSB, gint b2Min, gint b2Max,
                                              gbool isAsciiCompatible);

                 CORE_EXPLICIT DecoderEUC_SIM(Charset const &cs,
                                              CharArray2D &b2c, CharArray &b2cSB, gint b2Min, gint b2Max);

                 gint decode(ByteArray const &src, gint sp, gint len, CharArray &dst) override;

             protected:
                 CoderResult crMalformedOrUnderFlow(gint b) override;

                 CoderResult crMalformedOrUnmappable(gint b1, gint b2) override;
             };

            class Encoder : public virtual CharsetEncoder, public virtual ArrayEncoder {
            protected:
                CharArray& c2b;
                CharArray& c2bIndex;
                Surrogate::Parser sgp = {};
                gbool asciiCompatible;

            public:
                CORE_EXPLICIT Encoder(Charset const& cs, CharArray& c2b, CharArray& c2bIndex);

                CORE_EXPLICIT Encoder(Charset const& cs, CharArray& c2b, CharArray& c2bIndex, gbool isAsciiCompatible);

                CORE_EXPLICIT Encoder(Charset const& cs,
                                      gfloat averageBytesPerChar, gfloat maxBytesPerChar, ByteArray const& repl,
                                      CharArray& c2b,
                                      CharArray& c2bIndex, gbool isAsciiCompatible);

                gbool canEncode(gchar c) override;

                gint encode(CharArray const& src, gint sp, gint len, ByteArray& dst) override;

                gint encodeFromLatin1(ByteArray const& src, gint sp, gint len, ByteArray& dst) override;

                gint encodeFromUTF16(ByteArray const& src, gint sp, gint len, ByteArray& dst) override;

                gbool isASCIICompatible() const override;

                virtual gint encodeChar(gchar ch);

                static void initC2B(StringArray const& b2c, String const& b2cSB, String const& b2cNR,
                                    String const& c2bNR, gint b2Min, gint b2Max, CharArray& c2b, CharArray& c2bIndex);

            protected:
                gint MAX_SINGLEBYTE = 0xFF;
                ByteArray repl = replacement();

                virtual CoderResult encodeArray(io::CharBuffer& src, io::ByteBuffer& dst);

                virtual CoderResult encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst);

                CoderResult encode0(io::CharBuffer& src, io::ByteBuffer& dst) override;

                void replaceWith0(ByteArray const& newReplacement) override;
            };


             class EncoderEBCDIC : public virtual Encoder {
                 gint SBCS = 0;
                 gint DBCS = 1;
                 gbyte SO  = 0x0E;
                 gbyte SI  = 0x0F;

             public:
                 CORE_EXPLICIT EncoderEBCDIC(Charset const &cs,
                                             CharArray &c2b, CharArray &c2bIndex, gbool isAsciiCompatible);

                 gint encode(CharArray const &src, gint sp, gint len, ByteArray &dst) override;

                 gint encodeFromLatin1(ByteArray const &src, gint sp, gint len, ByteArray &dst) override;

                 gint encodeFromUTF16(ByteArray const &src, gint sp, gint len, ByteArray &dst) override;

             protected:
                 CoderResult flush0(io::ByteBuffer &out) override;

                 void reset0() override;

                 CoderResult encodeArray(io::CharBuffer &src, io::ByteBuffer &dst) override;

                 CoderResult encodeBuffer(io::CharBuffer &src, io::ByteBuffer &dst) override;

                 gint state = SBCS;
             };

             class EncoderDBCS_ONLY : public virtual Encoder {
             public:
                 CORE_EXPLICIT EncoderDBCS_ONLY(Charset const &cs, ByteArray const &repl,
                                                CharArray &c2b, CharArray &c2bIndex, gbool isAsciiCompatible);

                 gint encodeChar(gchar ch) override;
             };

             // EUC_SIMPLE
             class EncoderEUC_SIM : public virtual Encoder {
             public:
                 CORE_EXPLICIT EncoderEUC_SIM(Charset const &cs,
                                              CharArray &c2b, CharArray &c2bIndex, gbool isAsciiCompatible);
             };
        };

        CORE_WARNING_POP
    } // charset
} // core

#endif //CORE24_DOUBLEBYTE_H
