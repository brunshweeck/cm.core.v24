//
// Created by brunshweeck on 28/11/24.
//

#ifndef CORE24_NORMALIZER2_H
#define CORE24_NORMALIZER2_H

#include "Version.h"
#include <core/text/Normalizer.h>

namespace core {
    namespace text {
        class Normalizer2 final : public Object {
            enum Decomposition {
                NONE,
                CANONICAL,
                FRONT,
                NO_BREAK,
                INITIAL,
                MEDIAL,
                FINAL,
                ISOLATED,
                CIRCLE,
                SUPER,
                SUB,
                VERTICAL,
                WIDE,
                NARROW,
                SMALL,
                SQUARE,
                COMPACT,
                FRACTION,
            };

        public:
            /**
             * Returns the normalized form of the source string.
             * @param src source string
             * @return normalized src
             * @stable ICU 4.4
             */
            static String normalize(CharSequence const& src, Normalizer::Form mode);

            /**
             * Gets the decomposition mapping of c.
             * Roughly equivalent to normalizing the String form of c
             * on a DECOMPOSE Normalizer2 instance, but much faster, and except that this function
             * returns null if c does not have a decomposition mapping in this instance's data.
             * This function is independent of the mode of the Normalizer2.
             * @param c code point
             * @return c's decomposition mapping, if any; otherwise null
             * @stable ICU 4.6
             */
            static String getDecomposition(gint c);

            static Normalizer2::Decomposition getDecompositionStyle(gint c);

            /**
             * Performs pairwise composition of a &amp; b and returns the composite if there is one.
             *
             * <p>Returns a composite code point c only if c has a two-way mapping to a+b.
             * In standard Unicode normalization, this means that
             * c has a canonical decomposition to a+b
             * and c does not have the Full_Composition_Exclusion property.
             *
             * <p>This function is independent of the mode of the Normalizer2.
             * The default implementation returns a negative value.
             * @param a A (normalization starter) code point.
             * @param b Another code point.
             * @return The non-negative composite code point if there is one; otherwise a negative value.
             * @stable ICU 49
             */
            static gint composePair(gint a, gint b);

            /**
             * Gets the combining class of c.
             * The default implementation returns 0
             * but all standard implementations return the Unicode Canonical_Combining_Class value.
             * @param c code point
             * @return c's combining class
             * @stable ICU 49
             */
            static gint getCombiningClass(gint c);

            /**
             * Tests if the string is normalized.
             * Internally, in cases where the quickCheck() method would return "maybe"
             * (which is only possible for the two COMPOSE modes) this method
             * resolves to "yes" or "no" to provide a definitive result,
             * at the cost of doing more work in those cases.
             * @param src input string
             * @return true if s is normalized
             * @stable ICU 4.4
             */
            static gbool isNormalized(CharSequence const& src, Normalizer::Form mode);

            enum QuickCheckResult {
                NO,
                YES,
                MAYBE
            };

        private:
            /**
             * Writes the normalized form of the source string to the destination string
             * (replacing its contents) and returns the destination string.
             * The source and destination strings must be different objects.
             * @param src source string
             * @param dest destination string; its contents is replaced with normalized src
             * @return dest
             * @stable ICU 4.4
             */
            static XString& normalize(CharSequence const& src, XString& dest, Normalizer::Form mode);

            /**
             * Check if the input sequenced is in desired normalization form and return the last stable index.
             * if is already normalized the returned value is length of input sequence.
             *
             * @param src the input sequence
             * @param offset the start index
             * @param mode the normalization form
             * @return the last stable index for desired normalization
             */
            static gint spanQuickCheck(CharSequence const& src, gint offset, Normalizer::Form mode);

            static void decompose(CharSequence const& src, gint offset, XString& buffer, gbool canonical);

            static Decomposition getDecomposition(gint c, io::CharBuffer& cb);

            static void canonicalOrder(CharSequence const& src, gint offset, XString &buffer);

            static void compose(CharSequence const& src, gint offset, XString& buffer);

            class Hangul final : public Object {
            public:
                static CORE_FAST gint SBase = 0xac00;
                static CORE_FAST gint LBase = 0x1100;
                static CORE_FAST gint VBase = 0x1161;
                static CORE_FAST gint TBase = 0x11a7;
                static CORE_FAST gint LCount = 19;
                static CORE_FAST gint VCount = 21;
                static CORE_FAST gint TCount = 28;
                static CORE_FAST gint NCount = VCount * TCount;
                static CORE_FAST gint SCount = LCount * NCount;

                static gbool isHangul(gint c);

                static String getDecomposition(gint c);

                static gint composePair(gint a, gint b);
            };
        };
    } // text
} // core

#endif //CORE24_NORMALIZER2_H
