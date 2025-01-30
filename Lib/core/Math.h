//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_MATH_H
#define CORE24_MATH_H

#include <core/Object.h>

namespace core {
    /**
     * The class @c Math contains methods for performing basic
     * numeric operations such as the elementary exponential, logarithm,
     * square root, and trigonometric functions.
     *
     * <p>
     * The quality of implementation specifications concern two
     * properties, accuracy of the returned result and monotonicity of the
     * method.  Accuracy of the floating-point @c Math methods is
     * measured in terms of @a ulps, units in the last place.  For a
     * given floating-point format, an @a ulp of a
     * specific real number value is the distance between the two
     * floating-point values bracketing that numerical value.  When
     * discussing the accuracy of a method as a whole rather than at a
     * specific argument, the number of ulps cited is for the worst-case
     * error at any argument.  If a method always has an error less than
     * 0.5 ulps, the method always returns the floating-point number
     * nearest the exact result; such a method is @a correctly @a rounded.
     * A correctly rounded method is generally the best a
     * floating-point approximation can be; however, it is impractical for
     * many floating-point methods to be correctly rounded.  Instead, for
     * the @c Math class, a larger error bound of 1 or 2 ulps is
     * allowed for certain methods.  Informally, with a 1 ulp error bound,
     * when the exact result is a representable number, the exact result
     * should be returned as the computed result; otherwise, either of the
     * two floating-point values which bracket the exact result may be
     * returned.  For exact results large in magnitude, one of the
     * endpoints of the bracket may be infinite.  Besides accuracy at
     * individual arguments, maintaining proper relations between the
     * method at different arguments is also important.  Therefore, most
     * methods with more than 0.5 ulp errors are required to be
     * @a semi-monotonic: whenever the mathematical function is
     * non-decreasing, so is the floating-point approximation, likewise,
     * whenever the mathematical function is non-increasing, so is the
     * floating-point approximation.  Not all approximations that have 1
     * ulp accuracy will automatically meet the monotonicity requirements.
     * </p>
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
     * </p>
     * <h2>IEEE 754 Recommended Operations</h2>
     * <p>
     * The 2019 revision of the IEEE 754 floating-point standard includes
     * a section of recommended operations and the semantics of those
     * operations if they are included in a programming environment. The
     * recommended operations present in this class include @a sin,
     * @a cos, @a tan, @a asin, @a acos, @a atan, @a exp, @a expm1,
     * @a log, @a log10, @a log1p, @a sinh, @a cosh, @a tanh,
     * @a hypot, and @a pow.  (The @a sqrt operation is a required part of IEEE 754 from a different section
     * of the standard.) The special case behavior of the recommended
     * operations generally follows the guidance of the IEEE 754
     * standard. However, the @c pow method defines different
     * behavior for some arguments, as noted in its @a pow
     * specification. The IEEE 754 standard defines its operations to be
     * correctly rounded, which is a more stringent quality of
     * implementation condition than required for most of the methods in
     * question that are also included in this class.
     * </p>
     * @see <a href="https://standards.ieee.org/ieee/754/6210/">
     *      <cite>IEEE Standard for Floating-Point Arithmetic</cite></a>
     *
     */
    class Math final : public Object {
        /**
         * Don't let anyone instantiate this class.
         */
        CORE_IMPLICIT Math() = default;

    public:
        /**
         * @see Double::POSITIVE_INFINITY
         */
        static CORE_FAST gdouble INF = CORE_CONSTANT_POSITIVE_INFINITY;

        /**
         * @see Double::NaN
         */
        static CORE_FAST gdouble NaN = CORE_CONSTANT_NOT_A_NUMBER;

        /**
         * The @c gdouble value that is closer than any other to
         * @a e, the base of the natural logarithms.
         */
        static CORE_FAST gdouble E = 2.718281828459045235360287471352662498;

        /**
         * The @c gdouble value that is closer than any other to
         * @a pi (π), the ratio of the circumference of a circle to
         * its diameter.
         */
        static CORE_FAST gdouble PI = 3.141592653589793238462643383279502884;

        /**
         * The @c gdouble value that is closer than any other to
         * @a tau (τ), the ratio of the circumference of a circle
         * to its radius.
         *
         * @note
         * The value of @a pi is one half that of @a tau; in other
         * words, @a tau is gdouble @a pi .
         */
        static CORE_FAST gdouble TAU = 6.283185307179586476925286766559005768;

    private:
        /**
         * Constant by which to multiply an angular value in degrees to obtain an
         * angular value in radians. (pi / 180)
         */
        static CORE_FAST gdouble DEGREES_TO_RADIANS = 0.017453292519943295769236907684886127;

        /**
         * Constant by which to multiply an angular value in radians to obtain an
         * angular value in degrees. (180 / pi)
         */
        static CORE_FAST gdouble RADIANS_TO_DEGREES = 57.295779513082320876798154814105170320;

    public:
        /**
         * Returns the trigonometric sine of an angle.  Special cases:
         * - If the argument is NaN or an infinity, then the
         * result is NaN.
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         *
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   an angle, in radians.
         * @return  the sine of the argument.
         */
        static gdouble sin(gdouble a);

        /**
         * Returns the trigonometric cosine of an angle. Special cases:
         * - If the argument is NaN or an infinity, then the
         * result is NaN.
         * - If the argument is zero, then the result is @c 1.0.
         *
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   an angle, in radians.
         * @return  the cosine of the argument.
         */
        static gdouble cos(gdouble a);

        /**
         * Returns the trigonometric tangent of an angle.  Special cases:
         * - If the argument is NaN or an infinity, then the result
         * is NaN.
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   an angle, in radians.
         * @return  the tangent of the argument.
         */
        static gdouble tan(gdouble a);

        /**
         * Returns the arc sine of a value; the returned angle is in the
         * range -@a pi/2 through @a pi/2.  Special cases:
         * - If the argument is NaN or its absolute value is greater
         * than 1, then the result is NaN.
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         *
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   the value whose arc sine is to be returned.
         * @return  the arc sine of the argument.
         */
        static gdouble asin(gdouble a);

        /**
         * Returns the arc cosine of a value; the returned angle is in the
         * range 0.0 through @a pi.  Special case:
         * - If the argument is NaN or its absolute value is greater
         * than 1, then the result is NaN.
         * - If the argument is @c 1.0, the result is positive zero.
         *
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   the value whose arc cosine is to be returned.
         * @return  the arc cosine of the argument.
         */
        static gdouble acos(gdouble a);

        /**
         * Returns the arc tangent of a value; the returned angle is in the
         * range -@a pi/2 through @a pi/2.  Special cases:
         * - If the argument is NaN, then the result is NaN.
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * - If the argument is @c Double::isInfinite infinite,
         * then the result is the closest value to @a pi/2 with the
         * same sign as the input.
         *
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   the value whose arc tangent is to be returned.
         * @return  the arc tangent of the argument.
         */
        static gdouble atan(gdouble a);

        /**
         * Converts an angle measured in degrees to an approximately
         * equivalent angle measured in radians.  The conversion from
         * degrees to radians is generally inexact.
         *
         * @param   angdeg   an angle, in degrees
         * @return  the measurement of the angle @c angdeg
         *          in radians.
         *
         */
        static gdouble toRadians(gdouble angdeg);

        /**
         * Converts an angle measured in radians to an approximately
         * equivalent angle measured in degrees.  The conversion from
         * radians to degrees is generally inexact; users should
         * @a not expect @c cos(toRadians(90.0)) to exactly
         * equal @c 0.0.
         *
         * @param   angrad   an angle, in radians
         * @return  the measurement of the angle @c angrad
         *          in degrees.
         */
        static gdouble toDegrees(gdouble angrad);

        /**
         * Returns Euler's number @a e raised to the power of a
         * @c gdouble value.  Special cases:
         * - If the argument is NaN, the result is NaN.
         * - If the argument is positive infinity, then the result is
         * positive infinity.
         * - If the argument is negative infinity, then the result is
         * positive zero.
         * - If the argument is zero, then the result is @c 1.0.
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   the exponent to raise @a e to.
         * @return  the value @a e<sup>a</sup>,
         *          where @a e is the base of the natural logarithms.
         */
        static gdouble exp(gdouble a);

        /**
         * Returns the natural logarithm (base @a e) of a @c gdouble
         * value.  Special cases:
         * - If the argument is NaN or less than zero, then the result
         * is NaN.
         * - If the argument is positive infinity, then the result is
         * positive infinity.
         * - If the argument is positive zero or negative zero, then the
         * result is negative infinity.
         * - If the argument is @c 1.0, then the result is positive
         * zero.
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   a value
         * @return  the value @c ln(a), the natural logarithm of @c a.
         */
        static gdouble log(gdouble a);

        /**
         * Returns the base 10 logarithm of a @c gdouble value.
         * Special cases:
         *
         * - If the argument is NaN or less than zero, then the result
         * is NaN.
         * - If the argument is positive infinity, then the result is
         * positive infinity.
         * - If the argument is positive zero or negative zero, then the
         * result is negative infinity.
         * - If the argument is equal to 10<sup>n</sup> for
         * integer @a n, then the result is @a n. In particular,
         * if the argument is @c 1.0 (10<sup>0</sup>), then the
         * result is positive zero.
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   a value
         * @return  the base 10 logarithm of  @c a.
         *
         */
        static gdouble log10(gdouble a);

        /**
         * Returns the base 10 logarithm of a @c gdouble value.
         * Special cases:
         *
         * - If the argument is NaN or less than zero, then the result
         * is NaN.
         * - If the argument is positive infinity, then the result is
         * positive infinity.
         * - If the argument is positive zero or negative zero, then the
         * result is negative infinity.
         * - If the argument is equal to 10<sup>n</sup> for
         * integer @a n, then the result is @a n. In particular,
         * if the argument is @c 1.0 (10<sup>0</sup>), then the
         * result is positive zero.
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   a   a value
         * @return  the base 10 logarithm of  @c a.
         *
         */
        static gdouble log2(gdouble a);

        /**
         * Returns the correctly rounded positive square root of a
         * @c gdouble value.
         * Special cases:
         * - If the argument is NaN or less than zero, then the result
         * is NaN.
         * - If the argument is positive infinity, then the result is positive
         * infinity.
         * - If the argument is positive zero or negative zero, then the
         * result is the same as the argument.
         * Otherwise, the result is the @c gdouble value closest to
         * the true mathematical square root of the argument value.
         *
         * @note
         * This method corresponds to the squareRoot operation defined in
         * IEEE 754.
         *
         * @param   a   a value.
         * @return  the positive square root of @c a.
         *          If the argument is NaN or less than zero, the result is NaN.
         */
        static gdouble sqrt(gdouble a);


        /**
         * Returns the cube root of a @c gdouble value.  For
         * positive finite @c x, @c cbrt(-x) ==
         * -cbrt(x); that is, the cube root of a negative value is
         * the negative of the cube root of that value's magnitude.
         *
         * Special cases:
         * - If the argument is NaN, then the result is NaN.
         * - If the argument is infinite, then the result is an infinity
         * with the same sign as the argument.
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * </p>
         * @param   a   a value.
         * @return  the cube root of @c a.
         *
         */
        static gdouble cbrt(gdouble a);

        /**
         * Computes the remainder operation on two arguments as prescribed
         * by the IEEE 754 standard.
         * The remainder value is mathematically equal to
         * <code>f1 - f2</code> × @a n,
         * where @a n is the mathematical integer closest to the exact
         * mathematical value of the quotient @c f1/f2, and if two
         * mathematical integers are equally close to @c f1/f2,
         * then @a n is the integer that is even. If the remainder is
         * zero, its sign is the same as the sign of the first argument.
         * Special cases:
         * - If either argument is NaN, or the first argument is infinite,
         * or the second argument is positive zero or negative zero, then the
         * result is NaN.
         * - If the first argument is finite and the second argument is
         * infinite, then the result is the same as the first argument.
         *
         * @param   f1   the dividend.
         * @param   f2   the divisor.
         * @return  the remainder when @c f1 is divided by
         *          @c f2.
         */
        static gdouble IEEEremainder(gdouble f1, gdouble f2);

        /**
         * Returns the smallest (closest to negative infinity)
         * @c gdouble value that is greater than or equal to the
         * argument and is equal to a mathematical integer. Special cases:
         * - If the argument value is already equal to a
         * mathematical integer, then the result is the same as the
         * argument.
         * - If the argument is NaN or an infinity or
         * positive zero or negative zero, then the result is the same as
         * the argument.
         * - If the argument value is less than zero but
         * greater than -1.0, then the result is negative zero. Note
         * that the value of @c Math.ceil(x) is exactly the
         * value of @c -Math.floor(-x).
         *
         * @note
         * This method corresponds to the roundToIntegralTowardPositive
         * operation defined in IEEE 754.
         *
         * @param   a   a value.
         * @return  the smallest (closest to negative infinity)
         *          floating-point value that is greater than or equal to
         *          the argument and is equal to a mathematical integer.
         */
        static gdouble ceil(gdouble a);

        /**
         * Returns the largest (closest to positive infinity)
         * @c gdouble value that is less than or equal to the
         * argument and is equal to a mathematical integer. Special cases:
         * - If the argument value is already equal to a
         * mathematical integer, then the result is the same as the
         * argument.
         * - If the argument is NaN or an infinity or
         * positive zero or negative zero, then the result is the same as
         * the argument.
         *
         * @note
         * This method corresponds to the roundToIntegralTowardNegative
         * operation defined in IEEE 754.
         *
         * @param   a   a value.
         * @return  the largest (closest to positive infinity)
         *          floating-point value that less than or equal to the argument
         *          and is equal to a mathematical integer.
         */
        static gdouble floor(gdouble a);

        /**
         * Returns the @c gdouble value that is closest in value
         * to the argument and is equal to a mathematical integer. If two
         * @c gdouble values that are mathematical integers are
         * equally close, the result is the integer value that is
         * even. Special cases:
         * - If the argument value is already equal to a mathematical
         * integer, then the result is the same as the argument.
         * - If the argument is NaN or an infinity or positive zero or negative
         * zero, then the result is the same as the argument.
         *
         * @note
         * This method corresponds to the roundToIntegralTiesToEven
         * operation defined in IEEE 754.
         *
         * @param   a   a @c gdouble value.
         * @return  the closest floating-point value to @c a that is
         *          equal to a mathematical integer.
         */
        static gdouble rint(gdouble a);

        /**
         * Returns the angle @a theta from the conversion of rectangular
         * coordinates (@c x, @c y) to polar
         * coordinates (r, @a theta).
         * This method computes the phase @a theta by computing an arc tangent
         * of @c y/x in the range of -@a pi to @a pi. Special
         * cases:
         * - If either argument is NaN, then the result is NaN.
         * - If the first argument is positive zero and the second argument
         * is positive, or the first argument is positive and finite and the
         * second argument is positive infinity, then the result is positive
         * zero.
         * - If the first argument is negative zero and the second argument
         * is positive, or the first argument is negative and finite and the
         * second argument is positive infinity, then the result is negative zero.
         * - If the first argument is positive zero and the second argument
         * is negative, or the first argument is positive and finite and the
         * second argument is negative infinity, then the result is the
         * @c gdouble value closest to @a pi.
         * - If the first argument is negative zero and the second argument
         * is negative, or the first argument is negative and finite and the
         * second argument is negative infinity, then the result is the
         * @c gdouble value closest to -@a pi.
         * - If the first argument is positive and the second argument is
         * positive zero or negative zero, or the first argument is positive
         * infinity and the second argument is finite, then the result is the
         * @c gdouble value closest to @a pi/2.
         * - If the first argument is negative and the second argument is
         * positive zero or negative zero, or the first argument is negative
         * infinity and the second argument is finite, then the result is the
         * @c gdouble value closest to -@a pi/2.
         * - If both arguments are positive infinity, then the result is the
         * @c gdouble value closest to @a pi/4.
         * - If the first argument is positive infinity and the second argument
         * is negative infinity, then the result is the @c gdouble
         * value closest to 3*@a pi/4.
         * - If the first argument is negative infinity and the second argument
         * is positive infinity, then the result is the @c gdouble value
         * closest to -@a pi/4.
         * - If both arguments are negative infinity, then the result is the
         * @c gdouble value closest to -3*@a pi/4.
         *
         * <p>
         * The computed result must be within 2 ulps of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @note
         * For @a y with a positive sign and finite nonzero
         * @a x, the exact mathematical value of @c atan2 is
         * equal to:
         * - If @a x {@c > 0, atan(abs(@a y/@a x))
         * - If @a x {@c < 0, π - atan(abs(@a y/@a x))
         *
         * @param   y   the ordinate coordinate
         * @param   x   the abscissa coordinate
         * @return  the @a theta component of the point
         *          (@a r, @a theta)
         *          in polar coordinates that corresponds to the point
         *          (@a x, @a y) in Cartesian coordinates.
         */
        static gdouble atan2(gdouble y, gdouble x);

        /**
         * Returns the value of the first argument raised to the power of the
         * second argument. Special cases:
         *
         * - If the second argument is positive or negative zero, then the
         * result is 1.0.
         * - If the second argument is 1.0, then the result is the same as the
         * first argument.
         * - If the second argument is NaN, then the result is NaN.
         * - If the first argument is NaN and the second argument is nonzero,
         * then the result is NaN.
         * - If
         *   - the absolute value of the first argument is greater than 1
         * and the second argument is positive infinity, or
         *   - the absolute value of the first argument is less than 1 and
         * the second argument is negative infinity,
         * then the result is positive infinity.
         * - If
         *   - the absolute value of the first argument is greater than 1 and
         * the second argument is negative infinity, or
         *   - the absolute value of the
         * first argument is less than 1 and the second argument is positive
         * infinity,
         * then the result is positive zero.
         * - If the absolute value of the first argument equals 1 and the
         * second argument is infinite, then the result is NaN.
         * - If
         *   - the first argument is positive zero and the second argument
         * is greater than zero, or
         *   - the first argument is positive infinity and the second
         * argument is less than zero,
         * then the result is positive zero.
         * - If
         *   - the first argument is positive zero and the second argument
         * is less than zero, or
         *   - the first argument is positive infinity and the second
         * argument is greater than zero,
         * then the result is positive infinity.
         * - If
         *   - the first argument is negative zero and the second argument
         * is greater than zero but not a finite odd integer, or
         *   - the first argument is negative infinity and the second
         * argument is less than zero but not a finite odd integer,
         * then the result is positive zero.
         * - If
         *   - the first argument is negative zero and the second argument
         * is a positive finite odd integer, or
         *   - the first argument is negative infinity and the second
         * argument is a negative finite odd integer,
         * then the result is negative zero.
         * - If
         *   - the first argument is negative zero and the second argument
         * is less than zero but not a finite odd integer, or
         *   - the first argument is negative infinity and the second
         * argument is greater than zero but not a finite odd integer,
         * then the result is positive infinity.
         * - If
         *   - the first argument is negative zero and the second argument
         * is a negative finite odd integer, or
         *   - the first argument is negative infinity and the second
         * argument is a positive finite odd integer,
         * then the result is negative infinity.
         * - If the first argument is finite and less than zero
         * @li if the second argument is a finite even integer, the
         * result is equal to the result of raising the absolute value of
         * the first argument to the power of the second argument
         * - if the second argument is a finite odd integer, the result
         * is equal to the negative of the result of raising the absolute
         * value of the first argument to the power of the second
         * argument
         * - if the second argument is finite and not an integer, then
         * the result is NaN.
         * - If both arguments are integers, then the result is exactly equal
         * to the mathematical result of raising the first argument to the power
         * of the second argument if that result can in fact be represented
         * exactly as a @c gdouble value.
         * <p>
         * (In the foregoing descriptions, a floating-point value is
         * considered to be an integer if and only if it is finite and a
         * fixed point of the method @c ceil or,
         * equivalently, a fixed point of the method @c floor.
         * A value is a fixed point of a one-argument
         * method if and only if the result of applying the method to the
         * value is equal to the value.)
         * </p>
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @note
         * The special cases definitions of this method differ from the
         * special case definitions of the IEEE 754 recommended @c 
         * pow operation for @c ±1.0 raised to an infinite
         * power. This method treats such cases as indeterminate and
         * specifies a NaN is returned. The IEEE 754 specification treats
         * the infinite power as a large integer (large-magnitude
         * floating-point numbers are numerically integers, specifically
         * even integers) and therefore specifies @c 1.0 be returned.
         *
         * @param   a   the base.
         * @param   b   the exponent.
         * @return  the value @c a<sup>b</sup>.
         */
        static gdouble pow(gdouble a, gdouble b);

        /**
         * Returns the closest @c gint to the argument, with ties
         * rounding to positive infinity.
         *
         * <p>
         * Special cases:
         * - If the argument is NaN, the result is 0.
         * - If the argument is negative infinity or any value less than or
         * equal to the value of @c Integer::MIN_VALUE, the result is
         * equal to the value of @c Integer::MIN_VALUE.
         * - If the argument is positive infinity or any value greater than or
         * equal to the value of @c Integer::MAX_VALUE, the result is
         * equal to the value of @c Integer::MAX_VALUE.
         * </p>
         * @param   a   a floating-point value to be rounded to an integer.
         * @return  the value of the argument rounded to the nearest
         *          @c gint value.
         */
        static gint round(gfloat a);

        /**
         * Returns the closest @c glong to the argument, with ties
         * rounding to positive infinity.
         *
         * <p>
         * Special cases:
         * - If the argument is NaN, the result is 0.
         * - If the argument is negative infinity or any value less than or
         * equal to the value of @c Long::MIN_VALUE, the result is
         * equal to the value of @c Long::MIN_VALUE.
         * - If the argument is positive infinity or any value greater than or
         * equal to the value of @c Long::MAX_VALUE, the result is
         * equal to the value of @c Long::MAX_VALUE.
         * </p>
         * @param   a   a floating-point value to be rounded to a
         *          @c glong.
         * @return  the value of the argument rounded to the nearest
         *          @c glong value.
         */
        static glong round(gdouble a);

        /**
         * Returns a @c gdouble value with a positive sign, greater
         * than or equal to @c 0.0 and less than @c 1.0.
         * Returned values are chosen pseudorandomly with (approximately)
         * uniform distribution from that range.
         *
         * <p>
         * When this method is first called, it creates a single new
         * pseudorandom-number generator, exactly as if by the expression
         * @c new util::Random().
         *
         * This new pseudorandom-number generator is used thereafter for
         * all calls to this method and is used nowhere else.
         *
         * <p>
         * This method is properly synchronized to allow correct use by
         * more than one thread. However, if many threads need to generate
         * pseudorandom numbers at a great rate, it may reduce contention
         * for each thread to have its own pseudorandom-number generator.
         * </p>
         * @note
         * As the largest @c gdouble value less than @c 1.0
         * is @c Math.nextDown(1.0), a value @c x in the closed range
         * @c [x1,x2] where @c x1<=x2 may be defined by the statements
         *
         * @code
         * gdouble f = Math.random()/Math.nextDown(1.0);
         * gdouble x = x1*(1.0 - f) + x2*f;
         * @endcode
         *
         * @return  a pseudorandom @c gdouble greater than or equal
         * to @c 0.0 and less than @c 1.0.
         */
        static gdouble random();

        /**
         * Returns the sum of its arguments,
         * throwing an exception if the result overflows a @c gint.
         *
         * @param x the first value
         * @param y the second value
         * @return the result
         * @throws ArithmeticException if the result overflows a gint
         *
         */
        static gint addExact(gint x, gint y);

        /**
         * Returns the sum of its arguments,
         * throwing an exception if the result overflows a @c glong.
         *
         * @param x the first value
         * @param y the second value
         * @return the result
         * @throws ArithmeticException if the result overflows a glong
         *
         */
        static glong addExact(glong x, glong y);

        /**
         * Returns the difference of the arguments,
         * throwing an exception if the result overflows a @c gint.
         *
         * @param x the first value
         * @param y the second value to subtract from the first
         * @return the result
         * @throws ArithmeticException if the result overflows a gint
         *
         */
        static gint subtractExact(gint x, gint y);

        /**
         * Returns the difference of the arguments,
         * throwing an exception if the result overflows a @c glong.
         *
         * @param x the first value
         * @param y the second value to subtract from the first
         * @return the result
         * @throws ArithmeticException if the result overflows a glong
         *
         */
        static glong subtractExact(glong x, glong y);

        /**
         * Returns the product of the arguments,
         * throwing an exception if the result overflows a @c gint.
         *
         * @param x the first value
         * @param y the second value
         * @return the result
         * @throws ArithmeticException if the result overflows a gint
         *
         */
        static gint multiplyExact(gint x, gint y);

        /**
         * Returns the product of the arguments, throwing an exception if the result
         * overflows a @c glong.
         *
         * @param x the first value
         * @param y the second value
         * @return the result
         * @throws ArithmeticException if the result overflows a glong
         *
         */
        static glong multiplyExact(glong x, gint y);

        /**
         * Returns the product of the arguments,
         * throwing an exception if the result overflows a @c glong.
         *
         * @param x the first value
         * @param y the second value
         * @return the result
         * @throws ArithmeticException if the result overflows a glong
         *
         */
        static glong multiplyExact(glong x, glong y);

        /**
         * Returns the quotient of the arguments, throwing an exception if the
         * result overflows a @c gint.  Such overflow occurs in this method if
         * @c x is @c Integer::MIN_VALUE and @c y is @c -1.
         * In contrast, if @c Integer::MIN_VALUE / -1 were evaluated directly,
         * the result would be @c Integer::MIN_VALUE and no exception would be
         * thrown.
         * <p>
         * If @c y is zero, an @c ArithmeticException is thrown.
         * </p>
         * <p>
         * The built-in remainder operator @c "%" is a suitable counterpart
         * both for this method and for the built-in division operator @c "/".
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the quotient @c x / y
         * @throws ArithmeticException if @c y is zero or the quotient overflows a gint
         */
        static gint divideExact(gint x, gint y);

        /**
         * Returns the quotient of the arguments, throwing an exception if the
         * result overflows a @c glong.  Such overflow occurs in this method if
         * @c x is @c Long::MIN_VALUE and @c y is @c -1.
         * In contrast, if @c Long::MIN_VALUE / -1 were evaluated directly,
         * the result would be @c Long::MIN_VALUE and no exception would be
         * thrown.
         * <p>
         * If @c y is zero, an @c ArithmeticException is thrown
         * </p>
         * <p>
         * The built-in remainder operator @c "%" is a suitable counterpart
         * both for this method and for the built-in division operator @c "/".
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the quotient @c x/y
         * @throws ArithmeticException if @c y is zero or the quotient
         * overflows a glong
         */
        static glong divideExact(glong x, glong y);

        /**
         * Returns the largest (closest to positive infinity)
         * @c gint value that is less than or equal to the algebraic quotient.
         * This method is identical to @c Math::floorDiv(gint,gint) except that it
         * throws an @c ArithmeticException when the dividend is
         * @c Integer::MIN_VALUE Integer::MIN_VALUE and the divisor is
         * @c -1 instead of ignoring the integer overflow and returning
         * @c Integer::MIN_VALUE.
         * <p>
         * The floor modulus method @c Math::floorMod(gint,gint) is a suitable
         * counterpart both for this method and for the @c Math::floorDiv(gint,gint)
         * method.
         * </p>
         * <p>
         * For examples, see @c Math::floorDiv(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the largest (closest to positive infinity)
         * @c gint value that is less than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero, or the
         * dividend @c x is @c Integer::MIN_VALUE and the divisor @c y
         * is @c -1.
         *
         */
        static gint floorDivExact(gint x, gint y);

        /**
         * Returns the largest (closest to positive infinity)
         * @c glong value that is less than or equal to the algebraic quotient.
         * This method is identical to @c Math::floorDiv(glong,glong) except that it
         * throws an @c ArithmeticException when the dividend is
         * @c Long::MIN_VALUE Long::MIN_VALUE and the divisor is
         * @c -1 instead of ignoring the integer overflow and returning
         * @c Long::MIN_VALUE.
         * <p>
         * The floor modulus method @c Math::floorMod(glong,glong) is a suitable
         * counterpart both for this method and for the @c Math::floorDiv(glong,glong)
         * method.
         * </p>
         * <p>
         * For examples, see @c Math::floorDiv(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the largest (closest to positive infinity)
         * @c glong value that is less than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero, or the
         * dividend @c x is @c Long::MIN_VALUE and the divisor @c y
         * is @c -1.
         */
        static glong floorDivExact(glong x, glong y);

        /**
         * Returns the smallest (closest to negative infinity)
         * @c gint value that is greater than or equal to the algebraic quotient.
         * This method is identical to @c Math::ceilDiv(gint,gint) except that it
         * throws an @c ArithmeticException when the dividend is
         * @c Integer::MIN_VALUE Integer::MIN_VALUE and the divisor is
         * @c -1 instead of ignoring the integer overflow and returning
         * @c Integer::MIN_VALUE.
         * <p>
         * The ceil modulus method @c Math::ceilMod(gint,gint) is a suitable
         * counterpart both for this method and for the @c Math::ceilDiv(gint,gint)
         * method.
         * </p>
         * <p>
         * For examples, see @c Math::ceilDiv(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the smallest (closest to negative infinity)
         * @c gint value that is greater than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero, or the
         * dividend @c x is @c Integer::MIN_VALUE and the divisor @c y
         * is @c -1.
         *
         */
        static gint ceilDivExact(gint x, gint y);

        /**
         * Returns the smallest (closest to negative infinity)
         * @c glong value that is greater than or equal to the algebraic quotient.
         * This method is identical to @c Math::ceilDiv(glong,glong) except that it
         * throws an @c ArithmeticException when the dividend is
         * @c Long::MIN_VALUE Long::MIN_VALUE and the divisor is
         * @c -1 instead of ignoring the integer overflow and returning
         * @c Long::MIN_VALUE.
         * <p>
         * The ceil modulus method @c Math::ceilMod(glong,glong) is a suitable
         * counterpart both for this method and for the @c Math::ceilDiv(glong,glong)
         * method.
         * </p>
         * <p>
         * For examples, see @c Math::ceilDiv(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the smallest (closest to negative infinity)
         * @c glong value that is greater than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero, or the
         * dividend @c x is @c Long::MIN_VALUE and the divisor @c y
         * is @c -1.
         *
         */
        static glong ceilDivExact(glong x, glong y);

        /**
         * Returns the argument incremented by one, throwing an exception if the
         * result overflows a @c gint.
         * The overflow only occurs for @c Integer::MAX_VALUE the maximum value.
         *
         * @param a the value to increment
         * @return the result
         * @throws ArithmeticException if the result overflows a gint
         *
         */
        static gint incrementExact(gint a);

        /**
         * Returns the argument incremented by one, throwing an exception if the
         * result overflows a @c glong.
         * The overflow only occurs for @c Long::MAX_VALUE the maximum value.
         *
         * @param a the value to increment
         * @return the result
         * @throws ArithmeticException if the result overflows a glong
         *
         */
        static glong incrementExact(glong a);

        /**
         * Returns the argument decremented by one, throwing an exception if the
         * result overflows a @c gint.
         * The overflow only occurs for @c Integer::MIN_VALUE the minimum value.
         *
         * @param a the value to decrement
         * @return the result
         * @throws ArithmeticException if the result overflows a gint
         *
         */
        static gint decrementExact(gint a);

        /**
         * Returns the argument decremented by one, throwing an exception if the
         * result overflows a @c glong.
         * The overflow only occurs for @c Long::MIN_VALUE the minimum value.
         *
         * @param a the value to decrement
         * @return the result
         * @throws ArithmeticException if the result overflows a glong
         *
         */
        static glong decrementExact(glong a);

        /**
         * Returns the negation of the argument, throwing an exception if the
         * result overflows a @c gint.
         * The overflow only occurs for @c Integer::MIN_VALUE the minimum value.
         *
         * @param a the value to negate
         * @return the result
         * @throws ArithmeticException if the result overflows a gint
         *
         */
        static gint negateExact(gint a);

        /**
         * Returns the negation of the argument, throwing an exception if the
         * result overflows a @c glong.
         * The overflow only occurs for @c Long::MIN_VALUE the minimum value.
         *
         * @param a the value to negate
         * @return the result
         * @throws ArithmeticException if the result overflows a glong
         *
         */
        static glong negateExact(glong a);

        /**
         * Returns the value of the @c glong argument,
         * throwing an exception if the value overflows a @c gint.
         *
         * @param value the glong value
         * @return the argument as a gint
         * @throws ArithmeticException if the @c argument overflows a gint
         *
         */
        static gint toIntExact(glong value);

        /**
         * Returns the exact mathematical product of the arguments.
         *
         * @param x the first value
         * @param y the second value
         * @return the result
         *
         */
        static glong multiplyFull(gint x, gint y);

        /**
         * Returns as a @c glong the most significant 64 bits of the 128-bit
         * product of two 64-bit factors.
         *
         * @param x the first value
         * @param y the second value
         * @return the result
         */
        static glong multiplyHigh(glong x, glong y);

        /**
         * Returns as a @c glong the most significant 64 bits of the unsigned
         * 128-bit product of two unsigned 64-bit factors.
         *
         * @param x the first value
         * @param y the second value
         * @return the result
         */
        static glong unsignedMultiplyHigh(glong x, glong y);

        /**
         * Returns the largest (closest to positive infinity)
         * @c gint value that is less than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * @c Integer::MIN_VALUE Integer::MIN_VALUE and the divisor is @c -1,
         * then integer overflow occurs and
         * the result is equal to @c Integer::MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * negative infinity (floor) rounding mode.
         * The floor rounding mode gives different results from truncation
         * when the exact quotient is not an integer and is negative.
         *   - If the signs of the arguments are the same, the results of
         *       @c floorDiv and the @c / operator are the same.  <br>
         *       For example, @c floorDiv(4,3) == 1 and @c (4/3) == 1.
         *   - If the signs of the arguments are different, @c floorDiv
         *       returns the largest integer less than or equal to the quotient
         *       while the @c / operator returns the smallest integer greater
         *       than or equal to the quotient.
         *       They differ if and only if the quotient is not an integer.<br>
         *       For example, @c floorDiv(-4, 3) == -2,
         *       whereas @c (-4 / 3) == -1.
         * </p>
         *
         *
         * @param x the dividend
         * @param y the divisor
         * @return the largest (closest to positive infinity)
         * @c gint value that is less than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero
         *
         */
        static gint floorDiv(gint x, gint y);

        /**
         * Returns the largest (closest to positive infinity)
         * @c glong value that is less than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * @c Long::MIN_VALUE Long::MIN_VALUE and the divisor is @c -1,
         * then integer overflow occurs and
         * the result is equal to @c Long::MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * negative infinity (floor) rounding mode.
         * The floor rounding mode gives different results from truncation
         * when the exact result is not an integer and is negative.
         * </p>
         * <p>
         * For examples, see @c Math::floorDiv(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the largest (closest to positive infinity)
         * @c glong value that is less than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero
         */
        static gint floorDiv(glong x, gint y);

        /**
         * Returns the largest (closest to positive infinity)
         * @c glong value that is less than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * @c Long::MIN_VALUE Long::MIN_VALUE and the divisor is @c -1,
         * then integer overflow occurs and
         * the result is equal to @c Long::MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * negative infinity (floor) rounding mode.
         * The floor rounding mode gives different results from truncation
         * when the exact result is not an integer and is negative.
         * </p>
         * <p>
         * For examples, see @c Math::floorDiv(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the largest (closest to positive infinity)
         * @c glong value that is less than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero.
         */
        static glong floorDiv(glong x, glong y);

        /**
         * Returns the floor modulus of the @c gint arguments.
         * <p>
         * The floor modulus is @c r=x-(floorDiv(x,y)*y),
         * has the same sign as the divisor @c y or is zero, and
         * is in the range of @c -abs(y) < r < +abs(y).
         *
         * <p>
         * The relationship between @c floorDiv and @c floorMod is such that:
         *   - @c floorDiv(x,y)*y+floorMod(x,y)==x
         * </p>
         * <p>
         * The difference in values between @c floorMod and the @c % operator
         * is due to the difference between @c floorDiv and the @c /
         * operator, as detailed in @c Math::floorDiv(gint,gint).
         * </p>
         * <p>
         * Examples:
         *   - Regardless of the signs of the arguments, @c floorMod(x,y)
         *       is zero exactly when @c x % y is zero as well.
         *   - If neither @c floorMod(x,y) nor @c x%y is zero,
         *       they differ exactly when the signs of the arguments differ.<br>
         *       - @c Math::floorMod(+4,+3) == +1; and @c (+4%+3) == +1
         *       - @c Math::floorMod(-4,-3) == -1; and @c (-4%-3) == -1
         *       - @c Math::floorMod(+4,-3) == -2; and @c (+4%-3) == +1
         *       - @c Math::floorMod(-4,+3) == +2; and @c (-4%+3) == -1
         *
         * </p>
         *
         *
         * @param x the dividend
         * @param y the divisor
         * @return the floor modulus @c x-(floorDiv(x,y)*y)
         * @throws ArithmeticException if the divisor @c y is zero
         */
        static gint floorMod(gint x, gint y);

        /**
         * Returns the floor modulus of the @c glong and @c gint arguments.
         * <p>
         * The floor modulus is @c r=x-(floorDiv(x,y)*y),
         * has the same sign as the divisor @c y or is zero, and
         * is in the range of @c -abs(y)<r<+abs(y).
         *
         * <p>
         * The relationship between @c floorDiv and @c floorMod is such that:
         *   - @c Math::floorDiv(x,y) *y+floorMod(x,y)==x
         * </p>
         * <p>
         * For examples, see @c Math::floorMod(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the floor modulus @c x - (floorDiv(x,y) * y)
         * @throws ArithmeticException if the divisor @c y is zero.
         */
        static gint floorMod(glong x, gint y);

        /**
         * Returns the floor modulus of the @c glong arguments.
         * <p>
         * The floor modulus is @c r = x - (floorDiv(x,y) * y),
         * has the same sign as the divisor @c y or is zero, and
         * is in the range of @c -abs(y) < r < +abs(y).
         * </p>
         * <p>
         * The relationship between @c floorDiv and @c floorMod is such that:
         *   - @c floorDiv(x,y) * y + floorMod(x,y) == x
         * </p>
         * <p>
         * For examples, see @c Math::floorMod(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the floor modulus @c x - (floorDiv(x,y) * y)
         * @throws ArithmeticException if the divisor @c y is zero
         *
         */
        static glong floorMod(glong x, glong y);

        /**
         * Returns the smallest (closest to negative infinity)
         * @c gint value that is greater than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * @c Integer::MIN_VALUE Integer::MIN_VALUE and the divisor is @c -1,
         * then integer overflow occurs and
         * the result is equal to @c Integer::MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * positive infinity (ceiling) rounding mode.
         * The ceiling rounding mode gives different results from truncation
         * when the exact quotient is not an integer and is positive.
         *
         *   - If the signs of the arguments are different, the results of
         *       @c ceilDiv and the @c / operator are the same.  <br>
         *       For example, @c ceilDiv(-4,3) == -1 and @c (-4/3) == -1.
         *   - If the signs of the arguments are the same, @c ceilDiv
         *       returns the smallest integer greater than or equal to the quotient
         *       while the @c / operator returns the largest integer less
         *       than or equal to the quotient.
         *       They differ if and only if the quotient is not an integer.<br>
         *       For example, @c ceilDiv(4,3) == 2,
         *       whereas @c (4/3) == 1.
         *
         * </p>
         *
         * @param x the dividend
         * @param y the divisor
         * @return the smallest (closest to negative infinity)
         * @c gint value that is greater than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero
         *
         */
        static gint ceilDiv(gint x, gint y);

        /**
         * Returns the smallest (closest to negative infinity)
         * @c glong value that is greater than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * @c  Long::MIN_VALUE Long::MIN_VALUE and the divisor is @c -1,
         * then integer overflow occurs and
         * the result is equal to @c Long::MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * positive infinity (ceiling) rounding mode.
         * The ceiling rounding mode gives different results from truncation
         * when the exact result is not an integer and is positive.
         * </p>
         * <p>
         * For examples, see @c Math::ceilDiv(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the smallest (closest to negative infinity)
         * @c glong value that is greater than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero
         *
         */
        static gint ceilDiv(glong x, gint y);

        /**
         * Returns the smallest (closest to negative infinity)
         * @c glong value that is greater than or equal to the algebraic quotient.
         * There is one special case: if the dividend is
         * @c Long::MIN_VALUE Long::MIN_VALUE and the divisor is @c -1,
         * then integer overflow occurs and
         * the result is equal to @c Long::MIN_VALUE.
         * <p>
         * Normal integer division operates under the round to zero rounding mode
         * (truncation).  This operation instead acts under the round toward
         * positive infinity (ceiling) rounding mode.
         * The ceiling rounding mode gives different results from truncation
         * when the exact result is not an integer and is positive.
         * </p>
         * <p>
         * For examples, see @c Math::ceilDiv(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the smallest (closest to negative infinity)
         * @c glong value that is greater than or equal to the algebraic quotient.
         * @throws ArithmeticException if the divisor @c y is zero
         *
         */
        static glong ceilDiv(glong x, glong y);

        /**
         * Returns the ceiling modulus of the @c gint arguments.
         * <p>
         * The ceiling modulus is @c r = x - (ceilDiv(x,y) * y),
         * has the opposite sign as the divisor @c y or is zero, and
         * is in the range of @c -abs(y) < r < +abs(y).
         * </p>
         * <p>
         * The relationship between @c ceilDiv and @c ceilMod is such that:
         *   @li @c ceilDiv(x,y) * y + ceilMod(x,y) == x
         * </p>
         * <p>
         * The difference in values between @c ceilMod and the @c % operator
         * is due to the difference between @c ceilDiv and the @c /
         * operator, as detailed in @c Math::ceilDiv(gint,gint).
         * </p>
         * <p>
         * Examples:
         *
         *   - Regardless of the signs of the arguments, @c ceilMod(x,y)
         *       is zero exactly when @c x % y is zero as well.
         *   - If neither @c ceilMod(x,y) nor @c x % y is zero,
         *       they differ exactly when the signs of the arguments are the same.<br>
         *       - @c Math::ceilMod(+4,+3) == -2; and @c (+4%+3) == +1
         *       - @c Math::ceilMod(-4,-3) == +2; and @c (-4%-3) == -1
         *       - @c Math::ceilMod(+4,-3) == +1; and @c (+4%-3) == +1
         *       - @c Math::ceilMod(-4,+3) == -1; and @c (-4%+3) == -1
         *
         *
         * </p>
         *
         * @param x the dividend
         * @param y the divisor
         * @return the ceiling modulus @c x - (ceilDiv(x,y) * y)
         * @throws ArithmeticException if the divisor @c y is zero
         *
         */
        static gint ceilMod(gint x, gint y);

        /**
         * Returns the ceiling modulus of the @c glong and @c gint arguments.
         * <p>
         * The ceiling modulus is @c r=x-(ceilDiv(x,y)*y),
         * has the opposite sign as the divisor @c y or is zero, and
         * is in the range of @c -abs(y) < r < +abs(y).
         *
         * <p>
         * The relationship between @c ceilDiv and @c ceilMod is such that:
         *   - @c Math::ceilDiv(x,y)*y+ceilMod(x,y) == x
         * </p>
         * <p>
         * For examples, see @c Math::ceilMod(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the ceiling modulus @c x - (ceilDiv(x,y) * y)
         * @throws ArithmeticException if the divisor @c y is zero
         *
         */
        static gint ceilMod(glong x, gint y);

        /**
         * Returns the ceiling modulus of the @c glong arguments.
         * <p>
         * The ceiling modulus is @c r=x-(ceilDiv(x,y) * y),
         * has the opposite sign as the divisor @c y or is zero, and
         * is in the range of @c -abs(y) < r < +abs(y).
         *
         * <p>
         * The relationship between @c ceilDiv and @c ceilMod is such that:
         *
         *   - @c Math::ceilDiv(x,y)*y+Math::ceilMod(x,y) == x
         * </p>
         * <p>
         * For examples, see @c Math::ceilMod(gint,gint).
         * </p>
         * @param x the dividend
         * @param y the divisor
         * @return the ceiling modulus @c x-(ceilDiv(x,y) * y)
         * @throws ArithmeticException if the divisor @c y is zero
         *
         */
        static glong ceilMod(glong x, glong y);

        /**
         * Returns the absolute value of a @c gint value.
         * If the argument is not negative, the argument is returned.
         * If the argument is negative, the negation of the argument is returned.
         *
         * <p>
         * Note that if the argument is equal to the value of @c
         * Integer::MIN_VALUE, the most negative representable @c gint
         * value, the result is that same value, which is negative. In
         * contrast, the @c Math::absExact(gint) method throws an
         * @c ArithmeticException for this value.
         * </p>
         * @param   a   the argument whose absolute value is to be determined
         * @return  the absolute value of the argument.
         */
        static gint abs(gint a);

        /**
         * Returns the mathematical absolute value of a @c gint value
         * if it is exactly representable as a @c gint, throwing
         * @c ArithmeticException if the result overflows the
         * positive @c gint range.
         *
         * <p>
         * Since the range of two's complement integers is asymmetric
         * with one additional negative value , the
         * mathematical absolute value of @c Integer::MIN_VALUE
         * overflows the positive @c gint range, so an exception is
         * thrown for that argument.
         * </p>
         * @param  a  the argument whose absolute value is to be determined
         * @return the absolute value of the argument, unless overflow occurs
         * @throws ArithmeticException if the argument is @c Integer::MIN_VALUE
         *
         */
        static gint absExact(gint a);

        /**
         * Returns the absolute value of a @c glong value.
         * If the argument is not negative, the argument is returned.
         * If the argument is negative, the negation of the argument is returned.
         *
         * <p>
         * Note that if the argument is equal to the value of @c
         * Long::MIN_VALUE, the most negative representable @c glong
         * value, the result is that same value, which is negative. In
         * contrast, the @c Math::absExact(glong) method throws an
         * @c ArithmeticException for this value.
         * </p>
         * @param   a   the argument whose absolute value is to be determined
         * @return  the absolute value of the argument.
         */
        static glong abs(glong a);

        /**
         * Returns the mathematical absolute value of a @c glong value
         * if it is exactly representable as a @c glong, throwing
         * @c ArithmeticException if the result overflows the
         * positive @c glong range.
         *
         * <p>
         * Since the range of two's complement integers is asymmetric
         * with one additional negative value , the
         * mathematical absolute value of @c Long::MIN_VALUE overflows
         * the positive @c glong range, so an exception is thrown for
         * that argument.
         * </p>
         * @param  a  the argument whose absolute value is to be determined
         * @return the absolute value of the argument, unless overflow occurs
         * @throws ArithmeticException if the argument is @c Long::MIN_VALUE
         *
         */
        static glong absExact(glong a);

        /**
         * Returns the absolute value of a @c gfloat value.
         * If the argument is not negative, the argument is returned.
         * If the argument is negative, the negation of the argument is returned.
         * Special cases:
         * - If the argument is positive zero or negative zero, the
         * result is positive zero.
         * - If the argument is infinite, the result is positive infinity.
         * - If the argument is NaN, the result is NaN.
         *
         * @note As implied by the above, one valid implementation of
         * this method is given by the expression below which computes a
         * @c gfloat with the same exponent and significand as the
         * argument but with a guaranteed zero sign bit indicating a
         * positive value: @c Float::fromIntBits(0x7fffffff & Float::toRawIntBits(a))
         *
         * @param   a   the argument whose absolute value is to be determined
         * @return  the absolute value of the argument.
         */
        static gfloat abs(gfloat a);

        /**
         * Returns the absolute value of a @c gdouble value.
         * If the argument is not negative, the argument is returned.
         * If the argument is negative, the negation of the argument is returned.
         * Special cases:
         * - If the argument is positive zero or negative zero, the result
         * is positive zero.
         * - If the argument is infinite, the result is positive infinity.
         * - If the argument is NaN, the result is NaN.
         *
         * @note As implied by the above, one valid implementation of
         * this method is given by the expression below which computes a
         * @c gdouble with the same exponent and significand as the
         * argument but with a guaranteed zero sign bit indicating a
         * positive value:<br>
         * @code Double::fromLongBits((Double::toRawLongBits(a) << 1) >> 1)
         * @endcode
         * @param   a  the argument whose absolute value is to be determined
         * @return  the absolute value of the argument.
         */
        static gdouble abs(gdouble a);

        /**
         * Returns the greater of two @c gint values. That is, the
         * result is the argument closer to the value of
         * @c Integer::MAX_VALUE. If the arguments have the same value,
         * the result is that same value.
         *
         * @param   a   an argument.
         * @param   b   another argument.
         * @return  the larger of @c a and @c b.
         */
        static gint max(gint a, gint b);

        /**
         * Returns the greater of two @c glong values. That is, the
         * result is the argument closer to the value of
         * @c Long::MAX_VALUE. If the arguments have the same value,
         * the result is that same value.
         *
         * @param   a   an argument.
         * @param   b   another argument.
         * @return  the larger of @c a and @c b.
         */
        static glong max(glong a, glong b);

        /**
         * Returns the greater of two @c gfloat values.  That is,
         * the result is the argument closer to positive infinity. If the
         * arguments have the same value, the result is that same
         * value. If either value is NaN, then the result is NaN.  Unlike
         * the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one
         * argument is positive zero and the other negative zero, the
         * result is positive zero.
         *
         * @note
         * This method corresponds to the maximum operation defined in
         * IEEE 754.
         *
         * @param   a   an argument.
         * @param   b   another argument.
         * @return  the larger of @c a and @c b.
         */
        static gfloat max(gfloat a, gfloat b);

        /**
         * Returns the greater of two @c gdouble values.  That
         * is, the result is the argument closer to positive infinity. If
         * the arguments have the same value, the result is that same
         * value. If either value is NaN, then the result is NaN.  Unlike
         * the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one
         * argument is positive zero and the other negative zero, the
         * result is positive zero.
         *
         * @note
         * This method corresponds to the maximum operation defined in
         * IEEE 754.
         *
         * @param   a   an argument.
         * @param   b   another argument.
         * @return  the larger of @c a and @c b.
         */
        static gdouble max(gdouble a, gdouble b);

        /**
         * Returns the smaller of two @c gint values. That is,
         * the result the argument closer to the value of
         * @c Integer::MIN_VALUE.  If the arguments have the same
         * value, the result is that same value.
         *
         * @param   a   an argument.
         * @param   b   another argument.
         * @return  the smaller of @c a and @c b.
         */
        static gint min(gint a, gint b);

        /**
         * Returns the smaller of two @c glong values. That is,
         * the result is the argument closer to the value of
         * @c Long::MIN_VALUE. If the arguments have the same
         * value, the result is that same value.
         *
         * @param   a   an argument.
         * @param   b   another argument.
         * @return  the smaller of @c a and @c b.
         */
        static glong min(glong a, glong b);

        /**
         * Returns the smaller of two @c gfloat values.  That is,
         * the result is the value closer to negative infinity. If the
         * arguments have the same value, the result is that same
         * value. If either value is NaN, then the result is NaN.  Unlike
         * the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero.  If
         * one argument is positive zero and the other is negative zero,
         * the result is negative zero.
         *
         * @note
         * This method corresponds to the minimum operation defined in
         * IEEE 754.
         *
         * @param   a   an argument.
         * @param   b   another argument.
         * @return  the smaller of @c a and @c b.
         */
        static gfloat min(gfloat a, gfloat b);

        /**
         * Returns the smaller of two @c gdouble values.  That
         * is, the result is the value closer to negative infinity. If the
         * arguments have the same value, the result is that same
         * value. If either value is NaN, then the result is NaN.  Unlike
         * the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero. If one
         * argument is positive zero and the other is negative zero, the
         * result is negative zero.
         *
         * @note
         * This method corresponds to the minimum operation defined in
         * IEEE 754.
         *
         * @param   a   an argument.
         * @param   b   another argument.
         * @return  the smaller of @c a and @c b.
         */
        static gdouble min(gdouble a, gdouble b);

        /**
         * Clamps the value to fit between min and max. If the value is less
         * than @c min, then @c min is returned. If the value is greater
         * than @c max, then @c max is returned. Otherwise, the original
         * value is returned.
         * <p>
         * While the original value of type glong may not fit into the gint type,
         * the bounds have the gint type, so the result always fits the gint type.
         * This allows to use method to safely cast glong value to gint with
         * saturation.
         *
         * @param value value to clamp
         * @param min minimal allowed value
         * @param max maximal allowed value
         * @return a clamped value that fits into @c min...max interval
         * @throws IllegalArgumentException if @c min > max
         *
         *
         */
        static gint clamp(glong value, gint min, gint max);

        /**
         * Clamps the value to fit between min and max. If the value is less
         * than @c min, then @c min is returned. If the value is greater
         * than @c max, then @c max is returned. Otherwise, the original
         * value is returned.
         *
         * @param value value to clamp
         * @param min minimal allowed value
         * @param max maximal allowed value
         * @return a clamped value that fits into @c min...max interval
         * @throws IllegalArgumentException if @c min > max
         *
         *
         */
        static glong clamp(glong value, glong min, glong max);

        /**
         * Clamps the value to fit between min and max. If the value is less
         * than @c min, then @c min is returned. If the value is greater
         * than @c max, then @c max is returned. Otherwise, the original
         * value is returned. If value is NaN, the result is also NaN.
         * <p>
         * Unlike the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero.
         * E.g., @c clamp(-0.0, 0.0, 1.0) returns 0.0.
         * </p>
         * @param value value to clamp
         * @param min minimal allowed value
         * @param max maximal allowed value
         * @return a clamped value that fits into @c min...max interval
         * @throws IllegalArgumentException if either of @c min and @c max
         * arguments is NaN, or @c min > max, or @c min is +0.0, and
         * @c max is -0.0.
         */
        static gdouble clamp(gdouble value, gdouble min, gdouble max);

        /**
         * Clamps the value to fit between min and max. If the value is less
         * than @c min, then @c min is returned. If the value is greater
         * than @c max, then @c max is returned. Otherwise, the original
         * value is returned. If value is NaN, the result is also NaN.
         * <p>
         * Unlike the numerical comparison operators, this method considers
         * negative zero to be strictly smaller than positive zero.
         * E.g., @c clamp(-0.0f,0.0f,1.0f) returns 0.0f.
         * </p>
         * @param value value to clamp
         * @param min minimal allowed value
         * @param max maximal allowed value
         * @return a clamped value that fits into @c min...max interval
         * @throws IllegalArgumentException if either of @c min and @c max
         * arguments is NaN, or @c min > max, or @c min is +0.0f, and
         * @c max is -0.0f.
         *
         *
         */
        static gfloat clamp(gfloat value, gfloat min, gfloat max);

        /**
         * Returns the fused multiply add of the three arguments; that is,
         * returns the exact product of the first two arguments summed
         * with the third argument and then rounded once to the nearest
         * @c gdouble.
         *
         * The rounding is done using the @c round to nearest even
         * rounding mode.
         *
         * In contrast, if @c a * b + c is evaluated as a regular
         * floating-point expression, two rounding errors are involved,
         * the first for the multiply operation, the second for the
         * addition operation.
         *
         * <p>
         * Special cases:
         *
         * @li If any argument is NaN, the result is NaN.
         *
         * @li If one of the first two arguments is infinite and the
         * other is zero, the result is NaN.
         *
         * @li If the exact product of the first two arguments is infinite
         * (in other words, at least one of the arguments is infinite and
         * the other is neither zero nor NaN) and the third argument is an
         * infinity of the opposite sign, the result is NaN.
         * </p>
         * <p>
         * Note that @c fma(a, 1.0, c) returns the same
         * result as (@c a + c).  However,
         * @c fma(a, b, +0.0) does <em>not</em> always return the
         * same result as (@c a * b) since
         * @c fma(-0.0, +0.0, +0.0) is @c +0.0 while
         * (@c -0.0 * +0.0) is @c -0.0; @c fma(a, b, -0.0) is
         * equivalent to (@c a * b) however.
         * </p>
         * @note This method corresponds to the fusedMultiplyAdd
         * operation defined in IEEE 754.
         *
         * @param a a value
         * @param b a value
         * @param c a value
         *
         * @return (@a a × @a b + @a c)
         * computed, as if with unlimited range and precision, and rounded
         * once to the nearest @c gdouble value
         *
         *
         */
        static gdouble fma(gdouble a, gdouble b, gdouble c);

        /**
         * Returns the fused multiply add of the three arguments; that is,
         * returns the exact product of the first two arguments summed
         * with the third argument and then rounded once to the nearest
         * @c gfloat.
         *
         * The rounding is done using the @c round to nearest even
         * rounding mode.
         *
         * In contrast, if @c a * b + c is evaluated as a regular
         * floating-point expression, two rounding errors are involved,
         * the first for the multiply operation, the second for the
         * addition operation.
         *
         * <p>
         * Special cases:
         *
         * @li If any argument is NaN, the result is NaN.
         *
         * @li If one of the first two arguments is infinite and the
         * other is zero, the result is NaN.
         *
         * @li If the exact product of the first two arguments is infinite
         * (in other words, at least one of the arguments is infinite and
         * the other is neither zero nor NaN) and the third argument is an
         * infinity of the opposite sign, the result is NaN.
         * </p>
         * <p>
         * Note that @c fma(a, 1.0f, c) returns the same
         * result as (@c a + c).  However,
         * @c fma(a, b, +0.0f) does <em>not</em> always return the
         * same result as (@c a * b) since
         * @c fma(-0.0f, +0.0f, +0.0f) is @c +0.0f while
         * (@c -0.0f * +0.0f) is @c -0.0f; @c fma(a, b, -0.0f) is
         * equivalent to (@c a * b) however.
         * </p>
         * @note This method corresponds to the fusedMultiplyAdd
         * operation defined in IEEE 754.
         *
         * @param a a value
         * @param b a value
         * @param c a value
         *
         * @return (@a a × @a b + @a c)
         * computed, as if with unlimited range and precision, and rounded
         * once to the nearest @c gfloat value
         */
        static gfloat fma(gfloat a, gfloat b, gfloat c);

        /**
         * Returns the size of an ulp of the argument.  An ulp, unit in
         * the last place, of a @c gdouble value is the positive
         * distance between this floating-point value and the @c 
         * gdouble value next larger in magnitude.  Note that for non-NaN
         * @a x, <code>ulp(-@a x) == ulp(@a x)</code>.
         *
         * <p>
         * Special Cases:
         *
         * @li If the argument is NaN, then the result is NaN.
         * @li If the argument is positive or negative infinity, then the
         * result is positive infinity.
         * @li If the argument is positive or negative zero, then the result is
         * @c Double::MIN_VALUE.
         * @li If the argument is ±@c Double::MAX_VALUE, then
         * the result is equal to 2<sup>971</sup>.
         * </p>
         *
         * @param d the floating-point value whose ulp is to be returned
         * @return the size of an ulp of the argument
         * @author Joseph D. Darcy
         *
         */
        static gdouble ulp(gdouble d);

        /**
         * Returns the size of an ulp of the argument.  An ulp, unit in
         * the last place, of a @c gfloat value is the positive
         * distance between this floating-point value and the @c 
         * gfloat value next larger in magnitude.  Note that for non-NaN
         * @a x, <code>ulp(-@a x) == ulp(@a x)</code>.
         *
         * <p>
         * Special Cases:
         *
         * @li If the argument is NaN, then the result is NaN.
         * @li If the argument is positive or negative infinity, then the
         * result is positive infinity.
         * @li If the argument is positive or negative zero, then the result is
         * @c Float::MIN_VALUE.
         * @li If the argument is ±@c Float::MAX_VALUE, then
         * the result is equal to 2<sup>104</sup>.
         * </p>
         *
         * @param f the floating-point value whose ulp is to be returned
         * @return the size of an ulp of the argument
         * @author Joseph D. Darcy
         *
         */
        static gfloat ulp(gfloat f);

        /**
         * Returns the signum function of the argument; zero if the argument
         * is zero, 1.0 if the argument is greater than zero, -1.0 if the
         * argument is less than zero.
         *
         * <p>
         * Special Cases:
         *
         * @li If the argument is NaN, then the result is NaN.
         * @li If the argument is positive zero or negative zero, then the
         *      result is the same as the argument.
         * </p>
         *
         * @param d the floating-point value whose signum is to be returned
         * @return the signum function of the argument
         * @author Joseph D. Darcy
         *
         */
        static gdouble signum(gdouble d);

        /**
         * Returns the signum function of the argument; zero if the argument
         * is zero, 1.0f if the argument is greater than zero, -1.0f if the
         * argument is less than zero.
         *
         * <p>
         * Special Cases:
         *
         * @li If the argument is NaN, then the result is NaN.
         * @li If the argument is positive zero or negative zero, then the
         *      result is the same as the argument.
         * </p>
         *
         * @param f the floating-point value whose signum is to be returned
         * @return the signum function of the argument
         * @author Joseph D. Darcy
         *
         */
        static gfloat signum(gfloat f);

        /**
         * Returns the hyperbolic sine of a @c gdouble value.
         * The hyperbolic sine of @a x is defined to be
         * (@a e<sup>x</sup> - e<sup>-x</sup>)/2
         * where @a e is @c Math::E Euler's number.
         *
         * <p>
         * Special cases:
         *
         *
         * - If the argument is NaN, then the result is NaN.
         *
         * - If the argument is infinite, then the result is an infinity
         * with the same sign as the argument.
         *
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * </p>
         *
         * <p>
         * The computed result must be within 2.5 ulps of the exact result.
         * </p>
         * @param   x The number whose hyperbolic sine is to be returned.
         * @return  The hyperbolic sine of @c x.
         *
         */
        static gdouble sinh(gdouble x);

        /**
         * Returns the hyperbolic cosine of a @c gdouble value.
         * The hyperbolic cosine of @a x is defined to be
         * (@a e<sup>x</sup> + e<sup>-x</sup>)/2
         * where @a e is @c Math::E Euler's number.
         *
         * <p>
         * Special cases:
         * - If the argument is NaN, then the result is NaN.
         *
         * - If the argument is infinite, then the result is positive
         * infinity.
         *
         * - If the argument is zero, then the result is @c 1.0.
         * </p>
         * <p>
         * The computed result must be within 2.5 ulps of the exact result.
         * </p>
         * @param   x The number whose hyperbolic cosine is to be returned.
         * @return  The hyperbolic cosine of @c x.
         *
         */
        static gdouble cosh(gdouble x);

        /**
         * Returns the hyperbolic tangent of a @c gdouble value.
         * The hyperbolic tangent of @a x is defined to be
         * (@a e<sup>x</sup> - e<sup>-x</sup>)/(@a e<sup>x</sup> + e<sup>-x</sup>),
         * in other words, @c Math::sinh(@a x)/@c Math::cosh(@a x).  Note
         * that the absolute value of the exact tanh is always less than
         * 1.
         *
         * <p>
         * Special cases:
         * - If the argument is NaN, then the result is NaN.
         *
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * - If the argument is positive infinity, then the result is @c +1.0.
         * - If the argument is negative infinity, then the result is @c -1.0.
         * <p>
         * The computed result must be within 2.5 ulps of the exact result.
         * The result of @c tanh for any finite input must have
         * an absolute value less than or equal to 1.  Note that once the
         * exact result of tanh is within 1/2 of an ulp of the limit value
         * of ±1, correctly signed ±@c 1.0 should
         * be returned.
         * </p>
         * @param   x The number whose hyperbolic tangent is to be returned.
         * @return  The hyperbolic tangent of @c x.
         *
         */
        static gdouble tanh(gdouble x);

        /**
         * Returns sqrt(@a x<sup>2</sup> +@a y<sup>2</sup>)
         * without intermediate overflow or underflow.
         *
         * <p>
         * Special cases:
         * @li If either argument is infinite, then the result
         * is positive infinity.
         * @li If either argument is NaN and neither argument is infinite,
         * then the result is NaN.
         * @li If both arguments are zero, the result is positive zero.
         * </p>
         * <p>
         * The computed result must be within 1 ulp of the exact
         * result.  If one parameter is held constant, the results must be
         * semi-monotonic in the other parameter.
         * </p>
         * @param x a value
         * @param y a value
         * @return sqrt(@a x<sup>2</sup> +@a y<sup>2</sup>)
         * without intermediate overflow or underflow
         */
        static gdouble hypot(gdouble x, gdouble y);

        /**
         * Returns @a e<sup>x</sup> -1.  Note that for values of
         * @a x near 0, the exact sum of
         * @c expm1(x) + 1 is much closer to the true
         * result of @a e<sup>x</sup> than @c exp(x).
         *
         * <p>
         * Special cases:
         *
         * - If the argument is NaN, the result is NaN.
         *
         * - If the argument is positive infinity, then the result is
         * positive infinity.
         *
         * - If the argument is negative infinity, then the result is
         * -1.0.
         *
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * </p>
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.  The result of
         * @c expm1 for any finite input must be greater than or
         * equal to @c -1.0.  Note that once the exact result of
         * @a e<sup>x</sup> - 1 is within 1/2
         * ulp of the limit value -1, @c -1.0 should be
         * returned.
         * </p>
         * @param   x   the exponent to raise @a e to in the computation of
         *              @a e<sup>x</sup> -1.
         * @return  the value @a e<sup>x</sup> - 1.
         *
         */
        static gdouble expm1(gdouble x);

        /**
         * Returns the natural logarithm of the sum of the argument and 1.
         * Note that for small values @c x, the result of
         * @c log1p(x) is much closer to the true result of ln(1
         * + @c x) than the floating-point evaluation of
         * @c log(1.0+x).
         *
         * <p>
         * Special cases:
         * - If the argument is NaN or less than -1, then the result is
         * NaN.
         *
         * - If the argument is positive infinity, then the result is
         * positive infinity.
         *
         * - If the argument is negative one, then the result is
         * negative infinity.
         *
         * - If the argument is zero, then the result is a zero with the
         * same sign as the argument.
         * </p>
         * <p>
         * The computed result must be within 1 ulp of the exact result.
         * Results must be semi-monotonic.
         * </p>
         * @param   x   a value
         * @return the value ln(@c x + 1), the natural log of @c x + 1
         *
         */
        static gdouble log1p(gdouble x);

        /**
         * Returns the first floating-point argument with the sign of the
         * second floating-point argument.
         *
         * @note
         * This method corresponds to the copySign operation defined in
         * IEEE 754.
         *
         * @param magnitude  the parameter providing the magnitude of the result
         * @param sign   the parameter providing the sign of the result
         * @return a value with the magnitude of @c magnitude
         * and the sign of @c sign.
         *
         */
        static gdouble copySign(gdouble magnitude, gdouble sign);

        /**
         * Returns the first floating-point argument with the sign of the
         * second floating-point argument.
         *
         * @note
         * This method corresponds to the copySign operation defined in
         * IEEE 754.
         *
         * @param magnitude  the parameter providing the magnitude of the result
         * @param sign   the parameter providing the sign of the result
         * @return a value with the magnitude of @c magnitude
         * and the sign of @c sign.
         *
         */
        static gfloat copySign(gfloat magnitude, gfloat sign);

        /**
         * Returns the unbiased exponent used in the representation of a
         * @c gfloat.  Special cases:
         * - If the argument is NaN or infinite, then the result is
         * @c Float::MAX_EXPONENT + 1.
         * - If the argument is zero or subnormal, then the result is
         * @c Float::MIN_EXPONENT - 1.
         *
         * @note
         * This method is analogous to the logB operation defined in IEEE
         * 754, but returns a different value on subnormal arguments.
         *
         * @param f a @c gfloat value
         * @return the unbiased exponent of the argument
         *
         */
        static gint exponent(gfloat f);

        /**
         * Returns the unbiased exponent used in the representation of a
         * @c gdouble.  Special cases:
         *
         *
         * - If the argument is NaN or infinite, then the result is
         * @c Double::MAX_EXPONENT + 1.
         * - If the argument is zero or subnormal, then the result is
         * @c Double::MIN_EXPONENT - 1.
         *
         * @note
         * This method is analogous to the logB operation defined in IEEE
         * 754, but returns a different value on subnormal arguments.
         *
         * @param d a @c gdouble value
         * @return the unbiased exponent of the argument
         *
         */
        static gint exponent(gdouble d);

        /**
         * Returns the floating-point number adjacent to the first
         * argument in the direction of the second argument.  If both
         * arguments compare as equal the second argument is returned.
         *
         * <p>
         * Special cases:
         * @li If either argument is a NaN, then NaN is returned.
         * @li If both arguments are signed zeros, @c direction
         * is returned unchanged (as implied by the requirement of
         * returning the second argument if the arguments compare as
         * equal).
         * @li If @c start is ±@c Double::MIN_VALUE and @c direction
         * has a value such that the result should have a smaller
         * magnitude, then a zero with the same sign as @c start
         * is returned.
         * @li If @c start is infinite and
         * @c direction has a value such that the result should
         * have a smaller magnitude, @c Double::MAX_VALUE with the
         * same sign as @c start is returned.
         * @li If @c start is equal to ±
         * @c Double::MAX_VALUE and @c direction has a
         * value such that the result should have a larger magnitude, an
         * infinity with same sign as @c start is returned.
         * </p>
         *
         * @param start  starting floating-point value
         * @param direction value indicating which of
         * @c start's neighbors or @c start should
         * be returned
         * @return The floating-point number adjacent to @c start in the
         * direction of @c direction.
         *
         */
        static gdouble nextAfter(gdouble start, gdouble direction);

        /**
         * Returns the floating-point number adjacent to the first
         * argument in the direction of the second argument.  If both
         * arguments compare as equal a value equivalent to the second argument
         * is returned.
         *
         * <p>
         * Special cases:
         *
         * @li If either argument is a NaN, then NaN is returned.
         *
         * @li If both arguments are signed zeros, a value equivalent
         * to @c direction is returned.
         *
         * @li If @c start is
         * ±@c Float::MIN_VALUE and @c direction
         * has a value such that the result should have a smaller
         * magnitude, then a zero with the same sign as @c start
         * is returned.
         *
         * @li If @c start is infinite and
         * @c direction has a value such that the result should
         * have a smaller magnitude, @c Float::MAX_VALUE with the
         * same sign as @c start is returned.
         *
         * @li If @c start is equal to ±
         * @c Float::MAX_VALUE and @c direction has a
         * value such that the result should have a larger magnitude, an
         * infinity with same sign as @c start is returned.
         * </p>
         *
         * @param start  starting floating-point value
         * @param direction value indicating which of
         * @c start's neighbors or @c start should
         * be returned
         * @return The floating-point number adjacent to @c start in the
         * direction of @c direction.
         *
         */
        static gfloat nextAfter(gfloat start, gfloat direction);

        /**
         * Returns the floating-point value adjacent to @c d in
         * the direction of positive infinity.  This method is
         * semantically equivalent to @c nextAfter(d,
         * Double::POSITIVE_INFINITY); however, a @c nextUp
         * implementation may run faster than its equivalent
         * @c nextAfter call.
         *
         * <p>
         * Special Cases:
         *
         * @li If the argument is NaN, the result is NaN.
         *
         * @li If the argument is positive infinity, the result is
         * positive infinity.
         *
         * @li If the argument is zero, the result is @c Double::MIN_VALUE
         *
         * </p>
         *
         * @note This method corresponds to the nextUp
         * operation defined in IEEE 754.
         *
         * @param d starting floating-point value
         * @return The adjacent floating-point value closer to positive
         * infinity.
         *
         */
        static gdouble nextUp(gdouble d);

        /**
         * Returns the floating-point value adjacent to @c f in
         * the direction of positive infinity.  This method is
         * semantically equivalent to @c nextAfter(f,
         * Float::POSITIVE_INFINITY); however, a @c nextUp
         * implementation may run faster than its equivalent
         * @c nextAfter call.
         *
         * <p>
         * Special Cases:
         *
         * @li If the argument is NaN, the result is NaN.
         *
         * @li If the argument is positive infinity, the result is
         * positive infinity.
         *
         * @li If the argument is zero, the result is @c Float::MIN_VALUE
         *
         * </p>
         *
         * @note This method corresponds to the nextUp
         * operation defined in IEEE 754.
         *
         * @param f starting floating-point value
         * @return The adjacent floating-point value closer to positive
         * infinity.
         *
         */
        static gfloat nextUp(gfloat f);

        /**
         * Returns the floating-point value adjacent to @c d in
         * the direction of negative infinity.  This method is
         * semantically equivalent to @c nextAfter(d,
         * Double::NEGATIVE_INFINITY); however, a
         * @c nextDown implementation may run faster than its
         * equivalent @c nextAfter call.
         *
         * <p>
         * Special Cases:
         *
         * @li If the argument is NaN, the result is NaN.
         *
         * @li If the argument is negative infinity, the result is
         * negative infinity.
         *
         * @li If the argument is zero, the result is
         * @c -Double::MIN_VALUE
         *
         * </p>
         *
         * @note This method corresponds to the nextDown
         * operation defined in IEEE 754.
         *
         * @param d  starting floating-point value
         * @return The adjacent floating-point value closer to negative
         * infinity.
         *
         */
        static gdouble nextDown(gdouble d);

        /**
         * Returns the floating-point value adjacent to @c f in
         * the direction of negative infinity.  This method is
         * semantically equivalent to @c nextAfter(f,Float::NEGATIVE_INFINITY);
         * however, a @c nextDown implementation may run faster than its
         * equivalent @c nextAfter call.
         *
         * <p>
         * Special Cases:
         *
         * @li If the argument is NaN, the result is NaN.
         *
         * @li If the argument is negative infinity, the result is
         * negative infinity.
         *
         * @li If the argument is zero, the result is @c -Float::MIN_VALUE
         * </p>
         * @note This method corresponds to the nextDown
         * operation defined in IEEE 754.
         *
         * @param f  starting floating-point value
         * @return The adjacent floating-point value closer to negative
         * infinity.
         *
         */
        static gfloat nextDown(gfloat f);

        /**
         * Returns @c d × 2<sup>scaleFactor</sup>
         * rounded as if performed by a single correctly rounded
         * floating-point multiply.  If the exponent of the result is
         * between @c Double::MIN_EXPONENT and @c Double::MAX_EXPONENT,
         * the answer is calculated exactly.  If the
         * exponent of the result would be larger than @c Double::MAX_EXPONENT,
         * an infinity is returned.  Note that if
         * the result is subnormal, precision may be lost; that is, when
         * @code scalb(x, n) @endcode is subnormal, @code scalb(scalb(x, n), -n) @endcode
         * may not equal @a x.
         * When the result is non-NaN, the result has the same sign as @c d.
         *
         * <p>
         * Special cases:
         *
         * @li If the first argument is NaN, NaN is returned.
         * @li If the first argument is infinite, then an infinity of the
         * same sign is returned.
         * @li If the first argument is zero, then a zero of the same sign is returned.
         *
         * </p>
         * @note This method corresponds to the scaleB operation
         * defined in IEEE 754.
         *
         * @param d number to be scaled by a power of two.
         * @param scaleFactor power of 2 used to scale @c d
         * @return @c d × 2<sup>scaleFactor</sup>
         *
         */
        static gdouble scalb(gdouble d, gint scaleFactor);

        /**
         * Returns @c f × 2<sup>scaleFactor</sup>
         * rounded as if performed by a single correctly rounded
         * floating-point multiply.  If the exponent of the result is
         * between @c Float::MIN_EXPONENT and @c Float::MAX_EXPONENT,
         * the answer is calculated exactly.  If the
         * exponent of the result would be larger than @c 
         * Float::MAX_EXPONENT, an infinity is returned.  Note that if the
         * result is subnormal, precision may be lost; that is, when
         * @code scalb(x, n) @endcode is subnormal, @code scalb(scalb(x, n), -n) @endcode
         * may not equal @a x.
         * When the result is non-NaN, the result has the same sign as @c f.
         *
         * <p>
         * Special cases:
         *
         * @li If the first argument is NaN, NaN is returned.
         * @li If the first argument is infinite, then an infinity of the
         * same sign is returned.
         * @li If the first argument is zero, then a zero of the same
         * sign is returned.
         * </p>
         *
         * @note This method corresponds to the scaleB operation
         * defined in IEEE 754.
         *
         * @param f number to be scaled by a power of two.
         * @param scaleFactor power of 2 used to scale @c f
         * @return @c f × 2<sup>scaleFactor</sup>
         *
         */
        static gfloat scalb(gfloat f, gint scaleFactor);

        /**
         * Returns a floating-point power of two in the normal range.
         */
        static gdouble powerOfTwoD(gint n);

        /**
         * Returns a floating-point power of two in the normal range.
         */
        static gfloat powerOfTwoF(gint n);
    };
} // core

#endif //CORE24_MATH_H
