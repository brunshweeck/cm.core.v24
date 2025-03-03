//
// Created by brunshweeck on 28/11/24.
//

#ifndef CORE24_NORMALIZER2_PROPERTY_H
#define CORE24_NORMALIZER2_PROPERTY_H

#include "Version.h"

namespace core {
    namespace text {
        class Property final {
        public:
            gshort type : 8; /* 5 used */
            gshort direction : 8; /* 5 used */
            gchar ccc : 8;
            gchar joining : 3;
            gshort version : 8; /* 5 used */
            gchar eastAsianWidth : 3; /* 3 used */
            gchar nfQuickCheck : 8;
            gchar graphemeBreakClass : 5; /* 5 used */
            gchar wordBreakClass : 5; /* 5 used */
            gchar lineBreakClass : 6; /* 6 used */
            gchar sentenceBreakClass : 4; /* 4 used */
            gshort idnaStatus : 4; /* 3 used */

            static Property const& getProperty(gint c);

            static gint getPropertyIndex(gint c);

            Version const& getVersion() const;

            gint getCombiningClass() const;

            gint getType() const;

            static gint getDecompositionIndex(gint c);

            static glong getDecomposition(gint c);

            static gint getLigatureIndex(gint c);

            static glong getLigature(gint c);
        };
    }
}

#endif //CORE24_NORMALIZER2_PROPERTY_H
