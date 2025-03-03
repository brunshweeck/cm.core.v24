//
// Created by brunshweeck on 6 sept. 2024.
//

#ifndef CORE24_SURROGATE_H
#define CORE24_SURROGATE_H

#include <core/lang/AssertionError.h>
#include <core/lang/Character.h>
#include <core/charset/Charset.h>
#include <core/charset/CoderResult.h>
#include <core/io/CharBuffer.h>

namespace core {
    namespace charset {
        /**
         * Utility class for dealing with surrogates.
         */
        class Surrogate final : public Object {

            CORE_IMPLICIT Surrogate() = default;

        public:
            // TODO: Deprecate/remove the following redundant definitions
            static CORE_FAST gchar MIN_HIGH = Character::MIN_HIGH_SURROGATE;
            static CORE_FAST gchar MAX_HIGH = Character::MAX_HIGH_SURROGATE;
            static CORE_FAST gchar MIN_LOW = Character::MIN_LOW_SURROGATE;
            static CORE_FAST gchar MAX_LOW = Character::MAX_LOW_SURROGATE;
            static CORE_FAST gchar MIN = Character::MIN_SURROGATE;
            static CORE_FAST gchar MAX = Character::MAX_SURROGATE;
            static CORE_FAST gint UCS4_MIN = Character::MIN_SUPPLEMENTARY;
            static CORE_FAST gint UCS4_MAX = Character::MAX_CODE_POINT;

            /**
             * Tells whether the given value is in the high surrogate range.
             * Use of @b Character::isHighSurrogate is generally preferred.
             */
            static gbool isHigh(gint c);

            /**
             * Tells whether the given value is in the low surrogate range.
             * Use of @b Character::isLowSurrogate is generally preferred.
             */
            static gbool isLow(gint c);

            /**
             * Tells whether the given value is in the surrogate range.
             * Use of @b Character::isSurrogate is generally preferred.
             */
            static gbool is(gint c);

            /**
             * Tells whether the given UCS-4 character must be represented as a
             * surrogate pair in UTF-16.
             * Use of @b Character::isSupplementary is generally preferred.
             */
            static gbool neededFor(gint uc);

            /**
             * Returns the high UTF-16 surrogate for the given supplementary UCS-4 character.
             * Use of @b Character::highSurrogate is generally preferred.
             */
            static gchar high(gint uc);

            /**
             * Returns the low UTF-16 surrogate for the given supplementary UCS-4 character.
             * Use of @b Character::lowSurrogate is generally preferred.
             */
            static gchar low(gint uc);

            /**
             * Converts the given surrogate pair into a 32-bit UCS-4 character.
             * Use of @b Character::toCodePoint is generally preferred.
             */
            static gint toUCS4(gchar c, gchar d);

            /**
             * Surrogate parsing support.  Charset implementations may use instances of
             * this class to handle the details of parsing UTF-16 surrogate pairs.
             */
            class Parser final : public Object {

                gint chr = -1; // UCS-4
                CoderResult err = CoderResult::UNDERFLOW;
                gbool isUPair = false;

            public:
                CORE_IMPLICIT Parser() = default;

                /**
                 * Returns the UCS-4 character previously parsed.
                 */
                gint character() const;

                /**
                 * Tells whether the previously-parsed UCS-4 character was
                 * originally represented by a surrogate pair.
                 */
                gbool isPair() const;

                /**
                 * Returns the number of UTF-16 characters consumed by the previous
                 * parse.
                 */
                gint increment() const;

                /**
                 * If the previous parse operation detected an error, return the object
                 * describing that error.
                 */
                CoderResult error();

                /**
                 * Returns an unmappable-input result object, with the appropriate
                 * input length, for the previously-parsed character.
                 */
                CoderResult unmappableResult() const;

                /**
                 * Parses a UCS-4 character from the given source buffer, handling
                 * surrogates.
                 *
                 * @param  c    The first character
                 * @param  in   The source buffer, from which one more character
                 *              will be consumed if c is a high surrogate
                 *
                 * @return  Either a parsed UCS-4 character, in which case the isPair()
                 *          and increment() methods will return meaningful values, or
                 *          -1, in which case error() will return a descriptive result
                 *          object
                 */
                gint parse(gchar c, io::CharBuffer &in);

                /**
                 * Parses a UCS-4 character from the given source buffer, handling
                 * surrogates.
                 *
                 * @param  c    The first character
                 * @param  ia   The input array, from which one more character
                 *              will be consumed if c is a high surrogate
                 * @param  ip   The input index
                 * @param  il   The input limit
                 *
                 * @return  Either a parsed UCS-4 character, in which case the isPair()
                 *          and increment() methods will return meaningful values, or
                 *          -1, in which case error() will return a descriptive result
                 *          object
                 */
                gint parse(gchar c, CharArray const &ia, gint ip, gint il);
            };

            /**
             * Surrogate generation support.  Charset implementations may use instances
             * of this class to handle the details of generating UTF-16 surrogate
             * pairs.
             */
            class Generator final: public Object {
                CoderResult err = CoderResult::OVERFLOW;

            public:
                CORE_IMPLICIT Generator() = default;

                /**
                 * If the previous generation operation detected an error, return the
                 * object describing that error.
                 */
                CoderResult error();

                /**
                 * Generates one or two UTF-16 characters to represent the given UCS-4
                 * character.
                 *
                 * @param  uc   The UCS-4 character
                 * @param  len  The number of input bytes from which the UCS-4 value
                 *              was constructed (used when creating result objects)
                 * @param  dst  The destination buffer, to which one or two UTF-16
                 *              characters will be written
                 *
                 * @return  Either a positive count of the number of UTF-16 characters
                 *          written to the destination buffer, or -1, in which case
                 *          error() will return a descriptive result object
                 */
                gint generate(gint uc, gint len, io::CharBuffer &dst);

                /**
                 * Generates one or two UTF-16 characters to represent the given UCS-4
                 * character.
                 *
                 * @param  uc   The UCS-4 character
                 * @param  len  The number of input bytes from which the UCS-4 value
                 *              was constructed (used when creating result objects)
                 * @param  da   The destination array, to which one or two UTF-16
                 *              characters will be written
                 * @param  dp   The destination position
                 * @param  dl   The destination limit
                 *
                 * @return  Either a positive count of the number of UTF-16 characters
                 *          written to the destination buffer, or -1, in which case
                 *          error() will return a descriptive result object
                 */
                gint generate(gint uc, gint len, CharArray &da, gint dp, gint dl);
            };
        };
    } // charset
} // core

#endif //CORE24_SURROGATE_H
