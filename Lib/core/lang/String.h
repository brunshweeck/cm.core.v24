//
// Created by bruns on 10/05/2024.
//

#ifndef CORE24_STRING_H
#define CORE24_STRING_H

#include <core/lang/ByteArray.h>
#include <core/lang/CharArray.h>
#include <core/lang/CharSequence.h>
#include <core/lang/IntArray.h>

namespace core {
  /**
   * The @c String class represents character strings. All
   * string literals with suffix @a _S in C++ programs, such as @c "abc"_S, are
   * implemented as instances of this class.
   * <p>
   * Strings are constant; their values cannot be changed after they
   * are created. XString support mutable strings.
   * Because String objects are immutable they can be shared. For example:
   * @code
   *     String str = "abc"_S;
   * @endcode
   * </p>
   * <p>
   * is equivalent to:
   * @code
   *     CharArray data = {'a', 'b', 'c'};
   *     String str = String(data);
   * @endcode
   * </p>
   * <p>
   * Here are some more examples of how strings can be used:
   * @code
   *     Console::out.println("abc"_S);
   *     String cde = "cde"_S;
   *     Console::out.println("abc"_S + cde);
   *     String c = "abc"_S.subString(2, 3);
   *     String d = cde.subString(1, 2);
   * @endcode
   * </p>
   * <p>
   * The class @c String includes methods for examining
   * individual characters of the sequence, for comparing strings, for
   * searching strings, for extracting sub-strings, and for creating a
   * copy of a string with all characters translated to uppercase or to
   * lowercase. Case mapping is based on the Unicode Standard version
   * specified by the @c Character class.
   * </p>
   * <p>
   * The language provides special support for the string
   * concatenation operator (+), and for conversion of
   * other objects to strings.
   * </p>
   *
   * <p>
   * A @c String represents a string in the UTF-16 format
   * in which <em>supplementary characters</em> are represented by <em>surrogate
   * pairs</em> (see the section <i> Unicode Character Representations</i>
   * in the @c Character class for more information).
   * Index values refer to @c char code units, so a supplementary
   * character uses two positions in a @c String.
   * </p>
   * <p>
   * The @c String class provides methods for dealing with
   * Unicode code points (i.e., characters), in addition to those for
   * dealing with Unicode code units (i.e., @c char values).
   * </p>
   *
   * <p>
   * Unless otherwise noted, methods for comparing Strings do not take locale
   * into account.  The @c text::Collator class provides methods for
   * finer-grain, locale-sensitive String comparison.
   * </p>
   */
  class String final : public virtual CharSequence,
                       public virtual Comparable<String> {

    // Aliases
    CORE_ALIAS(ARRAY, Class< gbyte >::Pointer);
    CORE_ALIAS(BYTES, Class< gbyte >::Pointer);
    CORE_ALIAS(CHARS, Class< gchar >::Pointer);
    CORE_ALIAS(INTS, Class< gint >::Pointer);
    CORE_ALIAS(UNSAFE, misc::Unsafe);

    CORE_ALIAS(Charset, charset::Charset);

    // According Access
    CORE_ADD_AS_FRIEND(XString);
    CORE_ADD_AS_FRIEND(util::StringJoiner);

    /**
     * The array used for characters storage.
     */
    ARRAY value = null;

    /**
     * The number of 16 bits characters on this String.
     */
    gint count = 0;

    /**
     * The identifier of the encoding used to encode
     * the bytes in the @c value field.
     */
    enum Coder: gbool { LATIN1, UTF16 } coder = LATIN1;

    /**
     * Cache of hash value for the String
     */
    gint mutable hashValue = 0;

    /**
     * Cache if the hash has been calculated as actually being zero,
     * enabling us to avoid recalculating this.
     */
    gbool mutable hashIsZero = false;

    /**
     * If String compaction is disabled, the bytes in @c value field
     * are always encoded in UTF16.
     */
    static CORE_FAST gbool COMPACT_STRINGS = CORE_HAS_COMPACT_STRINGS;

  public:
    /**
     * Initializes a newly created @c String object so that it represents
     * an empty character sequence.  Note that use of this constructor is
     * unnecessary since Strings are immutable.
     */
    CORE_IMPLICIT String();

    /**
     * Initializes a newly created @c String object so that it represents
     * the same sequence of characters as the argument; in other words, the
     * newly created string is a copy of the argument string.
     *
     * @param  original A @c String
     */
    CORE_IMPLICIT String(String const& original);

    /**
     * Initializes a newly created @c String object so that it represents
     * the same sequence of characters as the argument; in other words, the
     * newly created string is a copy of the argument string.
     *
     * @note After this operation the original String will be empty.
     *
     * @param  original A @c String
     */
    CORE_IMPLICIT String(String&& original) CORE_NOTHROW;

    /**
     * Allocates a new @c String so that it represents the sequence of
     * characters currently contained in the character array argument. The
     * contents of the character array are copied; subsequent modification of
     * the character array does not affect the newly created string.
     *
     * @param  value The initial value of the string
     */
    CORE_EXPLICIT String(CharArray const& value);

    /**
     * Allocates a new @c String that contains characters from a subarray
     * of the character array argument. The @c offset argument is the
     * index of the first character of the subarray and the @c count
     * argument specifies the length of the subarray. The contents of the
     * subarray are copied; subsequent modification of the character array does
     * not affect the newly created string.
     *
     * @param  value Array that is the source of characters
     * @param  offset The initial offset
     * @param  count The length
     *
     * @throws  IndexOutOfBoundsException
     *          If @c offset is negative, @c count is negative, or
     *          @c offset is greater than @c value.length() - count
     */
    CORE_EXPLICIT String(CharArray const& value, gint offset, gint count);

    /**
     * Allocates a new @c String that contains characters from a subarray
     * of the @a Unicode code point array argument.  The @c offset argument
     * is the index of the first code point of the subarray and the @c count
     * argument specifies the length of the subarray.  The contents of the
     * subarray are converted to @c chars; subsequent modification of the @c int
     * array does not affect the newly created string.
     *
     * @param  codePoints Array that is the source of Unicode code points
     * @param  offset The initial offset
     * @param  count The length
     * @throws  IllegalArgumentException
     *          If any invalid Unicode code point is found in @c codePoints
     * @throws  IndexOutOfBoundsException
     *          If @c offset is negative, @c count is negative, or
     *          @c offset is greater than @c codePoints.length() - count
     */
    CORE_EXPLICIT String(IntArray const& codePoints, gint offset, gint count);

    /**
     * Allocates a new @c String constructed from a subarray of an array
     * of 8-bit integer values.
     *
     * The @c offset argument is the index of the first byte of the
     * subarray, and the @c count argument specifies the length of the
     * subarray.
     *
     * Each @c byte in the subarray is converted to a @c gchar as
     * specified in the @c String(ByteArray,int) constructor.
     *
     * @note This method does not properly convert bytes into characters.
     *       The recommended way to do this is via the @c String constructors
     *       that take a @c Charset, @a charset @a name, or that use
     *       the @a default @a charset.
     *
     * @param  ascii The bytes to be converted to characters.
     * @param  hibyte The top 8 bits of each 16-bit Unicode code unit.
     * @param  offset The initial offset
     * @param  count The length
     *
     * @throws  IndexOutOfBoundsException
     *          If @c offset is negative, @c count is negative, or
     *          @c offset is greater than @c ascii.length() - count
     */
    CORE_EXPLICIT String(ByteArray const& ascii, gint hibyte, gint offset, gint count);

    /**
     * Allocates a new @c String containing characters constructed from
     * an array of 8-bit integer values. Each character @c c in the
     * resulting string is constructed from the corresponding component
     * @c b in the byte array such that:
     * @code
     *  c == (gchar)(((hibyte & 0xff) << 8) | (b & 0xff))
     * @endcode
     *
     * @note This method does not properly convert bytes into characters.
     *       The recommended way to do this is via the @c String constructors
     *       that take a @c Charset, @a charset @a name, or that use
     *       the @a default @a charset.
     *
     * @param  ascii The bytes to be converted to characters
     * @param  hibyte The top 8 bits of each 16-bit Unicode code unit
     */
    CORE_EXPLICIT String(ByteArray const& ascii, gint hibyte);

    /**
     * Constructs a new @c String by decoding the specified array of bytes
     * using the specified @em charset.  The
     * length of the new @c String is a function of the charset, and hence
     * may not be equal to the length of the byte array.
     *
     * <p>
     * The behavior of this constructor when the given bytes are not valid
     * in the given charset is unspecified.  The
     * @b CharsetDecoder class should be used when more control
     * over the decoding process is required.
     * </p>
     * <p>
     * The contents of the string are unspecified if the byte array
     * is modified during string construction.
     * </p>
     * @param  bytes
     *         The bytes to be decoded into characters
     *
     * @param  charsetName
     *         The name of a supported @em charset
     *
     * @throws  UnsupportedEncodingException
     *          If the named charset is not supported
     */
    CORE_EXPLICIT String(ByteArray const& bytes, String const& charsetName);

    /**
     * Constructs a new @c String by decoding the specified array of
     * bytes using the specified @em charset.
     * The length of the new @c String is a function of the charset, and
     * hence may not be equal to the length of the byte array.
     *
     * <p> This method always replaces malformed-input and unmappable-character
     * sequences with this charset's default replacement string.  The
     * @b charset::CharsetDecoder class should be used when more control
     * over the decoding process is required.
     * </p>
     * <p>
     * The contents of the string are unspecified if the byte array
     * is modified during string construction.
     * </p>
     * @param  bytes
     *         The bytes to be decoded into characters
     *
     * @param  charset
     *         The @em charset to be used to
     *         decode the @c bytes
     */
    CORE_EXPLICIT String(ByteArray const& bytes, Charset const& charset);

    /**
     * Constructs a new @c String by decoding the specified subarray of
     * bytes using the <b> default charset</b>.
     * The length of the new @c String is a function of the charset,
     * and hence may not be equal to the length of the subarray.
     *
     * <p>
     * The behavior of this constructor when the given bytes are not valid
     * in the default charset is unspecified.  The
     * @b CharsetDecoder class should be used when more control
     * over the decoding process is required.
     * </p>
     * <p>
     * The contents of the string are unspecified if the byte array
     * is modified during string construction.
     * </p>
     * @param  bytes
     *         The bytes to be decoded into characters
     *
     * @param  offset
     *         The index of the first byte to decode
     *
     * @param  length
     *         The number of bytes to decode
     *
     * @throws  IndexOutOfBoundsException
     *          If @c offset is negative, @c length is negative, or
     *          @c offset is greater than @code bytes.length() - length() @endcode
     */
    CORE_EXPLICIT String(ByteArray const& bytes, gint offset, gint length);

    /**
     * Constructs a new @c String by decoding the specified array of bytes
     * using the <b> default charset</b>. The length
     * of the new @c String is a function of the charset, and hence may not
     * be equal to the length of the byte array.
     *
     * <p>
     * The behavior of this constructor when the given bytes are not valid
     * in the default charset is unspecified.  The
     * @b CharsetDecoder class should be used when more control
     * over the decoding process is required.
     * </p>
     * <p>
     * The contents of the string are unspecified if the byte array
     * is modified during string construction.
     * </p>
     * @param  bytes
     *         The bytes to be decoded into characters
     */
    CORE_EXPLICIT String(ByteArray const& bytes);

    /**
     * Allocate new String containing the characters constructed from
     * a literal string (pointer or array) of 8 bits, 16 bits or 32 bits chars values.
     *
     * @note @li If the given pointer is 8 bits char pointer (or 8 bits char array)
     *       and the property @c Unsafe::ANSI_LITERAL is true, the given
     *       pointer will be considered as Latin-1 string (MSVC compiler).
     *       In otherwise, as UTF-8 string.
     *       @li If the type of given literal is pointer type, the length is
     *       considered as index of first null character (U+0000).
     *       @code
     *        auto str = "hello\0 boy"; // char const*
     *        String s = str; // the result is "hello"
     *       @endcode
     *       @li If the type of given literal is array type, the length is
     *       considered as size of this array. Except that if the last character
     *       is null character the length will be size of this array minus one.
     *       @code
     *         auto&& str1 = "hello\0 boy"; // char const[11] (10 chars + null character)
     *         String s1 = str1; // the result is "hello\0 boy"
     *         char const str2[] = {'h', 'e', 'l', 'l', 'o', '\0', 'b', 'o', 'y'} // char const[10]
     *         String s2 = str2; // the result is "hello\0 boy"
     *         String s3 = "hello\0 boy"; // the result is "hello\0 boy"
     *       @endcode
     *       @li the supported literal char types: @c char (signed or unsigned), @c char16_t, @c char32_t,
     *       @c wchar_t.
     *       @li If the length of literal great that Integer::MAX_VALUE the output string will be the
     *       part of given literal (from index @c 0 to index @code Integer::MAX_VALUE - 1 @endcode)
     *
     *
     * @tparam Str the type of string literal
     * @param str the literal string
     * @throws IllegalArgumentException if the given pointer is @c null
     */
    template <class Str, ClassOf(1)::OnlyIf<Class<Str>::isString()> CaptureString = 1>
    CORE_IMPLICIT String(Str const& str): String() {
      glong len = 0; // the size of literal string
      gint bpc = 0; // number of bytes per char
      if (Class<Str>::isArray()) {
        // CharT const[len]
        CORE_ALIAS(CharT, typename Class<Str>::ArrayElement);
        bpc = Class<CharT>::size();
        len = Class<Str>::count();
        if (len > 0) {
          // Remove last null character (it mark end of literal string)
          if (str[len - 1] == '\0')
            len -= 1;
        }
      } else {
        // Class<Str>::isPointer()
        // CharT const*
        CORE_ALIAS(CharT, typename Class<Str>::PointerTarget);
        bpc = Class<CharT>::size();
        gint est = 0; // the estimation of literal string length
        if (!isNullPointer(str))
          while (str[est] != '\0')
            est++;
        else
          est = -1;
        len = est;
      }

      initStringFromLiteral((glong) str, len, bpc);
    }

    /**
     * Set this String content with given String.
     *
     * @param other A @c String
     * @return itself
     */
    String& operator=(String const& other);

    /**
     * Set this String content with given String.
     *
     * @param other A @c String
     * @return itself
     */
    String& operator=(String&& other) CORE_NOTHROW;

    /**
     * Return the length of this String. The length
     * is equals to the number of Unicode code unit in this String
     *
     * @return The length of chars sequence represented by this String
     */
    gint length() const override;

    /**
     * Return @c true if value returned by @c length() is @c 0.
     *
     * @return @c true if @c length() is @c 0.
     */
    gbool isEmpty() const override;

    /**
     * Returns the @c char value at the specified index.
     * An index ranges from @c 0 to @c length() - 1.
     * The first @c char value of the sequence is at index @c 0,
     * the next at index @c 1, and so on, as for array indexing.
     *
     * If the @c char value specified by the index is a @a surrogate, the surrogate
     * value is returned.
     *
     * @param index the index of the @c char value.
     * @throws IndexOutOfBoundsException if the @c index argument is negative or
     *                                    not less than the length of this string.
     * @return the @c char value at the specified index of this string.
     */
    gchar charAt(gint index) const override;

    /**
     * Returns the character (Unicode code point) at the specified
     * index. The index refers to @c char values
     * (Unicode code units) and ranges from @c 0 to @c length()-1.
     *
     * If the @c char value specified at the given index
     * is in the high-surrogate range, the following index is less
     * than the length of this @c String, and the
     * @c char value at the following index is in the
     * low-surrogate range, then the supplementary code point
     * corresponding to this surrogate pair is returned. Otherwise,
     * the @c char value at the given index is returned.
     *
     * @param      index the index to the @c char values
     * @return     the code point value of the character at the
     *             @c index
     * @throws     IndexOutOfBoundsException  if the @c index
     *             argument is negative or not less than the length of this
     *             string.
     */
    gint codePointAt(gint index) const;

    /**
     * Returns the number of Unicode code points in the specified text
     * range of this @c String. The text range begins at the
     * specified @c beginIndex and extends to the
     * @c char at index @code endIndex - 1 @endcode . Thus, the
     * length (in @c chars) of the text range is
     * @c endIndex-beginIndex. Unpaired surrogates within
     * the text range count as one code point each.
     *
     * @param beginIndex the index to the first @c char of
     *                   the text range.
     * @param endIndex the index after the last @c char of
     *                 the text range.
     * @return the number of Unicode code points in the specified text
     *         range
     * @throws IndexOutOfBoundsException if the @c beginIndex is negative,
     *         or @c endIndex is larger than the length of this @c String,
     *         or @c beginIndex is larger than @c endIndex.
     */
    gint codePointCount(gint beginIndex, gint endIndex) const;

    /**
     * Copies characters from this string into the destination character
     * array.
     *
     * The first character to be copied is at index @c beginIndex;
     * the last character to be copied is at index @c endIndex-1
     * (thus the total number of characters to be copied is @c endIndex- @c beginIndex).
     * The characters are copied into the subarray of @c dest starting at index @c offset
     * and ending at index: @code offset + (endIndex-beginIndex) - 1 @endcode
     *
     * @param beginIndex index of the first character in the string to copy.
     * @param endIndex   index after the last character in the string to copy.
     * @param dest       the destination array.
     * @param offset     the start offset in the destination array.
     * @throws IndexOutOfBoundsException If any of the following is true:
     *            @li @c beginIndex is negative.
     *            @li @c beginIndex is greater than @c endIndex .
     *            @li @c endIndex is greater than the length of this string .
     *            @li @c offset is negative .
     *            @li @code offset + (endIndex - beginIndex) @endcode is larger than @c dest.length() .
     */
    void toChars(gint beginIndex, gint endIndex, CharArray& dest, gint offset) const;

    /**
     * Copies characters from this string into the destination byte array. Each
     * byte receives the 8 low-order bits of the corresponding character. The
     * eight high-order bits of each character are not copied and do not
     * participate in the transfer in any way.
     *
     * The first character to be copied is at index @c beginIndex; the
     * last character to be copied is at index @c endIndex-1.  The total
     * number of characters to be copied is @c endIndex - @c beginIndex. The
     * characters, converted to bytes, are copied into the subarray of @c
     * dest starting at index @c offset and ending at index:
     * @code offset + (endIndex-beginIndex) - 1 @endcode
     *
     * @note  This method does not properly convert characters into
     * bytes.  The recommended way to do this is via the @c toBytes() method,
     * which uses the default charset.
     *
     * @param  beginIndex Index of the first character in the string to copy
     * @param  endIndex Index after the last character in the string to copy
     * @param  dest The destination array
     * @param  offset The start offset in the destination array
     * @throws  IndexOutOfBoundsException If any of the following is true:
     *            @li @c beginIndex is negative
     *            @li @c beginIndex is greater than @c endIndex .
     *            @li @c endIndex is greater than the length of this String .
     *            @li @c offset is negative .
     *            @li @code offset+(endIndex-beginIndex) @endcode is larger than @c dest.length()
     */
    void toBytes(gint beginIndex, gint endIndex, ByteArray& dest, gint offset) const;

    /**
     * Encodes this @c String into a sequence of bytes using the named
     * charset, storing the result into a new byte array.
     *
     * <p>
     * The behavior of this method when this string cannot be encoded in
     * the given charset is unspecified.  The @b charset.CharsetEncoder
     * class should be used when more control over the encoding process is required.
     * </p>
     *
     * @param  charsetName
     *         The name of a supported @em charset
     *
     * @return  The resultant byte array
     *
     * @throws  UnsupportedEncodingException
     *          If the named charset is not supported
     */
    ByteArray toBytes(String const& charsetName) const;

    /**
     * Encodes this @c String into a sequence of bytes using the given
     * @em charset, storing the result into a new byte array.
     *
     * <p>
     * This method always replaces malformed-input and unmappable-character
     * sequences with this charset's default replacement byte array.  The
     * @b.CharsetEncoder class should be used when more
     * control over the encoding process is required.
     * </p>
     * @param  charset
     *         The @em Charset to be used to encode
     *         the @c String
     *
     * @return  The resultant byte array
     */
    ByteArray toBytes(Charset const& charset) const;

    /**
     * Encodes this @c String into a sequence of bytes using the
     * <b> default charset</b>, storing the result
     * into a new byte array.
     *
     * <p>
     * The behavior of this method when this string cannot be encoded in
     * the default charset is unspecified.  The @b CharsetEncoder
     * class should be used when more control over the encoding process is required.
     * </p>
     * @return  The resultant byte array
     */
    ByteArray toBytes() const;

    /**
     * Compares this string to the specified object.  The result is @c true
     * if and only if the argument is not @c null and is a @c String object
     * that represents the same sequence of characters as this object.
     *
     * For finer-grained String comparison, refer to @a text::Collator.
     *
     * @param  obj The object to compare this @c String against
     * @return  @c true if the given object represents a @c String
     *          equivalent to this string, @c false otherwise
     */
    gbool equals(Object const& obj) const override;

    /**
     * Compares this string to the specified @c CharSequence.  The
     * result is @c true if and only if this @c String represents the
     * same sequence of char values as the specified sequence. Note that if the
     * @c CharSequence is a @c XString then the method
     * synchronizes on it.
     *
     * <p>
     * For finer-grained String comparison, refer to @b text::Collator.
     * </p>
     * @param  cs
     *         The sequence to compare this @c String against
     *
     * @return  @c true if this @c String represents the same
     *          sequence of char values as the specified sequence, @c false otherwise
     */
    gbool contentEquals(CharSequence const& cs) const;

    /**
     * Compares this @c String to another @c String, ignoring case
     * considerations.  Two strings are considered equal ignoring case if they
     * are of the same length and corresponding Unicode code points in the two
     * strings are equal ignoring case.
     *
     * <p>
     * Two Unicode code points are considered the same
     * ignoring case if at least one of the following is true:
     *
     *   @li The two Unicode code points are the same (as compared by the
     *        @c == operator)
     *   @li Calling @c Character::toLowerCase(Character.toUpperCase(int))
     *        on each Unicode code point produces the same result
     * </p>
     *
     * <p>
     * Note that this method does <em>not</em> take locale into account, and
     * will result in unsatisfactory results for certain locales.  The
     * @b text::Collator class provides locale-sensitive comparison.
     * </p>
     * @param  anotherString
     *         The @c String to compare this @c String against
     *
     * @return  @c true if the argument represents an equivalent
     *          @c String ignoring case; @c false otherwise
     *
     * @see  equals(Object)
     * @see  codePoints()
     */
    gbool equalsIgnoreCase(String const& anotherString) const;

    /**
     * Compares two strings lexicographically.
     * The comparison is based on the Unicode value of each character in
     * the strings. The character sequence represented by this
     * @c String object is compared lexicographically to the
     * character sequence represented by the argument string. The result is
     * a negative integer if this @c String object lexicographically precedes
     * the argument string. The result is a positive integer if this @c String
     * object lexicographically follows the argument string. The result is zero
     * if the strings are equal; @c compareTo returns @c 0 exactly when
     * the @c equals(Object) method would return @c true.
     *
     * This is the definition of lexicographic ordering. If two strings are
     * different, then either they have different characters at some index
     * that is a valid index for both strings, or their lengths are different,
     * or both. If they have different characters at one or more index
     * positions, let @a k be the smallest such index; then the string
     * whose character at position @a k has the smaller value, as
     * determined by using the @c < operator, lexicographically precedes the
     * other string. In this case, @c compareTo returns the
     * difference of the two character values at position @c k in
     * the two string -- that is, the value:
     * @code this.charAt(k)-anotherString.charAt(k) @endcode
     * If there is no index position at which they differ, then the shorter
     * string lexicographically precedes the longer string. In this case,
     * @c compareTo returns the difference of the lengths of the
     * strings -- that is, the value:
     * @code this.length()-anotherString.length() @endcode
     *
     * For finer-grained String comparison, refer to @c text::Collator.
     *
     * @param   anotherString   the @c String to be compared.
     * @return  the value @c 0 if the argument string is equal to
     *          this string; a value less than @c 0 if this string
     *          is lexicographically less than the string argument; and a
     *          value greater than @c 0 if this string is
     *          lexicographically greater than the string argument.
     */
    gint compareTo(String const& anotherString) const override;

    /**
     * Compares two strings lexicographically, ignoring case
     * differences. This method returns an integer whose sign is that of
     * calling @c compareTo with case folded versions of the strings
     * where case differences have been eliminated by calling
     * @c Character::toLowerCase(Character.toUpperCase(int)) on
     * each Unicode code point.
     * <p>
     * Note that this method does <em>not</em> take locale into account,
     * and will result in an unsatisfactory ordering for certain locales.
     * The @b text::Collator class provides locale-sensitive comparison.
     * </p>
     * @param   str   the @c String to be compared.
     * @return  a negative integer, zero, or a positive integer as the
     *          specified String is greater than, equal to, or less
     *          than this String, ignoring case considerations.
     * @see     java.text.Collator
     * @see     #codePoints()
     */
    gint compareToIgnoreCase(String const& str) const;

    /**
     * Tests if the substring of this string beginning at the
     * specified index starts with the specified prefix.
     *
     * @param   prefix    the prefix.
     * @param   offset   where to begin looking in this string.
     * @return  @c true if the character sequence represented by the
     *          argument is a prefix of the substring of this object starting
     *          at index @c offset; @c false otherwise.
     *          The result is @c false if @c offset is
     *          negative or greater than the length of this
     *          @c String object; otherwise the result is the same
     *          as the result of the expression
     *          <pre>
     *          this.subString(offset).startsWith(prefix)
     *          </pre>
     */
    gbool startsWith(String const& prefix, gint offset) const;

    /**
     * Tests if this string starts with the specified prefix.
     *
     * @param   prefix   the prefix.
     * @return  @c true if the character sequence represented by the
     *          argument is a prefix of the character sequence represented by
     *          this string; @c false otherwise.
     *          Note also that @c true will be returned if the
     *          argument is an empty string or is equal to this
     *          @c String object as determined by the
     *          @c equals(Object) method.
     */
    gbool startsWith(String const& prefix) const;

    /**
     * Tests if this string ends with the specified suffix.
     *
     * @param   suffix   the suffix.
     * @return  @c true if the character sequence represented by the
     *          argument is a suffix of the character sequence represented by
     *          this object; @c false otherwise. Note that the
     *          result will be @c true if the argument is the
     *          empty string or is equal to this @c String object
     *          as determined by the @c equals(Object) method.
     */
    gbool endsWith(String const& suffix) const;

    /**
     * Returns a hash code for this string. The hash code for a
     * @c String object is computed as
     * @code
     * s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1]
     * @endcode
     * using @c int arithmetic, where @c s[i] is the
     * <i>i</i>th character of the string, @c n is the length of
     * the string, and @c ^ indicates exponentiation.
     * (The hash value of the empty string is zero.)
     *
     * @return  a hash code value for this object.
     */
    gint hash() const override;

    /**
     * Returns the index within this string of the first occurrence of
     * the specified character. If a character with value
     * @c ch occurs in the character sequence represented by
     * this @c String object, then the index (in Unicode
     * code units) of the first such occurrence is returned. For
     * values of @c ch in the range from 0 to 0xFFFF
     * (inclusive), this is the smallest value <i>k</i> such that:
     * @code
     * this.charAt(<i>k</i>) == ch
     * @endcode
     * is true. For other values of @c ch, it is the
     * smallest value <i>k</i> such that:
     * @code
     * this.codePointAt(<i>k</i>) == ch
     * @endcode
     * is true. In either case, if no such character occurs in this
     * string, then @c -1 is returned.
     *
     * @param   ch   a character (Unicode code point).
     * @return  the index of the first occurrence of the character in the
     *          character sequence represented by this object, or
     *          @c -1 if the character does not occur.
     */
    gint indexOf(gint ch) const;

    /**
     * Returns the index within this string of the first occurrence of the
     * specified character, starting the search at the specified index.
     * <p>
     * If a character with value @c ch occurs in the
     * character sequence represented by this @c String
     * object at an index no smaller than @c fromIndex, then
     * the index of the first such occurrence is returned. For values
     * of @c ch in the range from 0 to 0xFFFF (inclusive),
     * this is the smallest value <i>k</i> such that:
     * @code
     * (this.charAt(<i>k</i>) == ch) @c && (<i>k</i> &gt;= fromIndex)
     * @endcode
     * is true. For other values of @c ch, it is the
     * smallest value <i>k</i> such that:
     * @code
     * (this.codePointAt(<i>k</i>) == ch) @c && (<i>k</i> &gt;= fromIndex)
     * @endcode
     * is true. In either case, if no such character occurs in this
     * string at or after position @c fromIndex, then
     * @c -1 is returned.
     * </p>
     * <p>
     * There is no restriction on the value of @c fromIndex. If it
     * is negative, it has the same effect as if it were zero: this entire
     * string may be searched. If it is greater than the length of this
     * string, it has the same effect as if it were equal to the length of
     * this string: @c -1 is returned.
     * </p>
     * <p>
     * All indices are specified in @c char values
     * (Unicode code units).
     * </p>
     * @param   ch          a character (Unicode code point).
     * @param   fromIndex   the index to start the search from.
     * @return  the index of the first occurrence of the character in the
     *          character sequence represented by this object that is greater
     *          than or equal to @c fromIndex, or @c -1
     *          if the character does not occur.
     */
    gint indexOf(gint ch, gint fromIndex) const;

    /**
     * Returns the index within this string of the last occurrence of
     * the specified character. For values of @c ch in the
     * range from 0 to 0xFFFF (inclusive), the index (in Unicode code
     * units) returned is the largest value <i>k</i> such that:
     * @code
     * this.charAt(<i>k</i>) == ch
     * @endcode
     * is true. For other values of @c ch, it is the
     * largest value <i>k</i> such that:
     * @code
     * this.codePointAt(<i>k</i>) == ch
     * @endcode
     * is true.  In either case, if no such character occurs in this
     * string, then @c -1 is returned.  The
     * @c String is searched backwards starting at the last
     * character.
     *
     * @param   ch   a character (Unicode code point).
     * @return  the index of the last occurrence of the character in the
     *          character sequence represented by this object, or
     *          @c -1 if the character does not occur.
     */
    gint lastIndexOf(gint ch) const;

    /**
     * Returns the index within this string of the last occurrence of
     * the specified character, searching backward starting at the
     * specified index. For values of @c ch in the range
     * from 0 to 0xFFFF (inclusive), the index returned is the largest
     * value <i>k</i> such that:
     * @code
     * (this.charAt(<i>k</i>) == ch) @c && (<i>k</i> &lt;= fromIndex)
     * @endcode
     * is true. For other values of @c ch, it is the
     * largest value <i>k</i> such that:
     * @code
     * (this.codePointAt(<i>k</i>) == ch) @c && (<i>k</i> &lt;= fromIndex)
     * @endcode
     * is true. In either case, if no such character occurs in this
     * string at or before position @c fromIndex, then
     * @c -1 is returned.
     *
     * <p>
     * All indices are specified in @c char values
     * (Unicode code units).
     * </p>
     * @param   ch          a character (Unicode code point).
     * @param   fromIndex   the index to start the search from. There is no
     *          restriction on the value of @c fromIndex. If it is
     *          greater than or equal to the length of this string, it has
     *          the same effect as if it were equal to one less than the
     *          length of this string: this entire string may be searched.
     *          If it is negative, it has the same effect as if it were -1:
     *          -1 is returned.
     * @return  the index of the last occurrence of the character in the
     *          character sequence represented by this object that is less
     *          than or equal to @c fromIndex, or @c -1
     *          if the character does not occur before that point.
     */
    gint lastIndexOf(gint ch, gint fromIndex) const;

    /**
     * Returns the index within this string of the first occurrence of the
     * specified substring.
     *
     * <p>
     * The returned index is the smallest value @c k for which:
     * @code
     * this.startsWith(str, k)
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
     *                   this.startsWith(str, k)
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
     * specified substring.  The last occurrence of the empty string ""
     * is considered to occur at the index value @c this.length().
     *
     * <p>
     * The returned index is the largest value @c k for which:
     * @code
     * this.startsWith(str, k)
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
     *     k <= Math.min(fromIndex, this.length()) &&
     *                   this.startsWith(str, k)
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
     * Returns a string that is a substring of this string. The
     * substring begins with the character at the specified index and
     * extends to the end of this string.
     * <p>
     * Examples:
     * @code
     * "unhappy".subString(2) returns "happy"
     * "Harrison".subString(3) returns "rison"
     * "emptiness".subString(9) returns "" (an empty string)
     * @endcode
     * </p>
     * @param      beginIndex   the beginning index, inclusive.
     * @return     the specified substring.
     * @throws     IndexOutOfBoundsException  if
     *             @c beginIndex is negative or larger than the
     *             length of this @c String object.
     */
    String subString(gint beginIndex) const;

    /**
     * Returns a string that is a substring of this string. The
     * substring begins at the specified @c beginIndex and
     * extends to the character at index @c endIndex - 1.
     * Thus, the length of the substring is @c endIndex-beginIndex.
     * <p>
     * Examples:
     * @code
     * "hamburger".subString(4, 8) returns "urge"
     * "smiles".subString(1, 5) returns "mile"
     * @endcode
     * </p>
     * @param      beginIndex   the beginning index, inclusive.
     * @param      endIndex     the ending index, exclusive.
     * @return     the specified substring.
     * @throws     IndexOutOfBoundsException  if the
     *             @c beginIndex is negative, or
     *             @c endIndex is larger than the length of
     *             this @c String object, or
     *             @c beginIndex is larger than
     *             @c endIndex.
     */
    String subString(gint beginIndex, gint endIndex) const;

    /**
     * Returns a character sequence that is a subsequence of this sequence.
     *
     * <p>
     * An invocation of this method of the form
     *
     * @code
     * str.subSequence(begin, end)
     * @endcode
     *
     * behaves in exactly the same way as the invocation
     *
     * @code
     * str.subString(begin, end)
     * @endcode
     * </p>
     * @note
     * This method is defined so that the @c String class can implement
     * the @c CharSequence interface.
     *
     * @param   startIndex   the beginning index, inclusive.
     * @param   endIndex     the end index, exclusive.
     * @return  the specified subsequence.
     *
     * @throws  IndexOutOfBoundsException
     *          if @c beginIndex or @c endIndex is negative,
     *          if @c endIndex is greater than @c length(),
     *          or if @c beginIndex is greater than @c endIndex
     *
     *
     */
    CharSequence& subSequence(gint startIndex, gint endIndex) const override;

    /**
     * Concatenates the specified string to the end of this string.
     * <p>
     * If the length of the argument string is @c 0, then this
     * @c String object is returned. Otherwise, a
     * @c String object is returned that represents a character
     * sequence that is the concatenation of the character sequence
     * represented by this @c String object and the character
     * sequence represented by the argument string.
     * </p>
     * <p>
     * Examples:
     * @code
     *  "cares".concat("s"); // returns "caress".
     *  "to".concat("get").concat("her"); // returns "together"
     * @endcode
     * </p>
     * @param   str   the @c String that is concatenated to the end
     *                of this @c String.
     * @return  a string that represents the concatenation of this object's
     *          characters followed by the string argument's characters.
     */
    String concat(String const& str) const;

    /**
     * Returns a string resulting from replacing all occurrences of
     * @c oldChar in this string with @c newChar.
     * <p>
     * If the character @c oldChar does not occur in the
     * character sequence represented by this @c String object,
     * then a reference to this @c String object is returned.
     * Otherwise, a @c String object is returned that
     * represents a character sequence identical to the character sequence
     * represented by this @c String object, except that every
     * occurrence of @c oldChar is replaced by an occurrence
     * of @c newChar.
     * </p>
     * <p>
     * Examples:
     * @code
     * "mesquite in your cellar".replace('e', 'o')
     *         returns "mosquito in your collar"
     * "the war of baronets".replace('r', 'y')
     *         returns "the way of bayonets"
     * "sparring with a purple porpoise".replace('p', 't')
     *         returns "starring with a turtle tortoise"
     * "JonL".replace('q', 'x') returns "JonL" (no change)
     * @endcode
     * </p>
     * @param   oldChar   the old character.
     * @param   newChar   the new character.
     * @return  a string derived from this string by replacing every
     *          occurrence of @c oldChar with @c newChar.
     */
    String replace(gchar oldChar, gchar newChar) const;

    /**
     * Tells whether this string matches the given regular expression.
     *
     * <p> An invocation of this method of the form
     * @code
     *  str.matches(regex)
     * @endcode
     * yields exactly the ame result as the expression
     *
     * @code
     *   Pattern::matches(regex, str)
     * @endcode
     * </p>
     * @param   regex
     *          the regular expression to which this string is to be matched
     *
     * @return  @c true if, and only if, this string matches the
     *          given regular expression
     *
     * @throws  PatternSyntaxException
     *          if the regular expression's syntax is invalid
     *
     * @see regex::Pattern
     */
    gbool matches(String const& regex) const;

    /**
     * Returns true if and only if this string contains the specified
     * sequence of char values.
     *
     * @param s the sequence to search for
     * @return true if this string contains @c s, false otherwise
     *
     */
    gbool contains(CharSequence const& s) const;

    /**
     * Replaces each substring of this string that matches the literal target
     * sequence with the specified literal replacement sequence. The
     * replacement proceeds from the beginning of the string to the end, for
     * example, replacing "aa" with "b" in the string "aaa" will result in
     * "ba" rather than "ab".
     *
     * @param  target The sequence of char values to be replaced
     * @param  replacement The replacement sequence of char values
     * @return  The resulting string
     *
     */
    String replace(CharSequence const& target, CharSequence const& replacement) const;

    /**
     * Replaces the first substring of this string that matches the given regular expression with the
     * given replacement.
     *
     * <p>
     * An invocation of this method of the form
     * @code
     *   str.replaceFirst(regex, repl)
     * @endcode
     * yields exactly the same result as the expression
     *
     * @code
     *   Pattern::compile(regex).matcher(str).replaceFirst(repl)
     * @endcode
     * </p>
     * <p>
     * Note that backslashes (@c '\') and dollar signs (@c '$') in the
     * replacement string may cause the results to be different from if it were
     * being treated as a literal replacement string; see
     * @b regex::Matcher::replaceFirst.
     * Use @b regex::Matcher::quoteReplacement to suppress the special
     * meaning of these characters, if desired.
     *
     * @param   regex
     *          the regular expression to which this string is to be matched
     * @param   replacement
     *          the string to be substituted for the first match
     *
     * @return  The resulting @c String
     *
     * @throws  PatternSyntaxException
     *          if the regular expression's syntax is invalid
     *
     * @see regex::Pattern
     */
    String replaceFirst(String const& regex, String const& replacement) const;

    /**
     * Replaces each substring of this string that matches the given regular expression with the
     * given replacement.
     *
     * <p>
     * An invocation of this method of the form
     * @code
     *   str.replaceAll(regex, repl)
     * @endcode
     * yields exactly the same result as the expression
     *
     * @code
     *   Pattern::compile(regex).matcher(str).replaceAll(repl)
     * @endcode
     *
     *<p>
     * Note that backslashes (@c '\') and dollar signs (@c '$') in the
     * replacement string may cause the results to be different from if it were
     * being treated as a literal replacement string; see
     * @b regex::Matcher::replaceAll.
     * Use @b regex::Matcher::quoteReplacement to suppress the special
     * meaning of these characters, if desired.
     * </p>
     * @param   regex
     *          the regular expression to which this string is to be matched
     * @param   replacement
     *          the string to be substituted for each match
     *
     * @return  The resulting @c String
     *
     * @throws  PatternSyntaxException
     *          if the regular expression's syntax is invalid
     *
     * @see regex::Pattern
     */
    String replaceAll(String const& regex, String const& replacement) const;

    /**
     * Splits this string around matches of the given regular expression.
     *
     * <p>
     * The array returned by this method contains each substring of this
     * string that is terminated by another substring that matches the given
     * expression or is terminated by the end of the string.  The substrings in
     * the array are in the order in which they occur in this string.  If the
     * expression does not match any part of the input then the resulting array
     * has just one element, namely this string.
     * </p>
     * <p>
     * When there is a positive-width match at the beginning of this
     * string then an empty leading substring is included at the beginning
     * of the resulting array. A zero-width match at the beginning however
     * never produces such empty leading substring.
     * </p>
     * <p>
     * The @c limit parameter controls the number of times the
     * pattern is applied and therefore affects the length of the resulting
     * array.
     * </p>
     *    @li <p>
     *    If the <i>limit</i> is positive then the pattern will be applied
     *    at most <i>limit</i>&nbsp;-&nbsp;1 times, the array's length will be
     *    no greater than <i>limit</i>, and the array's last entry will contain
     *    all input beyond the last matched delimiter.</p>
     *
     *    @li <p>
     *    If the <i>limit</i> is zero then the pattern will be applied as
     *    many times as possible, the array can have any length, and trailing
     *    empty strings will be discarded.</p>
     *
     *    @li <p>
     *    If the <i>limit</i> is negative then the pattern will be applied
     *    as many times as possible and the array can have any length.</p>
     *
     *
     * <p>
     * The string @c "boo:and:foo", for example, yields the
     * following results with these parameters:
     *
     * <table>
     * <caption>Split example showing regex, limit, and result</caption>
     * <thead>
     * <tr>
     *     <th>Regex</th>
     *     <th>Limit</th>
     *     <th>Result</th>
     * </tr>
     * </thead>
     * <tbody>
     * <tr><th>:</th>
     *     <th>2</th>
     *     <td>@c { "boo", "and:foo" }</td></tr>
     * <tr><!-- : -->
     *     <th>5</th>
     *     <td>{ "boo", "and", "foo" }</td></tr>
     * <tr><!-- : -->
     *     <th>-2</th>
     *     <td>{ "boo", "and", "foo" }</td></tr>
     * <tr><th>o</th>
     *     <th>5</th>
     *     <td>{ "b", "", ":and:f", "", "" }</td></tr>
     * <tr><!-- o -->
     *     <th>-2</th>
     *     <td>{ "b", "", ":and:f", "", "" }</td></tr>
     * <tr><!-- o -->
     *     <th>0</th>
     *     <td>{ "b", "", ":and:f" }</td></tr>
     * </tbody>
     * </table>
     * <p>
     * An invocation of this method of the form
     * @code
     *   str.split(regex, n)
     * @endcode
     * yields the same result as the expression
     *
     * @code
     *   Pattern::compile(regex).split(str, n)
     * @endcode
     * </p>
     *
     * @param  regex
     *         the delimiting regular expression
     *
     * @param  limit
     *         the result threshold, as described above
     *
     * @return  the array of strings computed by splitting this string
     *          around matches of the given regular expression
     *
     * @throws  PatternSyntaxException
     *          if the regular expression's syntax is invalid
     *
     * @see regex::Pattern
     */
    StringArray split(String const& regex, gint limit) const;

    /**
     * Splits this string around matches of the given regular expression and
     * returns both the strings and the matching delimiters.
     *
     * <p>
     * The array returned by this method contains each substring of this
     * string that is terminated by another substring that matches the given
     * expression or is terminated by the end of the string.
     * Each substring is immediately followed by the subsequence (the delimiter)
     * that matches the given expression, <em>except</em> for the last
     * substring, which is not followed by anything.
     * The substrings in the array and the delimiters are in the order in which
     * they occur in the input.
     * If the expression does not match any part of the input then the resulting
     * array has just one element, namely this string.
     * </p>
     * <p>
     * When there is a positive-width match at the beginning of this
     * string then an empty leading substring is included at the beginning
     * of the resulting array. A zero-width match at the beginning however
     * never produces such empty leading substring nor the empty delimiter.
     * </p>
     * <p>
     * The @c limit parameter controls the number of times the
     * pattern is applied and therefore affects the length of the resulting
     * array.
     *
     *    @li If the <i>limit</i> is positive then the pattern will be applied
     *    at most <i>limit</i>&nbsp;-&nbsp;1 times, the array's length will be
     *    no greater than 2 &times; <i>limit</i> - 1, and the array's last
     *    entry will contain all input beyond the last matched delimiter.
     *
     *    @li If the <i>limit</i> is zero then the pattern will be applied as
     *    many times as possible, the array can have any length, and trailing
     *    empty strings will be discarded.
     *
     *    @li If the <i>limit</i> is negative then the pattern will be applied
     *    as many times as possible and the array can have any length.
     * </p>
     *
     * <p>
     * The input @c "boo:::and::foo", for example, yields the following
     * results with these parameters:
     *
     * <table>
     * <caption>Split example showing regex, limit, and result</caption>
     * <thead>
     * <tr>
     *     <th>Regex</th>
     *     <th>Limit</th>
     *     <th>Result</th>
     * </tr>
     * </thead>
     * <tbody>
     * <tr><th>:+</th>
     *     <th>2</th>
     *     <td>{ "boo", ":::", "and::foo" }</td></tr>
     * <tr><!-- : -->
     *     <th>5</th>
     *     <td>{ "boo", ":::", "and", "::", "foo" }</td></tr>
     * <tr><!-- : -->
     *     <th>-1</th>
     *     <td>{ "boo", ":::", "and", "::", "foo" }</td></tr>
     * <tr><th>o</th>
     *     <th>5</th>
     *     <td>{ "b", "o", "", "o", ":::and::f", "o", "", "o", "" }</td></tr>
     * <tr><!-- o -->
     *     <th>-1</th>
     *     <td>{ "b", "o", "", "o", ":::and::f", "o", "", "o", "" }</td></tr>
     * <tr><!-- o -->
     *     <th>0</th>
     *     <td>{ "b", "o", "", "o", ":::and::f", "o", "", "o" }</td></tr>
     * </tbody>
     * </table>
     *
     * @note An invocation of this method of the form
     * @code
     *   str.splitWithDelimiters(regex, n)
     * @endcode
     * yields the same result as the expression
     *
     * @code
     *   Pattern::compile(regex).splitWithDelimiters(str, n)
     * @endcode
     *
     * @param  regex
     *         the delimiting regular expression
     *
     * @param  limit
     *         the result threshold, as described above
     *
     * @return  the array of strings computed by splitting this string
     *          around matches of the given regular expression, alternating
     *          substrings and matching delimiters
     */
    StringArray splitWithDelimiters(String const& regex, gint limit) const;

    /**
     * Splits this string around matches of the given regular expression.
     *
     * <p>
     * This method works as if by invoking the two-argument
     * @b split method with the given expression and a limit
     * argument of zero.  Trailing empty strings are therefore not included in
     * the resulting array.
     * </p>
     * <p>
     * The string @c "boo:and:foo", for example, yields the following
     * results with these expressions:
     *
     * <table>
     * <caption>Split examples showing regex and result</caption>
     * <thead>
     * <tr>
     *  <th>Regex</th>
     *  <th>Result</th>
     * </tr>
     * </thead>
     * <tbody>
     * <tr><th scope="row" style="font-weight:normal">:</th>
     *     <td>{ "boo", "and", "foo" }</td></tr>
     * <tr><th scope="row" style="font-weight:normal">o</th>
     *     <td>{ "b", "", ":and:f" }</td></tr>
     * </tbody>
     * </table>
     *
     * @param  regex
     *         the delimiting regular expression
     *
     * @return  the array of strings computed by splitting this string
     *          around matches of the given regular expression
     *
     * @throws  PatternSyntaxException
     *          if the regular expression's syntax is invalid
     *
     * @see regex::Pattern
     */
    StringArray split(String const& regex) const;

    /**
     * Returns a new String composed of copies of the
     * @c CharSequence elements joined together with a copy of
     * the specified @c delimiter.
     *
     * For example,
     * @code
     *     String message = String.join("-", "Java", "is", "cool");
     *     // message returned is: "Java-is-cool"
     * @endcode
     *
     * @param  delimiter the delimiter that separates each element
     * @param  elements the elements to join together.
     *
     * @return a new @c String that is composed of the @c elements
     *         separated by the @c delimiter
     *
     * @seeutil::StringJoiner
     */
    static String join(CharSequence const& delimiter, Array<CharSequence> const& elements);

    static String join(CharSequence const& delimiter, StringArray const& elements);

    /**
     * Returns a new @c String composed of copies of the
     * @c CharSequence elements joined together with a copy of the
     * specified @c delimiter.
     *
     * For example,
     * @code
     *     auto &strings = List<String>::of("My", "son", "is", "cool");
     *     String message = String.join(" ", strings);
     *     // message returned is: "My son is cool"
     *
     *     auto strings = LinkedHashSet<String>(List<String>::of("Paul", "is", "very", "cool"));
     *     String message = String.join("-", strings);
     *     // message returned is: "Java-is-very-cool"
     * @endcode
     *
     * @param  delimiter a sequence of characters that is used to separate each
     *         of the @c elements in the resulting @c String
     * @param  elements an @c Iterable that will have its @c elements
     *         joined together.
     *
     * @return a new @c String that is composed of the @c elements
     *         argument
     *
     * @see   util::StringJoiner
     */
    static String join(CharSequence const& delimiter, Iterable<CharSequence> const& elements);

    static String join(CharSequence const& delimiter, Iterable<String> const& elements);

    /**
     * Converts all the characters in this @c String to lower
     * case using the rules of the given @c Locale.  Case mapping is based
     * on the Unicode Standard version specified by the @b Character
     * class. Since case mappings are not always 1:1 char mappings, the resulting @c String
     * and this @c String may differ in length.
     * <p>
     *
     * @param locale use the case transformation rules for this locale
     * @return the @c String, converted to lowercase.
     * @see String::toLowerCase()
     * @see String::toUpperCase()
     * @see String::toUpperCase(Locale)
     */
    String toLowerCase(util::Locale const& locale) const;

    /**
     * Converts all the characters in this @c String to lower
     * case using the rules of the default locale. This method is equivalent to
     * @c toLowerCase(Locale::getDefault()).
     * <p>
     * @c Note: This method is locale sensitive, and may produce unexpected
     * results if used for strings that are intended to be interpreted locale
     * independently.
     * Examples are programming language identifiers, protocol keys, and HTML
     * tags.
     * For instance, @c "TITLE"_S.toLowerCase() in a Turkish locale
     * returns @c "t\u0131tle", where @c '\u0131' is the
     * LATIN SMALL LETTER DOTLESS I character.
     * To obtain correct results for locale insensitive strings, use
     * @c toLowerCase(Locale::ROOT).
     * </p>
     * @return  the @c String, converted to lowercase.
     */
    String toLowerCase() const;

    /**
     * Converts all the characters in this @c String to upper
     * case using the rules of the given @c Locale. Case mapping is based
     * on the Unicode Standard version specified by the @b Character
     * class. Since case mappings are not always 1:1 char mappings, the resulting @c String
     * and this @c String may differ in length.
     * <p>
     *
     * @param locale use the case transformation rules for this locale
     * @return the @c String, converted to uppercase.
     * @see  String::toUpperCase()
     * @see  String::toLowerCase()
     * @see  String::toLowerCase(Locale)
     */
    String toUpperCase(util::Locale const& locale) const;

    /**
     * Converts all the characters in this @c String to upper
     * case using the rules of the default locale. This method is equivalent to
     * @c toUpperCase(Locale::getDefault()).
     *
     * @note This method is locale sensitive, and may produce unexpected
     * results if used for strings that are intended to be interpreted locale
     * independently.
     * Examples are programming language identifiers, protocol keys, and HTML
     * tags.
     * For instance, @c "title".toUpperCase() in a Turkish locale
     * returns @c "T\u0130TLE", where @c '\u0130' is the
     * LATIN CAPITAL LETTER I WITH DOT ABOVE character.
     * To obtain correct results for locale insensitive strings, use
     * @code toUpperCase(Locale::ROOT) @endcode .
     *
     * @return  the @c String, converted to uppercase.
     * @see   String::toUpperCase(Locale)
     */
    String toUpperCase() const;

    /**
     * Returns a string whose value is this string, with all leading
     * and trailing space removed, where space is defined
     * as any character whose codepoint is less than or equal to
     * @c 'U+0020' (the space character).
     * <p>
     * If this @c String object represents an empty character
     * sequence, or the first and last characters of character sequence
     * represented by this @c String object both have codes
     * that are not space (as defined above), then a
     * reference to this @c String object is returned.
     * </p>
     * <p>
     * Otherwise, if all characters in this string are space (as
     * defined above), then a  @c String object representing an
     * empty string is returned.
     * </p>
     * <p>
     * Otherwise, let <i>k</i> be the index of the first character in the
     * string whose code is not a space (as defined above) and let
     * <i>m</i> be the index of the last character in the string whose code
     * is not a space (as defined above). A @c String
     * object is returned, representing the substring of this string that
     * begins with the character at index <i>k</i> and ends with the
     * character at index <i>m</i>-that is, the result of
     * @c this.subString(k, m + 1).
     * </p>
     * <p>
     * This method may be used to trim space (as defined above) from
     * the beginning and end of a string.
     * </p>
     *
     * @return  a string whose value is this string, with all leading
     *          and trailing space removed, or this string if it
     *          has no leading or trailing space.
     */
    String trim() const;

    /**
     * Returns a string whose value is this string, with all leading
     * and trailing @a whitespace removed.
     * <p>
     * If this @c String object represents an empty string,
     * or if all code points in this string are
     * @a whitespace, then an empty string
     * is returned.
     * </p>
     * <p>
     * Otherwise, returns a substring of this string beginning with the first
     * code point that is not a @a whitespace
     * up to and including the last code point that is not a
     * @a whitespace.
     * </p>
     * <p>
     * This method may be used to strip
     * @a whitespace from
     * the beginning and end of a string.
     * </p>
     * @return  a string whose value is this string, with all leading
     *          and trailing whitespace removed
     */
    String strip() const;

    /**
     * Returns a string whose value is this string, with all leading
     * @a whitespace removed.
     * <p>
     * If this @c String object represents an empty string,
     * or if all code points in this string are
     * @a whitespace, then an empty string
     * is returned.
     * </p>
     * <p>
     * Otherwise, returns a substring of this string beginning with the first
     * code point that is not a @a whitespace
     * up to and including the last code point of this string.
     * </p>
     * <p>
     * This method may be used to trim
     * @a whitespace from
     * the beginning of a string.
     * </p>
     *
     * @return  a string whose value is this string, with all leading white
     *          space removed
     */
    String stripLeading() const;

    /**
     * Returns a string whose value is this string, with all trailing
     * @a whitespace removed.
     * <p>
     * If this @c String object represents an empty string,
     * or if all characters in this string are
     * @a whitespace, then an empty string is returned.
     * </p>
     * <p>
     * Otherwise, returns a substring of this string beginning with the first
     * code point of this string up to and including the last code point
     * that is not a @a whitespace.
     * </p>
     * <p>
     * This method may be used to trim
     * @a whitespace from the end of a string.
     * </p>
     *
     * @return  a string whose value is this string, with all trailing white
     *          space removed
     */
    String stripTrailing() const;

    /**
     * Returns @c true if the string is empty or contains only
     * @a whitespace codepoints,
     * otherwise @c false.
     *
     * @return @c true if the string is empty or contains only
     *         @a whitespace codepoints,
     *         otherwise @c false
     */
    gbool isBlank() const;

    /**
     * Returns a string whose value is this string, with escape sequences
     * translated as if in a string literal.
     * <p>
     * Escape sequences are translated as follows; <br/>
     * ----------------------------------------------------------------- <br/>
     *                              Translation                          <br/>
     * ----------------------------------------------------------------- <br/>
     * | Escape             | Name            | Translation | Unicode    <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\b"           | backspace       | @c '\b'     | U+0008     <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\t"           | horizontal tab  | @c '\t'     | U+0009     <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\n"           | line feed       | @c '\n'     | U+000A     <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\f"           | form feed       | @c '\f'     | U+000C     <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\r"           | carriage return | @c '\r'     | U+000D     <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\s"           | space           | @c ' '      | U+0020     <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\""           | double quote    | @c '\"'    | U+0022      <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\'"           | single quote    | @c '\''    | U+0027      <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\\"          | backslash       | @c '\'    | U+005C      <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\\"          | backslash       | @c '\'    | U+005C      <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\0" ~ @c "\377"| Octal escape    | code point |             <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\xh..h"        | Hex escape      | code point | U+h..h      <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\uhhhh"        | Utf16 escape    | code point | U+hhhh      <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\uhhhhhhhh"    | Utf32 escape    | code point | U+hhhhhhhh  <br/>
     * ----------------------------------------------------------------- <br/>
     * | @c "\"           | Continuation     | discard    |             <br/>
     * ----------------------------------------------------------------- <br/>
     * </p>
     *
     * @throws IllegalArgumentException when an escape sequence is malformed.
     *
     * @return String with escape sequences translated.
     */
    String translateEscapes() const;

    /**
     * This object (which is already a string!) is itself returned.
     *
     * @return  the string itself.
     */
    String toString() const override;

    /**
     * Converts this string to a new character array.
     *
     * @return  a newly allocated character array whose length is the length
     *          of this string and whose contents are initialized to contain
     *          the character sequence represented by this string.
     */
    CharArray toChars() const;

    static String join(String const& delimiter, StringArray const& args);

    /**
     * Returns a formatted string using the specified format string and
     * arguments.
     *
     * <p>
     * The locale always used is the one returned by
     * @c Locale::getDefault(Locale::Category) with
     * @b Locale::Category::FORMAT category specified.
     * </p>
     *
     *
     * @note the format specified for general, character, and numeric types
     *       have the following syntax:
     *       @code
     *       %{index$}{flags}{width}{.precision}conversion
     *       @endcode
     *       @li index$ (optional) represent the position of argument from 1$ to n$
     *           (where n is number of arguments).
     *       @li flags (optional) is the set of character that modify the output. The set
     *           of valid flags depends on conversion.
     *           - '<' previous argument <br>
     *           - '-' Lef justify <br>
     *           - '#' Alternate form <br>
     *           - '+' Signed form <br>
     *           - ' ' Single extra space (U+0020) <br>
     *           - '0' Leading Zero Padding <br>
     *           - ',' localized Group separator <br>
     *           - '(' Prepend '(' and append ')' to negative <br>
     *       @li width (optional, positive) represent the minimum number of characters to be written
     *                  to output
     *       @li precision (optional, positive) represent the number of character in decimal part
     *       @li conversion (required):
     *           - 'a' Floating point in hexadecimal format
     *           - 'A' Floating point in hexadecimal format (uppercase)
     *           - 'b' Boolean
     *           - 'B' Boolean (uppercase)
     *           - 'c' Unicode character
     *           - 'C' Unicode character (uppercase)
     *           - 'd' Integer in decimal format
     *           - 'e' Floating point in scientific format
     *           - 'E' Floating point in scientific format (uppercase)
     *           - 'f' Floating point in decimal format
     *           - 'g' Floating point in decimal/scientific format.
     *           - 'G' Floating point in decimal/scientific format (uppercase)
     *           - 'h' Hash code
     *           - 'H' Hash code (uppercase)
     *           - 'n' Line break
     *           - 'o' Integer in octal format
     *           - '%' Percent symbol
     *           - 't' Date or Time prefix. it used with Date/Time conversions.
     *
     *       @li Date or Time Conversion <br>
     *          - 'H' hour of day for 24-hour clock, formatted as two digits
     *               with leading zero as necessary (00 - 23) <br>
     *          - 'I' hour for 12-hour clock, formatted as two digits
     *               with leading zero as necessary (01 - 12) <br>
     *          - 'k' hour for 24-hour clock  (0 - 23) <br>
     *          - 'l' hour for 12-hour clock  (1 - 12) <br>
     *          - 'M' Minute within the hour, formatted as two digits
     *               with leading zero as necessary (00 - 59) <br>
     *          - 'S' Seconds within the minute, formatted as two digits
     *               with leading zero as necessary (00 - 60) <br>
     *          - 'L' Millisecond within the second, formatted as three digits
     *               with leading zero as necessary (000 - 999) <br>
     *          - 'N' Millisecond within the second, formatted as nine digits
     *               with leading zero as necessary (000000000 - 999999999) <br>
     *          - 'p' Locale specific morning or afternoon marked in lowercase ("am" or "pm") <br>
     *          - 'z' The RFC 822 style numeric timezone offset from GMT (eg: -0800). This
     *                  value will be adjusted as necessary for Daylight Saving time. <br>
     *          - 'Z' The abbreviation for timezone. This value will be adjusted
     *                  as necessary for Daylight Saving time. <br>
     *          - 's' The epoch seconds (since 1970/01/01 00:00:00 UTC) <br>
     *          - 'Q' The epoch milliseconds (since 1970/01/01 00:00:00 UTC) <br>
     *          - 'B' The locale specific full month name (eg: "January", "February") <br>
     *          - 'b' The locale specific abbreviated month name (eg: "Jan", "Feb") <br>
     *          - 'h' same has @c 'b' <br>
     *          - 'A' The locale specific full day of week name (eg: "Sunday", "Monday") <br>
     *          - 'a' The locale specific short day of week name (eg: "Sun", "Mon") <br>
     *          - 'C' Four-digit year divided by 100, formatted has two digits
     *                  with leading zero as necessary (00 - 99) <br>
     *          - 'Y' Year, formatted as at the least four digits with leading
     *                  zero as necessary (eg: 0092 for 92 CE in Gregorian calendar) <br>
     *          - 'y' Last two digits of year formatted with leading zero as necessary (00 - 99) <br>
     *          - 'j' day of year formatted as three digits with leading zero as necessary (001 - 366) <br>
     *          - 'm' Month, formatted as two digits with leading zero as necessary (01 - 13) <br>
     *          - 'd' Day of month, formatted as two digits with leading zero as necessary (01 - 31) <br>
     *          - 'e' Day of month, formatted as two digits (1 - 31) <br>
     *          - 'R' Time formatted for 24-hour clock as "%tH:%tM" <br>
     *          - 'T' Time formatted for 24-hour clock as "%tH:%tM:%tS" <br>
     *          - 'r' Time formatted for 12-hour clock as "%tI:%tM:%tS %Tp". The location of
     *                  the morning or afternoon marker (@c '%Tp') may be locale dependent <br>
     *          - 'D' The Date formatted as "%tm/%td/%ty <br>
     *          - 'F' The ISO 8601 complete date formatted as "%tY-%tm-%td" <br>
     *          - 'c' The Date and time formatted as "%ta %tb %td %tT %tZ %tY"
     *                  (eg: @c "Sun Jul 20 16:17:00 EDT 1969"
     *
     * @param  format A format string
     *
     * @param  args
     *         Arguments referenced by the format specifiers in the format
     *         string.  If there are more arguments than format specifiers, the
     *         extra arguments are ignored.  The number of arguments is
     *         variable and may be zero.  The maximum number of arguments is
     *         limited by the compiler. The behaviour on a @c null argument
     *         depends on the conversion.
     *
     * @throws  IllegalFormatException
     *          If a format string contains an illegal syntax, a format
     *          specifier that is incompatible with the given arguments,
     *          insufficient arguments given the format string, or other
     *          illegal conditions.  For specification of all possible
     *          formatting errors, see the Details section of the
     *          formatter class specification.
     *
     * @return  A formatted string
     */
    template <class... Args>
    static String format(String const& format, Args const&... args) {
      Object& fmt = formatter(format, (gint) sizeof...(args));
      gbool _[] = {formatArg(fmt, args)...};
      finalize(fmt);
      CORE_IGNORE(_);
      return fmt.toString();
    }

    /**
     * Returns a formatted string using the specified locale, format string,
     * and arguments.
     *
     * @note the format specified for general, character, and numeric types
     *       have the following syntax:
     *       @code
     *       %{index$}{flags}{width}{.precision}conversion
     *       @endcode
     *       @li index$ (optional) represent the position of argument from 1$ to n$
     *           (where n is number of arguments).
     *       @li flags (optional) is the set of character that modify the output. The set
     *           of valid flags depends on conversion.
     *           - '<' previous argument <br>
     *           - '-' Lef justify <br>
     *           - '#' Alternate form <br>
     *           - '+' Signed form <br>
     *           - ' ' Single extra space (U+0020) <br>
     *           - '0' Leading Zero Padding <br>
     *           - ',' localized Group separator <br>
     *           - '(' Prepend '(' and append ')' to negative <br>
     *       @li width (optional, positive) represent the minimum number of characters to be written
     *                  to output
     *       @li precision (optional, positive) represent the number of character in decimal part
     *       @li conversion (required):
     *           - 'a' Floating point in hexadecimal format
     *           - 'A' Floating point in hexadecimal format (uppercase)
     *           - 'b' Boolean
     *           - 'B' Boolean (uppercase)
     *           - 'c' Unicode character
     *           - 'C' Unicode character (uppercase)
     *           - 'd' Integer in decimal format
     *           - 'e' Floating point in scientific format
     *           - 'E' Floating point in scientific format (uppercase)
     *           - 'f' Floating point in decimal format
     *           - 'g' Floating point in decimal/scientific format.
     *           - 'G' Floating point in decimal/scientific format (uppercase)
     *           - 'h' Hash code
     *           - 'H' Hash code (uppercase)
     *           - 'n' Line break
     *           - 'o' Integer in octal format
     *           - '%' Percent symbol
     *           - 't' Date or Time prefix. it used with Date/Time conversions.
     *
     *       @li Date or Time Conversion <br>
     *          - 'H' hour of day for 24-hour clock, formatted as two digits
     *               with leading zero as necessary (00 - 23) <br>
     *          - 'I' hour for 12-hour clock, formatted as two digits
     *               with leading zero as necessary (01 - 12) <br>
     *          - 'k' hour for 24-hour clock  (0 - 23) <br>
     *          - 'l' hour for 12-hour clock  (1 - 12) <br>
     *          - 'M' Minute within the hour, formatted as two digits
     *               with leading zero as necessary (00 - 59) <br>
     *          - 'S' Seconds within the minute, formatted as two digits
     *               with leading zero as necessary (00 - 60) <br>
     *          - 'L' Millisecond within the second, formatted as three digits
     *               with leading zero as necessary (000 - 999) <br>
     *          - 'N' Millisecond within the second, formatted as nine digits
     *               with leading zero as necessary (000000000 - 999999999) <br>
     *          - 'p' Locale specific morning or afternoon marked in lowercase ("am" or "pm") <br>
     *          - 'z' The RFC 822 style numeric timezone offset from GMT (eg: -0800). This
     *                  value will be adjusted as necessary for Daylight Saving time. <br>
     *          - 'Z' The abbreviation for timezone. This value will be adjusted
     *                  as necessary for Daylight Saving time. <br>
     *          - 's' The epoch seconds (since 1970/01/01 00:00:00 UTC) <br>
     *          - 'Q' The epoch milliseconds (since 1970/01/01 00:00:00 UTC) <br>
     *          - 'B' The locale specific full month name (eg: "January", "February") <br>
     *          - 'b' The locale specific abbreviated month name (eg: "Jan", "Feb") <br>
     *          - 'h' same has @c 'b' <br>
     *          - 'A' The locale specific full day of week name (eg: "Sunday", "Monday") <br>
     *          - 'a' The locale specific short day of week name (eg: "Sun", "Mon") <br>
     *          - 'C' Four-digit year divided by 100, formatted has two digits
     *                  with leading zero as necessary (00 - 99) <br>
     *          - 'Y' Year, formatted as at the least four digits with leading
     *                  zero as necessary (eg: 0092 for 92 CE in Gregorian calendar) <br>
     *          - 'y' Last two digits of year formatted with leading zero as necessary (00 - 99) <br>
     *          - 'j' day of year formatted as three digits with leading zero as necessary (001 - 366) <br>
     *          - 'm' Month, formatted as two digits with leading zero as necessary (01 - 13) <br>
     *          - 'd' Day of month, formatted as two digits with leading zero as necessary (01 - 31) <br>
     *          - 'e' Day of month, formatted as two digits (1 - 31) <br>
     *          - 'R' Time formatted for 24-hour clock as "%tH:%tM" <br>
     *          - 'T' Time formatted for 24-hour clock as "%tH:%tM:%tS" <br>
     *          - 'r' Time formatted for 12-hour clock as "%tI:%tM:%tS %Tp". The location of
     *                  the morning or afternoon marker (@c '%Tp') may be locale dependent <br>
     *          - 'D' The Date formatted as "%tm/%td/%ty <br>
     *          - 'F' The ISO 8601 complete date formatted as "%tY-%tm-%td" <br>
     *          - 'c' The Date and time formatted as "%ta %tb %td %tT %tZ %tY"
     *                  (eg: @c "Sun Jul 20 16:17:00 EDT 1969"
     *
     * @param  locale
     *         The @em locale to apply during
     *         formatting.
     *
     * @param  format A format string
     *
     * @param  args
     *         Arguments referenced by the format specifiers in the format
     *         string.  If there are more arguments than format specifiers, the
     *         extra arguments are ignored.  The number of arguments is
     *         variable and may be zero.  The maximum number of arguments is
     *         limited by the compiler. The behaviour on a @c null argument
     *         depends on the conversion.
     *
     * @throws  IllegalFormatException
     *          If a format string contains an illegal syntax, a format
     *          specifier that is incompatible with the given arguments,
     *          insufficient arguments given the format string, or other
     *          illegal conditions.  For specification of all possible
     *          formatting errors, see the Details section of the
     *          formatter class specification
     *
     * @return  A formatted string
     */
    template <class... Args>
    static String format(util::Locale const& locale, String const& format, Args const&... args) {
      Object& fmt = formatter(locale, format, (gint) sizeof...(args));
      gbool _[] = {formatArg(fmt, args)...};
      finalize(fmt);
      CORE_IGNORE(_);
      return fmt.toString();
    }

    /**
     * Formats using this string as the format string, and the supplied
     * arguments.
     *
     * @implSpec This method is equivalent to @c String.format(this, args).
     *
     * @param  args
     *         Arguments referenced by the format specifiers in this string.
     *
     * @return  A formatted string
     */
    template <class... Args>
    String formatted(Args const&... args) const {
      Object& fmt = formatter(*this, (gint) sizeof...(args));
      gbool _[] = {formatArg(fmt, args)...};
      finalize(fmt);
      CORE_IGNORE(_);
      return fmt.toString();
    }

    /**
     * Returns the string representation of the @c Object argument.
     *
     * @param   obj   an @c Object.
     * @return  if the argument is @c null, then a string equal to
     *          @c "null"; otherwise, the value of
     *          @c obj::toString() is returned.
     */
    static String valueOf(Object const& obj);

    /**
     * Returns the string representation of the @c char array
     * argument. The contents of the character array are copied; subsequent
     * modification of the character array does not affect the returned
     * string.
     *
     * @param   data     the character array.
     * @return  a @c String that contains the characters of the
     *          character array.
     */
    static String valueOf(CharArray const& data);

    /**
     * Returns the string representation of a specific subarray of the
     * @c char array argument.
     * <p>
     * The @c offset argument is the index of the first
     * character of the subarray. The @c count argument
     * specifies the length of the subarray. The contents of the subarray
     * are copied; subsequent modification of the character array does not
     * affect the returned string.
     * </p>
     *
     * @param   data     the character array.
     * @param   offset   initial offset of the subarray.
     * @param   count    length of the subarray.
     * @return  a @c String that contains the characters of the
     *          specified subarray of the character array.
     * @throws    IndexOutOfBoundsException if @c offset is
     *          negative, or @c count is negative, or
     *          @c offset+count is larger than
     *          @c data.length.
     */
    static String valueOf(CharArray const& data, gint offset, gint count);

    /**
     * Returns the string representation of the @c boolean argument.
     *
     * @param   b   a @c boolean.
     * @return  if the argument is @c true, a string equal to
     *          @c "true" is returned; otherwise, a string equal to
     *          @c "false" is returned.
     */
    static String valueOf(gbool b);

    /**
     * Returns the string representation of the @c char
     * argument.
     *
     * @param   c   a @c char.
     * @return  a string of length @c 1 containing
     *          as its single character the argument @c c.
     */
    static String valueOf(gchar c);

    /**
     * Returns the string representation of the @c int argument.
     * <p>
     * The representation is exactly the one returned by the
     * @c Integer::toString method of one argument.
     * </p>
     *
     * @param   i   an @c int.
     * @return  a string representation of the @c int argument.
     */
    static String valueOf(gint i);

    /**
     * Returns the string representation of the @c long argument.
     * <p>
     * The representation is exactly the one returned by the
     * @c Long::toString method of one argument.
     * </p>
     *
     * @param   l   a @c long.
     * @return  a string representation of the @c long argument.
     */
    static String valueOf(glong l);

    /**
     * Returns the string representation of the @c float argument.
     * <p>
     * The representation is exactly the one returned by the
     * @c Float::toString method of one argument.
     * </p>
     * @param   f   a @c float.
     * @return  a string representation of the @c float argument.
     */
    static String valueOf(gfloat f);

    /**
     * Returns the string representation of the @c double argument.
     * <p>
     * The representation is exactly the one returned by the
     * @c Double::toString method of one argument.
     * </p>
     * @param   d   a @c double.
     * @return  a  string representation of the @c double argument.
     */
    static String valueOf(gdouble d);

    /**
     * Returns the string representation of the @c enum argument.
     * <p>
     * The representation is exactly the one returned by the
     * @c Enum<?>::toString method of one argument.
     * </p>
     * @tparam E The type of given enum value
     * @param   value   a @c enum value.
     * @return  a  string representation of the @c enum argument.
     */
    template <class E, ClassOf(1)::OnlyIf<Class<E>::isEnum()> = 1>
    static String valueOf(E value) {
      CORE_ALIAS(Clazz, typename Class<E>::Object);
      return String::valueOf((Clazz)value);
    }

    /**
     * Returns a string whose value is the concatenation of this
     * string repeated @c count times.
     * <p>
     * If this string is empty or count is zero then the empty
     * string is returned.
     * </p>
     * @param   count number of times to repeat
     *
     * @return  A string composed of this string repeated
     *          @c count times or the empty string if this
     *          string is empty or count is zero
     *
     * @throws  IllegalArgumentException if the @c count is
     *          negative.
     *
     *
     */
    String repeat(gint count) const;

    Object& clone() const override;

    ~String() override;

    /**
     * A Comparator that orders @c String objects as by
     * @b compareToIgnoreCase.
     * This comparator is serializable.
     * <p>
     * Note that this Comparator does <em>not</em> take locale into account,
     * and will result in an unsatisfactory ordering for certain locales.
     * The @b text::Collator class provides locale-sensitive comparison.
     *
     * @see     text::Collator
     */
    static util::Comparator<String> const& CASE_INSENSITIVE_ORDER;

  private:
    Coder coding() const;

    StringArray split(String const& regex, gint limit, gbool withDelimiters) const;

    StringArray split(gchar ch, gint limit, gbool withDelimiters) const;

    static String join(String const& delimiter,
                       String const& prefix,
                       String const& suffix,
                       StringArray const& elements,
                       gint nbUsed);

    /* ::::::::::::::::::::::::: UTF-8 :::::::::::::::::::::::::::::::: */

    static CharArray decodeUTF8Literal(glong str, glong est);

    ByteArray encodeLatin1ToUTF8(gint off, gint len) const;

    ByteArray encodeUtf16ToUTF8(gint off, gint len) const;

    /* ::::::::::::::::::::::: ISO-8859-1 :::::::::::::::::::::::::: */

    ByteArray encodeLatin1ToISO8853_1(gint off, gint len, gbool doReplace) const;

    ByteArray encodeUTF16ToISO8853_1(gint off, gint len, gbool doReplace) const;

    /* ::::::::::::::::::::::: ASCII :::::::::::::::::::::::::: */

    ByteArray encodeLatin1ToASCII(gint off, gint len) const;

    ByteArray encodeUTF16ToASCII(gint off, gint len) const;

    /* ::::::::::::::::::::::: ????? :::::::::::::::::::::::::: */

    void initStringFromLiteral(glong str, glong est, gint bpc);

    template <class T, ClassOf(1)::OnlyIf<Class<T>::isPointer()>  = 1>
    static CORE_FAST gbool isNullPointer(T&& ptr) { return ptr == null; }

    template <class T, ClassOf(1)::OnlyIf<!Class<T>::isPointer()>  = 1>
    static CORE_FAST gbool isNullPointer(T&& ptr) { return false; }

    /* :::::::::::::::::::::::  Formatter  :::::::::::::::::::::::::: */

    static Object& formatter(String const& fmt, gint nArgs);

    static Object& formatter(util::Locale const& locale, String const& fmt, gint nArgs);

    // %b
    static gbool formatArg(Object& fmt, gbool arg);

    // %c
    static gbool formatArg(Object& fmt, gchar arg);

    // %d %c
    static gbool formatArg(Object& fmt, gbyte arg);

    // %d %c
    static gbool formatArg(Object& fmt, gshort arg);

    // %d %c
    static gbool formatArg(Object& fmt, gint arg);

    // %d
    static gbool formatArg(Object& fmt, glong arg);

    // %d %c
    static gbool formatUnsignedArg(Object& fmt, gbyte arg);

    // %d %c
    static gbool formatUnsignedArg(Object& fmt, gshort arg);

    // %d %c
    static gbool formatUnsignedArg(Object& fmt, gint arg);

    // %d
    static gbool formatUnsignedArg(Object& fmt, glong arg);

    // %f %e %g
    static gbool formatArg(Object& fmt, gfloat arg);

    // %f %e %g
    static gbool formatArg(Object& fmt, gdouble arg);

    // %s
    static gbool formatStringArg(Object& fmt, String arg);

    // %s
    template <class Str, ClassOf(1)::OnlyIf<Class<Str>::isString()>  = 1>
    static gbool formatArg(Object& fmt, Str const& arg) {
      return formatStringArg(fmt, String(arg));
    }

    // %c
    template <class Chr, ClassOf(1)::OnlyIf<Class<Chr>::isCharacter()>  = 1>
    static gbool formatArg(Object& fmt, Chr const& arg) {
      if (ClassOf(arg)::size() == 1) { // byte
        if (arg > Byte::MAX_VALUE)
          return formatUnsignedArg(fmt, (gbyte) arg);
        return formatArg(fmt, (gbyte) arg);
      }
      if (ClassOf(arg)::size() == 2) // utf16 char
        return formatArg(fmt, (gchar) arg);
      return formatArg(fmt, (gint) arg); // utf32 char
    }

    // %d
    template <class Int, ClassOf(1)::OnlyIf<Class<Int>::Integer()>  = 1>
    static gbool formatArg(Object& fmt, Int const& arg) {
      if (ClassOf(arg)::size() == 1) { // int 8
        if (arg > Byte::MAX_VALUE)
          return formatUnsignedArg(fmt, (gbyte) arg);
        return formatArg(fmt, (gbyte) arg);
      }
      if (ClassOf(arg)::size() == 2) { // int 16
        if (arg > 0x7FFF)
          return formatUnsignedArg(fmt, (gshort) arg);
        return formatArg(fmt, (gshort) arg);
      }
      if (ClassOf(arg)::size() == 4) { // int 32
        if (arg > 0x7FFFFFFF) // unsigned int 32
          return formatUnsignedArg(fmt, (gint) arg);
        return formatArg(fmt, (gint) arg);
      }
      if (ClassOf(arg)::size() == 8) {
        if (arg > 0x7FFFFFFFFFFFFFFF) // unsigned int 64
          return formatUnsignedArg(fmt, (glong) arg);
        return formatArg(fmt, (glong) arg); // int 64
      } // int 128 is not supported.
      if (arg >= 0 && arg <= 0xFFFFFFFFFFFFFFFF)
        return formatUnsignedArg(fmt, (glong) arg);
      return formatArg(fmt, (glong) arg);
    }

    // %f
    template <class Flt, ClassOf(1)::OnlyIf<Class<Flt>::isFloating()>  = 1>
    static gbool formatArg(Object& fmt, Flt const& arg) {
      if (ClassOf(arg)::size() == 1) // float 8 => float 32
        return formatArg(fmt, (gfloat) arg);
      if (ClassOf(arg)::size() == 2) // float 16 => float32
        return formatArg(fmt, (gfloat) arg);
      if (ClassOf(arg)::size() == 4) // float 32
        return formatArg(fmt, (gfloat) arg);
      if (ClassOf(arg)::size() == 8) // float 64
        return formatArg(fmt, (gdouble) arg); // int 64
      // float 128 is not supported (we'll convert it to float64).
      return formatArg(fmt, (gdouble) arg); // int 64
    }

    // %b
    template <class Bool, ClassOf(1)::OnlyIf<Class<Bool>::isBoolean()>  = 1>
    static gbool formatArg(Object& fmt, Bool const& arg) {
      return formatArg(fmt, (gbool) arg);
    }

    // %s (require #include <core/lang/Enum.h>)
    template <class E, ClassOf(1)::OnlyIf<Class<E>::isEnum()>  = 1>
    static gbool formatArg(Object& fmt, E const& arg) {
      CORE_ALIAS(Clazz, typename Class<E>::Object);
      return formatArg(fmt, (Clazz) arg);
    }

    // %s %h
    static gbool formatArg(Object& fmt, Object const& arg);

    // ???
    static gbool finalize(Object& fmt);
  };

  /* ::::::::::::::::::::::: String Concatenation Helpers :::::::::::::::::::::::::: */
  inline namespace literals {
    template <class T, ClassOf(1)::OnlyIf<Class<T>::isString()> CaptureString = 1>
    String operator+(String const& lhs, T&& rhs) {
      return lhs.concat(rhs);
    }

    template <class T, ClassOf(1)::OnlyIf<Class<T>::isString()> CaptureString = 1>
    String operator+(T&& lhs, String const& rhs) { return String(lhs).concat(rhs); }

    template <class T, ClassOf(1)::OnlyIf<Class<T>::isString()> CaptureString = 1>
    String& operator+=(String& lhs, T&& rhs) { return lhs = lhs.concat(rhs); }

    template <class E, ClassOf(1)::OnlyIf<Class<E>::isEnum()> CaptureEnum = 1>
    String operator+(E&& lhs, String const& rhs) {
      CORE_ALIAS(Clazz, typename Class<E>::Object);
      return ((Clazz) lhs) + rhs;
    }

    template <class E, ClassOf(1)::OnlyIf<Class<E>::isEnum()> CaptureEnum = 1>
    String& operator+=(String& lhs, E&& rhs) {
      CORE_ALIAS(Clazz, typename Class<E>::Object);
      return lhs + ((Clazz) rhs);
    }
  }
} // core

#endif // CORE24_STRING_H
