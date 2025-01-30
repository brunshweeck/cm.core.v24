//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/EUC_KR.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        EUC_KR::EUC_KR(): Charset("EUC-KR"_S, {
                                      "euc_kr"_S,
                                      "ksc5601"_S,
                                      "euckr"_S,
                                      "ks_c_5601-1987"_S,
                                      "ksc5601-1987"_S,
                                      "ksc5601_1987"_S,
                                      "ksc_5601"_S,
                                      "csEUCKR"_S,
                                      "5601"_S
                                  }) {
        }

        gbool EUC_KR::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<EUC_KR>::hasInstance(cs);
        }

        CharsetDecoder &EUC_KR::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0xA1, 0xFE, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &EUC_KR::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String EUC_KR::Holder::b2cSBStr = {};
        StringArray EUC_KR::Holder::b2cStr = {};
        CharArray2D EUC_KR::Holder::b2c = {};
        CharArray EUC_KR::Holder::b2cSB = {};
        CharArray EUC_KR::Holder::c2b = {};
        CharArray EUC_KR::Holder::c2bIndex = {};
        gint EUC_KR::Holder::initDecoderStatus = 123456789;
        gint EUC_KR::Holder::initEncoderStatus = 123456789;

        void EUC_KR::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/EUC_KR.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load EUC_KR Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void EUC_KR::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x9200);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0xA1, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load EUC_KR Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
