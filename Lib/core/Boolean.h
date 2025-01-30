//
// Created by bruns on 13/05/2024.
//

#ifndef CORE24_BOOLEAN_H
#define CORE24_BOOLEAN_H

#include <core/Byte.h>

namespace core {
    /**
     * The Boolean class wraps a value of the primitive type
     * @c gbool in an object. An object of type @c Boolean contains
     * a single field whose type is @c gbool.
     */
    class Boolean final : public Comparable<Boolean> {
    public:
        /**
         * The @c Boolean object corresponding to the primitive
         * value @c true.
         */
        static const Boolean TRUE;

        /**
         * The @c Boolean object corresponding to the primitive
         * value @c false.
         */
        static const Boolean FALSE;

    private:
        /**
         * The value of the Boolean.
         */
        gbool value = false;

    public:
        CORE_IMPLICIT Boolean() CORE_NOTHROW;

        /**
         * Allocates a @c Boolean object representing the
         * @c value argument.
         *
         * @param value   the value of the @c Boolean.
         */
        CORE_IMPLICIT Boolean(gbool value);

        /**
         * Parses the string argument as a boolean. The @c boolean
         * returned represents the value @c true if the string argument
         * is equal, ignoring case, to the string @c "true". Otherwise,
         * a false value is returned.
         *
         * Example:
         * @code{.cpp}
         *  Boolean::parseBoolean("True") // returns true.
         *  Boolean::parseBoolean("yes") // returns false.
         * @endcode
         *
         * @param s the @c String containing the boolean representation
         *          to be parsed
         * @return the boolean represented by the string argument.
         */
        static gbool parseBoolean(String const &s);

        /**
         * Returns the value of this @c Boolean object as a boolean
         * primitive.
         *
         * @return  the primitive @c boolean value of this object.
         */
        gbool booleanValue() const;

        /**
         * Returns a @c Boolean instance representing the specified
         * @c boolean value.  If the specified @c boolean value
         * is @c true, this method returns @c Boolean::TRUE;
         * if it is @c false, this method returns @c Boolean::FALSE.
         *
         * @param  b a boolean value.
         * @return a @c Boolean instance representing @c b.
         *
         */
        static Boolean valueOf(gbool b);

        /**
         * Returns a @c Boolean with a value represented by the
         * specified string.  The @c Boolean returned represents a
         * true value if the string argument is equal, ignoring case,
         * to the string @c "true". Otherwise, a false value is returned.
         *
         * @param s a string.
         * @return  the @c Boolean value represented by the string.
         */
        static Boolean valueOf(String const &s);

        /**
         * Returns a @c String object representing the specified
         * boolean.  If the specified boolean is @c true, then
         * the string @c "true" will be returned, otherwise the
         * string @c "false" will be returned.
         *
         * @param b the boolean to be converted
         * @return the string representation of the specified @c boolean
         *
         */
        static String toString(gbool b);

        /**
         * Returns a @c String object representing this Boolean's
         * value.  If this object represents the value @c true,
         * a string equal to @c "true" is returned. Otherwise, a
         * string equal to @c "false" is returned.
         *
         * @return  a string representation of this object.
         */
        String toString() const override;

        /**
         * Returns a hash code for this @c Boolean object.
         *
         * @return  the integer @c 1231 if this object represents
         *          @c true; returns the integer @c 1237 if this
         *          object represents @c false.
         */
        gint hash() const override;

        /**
         * Returns a hash code for a @c boolean value; compatible with
         * @c Boolean::hash().
         *
         * @param value the value to hash
         * @return a hash code value for a @c boolean value.
         *
         */
        static gint hash(gbool value);

        /**
         * Returns @c true if and only if the argument is not
         * @c null and is a @c Boolean object that
         * represents the same @c boolean value as this object.
         *
         * @param   obj   the object to compare with.
         * @return  @c true if the Boolean objects represent the
         *          same value; @c false otherwise.
         */
        gbool equals(Object const &obj) const override;

        /**
         * Compares this @c Boolean instance with another.
         *
         * @param   other the @c Boolean instance to be compared
         * @return  zero if this object represents the same boolean value as the
         *          argument; a positive value if this object represents true
         *          and the argument represents false; and a negative value if
         *          this object represents false and the argument represents true
         *
         */
        gint compareTo(Boolean const &other) const override;

        /**
         * Compares two @c boolean values.
         * The value returned is identical to what would be returned by:
         * @code
         *    Boolean::valueOf(x).compareTo(Boolean::valueOf(y))
         * @endcode
         *
         * @param  x the first @c boolean to compare
         * @param  y the second @c boolean to compare
         * @return the value @c 0 if @c x == y;
         *         a value less than @c 0 if @c !x && y; and
         *         a value greater than @c 0 if @c x && !y
         *
         */
        static gint compare(gbool x, gbool y);

        /**
         * Returns the result of applying the logical AND operator to the
         * specified @c boolean operands.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the logical AND of @c a and @c b
         *
         */
        static gbool logicalAnd(gbool a, gbool b);

        /**
         * Returns the result of applying the logical OR operator to the
         * specified @c boolean operands.
         *
         * @param a the first operand
         * @param b the second operand
         * @return the logical OR of @c a and @c b
         *
         */
        static gbool logicalOr(gbool a, gbool b);

        /**
         * Returns the result of applying the logical XOR operator to the
         * specified @c boolean operands.
         *
         * @param a the first operand
         * @param b the second operand
         * @return  the logical XOR of @c a and @c b
         *
         */
        static gbool logicalXor(gbool a, gbool b);

        /**
         * The number of bits used to represent a @c gbyte value in two's
         * complement binary form.
         */
        CORE_FAST static gint SIZE = 8;

        /**
         * The number of bytes used to represent a @c gbyte value in two's
         * complement binary form.
         */
        CORE_FAST static gint BYTES = SIZE / Byte::SIZE;

        Object &clone() const override;
    };
} // core

#endif // CORE24_BOOLEAN_H
