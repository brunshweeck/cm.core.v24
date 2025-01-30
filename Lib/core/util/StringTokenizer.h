//
// Created by brunshweeck on 17 sept. 2024.
//

#ifndef CORE24_STRINGTOKENIZER_H
#define CORE24_STRINGTOKENIZER_H

#include <core/String.h>

namespace core {
    namespace util {
        /**
         * The string tokenizer class allows an application to break a
         * string into tokens. The tokenization method is much simpler than
         * the one used by the @c StreamTokenizer class. The
         * @c StringTokenizer methods do not distinguish among
         * identifiers, numbers, and quoted strings, nor do they recognize
         * and skip comments.
         * <p>
         * The set of delimiters (the characters that separate tokens) may
         * be specified either at creation time or on a per-token basis.
         * </p>
         * <p>
         * An instance of @c StringTokenizer behaves in one of two
         * ways, depending on whether it was created with the
         * @c returnDelims flag having the value @c true
         * or @c false:
         *
         * @li If the flag is @c false, delimiter characters serve to
         *     separate tokens. A token is a maximal sequence of consecutive
         *     characters that are not delimiters.
         * @li If the flag is @c true, delimiter characters are themselves
         *     considered to be tokens. A token is thus either one delimiter
         *     character, or a maximal sequence of consecutive characters that are
         *     not delimiters.
         * </p>
         * <p>
         * A @c StringTokenizer object internally maintains a current
         * position within the string to be tokenized. Some operations advance this
         * current position past the characters processed.
         * </p>
         * <p>
         * A token is returned by taking a substring of the string that was used to
         * create the @c StringTokenizer object.
         * </p>
         * <p>
         * The following is one example of the use of the tokenizer. The code:
         * @code
         *     auto st = StringTokenizer("this is a test");
         *     while (st.hasMoreTokens())
         *         Console::out.println(st.nextToken());
         *
         * @endcode
         * </p>
         * <p>
         * prints the following output:
         * @code
         *     this
         *     is
         *     a
         *     test
         * @endcode
         *
         * </p>
         * <p>
         * @c StringTokenizer is a legacy class that is retained for
         * compatibility reasons although its use is discouraged in new code. It is
         * recommended that anyone seeking this functionality use the @c split
         * method of @c String or the java.util.regex package instead.
         * </p>
         * <p>
         * The following example illustrates how the @c String::split
         * method can be used to break up a string into its basic tokens:
         * @code
         *     auto result = "this is a test".split("\\s");
         *     for (String const &r : result)
         *         System.out.println(r);
         * @endcode
         * </p>
         * <p>
         * prints the following output:
         * @code
         *     this
         *     is
         *     a
         *     test
         * @endcode
         * </p>
         * @see     io.StreamTokenizer
         */
        class StringTokenizer final : public Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            gint currentPosition;
            gint mutable newPosition;
            gint maxPosition;
            String str;
            String delimiters;
            gbool retDelims;
            gbool delimsChanged;

            /**
             * maxDelim stores the value of the delimiter character with the
             * highest value. It is used to optimize the detection of delimiter
             * characters.
             *
             * It is unlikely to provide any optimization benefit in the
             * hasSurrogates case because most string characters will be
             * smaller than the limit, but we keep it so that the two code
             * paths remain similar.
             */
            gint maxDelimCodePoint = -1;

            /**
             * If delimiters include any surrogates (including surrogate
             * pairs), hasSurrogates is true and the tokenizer uses the
             * different code path. This is because String.indexOf(int)
             * doesn't handle unpaired surrogates as a single character.
             */
            gbool hasSurrogates = false;

            /**
             * When hasSurrogates is true, delimiters are converted to code
             * points and isDelimiter(int) is used to determine if the given
             * codepoint is a delimiter.
             */
            IntArray delimiterCodePoints;

        public:
            /**
             * Constructs a string tokenizer for the specified string. All
             * characters in the @c delim argument are the delimiters
             * for separating tokens.
             * <p>
             * If the @c returnDelims flag is @c true, then
             * the delimiter characters are also returned as tokens. Each
             * delimiter is returned as a string consisting of a single
             * <a href="../lang/Character.html#unicode">Unicode code point</a>
             * of the delimiter (which may be one or two @c chars). If the
             * flag is @c false, the delimiter characters are skipped
             * and only serve as separators between tokens.
             * </p>
             *
             * @param   str            a string to be parsed.
             * @param   delim          the delimiters.
             * @param   returnDelims   flag indicating whether to return the delimiters
             *                         as tokens.
             */
            CORE_EXPLICIT StringTokenizer(String str, String delim, gbool returnDelims);

            /**
             * Constructs a string tokenizer for the specified string. The
             * characters in the @c delim argument are the delimiters
             * for separating tokens. Delimiter characters themselves will not
             * be treated as tokens.
             *
             * @param   str     a string to be parsed.
             * @param   delim   the delimiters.
             */
            CORE_EXPLICIT StringTokenizer(String str, String delim);

            /**
             * Constructs a string tokenizer for the specified string. The
             * tokenizer uses the default delimiter set, which is
             * <code>"&nbsp;&#92;t&#92;n&#92;r&#92;f"</code>: the space character,
             * the tab character, the newline character, the carriage-return character,
             * and the form-feed character. Delimiter characters themselves will
             * not be treated as tokens.
             *
             * @param   str   a string to be parsed.
             */
            CORE_EXPLICIT StringTokenizer(String str);

            /**
             * Tests if there are more tokens available from this tokenizer is string.
             * If this method returns @c true, then a subsequent call to
             * @c nextToken with no argument will successfully return a token.
             *
             * @return  @c true if and only if there is at least one token
             *          in the string after the current position; @c false
             *          otherwise.
             */
            gbool hasMoreTokens() const;

            /**
             * Returns the next token from this string tokenizer.
             *
             * @return     the next token from this string tokenizer.
             * @throws     NoSuchElementException  if there are no more tokens in this
             *               tokenizer is string.
             */
            String nextToken();

            /**
             * Returns the next token in this string tokenizer is string. First,
             * the set of characters considered to be delimiters by this
             * @c StringTokenizer object is changed to be the characters in
             * the string @c delim. Then the next token in the string
             * after the current position is returned. The current position is
             * advanced beyond the recognized token.  The new delimiter set
             * remains the default after this call.
             *
             * @param      delim   the new delimiters.
             * @return     the next token, after switching to the new delimiter set.
             * @throws     NoSuchElementException  if there are no more tokens in this
             *               tokenizer is string.
             */
            String nextToken(String delim);

            /**
             * Calculates the number of times that this tokenizer is
             * @c nextToken method can be called before it generates an
             * exception. The current position is not advanced.
             *
             * @return  the number of tokens remaining in the string using the current
             *          delimiter set.
             * @see  StringTokenizer::nextToken()
             */
            gint countTokens() const;

            /**
             * Move this string tokenizer cursor to last returned token
             * and return true or false if operation is succeeded or failed.
             *
             * @return  true or false if operation is succeeded or failed.
             */
            gbool goBack();

        private:
            /**
             * Set maxDelimCodePoint to the highest char in the delimiter set.
             */
            void setMaxDelimiterCodePoint();

            /**
             * Skips delimiters starting from the specified position. If retDelims
             * is false, returns the index of the first non-delimiter character at or
             * after startPos. If retDelims is true, startPos is returned.
             */
            gint skipDelimiters(gint startPos) const;

            /**
             * Skips ahead from startPos and returns the index of the next delimiter
             * character encountered, or maxPosition if no such delimiter is found.
             */
            gint scanToken(gint startPos, gbool toBack) const;

            gbool isDelimiter(gint codePoint) const;
        };
    } // util
} // core

#endif //CORE24_STRINGTOKENIZER_H
