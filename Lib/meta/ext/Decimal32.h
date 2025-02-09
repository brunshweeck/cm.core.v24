//
// Created by admin on 04/12/24.
//

#ifndef DECIMAL32_H
#define DECIMAL32_H
#include "core/Float.h"
#include "core/String.h"

namespace core {
    namespace ext {
        class Decimal32 final : public Object {
        public:
            /*
             * For full details about this code see the following references:
             *
             * [1] Giulietti, "The Schubfach way to render doubles",
             *     https://drive.google.com/file/d/1gp5xv4CAa78SVgCeWfGqqI4FfYYYuNFb
             *
             * [2] IEEE Computer Society, "IEEE Standard for Floating-Point Arithmetic"
             *
             * [3] Bouvier & Zimmermann, "Division-Free Binary-to-Decimal Conversion"
             *
             * Divisions are avoided altogether for the benefit of those architectures
             * that do not provide specific machine instructions or where they are slow.
             * This is discussed in section 10 of [1].
             */

            /* The precision in bits */
            static CORE_FAST gint P = Float::PRECISION;

            /* Exponent width in bits */
            static CORE_FAST gint W = (Float::SIZE - 1) - (P - 1);

            /* Minimum value of the exponent: -(2^(W-1)) - P + 3 */
            static CORE_FAST gint Q_MIN = -(1 << (W - 1)) - P + 3;

            /* Maximum value of the exponent: 2^(W-1) - P */
            static CORE_FAST gint Q_MAX = (1 << (W - 1)) - P;

            /* 10^(E_MIN - 1) <= MIN_VALUE < 10^E_MIN */
            static CORE_FAST gint E_MIN = -44;

            /* 10^(E_MAX - 1) <= MAX_VALUE < 10^E_MAX */
            static CORE_FAST gint E_MAX = 39;

            /* Threshold to detect tiny values, as in section 8.2.1 of [1] */
            static CORE_FAST gint C_TINY = 8;

            /* The minimum and maximum k, as in section 8 of [1] */
            static CORE_FAST gint K_MIN = -45;
            static CORE_FAST gint K_MAX = 31;

            /* H is as in section 8.1 of [1] */
            static CORE_FAST gint H = 9;

            /* Minimum value of the significand of a normal value: 2^(P-1) */
            static CORE_FAST gint C_MIN = 1 << (P - 1);

            /* Mask to extract the biased exponent */
            static CORE_FAST gint BQ_MASK = (1 << W) - 1;

            /* Mask to extract the fraction bits */
            static CORE_FAST gint T_MASK = (1 << (P - 1)) - 1;

            /* Used in rop() */
            static CORE_FAST glong MASK_32 = (1L << 32) - 1;

            /* Used for left-to-tight digit extraction */
            static CORE_FAST gint MASK_28 = (1 << 28) - 1;

            static CORE_FAST gint NON_SPECIAL = 0;
            static CORE_FAST gint PLUS_ZERO = 1;
            static CORE_FAST gint MINUS_ZERO = 2;
            static CORE_FAST gint PLUS_INF = 3;
            static CORE_FAST gint MINUS_INF = 4;
            static CORE_FAST gint NAN = 5;

            /*
             * Room for the longer of the forms
             *     -ddddd.dddd         H + 2 characters
             *     -0.00ddddddddd      H + 5 characters
             *     -d.ddddddddE-ee     H + 6 characters
             * where there are H digits d
             */
            static CORE_FAST gint MAX_CHARS = H + 6;

            ByteArray bytes = ByteArray(MAX_CHARS);

            /* Index into bytes of rightmost valid character */
            gint index = 0;

            Decimal32() = default;

            /**
             * Returns a string representation of the @c float
             * argument. All characters mentioned below are ASCII characters.
             *
             * @param   v   the @c float to be converted.
             * @return a string representation of the argument.
             * @see Float#toString(float)
             */
            static String toString(gfloat v);

            /**
             * Appends the rendering of the @c v to @c app.
             *
             * <p>
             * The outcome is the same as if @c v were first
             * @b rendered and the resulting string were then
             * @b appended to @c app.
             * </p>
             * @param v the @c float whose rendering is appended.
             * @param app the @b Appendable to append to.
             * @throws IOException If an I/O error occurs
             */
            static Appendable& appendTo(gfloat v, Appendable& app);

            String toDecimalString(gfloat v);

            Appendable& appendDecimalTo(gfloat v, Appendable& app);

            /*
             * Returns
             *     PLUS_ZERO       iff v is 0.0
             *     MINUS_ZERO      iff v is -0.0
             *     PLUS_INF        iff v is POSITIVE_INFINITY
             *     MINUS_INF       iff v is NEGATIVE_INFINITY
             *     NAN             iff v is NaN
             */
            gint toDecimal(gfloat v);

            gint toDecimal(gint q, gint c, gint dk);

            /*
             * Computes rop(cp g 2^(-95))
             * See appendix and figure 11 of [1].
             */
            static gint rop(glong g, glong cp);

            /*
             * Formats the decimal f 10^e.
             */
            gint toChars(gint f, gint e);

            gint toChars1(gint h, gint l, gint e);

            gint toChars2(gint h, gint l, gint e);

            gint toChars3(gint h, gint l, gint e);

            void append8Digits(gint m);

            void removeTrailingZeroes();

            static gint y(gint a);

            void exponent(gint e);

            void append(gint c);

            void appendDigit(gint d);

            /* Using the deprecated constructor enhances performance */
            String charsToString() const;
        };
    } // ext
} // core

#endif //DECIMAL32_H
