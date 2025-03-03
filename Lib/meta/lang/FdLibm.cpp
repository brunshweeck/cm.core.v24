//
// Created by brunshweeck on 25 mai 2024.
//

#include "FdLibm.h"
#include <core/lang/Math.h>

namespace core {
    FdLibm::FdLibm() {
        //
    }

    gint FdLibm::__LO(gdouble x) {
        glong transducer = Double::toRawLongBits(x);
        return (gint) transducer;
    }

    gdouble FdLibm::__LO(gdouble x, gint low) {
        glong transX = Double::toRawLongBits(x);
        return Double::fromLongBits((transX & 0xFFFFFFFF00000000LL) | (low & 0x00000000FFFFFFFFLL));
    }

    gint FdLibm::__HI(gdouble x) {
        glong transducer = Double::toRawLongBits(x);
        return (gint) (transducer >> 32);
    }

    gdouble FdLibm::__HI(gdouble x, gint high) {
        glong transX = Double::toRawLongBits(x);
        return Double::fromLongBits((transX & 0x00000000FFFFFFFFL) | (((glong) high)) << 32);
    }

    FdLibm::Cbrt::Cbrt() {
        //
    }

    gdouble FdLibm::Cbrt::compute(gdouble x) {
        gdouble t = 0.0;
        gdouble sign;

        if (x == 0.0 || !Double::isFinite(x))
            return x; // Handles signed zeros properly

        sign = (x < 0.0) ? -1.0 : 1.0;

        x = Math::abs(x); // x <- |x|

        // Rough cbrt to 5 bits
        if (x < 0x1.0p-1022) {
            // subnormal number
            t = 0x1.0p54; // set t= 2**54
            t *= x;
            t = __HI(t, __HI(t) / 3 + B2);
        } else {
            gint hx = __HI(x); // high word of x
            t = __HI(t, hx / 3 + B1);
        }

        // New cbrt to 23 bits, may be implemented in single precision
        gdouble r, s, w;
        r = t * t / x;
        s = C + r * t;
        t *= G + F / (s + E + D / s);

        // Chopped to 20 bits and make it larger than cbrt(x)
        t = __LO(t, 0);
        t = __HI(t, __HI(t) + 0x00000001);

        // One-step newton iteration to 53 bits with error less than 0.667 ulps
        s = t * t; // t*t is exact
        r = x / s;
        w = t + t;
        r = (r - t) / (w + r); // r-s is exact
        t = t + t * r;

        // Restore the original sign bit
        return sign * t;
    }

    FdLibm::Hypot::Hypot() {
        //
    }

    gdouble FdLibm::Hypot::compute(gdouble x, gdouble y) {
        gdouble a = Math::abs(x);
        gdouble b = Math::abs(y);

        if (!Double::isFinite(a) || !Double::isFinite(b)) {
            if (a == INF || b == INF)
                return INF;
            else
                return a + b; // Propagate NaN significand bits
        }

        if (b > a) {
            gdouble tmp = a;
            a = b;
            b = tmp;
        }

        // assert a >= b;

        // Doing bitwise conversion after screening for NaN allows
        // the code to not worry about the possibility of
        // "negative" NaN values.

        // Note: the ha and hb variables are the high-order
        // 32-bits of a and b stored as integer values. The ha and
        // hb values are used first for a rough magnitude
        // comparison of a and b and second for simulating higher
        // precision by allowing a and b, respectively, to be
        // decomposed into non-overlapping portions. Both of these
        // uses could be eliminated. The magnitude comparison
        // could be eliminated by extracting and comparing the
        // exponents of a and b or just be performing a
        // floating-point divide.  Splitting a floating-point
        // number into non-overlapping portions can be
        // accomplished by judicious use of multiplies and
        // additions. For details see T. J. Dekker, A Floating-Point
        // Technique for Extending the Available Precision,
        // Numerische Mathematik, vol. 18, 1971, pp.224-242 and
        // subsequent work.

        gint ha = __HI(a); // high word of a
        gint hb = __HI(b); // high word of b

        if ((ha - hb) > 0x3c00000) {
            return a + b; // x / y > 2**60
        }

        gint k = 0;
        if (a > 0x1.00000FFFFFFFFp500) {
            // a > ~2**500
            // scale a and b by 2**-600
            ha -= 0x25800000;
            hb -= 0x25800000;
            a = a * TWO_MINUS_600;
            b = b * TWO_MINUS_600;
            k += 600;
        }
        gdouble t1, t2;
        if (b < 0x1.0p-500) {
            // b < 2**-500
            if (b < Double::MIN_NORMAL) {
                // subnormal b or 0 */
                if (b == 0.0)
                    return a;
                t1 = 0x1.0p1022; // t1 = 2^1022
                b *= t1;
                a *= t1;
                k -= 1022;
            } else {
                // scale a and b by 2^600
                ha += 0x25800000; // a *= 2^600
                hb += 0x25800000; // b *= 2^600
                a = a * TWO_PLUS_600;
                b = b * TWO_PLUS_600;
                k -= 600;
            }
        }
        // medium size a and b
        gdouble w = a - b;
        if (w > b) {
            t1 = 0;
            t1 = __HI(t1, ha);
            t2 = a - t1;
            w = Math::sqrt(t1 * t1 - (b * (-b) - t2 * (a + t1)));
        } else {
            gdouble y1, y2;
            a = a + a;
            y1 = 0;
            y1 = __HI(y1, hb);
            y2 = b - y1;
            t1 = 0;
            t1 = __HI(t1, ha + 0x00100000);
            t2 = a - t1;
            w = Math::sqrt(t1 * y1 - (w * (-w) - (t1 * y2 + t2 * b)));
        }
        if (k != 0) {
            return Math::powerOfTwoD(k) * w;
        } else
            return w;
    }

    FdLibm::Pow::Pow() {
    }

    gdouble FdLibm::Pow::compute(gdouble x, gdouble y) {
        gdouble z;
        gdouble r, s, t, u, v, w;
        gint i, j, k, n;

        // y == zero: x**0 = 1
        if (y == 0.0)
            return 1.0;

        // +/-NaN return x + y to propagate NaN significands
        if (Double::isNaN(x) || Double::isNaN(y))
            return x + y;

        gdouble const y_abs = Math::abs(y);
        gdouble x_abs = Math::abs(x);
        // Special values of y
        if (y == 2.0) {
            return x * x;
        } else if (y == 0.5) {
            if (x >= -Double::MAX_VALUE) // Handle x == -infinity later
                return Math::sqrt(x + 0.0); // Add 0.0 to properly handle x == -0.0
        } else if (y_abs == 1.0) {
            // y is  +/-1
            return (y == 1.0) ? x : 1.0 / x;
        } else if (y_abs == INF) {
            // y is +/-infinity
            if (x_abs == 1.0)
                return y - y; // inf**+/-1 is NaN
            else if (x_abs > 1.0) // (|x| > 1)**+/-inf = inf, 0
                return (y >= 0) ? y : 0.0;
            else // (|x| < 1)**-/+inf = inf, 0
                return (y < 0) ? -y : 0.0;
        }

        gint const hx = __HI(x);
        gint ix = hx & 0x7fffffff;

        /*
         * When x < 0, determine if y is an odd integer:
         * y_is_int = 0       ... y is not an integer
         * y_is_int = 1       ... y is an odd gint
         * y_is_int = 2       ... y is an even gint
         */
        gint y_is_int = 0;
        if (hx < 0) {
            if (y_abs >= 0x1.0p53) // |y| >= 2^53 = 9.007199254740992E15
                y_is_int = 2; // y is an even integer since ulp(2^53) = 2.0
            else if (y_abs >= 1.0) {
                // |y| >= 1.0
                glong y_abs_as_long = (glong) y_abs;
                if (((gdouble) y_abs_as_long) == y_abs) {
                    y_is_int = 2 - (gint) (y_abs_as_long & 0x1L);
                }
            }
        }

        // Special value of x
        if (x_abs == 0.0 ||
            x_abs == INF ||
            x_abs == 1.0) {
            z = x_abs; // x is +/-0, +/-inf, +/-1
            if (y < 0.0)
                z = 1.0 / z; // z = (1/|x|)
            if (hx < 0) {
                if (((ix - 0x3ff00000) | y_is_int) == 0) {
                    z = (z - z) / (z - z); // (-1)**non-gint is NaN
                } else if (y_is_int == 1)
                    z = -1.0 * z; // (x < 0)**odd = -(|x|**odd)
            }
            return z;
        }

        n = (hx >> 31) + 1;

        // (x < 0)**(non-gint) is NaN
        if ((n | y_is_int) == 0)
            return (x - x) / (x - x);

        s = 1.0; // s (sign of result -ve**odd) = -1 else = 1
        if ((n | (y_is_int - 1)) == 0)
            s = -1.0; // (-ve)**(odd gint)

        gdouble p_h, p_l, t1, t2;
        // |y| is huge
        if (y_abs > 0x1.00000ffffffffp31) {
            // if |y| > ~2**31
            static gdouble const INV_LN2 = 0x1.71547652b82fep0; //  1.44269504088896338700e+00 = 1/ln2
            static gdouble const INV_LN2_H = 0x1.715476p0; //  1.44269502162933349609e+00 = 24 bits of 1/ln2
            static gdouble const INV_LN2_L = 0x1.4ae0bf85ddf44p-26; //  1.92596299112661746887e-08 = 1/ln2 tail

            // Over/underflow if x is not close to one
            if (x_abs < 0x1.fffff00000000p-1) // |x| < ~0.9999995231628418
                return (y < 0.0) ? s * INF : s * 0.0;
            if (x_abs > 0x1.00000ffffffffp0) // |x| > ~1.0
                return (y > 0.0) ? s * INF : s * 0.0;
            /*
             * now |1-x| is tiny <= 2**-20, sufficient to compute
             * log(x) by x - x^2/2 + x^3/3 - x^4/4
             */
            t = x_abs - 1.0; // t has 20 trailing zeros
            w = (t * t) * (0.5 - t * (0.3333333333333333333333 - t * 0.25));
            u = INV_LN2_H * t; // INV_LN2_H has 21 sig. bits
            v = t * INV_LN2_L - w * INV_LN2;
            t1 = u + v;
            t1 = __LO(t1, 0);
            t2 = v - (t1 - u);
        } else {
            static gdouble const CP = 0x1.ec709dc3a03fdp-1; //  9.61796693925975554329e-01 = 2/(3ln2)
            static gdouble const CP_H = 0x1.ec709ep-1; //  9.61796700954437255859e-01 = (gfloat)cp
            static gdouble const CP_L = -0x1.e2fe0145b01f5p-28; // -7.02846165095275826516e-09 = tail of CP_H

            gdouble z_h, z_l, ss, s2, s_h, s_l, t_h, t_l;
            n = 0;
            // Take care of subnormal numbers
            if (ix < 0x00100000) {
                x_abs *= 0x1.0p53; // 2^53 = 9007199254740992.0
                n -= 53;
                ix = __HI(x_abs);
            }
            n += ((ix) >> 20) - 0x3ff;
            j = ix & 0x000fffff;
            // Determine interval
            ix = j | 0x3ff00000; // Normalize ix
            if (j <= 0x3988E)
                k = 0; // |x| <sqrt(3/2)
            else if (j < 0xBB67A)
                k = 1; // |x| <sqrt(3)
            else {
                k = 0;
                n += 1;
                ix -= 0x00100000;
            }
            x_abs = __HI(x_abs, ix);

            // Compute ss = s_h + s_l = (x-1)/(x+1) or (x-1.5)/(x+1.5)

            static gdouble const BP[] = {
                1.0,
                1.5
            };
            static gdouble const DP_H[] = {
                0.0,
                0x1.2b8034p-1
            }; // 5.84962487220764160156e-01
            static gdouble const DP_L[] = {
                0.0,
                0x1.cfdeb43cfd006p-27
            }; // 1.35003920212974897128e-08

            // Poly coefs for (3/2)*(log(x)-2s-2/3*s**3
            static gdouble const L1 = 0x1.3333333333303p-1; //  5.99999999999994648725e-01
            static gdouble const L2 = 0x1.b6db6db6fabffp-2; //  4.28571428578550184252e-01
            static gdouble const L3 = 0x1.55555518f264dp-2; //  3.33333329818377432918e-01
            static gdouble const L4 = 0x1.17460a91d4101p-2; //  2.72728123808534006489e-01
            static gdouble const L5 = 0x1.d864a93c9db65p-3; //  2.30660745775561754067e-01
            static gdouble const L6 = 0x1.a7e284a454eefp-3; //  2.06975017800338417784e-01
            u = x_abs - BP[k]; // BP[0]=1.0, BP[1]=1.5
            v = 1.0 / (x_abs + BP[k]);
            ss = u * v;
            s_h = ss;
            s_h = __LO(s_h, 0);
            // t_h=x_abs + BP[k] High
            t_h = 0.0;
            t_h = __HI(t_h, ((ix >> 1) | 0x20000000) + 0x00080000 + (k << 18));
            t_l = x_abs - (t_h - BP[k]);
            s_l = v * ((u - s_h * t_h) - s_h * t_l);
            // Compute log(x_abs)
            s2 = ss * ss;
            r = s2 * s2 * (L1 + s2 * (L2 + s2 * (L3 + s2 * (L4 + s2 * (L5 + s2 * L6)))));
            r += s_l * (s_h + ss);
            s2 = s_h * s_h;
            t_h = 3.0 + s2 + r;
            t_h = __LO(t_h, 0);
            t_l = r - ((t_h - 3.0) - s2);
            // u+v = ss*(1+...)
            u = s_h * t_h;
            v = s_l * t_h + t_l * ss;
            // 2/(3log2)*(ss + ...)
            p_h = u + v;
            p_h = __LO(p_h, 0);
            p_l = v - (p_h - u);
            z_h = CP_H * p_h; // CP_H + CP_L = 2/(3*log2)
            z_l = CP_L * p_h + p_l * CP + DP_L[k];
            // log2(x_abs) = (ss + ..)*2/(3*log2) = n + DP_H + z_h + z_l
            t = (gdouble) n;
            t1 = (((z_h + z_l) + DP_H[k]) + t);
            t1 = __LO(t1, 0);
            t2 = z_l - (((t1 - t) - DP_H[k]) - z_h);
        }

        // Split up y into (y1 + y2) and compute (y1 + y2) * (t1 + t2)
        gdouble y1 = y;
        y1 = __LO(y1, 0);
        p_l = (y - y1) * t1 + y * t2;
        p_h = y1 * t1;
        z = p_l + p_h;
        j = __HI(z);
        i = __LO(z);
        if (j >= 0x40900000) {
            // z >= 1024
            if (((j - 0x40900000) | i) != 0) // if z > 1024
                return s * INF; // Overflow
            else {
                static gdouble const OVT = 8.0085662595372944372e-0017; // -(1024-log2(ovfl+.5ulp))
                if (p_l + OVT > z - p_h)
                    return s * INF; // Overflow
            }
        } else if ((j & 0x7fffffff) >= 0x4090cc00) {
            // z <= -1075
            if (((j - 0xc090cc00) | i) != 0) // z < -1075
                return s * 0.0; // Underflow
            else {
                if (p_l <= z - p_h)
                    return s * 0.0; // Underflow
            }
        }
        /*
         * Compute 2**(p_h+p_l)
         */
        // Poly coefs for (3/2)*(log(x)-2s-2/3*s**3
        static gdouble const P1 = 0x1.555555555553ep-3; //  1.66666666666666019037e-01
        static gdouble const P2 = -0x1.6c16c16bebd93p-9; // -2.77777777770155933842e-03
        static gdouble const P3 = 0x1.1566aaf25de2cp-14; //  6.61375632143793436117e-05
        static gdouble const P4 = -0x1.bbd41c5d26bf1p-20; // -1.65339022054652515390e-06
        static gdouble const P5 = 0x1.6376972bea4d0p-25; //  4.13813679705723846039e-08
        static gdouble const LG2 = 0x1.62e42fefa39efp-1; //  6.93147180559945286227e-01
        static gdouble const LG2_H = 0x1.62e43p-1; //  6.93147182464599609375e-01
        static gdouble const LG2_L = -0x1.05c610ca86c39p-29; // -1.90465429995776804525e-09
        i = j & 0x7fffffff;
        k = (i >> 20) - 0x3ff;
        n = 0;
        if (i > 0x3fe00000) {
            // if |z| > 0.5, set n = [z + 0.5]
            n = j + (0x00100000 >> (k + 1));
            k = ((n & 0x7fffffff) >> 20) - 0x3ff; // new k for n
            t = 0.0;
            t = __HI(t, (n & ~(0x000fffff >> k)));
            n = ((n & 0x000fffff) | 0x00100000) >> (20 - k);
            if (j < 0)
                n = -n;
            p_h -= t;
        }
        t = p_l + p_h;
        t = __LO(t, 0);
        u = t * LG2_H;
        v = (p_l - (t - p_h)) * LG2 + t * LG2_L;
        z = u + v;
        w = v - (z - u);
        t = z * z;
        t1 = z - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
        r = (z * t1) / (t1 - 2.0) - (w + z * w);
        z = 1.0 - (r - z);
        j = __HI(z);
        j += (n << 20);
        if ((j >> 20) <= 0)
            z = Math::scalb(z, n); // subnormal output
        else {
            gint z_hi = __HI(z);
            z_hi += (n << 20);
            z = __HI(z, z_hi);
        }
        return s * z;
    }

    static CORE_FAST gdouble half[] = {0.5, -0.5,};
    static CORE_FAST gdouble ln2HI[] = {
        0x1.62e42feep-1, //  6.93147180369123816490e-01
        -0x1.62e42feep-1
    }; // -6.93147180369123816490e-01
    static CORE_FAST gdouble ln2LO[] = {
        0x1.a39ef35793c76p-33, //  1.90821492927058770002e-10
        -0x1.a39ef35793c76p-33
    }; // -1.90821492927058770002e-10

    FdLibm::Exp::Exp() {
    }

    gdouble FdLibm::Exp::compute(gdouble x) {
        gdouble y;
        gdouble hi = 0.0;
        gdouble lo = 0.0;
        gdouble c;
        gdouble t;
        gint k = 0;
        gint xsb;
        /*unsigned*/
        gint hx;

        hx = __HI(x); /* high word of x */
        xsb = (hx >> 31) & 1; /* sign bit of x */
        hx &= 0x7fffffff; /* high word of |x| */

        /* filter out non-finite argument */
        if (hx >= 0x40862E42) {
            /* if |x| >= 709.78... */
            if (hx >= 0x7ff00000) {
                if (((hx & 0xfffff) | __LO(x)) != 0)
                    return x + x; /* NaN */
                else
                    return (xsb == 0) ? x : 0.0; /* exp(+-inf) = {inf, 0} */
            }
            if (x > o_threshold)
                return huge * huge; /* overflow */
            if (x < u_threshold) // unsigned compare needed here?
                return twom1000 * twom1000; /* underflow */
        }

        /* argument reduction */
        if (hx > 0x3fd62e42) {
            /* if  |x| > 0.5 ln2 */
            if (hx < 0x3FF0A2B2) {
                /* and |x| < 1.5 ln2 */
                hi = x - ln2HI[xsb];
                lo = ln2LO[xsb];
                k = 1 - xsb - xsb;
            } else {
                k = (gint) (invln2 * x + half[xsb]);
                t = k;
                hi = x - t * ln2HI[0]; /* t*ln2HI is exact here */
                lo = t * ln2LO[0];
            }
            x = hi - lo;
        } else if (hx < 0x3e300000) {
            /* when |x|<2**-28 */
            if (huge + x > one)
                return one + x; /* trigger inexact */
        } else {
            k = 0;
        }

        /* x is now in primary range */
        t = x * x;
        c = x - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));
        if (k == 0)
            return one - ((x * c) / (c - 2.0) - x);
        else
            y = one - ((lo - (x * c) / (2.0 - c)) - hi);

        if (k >= -1021) {
            y = __HI(y, __HI(y) + (k << 20)); /* add k to y's exponent */
            return y;
        } else {
            y = __HI(y, __HI(y) + ((k + 1000) << 20)); /* add k to y's exponent */
            return y * twom1000;
        }
    }
} // core
