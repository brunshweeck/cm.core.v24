//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_16LE_BOM.h>
#include <meta/charset/UTF_16Coder.h>

namespace core {
    namespace charset {
        UTF_16LE_BOM::UTF_16LE_BOM()
            : Unicode("x-UTF-16LE-BOM"_S, StringArray::of("UnicodeLittle"_S)) {
        }

        CharsetDecoder& UTF_16LE_BOM::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_16Coder::Decoder>(*this, UTF_16Coder::NONE, UTF_16Coder::LITTLE);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_16LE_BOM::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_16Coder::Encoder>(*this, UTF_16Coder::LITTLE, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core

