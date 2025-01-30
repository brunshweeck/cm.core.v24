//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/UTF_32BE_BOM.h>
#include <meta/charset/UTF_32Coder.h>

namespace core {
    namespace charset {
        UTF_32BE_BOM::UTF_32BE_BOM()
            : Unicode("X-UTF-32BE-BOM"_S, StringArray::of("UTF_32BE_BOM"_S, "UTF-32BE-BOM"_S)) {
        }

        CharsetDecoder& UTF_32BE_BOM::newDecoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Decoder>(*this, UTF_32Coder::BIG);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_32BE_BOM::newEncoder() const {
            try {
                return UNSAFE::newInstance<UTF_32Coder::Encoder>(*this, UTF_32Coder::BIG, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
