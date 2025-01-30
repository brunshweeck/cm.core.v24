//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_MS950_HKSCS_XP_H
#define CORE24_MS950_HKSCS_XP_H

#include <core/charset/Charset.h>
#include <meta/charset/HKSCS.h>
#include <meta/charset/MS950.h>

namespace core {
    namespace charset {
        class MS950_HKSCS_XP final : public Charset {
        public:
            CORE_IMPLICIT MS950_HKSCS_XP();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &newDecoder() const override;

            CharsetEncoder &newEncoder() const override;

            static gint initDecoderStatus;
            static gint initEncoderStatus;

            static void loadDecoderData();

            static void loadEncoderData();

            class Decoder final : public HKSCS::Decoder {
            public:
                static DoubleByte::Decoder *ms950Decoder;

                /*
                 * Note current decoder decodes 0x8BC2 --> U+F53A
                 * ie. maps to Unicode PUA.
                 * Unaccounted discrepancy between this mapping
                 * inferred from MS950/windows-950 and the published
                 * MS HKSCS mappings which maps 0x8BC2 --> U+5C22
                 * a character defined with the Unified CJK block
                 */
                static CharArray2D b2cBmp;

                CORE_EXPLICIT Decoder(Charset const &cs);

                gchar decodeDoubleEx(gint b1, gint b2) const override;
            };

            class Encoder final : public HKSCS::Encoder {
            public:
                static DoubleByte::Encoder *ms950Encoder;

                /*
                 * Note current encoder encodes U+F53A --> 0x8BC2
                 * Published MS HKSCS mappings show
                 * U+5C22 <--> 0x8BC2
                 */
                static CharArray2D c2bBmp;

                CORE_EXPLICIT Encoder(Charset const &cs);

                gint encodeSupp(gint cp) const override;
            };
        };
    } // charset
} // core

#endif //CORE24_MS950_HKSCS_XP_H
