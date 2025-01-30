//
// Created by brunshweeck on 26 mai 2024.
//

#include <core/Character.h>
#include <core/CharArray.h>
#include <core/IllegalArgumentException.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/String.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <meta/CharacterData.h>

namespace core {
    using namespace misc;

    Character::Character() CORE_NOTHROW {
        CORE_IGNORE(this);
    }

    Character::Character(gchar value) : value(value) {
    }

    Character Character::valueOf(gchar c) {
        return c;
    }

    gchar Character::charValue() const {
        return value;
    }

    gint Character::hash() const {
        return hash(value);
    }

    gint Character::hash(gchar value) {
        return value;
    }

    gbool Character::equals(Object const &obj) const {
        return this == &obj ||
               Class<Character>::hasInstance(obj) && value == CORE_XCAST(Character const, obj).value;
    }

    String Character::toString() const {
        return toString(value);
    }

    String Character::toString(gchar c) {
        return String(CharArray::of(c));
    }

    String Character::toString(gint codePoint) {
        return !isValidCodePoint(codePoint)
                   ? "?"_S
                   : isBmpCodePoint(codePoint)
                         ? toString(CORE_CAST(gchar, codePoint))
                         : String(CharArray::of(highSurrogate(codePoint), lowSurrogate(codePoint)));
    }

    gbool Character::isValidCodePoint(gint codePoint) {
        // Optimized form of:
        //     codePoint >= MIN_CODE_POINT && codePoint <= MAX_CODE_POINT
        gint plane = CORE_CAST(gint, (codePoint + 0u) >> 16);
        return plane < ((MAX_CODE_POINT + 1) >> 16);
    }

    gbool Character::isBmpCodePoint(gint codePoint) {
        return (codePoint + 0u) >> 16 == 0;
        // Optimized form of:
        //     codePoint >= MIN_VALUE && codePoint <= MAX_VALUE
        // We consistently use logical shift (>>>) to facilitate
        // additional runtime optimizations.
    }

    gbool Character::isSupplementary(gint codePoint) {
        return codePoint >= MIN_SUPPLEMENTARY
               && codePoint < MAX_CODE_POINT + 1;
    }

    gbool Character::isHighSurrogate(gchar ch) {
        // Help VM constant-fold; MAX_HIGH_SURROGATE + 1 == MIN_LOW_SURROGATE
        return ch >= MIN_HIGH_SURROGATE && ch < (MAX_HIGH_SURROGATE + 1);
    }

    gbool Character::isLowSurrogate(gchar ch) {
        return ch >= MIN_LOW_SURROGATE && ch < (MAX_LOW_SURROGATE + 1);
    }

    gbool Character::isSurrogate(gchar ch) {
        return ch >= MIN_SURROGATE && ch < (MAX_SURROGATE + 1);
    }

    gbool Character::isSurrogatePair(gchar high, gchar low) {
        return isHighSurrogate(high) && isLowSurrogate(low);
    }

    gint Character::charCount(gint codePoint) {
        if (!Character::isValidCodePoint(codePoint))
            return 1;
        return codePoint >= MIN_SUPPLEMENTARY ? 2 : 1;
    }

    gint Character::toCodePoint(gchar high, gchar low) {
        // Optimized form of:
        // return ((high - MIN_HIGH_SURROGATE) << 10)
        //         + (low - MIN_LOW_SURROGATE)
        //         + MIN_SUPPLEMENTARY_CODE_POINT;
        return ((high << 10) + low) + (MIN_SUPPLEMENTARY
                                       - (MIN_HIGH_SURROGATE << 10)
                                       - MIN_LOW_SURROGATE);
    }

    gint Character::codePointAt(CharSequence const &seq, gint index) {
        const gchar c1 = seq.charAt(index);
        if (isHighSurrogate(c1) && ++index < seq.length()) {
            gchar const c2 = seq.charAt(index);
            if (isLowSurrogate(c2)) {
                return toCodePoint(c1, c2);
            }
        }
        return c1;
    }

    gint Character::codePointAt(CharArray const &a, gint index) {
        return codePointAt(a, index, a.length());
    }

    gint Character::codePointAt(CharArray const &a, gint index, gint limit) {
        try {
            Preconditions::checkIndexFromSize(0, limit, a.length());
            Preconditions::checkIndex(index, limit);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }

        gchar const c1 = a[index];
        if (isHighSurrogate(c1) && ++index < limit) {
            gchar const c2 = a[index];
            if (isLowSurrogate(c2)) {
                return toCodePoint(c1, c2);
            }
        }
        return c1;
    }

    gint Character::codePointBefore(CharSequence const &seq, gint index) {
        gchar const c2 = seq.charAt(--index);
        if (isLowSurrogate(c2) && index > 0) {
            gchar const c1 = seq.charAt(--index);
            if (isHighSurrogate(c1)) {
                return toCodePoint(c1, c2);
            }
        }
        return c2;
    }

    gint Character::codePointBefore(CharArray const &a, gint index) {
        return codePointBefore(a, index, 0);
    }

    gint Character::codePointBefore(CharArray const &a, gint index, gint start) {
        if (index <= start || start < 0 || index > a.length()) {
            IndexOutOfBoundsException().throws($ftrace());
        }
        gchar const c2 = a[--index];
        if (isLowSurrogate(c2) && index > start) {
            gchar const c1 = a[--index];
            if (isHighSurrogate(c1)) {
                return toCodePoint(c1, c2);
            }
        }
        return c2;
    }

    gchar Character::highSurrogate(gint codePoint) {
        return CORE_CAST(gchar, ((codePoint + 0u) >> 10)
                         + (MIN_HIGH_SURROGATE - (MIN_SUPPLEMENTARY >> 10)));
    }

    gchar Character::lowSurrogate(gint codePoint) {
        return CORE_CAST(gchar, (codePoint & 0x3ff) + MIN_LOW_SURROGATE);
    }

    static void toSurrogates(gint codePoint, CharArray &dst, gint index) {
        // We write elements "backwards" to guarantee all-or-nothing
        dst[index + 1] = Character::lowSurrogate(codePoint);
        dst[index] = Character::highSurrogate(codePoint);
    }

    gint Character::toChars(gint codePoint, CharArray &dst, gint dstIndex) {
        if (isBmpCodePoint(codePoint)) {
            dst[dstIndex] = codePoint;
            return 1;
        } else if (isValidCodePoint(codePoint)) {
            toSurrogates(codePoint, dst, dstIndex);
            return 2;
        } else {
            IllegalArgumentException(/*"Not a valid Unicode code point: 0x"_S
                                     + Integer::toHexString(codePoint)*/).throws($ftrace());
        }
    }

    CharArray Character::toChars(gint codePoint) {
        if (isBmpCodePoint(codePoint)) {
            return CharArray::of(CORE_CAST(gchar, codePoint));
        } else if (isValidCodePoint(codePoint)) {
            CharArray result(2);
            toSurrogates(codePoint, result, 0);
            return result;
        } else {
            IllegalArgumentException(/*"Not a valid Unicode code point: 0x"_S
                                     + Integer::toHexString(codePoint).toUpperCase()*/).
                    throws($ftrace());
        }
    }

    gint Character::codePointCount(CharSequence const &seq, gint beginIndex, gint endIndex) {
        gint n = endIndex - beginIndex;
        for (gint i = beginIndex; i < endIndex;) {
            if (isHighSurrogate(seq.charAt(i++)) && i < endIndex &&
                isLowSurrogate(seq.charAt(i))) {
                n--;
                i++;
            }
        }
        return n;
    }

    gint Character::codePointCount(CharArray const &a, gint offset, gint count) {
        gint const endIndex = offset + count;
        gint n = count;
        for (gint i = offset; i < endIndex;) {
            if (isHighSurrogate(a[i++]) && i < endIndex &&
                isLowSurrogate(a[i])) {
                n--;
                i++;
            }
        }
        return n;
    }

    gint Character::offsetByCodePoints(CharSequence const &seq, gint index, gint codePointOffset) {
        gint const length = seq.length();

        try {
            Preconditions::checkIndex(index, length);
        } catch (IndexOutOfBoundsException const &ex) { ex.throws($ftrace()); }


        gint x = index;
        if (codePointOffset >= 0) {
            gint i;
            for (i = 0; x < length && i < codePointOffset; i++) {
                if (isHighSurrogate(seq.charAt(x++)) && x < length &&
                    isLowSurrogate(seq.charAt(x))) {
                    x++;
                }
            }
            if (i < codePointOffset) {
                IndexOutOfBoundsException().throws($ftrace());
            }
        } else {
            gint i;
            for (i = codePointOffset; x > 0 && i < 0; i++) {
                if (isLowSurrogate(seq.charAt(--x)) && x > 0 &&
                    isHighSurrogate(seq.charAt(x - 1))) {
                    x--;
                }
            }
            if (i < 0) {
                IndexOutOfBoundsException().throws($ftrace());
            }
        }
        return x;
    }

    gint Character::offsetByCodePoints(CharArray const &a, gint start, gint count, gint index, gint codePointOffset) {
        try {
            Preconditions::checkIndexFromSize(start, count, a.length());
            Preconditions::checkIndex(index, a.length());
        } catch (Throwable const &ex) { ex.throws($ftrace()); }

        gint x = index;
        if (codePointOffset >= 0) {
            gint const limit = start + count;
            gint i;
            for (i = 0; x < limit && i < codePointOffset; i++) {
                if (isHighSurrogate(a[x++]) && x < limit &&
                    isLowSurrogate(a[x])) {
                    x++;
                }
            }
            if (i < codePointOffset) {
                IndexOutOfBoundsException().throws($ftrace());
            }
        } else {
            gint i;
            for (i = codePointOffset; x > start && i < 0; i++) {
                if (isLowSurrogate(a[--x]) && x > start &&
                    isHighSurrogate(a[x - 1])) {
                    x--;
                }
            }
            if (i < 0) {
                IndexOutOfBoundsException().throws($ftrace());
            }
        }
        return x;
    }

    gbool Character::isLowerCase(gchar ch) {
        return isLowerCase(CORE_CAST(gint, ch));
    }

    gbool Character::isLowerCase(gint codePoint) {
        if (codePoint >= 'a' && codePoint <= 'z')
            return true;
        return codePoint > 0x7F && CharacterData::of(codePoint).isLowerCase(codePoint);
    }

    gbool Character::isUpperCase(gchar ch) {
        return isUpperCase(CORE_CAST(gint, ch));
    }

    gbool Character::isUpperCase(gint codePoint) {
        if (codePoint >= 'A' && codePoint <= 'Z')
            return true;
        return codePoint > 0x7F && CharacterData::of(codePoint).isUpperCase(codePoint);
    }

    gbool Character::isTitleCase(gchar ch) {
        return isTitleCase(CORE_CAST(gint, ch));
    }

    gbool Character::isTitleCase(gint codePoint) {
        return category(codePoint) == TITLECASE_LETTER;
    }

    gbool Character::isDigit(gchar ch) {
        return isDigit(CORE_CAST(gint, ch));
    }

    gbool Character::isDigit(gint codePoint) {
        if (codePoint >= '0' && codePoint <= '9')
            return true;
        return codePoint > 0x7F && CharacterData::of(codePoint).isDigit(codePoint);
    }

    gbool Character::isDefined(gchar ch) {
        return isDefined(CORE_CAST(gint, ch));
    }

    gbool Character::isDefined(gint codePoint) {
        return category(codePoint) != Category::UNASSIGNED;
    }

    gbool Character::isLetter(gchar ch) {
        return isLetter(CORE_CAST(gint, ch));
    }

    gbool Character::isLetter(gint codePoint) {
        if (codePoint <= 0x7F)
            return codePoint >= 'a' && codePoint <= 'z' ||
                   codePoint >= 'A' && codePoint <= 'Z';

        return ((((1 << CORE_CAST(gint, Category::UPPERCASE_LETTER)) |
                  (1 << CORE_CAST(gint, Category::LOWERCASE_LETTER)) |
                  (1 << CORE_CAST(gint, Category::TITLECASE_LETTER)) |
                  (1 << CORE_CAST(gint, Category::MODIFIER_LETTER)) |
                  (1 << CORE_CAST(gint, Category::OTHER_LETTER)))
                 >> CORE_CAST(gint, category(codePoint))) & 1) != 0;
    }

    gbool Character::isLetterOrDigit(gchar ch) {
        return isLetterOrDigit(CORE_CAST(gint, ch));
    }

    gbool Character::isLetterOrDigit(gint codePoint) {
        if (codePoint <= 0x7F)
            return codePoint >= 'a' && codePoint <= 'z' ||
                   codePoint >= 'A' && codePoint <= 'Z' ||
                   codePoint >= '0' && codePoint <= '9';

        return ((((1 << CORE_CAST(gint, Category::UPPERCASE_LETTER)) |
                  (1 << CORE_CAST(gint, Category::LOWERCASE_LETTER)) |
                  (1 << CORE_CAST(gint, Category::TITLECASE_LETTER)) |
                  (1 << CORE_CAST(gint, Category::MODIFIER_LETTER)) |
                  (1 << CORE_CAST(gint, Category::OTHER_LETTER)) |
                  (1 << CORE_CAST(gint, Category::DECIMAL_DIGIT_NUMBER)))
                 >> CORE_CAST(gint, category(codePoint))) & 1) != 0;
    }

    gbool Character::isAlphabetic(gint codePoint) {
        if (codePoint <= 0x7F)
            return codePoint >= 'a' && codePoint <= 'z' ||
                   codePoint >= 'A' && codePoint <= 'Z';

        return (((((1 << CORE_CAST(gint, Category::UPPERCASE_LETTER)) |
                   (1 << CORE_CAST(gint, Category::LOWERCASE_LETTER)) |
                   (1 << CORE_CAST(gint, Category::TITLECASE_LETTER)) |
                   (1 << CORE_CAST(gint, Category::MODIFIER_LETTER)) |
                   (1 << CORE_CAST(gint, Category::OTHER_LETTER)) |
                   (1 << CORE_CAST(gint, Category::LETTER_NUMBER)))
                  >> CORE_CAST(gint, category(codePoint))) & 1) != 0) ||
               CharacterData::of(codePoint).isOtherAlphabetic(codePoint);
    }

    gbool Character::isIdeographic(gint codePoint) {
        return CharacterData::of(codePoint).isIdeographic(codePoint);
    }

    gbool Character::isEmoji(gint codePoint) {
        return CharacterData::of(codePoint).isEmoji(codePoint);
    }

    gbool Character::isEmojiPresentation(gint codePoint) {
        return CharacterData::of(codePoint).isEmojiPresentation(codePoint);
    }

    gbool Character::isEmojiModifier(gint codePoint) {
        return CharacterData::of(codePoint).isEmojiModifier(codePoint);
    }

    gbool Character::isEmojiModifierBase(gint codePoint) {
        return CharacterData::of(codePoint).isEmojiModifierBase(codePoint);
    }

    gbool Character::isEmojiComponent(gint codePoint) {
        return CharacterData::of(codePoint).isEmojiComponent(codePoint);
    }

    gbool Character::isExtendedPictographic(gint codePoint) {
        return CharacterData::of(codePoint).isExtendedPictographic(codePoint);
    }

    gchar Character::toLowerCase(gchar ch) {
        return toLowerCase(CORE_CAST(gint, ch));
    }

    gint Character::toLowerCase(gint codePoint) {
        return CharacterData::of(codePoint).toLowerCase(codePoint);
    }

    gchar Character::toUpperCase(gchar ch) {
        return toUpperCase(CORE_CAST(gint, ch));
    }

    gint Character::toUpperCase(gint codePoint) {
        return CharacterData::of(codePoint).toUpperCase(codePoint);
    }

    gchar Character::toTitleCase(gchar ch) {
        return toTitleCase(CORE_CAST(gint, ch));
    }

    gint Character::toTitleCase(gint codePoint) {
        return CharacterData::of(codePoint).toTitleCase(codePoint);
    }

    gint Character::digit(gchar ch, gint radix) {
        return digit(CORE_CAST(gint, ch), radix);
    }

    gint Character::digit(gint codePoint, gint radix) {
        return CharacterData::of(codePoint).digit(codePoint, radix);
    }

    gint Character::numericValue(gchar ch) {
        return numericValue(CORE_CAST(gint, ch));
    }

    gint Character::numericValue(gint codePoint) {
        return CharacterData::of(codePoint).numericValue(codePoint);
    }

    gbool Character::isSpace(gchar ch) {
        return (ch <= 0x0020) &&
               (((((1LL << 0x0009) |
                   (1LL << 0x000A) |
                   (1LL << 0x000C) |
                   (1LL << 0x000D) |
                   (1LL << 0x0020)) >> ch) & 1LL) != 0);
    }

    gbool Character::isUnicodeSpace(gchar ch) {
        return isUnicodeSpace(CORE_CAST(gint, ch));
    }

    gbool Character::isUnicodeSpace(gint codePoint) {
        return ((((1 << CORE_CAST(gint, Category::SPACE_SEPARATOR)) |
                  (1 << CORE_CAST(gint, Category::LINE_SEPARATOR)) |
                  (1 << CORE_CAST(gint, Category::PARAGRAPH_SEPARATOR)))
                 >> CORE_CAST(gint, category(codePoint))) & 1) != 0;
    }

    gbool Character::isWhitespace(gchar ch) {
        return isWhitespace(CORE_CAST(gint, ch));
    }

    gbool Character::isWhitespace(gint codePoint) {
        return CharacterData::of(codePoint).isWhitespace(codePoint);
    }

    gbool Character::isISOControl(gchar ch) {
        return isISOControl(CORE_CAST(gint, ch));
    }

    gbool Character::isISOControl(gint codePoint) {
        // Optimized form of:
        //     (codePoint >= 0x00 && codePoint <= 0x1F) ||
        //     (codePoint >= 0x7F && codePoint <= 0x9F);
        return codePoint <= 0x9F &&
               (codePoint >= 0x7F || ((codePoint + 0u) >> 5 == 0));
    }

    Character::Category Character::category(gchar ch) {
        return category(CORE_CAST(gint, ch));
    }

    Character::Category Character::category(gint codePoint) {
        return CORE_CAST(Category, CharacterData::of(codePoint).type(codePoint));
    }

    gchar Character::forDigit(gint digit, gint radix) {
        if ((digit >= radix) || (digit < 0)) {
            return '\0';
        }
        if ((radix < Character::MIN_RADIX) || (radix > Character::MAX_RADIX)) {
            return '\0';
        }
        if (digit < 10) {
            return ('0' + digit);
        }
        return ('a' - 10 + digit);
    }

    Character::Directionality Character::directionality(gchar ch) {
        return directionality(CORE_CAST(gint, ch));
    }

    Character::Directionality Character::directionality(gint codePoint) {
        return CORE_CAST(Directionality, CharacterData::of(codePoint).directionality(codePoint));
    }

    gbool Character::isMirrored(gchar ch) {
        return isMirrored(CORE_CAST(gint, ch));
    }

    gbool Character::isMirrored(gint codePoint) {
        return CharacterData::of(codePoint).isMirrored(codePoint);
    }

    gint Character::compareTo(Character const &anotherCharacter) const {
        return compare(value, anotherCharacter.value);
    }

    gint Character::compare(gchar x, gchar y) {
        return x - y;
    }

    gchar Character::reverseBytes(gchar ch) {
        return ((ch & 0xFF00) >> 8) | (ch << 8);
    }

    String Character::name(gint codePoint) {
        CORE_IGNORE(codePoint);
        return ""_S;
    }

    gint Character::codePointOf(String const &name) {
        CORE_IGNORE(name);
        return -1;
    }

    Object &Character::clone() const {
        try {
            return UNSAFE::newInstance<Character>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
