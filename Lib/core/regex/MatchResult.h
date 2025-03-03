//
// Created by bruns on 14/10/2024.
//

#ifndef CORE24_MATCHRESULT_H
#define CORE24_MATCHRESULT_H

#include <core/lang/Object.h>

namespace core {
    namespace regex {
        /**
         * The result of a match operation.
         *
         * <p>
         * This interface contains query methods used to determine the
         * results of a match against a regular expression. The match boundaries,
         * groups and group boundaries can be seen but not modified through
         * a @c MatchResult.
         * </p>
         * @note
         * Support for named groups is implemented by the default methods
         * @b start(String), @b end(String) and @b group(String).
         * They all make use of the map returned by @b namedGroups(), whose
         * default implementation simply throws @b UnsupportedOperationException.
         * It is thus sufficient to override @b namedGroups() for these methods
         * to work. However, overriding them directly might be preferable for
         * performance or other reasons.
         *
         * @see Matcher
         */
        class MatchResult : public virtual Object {
        public:
            /**
             * Returns the start index of the match.
             *
             * @return  The index of the first character matched
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             */
            virtual gint start() const = 0;

            /**
             * Returns the start index of the subsequence captured by the given group
             * during this match.
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
            virtual gint start(gint group) const = 0;

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
             *
             * @note
             * The default implementation of this method invokes @b namedGroups()
             * to obtain the group number from the @c name argument, and uses it
             * as argument to an invocation of @b #start(gint).
             */
            virtual gint start(String const& name) const;

            /**
             * Returns the offset after the last character matched.
             *
             * @return  The offset after the last character matched
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             */
            virtual gint end() const = 0;

            /**
             * Returns the offset after the last character of the subsequence
             * captured by the given group during this match.
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
            virtual gint end(gint group) const = 0;

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
             *
             * @note
             * The default implementation of this method invokes @b namedGroups()
             * to obtain the group number from the @c name argument, and uses it
             * as argument to an invocation of @b end(gint).
             */
            virtual gint end(String const& name) const;

            /**
             * Returns the input subsequence matched by the previous match.
             *
             * <p>
             * For a matcher <i>m</i> with input sequence <i>s</i>,
             * the expressions <i>m.</i>@c group() and
             * <i>s.</i>@c subString(<i>m.</i>@c start(), <i>m.</i>@c end())
             * are equivalent.
             * </p>
             *
             * <p>
             * Note that some patterns, for example @c a*, match the empty
             * string.  This method will return the empty string when the pattern
             * successfully matches the empty string in the input.
             * </p>
             *
             * @return The (possibly empty) subsequence matched by the previous match,
             *         in string form
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             */
            virtual String group() const = 0;

            /**
             * Returns the input subsequence captured by the given group during the
             * previous match operation.
             *
             * <p>
             * For a matcher <i>m</i>, input sequence <i>s</i>, and group index
             * <i>g</i>, the expressions <i>m.</i>@c group(<i>g</i>@c ) and
             * <i>s.</i>@c subString(<i>m.</i>@c start(<i>g</i>@c ), <i>m.</i>@c end(<i>g</i>@c ))
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
             * any part of the input sequence, then empty String is returned. Note
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
             *          failed to match part of the input
             *
             * @throws  IllegalStateException
             *          If no match has yet been attempted,
             *          or if the previous match operation failed
             *
             * @throws  IndexOutOfBoundsException
             *          If there is no capturing group in the pattern
             *          with the given index
             */
            virtual String group(gint group) const = 0;

            /**
             * Returns the input subsequence captured by the given
             * <a href="Pattern.html#groupname">named-capturing group</a> during the
             * previous match operation.
             *
             * <p>
             * If the match was successful but the group specified failed to match
             * any part of the input sequence, then @c "" is returned. Note
             * that some groups, for example @c (a*), match the empty string.
             * This method will return the empty string when such a group successfully
             * matches the empty string in the input.
             * </p>
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
             *
             * @note
             * The default implementation of this method invokes @b namedGroups()
             * to obtain the group number from the @c name argument, and uses it
             * as argument to an invocation of @b group(int).
             */
            virtual String group(String const& name) const;

            /**
             * Returns the number of capturing groups in this match result's pattern.
             *
             * <p> Group zero denotes the entire pattern by convention. It is not
             * included in this count.
             * </p>
             * <p> Any non-negative integer smaller than or equal to the value
             * returned by this method is guaranteed to be a valid group index for
             * this matcher.
             * </p>
             *
             * @return The number of capturing groups in this matcher's pattern
             */
            virtual gint groupCount() const = 0;

            /**
             * Returns an unmodifiable map from capturing group names to group numbers.
             * If there are no named groups, returns an empty map.
             *
             * @return an unmodifiable map from capturing group names to group numbers
             *
             * @throws UnsupportedOperationException if the implementation does not
             *          support named groups.
             *
             * @note The default implementation of this method always throws
             *          @b UnsupportedOperationException
             *
             * @details
             * This method must be overridden by an implementation that supports
             * named groups.
             */
            virtual util::Map<String, Integer> const& namedGroups() const;

            /**
             * Returns whether @c this contains a valid match from
             * a previous match or find operation.
             *
             * @return whether @c this contains a valid match
             *
             * @throws UnsupportedOperationException if the implementation cannot report
             *          whether it has a match
             *
             * @note The default implementation of this method always throws
             *          @b UnsupportedOperationException
             */
            virtual gbool hasMatch() const;

        private:
            gint groupNumber(String const& name) const;
        };
    } // regex
} // core

#endif //CORE24_MATCHRESULT_H
