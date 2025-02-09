//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_DOUBLE_H
#define CORE24_DOUBLE_H

#include <core/Math.h>
#include <core/Byte.h>

namespace core
{
    /**
     * The @c Double class wraps a value of the primitive type
     * @c gdouble in an object. An object of type
     * @c Double contains a single field whose type is
     * @c gdouble.
     *
     * <h2>Floating-point Equality, Equivalence, and Comparison</h2>
     *
     * IEEE 754 floating-point values include finite nonzero values,
     * signed zeros (@c +0.0 and @c -0.0), signed infinities
     * (@a positive infinity and @a negative infinity), and
     * @a NaN (not-a-number).
     *
     * <p>An <em>equivalence relation</em> on a set of values is a boolean
     * relation on pairs of values that is reflexive, symmetric, and
     * transitive. For more discussion of equivalence relations and object
     * equality, see the @c Object#equals Object.equals
     * specification. An equivalence relation partitions the values it
     * operates over into sets called <i>equivalence classes</i>.  All the
     * members of the equivalence class are equal to each other under the
     * relation. An equivalence class may contain only a single member. At
     * least for some purposes, all the members of an equivalence class
     * are substitutable for each other.  In particular, in a numeric
     * expression equivalent values can be <em>substituted</em> for one
     * another without changing the result of the expression, meaning
     * changing the equivalence class of the result of the expression.
     *
     * <p>Notably, the built-in @c == operation on floating-point
     * values is <em>not</em> an equivalence relation. Despite not
     * defining an equivalence relation, the semantics of the IEEE 754
     * @c == operator were deliberately designed to meet other needs
     * of numerical computation. There are two exceptions where the
     * properties of an equivalence relation are not satisfied by @c
     * == on floating-point values:
     *
     * <ul>
     *
     * <li>If @c v1 and @c v2 are both NaN, then @c v1
     * == v2 has the value @c false. Therefore, for two NaN
     * arguments the <em>reflexive</em> property of an equivalence
     * relation is <em>not</em> satisfied by the @c == operator.
     *
     * <li>If @c v1 represents @c +0.0 while @c v2
     * represents @c -0.0, or vice versa, then @c v1 == v2 has
     * the value @c true even though @c +0.0 and @c -0.0
     * are distinguishable under various floating-point operations. For
     * example, @c 1.0/+0.0 evaluates to positive infinity while
     * @c 1.0/-0.0 evaluates to <em>negative</em> infinity and
     * positive infinity and negative infinity are neither equal to each
     * other nor equivalent to each other. Thus, while a signed zero input
     * most commonly determines the sign of a zero result, because of
     * dividing by zero, @c +0.0 and @c -0.0 may not be
     * substituted for each other in general. The sign of a zero input
     * also has a non-substitutable effect on the result of some math
     * library methods.
     *
     * </ul>
     *
     * <p>For ordered comparisons using the built-in comparison operators
     * (@c <, @c <=, etc.), NaN values have another anomalous
     * situation: a NaN is neither less than, nor greater than, nor equal
     * to any value, including itself. This means the <i>dichotomy of
     * comparison</i> does <em>not</em> hold.
     *
     * <p>To provide the appropriate semantics for @c equals and
     * @c compareTo methods, those methods cannot simply be wrappers
     * around @c == or ordered comparison operations. Instead, @c equals uses
     * <i> representation equivalence</i>, defining NaN arguments to be equal to each other,
     * restoring reflexivity, and defining @c +0.0 to <em>not</em> be
     * equal to @c -0.0. For comparisons, @c compareTo defines a total order where @c -0.0
     * is less than @c +0.0 and where a NaN is equal to itself and considered
     * greater than positive infinity.
     *
     * <p>The operational semantics of @c equals and @c
     * compareTo are expressed in terms of @c bit-wise converting the floating-point values to integral values.
     *
     * <p>The <em>natural ordering</em> implemented by
     * @c compareTo is @c Comparable consistent with equals. That
     * is, two objects are reported as equal by @c equals if and only
     * if @c compareTo on those objects returns zero.
     *
     * <p>The adjusted behaviors defined for @c equals and @c
     * compareTo allow instances of wrapper classes to work properly with
     * conventional data structures. For example, defining NaN
     * values to be @c equals to one another allows NaN to be used as
     * an element of a @c HashSet or as the key of a @c HashMap. Similarly, defining
     * @c compareTo as a total ordering, including @c +0.0, @c
     * -0.0, and NaN, allows instances of wrapper classes to be used as
     * elements of a @c SortedSet or as keys of a @c SortedMap.
     *
     * <p>Comparing numerical equality to various useful equivalence
     * relations that can be defined over floating-point values:
     *
     * <dl>
     * <dt><i>numerical equality</i> (@c ==
     * operator): (<em>Not</em> an equivalence relation)</dt>
     * <dd>Two floating-point values represent the same extended real
     * number. The extended real numbers are the real numbers augmented
     * with positive infinity and negative infinity. Under numerical
     * equality, @c +0.0 and @c -0.0 are equal since they both
     * map to the same real value, 0. A NaN does not map to any real
     * number and is not equal to any value, including itself.
     * </dd>
     *
     * <dt><i>bit-wise equivalence</i>:</dt>
     * <dd>The bits of the two floating-point values are the same. This
     * equivalence relation for @c gdouble values @c a and @c
     * b is implemented by the expression
     * <br>@c Double::doubleTo<code>Raw</code>LongBits(a) == Double::doubleTo<code>Raw</code>LongBits(b)<br>
     * Under this relation, @c +0.0 and @c -0.0 are
     * distinguished from each other and every bit pattern encoding a NaN
     * is distinguished from every other bit pattern encoding a NaN.
     * </dd>
     *
     * <dt><i>representation equivalence</i>:</dt>
     * <dd>The two floating-point values represent the same IEEE 754
     * <i>datum</i>. In particular, for @c finite values,
     * the sign, @c exponent, and significand components of the floating-point values
     * are the same. Under this relation:
     * <ul>
     * <li> @c +0.0 and @c -0.0 are distinguished from each other.
     * <li> every bit pattern encoding a NaN is considered equivalent to each other
     * <li> positive infinity is equivalent to positive infinity; negative
     *      infinity is equivalent to negative infinity.
     * </ul>
     * Expressions implementing this equivalence relation include:
     * <ul>
     * <li>@c Double::doubleToLongBits(a) == Double::doubleToLongBits(b)
     * <li>@c Double::valueOf(a).equals(Double::valueOf(b))
     * <li>@c Double::compare(a, b) == 0
     * </ul>
     * Note that representation equivalence is often an appropriate notion
     * of equivalence to test the behavior of @c StrictMath math
     * libraries.
     * </dd>
     * </dl>
     *
     * For two binary floating-point values @c a and @c b, if
     * neither of @c a and @c b is zero or NaN, then the three
     * relations numerical equality, bit-wise equivalence, and
     * representation equivalence of @c a and @c b have the same
     * @c true/@c false value. In other words, for binary
     * floating-point values, the three relations only differ if at least
     * one argument is zero or NaN.
     *
     * @see <a href="https://standards.ieee.org/ieee/754/6210/">
     *      <cite>IEEE Standard for Floating-Point Arithmetic</cite></a>
     *
     */
    class Double final : public virtual Number, public virtual Comparable<Double>
    {
    public:
        /**
         * A constant holding the smallest of type
         * @c gdouble such that 1.0 + EPSILON != 1.0 . It is equal to the value returned by
         * @c Double::fromLongBits(0x3cb0000000000000L).
         */
        static CORE_FAST gdouble EPSILON = 0x1.0p-52;// 2.2204460492503131E-16;

        /**
         * A constant holding the positive infinity of type
         * @c gdouble. It is equal to the value returned by
         * @c Double::fromLongBits(0x7ff0000000000000L).
         */
        static CORE_FAST gdouble POSITIVE_INFINITY = Math::INF;

        /**
         * A constant holding the negative infinity of type
         * @c gdouble. It is equal to the value returned by
         * @c Double::fromLongBits(0xfff0000000000000L).
         */
        static CORE_FAST gdouble NEGATIVE_INFINITY = -POSITIVE_INFINITY;

        /**
         * A constant holding a Not-a-Number (NaN) value of type
         * @c gdouble. It is equivalent to the value returned by
         * @c Double::fromLongBits(0x7ff8000000000000L).
         */
        static CORE_FAST gdouble NaN = Math::NaN;

        /**
         * A constant holding the largest positive finite value of type
         * @c gdouble,
         * (2-2<sup>-52</sup>)&middot;2<sup>1023</sup>.  It is equal to
         * the hexadecimal floating-point literal
         * @c 0x1.fffffffffffffP+1023 and also equal to
         * @c Double::fromLongBits(0x7fefffffffffffffL).
         */
        static CORE_FAST gdouble MAX_VALUE = 0x1.fffffffffffffp+1023; // 1.7976931348623157e+308

        /**
         * A constant holding the smallest positive normal value of type
         * @c gdouble, 2<sup>-1022</sup>.  It is equal to the
         * hexadecimal floating-point literal @c 0x1.0p-1022 and also
         * equal to @c Double::fromLongBits(0x0010000000000000L).
         *
         *
         */
        static CORE_FAST gdouble MIN_NORMAL = 0x1.0p-1022; // 2.2250738585072014E-308

        /**
         * A constant holding the smallest positive nonzero value of type
         * @c gdouble, 2<sup>-1074</sup>. It is equal to the
         * hexadecimal floating-point literal
         * @c 0x0.0000000000001P-1022 and also equal to
         * @c Double::fromLongBits(0x1L).
         */
        static CORE_FAST gdouble MIN_VALUE = 0x0.0000000000001p-1022; // 4.9e-324

        /**
         * The number of bits used to represent a @c gdouble value.
         *
         *
         */
        static CORE_FAST gint SIZE = 1 << 6;

        /**
         * The number of bits in the significand of a @c gdouble value.
         *
         *
         */
        static CORE_FAST gint PRECISION = 53;

        /**
         * Maximum exponent a finite @c gdouble variable may have.
         * It is equal to the value returned by
         * @c Math.getExponent(Double::MAX_VALUE).
         *
         *
         */
        static CORE_FAST gint MAX_EXPONENT = (1 << (SIZE - PRECISION - 1)) - 1; // 1023

        /**
         * Minimum exponent a normalized @c gdouble variable may
         * have.  It is equal to the value returned by
         * @c Math.getExponent(Double::MIN_NORMAL).
         *
         *
         */
        static CORE_FAST gint MIN_EXPONENT = 1 - MAX_EXPONENT; // -1022

        /**
         * The number of bytes used to represent a @c gdouble value.
         * 
         */
        static CORE_FAST gint BYTES = SIZE / Byte::SIZE;

        /**
         * Returns a string representation of the @c gdouble
         * argument. All characters mentioned below are ASCII characters.
         * <ul>
         * <li>If the argument is NaN, the result is the string "@c NaN".  </li>
         * <li>Otherwise, the result is a string that represents the sign and  </li>
         * magnitude (absolute value) of the argument. If the sign is negative,
         * the first character of the result is '@c -' (@c '&#92;u002D');
         * if the sign is positive, no sign character
         * appears in the result. As for the magnitude <i>m</i>:
         * <ul>
         * <li>If <i>m</i> is infinity, it is represented by the characters
         * @c "inf"; thus, positive infinity produces the result
         * @c "inf" and negative infinity produces the result @c "-inf". </li>
         *
         * <li>If <i>m</i> is zero, it is represented by the characters
         * @c "0.0"; thus, negative zero produces the result
         * @c "-0.0" and positive zero produces the result
         * @c "0.0". </li>
         *
         * <li> Otherwise, <i>m</i> is positive and finite.
         * It is converted to a string in two stages:
         * <ul>
         * <li> <em>Selection of a decimal</em>:
         * A well-defined decimal <i>d</i><sub><i>m</i></sub>
         * is selected to represent <i>m</i>.
         * This decimal is (almost always) the <em>shortest</em> one that
         * rounds to <i>m</i> according to the round to nearest
         * rounding policy of IEEE 754 floating-point arithmetic. </li>
         * <li> <em>Formatting as a string</em>:
         * The decimal <i>d</i><sub><i>m</i></sub> is formatted as a string,
         * either in plain or in computerized scientific notation,
         * depending on its value. </li>
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
         * Here <i>e</i> is formatted as by @c Integer#toString(int).
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
         * <p>To create localized string representations of a floating-point
         * value, use subclasses of @c text::NumberFormat.
         *
         * @param   d   the @c gdouble to be converted.
         * @return a string representation of the argument.
         */
        static String toString(gdouble d);

        /**
         * Returns a hexadecimal string representation of the
         * @c gdouble argument. All characters mentioned below
         * are ASCII characters.
         *
         * <ul>
         * <li>If the argument is NaN, the result is the string
         *     "@c NaN".
         * <li>Otherwise, the result is a string that represents the sign
         * and magnitude of the argument. If the sign is negative, the
         * first character of the result is '@c -'
         * (@c '&#92;u002D'); if the sign is positive, no sign
         * character appears in the result. As for the magnitude <i>m</i>:
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
         * <li>If <i>m</i> is a @c gdouble value with a
         * normalized representation, sub-strings are used to represent the
         * significand and exponent fields.  The significand is
         * represented by the characters @c "0x1."
         * followed by a lowercase hexadecimal representation of the rest
         * of the significand as a fraction.  Trailing zeros in the
         * hexadecimal representation are removed unless all the digits
         * are zero, in which case a single zero is used. Next, the
         * exponent is represented by @c "p" followed
         * by a decimal string of the unbiased exponent as if produced by
         * a call to @c Integer#toString(int) Integer.toString on the
         * exponent value.
         *
         * <li>If <i>m</i> is a @c gdouble value with a subnormal
         * representation, the significand is represented by the
         * characters @c "0x0." followed by a
         * hexadecimal representation of the rest of the significand as a
         * fraction.  Trailing zeros in the hexadecimal representation are
         * removed. Next, the exponent is represented by
         * @c "p-1022".  Note that there must be at
         * least one nonzero digit in a subnormal significand.
         *
         * </ul>
         *
         * </ul>
         *
         * <table class="striped">
         * <caption>Examples</caption>
         * <thead>
         * <tr><th>Floating-point Value</th><th>Hexadecimal String</th>
         * </thead>
         * <tbody>
         * <tr><th>@c 1.0</th> <td>@c 0x1.0p0</td>
         * <tr><th>@c -1.0</th> <td>@c -0x1.0p0</td>
         * <tr><th>@c 2.0</th> <td>@c 0x1.0p1</td>
         * <tr><th>@c 3.0</th> <td>@c 0x1.8p1</td>
         * <tr><th>@c 0.5</th> <td>@c 0x1.0p-1</td>
         * <tr><th>@c 0.25</th> <td>@c 0x1.0p-2</td>
         * <tr><th>@c Double::MAX_VALUE</th>
         *     <td>@c 0x1.fffffffffffffp1023</td>
         * <tr><th>@c Minimum Normal Value</th>
         *     <td>@c 0x1.0p-1022</td>
         * <tr><th>@c Maximum Subnormal Value</th>
         *     <td>@c 0x0.fffffffffffffp-1022</td>
         * <tr><th>@c Double::MIN_VALUE</th>
         *     <td>@c 0x0.0000000000001p-1022</td>
         * </tbody>
         * </table>
         * @param   d   the @c gdouble to be converted.
         * @return a hex string representation of the argument.
         */
        static String toHexString(gdouble d);

        /**
         * Returns a @c Double object holding the
         * @c gdouble value represented by the argument string
         * @c s.
         *
         * <p>Leading and trailing whitespace characters in @c s
         * are ignored.  Whitespace is removed as if by the @c
         * String#trim method; that is, both ASCII space and control
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
         * binary value that is then rounded to type @c gdouble
         * by the usual round-to-nearest rule of IEEE 754 floating-point
         * arithmetic, which includes preserving the sign of a zero
         * value.
         *
         * Note that the round-to-nearest rule also implies overflow and
         * underflow behaviour; if the exact value of @c s is large
         * enough in magnitude (greater than or equal to (@c
         * #MAX_VALUE + @c Math#ulp(gdouble) ulp(MAX_VALUE)/2),
         * rounding to @c gdouble will result in an infinity and if the
         * exact value of @c s is small enough in magnitude (less
         * than or equal to @c Double::MIN_VALUE/2), rounding to gfloat will
         * result in a zero.
         *
         * Finally, after rounding a @c Double object representing
         * this @c gdouble value is returned.
         *
         * <p> To interpret localized string representations of a
         * floating-point value, use subclasses of @c text::NumberFormat.
         *
         * <p>Note that trailing format specifiers, specifiers that
         * determine the type of floating-point literal (@c 1.0f is a @c gfloat value;
         * @c 1.0 is a @c gdouble value), do <em>not</em> influence the results of this method.
         * In other words, the numerical value of the input string is converted
         * directly to the target floating-point type.  The two-step
         * sequence of conversions, string to @c gfloat followed
         * by @c gfloat to @c gdouble, is <em>not</em>
         * equivalent to converting a string directly to
         * @c gdouble. For example, the @c gfloat literal @c 0.1f is equal to the @c gdouble
         * value @c 0.10000000149011612; the @c gfloat literal @c 0.1f represents a different numerical
         * value than the @c gdouble literal @c 0.1. (The numerical value 0.1 cannot be exactly
         * represented in a binary floating-point number.)
         *
         * @param      s   the string to be parsed.
         * @return     a @c Double object holding the value
         *             represented by the @c String argument.
         * @throws     NumberFormatException  if the string does not contain a
         *             parsable number.
         */
        static Double valueOf(String const& s);

        /**
         * Returns a @c Double instance representing the specified
         * @c gdouble value.
         * If a new @c Double instance is not required, this method
         * should generally be used in preference to the constructor
         * @c Double::Double(gdouble), as this method is likely to yield
         * significantly better space and time performance by caching
         * frequently requested values.
         *
         * @param  d a gdouble value.
         * @return a @c Double instance representing @c d.
         *
         */
        static Double valueOf(gdouble d);

        /**
         * Returns a new @c gdouble initialized to the value
         * represented by the specified @c String, as performed
         * by the @c valueOf method of class
         * @c Double::
         *
         * @param  s   the string to be parsed.
         * @return the @c gdouble value represented by the string
         *         argument.
         *
         * @throws NumberFormatException if the string does not contain
         *         a parsable @c gdouble.
         * @see    Double#valueOf(String)
         *
         */
        static gdouble parseDouble(String const& s);

        /**
         * Returns @c true if the specified number is a
         * Not-a-Number (NaN) value, @c false otherwise.
         *
         * @note
         * This method corresponds to the isNaN operation defined in IEEE
         * 754.
         *
         * @param   v   the value to be tested.
         * @return  @c true if the value of the argument is NaN;
         *          @c false otherwise.
         */
        static gbool isNaN(gdouble v);

        /**
         * Returns @c true if the specified number is infinitely
         * large in magnitude, @c false otherwise.
         *
         * @note
         * This method corresponds to the isInfinite operation defined in
         * IEEE 754.
         *
         * @param   v   the value to be tested.
         * @return  @c true if the value of the argument is positive
         *          infinity or negative infinity; @c false otherwise.
         */
        static gbool isInfinite(gdouble v);

        /**
         * Returns @c true if the argument is a finite floating-point
         * value; returns @c false otherwise (for NaN and infinity
         * arguments).
         *
         * @note
         * This method corresponds to the isFinite operation defined in
         * IEEE 754.
         *
         * @param d the @c gdouble value to be tested
         * @return @c true if the argument is a finite
         * floating-point value, @c false otherwise.
         *
         */
        static gbool isFinite(gdouble d);

    private:
        /**
         * The value of the Double::
         *
         * @serial
         */
        gdouble value = 0.0;

    public:
        /**
         * Construct new instance of @c Double with value positive zero
         */
        CORE_IMPLICIT Double() CORE_NOTHROW;

        /**
         * Constructs a newly allocated @c Double object that
         * represents the primitive @c gdouble argument.
         *
         * @param   value   the value to be represented by the @c Double::
         *
         * @deprecated
         * It is rarely appropriate to use this constructor. The static factory
         * @c Double::valueOf(gdouble) is generally a better choice, as it is
         * likely to yield significantly better space and time performance.
         */
        CORE_IMPLICIT Double(gdouble value);

        /**
         * Constructs a newly allocated @c Double object that
         * represents the floating-point value of type @c gdouble
         * represented by the string. The string is converted to a
         * @c gdouble value as if by the @c valueOf method.
         *
         * @param  s  a string to be converted to a @c Double::
         * @throws    NumberFormatException if the string does not contain a
         *            parsable number.
         *
         * @deprecated
         * It is rarely appropriate to use this constructor.
         * Use @c Double::parseDouble(String) to convert a string to a
         * @c gdouble primitive, or use @c Double::valueOf(String)
         * to convert a string to a @c Double object.
         */
        CORE_EXPLICIT Double(String const& s);

        /**
         * Returns @c true if this @c Double value is
         * a Not-a-Number (NaN), @c false otherwise.
         *
         * @return  @c true if the value represented by this object is
         *          NaN; @c false otherwise.
         */
        gbool isNaN() const;

        /**
         * Returns @c true if this @c Double value is
         * infinitely large in magnitude, @c false otherwise.
         *
         * @return  @c true if the value represented by this object is
         *          positive infinity or negative infinity;
         *          @c false otherwise.
         */
        gbool isInfinite() const;

        /**
         * Returns a string representation of this @c Double object.
         * The primitive @c gdouble value represented by this
         * object is converted to a string exactly as if by the method
         * @c toString of one argument.
         *
         * @return  a @c String representation of this object.
         * @see Double#toString(gdouble)
         */
        String toString() const override;

        /**
         * Returns the value of this @c Double as a @c byte
         * after a narrowing primitive conversion.
         *
         * @return  the @c gdouble value represented by this object
         *          converted to type @c byte
         */
        gbyte byteValue() const override;

        /**
         * Returns the value of this @c Double as a @c short
         * after a narrowing primitive conversion.
         *
         * @return  the @c gdouble value represented by this object
         *          converted to type @c short
         */
        gshort shortValue() const override;

        /**
         * Returns the value of this @c Double as an @c int
         * after a narrowing primitive conversion.
         *
         * @return  the @c gdouble value represented by this object
         *          converted to type @c int
         */
        gint intValue() const override;

        /**
         * Returns the value of this @c Double as a @c long
         * after a narrowing primitive conversion.
         *
         * @return  the @c gdouble value represented by this object
         *          converted to type @c long
         */
        glong longValue() const override;

        /**
         * Returns the value of this @c Double as a @c gfloat
         * after a narrowing primitive conversion.
         *
         * @note
         * This method corresponds to the convertFormat operation defined
         * in IEEE 754.
         *
         * @return  the @c gdouble value represented by this object
         *          converted to type @c gfloat
         */
        gfloat floatValue() const override;

        /**
         * Returns the @c gdouble value of this @c Double object.
         *
         * @return the @c gdouble value represented by this object
         */
        gdouble doubleValue() const override;

        /**
         * Returns a hash code for this @c Double object. The
         * result is the exclusive OR of the two halves of the
         * @c long integer bit representation, exactly as
         * produced by the method @c Double::doubleToLongBits(gdouble), of
         * the primitive @c gdouble value represented by this
         * @c Double object. That is, the hash code is the value
         * of the expression:
         *
         * <blockquote>
         *  @c (int)(v^(v>>>32))
         * </blockquote>
         *
         * where @c v is defined by:
         *
         * <blockquote>
         *  @c long v = Double::doubleToLongBits(this.doubleValue());
         * </blockquote>
         *
         * @return  a @c hash code value for this object.
         */
        gint hash() const override;

        /**
         * Returns a hash code for a @c gdouble value; compatible with
         * @c Double::hash().
         *
         * @param value the value to hash
         * @return a hash code value for a @c gdouble value.
         *
         */
        static gint hash(gdouble value);

        /**
         * Compares this object against the specified object.  The result
         * is @c true if and only if the argument is not
         * @c null and is a @c Double object that
         * represents a @c gdouble that has the same value as the
         * @c gdouble represented by this object. For this
         * purpose, two @c gdouble values are considered to be
         * the same if and only if the method @c 
         * #doubleToLongBits(gdouble) returns the identical
         * @c long value when applied to each.
         *
         * @note
         * This method is defined in terms of @c 
         * #doubleToLongBits(gdouble) rather than the @c == operator
         * on @c gdouble values since the @c == operator does
         * <em>not</em> define an equivalence relation and to satisfy the
         * @c equals contract an equivalence relation must be implemented;
         */
        gbool equals(Object const& obj) const override;

        /**
         * Returns a representation of the specified floating-point value
         * according to the IEEE 754 floating-point "gdouble
         * format" bit layout.
         *
         * <p>Bit 63 (the bit that is selected by the mask
         * @c 0x8000000000000000L) represents the sign of the
         * floating-point number. Bits
         * 62-52 (the bits that are selected by the mask
         * @c 0x7ff0000000000000L) represent the exponent. Bits 51-0
         * (the bits that are selected by the mask
         * @c 0x000fffffffffffffL) represent the significand
         * (sometimes called the mantissa) of the floating-point number.
         *
         * <p>If the argument is positive infinity, the result is
         * @c 0x7ff0000000000000L.
         *
         * <p>If the argument is negative infinity, the result is
         * @c 0xfff0000000000000L.
         *
         * <p>If the argument is NaN, the result is
         * @c 0x7ff8000000000000L.
         *
         * <p>In all cases, the result is a @c long integer that, when
         * given to the @c Double::fromLongBits(long) method, will produce a
         * floating-point value the same as the argument to
         * @c doubleToLongBits (except all NaN values are
         * collapsed to a single "canonical" NaN value).
         *
         * @param   value   a @c gdouble precision floating-point number.
         * @return the bits that represent the floating-point number.
         */
        static glong toLongBits(gdouble value);

        /**
         * Returns a representation of the specified floating-point value
         * according to the IEEE 754 floating-point "gdouble
         * format" bit layout, preserving Not-a-Number (NaN) values.
         *
         * <p>Bit 63 (the bit that is selected by the mask
         * @c 0x8000000000000000L) represents the sign of the
         * floating-point number. Bits
         * 62-52 (the bits that are selected by the mask
         * @c 0x7ff0000000000000L) represent the exponent. Bits 51-0
         * (the bits that are selected by the mask
         * @c 0x000fffffffffffffL) represent the significand
         * (sometimes called the mantissa) of the floating-point number.
         *
         * <p>If the argument is positive infinity, the result is
         * @c 0x7ff0000000000000L.
         *
         * <p>If the argument is negative infinity, the result is
         * @c 0xfff0000000000000L.
         *
         * <p>If the argument is NaN, the result is the @c long
         * integer representing the actual NaN value.  Unlike the
         * @c doubleToLongBits method,
         * @c doubleToRawLongBits does not collapse all the bit
         * patterns encoding a NaN to a single "canonical" NaN
         * value.
         *
         * <p>In all cases, the result is a @c long integer that,
         * when given to the @c Double::fromLongBits(long) method, will
         * produce a floating-point value the same as the argument to
         * @c doubleToRawLongBits.
         *
         * @param   value   a @c gdouble precision floating-point number.
         * @return the bits that represent the floating-point number.
         *
         */
        static glong toRawLongBits(gdouble value);

        /**
         * Returns the @c gdouble value corresponding to a given
         * bit representation.
         * The argument is considered to be a representation of a
         * floating-point value according to the IEEE 754 floating-point
         * "gdouble format" bit layout.
         *
         * <p>If the argument is @c 0x7ff0000000000000L, the result
         * is positive infinity.
         *
         * <p>If the argument is @c 0xfff0000000000000L, the result
         * is negative infinity.
         *
         * <p>If the argument is any value in the range
         * @c 0x7ff0000000000001L through
         * @c 0x7fffffffffffffffL or in the range
         * @c 0xfff0000000000001L through
         * @c 0xffffffffffffffffL, the result is a NaN.  No IEEE
         * 754 floating-point operation provided by Java can distinguish
         * between two NaN values of the same type with different bit
         * patterns.  Distinct values of NaN are only distinguishable by
         * use of the @c Double::doubleToRawLongBits method.
         *
         * <p>In all other cases, let <i>s</i>, <i>e</i>, and <i>m</i> be three
         * values that can be computed from the argument:
         *
         * @code
         * int s = ((bits >> 63) == 0) ? 1 : -1;
         * int e = (int)((bits >> 52) & 0x7ffL);
         * long m = (e == 0) ?
         *                 (bits & 0xfffffffffffffL) << 1 :
         *                 (bits & 0xfffffffffffffL) | 0x10000000000000L;
         * @endcode 
         *
         * Then the floating-point result equals the value of the mathematical
         * expression <i>s</i>&middot;<i>m</i>&middot;2<sup><i>e</i>-1075</sup>.
         *
         * <p>Note that this method may not be able to return a
         * @c gdouble NaN with exactly same bit pattern as the
         * @c long argument.  IEEE 754 distinguishes between two
         * kinds of NaNs, quiet NaNs and <i>signaling NaNs</i>.  The
         * differences between the two kinds of NaN are generally not
         * visible in Java.  Arithmetic operations on signaling NaNs turn
         * them into quiet NaNs with a different, but often similar, bit
         * pattern.  However, on some processors merely copying a
         * signaling NaN also performs that conversion.  In particular,
         * copying a signaling NaN to return it to the calling method
         * may perform this conversion.  So @c fromLongBits
         * may not be able to return a @c gdouble with a
         * signaling NaN bit pattern.  Consequently, for some
         * @c long values,
         * @c doubleToRawLongBits(fromLongBits(start)) may
         * <i>not</i> equal @c start.  Moreover, which
         * particular bit patterns represent signaling NaNs is platform
         * dependent; although all NaN bit patterns, quiet or signaling,
         * must be in the NaN range identified above.
         *
         * @param   bits   any @c long integer.
         * @return  the @c gdouble floating-point value with the same
         *          bit pattern.
         */
        static gdouble fromLongBits(glong bits);

        /**
         * Compares two @c Double objects numerically.
         *
         * This method imposes a total order on @c Double objects
         * with two differences compared to the incomplete order defined by
         * the Java language numerical comparison operators (@c <, <=,
         * ==, >=, >) on @c gdouble values.
         *
         * <ul><li> A NaN is <em>unordered</em> with respect to other
         *          values and unequal to itself under the comparison
         *          operators.  This method chooses to define @c 
         *          Double::NaN to be equal to itself and greater than all
         *          other @c gdouble values (including @c 
         *          Double::POSITIVE_INFINITY).
         *
         *      <li> Positive zero and negative zero compare equal
         *      numerically, but are distinct and distinguishable values.
         *      This method chooses to define positive zero (@c +0.0d),
         *      to be greater than negative zero (@c -0.0d).
         * </ul>

         * This ensures that the <i>natural ordering</i> of @c Double
         * objects imposed by this method is <i>consistent with equals</i>.
         *
         * @param   anotherDouble   the @c Double to be compared.
         * @return  the value @c 0 if @c anotherDouble is
         *          numerically equal to this @c Double; a value
         *          less than @c 0 if this @c Double
         *          is numerically less than @c anotherDouble;
         *          and a value greater than @c 0 if this
         *          @c Double is numerically greater than
         *          @c anotherDouble::
         */
        gint compareTo(Double const& anotherDouble) const override;

        /**
         * Compares the two specified @c gdouble values. The sign
         * of the integer value returned is the same as that of the
         * integer that would be returned by the call:
         * @code
         *    Double::valueOf(d1).compareTo(Double::valueOf(d2))
         * @endcode
         *
         * @param   d1        the first @c gdouble to compare
         * @param   d2        the second @c gdouble to compare
         * @return  the value @c 0 if @c d1 is
         *          numerically equal to @c d2; a value less than
         *          @c 0 if @c d1 is numerically less than
         *          @c d2; and a value greater than @c 0
         *          if @c d1 is numerically greater than
         *          @c d2.
         */
        static gint compare(gdouble d1, gdouble d2);

        /**
         * Adds two @c gdouble values together as per the + operator.
         *
         * @note This method corresponds to the addition operation
         * defined in IEEE 754.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the sum of @c a and @c b
         *
         */
        static gdouble sum(gdouble a, gdouble b);

        /**
         * Returns the greater of two @c gdouble values
         * as if by calling @c Math::max.
         *
         * @note
         * This method corresponds to the maximum operation defined in
         * IEEE 754.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the greater of @c a and @c b
         *
         */
        static gdouble max(gdouble a, gdouble b);

        /**
         * Returns the smaller of two @c gdouble values
         * as if by calling @c Math#min(gdouble, gdouble) Math::min.
         *
         * @note
         * This method corresponds to the minimum operation defined in
         * IEEE 754.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the smaller of @c a and @c b.
         */
        static gdouble min(gdouble a, gdouble b);

        CORE_IMPLICIT operator gdouble() const;

        CORE_IMPLICIT operator gdouble&();

        Object& clone() const override;
    };
} // core

#endif //CORE24_DOUBLE_H
