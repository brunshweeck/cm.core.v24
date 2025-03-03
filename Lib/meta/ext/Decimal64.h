//
// Created by admin on 03/12/24.
//

#ifndef DECIMAL64_H
#define DECIMAL64_H
#include "DecimalFormatter.h"
#include <core/lang/Appendable.h>
#include <core/misc/Unsafe.h>
#include <meta/lang/MathUtils.h>

namespace core {
    namespace ext {

        class Decimal64 final : public Object {
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
            static CORE_FAST gint P = Double::PRECISION;

            /* Exponent width in bits */
            static CORE_FAST gint W = (Double::SIZE - 1) - (P - 1);

            /* Minimum value of the exponent: -(2^(W-1)) - P + 3 */
            static CORE_FAST gint Q_MIN = -(1 << (W - 1)) - P + 3;

            /* Maximum value of the exponent: 2^(W-1) - P */
            static CORE_FAST gint Q_MAX = (1 << (W - 1)) - P;

            /* 10^(E_MIN - 1) <= MIN_VALUE < 10^E_MIN */
            static CORE_FAST gint E_MIN = -323;

            /* 10^(E_MAX - 1) <= MAX_VALUE < 10^E_MAX */
            static CORE_FAST gint E_MAX = 309;

            /* Threshold to detect tiny values, as in section 8.2.1 of [1] */
            static CORE_FAST glong C_TINY = 3;

            /* The minimum and maximum k, as in section 8 of [1] */
            static CORE_FAST gint K_MIN = -324;
            static CORE_FAST gint K_MAX = 292;

            /* H is as in section 8.1 of [1] */
            static CORE_FAST gint H = 17;

            /* Minimum value of the significand of a normal value: 2^(P-1) */
            static CORE_FAST glong C_MIN = 1L << (P - 1);

            /* Mask to extract the biased exponent */
            static CORE_FAST gint BQ_MASK = (1 << W) - 1;

            /* Mask to extract the fraction bits */
            static CORE_FAST glong T_MASK = (1ULL << (P - 1)) - 1;

            /* Used in rop() */
            static CORE_FAST glong MASK_63 = (1ULL << 63) - 1; // Long::MAX_VALUE

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
             *     -ddddd.dddddddddddd         H + 2 characters
             *     -0.00ddddddddddddddddd      H + 5 characters
             *     -d.ddddddddddddddddE-eee    H + 7 characters
             * where there are H digits d
             */
            static CORE_FAST gint MAX_CHARS = H + 7;

            ByteArray bytes;

            /* Index into bytes of rightmost valid character */
            gint index = 0;

            CORE_EXPLICIT Decimal64(gbool noChars);

            /**
             * Returns a string representation of the @c double
             * argument. All characters mentioned below are ASCII characters.
             *
             * @param   v   the @c double to be converted.
             * @return a string representation of the argument.
             * @see Double#toString(double)
             */
            static String toString(gdouble v);

            /**
             * Splits the decimal <i>d</i> described in
             * @b Double::toString(double) in integers <i>f</i> and <i>e</i>
             * such that <i>d</i> = <i>f</i> 10<sup><i>e</i></sup>.
             *
             * <p>Further, determines integer <i>n</i> such that <i>n</i> = 0 when
             * <i>f</i> = 0, and
             * 10<sup><i>n</i>-1</sup> &le; <i>f</i> &lt; 10<sup><i>n</i></sup>
             * otherwise.
             *
             * <p>The argument @c v is assumed to be a positive finite value or
             * positive zero.
             * Further, @c fd must not be @c null.
             *
             * @param v     the finite @c double to be split.
             * @param fd    the object that will carry <i>f</i>, <i>e</i>, and <i>n</i>.
             */
            static void split(gdouble v, DecimalFormatter& fd);

            /**
             * Appends the rendering of the @c v to @c app.
             *
             * <p>
             * The outcome is the same as if @c v were first
             * @b rendered and the resulting string were then
             * @b appended to @c app.
             * </p>
             * @param v the @c double whose rendering is appended.
             * @param app the @b Appendable to append to.
             * @throws IOException If an I/O error occurs
             */
            static Appendable& appendTo(gdouble v, Appendable& app);

            String toDecimalString(gdouble v);

            Appendable& appendDecimalTo(gdouble v, Appendable& app);

            /*
             * Returns
             *     PLUS_ZERO       iff v is 0.0
             *     MINUS_ZERO      iff v is -0.0
             *     PLUS_INF        iff v is POSITIVE_INFINITY
             *     MINUS_INF       iff v is NEGATIVE_INFINITY
             *     NAN             iff v is NaN
             */
            gint toDecimal(gdouble v, Object& fd);

            gint toDecimal(gint q, glong c, gint dk, Object& fd);

            /*
             * Computes rop(cp g 2^(-127)), where g = g1 2^63 + g0
             * See section 9.9 and figure 8 of [1].
             */
            static glong rop(glong g1, glong g0, glong cp);

            /*
             * Formats the decimal f 10^e.
             */
            gint toChars(glong f, gint e, Object& fd);

            gint toChars1(gint h, gint m, gint l, gint e);

            gint toChars2(gint h, gint m, gint l, gint e);

            gint toChars3(gint h, gint m, gint l, gint e);

            void lowDigits(gint l);

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

#endif //DECIMAL64_H
