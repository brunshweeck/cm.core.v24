//
// Created by bruns on 15/10/2024.
//

#ifndef CORE24_MATCHER_H
#define CORE24_MATCHER_H

#include <core/regex/MatchResult.h>
#include <core/regex/Pattern.h>
#include <core/util/Optional.h>
#include <core/util/Map.h>

namespace core {
    namespace regex {
        /**
         * An engine that performs match operations on a <em>character sequence</em>
         * by interpreting a @b Pattern.
         *
         * <p>
         * A matcher is created from a pattern by invoking the pattern's
         * @b matcher method.  Once created, a matcher can be used to
         * perform three different kinds of match operations:
         *
         *   @li The @b matches method attempts to match the entire
         *   input sequence against the pattern.
         *
         *   @li The @b lookingAt method attempts to match the
         *   input sequence, starting at the beginning, against the pattern.
         *
         *   @li The @b find method scans the input sequence looking
         *   for the next subsequence that matches the pattern.
         * </p>
         * <p>
         * Each of these methods returns a boolean indicating success or failure.
         * More information about a successful match can be obtained by querying the
         * state of the matcher.
         * </p>
         * <p>
         * A matcher finds matches in a subset of its input called the
         * <i>region</i>. By default, the region contains all the matcher's input.
         * The region can be modified via the @b region method
         * and queried via the @b regionStart and @b regionEnd methods.
         * The way that the region boundaries interact
         * with some pattern constructs can be changed. See @b useAnchoringBounds and
         * @b useTransparentBounds for more details.
         * </p>
         * <p>
         * This class also defines methods for replacing matched subsequences with
         * new strings whose contents can, if desired, be computed from the match
         * result.  The @b appendReplacement and @b appendTail methods can be used
         * in tandem in order to collect the result into an existing string buffer.
         * Alternatively,
         * the more convenient @b replaceAll method can be used to
         * create a string in which every matching subsequence in the input sequence
         * is replaced.
         * </p>
         * <p>
         * The explicit state of a matcher includes the start and end indices of
         * the most recent successful match.  It also includes the start and end
         * indices of the input subsequence captured by each capturing group in the
         * pattern as well as a total count of such subsequences.  As a convenience,
         * methods are also provided for returning these captured subsequences in string form.
         * </p>
         * <p>
         * The explicit state of a matcher is initially undefined; attempting to
         * query any part of it before a successful match will cause an
         * @b IllegalStateException to be thrown.  The explicit state of a matcher is
         * recomputed by every match operation.
         * </p>
         * <p>
         * The implicit state of a matcher includes the input character sequence as
         * well as the <i>append position</i>, which is initially zero and is updated
         * by the @b appendReplacement method.
         * </p>
         * <p>
         * A matcher may be reset explicitly by invoking its @b reset()
         * method or, if a new input sequence is desired, its @b reset(CharSequence) method.
         * Resetting a matcher discards its explicit state information and sets the append
         * position to zero.
         * </p>
         * <p>
         * Instances of this class are not safe for use by multiple concurrent
         * threads.
         * </p>
         */
        class Matcher final : public MatchResult {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_AS_FRIEND(Pattern);

            class HashSet final : public Object {
                IntArray entries;
                IntArray hashes;
                gint pos = 0;

            public:
                HashSet();

                gbool contains(gint i);

                void add(gint i);

                void clear();

                void expand();

                Object &clone() const override;
            };

            /**
             * The Pattern object that created this Matcher.
             */
            Pattern parent;
            /**
             * The storage used by groups. They may contain invalid values if
             * a group was skipped during the matching.
             */
            IntArray mutable groups;

            /**
             * The range within the sequence that is to be matched. Anchors
             * will match at these "hard" boundaries. Changing the region
             * changes these values.
             */
            gint from = 0, to = 0;

            /**
             * Lookbehind uses this value to ensure that the subexpression
             * match ends at the point where the lookbehind was encountered.
             */
            gint lookbehindTo = 0;

            /**
             * The original string being matched.
             */
            util::Optional<CharSequence> text;

            /**
             * Matcher state used by the last node. NOANCHOR is used when a
             * match does not have to consume all the input. ENDANCHOR is
             * the mode used for matching all the input.
             */
            static CORE_FAST gint ENDANCHOR = 1;

            static CORE_FAST gint NOANCHOR = 0;

            gint mutable acceptMode = NOANCHOR;

            /**
             * The range of string that last matched the pattern. If the last
             * match failed then first is -1; last initially holds 0 then it
             * holds the index of the end of the last match (which is where the
             * next search starts).
             */
            gint mutable first = -1, last = 0;

            /**
             * The end index of what matched in the last match operation.
             */
            gint mutable oldLast = -1;

            /**
             * The index of the last position appended in a substitution.
             */
            gint lastAppendPosition = 0;

            /**
             * Storage used by nodes to tell what repetition they are on in
             * a pattern, and where groups begin. The nodes themselves are stateless,
             * so they rely on this field to hold state during a match.
             */
            IntArray locals;

            /**
             * Storage used by top greedy Loop node to store a specific hash set to
             * keep the beginning index of the failed repetition match. The nodes
             * themselves are stateless, so they rely on this field to hold state
             * during a match.
             */
            Array<HashSet> localsPos;

            /**
             * Boolean indicating whether or not more input could change
             * the results of the last match.
             *
             * If hitEnd is true, and a match was found, then more input
             * might cause a different match to be found.
             * If hitEnd is true and a match was not found, then more
             * input could cause a match to be found.
             * If hitEnd is false and a match was found, then more input
             * will not change the match.
             * If hitEnd is false and a match was not found, then more
             * input will not cause a match to be found.
             */
            gbool mutable hitEnd_ = false;

            /**
             * Boolean indicating whether more input could change
             * a positive match into a negative one.
             *
             * If requireEnd is true, and a match was found, then more
             * input could cause the match to be lost.
             * If requireEnd is false and a match was found, then more
             * input might change the match but the match won't be lost.
             * If a match was not found, then requireEnd has no meaning.
             */
            gbool mutable requireEnd = false;

            /**
             * If transparentBounds is true then the boundaries of this
             * matcher's region are transparent to lookahead, lookbehind,
             * and boundary matching constructs that try to see beyond them.
             */
            gbool transparentBounds = false;

            /**
             * If anchoringBounds is true then the boundaries of this
             * matcher's region match anchors such as ^ and $.
             */
            gbool anchoringBounds = true;

            /**
             * Number of times this matcher's state has been modified
             */
            gint mutable modCount = 0;

            util::Optional<util::Map<String, Integer> > mutable namedGroups_;

            /**
             * No default constructor.
             */
            Matcher();

            /**
             * All matchers have the state used by Pattern during a match.
             */
            CORE_EXPLICIT Matcher(Pattern const &parent, CharSequence const &text);

        public:
            Matcher(Matcher const &other);

            Matcher(Matcher &&other) CORE_NOTHROW;

            Matcher &operator=(Matcher const &other);

            Matcher &operator=(Matcher &&other) CORE_NOTHROW;

            /**
             * Returns the pattern that is interpreted by this matcher.
             *
             * @return  The pattern for which this matcher was created
             */
            Pattern getPattern() const;

            /**
             * Returns the match state of this matcher as a @b MatchResult.
             * The result is unaffected by subsequent operations performed upon this
             * matcher.
             *
             * @return  a @c MatchResult with the state of this matcher
             */
            MatchResult &toMatchResult() const;

            /**
             * Changes the @c Pattern that this @c Matcher uses to
             * find matches with.
             *
             * <p>
             * This method causes this matcher to lose information
             * about the groups of the last match that occurred. The
             * matcher's position in the input is maintained and its
             * last append position is unaffected.
             * </p>
             *
             * @param  newPattern
             *         The new pattern used by this matcher
             * @return  This matcher
             */
            Matcher &setPattern(Pattern const &newPattern);

            /**
             * Resets this matcher.
             *
             * <p> Resetting a matcher discards all of its explicit state information
             * and sets its append position to zero. The matcher's region is set to the
             * default region, which is its entire character sequence. The anchoring
             * and transparency of this matcher's region boundaries are unaffected.
             *
             * @return  This matcher
             */
            Matcher &reset();

            /**
             * Resets this matcher with a new input sequence.
             *
             * <p> Resetting a matcher discards all of its explicit state information
             * and sets its append position to zero.  The matcher's region is set to
             * the default region, which is its entire character sequence.  The
             * anchoring and transparency of this matcher's region boundaries are
             * unaffected.
             *
             * @param  input
             *         The new input character sequence
             *
             * @return  This matcher
             */
            Matcher &reset(CharSequence const &input);

            /**
             * Returns the start index of the previous match.
             *
             * @return  The index of the first character matched
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             */
            gint start() const override;

            /**
             * Returns the start index of the subsequence captured by the given group
             * during the previous match operation.
             *
             * <p>
             * Capturing groups are indexed from left
             * to right, starting at one.  Group zero denotes the entire pattern, so
             * the expression <i>m.</i>@c start(0) is equivalent to
             * <i>m.</i>@c start().
             * </p>
             *
             * @param  group
             *         The index of a capturing group in this matcher's pattern
             *
             * @return  The index of the first character captured by the group,
             *          or @c -1 if the match was successful but the group
             *          itself did not match anything
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             *
             * @throws  IndexOutOfBoundsException
             *          If there is no capturing group in the pattern
             *          with the given index
             */
            gint start(gint group) const override;

            /**
             * Returns the start index of the subsequence captured by the given
             * <a href="Pattern.html#groupname">named-capturing group</a> during the
             * previous match operation.
             *
             * @param  name
             *         The name of a named-capturing group in this matcher's pattern
             *
             * @return  The index of the first character captured by the group,
             *          or @c -1 if the match was successful but the group
             *          itself did not match anything
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             *
             * @throws  IllegalArgumentException
             *          If there is no capturing group in the pattern
             *          with the given name
             */
            gint start(String const &name) const override;

            /**
             * Returns the offset after the last character matched.
             *
             * @return  The offset after the last character matched
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             */
            gint end() const override;

            /**
             * Returns the offset after the last character of the subsequence
             * captured by the given group during the previous match operation.
             *
             * <p>
             * Capturing groups are indexed from left
             * to right, starting at one.  Group zero denotes the entire pattern, so
             * the expression <i>m.</i>@c end(0) is equivalent to
             * <i>m.</i>@c end().
             * </p>
             *
             * @param  group
             *         The index of a capturing group in this matcher's pattern
             *
             * @return  The offset after the last character captured by the group,
             *          or @c -1 if the match was successful
             *          but the group itself did not match anything
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             *
             * @throws  IndexOutOfBoundsException
             *          If there is no capturing group in the pattern
             *          with the given index
             */
            gint end(gint group) const override;

            /**
             * Returns the offset after the last character of the subsequence
             * captured by the given <a href="Pattern.html#groupname">named-capturing
             * group</a> during the previous match operation.
             *
             * @param  name
             *         The name of a named-capturing group in this matcher's pattern
             *
             * @return  The offset after the last character captured by the group,
             *          or @c -1 if the match was successful
             *          but the group itself did not match anything
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             *
             * @throws  IllegalArgumentException
             *          If there is no capturing group in the pattern
             *          with the given name
             * @since 1.8
             */
            gint end(String const &name) const override;

            /**
             * Returns the input subsequence matched by the previous match.
             *
             * <p>
             * For a matcher <i>m</i> with input sequence <i>s</i>,
             * the expressions <i>m.</i>@c group() and
             * <i>s.</i>@c substring(<i>m.</i>@c start(), <i>m.</i>@c end()) are equivalent.
             * </p>
             * <p>
             * Note that some patterns, for example @c a*, match the empty
             * string.  This method will return the empty string when the pattern
             * successfully matches the empty string in the input.
             * </p>
             *
             * @return The (possibly empty) subsequence matched by the previous match,
             *         in string form or @c "" if a matcher with a previous
             *         match has changed its @b regex::Pattern,
             *         but no new match has yet been attempted
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             */
            String group() const override;

            /**
             * Returns the input subsequence captured by the given group during the
             * previous match operation.
             *
             * <p>
             * For a matcher <i>m</i>, input sequence <i>s</i>, and group index
             * <i>g</i>, the expressions <i>m.</i>@c group(<i>g</i>@c ) and
             * <i>s.</i>@c substring(<i>m.</i>@c start(<i>g</i>@c ), <i>m.</i>@c end(<i>g</i>@c ))
             * are equivalent.
             * </p>
             *
             * <p>
             * Capturing groups are indexed from left
             * to right, starting at one.  Group zero denotes the entire pattern, so
             * the expression @c m.group(0) is equivalent to @c m.group().
             * </p>
             *
             * <p>
             * If the match was successful but the group specified failed to match
             * any part of the input sequence, then @c "" is returned. Note
             * that some groups, for example @c (a*), match the empty string.
             * This method will return the empty string when such a group successfully
             * matches the empty string in the input.
             * </p>
             *
             * @param  group
             *         The index of a capturing group in this matcher's pattern
             *
             * @return  The (possibly empty) subsequence captured by the group
             *          during the previous match, or @c "" if the group
             *          failed to match part of the input or if the matcher's
             *          @b regex::Pattern has changed after a
             *          successful match, but a new match has not been attempted
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             *
             * @throws  IndexOutOfBoundsException
             *          If there is no capturing group in the pattern
             *          with the given index
             */
            String group(gint group) const override;

            /**
             * Returns the input subsequence captured by the given
             * <a href="Pattern.html#groupname">named-capturing group</a> during the
             * previous match operation.
             *
             * <p> If the match was successful but the group specified failed to match
             * any part of the input sequence, then @c "" is returned. Note
             * that some groups, for example @c (a*), match the empty string.
             * This method will return the empty string when such a group successfully
             * matches the empty string in the input.  </p>
             *
             * @param  name
             *         The name of a named-capturing group in this matcher's pattern
             *
             * @return  The (possibly empty) subsequence captured by the named group
             *          during the previous match, or @c "" if the group
             *          failed to match part of the input
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             *
             * @throws  IllegalArgumentException
             *          If there is no capturing group in the pattern
             *          with the given name
             */
            String group(String const &name) const override;

            /**
             * Returns the number of capturing groups in this matcher's pattern.
             *
             * <p> Group zero denotes the entire pattern by convention. It is not
             * included in this count.
             *
             * <p> Any non-negative integer smaller than or equal to the value
             * returned by this method is guaranteed to be a valid group index for
             * this matcher.  </p>
             *
             * @return The number of capturing groups in this matcher's pattern
             */
            gint groupCount() const override;

            /**
             * Attempts to match the entire region against the pattern.
             *
             * <p>
             * If the match succeeds then more information can be obtained via the
             * @c start, @c end, and @c group methods.
             * </p>
             *
             * @return  @c true if, and only if, the entire region sequence
             *          matches this matcher's pattern
             */
            gbool matches();

            /**
             * Attempts to find the next subsequence of the input sequence that matches
             * the pattern.
             *
             * <p> This method starts at the beginning of this matcher's region, or, if
             * a previous invocation of the method was successful and the matcher has
             * not since been reset, at the first character not matched by the previous
             * match.
             *
             * <p> If the match succeeds then more information can be obtained via the
             * @c start, @c end, and @c group methods.  </p>
             *
             * @return  @c true if, and only if, a subsequence of the input
             *          sequence matches this matcher's pattern
             */
            gbool find();

            /**
             * Resets this matcher and then attempts to find the next subsequence of
             * the input sequence that matches the pattern, starting at the specified
             * index.
             *
             * <p>
             * If the match succeeds then more information can be obtained via the
             * @c start, @c end, and @c group methods, and subsequent
             * invocations of the @b find() method will start at the first
             * character not matched by this match.
             * </p>
             *
             * @param start the index to start searching for a match
             * @throws  IndexOutOfBoundsException
             *          If start is less than zero or if start is greater than the
             *          length of the input sequence.
             *
             * @return  @c true if, and only if, a subsequence of the input
             *          sequence starting at the given index matches this matcher's
             *          pattern
             */
            gbool find(gint start);

            /**
             * Attempts to match the input sequence, starting at the beginning of the
             * region, against the pattern.
             *
             * <p> Like the @b matches method, this method always starts
             * at the beginning of the region; unlike that method, it does not
             * require that the entire region be matched.
             *
             * <p> If the match succeeds then more information can be obtained via the
             * @c start, @c end, and @c group methods.  </p>
             *
             * @return  @c true if, and only if, a prefix of the input
             *          sequence matches this matcher's pattern
             */
            gbool lookingAt();

            /**
             * Returns a literal replacement @c String for the specified
             * @c String.
             *
             * This method produces a @c String that will work
             * as a literal replacement @c s in the
             * @c appendReplacement method of the @b Matcher class.
             * The @c String produced will match the sequence of characters
             * in @c s treated as a literal sequence. Slashes ('\') and
             * dollar signs ('$') will be given no special meaning.
             *
             * @param  s The string to be literalized
             * @return  A literal string replacement
             */
            static String quoteReplacement(String const &s);

            /**
             * Implements a non-terminal append-and-replace step.
             *
             * <p> This method performs the following actions: </p>
             *
             *   @li It reads characters from the input sequence, starting at the
             *   append position, and appends them to the given string buffer.  It
             *   stops after reading the last character preceding the previous match,
             *   that is, the character at index @b start() @c - @c 1.
             *
             *   @li It appends the given replacement string to the string buffer.
             *
             *
             *   @li It sets the append position of this matcher to the index of
             *   the last character matched, plus one, that is, to @b end().
             *
             * <p>
             * The replacement string may contain references to subsequences
             * captured during the previous match: Each occurrence of
             * <code>${</code><i>name</i><code>}</code> or @c $<i>g</i>
             * will be replaced by the result of evaluating the corresponding
             * @b group(name) or @b group(g) respectively. For @c $<i>g</i>,
             * the first number after the @c $ is always treated as part of
             * the group reference. Subsequent numbers are incorporated into g if
             * they would form a legal group reference. Only the numerals '0'
             * through '9' are considered as potential components of the group
             * reference. If the second group matched the string @c "foo", for
             * example, then passing the replacement string @c "$2bar" would
             * cause @c "foobar" to be appended to the string buffer. A dollar
             * sign (@c $) may be included as a literal in the replacement
             * string by preceding it with a backslash (@c \$).
             *
             * <p>
             * Note that backslashes (@c \) and dollar signs (@c $) in
             * the replacement string may cause the results to be different than if it
             * were being treated as a literal replacement string. Dollar signs may be
             * treated as references to captured subsequences as described above, and
             * backslashes are used to escape literal characters in the replacement
             * string.
             * </p>
             * <p>
             * This method is intended to be used in a loop together with the
             * @b appendTail and @b find
             * methods.  The following code, for example, writes <code> one dog two dogs
             * in the yard</code> to the standard-output stream:
             * </p>
             *
             * @code
             * Pattern p = Pattern::compile("cat");
             * Matcher m = p.matcher("one cat two cats in the yard");
             * XString sb;
             * while (m.find()) {
             *     m.appendReplacement(sb, "dog");
             * }
             * m.appendTail(sb);
             * Console::out.println(sb.toString());
             * @endcode
             *
             * @param  sb
             *         The target string buffer
             *
             * @param  replacement
             *         The replacement string
             *
             * @return  This matcher
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             *
             * @throws  IllegalArgumentException
             *          If the replacement string refers to a named-capturing
             *          group that does not exist in the pattern
             *
             * @throws  IndexOutOfBoundsException
             *          If the replacement string refers to a capturing group
             *          that does not exist in the pattern
             */
            Matcher &appendReplacement(XString &sb, String const &replacement);

            /**
             * Implements a terminal append-and-replace step.
             *
             * <p>
             * This method reads characters from the input sequence, starting at
             * the append position, and appends them to the given string buffer.  It is
             * intended to be invoked after one or more invocations of the @b appendReplacement method in
             * order to copy the remainder of the input sequence.
             * </p>
             *
             * @param  sb
             *         The target string buffer
             *
             * @return  The target string buffer
             */
            XString &appendTail(XString &sb);

            /**
             * Replaces every subsequence of the input sequence that matches the
             * pattern with the given replacement string.
             *
             * <p>
             * This method first resets this matcher.  It then scans the input
             * sequence looking for matches of the pattern.  Characters that are not
             * part of any match are appended directly to the result string; each match
             * is replaced in the result by the replacement string.  The replacement
             * string may contain references to captured subsequences as in the @b appendReplacement method.
             * </p>
             * <p>
             * Note that backslashes (@c \) and dollar signs (@c $) in
             * the replacement string may cause the results to be different than if it
             * were being treated as a literal replacement string. Dollar signs may be
             * treated as references to captured subsequences as described above, and
             * backslashes are used to escape literal characters in the replacement
             * string.
             * </p>
             * <p>
             * Given the regular expression @c a*b, the input
             * @c "aabfooaabfooabfoob", and the replacement string
             * @c "-", an invocation of this method on a matcher for that
             * expression would yield the string @c "-foo-foo-foo-".
             * </p>
             * <p>
             * Invoking this method changes this matcher's state.  If the matcher
             * is to be used in further matching operations then it should first be
             * reset.
             * </p>
             *
             * @param  replacement
             *         The replacement string
             *
             * @return  The string constructed by replacing each matching subsequence
             *          by the replacement string, substituting captured subsequences
             *          as needed
             */
            String replaceAll(String const &replacement);

            /**
             * Replaces every subsequence of the input sequence that matches the
             * pattern with the result of applying the given replacer function to the
             * match result of this matcher corresponding to that subsequence.
             * Exceptions thrown by the function are relayed to the caller.
             *
             * <p>
             * This method first resets this matcher.  It then scans the input
             * sequence looking for matches of the pattern.  Characters that are not
             * part of any match are appended directly to the result string; each match
             * is replaced in the result by the applying the replacer function that
             * returns a replacement string.  Each replacement string may contain
             * references to captured subsequences as in the @b appendReplacement method.
             * </p>
             * <p>
             * Note that backslashes (@c \) and dollar signs (@c $) in
             * a replacement string may cause the results to be different than if it
             * were being treated as a literal replacement string. Dollar signs may be
             * treated as references to captured subsequences as described above, and
             * backslashes are used to escape literal characters in the replacement
             * string.
             * </p>
             * <p>
             * Given the regular expression @c dog, the input
             * @c "zzzdogzzzdogzzz", and the function
             * @code [](MatchResult const &mr) { return mr.group().toUpperCase(); } @endcode,
             * an invocation of this method on
             * a matcher for that expression would yield the string
             * @c "zzzDOGzzzDOGzzz".
             * </p>
             * <p>
             * Invoking this method changes this matcher's state.  If the matcher
             * is to be used in further matching operations then it should first be
             * reset.
             * </p>
             *
             * <p>
             * The replacer function should not modify this matcher's state during
             * replacement.  This method will, on a best-effort basis, throw a
             * @b util::ConcurrentModificationException if such modification is
             * detected.
             * </p>
             * <p>
             * The state of each match result passed to the replacer function is
             * guaranteed to be constant only for the duration of the replacer function
             * call and only if the replacer function does not modify this matcher's
             * state.
             * </p>
             * @note
             * This implementation applies the replacer function to this matcher, which
             * is an instance of @c MatchResult.
             *
             * @param  replacer
             *         The function to be applied to the match result of this matcher
             *         that returns a replacement string.
             * @return  The string constructed by replacing each matching subsequence
             *          with the result of applying the replacer function to that
             *          matched subsequence, substituting captured subsequences as
             *          needed.
             * @throws ConcurrentModificationException if it is detected, on a
             *         best-effort basis, that the replacer function modified this
             *         matcher's state
             */
            String replaceAll(function::Function<MatchResult, String> const &replacer);

            /**
             * Replaces the first subsequence of the input sequence that matches the
             * pattern with the given replacement string.
             *
             * <p>
             * This method first resets this matcher.  It then scans the input
             * sequence looking for a match of the pattern.  Characters that are not
             * part of the match are appended directly to the result string; the match
             * is replaced in the result by the replacement string.  The replacement
             * string may contain references to captured subsequences as in the @b appendReplacement method.
             * </p>
             * <p>
             * Note that backslashes (@c \) and dollar signs (@c $) in
             * the replacement string may cause the results to be different from if it
             * were being treated as a literal replacement string. Dollar signs may be
             * treated as references to captured subsequences as described above, and
             * backslashes are used to escape literal characters in the replacement
             * string.
             * </p>
             * <p>
             * Given the regular expression @c dog, the input
             * @c "zzzdogzzzdogzzz", and the replacement string
             * @c "cat", an invocation of this method on a matcher for that
             * expression would yield the string @c "zzzcatzzzdogzzz".
             * </p>
             *
             * <p>
             * Invoking this method changes this matcher's state.  If the matcher
             * is to be used in further matching operations then it should first be
             * reset.
             * </p>
             *
             * @param  replacement
             *         The replacement string
             * @return  The string constructed by replacing the first matching
             *          subsequence by the replacement string, substituting captured
             *          subsequences as needed
             */
            String replaceFirst(String const &replacement);

            /**
             * Replaces the first subsequence of the input sequence that matches the
             * pattern with the result of applying the given replacer function to the
             * match result of this matcher corresponding to that subsequence.
             * Exceptions thrown by the replace function are relayed to the caller.
             *
             * <p>
             * This method first resets this matcher.  It then scans the input
             * sequence looking for a match of the pattern.  Characters that are not
             * part of the match are appended directly to the result string; the match
             * is replaced in the result by the applying the replacer function that
             * returns a replacement string.  The replacement string may contain
             * references to captured subsequences as in the @b appendReplacement method.
             * </p>
             * <p>
             * Note that backslashes (@c \) and dollar signs (@c $) in
             * the replacement string may cause the results to be different than if it
             * were being treated as a literal replacement string. Dollar signs may be
             * treated as references to captured subsequences as described above, and
             * backslashes are used to escape literal characters in the replacement
             * string.
             * </p>
             * <p>
             * Given the regular expression @c dog, the input
             * @c "zzzdogzzzdogzzz", and the function
             * @code [](MatchResult const &mr) { return mr.group().toUpperCase(); } @endcode,
             * an invocation of this method on a matcher for that expression would yield the string
             * @c "zzzDOGzzzdogzzz".
             * </p>
             * <p>
             * Invoking this method changes this matcher's state.  If the matcher
             * is to be used in further matching operations then it should first be
             * reset.
             * </p>
             * <p>
             * The replacer function should not modify this matcher's state during
             * replacement.  This method will, on a best-effort basis, throw a
             * @b util::ConcurrentModificationException if such modification is
             * detected.
             * </p>
             * <p>
             * The state of the match result passed to the replacer function is
             * guaranteed to be constant only for the duration of the replacer function
             * call and only if the replacer function does not modify this matcher's
             * state.
             * </p>
             * @note
             * This implementation applies the replacer function to this matcher, which
             * is an instance of @c MatchResult.
             *
             * @param  replacer
             *         The function to be applied to the match result of this matcher
             *         that returns a replacement string.
             * @return  The string constructed by replacing the first matching
             *          subsequence with the result of applying the replacer function to
             *          the matched subsequence, substituting captured subsequences as
             *          needed.
             * @throws NullPointerException if the replacer function is null
             * @throws ConcurrentModificationException if it is detected, on a
             *         best-effort basis, that the replacer function modified this
             *         matcher's state
             */
            String replaceFirst(function::Function<MatchResult, String> const &replacer);

            /**
             * Sets the limits of this matcher's region. The region is the part of the
             * input sequence that will be searched to find a match. Invoking this
             * method resets the matcher, and then sets the region to start at the
             * index specified by the @c start parameter and end at the
             * index specified by the @c end parameter.
             *
             * <p>
             * Depending on the transparency and anchoring being used (see
             * @b useTransparentBounds and @b useAnchoringBounds), certain
             * constructs such as anchors may behave differently at or around the
             * boundaries of the region.
             * </p>
             * @param  start
             *         The index to start searching at (inclusive)
             * @param  end
             *         The index to end searching at (exclusive)
             * @throws  IndexOutOfBoundsException
             *          If start or end is less than zero, if
             *          start is greater than the length of the input sequence, if
             *          end is greater than the length of the input sequence, or if
             *          start is greater than end.
             * @return  this matcher
             */
            Matcher &region(int start, int end);

            /**
             * Reports the start index of this matcher's region. The
             * searches this matcher conducts are limited to finding matches
             * within @b regionStart (inclusive) and @b regionEnd (exclusive).
             *
             * @return  The starting point of this matcher's region
             */
            gint regionStart() const;

            /**
             * Reports the end index (exclusive) of this matcher's region.
             * The searches this matcher conducts are limited to finding matches
             * within @b regionStart (inclusive) and @b regionEnd (exclusive).
             *
             * @return  the ending point of this matcher's region
             */
            gint regionEnd() const;

            /**
             * Queries the transparency of region bounds for this matcher.
             *
             * <p>
             * This method returns @c true if this matcher uses
             * <i>transparent</i> bounds, @c false if it uses <i>opaque</i>
             * bounds.
             * </p>
             * <p>
             * See @b useTransparentBounds for a description of transparent and opaque bounds.
             * </p>
             * <p>
             * By default, a matcher uses opaque region boundaries.
             * </p>
             * @return @c true iff this matcher is using transparent bounds,
             *         @c false otherwise.
             * @see regex::Matcher::useTransparentBounds(gbool)
             */
            gbool hasTransparentBounds() const;

            /**
             * Sets the transparency of region bounds for this matcher.
             *
             * <p>
             * Invoking this method with an argument of @c true will set this
             * matcher to use <i>transparent</i> bounds. If the boolean
             * argument is @c false, then <i>opaque</i> bounds will be used.
             * </p>
             * <p>
             * Using transparent bounds, the boundaries of this
             * matcher's region are transparent to lookahead, lookbehind,
             * and boundary matching constructs. Those constructs can see beyond the
             * boundaries of the region to see if a match is appropriate.
             * </p>
             * <p>
             * Using opaque bounds, the boundaries of this matcher's
             * region are opaque to lookahead, lookbehind, and boundary matching
             * constructs that may try to see beyond them. Those constructs cannot
             * look past the boundaries so they will fail to match anything outside
             * of the region.
             * </p>
             * <p>
             * By default, a matcher uses opaque bounds.
             * </p>
             * @param  b a boolean indicating whether to use opaque or transparent
             *         regions
             * @return this matcher
             * @see java.util.regex.Matcher#hasTransparentBounds
             */
            Matcher &useTransparentBounds(gbool b);

            /**
             * Queries the anchoring of region bounds for this matcher.
             *
             * <p> This method returns @c true if this matcher uses
             * <i>anchoring</i> bounds, @c false otherwise.
             *
             * <p> See @b useAnchoringBounds for a
             * description of anchoring bounds.
             *
             * <p> By default, a matcher uses anchoring region boundaries.
             *
             * @return @c true iff this matcher is using anchoring bounds,
             *         @c false otherwise.
             * @see java.util.regex.Matcher#useAnchoringBounds(boolean)
             */
            gbool hasAnchoringBounds() const;

            /**
             * Sets the anchoring of region bounds for this matcher.
             *
             * <p> Invoking this method with an argument of @c true will set this
             * matcher to use <i>anchoring</i> bounds. If the boolean
             * argument is @c false, then <i>non-anchoring</i> bounds will be
             * used.
             *
             * <p> Using anchoring bounds, the boundaries of this
             * matcher's region match anchors such as ^ and $.
             *
             * <p> Without anchoring bounds, the boundaries of this
             * matcher's region will not match anchors such as ^ and $.
             *
             * <p> By default, a matcher uses anchoring region boundaries.
             *
             * @param  b a boolean indicating whether or not to use anchoring bounds.
             * @return this matcher
             * @see java.util.regex.Matcher#hasAnchoringBounds
             */
            Matcher &useAnchoringBounds(gbool b);

            /**
             * <p>Returns the string representation of this matcher. The
             * string representation of a @c Matcher contains information
             * that may be useful for debugging. The exact format is unspecified.
             *
             * @return  The string representation of this matcher
             */
            String toString() const override;

            /**
             * <p>Returns true if the end of input was hit by the search engine in
             * the last match operation performed by this matcher.
             *
             * <p>When this method returns true, then it is possible that more input
             * would have changed the result of the last search.
             *
             * @return  true iff the end of input was hit in the last match; false
             *          otherwise
             */
            gbool hitEnd() const;

            /**
             * <p>
             * Returns true if more input could change a positive match into a
             * negative one.
             * </p>
             * <p>
             * If this method returns true, and a match was found, then more
             * input could cause the match to be lost. If this method returns false
             * and a match was found, then more input might change the match but the
             * match won't be lost. If a match was not found, then requireEnd has no
             * meaning.
             * </p>
             * @return  true iff more input could change a positive match into a
             *          negative one.
             */
            gbool requiresEnd() const;

            util::Map<String, Integer> &namedGroups() const override;

            gbool hasMatch() const override;

            gbool equals(const Object &o) const override;

            Object &clone() const override;

        private:
            gint minStart() const;

            gint maxEnd() const;

            /**
             * Processes replacement string to replace group references with
             * groups.
             */
            void appendExpandedReplacement(Appendable &app, String const &replacement) const;

            /**
             * Initiates a search to find a Pattern within the given bounds.
             * The groups are filled with default values and the match of the root
             * of the state machine is called. The state machine will hold the state
             * of the match as it proceeds in this matcher.
             *
             * Matcher.from is not set here, because it is the "hard" boundary
             * of the start of the search which anchors will set to. The from param
             * is the "soft" boundary of the start of the search, meaning that the
             * regex tries to match at that index but ^ won't match there. Subsequent
             * calls to the search methods start at a new "soft" boundary which is
             * the end of the previous match.
             */
            gbool search(gint from);

            /**
             * Initiates a search for an anchored match to a Pattern within the given
             * bounds. The groups are filled with default values and the match of the
             * root of the state machine is called. The state machine will hold the
             * state of the match as it proceeds in this matcher.
             */
            gbool match(gint from, gint anchor);

            /**
             * Returns the end index of the text.
             *
             * @return the index after the last character in the text
             */
            gint getTextLength() const;

            /**
             * Generates a String from this matcher's input in the specified range.
             *
             * @param  beginIndex   the beginning index, inclusive
             * @param  endIndex     the ending index, exclusive
             * @return A String generated from this matcher's input
             */
            CharSequence &subSequence(gint beginIndex, gint endIndex) const;

            /**
             * Returns this matcher's input character at index i.
             *
             * @return A char from the specified index
             */
            gchar charAt(gint i) const;

            /**
             * Returns the group index of the matched capturing group.
             *
             * @return the index of the named-capturing group
             */
            gint getMatchedGroupIndex(String const &name) const;

            void checkGroup(gint group) const;

            void checkMatch() const;
        };
    } // regex
} // core

#endif //CORE24_MATCHER_H
