//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_FLOAT_H
#define CORE24_FLOAT_H

#include <core/Math.h>
#include <core/Double.h>

namespace core {
    /**
     * The @c Float class wraps a value of primitive type
     * @c gfloat in an object. An object of type
     * @c Float contains a single field whose type is
     * @c gfloat.
     */
    class Float final : public virtual Number, public virtual Comparable<Float> {
    public:
        /**
         * A constant holding the smallest of type
         * @c gfloat such that 1.0f + EPSILON != 1.0f . It is equal to the value returned by
         * @c Float::fromIntBits(0x34000000).
         */
        static CORE_FAST gfloat EPSILON = 0x1.0P-23F; //1.192092896E-7F;
        /**
         * A constant holding the positive infinity of type
         * @c gfloat. It is equal to the value returned by
         * @c Float::fromIntBits(0x7f800000).
         */
        static CORE_FAST gfloat POSITIVE_INFINITY = CORE_FCAST(gfloat, Math::INF);

        /**
         * A constant holding the negative infinity of type
         * @c gfloat. It is equal to the value returned by
         * @c Float::fromIntBits(0xff800000).
         */
        static CORE_FAST gfloat NEGATIVE_INFINITY = -POSITIVE_INFINITY;

        /**
         * A constant holding a Not-a-Number (NaN) value of type
         * @c gfloat.  It is equivalent to the value returned by
         * @c Float::fromIntBits(0x7fc00000).
         */
        static CORE_FAST gfloat const NaN = CORE_CAST(gfloat, Math::NaN);

        /**
         * A constant holding the largest positive finite value of type
         * @c gfloat, (2-2<sup>-23</sup>)&middot;2<sup>127</sup>.
         * It is equal to the hexadecimal floating-point literal
         * @c 0x1.fffffeP+127f and also equal to
         * @c Float::fromIntBits(0x7f7fffff).
         */
        static CORE_FAST gfloat MAX_VALUE = 0x1.FFFFFEP+127F; // 3.4028235e+38f

        /**
         * A constant holding the smallest positive normal value of type
         * @c gfloat, 2<sup>-126</sup>.  It is equal to the
         * hexadecimal floating-point literal @c 0x1.0p-126f and also
         * equal to @c Float::fromIntBits(0x00800000).
         *
         *
         */
        static CORE_FAST gfloat MIN_NORMAL = 0x1.0P-126F; // 1.17549435E-38f

        /**
         * A constant holding the smallest positive nonzero value of type
         * @c gfloat, 2<sup>-149</sup>. It is equal to the
         * hexadecimal floating-point literal @c 0x0.000002P-126f
         * and also equal to @c Float::fromIntBits(0x1).
         */
        static CORE_FAST gfloat MIN_VALUE = 0x0.000002P-126F; // 1.4e-45f

        /**
         * The number of bits used to represent a @c gfloat value.
         *
         *
         */
        static CORE_FAST gint SIZE = 32;

        /**
         * The number of bits in the significand of a @c gfloat value.
         * This is the parameter N in section {@jls 4.2.3 of
         * <cite>The Java Language Specification</cite>.
         *
         *
         */
        static CORE_FAST gint PRECISION = 24;

        /**
         * Maximum exponent a finite @c gfloat variable may have.  It
         * is equal to the value returned by @c 
         * Math.getExponent(Float.MAX_VALUE).
         *
         *
         */
        static CORE_FAST gint MAX_EXPONENT = (1 << (SIZE - PRECISION - 1)) - 1; // 127

        /**
         * Minimum exponent a normalized @c gfloat variable may have.
         * It is equal to the value returned by @c 
         * Math.getExponent(Float.MIN_NORMAL).
         *
         *
         */
        static CORE_FAST gint MIN_EXPONENT = 1 - MAX_EXPONENT; // -126

        /**
         * The number of bytes used to represent a @c gfloat value.
         *
         *
         */
        static CORE_FAST gint BYTES = SIZE / Byte::SIZE;

        /**
         * Returns a string representation of the @c gfloat
         * argument. All characters mentioned below are ASCII characters.
         * <ul>
         * <li>If the argument is NaN, the result is the string
         * "@c NaN".
         * <li>Otherwise, the result is a string that represents the sign and
         *     magnitude (absolute value) of the argument. If the sign is
         *     negative, the first character of the result is
         *     '@c -' (@c '&#92;u002D'); if the sign is
         *     positive, no sign character appears in the result. As for
         *     the magnitude <i>m</i>:
         * <ul>
         * <li>If <i>m</i> is infinity, it is represented by the characters
         *     @c "Infinity"; thus, positive infinity produces
         *     the result @c "Infinity" and negative infinity
         *     produces the result @c "-Infinity".
         * <li>If <i>m</i> is zero, it is represented by the characters
         *     @c "0.0"; thus, negative zero produces the result
         *     @c "-0.0" and positive zero produces the result
         *     @c "0.0".
         *
         * <li> Otherwise <i>m</i> is positive and finite.
         * It is converted to a string in two stages:
         * <ul>
         * <li> <em>Selection of a decimal</em>:
         * A well-defined decimal <i>d</i><sub><i>m</i></sub>
         * is selected to represent <i>m</i>.
         * This decimal is (almost always) the <em>shortest</em> one that
         * rounds to <i>m</i> according to the round to nearest
         * rounding policy of IEEE 754 floating-point arithmetic.
         * <li> <em>Formatting as a string</em>:
         * The decimal <i>d</i><sub><i>m</i></sub> is formatted as a string,
         * either in plain or in computerized scientific notation,
         * depending on its value.
         * </ul>
         * </ul>
         * </ul>
         *
         * <p>A <em>decimal</em> is a number of the form
         * <i>s</i>&times;10<sup><i>i</i></sup>
         * for some (unique) integers <i>s</i> &gt; 0 and <i>i</i> such that
         * <i>s</i> is not a multiple of 10.
         * These integers are the <em>significand</em> and
         * the <em>exponent</em>, respectively, of the decimal.
         * The <em>length</em> of the decimal is the (unique)
         * positive integer <i>n</i> meeting
         * 10<sup><i>n</i>-1</sup> &le; <i>s</i> &lt; 10<sup><i>n</i></sup>.
         *
         * <p>The decimal <i>d</i><sub><i>m</i></sub> for a finite positive <i>m</i>
         * is defined as follows:
         * <ul>
         * <li>Let <i>R</i> be the set of all decimals that round to <i>m</i>
         * according to the usual <em>round to nearest</em> rounding policy of
         * IEEE 754 floating-point arithmetic.
         * <li>Let <i>p</i> be the minimal length over all decimals in <i>R</i>.
         * <li>When <i>p</i> &ge; 2, let <i>T</i> be the set of all decimals
         * in <i>R</i> with length <i>p</i>.
         * Otherwise, let <i>T</i> be the set of all decimals
         * in <i>R</i> with length 1 or 2.
         * <li>Define <i>d</i><sub><i>m</i></sub> as the decimal in <i>T</i>
         * that is closest to <i>m</i>.
         * Or if there are two such decimals in <i>T</i>,
         * select the one with the even significand.
         * </ul>
         *
         * <p>The (uniquely) selected decimal <i>d</i><sub><i>m</i></sub>
         * is then formatted.
         * Let <i>s</i>, <i>i</i> and <i>n</i> be the significand, exponent and
         * length of <i>d</i><sub><i>m</i></sub>, respectively.
         * Further, let <i>e</i> = <i>n</i> + <i>i</i> - 1 and let
         * <i>s</i><sub>1</sub>&hellip;<i>s</i><sub><i>n</i></sub>
         * be the usual decimal expansion of <i>s</i>.
         * Note that <i>s</i><sub>1</sub> &ne; 0
         * and <i>s</i><sub><i>n</i></sub> &ne; 0.
         * Below, the decimal point @c '.' is @c '&#92;u002E'
         * and the exponent indicator @c 'E' is @c '&#92;u0045'.
         * <ul>
         * <li>Case -3 &le; <i>e</i> &lt; 0:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <code>0.0</code>&hellip;<code>0</code><!--
         * --><i>s</i><sub>1</sub>&hellip;<i>s</i><sub><i>n</i></sub>,
         * where there are exactly -(<i>n</i> + <i>i</i>) zeroes between
         * the decimal point and <i>s</i><sub>1</sub>.
         * For example, 123 &times; 10<sup>-4</sup> is formatted as
         * @c 0.0123.
         * <li>Case 0 &le; <i>e</i> &lt; 7:
         * <ul>
         * <li>Subcase <i>i</i> &ge; 0:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>s</i><sub>1</sub>&hellip;<i>s</i><sub><i>n</i></sub><!--
         * --><code>0</code>&hellip;<code>0.0</code>,
         * where there are exactly <i>i</i> zeroes
         * between <i>s</i><sub><i>n</i></sub> and the decimal point.
         * For example, 123 &times; 10<sup>2</sup> is formatted as
         * @c 12300.0.
         * <li>Subcase <i>i</i> &lt; 0:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>s</i><sub>1</sub>&hellip;<!--
         * --><i>s</i><sub><i>n</i>+<i>i</i></sub><code>.</code><!--
         * --><i>s</i><sub><i>n</i>+<i>i</i>+1</sub>&hellip;<!--
         * --><i>s</i><sub><i>n</i></sub>,
         * where there are exactly -<i>i</i> digits to the right of
         * the decimal point.
         * For example, 123 &times; 10<sup>-1</sup> is formatted as
         * @c 12.3.
         * </ul>
         * <li>Case <i>e</i> &lt; -3 or <i>e</i> &ge; 7:
         * computerized scientific notation is used to format
         * <i>d</i><sub><i>m</i></sub>.
         * Here <i>e</i> is formatted as by @c Integer::toString(int).
         * <ul>
         * <li>Subcase <i>n</i> = 1:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>s</i><sub>1</sub><code>.0E</code><i>e</i>.
         * For example, 1 &times; 10<sup>23</sup> is formatted as
         * @c 1.0E23.
         * <li>Subcase <i>n</i> &gt; 1:
         * <i>d</i><sub><i>m</i></sub> is formatted as
         * <i>s</i><sub>1</sub><code>.</code><i>s</i><sub>2</sub><!--
         * -->&hellip;<i>s</i><sub><i>n</i></sub><code>E</code><i>e</i>.
         * For example, 123 &times; 10<sup>-21</sup> is formatted as
         * @c 1.23E-19.
         * </ul>
         * </ul>
         *
         * @param   f   the @c gfloat to be converted.
         * @return a string representation of the argument.
         */
        static String toString(gfloat f);

        /**
         * Returns a hexadecimal string representation of the
         * @c gfloat argument. All characters mentioned below are
         * ASCII characters.
         *
         * <ul>
         * <li>If the argument is NaN, the result is the string
         *     "@c NaN".
         * <li>Otherwise, the result is a string that represents the sign and
         * magnitude (absolute value) of the argument. If the sign is negative,
         * the first character of the result is '@c -'
         * (@c '&#92;u002D'); if the sign is positive, no sign character
         * appears in the result. As for the magnitude <i>m</i>:
         *
         * <ul>
         * <li>If <i>m</i> is infinity, it is represented by the string
         * @c "Infinity"; thus, positive infinity produces the
         * result @c "Infinity" and negative infinity produces
         * the result @c "-Infinity".
         *
         * <li>If <i>m</i> is zero, it is represented by the string
         * @c "0x0.0p0"; thus, negative zero produces the result
         * @c "-0x0.0p0" and positive zero produces the result
         * @c "0x0.0p0".
         *
         * <li>If <i>m</i> is a @c gfloat value with a
         * normalized representation, sub-strings are used to represent the
         * significand and exponent fields.  The significand is
         * represented by the characters @c "0x1."
         * followed by a lowercase hexadecimal representation of the rest
         * of the significand as a fraction.  Trailing zeros in the
         * hexadecimal representation are removed unless all the digits
         * are zero, in which case a single zero is used. Next, the
         * exponent is represented by @c "p" followed
         * by a decimal string of the unbiased exponent as if produced by
         * a call to @c Integer::toString on the
         * exponent value.
         *
         * <li>If <i>m</i> is a @c gfloat value with a subnormal
         * representation, the significand is represented by the
         * characters @c "0x0." followed by a
         * hexadecimal representation of the rest of the significand as a
         * fraction.  Trailing zeros in the hexadecimal representation are
         * removed. Next, the exponent is represented by
         * @c "p-126".  Note that there must be at
         * least one nonzero digit in a subnormal significand.
         *
         * </ul>
         *
         * </ul>
         *
         * <table class="striped">
         * <caption>Examples</caption>
         * <thead>
         * <tr><th scope="col">Floating-point Value</th><th scope="col">Hexadecimal String</th>
         * </thead>
         * <tbody>
         * <tr><th scope="row">@c 1.0</th> <td>@c 0x1.0p0</td>
         * <tr><th scope="row">@c -1.0</th>        <td>@c -0x1.0p0</td>
         * <tr><th scope="row">@c 2.0</th> <td>@c 0x1.0p1</td>
         * <tr><th scope="row">@c 3.0</th> <td>@c 0x1.8p1</td>
         * <tr><th scope="row">@c 0.5</th> <td>@c 0x1.0p-1</td>
         * <tr><th scope="row">@c 0.25</th>        <td>@c 0x1.0p-2</td>
         * <tr><th scope="row">@c Float.MAX_VALUE</th>
         *     <td>@c 0x1.fffffep127</td>
         * <tr><th scope="row">@c Minimum Normal Value</th>
         *     <td>@c 0x1.0p-126</td>
         * <tr><th scope="row">@c Maximum Subnormal Value</th>
         *     <td>@c 0x0.fffffep-126</td>
         * <tr><th scope="row">@c Float.MIN_VALUE</th>
         *     <td>@c 0x0.000002p-126</td>
         * </tbody>
         * </table>
         * @param   f   the @c gfloat to be converted.
         * @return a hex string representation of the argument.
         *
         * @author Joseph D. Darcy
         */
        static String toHexString(gfloat f);

        /**
         * Returns a @c Float object holding the
         * @c gfloat value represented by the argument string
         * @c s.
         *
         * <p>If @c s is @c null, then a
         * @c NullPointerException is thrown.
         *
         * <p>Leading and trailing whitespace characters in @c s
         * are ignored.  Whitespace is removed as if by the @c
         * String::trim method; that is, both ASCII space and control
         * characters are removed. The rest of @c s should
         * constitute a <i>FloatValue</i> as described by the lexical
         * syntax rules:
         *
         * <blockquote>
         * <dl>
         * <dt><i>FloatValue:</i>
         * <dd><i>Sign<sub>opt</sub></i> @c NaN
         * <dd><i>Sign<sub>opt</sub></i> @c Infinity
         * <dd><i>Sign<sub>opt</sub> FloatingPointLiteral</i>
         * <dd><i>Sign<sub>opt</sub> HexFloatingPointLiteral</i>
         * <dd><i>SignedInteger</i>
         * </dl>
         *
         * <dl>
         * <dt><i>HexFloatingPointLiteral</i>:
         * <dd> <i>HexSignificand BinaryExponent FloatTypeSuffix<sub>opt</sub></i>
         * </dl>
         *
         * <dl>
         * <dt><i>HexSignificand:</i>
         * <dd><i>HexNumeral</i>
         * <dd><i>HexNumeral</i> @c .
         * <dd>@c 0x <i>HexDigits<sub>opt</sub>
         *     </i>@c .<i> HexDigits</i>
         * <dd>@c 0X<i> HexDigits<sub>opt</sub>
         *     </i>@c . <i>HexDigits</i>
         * </dl>
         *
         * <dl>
         * <dt><i>BinaryExponent:</i>
         * <dd><i>BinaryExponentIndicator SignedInteger</i>
         * </dl>
         *
         * <dl>
         * <dt><i>BinaryExponentIndicator:</i>
         * <dd>@c p
         * <dd>@c P
         * </dl>
         *
         * </blockquote>
         *
         * where <i>Sign</i>, <i>FloatingPointLiteral</i>,
         * <i>HexNumeral</i>, <i>HexDigits</i>, <i>SignedInteger</i> and
         * <i>FloatTypeSuffix</i> are as defined in the lexical structure
         * sections of
         * <cite>The Java Language Specification</cite>,
         * except that underscores are not accepted between digits.
         * If @c s does not have the form of
         * a <i>FloatValue</i>, then a @c NumberFormatException
         * is thrown. Otherwise, @c s is regarded as
         * representing an exact decimal value in the usual
         * "computerized scientific notation" or as an exact
         * hexadecimal value; this exact numerical value is then
         * conceptually converted to an "infinitely precise"
         * binary value that is then rounded to type @c gfloat
         * by the usual round-to-nearest rule of IEEE 754 floating-point
         * arithmetic, which includes preserving the sign of a zero
         * value.
         *
         * Note that the round-to-nearest rule also implies overflow and
         * underflow behaviour; if the exact value of @c s is large
         * enough in magnitude (greater than or equal to (@c MAX_VALUE + @c Math::ulp(MAX_VALUE)/2),
         * rounding to @c gfloat will result in an infinity and if the
         * exact value of @c s is small enough in magnitude (less
         * than or equal to @c MIN_VALUE/2), rounding to gfloat will
         * result in a zero.
         *
         * Finally, after rounding a @c Float object representing
         * this @c gfloat value is returned.
         *
         * <p>Note that trailing format specifiers, specifiers that
         * determine the type of a floating-point literal
         * (@c 1.0f is a @c gfloat value;
         * @c 1.0d is a @c double value), do
         * <em>not</em> influence the results of this method.  In other
         * words, the numerical value of the input string is converted
         * directly to the target floating-point type.  In general, the
         * two-step sequence of conversions, string to @c double
         * followed by @c double to @c gfloat, is
         * <em>not</em> equivalent to converting a string directly to
         * @c gfloat.  For example, if first converted to an
         * intermediate @c double and then to
         * @c gfloat, the string<br>
         * @c "1.00000017881393421514957253748434595763683319091796875001"<br>
         * results in the @c gfloat value
         * @c 1.0000002f; if the string is converted directly to
         * @c gfloat, <code>1.000000@c 1f</code> results.
         *
         * <p>To avoid calling this method on an invalid string and having
         * a @c NumberFormatException be thrown, the documentation
         * for @c Double::valueOf lists a regular
         * expression which can be used to screen the input.
         *
         * @param   s   the string to be parsed.
         * @return  a @c Float object holding the value
         *          represented by the @c String argument.
         * @throws  NumberFormatException  if the string does not contain a
         *          parsable number.
         */
        static Float valueOf(String const &s);

        /**
         * Returns a @c Float instance representing the specified
         * @c gfloat value.
         * If a new @c Float instance is not required, this method
         * should generally be used in preference to the constructor
         * @c Float(gfloat), as this method is likely to yield
         * significantly better space and time performance by caching
         * frequently requested values.
         *
         * @param  f a gfloat value.
         * @return a @c Float instance representing @c f.
         *
         */
        static Float valueOf(gfloat f);

        /**
         * Returns a new @c gfloat initialized to the value
         * represented by the specified @c String, as performed
         * by the @c valueOf method of class @c Float.
         *
         * @param  s the string to be parsed.
         * @return the @c gfloat value represented by the string
         *         argument.
         *
         * @throws NumberFormatException if the string does not contain a
         *               parsable @c gfloat.
         * @see    Float#valueOf(String)
         *
         */
        static gfloat parseFloat(String const &s);

        /**
         * Returns @c true if the specified number is a
         * Not-a-Number (NaN) value, @c false otherwise.
         *
         * @apiNote
         * This method corresponds to the isNaN operation defined in IEEE
         * 754.
         *
         * @param   v   the value to be tested.
         * @return  @c true if the argument is NaN;
         *          @c false otherwise.
         */
        static gbool isNaN(gfloat v);

        /**
         * Returns @c true if the specified number is infinitely
         * large in magnitude, @c false otherwise.
         *
         * @apiNote
         * This method corresponds to the isInfinite operation defined in
         * IEEE 754.
         *
         * @param   v   the value to be tested.
         * @return  @c true if the argument is positive infinity or
         *          negative infinity; @c false otherwise.
         */
        static gbool isInfinite(gfloat v);


        /**
         * Returns @c true if the argument is a finite floating-point
         * value; returns @c false otherwise (for NaN and infinity
         * arguments).
         *
         * @apiNote
         * This method corresponds to the isFinite operation defined in
         * IEEE 754.
         *
         * @param f the @c gfloat value to be tested
         * @return @c true if the argument is a finite
         * floating-point value, @c false otherwise.
         *
         */
        static gbool isFinite(gfloat f);

    private:
        /**
         * The value of the Float.
         *
         * @serial
         */
        gfloat value = 0.0f;

    public:
        /**
         * Construct new instance of @c Float with value positive zero.
         */
        CORE_IMPLICIT Float() CORE_NOTHROW;

        /**
         * Constructs a newly allocated @c Float object that
         * represents the primitive @c gfloat argument.
         *
         * @param   value   the value to be represented by the @c Float.
         *
         * @deprecated
         * It is rarely appropriate to use this constructor. The static factory
         * @c valueOf(gfloat) is generally a better choice, as it is
         * likely to yield significantly better space and time performance.
         */
        CORE_IMPLICIT Float(gfloat value);

        /**
         * Constructs a newly allocated @c Float object that
         * represents the floating-point value of type @c gfloat
         * represented by the string. The string is converted to a
         * @c gfloat value as if by the @c valueOf method.
         *
         * @param   s   a string to be converted to a @c Float.
         * @throws      NumberFormatException if the string does not contain a
         *              parsable number.
         *
         * @deprecated
         * It is rarely appropriate to use this constructor.
         * Use @c parseFloat(String) to convert a string to a
         * @c gfloat primitive, or use @c valueOf(String)
         * to convert a string to a @c Float object.
         */
        CORE_EXPLICIT Float(String const &s);

        /**
         * Returns @c true if this @c Float value is a
         * Not-a-Number (NaN), @c false otherwise.
         *
         * @return  @c true if the value represented by this object is
         *          NaN; @c false otherwise.
         */
        gbool isNaN() const;

        /**
         * Returns @c true if this @c Float value is
         * infinitely large in magnitude, @c false otherwise.
         *
         * @return  @c true if the value represented by this object is
         *          positive infinity or negative infinity;
         *          @c false otherwise.
         */
        gbool isInfinite() const;

        /**
         * Returns a string representation of this @c Float object.
         * The primitive @c gfloat value represented by this object
         * is converted to a @c String exactly as if by the method
         * @c toString of one argument.
         *
         * @return  a @c String representation of this object.
         * @see Float#toString(gfloat)
         */
        String toString() const override;

        /**
         * Returns the value of this @c Float as a @c byte after
         * a narrowing primitive conversion.
         *
         * @return  the @c gfloat value represented by this object
         *          converted to type @c byte
         * @jls 5.1.3 Narrowing Primitive Conversion
         */
        gbyte byteValue() const override;

        /**
         * Returns the value of this @c Float as a @c short
         * after a narrowing primitive conversion.
         *
         * @return  the @c gfloat value represented by this object
         *          converted to type @c short
         * @jls 5.1.3 Narrowing Primitive Conversion
         *
         */
        gshort shortValue() const override;

        /**
         * Returns the value of this @c Float as an @c int after
         * a narrowing primitive conversion.
         *
         * @return  the @c gfloat value represented by this object
         *          converted to type @c int
         * @jls 5.1.3 Narrowing Primitive Conversion
         */
        gint intValue() const override;

        /**
         * Returns value of this @c Float as a @c long after a
         * narrowing primitive conversion.
         *
         * @return  the @c gfloat value represented by this object
         *          converted to type @c long
         * @jls 5.1.3 Narrowing Primitive Conversion
         */
        glong longValue() const override;

        /**
         * Returns the @c gfloat value of this @c Float object.
         *
         * @return the @c gfloat value represented by this object
         */
        gfloat floatValue() const override;

        /**
         * Returns the value of this @c Float as a @c double
         * after a widening primitive conversion.
         *
         * @apiNote
         * This method corresponds to the convertFormat operation defined
         * in IEEE 754.
         *
         * @return the @c gfloat value represented by this
         *         object converted to type @c double
         * @jls 5.1.2 Widening Primitive Conversion
         */
        gdouble doubleValue() const override;

        /**
         * Returns a hash code for this @c Float object. The
         * result is the integer bit representation, exactly as produced
         * by the method @c toIntBits(gfloat), of the primitive
         * @c gfloat value represented by this @c Float
         * object.
         *
         * @return a hash code value for this object.
         */
        gint hash() const override;

        /**
         * Returns a hash code for a @c gfloat value; compatible with
         * @c Float::hash().
         *
         * @param value the value to hash
         * @return a hash code value for a @c gfloat value.
         *
         */
        static gint hash(gfloat value);

        /**
         * Compares this object against the specified object.  The result
         * is @c true if and only if the argument is not
         * @c null and is a @c Float object that
         * represents a @c gfloat with the same value as the
         * @c gfloat represented by this object. For this
         * purpose, two @c gfloat values are considered to be the
         * same if and only if the method @c toIntBits(gfloat)
         * returns the identical @c int value when applied to
         * each.
         *
         * @apiNote
         * This method is defined in terms of @c toIntBits(gfloat)
         * rather than the @c == operator on @c gfloat values since the @c == operator does
         * <em>not</em> define an equivalence relation and to satisfy the
         * @c equals contract an equivalence relation must be implemented;
         *
         * @param obj the object to be compared
         * @return  @c true if the objects are the same;
         *          @c false otherwise.
         * @see Float#toIntBits(gfloat)
         * @jls 15.21.1 Numerical Equality Operators == and !=
         */
        gbool equals(Object const &obj) const override;

        /**
         * Returns a representation of the specified floating-point value
         * according to the IEEE 754 floating-point "single format" bit
         * layout.
         *
         * <p>Bit 31 (the bit that is selected by the mask
         * @c 0x80000000) represents the sign of the floating-point
         * number.
         * Bits 30-23 (the bits that are selected by the mask
         * @c 0x7f800000) represent the exponent.
         * Bits 22-0 (the bits that are selected by the mask
         * @c 0x007fffff) represent the significand (sometimes called
         * the mantissa) of the floating-point number.
         *
         * <p>If the argument is positive infinity, the result is
         * @c 0x7f800000.
         *
         * <p>If the argument is negative infinity, the result is
         * @c 0xff800000.
         *
         * <p>If the argument is NaN, the result is @c 0x7fc00000.
         *
         * <p>In all cases, the result is an integer that, when given to the
         * @c fromIntBits(int) method, will produce a floating-point
         * value the same as the argument to @c toIntBits
         * (except all NaN values are collapsed to a single
         * "canonical" NaN value).
         *
         * @param   value   a floating-point number.
         * @return the bits that represent the floating-point number.
         */
        static gint toIntBits(gfloat value);

        /**
         * Returns a representation of the specified floating-point value
         * according to the IEEE 754 floating-point "single format" bit
         * layout, preserving Not-a-Number (NaN) values.
         *
         * <p>Bit 31 (the bit that is selected by the mask
         * @c 0x80000000) represents the sign of the floating-point
         * number.
         * Bits 30-23 (the bits that are selected by the mask
         * @c 0x7f800000) represent the exponent.
         * Bits 22-0 (the bits that are selected by the mask
         * @c 0x007fffff) represent the significand (sometimes called
         * the mantissa) of the floating-point number.
         *
         * <p>If the argument is positive infinity, the result is
         * @c 0x7f800000.
         *
         * <p>If the argument is negative infinity, the result is
         * @c 0xff800000.
         *
         * <p>If the argument is NaN, the result is the integer representing
         * the actual NaN value.  Unlike the @c toIntBits
         * method, @c toRawIntBits does not collapse all the
         * bit patterns encoding a NaN to a single "canonical"
         * NaN value.
         *
         * <p>In all cases, the result is an integer that, when given to the
         * @c fromIntBits(int) method, will produce a
         * floating-point value the same as the argument to
         * @c toRawIntBits.
         *
         * @param   value   a floating-point number.
         * @return the bits that represent the floating-point number.
         *
         */
        static gint toRawIntBits(gfloat value);

        /**
         * Returns the @c gfloat value corresponding to a given
         * bit representation.
         * The argument is considered to be a representation of a
         * floating-point value according to the IEEE 754 floating-point
         * "single format" bit layout.
         *
         * <p>If the argument is @c 0x7f800000, the result is positive
         * infinity.
         *
         * <p>If the argument is @c 0xff800000, the result is negative
         * infinity.
         *
         * <p>If the argument is any value in the range
         * @c 0x7f800001 through @c 0x7fffffff or in
         * the range @c 0xff800001 through
         * @c 0xffffffff, the result is a NaN.  No IEEE 754
         * floating-point operation provided by Java can distinguish
         * between two NaN values of the same type with different bit
         * patterns.  Distinct values of NaN are only distinguishable by
         * use of the @c Float::toRawIntBits method.
         *
         * <p>In all other cases, let <i>s</i>, <i>e</i>, and <i>m</i> be three
         * values that can be computed from the argument:
         *
         * @c  @code
         * int s = ((bits >> 31) == 0) ? 1 : -1;
         * int e = ((bits >> 23) & 0xff);
         * int m = (e == 0) ?
         *                 (bits & 0x7fffff) << 1 :
         *                 (bits & 0x7fffff) | 0x800000;
         * @endcode 
         *
         * Then the floating-point result equals the value of the mathematical
         * expression <i>s</i>&middot;<i>m</i>&middot;2<sup><i>e</i>-150</sup>.
         *
         * <p>Note that this method may not be able to return a
         * @c gfloat NaN with exactly same bit pattern as the
         * @c int argument.  IEEE 754 distinguishes between two
         * kinds of NaNs, quiet NaNs and <i>signaling NaNs</i>.  The
         * differences between the two kinds of NaN are generally not
         * visible in Java.  Arithmetic operations on signaling NaNs turn
         * them into quiet NaNs with a different, but often similar, bit
         * pattern.  However, on some processors merely copying a
         * signaling NaN also performs that conversion.  In particular,
         * copying a signaling NaN to return it to the calling method may
         * perform this conversion.  So @c fromIntBits may
         * not be able to return a @c gfloat with a signaling NaN
         * bit pattern.  Consequently, for some @c int values,
         * @c toRawIntBits(fromIntBits(start)) may
         * <i>not</i> equal @c start.  Moreover, which
         * particular bit patterns represent signaling NaNs is platform
         * dependent; although all NaN bit patterns, quiet or signaling,
         * must be in the NaN range identified above.
         *
         * @param   bits   an integer.
         * @return  the @c gfloat floating-point value with the same bit
         *          pattern.
         */
        static gfloat fromIntBits(gint bits);

        /**
         * {@return the @c gfloat value closest to the numerical value
         * of the argument, a floating-point binary16 value encoded in a
         * @c short The conversion is exact; all binary16 values can
         * be exactly represented in @c gfloat.
         *
         * Special cases:
         * <ul>
         * <li> If the argument is zero, the result is a zero with the
         * same sign as the argument.
         * <li> If the argument is infinite, the result is an infinity
         * with the same sign as the argument.
         * <li> If the argument is a NaN, the result is a NaN.
         * </ul>
         *
         * <h4><a id=binary16Format>IEEE 754 binary16 format</a></h4>
         * The IEEE 754 standard defines binary16 as a 16-bit format, along
         * with the 32-bit binary32 format (corresponding to the @c 
         * gfloat type) and the 64-bit binary64 format (corresponding to
         * the @c double type). The binary16 format is similar to the
         * other IEEE 754 formats, except smaller, having all the usual
         * IEEE 754 values such as NaN, signed infinities, signed zeros,
         * and subnormals. The parameters (JLS {@jls 4.2.3) for the
         * binary16 format are N = 11 precision bits, K = 5 exponent bits,
         * <i>E</i><sub><i>max</i></sub> = 15, and
         * <i>E</i><sub><i>min</i></sub> = -14.
         *
         * @apiNote
         * This method corresponds to the convertFormat operation defined
         * in IEEE 754 from the binary16 format to the binary32 format.
         * The operation of this method is analogous to a primitive
         * widening conversion (JLS {@jls 5.1.2).
         *
         * @param floatBinary16 the binary16 value to convert to @c gfloat
         *
         */
        static gfloat fromFloat16(gshort floatBinary16);

        /**
         * {@return the floating-point binary16 value, encoded in a @c 
         * short, closest in value to the argument
         * The conversion is computed under the round to nearest even rounding mode.
         *
         * Special cases:
         * <ul>
         * <li> If the argument is zero, the result is a zero with the
         * same sign as the argument.
         * <li> If the argument is infinite, the result is an infinity
         * with the same sign as the argument.
         * <li> If the argument is a NaN, the result is a NaN.
         * </ul>
         *
         * The <i>binary16 format</i> is discussed in more detail in the @c fromFloat16 method.
         *
         * @note
         * This method corresponds to the convertFormat operation defined
         * in IEEE 754 from the binary32 format to the binary16 format.
         * The operation of this method is analogous to a primitive
         * narrowing conversion (JLS {@jls 5.1.3).
         *
         * @param f the @c gfloat value to convert to binary16
         *
         */
        static gshort toFloat16(gfloat f);

        /**
         * Compares two @c Float objects numerically.
         *
         * This method imposes a total order on @c Float objects
         * with two differences compared to the incomplete order defined by
         * the Java language numerical comparison operators (@c <, <=,
         * ==, >=, >) on @c gfloat values.
         *
         * <ul><li> A NaN is <em>unordered</em> with respect to other
         *          values and unequal to itself under the comparison
         *          operators.  This method chooses to define @c 
         *          Float.NaN to be equal to itself and greater than all
         *          other @c double values (including @c 
         *          Float.POSITIVE_INFINITY).
         *
         *      <li> Positive zero and negative zero compare equal
         *      numerically, but are distinct and distinguishable values.
         *      This method chooses to define positive zero (@c +0.0f),
         *      to be greater than negative zero (@c -0.0f).
         * </ul>
         *
         * This ensures that the <i>natural ordering</i> of @c Float
         * objects imposed by this method is <i>consistent with equals</i>
         *
         * @param   anotherFloat   the @c Float to be compared.
         * @return  the value @c 0 if @c anotherFloat is
         *          numerically equal to this @c Float; a value
         *          less than @c 0 if this @c Float
         *          is numerically less than @c anotherFloat;
         *          and a value greater than @c 0 if this
         *          @c Float is numerically greater than
         *          @c anotherFloat.
         */
        gint compareTo(Float const &anotherFloat) const override;

        /**
         * Compares the two specified @c gfloat values. The sign
         * of the integer value returned is the same as that of the
         * integer that would be returned by the call:
         * <pre>
         *    Float.valueOf(f1).compareTo(Float.valueOf(f2))
         * </pre>
         *
         * @param   f1        the first @c gfloat to compare.
         * @param   f2        the second @c gfloat to compare.
         * @return  the value @c 0 if @c f1 is
         *          numerically equal to @c f2; a value less than
         *          @c 0 if @c f1 is numerically less than
         *          @c f2; and a value greater than @c 0
         *          if @c f1 is numerically greater than
         *          @c f2.
         *
         */
        static gint compare(gfloat f1, gfloat f2);

        /**
         * Adds two @c gfloat values together as per the + operator.
         *
         * @apiNote This method corresponds to the addition operation
         * defined in IEEE 754.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the sum of @c a and @c b
         * @jls 4.2.4 Floating-Point Operations
         *
         */
        static gfloat sum(gfloat a, gfloat b);

        /**
         * Returns the greater of two @c gfloat values
         * as if by calling @c Math::max.
         *
         * @apiNote
         * This method corresponds to the maximum operation defined in
         * IEEE 754.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the greater of @c a and @c b
         *
         */
        static gfloat max(gfloat a, gfloat b);

        /**
         * Returns the smaller of two @c gfloat values
         * as if by calling @c Math::min.
         *
         * @apiNote
         * This method corresponds to the minimum operation defined in
         * IEEE 754.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the smaller of @c a and @c b
         *
         */
        static gfloat min(gfloat a, gfloat b);

        CORE_IMPLICIT operator gfloat() const;

        CORE_IMPLICIT operator gfloat &();

        Object &clone() const override;
    };
} // core

#endif //CORE24_FLOAT_H
