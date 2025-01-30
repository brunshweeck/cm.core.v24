//
// Created by bruns on 09/05/2024.
//

#ifndef CORE24_CHARSEQUENCE_H
#define CORE24_CHARSEQUENCE_H

#include "Class.h"

namespace core {
    /**
     * The class @c CharSequence represent the @a readable sequence
     * of chars values. this class provide the uniform read-only
     *  access to many different kinds of chars sequences.
     *
     * A @c char value represents a character in the @a Basic
     * @a Multilingual @a Plane (@b BMP) or @a surrogate.
     */
    class CharSequence : public virtual Object {
    public:
        /**
         * Returns the length of this character sequence.
         * The length is the number of 16-bit chars in the sequence.
         *
         * @return the number of chars in this sequence
         */
        virtual gint length() const = 0;

        /**
         * Returns @c true if this character sequence is empty.
         *
         * @return @c true if @c length() return @c 0, otherwise
         *          @c false
         */
        virtual gbool isEmpty() const;

        /**
         * Returns the char value at the specified index.
         * An index ranges from zero to @c length()-1.
         * The first @c gchar value of the sequence is at
         * index zero, the next at index one, and so on, as
         * for array indexing.
         *
         * If the gchar value specified by the index is a
         * @c surrogate, the surrogate value is returned.
         *
         * @param index The index of gchar value returned.
         * @throws IndexOutOfBoundsException if the @c index argument is negative or not
         *                                      less than @c length()
         * @return The specified gchar value at given index
         */
        virtual gchar charAt(gint index) const = 0;

        /**
         * Returns a @c CharSequence that is a subsequence of this sequence.
         * The subsequence starts with the @c char value at the specified index and
         * ends with the @c gchar value at index @c endIndex - 1.  The length
         * (in @c gchar) of the
         * returned sequence is @c endIndex - startIndex, so if @c startIndex == endIndex
         * then an empty sequence is returned.
         *
         * @param startIndex   the start index, inclusive
         * @param endIndex     the end index, exclusive
         *
         * @return  the specified subsequence
         *
         * @throws  IndexOutOfBoundsException if @c startIndex or @c endIndex are negative,
         *                if @c endIndex is greater than @c length(),
         *                or if @c startIndex is greater than @c endIndex
         */
        virtual CharSequence &subSequence(gint startIndex, gint endIndex) const = 0;
    };
} // core

#endif // CORE24_CHARSEQUENCE_H
