//
// Created by bruns on 13/05/2024.
//

#include <core/Character.h>
#include <core/Integer.h>
#include <core/Long.h>
#include <core/Math.h>
#include <core/NumberFormatException.h>
#include <core/String.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gchar digits[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
        'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    String Long::toString(glong i, gint radix) {
        if (radix < Character::MIN_RADIX || radix > Character::MAX_RADIX)
            radix = 10;
        if (radix == 10)
            return toString(i);
        CharArray ca = CharArray(65);
        gint cursor = 64;
        gbool negative = (i < 0);

        if (!negative) {
            i = -i;
        }

        while (i <= -radix) {
            ca[cursor--] = (gbyte) digits[(gint) (-(i % radix))];
            i = i / radix;
        }
        ca[cursor] = (gbyte) digits[(gint) (-i)];

        if (negative) {
            ca[--cursor] = '-';
        }
        return String(ca, cursor, 65 - cursor);
    }

    String core::Long::toUnsignedString(glong i, gint radix) {
        if (i >= 0)
            return toString(i, radix);
        else {
            gint shift = 0;
            goto SelectRadix;
        StringConvert: {
                // assert shift > 0 && shift <=5 : "Illegal shift value";
                int mag = Long::SIZE - Long::numberOfLeadingZeros(i);
                int count = Math::max(((mag + (shift - 1)) / shift), 1);
                CharArray ca(count);
                gint next = count;
                radix = 1 << shift;
                gint mask = radix - 1;
                do {
                    ca[--next] = (gbyte) digits[((gint) i) & mask];
                    i = (glong) ((i + 0uLL) >> shift);
                } while (next > 0);
                return String(ca, 0, count - next);
            }
        SelectRadix:
            switch (radix) {
                case 2: shift = 1;
                    goto StringConvert;
                case 4: shift = 2;
                    goto StringConvert;
                case 8: shift = 3;
                    goto StringConvert;
                case 10: {
                    /*
                     * We can get the effect of an unsigned division by 10
                     * on a long value by first shifting right, yielding a
                     * positive value, and then dividing by 5.  This
                     * allows the last digit and preceding digits to be
                     * isolated more quickly than by an initial conversion
                     * to BigInteger.
                     */
                    glong quot = (gint) ((i + 0u) >> 1) / 5;
                    glong rem = i - quot * 10;
                    return toString(quot) + rem;
                }
                case 16: shift = 4;
                    goto StringConvert;
                case 32: shift = 5;
                    goto StringConvert;
                default: {
                    // extract the last digit, with remainderUnsigned(long, long)
                    // and convert the first digits using divideUnsigned(long, long) and with toString(long)
                    glong quot = divideUnsigned(i, radix);
                    glong rem = remainderUnsigned(i, radix);
                    //                    CORE_ASSERT(rem < radix);
                    return toString(quot, radix) + toString(rem, radix);
                }
            }
        }
    }

    String Long::toHexString(glong i) {
        return toUnsignedString(i, 16);
    }

    String Long::toOctalString(glong i) {
        return toUnsignedString(i, 8);
    }

    String Long::toBinaryString(glong i) {
        return toUnsignedString(i, 2);
    }

    String Long::toString(glong i) {
        gint size = 0;
    StringSize: {
            glong x = i;
            gint d = 1;
            if (x >= 0) {
                d = 0;
                x = -x;
            }
            glong p = -10;
            for (gint j = 1; j < 19; j++) {
                if (x > p) {
                    size = j + d;
                    goto FormatString;
                }
                p = 10 * p;
            }
            size = 19 + d;
        }
    FormatString:
        CharArray ca(size);
    CopyChars: {
            glong q;
            gint r;
            gint cursor = size;

            gbool negative = (i < 0);
            if (!negative) {
                i = -i;
            }

            // Get 2 digits/iteration using longs until quotient fits into an int
            while (i <= Integer::MIN_VALUE) {
                q = i / 100;
                r = (gint) ((q * 100) - i);
                i = q;
                ca[--cursor] = (gbyte) digits[r % 10];
                ca[--cursor] = (gbyte) digits[r / 10];
            }

            // Get 2 digits/iteration using ints
            gint q2;
            gint i2 = (gint) i;
            while (i2 <= -100) {
                q2 = i2 / 100;
                r = (q2 * 100) - i2;
                i2 = q2;
                ca[--cursor] = digits[r % 10];
                ca[--cursor] = digits[r / 10];
            }

            // We know there are at most two digits left at this point.
            ca[--cursor] = digits[(-i2) % 10];
            if (i2 < -9) {
                ca[--cursor] = digits[(-i2) / 10];
            }

            if (negative) {
                ca[--cursor] = '-';
            }
            return String(ca, 0, size - cursor);
        }
    }

    String Long::toUnsignedString(glong i) {
        return toUnsignedString(i, 10);
    }

    glong Long::parseLong(const String &s, gint radix) {
        if (radix < Character::MIN_RADIX) {
            NumberFormatException("Radix "_S + radix + " less than 2"_S).throws($ftrace());
        }
        if (radix > Character::MAX_RADIX) {
            NumberFormatException("Radix "_S + radix + " less than 2"_S).throws($ftrace());
        }

        gbool negative = false;
        gint i = 0, len = s.length();
        glong limit = -Long::MAX_VALUE;

        if (len > 0) {
            gchar firstChar = s.charAt(0);
            if (firstChar < '0') {
                // Possible leading "+" or "-"
                if (firstChar == '-') {
                    negative = true;
                    limit = Long::MIN_VALUE;
                } else if (firstChar != '+') {
                    NumberFormatException("For input string: \""_S + s + "\" under radix "_S + radix).throws(
                        $ftrace());
                }

                if (len == 1) {
                    // Cannot have lone "+" or "-"
                    NumberFormatException("For input string: \""_S + s + "\" under radix "_S + radix).throws(
                        $ftrace());
                }
                i++;
            }
            glong multmin = limit / radix;
            glong result = 0;
            while (i < len) {
                // Accumulating negatively avoids surprises near MAX_VALUE
                gint digit = Character::digit(s.charAt(i++), radix);
                if (digit < 0 || result < multmin) {
                    NumberFormatException("For input string: \""_S + s + "\" under radix "_S + radix).throws(
                        $ftrace());
                }
                result *= radix;
                if (result < limit + digit) {
                    NumberFormatException("For input string: \""_S + s + "\" under radix "_S + radix).throws(
                        $ftrace());
                }
                result -= digit;
            }
            return negative ? result : -result;
        } else {
            NumberFormatException("For input string: \""_S + s + "\" under radix "_S + radix).throws(
                $ftrace());
        }
    }

    glong Long::parseLong(const CharSequence &s, gint beginIndex, gint endIndex, gint radix) {
        if (radix < Character::MIN_RADIX) {
            NumberFormatException("Radix "_S + radix + " less than 2"_S).throws($ftrace());
        }
        if (radix > Character::MAX_RADIX) {
            NumberFormatException("Radix "_S + radix + " less than 2"_S).throws($ftrace());
        }

        gbool negative = false;
        gint i = beginIndex;
        glong limit = -Long::MAX_VALUE;

        if (i < endIndex) {
            gchar firstChar = s.charAt(i);
            if (firstChar < '0') {
                // Possible leading "+" or "-"
                if (firstChar == '-') {
                    negative = true;
                    limit = Long::MIN_VALUE;
                } else if (firstChar != '+') {
                    NumberFormatException(
                        "Error at index "_S + (i - beginIndex) + ", in \""_S + s.subSequence(beginIndex, endIndex) +
                        "\""_S).throws($ftrace());
                }
                i++;
            }
            if (i >= endIndex) {
                // Cannot have lone "+", "-" or ""
                NumberFormatException(
                            "Error at index "_S + (i - beginIndex) + ", in \""_S + s.subSequence(beginIndex, endIndex) +
                            "\""_S)
                        .throws($ftrace());
            }
            glong multmin = limit / radix;
            glong result = 0;
            while (i < endIndex) {
                // Accumulating negatively avoids surprises near MAX_VALUE
                gint digit = Character::digit(s.charAt(i), radix);
                if (digit < 0 || result < multmin) {
                    NumberFormatException(
                        "Error at index "_S + (i - beginIndex) + ", in \""_S + s.subSequence(beginIndex, endIndex) +
                        "\""_S).throws($ftrace());
                }
                result *= radix;
                if (result < limit + digit) {
                    NumberFormatException(
                        "Error at index "_S + (i - beginIndex) + ", in \""_S + s.subSequence(beginIndex, endIndex) +
                        "\""_S).throws($ftrace());
                }
                i++;
                result -= digit;
            }
            return negative ? result : -result;
        } else {
            NumberFormatException(""_S).throws($ftrace());
        }
    }

    glong Long::parseLong(const String &s) {
        try {
            return parseLong(s, 10);
        } catch (Exception const &ex) {
            ex.throws($ftrace());
        }
    }

    glong Long::parseUnsignedLong(const String &s, gint radix) {
        gint len = s.length();
        if (len > 0) {
            gchar firstChar = s.charAt(0);
            if (firstChar == '-') {
                NumberFormatException("Illegal leading minus sign "
                                      "on unsigned string "_S + s + "."_S).throws($ftrace());
            } else {
                if (len <= 12 || // Long::MAX_VALUE in Character::MAX_RADIX is 13 digits
                    (radix == 10 && len <= 18)) {
                    // Long::MAX_VALUE in base 10 is 19 digits
                    try {
                        return parseLong(s, radix);
                    } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
                }

                // No need for range checks on len due to testing above.
                glong first;
                try {
                    first = parseLong(s, 0, len - 1, radix);
                } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
                gint second = Character::digit(s.charAt(len - 1), radix);
                if (second < 0) {
                    NumberFormatException("Bad digit at end of "_S + s).throws($ftrace());
                }
                glong result = first * radix + second;

                /*
                 * Test leftmost bits of multiprecision extension of first*radix
                 * for overflow. The number of bits needed is defined by
                 * GUARD_BIT = ceil(log2(Character::MAX_RADIX)) + 1 = 7. Then
                 * int guard = radix*(int)(first >>> (64 - GUARD_BIT)) and
                 * overflow is tested by splitting guard in the ranges
                 * guard < 92, 92 <= guard < 128, and 128 <= guard, where
                 * 92 = 128 - Character::MAX_RADIX. Note that guard cannot take
                 * on a value which does not include a prime factor in the legal
                 * radix range.
                 */
                gint guard = radix * (gint) ((first + 0uLL) >> 57);
                if (guard >= 128 ||
                    (result >= 0 && guard >= 128 - Character::MAX_RADIX)) {
                    /*
                     * For purposes of exposition, the programmatic statements
                     * below should be taken to be multi-precision, i.e., not
                     * subject to overflow.
                     *
                     * A) Condition guard >= 128:
                     * If guard >= 128 then first*radix >= 2^7 * 2^57 = 2^64
                     * hence always overflow.
                     *
                     * B) Condition guard < 92:
                     * Define left7 = first >>> 57.
                     * Given first = (left7 * 2^57) + (first & (2^57 - 1)) then
                     * result <= (radix*left7)*2^57 + radix*(2^57 - 1) + second.
                     * Thus, if radix*left7 < 92, radix <= 36, and second < 36,
                     * then result < 92*2^57 + 36*(2^57 - 1) + 36 = 2^64 hence
                     * never overflow.
                     *
                     * C) Condition 92 <= guard < 128:
                     * first*radix + second >= radix*left7*2^57 + second
                     * so that first*radix + second >= 92*2^57 + 0 > 2^63
                     *
                     * D) Condition guard < 128:
                     * radix*first <= (radix*left7) * 2^57 + radix*(2^57 - 1)
                     * so
                     * radix*first + second <= (radix*left7) * 2^57 + radix*(2^57 - 1) + 36
                     * thus
                     * radix*first + second < 128 * 2^57 + 36*2^57 - radix + 36
                     * whence
                     * radix*first + second < 2^64 + 2^6*2^57 = 2^64 + 2^63
                     *
                     * E) Conditions C, D, and result >= 0:
                     * C and D combined imply the mathematical result
                     * 2^63 < first*radix + second < 2^64 + 2^63. The lower
                     * bound is therefore negative as a signed long, but the
                     * upper bound is too small to overflow again after the
                     * signed long overflows to positive above 2^64 - 1. Hence,
                     * result >= 0 implies overflow given C and D.
                     */
                    NumberFormatException("String value "_S + s
                                          + " exceeds range of unsigned glong."_S).throws($ftrace());
                }
                return result;
            }
        } else {
            NumberFormatException("For input string: \""_S + s + "\" under radix "_S + radix).throws(
                $ftrace());
        }
    }

    glong Long::parseUnsignedLong(const CharSequence &s, gint beginIndex, gint endIndex, gint radix) {
        gint start = beginIndex, len = endIndex - beginIndex;

        if (len > 0) {
            gchar firstChar = s.charAt(start);
            if (firstChar == '-') {
                NumberFormatException("Illegal leading minus sign on unsigned string "_S
                                      + s.subSequence(start, start + len).toString() + "."_S).throws(
                    $ftrace());
            } else {
                if (len <= 12 || // Long::MAX_VALUE in Character::MAX_RADIX is 13 digits
                    (radix == 10 && len <= 18)) {
                    // Long::MAX_VALUE in base 10 is 19 digits
                    try {
                        return parseLong(s, start, start + len, radix);
                    } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
                }

                // No need for range checks on end due to testing above.
                glong first;
                try {
                    first = parseLong(s, start, start + len - 1, radix);
                } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
                gint second = Character::digit(s.charAt(start + len - 1), radix);
                if (second < 0) {
                    NumberFormatException("Bad digit at end of "_S +
                                          s.subSequence(start, start + len).toString()).throws($ftrace());
                }
                glong result = first * radix + second;

                /*
                 * Test leftmost bits of multiprecision extension of first*radix
                 * for overflow. The number of bits needed is defined by
                 * GUARD_BIT = ceil(log2(Character::MAX_RADIX)) + 1 = 7. Then
                 * int guard = radix*(int)(first >>> (64 - GUARD_BIT)) and
                 * overflow is tested by splitting guard in the ranges
                 * guard < 92, 92 <= guard < 128, and 128 <= guard, where
                 * 92 = 128 - Character::MAX_RADIX. Note that guard cannot take
                 * on a value which does not include a prime factor in the legal
                 * radix range.
                 */
                gint guard = radix * (gint) ((first + 0uLL) >> 57);
                if (guard >= 128 ||
                    (result >= 0 && guard >= 128 - Character::MAX_RADIX)) {
                    /*
                     * For purposes of exposition, the programmatic statements
                     * below should be taken to be multi-precision, i.e., not
                     * subject to overflow.
                     *
                     * A) Condition guard >= 128:
                     * If guard >= 128 then first*radix >= 2^7 * 2^57 = 2^64
                     * hence always overflow.
                     *
                     * B) Condition guard < 92:
                     * Define left7 = first >>> 57.
                     * Given first = (left7 * 2^57) + (first & (2^57 - 1)) then
                     * result <= (radix*left7)*2^57 + radix*(2^57 - 1) + second.
                     * Thus, if radix*left7 < 92, radix <= 36, and second < 36,
                     * then result < 92*2^57 + 36*(2^57 - 1) + 36 = 2^64 hence
                     * never overflow.
                     *
                     * C) Condition 92 <= guard < 128:
                     * first*radix + second >= radix*left7*2^57 + second
                     * so that first*radix + second >= 92*2^57 + 0 > 2^63
                     *
                     * D) Condition guard < 128:
                     * radix*first <= (radix*left7) * 2^57 + radix*(2^57 - 1)
                     * so
                     * radix*first + second <= (radix*left7) * 2^57 + radix*(2^57 - 1) + 36
                     * thus
                     * radix*first + second < 128 * 2^57 + 36*2^57 - radix + 36
                     * whence
                     * radix*first + second < 2^64 + 2^6*2^57 = 2^64 + 2^63
                     *
                     * E) Conditions C, D, and result >= 0:
                     * C and D combined imply the mathematical result
                     * 2^63 < first*radix + second < 2^64 + 2^63. The lower
                     * bound is therefore negative as a signed long, but the
                     * upper bound is too small to overflow again after the
                     * signed long overflows to positive above 2^64 - 1. Hence,
                     * result >= 0 implies overflow given C and D.
                     */
                    NumberFormatException("String value "_S + s.subSequence(start, start + len).toString()
                                          + " exceeds range of unsigned glong."_S).throws($ftrace());
                }
                return result;
            }
        } else {
            NumberFormatException("For input string: \"\" under radix "_S + radix).throws($ftrace());
        }
    }

    glong Long::parseUnsignedLong(const String &s) {
        try {
            return parseUnsignedLong(s, 10);
        } catch (NumberFormatException const &ex) {
            ex.throws($ftrace());
        }
    }

    Long Long::valueOf(const String &s, gint radix) {
        try {
            return valueOf(parseLong(s, radix));
        } catch (NumberFormatException const &ex) {
            ex.throws($ftrace());
        }
    }

    Long Long::valueOf(const String &s) {
        try {
            return valueOf(s, 10);
        } catch (NumberFormatException const &ex) {
            ex.throws($ftrace());
        }
    }

    Long Long::valueOf(glong l) {
        return l;
    }

    Long Long::decode(const String &nm) {
        gint radix = 10;
        gint index = 0;
        gbool negative = false;
        glong result;

        if (nm.isEmpty())
            NumberFormatException("Zero length string"_S).throws($ftrace());
        gchar firstChar = nm.charAt(0);
        // Handle sign, if present
        if (firstChar == '-') {
            negative = true;
            index++;
        } else if (firstChar == '+')
            index++;

        // Handle radix specifier, if present
        if (nm.startsWith("0x"_S, index) || nm.startsWith("0X"_S, index)) {
            index += 2;
            radix = 16;
        } else if (nm.startsWith("#"_S, index)) {
            index++;
            radix = 16;
        } else if (nm.startsWith("0"_S, index) && nm.length() > 1 + index) {
            index++;
            radix = 8;
        }

        if (nm.startsWith("-"_S, index) || nm.startsWith("+"_S, index))
            NumberFormatException("Sign character in wrong position"_S).throws($ftrace());

        try {
            result = parseLong(nm, index, nm.length(), radix);
            result = negative ? -result : result;
        } catch (NumberFormatException const &e) {
            // If number is Long.MIN_VALUE, we'll end up here. The next line
            // handles this case, and causes any genuine format error to be
            // rethrown.
            String constant = negative
                                  ? ("-"_S + nm.subString(index))
                                  : nm.subString(index);
            try {
                result = parseLong(constant, radix);
            } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
        }
        return result;
    }

    Long::Long(glong value) : value(value) {
    }

    gbyte Long::byteValue() const {
        return (gbyte) value;
    }

    gshort Long::shortValue() const {
        return (gshort) value;
    }

    gint Long::intValue() const {
        return (gint) value;
    }

    glong Long::longValue() const {
        return value;
    }

    gfloat Long::floatValue() const {
        return (gfloat) value;
    }

    gdouble Long::doubleValue() const {
        return (gdouble) value;
    }

    String Long::toString() const {
        return toString(value);
    }

    gint Long::hash() const {
        return hash(value);
    }

    gint Long::hash(glong value) {
        return (gint) ((value) ^ ((value + 0ULL) >> 32));
    }

    gbool Long::equals(const Object &obj) const {
        return this == &obj || Class<Long>::hasInstance(obj) && value == CORE_XCAST(Long const, obj).value;
    }

    gint Long::compareTo(const Long &anotherLong) const {
        return compare(value, anotherLong.value);
    }

    gint Long::compare(glong x, glong y) {
        return x > y ? 1 : x < y ? -1 : 0;
    }

    gint Long::compareUnsigned(glong x, glong y) {
        return compare(x + MIN_VALUE, y + MIN_VALUE);
    }

    glong Long::divideUnsigned(glong dividend, glong divisor) {
        /* See Hacker's Delight (2nd ed), section 9.3 */
        if (divisor >= 0) {
            glong q = (glong) ((dividend + 0uLL) >> 1) / divisor << 1;
            glong r = dividend - q * divisor;
            return (glong) (q + (((r | ~(r - divisor)) + 0uLL) >> (Long::SIZE - 1)));
        }
        return (glong) (((dividend & ~(dividend - divisor)) + 0uLL) >> (Long::SIZE - 1));
    }

    glong Long::remainderUnsigned(glong dividend, glong divisor) {
        /* See Hacker's Delight (2nd ed), section 9.3 */
        if (divisor >= 0) {
            glong q = (glong) ((dividend + 0uLL) >> 1) / divisor << 1;
            glong r = dividend - q * divisor;
            /*
             * Here, 0 <= r < 2 * divisor
             * (1) When 0 <= r < divisor, the remainder is simply r.
             * (2) Otherwise the remainder is r - divisor.
             *
             * In case (1), r - divisor < 0. Applying ~ produces a long with
             * sign bit 0, so >> produces 0. The returned value is thus r.
             *
             * In case (2), a similar reasoning shows that >> produces -1,
             * so the returned value is r - divisor.
             */
            return r - ((~(r - divisor) >> (Long::SIZE - 1)) & divisor);
        }
        /*
         * (1) When dividend >= 0, the remainder is dividend.
         * (2) Otherwise
         *      (2.1) When dividend < divisor, the remainder is dividend.
         *      (2.2) Otherwise the remainder is dividend - divisor
         *
         * A reasoning similar to the above shows that the returned value
         * is as expected.
         */
        return dividend - (((dividend & ~(dividend - divisor)) >> (Long::SIZE - 1)) & divisor);
    }

    glong Long::highestOneBit(glong i) {
        return i & (MIN_VALUE >> numberOfLeadingZeros(i));
    }

    glong Long::lowestOneBit(glong i) {
        // HD, Section 2-1
        return i & -i;
    }

    gint Long::numberOfLeadingZeros(glong i) {
        int x = (int) ((i + 0uLL) >> 32);
        return x == 0
                   ? 32 + Integer::numberOfLeadingZeros((int) i)
                   : Integer::numberOfLeadingZeros(x);
    }

    gint Long::numberOfTrailingZeros(glong i) {
        int x = (int) i;
        return x == 0
                   ? 32 + Integer::numberOfTrailingZeros((int) ((i + 0uLL) >> 32))
                   : Integer::numberOfTrailingZeros(x);
    }

    gint Long::bitCount(glong i) {
        // HD, Figure 5-2
        i = i - ((glong) ((i + 0uLL) >> 1) & 0x5555555555555555LL);
        i = (i & 0x3333333333333333LL) + ((glong) ((i + 0uLL) >> 2) & 0x3333333333333333LL);
        i = (i + (glong) ((i + 0uLL) >> 4)) & 0x0f0f0f0f0f0f0f0fLL;
        i = i + (glong) ((i + 0uLL) >> 8);
        i = i + (glong) ((i + 0uLL) >> 16);
        i = i + (glong) ((i + 0uLL) >> 32);
        return (int) i & 0x7f;
    }

    glong Long::rotateLeft(glong i, gint distance) {
        return (glong) ((i << distance) | ((i + 0uLL) >> -distance));
    }

    glong Long::rotateRight(glong i, gint distance) {
        return (glong) (((i + 0uLL) >> distance) | (i << -distance));
    }

    glong Long::reverse(glong i) {
        // HD, Figure 7-1
        i = (i & 0x5555555555555555LL) << 1 | (glong) ((i + 0uLL) >> 1) & 0x5555555555555555LL;
        i = (i & 0x3333333333333333LL) << 2 | (glong) ((i + 0uLL) >> 2) & 0x3333333333333333LL;
        i = (i & 0x0f0f0f0f0f0f0f0fLL) << 4 | (glong) ((i + 0uLL) >> 4) & 0x0f0f0f0f0f0f0f0fLL;

        return reverseBytes(i);
    }

    gint Long::signum(glong i) {
        // HD, Section 2-7
        return (int) ((i >> 63) | ((-i + 0uLL) >> 63));
    }

    glong Long::reverseBytes(glong i) {
        i = (i & 0x00ff00ff00ff00ffLL) << 8 | (glong) ((i + 0uLL) >> 8) & 0x00ff00ff00ff00ffLL;
        return (i << 48) | ((i & 0xffff0000LL) << 16) |
               ((glong) ((i + 0uLL) >> 16) & 0xffff0000LL) | (glong) ((i + 0uLL) >> 48);
    }

    glong Long::sum(glong a, glong b) {
        return a + b;
    }

    glong Long::max(glong a, glong b) {
        return Math::max(a, b);
    }

    glong Long::min(glong a, glong b) {
        return Math::min(a, b);
    }

    Long::operator glong() const {
        return longValue();
    }

    Long::operator glong &() {
        return value;
    }

    Object &Long::clone() const {
        try {
            return UNSAFE::newInstance<Long>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
