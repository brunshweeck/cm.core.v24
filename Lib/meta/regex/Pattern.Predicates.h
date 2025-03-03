//
// Created by bruns on 17/10/2024.
//

#ifndef CORE24_CHARPREDICATES_H
#define CORE24_CHARPREDICATES_H

#include <core/lang/BooleanArray.h>
#include <core/function/IntPredicate.h>
#include <core/regex/Pattern.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;
    using namespace function;

    namespace regex {
        static IntPredicate ALWAYS_FALSE = [](gint) -> gbool { return false; };

        class Pattern::CharPredicate : public Object {
        public:
            IntPredicate predicate = ALWAYS_FALSE;

            CharPredicate() = default;

            CORE_EXPLICIT CharPredicate(IntPredicate predicate);

            virtual Optional<CharPredicate> logicalAnd(const Optional<CharPredicate> &other) const;

            virtual Optional<CharPredicate> negate() const;

            virtual Optional<CharPredicate> logicalOr(const Optional<CharPredicate> &other) const;

            virtual Optional<CharPredicate> logicalOr(const Optional<CharPredicate> &second,
                                                      Optional<CharPredicate> const &third) const;

            virtual gbool test(gint c) const;

            Object &clone() const override;
        };

        class Pattern::BmpCharPredicate : public CharPredicate {
        public:
            BmpCharPredicate() = default;

            CORE_EXPLICIT BmpCharPredicate(IntPredicate predicate);
        };

        class Pattern::BitClass final : public BmpCharPredicate {
        public:
            BooleanArray bits;

            BitClass();

            BitClass &add(gint c, gint flags);

            gbool test(gint ch) const override;
        };

        class Pattern::Predicates final : public Object {
        public:
            static Optional<CharPredicate> ALPHABETIC();

            // \p{gc=Decimal_Number}
            static Optional<CharPredicate> DIGIT();

            static Optional<CharPredicate> LETTER();

            static Optional<CharPredicate> IDEOGRAPHIC();

            static Optional<CharPredicate> LOWERCASE();

            static Optional<CharPredicate> UPPERCASE();

            static Optional<CharPredicate> TITLECASE();

            // \p{Whitespace}
            static Optional<CharPredicate> WHITE_SPACE();

            // \p{gc=Control}
            static Optional<CharPredicate> CONTROL();

            // \p{gc=Punctuation}
            static Optional<CharPredicate> PUNCTUATION();

            // \p{gc=Decimal_Number}
            // \p{Hex_Digit}    -> PropList.txt: Hex_Digit
            static Optional<CharPredicate> HEX_DIGIT();

            static Optional<CharPredicate> ASSIGNED();

            // PropList.txt:Non-character
            static Optional<CharPredicate> NON_CHARACTER();

            // \p{alpha}
            // \p{digit}
            static Optional<CharPredicate> ALNUM();

            // \p{Whitespace} --
            // [\N{LF} \N{VT} \N{FF} \N{CR} \N{NEL}  -> 0xa, 0xb, 0xc, 0xd, 0x85
            //  \p{gc=Line_Separator}
            //  \p{gc=Paragraph_Separator}]
            static Optional<CharPredicate> BLANK();

            // [^
            //  \p{space}
            //  \p{gc=Control}
            //  \p{gc=Surrogate}
            //  \p{gc=Unassigned}]
            static Optional<CharPredicate> GRAPH();

            // \p{graph}
            // \p{blank}
            // -- \p{cntrl}
            static Optional<CharPredicate> PRINT();

            //  200C..200D    PropList.txt:Join_Control
            static Optional<CharPredicate> JOIN_CONTROL();

            //  \p{alpha}
            //  \p{gc=Mark}
            //  \p{digit}
            //  \p{gc=Connector_Punctuation}
            //  \p{Join_Control}    200C..200D
            static Optional<CharPredicate> WORD();

            /////////////////////////////////////////////////////////////////////////////

            static Optional<CharPredicate> getPosixPredicate(String const &name, gbool caseIns);

            static Optional<CharPredicate> getUnicodePredicate(String const &name, gbool caseIns);

            static Optional<CharPredicate> forUnicodeProperty(String const &name, gbool caseIns);

            /////////////////////////////////////////////////////////////////////////////

            /**
             * Returns a predicate matching all characters belong to a named
             * UnicodeScript.
             */
            static Optional<CharPredicate> forUnicodeScript(String const &name);

            /**
             * Returns a predicate matching all characters in a UnicodeBlock.
             */
            static Optional<CharPredicate> forUnicodeBlock(String const &name);

            /////////////////////////////////////////////////////////////////////////////

            // unicode categories, aliases, properties, java methods ...
            static Optional<CharPredicate> forProperty(const String &name, gbool caseIns);

            static Optional<CharPredicate> category(gint typeMask);

            static Optional<CharPredicate> range(gint lower, gint upper);

            static Optional<CharPredicate> ctype(gint ctype);

            /////////////////////////////////////////////////////////////////////////////

            /**
             * Posix ASCII variants, not in the lookup map
             */
            static Optional<CharPredicate> ASCII_DIGIT();

            static Optional<CharPredicate> ASCII_WORD();

            static Optional<CharPredicate> ASCII_SPACE();

            /////////////////////////////////////////////////////////////////////////////

            /**
             * Emoji related binary properties
             */
            static Optional<CharPredicate> EMOJI();

            static Optional<CharPredicate> EMOJI_PRESENTATION();

            static Optional<CharPredicate> EMOJI_MODIFIER();

            static Optional<CharPredicate> EMOJI_MODIFIER_BASE();

            static Optional<CharPredicate> EMOJI_COMPONENT();

            static Optional<CharPredicate> EXTENDED_PICTOGRAPHIC();

            static Optional<CharPredicate> forPOSIXName(String const &name, gbool ci);
        };
    } // regex
} // core

#endif //CORE24_CHARPREDICATES_H
