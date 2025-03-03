//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/JIS_X_0208_MS5022X.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        JIS_X_0208_MS5022X::JIS_X_0208_MS5022X(): Charset("x-JIS0208_MS5022X"_S, {}) {
        }

        gbool JIS_X_0208_MS5022X::contains(const Charset &cs) const {
            return Class<JIS_X_0208_MS5022X>::hasInstance(cs);
        }

        CharsetDecoder &JIS_X_0208_MS5022X::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderDBCS_ONLY>(
                    *this, Holder::b2c, Holder::b2cSB, 0x21, 0x7E, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &JIS_X_0208_MS5022X::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderDBCS_ONLY>(*this, ByteArray({0x21, 0x29}), Holder::c2b,
                                                                Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String JIS_X_0208_MS5022X::Holder::b2cSBStr = {};
        StringArray JIS_X_0208_MS5022X::Holder::b2cStr = {};
        CharArray2D JIS_X_0208_MS5022X::Holder::b2c = {};
        CharArray JIS_X_0208_MS5022X::Holder::b2cSB = {};
        CharArray JIS_X_0208_MS5022X::Holder::c2b = {};
        CharArray JIS_X_0208_MS5022X::Holder::c2bIndex = {};
        gint JIS_X_0208_MS5022X::Holder::initDecoderStatus = 123456789;
        gint JIS_X_0208_MS5022X::Holder::initEncoderStatus = 123456789;

        void JIS_X_0208_MS5022X::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/JIS_X_0208_MS5022X.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load JIS_X_0208_MS5022X Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void JIS_X_0208_MS5022X::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x6500);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = String(CharArray{
                        0x215D, 0xFF0D, 0x2142, 0x2225, 0x2123, 0xFF61, 0x2156, 0xFF62,
                        0x2157, 0xFF63, 0x2122, 0xFF64, 0x2126, 0xFF65, 0x2572, 0xFF66,
                        0x2521, 0xFF67, 0x2523, 0xFF68, 0x2525, 0xFF69, 0x2527, 0xFF6A,
                        0x2529, 0xFF6B, 0x2563, 0xFF6C, 0x2565, 0xFF6D, 0x2567, 0xFF6E,
                        0x2543, 0xFF6F, 0x213C, 0xFF70, 0x2522, 0xFF71, 0x2524, 0xFF72,
                        0x2526, 0xFF73, 0x2528, 0xFF74, 0x252A, 0xFF75, 0x252B, 0xFF76,
                        0x252D, 0xFF77, 0x252F, 0xFF78, 0x2531, 0xFF79, 0x2533, 0xFF7A,
                        0x2535, 0xFF7B, 0x2537, 0xFF7C, 0x2539, 0xFF7D, 0x253B, 0xFF7E,
                        0x253D, 0xFF7F, 0x253F, 0xFF80, 0x2541, 0xFF81, 0x2544, 0xFF82,
                        0x2546, 0xFF83, 0x2548, 0xFF84, 0x254A, 0xFF85, 0x254B, 0xFF86,
                        0x254C, 0xFF87, 0x254D, 0xFF88, 0x254E, 0xFF89, 0x254F, 0xFF8A,
                        0x2552, 0xFF8B, 0x2555, 0xFF8C, 0x2558, 0xFF8D, 0x255B, 0xFF8E,
                        0x255E, 0xFF8F, 0x255F, 0xFF90, 0x2560, 0xFF91, 0x2561, 0xFF92,
                        0x2562, 0xFF93, 0x2564, 0xFF94, 0x2566, 0xFF95, 0x2568, 0xFF96,
                        0x2569, 0xFF97, 0x256A, 0xFF98, 0x256B, 0xFF99, 0x256C, 0xFF9A,
                        0x256D, 0xFF9B, 0x256F, 0xFF9C, 0x2573, 0xFF9D, 0x212B, 0xFF9E,
                        0x212C, 0xFF9F, 0x2171, 0xFFE0, 0x2172, 0xFFE1, 0x224C, 0xFFE2
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x21, 0x7E, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load JIS_X_0208_MS5022X Encoder table"_S, ex).throws(
                        $ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
