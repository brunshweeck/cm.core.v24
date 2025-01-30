//
// Created by bruns on 10/05/2024.
//

#ifndef CORE24_STRINGUTILS_H
#define CORE24_STRINGUTILS_H


namespace core {
    using namespace util;

    class StringUtils final : public Object {
    public:
        CORE_ALIAS(CHARS, Class< gchar >::Pointer);
        CORE_ALIAS(INTS, Class< gint >::Pointer);
        CORE_ALIAS(LONGS, Class< glong >::Pointer);
        CORE_ALIAS(BYTES, Class< gbyte >::Pointer);
        CORE_ALIAS(UNSAFE, misc::Unsafe);

        static void copyLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static void copyLatin1ToUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static void copyLatin1ToUTF16(BYTES val1, gint off1, CHARS val2, gint off2, gint count);

        static void copyLatin1ToUTF32(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static void copyLatin1ToUTF32(BYTES val1, gint off1, INTS val2, gint off2, gint count);

        static void copyUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static void copyUTF16(BYTES val1, gint off1, CHARS val2, gint off2, gint count);

        static void copyUTF16(CHARS val1, gint off1, BYTES val2, gint off2, gint count);

        static void copyUTF16ToLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static void copyUTF16ToLatin1(CHARS val1, gint off1, BYTES val2, gint off2, gint count);

        static gint copyUTF16ToUTF32(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint copyUTF16ToUTF32(BYTES val1, gint off1, INTS val2, gint off2, gint count);

        static gint copyUTF16ToUTF32(CHARS val1, gint off1, BYTES val2, gint off2, gint count);

        static gint copyUTF16ToUTF32(CHARS val1, gint off1, INTS val2, gint off2, gint count);

        static void copyUTF32ToUTF16(INTS val1, gint off1, BYTES val2, gint off2, gint count);

        static BYTES copyOfLatin1(BYTES val, gint off, gint count);

        static BYTES copyOfLatin1(BYTES val, gint off, gint count, gint newLength);

        static BYTES copyOfUTF16(BYTES val, gint off, gint count);

        static BYTES copyOfUTF16(BYTES val, gint off, gint count, gint newLength);

        static BYTES copyOfUTF16(CHARS val, gint off, gint count);

        static BYTES copyOfUTF16(CHARS val, gint off, gint count, gint newLength);

        static BYTES copyOfLatin1ToUTF16(BYTES val, gint off, gint count);

        static BYTES copyOfLatin1ToUTF16(BYTES val, gint off, gint count, gint newLength);

        static BYTES copyOfUTF16ToLatin1(BYTES val, gint off, gint count);

        static BYTES copyOfUTF16ToLatin1(BYTES val, gint off, gint count, gint newLength);

        static BYTES copyOfUTF32ToUTF16(INTS val, gint off, gint count, gint &length);

        static BYTES newLatin1String(glong count);

        static BYTES newUTF16String(glong count);

        static BYTES extendLatin1String(BYTES val, gint oldCount, gint count);

        static BYTES extendUTF16String(BYTES val, gint oldCount, gint count);

        static gchar readLatin1CharAt(BYTES val, gint index);

        static gchar readUTF16CharAt(BYTES val, gint index);

        static gchar readUTF16CharAt(CHARS val, gint index);

        static gint readUTF32CharAt(BYTES val, gint index);

        static gint readUTF32CharAt(CHARS val, gint index);

        static gint readUTF32CharAt(INTS val, gint index);

        static glong readLong(BYTES val, gint index);

        static gint readInt(BYTES val, gint index);

        static gint readLatin1CodePointAt(BYTES val, gint index);

        static gint readUTF16CodePointAt(BYTES val, gint index);

        static gint readUTF16CodePointAt(CHARS val, gint index);

        static gint readUTF32CodePointAt(BYTES val, gint index);

        static gint readUTF32CodePointAt(CHARS val, gint index);

        static gint readUTF32CodePointAt(INTS val, gint index);

        static BYTES inflateUTF16ToLatin1(BYTES val, gint off, gint count);

        static BYTES inflateUTF16ToLatin1(CHARS val, gint off, gint count);

        static BYTES inflateUTF32ToLatin1(INTS val, gint off, gint count);

        static void destroyString(BYTES val);

        static void fillLatin1String(BYTES val, gint off, gint count, gchar value);

        static void fillUTF16String(BYTES val, gint off, gint count, gchar value);

        static void writeLatin1CharAt(BYTES val, gint index, gchar value);

        static void writeUTF16CharAt(BYTES val, gint index, gchar value);

        static void writeUTF16CharAt(CHARS val, gint index, gchar value);

        static void writeUTF32CharAt(BYTES val, gint index, gint value);

        static void writeUTF32CharAt(CHARS val, gint index, gint value);

        static void writeUTF32CharAt(INTS val, gint index, gint value);

        static gbool isLatin1(gint ch);

        static gbool isHighSurrogate(gint ch);

        static gbool isLowSurrogate(gint ch);

        static gint toCodePoint(gchar high, gchar low);

        static gchar highSurrogate(gint codePoint);

        static gchar lowSurrogate(gint codePoint);

        static gbool isSupplementary(gint codePoint);

        static gint compareToLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint compareToLatin1IC(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint compareToUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint compareToUTF16IC(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint compareUTF16ToLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint compareUTF16ToLatin1IC(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint compareLatin1ToUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint compareLatin1ToUTF16IC(BYTES val1, gint off1, BYTES val2, gint off2, gint count);

        static gint indexOfLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2);

        static gint indexOfLatin1$UTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2);

        static gint indexOfUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2);

        static gint lastIndexOfLatin1(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2);

        static gint lastIndexOfLatin1$UTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2);

        static gint lastIndexOfUTF16(BYTES val1, gint off1, BYTES val2, gint off2, gint count1, gint count2);

        static gint numberOfLatin1CodePoints(BYTES val, gint off, gint count);

        static gint numberOfUTF16CodePoints(BYTES val, gint off, gint count);

        static gint hashLatin1String(BYTES val, gint off, gint count);

        static gint hashUTF16String(BYTES val, gint off, gint count);

        static gint hashUTF16String(CHARS val, gint off, gint count);

        static gint hashUTF32String(BYTES val, gint off, gint count);

        static gint hashUTF32String(INTS val, gint off, gint count);

        static gint indexOfLatin1(BYTES val, gint off, gchar c, gint count);

        static gint indexOfLatin1(BYTES val, gint off, gint c, gint count);

        static gint indexOfUTF16(BYTES val, gint off, gchar c, gint count);

        static gint indexOfUTF16(BYTES val, gint off, gint c, gint count);

        static gint lastIndexOfLatin1(BYTES val, gint off, gchar c, gint count);

        static gint lastIndexOfLatin1(BYTES val, gint off, gint c, gint count);

        static gint lastIndexOfUTF16(BYTES val, gint off, gchar c, gint count);

        static gint lastIndexOfUTF16(BYTES val, gint off, gint c, gint count);

        static void shiftLatin1(BYTES val, gint off, gint n, gint count);

        static void shiftUTF16(BYTES val, gint off, gint n, gint count);

        // Fast version of Arrays::vectorizedMismatch optimized for String class.
        static gint vectorizeMismatch(BYTES a, BYTES b, glong length, gint log2IndexScale);
        static gint mismatch(BYTES a, gint aOff, BYTES b, gint bOff, gint count, gint log2IndexScale);
    };
} // core

#endif // CORE24_STRINGUTILS_H
