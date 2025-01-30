//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_16BE.h>
#include <meta/charset/UTF_16Coder.h>

namespace core {
    namespace charset {
        UTF_16BE::UTF_16BE()
            : Unicode("UTF-16BE"_S,
                      StringArray::of("UTF_16BE"_S, "ISO-10646-UCS-2"_S, "X-UTF-16BE"_S, "UnicodeBigUnmarked"_S)) {
        }

        CharsetDecoder& UTF_16BE::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_16Coder::Decoder>(*this, UTF_16Coder::BIG);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_16BE::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_16Coder::Encoder>(*this, UTF_16Coder::BIG, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
