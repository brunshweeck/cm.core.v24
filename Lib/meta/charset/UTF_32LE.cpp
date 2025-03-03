//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_32LE.h>
#include <meta/charset/UTF_32Coder.h>

namespace core {
    namespace charset {
        UTF_32LE::UTF_32LE()
            : Unicode("UTF-32LE"_S, StringArray::of("UTF_32LE"_S, "X-UTF-32LE"_S)) {
        }

        CharsetDecoder& UTF_32LE::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Decoder>(*this, UTF_32Coder::LITTLE);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_32LE::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Encoder>(*this, UTF_32Coder::LITTLE, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
