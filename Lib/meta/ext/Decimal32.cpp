//
// Created by admin on 04/12/24.
//

#include "Decimal32.h"

#include <core/lang/Math.h>
#include <core/lang/XString.h>
#include <meta/lang/MathUtils.h>

namespace core {
  namespace ext {
    String Decimal32::toString(gfloat v) {
      return Decimal32().toDecimalString(v);
    }

    Appendable& Decimal32::appendTo(gfloat v, Appendable& app) {
      return Decimal32().appendDecimalTo(v, app);
    }

    String Decimal32::toDecimalString(gfloat v) {
      switch (toDecimal(v)) {
        case NON_SPECIAL: return charsToString();
        case PLUS_ZERO: return "0.0";
        case MINUS_ZERO: return "-0.0";
        case PLUS_INF: return "Infinity";
        case MINUS_INF: return "-Infinity";
        default: return "NaN";
      };
    }

    Appendable& Decimal32::appendDecimalTo(gfloat v, Appendable& app) {
      switch (toDecimal(v)) {
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

    gint Decimal32::toDecimal(gfloat v) {
      /*
   * For full details see references [2] and [1].
   *
   * For finite v != 0, determine integers c and q such that
   *     |v| = c 2^q    and
   *     Q_MIN <= q <= Q_MAX    and
   *         either    2^(P-1) <= c < 2^P                 (normal)
   *         or        0 < c < 2^(P-1)  and  q = Q_MIN    (subnormal)
   */
      gint bits = Float::toRawIntBits(v);
      gint t = bits & T_MASK;
      gint bq = ((bits | 0U) >> P - 1) & BQ_MASK;
      if (bq < BQ_MASK) {
        index = -1;
        if (bits < 0) {
          append('-');
        }
        if (bq != 0) {
          /* normal value. Here mq = -q */
          gint mq = -Q_MIN + 1 - bq;
          gint c = C_MIN | t;
          /* The fast path discussed in section 8.3 of [1] */
          if (0 < mq & mq < P) {
            gint f = c >> mq;
            if (f << mq == c) {
              return toChars(f, 0);
            }
          }
          return toDecimal(-mq, c, 0);
        }
        if (t != 0) {
          /* subnormal value */
          return t < C_TINY
                   ? toDecimal(Q_MIN, 10 * t, -1)
                   : toDecimal(Q_MIN, t, 0);
        }
        return bits == 0 ? PLUS_ZERO : MINUS_ZERO;
      }
      if (t != 0) {
        return NAN;
      }
      return bits > 0 ? PLUS_INF : MINUS_INF;
    }

    gint Decimal32::toDecimal(gint q, gint c, gint dk) {
      /*
   * The skeleton corresponds to figure 7 of [1].
   * The efficient computations are those summarized in figure 9.
   * Also check the appendix.
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
      gint out = c & 0x1;
      glong cb = c << 2;
      glong cbr = cb + 2;
      glong cbl;
      gint k;
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
      gint h = q + MathUtils::flog2pow10(-k) + 33;

      /* g is as in the appendix */
      glong g = MathUtils::g1(k) + 1;

      gint vb = rop(g, cb << h);
      gint vbl = rop(g, cbl << h);
      gint vbr = rop(g, cbr << h);

      gint s = vb >> 2;
      if (s >= 100) {
        /*
     * For n = 9, m = 1 the table in section 10 of [1] shows
     *     s' = floor(s / 10) = floor(s 1_717_986_919 / 2^34)
     *
     * sp10 = 10 s'
     * tp10 = 10 t'
     * upin    iff    u' = sp10 10^k in Rv
     * wpin    iff    w' = tp10 10^k in Rv
     * See section 9.3 of [1].
     */
        gint sp10 = 10 * (gint) (s * 1717986919L >> 34);
        gint tp10 = sp10 + 10;
        gbool upin = vbl + out <= sp10 << 2;
        gbool wpin = (tp10 << 2) + out <= vbr;
        if (upin != wpin) {
          return toChars(upin ? sp10 : tp10, k);
        }
      }

      /*
   * 10 <= s < 100    or    s >= 100  and  u', w' not in Rv
   * uin    iff    u = s 10^k in Rv
   * win    iff    w = t 10^k in Rv
   * See section 9.3 of [1].
   */
      gint t = s + 1;
      gbool uin = vbl + out <= s << 2;
      gbool win = (t << 2) + out <= vbr;
      if (uin != win) {
        /* Exactly one of u or w lies in Rv */
        return toChars(uin ? s : t, k + dk);
      }
      /*
   * Both u and w lie in Rv: determine the one closest to v.
   * See section 9.3 of [1].
   */
      gint cmp = vb - (s + t << 1);
      return toChars(cmp < 0 || cmp == 0 && (s & 0x1) == 0 ? s : t, k + dk);
    }

    gint Decimal32::rop(glong g, glong cp) {
      glong x1 = Math::multiplyHigh(g, cp);
      glong vbp = x1 >> 31;
      return (gint) (vbp | (x1 & MASK_32) + MASK_32 >> 32);
    }

    gint Decimal32::toChars(gint f, gint e) {
      /*
   * For details not discussed here see section 10 of [1].
   *
   * Determine len such that
   *     10^(len-1) <= f < 10^len
   */
      gint len = MathUtils::flog10pow2(Integer::SIZE - Integer::numberOfLeadingZeros(f));
      if (f >= MathUtils::powerOfTeen(len)) {
        len += 1;
      }

      /*
   * Let fp and ep be the original f and e, respectively.
   * Transform f and e to ensure
   *     10^(H-1) <= f < 10^H
   *     fp 10^ep = f 10^(e-H) = 0.f 10^e
   */
      f *= (gint) MathUtils::powerOfTeen(H - len);
      e += len;

      /*
   * The toChars?() methods perform left-to-right digits extraction
   * using ints, provided that the arguments are limited to 8 digits.
   * Therefore, split the H = 9 digits of f into:
   *     h = the most significant digit of f
   *     l = the last 8, least significant digits of f
   *
   * For n = 9, m = 8 the table in section 10 of [1] shows
   *     floor(f / 10^8) = floor(1_441_151_881 f / 2^57)
   */
      gint h = (gint) (f * 1441151881L >> 57);
      gint l = f - 100000000 * h;

      if (0 < e && e <= 7) {
        return toChars1(h, l, e);
      }
      if (-3 < e && e <= 0) {
        return toChars2(h, l, e);
      }
      return toChars3(h, l, e);
    }

    gint Decimal32::toChars1(gint h, gint l, gint e) {
      /*
   * 0 < e <= 7: plain format without leading zeroes.
   * Left-to-right digits extraction:
   * algorithm 1 in [3], with b = 10, k = 8, n = 28.
   */
      appendDigit(h);
      gint y = Decimal32::y(l);
      gint t;
      gint i = 1;
      for (; i < e; ++i) {
        t = 10 * y;
        appendDigit((t | 0U) >> 28);
        y = t & MASK_28;
      }
      append('.');
      for (; i <= 8; ++i) {
        t = 10 * y;
        appendDigit(t >> 28);
        y = t & MASK_28;
      }
      removeTrailingZeroes();
      return NON_SPECIAL;
    }

    gint Decimal32::toChars2(gint h, gint l, gint e) {
      /* -3 < e <= 0: plain format with leading zeroes */
      appendDigit(0);
      append('.');
      for (; e < 0; ++e) {
        appendDigit(0);
      }
      appendDigit(h);
      append8Digits(l);
      removeTrailingZeroes();
      return NON_SPECIAL;
    }

    gint Decimal32::toChars3(gint h, gint l, gint e) {
      /* -3 >= e | e > 7: computerized scientific notation */
      appendDigit(h);
      append('.');
      append8Digits(l);
      removeTrailingZeroes();
      exponent(e - 1);
      return NON_SPECIAL;
    }

    void Decimal32::append8Digits(gint m) {
      /*
   * Left-to-right digits extraction:
   * algorithm 1 in [3], with b = 10, k = 8, n = 28.
   */
      gint y = Decimal32::y(m);
      for (gint i = 0; i < 8; ++i) {
        gint t = 10 * y;
        appendDigit((t | 0U) >> 28);
        y = t & MASK_28;
      }
    }

    void Decimal32::removeTrailingZeroes() {
      while (bytes[index] == '0') {
        --index;
      }
      /* ... but do not remove the one directly to the right of '.' */
      if (bytes[index] == '.') {
        ++index;
      }
    }

    gint Decimal32::y(gint a) {
      /*
   * Algorithm 1 in [3] needs computation of
   *     floor((a + 1) 2^n / b^k) - 1
   * with a < 10^8, b = 10, k = 8, n = 28.
   * Noting that
   *     (a + 1) 2^n <= 10^8 2^28 < 10^17
   * For n = 17, m = 8 the table in section 10 of [1] leads to:
   */
      return (gint) (Math::multiplyHigh(
        (glong) (a + 1) << 28, 193428131138340668L) >> 20) - 1;
    }

    void Decimal32::exponent(gint e) {
      append('E');
      if (e < 0) {
        append('-');
        e = -e;
      }
      if (e < 10) {
        appendDigit(e);
        return;
      }
      /*
   * For n = 2, m = 1 the table in section 10 of [1] shows
   *     floor(e / 10) = floor(103 e / 2^10)
   */
      gint d = e * 103 >> 10;
      appendDigit(d);
      appendDigit(e - 10 * d);
    }

    void Decimal32::append(gint c) {
      bytes[++index] = (gbyte) c;
    }

    void Decimal32::appendDigit(gint d) {
      bytes[++index] = (gbyte) ('0' + d);
    }

    String Decimal32::charsToString() const {
      return String(bytes, 0, 0, index + 1);
    }
  } // ext
} // core
