//
// Created by brunshweeck on 3 juin 2024.
//

#ifndef CORE24_COMPLEX_H
#define CORE24_COMPLEX_H

#include <core/Byte.h>
#include <core/Math.h>

namespace core {
    /**
     * Representation of a Complex number, i.e. a number which has both a
     * real and imaginary part.
     * <p>
     * Implementations of arithmetic operations handle @c NaN and
     * infinite values according to the rules for @b Double, i.e.
     * @b equals is an equivalence relation for all instances that have
     * a @c NaN in either real or imaginary part, e.g. the following are
     * considered equal: <br/>
     *   - @code 1 + NaNi @endcode <br/>
     *   - @code NaN + i @endcode <br/>
     *   - @code NaN + NaNi @endcode
     * </p>
     * <p>
     * Note that this contradicts the IEEE-754 standard for floating
     * point numbers (according to which the test @code x == x @endcode) must fail if
     * @c x is @c NaN. The method
     * @b equals for primitive double conforms with IEEE-754 while this class conforms
     * with the standard behavior for Core24 object types.
     * </p>
     */
    class Complex final : public virtual Number, public virtual Comparable<Complex> {
    public:
        /**
         * The square root of -1.0 .
         * A number representing "0.0 + 1.0i"
         */
        static const Complex I;

        /**
         * A constant holding the pure real positive infinity of type @c Complex .
         * It equivalent to @c Complex(Math::INF)
         * @see Math::INF
         */
        static const Complex INF;

        /**
         * A constant holding the Not-a-Number (NaN) of type @c Complex .
         * It equivalent to @c Complex(Math::NaN, Math::NaN)
         * @see Math::NaN
         */
        static const Complex NaN;

        /**
         * A constant holding the pure imaginary positive infinity of type @c Complex .
         * It equivalent to @c Complex(0, Math::INF)
         * @see Math::INF
         */
        static const Complex INFi;

        /**
         * A complex number representing "1.0 + 0.0i"
         */
        static const Complex ONE;

        /**
         * A complex number representing "0.0 + 0.0i"
         */
        static const Complex ZERO;

        /**
         * Construct new @c Complex with null real and imaginary value
         */
        CORE_IMPLICIT Complex() = default;

        /**
         * Construct new @c Complex object with given real part and imaginary part.
         *
         * @param real The real part
         * @param imag The imaginary part
         */
        CORE_EXPLICIT Complex(gdouble real, gdouble imag);

        /**
         * Construct newly allocated @c Complex object that
         * represent the native @c gdouble argument as real
         * part.
         *
         * @note The imaginary will be set to zero.
         *
         * @param real The real part
         */
        CORE_IMPLICIT Complex(gdouble real);

        /**
         * Construct newly allocated @c Complex object that represent
         * specified primitive complex value.
         *
         * @param z the complex value
         */
        CORE_IMPLICIT Complex(_Dcomplex const &z);

        /**
         * Construct newly allocated @c Complex object that represent
         * specified primitive complex value.
         *
         * @tparam Z the type of primitive complex
         * @param z the complex value
         */
        template<class Z, ClassOf(1)::OnlyIf<Class<Complex>::isSame<Class<Z>::Object>()> = 1>
        CORE_IMPLICIT Complex(Z const &z)
        : Complex( initLiteral((glong) &z,
                            Class<Z>::size(),
                            Class<Z>::template isSameToOne<_Fcomplex, _Dcomplex, _Lcomplex>())) {
            CORE_FAST_ASSERT(Class<Z>::size() % 2 == 0);
        }

        /**
         * Returns @c true if this @c Complex value is
         * a Not-a-Number (NaN), @c false otherwise.
         *
         * @note Complex object is Not-a-Number (NaN) if any
         * part of this object is Not-a-Number.
         */
        gbool isNaN() const;

        /**
         * Returns @c true if this @c Complex value is
         * infinite, @c false otherwise.
         *
         * @note Complex object is infinite if any
         * part of this object is infinite.
         */
        gbool isInfinite() const;

        /**
         * Returns @c true if this @c Complex value is
         * a finite, @c false otherwise.
         *
         * @note Complex object is finite if all
         * parts of this object is finite.
         */
        gbool isFinite() const;

        /**
         * Returns @c true if this @c Complex value is
         * a pure real, @c false otherwise.
         *
         * @note Complex object is pure real if the
         * imaginary part is set to value zero.
         */
        gbool isReal() const;

        /**
         * Returns @c true if this @c Complex value is
         * a pure imaginary, @c false otherwise.
         *
         * @note Complex object is pure imaginary if the
         * real part is set to value zero.
         */
        gbool isImag() const;

        /**
         * Return the @c Complex object holding the value represented by the
         * argument string.
         * All whitespace characters in given string are ignored. Whitespace is removed
         * as if by @c String::strip method; that is, both ascii space and control characters are removed.
         * The rest of string should constitute a @a value (single or double) as described by the lexical
         * syntax rules:
         * <p>
         * 1- @a Single-Value: <br/>
         *   - Sign<sub>opt</sub><em>NaN</em> <br/>
         *   - Sign<sub>opt</sub><em>inf</em> <br/>
         *   - Sign<sub>opt</sub><em>infinity</em> <br/>
         *   - Sign<sub>opt</sub><em> floating literal</em> <br/>
         *   - Sign<sub>opt</sub><em>NaNi</em> <br/>
         *   - Sign<sub>opt</sub><em>infi</em> <br/>
         *   - Sign<sub>opt</sub><em>infinityi</em> <br/>
         *   - Sign<sub>opt</sub><em>(NaN)i</em> <br/>
         *   - Sign<sub>opt</sub><em>(inf)i</em> <br/>
         *   - Sign<sub>opt</sub><em>(infinity)i</em> <br/>
         *   - Sign<sub>opt</sub><em> floating literal</em> <b>i</b>
         * </p>
         * <p>
         * 2- Evaluable Expression: such as <br/>
         *   - exp<sub>1</sub> op expr<sub>2</sub> <br/>
         *   - exp<sub>1</sub> op<sub>1</sub> expr<sub>2</sub> ... exp<sub>k</sub> <br> <br/>
         *   ( where expN=single-value and op=+/-)
         * </p>
         *
         * Example:
         * @code
         *  auto z1 = Complex::valueOf("2"_S)          // z1 = 2.0 + 0.0i
         *  auto z2 = Complex::valueOf("5i"_S)         // z2 = 0.0 + 5.0i
         *  auto z3 = Complex::valueOf("2 + 5i"_S)     // z3 = 2.0 + 5.0i
         *  auto z4 = Complex::valueOf("2 + NaNi"_S)   // z4 = 2.0 + NaNi
         *  auto z5 = Complex::valueOf("2 + INFi"_S)   // z5 = 2.0 + INFi
         *  auto z6 = Complex::valueOf("2 + (inf)i"_S) // z6 = 2.0 + INFi
         *  auto z7 = Complex::valueOf("2 + (NaN)i"_S) // z7 = 2.0 + NaNi
         *  auto z8 = Complex::valueOf("1 + 2i + 3 - 5i + 6i - 7.5 + 8i"_S) // z8 = 2.5 + 11.0i
         * @endcode
         *
         * @note The case-sensitive is ignored.
         *
         * @param expression The String to be parsed
         * @throw NumberFormatException If the string do not contain a parsable number.
         */
        static Complex valueOf(String const &expression);

        /**
         * Return a @c Complex object representing the specified
         * @c gdouble value
         *
         * @param real
         */
        static Complex valueOf(gdouble real);

        /**
         * Return the @c String representing this @c Complex object.
         *
         * The native @c gdouble values represented by parts of this
         * object is converted to a string exactly as if by the
         * @c Double::toString method of one argument.
         *
         * @note If one part only is set to zero, this part will be
         * ignored
         *
         * @sa Double::toString
         */
        String toString() const override;

    private:
        /**
         * The field representing the real part of
         * this complex
         */
        gdouble re = 0.0;

        /**
         * The field representing the imaginary part of
         * this complex
         */
        gdouble im = 0.0;

    public:
        /**
         * Return the real part of this @c Complex.
         *
         * @return the real part of this number.
         */
        gdouble real() const;

        /**
         * Return the imaginary part of this @c Complex.
         *
         * @return the imaginary part of this number.
         */
        gdouble imag() const;

        /**
         * Returns the real part of this number as a @c gint.
         *
         * @return The numeric value represented real part of this object
         * after conversion to type @c gint
         */
        gint intValue() const override;

        /**
         * Returns the real part of this number as a @c glong.
         *
         * @return The numeric value represented real part of this object
         * after conversion to type @c glong
         */
        glong longValue() const override;

        /**
         * Returns the real part of this number as a @c gfloat.
         *
         * @return The numeric value represented real part of this object
         * after conversion to type @c gfloat
         */
        gfloat floatValue() const override;

        /**
         * Returns the real part of this number as a @c gdouble.
         *
         * @return The numeric value represented real part of this object
         * after conversion to type @c gdouble
         */
        gdouble doubleValue() const override;

        /**
         * Returns the real part of this number as a @c gbyte.
         *
         * @return The numeric value represented real part of this object
         * after conversion to type @c gbyte
         */
        gbyte byteValue() const override;

        /**
         * Returns the real part of this number as a @c gshort.
         *
         * @return The numeric value represented real part of this object
         * after conversion to type @c gshort
         */
        gshort shortValue() const override;

        /**
         * Returns the multiplicative inverse of @c this element.
         * @return the inverse of @c this.
         */
        Complex reciprocal() const;

        /**
         * Return the reverse form of this complex.
         * <p>
         * This operation is equivalent to
         * @c ~(z*1i) for all complex @c z.
         *
         * @note After this operation the real part will be
         * an imaginary part and vice versa.
         *
         * @return the reverse form of this number.
         */
        Complex reverse() const;

        /**
         *
         * Return the conjugate form of this complex. For
         * simplify this operation you have possibility to
         * use @a complement operator @c (~), such as:
         * @code
         *     Complex z = 2+3i;
         *     Complex z2 = z.conjugate();
         *     Complex z3 = ~z;
         * @endcode
         *
         * @note After this operation, the sign of
         * imaginary part will be changed by it opposite
         *
         * @return the conjugate form of this complex.
     */

        /**
        * Returns the conjugate of this complex number.
        * The conjugate of @code a + bi @endcode is @code a - bi @endcode.
        * <p>
        * @b NaN is returned if either the real or imaginary
        * part of this Complex number equals @c Double.NaN.
        * </p>
        * <p>
        * If the imaginary part is infinite, and the real part is not
        * @c NaN, the returned value has infinite imaginary part
        * of the opposite sign, e.g. the conjugate of
        * @code 1 + INFi @endcode is @code 1 - INFi @endcode .
        * </p>
        * @return the conjugate of this Complex object.
        */
        Complex conjugate() const;

        /**
         *
         * Return the negate form of this complex. For
         * simplify this operation you have possibility to
         * use a prefixed @a minus operator @c (-), such as:
         * @code
         *     Complex z = 2+3i;
         *     Complex z2 = z.negate();
         *     Complex z3 = -z;
         * @endcode
         *
         * @note After this operation, the sign of
         * all parts will be changed by it opposite
         *
         * @return the conjugate form of this complex.
         */
        Complex negate() const;

        /**
         * Test for equality with another object.
         * If both the real and imaginary parts of two complex numbers
         * are exactly the same, and neither is @c Double::NaN, the two
         * Complex objects are considered to be equal.
         * The behavior is the same as for  @b Double:
         *
         *  @b All @c NaN values are considered to be equal,
         *   i.e, if either (or both) real and imaginary parts of the complex
         *   number are equal to @c Double::NaN, the complex number is equal
         *   to @c NaN.
         *
         *  @b Instances constructed with different representations of zero (i.e.
         *   either "0" or "-0") are <em>not</em> considered to be equal.
         *
         *
         * @param other Object to test for equality with this instance.
         * @return @c true if the objects are equal, @c false if object
         * is @c null, not an instance of @c Complex, or not equal to
         * this instance.
         */
        gbool equals(Object const &other) const override;

        /**
         * Return the hash code value of this @c Complex
         * number
         *
         * @return the hash code value of this complex
         */
        gint hash() const override;

        gint compareTo(Complex const &anotherComplex) const override;

        /**
         * Returns a @c Complex whose value is @code (this + addend) @endcode.
         * Uses the definitional formula
         * <p>
         *   @code (a + bi) + (c + di) = (a+c) + (b+d)i @endcode
         * </p>
         * If either @c this or @c addend has a @c NaN value in
         * either part, @b NaN is returned; otherwise @c Infinite
         * and @c NaN values are returned in the parts of the result
         * according to the rules for @b Double arithmetic.
         *
         * @param  addend Value to be added to this @c Complex.
         * @return @code this + addend @endcode.
         */
        Complex plus(Complex const &addend) const;

        /**
         * Returns a @c Complex whose value is @code (this + addend) @endcode,
         * with @c addend interpreted as a real number.
         *
         * @param addend Value to be added to this @c Complex.
         * @return @code this + addend @endcode.
         * @see plus(Complex)
         */
        Complex plus(gdouble addend) const;

        /**
         * Returns a @c Complex whose value is
         * @code (this - subtrahend) @endcode.
         * Uses the definitional formula
         * <p>
         *  @code (a + bi) - (c + di) = (a-c) + (b-d)i @endcode
         * </p>
         * If either @c this or @c subtrahend has a @c NaN value in either part,
         * @b NaN is returned; otherwise infinite and @c NaN values are
         * returned in the parts of the result according to the rules for
         * @b Double arithmetic.
         *
         * @param  subtrahend value to be subtracted from this @c Complex.
         * @return @code this - subtrahend @endcode.
         */
        Complex subtract(Complex const &subtrahend) const;

        /**
         * Returns a @c Complex whose value is
         * @code (this - subtrahend) @endcode.
         *
         * @param  subtrahend value to be subtracted from this @c Complex.
         * @return @code this - subtrahend @endcode.
         * @see subtract(Complex)
         */
        Complex subtract(gdouble subtrahend) const;

        /**
         * Returns a @c Complex whose value is @code this * factor @endcode.
         * Implements preliminary checks for @c NaN and infinity followed by
         * the definitional formula:
         * <p>
         *   @code (a + bi)(c + di) = (ac - bd) + (ad + bc)i @endcode.
         * </p>
         * Returns @b NaN if either @c this or @c factor has one or
         * more @c NaN parts.
         * <p>
         * Returns @b INF if neither @c this nor @c factor has one
         * or more @c NaN parts and if either @c this or @c factor
         * has one or more infinite parts (same result is returned regardless of
         * the sign of the components).
         * </p><p>
         * Returns finite values in components of the result per the definitional
         * formula in all remaining cases.</p>
         *
         * @param  factor value to be multiplied by this @c Complex.
         * @return @code this * factor @endcode.
         */
        Complex multiply(Complex const &factor) const;

        /**
         * Returns a @c Complex whose value is @code this * factor @endcode, with @c factor
         * interpreted as a real number.
         *
         * @param  factor value to be multiplied by this @c Complex.
         * @return @code this * factor @endcode.
         * @see multiply(Complex)
         */
        Complex multiply(gdouble factor) const;

        /**
         * Returns a @c Complex whose value is
         * @code (this / divisor) @endcode.
         * Implements the definitional formula
         * <p>
         *  @code
         *    a + bi          ac + bd + (bc - ad)i
         *    ----------- = -------------------------
         *    c + di         c<sup>2</sup> + d<sup>2</sup>
         *  @endcode
         * </p>
         * but uses
         * <a href="http://doi.acm.org/10.1145/1039813.1039814">
         * prescaling of operands</a> to limit the effects of overflows and
         * underflows in the computation.
         * <p>
         * @c Infinite and @c NaN values are handled according to the
         * following rules, applied in the order presented: <br/>
         *
         *  - If either @c this or @c divisor has a @c NaN value
         *   in either part, @b NaN is returned. <br/>
         *
         *  - If @c divisor equals @b ZERO, @b NaN is returned. <br/>
         *
         *  - If @c this and @c divisor are both infinite,
         *   @b NaN is returned. <br/>
         *
         *  - If @c this is finite (i.e., has no @c Infinite or
         *   @c NaN parts) and @c divisor is infinite (one or both parts
         *   infinite), @b ZERO is returned. <br/>
         *
         *  - If @c this is infinite and @c divisor is finite,
         *   @c NaN values are returned in the parts of the result if the
         *   @b Double rules applied to the definitional formula
         *   force @c NaN results. <br/>
         *
         * @param divisor Value by which this @c Complex is to be divided.
         * @return @code this / divisor @endcode.
         */
        Complex divide(Complex const &divisor) const;

        /**
         * Returns a @c Complex whose value is @code (this / divisor) @endcode,
         * with @c divisor interpreted as a real number.
         *
         * @param  divisor Value by which this @c Complex is to be divided.
         * @return @code this / divisor @endcode.
         * @see divide(Complex)
         */
        Complex divide(gdouble divisor) const;

        /**
         * Return the absolute value of this complex number.
         * Returns @c NaN if either real or imaginary part is @c NaN
         * and @c Double::POSITIVE_INF if neither part is @c NaN,
         * but at least one part is infinite.
         *
         * @return the absolute value.
         */
        gdouble norm() const;


        /**
         * Compute the argument of this complex number.
         * The argument is the angle phi between the positive real axis and
         * the point representing this number in the complex plane.
         * The value returned is between -PI (not inclusive)
         * and PI (inclusive), with negative values returned for numbers with
         * negative imaginary parts.
         * <p>
         * If either real or imaginary part (or both) is NaN, NaN is returned.
         * Infinite parts are handled as @c Math::atan2 handles them,
         * essentially treating finite parts as zero in the presence of an
         * infinite coordinate and returning a multiple of pi/4 depending on
         * the signs of the infinite parts.
         * See the documentation for @c Math::atan2 for full details.
         *
         * @return the argument of @c this.
         */
        gdouble phase() const;


        /**
         * Compute the argument of this complex number.
         * The argument is the angle phi between the positive real axis and
         * the point representing this number in the complex plane.
         * The value returned is between -PI (not inclusive)
         * and PI (inclusive), with negative values returned for numbers with
         * negative imaginary parts.
         * <p>
         * If either real or imaginary part (or both) is NaN, NaN is returned.
         * Infinite parts are handled as @c Math::atan2 handles them,
         * essentially treating finite parts as zero in the presence of an
         * infinite coordinate and returning a multiple of pi/4 depending on
         * the signs of the infinite parts.
         * See the documentation for @c Math::atan2 for full details.
         *
         * @return the argument of @c this.
         */
        gdouble argument() const;

        /**
         * Computes the n-th roots of this complex number.
         * The nth roots are defined by the formula:
         * <p>
         *  @code
         *   z<sub>k</sub> = abs<sup>1/n</sup> (cos(phi + 2kPI/n) + i (sin(phi + 2kPI/n)))
         *  @endcode
         * </p>
         * for <i>k=0, 1, ..., n-1</i>, where @c norm and @c phi
         * are respectively the @b modulus and @b argument of this complex number.
         * <p>
         * If one or both parts of this complex number is NaN, a list with just
         * one element, @b NaN is returned.
         * if neither part is NaN, but at least one part is infinite, the result
         * is a one-element list containing @b INF.
         *
         * @param n Degree of root.
         * @return a List of all <code>n</code>-th roots of @c this.
         */
        Array<Complex> nthRoot(gint n) const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/Cosine.html" TARGET="_top">
         * cosine</a> of this complex number.
         * Implements the formula:
         * <p>
         *  @code cos(a + bi) = cos(a)cosh(b) - sin(a)sinh(b)i @endcode.
         * </p><p>
         * where the (real) functions on the right-hand side are
         * @b Math::sin, @b Math::cos,
         * @b Math::cosh and @b Math::sinh.
         * </p><p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p><p>
         * Infinite values in real or imaginary parts of the input may result in
         * infinite or NaN values returned in parts of the result.</p>
         * <p>
         *  Examples:
         *  @code
         *   cos(1 + INFi) = 1 \u2213 INF i
         *   cos(+INF + i) = NaN + NaN i
         *   cos(+INF + INFi) = NaN + NaN i
         *  @endcode
         * </p>
         *
         * @return the cosine of this complex number.
         */
        Complex cos() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/InverseCosine.html" TARGET="_top">
         * inverse cosine</a> of this complex number.
         * Implements the formula:
         * <p>
         *  @code acos(z) = -i (log(z + i (sqrt(1 - z<sup>2</sup>)))) @endcode
         * </p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN or infinite.
         *
         * @return the inverse cosine of this complex number.
         */
        Complex acos() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/HyperbolicCosine.html" TARGET="_top">
         * hyperbolic cosine</a> of this complex number.
         * Implements the formula:
         * @code
         *   cosh(a + bi) = cosh(a)cos(b) + sinh(a)sin(b)i
         * @endcode
         * where the (real) functions on the right-hand side are
         * @b Math::sin, @b Math::cos, @b Math::cosh and @b Math::sinh.
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p>
         * Infinite values in real or imaginary parts of the input may result in
         * infinite or NaN values returned in parts of the result.
         * <p>
         *  Examples:
         *  @code
         *   cosh(1 + INFi) = NaN + NaNi
         *   cosh(+INF + i) = INF + INFi
         *   cosh(+INF + INFi) = NaN + NaNi
         *  @endcode
         * </p>
         *
         * @return the hyperbolic cosine of this complex number.
         */
        Complex cosh() const;

        /**
         * Return arc hyperbolic cosine of this @c Complex number
         */
        Complex acosh() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/Sine.html" TARGET="_top">
         * sine</a>
         * of this complex number.
         * Implements the formula:
         * @code
         *   sin(a + bi) = sin(a)cosh(b) - cos(a)sinh(b)i
         * @endcode.
         * where the (real) functions on the right-hand side are
         * @b Math::sin, @b Math::cos, @b Math::cosh and @b Math::sinh.
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p><p>
         * Infinite values in real or imaginary parts of the input may result in
         * infinite or @c NaN values returned in parts of the result.
         * <p>
         *  Examples:
         *  @code
         *   sin(1 + INFi) = 1 + INFi
         *   sin(+INF + i) = NaN + NaNi
         *   sin(+INF + INFi) = NaN + NaNi
         *  @endcode
         * </p>
         *
         * @return the sine of this complex number.
         */
        Complex sin() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/InverseSine.html" TARGET="_top">
         * inverse sine</a> of this complex number.
         * Implements the formula:
         * <p>
         *  @code asin(z) = -i (log(sqrt(1 - z<sup>2</sup>) + iz)) @endcode
         * </p><p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN or infinite.</p>
         *
         * @return the inverse sine of this complex number.
         */
        Complex asin() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/HyperbolicSine.html" TARGET="_top">
         * hyperbolic sine</a> of this complex number.
         * Implements the formula:
         * @code
         *   sinh(a + bi) = sinh(a)cos(b) + cosh(a)sin(b)i
         * @endcode .
         * where the (real) functions on the right-hand side are
         * @b Math::sin, @b Math::cos, @b Math::cosh and @b Math::sinh.
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p><p>
         * Infinite values in real or imaginary parts of the input may result in
         * infinite or NaN values returned in parts of the result.
         * <p>
         *  Examples:
         *  @code
         *   sinh(1 + INFi) = NaN + NaN i
         *   sinh(+INF + i) = + INF + INF i
         *   sinh(+INF + INFi) = NaN + NaN i
         * @endcode
         * </p>
         * @return the hyperbolic sine of @c this.
         */
        Complex sinh() const;

        /**
         * Return arc hyperbolic sine of this @c Complex number
         */
        Complex asinh() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/Tangent.html" TARGET="_top">
         * tangent</a> of this complex number.
         * Implements the formula:
         * @code
         *   tan(a + bi) = sin(2a)/(cos(2a)+cosh(2b)) + [sinh(2b)/(cos(2a)+cosh(2b))]i
         * @endcode .
         * where the (real) functions on the right-hand side are
         * @b Math::sin, @b Math::cos, @b Math::cosh and
         * @b Math::sinh.
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p>
         * Infinite (or critical) values in real or imaginary parts of the input may
         * result in infinite or NaN values returned in parts of the result.
         * <p>
         *  Examples:
         *  @code
         *   tan(a + INFi) = 0 + i
         *   tan(+INF + bi) = NaN + NaNi
         *   tan(+INF + INFi) = NaN + NaNi
         *   tan(+PI/2 + 0i) = +INF + NaNi
         *  @endcode
         * </p>
         *
         * @return the tangent of @c this.
         */
        Complex tan() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/InverseTangent.html" TARGET="_top">
         * inverse tangent</a> of this complex number.
         * Implements the formula:
         * <p>
         * @code atan(z) = (i/2) log((i + z)/(i - z)) @endcode
         * </p><p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN or infinite.</p>
         *
         * @return the inverse tangent of this complex number
         */
        Complex atan() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/HyperbolicTangent.html" TARGET="_top">
         * hyperbolic tangent</a> of this complex number.
         * Implements the formula:
         * @code
         *   tan(a + bi) = sinh(2a)/(cosh(2a)+cos(2b)) + [sin(2b)/(cosh(2a)+cos(2b))]i
         * @endcode.
         * where the (real) functions on the right-hand side are
         * @b Math::sin, @b Math::cos, @b Math::cosh and
         * @b Math::sinh.
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p>
         * Infinite values in real or imaginary parts of the input may result in
         * infinite or NaN values returned in parts of the result.
         * <p>
         *  Examples:
         *  @code
         *   tanh(a + INFi) = NaN + NaN i
         *   tanh(+INF + bi) = +1 + 0 i
         *   tanh(+INF + INFi) = NaN + NaN i
         *   tanh(0 + (PI/2)i) = NaN + INF i
         * @endcode
         * </p>
         * @return the hyperbolic tangent of @c this.
         */
        Complex tanh() const;

        /**
         * Return arc hyperbolic tangent of this @c Complex number
         */
        Complex atanh() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/ExponentialFunction.html" TARGET="_top">
         * exponential function</a> of this complex number.
         * Implements the formula:
         * @code
         *   exp(a + bi) = exp(a)cos(b) + exp(a)sin(b)i
         * @endcode .
         * where the (real) functions on the right-hand side are
         * @b Math::exp, @b Math::cos, and
         * @b Math::sin.
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p>
         * Infinite values in real or imaginary parts of the input may result in
         * infinite or NaN values returned in parts of the result.
         * <p>
         *  Examples:
         *  @code
         *   exp(1 + INFi) = NaN + NaNi
         *   exp(INF + i) = INF + INF i
         *   exp(-INF + i) = 0 + 0i
         *   exp(+INF + INFi) = NaN + NaNi
         * @endcode
         * </p>
         * @return <code><i>e</i><sup>this</sup></code>.
         * @since 1.2
         */
        Complex exp() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/NaturalLogarithm.html" TARGET="_top">
         * natural logarithm</a> of this complex number.
         * Implements the formula:
         * @code
         *   log(a + bi) = ln(|a + bi|) + arg(a + bi)i
         * @endcode.
         * where ln on the right hand side is @b Math::log,
         * @code |a + bi| @endcode is the modulus, @b Complex::abs,  and
         * @code arg(a + bi) = @endcode @b Math::atan2(b, a).
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p>
         * Infinite (or critical) values in real or imaginary parts of the input may
         * result in infinite or NaN values returned in parts of the result.
         * <p>
         *  Examples:
         *  @code
         *   log(1 + INFi) = INF + (PI/2)i .
         *   log(INF + i) = INF + 0i
         *   log(-INF + i) = INF + PIi
         *   log(INF + INFi) = INF + (PI/4)i .
         *   log(-INF + INFi) = INF + (3PI/4)i .
         *   log(0 + 0i) = -INF + 0i
         *  @endcode
         * </p>
         *
         * @return the value <code>ln &nbsp; this</code>, the natural logarithm
         * of @c this.
         */
        Complex log() const;

        /**
         * Returns of value of this complex number raised to the power of @c x.
         * Implements the formula:
         * @code
         *   y<sup>x</sup> = exp(x&middot;log(y))
         * @endcode
         * where @c exp and @c log are @b exp and
         * @b log, respectively.
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN or infinite, or if @c y
         * equals @b Complex::ZERO.</p>
         *
         * @param  x exponent to which this @c Complex is to be raised.
         * @return <code> this<sup>x</sup></code>.
         */
        Complex pow(Complex const &x) const;

        /**
         * Returns of value of this complex number raised to the power of @c x.
         *
         * @param  x exponent to which this @c Complex is to be raised.
         * @return <code>this<sup>x</sup></code>.
         * @see pow(Complex)
         */
        Complex pow(gdouble x) const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/SquareRoot.html" TARGET="_top">
         * square root</a> of this complex number.
         * Implements the following algorithm to compute @code sqrt(a + bi) @endcode : <br/>
         * - Let @code t = sqrt((|a| + |a + bi|) / 2) @endcode <br/>
         * - if @code  a &#8805; 0 @endcode return @code t + (b/2t)i @endcode
         *  else return @code |b|/2t + sign(b)t i  @endcode . <br/>
         *
         * where
         * - @code |a| = @endcode @b Math::abs(a) <br/>
         * - @code |a + bi| = @endcode @b Complex::abs(a + bi) <br/>
         * - @code sign(b) =  @endcode @b copySign(1d, b) <br/>
         *
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p>
         * Infinite values in real or imaginary parts of the input may result in
         * infinite or NaN values returned in parts of the result.
         * <p>
         *  Examples:
         *  @code
         *   sqrt(1 + INFi) = INF + NaNi
         *   sqrt(INF + i) = INF + 0i
         *   sqrt(-INF + i) = 0 + INF i
         *   sqrt(INF + INFi) = INF + NaNi
         *   sqrt(-INF + INFi) = NaN + INF i
         *  @endcode
         * </p>
         *
         * @return the square root of @c this.
         * @since 1.2
         */
        Complex sqrt() const;

        /**
         * Compute the
         * <a href="http://mathworld.wolfram.com/SquareRoot.html" TARGET="_top">
         * square root</a> of <code>1 - this<sup>2</sup></code> for this complex
         * number.
         * Computes the result directly as
         * @code sqrt(ONE.subtract(z.multiply(z))) @endcode .
         * <p>
         * Returns @b Complex::NaN if either real or imaginary part of the
         * input argument is @c NaN.
         * </p>
         * Infinite values in real or imaginary parts of the input may result in
         * infinite or NaN values returned in parts of the result.
         *
         * @return the square root of <code>1 - this<sup>2</sup></code>.
         */
        Complex sqrt1z() const;

        /**
         * Obtain newly created @c Complex object with
         * given polar coordinates.
         *
         * @param rho the distance from zero (norm).
         * @param phi the phase angle.
         *
         * @sa Complex::norm
         * @sa Complex::phase
         */
        static Complex polar(gdouble rho, gdouble phi);

        /**
         * Return the shadow copy of this object.
         *
         * @return the shadow copy of this object
         */
        Object &clone() const override;

        /**
         * The number of bits used to represent a @c gbyte value in two's
         * complement binary form.
         */
        static CORE_FAST gint SIZE = 1 << 7;

        /**
         * The number of bytes used to represent a @c gbyte value in two's
         * complement binary form.
         */
        static CORE_FAST gint BYTES = SIZE / Byte::SIZE;

    private:
        static Complex initLiteral(glong literal, gint memSize, gbool isFloating);
    };
} // core

#endif //CORE24_COMPLEX_H
