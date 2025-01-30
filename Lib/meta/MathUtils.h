//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_MATHUTILS_H
#define CORE24_MATHUTILS_H

#include <core/Object.h>

namespace core {
    /**
     * The class @c MathUtils contains methods for performing basic
     * numeric operations such as the elementary exponential, logarithm,
     * square root, and trigonometric functions.
     *
     * <p>To help ensure portability of programs, the definitions of
     * some of the numeric functions in this package require that they
     * produce the same results as certain published algorithms. These
     * algorithms are available from the well-known network library
     * @c netlib as the package "Freely Distributable Math
     * Library," <a href="https://www.netlib.org/fdlibm/">FdLibm</a>. These
     * algorithms, which are written in the C programming language, are
     * then to be understood as executed with all floating-point
     * operations following the rules of Java floating-point arithmetic.
     *
     * <p>The math library is defined with respect to
     * @c fdlibm version 5.3. Where @c FdLibm provides
     * more than one definition for a function (such as
     * @c acos), use the "IEEE 754 core function" version
     * (residing in a file whose name begins with the letter
     * @c e).  The methods which require @c FdLibm
     * semantics are @c sin, @c cos, @c tan,
     * @c asin, @c acos, @c atan, @c exp, @c log, @c log10,
     * @c cbrt, @c atan2, @c pow, @c sinh, @c cosh, @c tanh,
     * @c hypot, @c expm1, and @c log1p.
     *
     * <p>
     * The platform uses signed two's complement integer arithmetic with
     * gint and glong primitive types.  The developer should choose
     * the primitive type to ensure that arithmetic operations consistently
     * produce correct results, which in some cases means the operations
     * will not overflow the range of values of the computation.
     * The best practice is to choose the primitive type and algorithm to avoid
     * overflow. In cases where the size is @c gint or @c glong and
     * overflow errors need to be detected, the methods whose names end with
     * @c Exact throw an @c ArithmeticException when the results overflow.
     *
     * <h2>IEEE 754 Recommended Operations</h2>
     *
     * The @a Math class discusses how the shared
     * quality of implementation criteria for selected @c Math and
     * @c MathUtils methods relate to the IEEE 754 recommended operations.
     *
     * @see <a href="https://standards.ieee.org/ieee/754/6210/">
     *      <cite>IEEE Standard for Floating-Point Arithmetic</cite></a>
     */
    class MathUtils final : public Object {
        MathUtils() = default;

    public:
        /**
         * Returns the trigonometric sine of an angle. Special cases:
         * <ul><li>If the argument is NaN or an infinity, then the
         * result is NaN.
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.</ul>
         *
         * @param   a   an angle, in radians.
         * @return  the sine of the argument.
         */
        static gdouble sin(gdouble a) /* @native */;

        /**
         * Returns the trigonometric cosine of an angle. Special cases:
         * <ul><li>If the argument is NaN or an infinity, then the
         * result is NaN.
         * <li>If the argument is zero, then the result is @c 1.0.
         * </ul>
         *
         * @param   a   an angle, in radians.
         * @return  the cosine of the argument.
         */
        static gdouble cos(gdouble a) /* @native */;

        /**
         * Returns the trigonometric tangent of an angle. Special cases:
         * <ul><li>If the argument is NaN or an infinity, then the result
         * is NaN.
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.</ul>
         *
         * @param   a   an angle, in radians.
         * @return  the tangent of the argument.
         */
        static gdouble tan(gdouble a) /* @native */;

        /**
         * Returns the arc sine of a value; the returned angle is in the
         * range -@a pi/2 through @a pi/2.  Special cases:
         * <ul><li>If the argument is NaN or its absolute value is greater
         * than 1, then the result is NaN.
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.</ul>
         *
         * @param   a   the value whose arc sine is to be returned.
         * @return  the arc sine of the argument.
         */
        static gdouble asin(gdouble a) /* @native */;

        /**
         * Returns the arc cosine of a value; the returned angle is in the
         * range 0.0 through @a pi.  Special case:
         * <ul><li>If the argument is NaN or its absolute value is greater
         * than 1, then the result is NaN.
         * <li>If the argument is @c 1.0, the result is positive zero.
         * </ul>
         *
         * @param   a   the value whose arc cosine is to be returned.
         * @return  the arc cosine of the argument.
         */
        static gdouble acos(gdouble a) /* @native */;

        /**
         * Returns the arc tangent of a value; the returned angle is in the
         * range -@a pi/2 through @a pi/2.  Special cases:
         * <ul><li>If the argument is NaN, then the result is NaN.
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * <li>If the argument is @a infinite,
         * then the result is the closest value to @a pi/2 with the
         * same sign as the input.
         * </ul>
         *
         * @param   a   the value whose arc tangent is to be returned.
         * @return  the arc tangent of the argument.
         */
        static gdouble atan(gdouble a) /* @native */;

        /**
         * Returns the natural logarithm (base @a e) of a @c gdouble
         * value. Special cases:
         * <ul><li>If the argument is NaN or less than zero, then the result
         * is NaN.
         * <li>If the argument is positive infinity, then the result is
         * positive infinity.
         * <li>If the argument is positive zero or negative zero, then the
         * result is negative infinity.
         * <li>If the argument is @c 1.0, then the result is positive
         * zero.
         * </ul>
         *
         * @param   a   a value
         * @return  the value ln @c a, the natural logarithm of @c a.
         */
        static gdouble log(gdouble a) /* @native */;

        /**
         * Returns the base 10 logarithm of a @c gdouble value.
         * Special cases:
         *
         * <ul><li>If the argument is NaN or less than zero, then the result
         * is NaN.
         * <li>If the argument is positive infinity, then the result is
         * positive infinity.
         * <li>If the argument is positive zero or negative zero, then the
         * result is negative infinity.
         * <li>If the argument is equal to 10<sup>@a n</sup> for
         * integer @a n, then the result is @a n. In particular,
         * if the argument is @c 1.0 (10<sup>0</sup>), then the
         * result is positive zero.
         * </ul>
         *
         * @param   a   a value
         * @return  the base 10 logarithm of  @c a.
         */
        static gdouble log10(gdouble a) /* @native */;

        /**
         * Returns the correctly rounded positive square root of a
         * @c gdouble value.
         * Special cases:
         * <ul><li>If the argument is NaN or less than zero, then the result
         * is NaN.
         * <li>If the argument is positive infinity, then the result is positive
         * infinity.
         * <li>If the argument is positive zero or negative zero, then the
         * result is the same as the argument.</ul>
         * Otherwise, the result is the @c gdouble value closest to
         * the true mathematical square root of the argument value.
         *
         * @param   a   a value.
         * @return  the positive square root of @c a.
         */
        static gdouble sqrt(gdouble a) /* @native */;

        /**
         * Computes the remainder operation on two arguments as prescribed
         * by the IEEE 754 standard.
         * The remainder value is mathematically equal to
         * @c f1-f2 &times; @a n,
         * where @a n is the mathematical integer closest to the exact
         * mathematical value of the quotient @c f1/f2, and if two
         * mathematical integers are equally close to @c f1/f2,
         * then @a n is the integer that is even. If the remainder is
         * zero, its sign is the same as the sign of the first argument.
         * Special cases:
         * <ul><li>If either argument is NaN, or the first argument is infinite,
         * or the second argument is positive zero or negative zero, then the
         * result is NaN.
         * <li>If the first argument is finite and the second argument is
         * infinite, then the result is the same as the first argument.</ul>
         *
         * @param   f1   the dividend.
         * @param   f2   the divisor.
         * @return  the remainder when @c f1 is divided by
         *          @c f2.
         */
        static gdouble IEEEremainder(gdouble f1, gdouble f2) /* @native */;

        /**
         * Returns the angle @a theta from the conversion of rectangular
         * coordinates (@c x, @c y) to polar
         * coordinates (r, @a theta).
         * This method computes the phase @a theta by computing an arc tangent
         * of @c y/x in the range of -@a pi to @a pi. Special
         * cases:
         * <ul><li>If either argument is NaN, then the result is NaN.
         * <li>If the first argument is positive zero and the second argument
         * is positive, or the first argument is positive and finite and the
         * second argument is positive infinity, then the result is positive
         * zero.
         * <li>If the first argument is negative zero and the second argument
         * is positive, or the first argument is negative and finite and the
         * second argument is positive infinity, then the result is negative zero.
         * <li>If the first argument is positive zero and the second argument
         * is negative, or the first argument is positive and finite and the
         * second argument is negative infinity, then the result is the
         * @c gdouble value closest to @a pi.
         * <li>If the first argument is negative zero and the second argument
         * is negative, or the first argument is negative and finite and the
         * second argument is negative infinity, then the result is the
         * @c gdouble value closest to -@a pi.
         * <li>If the first argument is positive and the second argument is
         * positive zero or negative zero, or the first argument is positive
         * infinity and the second argument is finite, then the result is the
         * @c gdouble value closest to @a pi/2.
         * <li>If the first argument is negative and the second argument is
         * positive zero or negative zero, or the first argument is negative
         * infinity and the second argument is finite, then the result is the
         * @c gdouble value closest to -@a pi/2.
         * <li>If both arguments are positive infinity, then the result is the
         * @c gdouble value closest to @a pi/4.
         * <li>If the first argument is positive infinity and the second argument
         * is negative infinity, then the result is the @c gdouble
         * value closest to 3*@a pi/4.
         * <li>If the first argument is negative infinity and the second argument
         * is positive infinity, then the result is the @c gdouble value
         * closest to -@a pi/4.
         * <li>If both arguments are negative infinity, then the result is the
         * @c gdouble value closest to -3*@a pi/4.</ul>
         *
         * @apiNote
         * For @a y with a positive sign and finite nonzero
         * @a x, the exact mathematical value of @c atan2 is
         * equal to:
         * <ul>
         * <li>If @a x>0, atan(abs(@a y/@a x))
         * <li>If @a x<0, &pi; - atan(abs(@a y/@a x))
         * </ul>
         *
         * @param   y   the ordinate coordinate
         * @param   x   the abscissa coordinate
         * @return  the @a theta component of the point
         *          (@a r, @a theta)
         *          in polar coordinates that corresponds to the point
         *          (@a x, @a y) in Cartesian coordinates.
         */
        static gdouble atan2(gdouble y, gdouble x) /* @native */;

        /**
         * Returns the hyperbolic sine of a @c gdouble value.
         * The hyperbolic sine of @a x is defined to be
         * (@a e<sup>x</sup> - e<sup>-x</sup>)/2
         * where @a e is Euler's number.
         *
         * <p>Special cases:
         * <ul>
         *
         * <li>If the argument is NaN, then the result is NaN.
         *
         * <li>If the argument is infinite, then the result is an infinity
         * with the same sign as the argument.
         *
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         *
         * </ul>
         *
         * @param   x The number whose hyperbolic sine is to be returned.
         * @return  The hyperbolic sine of @c x.
         */
        static gdouble sinh(gdouble x) /* @native */;

        /**
         * Returns the hyperbolic cosine of a @c gdouble value.
         * The hyperbolic cosine of @a x is defined to be
         * (@a e<sup>x</sup> + e<sup>-x</sup>)/2
         * where @a e is Euler's number.
         *
         * <p>Special cases:
         * <ul>
         *
         * <li>If the argument is NaN, then the result is NaN.
         *
         * <li>If the argument is infinite, then the result is positive
         * infinity.
         *
         * <li>If the argument is zero, then the result is @c 1.0.
         *
         * </ul>
         *
         * @param   x The number whose hyperbolic cosine is to be returned.
         * @return  The hyperbolic cosine of @c x.
         */
        static gdouble cosh(gdouble x) /* @native */;

        /**
         * Returns the hyperbolic tangent of a @c gdouble value.
         * The hyperbolic tangent of @a x is defined to be
         * (@a e<sup>x</sup> - e<sup>-x</sup>)/(@a e<sup>x</sup> + e<sup>-x</sup>),
         * in other words, @c sinh(x)/cosh(x).  Note
         * that the absolute value of the exact tanh is always less than
         * 1.
         *
         * <p>Special cases:
         * <ul>
         *
         * <li>If the argument is NaN, then the result is NaN.
         *
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         *
         * <li>If the argument is positive infinity, then the result is
         * @c +1.0.
         *
         * <li>If the argument is negative infinity, then the result is
         * @c -1.0.
         *
         * </ul>
         *
         * @param   x The number whose hyperbolic tangent is to be returned.
         * @return  The hyperbolic tangent of @c x.
         */
        static gdouble tanh(gdouble x) /* @native */;

        /**
         * Returns @a e<sup>x</sup> -1.  Note that for values of
         * @a x near 0, the exact sum of
         * @c expm1(x) + 1 is much closer to the true
         * result of @a e<sup>x</sup> than @c exp(x).
         *
         * <p>Special cases:
         * <ul>
         * <li>If the argument is NaN, the result is NaN.
         *
         * <li>If the argument is positive infinity, then the result is
         * positive infinity.
         *
         * <li>If the argument is negative infinity, then the result is
         * -1.0.
         *
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         *
         * </ul>
         *
         * @param   x   the exponent to raise @a e to in the computation of
         *              @a e<sup>x</sup>-1.
         * @return  the value @a e<sup>x</sup>-1.
         */
        static gdouble expm1(gdouble x) /* @native */;

        /**
         * Returns the natural logarithm of the sum of the argument and 1.
         * Note that for small values @c x, the result of
         * @c log1p(x) is much closer to the true result of ln(1
         * + @c x) than the floating-point evaluation of
         * @c log(1.0+x).
         *
         * <p>Special cases:
         * <ul>
         *
         * <li>If the argument is NaN or less than -1, then the result is
         * NaN.
         *
         * <li>If the argument is positive infinity, then the result is
         * positive infinity.
         *
         * <li>If the argument is negative one, then the result is
         * negative infinity.
         *
         * <li>If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         *
         * </ul>
         *
         * @param   x   a value
         * @return the value ln(@c x + 1), the natural
         * log of @c x+1
         */
        static gdouble log1p(gdouble x) /* @native */;

        static gdouble fma(gdouble a, gdouble b, gdouble c);

        static gfloat fma(gfloat a, gfloat b, gfloat c);

        /**
         * Returns 10<sup>{@code e}</sup>.
         *
         * @param e The exponent which must meet
         *          0 &le; {@code e} &le; {@link #H}.
         * @return 10<sup>{@code e}</sup>.
         */
        static glong powerOfTeen(gint e);

        /**
         * Returns the unique integer <i>k</i> such that
         * 10<sup><i>k</i></sup> &le; 2<sup>{@code e}</sup>
         * &lt; 10<sup><i>k</i>+1</sup>.
         * <p>
         * The result is correct when |{@code e}| &le; 6_432_162.
         * Otherwise the result is undefined.
         *
         * @param e The exponent of 2, which should meet
         *          |{@code e}| &le; 6_432_162 for safe results.
         * @return &lfloor;log<sub>10</sub>2<sup>{@code e}</sup>&rfloor;.
         */
        static int flog10pow2(int e);

        /**
            * Returns the unique integer <i>k</i> such that
            * 10<sup><i>k</i></sup> &le; 3/4 &middot; 2<sup>{@code e}</sup>
            * &lt; 10<sup><i>k</i>+1</sup>.
            * <p>
            * The result is correct when
            * -3_606_689 &le; {@code e} &le; 3_150_619.
            * Otherwise the result is undefined.
            *
            * @param e The exponent of 2, which should meet
            *          -3_606_689 &le; {@code e} &le; 3_150_619 for safe results.
            * @return &lfloor;log<sub>10</sub>(3/4 &middot;
            * 2<sup>{@code e}</sup>)&rfloor;.
            */
        static int flog10threeQuartersPow2(int e);

        /**
            * Returns the unique integer <i>k</i> such that
            * 2<sup><i>k</i></sup> &le; 10<sup>{@code e}</sup>
            * &lt; 2<sup><i>k</i>+1</sup>.
            * <p>
            * The result is correct when |{@code e}| &le; 1_838_394.
            * Otherwise the result is undefined.
            *
            * @param e The exponent of 10, which should meet
            *          |{@code e}| &le; 1_838_394 for safe results.
            * @return &lfloor;log<sub>2</sub>10<sup>{@code e}</sup>&rfloor;.
            */
        static int flog2pow10(int e);

        /**
         * Let 10<sup>-{@code k}</sup> = <i>&beta;</i> 2<sup><i>r</i></sup>,
         * for the unique pair of integer <i>r</i> and real <i>&beta;</i> meeting
         * 2<sup>125</sup> &le; <i>&beta;</i> &lt; 2<sup>126</sup>.
         * Further, let <i>g</i> = &lfloor;<i>&beta;</i>&rfloor; + 1.
         * Split <i>g</i> into the higher 63 bits <i>g</i><sub>1</sub> and
         * the lower 63 bits <i>g</i><sub>0</sub>. Thus,
         * <i>g</i><sub>1</sub> =
         * &lfloor;<i>g</i> 2<sup>-63</sup>&rfloor;
         * and
         * <i>g</i><sub>0</sub> =
         * <i>g</i> - <i>g</i><sub>1</sub> 2<sup>63</sup>.
         * <p>
         * This method returns <i>g</i><sub>1</sub> while
         * {@link #g0(int)} returns <i>g</i><sub>0</sub>.
         * <p>
         * If needed, the exponent <i>r</i> can be computed as
         * <i>r</i> = {@code flog2pow10(-k)} - 125 (see {@link #flog2pow10(int)}).
         *
         * @param k The exponent of 10, which must meet
         *          {@link #K_MIN} &le; {@code e} &le; {@link #K_MAX}.
         * @return <i>g</i><sub>1</sub> as described above.
         */
        static glong g1(int k);

        /**
            * Returns <i>g</i><sub>0</sub> as described in
            * {@link #g1(int)}.
            *
            * @param k The exponent of 10, which must meet
            *          {@link #K_MIN} &le; {@code e} &le; {@link #K_MAX}.
            * @return <i>g</i><sub>0</sub> as described in
            * {@link #g1(int)}.
            */
        static glong g0(int k);

    private:
        /*
         * For full details about this code see the following reference:
         *
         *     Giulietti, "The Schubfach way to render doubles",
         *     https://drive.google.com/file/d/1gp5xv4CAa78SVgCeWfGqqI4FfYYYuNFb
         */

        /*
         * The boundaries for k in g0(int) and g1(int).
         * K_MIN must be DoubleToDecimal.K_MIN or less.
         * K_MAX must be DoubleToDecimal.K_MAX or more.
         */
        static CORE_FAST gint K_MIN = -324;
        static CORE_FAST gint K_MAX = 292;

        /* Must be DoubleToDecimal.H or more */
        static CORE_FAST gint H = 17;

        /* C_10 = floor(log10(2) * 2^Q_10), A_10 = floor(log10(3/4) * 2^Q_10) */
        static CORE_FAST gint Q_10 = 41;
        static CORE_FAST glong C_10 = 661971961083LL;
        static CORE_FAST glong A_10 = -274743187321L;

        /* C_2 = floor(log2(10) * 2^Q_2) */
        static CORE_FAST gint Q_2 = 38;
        static CORE_FAST glong C_2 = 913124641741LL;
    };
} // core

#endif //CORE24_MATHUTILS_H
