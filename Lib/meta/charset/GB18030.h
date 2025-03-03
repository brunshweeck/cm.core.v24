//
// Created by brunshweeck on 8 sept. 2024.
//

#ifndef CORE24_GB18030_H
#define CORE24_GB18030_H

#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <meta/charset/Surrogate.h>

namespace core {
    namespace charset {
        class GB18030 final : public virtual Charset {
            static CORE_FAST gint GB18030_SINGLE_BYTE = 1;
            static CORE_FAST gint GB18030_DOUBLE_BYTE = 2;
            static CORE_FAST gint GB18030_FOUR_BYTE = 3;

        public:
            static CORE_FAST gbool IS_2000 = UNSAFE::USE_CHARSET_GB18030_2000;

            CORE_IMPLICIT GB18030();

            gbool contains(Charset const& cs) const override;

            CharsetDecoder& newDecoder() const override;

            CharsetEncoder& newEncoder() const override;

        private:
            static ShortArray decoderIndex1;
            static StringArray decoderIndex2;
            static ShortArray index1;
            static StringArray index2;
            static ShortArray encoderIndex1;
            static StringArray encoderIndex2;

            static gint initStatus;
            static gint initDecoderStatus;
            static gint initEncoderStatus;

            static void initializeIndex();
            static void initialize2000Index();
            static void initializeDecoderIndex();
            static void initializeDecoder2000Index();
            static void initializeEncoderIndex();
            static void initializeEncoder2000Index();

            class Decoder final : public CharsetDecoder {
                static CORE_FAST gchar REPLACE_CHAR = u'\uFFFD';
                gint currentState = GB18030_DOUBLE_BYTE;

            public:
                CORE_EXPLICIT Decoder(Charset const& cs);

            protected:
                static gchar decodeDouble(gint byte1, gint byte2) ;

                void reset0() override;

                CoderResult decode0(io::ByteBuffer& src, io::CharBuffer& dst) override;

            private:
                static gchar getChar(gint offset);

                CoderResult decodeArray(io::ByteBuffer& src, io::CharBuffer& dst);

                CoderResult decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst);
            };

            class Encoder final : public CharsetEncoder {
                gint currentState = GB18030_DOUBLE_BYTE;
                Surrogate::Parser sgp = {};

            public:
                CORE_EXPLICIT Encoder(Charset const& cs);

                gbool canEncode(gchar c) override;

            protected:
                void reset0() override;

                CoderResult encode0(io::CharBuffer& src, io::ByteBuffer& dst) override;

            private:
                static gint getGB18030(ShortArray const &outerIndex, StringArray const &innerEncoderIndex, gchar ch);

                CoderResult encodeArray(io::CharBuffer& src, io::ByteBuffer& dst);

                CoderResult encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst);
            };
        };
    } // charset
} // core

#endif //CORE24_GB18030_H
