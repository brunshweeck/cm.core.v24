//
// Created by bruns on 13/05/2024.
//

#ifndef CORE24_BYTE_H
#define CORE24_BYTE_H

#include <core/Comparable.h>
#include <core/Number.h>

namespace core
{
    /**
     *
     * The @c Byte class wraps a value of primitive type @c gbyte
     * in an object.  An object of type @c Byte contains a single
     * field whose type is @c gbyte.
     */
    class Byte : public virtual Number, public virtual Comparable<Byte>
    {
    public:
        /**
         * A constant holding the maximum value a byte can have, @c 2<sup>7</sup>-1
         */
        static CORE_FAST gbyte MAX_VALUE = 127;

        /**
         * A constant holding the minimum value a byte can have, @c -22<sup>7</sup>
         */
        static CORE_FAST gbyte MIN_VALUE = -128;

        /**
         * Returns a new @c String object representing the
         * specified @c gbyte. The radix is assumed to be 10.
         *
         * @param value the @c gbyte to be converted
         * @return the string representation of the specified @c gbyte
         */
        static String toString(gbyte value);

        /**
         * Returns a @c Byte instance representing the specified
         * @c gbyte value.
         * If a new @c Byte instance is not required, this method
         * should generally be used in preference to the constructor
         * @c Byte(gbyte), as this method is likely to yield
         * significantly better space and time performance since
         * all byte values are cached.
         *
         * @param  b a byte value.
         * @return a @c Byte instance representing @c b.
         */
        static Byte valueOf(gbyte b);

        /**
         * Parses the string argument as a signed @c gbyte in the
         * radix specified by the second argument. The characters in the
         * string must all be digits, of the specified radix (as
         * determined by whether @c Character::digit(gchar,gint)
         * returns a non-negative value) except that the first
         * character may be an ASCII minus sign @c '-'('\\u002D')
         * to indicate a negative value or an ASCII plus sign @c '+'('\\u002B') to
         * indicate a positive value.  The resulting @c gbyte value is
         * returned.
         *
         * An exception of type @c NumberFormatException is
         * thrown if any of the following situations occurs:
         *
         * - The first argument is a string of length zero.
         * - The radix is either smaller than @c Character::MIN_RADIX or
         *   larger than @c Character::MAX_RADIX.
         *
         * - Any character of the string is not a digit of the
         * specified radix, except that the first character may be a minus
         * sign @c '-'('\\u002D') or plus sign @c '+'('\\u002B') provided that the
         * string is longer than length @c 1.
         *
         * - The value represented by the string is not a value of type @c gbyte.
         *
         *
         * @param s         the @c String containing the @c gbyte
         *                  representation to be parsed
         * @param radix     the radix to be used while parsing @c s
         * @return          the @c gbyte value represented by the string
         *                   argument in the specified radix
         * @throws          NumberFormatException If the string does
         *                  not contain a parsable @c gbyte.
         */
        static gbyte parseByte(String const& s, gint radix);

        /**
         * Parses the string argument as a signed decimal @c
         * byte. The characters in the string must all be decimal digits,
         * except that the first character may be an ASCII minus sign
         * @c '-'('\\u002D') to indicate a negative
         * value or an ASCII plus sign @c '+'('\\u002B') to indicate a positive value. The
         * resulting @c gbyte value is returned, exactly as if the
         * argument and the radix 10 were given as arguments to 
         * the @c parseByte(String,gint) method.
         *
         * @param s         a @c String containing the
         *                  @c gbyte representation to be parsed
         * @return          the @c gbyte value represented by the
         *                  argument in decimal
         * @throws          NumberFormatException if the string does not
         *                  contain a parsable @c gbyte.
         */
        static gbyte parseByte(String const& s);

        /**
         * Returns a @c Byte object holding the value
         * extracted from the specified @c String when parsed
         * with the radix given by the second argument. The first argument
         * is interpreted as representing a signed @c gbyte in
         * the radix specified by the second argument, exactly as if the
         * argument were given to the @c parseByte(String,gint) method. 
         * The result is a @c Byte object that represents the @c gbyte value 
         * specified by the string.
         *
         *  In other words, this method returns a @c Byte object
         * equal to the value of: @c Byte::valueOf(Byte::parseByte(s,radix))
         *
         * @param s         the string to be parsed
         * @param radix     the radix to be used in interpreting @c s
         * @return          a @c Byte object holding the value
         *                  represented by the string argument in the
         *                  specified radix.
         * @throws          NumberFormatException If the @c String does
         *                  not contain a parsable @c gbyte.
         */
        static Byte valueOf(String const& s, gint radix);

        /**
         * Returns a @c Byte object holding the value
         * given by the specified @c String. The argument is
         * interpreted as representing a signed decimal @c gbyte,
         * exactly as if the argument were given to the @c parseByte(String) method. 
         * The result is a @c Byte object that represents the @c gbyte
         * value specified by the string.
         *
         *  In other words, this method returns a @c Byte object
         * equal to the value of: @c Byte::valueOf(Byte::parseByte(s)).
         *
         * @param s         the string to be parsed
         * @return          a @c Byte object holding the value
         *                  represented by the string argument
         * @throws          NumberFormatException If the @c String does
         *                  not contain a parsable @c gbyte.
         */
        static Byte valueOf(String const& s);

        /**
         * Decodes a @c String into a @c Byte.
         * Accepts decimal, hexadecimal, and octal numbers given by
         * the following grammar:
         * 
         * <li> DecodableString:
         * <ul>
         * <li> @a Sign <sub>(opt)</sub> DecimalNumeral
         * <li> @a Sign <sub>(opt)</sub> @c 0x @a HexDigits
         * <li> @a Sign <sub>(opt)</sub> @c 0X @a HexDigits
         * <li> @a Sign <sub>(opt)</sub> @c # @a HexDigits
         * <li> @a Sign <sub>(opt)</sub> @c 0 @a OctalDigits
         * <li> @a Sign <sub>(opt)</sub> @c 0b @a BinaryDigits
         * <li> @a Sign <sub>(opt)</sub> @c 0B @a BinaryDigits
         * </ul>
         * <li> Sign:
         * <ul>
         * <li> @c -
         * <li> @c +
         * <ul>
         * The sequence of characters following an optional
         * sign and/or radix specifier ( @c "0x", @c "0X",
         * @c "#", @c "0b", @c "0B", or @a leading-zero) is parsed as by the
         * @c Byte::parseByte method with the indicated radix (10, 16, 8 or 2).
         * This sequence of characters must represent a positive value or
         * a @c NumberFormatException will be thrown.  The result is
         * negated if first character of the specified @c String is
         * the minus sign.  No whitespace characters are permitted in the
         * @c String.
         *
         * @param   nm the @c String to decode.
         * @return  A @c Byte object holding the @c gbyte value represented by @c nm
         * @throws  NumberFormatException  if the @c String does not contain a parsable @c gbyte.
         */
        static Byte decode(String const& nm);

    private:
        /**
         * The value of the @c Byte.
         */
        gbyte value = 0;

    public:
        /**
         * Construct new @c Byte with null value.
         */
        CORE_IMPLICIT Byte() CORE_NOTHROW;

        /**
         * Constructs a newly allocated @c Byte object that
         * represents the specified @c gbyte value.
         *
         * @param value     the value to be represented by the @c Byte.
         */
        CORE_IMPLICIT Byte(gbyte value);

        /**
         * Returns the value of this @c Byte as a @c gbyte.
         */
        gbyte byteValue() const override;

        /**
         * Returns the value of this @c Byte as a @c gshort after
         * a widening primitive conversion.
         */
        gshort shortValue() const override;

        /**
         * Returns the value of this @c Byte as an @c gint after
         * a widening primitive conversion.
         */
        gint intValue() const override;

        /**
         * Returns the value of this @c Byte as a @c glong after
         * a widening primitive conversion.
         */
        glong longValue() const override;

        /**
         * Returns the value of this @c Byte as a @c gfloat after
         * a widening primitive conversion.
         */
        gfloat floatValue() const override;

        /**
         * Returns the value of this @c Byte as a @c gdouble
         * after a widening primitive conversion.
         */
        gdouble doubleValue() const override;

        /**
         * Returns a @c String object representing this @c Byte 's value.  
         * The value is converted to signed decimal representation and returned 
         * as a string, exactly as if the @c gbyte value were given as an argument 
         * to the @c Byte::toString(gbyte) method.
         *
         * @return  a string representation of the value of this object in base @c 10.
         */
        String toString() const override;

        /**
         * Returns a hash code for this @c Byte; equal to the result
         * of invoking @c intValue().
         *
         * @return a hash code value for this @c Byte
         */
        gint hash() const override;

        /**
         * Returns a hash code for a @c gbyte value; compatible with
         * @c Byte::hash().
         *
         * @param value the value to hash
         * @return a hash code value for a @c gbyte value.
         */
        static gint hash(gbyte value);

        /**
         * Compares this object to the specified object.  The result is
         * @c true if and only if the argument is a @c Byte object that
         * contains the same @c gbyte value as this object.
         *
         * @param obj       the object to compare with
         * @return          @c true if the objects are the same;
         *                  @c false otherwise.
         */
        gbool equals(Object const& obj) const override;

        /**
         * Compares two @c Byte objects numerically.
         *
         * @param   anotherByte   the @c Byte to be compared.
         * @return  the value @c 0 if this @c Byte is
         *          equal to the argument @c Byte; a value less than
         *          @c 0 if this @c Byte is numerically less
         *          than the argument @c Byte; and a value greater than
         *           @c 0 if this @c Byte is numerically
         *           greater than the argument @c Byte (signed
         *           comparison).
         *
         */
        gint compareTo(Byte const& anotherByte) const override;

        /**
         * Compares two @c gbyte values numerically.
         * The value returned is identical to what would be returned by:
         * @c Byte::valueOf(x).compareTo(Byte::valueOf(y))
         *
         * @param  x the first @c gbyte to compare
         * @param  y the second @c gbyte to compare
         * @return the value @c 0 if @c x == y;
         *         a value less than @c 0 if @c x < y; and
         *         a value greater than @c 0 if @c x > y
         *
         */
        static gint compare(gbyte x, gbyte y);

        /**
         * Compares two @c gbyte values numerically treating the values
         * as unsigned.
         *
         * @param  x the first @c gbyte to compare
         * @param  y the second @c gbyte to compare
         * @return the value @c 0 if @c x==y; a value less
         *         than @c 0 if @c x<y as unsigned values; and
         *         a value greater than @c 0 if @c x>y as
         *         unsigned values
         *
         */
        static gint compareUnsigned(gbyte x, gbyte y);

        /**
         * Converts the argument to an @c gint by an unsigned
         * conversion.  In an unsigned conversion to an @c gint, the
         * high-order 24 bits of the @c gint are zero and the
         * low-order 8 bits are equal to the bits of the @c gbyte argument.
         *
         * Consequently, zero and positive @c gbyte values are mapped
         * to a numerically equal @c gint value and negative @c gbyte values 
         * are mapped to an @c gint value equal to the input plus @c 2**8.
         *
         * @param  x the value to convert to an unsigned @c gint
         * @return the argument converted to @c gint by an unsigned
         *         conversion
         *
         */
        static gint toUnsignedInt(gbyte x);

        /**
         * Converts the argument to a @c glong by an unsigned
         * conversion.  In an unsigned conversion to a @c glong, the
         * high-order 56 bits of the @c glong are zero and the
         * low-order 8 bits are equal to the bits of the @c gbyte argument.
         *
         * Consequently, zero and positive @c gbyte values are mapped
         * to a numerically equal @c glong value and negative @c byte values 
         * are mapped to a @c glong value equal to the input plus @c 2**8.
         *
         * @param  x the value to convert to an unsigned @c glong
         * @return the argument converted to @c glong by an unsigned
         *         conversion
         *
         */
        static glong toUnsignedLong(gbyte x);

        /**
         * The number of bits used to represent a @c gbyte value in two's
         * complement binary form.
         */
        static CORE_FAST gint SIZE = 1 << 3;

        /**
         * The number of bytes used to represent a @c gbyte value in two's
         * complement binary form.
         */
        static CORE_FAST gint BYTES = SIZE / Byte::SIZE;

        CORE_IMPLICIT operator gbyte() const;

        CORE_IMPLICIT operator gbyte&();

        Object& clone() const override;
    };
} // core

#endif // CORE24_BYTE_H
