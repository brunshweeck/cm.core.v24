//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_32LE_BOM.h>
#include <meta/charset/UTF_32Coder.h>

namespace core {
    namespace charset {
        UTF_32LE_BOM::UTF_32LE_BOM()
            : Unicode("X-UTF-32LE-BOM"_S, StringArray::of("UTF_32LE_BOM"_S, "UTF-32LE-BOM"_S)) {
        }

        CharsetDecoder& UTF_32LE_BOM::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Decoder>(*this, UTF_32Coder::LITTLE);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_32LE_BOM::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Encoder>(*this, UTF_32Coder::LITTLE, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
