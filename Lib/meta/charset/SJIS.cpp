//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/SJIS.h>

namespace core {
    namespace charset {
        SJIS::SJIS(): Charset("Shift_JIS"_S, StringArray::of(
                                  "sjis"_S,
                                  "shift_jis"_S,
                                  "shift-jis"_S,
                                  "ms_kanji"_S,
                                  "x-sjis"_S,
                                  "csShiftJIS"_S)) {
        }

        gbool SJIS::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<SJIS>::hasInstance(cs);
        }

        CharsetDecoder &SJIS::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFC, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &SJIS::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String SJIS::Holder::b2cSBStr = {};
        StringArray SJIS::Holder::b2cStr = {};
        CharArray2D SJIS::Holder::b2c = {};
        CharArray SJIS::Holder::b2cSB = {};
        CharArray SJIS::Holder::c2b = {};
        CharArray SJIS::Holder::c2bIndex = {};
        gint SJIS::Holder::initDecoderStatus = 123456789;
        gint SJIS::Holder::initEncoderStatus = 123456789;

        void SJIS::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/SJIS.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load SJIS Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void SJIS::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x5E00);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = String(CharArray{
                        0x005C, 0x00A5, 0x007E, 0x203E,
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFC, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load SJIS Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
