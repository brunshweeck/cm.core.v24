//
// Created by brunshweeck on 29 mai 2024.
//

#ifndef CORE24_XSTRING_H
#define CORE24_XSTRING_H

#include <core/lang/Appendable.h>
#include <core/lang/CharSequence.h>
#include <core/lang/Integer.h>
#include <core/lang/Long.h>
#include <core/lang/String.h>

namespace core {
    /**
     * A mutable sequence of characters.  This class provides an API compatible
     * with @c SyncXString, but with no guarantee of synchronization.
     * This class is designed for use as a drop-in replacement for
     * @c SyncString in places where the string buffer was being
     * used by a single thread (as is generally the case).   Where possible,
     * it is recommended that this class be used in preference to
     * @c SyncXString as it will be faster under most implementations.
     *
     * <p>
     * The principal operations on a @c XString are the
     * @c append and @c insert methods, which are
     * overloaded to accept data of any type. Each effectively
     * converts a given datum to a string and then appends or inserts the
     * characters of that string to the string builder. The
     * @c append method always adds these characters at the end
     * of the builder; the @c insert method adds the characters at
     * a specified point.
     * </p>
     * <p>
     * For example, if @c z refers to a XString object
     * whose current contents are @c "start"_S, then
     * the method call @c z.append("le"_S) would cause the XString
     * to contain @c "startle"_S, whereas
     * @c z.insert(4, "le") would alter the string builder to
     * contain @c "starlet"_S.
     * </p>
     * <p>
     * In general, if sb refers to an instance of a @c XString,
     * then @c sb.append(x) has the same effect as
     * @code
     *  sb.insert(sb.length(), x).
     * @endcode
     * </p>
     * <p>
     * Every string builder has a capacity. As long as the length of the
     * character sequence contained in the string builder does not exceed
     * the capacity, it is not necessary to allocate a new internal
     * buffer. If the internal buffer overflows, it is automatically made larger.
     * </p>
     *
     * <p>
     * Instances of @c XString are not safe for
     * use by multiple threads. If such synchronization is required then it is
     * recommended that @c SyncString be used.
     * </p>
     */
    class XString final : public virtual CharSequence,
                          public virtual Comparable<XString>,
                          public virtual Appendable {
        CORE_ALIAS(ARRAY, Class<gbyte>::Pointer);
        CORE_ALIAS(BYTES, Class<gbyte>::Pointer);
        CORE_ALIAS(CHARS, Class<gchar>::Pointer);
        CORE_ALIAS(INTS, Class<gint>::Pointer);
        CORE_ALIAS(UNSAFE, misc::Unsafe);

        CORE_ALIAS(Coder, String::Coder);
        CORE_ADD_AS_FRIEND(String);

        /**
         * The array used for characters storage.
         */
        ARRAY value = { };

        /**
         * The number of 16 bits characters on this String.
         */
        gint count = 0;

        /**
         * The capacity
         */
        gint limit = 0;

        /**
         * The identifier of the encoding used to encode
         * the bytes in the @c value field.
         */
        Coder coder = String::LATIN1;

        /**
         *  The attribute indicates @c value might be compressible to LATIN1 if it is UTF16-encoded.
         *  An inflated byte array becomes compressible only when those non-latin1 chars are deleted.
         *  We simply set this attribute in all methods which may delete chars. Therefore, there are
         *  false positives. Subclasses and String need to handle it properly.
         */
        gbool maybeLatin1 = false;

    public:
        /**
         * Constructs a string builder with no characters in it and an
         * initial capacity of 16 characters.
         */
        CORE_IMPLICIT XString();

        /**
         * Constructs a string builder with no characters in it and an
         * initial capacity specified by the @c capacity argument.
         *
         * @param   capacity  the initial capacity.
         * @throws  IllegalArgumentException  if the @c capacity
         *               argument is less than @c 0.
         */
        CORE_EXPLICIT XString(gint capacity);

        /**
         * Constructs a string builder initialized to the contents of the
         * specified string. The initial capacity of the string builder is
         * @c 16 plus the length of the string argument.
         *
         * @param   str   the initial contents of the buffer.
         */
        CORE_EXPLICIT XString(String const& str);

        /**
         * Constructs a string builder that contains the same characters
         * as the specified @c CharSequence. The initial capacity of
         * the string builder is @c 16 plus the length of the
         * @c CharSequence argument.
         *
         * @param      seq   the sequence to copy.
         */
        CORE_EXPLICIT XString(CharSequence const& seq);

        /**
         * Compares two @c StringBuilder instances lexicographically. This method
         * follows the same rules for lexicographical comparison as defined in the
         * @em  CharSequence::compare(this,another) method.
         *
         * <p>
         * For finer-grained, locale-sensitive String comparison, refer to
         * @c Collator.
         * </p>
         * @param another the @c StringBuilder to be compared with
         *
         * @return  the value @c 0 if this @c StringBuilder contains the same
         * character sequence as that of the argument @c StringBuilder; a negative integer
         * if this @c StringBuilder is lexicographically less than the
         * @c StringBuilder argument; or a positive integer if this @c StringBuilder
         * is lexicographically greater than the @c StringBuilder argument.
         *
         */
        gint compareTo(XString const& another) const override;

        /**
         * Returns the length (character count).
         *
         * @return  the length of the sequence of characters currently
         *          represented by this object
         */
        gint length() const override;

        /**
         * Returns the current capacity. The capacity is the number of characters
         * that can be stored (including already written characters), beyond which
         * an allocation will occur.
         *
         * @return  the current capacity
         */
        gint capacity() const;

        /**
         * Ensures that the capacity is at least equal to the specified minimum.
         * If the current capacity is less than the argument, then a new internal
         * array is allocated with greater capacity. The new capacity is the
         * larger of:
         *
         * @li The @c minimumCapacity argument.
         * @li Twice the old capacity, plus @c 2.
         *
         * If the @c minimumCapacity argument is non-positive, this
         * method takes no action and simply returns.
         * Note that subsequent operations on this object can reduce the
         * actual capacity below that requested here.
         *
         * @param   minCapacity   the minimum desired capacity.
         */
        void ensureCapacity(gint minCapacity);

    private:
        /**
         * Returns a capacity at least as large as the given minimum capacity.
         * Returns the current capacity increased by the current length+2 if
         * that suffices.
         * Will not return a capacity greater than
         * @c (SOFT_MAX_ARRAY_LENGTH >> coder)
         * unless the given minimum capacity is greater than that.
         *
         * @param  minCapacity the desired minimum capacity
         * @throws OutOfMemoryError if minCapacity is less than zero or
         *         greater than (Integer.MAX_VALUE >> coder)
         */
        gint newCapacity(gint minCapacity) const;

    public:
        /**
         * Attempts to reduce storage used for the character sequence.
         * If the buffer is larger than necessary to hold its current sequence of
         * characters, then it may be resized to become more space efficient.
         * Calling this method may, but is not required to, affect the value
         * returned by a subsequent call to the @c capacity() method.
         */
        void trimToSize();

        /**
         * Sets the length of the character sequence.
         * The sequence is changed to a new character sequence
         * whose length is specified by the argument. For every non-negative
         * index @a k less than @c newLength, the character at
         * index @a k in the new character sequence is the same as the
         * character at index @a k in the old sequence if @a k is less
         * than the length of the old character sequence; otherwise, it is the
         * null character @c '\\u0000'.
         *
         * In other words, if the @c newLength argument is less than
         * the current length, the length is changed to the specified length.
         * <p>
         * If the @c newLength argument is greater than or equal
         * to the current length, sufficient null characters
         * (@c '\\u0000') are appended so that
         * length becomes the @c newLength argument.
         * <p>
         * The @c newLength argument must be greater than or equal
         * to @c 0.
         *
         * @param      newLength   the new length
         * @throws     IndexOutOfBoundsException  if the
         *               @c newLength argument is negative.
         */
        void setLength(gint newLength);

        /**
         * Returns the @c gchar value in this sequence at the specified index.
         * The first @c gchar value is at index @c 0, the next at index
         * @c 1, and so on, as in array indexing.
         * <p>
         * The index argument must be greater than or equal to
         * @c 0, and less than the length of this sequence.
         *
         * <p>If the @c gchar value specified by the index is a
         * <i> surrogate</i>, the surrogate value is returned.
         *
         * @param      index   the index of the desired @c gchar value.
         * @return     the @c gchar value at the specified index.
         * @throws     IndexOutOfBoundsException  if @c index is
         *             negative or greater than or equal to @c length().
         */
        gchar charAt(gint index) const override;

        /**
         * Returns the character (Unicode code point) at the specified
         * index. The index refers to @c gchar values
         * (Unicode code units) and ranges from @c 0 to
         * @c length()@c -1.
         *
         * <p>
         * If the @c gchar value specified at the given index
         * is in the high-surrogate range, the following index is less
         * than the length of this sequence, and the
         * @c gchar value at the following index is in the
         * low-surrogate range, then the supplementary code point
         * corresponding to this surrogate pair is returned. Otherwise,
         * the @c gchar value at the given index is returned.
         * </p>
         * @param      index the index to the @c gchar values
         * @return     the code point value of the character at the
         *             @c index
         * @throws     IndexOutOfBoundsException  if the @c index
         *             argument is negative or not less than the length of this
         *             sequence.
         */
        gint codePointAt(gint index) const;

        /**
         * Characters are copied from this sequence into the
         * destination character array @c dst. The first character to
         * be copied is at index @c srcBegin; the last character to
         * be copied is at index @c srcEnd-1. The total number of
         * characters to be copied is @c srcEnd-srcBegin. The
         * characters are copied into the subarray of @c dst starting
         * at index @c dstBegin and ending at index:
         * @code
         *  dstbegin + (srcEnd-srcBegin)-1
         * @endcode
         *
         * @param      srcBegin   start copying at this offset.
         * @param      srcEnd     stop copying at this offset.
         * @param      dst        the array to copy the data into.
         * @param      dstBegin   offset into @c dst.
         * @throws     IndexOutOfBoundsException  if any of the following is true:
         *             - @c srcBegin is negative
         *             - @c dstBegin is negative
         *             - the @c srcBegin argument is greater than the @c srcEnd argument.
         *             - @c srcEnd is greater than @c this.length().
         *             - @c dstBegin+srcEnd-srcBegin is greater than @c dst.length()
         */
        void toChars(gint srcBegin, gint srcEnd, CharArray& dst, gint dstBegin) const;

        /**
         * The character at the specified index is set to @c ch. This
         * sequence is altered to represent a new character sequence that is
         * identical to the old character sequence, except that it contains the
         * character @c ch at position @c index.
         * <p>
         * The index argument must be greater than or equal to
         * @c 0, and less than the length of this sequence.
         * </p>
         * @param      index   the index of the character to modify.
         * @param      ch      the new character.
         * @throws     IndexOutOfBoundsException  if @c index is
         *             negative or greater than or equal to @c length().
         */
        void setCharAt(gint index, gchar ch);

        /**
         * Appends the string representation of the @c Object argument.
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(Object),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   obj   an @c Object.
         * @return  a reference to this object.
         */
        XString& append(Object const& obj);

        /**
         * Appends the string representation of the @c Enum<?> argument.
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(Object),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @tparam E The primitive enum type
         * @param   obj   an @c Object.
         * @return  a reference to this object.
         * @warning This calling require inclusion of header file <core/lang/Enum.h>
         */
        template <class E, ClassOf(1)::OnlyIf<Class<E>::isEnum()> = 1>
        XString& append(E anEnum) {
          CORE_ALIAS(Enum, typename Class<E>::Object);
          return append((Enum) anEnum);
        }

        /**
         * Appends the specified string to this character sequence.
         * <p>
         * The characters of the @c String argument are appended, in
         * order, increasing the length of this sequence by the length of the
         * argument. If @c str is @c null, then the four
         * characters @c "null" are appended.
         * </p>
         * <p>
         * Let @a n be the length of this character sequence just prior to
         * execution of the @c append method. Then the character at
         * index @a k in the new character sequence is equal to the character
         * at index @a k in the old character sequence, if @a k is less
         * than @a n; otherwise, it is equal to the character at index
         * @a k-n in the argument @c str.
         * </p>
         * @param   str   a string.
         * @return  a reference to this object.
         */
        XString& append(String const& str);

        /**
         * Appends the specified string to this character sequence.
         * <p>
         * The characters of the @c String argument are appended, in
         * order, increasing the length of this sequence by the length of the
         * argument. If @c str is @c null, then the four
         * characters @c "null" are appended.
         * </p>
         * <p>
         * Let @a n be the length of this character sequence just prior to
         * execution of the @c append method. Then the character at
         * index @a k in the new character sequence is equal to the character
         * at index @a k in the old character sequence, if @a k is less
         * than @a n; otherwise, it is equal to the character at index
         * @a k-n in the argument @c str.
         * </p>
         * @param   str   a string.
         * @return  a reference to this object.
         */
        template <class Str, ClassOf(1)::OnlyIf<Class<Str>::isString()>  = 1>
        XString& append(Str const& str) { return append(String(str)); }

        /**
         * Appends the specified @c XString to this sequence.
         * <p>
         * The characters of the @c XString argument are appended,
         * in order, to the contents of this @c XString, increasing the
         * length of this @c XString by the length of the argument.
         * If @c sb is @c null, then the four characters
         * @c "null" are appended to this @c XString.
         * </p>
         * <p>
         * Let @a n be the length of the old character sequence, the one
         * contained in the @c XString just prior to execution of the
         * @c append method. Then the character at index @a k in
         * the new character sequence is equal to the character at index @a k
         * in the old character sequence, if @a k is less than @a n;
         * otherwise, it is equal to the character at index @a k-n in the
         * argument @c sb.
         * </p>
         * <p>
         * This method synchronizes on @c this, the destination
         * object, but does not synchronize on the source (@c sb).
         * </p>
         * @param   sb   the @c XString to append.
         * @return  a reference to this object.
         *
         */
        XString& append(XString const& sb);

        /**
         * Appends the specified @c CharSequence to this
         * sequence.
         * <p>
         * The characters of the @c CharSequence argument are appended,
         * in order, increasing the length of this sequence by the length of the
         * argument.
         * </p>
         * <p>The result of this method is exactly the same as if it were an
         * invocation of this.append(s, 0, s.length());
         * </p>
         * <p>
         * This method synchronizes on @c this, the destination
         * object, but does not synchronize on the source (@c s).
         * </p>
         * @param   s the @c CharSequence to append.
         * @return  a reference to this object.
         *
         */
        XString& append(CharSequence const& s) override;

        /**
         * Appends a subsequence of the specified @c CharSequence to this
         * sequence.
         * <p>
         * Characters of the argument @c s, starting at
         * index @c start, are appended, in order, to the contents of
         * this sequence up to the (exclusive) index @c end. The length
         * of this sequence is increased by the value of @c end-start.
         * </p>
         * <p>
         * Let @a n be the length of this character sequence just prior to
         * execution of the @c append method. Then the character at
         * index @a k in this character sequence becomes equal to the
         * character at index @a k in this sequence, if @a k is less than
         * @a n; otherwise, it is equal to the character at index
         * @a k+start-n in the argument @c s.
         * </p>
         *
         * @param   s the sequence to append.
         * @param   start   the starting index of the subsequence to be appended.
         * @param   end     the end index of the subsequence to be appended.
         * @return  a reference to this object.
         * @throws     IndexOutOfBoundsException if
         *             @c start is negative, or
         *             @c start is greater than @c end or
         *             @c end is greater than @c s.length()
         */
        XString& append(CharSequence const& s, gint start, gint end) override;


        /**
         * Appends the string representation of the @c char array
         * argument to this sequence.
         * <p>
         * The characters of the array argument are appended, in order, to
         * the contents of this sequence. The length of this sequence
         * increases by the length of the argument.
         * </p>
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(CharArray),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   str   the characters to be appended.
         * @return  a reference to this object.
         */
        XString& append(CharArray const& str);

        /**
         * Appends the string representation of a subarray of the
         * @c char array argument to this sequence.
         * <p>
         * Characters of the @c char array @c str, starting at
         * index @c offset, are appended, in order, to the contents
         * of this sequence. The length of this sequence increases
         * by the value of @c len.
         * </p>
         * <p>
         * The overall effect is exactly as if the arguments were converted
         * to a string by the method @c String::valueOf(CharArray,gint,gint),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   str      the characters to be appended.
         * @param   offset   the index of the first @c gchar to append.
         * @param   len      the number of @c chars to append.
         * @return  a reference to this object.
         * @throws IndexOutOfBoundsException
         *         if @c offset < 0 or @c len < 0
         *         or @c offset+len > str.length
         */
        XString& append(CharArray const& str, gint offset, gint len);

        /**
         * Appends the string representation of the @c boolean
         * argument to the sequence.
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(boolean),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   b   a @c boolean.
         * @return  a reference to this object.
         */
        XString& append(gbool b);

        /**
         * Appends the string representation of the @c gchar
         * argument to this sequence.
         * <p>
         * The argument is appended to the contents of this sequence.
         * The length of this sequence increases by @c 1.
         * </p>
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(gchar),
         * and the character in that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   c   a @c gchar.
         * @return  a reference to this object.
         */
        XString& append(gchar c) override;

        template <class CharT, Class<gbool>::OnlyIf<Class<CharT>::isCharacter()>  = true>
        XString& append(CharT c) {
            if (Class<CharT>::size() == 1)
                // Latin1
                return append(CORE_CAST(gchar, c & 0xFF));
            if (Class<CharT>::size() == 2)
                // UTF16
                return append(CORE_CAST(gchar, c));
            // UTF32
            return appendCodePoint(CORE_CAST(gint, c));
        }

        /**
         * Appends the string representation of the @c gint
         * argument to this sequence.
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(gint),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   i   an @c gint.
         * @return  a reference to this object.
         */
        XString& append(gint i);

        template <class Num, Class<gbool>::OnlyIf<Class<Num>::isNumber()>  = true>
        XString& append(Num num) {
            if (Class<Num>::isInteger()) {
                if (Class<Num>::size() == 4) {
                    if (num <= Integer::MAX_VALUE)
                        // Signed integer
                        return append(CORE_CAST(gint, num));
                    // Unsigned integer
                    return append(Integer::toUnsignedLong(num));
                }
                if (num <= Long::MAX_VALUE)
                    // Signed Long
                    return append(CORE_CAST(glong, num));
                // Unsigned long
                return append(Long::toUnsignedString(num));
            }
            if (Class<Num>::size() == 4)
                // Float 16~32
                return append(CORE_CAST(gfloat, num));
            // Float 64~128
            return append(CORE_CAST(gdouble, num));
        }

        /**
         * Appends the string representation of the @c glong
         * argument to this sequence.
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(glong),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   l   a @c glong.
         * @return  a reference to this object.
         */
        XString& append(glong l);

        /**
         * Appends the string representation of the @c float
         * argument to this sequence.
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(float),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   f   a @c float.
         * @return  a reference to this object.
         */
        XString& append(gfloat f);

        /**
         * Appends the string representation of the @c gdouble
         * argument to this sequence.
         * <p>
         * The overall effect is exactly as if the argument were converted
         * to a string by the method @c String::valueOf(gdouble),
         * and the characters of that string were then
         * @c appended to this character sequence.
         * </p>
         * @param   d   a @c gdouble.
         * @return  a reference to this object.
         */
        XString& append(gdouble d);

        /**
         * Removes the characters in a substring of this sequence.
         * The substring begins at the specified @c start and extends to
         * the character at index @c end-1 or to the end of the
         * sequence if no such character exists. If
         * @c start is equal to @c end, no changes are made.
         *
         * @param      start  The beginning index, inclusive.
         * @param      end    The ending index, exclusive.
         * @return     This object.
         * @throws     IndexOutOfBoundsException  if @c start
         *             is negative, greater than @c length(), or
         *             greater than @c end.
         */
        XString& remove(gint start, gint end);

        /**
         * Appends the string representation of the @c codePoint
         * argument to this sequence.
         *
         * <p>
         * The argument is appended to the contents of this sequence.
         * The length of this sequence increases by
         * @c Character#charCount(gint) Character.charCount(codePoint).
         * </p>
         * <p>
         * The overall effect is exactly as if the argument were
         * converted to a @c char array by the method
         * @c Character#toChars(gint) and the character in that array
         * were then @c appended to this character
         * sequence.
         * </p>
         * @param   codePoint   a Unicode code point
         * @return  a reference to this object.
         * @throws    IllegalArgumentException if the specified
         * @c codePoint isn't a valid Unicode code point
         */
        XString& appendCodePoint(gint codePoint);

        /**
         * Removes the @c gchar at the specified position in this
         * sequence. This sequence is shortened by one @c gchar.
         *
         * <p>
         * Note: If the character at the given index is a supplementary
         * character, this method does not remove the entire character. If
         * correct handling of supplementary characters is required,
         * determine the number of @c chars to remove by calling
         * @c Character.charCount(thisSequence.codePointAt(index)),
         * where @c thisSequence is this sequence.
         * </p>
         * @param       index  Index of @c gchar to remove
         * @return      This object.
         * @throws      IndexOutOfBoundsException  if the @c index
         *              is negative or greater than or equal to
         *              @c length().
         */
        XString& deleteCharAt(gint index);

        /**
         * Replaces the characters in a substring of this sequence
         * with characters in the specified @c String. The substring
         * begins at the specified @c start and extends to the character
         * at index @c end-1 or to the end of the
         * sequence if no such character exists. First the
         * characters in the substring are removed and then the specified
         * @c String is inserted at @c start. (This
         * sequence will be lengthened to accommodate the
         * specified String if necessary.)
         *
         * @param      start    The beginning index, inclusive.
         * @param      end      The ending index, exclusive.
         * @param      str   String that will replace previous contents.
         * @return     This object.
         * @throws     IndexOutOfBoundsException  if @c start
         *             is negative, greater than @c length(), or
         *             greater than @c end.
         */
        XString& replace(gint start, gint end, String const& str);

        /**
         * Returns a new @c String that contains a subsequence of
         * characters currently contained in this character sequence. The
         * substring begins at the specified index and extends to the end of
         * this sequence.
         *
         * @param      start    The beginning index, inclusive.
         * @return     The new string.
         * @throws     IndexOutOfBoundsException  if @c start is
         *             less than zero, or greater than the length of this object.
         */
        String subString(gint start) const;

        /**
         * Returns a new character sequence that is a subsequence of this sequence.
         *
         * <p>
         * An invocation of this method of the form
         * @code
         *  sb.subSequence(begin, end)
         * @endcode
         *
         * behaves in exactly the same way as the invocation
         * @code
         *  sb.subString(begin, end)
         * @endcode
         *
         * This method is provided so that this class can
         * implement the @c CharSequence interface.
         * </p>
         * @param      start   the start index, inclusive.
         * @param      end     the end index, exclusive.
         * @return     the specified subsequence.
         *
         * @throws  IndexOutOfBoundsException
         *          if @c start or @c end are negative,
         *          if @c end is greater than @c length(),
         *          or if @c start is greater than @c end
         */
        CharSequence& subSequence(gint start, gint end) const override;

        /**
         * Returns a new @c String that contains a subsequence of
         * characters currently contained in this sequence. The
         * substring begins at the specified @c start and
         * extends to the character at index @c end-1.
         *
         * @param      start    The beginning index, inclusive.
         * @param      end      The ending index, exclusive.
         * @return     The new string.
         * @throws     IndexOutOfBoundsException  if @c start
         *             or @c end are negative or greater than
         *             @c length(), or @c start is
         *             greater than @c end.
         */
        String subString(gint start, gint end) const;

        /**
         * Inserts the string representation of a subarray of the @c str
         * array argument into this sequence. The subarray begins at the
         * specified @c offset and extends @c len @c chars.
         * The characters of the subarray are inserted into this sequence at
         * the position indicated by @c index. The length of this
         * sequence increases by @c len @c chars.
         *
         * @param      index    position at which to insert subarray.
         * @param      str       A @c char array.
         * @param      offset   the index of the first @c gchar in subarray to
         *             be inserted.
         * @param      len      the number of @c chars in the subarray to
         *             be inserted.
         * @return     This object
         * @throws     IndexOutOfBoundsException  if @c index
         *             is negative or greater than @c length(), or
         *             @c offset or @c len are negative, or
         *             @c (offset+len) is greater than
         *             @c str.length.
         */
        XString& insert(gint index, CharArray const& str, gint offset, gint len);

        /**
         * Inserts the string representation of the @c Object
         * argument into this character sequence.
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(Object),
         * and the characters of that string were then
         * @c inserted into this character sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      obj      an @c Object.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, Object const& obj);

        /**
         * Inserts the string representation of the @c Object
         * argument into this character sequence.
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(Object),
         * and the characters of that string were then
         * @c inserted into this character sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      obj      an @c Object.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         * @warning This calling require inclusion of header file <core/lang/Enum.h>
         */
        template <class E, ClassOf(1)::OnlyIf<Class<E>::isEnum()> = 1>
        XString& insert(gint index, E anEnum) {
          CORE_ALIAS(Enum, typename Class<E>::Object);
          return insert(index, (Enum) anEnum);
        }

        /**
         * Inserts the string into this character sequence.
         * <p>
         * The characters of the @c String argument are inserted, in
         * order, into this sequence at the indicated offset, moving up any
         * characters originally above that position and increasing the length
         * of this sequence by the length of the argument. If
         * @c str is @c null, then the four characters
         * @c "null" are inserted into this sequence.
         * </p>
         * <p>
         * The character at index @a k in the new character sequence is
         * equal to:
         *
         * @li the character at index @a k in the old character sequence, if
         * @a k is less than @c offset
         * @li the character at index @a k@c -offset in the
         * argument @c str, if @a k is not less than
         * @c offset but is less than @c offset+str.length()
         * @li the character at index @a k@c -str.length() in the
         * old character sequence, if @a k is not less than
         * @c offset+str.length()
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      str      a string.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, String const& str);

        template <class Str, ClassOf(1)::OnlyIf<Class<Str>::isString()>  = 1>
        XString& insert(gint offset, Str const& str) { return insert(offset, String(str)); }

        /**
         * Inserts the string representation of the @c char array
         * argument into this sequence.
         * <p>
         * The characters of the array argument are inserted into the
         * contents of this sequence at the position indicated by
         * @c offset. The length of this sequence increases by
         * the length of the argument.
         * </p>
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(CharArray),
         * and the characters of that string were then
         * @c inserted into this character sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      str      a character array.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, CharArray const& str);

        /**
         * Inserts the specified @c CharSequence into this sequence.
         * <p>
         * The characters of the @c CharSequence argument are inserted,
         * in order, into this sequence at the indicated offset, moving up
         * any characters originally above that position and increasing the length
         * of this sequence by the length of the argument s.
         * <p>
         * The result of this method is exactly the same as if it were an
         * invocation of this object's
         * @code insert(dstOffset, s, 0, s.length()) @endcode  method.
         * </p>
         *
         * @param      dstOffset   the offset.
         * @param      s the sequence to be inserted
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint dstOffset, CharSequence const& s);

        /**
         * Inserts a subsequence of the specified @c CharSequence into
         * this sequence.
         * <p>
         * The subsequence of the argument @c s specified by
         * @c start and @c end are inserted,
         * in order, into this sequence at the specified destination offset, moving
         * up any characters originally above that position. The length of this
         * sequence is increased by @code end - start @endcode .
         * </p>
         * <p>
         * The character at index @a k in this sequence becomes equal to:
         *
         * @li the character at index @a k in this sequence, if
         * @a k is less than @c dstOffset
         * @li the character at index @code k + start - dstOffset @endcode in
         * the argument @c s, if @a k is greater than or equal to
         * @c dstOffset but is less than @code dstOffset + end - start @endcode
         * @li the character at index @code k - (end - start) @endcode in this
         * sequence, if @a k is greater than or equal to
         * @c dstOffset+end-start
         * </p>
         * <p>
         * The @c dstOffset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * <p>
         * The start argument must be non-negative, and not greater than
         * @c end.
         * </p>
         * <p>
         * The end argument must be greater than or equal to
         * @c start, and less than or equal to the length of s.
         * </p>
         *
         * @param      dstOffset   the offset in this sequence.
         * @param      s       the sequence to be inserted.
         * @param      start   the starting index of the subsequence to be inserted.
         * @param      end     the end index of the subsequence to be inserted.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if @c dstOffset
         *             is negative or greater than @c this.length(), or
         *              @c start or @c end are negative, or
         *              @c start is greater than @c end or
         *              @c end is greater than @c s.length()
         */
        XString& insert(gint dstOffset, CharSequence const& s, gint start, gint end);

        /**
         * Inserts the string representation of the @c boolean
         * argument into this sequence.
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(boolean),
         * and the characters of that string were then
         * @c inserted into this character sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      b        a @c boolean.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, gbool b);

        /**
         * Inserts the string representation of the @c gchar
         * argument into this sequence.
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(gchar),
         * and the character in that string were then
         * @c inserted into this character sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      c        a @c gchar.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, gchar c);

        template <class CharT, Class<gbool>::OnlyIf<Class<CharT>::isCharacter()>  = true>
        XString& insert(gint offset, CharT c) {
            if (Class<CharT>::size() == 1)
                // Latin1
                return insert(offset, CORE_CAST(gchar, c & 0xFF));
            // UTF16
            if (Class<CharT>::size() == 2)
                return insert(offset, CORE_CAST(gchar, c));
            if (c <= 0xFFFF)
                return insert(offset, CORE_CAST(gchar, c));
            // UTF32
            if (Character::isSupplementary(c)) {
                gchar const hi = Character::highSurrogate(c);
                gchar const lo = Character::lowSurrogate(c);
                ensureCapacity(length() + 2);
                insert(offset + 0, hi);
                return insert(offset + 1, lo);
            }
            // Invalid Codepoint
            return insert(offset, '?');
        }

        /**
         * Inserts the string representation of the second @c gint
         * argument into this sequence.
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(gint),
         * and the characters of that string were then
         * @c inserted into this character sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      i        an @c gint.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, gint i);

        template <class Num, Class<gbool>::OnlyIf<Class<Num>::isNumber()>  = true>
        XString& insert(gint offset, Num num) {
            if (Class<Num>::isInteger()) {
                if (Class<Num>::size() <= 4) {
                    if (num <= Integer::MAX_VALUE)
                        return insert(offset, CORE_CAST(gint, num));
                    return insert(offset, Integer::toUnsignedLong(num));
                }
                if (num <= Long::MAX_VALUE)
                    return insert(offset, CORE_CAST(glong, num));
                // unsigned long
                return insert(offset, Long::toUnsignedString(num));
            }
            if (Class<Num>::size() == 4)
                return insert(offset, CORE_CAST(gfloat, num));
            return insert(offset, CORE_CAST(gdouble, num));
        }

        /**
         * Inserts the string representation of the @c glong
         * argument into this sequence.
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(glong),
         * and the characters of that string were then
         * @c inserted into this character
         * sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      l        a @c glong.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, glong l);

        /**
         * Inserts the string representation of the @c float
         * argument into this sequence.
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(float),
         * and the characters of that string were then
         * @c inserted into this character sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      f        a @c float.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, gfloat f);

        /**
         * Inserts the string representation of the @c gdouble
         * argument into this sequence.
         * <p>
         * The overall effect is exactly as if the second argument were
         * converted to a string by the method @c String::valueOf(gdouble),
         * and the characters of that string were then
         * @c inserted into this character sequence at the indicated offset.
         * </p>
         * <p>
         * The @c offset argument must be greater than or equal to
         * @c 0, and less than or equal to the @em length
         * of this sequence.
         * </p>
         * @param      offset   the offset.
         * @param      d        a @c gdouble.
         * @return     a reference to this object.
         * @throws     IndexOutOfBoundsException  if the offset is invalid.
         */
        XString& insert(gint offset, gdouble d);

        /**
         * Returns the index within this string of the first occurrence of the
         * specified substring.
         *
         * <p>
         * The returned index is the smallest value @c k for which:
         * @code
         * this.toString().startsWith(str, k)
         * @endcode
         * If no such value of @c k exists, then @c -1 is returned.
         * </p>
         * @param   str   the substring to search for.
         * @return  the index of the first occurrence of the specified substring,
         *          or @c -1 if there is no such occurrence.
         */
        gint indexOf(String const& str) const;

        /**
         * Returns the index within this string of the first occurrence of the
         * specified substring, starting at the specified index.
         *
         * <p>
         * The returned index is the smallest value @c k for which:
         * @code
         *     k >= Math.min(fromIndex, this.length()) &&
         *                   this.toString().startsWith(str, k)
         * @endcode
         * If no such value of @c k exists, then @c -1 is returned.
         * </p>
         * @param   str         the substring to search for.
         * @param   fromIndex   the index from which to start the search.
         * @return  the index of the first occurrence of the specified substring,
         *          starting at the specified index,
         *          or @c -1 if there is no such occurrence.
         */
        gint indexOf(String const& str, gint fromIndex) const;

        /**
         * Returns the index within this string of the last occurrence of the
         * specified substring.  The last occurrence of the empty string "" is
         * considered to occur at the index value @c this.length().
         *
         * <p>
         * The returned index is the largest value @c k for which:
         * @code
         * this.toString().startsWith(str, k)
         * @endcode
         * If no such value of @c k exists, then @c -1 is returned.
         * </p>
         * @param   str   the substring to search for.
         * @return  the index of the last occurrence of the specified substring,
         *          or @c -1 if there is no such occurrence.
         */
        gint lastIndexOf(String const& str) const;

        /**
         * Returns the index within this string of the last occurrence of the
         * specified substring, searching backward starting at the specified index.
         *
         * <p>
         * The returned index is the largest value @c k for which:
         * @code
         *     k <= Math::min(fromIndex, this.length()) &&
         *                   this.toString().startsWith(str, k)
         * @endcode
         * If no such value of @c k exists, then @c -1 is returned.
         * </p>
         * @param   str         the substring to search for.
         * @param   fromIndex   the index to start the search from.
         * @return  the index of the last occurrence of the specified substring,
         *          searching backward from the specified index,
         *          or @c -1 if there is no such occurrence.
         */
        gint lastIndexOf(String const& str, gint fromIndex) const;

        /**
         * Causes this character sequence to be replaced by the reverse of
         * the sequence. If there are any surrogate pairs included in the
         * sequence, these are treated as single characters for the
         * reverse operation. Thus, the order of the high-low surrogates
         * is never reversed.
         * <p>
         * Let @a n be the character length of this character sequence
         * (not the length in @c gchar values) just prior to
         * execution of the @c reverse method. Then the
         * character at index @a k in the new character sequence is
         * equal to the character at index @a n-k-1 in the old
         * character sequence.
         * </p>
         * <p>
         * Note that the reverse operation may result in producing
         * surrogate pairs that were unpaired low-surrogates and
         * high-surrogates before the operation. For example, reversing
         * "\uDC00\uD800" produces "\uD800\uDC00" which is
         * a valid surrogate pair.
         * </p>
         * @return  a reference to this object.
         */
        XString& reverse();

        /**
         * Returns a string representing the data in this sequence.
         * A new @c String object is allocated and initialized to
         * contain the character sequence currently represented by this
         * object. This @c String is then returned. Subsequent
         * changes to this sequence do not affect the contents of the
         * @c String.
         *
         * @return  a string representation of this sequence of characters.
         */
        String toString() const override;

        gbool equals(const Object& obj) const override;

        gint hash() const override;

        Object& clone() const override;

        ~XString() override;

    private:
        Coder coding() const;

        void shift(gint offset, gint len);
    };
} // core

#endif //CORE24_XSTRING_H
