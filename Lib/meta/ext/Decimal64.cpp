//
// Created by admin on 03/12/24.
//

#include "Decimal64.h"

#include <core/lang/Math.h>
#include <core/lang/XString.h>
#include <meta/ext/DecimalFormatter.h>

namespace core {
    namespace ext {
        Decimal64::Decimal64(gbool noChars) {
            bytes = noChars ? ByteArray() : ByteArray(MAX_CHARS);
        }

        String Decimal64::toString(gdouble v) {
            return Decimal64(false).toDecimalString(v);
        }

        void Decimal64::split(gdouble v, DecimalFormatter& fd) {
            Decimal64(true).toDecimal(v, fd);
        }

        Appendable& Decimal64::appendTo(gdouble v, Appendable& app) {
            return Decimal64(false).appendDecimalTo(v, app);
        }

        String Decimal64::toDecimalString(gdouble v) {
            switch (toDecimal(v, null)) {
                case NON_SPECIAL: return charsToString();
                case PLUS_ZERO: return "0.0";
                case MINUS_ZERO: return "-0.0";
                case PLUS_INF: return "Infinity";
                case MINUS_INF: return "-Infinity";
                default: return "NaN";
            }
        }

        Appendable& Decimal64::appendDecimalTo(gdouble v, Appendable& app) {
            switch (toDecimal(v, null)) {
                case NON_SPECIAL: {
                    CharArray chars = CharArray(index + 1);
                    for (gint i = 0; i < chars.length(); ++i) {
                        chars[i] = (gchar) bytes[i];
                    }
                    if (Class<XString>::hasInstance(app)) {
                        return CORE_XCAST(XString, app).append(chars);
                    }
                    for (gchar c : chars) {
                        app.append(c);
                    }
                    return app;
                }
                case PLUS_ZERO: return app.append("0.0"_Sl);
                case MINUS_ZERO: return app.append("-0.0"_Sl);
                case PLUS_INF: return app.append("Infinity"_Sl);
                case MINUS_INF: return app.append("-Infinity"_Sl);
                default: return app.append("NaN"_Sl);
            }
        }

        gint Decimal64::toDecimal(gdouble v, Object& fd) {
            /*
             * For full details see references [2] and [1].
             *
             * For finite v != 0, determine integers c and q such that
             *     |v| = c 2^q    and
             *     Q_MIN <= q <= Q_MAX    and
             *         either    2^(P-1) <= c < 2^P                 (normal)
             *         or        0 < c < 2^(P-1)  and  q = Q_MIN    (subnormal)
             */
            glong bits = Double::toRawLongBits(v);
            glong t = bits & T_MASK;
            gint bq = (gint) ((bits | 0ULL) >> P - 1) & BQ_MASK;
            if (bq < BQ_MASK) {
                index = -1;
                if (bits < 0) {
                    /*
                     * fd != null implies bytes == null and bits >= 0
                     * Thus, when fd != null, control never reaches here.
                     */
                    append('-');
                }
                if (bq != 0) {
                    /* normal value. Here mq = -q */
                    gint mq = -Q_MIN + 1 - bq;
                    glong c = C_MIN | t;
                    /* The fast path discussed in section 8.3 of [1] */
                    if (0 < mq & mq < P) {
                        glong f = c >> mq;
                        if (f << mq == c) {
                            return toChars(f, 0, fd);
                        }
                    }
                    return toDecimal(-mq, c, 0, fd);
                }
                if (t != 0) {
                    /* subnormal value */
                    return t < C_TINY
                               ? toDecimal(Q_MIN, 10 * t, -1, fd)
                               : toDecimal(Q_MIN, t, 0, fd);
                }
                return bits == 0 ? PLUS_ZERO : MINUS_ZERO;
            }
            if (t != 0) {
                return NAN;
            }
            return bits > 0 ? PLUS_INF : MINUS_INF;
        }

        gint Decimal64::toDecimal(gint q, glong c, gint dk, Object& fd) {
            /*
             * The skeleton corresponds to figure 7 of [1].
             * The efficient computations are those summarized in figure 9.
             *
             * Here's a correspondence between Java names and names in [1],
             * expressed as approximate LaTeX source code and informally.
             * Other names are identical.
             * cb:     \bar{c}     "c-bar"
             * cbr:    \bar{c}_r   "c-bar-r"
             * cbl:    \bar{c}_l   "c-bar-l"
             *
             * vb:     \bar{v}     "v-bar"
             * vbr:    \bar{v}_r   "v-bar-r"
             * vbl:    \bar{v}_l   "v-bar-l"
             *
             * rop:    r_o'        "r-o-prime"
             */
            gint out = (gint) c & 0x1;
            glong cb = c << 2;
            glong cbr = cb + 2LL;
            glong cbl = 0;
            gint k = 0;
            /*
             * flog10pow2(e) = floor(log_10(2^e))
             * flog10threeQuartersPow2(e) = floor(log_10(3/4 2^e))
             * flog2pow10(e) = floor(log_2(10^e))
             */
            if (c != C_MIN | q == Q_MIN) {
                /* regular spacing */
                cbl = cb - 2;
                k = MathUtils::flog10pow2(q);
            } else {
                /* irregular spacing */
                cbl = cb - 1;
                k = MathUtils::flog10threeQuartersPow2(q);
            }
            gint h = q + MathUtils::flog2pow10(-k) + 2;

            /* g1 and g0 are as in section 9.8.3 of [1], so g = g1 2^63 + g0 */
            glong g1 = MathUtils::g1(k);
            glong g0 = MathUtils::g0(k);

            glong vb = rop(g1, g0, cb << h); // 49382715600000003
            glong vbl = rop(g1, g0, cbl << h); // 49382715599999999
            glong vbr = rop(g1, g0, cbr << h); // 49382715600000007

            glong s = vb >> 2; // 12345678900000000
            if (s >= 100) {
                /*
                 * For n = 17, m = 1 the table in section 10 of [1] shows
                 *     s' = floor(s / 10) = floor(s 115_292_150_460_684_698 / 2^60)
                 *        = floor(s 115_292_150_460_684_698 2^4 / 2^64)
                 *
                 * sp10 = 10 s'
                 * tp10 = 10 t'
                 * upin    iff    u' = sp10 10^k in Rv
                 * wpin    iff    w' = tp10 10^k in Rv
                 * See section 9.3 of [1].
                 */
                glong sp10 = 10 * Math::multiplyHigh(s, 115292150460684698LL << 4); // 12345678900000000
                glong tp10 = sp10 + 10; // 12345678900000010
                gbool upin = vbl + (glong) out <= sp10 << 2; // true
                gbool wpin = (tp10 << 2) + (glong) out <= vbr; // false
                if (upin != wpin) {
                    return toChars(upin ? sp10 : tp10, k, fd);
                }
            }

            /*
             * 10 <= s < 100    or    s >= 100  and  u', w' not in Rv
             * uin    iff    u = s 10^k in Rv
             * win    iff    w = t 10^k in Rv
             * See section 9.3 of [1].
             */
            glong t = s + 1;
            gbool uin = vbl + out <= s << 2;
            gbool win = (t << 2) + out <= vbr;
            if (uin != win) {
                /* Exactly one of u or w lies in Rv */
                return toChars(uin ? s : t, k + dk, fd);
            }
            /*
             * Both u and w lie in Rv: determine the one closest to v.
             * See section 9.3 of [1].
             */
            glong cmp = vb - (s + t << 1);
            return toChars(cmp < 0 || cmp == 0 && (s & 0x1) == 0 ? s : t, k + dk, fd);
        }

        glong Decimal64::rop(glong g1, glong g0, glong cp) {
            glong x1 = Math::multiplyHigh(g0, cp);
            glong y0 = g1 * cp;
            glong y1 = Math::multiplyHigh(g1, cp);
            glong z = ((y0 | 0ULL) >> 1) + x1;
            glong vbp = y1 + ((z | 0ULL) >> 63);
            return vbp | (((z & MASK_63) + MASK_63) | 0ULL) >> 63;
        }

        gint Decimal64::toChars(glong f, gint e, Object& fd) {
            /*
             * For details not discussed here see section 10 of [1].
             *
             * Determine len such that
             *     10^(len-1) <= f < 10^len
             */
            gint len = MathUtils::flog10pow2(Long::SIZE - Long::numberOfLeadingZeros(f)); // 16
            if (f >= MathUtils::powerOfTeen(len)) {
                len += 1; // 17
            }
            if (fd != null) {
                DecimalFormatter& df = CORE_XCAST(DecimalFormatter, fd);
                df.set(f, e, len);
                return NON_SPECIAL;
            }

            /*
             * Let fp and ep be the original f and e, respectively.
             * Transform f and e to ensure
             *     10^(H-1) <= f < 10^H
             *     fp 10^ep = f 10^(e-H) = 0.f 10^e
             */
            f *= MathUtils::powerOfTeen(H - len);
            e += len;

            /*
             * The toChars?() methods perform left-to-right digits extraction
             * using ints, provided that the arguments are limited to 8 digits.
             * Therefore, split the H = 17 digits of f into:
             *     h = the most significant digit of f
             *     m = the next 8 most significant digits of f
             *     l = the last 8, least significant digits of f
             *
             * For n = 17, m = 8 the table in section 10 of [1] shows
             *     floor(f / 10^8) = floor(193_428_131_138_340_668 f / 2^84) =
             *     floor(floor(193_428_131_138_340_668 f / 2^64) / 2^20)
             * and for n = 9, m = 8
             *     floor(hm / 10^8) = floor(1_441_151_881 hm / 2^57)
             */
            glong hm = (Math::multiplyHigh(f, 193428131138340668L) | 0ULL) >> 20;
            gint l = (gint) (f - 100000000L * hm);
            gint h = (gint) (((hm * 1441151881L) | 0ULL) >> 57);
            gint m = (gint) (hm - 100000000 * h);

            if (0 < e && e <= 7) {
                return toChars1(h, m, l, e);
            }
            if (-3 < e && e <= 0) {
                return toChars2(h, m, l, e);
            }
            return toChars3(h, m, l, e);
        }

        gint Decimal64::toChars1(gint h, gint m, gint l, gint e) {
            /*
             * 0 < e <= 7: plain format without leading zeroes.
             * Left-to-right digits extraction:
             * algorithm 1 in [3], with b = 10, k = 8, n = 28.
             */
            appendDigit(h);
            gint y = Decimal64::y(m);
            gint t;
            gint i = 1;
            for (; i < e; ++i) {
                t = 10 * y;
                appendDigit((t | 0ULL) >> 28);
                y = t & MASK_28;
            }
            append('.');
            for (; i <= 8; ++i) {
                t = 10 * y;
                appendDigit((t | 0ULL) >> 28);
                y = t & MASK_28;
            }
            lowDigits(l);
            return NON_SPECIAL;
        }

        gint Decimal64::toChars2(gint h, gint m, gint l, gint e) {
            /* -3 < e <= 0: plain format with leading zeroes */
            appendDigit(0);
            append('.');
            for (; e < 0; ++e) {
                appendDigit(0);
            }
            appendDigit(h);
            append8Digits(m);
            lowDigits(l);
            return NON_SPECIAL;
        }

        gint Decimal64::toChars3(gint h, gint m, gint l, gint e) {
            /* -3 >= e | e > 7: computerized scientific notation */
            appendDigit(h);
            append('.');
            append8Digits(m);
            lowDigits(l);
            exponent(e - 1);
            return NON_SPECIAL;
        }

        void Decimal64::lowDigits(gint l) {
            if (l != 0) {
                append8Digits(l);
            }
            removeTrailingZeroes();
        }

        void Decimal64::append8Digits(gint m) {
            /*
             * Left-to-right digits extraction:
             * algorithm 1 in [3], with b = 10, k = 8, n = 28.
             */
            gint y = Decimal64::y(m);
            for (gint i = 0; i < 8; ++i) {
                gint t = 10 * y;
                appendDigit((t | 0ULL) >> 28);
                y = t & MASK_28;
            }
        }

        void Decimal64::removeTrailingZeroes() {
            while (bytes[index] == '0') {
                --index;
            }
            /* ... but do not remove the one directly to the right of '.' */
            if (bytes[index] == '.') {
                ++index;
            }
        }

        gint Decimal64::y(gint a) {
            /*
             * Algorithm 1 in [3] needs computation of
             *     floor((a + 1) 2^n / b^k) - 1
             * with a < 10^8, b = 10, k = 8, n = 28.
             * Noting that
             *     (a + 1) 2^n <= 10^8 2^28 < 10^17
             * For n = 17, m = 8 the table in section 10 of [1] leads to:
             */
            return (gint) ((Math::multiplyHigh((glong) (a + 1) << 28, 193428131138340668L) | 0ULL) >> 20) - 1;
        }

        void Decimal64::exponent(gint e) {
            append('E');
            if (e < 0) {
                append('-');
                e = -e;
            }
            if (e < 10) {
                appendDigit(e);
                return;
            }
            gint d;
            if (e >= 100) {
                /*
                 * For n = 3, m = 2 the table in section 10 of [1] shows
                 *     floor(e / 100) = floor(1_311 e / 2^17)
                 */
                d = ((e * 1311) | 0ULL) >> 17;
                appendDigit(d);
                e -= 100 * d;
            }
            /*
             * For n = 2, m = 1 the table in section 10 of [1] shows
             *     floor(e / 10) = floor(103 e / 2^10)
             */
            d = ((e * 103) | 0ULL) >> 10;
            appendDigit(d);
            appendDigit(e - 10 * d);
        }

        void Decimal64::append(gint c) {
            bytes[++index] = (gbyte) c;
        }

        void Decimal64::appendDigit(gint d) {
            bytes[++index] = (gbyte) ('0' + d);
        }

        String Decimal64::charsToString() const {
            return String(bytes, 0, 0, index + 1);
        }
    } // ext
} // core
