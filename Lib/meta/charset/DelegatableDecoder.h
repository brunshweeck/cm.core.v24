//
// Created by brunshweeck on 8 sept. 2024.
//

#ifndef CORE24_DELEGATABLEDECODER_H
#define CORE24_DELEGATABLEDECODER_H

#include <core/Object.h>
#include <core/charset/CoderResult.h>

namespace core {
    namespace charset {
        /**
         * A decoder that can be delegated to by another decoder
         * when normal inheritance cannot be used.
         * Used by autodecting decoders.
         */
        class DelegatableDecoder : public virtual Object {
        public:
            virtual CoderResult decode0(io::ByteBuffer &src, io::CharBuffer &dst) = 0;

            virtual void reset0() = 0;

            virtual CoderResult flush0(io::CharBuffer &out) = 0;
        };
    } // charset
} // core

#endif //CORE24_DELEGATABLEDECODER_H
