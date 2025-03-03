//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_16.h>
#include <meta/charset/UTF_16Coder.h>

namespace core {
    namespace charset {
        UTF_16::UTF_16()
            : Unicode("UTF-16"_S, StringArray::of("UTF_16"_S, "utf16"_S, "unicode"_S, "UnicodeBig"_S)) {
        }

        CharsetDecoder& UTF_16::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_16Coder::Decoder>(*this, UTF_16Coder::NONE);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_16::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_16Coder::Encoder>(*this, UTF_16Coder::BIG, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
