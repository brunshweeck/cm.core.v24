//
// Created by bruns on 13/05/2024.
//

#ifndef CORE24_LONG_H
#define CORE24_LONG_H

#include <core/lang/Byte.h>

namespace core {
    /**
     * The @c Long class wraps a value of the primitive type @c glong
     * in an object. An object of type @c Long contains a
     * single field whose type is @c glong.
     *
     * Implementation note: The implementations of the "bit twiddling"
     * methods (such as @c Long::highestOneBit and @c Long::numberOfTrailingZeros) are
     * based on material from Henry S. Warren, Jr.'s <i>Hacker's
     * Delight</i>, (Addison Wesley, 2002).
     */
    class Long : public virtual Number, public virtual Comparable<Long> {
    public:
        /**
         * A constant holding the maximum value a glong can
         * have, 2<sup>63</sup> - 1
         */
        static CORE_FAST glong MAX_VALUE = (1ULL << 63) - 1;

        /**
         * A constant holding the minimum value a glong can
         * have, -2<sup>63</sup>
         */
        static CORE_FAST glong MIN_VALUE = 1LL << 63;

        /**
         * Returns a string representation of the first argument in the
         * radix specified by the second argument.
         *
         * If the radix is smaller than @c Character::MIN_RADIX
         * or larger than @c Character::MAX_RADIX, then the radix
         * @c 10 is used instead.
         *
         * If the first argument is negative, the first element of the
         * result is the ASCII minus sign @c '-'. If the first argument is not
         * negative, no sign character appears in the result.
         *
         * The remaining characters of the result represent the magnitude
         * of the first argument. If the magnitude is zero, it is
         * represented by a single zero character @c '0' otherwise, the first character of
         * the representation of the magnitude will not be the zero
         * character.  The following ASCII characters are used as digits:
         * @a 0123456789abcdefghijklmnopqrstuvwxyz.
         *
         * If @c radix is @c N, then the first @c N of these characters
         * are used as radix-@c N digits in the order shown. Thus,
         * the digits for hexadecimal (radix 16) are
         * @c 0123456789abcdef. If uppercase letters are
         * desired, the @c String::toUpperCase() method may
         * be called on the result: Long::toString(n, 16).toUpperCase()
         *
         * @param   i       a @c glong to be converted to a string.
         * @param   radix   the radix to use in the string representation.
         * @return  a string representation of the argument in the specified radix.
         */
        static String toString(glong i, gint radix);

        /**
         * Returns a string representation of the first argument as an
         * unsigned integer value in the radix specified by the second
         * argument.
         *
         * If the radix is smaller than @c Character::MIN_RADIX
         * or larger than @c Character::MAX_RADIX, then the radix
         * @c 10 is used instead.
         *
         * @note Since the first argument is treated as an unsigned
         * value, no leading sign character is printed.
         *
         * If the magnitude is zero, it is represented by a single zero
         * character @c '0'; otherwise, the first character of the representation
         * of the magnitude will not be the zero character.
         *
         * The behavior of radixes and the characters used as digits
         * are the same as @c Long::toString.
         *
         * @param   i       an integer to be converted to an unsigned string.
         * @param   radix   the radix to use in the string representation.
         * @return  an unsigned string representation of the argument in the specified radix.
         *
         */
        static String toUnsignedString(glong i, gint radix);

        /**
         * Returns a string representation of the @c glong
         * argument as an unsigned integer in radix 16.
         *
         * The unsigned @c glong value is the argument plus
         * 2<sup>64</sup> if the argument is negative; otherwise, it is
         * equal to the argument.  This value is converted to a string of
         * ASCII digits in hexadecimal (radix 16) with no extra
         * leading @c 0s.
         *
         * The value of the argument can be recovered from the returned
         * string @c s by calling @c Long::parseUnsignedLong(s,16).
         *
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character @c '0';
         * otherwise, the first character of the representation of the
         * unsigned magnitude will not be the zero character. The
         * following characters are used as hexadecimal digits:
         * @a 0123456789abcdef
         *
         * If uppercase letters are desired,
         * the @c String#toUpperCase() method may be called
         * on the result: @c Long::toHexString(n).toUpperCase()
         *
         * @param   i   a @c glong to be converted to a string.
         * @return  the string representation of the unsigned @c glong
         *          value represented by the argument in hexadecimal
         *          (radix 16).
         *
         */
        static String toHexString(glong i);

        /**
         * Returns a string representation of the @c glong
         * argument as an unsigned integer in radix 8.
         *
         * The unsigned @c glong value is the argument plus
         * 2<sup>64</sup> if the argument is negative; otherwise, it is
         * equal to the argument.  This value is converted to a string of
         * ASCII digits in octal (radix 8) with no extra leading
         * @c 0s.
         *
         * The value of the argument can be recovered from the returned
         * string @c s by calling @c Long::parseUnsignedLong(s, 8).
         *
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character @c '0'; otherwise, the first character
         * of the representation of the unsigned magnitude will not be the zero character.
         * The following characters are used as octal digits: @a 01234567
         *
         * @param   i   a @c glong to be converted to a string.
         * @return  the string representation of the unsigned @c glong
         *          value represented by the argument in octal (radix 8).
         *
         */
        static String toOctalString(glong i);

        /**
         * Returns a string representation of the @c glong
         * argument as an unsigned integer in radix 2.
         *
         * The unsigned @c glong value is the argument plus
         * 2<sup>64</sup> if the argument is negative; otherwise, it is
         * equal to the argument.  This value is converted to a string of
         * ASCII digits in binary (radix 2) with no extra leading
         * @c 0s.
         *
         * The value of the argument can be recovered from the returned
         * string @c s by calling @c Long::parseUnsignedLong(s,2).
         *
         * If the unsigned magnitude is zero, it is represented by a
         * single zero character @c '0'; otherwise, the first character of
         * the representation of the unsigned magnitude will not be the zero character.
         * The characters @c '0' and @c '1' are used as binary digits.
         *
         * @param   i   a @c glong to be converted to a string.
         * @return  the string representation of the unsigned @c glong
         *          value represented by the argument in binary (radix 2).
         *
         */
        static String toBinaryString(glong i);

        /**
         * Returns a @c String object representing the specified
         * @c glong.  The argument is converted to signed decimal
         * representation and returned as a string, exactly as if the
         * argument and the radix 10 were given as arguments to the
         * @c Long::toString(glong,gint) method.
         *
         * @param   i   a @c glong to be converted.
         * @return  a string representation of the argument in radix 10.
         */
        static String toString(glong i);

        /**
         * Returns a string representation of the argument as an unsigned
         * decimal value.
         *
         * The argument is converted to unsigned decimal representation
         * and returned as a string exactly as if the argument and radix
         * 10 were given as arguments to the @c Long::toUnsignedString(glong,int)
         * method.
         *
         * @param   i  an integer to be converted to an unsigned string.
         * @return  an unsigned string representation of the argument.
         */
        static String toUnsignedString(glong i);

        /**
         * Parses the string argument as a signed @c glong in the
         * radix specified by the second argument. The characters in the
         * string must all be digits of the specified radix (as determined
         * by whether @c Character::digit(char,gint) returns
         * a non-negative value), except that the first character may be an
         * ASCII minus sign @c '-' to indicate a negative value or
         * an ASCII plus sign @c '+' to indicate a positive value. The
         * resulting @c glong value is returned.
         *
         * @note Neither the character @c L nor @c l is permitted to appear at the end
         * of the string as a type indicator, as would be permitted in
         * C++ programming language source code - except that either
         * @c L or @c l may appear as a digit for a radix greater than or equal to 22.
         *
         * An exception of type @c NumberFormatException is
         * thrown if any of the following situations occurs:
         * <ul>
         *
         * <li>The first argument is a string of length zero.
         * <li>The @c radix is either smaller than @c Character::MIN_RADIX or larger than
         * @c Character::MAX_RADIX.
         * <li>Any character of the string is not a digit of the specified
         * radix, except that the first character may be a minus sign
         * @c '-' or plus sign @c '+' provided that the string is
         * longer than length 1.
         *
         * <li>The value represented by the string is not a value of type @c glong.
         * </ul>
         *
         * Examples:
         * @code
         * parseLong("0", 10) returns 0L
         * parseLong("473", 10) returns 473L
         * parseLong("+42", 10) returns 42L
         * parseLong("-0", 10) returns 0L
         * parseLong("-FF", 16) returns -255L
         * parseLong("1100110", 2) returns 102L
         * parseLong("99", 8) throws a NumberFormatException
         * parseLong("Hazelnut", 10) throws a NumberFormatException
         * parseLong("Hazelnut", 36) returns 1356099454469L
         * @endcode
         *
         * @param      s       the @c String containing the
         *                     @c glong representation to be parsed.
         * @param      radix   the radix to be used while parsing @c s.
         * @return     the @c glong represented by the string argument in
         *             the specified radix.
         * @throws     NumberFormatException  if the string does not contain a
         *             parsable @c glong.
         */
        static glong parseLong(String const &s, gint radix);

        /**
         * Parses the @cCharSequence argument as a signed @c glong in
         * the specified @c radix, beginning at the specified
         * @c beginIndex and extending to @c endIndex - 1.
         *
         * The method does not take steps to guard against the
         * @c CharSequence being mutated while parsing.
         *
         * @param      s   the @c CharSequence containing the @c glong
         *                  representation to be parsed
         * @param      beginIndex   the beginning index, inclusive.
         * @param      endIndex     the ending index, exclusive.
         * @param      radix   the radix to be used while parsing @c s.
         * @return     the signed @c glong represented by the subsequence in
         *             the specified radix.
         * @throws     IndexOutOfBoundsException  if @c beginIndex is
         *             negative, or if @c beginIndex is greater than
         *             @c endIndex or if @c endIndex is greater than
         *             @c s.length().
         * @throws     NumberFormatException  if the @c CharSequence does not
         *             contain a parsable @c glong in the specified
         *             @c radix, or if @c radix is either smaller than
         *             @cCharacter#MIN_RADIX or larger than
         *             @cCharacter#MAX_RADIX.
         *
         */
        static glong parseLong(CharSequence const &s, gint beginIndex, gint endIndex, gint radix);

        /**
         * Parses the string argument as a signed decimal @c glong.
         * The characters in the string must all be decimal digits, except
         * that the first character may be an ASCII minus sign @c '-'
         * (@c \\u002D') to indicate a negative value or an
         * ASCII plus sign @c '+' to indicate a positive value.
         * The resulting @c glong value is returned, exactly as if the argument
         * and the radix @c 10 were given as arguments to the @c Long::parseLong(String,gint) method.
         *
         * @note that neither the character @c L nor @c l is permitted to appear at the end
         * of the string as a type indicator, as would be permitted in
         * C++ programming language source code.
         *
         * @param      s   a @c String containing the @c glong representation to be parsed
         * @return     the @c glong represented by the argument in decimal.
         * @throws     NumberFormatException  if the string does not contain a parsable @c glong.
         */
        static glong parseLong(String const &s);

        /**
         * Parses the string argument as an unsigned @c glong in the
         * radix specified by the second argument.  An unsigned integer
         * maps the values usually associated with negative numbers to
         * positive numbers larger than @c MAX_VALUE.
         *
         * The characters in the string must all be digits of the
         * specified radix (as determined by whether @c Character::digit(char,gint) returns a non-negative
         * value), except that the first character may be an ASCII plus
         * sign @c '+'. The resulting
         * integer value is returned.
         *
         * An exception of type @c NumberFormatException is
         * thrown if any of the following situations occurs:
         * <li>The first argument is @c null or is a string of length zero.
         * <li>The radix is either smaller than
         * @c Character::MIN_RADIX or larger than @c Character::MAX_RADIX.
         *
         * <li>Any character of the string is not a digit of the specified
         * radix, except that the first character may be a plus sign
         * @c '+' provided that the string is longer than length 1.
         *
         * <li>The value represented by the string is larger than the
         * largest unsigned @c glong, 2<sup>64</sup>-1.
         *
         *
         * @param      s   the @c String containing the unsigned integer
         *                  representation to be parsed
         * @param      radix   the radix to be used while parsing @c s.
         * @return     the unsigned @c glong represented by the string
         *             argument in the specified radix.
         * @throws     NumberFormatException if the @c String
         *             does not contain a parsable @c glong.
         *
         */
        static glong parseUnsignedLong(String const &s, gint radix);

        /**
         * Parses the @c CharSequence argument as an unsigned @c glong in
         * the specified @c radix, beginning at the specified
         * @c beginIndex and extending to @c endIndex - 1.
         *
         * The method does not take steps to guard against the
         * @c CharSequence being mutated while parsing.
         *
         * @param      s   the @c CharSequence containing the unsigned
         *                 @c glong representation to be parsed
         * @param      beginIndex   the beginning index, inclusive.
         * @param      endIndex     the ending index, exclusive.
         * @param      radix   the radix to be used while parsing @c s.
         * @return     the unsigned @c glong represented by the subsequence in
         *             the specified radix.
         * @throws     IndexOutOfBoundsException  if @c beginIndex is
         *             negative, or if @c beginIndex is greater than
         *             @c endIndex or if @c endIndex is greater than
         *             @c s.length().
         * @throws     NumberFormatException  if the @c CharSequence does not
         *             contain a parsable unsigned @c glong in the specified
         *             @c radix, or if @c radix is either smaller than
         *             @c Character::MIN_RADIX or larger than
         *             @c Character::MAX_RADIX.
         *
         */
        static glong parseUnsignedLong(CharSequence const &s, gint beginIndex, gint endIndex, gint radix);

        /**
         * Parses the string argument as an unsigned decimal @c glong. The
         * characters in the string must all be decimal digits, except
         * that the first character may be an ASCII plus sign @c '+'.
         * The resulting integer value is returned, exactly as if the argument and the radix 10 were
         * given as arguments to the @c Long::parseUnsignedLong(String,gint) method.
         *
         * @param s   a @c String containing the unsigned @c glong representation to be parsed
         * @return    the unsigned @c glong value represented by the decimal string argument
         * @throws    NumberFormatException  if the string does not contain a
         *            parsable unsigned integer.
         *
         */
        static glong parseUnsignedLong(String const &s);

        /**
         * Returns a @c Long object holding the value
         * extracted from the specified @c String when parsed
         * with the radix given by the second argument.  The first
         * argument is interpreted as representing a signed
         * @c glong in the radix specified by the second
         * argument, exactly as if the arguments were given to the
         * @c Long::parseLong(String,gint) method. The result is a
         * @c Long object that represents the @c glong
         * value specified by the string.
         *
         * In other words, this method returns a @c Long object equal
         * to the value of: @c Long::valueOf(Long::parseLong(s, radix))
         *
         * @param      s       the string to be parsed
         * @param      radix   the radix to be used in interpreting @c s
         * @return     a @c Long object holding the value
         *             represented by the string argument in the specified
         *             radix.
         * @throws     NumberFormatException  If the @c String does not
         *             contain a parsable @c glong.
         */
        static Long valueOf(String const &s, gint radix);

        /**
         * Returns a @c Long object holding the value
         * of the specified @c String. The argument is
         * interpreted as representing a signed decimal @c glong,
         * exactly as if the argument were given to the
         * @c Long::parseLong(String) method. The result is a
         * @c Long object that represents the integer value
         * specified by the string.
         *
         * In other words, this method returns a @c Long object
         * equal to the value of: @c Long::valueOf(Long::parseLong(s))
         *
         * @param      s   the string to be parsed.
         * @return     a @c Long object holding the value
         *             represented by the string argument.
         * @throws     NumberFormatException  If the string cannot be parsed
         *             as a @c glong.
         */
        static Long valueOf(String const &s);

        /**
         * Returns a @c Long instance representing the specified
         * @c glong value.
         * If a new @c Long instance is not required, this method
         * should generally be used in preference to the constructor
         * @c Long(glong), as this method is likely to yield
         * significantly better space and time performance by caching
         * frequently requested values.
         *
         * This method will always cache values in the range -128 to 127,
         * inclusive, and may cache other values outside of this range.
         *
         * @param  l a glong value.
         * @return a @c Long instance representing @c l.
         *
         */
        static Long valueOf(glong l);

        /**
         * Decodes a @c String into a @c Long.
         * Accepts decimal, hexadecimal, and octal numbers given by the
         * following grammar:
         *
         * <dl>
         * <dt><i>DecodableString:</i>
         * <dd><i>Sign<sub>(opt)</sub> DecimalNumeral</i>
         * <dd><i>Sign<sub>(opt)</sub></i> @c 0x <i>HexDigits</i>
         * <dd><i>Sign<sub>(opt)</sub></i> @c 0X <i>HexDigits</i>
         * <dd><i>Sign<sub>(opt)</sub></i> @c # <i>HexDigits</i>
         * <dd><i>Sign<sub>(opt)</sub></i> @c 0 <i>OctalDigits</i>
         *
         * <dt><i>Sign:</i>
         * <dd>@c -
         * <dd>@c +
         * </dl>
         *
         * The sequence of characters following an optional
         * sign and/or radix specifier ("@c 0x", "@c 0X",
         * "@c #", or leading zero) is parsed as by the @c 
         * Long::parseLong method with the indicated radix (10, 16, or 8).
         * This sequence of characters must represent a positive value or
         * a @cNumberFormatException will be thrown.  The result is
         * negated if first character of the specified @c String is
         * the minus sign.  No whitespace characters are permitted in the
         * @c String.
         *
         * @param     nm the @c String to decode.
         * @return    a @c Long object holding the @c glong
         *            value represented by @c nm
         * @throws    NumberFormatException  if the @c String does not
         *            contain a parsable @c glong.
         */
        static Long decode(String const &nm);

    private:
        /**
         * The value of the @c Long.
         */
        glong value;

    public:
        /**
         * Constructs a newly allocated @c Long object that
         * represents the specified @c glong argument.
         *
         * @param   value   the value to be represented by the
         *          @c Long object.
         *
         * @deprecated
         * It is rarely appropriate to use this constructor. The static factory
         * @c#valueOf(glong) is generally a better choice, as it is
         * likely to yield significantly better space and time performance.
         */
        CORE_IMPLICIT Long(glong value);

        /**
         * Returns the value of this @c Long as a @c byte after
         * a narrowing primitive conversion.
         * @jls 5.1.3 Narrowing Primitive Conversion
         */
        gbyte byteValue() const override;

        /**
         * Returns the value of this @c Long as a @c short after
         * a narrowing primitive conversion.
         * @jls 5.1.3 Narrowing Primitive Conversion
         */
        gshort shortValue() const override;

        /**
         * Returns the value of this @c Long as an @c gint after
         * a narrowing primitive conversion.
         * @jls 5.1.3 Narrowing Primitive Conversion
         */
        gint intValue() const override;

        /**
         * Returns the value of this @c Long as a
         * @c glong value.
         */
        glong longValue() const override;

        /**
         * Returns the value of this @c Long as a @c float after
         * a widening primitive conversion.
         * @jls 5.1.2 Widening Primitive Conversion
         */
        gfloat floatValue() const override;

        /**
         * Returns the value of this @c Long as a @c double
         * after a widening primitive conversion.
         * @jls 5.1.2 Widening Primitive Conversion
         */
        gdouble doubleValue() const override;

        /**
         * Returns a @c String object representing this
         * @c Long's value.  The value is converted to signed
         * decimal representation and returned as a string, exactly as if
         * the @c glong value were given as an argument to the
         * @cLong::toString(glong) method.
         *
         * @return  a string representation of the value of this object in
         *          radix 10.
         */
        String toString() const override;

        /**
         * Returns a hash code for this @c Long. The result is
         * the exclusive OR of the two halves of the primitive
         * @c glong value held by this @c Long
         * object. That is, the hash code is the value of the expression:
         *
         * <blockquote>
         *  @c (gint)(this.longValue()^(this.longValue()>>>32))
         * </blockquote>
         *
         * @return  a hash code value for this object.
         */
        gint hash() const override;

        /**
         * Returns a hash code for a @c glong value; compatible with
         * @c Long::hash().
         *
         * @param value the value to hash
         * @return a hash code value for a @c glong value.
         *
         */
        static gint hash(glong value);

        /**
         * Compares this object to the specified object.  The result is
         * @c true if and only if the argument is not
         * @c null and is a @c Long object that
         * contains the same @c glong value as this object.
         *
         * @param   obj   the object to compare with.
         * @return  @c true if the objects are the same;
         *          @c false otherwise.
         */
        gbool equals(Object const &obj) const override;

        /**
         * Compares two @c Long objects numerically.
         *
         * @param   anotherLong   the @c Long to be compared.
         * @return  the value @c 0 if this @c Long is
         *          equal to the argument @c Long; a value less than
         *          @c 0 if this @c Long is numerically less
         *          than the argument @c Long; and a value greater
         *          than @c 0 if this @c Long is numerically
         *           greater than the argument @c Long (signed
         *           comparison).
         *
         */
        gint compareTo(Long const &anotherLong) const override;

        /**
         * Compares two @c glong values numerically.
         * The value returned is identical to what would be returned by:
         * <pre>
         *    Long::valueOf(x).compareTo(Long::valueOf(y))
         * </pre>
         *
         * @param  x the first @c glong to compare
         * @param  y the second @c glong to compare
         * @return the value @c 0 if @c x == y;
         *         a value less than @c 0 if @c x < y; and
         *         a value greater than @c 0 if @c x > y
         *
         */
        static gint compare(glong x, glong y);

        /**
         * Compares two @c glong values numerically treating the values
         * as unsigned.
         *
         * @param  x the first @c glong to compare
         * @param  y the second @c glong to compare
         * @return the value @c 0 if @c x == y; a value less
         *         than @c 0 if @c x < y as unsigned values; and
         *         a value greater than @c 0 if @c x > y as
         *         unsigned values
         *
         */
        static gint compareUnsigned(glong x, glong y);

        /**
         * Returns the unsigned quotient of dividing the first argument by
         * the second where each argument and the result is interpreted as
         * an unsigned value.
         *
         * @note: In two's complement arithmetic, the three other
         * basic arithmetic operations of add, subtract, and multiply are
         * bit-wise identical if the two operands are regarded as both
         * being signed or both being unsigned.  Therefore separate
         * @c  Long::addUnsigned, etc. methods are not provided.
         *
         * @param dividend the value to be divided
         * @param divisor the value doing the dividing
         * @return the unsigned quotient of the first argument divided by
         * the second argument
         *
         */
        static glong divideUnsigned(glong dividend, glong divisor);

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
        static glong remainderUnsigned(glong dividend, glong divisor);

        // Bit Twiddling

        /**
         * The number of bits used to represent a @c glong value in two's
         * complement binary form.
         *
         *
         */
        static CORE_FAST gint SIZE = 64;

        /**
         * The number of bytes used to represent a @c glong value in two's
         * complement binary form.
         *
         *
         */
        static CORE_FAST gint BYTES = SIZE / Byte::SIZE;

        /**
         * Returns a @c glong value with at most a single one-bit, in the
         * position of the highest-order ("leftmost") one-bit in the specified
         * @c glong value.  Returns zero if the specified value has no
         * one-bits in its two's complement binary representation, that is, if it
         * is equal to zero.
         *
         * @param i the value whose highest one bit is to be computed
         * @return a @c glong value with a single one-bit, in the position
         *     of the highest-order one-bit in the specified value, or zero if
         *     the specified value is itself equal to zero.
         *
         */
        static glong highestOneBit(glong i);

        /**
         * Returns a @c glong value with at most a single one-bit, in the
         * position of the lowest-order ("rightmost") one-bit in the specified
         * @c glong value.  Returns zero if the specified value has no
         * one-bits in its two's complement binary representation, that is, if it
         * is equal to zero.
         *
         * @param i the value whose lowest one bit is to be computed
         * @return a @c glong value with a single one-bit, in the position
         *     of the lowest-order one-bit in the specified value, or zero if
         *     the specified value is itself equal to zero.
         *
         */
        static glong lowestOneBit(glong i);

        /**
         * Returns the number of zero bits preceding the highest-order
         * ("leftmost") one-bit in the two's complement binary representation
         * of the specified @c glong value.  Returns 64 if the
         * specified value has no one-bits in its two's complement representation,
         * in other words if it is equal to zero.
         *
         * @note This method is closely related to the logarithm radix 2.
         * For all positive @c glong values x:
         *
         * <li> @c Math::floor(Math::log2(x)) = @c 63-Long::numberOfLeadingZeros(x)
         * <li> @c Math::ceil(Math::log2(x)) = @c 64-Long::numberOfLeadingZeros(x-1)
         *
         * @param i the value whose number of leading zeros is to be computed
         * @return the number of zero bits preceding the highest-order
         *     ("leftmost") one-bit in the two's complement binary representation
         *     of the specified @c glong value, or 64 if the value
         *     is equal to zero.
         *
         */
        static gint numberOfLeadingZeros(glong i);

        /**
         * Returns the number of zero bits following the lowest-order ("rightmost")
         * one-bit in the two's complement binary representation of the specified
         * @c glong value.  Returns 64 if the specified value has no
         * one-bits in its two's complement representation, in other words if it is
         * equal to zero.
         *
         * @param i the value whose number of trailing zeros is to be computed
         * @return the number of zero bits following the lowest-order ("rightmost")
         *     one-bit in the two's complement binary representation of the
         *     specified @c glong value, or 64 if the value is equal
         *     to zero.
         *
         */
        static gint numberOfTrailingZeros(glong i);

        /**
         * Returns the number of one-bits in the two's complement binary
         * representation of the specified @c glong value.  This function is
         * sometimes referred to as the <i>population count</i>.
         *
         * @param i the value whose bits are to be counted
         * @return the number of one-bits in the two's complement binary
         *     representation of the specified @c glong value.
         *
         */
        static gint bitCount(glong i);

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified @c glong value left by the
         * specified number of bits.  (Bits shifted out of the left hand, or
         * high-order, side reenter on the right, or low-order.)
         *
         * @note Left rotation with a negative distance is equivalent to
         * right rotation: @c rotateLeft(val,-distance) == @c rotateRight(val,distance).
         * @note Rotation by any multiple of 64 is a no-op, so all but the last six bits of
         * the rotation distance can be ignored, even if the distance is negative:
         * @c Long::rotateLeft(val,distance) == @c Long::rotateLeft(val,distance&0x3F).
         *
         * @param i the value whose bits are to be rotated left
         * @param distance the number of bit positions to rotate left
         * @return the value obtained by rotating the two's complement binary
         *     representation of the specified @c glong value left by the
         *     specified number of bits.
         *
         */
        static glong rotateLeft(glong i, gint distance);

        /**
         * Returns the value obtained by rotating the two's complement binary
         * representation of the specified @c glong value right by the
         * specified number of bits.  (Bits shifted out of the right hand, or
         * low-order, side reenter on the left, or high-order.)
         *
         * @note Right rotation with a negative distance is equivalent to
         * left rotation: @c rotateRight(val,-distance)== @c rotateLeft(val,distance).
         *
         * @note Rotation by any multiple of 64 is a
         * no-op, so all but the last six bits of the rotation distance can be
         * ignored, even if the distance is negative:
         * @c rotateRight(val,distance) == @c rotateRight(val,distance&0x3F).
         *
         * @param i the value whose bits are to be rotated right
         * @param distance the number of bit positions to rotate right
         * @return the value obtained by rotating the two's complement binary
         *     representation of the specified @c glong value right by the
         *     specified number of bits.
         *
         */
        static glong rotateRight(glong i, gint distance);

        /**
         * Returns the value obtained by reversing the order of the bits in the
         * two's complement binary representation of the specified @c glong
         * value.
         *
         * @param i the value to be reversed
         * @return the value obtained by reversing order of the bits in the
         *     specified @c glong value.
         *
         */
        static glong reverse(glong i);

        /**
         * Returns the signum function of the specified @c glong value.  (The
         * return value is -1 if the specified value is negative; 0 if the
         * specified value is zero; and 1 if the specified value is positive.)
         *
         * @param i the value whose signum is to be computed
         * @return the signum function of the specified @c glong value.
         *
         */
        static gint signum(glong i);

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified @c glong value.
         *
         * @param i the value whose bytes are to be reversed
         * @return the value obtained by reversing the bytes in the specified
         *     @c glong value.
         *
         */
        static glong reverseBytes(glong i);

        /**
         * Adds two @c glong values together as per the + operator.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the sum of @c a and @c b
         *
         */
        static glong sum(glong a, glong b);

        /**
         * Returns the greater of two @c glong values
         * as if by calling @c Math::max(glong,glong) Math.max.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the greater of @c a and @c b
         * @see java.util.function.BinaryOperator
         *
         */
        static glong max(glong a, glong b);

        /**
         * Returns the smaller of two @c glong values
         * as if by calling @c Math::min(glong,glong) Math.min.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the smaller of @c a and @c b
         */
        static glong min(glong a, glong b);

        CORE_IMPLICIT operator glong() const;

        CORE_IMPLICIT operator glong &();

        Object &clone() const override;
    };
} // core

#endif // CORE24_LONG_H
