//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_16LE.h>
#include <meta/charset/UTF_16Coder.h>

namespace core {
    namespace charset {
        UTF_16LE::UTF_16LE()
            : Unicode("UTF-16LE"_S, StringArray::of("UTF_16LE"_S, "X-UTF-16LE"_S, "UnicodeLittleUnmarked"_S)) {
        }

        CharsetDecoder& UTF_16LE::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_16Coder::Decoder>(*this, UTF_16Coder::LITTLE);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_16LE::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_16Coder::Encoder>(*this, UTF_16Coder::LITTLE, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
