//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_UTF_16CODER_H
#define CORE24_UTF_16CODER_H

#include <meta/charset/Surrogate.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>

namespace core {
    namespace charset {
        class UTF_16Coder final : public Object {
        public:
            static CORE_FAST gchar BYTE_ORDER_MARK = 0xfeff;
            static CORE_FAST gchar REVERSED_MARK = 0xffef;

            enum {
                NONE = 0,
                BIG = 1,
                LITTLE = 2,
            };

            class Decoder : public CharsetDecoder {
                gint exceptedByteOrder;
                gint currentByteOrder;
                gint defaultByteOrder = BIG;

            public:
                CORE_EXPLICIT Decoder(Charset const& cs, gint bo);

                CORE_EXPLICIT Decoder(Charset const& cs, gint bo, gint defaultBO);

            protected:
                CoderResult decode0(io::ByteBuffer& src, io::CharBuffer& dst) override;

                void reset0() override;

            private:
                gchar decode(gint b1, gint b2) const;
            };

            class Encoder : public CharsetEncoder {

                int byteOrder; /* Byte order in use */
                gbool usesMark; /* Write an initial BOM */
                gbool needsMark;

                Surrogate::Parser sgp = {};
            public:
                CORE_EXPLICIT Encoder(Charset const& cs, gint bo, gbool m);

                gbool canEncode(gchar c) override;

            protected:
                void reset0() override;

                CoderResult encode0(io::CharBuffer& in, io::ByteBuffer& out) override;

            private:
                void put(gchar c, io::ByteBuffer &dst);
            };
        };
    } // charset
} // core

#endif //CORE24_UTF_16CODER_H
