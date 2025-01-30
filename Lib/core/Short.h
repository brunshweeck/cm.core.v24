//
// Created by bruns on 13/05/2024.
//

#ifndef CORE24_SHORT_H
#define CORE24_SHORT_H

#include "Byte.h"

namespace core {

    /**
     * The @c Short class wraps a value of primitive type
     * @c short in an object.  An object of type @c Short contains a
     * single field whose type is @c short.
     *
     * <p>
     * In addition, this class provides several methods for converting
     * a @c short to a @c String and a @c String to a
     * @c short, as well as other constants and methods useful when
     * dealing with a @c short.
     * </p>
     */
    class Short final : public virtual Number, public virtual Comparable<Short> {
    public:
        /**
         * A constant holding the maximum value a gshort can have, 2^15^ - 1
         */
        static CORE_FAST gshort MAX_VALUE = (1 << 15) - 1;

        /**
         * A constant holding the maximum value a gshort can have, 2^15^ - 1
         */
        static CORE_FAST gshort MIN_VALUE = -(1 << 15);

        /**
         * Returns a new @c String object representing the
         * specified @c gshort. The radix is assumed to be 10.
         *
         * @param s the @c gshort to be converted
         * @return the string representation of the specified @c gshort
         */
        static String toString(gshort value);

        /**
         * Parses the string argument as a signed @c gshort in the
         * radix specified by the second argument. The characters in the
         * string must all be digits, of the specified radix (as
         * determined by whether @c Character::digit(gchar,gint) returns
         * a non negative value) except that the first character may be an ASCII minus
         * sign @c '-'('\u002D') to indicate a negative value or an
         * ASCII plus sign @c '+' (@c '\u002B') to indicate a positive value.
         * The resulting @c gshort value is returned.
         *
         * An exception of type @c NumberFormatException is
         * thrown if any of the following situations occurs:
         *
         * @li The first argument is @c null or is a string of
         * length zero.
         *
         * @li The radix is either smaller than @c Character::MIN_RADIX
         * or larger than @c Character::MAX_RADIX.
         *
         * @li Any character of the string is not a digit of the
         * specified radix, except that the first character may be a minus
         * sign @c '-' (@c '\u002D') or plus sign
         * @c '+' (@c '\u002B') provided that the
         * string is longer than length 1.
         *
         * @li The value represented by the string is not a value of type
         * @c gshort.
         *
         * @param s         the @c String containing the
         *                  @c gshort representation to be parsed
         * @param radix     the radix to be used while parsing @c s
         * @return          the @c gshort represented by the string
         *                  argument in the specified radix.
         * @throws NumberFormatException If the @c String does not contains
         *                  a parsable @c gshort.
         */
        static gshort parseShort(String const& s, gint radix);

        /**
         * Parses the string argument as a signed decimal @c
         * gshort. The characters in the string must all be decimal
         * digits, except that the first character may be an ASCII minus
         * sign @c '-' (@c '\u002D') to indicate a
         * negative value or an ASCII plus sign @c '+'
         * (@c '\u002B') to indicate a positive value.  The
         * resulting @c gshort value is returned, exactly as if the
         * argument and the radix 10 were given as arguments to the
         * @c Short::parseShort(String, int) method.
         *
         * @param s a @c String containing the @c gshort
         *          representation to be parsed
         * @return  the @c gshort value represented by the
         *          argument in decimal.
         * @throws  NumberFormatException If the string does not
         *          contain a parsable @c gshort.
         */
        static gshort parseShort(String const& s);

        /**
         * Returns a @c Short object holding the value
         * extracted from the specified @c String when parsed
         * with the radix given by the second argument. The first argument
         * is interpreted as representing a signed @c gshort in
         * the radix specified by the second argument, exactly as if the
         * argument were given to the @c Short::parseShort(String,
         * int) method. The result is a @c Short object that
         * represents the @c gshort value specified by the string.
         *
         * In other words, this method returns a @c Short object
         * equal to the value of:
         *
         * <blockquote>
         *  @c Short::valueOf(Short.parseShort(s, radix))
         * </blockquote>
         *
         * @param s         the string to be parsed
         * @param radix     the radix to be used in interpreting @c s
         * @return          a @c Short object holding the value
         *                  represented by the string argument in the
         *                  specified radix.
         * @throws          NumberFormatException If the @c String does
         *                  not contain a parsable @c gshort.
         */
        static Short valueOf(String const& s, gint radix);

        /**
         * Returns a @c Short object holding the
         * value given by the specified @c String. The argument
         * is interpreted as representing a signed decimal
         * @c gshort, exactly as if the argument were given to
         * the @c Short::parseShort(String) method. The result is
         * a @c Short object that represents the
         * @c gshort value specified by the string.
         *
         * In other words, this method returns a @c Short object
         * equal to the value of:
         *
         * <blockquote>
         *  @c Short::valueOf(Short.parseShort(s))
         * </blockquote>
         *
         * @param s the string to be parsed
         * @return  a @c Short object holding the value
         *          represented by the string argument
         * @throws  NumberFormatException If the @c String does
         *          not contain a parsable @c gshort.
         */
        static Short valueOf(String const& s);

        /**
         * Returns a @c Short instance representing the specified
         * @c gshort value.
         * If a new @c Short instance is not required, this method
         * should generally be used in preference to the constructor
         * @c Short::Short(short), as this method is likely to yield
         * significantly better space and time performance by caching
         * frequently requested values.
         *
         * This method will always cache values in the range -128 to 127,
         * inclusive, and may cache other values outside of this range.
         *
         * @param  s a gshort value.
         * @return a @c Short instance representing @c s.
         *
         */
        static Short valueOf(gshort s);

        /**
         * Decodes a @c String into a @c Short.
         * Accepts decimal, hexadecimal, and octal numbers given by
         * the following grammar:
         *
         * @a Decodable String:
         *
         * @li @a Sign ( @a opt) DecimalNumeral
         * @li @a Sign ( @a opt) @c 0x @a HexDigits
         * @li @a Sign ( @a opt) @c 0X @a HexDigits
         * @li @a Sign ( @a opt) @c # @a HexDigits
         * @li @a Sign ( @a opt) @c 0 @a OctalDigits
         * @li @a Sign ( @a opt) @c 0b @a BinaryDigits
         * @li @a Sign ( @a opt) @c 0B @a BinaryDigits
         *
         * @a Sign:
         *
         * @li @c -
         * @li @c +
         *
         * The sequence of characters following an optional
         * sign and/or radix specifier (@c "0x", @c "0X", @c "0b", @c "0B"
         * @c "#", or leading zero) is parsed as by the @c Short::parseShort
         * method with the indicated radix (10, 16, or
         * 8).  This sequence of characters must represent a positive
         * value or a @c NumberFormatException will be thrown.  The
         * result is negated if first character of the specified @c
         * String is the minus sign.  No whitespace characters are
         * permitted in the @c String.
         *
         * @param     nm the @c String to decode.
         * @return    a @c Short object holding the @c gshort
         *            value represented by @c nm
         * @throws    NumberFormatException  if the @c String does not
         *            contain a parsable @c gshort.
         */
        static Short decode(String const& nm);

    private:
        /**
         * The value of the @c Short.
         */
        gshort value;

    public:
        /**
         * Constructs a newly allocated @c Short object that
         * represents the specified @c gshort value.
         *
         * @param value     the value to be represented by the
         *                  @c Short.
         *
         * @deprecated
         * It is rarely appropriate to use this constructor. The static factory
         * @c Short::valueOf(short) is generally a better choice, as it is
         * likely to yield significantly better space and time performance.
         */
        CORE_IMPLICIT Short(gshort value);

        /**
         * Returns the value of this @c Short as a @c byte after
         * a narrowing primitive conversion.
         */
        gbyte byteValue() const override;

        /**
         * Returns the value of this @c Short as a
         * @c gshort.
         */
        gshort shortValue() const override;

        /**
         * Returns the value of this @c Short as an @c int after
         * a widening primitive conversion.
         */
        gint intValue() const override;

        /**
         * Returns the value of this @c Short as a @c long after
         * a widening primitive conversion.
         */
        glong longValue() const override;

        /**
         * Returns the value of this @c Short as a @c float
         * after a widening primitive conversion.
         */
        gfloat floatValue() const override;

        /**
         * Returns the value of this @c Short as a @c double
         * after a widening primitive conversion.
         */
        gdouble doubleValue() const override;

        /**
         * Returns a @c String object representing this
         * @c Short's value.  The value is converted to signed
         * decimal representation and returned as a string, exactly as if
         * the @c gshort value were given as an argument to the
         * @c Short::toString(short) method.
         *
         * @return  a string representation of the value of this object in
         *          base 10.
         */
        String toString() const override;

        /**
         * Returns a hash code for this @c Short; equal to the result
         * of invoking @c intValue().
         *
         * @return a hash code value for this @c Short
         */
        gint hash() const override;

        /**
         * Returns a hash code for a @c gshort value; compatible with
         * @c Short::hash().
         *
         * @param value the value to hash
         * @return a hash code value for a @c gshort value.
         *
         */
        static gint hash(gshort value);

        /**
         * Compares this object to the specified object.  The result is
         * @c true if and only if the argument is not
         * @c null and is a @c Short object that
         * contains the same @c gshort value as this object.
         *
         * @param obj       the object to compare with
         * @return          @c true if the objects are the same;
         *                  @c false otherwise.
         */
        gbool equals(Object const& obj) const override;

        /**
         * Compares two @c Short objects numerically.
         *
         * @param   anotherShort   the @c Short to be compared.
         * @return  the value @c 0 if this @c Short is
         *          equal to the argument @c Short; a value less than
         *          @c 0 if this @c Short is numerically less
         *          than the argument @c Short; and a value greater than
         *           @c 0 if this @c Short is numerically
         *           greater than the argument @c Short (signed
         *           comparison).
         *
         */
        gint compareTo(Short const& anotherShort) const override;

        /**
         * Compares two @c gshort values numerically.
         * The value returned is identical to what would be returned by:
         * <pre>
         *    Short::valueOf(x).compareTo(Short::valueOf(y))
         * </pre>
         *
         * @param  x the first @c gshort to compare
         * @param  y the second @c gshort to compare
         * @return the value @c 0 if @c x==y;
         *         a value less than @c 0 if @c x<y; and
         *         a value greater than @c 0 if @c x>y
         *
         */
        static gint compare(gshort x, gshort y);

        /**
         * Compares two @c gshort values numerically treating the values
         * as unsigned.
         *
         * @param  x the first @c gshort to compare
         * @param  y the second @c gshort to compare
         * @return the value @c 0 if @c x==y; a value less
         *         than @c 0 if @c x<y as unsigned values; and
         *         a value greater than @c 0 if @c x>y as
         *         unsigned values
         *
         */
        static gint compareUnsigned(gshort x, gshort y);

        /**
         * The number of bits used to represent a @c gshort value in two's
         * complement binary form.
         *
         */
        static CORE_FAST gint SIZE = 1 << 4;

        /**
         * The number of bytes used to represent a @c gshort value in two's
         * complement binary form.
         *
         *
         */
        static CORE_FAST gint BYTES = SIZE / Byte::SIZE;

        /**
         * Returns the value obtained by reversing the order of the bytes in the
         * two's complement representation of the specified @c gshort value.
         *
         * @param i the value whose bytes are to be reversed
         * @return the value obtained by reversing (or, equivalently, swapping)
         *     the bytes in the specified @c gshort value.
         *
         */
        static gshort reverseBytes(gshort i);


        /**
         * Converts the argument to an @c int by an unsigned
         * conversion.  In an unsigned conversion to an @c int, the
         * high-order 16 bits of the @c int are zero and the
         * low-order 16 bits are equal to the bits of the @c gshort argument.
         *
         * Consequently, zero and positive @c gshort values are mapped
         * to a numerically equal @c int value and negative @c
         * gshort values are mapped to an @c int value equal to the
         * input plus 2<sup>16</sup>.
         *
         * @param  x the value to convert to an unsigned @c int
         * @return the argument converted to @c int by an unsigned
         *         conversion
         *
         */
        static gint toUnsignedInt(gshort x);

        /**
         * Converts the argument to a @c long by an unsigned
         * conversion.  In an unsigned conversion to a @c long, the
         * high-order 48 bits of the @c long are zero and the
         * low-order 16 bits are equal to the bits of the @c gshort argument.
         *
         * Consequently, zero and positive @c gshort values are mapped
         * to a numerically equal @c long value and negative @c
         * gshort values are mapped to a @c long value equal to the
         * input plus 2<sup>16</sup>.
         *
         * @param  x the value to convert to an unsigned @c long
         * @return the argument converted to @c long by an unsigned
         *         conversion
         *
         */
        static glong toUnsignedLong(gshort x);

        CORE_IMPLICIT operator gshort() const;

        CORE_IMPLICIT operator gshort&();

        Object& clone() const override;
    };
} // core

#endif // CORE24_SHORT_H
