//
// Created by bruns on 13/10/2024.
//

#ifndef CORE24_PATTERN_H
#define CORE24_PATTERN_H

#include <core/util/Locale.h>

namespace core {
  namespace regex {
    /**
     * A compiled representation of a regular expression.
     *
     * <p>
     * A regular expression, specified as a string, must first be compiled into
     * an instance of this class.  The resulting pattern can then be used to create
     * a @b Matcher object that can match arbitrary
     * <em> character sequences </em> against the regular
     * expression.  All of the state involved in performing a match resides in the
     * matcher, so many matchers can share the same pattern.
     * </p>
     * <p>
     * A typical invocation sequence is thus
     *
     * @code
     *   auto p = Pattern::compile("a*b");
     *   auto m = p.matcher("aaaaab");
     *   auto b = m.matches();
     * @endcode
     * </p>
     * <p>
     * A @b matches method is defined by this class as a
     * convenience for when a regular expression is used just once.  This method
     * compiles an expression and matches an input sequence against it in a single
     * invocation.  The statement
     *
     * @code
     *   auto b = Pattern::matches("a*b", "aaaaab");
     * @endcode
     * is equivalent to the three statements above, though for repeated matches it
     * is less efficient since it does not allow the compiled pattern to be reused.
     * </p>
     * <h3>Summary of regular-expression constructs</h3>
     *
     * <b>Regular expression constructs, and what they match</b> <br>
     * <p>
     * @b Construct -> @b Matches
     * </p>
     * <p>
     * 1- Characters: <br/>
     * - x -> The character x <br/>
     * - @c "\\" -> The backslash character <br/>
     * - @c "\0x" -> The character with octal value @c 0x (0 <= x <= 7) <br/>
     * - @c "\0xx" -> The character with octal value @c 0xx (0 <= x <= 7) <br/>
     * - @c "\0yxx" -> The character with octal value @c 0yxx (0 <= y <= 3, 0 <= x <= 7) <br/>
     * - @c "\xhh" -> The character with hexadecimal value @c 0xhh <br/>
     * - @c "\uhhhh" -> The character with hexadecimal value @c 0xhhhh <br/>
     * - @c "\xh...h" -> The character with hexadecimal value @c 0xh...h (0 <= @c 0xh...h <= 0x10FFFF) <br/>
     * - @c "\N{name}" -> The character with Unicode character name <i>'name'</i> <br/>
     * - @c "\t" -> The tab character (@c '\u0009') <br/>
     * - @c "\n" -> The newline (line feed) character (@c '\u000A') <br/>
     * - @c "\r" -> The carriage-return character (@c '\u000D') <br/>
     * - @c "\f" -> The form-feed character (@c '\u000C') <br/>
     * - @c "\a" -> The alert (bell) character (@c '\u0007') <br/>
     * - @c "\e" -> The escape character (@c '\u001B') <br/>
     * - @c "\cx" -> The control character corresponding to x <br/>
     * </p>
     * <p>
     * 2- Character classes:
     * - @c "[abc]" -> @c a, @c b, or @c c (simple class) <br/>
     * - @c "[^abc]" -> Any character except @c a, @c b, or @c c (negation) <br/>
     * - @c "[a-zA-Z]" -> @c a through @c z or @c A through @c Z, inclusive (range) <br/>
     * - @c "[a-d[m-p]]" -> @c a through @c d, or @c m through @c p:  [a-dm-p] (union) <br/>
     * - @c "[a-z&&[def]]" -> @c d, @c e, or @c f (intersection) <br/>
     * - @c "[a-z&&[^bc]]" -> @c a through @c z, except for @c b and @c c: [ad-z] (subtraction) <br/>
     * - @c "[a-z&&[^m-p]]" -> @c a through @c z, and not @c m through @c p:  [a-lq-z] (subtraction) <br/>
     * </p>
     * <p>
     * 3- Predefined character classes <br/>
     * - @c "." -> Any character (may or may not match <a href="#lt">line terminators</a>) <br/>
     * - @c "\d" -> A digit:  [0-9] if UNICODE_CHARACTER_CLASS is not set. See Unicode Support. <br/>
     * - @c "\D" -> A non-digit:  [^0-9] <br/>
     * - @c "\h" -> A horizontal whitespace character: @c "[\t\xA0\u1680\u180e\u2000-\u200a\u202f\u205f\u3000]" <br/>
     * - @c "\H" -> A non-horizontal whitespace character: [^\h] <br/>
     * - @c "\s" -> A whitespace character:  @c "[\t\n\x0B\f\r]" if UNICODE_CHARACTER_CLASS is not set.
     *              See Unicode Support. <br/>
     * - @c "\S" -> A non-whitespace character: @c "[^\s]" <br/>
     * - @c "\v" -> A vertical whitespace character: @c "[\n\x0B\f\r\x85\u2028\u2029]" <br/>
     * - @c "\V" -> A non-vertical whitespace character: @c "[^\v]" <br/>
     * - @c "\w" -> A word character: @c "[a-zA-Z_0-9]" if UNICODE_CHARACTER_CLASS is not set.
     *              See Unicode Support. <br/>
     * - @c "\W" -> A non-word character:  @c "[^\w]" <br/>
     * </p>
     * <p>
     * 4- <b>POSIX character classes (US-ASCII only)</b> <br/>
     * - @c "\p{Lower}" -> A lower-case alphabetic character:  [a-z] <br/>
     * - @c "\p{Upper}" -> An upper-case alphabetic character: [A-Z] <br/>
     * - @c "\p{ASCII}" -> All ASCII: @c "[\x00-\x7F]" <br/>
     * - @c "\p{Alpha}" -> An alphabetic character: @c "[\p{Lower}\p{Upper}]" <br/>
     * - @c "\p{Digit}" -> A decimal digit:  [0-9] <br/>
     * - @c "\p{Alnum}" -> An alphanumeric character: @c "[\p{Alpha}\p{Digit}]" <br/>
     * - @c "\p{Punct}" -> Punctuation: One of !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~ <br/>
     * - @c "\p{Graph}" -> A visible character: @c "[\p{Alnum}\p{Punct}]" <br/>
     * - @c "\p{Print}" -> A printable character:  @c "[\p{Graph}\x20]" <br/>
     * - @c "\p{Blank}" -> A space or a tab:  @c "[\t]" <br/>
     * - @c "\p{Cntrl}" -> A control character: @c "[\x00-\x1F\x7F]" <br/>
     * - @c "\p{XDigit}" -> A hexadecimal digit: @c "[0-9a-fA-F]" <br/>
     * - @c "\p{Space}" -> A whitespace character:  @c "\t\n\x0B\f\r" <br/>
     * </p>
     * <p>
     * 5- Classes for Unicode scripts, blocks, categories and binary properties <br/>
     * - @c "\p{IsLatin}" -> A Latin script character (script) <br/>
     * - @c "\p{InGreek}" -> A character in the Greek block (block) <br/>
     * - @c "\p{Lu}" -> An uppercase letter (category) <br/>
     * - @c "\p{IsAlphabetic}" -> An alphabetic character (binary property) <br/>
     * - @c "\p{Sc}" -> A currency symbol <br/>
     * - @c "\P{InGreek}" -> Any character except one in the Greek block (negation) <br/>
     * - @c "[\p{L}&&[^\p{Lu}]]" -> Any letter except an uppercase letter (subtraction) <br/>
     * </p>
     * <p>
     * 6- Boundary matchers
     * - @c "^" -> The beginning of a line <br/>
     * - @c "$" -> The end of a line <br/>
     * - @c "\b" -> A word boundary: (?:(?<=\w)(?=\W)|(?<=\W)(?=\w)) (the location
     *     where a non-word character abuts a word character) <br/>
     * - @c "\b{g}" -> A Unicode extended grapheme cluster boundary <br/>
     * - @c "\B" -> A non-word boundary: [^\b] <br/>
     * - @c "\A" -> The beginning of the input <br/>
     * - @c "\G" -> The end of the previous match <br/>
     * - @c "\Z" -> The end of the input but for the final terminator, if any <br/>
     * - @c "\z" -> The end of the input <br/>
     * </p>
     * <p>
     * 7- Linebreak matcher <br/>
     * - @c "\R" -> Any Unicode linebreak sequence, is equivalent to
     *     @c "\u000D\u000A|[\u000A\u000B\u000C\u000D\u0085\u2028\u2029]" <br/>
     * </p>
     * <p>
     * 8- Unicode Extended Grapheme matcher <br/>
     * - @c "\X" -> Any Unicode extended grapheme cluster <br/>
     * </p>
     * <p>
     * 9- Greedy quantifiers <br/>
     * - @c "x?" -> x, once or not at all <br/>
     * - @c "x*" -> x, zero or more times <br/>
     * - @c "x+" -> x, one or more times <br/>
     * - @c "x{n}" -> x, exactly n times <br/>
     * - @c "x{n,}" -> x, at least n times <br/>
     * - @c "x{n,m}" -> <i>X</i>, at least n but not more than m times <br/>
     * </p>
     * <p>
     * 10- Reluctant quantifiers <br/>
     * - @c "x??" -> x, once or not at all <br/>
     * - @c "x*?" -> x, zero or more times <br/>
     * - @c "x+?" -> x, one or more times <br/>
     * - @c "x{n}?" -> x, exactly n times <br/>
     * - @c "x{n,}?" -> x, at least n times <br/>
     * - @c "x{n,m}?" -> x, at least n but not more than m times <br/>
     * </p>
     * <p>
     * 11- Possessive quantifiers <br/>
     * - @c "x?+" -> x, once or not at all <br/>
     * - @c "x*+" -> x, zero or more times <br/>
     * - @c "x++" -> x, one or more times <br/>
     * - @c "x{n}+" -> x, exactly n times <br/>
     * - @c "x{n,}+" -> x, at least n times <br/>
     * - @c "x{n,m}+" -> x, at least n but not more than m times <br/>
     * </p>
     * <p>
     * 12- Logical operators <br/>
     * - @c "xy" -> x followed by y <br/>
     * - @c "x|y" -> Either x or y <br/>
     * - @c "(x)" -> X, as a capturing group <br/>
     * </p>
     * <p>
     * 13- Back references <br/>
     * - @c "\n" -> Whatever the n<sup>th</sup> capturing group matched <br/>
     * - @c "\k" @code<name>@endcode -> Whatever the named-capturing group "name" matched <br/>
     * </p>
     * <p>
     * 14- Quotation <br/>
     * - @c "\" -> Nothing, but quotes the following character <br/>
     * - @c "\Q" -> Nothing, but quotes all characters until @c \E <br/>
     * - @c "\E" -> Nothing, but ends quoting started by @c \Q <br/>
     * </p>
     * <p>
     * 15- Special constructs (named-capturing and non-capturing) <br/>
     * - @c "(?name x)" -> x, as a named-capturing group <br/>
     * - @c "(?:x)" -> x, as a non-capturing group <br/>
     * - @c "(?idmsuxU-idmsuxU)" -> Nothing, but turns match flags i d m s u x U on - off <br/>
     * - @c "(?idmsuxU-idmsuxU:x)" -> x, as a non-capturing group with the given flags i d
     *  m s u x U on - off <br/>
     * - @c "(?=x)" -> x, via zero-width positive lookahead <br/>
     * - @c "(?!x)" -> x, via zero-width negative lookahead <br/>
     * - @c "(?<=x)" -> x, via zero-width positive lookbehind <br/>
     * - @c "(?<!x)" -> x, via zero-width negative lookbehind <br/>
     * - @c "(?>x)" -> x, as an independent, non-capturing group <br/>
     * </p>
     *
     * <h3>Backslashes, escapes, and quoting</h3>
     * <p>
     * The backslash character (@c '\') serves to introduce escaped
     * constructs, as defined in the table above, as well as to quote characters
     * that otherwise would be interpreted as unescaped constructs.  Thus the
     * expression @c '\\' matches a single backslash and @c '{' matches a
     * left brace.
     * </p>
     * <h3>Character Classes</h3>
     * <p>
     * Character classes may appear within other character classes, and
     * may be composed by the union operator (implicit) and the intersection
     * operator (@c '&&').
     * The union operator denotes a class that contains every character that is
     * in at least one of its operand classes.  The intersection operator
     * denotes a class that contains every character that is in both of its
     * operand classes.
     * </p>
     * <p>
     * The precedence of character-class operators is as follows, from
     * highest to lowest:
     * </p>
     * <b>Precedence of character class operators.</b>
     * <p>
     * Precedence -> Name -> Example <br/>
     * - 1 -> Literal escape -> @c '\x' <br/>
     * - 2 -> Grouping -> @c "[...]" <br/>
     * - 3 -> Range -> @c "a-z" <br/>
     * - 4 -> Union -> @c "[a-e][i-u]" <br/>
     * - 5 -> Intersection -> @c "[a-z&&[aeiou]]" <br/>
     * </p>
     * <p>
     * Note that a different set of metacharacters are in effect inside
     * a character class than outside a character class. For instance, the
     * regular expression @c . loses its special meaning inside a
     * character class, while the expression @c - becomes a range
     * forming metacharacter.
     * </p>
     * <h3>Line terminators</h3>
     * <p>
     * A <i>line terminator</i> is a one- or two-character sequence that marks
     * the end of a line of the input character sequence.  The following are
     * recognized as line terminators: <br/>
     * - A newline (line feed) character (@c '\n'), <br/>
     * - A carriage-return character followed immediately by a newline character (@c "\r\n"), <br/>
     * - A standalone carriage-return character (@c '\r'), <br/>
     * - A next-line character (@c '\u0085'), <br/>
     * - A line-separator character (@c '\u2028'), or <br/>
     * - A paragraph-separator character (@c '\u2029'). <br/>
     * <p>
     * If @b UNIX_LINES mode is activated, then the only line terminators
     * recognized are newline characters.
     * </p>
     * <p>
     * The regular expression @c . matches any character except a line
     * terminator unless the @b DOTALL flag is specified.
     * </p>
     * <p>
     * If @b MULTILINE mode is not activated, the regular expression
     * @c ^ ignores line terminators and only matches at the beginning of
     * the entire input sequence. The regular expression @c $ matches at the
     * end of the entire input sequence, but also matches just before the last line
     * terminator if this is not followed by any other input character. Other line
     * terminators are ignored, including the last one if it is followed by other
     * input characters.
     * </p>
     * <p>
     * If @b MULTILINE mode is activated then
     * @c ^ matches at the beginning of input and after any line terminator
     * except at the end of input. When in @b MULTILINE mode @c $
     * matches just before a line terminator or the end of the input sequence.
     * </p>
     * <h3>Groups and capturing</h3>
     *
     * <h3>Group number</h3>
     * <p>
     * Capturing groups are numbered by counting their opening parentheses from
     * left to right.  In the expression @c ((A)(B(C))), for example, there
     * are four such groups: <br/>
     * - ((A)(B(C))) <br/>
     * - (A) <br/>
     * - (B(C)) <br/>
     * - (C) <br/>
     * </p>
     * <p>
     * Group zero always stands for the entire expression.
     * </p>
     * <p>
     * Capturing groups are so named because, during a match, each subsequence
     * of the input sequence that matches such a group is saved.  The captured
     * subsequence may be used later in the expression, via a back reference, and
     * may also be retrieved from the matcher once the match operation is complete.
     * </p>
     * <h3>Group name</h3>
     * <p>
     * A capturing group can also be assigned a "name", a @c named-capturing group,
     * and then be back-referenced later by the "name". Group names are composed of
     * the following characters. The first character must be a @c letter. <br/>
     *
     * - The uppercase letters @c 'A' through @c 'Z' (@c '\u0041' through @c '\u005a'), <br/>
     * - The lowercase letters @c 'a' through @c 'z' (@c '\u0061' through @c '\u007a'), <br/>
     * - The digits @c '0' through @c '9' (@c '\u0030' through @c '\u0039'), <br/>
     * </p>
     * <p>
     * A @code named-capturing group @endcode is still numbered as described in Group number.
     * </p>
     * <p>
     * The captured input associated with a group is always the subsequence
     * that the group most recently matched.  If a group is evaluated a second time
     * because of quantification then its previously-captured value, if any, will
     * be retained if the second evaluation fails.  Matching the string
     * @c "aba" against the expression @code (a(b)?)+ @endcode , for example, leaves
     * group two set to @c "b".  All captured input is discarded at the
     * beginning of each match.
     * </p>
     * <p>
     * Groups beginning with @c (?} are either pure, <i>non-capturing</i> groups
     * that do not capture text and do not count towards the group total, or
     * <i>named-capturing</i> group.
     * </p>
     */
    class Pattern final : public Object {
      CORE_ADD_AS_FRIEND(Matcher);
      class Self;
      CORE_ALIAS(SelfData, Class<Self>::Pointer);

      /*
       * Pattern has only two serialized components: The pattern string
       * and the flags, which are all that is needed to recompile the pattern
       * when it is deserialized.
       */

      /**
       * The original regular-expression pattern string.
       *
       */
      String regex;

      /**
       * The original pattern flags.
       *
       */
      gint flags = 0;

      /**
       * The Pattern data.
       */
      SelfData self = { };

      /**
       * This private constructor is used to create all Patterns. The pattern
       * string and match flags are all that is needed to completely describe
       * a Pattern. An empty pattern string results in an object tree with
       * only a Start node and a LastNode node.
       */
      CORE_EXPLICIT Pattern(String regex, gint flags);

    public:
      /*
       * Regular expression modifier values.  Instead of being passed as
       * arguments, they can also be passed as inline modifiers.
       * For example, the following statements have the same effect.
       *
       *   Pattern p1 = Pattern::compile("abc", Pattern::CASE_INSENSITIVE|Pattern::MULTILINE);
       *   Pattern p2 = Pattern::compile("(?im)abc", 0);
       */
      enum Flags {
        /**
         * Enables Unix lines mode.
         *
         * <p> In this mode, only the @c '\n' line terminator is recognized
         * in the behavior of @c ., @c ^, and @c $.
         * </p>
         * <p>
         * Unix lines mode can also be enabled via the embedded flag
         * expression @c (?d).
         * </p>
         */
        UNIX_LINES = 0x1,

        /**
         * Enables case-insensitive matching.
         *
         * <p> By default, case-insensitive matching assumes that only characters
         * in the US-ASCII charset are being matched.  Unicode-aware
         * case-insensitive matching can be enabled by specifying the
         * @b UNICODE_CASE flag in conjunction with this flag.
         *
         * <p> Case-insensitive matching can also be enabled via the embedded flag
         * expression @c (?i).
         *
         * <p> Specifying this flag may impose a slight performance penalty.  </p>
         */
        CASE_INSENSITIVE = 0x2,

        /**
         * Permits whitespace and comments in pattern.
         *
         * <p>
         * In this mode, whitespace is ignored, and embedded comments starting
         * with @c # are ignored until the end of a line. Comments mode ignores
         * whitespace within a character class contained in a pattern string. Such
         * whitespace must be escaped in order to be considered significant.
         * </p>
         *
         * <p>
         * Comments mode can also be enabled via the embedded flag
         * expression @c (?x).
         * </p>
         */
        COMMENTS = 0x4,

        /**
         * Enables multiline mode.
         *
         * <p> In multiline mode the expressions @c ^ and @c $ match
         * just after or just before, respectively, a line terminator or the end of
         * the input sequence.  By default, these expressions only match at the
         * beginning and the end of the entire input sequence.
         *
         * <p>
         * Multiline mode can also be enabled via the embedded flag
         * expression @c (?m).
         * </p>
         */
        MULTILINE = 0x8,

        /**
         * Enables literal parsing of the pattern.
         *
         * <p> When this flag is specified then the input string that specifies
         * the pattern is treated as a sequence of literal characters.
         * Metacharacters or escape sequences in the input sequence will be
         * given no special meaning.
         *
         * <p>The flags CASE_INSENSITIVE and UNICODE_CASE retain their impact on
         * matching when used in conjunction with this flag. The other flags
         * become superfluous.
         *
         * <p> There is no embedded flag character for enabling literal parsing.
         */
        LITERAL = 0x10,

        /**
         * Enables dotall mode.
         *
         * <p> In dotall mode, the expression @c . matches any character,
         * including a line terminator.  By default, this expression does not match
         * line terminators.
         *
         * <p> Dotall mode can also be enabled via the embedded flag
         * expression @c (?s).  (The @c s is a mnemonic for
         * "single-line" mode, which is what this is called in Perl.)  </p>
         */
        DOTALL = 0x20,

        /**
         * Enables Unicode-aware case folding.
         *
         * <p> When this flag is specified then case-insensitive matching, when
         * enabled by the @b CASE_INSENSITIVE flag, is done in a manner
         * consistent with the Unicode Standard.  By default, case-insensitive
         * matching assumes that only characters in the US-ASCII charset are being
         * matched.
         *
         * <p> Unicode-aware case folding can also be enabled via the embedded flag
         * expression @c (?u).
         *
         * <p> Specifying this flag may impose a performance penalty.  </p>
         */
        UNICODE_CASE = 0x40,

        /**
         * Enables canonical equivalence.
         *
         * <p> When this flag is specified then two characters will be considered
         * to match if, and only if, their full canonical decompositions match.
         * The expression <code>"a&#92;u030A"</code>, for example, will match the
         * string <code>"&#92;u00E5"</code> when this flag is specified.  By default,
         * matching does not take canonical equivalence into account.
         *
         * <p> There is no embedded flag character for enabling canonical
         * equivalence.
         *
         * <p> Specifying this flag may impose a performance penalty
         * and a moderate risk of memory exhaustion.</p>
         */
        CANON_EQ = 0x80,

        /**
         * Enables the Unicode version of <i>Predefined character classes</i> and
         * <i>POSIX character classes</i>.
         *
         * <p> When this flag is specified then the (US-ASCII only)
         * <i>Predefined character classes</i> and <i>POSIX character classes</i>
         * are in conformance with
         * <a href="http://www.unicode.org/reports/tr18/"><i>Unicode Technical
         * Standard #18: Unicode Regular Expressions</i></a>
         * <i>Annex C: Compatibility Properties</i>.
         * <p>
         * The UNICODE_CHARACTER_CLASS mode can also be enabled via the embedded
         * flag expression @c (?U).
         * <p>
         * The flag implies UNICODE_CASE, that is, it enables Unicode-aware case
         * folding.
         * <p>
         * Specifying this flag may impose a performance penalty.
         * </p>
         *
         * @spec https://www.unicode.org/reports/tr18 Unicode Regular Expressions
         * @since 1.7
         */
        UNICODE_CHARACTER_CLASS = 0x100,
      };

      /**
       * Contains all possible flags for compile(regex, flags).
       */
      static CORE_FAST gint ALL_FLAGS = CASE_INSENSITIVE | MULTILINE | DOTALL |
          UNICODE_CASE | CANON_EQ | UNIX_LINES |
          LITERAL | UNICODE_CHARACTER_CLASS | COMMENTS;

      /**
       * Compiles the given regular expression into a pattern.
       *
       * @param  regex
       *         The expression to be compiled
       * @return the given regular expression compiled into a pattern
       * @throws  PatternSyntaxException
       *          If the expression's syntax is invalid
       */
      static Pattern compile(String const& regex);

      /**
       * Compiles the given regular expression into a pattern with the given
       * flags.
       *
       * <p>Setting @b CANON_EQ among the flags may impose a moderate risk
       * of memory exhaustion.</p>
       *
       * @param  regex
       *         The expression to be compiled
       *
       * @param  flags
       *         Match flags, a bit mask that may include
       *         @b CASE_INSENSITIVE, @b MULTILINE, @b DOTALL,
       *         @b UNICODE_CASE, @b CANON_EQ, @b UNIX_LINES,
       *         @b LITERAL, @b UNICODE_CHARACTER_CLASS
       *         and @b COMMENTS
       *
       * @return the given regular expression compiled into a pattern with the given flags
       * @throws  IllegalArgumentException
       *          If bit values other than those corresponding to the defined
       *          match flags are set in @c flags
       *
       * @throws  PatternSyntaxException
       *          If the expression's syntax is invalid
       *
       * @note If @b CANON_EQ is specified and the number of combining
       * marks for any character is too large, an @b OutOfMemoryError
       * is thrown.
       */
      static Pattern compile(String const& regex, gint flags);

      /**
       * Returns the regular expression from which this pattern was compiled.
       *
       * @return  The source of this pattern
       */
      String pattern() const;

      CORE_IMPLICIT Pattern(Pattern const& other);

      CORE_IMPLICIT Pattern(Pattern&& other) CORE_NOTHROW;

      Pattern& operator=(Pattern const& other);

      Pattern& operator=(Pattern&& other) CORE_NOTHROW;

      ~Pattern() override;

      /**
       * <p>Returns the string representation of this pattern. This
       * is the regular expression from which this pattern was
       * compiled.</p>
       *
       * @return  The string representation of this pattern
       */
      String toString() const override;

      /**
       * Creates a matcher that will match the given input against this pattern.
       *
       * @param  input
       *         The character sequence to be matched
       *
       * @return  A new matcher for this pattern
       *
       * @note When a @b Pattern is deserialized, compilation is deferred
       * until a direct or indirect invocation of this method. Thus, if a
       * deserialized pattern has @b CANON_EQ among its flags and the number
       * of combining marks for any character is too large, an
       * @b OutOfMemoryError is thrown, as in @b compile(String, int).
       */
      Matcher matcher(CharSequence const& input) const;

      /**
       * Returns this pattern's match flags.
       *
       * @return  The match flags specified when this pattern was compiled
       */
      gint getFlags() const;

      /**
       * Compiles the given regular expression and attempts to match the given
       * input against it.
       *
       * <p> An invocation of this convenience method of the form
       *
       * <blockquote><pre>
       * Pattern.matches(regex, input);</pre></blockquote>
       *
       * behaves in exactly the same way as the expression
       *
       * <blockquote><pre>
       * Pattern.compile(regex).matcher(input).matches()</pre></blockquote>
       *
       * <p> If a pattern is to be used multiple times, compiling it once and reusing
       * it will be more efficient than invoking this method each time.  </p>
       *
       * @param  regex
       *         The expression to be compiled
       *
       * @param  input
       *         The character sequence to be matched
       * @return whether the regular expression matches on the input
       * @throws  PatternSyntaxException
       *          If the expression's syntax is invalid
       */
      static gbool matches(String const& regex, CharSequence const& input);

      /**
       * Splits the given input sequence around matches of this pattern.
       *
       * <p>
       * The array returned by this method contains each substring of the
       * input sequence that is terminated by another subsequence that matches
       * this pattern or is terminated by the end of the input sequence.  The
       * substrings in the array are in the order in which they occur in the
       * input. If this pattern does not match any subsequence of the input then
       * the resulting array has just one element, namely the input sequence in
       * string form.
       * </p>
       * <p>
       * When there is a positive-width match at the beginning of the input
       * sequence then an empty leading substring is included at the beginning
       * of the resulting array. A zero-width match at the beginning however
       * never produces such empty leading substring.
       * </p>
       * <p>
       * The @c limit parameter controls the number of times the
       * pattern is applied and therefore affects the length of the resulting
       * array.
       *
       *    @li
       *    If the <i>limit</i> is positive then the pattern will be applied
       *    at most <i>limit</i> - 1 times, the array's length will be
       *    no greater than <i>limit</i>, and the array's last entry will contain
       *    all input beyond the last matched delimiter.
       *
       *    @li
       *    If the <i>limit</i> is zero then the pattern will be applied as
       *    many times as possible, the array can have any length, and trailing
       *    empty strings will be discarded.
       *
       *    @li
       *    If the <i>limit</i> is negative then the pattern will be applied
       *    as many times as possible and the array can have any length.
       * </p>
       * <p>
       * The input @c "boo:and:foo", for example, yields the following
       * results with these parameters: <br>
       * <b>Split example showing regex, limit, and result</b> <br>
       * <b> Regex --> Limit --> Result</b>
       * @li @c : --> 2 --> { "boo", "and:foo" }
       * @li @c : --> 5 -> { "boo", "and", "foo" }
       * @li @c : --> -2 -->  { "boo", "and", "foo" }
       * @li @c o --> 5 -->  { "b", "", ":and:f", "", "" }
       * @li @c o --> -2 --> { "b", "", ":and:f", "", "" }
       * @li @c o --> 0 -->  { "b", "", ":and:f" }
       * </p>
       * @param  input
       *         The character sequence to be split
       *
       * @param  limit
       *         The result threshold, as described above
       *
       * @return  The array of strings computed by splitting the input
       *          around matches of this pattern
       */
      StringArray split(CharSequence const& input, gint limit) const;

      /**
       * Splits the given input sequence around matches of this pattern and
       * returns both the strings and the matching delimiters.
       *
       * <p>
       * The array returned by this method contains each substring of the
       * input sequence that is terminated by another subsequence that matches
       * this pattern or is terminated by the end of the input sequence.
       * Each substring is immediately followed by the subsequence (the delimiter)
       * that matches this pattern, <em>except</em> for the last substring, which
       * is not followed by anything.
       * The substrings in the array and the delimiters are in the order in which
       * they occur in the input.
       * If this pattern does not match any subsequence of the input then the
       * resulting array has just one element, namely the input sequence in string
       * form.
       * </p>
       * <p>
       * When there is a positive-width match at the beginning of the input
       * sequence then an empty leading substring is included at the beginning
       * of the resulting array.
       * A zero-width match at the beginning however never produces such empty
       * leading substring nor the empty delimiter.
       * </p>
       * <p>
       * The @c limit parameter controls the number of times the
       * pattern is applied and therefore affects the length of the resulting
       * array.
       *
       * @li If the <i>limit</i> is positive then the pattern will be applied
       *    at most <i>limit</i> - 1 times, the array's length will be
       *    no greater than 2 &times; <i>limit</i> - 1, and the array's last
       *    entry will contain all input beyond the last matched delimiter.
       *
       * @li If the <i>limit</i> is zero then the pattern will be applied as
       *    many times as possible, the array can have any length, and trailing
       *    empty strings, whether substrings or delimiters, will be discarded.
       *
       * @li If the <i>limit</i> is negative then the pattern will be applied
       *    as many times as possible and the array can have any length.
       * </p>
       * <p>
       * The input @c "boo:::and::foo", for example, yields the following
       * results with these parameters:
       *
       * <b>Split example showing regex, limit, and result</b> <br/>
       * <b> Regex --> Limit --> Result </b>
       * @li @c :+ --> 2 --> { "boo", ":::", "and::foo" }
       * @li @c : --> 5 --> { "boo", ":::", "and", "::", "foo" }
       * @li @c : --> -1 --> { "boo", ":::", "and", "::", "foo" }
       * @li @c o --> 5 --> { "b", "o", "", "o", ":::and::f", "o", "", "o", "" }
       * @li @c o --> -1 --> { "b", "o", "", "o", ":::and::f", "o", "", "o", "" }
       * @li @c o --> 0 --> { "b", "o", "", "o", ":::and::f", "o", "", "o" }
       * </p>
       *
       * @param  input
       *         The character sequence to be split
       *
       * @param  limit
       *         The result threshold, as described above
       *
       * @return  The array of strings computed by splitting the input
       *          around matches of this pattern, alternating
       *          substrings and matching delimiters
       */
      StringArray splitWithDelimiters(CharSequence const& input, gint limit) const;

      /**
       * Splits the given input sequence around matches of this pattern.
       *
       * <p>
       * This method works as if by invoking the two-argument @b split
       * method with the given input
       * sequence and a limit argument of zero.  Trailing empty strings are
       * therefore not included in the resulting array.
       * </p>
       *
       *
       * @param  input
       *         The character sequence to be split
       *
       * @return  The array of strings computed by splitting the input
       *          around matches of this pattern
       */
      StringArray split(CharSequence const& input) const;

      /**
       * Returns a literal pattern @c String for the specified
       * @c String.
       *
       * <p>
       * This method produces a @c String that can be used to
       * create a @c Pattern that would match the string
       * @c s as if it were a literal pattern.
       * </p>
       * <p>
       * Metacharacters or escape sequences in the input sequence will
       * be given no special meaning.
       * </p>
       * @param  s The string to be literalized
       * @return  A literal string replacement
       */
      static String quote(String const& s);

      /**
       * Returns an unmodifiable map from capturing group names to group numbers.
       * If there are no named groups, returns an empty map.
       *
       * @return an unmodifiable map from capturing group names to group numbers
       */
      util::Map<String, Integer>& namedGroups() const;

      /**
       * Creates a predicate that tests if this pattern is found in a given input
       * string.
       *
       * @note
       * This method creates a predicate that behaves as if it creates a matcher
       * from the input sequence and then calls @c find, for example a
       * predicate of the form:
       * @code
       *   [&](String const &s) { return matcher(s).find(); }
       * @endcode
       *
       * @return  The predicate which can be used for finding a match on a
       *          subsequence of a string
       * @see     Matcher::find
       */
      function::Predicate<String> asPredicate() const;

      /**
       * Creates a predicate that tests if this pattern matches a given input string.
       *
       * @note
       * This method creates a predicate that behaves as if it creates a matcher
       * from the input sequence and then calls @c matches, for example a
       * predicate of the form:
       * @code
       *   [&](String const &s) { return matcher(s).matches(); }
       * @endcode
       *
       * @return  The predicate which can be used for matching an input string
       *          against this pattern.
       * @see     Matcher::matches
       */
      function::Predicate<String> asMatchPredicate() const;

    private:
      StringArray split(CharSequence const& input, int limit, gbool withDelimiters) const;

      /**
       * The following classes are the building components of the object
       * tree that represents a compiled regular expression. The object tree
       * is made of individual elements that handle constructs in the Pattern.
       * Each type of object knows how to match its equivalent construct with
       * the match() method.
       */

      class Analyzer;

      /**
       * Base class for all node classes. Subclasses should override the match()
       * method as appropriate. This class is an accepting node, so its match()
       * always returns true.
       */
      class Node;
      class LastNode;

      /**
       * Used for REs that can start anywhere within the input string.
       * This basically tries to match repeatedly at each spot in the
       * input string, moving forward after each try. An anchored search
       * or a BnM will bypass this node completely.
       */
      class Start;

      /*
       * StartS supports supplementary characters, including unpaired surrogates.
       */
      class StartS;

      /**
       * Node to anchor at the beginning of input. This object implements the
       * match for a \A sequence, and the caret anchor will use this if not in
       * multiline mode.
       */
      class Begin;

      /**
       * Node to anchor at the end of input. This is the absolute end, so this
       * should not match at the last newline before the end as $ will.
       */
      class End;

      /**
       * Node to anchor at the beginning of a line. This is essentially the
       * object to match for the multiline ^.
       */
      class Caret;

      /**
       * Node to anchor at the beginning of a line when in unixdot mode.
       */
      class UnixCaret;

      /**
       * Node to match the location where the last match ended.
       * This is used for the \G construct.
       */
      class LastMatch;

      /**
       * Node to anchor at the end of a line or the end of input based on the
       * multiline mode.
       *
       * When not in multiline mode, the $ can only match at the very end
       * of the input, unless the input ends in a line terminator in which
       * it matches right before the last line terminator.
       *
       * Note that \r\n is considered an atomic line terminator.
       *
       * Like ^ the $ operator matches at a position, it does not match the
       * line terminators themselves.
       */
      class Dollar;

      /**
       * Node to anchor at the end of a line or the end of input based on the
       * multiline mode when in unix lines mode.
       */
      class UnixDollar;

      /**
       * Node class that matches a Unicode line ending '\R'
       */
      class LineEnding;

      /**
       * Abstract node class to match one character satisfying some
       * boolean property.
       */
      class CharProperty;

      /**
       * Optimized version of CharProperty that works only for
       * properties never satisfied by Supplementary characters.
       */
      class BmpCharProperty;
      class NFCCharProperty;

      /**
       * Node class that matches a Unicode extended grapheme cluster
       */
      class XGrapheme;

      /**
       * Node class that handles grapheme boundaries
       */
      class GraphemeBound;

      /**
       * Base class for all Slice nodes
       */
      class SliceNode;

      /**
       * Node class for a case-sensitive/BMP-only sequence of literal
       * characters.
       */
      class Slice;

      /**
       * Node class for a case_insensitive/BMP-only sequence of literal
       * characters.
       */
      class SliceI;

      /**
       * Node class for a unicode_case_insensitive/BMP-only sequence of
       * literal characters. Uses Unicode case folding.
       */
      class SliceU;

      /**
       * Node class for a case-sensitive sequence of literal characters
       * including supplementary characters.
       */
      class SliceS;

      /**
       * Node class for a case-insensitive sequence of literal characters
       * including supplementary characters.
       */
      class SliceIS;

      /**
       * Node class for a case-insensitive sequence of literal characters.
       * Uses Unicode case folding.
       */
      class SliceUS;

      /**
       * The 0 or 1 quantifier. This one class implements all three types.
       */
      class Ques;

      /**
       * Handles the greedy style repetition with the specified minimum
       * and the maximum equal to MAX_REPS, for *, + and {N,} quantifiers.
       */
      class CharPropertyGreedy;
      class BmpCharPropertyGreedy;

      /**
       * Handles the curly-brace style repetition with a specified minimum and
       * maximum occurrences. The * quantifier is handled as a special case.
       * This class handles the three types.
       */
      class Curly;

      /**
       * Handles the curly-brace style repetition with a specified minimum and
       * maximum occurrences in deterministic cases. This is an iterative
       * optimization over the Prolog and Loop system which would handle this
       * in a recursive way. The * quantifier is handled as a special case.
       * If capture is true then this class saves group settings and ensures
       * that groups are unset when backing off of a group match.
       */
      class GroupCurly;

      /**
       * A Guard node at the end of each atom node in a Branch. It
       * serves the purpose of chaining the "match" operation to
       * "next" but not the "study", so we can collect the TreeInfo
       * of each atom node without including the TreeInfo of the
       * "next".
       */
      class BranchConn;

      /**
       * Handles the branching of alternations. Note this is also used for
       * the ? quantifier to branch between the case where it matches once
       * and where it does not occur.
       */
      class Branch;

      /**
       * The GroupHead saves the location where the group begins in the locals
       * and restores them when the match is done.
       *
       * The matchRef is used when a reference to this group is accessed later
       * in the expression. The locals will have a negative value in them to
       * indicate that we do not want to unset the group if the reference
       * doesn't match.
       */
      class GroupHead;

      /**
       * The GroupTail handles the setting of group beginning and ending
       * locations when groups are successfully matched. It must also be able to
       * unset groups that have to be backed off of.
       *
       * The GroupTail node is also used when a previous group is referenced,
       * and in that case no group information needs to be set.
       */
      class GroupTail;

      /**
       * This sets up a loop to handle a recursive quantifier structure.
       */
      class Prolog;

      /**
       * Handles the repetition count for a greedy Curly. The matchInit
       * is called from the Prolog to save the index of where the group
       * beginning is stored. A zero length group check occurs in the
       * normal match but is skipped in the matchInit.
       */
      class Loop;

      /**
       * Handles the repetition count for a reluctant Curly. The matchInit
       * is called from the Prolog to save the index of where the group
       * beginning is stored. A zero length group check occurs in the
       * normal match but is skipped in the matchInit.
       */
      class LazyLoop;

      /**
       * Refers to a group in the regular expression. Attempts to match
       * whatever the group referred to last matched.
       */
      class BackRef;
      class CIBackRef;

      /**
       * Searches until the next instance of its atom. This is useful for
       * finding the atom efficiently without passing an instance of it
       * (greedy problem) and without a lot of wasted search time (reluctant
       * problem).
       */
      class First;

      /**
       * Zero width positive lookahead.
       */
      class Pos;

      /**
       * Zero width negative lookahead.
       */
      class Neg;

      /**
       * For use with lookbehinds; matches the position where the lookbehind
       * was encountered.
       */
      class LookBehindEndNode;

      /**
       * Zero width positive lookbehind.
       */
      class Behind;

      /**
       * Zero width positive lookbehind, including supplementary
       * characters or unpaired surrogates.
       */
      class BehindS;

      /**
       * Zero width negative lookbehind.
       */
      class NotBehind;

      /**
       * Zero width negative lookbehind, including supplementary
       * characters or unpaired surrogates.
       */
      class NotBehindS;

      /**
       * Handles word boundaries. Includes a field to allow this one class to
       * deal with the different types of word boundaries we can match. The word
       * characters include underscores, letters, and digits. Non spacing marks
       * can are also part of a word if they have a base character, otherwise
       * they are ignored for purposes of finding word boundaries.
       */
      class Bound;

      /**
       * Attempts to match a slice in the input using the Boyer-Moore string
       * matching algorithm. The algorithm is based on the idea that the
       * pattern can be shifted farther ahead in the search text if it is
       * matched right to left.
       * <p>
       * The pattern is compared to the input one character at a time, from
       * the rightmost character in the pattern to the left. If the characters
       * all match the pattern has been found. If a character does not match,
       * the pattern is shifted right a distance that is the maximum of two
       * functions, the bad character shift and the good suffix shift. This
       * shift moves the attempted match position through the input more
       * quickly than a naive one position at a time check.
       * </p>
       * <p>
       * The bad character shift is based on the character from the text that
       * did not match. If the character does not appear in the pattern, the
       * pattern can be shifted completely beyond the bad character. If the
       * character does occur in the pattern, the pattern can be shifted to
       * line the pattern up with the next occurrence of that character.
       * </p>
       * <p>
       * The good suffix shift is based on the idea that some subset on the right
       * side of the pattern has matched. When a bad character is found, the
       * pattern can be shifted right by the pattern length if the subset does
       * not occur again in pattern, or by the amount of distance to the
       * next occurrence of the subset in the pattern.
       * </p>
       * Boyer-Moore search methods adapted from code by Amy Yu.
       */
      class BnM;

      /**
       * Supplementary support version of BnM(). Unpaired surrogates are
       * also handled by this class.
       */
      class BnMS;

      class CharPredicate;
      class BmpCharPredicate;

      /**
       *  Creates a bit vector for matching Latin-1 values. A normal BitClass
       *  never matches values above Latin-1, and a complemented BitClass always
       *  matches values above Latin-1.
       */
      class BitClass;
      class Predicates;

      class Grapheme;
      class IndicConjunctBreak;

      /**
       * Utility class that implements the standard C ctype functionality.
       */
      class ASCII;

      /**
       * Used to accumulate information about a subtree of the object graph
       * so that optimizations can be applied to the subtree.
       */
      class TreeInfo;

      static Node& accept;
      static Node& lastAccept;
    };
  } // regex
} // core

#endif //CORE24_PATTERN_H
