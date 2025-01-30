//
// Created by brunshweeck on 17 sept. 2024.
//

#ifndef CORE24_STRINGJOINER_H
#define CORE24_STRINGJOINER_H

#include <core/StringArray.h>

namespace core {
    namespace util {
        /**
         * @c StringJoiner is used to construct a sequence of characters separated
         * by a delimiter and optionally starting with a supplied prefix
         * and ending with a supplied suffix.
         * <p>
         * Prior to adding something to the @c StringJoiner, its
         * @c sj.toString() method will, by default, return @code prefix + suffix @endcode.
         * However, if the @c setEmptyValue method is called, the @c empty
         * supplied will be returned instead. This can be used, for example, when
         * creating a string using set notation to indicate an empty set, i.e.
         * <code>"{}"</code>, where the @c prefix is <code>"{"</code>, the
         * @c suffix is <code>"}"</code> and nothing has been added to the
         * @c StringJoiner.
         * </p>
         * @note
         * <p>
         * The String @c "[George:Sally:Fred]" may be constructed as follows:
         *
         * @code
         *   StringJoiner sj = StringJoiner(":"_S, "["_S, "]"_S);
         *   sj.add("George"_S).add("Sally").add("Fred");
         *   String desiredString = sj.toString();
         * @endcode
         * </p>
        */
        class StringJoiner final : public Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            String delimiter;
            String prefix;
            String suffix;

            /** Contains all the string components added so far. */
            StringArray elts;

            /** The number of string components added so far. */
            gint count;

            /** Total length in chars so far, excluding prefix and suffix. */
            gint capacity;

            /**
             * When overridden by the user to be non-null via @b setEmptyValue(CharSequence),
             * the string returned by toString() when no elements have yet been added.
             * When null, prefix + suffix is used as the empty value.
             */
            String empty;

        public:
            /**
             * Constructs a @c StringJoiner with no characters in it, with no
             * @c prefix or @c suffix, and a copy of the supplied
             * @c delimiter.
             * If no characters are added to the @c StringJoiner and methods
             * accessing the value of it are invoked, it will not return a
             * @c prefix or @c suffix (or properties thereof) in the result,
             * unless @c setEmptyValue has first been called.
             *
             * @param  delimiter the sequence of characters to be used between each
             *         element added to the @c StringJoiner value
             */
            CORE_EXPLICIT StringJoiner(CharSequence const& delimiter);

            /**
             * Constructs a @c StringJoiner with no characters in it using copies
             * of the supplied @c prefix, @c delimiter and @c suffix.
             * If no characters are added to the @c StringJoiner and methods
             * accessing the string value of it are invoked, it will return the
             * @code prefix + suffix @endcode (or properties thereof) in the result, unless
             * @c setEmptyValue has first been called.
             *
             * @param  delimiter the sequence of characters to be used between each
             *         element added to the @c StringJoiner
             * @param  prefix the sequence of characters to be used at the beginning
             * @param  suffix the sequence of characters to be used at the end
             */
            CORE_EXPLICIT StringJoiner(CharSequence const& delimiter,
                                       CharSequence const& prefix,
                                       CharSequence const& suffix);

            /**
             * Sets the sequence of characters to be used when determining the string
             * representation of this @c StringJoiner and no elements have been
             * added yet, that is, when it is empty.  A copy of the @c empty
             * parameter is made for this purpose. Note that once an add method has been
             * called, the @c StringJoiner is no longer considered empty, even if
             * the element(s) added correspond to the empty @c String.
             *
             * @param  emptyValue the characters to return as the value of an empty
             *         @c StringJoiner
             * @return this @c StringJoiner itself so the calls may be chained
             */
            StringJoiner& setEmptyValue(CharSequence const& emptyValue);

            /**
             * Returns the current value, consisting of the @c prefix, the values
             * added so far separated by the @c delimiter, and the @c suffix,
             * unless no elements have been added in which case, the
             * @code prefix + suffix @endcode or the @c empty characters are returned.
             *
             * @return the string representation of this @c StringJoiner
             */
            String toString() const override;

            /**
             * Adds a copy of the given @c CharSequence value as the next
             * element of the @c StringJoiner value.
             *
             * @param  newElement The element to add
             * @return a reference to this @c StringJoiner
             */
            StringJoiner& add(CharSequence const& newElement);

            /**
             * Adds the contents of the given @c StringJoiner without prefix and
             * suffix as the next element if it is non-empty. If the given
             * @c StringJoiner is empty, the call has no effect.
             *
             * <p>
             * A @c StringJoiner is empty if @b add()
             * has never been called, and if @c merge() has never been called
             * with a non-empty @c StringJoiner argument.
             * </p>
             * <p>
             * If the other @c StringJoiner is using a different delimiter,
             * then elements from the other @c StringJoiner are concatenated with
             * that delimiter and the result is appended to this @c StringJoiner
             * as a single element.
             * </p>
             * @param other The @c StringJoiner whose contents should be merged
             *              into this one
             */
            StringJoiner& merge(StringJoiner const& other);

            /**
             * Returns the length of the @c String representation
             * of this @c StringJoiner. Note that if
             * no add methods have been called, then the length of the @c String
             * representation (either @code prefix + suffix @endcode or @c empty)
             * will be returned. The value should be equivalent to
             * @code toString().length() @endcode .
             *
             * @return the length of the current value of @c StringJoiner
             */
            gint length() const;

        private:
            gint checkAddLength(gint oldLength, gint inc) const;

            String compactElements() const;
        };
    } // util
} // core

#endif //CORE24_STRINGJOINER_H
