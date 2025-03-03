//
// Created by bruns on 13/05/2024.
//

#include <core/lang/Character.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include "Integer.h"
#include <core/lang/Long.h>
#include <core/lang/Math.h>
#include <core/lang/NumberFormatException.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gchar digits[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8',
        '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
        'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    String Integer::toString(gint i, gint radix) {
        if (radix < 2 || radix > 36)
            radix = 10;
        if (radix == 10) {
            return toString(i);
        }

        gbool negative = i < 0;
        if (!negative) {
            i = -i;
        }

        CharArray ca = CharArray(33);
        gint next = 32;

        while (i <= -radix) {
            ca[next--] = digits[-(i % radix)];
            i /= radix;
        }

        ca[next] = digits[-i];

        if (negative) {
            ca[--next] = '-';
        }

        return String(ca, next, 33 - next);
    }

    String Integer::toUnsignedString(gint i, gint radix) {
        return Long::toString(toUnsignedLong(i), radix);
    }

    String Integer::toHexString(gint i) {
        return toUnsignedString(i, 16);
    }

    String Integer::toOctalString(gint i) {
        return toUnsignedString(i, 8);
    }

    String Integer::toBinaryString(gint i) {
        return toUnsignedString(i, 2);
    }

    String Integer::toString(gint i) {
        gint size = 0;
    StringSize: {
            gint x = i;
            gint d = 1;
            if (x >= 0) {
                d = 0;
                x = -x;
            }
            gint p = -10;
            for (gint j = 1; j < 10; j++) {
                if (x > p) {
                    size = j + d;
                    goto FormatInt;
                }
                p = 10 * p;
            }
            size = 10 + d;
        }
    FormatInt:
        ByteArray ba(size);
        gint cursor = size;
    GetChars: {
            gint q, r;
            gbool negative = i < 0;

            if (!negative) {
                i = -i;
            }

            // Generate two digits per iteration
            while (i <= -100) {
                q = i / 100;
                r = (q * 100) - i;
                i = q;
                ba[--cursor] = (gbyte) digits[r % 10];
                ba[--cursor] = (gbyte) digits[r / 10];
            }

            // We know there are at most two digits left at this point.
            ba[--cursor] = (gbyte) digits[(-i) % 10];
            if (i < -9) {
                ba[--cursor] = (gbyte) digits[-i / 10];
            }

            if (negative) {
                ba[--cursor] = '-';
            }
        }
        return String(ba, 0, cursor, size - cursor);
    }

    String Integer::toUnsignedString(gint i) {
        return toUnsignedString(i, 10);
    }

    gint Integer::parseInt(const String &s, gint radix) {
        if (radix < Character::MIN_RADIX) {
            NumberFormatException("Radix "_S + radix + " less than 2"_S).throws($ftrace());
        }
        if (radix > Character::MAX_RADIX) {
            NumberFormatException("Radix "_S + radix + " less than 2"_S).throws($ftrace());
        }

        gbool negative = false;
        gint i = 0, len = s.length();
        gint limit = -Integer::MAX_VALUE;

        if (len > 0) {
            gchar firstChar = s.charAt(0);
            if (firstChar < '0') {
                // Possible leading "+" or "-"
                if (firstChar == '-') {
                    negative = true;
                    limit = Integer::MIN_VALUE;
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
            gint multmin = limit / radix;
            gint result = 0;
            while (i < len) {
                // Accumulating negatively avoids surprises near MAX_VALUE
                gchar c = s.charAt(i++);
                gint digit = Character::digit(c, radix);
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

    gint Integer::parseInt(const CharSequence &s, gint beginIndex, gint endIndex, gint radix) {
        if (radix < Character::MIN_RADIX) {
            NumberFormatException("Radix "_S + radix + " less than 2"_S).throws($ftrace());
        }
        if (radix > Character::MAX_RADIX) {
            NumberFormatException("Radix "_S + radix + " less than 2"_S).throws($ftrace());
        }

        gbool negative = false;
        gint i = beginIndex;
        gint limit = -Integer::MAX_VALUE;

        if (i < endIndex) {
            gchar firstChar = s.charAt(i);
            if (firstChar < '0') {
                // Possible leading "+" or "-"
                if (firstChar == '-') {
                    negative = true;
                    limit = Integer::MIN_VALUE;
                } else if (firstChar != '+') {
                    NumberFormatException(
                        "Error at index "_S + (i - beginIndex) + ", in \""_S + s.subSequence(beginIndex, endIndex) +
                        "\"."_S).throws($ftrace());
                }
                i++;
                if (i == endIndex) {
                    // Cannot have lone "+" or "-"
                    NumberFormatException(
                        "Error at index "_S + (i - beginIndex) + ", in \""_S + s.subSequence(beginIndex, endIndex) +
                        "\"."_S).throws($ftrace());
                }
            }
            gint multmin = limit / radix;
            gint result = 0;
            while (i < endIndex) {
                // Accumulating negatively avoids surprises near MAX_VALUE
                gint digit = Character::digit(s.charAt(i), radix);
                if (digit < 0 || result < multmin) {
                    NumberFormatException(
                        "Error at index "_S + (i - beginIndex) + ", in \""_S + s.subSequence(beginIndex, endIndex) +
                        "\"."_S).throws($ftrace());
                }
                result *= radix;
                if (result < limit + digit) {
                    NumberFormatException(
                        "Error at index "_S + (i - beginIndex) + ", in \""_S + s.subSequence(beginIndex, endIndex) +
                        "\"."_S).throws($ftrace());
                }
                i++;
                result -= digit;
            }
            return negative ? result : -result;
        } else {
            NumberFormatException("For input string: \""_S + s + "\" under radix "_S + radix).throws(
                $ftrace());
        }
    }

    gint Integer::parseInt(const String &s) {
        try {
            return parseInt(s, 10);
        } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
    }

    gint Integer::parseUnsignedInt(const String &s, gint radix) {
        gint len = s.length();
        if (len > 0) {
            gchar firstChar = s.charAt(0);
            if (firstChar == '-') {
                NumberFormatException(("Illegal leading minus sign "_S
                                       "on unsigned string "_S + s + "."_S)).throws($ftrace());
            } else {
                if (len <= 5 || // Integer.MAX_VALUE in Character.MAX_RADIX is 6 digits
                    (radix == 10 && len <= 9)) {
                    // Integer.MAX_VALUE in base 10 is 10 digits
                    try {
                        return parseInt(s, radix);
                    } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
                } else {
                    glong ell;
                    try {
                        ell = Long::parseLong(s, radix);
                    } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
                    if ((ell & 0xffffffff00000000LL) == 0) {
                        return (gint) ell;
                    } else {
                        NumberFormatException("String value "_S + s +
                                              " exceeds range of unsigned int."_S).throws($ftrace());
                    }
                }
            }
        } else {
            NumberFormatException("For input string: \""_S + s + "\" under radix "_S + radix).throws(
                $ftrace());
        }
    }

    gint Integer::parseUnsignedInt(const CharSequence &s, gint beginIndex, gint endIndex, gint radix) {
        if (beginIndex < 0 || beginIndex > endIndex || endIndex > s.length()) {
            IndexOutOfBoundsException("Range index ["_S + beginIndex + ", "_S
                                      + endIndex + " out of bounds for length "_S + s.length()).throws(
                $ftrace());
        }

        gint start = beginIndex, len = endIndex - beginIndex;

        if (len > 0) {
            gchar firstChar = s.charAt(start);
            if (firstChar == '-') {
                NumberFormatException("Illegal leading minus sign "_S
                                      "on unsigned string "_S + s.toString() + "."_S).throws($ftrace());
            } else {
                if (len <= 5 || // Integer::MAX_VALUE in Character::MAX_RADIX is 6 digits
                    (radix == 10 && len <= 9)) {
                    // Integer::MAX_VALUE in base 10 is 10 digits
                    try {
                        return parseInt(s, start, start + len, radix);
                    } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
                } else {
                    glong ell;
                    try {
                        ell = Long::parseLong(s, start, start + len, radix);
                    } catch (NumberFormatException const &ex) { ex.throws($ftrace()); }
                    if ((ell & 0xffffffff00000000LL) == 0) {
                        return (gint) ell;
                    } else {
                        NumberFormatException("String value "_S + s.toString() +
                                              " exceeds range of unsigned int."_S).throws($ftrace());
                    }
                }
            }
        } else {
            NumberFormatException().throws($ftrace());
        }
    }

    gint Integer::parseUnsignedInt(const String &s) {
        try {
            return parseUnsignedInt(s, 10);
        } catch (NumberFormatException const &ex) {
            ex.throws($ftrace());
        }
    }

    Integer Integer::valueOf(const String &s, gint radix) {
        try {
            return parseUnsignedInt(s, radix);
        } catch (NumberFormatException const &ex) {
            ex.throws($ftrace());
        }
    }

    Integer Integer::valueOf(const String &s) {
        try {
            return valueOf(s, 10);
        } catch (NumberFormatException const &ex) {
            ex.throws($ftrace());
        }
    }

    Integer Integer::valueOf(gint i) {
        return i;
    }

    Integer::Integer(gint value) : value(value) {
    }

    gbyte Integer::byteValue() const {
        return (gbyte) value;
    }

    gshort Integer::shortValue() const {
        return (gshort) value;
    }

    gint Integer::intValue() const {
        return value;
    }

    glong Integer::longValue() const {
        return value;
    }

    gfloat Integer::floatValue() const {
        return (gfloat) value;
    }

    gdouble Integer::doubleValue() const {
        return (gdouble) value;
    }

    String Integer::toString() const {
        return toString(value);
    }

    gint Integer::hash() const {
        return hash(value);
    }

    gint Integer::hash(gint value) {
        return value;
    }

    gbool Integer::equals(const Object &obj) const {
        return this == &obj || Class<Integer>::hasInstance(obj) && value == CORE_XCAST(Integer const, obj).value;
    }

    Integer Integer::decode(const String &nm) {
        gint radix = 10;
        gint index = 0;
        gbool negative = false;
        gint result;

        if (nm.isEmpty()) {
            NumberFormatException("Zero length string"_S).throws($ftrace());
        }
        gchar firstChar = nm.charAt(0);
        // Handle sign, if present
        if (firstChar == '-') {
            negative = true;
            index++;
        } else if (firstChar == '+') {
            index++;
        }

        // Handle radix specifier, if present
        if (nm.startsWith("0x"_S, index) || nm.startsWith("0X"_S, index)) {
            index += 2;
            radix = 16;
        } else if (nm.startsWith("0b"_S, index) || nm.startsWith("0B"_S, index)) {
            index += 2;
            radix = 2;
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
            result = parseInt(nm, index, nm.length(), radix);
            result = negative ? -result : result;
        } catch (NumberFormatException const &e) {
            // If number is Integer.MIN_VALUE, we'll end up here. The next line
            // handles this case, and causes any genuine format error to be
            // rethrown.
            String constant = negative
                                  ? ("-"_S + nm.subString(index))
                                  : nm.subString(index);
            try {
                result = parseInt(constant, radix);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }
        return result;
    }

    gint Integer::compareTo(Integer const &anotherInteger) const {
        return compare(value, anotherInteger.value);
    }

    gint Integer::compare(gint x, gint y) {
        return x - y;
    }

    gint Integer::compareUnsigned(gint x, gint y) {
        return compare(x + MIN_VALUE, y + MIN_VALUE);
    }

    glong Integer::toUnsignedLong(gint x) {
        return ((glong) x) & 0xffffffffLL;
    }

    gint Integer::divideUnsigned(gint dividend, gint divisor) {
        // In lieu of tricky code, for now just use long arithmetic.
        return (gint) (toUnsignedLong(dividend) / toUnsignedLong(divisor));
    }

    gint Integer::remainderUnsigned(gint dividend, gint divisor) {
        // In lieu of tricky code, for now just use long arithmetic.
        return (gint) (toUnsignedLong(dividend) % toUnsignedLong(divisor));
    }

    gint Integer::highestOneBit(gint i) {
        return i & (MIN_VALUE >> numberOfLeadingZeros(i));
    }

    gint Integer::lowestOneBit(gint i) {
        // HD, Section 2-1
        return i & -i;
    }

    gint Integer::numberOfLeadingZeros(gint i) {
        // HD, Count leading 0's
        if (i <= 0)
            return i == 0 ? 32 : 0;
        gint n = 31;
        if (i >= 1 << 16) {
            n -= 16;
            i >>= 16;
        }
        if (i >= 1 << 8) {
            n -= 8;
            i >>= 8;
        }
        if (i >= 1 << 4) {
            n -= 4;
            i >>= 4;
        }
        if (i >= 1 << 2) {
            n -= 2;
            i >>= 2;
        }
        return n - (i >> 1);
    }

    gint Integer::numberOfTrailingZeros(gint i) {
        // HD, Count trailing 0's
        i = ~i & (i - 1);
        if (i <= 0) return i & 32;
        gint n = 1;
        if (i > 1 << 16) {
            n += 16;
            i >>= 16;
        }
        if (i > 1 << 8) {
            n += 8;
            i >>= 8;
        }
        if (i > 1 << 4) {
            n += 4;
            i >>= 4;
        }
        if (i > 1 << 2) {
            n += 2;
            i >>= 2;
        }
        return n + (i >> 1);
    }

    gint Integer::bitCount(gint i) {
        // HD, Figure 5-2
        i = i - (gint) (((i + 0u) >> 1) & 0x55555555);
        i = (i & 0x33333333) + (gint) (((i + 0u) >> 2) & 0x33333333);
        i = (i + (gint) ((i + 0u) >> 4)) & 0x0f0f0f0f;
        i = i + (gint) ((i + 0u) >> 8);
        i = i + (gint) ((i + 0u) >> 16);
        return i & 0x3f;
    }

    gint Integer::rotateLeft(gint i, gint distance) {
        return (i << distance) | (gint) ((i + 0u) >> -distance);
    }

    gint Integer::rotateRight(gint i, gint distance) {
        return (gint) ((i + 0u) >> distance) | (i << -distance);
    }

    gint Integer::reverse(gint i) {
        // HD, Figure 7-1
        i = (i & 0x55555555) << 1 | (gint) ((i + 0u) >> 1) & 0x55555555;
        i = (i & 0x33333333) << 2 | (gint) ((i + 0u) >> 2) & 0x33333333;
        i = (i & 0x0f0f0f0f) << 4 | (gint) ((i + 0u) >> 4) & 0x0f0f0f0f;

        return reverseBytes(i);
    }

    gint Integer::signum(gint i) {
        // HD, Section 2-7
        return (i >> 31) | (gint) ((-i + 0u) >> 31);
    }

    gint Integer::reverseBytes(gint i) {
        return (gint) ((i << 24) |
                       ((i & 0xff00) << 8) |
                       (((i + 0u) >> 8) & 0xff00) |
                       ((i + 0u) >> 24));
    }

    gint Integer::sum(gint a, gint b) {
        return a + b;
    }

    gint Integer::max(gint a, gint b) {
        return Math::max(a, b);
    }

    gint Integer::min(gint a, gint b) {
        return Math::min(a, b);
    }

    Integer::operator gint() const {
        return intValue();
    }

    Integer::operator gint &() {
        return value;
    }

    Object &Integer::clone() const {
        try {
            return UNSAFE::newInstance<Integer>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
