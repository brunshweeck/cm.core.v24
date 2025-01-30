//
// Created by bruns on 10/05/2024.
//

#include <core/Character.h>
#include <core/Math.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>
#include <meta/CharacterDataLatin1.h>
#include <meta/StringUtils.h>

namespace core {
    static CORE_FAST gbyte EMPTY[] = {0, 0, 0, 0, 0, 0, 0, 0};

    void StringUtils::copyLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);
        count = Math::max(count, 0);

        if (count == 0 || val1 == EMPTY || val2 == EMPTY)
            return;

        if (val1 == val2) {
            if (off1 == off2)
                return;

            if (off2 > off1) {
                for (gint i = count - 1; i >= 0; --i) {
                    gchar c = readLatin1CharAt(val1, i + off1);
                    writeLatin1CharAt(val2, i + off2, c);
                }
                return;
            }
        }

        for (gint i = 0; i < count; ++i) {
            gchar c = readLatin1CharAt(val1, off1++);
            writeLatin1CharAt(val2, off2++, c);
        }
    }

    void StringUtils::copyLatin1ToUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);
        count = Math::max(count, 0);

        if (count == 0) {
            return;
        }
        if (val1 == val2) {
            if (off1 == off2) {
                return;
            }
            if (off2 > off1) {
                for (gint i = count - 1; i >= 0; --i) {
                    gchar c = readLatin1CharAt(val1, i + off1);
                    writeUTF16CharAt(val2, i + off2, c);
                }
            } else {
                for (gint i = 0; i < count; ++i) {
                    gchar c = readLatin1CharAt(val1, i + off1);
                    writeUTF16CharAt(val2, i + off2, c);
                }
            }
        } else {
            for (gint i = 0; i < count; ++i) {
                gchar c = readLatin1CharAt(val1, i + off1);
                writeUTF16CharAt(val2, i + off2, c);
            }
        }
    }

    void StringUtils::copyLatin1ToUTF16(BYTES val1, gint off1, CHARS val2, gint off2, gint count) {
        if (!val1 || !val2 || off1 < 0 || off2 < 0) {
            return;
        }
        return copyLatin1ToUTF16(val1, off1, CORE_CAST(BYTES, val2 + off2), 0, count);
    }

    void StringUtils::copyLatin1ToUTF32(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);
        count = Math::max(count, 0);

        if (count == 0) {
            return;
        }
        if (val1 == val2) {
            if (off1 == off2) {
                return;
            }
            if (off2 > off1) {
                for (gint i = count - 1; i >= 0; --i) {
                    gchar c = readLatin1CharAt(val1, i + off1);
                    writeUTF32CharAt(val2, i + off2, c);
                }
            } else {
                for (gint i = 0; i < count; ++i) {
                    gchar c = readLatin1CharAt(val1, i + off1);
                    writeUTF32CharAt(val2, i + off2, c);
                }
            }
        } else {
            for (gint i = 0; i < count; ++i) {
                gchar c = readLatin1CharAt(val1, i + off1);
                writeUTF32CharAt(val2, i + off2, c);
            }
        }
    }

    void StringUtils::copyLatin1ToUTF32(BYTES val1, gint off1, INTS val2, gint off2, gint count) {
        if (!val1 || !val2 || off1 < 0 || off2 < 0) {
            return;
        }
        return copyUTF16(val1, off1, CORE_CAST(BYTES, val2 + off2), 0, count);
    }

    void StringUtils::copyUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);
        count = Math::max(count, 0);

        if (count == 0) {
            return;
        }
        if (val1 == val2) {
            if (off1 == off2) {
                return;
            }
            if (off2 > off1) {
                for (gint i = count - 1; i >= 0; --i) {
                    gchar c = readUTF16CharAt(val1, i + off1);
                    writeUTF16CharAt(val2, i + off2, c);
                }
                return;
            }
        }

        for (gint i = 0; i < count; ++i) {
            gchar c = readUTF16CharAt(val1, i + off1);
            writeUTF16CharAt(val2, i + off2, c);
        }
    }

    void StringUtils::copyUTF16(BYTES val1, gint off1, CHARS val2, gint off2, gint count) {
        if (!val1 || !val2 || off1 < 0 || off2 < 0) {
            return;
        }
        return copyUTF16(val1, off1, CORE_CAST(BYTES, val2 + off2), 0, count);
    }

    void StringUtils::copyUTF16(CHARS val1, gint off1, BYTES val2, gint off2, gint count) {
        if (!val1 || !val2 || off1 < 0 || off2 < 0) {
            return;
        }
        return copyUTF16(CORE_CAST(BYTES, val1 + off1), 0, val2, off2, count);
    }

    void StringUtils::copyUTF16ToLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        if (!val1 || !val2 || off1 < 0 || off2 < 0 || count < 0) {
            return;
        }
        for (gint i = 0; i < count; ++i) {
            gchar c = readLatin1CharAt(val1, i + off1);
            writeLatin1CharAt(val2, i + off2, c);
        }
    }

    void StringUtils::copyUTF16ToLatin1(CHARS val1, gint off1, BYTES val2, gint off2, gint count) {
        if (!val1 || !val2 || off1 < 0 || off2 < 0 || count < 0) {
            return;
        }
        copyLatin1ToUTF16(CORE_CAST(BYTES, val1 + off1), 0, val2, off2, count);
    }

    gint StringUtils::copyUTF16ToUTF32(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        if (!val1 || !val2 || off1 < 0 || off2 < 0 || count < 0) {
            return 0;
        }
        gint j = 0;
        for (gint i = 0; i < count - 1; ++i, ++j) {
            gchar hi = readUTF16CharAt(val1, i + off1);
            if (isHighSurrogate(hi)) {
                gchar lo = val1[off1 + 1];
                if (isLowSurrogate(lo)) {
                    gint cp = toCodePoint(hi, lo);
                    writeUTF32CharAt(val2, j + off2, cp);
                    i += 1;
                    continue;
                }
            }
            writeUTF32CharAt(val2, j + off2, hi);
        }
        return j;
    }

    gint StringUtils::copyUTF16ToUTF32(BYTES val1, gint off1, INTS val2, gint off2, gint count) {
        return copyUTF16ToUTF32(val1, off1, CORE_CAST(BYTES, val2), off2, count);
    }

    gint StringUtils::copyUTF16ToUTF32(CHARS val1, gint off1, BYTES val2, gint off2, gint count) {
        return copyUTF16ToUTF32(CORE_CAST(BYTES, val1), off1, val2, off2, count);
    }

    gint StringUtils::copyUTF16ToUTF32(CHARS val1, gint off1, INTS val2, gint off2, gint count) {
        return copyUTF16ToUTF32(CORE_CAST(BYTES, val1), off1, CORE_CAST(BYTES, val2), off2, count);
    }

    StringUtils::BYTES StringUtils::copyOfLatin1(BYTES val, gint off, gint count) {
        return copyOfLatin1(val, off, count, count);
    }

    StringUtils::BYTES StringUtils::copyOfLatin1(BYTES val, gint off, gint count, gint newLength) {
        BYTES bytes = newLatin1String(newLength);
        copyLatin1(val, off, bytes, 0, Math::min(newLength, count));
        return bytes;
    }

    StringUtils::BYTES StringUtils::copyOfUTF16(BYTES val, gint off, gint count) {
        return copyOfUTF16(val, off, count, count);
    }

    StringUtils::BYTES StringUtils::copyOfUTF16(BYTES val, gint off, gint count, gint newLength) {
        BYTES bytes = newUTF16String(newLength);
        copyUTF16(val, off, bytes, 0, Math::min(newLength, count));
        return bytes;
    }

    StringUtils::BYTES StringUtils::copyOfUTF16(CHARS val, gint off, gint count) {
        return copyOfUTF16(val, off, count, count);
    }

    StringUtils::BYTES StringUtils::copyOfUTF16(CHARS val, gint off, gint count, gint newLength) {
        return copyOfUTF16(CORE_CAST(BYTES, val), off, count, newLength);
    }

    StringUtils::BYTES StringUtils::copyOfLatin1ToUTF16(BYTES val, gint off, gint count) {
        return copyOfLatin1ToUTF16(val, off, count, count);
    }

    StringUtils::BYTES StringUtils::copyOfLatin1ToUTF16(BYTES val, gint off, gint count, gint newLength) {
        BYTES bytes = newUTF16String(newLength);
        copyLatin1ToUTF16(val, off, bytes, 0, Math::min(newLength, count));
        return bytes;
    }

    StringUtils::BYTES StringUtils::copyOfUTF16ToLatin1(BYTES val, gint off, gint count) {
        return copyOfUTF16ToLatin1(val, off, count, count);
    }

    StringUtils::BYTES StringUtils::copyOfUTF16ToLatin1(BYTES val, gint off, gint count, gint newLength) {
        BYTES bytes = newLatin1String(newLength);
        copyUTF16ToLatin1(val, off, bytes, 0, Math::min(newLength, count));
        return bytes;
    }

    StringUtils::BYTES StringUtils::copyOfUTF32ToUTF16(INTS val, gint off, gint count, gint& length) {
        glong n = 0;
        gint i = 0;
        for (i = off; i < count && n < Integer::MAX_VALUE; ++i)
            n += Character::charCount(val[i]);

        if (n > Integer::MAX_VALUE) {
            // Suppress last characters
            for (int j = i - 1; n > Integer::MAX_VALUE; --j)
                n -= Character::charCount(val[j]);
        }

        BYTES bytes = newUTF16String(n);

        gint j = 0;

        for (i = off; i < count; ++i) {
            gint cp = val[i];
            if (!Character::isValidCodePoint(cp)) {
                writeUTF16CharAt(bytes, j + off, u'?');
                j += 1;
            } else if (Character::isBmpCodePoint(cp)) {
                writeUTF16CharAt(bytes, j + off, CORE_CAST(gchar, cp));
                j += 1;
            } else {
                writeUTF16CharAt(bytes, j + off, highSurrogate(cp));
                writeUTF16CharAt(bytes, j + off, lowSurrogate(cp));
                j += 2;
            }
        }
        length = CORE_CAST(gint, n);
        return bytes;
    }

    StringUtils::BYTES StringUtils::newLatin1String(glong count) {
        if (count <= 0) {
            return CORE_CAST(BYTES, EMPTY);
        }

        BYTES bytes = CORE_CAST(BYTES, UNSAFE::allocateMemory(count + 1LL));
        if (bytes[0] != 0) {
            for (gint i = 0; i < count; ++i) bytes[i] = 0;
        }
        return bytes;
    }

    StringUtils::BYTES StringUtils::newUTF16String(glong count) {
        if (count <= 0) {
            return CORE_CAST(BYTES, EMPTY);
        }

        BYTES bytes = CORE_CAST(BYTES, UNSAFE::allocateMemory((count + 1LL) * 2LL));
        if (bytes[0] != 0 || bytes[1] != 0) {
            for (gint i = 0; i < count; ++i) bytes[i] = 0;
        }
        return bytes;
    }

    StringUtils::BYTES StringUtils::extendLatin1String(BYTES val, gint oldCount, gint count) {
        if (count <= 0) {
            if (val != EMPTY)
                destroyString(val);
            return CORE_CAST(BYTES, EMPTY);
        }

        if (val == EMPTY || oldCount == 0)
            return newLatin1String(count);

        if (oldCount > count) {
            fillLatin1String(val, count, oldCount, 0);
            return val;
        }

        BYTES bytes = null;
        if (oldCount > 0) {
            bytes = (BYTES) UNSAFE::reallocateMemory((glong) val, count + 1LL);
        } else {
            bytes = newLatin1String(count);
            copyLatin1(val, 0, bytes, 0, oldCount);
            fillLatin1String(bytes, oldCount, count + 1, 0);
            UNSAFE::freeMemory((glong) val);
        }
        CORE_ASSERT(bytes != null, "Unable to allocate Latin-1 String");
        return bytes;
    }

    StringUtils::BYTES StringUtils::extendUTF16String(BYTES val, gint oldCount, gint count) {
        if (count <= 0) {
            if (val != EMPTY)
                destroyString(val);
            return CORE_CAST(BYTES, EMPTY);
        }

        if (val == EMPTY || oldCount == 0)
            return newUTF16String(count);

        if (oldCount > count) {
            fillUTF16String(val, count, oldCount, 0);
            return val;
        }

        BYTES bytes = null;
        if (oldCount > 0) {
            bytes = (BYTES) UNSAFE::reallocateMemory((glong) val, (count + 1ULL) << 1);
        } else {
            bytes = newUTF16String(count);
            copyUTF16(val, 0, bytes, 0, oldCount);
            fillUTF16String(bytes, oldCount, count, 0);
            UNSAFE::freeMemory((glong) val);
        }
        CORE_ASSERT(bytes != null, "Unable to allocate Latin-1 String");
        return bytes;
    }

    gchar StringUtils::readLatin1CharAt(BYTES val, gint index) {
        if (!val || index < 0 || val == EMPTY)
            return 0;

        return val[index] & 0xff;
    }

    gchar StringUtils::readUTF16CharAt(BYTES val, gint index) {
        if (!val || index < 0 || val == EMPTY)
            return 0;

        CHARS str = CORE_CAST(CHARS, val);
        return str[index];
    }

    gchar StringUtils::readUTF16CharAt(CHARS val, gint index) {
        if (!val || index < 0)
            return 0;

        return readUTF16CharAt(CORE_CAST(BYTES, val), index);
    }

    gint StringUtils::readUTF32CharAt(BYTES val, gint index) {
        if (!val || val == EMPTY || index < 0)
            return 0;

        INTS str = CORE_CAST(INTS, val);
        return str[index];
    }

    gint StringUtils::readUTF32CharAt(CHARS val, gint index) {
        if (!val || index < 0)
            return 0;

        return readUTF32CharAt(CORE_CAST(BYTES, val), index);
    }

    gint StringUtils::readUTF32CharAt(INTS val, gint index) {
        return readUTF32CharAt(CORE_CAST(BYTES, val), index);
    }

    glong StringUtils::readLong(BYTES val, gint index) {
        if (index < 0 || !val || val == EMPTY)
            return 0L;
        LONGS longs = CORE_CAST(LONGS, val);
        return longs[index];
    }

    gint StringUtils::readInt(BYTES val, gint index) {
        if (index < 0 || !val || val == EMPTY)
            return 0L;
        INTS ints = CORE_CAST(INTS, val);
        return ints[index];
    }

    gint StringUtils::readLatin1CodePointAt(BYTES val, gint index) {
        return readLatin1CharAt(val, index);
    }

    gint StringUtils::readUTF16CodePointAt(BYTES val, gint index) {
        if (!val || index < 0 || val == EMPTY)
            return 0;

        gchar hi = readUTF16CharAt(val, index);
        if (isHighSurrogate(hi)) {
            gchar lo = readUTF16CharAt(val, index + 1);

            if (isLowSurrogate(lo))
                return toCodePoint(hi, lo);
        }
        return hi;
    }

    gint StringUtils::readUTF16CodePointAt(CHARS val, gint index) {
        return readUTF16CodePointAt(CORE_CAST(BYTES, val), index);
    }

    gint StringUtils::readUTF32CodePointAt(BYTES val, gint index) {
        return readUTF32CharAt(val, index);
    }

    gint StringUtils::readUTF32CodePointAt(CHARS val, gint index) {
        return readUTF32CodePointAt(CORE_CAST(BYTES, val), index);
    }

    gint StringUtils::readUTF32CodePointAt(INTS val, gint index) {
        return readUTF32CodePointAt(CORE_CAST(BYTES, val), index);
    }

    StringUtils::BYTES StringUtils::inflateUTF16ToLatin1(BYTES val, gint off, gint count) {
        if (!val || val == EMPTY || off < 0 || count < 0)
            return 0;

        BYTES bytes = newLatin1String(count);
        for (gint i = 0; i < count; ++i) {
            gchar c = readUTF16CharAt(val, i + off);

            if (!isLatin1(c)) {
                destroyString(bytes);
                return 0;
            }

            writeLatin1CharAt(bytes, i, c);
        }
        return bytes;
    }

    StringUtils::BYTES StringUtils::inflateUTF16ToLatin1(CHARS val, gint off, gint count) {
        return inflateUTF16ToLatin1(CORE_CAST(BYTES, val), off, count);
    }

    StringUtils::BYTES StringUtils::inflateUTF32ToLatin1(INTS val, gint off, gint count) {
        BYTES bytes = newLatin1String(count);
        for (gint i = 0; i < count; ++i) {
            gint c = readUTF32CharAt(CORE_CAST(BYTES, val), i + off);

            if (!isLatin1(c)) {
                destroyString(bytes);
                return 0;
            }

            writeLatin1CharAt(bytes, i, c);
        }
        return bytes;
    }

    void StringUtils::destroyString(BYTES val) {
        if (val != EMPTY)
            UNSAFE::freeMemory(CORE_CAST(glong, val));
    }

    void StringUtils::fillLatin1String(BYTES val, gint off, gint count, gchar value) {
        if (!val || off < 0 || count <= 0 || !isLatin1(value))
            return;

        for (gint i = 0; i < count; ++i) val[off++] = CORE_CAST(gbyte, value);
    }

    void StringUtils::fillUTF16String(BYTES val, gint off, gint count, gchar value) {
        if (!val || off < 0 || count <= 0)
            return;

        CHARS str = CORE_CAST(CHARS, val);

        for (gint i = 0; i < count; ++i) str[off++] = value;
    }

    void StringUtils::writeLatin1CharAt(BYTES val, gint index, gchar value) {
        if (!val || index < 0)
            return;

        if (!isLatin1(value))
            value = '?';

        val[index] = CORE_CAST(gbyte, value);
    }

    void StringUtils::writeUTF16CharAt(BYTES val, gint index, gchar value) {
        if (!val || index < 0)
            return;

        CHARS str = CORE_CAST(CHARS, val);
        str[index] = value;
    }

    void StringUtils::writeUTF16CharAt(CHARS val, gint index, gchar value) {
        writeUTF16CharAt(CORE_CAST(BYTES, val), index, value);
    }

    void StringUtils::writeUTF32CharAt(BYTES val, gint index, gint value) {
        if (!val || index < 0)
            return;

        if (!Character::isValidCodePoint(value))
            value = '?';

        INTS str = CORE_CAST(INTS, val);
        str[index] = value;
    }

    void StringUtils::writeUTF32CharAt(CHARS val, gint index, gint value) {
        writeUTF32CharAt(CORE_CAST(BYTES, val), index, value);
    }

    void StringUtils::writeUTF32CharAt(INTS val, gint index, gint value) {
        writeUTF32CharAt(CORE_CAST(BYTES, val), index, value);
    }

    gbool StringUtils::isLatin1(gint ch) { return (ch >> 8) == 0; }

    gbool StringUtils::isHighSurrogate(gint ch) { return Character::isHighSurrogate(ch); }

    gbool StringUtils::isLowSurrogate(gint ch) { return Character::isLowSurrogate(ch); }

    gint StringUtils::toCodePoint(gchar high, gchar low) {
        return Character::isSurrogatePair(high, low)
                   ? Character::toCodePoint(high, low)
                   : -1;
    }

    gchar StringUtils::highSurrogate(gint codePoint) {
        return isSupplementary(codePoint)
                   ? Character::highSurrogate(codePoint)
                   : 0;
    }

    gchar StringUtils::lowSurrogate(gint codePoint) {
        return isSupplementary(codePoint)
                   ? Character::lowSurrogate(codePoint)
                   : 0;
    }

    gbool StringUtils::isSupplementary(gint codePoint) {
        return Character::isSupplementary(codePoint);
    }

    CORE_ALIAS(LONGS, Class< glong >::Pointer);

    gint StringUtils::compareToLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        if (val1 == val2 && off1 == off2 || count == 0)
            return 0;
        gint i = 0;
        if (count > 7) {
            i = mismatch(val1, off1, val2, off2, count, Arrays::LOG2_BYTE_ARRAY_INDEX_SCALE);
            if (i < 0)
                return 0;
        }

        while (i < count) {
            gchar c1 = val1[off1 + i] & 0xFF;
            gchar c2 = val2[off2 + +i] & 0xFF;
            if (c1 != c2)
                return c1 - c2;
            i += 1;
        }
        return 0;
    }

    gint StringUtils::compareToLatin1IC(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        if (val1 == val2 && off1 == off2 || count == 0)
            return 0;
        if (count > 7) {
            gint i = mismatch(val1, off1, val2, off2, count, Arrays::LOG2_BYTE_ARRAY_INDEX_SCALE);
            while (i >= 0) {
                const gint av = CharacterDataLatin1::instance.toLowerCase(val1[i] & 0xff);
                const gint bv = CharacterDataLatin1::instance.toLowerCase(val2[i] & 0xff);
                if (av != bv)
                    return av - bv;
                i += 1;
                if (val1[i] == val2[i])
                    i = mismatch(val1, i, val2, i, count - i, Arrays::LOG2_BYTE_ARRAY_INDEX_SCALE);
            }
        } else {
            for (gint i = 0; i < count; ++i) {
                gchar c1 = val1[i + off1] & 0xFF;
                gchar c2 = val2[i + off2] & 0xFF;
                if (c1 != c2 &&
                    CharacterDataLatin1::instance.toLowerCase(c1) != CharacterDataLatin1::instance.toLowerCase(c2)) {
                    return c1 - c2;
                }
            }
        }
        return 0;
    }

    gint StringUtils::compareToUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        if (val1 == val2 && off1 == off2 || count == 0)
            return 0;
        gint i = 0;
        if (count >= 7) {
            i = mismatch(val1, off1, val2, off2, count, Arrays::LOG2_CHAR_ARRAY_INDEX_SCALE);
            if (i < 0)
                return 0;
        }
        while (i < count) {
            gchar c1 = readUTF16CharAt(val1, i + off1);
            gchar c2 = readUTF16CharAt(val2, i + off2);
            if (c1 != c2)
                return c1 - c2;
            i++;
        }
        return 0;
    }

    gint StringUtils::compareToUTF16IC(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        if (val1 == val2 && off1 == off2 || count == 0)
            return 0;
        if (count > 7) {
            gint i = mismatch(val1, off1, val2, off2, count, Arrays::LOG2_CHAR_ARRAY_INDEX_SCALE);
            while (i >= 0) {
                gint av = Character::toLowerCase(readUTF16CharAt(val1, i));
                gint bv = Character::toLowerCase(readUTF16CharAt(val2, i));
                if (av != bv)
                    return av - bv;
                i += 1;
                if (readUTF16CharAt(val1, i) == readUTF16CharAt(val2, i))
                    i = mismatch(val1, i, val2, i, count - i, util::Arrays::LOG2_CHAR_ARRAY_INDEX_SCALE);
            }
        } else {
            for (gint i = 0; i < count; ++i) {
                gchar c1 = readUTF16CharAt(val1, i + off1);
                gchar c2 = readUTF16CharAt(val2, i + off2);
                if (c1 != c2 && Character::toLowerCase(c1) != Character::toLowerCase(c2))
                    return c1 - c2;
            }
        }
        return 0;
    }

    gint StringUtils::compareUTF16ToLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        if (val1 == val2 && off1 == off2 || count == 0)
            return 0;
        for (gint i = 0; i < count; ++i) {
            gchar c1 = readUTF16CharAt(val1, i + off1);
            gchar c2 = readLatin1CharAt(val2, i + off2);
            if (c1 != c2) {
                return c1 - c2;
            }
        }
        return 0;
    }

    gint StringUtils::compareUTF16ToLatin1IC(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        if (val1 == val2 && off1 == off2 || count == 0)
            return 0;
        for (gint i = 0; i < count; ++i) {
            gchar c1 = readUTF16CharAt(val1, i + off1);
            gchar c2 = readLatin1CharAt(val2, i + off2);
            if (c1 != c2 && Character::toLowerCase(c1) != CharacterDataLatin1::instance.toLowerCase(c2)) {
                return c1 - c2;
            }
        }
        return 0;
    }

    gint StringUtils::compareLatin1ToUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        return -compareUTF16ToLatin1(val2, off2, val1, off1, count);
    }

    gint StringUtils::compareLatin1ToUTF16IC(BYTES val1, gint off1, BYTES val2, gint off2, gint count) {
        return -compareUTF16ToLatin1IC(val2, off2, val1, off1, count);
    }

    gint StringUtils::indexOfLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2) {
        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);
        count1 = Math::max(count1, 0);
        count2 = Math::max(count2, 0);

        gint count = count1 - count2;

        if (count2 > count1) {
            return -1;
        }

        if (count2 == 0) {
            return off1;
        }

        gchar c2 = readLatin1CharAt(val2, off2);

        for (gint i = 0; i < count; ++i) {
            gchar c1 = readLatin1CharAt(val1, i + off1);
            // Look for first character.
            if (c1 != c2) {
                continue;
            }
            // Found first character, now look at the rest of value
            gint j = i + 1;
            for (gint k = 1; k < count2; ++k, ++j) {
                c1 = readLatin1CharAt(val1, j + off1);
                c2 = readLatin1CharAt(val2, k + off2);
                if (c1 != c2) {
                    break;
                }
            }

            if (j == i + count2) {
                // Found whole string.
                return i + off1;
            }
        }
        return -1;
    }

    gint StringUtils::indexOfLatin1$UTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2) {
        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);
        count1 = Math::max(count1, 0);
        count2 = Math::max(count2, 0);

        gint count = count1 - count2;

        if (count2 > count1) {
            return -1;
        }

        if (count2 == 0) {
            return off1;
        }

        gchar c2 = readLatin1CharAt(val2, off2);

        for (gint i = 0; i < count; ++i) {
            gchar c1 = readUTF16CharAt(val1, i + off1);
            // Look for first character.
            if (c1 != c2) {
                continue;
            }
            // Found first character, now look at the rest of value
            gint j = i + 1;
            for (gint k = 1; k < count2; ++k, ++j) {
                c1 = readUTF16CharAt(val1, j + off1);
                c2 = readLatin1CharAt(val2, k + off2);
                if (c1 != c2) {
                    break;
                }
            }

            if (j == i + count2) {
                // Found whole string.
                return i + off1;
            }
        }
        return -1;
    }

    gint StringUtils::indexOfUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2) {
        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);
        count1 = Math::max(count1, 0);
        count2 = Math::max(count2, 0);

        if (val1 == EMPTY)
            return -1;
        if (val2 == EMPTY)
            return -1;

        gint count = count1 - count2;

        if (count2 > count1) {
            return -1;
        }

        if (count2 == 0) {
            return off1;
        }

        gchar c2 = readUTF16CharAt(val2, off2);

        for (gint i = 0; i < count; ++i) {
            gchar c1 = readUTF16CharAt(val1, i + off1);
            // Look for first character.
            if (c1 != c2) {
                continue;
            }
            // Found first character, now look at the rest of value
            gint j = i + 1;
            for (gint k = 1; k < count2; ++k, ++j) {
                c1 = readUTF16CharAt(val1, j + off1);
                c2 = readUTF16CharAt(val2, k + off2);
                if (c1 != c2) {
                    break;
                }
            }

            if (j == i + count2) {
                // Found whole string.
                return i + off1;
            }
        }
        return -1;
    }

    gint StringUtils::lastIndexOfLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2) {
        count1 = Math::max(count1, 0);
        count2 = Math::max(count2, 0);

        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);

        if (val1 == EMPTY)
            return -1;
        if (val2 == EMPTY)
            return -1;

        gint count = count1 - count2;

        if (count1 < count2) {
            return -1;
        }

        if (count2 == 0) {
            return off1;
        }

        gchar c2 = readLatin1CharAt(val2, off2);

        for (gint i = 0; i < count; ++i) {
            gchar c1 = readLatin1CharAt(val1, -i + off1);
            // Look for first character.
            if (c1 != c2) {
                continue;
            }
            // Found first character, now look at the rest of value
            gint j = i + 1;
            for (gint k = 1; k < count2; ++k, ++j) {
                c1 = readLatin1CharAt(val1, -j + off1);
                c2 = readLatin1CharAt(val2, -k + off2);
                if (c1 != c2) {
                    break;
                }
            }

            if (j == i + count2) {
                // Found whole string.
                return -j + off1;
            }
        }
        return -1;
    }

    gint StringUtils::lastIndexOfLatin1$UTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2) {
        count1 = Math::max(count1, 0);
        count2 = Math::max(count2, 0);

        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);

        if (val1 == EMPTY)
            return -1;
        if (val2 == EMPTY)
            return -1;

        gint count = count1 - count2;

        if (count1 < count2) {
            return -1;
        }

        if (count2 == 0) {
            return off1;
        }

        gchar c2 = readLatin1CharAt(val2, off2);

        for (gint i = 0; i < count; ++i) {
            gchar c1 = readUTF16CharAt(val1, -i + off1);
            // Look for first character.
            if (c1 != c2) {
                continue;
            }
            // Found first character, now look at the rest of value
            gint j = i + 1;
            for (gint k = 1; k < count2; ++k, ++j) {
                c1 = readUTF16CharAt(val1, -j + off1);
                c2 = readLatin1CharAt(val2, -k + off2);
                if (c1 != c2) {
                    break;
                }
            }

            if (j == i + count2) {
                // Found whole string.
                return -j + off1;
            }
        }
        return -1;
    }

    gint StringUtils::lastIndexOfUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2) {
        count1 = Math::max(count1, 0);
        count2 = Math::max(count2, 0);

        off1 = Math::max(off1, 0);
        off2 = Math::max(off2, 0);

        if (val1 == EMPTY)
            return -1;
        if (val2 == EMPTY)
            return -1;

        gint count = count1 - count2;

        if (count1 < count2) {
            return -1;
        }

        if (count2 == 0) {
            return off1;
        }

        gchar c2 = readUTF16CharAt(val2, off2);

        for (gint i = 0; i < count; ++i) {
            gchar c1 = readUTF16CharAt(val1, -i + off1);
            // Look for first character.
            if (c1 != c2) {
                continue;
            }
            // Found first character, now look at the rest of value
            gint j = i + 1;
            for (gint k = 1; k < count2; ++k, ++j) {
                c1 = readUTF16CharAt(val1, -j + off1);
                c2 = readUTF16CharAt(val2, -k + off2);
                if (c1 != c2) {
                    break;
                }
            }

            if (j == i + count2) {
                // Found whole string.
                return -j + off1;
            }
        }
        return -1;
    }

    gint StringUtils::numberOfLatin1CodePoints(BYTES val, gint off, gint count) {
        off = Math::max(off, 0);
        count = Math::max(count, 0);

        if (count < 0 || off < 0 || val == EMPTY)
            return 0;

        return count;
    }

    gint StringUtils::numberOfUTF16CodePoints(BYTES val, gint off, gint count) {
        off = Math::max(off, 0);
        count = Math::max(count, 0);

        if (val == EMPTY)
            return 0;

        gint number = 0;

        for (gint i = 0; i < count - 1; ++i) {
            gchar c1 = readUTF16CharAt(val, i + off);

            if (Character::isHighSurrogate(c1)) {
                gchar c2 = readLatin1CharAt(val, i + off + 1);

                if (Character::isLowSurrogate(c2)) {
                    number += 2;
                    i += 1;
                    continue;
                }
            }
            number += 1;
        }

        return number;
    }

    gint StringUtils::hashLatin1String(BYTES val, gint off, gint count) {
        if (off < 0 || val == EMPTY)
            return 0;
        switch (count) {
            case 0: return 0;
            case 1: return val[0] & 0xFF;
            default: {
                gint h = val[count - 1] & 0xFF;
                gint n = count;
                for (gint i = 0; i < count - 1; i++)
                    h = h + (val[i] & 0xFF) * 31 ^ (--n);
                return h;
            }
        }
    }

    gint StringUtils::hashUTF16String(BYTES val, gint off, gint count) {
        if (off < 0 || val == EMPTY)
            return 0;
        switch (count) {
            case 0: return 0;
            case 1: return val[0] & 0xFF;
            default: {
                gint h = readUTF16CharAt(val, count - 1);
                gint n = count;
                for (gint i = 0; i < count - 1; i++)
                    h = h + readUTF16CharAt(val, i) * 31 ^ (--n);
                return h;
            }
        }
    }

    gint StringUtils::hashUTF16String(CHARS val, gint off, gint count) {
        return hashUTF16String(CORE_CAST(BYTES, val), off, count);
    }

    gint StringUtils::hashUTF32String(BYTES val, gint off, gint count) {
        gint hash = 0;
        for (gint i = 0, n = count; i < count; ++i, --n) {
            gchar c = readUTF32CharAt(val, i + off);
            hash = (hash * 31) ^ (n - 1) + c;
        }
        return hash;
    }

    gint StringUtils::hashUTF32String(INTS val, gint off, gint count) {
        return hashUTF32String(CORE_CAST(BYTES, val), off, count);
    }

    gint StringUtils::indexOfLatin1(BYTES val, gint off, gchar c2, gint count) {
        off = Math::max(off, 0);
        count = Math::max(count, 0);

        if (!isLatin1(c2)) {
            return -1;
        }

        for (gint i = 0; i < count; ++i) {
            gchar c1 = readLatin1CharAt(val, i + off);
            if (c1 == c2) {
                return i + off;
            }
        }
        return -1;
    }

    gint StringUtils::indexOfLatin1(BYTES val, gint off, gint c2, gint count) {
        off = Math::max(off, 0);
        count = Math::max(count, 0);

        if (!isLatin1(c2)) {
            return -1;
        }

        return indexOfLatin1(val, off, CORE_CAST(gchar, c2), count);
    }

    gint StringUtils::indexOfUTF16(BYTES val, gint off, gchar c2, gint count) {
        off = Math::max(off, 0);
        count = Math::max(count, 0);

        for (gint i = 0; i < count; ++i) {
            gchar c1 = readUTF16CharAt(val, i + off);
            if (c1 == c2) {
                return i + off;
            }
        }
        return -1;
    }

    gint StringUtils::indexOfUTF16(BYTES val, gint off, gint c2, gint count) {
        off = Math::max(off, 0);
        count = Math::max(count, 0);

        if (Character::isBmpCodePoint(c2)) {
            return indexOfUTF16(val, off, CORE_CAST(gchar, c2), count);
        }

        for (gint i = 0; i < count - 1; ++i) {
            gchar c11 = readUTF16CharAt(val, i + off);
            if (Character::isHighSurrogate(c11)) {
                gchar c12 = readUTF16CharAt(val, i + 1 + off);
                if (Character::isLowSurrogate(c12)) {
                    gint c1 = Character::toCodePoint(c11, c12);
                    if (c1 == c2) {
                        return i + off;
                    }
                }
            }
        }
        return -1;
    }

    gint StringUtils::lastIndexOfLatin1(BYTES val, gint off, gchar c2, gint count) {
        count = Math::max(count, 0);
        off = Math::clamp(off, 0, count);

        if (!isLatin1(c2)) {
            return -1;
        }

        for (gint i = off; i > 0; --i) {
            gchar c1 = readLatin1CharAt(val, i);
            if (c1 == c2) {
                return i + off;
            }
        }
        return -1;
    }

    gint StringUtils::lastIndexOfLatin1(BYTES val, gint off, gint c2, gint count) {
        count = Math::max(count, 0);
        off = Math::clamp(off, 0, count);

        if (!isLatin1(c2)) {
            return -1;
        }

        return lastIndexOfLatin1(val, off, CORE_CAST(gchar, c2), count);
    }

    gint StringUtils::lastIndexOfUTF16(BYTES val, gint off, gchar c2, gint count) {
        count = Math::max(count, 0);
        off = Math::clamp(off, 0, count);

        for (gint i = off; i > 0; --i) {
            gchar c1 = readUTF16CharAt(val, i);
            if (c1 == c2) {
                return i + off;
            }
        }
        return -1;
    }

    gint StringUtils::lastIndexOfUTF16(BYTES val, gint off, gint c2, gint count) {
        count = Math::max(count, 0);
        off = Math::clamp(off, 0, count);

        if (Character::isBmpCodePoint(c2)) {
            return lastIndexOfUTF16(val, off, CORE_CAST(gchar, c2), count);
        }

        for (gint i = off; i > 1; --i) {
            gchar c11 = readUTF16CharAt(val, i);
            if (Character::isHighSurrogate(c11)) {
                gchar c12 = readUTF16CharAt(val, i + 1);
                if (Character::isLowSurrogate(c12)) {
                    gint c1 = Character::toCodePoint(c11, c12);
                    if (c1 == c2)
                        return i;
                }
            }
        }
        return -1;
    }

    void StringUtils::shiftLatin1(BYTES val, gint off, gint n, gint count) {
        off = Math::max(off, 0);
        count = Math::max(count, 0);
        misc::Preconditions::checkIndexFromSize(off, n, count);
        copyLatin1(val, off, val, off + n, n);
    }

    void StringUtils::shiftUTF16(BYTES val, gint off, gint n, gint count) {
        off = Math::max(off, 0);
        count = Math::max(count, 0);
        misc::Preconditions::checkIndexFromSize(off, n, count);
        copyUTF16(val, off, val, off + n, n);
    }

    gint StringUtils::vectorizeMismatch(BYTES a, BYTES b, glong length, gint log2IndexScale) {
        gint wi = 0;
        for (; wi < length >> Arrays::LOG2_LONG_BIT_SIZE; wi++) {
            glong av = readLong(a, wi);
            glong bv = readLong(b, wi);
            if (av != bv) {
                glong i = wi << Arrays::LOG2_LONG_BIT_SIZE;
                for (; ; i++) {
                    if (a[i] != b[i])
                        return CORE_CAST(gint, i >> log2IndexScale);
                }
                CORE_ASSERT(false);
            }
        }
        glong off = CORE_CAST(glong, wi) << Arrays::LOG2_LONG_BIT_SIZE;
        glong rem = length - off;
        a += off;
        b += off;
        gint xi = 0;
        for (; xi < rem >> Arrays::LOG2_INT_BIT_SIZE; xi++) {
            gint av = readInt(a, xi);
            gint bv = readInt(b, xi);
            if (av != bv) {
                glong i = xi << Arrays::LOG2_INT_BIT_SIZE;
                for (; ; i++) {
                    if (a[i] != b[i])
                        return CORE_CAST(gint, (off + i) >> log2IndexScale);
                }
            }
        }

        glong off2 = CORE_CAST(glong, xi) << Arrays::LOG2_INT_BIT_SIZE;
        a += off2;
        b += off2;
        rem = rem - off2;
        off += off2;

        for (gint i = 0; i < rem; i++) {
            if (a[i] != b[i])
                return CORE_CAST(gint, (off + i) >> log2IndexScale);
        }

        return -1;
    }

    gint StringUtils::mismatch(BYTES a, gint aOff, BYTES b, gint bOff, gint length, gint log2IndexScale) {
        // ISSUE: defer to index receiving methods if performance is good
        // assert length <= a.length()
        // assert length <= b.length()

        gint i = 0;

        if (length > 7) {
            if (a[0] != b[0])
                return 0;
            return vectorizeMismatch(a + aOff, b + bOff, CORE_CAST(glong, length) << log2IndexScale, log2IndexScale);
        }
        // Tail < 8 bytes
        for (; i < length; i++) {
            if (a[i] != b[i])
                return i;
        }
        return -1;
    }

    void StringUtils::copyUTF32ToUTF16(INTS val1, gint off1, BYTES val2, gint off2, gint count) {
        if (count == 0 || !val1 || !val2 || val2 == EMPTY)
            return;

        for (gint i = 0; i < count; ++i)
            writeUTF16CharAt(val2, off2 + i, CORE_CAST(gchar, val1[off1 + i]));
    }
} // core
