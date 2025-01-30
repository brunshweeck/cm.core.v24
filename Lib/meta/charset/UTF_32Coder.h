//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_UTF_32CODER_H
#define CORE24_UTF_32CODER_H

#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>

namespace core {
    namespace charset {
        class UTF_32Coder : public Object {
        public:
            static CORE_FAST gint BOM_BIG = 0x0000FEFF;
            static CORE_FAST gint BOM_LITTLE = CORE_CAST(gint, 0xFFFE0000);

            enum {
                NONE = 0,
                BIG = 1,
                LITTLE = 2
            };

            class Decoder : public CharsetDecoder {
                gint currentBO;
                gint expectedBO;

            public:
                CORE_EXPLICIT Decoder(Charset const& cs, gint bo);

            protected:
                CoderResult decode0(io::ByteBuffer& in, io::CharBuffer& out) override;

                void reset0() override;

            private:
                gint getCP(io::ByteBuffer &src) const;
            };

            class Encoder : public CharsetEncoder {
                gbool doBOM = false;
                gbool doneBOM = true;
                gint byteOrder;

            public:
                CORE_EXPLICIT Encoder(Charset const& cs, gint byteOrder, gbool doBOM);

            protected:
                CoderResult encode0(io::CharBuffer& in, io::ByteBuffer& out) override;

                void reset0() override;

            private:
                void put(gint cp, io::ByteBuffer &dst) const;
            };
        };
    } // charset
} // core

#endif //CORE24_UTF_32CODER_H
