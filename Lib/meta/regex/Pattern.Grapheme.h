//
// Created by bruns on 16/10/2024.
//

#ifndef CORE24_PATTERN_GRAPHEME_H
#define CORE24_PATTERN_GRAPHEME_H

#include <core/regex/Pattern.h>

namespace core {
    namespace regex {
        class Pattern::Grapheme final: public Object {
        public:



            /**
             * Look for the next extended grapheme cluster boundary in a CharSequence.
             * It assumes the start of the char sequence at offset {@code off} is a boundary.
             * <p>
             * See Unicode Standard Annex #29 Unicode Text Segmentation for the specification
             * for the extended grapheme cluster boundary rules. The following implementation
             * is based on the annex for Unicode version 15.1.
             *
             * @spec http://www.unicode.org/reports/tr29/tr29-43.html
             * @param src the {@code CharSequence} to be scanned
             * @param off offset to start looking for the next boundary in the src
             * @param limit limit offset in the src (exclusive)
             * @return the next grapheme boundary
             */
            static gint nextBoundary(CharSequence const& src, gint off, gint limit);

            // types
            static CORE_FAST gint OTHER = 0;
            static CORE_FAST gint CR = 1;
            static CORE_FAST gint LF = 2;
            static CORE_FAST gint CONTROL = 3;
            static CORE_FAST gint EXTEND = 4;
            static CORE_FAST gint ZWJ = 5;
            static CORE_FAST gint RI = 6;
            static CORE_FAST gint PREPEND = 7;
            static CORE_FAST gint SPACINGMARK = 8;
            static CORE_FAST gint L = 9;
            static CORE_FAST gint V = 10;
            static CORE_FAST gint T = 11;
            static CORE_FAST gint LV = 12;
            static CORE_FAST gint LVT = 13;
            static CORE_FAST gint EXTENDED_PICTOGRAPHIC = 14;

            static CORE_FAST gint FIRST_TYPE = 0;
            static CORE_FAST gint LAST_TYPE = 14;

            static gint initRules();

            // Hangul syllables
            static CORE_FAST gint SYLLABLE_BASE = 0xAC00;
            static CORE_FAST gint LCOUNT = 19;
            static CORE_FAST gint VCOUNT = 21;
            static CORE_FAST gint TCOUNT = 28;
            static CORE_FAST gint NCOUNT = VCOUNT * TCOUNT; // 588
            static CORE_FAST gint SCOUNT = LCOUNT * NCOUNT; // 11172

            // #tr29: SpacingMark exceptions: The following (which have
            // General_Category = Spacing_Mark and would otherwise be included)
            // are specifically excluded
            static gbool isExcludedSpacingMark(gint cp);

            static gint getType(gint cp);

            /**
             * Checks for a possible GB9c Indic Conjunct Break sequence. If it is
             * repetitive, e.g., Consonant1/Linker1/Consonant2/Linker2/Consonant3, only
             * the first part of the sequence (Consonant1/Linker1/Consonant2) is
             * recognized. The rest is analyzed in the next iteration of the grapheme
             * cluster boundary search.
             *
             * @param src the source char sequence
             * @param index the index that points to the starting Linking Consonant
             * @param limit limit to the char sequence
             * @return the advance in index if the indic conjunct break sequence
             *      is found, it will be negative if the sequence is not found
             */
            static gint checkIndicConjunctBreak(CharSequence const& src, gint index, gint limit);

            static gbool rules[LAST_TYPE + 1][LAST_TYPE + 1];
            static gint rulesCount;
        };
    } // regex
} // core

#endif //CORE24_PATTERN_GRAPHEME_H
