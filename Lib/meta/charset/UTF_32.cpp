//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_32.h>
#include <meta/charset/UTF_32Coder.h>

namespace core {
    namespace charset {
        UTF_32::UTF_32()
            : Unicode("UTF-32"_S, StringArray::of("UTF_32"_S, "UTF32"_S)) {
        }

        CharsetDecoder& UTF_32::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Decoder>(*this, UTF_32Coder::NONE);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_32::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Encoder>(*this, UTF_32Coder::BIG, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
