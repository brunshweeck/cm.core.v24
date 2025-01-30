//
// Created by bruns on 17/10/2024.
//

#include <meta/regex/Pattern.Predicates.h>

#include "Pattern.ASCII.h"
#include "Pattern.Self.h"

namespace core {
    namespace regex {
        //
        Pattern::CharPredicate::CharPredicate(IntPredicate predicate) {
            this->predicate = UNSAFE::moveInstance(predicate);
        }

        Optional<Pattern::CharPredicate>
        Pattern::CharPredicate::logicalAnd(const Optional<CharPredicate>& other) const {
            CharPredicate const& p2 = other.get();
            if (Class<BmpCharPredicate>::hasInstance(*this) && Class<BmpCharPredicate>::hasInstance(p2))
                return UNSAFE::newInstance<BmpCharPredicate>([&](gint ch) -> gbool { return test(ch) && p2.test(ch); });
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return test(ch) && p2.test(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::CharPredicate::negate() const {
            if (Class<BmpCharPredicate>::hasInstance(*this))
                return UNSAFE::newInstance<BmpCharPredicate>([&](gint ch) -> gbool { return !test(ch); });
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return !test(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::CharPredicate::logicalOr(const Optional<CharPredicate>& other) const {
            CharPredicate const& p2 = other.get();
            if (Class<BmpCharPredicate>::hasInstance(*this) && Class<BmpCharPredicate>::hasInstance(p2))
                return UNSAFE::newInstance<BmpCharPredicate>([&](gint ch) -> gbool { return test(ch) || p2.test(ch); });
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return test(ch) || p2.test(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::CharPredicate::logicalOr(const Optional<CharPredicate>& second,
                                                                           Optional<CharPredicate> const& third) const {
            CharPredicate const& p2 = second.get();
            CharPredicate const& p3 = third.get();
            if (Class<BmpCharPredicate>::hasInstance(*this) && Class<BmpCharPredicate>::hasInstance(p2)
                && Class<BmpCharPredicate>::hasInstance(p3)) {
                return UNSAFE::newInstance<BmpCharPredicate>(
                    [&](gint ch) -> gbool { return test(ch) || p2.test(ch) || p3.test(ch); });
            }
            return UNSAFE::newInstance<CharPredicate>(
                [&](gint ch) -> gbool { return test(ch) || p2.test(ch) || p3.test(ch); });
        }

        gbool Pattern::CharPredicate::test(gint c) const { return predicate.test(c); }

        Object& Pattern::CharPredicate::clone() const { return (CharPredicate&)*this; }

        Pattern::BmpCharPredicate::BmpCharPredicate(IntPredicate predicate) {
            this->predicate = UNSAFE::moveInstance(predicate);
        }

        Pattern::BitClass::BitClass() {
            this->bits = BooleanArray(256);
        }

        Pattern::BitClass& Pattern::BitClass::add(gint c, gint flags) {
            CORE_ASSERT(c >= 0 && c <= 255);
            if ((flags & CASE_INSENSITIVE) != 0) {
                if (ASCII::isAscii(c)) {
                    bits[ASCII::toUpper(c)] = true;
                    bits[ASCII::toLower(c)] = true;
                } else if ((flags & UNICODE_CASE) != 0) {
                    bits[Character::toLowerCase(c)] = true;
                    bits[Character::toUpperCase(c)] = true;
                }
            }
            bits[c] = true;
            return *this;
        }

        gbool Pattern::BitClass::test(gint ch) const {
            return ch < 256 && bits[ch];
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::ALPHABETIC() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return Character::isAlphabetic(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::DIGIT() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return Character::isDigit(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::LETTER() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return Character::isLetter(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::IDEOGRAPHIC() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return Character::isIdeographic(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::LOWERCASE() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return Character::isLowerCase(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::UPPERCASE() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return Character::isUpperCase(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::TITLECASE() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return Character::isTitleCase(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::WHITE_SPACE() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool {
                return (((
                            (1 << Character::SPACE_SEPARATOR) |
                            (1 << Character::LINE_SEPARATOR) |
                            (1 << Character::PARAGRAPH_SEPARATOR)) >> Character::category(ch)) & 1) != 0
                        || (ch >= 0x9 && ch <= 0xd) || (ch == 0x85);
            });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::CONTROL() {
            return UNSAFE::newInstance<CharPredicate>(
                [&](gint ch) -> gbool { return Character::category(ch) == Character::CONTROL; });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::PUNCTUATION() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool {
                return ((((1 << Character::CONNECTOR_PUNCTUATION) |
                    (1 << Character::DASH_PUNCTUATION) |
                    (1 << Character::START_PUNCTUATION) |
                    (1 << Character::END_PUNCTUATION) |
                    (1 << Character::OTHER_PUNCTUATION) |
                    (1 << Character::INITIAL_QUOTE_PUNCTUATION) |
                    (1 << Character::FINAL_QUOTE_PUNCTUATION)) >> Character::category(ch)) & 1) != 0;
            });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::HEX_DIGIT() {
            return DIGIT().get().logicalOr(
                UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool {
                    return (ch >= 0x0030 && ch <= 0x0039) ||
                            (ch >= 0x0041 && ch <= 0x0046) ||
                            (ch >= 0x0061 && ch <= 0x0066) ||
                            (ch >= 0xFF10 && ch <= 0xFF19) ||
                            (ch >= 0xFF21 && ch <= 0xFF26) ||
                            (ch >= 0xFF41 && ch <= 0xFF46);
                })
            );
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::ASSIGNED() {
            return UNSAFE::newInstance<CharPredicate>(
                [&](gint ch) -> gbool { return Character::category(ch) != Character::UNASSIGNED; });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::NON_CHARACTER() {
            return UNSAFE::newInstance<CharPredicate>(
                [&](gint ch) -> gbool { return (ch & 0xfffe) == 0xfffe || (ch >= 0xfdd0 && ch <= 0xfdef); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::ALNUM() {
            return ALPHABETIC().get().logicalOr(DIGIT());
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::BLANK() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool {
                return Character::category(ch) == Character::SPACE_SEPARATOR || ch == 0x9; // \N{HT}
            });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::GRAPH() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool {
                return ((((1 << Character::SPACE_SEPARATOR) |
                            (1 << Character::LINE_SEPARATOR) |
                            (1 << Character::PARAGRAPH_SEPARATOR) |
                            (1 << Character::CONTROL) |
                            (1 << Character::SURROGATE) |
                            (1 << Character::UNASSIGNED)) >> Character::category(ch)) & 1)
                        == 0;
            });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::PRINT() {
            return DIGIT().get()
                          .logicalOr(BLANK()).get()
                          .logicalAnd(CONTROL().get().negate());
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::JOIN_CONTROL() {
            return UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool { return ch == 0x200C || ch == 0x200D; });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::WORD() {
            return ALPHABETIC().get().logicalOr(UNSAFE::newInstance<CharPredicate>([&](gint ch) -> gbool {
                                                    return ((((1 << Character::NON_SPACING_MARK) |
                                                            (1 << Character::ENCLOSING_MARK) |
                                                            (1 << Character::COMBINING_SPACING_MARK) |
                                                            (1 << Character::DECIMAL_DIGIT_NUMBER) |
                                                            (1 << Character::CONNECTOR_PUNCTUATION))
                                                        >> Character::category(ch)) & 1) != 0;
                                                }),
                                                JOIN_CONTROL());
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::getPosixPredicate(String const& name, gbool caseIns) {
            if (name.length() == 5) {
                gchar c = name.charAt(0);
                if (c == 'A' && name == "ALPHA") return ALPHABETIC();
                if (c == 'L' && name == "LOWER")
                    return caseIns
                               ? LOWERCASE().get().logicalOr(UPPERCASE(), TITLECASE())
                               : LOWERCASE();
                if (c == 'U' && name == "UPPER")
                    return caseIns
                               ? UPPERCASE().get().logicalOr(LOWERCASE(), TITLECASE())
                               : UPPERCASE();
                if (c == 'S' && name == "SPACE") return WHITE_SPACE();
                if (c == 'P' && name == "PUNCT") return WHITE_SPACE();
                if (c == 'A' && name == "ALNUM") return ALNUM();
                if (c == 'C' && name == "CNTRL") return CONTROL();
                if (c == 'D' && name == "DIGIT") return DIGIT();
                if (c == 'B' && name == "BLANK") return BLANK();
                if (c == 'G' && name == "GRAPH") return GRAPH();
                if (c == 'P' && name == "PRINT") return PRINT();
            }

            if (name.length() == 6) {
                if (name.charAt(0) == 'X' && name == "XDIGIT") return HEX_DIGIT();
            }
            return { };
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::getUnicodePredicate(String const& name, gbool caseIns) {
            if (name.length() < 4)
                return { };
            gchar c = name.charAt(0);
            if (c == 'A' && name == "ALPHABETIC") return ALPHABETIC();
            if (c == 'A' && name == "ASSIGNED") return ASSIGNED();
            if (c == 'C' && name == "CONTROL") return CONTROL();
            if (c == 'E' && name == "EMOJI") return EMOJI();
            if (c == 'E' && name == "EMOJI_PRESENTATION") return EMOJI_PRESENTATION();
            if (c == 'E' && name == "EMOJI_MODIFIER") return EMOJI_MODIFIER();
            if (c == 'E' && name == "EMOJI_MODIFIER_BASE") return EMOJI_MODIFIER_BASE();
            if (c == 'E' && name == "EMOJI_COMPONENT") return EMOJI_COMPONENT();
            if (c == 'E' && name == "EXTENDED_PICTOGRAPHIC") return EXTENDED_PICTOGRAPHIC();
            if (c == 'H' && name == "HEX_DIGIT") return HEX_DIGIT();
            if (c == 'H' && name == "HEXDIGIT") return HEX_DIGIT();
            if (c == 'I' && name == "IDEOGRAPHIC") return IDEOGRAPHIC();
            if (c == 'J' && name == "JOIN_CONTROL") return JOIN_CONTROL();
            if (c == 'J' && name == "JOINCONTROL") return JOIN_CONTROL();
            if (c == 'L' && name == "LETTER") return LETTER();
            if (c == 'L' && name == "LOWERCASE")
                return caseIns
                           ? LOWERCASE().get().logicalOr(UPPERCASE(), TITLECASE())
                           : LOWERCASE();
            if (c == 'N' && name == "NON_CHARACTER") return NON_CHARACTER();
            if (c == 'N' && name == "NONCHARACTER") return NON_CHARACTER();
            if (c == 'T' && name == "TITLECASE")
                return caseIns
                           ? TITLECASE().get().logicalOr(LOWERCASE(), UPPERCASE())
                           : TITLECASE();
            if (c == 'P' && name == "PUNCTUATION") return PUNCTUATION();
            if (c == 'U' && name == "UPPERCASE")
                return caseIns
                           ? UPPERCASE().get().logicalOr(LOWERCASE(), TITLECASE())
                           : UPPERCASE();
            if (c == 'W' && name == "WHITESPACE") return WHITE_SPACE();
            if (c == 'W' && name == "WHITE_SPACE") return WHITE_SPACE();
            if (c == 'W' && name == "WORD") return WORD();

            return { };
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::forUnicodeProperty(String const& name, gbool caseIns) {
            String prop = name.toUpperCase(Locale::ROOT);
            Optional<CharPredicate> p = getUnicodePredicate(prop, caseIns);
            if (p.isPresent())
                return p;
            return getPosixPredicate(prop, caseIns);
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::forUnicodeScript(String const& name) {
            return UNSAFE::newInstance<CharPredicate>();
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::forUnicodeBlock(String const& name) {
            return UNSAFE::newInstance<CharPredicate>();
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::forProperty(const String& name, gbool caseIns) {
            if (name.length() == 2) {
                if (name == "Cn") return category(1 << Character::UNASSIGNED);
                if (name == "Lu")
                    return category(caseIns
                                        ? (1 << Character::LOWERCASE_LETTER) |
                                        (1 << Character::UPPERCASE_LETTER) |
                                        (1 << Character::TITLECASE_LETTER)
                                        : (1 << Character::UPPERCASE_LETTER));
                if (name == "Ll")
                    return category(caseIns
                                        ? (1 << Character::LOWERCASE_LETTER) |
                                        (1 << Character::UPPERCASE_LETTER) |
                                        (1 << Character::TITLECASE_LETTER)
                                        : (1 << Character::LOWERCASE_LETTER));
                if (name == "Lt")
                    return category(caseIns
                                        ? (1 << Character::LOWERCASE_LETTER) |
                                        (1 << Character::UPPERCASE_LETTER) |
                                        (1 << Character::TITLECASE_LETTER)
                                        : (1 << Character::TITLECASE_LETTER));
                if (name == "Lm") return category(1 << Character::MODIFIER_LETTER);
                if (name == "Lo") return category(1 << Character::OTHER_LETTER);
                if (name == "Mn") return category(1 << Character::NON_SPACING_MARK);
                if (name == "Me") return category(1 << Character::ENCLOSING_MARK);
                if (name == "Mc") return category(1 << Character::COMBINING_SPACING_MARK);
                if (name == "Nd") return category(1 << Character::DECIMAL_DIGIT_NUMBER);
                if (name == "Nl") return category(1 << Character::LETTER_NUMBER);
                if (name == "No") return category(1 << Character::OTHER_NUMBER);
                if (name == "Zs") return category(1 << Character::SPACE_SEPARATOR);
                if (name == "Zl") return category(1 << Character::LINE_SEPARATOR);
                if (name == "Zo") return category(1 << Character::PARAGRAPH_SEPARATOR);
                if (name == "Cc") return category(1 << Character::CONTROL);
                if (name == "Cf") return category(1 << Character::FORMAT);
                if (name == "Co") return category(1 << Character::PRIVATE_USE);
                if (name == "Cs") return category(1 << Character::SURROGATE);
                if (name == "Pd") return category(1 << Character::DASH_PUNCTUATION);
                if (name == "Ps") return category(1 << Character::START_PUNCTUATION);
                if (name == "Pe") return category(1 << Character::END_PUNCTUATION);
                if (name == "Pc") return category(1 << Character::CONNECTOR_PUNCTUATION);
                if (name == "Pi") return category(1 << Character::INITIAL_QUOTE_PUNCTUATION);
                if (name == "Pf") return category(1 << Character::FINAL_QUOTE_PUNCTUATION);
                if (name == "Po") return category(1 << Character::OTHER_PUNCTUATION);
                if (name == "Sm") return category(1 << Character::MATH_SYMBOL);
                if (name == "Sc") return category(1 << Character::CURRENCY_SYMBOL);
                if (name == "Sk") return category(1 << Character::MODIFIER_SYMBOL);
                if (name == "So") return category(1 << Character::OTHER_SYMBOL);

                if (name == "LC")
                    return category(((1 << Character::UPPERCASE_LETTER) |
                        (1 << Character::LOWERCASE_LETTER) |
                        (1 << Character::TITLECASE_LETTER)));
                if (name == "LD")
                    return category(((1 << Character::UPPERCASE_LETTER) |
                        (1 << Character::LOWERCASE_LETTER) |
                        (1 << Character::TITLECASE_LETTER) |
                        (1 << Character::MODIFIER_LETTER) |
                        (1 << Character::OTHER_LETTER) |
                        (1 << Character::DECIMAL_DIGIT_NUMBER)));
                if (name == "L1") return range(0x00, 0xFF); // Latin-1
            }
            if (name.length() == 1) {
                if (name == "L")
                    return category(((1 << Character::UPPERCASE_LETTER) |
                        (1 << Character::LOWERCASE_LETTER) |
                        (1 << Character::TITLECASE_LETTER) |
                        (1 << Character::MODIFIER_LETTER) |
                        (1 << Character::OTHER_LETTER)));
                if (name == "M")
                    return category(((1 << Character::NON_SPACING_MARK) |
                        (1 << Character::ENCLOSING_MARK) |
                        (1 << Character::COMBINING_SPACING_MARK)));
                if (name == "N")
                    return category(((1 << Character::DECIMAL_DIGIT_NUMBER) |
                        (1 << Character::LETTER_NUMBER) |
                        (1 << Character::OTHER_NUMBER)));
                if (name == "Z")
                    return category(((1 << Character::SPACE_SEPARATOR) |
                        (1 << Character::LINE_SEPARATOR) |
                        (1 << Character::PARAGRAPH_SEPARATOR)));
                if (name == "C")
                    return category(((1 << Character::CONTROL) |
                        (1 << Character::FORMAT) |
                        (1 << Character::PRIVATE_USE) |
                        (1 << Character::SURROGATE) |
                        (1 << Character::UNASSIGNED))); // Other
                if (name == "P")
                    return category(((1 << Character::DASH_PUNCTUATION) |
                        (1 << Character::START_PUNCTUATION) |
                        (1 << Character::END_PUNCTUATION) |
                        (1 << Character::CONNECTOR_PUNCTUATION) |
                        (1 << Character::OTHER_PUNCTUATION) |
                        (1 << Character::INITIAL_QUOTE_PUNCTUATION) |
                        (1 << Character::FINAL_QUOTE_PUNCTUATION)));
                if (name == "S") return category(1 << Character::UNASSIGNED);
            }
            if (name.length() == 3) {
                if (name == "all") return Self::ALL();
            }
            if (name.length() == 5) {
                if (name == "ASCII") return range(0x00, 0x7F); // ASCII
                if (name == "Alnum") return ctype(ASCII::ALNUM); // Alphanumeric characters
                if (name == "Alpha") return ctype(ASCII::ALPHA); // Alphabetic characters
                if (name == "Blank") return ctype(ASCII::BLANK); // Space and tab characters
                if (name == "Cntrl") return ctype(ASCII::CNTRL); // Control characters
                if (name == "Digit") return range('0', '9'); // Numeric characters
                if (name == "Graph") return ctype(ASCII::GRAPH); // printable and visible
                if (name == "Lower") return caseIns ? ctype(ASCII::ALPHA) : range('a', 'z'); // Lower-case alphabetic
                if (name == "Print") return range(0x20, 0x7E); // Printable characters
                if (name == "Punct") return ctype(ASCII::PUNCT); // Punctuation characters
                if (name == "Space") return ctype(ASCII::SPACE); // Space characters
                if (name == "Upper") return caseIns ? ctype(ASCII::ALPHA) : range('A', 'Z'); // Upper-case alphabetic
            }
            if (name.length() == 6) {
                if (name == "XDigit") return ctype(ASCII::XDIGIT); // hexadecimal digits
            }
            return { };
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::category(gint typeMask) {
            return UNSAFE::newInstance<CharPredicate>(
                [typeMask](gint ch) -> gbool { return (typeMask & (1 << Character::category(ch))) != 0; });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::range(gint lower, gint upper) {
            return UNSAFE::newInstance<BmpCharPredicate>(
                [lower, upper](gint ch) -> gbool { return lower <= ch && ch <= upper; });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::ctype(gint ctype) {
            return UNSAFE::newInstance<BmpCharPredicate>(
                [ctype](gint ch) -> gbool { return ch < 128 && ASCII::isType(ch, ctype); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::ASCII_DIGIT() {
            return UNSAFE::newInstance<BmpCharPredicate>(
                [](gint ch) -> gbool { return ch < 128 && ASCII::isDigit(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::ASCII_WORD() {
            return UNSAFE::newInstance<BmpCharPredicate>(
                [](gint ch) -> gbool { return ch < 128 && ASCII::isWord(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::ASCII_SPACE() {
            return UNSAFE::newInstance<BmpCharPredicate>(
                [](gint ch) -> gbool { return ch < 128 && ASCII::isSpace(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::EMOJI() {
            return UNSAFE::newInstance<CharPredicate>([](gint ch) -> gbool { return Character::isEmoji(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::EMOJI_PRESENTATION() {
            return UNSAFE::newInstance<CharPredicate>(
                [](gint ch) -> gbool { return Character::isEmojiPresentation(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::EMOJI_MODIFIER() {
            return UNSAFE::newInstance<CharPredicate>([](gint ch) -> gbool { return Character::isEmojiModifier(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::EMOJI_MODIFIER_BASE() {
            return UNSAFE::newInstance<CharPredicate>(
                [](gint ch) -> gbool { return Character::isEmojiModifierBase(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::EMOJI_COMPONENT() {
            return UNSAFE::newInstance<CharPredicate>([](gint ch) -> gbool { return Character::isEmojiComponent(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::EXTENDED_PICTOGRAPHIC() {
            return UNSAFE::newInstance<CharPredicate>(
                [](gint ch) -> gbool { return Character::isExtendedPictographic(ch); });
        }

        Optional<Pattern::CharPredicate> Pattern::Predicates::forPOSIXName(String const& name, gbool ci) {
            String prop = name.toUpperCase(Locale::ROOT);
            return getPosixPredicate(prop, ci);
        }
    } // regex
} // core
