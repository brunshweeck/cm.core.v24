//
// Created by brunshweeck on 15 sept. 2024.
//

#include <core/text/Normalizer.h>
#include <meta/text/Normalizer2.h>

namespace core {
    namespace text {
        String Normalizer::normalize(CharSequence const& src, Form form) {
            return Normalizer2::normalize(src, form);
        }

        gbool Normalizer::isNormalized(CharSequence const& src, Form form) {
            return Normalizer2::isNormalized(src, form);
        }
    } // text
} // core
