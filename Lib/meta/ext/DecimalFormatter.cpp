//
// Created by admin on 03/12/24.
//

#include "DecimalFormatter.h"

#include "Decimal64.h"
#include "core/IllegalArgumentException.h"

namespace core {
    namespace ext {
        DecimalFormatter DecimalFormatter::valueOf(gdouble v, gint prec, gchar form) {
            DecimalFormatter fd;
            Decimal64::split(v, fd);
            switch (form) {
                case SCIENTIFIC: return fd.scientific(prec);
                case PLAIN: return fd.plain(prec);
                case GENERAL: return fd.general(prec);
                default: IllegalArgumentException(String::format("unsupported form '%c'", form)).throws($ftrace());
            }
        }

        void DecimalFormatter::set(glong f, gint e, gint n) {
            /* Initially, n = 0 if f = 0, and 10^{n-1} <= f < 10^n if f != 0 */
            DecimalFormatter::f = f;
            DecimalFormatter::e = e;
            DecimalFormatter::n = n;
        }

        CharArray DecimalFormatter::getExponent() const {
            return exp;
        }

        CharArray DecimalFormatter::getMantissa() const {
            return digits;
        }

        gint DecimalFormatter::getExponentRounded() const {
            return n + e - 1;
        }

        DecimalFormatter DecimalFormatter::plain(gint prec) {
            /*
             * Rounding d = f 10^e to prec digits in plain mode means the same
             * as rounding it to the p = n + e + prec most significand digits of d,
             * with the understanding that p < 0 cuts off all its digits.
             */
            round((glong)(n + e) + (glong)prec); // n + e is well inside the int range
            return plainChars();
        }

        DecimalFormatter& DecimalFormatter::plainChars() {
            if (e >= 0) {
                plainCharsPureInteger();
            } else if (n + e > 0) {
                plainCharsMixed();
            } else {
                plainCharsPureFraction();
            }
            return *this;
        }

        void DecimalFormatter::plainCharsPureInteger() {
            digits = CharArray(n + e);
            fillWithZeros(n, n + e);
            fillWithDigits(f, 0, n);
        }

        void DecimalFormatter::plainCharsMixed() {
            digits = CharArray(n + 1);
            glong x = fillWithDigits(f, n + 1 + e, n + 1);
            digits[n + e] = '.';
            fillWithDigits(x, 0, n + e);
        }

        void DecimalFormatter::plainCharsPureFraction() {
            digits = CharArray(2 - e);
            glong x = f;
            fillWithDigits(x, 2 - e - n, 2 - e);
            fillWithZeros(0, 2 - e - n);
            digits[1] = '.';
        }

        DecimalFormatter DecimalFormatter::scientific(gint prec) {
            /*
         * Rounding d = f 10^e to prec digits in scientific mode means the same
         * as rounding it to the p = prec + 1 most significand digits of d.
         */
            round(prec + 1L);
            return scientificChars(prec);
        }

        DecimalFormatter DecimalFormatter::scientificChars(gint prec) {
            if (prec != 0) {
                scientificCharsWithFraction();
            } else {
                scientificCharsNoFraction();
            }
            expChars();
            return *this;
        }

        void DecimalFormatter::scientificCharsWithFraction() {
            digits = CharArray(1 + n); // room for leading digit and for '.'
            glong x = fillWithDigits(f, 2, 1 + n);
            digits[1] = '.';
            digits[0] = toDigit(x);
        }

        void DecimalFormatter::scientificCharsNoFraction() {
            digits = CharArray(1);
            digits[0] = toDigit(f);
        }

        DecimalFormatter DecimalFormatter::general(gint prec) {
            /*
         * Rounding d = f 10^e to prec digits in general mode means the same
         * as rounding it to the p = prec most significand digits of d, and then
         * deciding whether to format it in plain or scientific mode, depending
         * on the rounded value.
         */
            round(prec);
            gint er = getExponentRounded();
            if (-4 <= er && er < prec) {
                plainChars();
            } else {
                scientificChars(prec - 1);
            }
            return *this;
        }

        void DecimalFormatter::expChars() {
            gint er = getExponentRounded();
            gint aer = Math::abs(er);
            exp = CharArray(aer >= 100 ? 4 : 3);
            gint q;
            if (aer >= 100) {
                q = aer / 10;
                exp[3] = toDigit(aer - 10 * q);
                aer = q;
            }
            q = aer / 10;
            exp[2] = toDigit(aer - 10 * q);
            exp[1] = toDigit(q);
            exp[0] = er >= 0 ? '+' : '-';
        }

        void DecimalFormatter::round(glong pp) {
            /*
             * Let d = f 10^e, and let p shorten pp.
             * This method rounds d to the p most significant digits.
             * It does so by possibly modifying f, e and n.
             * When f becomes 0, e and n are normalized to 0 and 1, resp.
             *
             * For any real x let
             *      r(x) = floor(x + 1/2)
             * which is rounding to the closest integer, with ties rounded toward
             * positive infinity.
             *
             * When f = 0 there's not much to say, except that this holds iff n = 0.
             *
             * Otherwise, since
             *      10^{n-1} <= f < 10^n
             * it follows that
             *      10^{e+n-1} <= d < 10^{e+n}
             * To round d to the most significant p digits, first scale d to the
             * range [10^{p-1}, 10^p), cutoff the fractional digits by applying r,
             * and finally scale back.
             * To this end, first define
             *      ds = d 10^{p-e-n}
             * which ensures
             *      10^{p-1} <= ds < 10^p
             *
             * Now, if p < 0 (that is, if p <= -1) then
             *      ds < 10^p <= 10^{-1} < 1/2
             * so that
             *      r(ds) = 0
             * Thus, rounding d to p < 0 digits leads to 0.
             */
            if (n == 0 || pp < 0) {
                f = 0;
                e = 0;
                n = 1;
                return;
            }

            /*
             * Further, if p >= n then
             *      ds = f 10^e 10^{p-e-n} = f 10^{p-n}
             * which shows that ds is an integer, so r(ds) = ds. That is,
             * rounding to p >= n digits leads to a result equal to d.
             */
            if (pp >= (glong)n) { // no rounding needed
                return;
            }

            /*
             * Finally, 0 <= p < n. When p = 0 it follows that
             *      10^{-1} <= ds < 1
             *      0 <= f' = r(ds) <= 1
             * that is, f' is either 0 or 1.
             *
             * Otherwise
             *      10^{p-1} <= ds < 10^p
             *      1 <= 10^{p-1} <= f' = r(ds) <= 10^p
             * Note that f' = 10^p is a possible outcome.
             *
             * Scale back, where e' = e + n - p
             *      d' = f' 10^{e+n-p} = f' 10^e', with 10^{e+n-1} <= d' <= 10^{e+n}
             *
             * Since n > p, f' can be computed in integer arithmetic as follows,
             * where / denotes division in the real numbers:
             *      f' = r(ds) = r(f 10^{p-n}) = r(f / 10^{n-p})
             *          = floor(f / 10^{n-p} + 1/2)
             *          = floor((f + 10^{n-p}/2) / 10^{n-p})
             */
            gint p = (gint) pp; // 0 <= pp < n, safe cast
            e += n - p; // new e is well inside the int range
            glong pow10 = MathUtils::powerOfTeen(n - p);
            f = (f + (pow10 >> 1)) / pow10;
            if (p == 0) {
                n = 1;
                if (f == 0L) {
                    e = 0;
                }
                return;
            }

            n = p;
            if (f == MathUtils::powerOfTeen(p)) {
                /*
                 * f is n + 1 digits long.
                 * Absorb one trailing zero into e and reduce f accordingly.
                 */
                f /= 10;
                e += 1;
            }
        }

        glong DecimalFormatter::fillWithDigits(glong x, gint from, gint to) {
            while (to > from) {
                glong q = x / 10;
                digits[--to] = toDigit(x - q * 10);
                x = q;
            }
            return x;
        }

        void DecimalFormatter::fillWithZeros(gint from, gint to) {
            while (to > from) {
                digits[--to] = '0';
            }
        }

        gchar DecimalFormatter::toDigit(glong d) {
            return toDigit((gint)d);
        }

        gchar DecimalFormatter::toDigit(gint d) {
            return (gchar)(d + '0');
        }
    } // ext
} // core
