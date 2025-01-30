//
// Created by brunshweeck on 29 mai 2024.
//

#include "XString.h"

#include <core/IllegalArgumentException.h>
#include <core/Math.h>
#include <core/OutOfMemoryError.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>
#include <meta/StringUtils.h>

namespace core {
    using namespace misc;
    using namespace util;

    XString::Coder XString::coding() const {
        return String::COMPACT_STRINGS && coder == String::LATIN1 ? String::LATIN1 : String::UTF16;
    }

    void XString::shift(gint offset, gint len) {
        if (!value) {
            if (String::COMPACT_STRINGS) {
                value = StringUtils::newLatin1String(16);
                coder = Coder::LATIN1;
            } else {
                value = StringUtils::newUTF16String(16);
                coder = Coder::UTF16;
            }
            count = 0;
            limit = 16;
        }

        gint cnt = count;
        gint lim = limit;
        Coder c = coding();

        try {
            Preconditions::checkIndex(offset, cnt);
            Preconditions::checkIndex(offset + len, lim);

            if (c == Coder::LATIN1)
                StringUtils::copyLatin1(value, offset, value, offset + len, cnt);
            else
                StringUtils::copyUTF16(value, offset, value, offset + len, cnt);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString::XString() : XString(16) {}

    XString::XString(gint capacity) {
        if (capacity < 0) {
            IllegalArgumentException("Negative capacity."_S).throws($ftrace());
        }

        gint length = Math::max(capacity, 16);
        if (String::COMPACT_STRINGS) {
            coder = String::LATIN1;
            value = StringUtils::newLatin1String(length);
        } else {
            coder = String::UTF16;
            value = StringUtils::newUTF16String(length);
        }
        limit = length;
        count = 0;
    }

    XString::XString(String const& str) {
        gint length = str.length();
        gint len = length < Integer::MAX_VALUE - 16 ? length + 16 : Integer::MAX_VALUE;

        Coder strCoder = str.coding();

        if (len < 0)
            len = Integer::MAX_VALUE;

        if (strCoder == String::LATIN1) {
            coder = String::LATIN1;
            value = StringUtils::newLatin1String(len);
        } else {
            coder = String::UTF16;
            value = StringUtils::newUTF16String(len);
        }
        limit = len;
        count = 0;
        append(str);
    }

    XString::XString(CharSequence const& seq) {
        gint len = seq.length();
        Coder c = Coder::LATIN1;
        gbool ml = false;

        if (len < 0)
            IllegalArgumentException("Negative String length."_S).throws($ftrace());

        len += 16;
        if (len < 0)
            len = Integer::MAX_VALUE;

        if (Class<XString>::hasInstance(seq)) {
            XString const& xs = CORE_XCAST(XString const, seq);
            c = xs.coding();
            ml = String::COMPACT_STRINGS && xs.maybeLatin1;
        } else if (Class<String>::hasInstance(seq))
            c = CORE_XCAST(String const, seq).coding();
        else
            c = String::COMPACT_STRINGS ? String::LATIN1 : String::UTF16;
        if (c == String::LATIN1)
            value = StringUtils::newLatin1String(len);
        else
            value = StringUtils::newUTF16String(len);
        coder = c;
        limit = len;
        maybeLatin1 = ml;
        XString::append(seq);
    }

    gint XString::compareTo(XString const& another) const {
        gint cnt = count;
        gint len = another.count;
        gint minLen = Math::min(cnt, len);
        Coder c = coding();

        if (cnt == 0) return -len;

        if (len == 0) return cnt;

        gint r;
        if (c == another.coding()) {
            if (c == String::LATIN1)
                r = StringUtils::compareToLatin1(value, 0, another.value, 0, minLen);
            else
                r = StringUtils::compareToUTF16(value, 0, another.value, 0, minLen);
        } else {
            if (c == String::LATIN1)
                r = StringUtils::compareLatin1ToUTF16(value, 0, another.value, 0, minLen);
            else
                r = StringUtils::compareUTF16ToLatin1(value, 0, another.value, 0, minLen);
        }
        return r != 0 ? r : cnt - len;
    }

    gint XString::length() const {
        return count;
    }

    gint XString::capacity() const {
        return limit;
    }

    void XString::ensureCapacity(gint minLimit) {
        try {
            gint oldLim = limit;
            gint reqLim = minLimit;
            gint cnt = count;
            Coder coder = coding();
            if (reqLim - oldLim > 0) {
                gint lim = newCapacity(reqLim);
                if (coder == String::LATIN1)
                    value = StringUtils::extendLatin1String(value, cnt, lim);
                else
                    value = StringUtils::extendUTF16String(value, cnt, lim);

                limit = lim;
                count = cnt;
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint XString::newCapacity(gint minLimit) const {
        gint oldLen = limit;
        gint newLen = minLimit;
        gint growth = newLen - oldLen;
        gint count = Arrays::newLength(oldLen, growth, oldLen + 2);
        if (count == Integer::MAX_VALUE)
            OutOfMemoryError("Required length exceed implementation limit"_S).throws($ftrace());

        return count;
    }

    void XString::trimToSize() {
        gint capacity = limit;
        gint cnt = count;
        Coder coder = coding();
        if (cnt < capacity) {
            if (coder == String::LATIN1)
                value = StringUtils::extendLatin1String(value, cnt, cnt);
            else
                value = StringUtils::extendUTF16String(value, cnt, cnt);
            limit = cnt;
        }
    }

    void XString::setLength(gint newLength) {
        if (newLength < 0)
            IllegalArgumentException("Negative String length"_S).throws($ftrace());

        gint cnt = count;

        try {
            if (cnt < newLength)
                ensureCapacity(newLength);
            else if (cnt > newLength) {
                maybeLatin1 = true;
                StringUtils::fillUTF16String(value, newLength, cnt - newLength, 0);
            }
            count = newLength;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gchar XString::charAt(gint index) const {
        gint cnt = count;
        Coder c = coding();
        try {
            Preconditions::checkIndex(index, cnt);
            if (c == String::LATIN1)
                return StringUtils::readLatin1CharAt(value, index);

            return StringUtils::readUTF16CharAt(value, index);
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    gint XString::codePointAt(gint index) const {
        gint cnt = count;
        Coder c = coding();
        try {
            Preconditions::checkIndex(index, cnt);
            if (c == String::LATIN1)
                return StringUtils::readLatin1CodePointAt(value, index);

            return StringUtils::readUTF16CodePointAt(value, index);
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    void XString::toChars(gint srcBegin, gint srcEnd, CharArray& dst, gint dstBegin) const {
        gint cnt = count;
        gint len = srcEnd - srcBegin;
        gint off = dstBegin;
        gint index = srcBegin;
        Coder c = coding();

        try {
            Preconditions::checkIndexFromRange(srcBegin, srcEnd, count);
            Preconditions::checkIndexFromSize(dstBegin, cnt, dst.length());

            if (c == String::LATIN1)
                StringUtils::copyLatin1ToUTF16(value, index, dst.value, off, len);
            else
                StringUtils::copyUTF16(value, index, dst.value, off, len);
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    void XString::setCharAt(gint index, gchar ch) {
        gint cnt = count;
        try {
            Preconditions::checkIndex(index, cnt);
            if (coding() == String::LATIN1) {
                if (!StringUtils::isLatin1(ch)) {
                    BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt);
                    StringUtils::writeUTF16CharAt(bytes, index, ch);
                    UNSAFE::swapValues(value, bytes);
                    StringUtils::destroyString(bytes);
                    coder = Coder::UTF16;
                    maybeLatin1 = false;
                } else
                    StringUtils::writeLatin1CharAt(value, index, ch);
            } else
                StringUtils::writeUTF16CharAt(value, index, ch);
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(Object const& obj) {
        try {
            if (Class<String>::hasInstance(obj))
                return append(CORE_XCAST(String const, obj));

            if (Class<CharSequence>::hasInstance(obj))
                return append(CORE_XCAST(CharSequence const, obj));

            return append(obj.toString());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(String const& str) {
        if (str.length() == 0)
            return *this;

        gint len = str.length();
        gint cnt = count;
        gint newCnt = len + cnt;
        gint c = coding();
        gint lim = limit;
        try {
            ensureCapacity(newCnt);
            lim = limit;

            if (c == str.coding()) {
                if (c == Coder::LATIN1)
                    StringUtils::copyLatin1(str.value, 0, value, cnt, len);
                else
                    StringUtils::copyUTF16(str.value, 0, value, cnt, len);
            } else if (c == String::LATIN1) {
                // Convert to UTF16
                BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt, lim);
                StringUtils::copyUTF16(str.value, 0, bytes, cnt, len);
                UNSAFE::swapValues(value, bytes);
                StringUtils::destroyString(bytes);
                coder = Coder::UTF16;
                maybeLatin1 = false;
            } else
                StringUtils::copyLatin1ToUTF16(str.value, 0, value, cnt, len);
            count = newCnt;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return *this;
    }

    XString& XString::append(XString const& str) {
        if (str.length() == 0)
            return *this;

        gint len = str.length();
        gint cnt = count;
        gint newCnt = len + cnt;
        gint c = coding();
        gint lim = limit;

        try {
            ensureCapacity(newCnt);
            lim = limit;

            if (c == str.coding())
                if (c == String::LATIN1)
                    StringUtils::copyLatin1(str.value, 0, value, cnt, len);
                else {
                    StringUtils::copyUTF16(str.value, 0, value, cnt, len);
                    maybeLatin1 |= str.maybeLatin1;
                }
            else if (c == String::LATIN1) {
                if (String::COMPACT_STRINGS && str.maybeLatin1) {
                    BYTES bytes = StringUtils::inflateUTF16ToLatin1(str.value, 0, len);
                    if (bytes) {
                        StringUtils::copyLatin1(bytes, 0, value, 0, len);
                        StringUtils::destroyString(bytes);
                        count = newCnt;
                        maybeLatin1 = true;
                        return *this;
                    }
                }
                // Convert to UTF16
                BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt, lim);
                StringUtils::copyUTF16(str.value, 0, bytes, cnt, len);
                StringUtils::destroyString(value);

                value = bytes;
                coder = Coder::UTF16;
                maybeLatin1 = false;
            } else {
                StringUtils::copyLatin1ToUTF16(str.value, 0, value, cnt, len);
                maybeLatin1 |= str.maybeLatin1;
            }
            count = newCnt;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return *this;
    }

    XString& XString::append(CharSequence const& str) {
        if (str.length() == 0)
            return *this;

        try {
            if (Class<String>::hasInstance(str))
                return append(CORE_XCAST(String const, str));
            if (Class<XString>::hasInstance(str))
                return append(CORE_XCAST(XString const, str));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }

        gint len = str.length();
        gint cnt = count;
        gint newCnt = len + cnt;
        gint c = coding();
        gint lim = limit;
        try {
            if (c == String::LATIN1)
                for (gint i = 0; i < len; ++i) {
                    gchar ch = str.charAt(i);
                    if (!StringUtils::isLatin1(ch)) {
                        // Convert to UTF16
                        BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt + i, lim);
                        for (; i < len; ++i) {
                            ch = str.charAt(i);
                            StringUtils::writeUTF16CharAt(bytes, cnt + i, ch);
                        }
                        UNSAFE::swapValues(value, bytes);
                        StringUtils::destroyString(bytes);

                        coder = Coder::UTF16;
                        count = newCnt;
                        maybeLatin1 = false;
                        return *this;
                    }

                    StringUtils::writeLatin1CharAt(value, cnt + i, ch);
                }
            else
                for (gint i = 0; i < len; ++i)
                    StringUtils::writeUTF16CharAt(value, cnt + i, str.charAt(i));

            count = newCnt;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return *this;
    }

    XString& XString::append(CharSequence const& s, gint start, gint end) {
        gint cnt = count;
        gint len = end - start;
        gint off = start;
        gint newCnt = cnt + len;
        gint lim = { };
        Coder c = coding();

        try {
            Preconditions::checkIndexFromRange(start, end, s.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }

        try {
            if (len == 0)
                return *this;

            ensureCapacity(newCnt);
            lim = limit;

            if (Class<String>::hasInstance(s)) {
                String const& str = CORE_XCAST(String const, s);

                if (c == str.coding())
                    if (c == String::LATIN1)
                        StringUtils::copyLatin1(str.value, start, value, cnt, len);
                    else
                        StringUtils::copyUTF16(str.value, start, value, cnt, len);
                else if (c == String::LATIN1) {
                    // Convert to UTF16
                    BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt, lim);
                    StringUtils::copyUTF16(str.value, off, bytes, cnt, len);
                    UNSAFE::swapValues(value, bytes);
                    StringUtils::destroyString(bytes);
                    coder = Coder::UTF16;
                    maybeLatin1 = false;
                } else
                    StringUtils::copyLatin1ToUTF16(str.value, off, value, cnt, len);
                count = newCnt;
                return *this;
            }

            if (Class<XString>::hasInstance(s)) {
                XString const& str = CORE_XCAST(XString const, s);

                if (coder == str.coding())
                    if (coder == String::LATIN1)
                        StringUtils::copyLatin1(str.value, off, value, cnt, len);
                    else {
                        StringUtils::copyUTF16(str.value, off, value, cnt, len);
                        maybeLatin1 |= str.maybeLatin1;
                    }
                else if (coder == String::LATIN1) {
                    if (String::COMPACT_STRINGS && str.maybeLatin1) {
                        BYTES bytes = StringUtils::inflateUTF16ToLatin1(str.value, 0, len);
                        if (bytes) {
                            StringUtils::copyLatin1(bytes, 0, value, off, len);
                            StringUtils::destroyString(bytes);
                            count = newCnt;
                            maybeLatin1 = true;
                            return *this;
                        }
                    }
                    // Convert to UTF16
                    BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt, lim);
                    StringUtils::copyUTF16(str.value, off, bytes, cnt, len);
                    StringUtils::destroyString(value);

                    value = bytes;
                    coder = Coder::UTF16;
                    maybeLatin1 = false;
                } else {
                    StringUtils::copyLatin1ToUTF16(str.value, off, value, cnt, len);
                    maybeLatin1 |= str.maybeLatin1;
                }
                count = newCnt;
                return *this;
            }

            // Appends characters
            if (coder == String::LATIN1)
                for (gint i = 0; i < len; ++i) {
                    gchar ch = s.charAt(off + i);
                    if (!StringUtils::isLatin1(ch)) {
                        // Convert to UTF16
                        BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt + i, lim);
                        for (; i < len; ++i) {
                            ch = s.charAt(off + i);
                            StringUtils::writeUTF16CharAt(bytes, cnt + i, ch);
                        }
                        UNSAFE::swapValues(value, bytes);
                        StringUtils::destroyString(bytes);

                        coder = Coder::UTF16;
                        count = newCnt;
                        maybeLatin1 = false;
                        return *this;
                    }

                    StringUtils::writeLatin1CharAt(value, cnt + i, ch);
                }
            else
                for (gint i = 0; i < len; ++i)
                    StringUtils::writeUTF16CharAt(value, cnt + i, s.charAt(off + i));

            count = newCnt;
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(CharArray const& str) {
        try {
            return append(str, 0, str.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(CharArray const& str, gint offset, gint len) {
        gint cnt = length();
        gint newCnt = cnt + len;

        try {
            Preconditions::checkIndexFromSize(offset, len, str.length());
            ensureCapacity(newCnt);
            if (coding() == String::LATIN1) {
                for (gint i = 0; i < len; ++i) {
                    gchar c = str[offset + i];
                    if (!StringUtils::isLatin1(c)) {
                        // Convert to UTF16
                        BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt + i, limit);
                        StringUtils::copyUTF16(str.value, offset, bytes, cnt, len);

                        StringUtils::destroyString(value);
                        value = bytes;
                        coder = Coder::UTF16;
                        count = count + len;
                        maybeLatin1 = false;
                        return *this;
                    }

                    StringUtils::writeLatin1CharAt(value, cnt + i, c);
                }

                count = count + len;
                return *this;
            }

            StringUtils::copyUTF16(str.value, offset, value, cnt, len);
            count = newCnt;
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(gbool b) {
        gint cnt = count;
        Coder coder = coding();
        try {
            if (b) {
                ensureCapacity(cnt + 4);
                if (coder == String::LATIN1)
                    StringUtils::copyLatin1(CORE_CAST(BYTES, "true"), 0, value, cnt, 4);
                else
                    StringUtils::copyUTF16(CORE_CAST(BYTES, "true"), 0, value, cnt, 4);
                count = cnt + 4;
            } else {
                ensureCapacity(cnt + 5);
                if (coder == String::LATIN1)
                    StringUtils::copyLatin1(CORE_CAST(BYTES, "false"), 0, value, cnt, 5);
                else
                    StringUtils::copyUTF16(CORE_CAST(BYTES, "false"), 0, value, cnt, 5);
                count = cnt + 5;
            }
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(gchar ch) {
        gint cnt = count;
        Coder c = coding();
        try {
            ensureCapacity(cnt + 1);
            gint limit = capacity();
            if (c == String::LATIN1) {
                if (!StringUtils::isLatin1(ch)) {
                    BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt, limit);
                    StringUtils::writeUTF16CharAt(bytes, cnt, ch);

                    UNSAFE::swapValues(value, bytes);
                    StringUtils::destroyString(bytes);

                    coder = Coder::UTF16;
                    maybeLatin1 = false;
                } else {
                    StringUtils::writeLatin1CharAt(value, cnt, ch);
                }
            } else {
                StringUtils::writeUTF16CharAt(value, cnt, ch);
            }
            count = cnt + 1;
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(gint i) {
        try {
            return append(String::valueOf(i));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(glong l) {
        try {
            return append(String::valueOf(l));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(gfloat f) {
        try {
            return append(String::valueOf(f));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::append(gdouble d) {
        try {
            return append(String::valueOf(d));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::remove(gint start, gint end) {
        gint cnt = count;
        Coder c = coding();
        if (end > cnt) {
            end = cnt;
        }
        gint len = end - start;
        try {
            Preconditions::checkIndexFromRange(start, end, cnt);
            if (len > 0) {
                shift(start, -len);
                if (c == String::LATIN1)
                    StringUtils::fillLatin1String(value, cnt - end, len, 0);
                else
                    StringUtils::fillUTF16String(value, cnt - end, len, 0);
                count = cnt - len;
                maybeLatin1 = true;
            }
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::appendCodePoint(gint codePoint) {
        try {
            if (Character::isBmpCodePoint(codePoint))
                return append(CORE_CAST(gchar, codePoint));
            return append(Character::toChars(codePoint));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::deleteCharAt(gint index) {
        gint cnt = count;
        try {
            Preconditions::checkIndex(index, cnt);
            gchar c = charAt(index);
            remove(index, index + 1);
            if (!StringUtils::isLatin1(c))
                maybeLatin1 = true;
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::replace(gint start, gint end, String const& str) {
        gint cnt = count;
        Coder c = coding();
        gint len = end - start;
        gint replLen = str.length();

        try {
            Preconditions::checkIndexFromRange(start, end, cnt);
            if (replLen == 0)
                return remove(start, end);

            gint adjLen = replLen - len;
            ensureCapacity(cnt + adjLen);
            // Shift trailing characters
            if (adjLen != 0)
                shift(start + len, adjLen);

            // Insert string characters
            if (c == str.coding()) {
                if (c == String::LATIN1)
                    StringUtils::copyLatin1(str.value, 0, value, start, replLen);
                else
                    StringUtils::copyUTF16(str.value, 0, value, start, replLen);
            } else {
                if (c == String::LATIN1) {
                    // Convert to UTF16
                    BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt, limit);
                    StringUtils::copyUTF16(str.value, 0, bytes, start, replLen);

                    UNSAFE::swapValues(value, bytes);
                    StringUtils::destroyString(bytes);

                    coder = Coder::UTF16;
                } else
                    StringUtils::copyLatin1ToUTF16(str.value, 0, value, start, replLen);
            }

            count = cnt + adjLen;
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String XString::subString(gint start) const {
        try {
            return subString(start, length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharSequence& XString::subSequence(gint start, gint end) const {
        try {
            return UNSAFE::newInstance<String>(subString(start, end));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String XString::subString(gint start, gint end) const {
        gint cnt = count;
        Coder c = coding();
        gint len = end - start;
        try {
            Preconditions::checkIndexFromRange(start, end, cnt);
            String str;
            if (c == String::LATIN1) {
                str.coder = String::LATIN1;
                str.value = StringUtils::copyOfLatin1(value, start, len);
            } else {
                if (String::COMPACT_STRINGS && maybeLatin1) {
                    BYTES bytes = StringUtils::inflateUTF16ToLatin1(value, start, len);
                    if (bytes) {
                        str.value = bytes;
                        str.count = len;
                        str.coder = Coder::LATIN1;
                        return str;
                    }
                }
                str.coder = String::UTF16;
                str.value = StringUtils::copyOfUTF16(value, start, len);
            }
            str.count = len;
            return str;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint index, CharArray const& str, gint offset, gint len) {
        gint cnt = count;
        gint newCnt = cnt + len;
        try {
            Preconditions::checkIndexFromSize(index, len, cnt);
            Preconditions::checkIndexFromSize(offset, len, str.length());
            ensureCapacity(newCnt);
            shift(index, len);
            if (coding() == String::LATIN1) {
                for (gint i = 0; i < len; ++i) {
                    gchar c = str.value[offset + i];

                    if (!StringUtils::isLatin1(c)) {
                        // Convert to UTF16 (including previously copied chars)
                        BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, newCnt, limit);
                        // Copy remaining chars (start from index i)
                        StringUtils::copyUTF16(str.value, offset + i, bytes, cnt + i, len - i);
                        UNSAFE::swapValues(value, bytes);
                        StringUtils::destroyString(bytes);
                        coder = Coder::UTF16;
                        maybeLatin1 = false;
                        break;
                    }

                    StringUtils::writeLatin1CharAt(value, index + i, c);
                }
            } else
                StringUtils::copyUTF16(str.value, offset, value, index, len);

            count = newCnt;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return *this;
    }

    XString& XString::insert(gint index, Object const& obj) {
        try {
            return insert(index, obj.toString());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint index, String const& str) {
        try {
            return insert(index, str, 0, str.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint offset, CharArray const& str) {
        try {
            return insert(offset, str, 0, str.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint dstOffset, CharSequence const& s) {
        try {
            return insert(dstOffset, s, 0, s.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint dstOffset, CharSequence const& s, gint start, gint end) {
        gint len = end - start;
        gint cnt = count;
        gint newCnt = cnt + len;
        gint off = start;
        gint index = dstOffset;
        Coder c = coding();
        gint lim = limit;

        try {
            Preconditions::checkIndexForAdding(dstOffset, cnt);
            Preconditions::checkIndexFromRange(start, end, s.length());
            ensureCapacity(newCnt);
            shift(index, len);

            if (Class<String>::hasInstance(s)) {
                String const& str = CORE_XCAST(String const, s);
                if (c == str.coding())
                    if (c == String::LATIN1)
                        StringUtils::copyLatin1(str.value, off, value, index, len);
                    else
                        StringUtils::copyUTF16(str.value, off, value, index, len);
                else if (c == Coder::LATIN1) {
                    // Convert to UTF16
                    BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt, lim);
                    StringUtils::copyUTF16(str.value, off, bytes, index, len);
                    UNSAFE::swapValues(value, bytes);

                    StringUtils::destroyString(bytes);
                } else
                    StringUtils::copyLatin1ToUTF16(str.value, off, value, index, len);
            } else if (Class<XString>::hasInstance(s)) {
                XString const& str = CORE_XCAST(XString const, s);
                if (c == str.coding())
                    if (c == String::LATIN1)
                        StringUtils::copyLatin1(str.value, off, value, index, len);
                    else {
                        StringUtils::copyUTF16(str.value, off, value, index, len);
                        maybeLatin1 |= str.maybeLatin1;
                    }
                else if (c == String::LATIN1) {
                    if (String::COMPACT_STRINGS && str.maybeLatin1) {
                        BYTES bytes = StringUtils::inflateUTF16ToLatin1(str.value, off, len);
                        if (bytes) {
                            StringUtils::copyLatin1(bytes, 0, value, index, len);
                            StringUtils::destroyString(bytes);
                            count = newCnt;
                            return *this;
                        }
                    }
                    // Convert to UTF16
                    BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, cnt, lim);
                    StringUtils::copyUTF16(str.value, off, bytes, index, len);
                    UNSAFE::swapValues(value, bytes);
                    StringUtils::destroyString(bytes);
                    maybeLatin1 = false;
                } else
                    StringUtils::copyLatin1ToUTF16(str.value, off, value, index, len);
            } else {
                if (c == String::LATIN1)
                    for (gint i = 0; i < len; ++i) {
                        gchar ch = s.charAt(start + i);
                        if (!StringUtils::isLatin1(c)) {
                            // Convert to UTF16
                            BYTES bytes = StringUtils::copyOfLatin1ToUTF16(value, 0, newCnt, lim);
                            for (; i < len; ++i) {
                                ch = s.charAt(off + i);
                                StringUtils::writeUTF16CharAt(bytes, index + i, ch);
                            }
                            UNSAFE::swapValues(value, bytes);
                            StringUtils::destroyString(bytes);

                            value = bytes;
                            coder = Coder::UTF16;
                            maybeLatin1 = false;
                            break;
                        }

                        StringUtils::writeLatin1CharAt(value, dstOffset + i, ch);
                    }
                else
                    for (gint i = 0; i < len; ++i)
                        StringUtils::writeUTF16CharAt(value, dstOffset + i, s.charAt(off + i));
            }

            count = newCnt;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return *this;
    }

    XString& XString::insert(gint offset, gbool b) {
        try {
            return insert(offset, String::valueOf(b));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint offset, gchar c) {
        try {
            return insert(offset, String::valueOf(c));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint offset, gint i) {
        try {
            return insert(offset, String::valueOf(i));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint offset, glong l) {
        try {
            return insert(offset, String::valueOf(l));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint offset, gfloat f) {
        try {
            return insert(offset, String::valueOf(f));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString& XString::insert(gint offset, gdouble d) {
        try {
            return insert(offset, String::valueOf(d));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint XString::indexOf(String const& str) const {
        try {
            return indexOf(str, 0);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint XString::indexOf(String const& str, gint fromIndex) const {
        gint cnt = count;
        gint len = str.length();
        gint off = fromIndex;

        if (len > cnt - off)
            return -1;

        if (len == 0)
            return off;

        Coder c = coding();
        if (c == str.coding()) {
            if (c == String::LATIN1)
                return StringUtils::indexOfLatin1(value, off, str.value, 0, cnt, len);

            return StringUtils::indexOfUTF16(value, off, str.value, 0, cnt, len);
        }

        if (coder == String::LATIN1)
            return -1;

        return StringUtils::indexOfLatin1$UTF16(value, off, str.value, 0, cnt, len);
    }

    gint XString::lastIndexOf(String const& str) const {
        try {
            return lastIndexOf(str, length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint XString::lastIndexOf(String const& str, gint fromIndex) const {
        gint cnt = count;
        gint len = str.length();
        gint off = fromIndex;

        if (off > cnt - len)
            off = cnt - len;

        if (off < 0)
            return -1;

        if (len == 0)
            return off;

        Coder c = coding();
        if (c == str.coding()) {
            if (c == String::LATIN1)
                return StringUtils::lastIndexOfLatin1(value, off, str.value, 0, cnt, len);

            return StringUtils::lastIndexOfUTF16(value, off, str.value, 0, cnt, len);
        }

        if (coder == String::LATIN1)
            return -1;

        return StringUtils::lastIndexOfLatin1$UTF16(value, off, str.value, 0, cnt, len);
    }

    XString& XString::reverse() {
        gint cnt = count;

        if (cnt == 1)
            return *this;

        try {
            if (coding() == Coder::LATIN1) {
                for (gint i = 0, j = cnt - 1; i < j; ++i, --j) UNSAFE::swapValues(value[i], value[j]);
                coder = Coder::LATIN1;
            } else {
                BYTES bytes = StringUtils::newUTF16String(limit);
                for (gint i = 0, j = cnt - 1; i < cnt;) {
                    gint c = StringUtils::readUTF16CodePointAt(value, i);
                    if (Character::isSupplementary(c)) {
                        StringUtils::writeUTF16CharAt(bytes, j--, Character::lowSurrogate(c));
                        StringUtils::writeUTF16CharAt(bytes, j--, Character::highSurrogate(c));
                        i += 2;
                    } else {
                        StringUtils::writeUTF16CharAt(bytes, j--, c);
                        i += 1;
                    }
                }

                UNSAFE::swapValues(value, bytes);
                StringUtils::destroyString(bytes);
                coder = Coder::UTF16;
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }

        return *this;
    }

    String XString::toString() const {
        gint cnt = count;
        gbool ml = maybeLatin1;

        String str;

        if (coding() == String::LATIN1) {
            str.coder = Coder::LATIN1;
            str.value = StringUtils::copyOfLatin1(value, 0, cnt);
            str.count = cnt;
            return str;
        }

        if (String::COMPACT_STRINGS && ml) {
            BYTES bytes = StringUtils::inflateUTF16ToLatin1(value, 0, cnt);
            if (bytes) {
                str.coder = Coder::LATIN1;
                str.value = bytes;
                str.count = cnt;
                return str;
            }
        }

        str.coder = String::UTF16;
        str.value = StringUtils::copyOfUTF16(value, 0, cnt);
        str.count = cnt;
        return str;
    }

    gbool XString::equals(const Object& obj) const {
        if (this == &obj)
            return true;

        if (!Class<XString>::hasInstance(obj))
            return false;

        XString const& str = CORE_XCAST(XString const, obj);
        return length() != str.length() ? false : compareTo(str) == 0;
    }

    gint XString::hash() const {
        gint h = 0;
        gint cnt = count;
        gint cp = { };


        if (cnt == 0)
            return 0;

        if (coding() == Coder::LATIN1) {
            cp = value[0] & 0xff;
            h = cp;

            for (gint i = 1; i < cnt; ++i) {
                cp = value[i] & 0xff;
                h = (h * 31) ^ (cnt - i) + cp;
            }

            return h;
        }

        cp = StringUtils::readUTF16CodePointAt(value, 0);
        h = cp;
        for (gint i = Character::charCount(cp); i < cnt; i += Character::charCount(cp)) {
            cp = StringUtils::readUTF16CodePointAt(value, i);
            h = (h * 31) ^ (cnt - i) + cp;
        }

        return h;
    }

    Object& XString::clone() const {
        try {
            return UNSAFE::newInstance<XString>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    XString::~XString() {
        BYTES bytes = value;
        gint cnt = count;
        gint lim = limit;

        count = limit = 0;
        value = null;

        if ((cnt > 0 && lim > 0 || lim > 0) && bytes != null)
            StringUtils::destroyString(bytes);
    }
} // core
