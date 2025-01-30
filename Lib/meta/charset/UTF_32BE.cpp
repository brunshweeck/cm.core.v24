//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_32BE.h>
#include <meta/charset/UTF_32Coder.h>

namespace core {
    namespace charset {
        UTF_32BE::UTF_32BE()
        : Unicode("UTF-32BE"_S, StringArray::of("UTF_32BE"_S, "X-UTF-32BE"_S)) {
        }

        CharsetDecoder& UTF_32BE::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Decoder>(*this, UTF_32Coder::BIG);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_32BE::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Encoder>(*this, UTF_32Coder::BIG, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
