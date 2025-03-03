//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_FDLIBM_H
#define CORE24_FDLIBM_H

#include <core/lang/Double.h>

namespace core {
    /**
     * Port of the "Freely Distributable Math Library", version 5.3, from C.
     *
     * <p>The C version of fdlibm relied on the idiom of pointer aliasing
     * a 64-bit double floating-point value as a two-element array of
     * 32-bit integers and reading and writing the two halves of the
     * double independently. This coding pattern was problematic to C
     * optimizers and not directly expressible in Java. Therefore, rather
     * than a memory level overlay, if portions of a double need to be
     * operated on as integer values, the standard library methods for
     * bitwise floating-point to integer conversion,
     * Double.longBitsToDouble and Double.doubleToRawLongBits, are directly
     * or indirectly used.
     *
     * <p>The C version of fdlibm also took some pains to signal the
     * correct IEEE 754 exceptional conditions divide by zero, invalid,
     * overflow and underflow. For example, overflow would be signaled by
     * @c huge*huge where @c huge was a large constant that
     * would overflow when squared. Since IEEE floating-point exceptional
     * handling is not supported natively in the JVM, such coding patterns
     * have been omitted from this port. For example, rather than
     * @code return huge*huge @endcode, this port will use @code return Math::INF @endcode.
     *
     * <p>Various comparison and arithmetic operations in fdlibm could be
     * done either based on the integer view of a value or directly on the
     * floating-point representation. Which idiom is faster may depend on
     * platform specific factors. However, for code clarity if no other
     * reason, this port will favor expressing the semantics of those
     * operations in terms of floating-point operations when convenient to
     * do so.
     */
    class FdLibm final : public Object {
        CORE_EXPLICIT FdLibm();

        static CORE_FAST gdouble INF = Double::POSITIVE_INFINITY;

        /**
         * Return the low-order 32 bits of the double argument as an int.
         */
        static gint __LO(gdouble x);

        /**
         * Return a double with its low-order bits of the second argument
         * and the high-order bits of the first argument..
         */
        static gdouble __LO(gdouble x, gint low);

        /**
         * Return the high-order 32 bits of the double argument as an int.
         */
        static gint __HI(gdouble x);

        /**
         * Return a double with its high-order bits of the second argument
         * and the low-order bits of the first argument..
         */
        static gdouble __HI(gdouble x, gint high);

    public:
        /**
         * cbrt(x)
         * Return cube root of x
         */
        class Cbrt final : public Object {
            static CORE_FAST gint B1 = 715094163; /* B1 = (682-0.03306235651)*2**20 */
            static CORE_FAST gint B2 = 696219795; /* B2 = (664-0.03306235651)*2**20 */

            static CORE_FAST gdouble C = 0x1.15f15f15f15f1p-1; //   19/35   ~= 5.42857142857142815906e-01
            static CORE_FAST gdouble D = -0x1.691de2532c834p-1; // -864/1225 ~= 7.05306122448979611050e-01
            static CORE_FAST gdouble E = 0x1.6a0ea0ea0ea0fp0; //   99/70   ~= 1.41428571428571436819e+00
            static CORE_FAST gdouble F = 0x1.9b6db6db6db6ep0; //   45/28   ~= 1.60714285714285720630e+00
            static CORE_FAST gdouble G = 0x1.6db6db6db6db7p-2; //    5/14   ~= 3.57142857142857150787e-01

            CORE_EXPLICIT Cbrt();

        public:
            static gdouble compute(gdouble x);
        };

        /*
         * hypot(x,y)
         *
         * Method :
         *      If (assume round-to-nearest) z = x*x + y*y
         *      has error less than sqrt(2)/2 ulp, than
         *      sqrt(z) has error less than 1 ulp (exercise).
         *
         *      So, compute sqrt(x*x + y*y) with some care as
         *      follows to get the error below 1 ulp:
         *
         *      Assume x > y > 0;
         *      (if possible, set rounding to round-to-nearest)
         *      1. if x > 2y  use
         *              x1*x1 + (y*y + (x2*(x + x1))) for x*x + y*y
         *      where x1 = x with lower 32 bits cleared, x2 = x - x1; else
         *      2. if x <= 2y use
         *              t1*y1 + ((x-y) * (x-y) + (t1*y2 + t2*y))
         *      where t1 = 2x with lower 32 bits cleared, t2 = 2x - t1,
         *      y1= y with lower 32 bits chopped, y2 = y - y1.
         *
         *      NOTE: scaling may be necessary if some argument is too
         *            large or too tiny
         *
         * Special cases:
         *      hypot(x,y) is INF if x or y is +INF or -INF; else
         *      hypot(x,y) is NAN if x or y is NAN.
         *
         * Accuracy:
         *      hypot(x,y) returns sqrt(x^2 + y^2) with error less
         *      than 1 ulp (unit in the last place)
         */
        class Hypot final : public Object {
        public :
            static CORE_FAST gdouble TWO_MINUS_600 = 0x1.0p-600;
            static CORE_FAST gdouble TWO_PLUS_600 = 0x1.0p+600;

        private:
            CORE_EXPLICIT Hypot();

        public:
            static gdouble compute(gdouble x, gdouble y);
        };

        /**
         * Compute x**y
         *                    n
         * Method:  Let x =  2   * (1+f)
         *      1. Compute and return log2(x) in two pieces:
         *              log2(x) = w1 + w2,
         *         where w1 has 53 - 24 = 29 bit trailing zeros.
         *      2. Perform y*log2(x) = n+y' by simulating multi-precision
         *         arithmetic, where |y'| <= 0.5.
         *      3. Return x**y = 2**n*exp(y'*log2)
         *
         * Special cases:
         *      1.  (anything) ** 0  is 1
         *      2.  (anything) ** 1  is itself
         *      3.  (anything) ** NAN is NAN
         *      4.  NAN ** (anything except 0) is NAN
         *      5.  +-(|x| > 1) **  +INF is +INF
         *      6.  +-(|x| > 1) **  -INF is +0
         *      7.  +-(|x| < 1) **  +INF is +0
         *      8.  +-(|x| < 1) **  -INF is +INF
         *      9.  +-1         ** +-INF is NAN
         *      10. +0 ** (+anything except 0, NAN)               is +0
         *      11. -0 ** (+anything except 0, NAN, odd integer)  is +0
         *      12. +0 ** (-anything except 0, NAN)               is +INF
         *      13. -0 ** (-anything except 0, NAN, odd integer)  is +INF
         *      14. -0 ** (odd integer) = -( +0 ** (odd integer) )
         *      15. +INF ** (+anything except 0,NAN) is +INF
         *      16. +INF ** (-anything except 0,NAN) is +0
         *      17. -INF ** (anything)  = -0 ** (-anything)
         *      18. (-anything) ** (integer) is (-1)**(integer)*(+anything**integer)
         *      19. (-anything except 0 and inf) ** (non-integer) is NAN
         *
         * Accuracy:
         *      pow(x,y) returns x**y nearly rounded. In particular
         *                      pow(integer,integer)
         *      always returns the correct integer provided it is
         *      representable.
         */
        class Pow final : public Object {
            CORE_EXPLICIT Pow();

        public:
            static gdouble compute(gdouble x, gdouble y);
        };

        /**
         * Returns the exponential of x.
         *
         * Method
         *   1. Argument reduction:
         *      Reduce x to an r so that |r| <= 0.5*ln2 ~ 0.34658.
         *      Given x, find r and integer k such that
         *
         *               x = k*ln2 + r,  |r| <= 0.5*ln2.
         *
         *      Here r will be represented as r = hi-lo for better
         *      accuracy.
         *
         *   2. Approximation of exp(r) by a special rational function on
         *      the interval [0,0.34658]:
         *      Write
         *          R(r**2) = r*(exp(r)+1)/(exp(r)-1) = 2 + r*r/6 - r**4/360 + ...
         *      We use a special Reme algorithm on [0,0.34658] to generate
         *      a polynomial of degree 5 to approximate R. The maximum error
         *      of this polynomial approximation is bounded by 2**-59. In
         *      other words,
         *          R(z) ~ 2.0 + P1*z + P2*z**2 + P3*z**3 + P4*z**4 + P5*z**5
         *      (where z=r*r, and the values of P1 to P5 are listed below)
         *      and
         *          |                  5          |     -59
         *          | 2.0+P1*z+...+P5*z   -  R(z) | <= 2
         *          |                             |
         *      The computation of exp(r) thus becomes
         *                             2*r
         *              exp(r) = 1 + -------
         *                            R - r
         *                                 r*R1(r)
         *                     = 1 + r + ----------- (for better accuracy)
         *                                2 - R1(r)
         *      where
         *                               2       4             10
         *              R1(r) = r - (P1*r  + P2*r  + ... + P5*r   ).
         *
         *   3. Scale back to obtain exp(x):
         *      From step 1, we have
         *         exp(x) = 2^k * exp(r)
         *
         * Special cases:
         *      exp(INF) is INF, exp(NaN) is NaN;
         *      exp(-INF) is 0, and
         *      for finite argument, only exp(0)=1 is exact.
         *
         * Accuracy:
         *      according to an error analysis, the error is always less than
         *      1 ulp (unit in the last place).
         *
         * Misc. info.
         *      For IEEE double
         *          if x >  7.09782712893383973096e+02 then exp(x) overflow
         *          if x < -7.45133219101941108420e+02 then exp(x) underflow
         *
         * Constants:
         * The hexadecimal values are the intended ones for the following
         * constants. The decimal values may be used, provided that the
         * compiler will convert from decimal to binary accurately enough
         * to produce the hexadecimal values shown.
         */
        class Exp final : public Object {
            static CORE_FAST gdouble one = 1.0;
            // static CORE_FAST gdouble half[] = {0.5, -0.5,};
            static CORE_FAST gdouble huge = 1.0e+300;
            static CORE_FAST gdouble twom1000 = 0x1.0p-1000; //  9.33263618503218878990e-302 = 2^-1000
            static CORE_FAST gdouble o_threshold = 0x1.62e42fefa39efp9; //  7.09782712893383973096e+02
            static CORE_FAST gdouble u_threshold = -0x1.74910d52d3051p9; // -7.45133219101941108420e+02;
            // static CORE_FAST gdouble ln2HI[] = {0x1.62e42feep-1,         //  6.93147180369123816490e-01
            //                                     -0x1.62e42feep-1};        // -6.93147180369123816490e-01
            // static CORE_FAST gdouble ln2LO[] = {0x1.a39ef35793c76p-33,   //  1.90821492927058770002e-10
            //                                     -0x1.a39ef35793c76p-33};  // -1.90821492927058770002e-10
            static CORE_FAST gdouble invln2 = 0x1.71547652b82fep0; //  1.44269504088896338700e+00

            static CORE_FAST gdouble P1 = 0x1.555555555553ep-3; //  1.66666666666666019037e-01
            static CORE_FAST gdouble P2 = -0x1.6c16c16bebd93p-9; // -2.77777777770155933842e-03
            static CORE_FAST gdouble P3 = 0x1.1566aaf25de2cp-14; //  6.61375632143793436117e-05
            static CORE_FAST gdouble P4 = -0x1.bbd41c5d26bf1p-20; // -1.65339022054652515390e-06
            static CORE_FAST gdouble P5 = 0x1.6376972bea4d0p-25; //  4.13813679705723846039e-08

            CORE_EXPLICIT Exp();

        public:
            static gdouble compute(gdouble x);
        };
    };
} // core

#endif //CORE24_FDLIBM_H
