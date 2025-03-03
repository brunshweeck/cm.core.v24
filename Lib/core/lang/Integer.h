//
// Created by bruns on 13/05/2024.
//

#ifndef CORE24_INTEGER_H
#define CORE24_INTEGER_H

#include <core/lang/Byte.h>
#include "Math.h"

namespace core
{

    /**
     * The @c Integer class wraps a value of the primitive type
     * @c gint in an object. An object of type @c Integer
     * contains a single field whose type is @c gint.
     *
     * @note: The implementations of the "bit twiddling" methods (such as
     * @c Integer::highestOneBit and @c Integer::numberOfTrailingZeros) are
     * based on material from Henry S. Warren, Jr.'s Hacker's Delight, (Addison Wesley, 2002).
     */
    class Integer : public virtual Number, public virtual Comparable<Integer>
    {
    public:
        /**
         * A constant holding the maximum value a gint can
         * have, 2<sup>31</sup> - 1
         */
        static CORE_FAST gint MAX_VALUE = (gint) ((1LL << 31) - 1);

        /**
         * A constant holding the minimum value a gint can
         * have, -2<sup>31</sup>
         */
        static CORE_FAST gint MIN_VALUE = (gint) -(1LL << 31);

    public:

        /**
         * Returns a string representation of the first argument in the
         * radix specified by the second argument.
         *
         * If the radix is smaller than @c Character::MIN_RADIX
         * or larger than @c Character::MAX_RADIX, then the radix
         * @c 10 is used instead.
         *
         * If the first argument is negative, the first element of the
         * result is the ASCII minus character @c '-'
         * (@c '\\u002D'). If the first argument is not
         * negative, no sign character appears in the result.
         *
         * The remaining characters of the result represent the magnitude
         * of the first argument. If the magnitude is zero, it is
         * represented by a single zero character @c '0'
         * (@c '\\u0030'); otherwise, the first character of
         * the representation of the magnitude will not be the zero
         * character.  The following ASCII characters are used as digits:
         * @c 0123456789abcdefghijklmnopqrstuvwxyz
         *
         * These are @c '\\u0030' through
         * @c '\\u0039' and @c '\\u0061' through
         * @c '\\u007A'. If @c radix is @a N, then the first @a N of these characters
         * are used as radix-@a N digits in the order shown. Thus,
         * the digits for hexadecimal (radix 16) are
         * @c 0123456789abcdef. If uppercase letters are
         * desired, the @c String::toUpperCase() method may
         * be called on the result:
         *
         * <blockquote>
         *  @c Integer.toString(n, 16).toUpperCase()
         * </blockquote>
         *
         * @param   i       an integer to be converted to a string.
         * @param   radix   the radix to use in the string representation.
         * @return  a string representation of the argument in the specified radix.
         */
        static String toString(gint i, gint radix);

        /**
         * Returns a string representation of the first argument as an
         * unsigned integer value in the radix specified by the second
         * argument.
         *
         * If the radix is smaller than @c Character::MIN_RADIX
         * or larger than @c Character::MAX_RADIX, then the radix
         * @c 10 is used instead.
         *
         * Note that since the first argument is treated as an unsigned
         * value, no leading sign character is printed.
         *
         * If the magnitude is zero, it is represented by a single zero
         * character @c '0' (@c '\\u0030'); otherwise,
         * the first character of the representation of the magnitude will
         * not be the zero character.
         *
         * The behavior of radixes and the characters used as digits
         * are the same as @c Integer::toString(gint,gint) toString.
         *
         * @param   i       an integer to be converted to an unsigned string.
         * @param   radix   the radix to use in the string representation.
         * @return  an unsigned string representation of the argument in the specified radix.
         *
         */
        static String toUnsignedString(gint i, gint radix);

        /**
         * Returns a string representation of the integer argument as an
         * unsigned integer in base 16.
         *
         * The unsigned integer value is the argument plus 2<sup>32</sup>
         * if the argument is negative; otherwise, it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in hexadecimal (base 16) with no extra leading
         * @c 0s.
         *
         * The value of the argument can be recovered from the returned
         * string @c s by calling @c
         * Integer::parseUnsignedInt(String,gint)
         * Integer::parseUnsignedInt(s, 16).
         *
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character @c '0' (@c '\\u0030');
         * otherwise, the first character of the representation of the
         * unsigned magnitude will not be the zero character. The
         * following characters are used as hexadecimal digits:
         *
         * <blockquote>
         *  @c 0123456789abcdef
         * </blockquote>
         *
         * These are the characters @c '\\u0030' through
         * @c '\\u0039' and @c '\\u0061' through
         * @c '\\u0066'. If uppercase letters are
         * desired, the @c String::toUpperCase() method may
         * be called on the result:
         *
         * <blockquote>
         *  @c Integer.toHexString(n).toUpperCase()
         * </blockquote>
         *
         * @apiNote
         * The @c java.util.HexFormat class provides formatting and parsing
         * of byte arrays and primitives to return a string or adding to an @c Appendable.
         * @c HexFormat formats and parses uppercase or lowercase hexadecimal characters,
         * with leading zeros and for byte arrays includes for each byte
         * a delimiter, prefix, and suffix.
         *
         * @param   i   an integer to be converted to a string.
         * @return  the string representation of the unsigned integer value
         *          represented by the argument in hexadecimal (base 16).
         *
         */
        static String toHexString(gint i);

        /**
         * Returns a string representation of the integer argument as an
         * unsigned integer in base 8.
         *
         * The unsigned integer value is the argument plus 2<sup>32</sup>
         * if the argument is negative; otherwise, it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in octal (base 8) with no extra leading @c 0s.
         *
         * The value of the argument can be recovered from the returned
         * string @c s by calling @c
         * Integer::parseUnsignedInt(String,gint)
         * Integer::parseUnsignedInt(s, 8).
         *
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character @c '0' (@c '\\u0030');
         * otherwise, the first character of the representation of the
         * unsigned magnitude will not be the zero character. The
         * following characters are used as octal digits:
         *
         * <blockquote>
         * @c 01234567
         * </blockquote>
         *
         * These are the characters @c '\\u0030' through
         * @c '\\u0037'.
         *
         * @param   i   an integer to be converted to a string.
         * @return  the string representation of the unsigned integer value
         *          represented by the argument in octal (base 8).
         */
        static String toOctalString(gint i);

        /**
         * Returns a string representation of the integer argument as an
         * unsigned integer in base 2.
         *
         * The unsigned integer value is the argument plus 2<sup>32</sup>
         * if the argument is negative; otherwise it is equal to the
         * argument.  This value is converted to a string of ASCII digits
         * in binary (base 2) with no extra leading @c 0s.
         *
         * The value of the argument can be recovered from the returned
         * string @c s by calling @c
         * Integer::parseUnsignedInt(String,gint)
         * Integer::parseUnsignedInt(s, 2).
         *
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character @c '0' (@c '\\u0030');
         * otherwise, the first character of the representation of the
         * unsigned magnitude will not be the zero character. The
         * characters @c '0' (@c '\\u0030') and @c
         * '1' (@c '\\u0031') are used as binary digits.
         *
         * @param   i   an integer to be converted to a string.
         * @return  the string representation of the unsigned integer value
         *          represented by the argument in binary (base 2).
         *
         */
        static String toBinaryString(gint i);

        /**
         * Returns a @c String object representing the
         * specified integer. The argument is converted to signed decimal
         * representation and returned as a string, exactly as if the
         * argument and radix 10 were given as arguments to the @c Integer::toString(gint,gint)
         * method.
         *
         * @param   i   an integer to be converted.
         * @return  a string representation of the argument in base 10.
         */
        static String toString(gint i);

        /**
         * Returns a string representation of the argument as an unsigned
         * decimal value.
         *
         * The argument is converted to unsigned decimal representation
         * and returned as a string exactly as if the argument and radix
         * 10 were given as arguments to the @c Integer::toUnsignedString(gint,gint) method.
         *
         * @param   i  an integer to be converted to an unsigned string.
         * @return  an unsigned string representation of the argument.
         */
        static String toUnsignedString(gint i);

        /**
         * Parses the string argument as a signed integer in the radix
         * specified by the second argument. The characters in the string
         * must all be digits of the specified radix (as determined by
         * whether @c Character::digit(char,gint) returns a
         * non-negative value), except that the first character may be an
         * ASCII minus sign @c '-' (@c '\\u002D') to
         * indicate a negative value or an ASCII plus sign @c '+'
         * (@c '\\u002B') to indicate a positive value. The
         * resulting integer value is returned.
         *
         * An exception of type @c NumberFormatException is
         * thrown if any of the following situations occurs:
         * <ul>
         * <li>The first argument is @c null or is a string of
         * length zero.
         *
         * <li>The radix is either smaller than
         * @c Character::MIN_RADIX or
         * larger than @c Character::MAX_RADIX.
         *
         * <li>Any character of the string is not a digit of the specified
         * radix, except that the first character may be a minus sign
         * @c '-' (@c '\\u002D') or plus sign
         * @c '+' (@c '\\u002B') provided that the
         * string is longer than length 1.
         *
         * <li>The value represented by the string is not a value of type
         * @c gint.
         * </ul>
         *
         * Examples:
         * <blockquote><pre>
         * parseInt("0", 10) returns 0
         * parseInt("473", 10) returns 473
         * parseInt("+42", 10) returns 42
         * parseInt("-0", 10) returns 0
         * parseInt("-FF", 16) returns -255
         * parseInt("1100110", 2) returns 102
         * parseInt("2147483647", 10) returns 2147483647
         * parseInt("-2147483648", 10) returns -2147483648
         * parseInt("2147483648", 10) throws a NumberFormatException
         * parseInt("99", 8) throws a NumberFormatException
         * parseInt("Kona", 10) throws a NumberFormatException
         * parseInt("Kona", 27) returns 411787
         * </pre></blockquote>
         *
         * @param      s   the @c String containing the integer
         *                  representation to be parsed
         * @param      radix   the radix to be used while parsing @c s.
         * @return     the integer represented by the string argument in the
         *             specified radix.
         * @throws     NumberFormatException if the @c String
         *             does not contain a parsable @c gint.
         */
        static gint parseInt(String const &s, gint radix);

        /**
         * Parses the @c CharSequence argument as a signed @c gint in the
         * specified @c radix, beginning at the specified @c beginIndex
         * and extending to @c endIndex - 1.
         *
         * The method does not take steps to guard against the
         * @c CharSequence being mutated while parsing.
         *
         * @param      s   the @c CharSequence containing the @c gint
         *                  representation to be parsed
         * @param      beginIndex   the beginning index, inclusive.
         * @param      endIndex     the ending index, exclusive.
         * @param      radix   the radix to be used while parsing @c s.
         * @return     the signed @c gint represented by the subsequence in
         *             the specified radix.
         * @throws     NullPointerException  if @c s is null.
         * @throws     IndexOutOfBoundsException  if @c beginIndex is
         *             negative, or if @c beginIndex is greater than
         *             @c endIndex or if @c endIndex is greater than
         *             @c s.length().
         * @throws     NumberFormatException  if the @c CharSequence does not
         *             contain a parsable @c gint in the specified
         *             @c radix, or if @c radix is either smaller than
         *             @c Character::MIN_RADIX or larger than
         *             @c Character::MAX_RADIX.
         *
         */
        static gint parseInt(CharSequence const &s, gint beginIndex, gint endIndex, gint radix);

        /**
         * Parses the string argument as a signed decimal integer. The
         * characters in the string must all be decimal digits, except
         * that the first character may be an ASCII minus sign @c '-'
         * (@c '\\u002D') to indicate a negative value or an
         * ASCII plus sign @c '+' (@c '\\u002B') to
         * indicate a positive value. The resulting integer value is
         * returned, exactly as if the argument and the radix 10 were
         * given as arguments to the @c #parseInt(String,
         * gint) method.
         *
         * @param s    a @c String containing the @c gint
         *             representation to be parsed
         * @return     the integer value represented by the argument in decimal.
         * @throws     NumberFormatException  if the string does not contain a
         *               parsable integer.
         */
        static gint parseInt(String const &s);

        /**
         * Parses the string argument as an unsigned integer in the radix
         * specified by the second argument.  An unsigned integer maps the
         * values usually associated with negative numbers to positive
         * numbers larger than @c MAX_VALUE.
         *
         * The characters in the string must all be digits of the
         * specified radix (as determined by whether @c
         * Character::digit(char,gint) returns a non-negative
         * value), except that the first character may be an ASCII plus
         * sign @c '+' (@c '\\u002B'). The resulting
         * integer value is returned.
         *
         * An exception of type @c NumberFormatException is
         * thrown if any of the following situations occurs:
         * <ul>
         * <li>The first argument is @c null or is a string of
         * length zero.
         *
         * <li>The radix is either smaller than
         * @c Character::MIN_RADIX or
         * larger than @c Character::MAX_RADIX.
         *
         * <li>Any character of the string is not a digit of the specified
         * radix, except that the first character may be a plus sign
         * @c '+' (@c '\\u002B') provided that the
         * string is longer than length 1.
         *
         * <li>The value represented by the string is larger than the
         * largest unsigned @c gint, 2<sup>32</sup>-1.
         *
         * </ul>
         *
         *
         * @param      s   the @c String containing the unsigned integer
         *                  representation to be parsed
         * @param      radix   the radix to be used while parsing @c s.
         * @return     the integer represented by the string argument in the
         *             specified radix.
         * @throws     NumberFormatException if the @c String
         *             does not contain a parsable @c gint.
         *
         */
        static gint parseUnsignedInt(String const &s, gint radix);

        /**
         * Parses the @c CharSequence argument as an unsigned @c gint in
         * the specified @c radix, beginning at the specified
         * @c beginIndex and extending to @c endIndex - 1.
         *
         * The method does not take steps to guard against the
         * @c CharSequence being mutated while parsing.
         *
         * @param      s   the @c CharSequence containing the unsigned
         *                 @c gint representation to be parsed
         * @param      beginIndex   the beginning index, inclusive.
         * @param      endIndex     the ending index, exclusive.
         * @param      radix   the radix to be used while parsing @c s.
         * @return     the unsigned @c gint represented by the subsequence in
         *             the specified radix.
         * @throws     NullPointerException  if @c s is null.
         * @throws     IndexOutOfBoundsException  if @c beginIndex is
         *             negative, or if @c beginIndex is greater than
         *             @c endIndex or if @c endIndex is greater than
         *             @c s.length().
         * @throws     NumberFormatException  if the @c CharSequence does not
         *             contain a parsable unsigned @c gint in the specified
         *             @c radix, or if @c radix is either smaller than
         *             @c Character::MIN_RADIX or larger than
         *             @c Character::MAX_RADIX.
         *
         */
        static gint parseUnsignedInt(CharSequence const &s, gint beginIndex, gint endIndex, gint radix);

        /**
         * Parses the string argument as an unsigned decimal integer. The
         * characters in the string must all be decimal digits, except
         * that the first character may be an ASCII plus sign @c 
         * '+' (@c '\\u002B'). The resulting integer value
         * is returned, exactly as if the argument and the radix 10 were
         * given as arguments to the @c Integer::parseUnsignedInt(String,gint) method.
         *
         * @param s   a @c String containing the unsigned @c gint
         *            representation to be parsed
         * @return    the unsigned integer value represented by the argument in decimal.
         * @throws    NumberFormatException  if the string does not contain a
         *            parsable unsigned integer.
         *
         */
        static gint parseUnsignedInt(String const &s);

        /**
         * Returns an @c Integer object holding the value
         * extracted from the specified @c String when parsed
         * with the radix given by the second argument. The first argument
         * is interpreted as representing a signed integer in the radix
         * specified by the second argument, exactly as if the arguments
         * were given to the @c Integer::parseInt(String,gint)
         * method. The result is an @c Integer object that
         * represents the integer value specified by the string.
         *
         * In other words, this method returns an @c Integer
         * object equal to the value of:
         *
         * <blockquote>
         *  @c Integer.valueOf(Integer.parseInt(s, radix))
         * </blockquote>
         *
         * @param      s   the string to be parsed.
         * @param      radix the radix to be used in interpreting @c s
         * @return     an @c Integer object holding the value
         *             represented by the string argument in the specified
         *             radix.
         * @throws    NumberFormatException if the @c String
         *            does not contain a parsable @c gint.
         */
        static Integer valueOf(String const &s, gint radix);

        /**
         * Returns an @c Integer object holding the
         * value of the specified @c String. The argument is
         * interpreted as representing a signed decimal integer, exactly
         * as if the argument were given to the @c Integer::parseInt(String) method. The result is an
         * @c Integer object that represents the integer value
         * specified by the string.
         *
         * In other words, this method returns an @c Integer
         * object equal to the value of:
         *
         * <blockquote>
         *  @c Integer.valueOf(Integer.parseInt(s))
         * </blockquote>
         *
         * @param      s   the string to be parsed.
         * @return     an @c Integer object holding the value
         *             represented by the string argument.
         * @throws     NumberFormatException  if the string cannot be parsed
         *             as an integer.
         */
        static Integer valueOf(String const &s);

        /**
         * Returns an @c Integer instance representing the specified
         * @c gint value.  If a new @c Integer instance is not
         * required, this method should generally be used in preference to
         * the constructor @c Integer(gint), as this method is likely
         * to yield significantly better space and time performance by
         * caching frequently requested values.
         *
         * This method will always cache values in the range -128 to 127,
         * inclusive, and may cache other values outside of this range.
         *
         * @param  i an @c gint value.
         * @return an @c Integer instance representing @c i.
         *
         */
        static Integer valueOf(gint i);

    private:

        /**
         * The value of the @c Integer.
         */
        gint value;

    public:

        /**
         * Constructs a newly allocated @c Integer object that
         * represents the specified @c gint value.
         *
         * @param   value   the value to be represented by the
         *                  @c Integer object.
         *
         * @deprecated
         * It is rarely appropriate to use this constructor. The static factory
         * @c Integer::valueOf(gint) is generally a better choice, as it is
         * likely to yield significantly better space and time performance.
         */
        CORE_IMPLICIT Integer(gint value);

        /**
         * Returns the value of this @c Integer as a @c byte
         * after a narrowing primitive conversion.
         */
        gbyte byteValue() const override;

        /**
         * Returns the value of this @c Integer as a @c short
         * after a narrowing primitive conversion.
         * @jls 5.1.3 Narrowing Primitive Conversion
         */
        gshort shortValue() const override;

        /**
         * Returns the value of this @c Integer as an
         * @c gint.
         */
        gint intValue() const override;

        /**
         * Returns the value of this @c Integer as a @c long
         * after a widening primitive conversion.
         */
        glong longValue() const override;

        /**
         * Returns the value of this @c Integer as a @c float
         * after a widening primitive conversion.
         */
        gfloat floatValue() const override;

        /**
         * Returns the value of this @c Integer as a @c double
         * after a widening primitive conversion.
         */
        gdouble doubleValue() const override;

        /**
         * Returns a @c String object representing this
         * @c Integer's value. The value is converted to signed
         * decimal representation and returned as a string, exactly as if
         * the integer value were given as an argument to the @c Integer::toString(gint)
         * method.
         *
         * @return  a string representation of the value of this object in
         *          base 10.
         */
        String toString() const override;

        /**
         * Returns a hash code for this @c Integer.
         *
         * @return  a hash code value for this object, equal to the
         *          primitive @c gint value represented by this
         *          @c Integer object.
         */
        gint hash() const override;

        /**
         * Returns a hash code for an @c gint value; compatible with
         * @c Integer::hash().
         *
         * @param value the value to hash
         *
         *
         * @return a hash code value for an @c gint value.
         */
        static gint hash(gint value);

        /**
         * Compares this object to the specified object.  The result is
         * @c true if and only if the argument is not
         * @c null and is an @c Integer object that
         * contains the same @c gint value as this object.
         *
         * @param   obj   the object to compare with.
         * @return  @c true if the objects are the same;
         *          @c false otherwise.
         */
        gbool equals(Object const &obj) const override;

        /**
         * Decodes a @c String into an @c Integer.
         * Accepts decimal, hexadecimal, and octal numbers given
         * by the following grammar:
         *
         * <blockquote>
         * <dl>
         * <dt><i>DecodableString:</i>
         * <dd><i>Sign<sub>opt</sub> DecimalNumeral</i>
         * <dd><i>Sign<sub>opt</sub></i> @c 0x <i>HexDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> @c 0X <i>HexDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> @c # <i>HexDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> @c 0 <i>OctalDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> @c 0b <i>BinaryDigits</i>
         * <dd><i>Sign<sub>opt</sub></i> @c 0B <i>BinaryDigits</i>
         *
         * <dt><i>Sign:</i>
         * <dd>@c -
         * <dd>@c +
         * </dl>
         * </blockquote>
         *
         * The sequence of characters following an optional
         * sign and/or radix specifier (@c "0x", @c "0X",
         * @c "#", leading zero, @c "0b" or @c "0B") is parsed as by the
         * @c Integer::parseInt method with the indicated radix
         * (10, 16, 8, or 2).
         * This sequence of characters must represent a positive
         * value or a @c NumberFormatException will be thrown.  The
         * result is negated if first character of the specified @c 
         * String is the minus sign.  No whitespace characters are
         * permitted in the @c String.
         *
         * @param     nm the @c String to decode.
         * @return    an @c Integer object holding the @c gint
         *             value represented by @c nm
         * @throws    NumberFormatException  if the @c String does not
         *            contain a parsable integer.
         */
        static Integer decode(String const &nm);

        /**
         * Compares two @c Integer objects numerically.
         *
         * @param   anotherInteger   the @c Integer to be compared.
         * @return  the value @c 0 if this @c Integer is
         *          equal to the argument @c Integer; a value less than
         *          @c 0 if this @c Integer is numerically less
         *          than the argument @c Integer; and a value greater
         *          than @c 0 if this @c Integer is numerically
         *           greater than the argument @c Integer (signed
         *           comparison).
         *
         */
        gint compareTo(Integer const &anotherInteger) const override;

        /**
         * Compares two @c gint values numerically.
         * The value returned is identical to what would be returned by:
         * <pre>
         *    Integer.valueOf(x).compareTo(Integer.valueOf(y))
         * </pre>
         *
         * @param  x the first @c gint to compare
         * @param  y the second @c gint to compare
         * @return the value @c 0 if @c x == y;
         *         a value less than @c 0 if @c x < y; and
         *         a value greater than @c 0 if @c x > y
         *
         */
        static gint compare(gint x, gint y);

        /**
         * Compares two @c gint values numerically treating the values
         * as unsigned.
         *
         * @param  x the first @c gint to compare
         * @param  y the second @c gint to compare
         * @return the value @c 0 if @c x == y; a value less
         *         than @c 0 if @c x < y as unsigned values; and
         *         a value greater than @c 0 if @c x > y as
         *         unsigned values
         *
         */
        static gint compareUnsigned(gint x, gint y);

        /**
         * Converts the argument to a @c long by an unsigned
         * conversion.  In an unsigned conversion to a @c long, the
         * high-order 32 bits of the @c long are zero and the
         * low-order 32 bits are equal to the bits of the integer
         * argument.
         *
         * Consequently, zero and positive @c gint values are mapped
         * to a numerically equal @c long value and negative @c 
         * gint values are mapped to a @c long value equal to the
         * input plus 2<sup>32</sup>.
         *
         * @param  x the value to convert to an unsigned @c long
         * @return the argument converted to @c long by an unsigned
         *         conversion
         *
         */
        static glong toUnsignedLong(gint x);

        /**
         * Returns the unsigned quotient of dividing the first argument by
         * the second where each argument and the result is interpreted as
         * an unsigned value.
         *
         * <p>Note that in two's complement arithmetic, the three other
         * basic arithmetic operations of add, subtract, and multiply are
         * bit-wise identical if the two operands are regarded as both
         * being signed or both being unsigned.  Therefore separate @c 
         * addUnsigned, etc. methods are not provided.
         *
         * @param dividend the value to be divided
         * @param divisor the value doing the dividing
         * @return the unsigned quotient of the first argument divided by
         * the second argument
         *
         */
        static gint divideUnsigned(gint dividend, gint divisor);

        /**
         * Returns the unsigned remainder from dividing the first argument
         * by the second where each argument and the result is interpreted
         * as an unsigned value.
         *
         * @param dividend the value to be divided
         * @param divisor the value doing the dividing
         * @return the unsigned remainder of the first argument divided by
         * the second argument
         *
         */
        static gint remainderUnsigned(gint dividend, gint divisor);


        // Bit twiddling

        /**
         * The number of bits used to represent an @c gint value in two's
         * complement binary form.
         *
         *
         */
        static CORE_FAST gint SIZE = 32;

        /**
         * The number of bytes used to represent an @c gint value in two's
         * complement binary form.
         *
         *
         */
        static CORE_FAST gint BYTES = SIZE / Byte::SIZE;

        /**
         * Returns an @c gint value with at most a single one-bit, in the
         * position of the highest-order ("leftmost") one-bit in the specified
         * @c gint value.  Returns zero if the specified value has no
         * one-bits in its two's complement binary representation, that is, if it
         * is equal to zero.
         *
         * @param i the value whose highest one bit is to be computed
         * @return an @c gint value with a single one-bit, in the position
         *     of the highest-order one-bit in the specified value, or zero if
         *     the specified value is itself equal to zero.
         *
         */
        static gint highestOneBit(gint i);

        /**
         * Returns an @c gint value with at most a single one-bit, in the
         * position of the lowest-order ("rightmost") one-bit in the specified
         * @c gint value.  Returns zero if the specified value has no
         * one-bits in its two's complement binary representation, that is, if it
         * is equal to zero.
         *
         * @param i the value whose lowest one bit is to be computed
         * @return an @c gint value with a single one-bit, in the position
         *     of the lowest-order one-bit in the specified value, or zero if
         *     the specified value is itself equal to zero.
         *
         */
        static gint lowestOneBit(gint i);

        /**
         * Returns the number of zero bits preceding the highest-order
         * ("leftmost") one-bit in the two's complement binary representation
         * of the specified @c gint value.  Returns 32 if the
         * specified value has no one-bits in its two's complement representation,
         * in other words if it is equal to zero.
         *
         * <p>Note that this method is closely related to the logarithm base 2.
         * For all positive @c gint values x:
         * <ul>
         * <li>floor(log<sub>2</sub>(x)) = @c 31 - numberOfLeadingZeros(x)
         * <li>ceil(log<sub>2</sub>(x)) = @c 32 - numberOfLeadingZeros(x - 1)
         * </ul>
         *
         * @param i the value whose number of leading zeros is to be computed
         * @return the number of zero bits preceding the highest-order
         *     ("leftmost") one-bit in the two's complement binary representation
         *     of the specified @c gint value, or 32 if the value
         *     is equal to zero.
         *
         */
        static gint numberOfLeadingZeros(gint i);

        /**
         * Returns the number of zero bits following the lowest-order ("rightmost")
         * one-bit in the two's complement binary representation of the specified
         * @c gint value.  Returns 32 if the specified value has no
         * one-bits in its two's complement representation, in other words if it is
         * equal to zero.
         *
         * @param i the value whose number of trailing zeros is to be computed
         * @return the number of zero bits following the lowest-order ("rightmost")
         *     one-bit in the two's complement binary representation of the
         *     specified @c gint value, or 32 if the value is equal
         *     to zero.
         *
         */
        static gint numberOfTrailingZeros(gint i);

        /**
         * Returns the number of one-bits in the two's complement binary
         * representation of the specified @c gint value.  This function is
         * sometimes referred to as the <i>population count</i>.
         *
         * @param i the value whose bits are to be counted
         * @return the number of one-bits in the two's complement binary
         *     representation of the specified @c gint value.
         *
         */
        static gint bitCount(gint i);

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified @c gint value left by the
         * specified number of bits.  (Bits shifted out of the left hand, or
         * high-order, side reenter on the right, or low-order.)
         *
         * <p>Note that left rotation with a negative distance is equivalent to
         * right rotation: @c rotateLeft(val, -distance) == rotateRight(val,
         * distance).  Note also that rotation by any multiple of 32 is a
         * no-op, so all but the last five bits of the rotation distance can be
         * ignored, even if the distance is negative: @c rotateLeft(val,
         * distance) == rotateLeft(val, distance & 0x1F).
         *
         * @param i the value whose bits are to be rotated left
         * @param distance the number of bit positions to rotate left
         * @return the value obtained by rotating the two's complement binary
         *     representation of the specified @c gint value left by the
         *     specified number of bits.
         *
         */
        static gint rotateLeft(gint i, gint distance);

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified @c gint value right by the
         * specified number of bits.  (Bits shifted out of the right hand, or
         * low-order, side reenter on the left, or high-order.)
         *
         * <p>Note that right rotation with a negative distance is equivalent to
         * left rotation: @c rotateRight(val, -distance) == rotateLeft(val,
         * distance).  Note also that rotation by any multiple of 32 is a
         * no-op, so all but the last five bits of the rotation distance can be
         * ignored, even if the distance is negative: @c rotateRight(val,
         * distance) == rotateRight(val, distance & 0x1F).
         *
         * @param i the value whose bits are to be rotated right
         * @param distance the number of bit positions to rotate right
         * @return the value obtained by rotating the two's complement binary
         *     representation of the specified @c gint value right by the
         *     specified number of bits.
         *
         */
        static gint rotateRight(gint i, gint distance);

        /**
         * Returns the value obtained by reversing the order of the bits in the
         * two's complement binary representation of the specified @c gint
         * value.
         *
         * @param i the value to be reversed
         * @return the value obtained by reversing order of the bits in the
         *     specified @c gint value.
         *
         */
        static gint reverse(gint i);

        /**
         * Returns the signum function of the specified @c gint value.  (The
         * return value is -1 if the specified value is negative; 0 if the
         * specified value is zero; and 1 if the specified value is positive.)
         *
         * @param i the value whose signum is to be computed
         * @return the signum function of the specified @c gint value.
         *
         */
        static gint signum(gint i);

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified @c gint value.
         *
         * @param i the value whose bytes are to be reversed
         * @return the value obtained by reversing the bytes in the specified
         *     @c gint value.
         *
         */
        static gint reverseBytes(gint i);

        /**
         * Adds two integers together as per the + operator.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the sum of @c a and @c b
         *
         */
        static gint sum(gint a, gint b);

        /**
         * Returns the greater of two @c gint values
         * as if by calling @c Math::max(gint,gint) Math.max.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the greater of @c a and @c b
         *
         */
        static gint max(gint a, gint b);

        /**
         * Returns the smaller of two @c gint values
         * as if by calling @c Math::min(gint,gint) Math.min.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the smaller of @c a and @c b
         *
         */
        static gint min(gint a, gint b);

        CORE_IMPLICIT operator gint() const;

        CORE_IMPLICIT operator gint &();

        Object& clone() const override;
    };
} // core

#endif // CORE24_INTEGER_H
