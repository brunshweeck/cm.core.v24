//
// Created by bruns on 10/05/2024.
//

#include "String.h"

#include <core/ArithmeticException.h>
#include <core/Complex.h>
#include <core/IllegalArgumentException.h>
#include <core/StringArray.h>
#include <core/XString.h>
#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CharacterCodingException.h>
#include <core/charset/IllegalCharsetNameException.h>
#include <core/charset/UnsupportedCharsetException.h>
#include <core/charset/UnmappableCharacterException.h>
#include <core/io/ByteBuffer.h>
#include <core/regex/Matcher.h>
#include <core/regex/Pattern.h>
#include <core/util/ArrayList.h>

// --------- Meta --------------
#include <meta/CharacterDataLatin1.h>
#include <meta/StringUtils.h>
#include <meta/charset/ArrayDecoder.h>
#include <meta/charset/ArrayEncoder.h>
#include <meta/ext/Formatter.h>

namespace core {
    using namespace util;
    using namespace misc;
    using namespace charset;

    String::Coder String::coding() const {
        return COMPACT_STRINGS && coder == LATIN1 ? LATIN1 : UTF16;
    }

    StringArray String::split(String const& regex, gint limit, gbool withDelimiters) const {
        /* fast-path if the regex is a
         * (1) one-char String and this character is not one of the
         *     RegEx's meta characters ".$|()[{^?*+\\", or
         * (2) two-char String and the first char is the backslash and
         *     the second is not the ascii digit or ascii letter.
         */
        gchar ch = 0;
        if ((regex.length() == 1 && R"(.$|()[{^?*+\)"_Sl.indexOf(ch = regex.charAt(0)) == -1 || regex.length() == 2 &&
            regex.charAt(0) == '\\' && ((ch = regex.charAt(1)) - '0' | '9' - ch) < 0 && (ch - 'a' | 'z' - ch) < 0 && (ch
                - 'A' | 'Z' - ch) < 0) && (ch < Character::MIN_HIGH_SURROGATE || ch > Character::MAX_LOW_SURROGATE)) {
            // All the checks above can potentially be constantly folded by
            // a JIT/AOT compiler when the regex is a constant string.
            // That requires method inlining of the checks, which is only
            // possible when the actual split logic is in a separate method
            // because the large split loop can usually not be inlined.
            try {
                return split(ch, limit, withDelimiters);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
        try {
            regex::Pattern pattern = regex::Pattern::compile(regex);
            return withDelimiters
                       ? pattern.splitWithDelimiters(*this, limit)
                       : pattern.split(*this, limit);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    StringArray String::split(gchar ch, gint limit, gbool withDelimiters) const {
        // try {  } catch (Throwable const& ex) { ex.throws($ftrace()); }
        int matchCount = 0;
        int off = 0;
        int next;
        gbool limited = limit > 0;
        ArrayList<String> list = ArrayList<String>();
        String del = valueOf(ch);
        while ((next = indexOf(ch, off)) != -1) {
            if (!limited || matchCount < limit - 1) {
                list.add(subString(off, next));
                if (withDelimiters) {
                    list.add(del);
                }
                off = next + 1;
                ++matchCount;
            } else { // last one
                int last = length();
                list.add(subString(off, last));
                off = last;
                ++matchCount;
                break;
            }
        }
        // If no match was found, return this
        if (off == 0)
            return StringArray(1, *this);

        // Add remaining segment
        if (!limited || matchCount < limit)
            list.add(subString(off, length()));

        // Construct result
        int resultSize = list.size();
        if (limit == 0) {
            while (resultSize > 0 && list.get(resultSize - 1).isEmpty()) {
                resultSize--;
            }
        }
        StringArray result = StringArray(resultSize);
        for (int i = 0; i < resultSize; i++) {
            result[i] = list.get(i);
            UNSAFE::deleteInstance(list.get(i));
        }
        return result;
    }

    String::String() {
        value = COMPACT_STRINGS ? StringUtils::newLatin1String(0) : StringUtils::newUTF16String(0);
        coder = COMPACT_STRINGS ? LATIN1 : UTF16;
        count = 0;
        hashValue = 0;
        hashIsZero = false;
    }

    String::String(String const& original) : String() {
        gint len = original.length();
        if (original.coding() == LATIN1) {
            value = StringUtils::copyOfLatin1(original.value, 0, len);
            coder = LATIN1;
        } else {
            value = StringUtils::copyOfUTF16(original.value, 0, len);
            coder = UTF16;
        }
        count = len;
        hashValue = original.hashValue;
        hashIsZero = original.hashIsZero;
    }

    String::String(String&& original) CORE_NOTHROW: String() {
        UNSAFE::swapValues(value, original.value);
        UNSAFE::swapValues(count, original.count);
        UNSAFE::swapValues(coder, original.coder);
        UNSAFE::swapValues(hashValue, original.hashValue);
        UNSAFE::swapValues(hashIsZero, original.hashIsZero);
    }

    String::String(CharArray const& value) : String(value, 0, value.length()) {
        //
    }

    String::String(CharArray const& value, gint offset, gint count) : String() {
        Preconditions::checkIndexFromSize(offset, count, value.length());

        if (count > 0) {
            if (COMPACT_STRINGS) {
                BYTES bytes = StringUtils::inflateUTF16ToLatin1(value.value, offset, count);
                if (bytes != null) {
                    String::value = bytes;
                    String::count = count;
                    coder = LATIN1;
                } else
                    goto UTF16_STRING;
            } else {
            UTF16_STRING:
                String::value = StringUtils::copyOfUTF16(value.value, offset, count);
                String::count = count;
                coder = UTF16;
            }
        }
    }

    String::String(IntArray const& codePoints, gint offset, gint count) : String() {
        Preconditions::checkIndexFromSize(offset, count, codePoints.length());

        if (count > 0) {
            if (COMPACT_STRINGS) {
                BYTES bytes = StringUtils::inflateUTF32ToLatin1(codePoints.value, offset, count);
                if (bytes != null) {
                    value = bytes;
                    String::count = count;
                    coder = LATIN1;
                } else
                    goto UTF16_STRING;
            } else {
            UTF16_STRING:
                value = StringUtils::copyOfUTF32ToUTF16(codePoints.value, offset, count, String::count);
                coder = UTF16;
            }
        }
    }

    String::String(ByteArray const& ascii, gint hibyte, gint offset, gint count) : String() {
        Preconditions::checkIndexFromSize(offset, count, ascii.length());

        if (count > 0) {
            if (COMPACT_STRINGS && hibyte == 0) {
                value = StringUtils::copyOfLatin1(ascii.value, offset, count);
                coder = LATIN1;
            } else {
                value = StringUtils::newUTF16String(count);
                coder = UTF16;
                for (int i = 0; i < count; ++i) {
                    gchar c = ((hibyte & 0xff) << 8) | (ascii.value[i] & 0xff);
                    StringUtils::writeUTF16CharAt(value, i, c);
                }
            }
            String::count = count;
        }
    }

    String::String(ByteArray const& ascii, gint hibyte) : String(ascii, hibyte, 0, ascii.length()) {}

    String::String(ByteArray const& bytes, String const& charsetName)
        : String(bytes, charset::Charset::forName(charsetName)) {}

    String::String(ByteArray const& bytes, charset::Charset const& charset): String() {
        gint length = bytes.length();
        if (length == 0)
            return;

        if (&charset == &charset::Charset::UTF_8) {
            CharArray ca = decodeUTF8Literal((glong) bytes.value, length);
            if (COMPACT_STRINGS) {
                BYTES latin1 = StringUtils::inflateUTF16ToLatin1(ca.value, 0, ca.length());
                if (!latin1)
                    goto TRY_UTF16_;
                value = latin1;
                count = ca.length();
                coder = LATIN1;
            } else {
            TRY_UTF16_:
                value = StringUtils::copyOfUTF16(ca.value, 0, ca.length());
                count = ca.length();
                coder = UTF16;
            }
            return;
        }

        if (&charset == &charset::Charset::ISO_8859_1) {
            if (COMPACT_STRINGS) {
                value = StringUtils::copyOfLatin1(bytes.value, 0, length);
                count = length;
                coder = LATIN1;
            } else {
                value = StringUtils::copyOfLatin1ToUTF16(bytes.value, 0, length);
                count = length;
                coder = UTF16;
            }
            return;
        }

        if (&charset == &charset::Charset::US_ASCII) {
            if (COMPACT_STRINGS) {
                value = StringUtils::newLatin1String(length);
                for (gint i = 0; i < length; ++i) {
                    if (bytes.value[i] < 0)
                        value[i] = '?';
                    else
                        value[i] = bytes.value[i];
                }
                count = length;
                coder = LATIN1;
            } else {
                value = StringUtils::newUTF16String(length);

                for (gint i = 0; i < length; ++i) {
                    if (bytes.value[i] < 0)
                        StringUtils::writeUTF16CharAt(value, i, '?');
                    else
                        StringUtils::writeUTF16CharAt(value, i, bytes.value[i]);
                }
                count = length;
                coder = UTF16;
            }
            return;
        }

        charset::CharsetDecoder& dec = charset.newDecoder();
        if (Class<charset::ArrayDecoder>::hasInstance(dec)) {
            charset::ArrayDecoder& ad = CORE_XCAST(charset::ArrayDecoder, dec);
            // ascii
            if (ad.isASCIICompatible()) {
                gbool hasNegative = false;
                for (gint i = 0; i < length; ++i) {
                    if (bytes.value[i] < 0) {
                        hasNegative = true;
                        break;
                    }
                }

                if (!hasNegative) {
                    if (COMPACT_STRINGS) {
                        value = StringUtils::copyOfLatin1(bytes.value, 0, length);
                        count = length;
                        coder = LATIN1;
                    } else {
                        value = StringUtils::copyOfLatin1ToUTF16(bytes.value, 0, length);
                        count = length;
                        coder = UTF16;
                    }
                    return;
                }
            }

            // fast-path for always Latin1 decodable single byte
            if (ad.isLatin1Decodable()) {
                ByteArray dst = ByteArray(length);
                gint cnt = ad.decodeToLatin1(bytes, 0, length, dst);
                if (COMPACT_STRINGS) {
                    value = StringUtils::copyOfLatin1(dst.value, 0, cnt);
                    count = cnt;
                    coder = LATIN1;
                } else {
                    value = StringUtils::copyOfLatin1ToUTF16(dst.value, 0, cnt);
                    count = cnt;
                    coder = UTF16;
                }
                return;
            }

            gint en = (gint) (length * dec.maxCharsPerByte());
            dec.onMalformedInput(charset::Charset::REPLACE).onMalformedInput(charset::Charset::REPLACE);
            CharArray dst = CharArray(en);
            gint cnt = ad.decode(bytes, 0, length, dst);
            if (COMPACT_STRINGS) {
                BYTES latin1 = StringUtils::inflateUTF16ToLatin1(dst.value, 0, cnt);
                if (!latin1)
                    goto TRY_UTF16__;
                count = cnt;
                coder = LATIN1;
            } else {
            TRY_UTF16__:
                value = StringUtils::copyOfUTF16(dst.value, 0, cnt);
                count = cnt;
                coder = UTF16;
            }
            return;
        }

        // decode using CharsetDecoder
        gint en = (gint) (length * dec.maxCharsPerByte());
        dec.onMalformedInput(charset::Charset::REPLACE).onUnmappableCharacter(charset::Charset::REPLACE);
        CharArray dst = CharArray(en);
        gint cnt = 0;
        try {
            io::ByteBuffer& bb = io::ByteBuffer::wrap((ByteArray&) bytes, 0, length);
            io::CharBuffer& cb = io::CharBuffer::wrap(dst, 0, length);
            charset::CoderResult cr = dec.decode(bb, cb, true);
            if (!cr.isUnderflow())
                cr.throwException();
            cr = dec.flush(cb);
            if (!cr.isUnderflow())
                cr.throwException();
            cnt = cb.position();
            UNSAFE::deleteInstance(bb);
            UNSAFE::deleteInstance(cb);
        } catch (charset::CharacterCodingException const& ex) {
            // Substitution is enabled, so this shouldn't happen
            Error(ex).throws($ftrace());
        }
        UNSAFE::deleteInstance(dec);
        if (COMPACT_STRINGS) {
            BYTES latin1 = StringUtils::inflateUTF16ToLatin1(dst.value, 0, cnt);
            if (!latin1)
                goto TRY_UTF16;
            value = latin1;
            count = cnt;
            coder = LATIN1;
        } else {
        TRY_UTF16:
            value = StringUtils::copyOfUTF16(dst.value, 0, cnt);
            count = cnt;
            coder = UTF16;
        }
    }

    String::String(ByteArray const& bytes, gint offset, gint length): String() {
        try {
            Preconditions::checkIndexFromSize(offset, length, bytes.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }

        if (length == 0)
            return;

        charset::Charset& charset = charset::Charset::defaultCharset();
        if (length == 0)
            return;

        if (&charset == &charset::Charset::UTF_8) {
            CharArray ca = decodeUTF8Literal((glong) bytes.value + offset, length);
            if (COMPACT_STRINGS) {
                BYTES latin1 = StringUtils::inflateUTF16ToLatin1(ca.value, 0, ca.length());
                if (!latin1)
                    goto TRY_UTF16;
                value = latin1;
                count = ca.length();
                coder = LATIN1;
            } else {
            TRY_UTF16:
                value = StringUtils::copyOfUTF16(ca.value, 0, ca.length());
                count = ca.length();
                coder = UTF16;
            }
            return;
        }

        if (&charset == &charset::Charset::ISO_8859_1) {
            if (COMPACT_STRINGS) {
                value = StringUtils::copyOfLatin1(bytes.value, offset, length);
                count = length;
                coder = LATIN1;
            } else {
                value = StringUtils::copyOfLatin1ToUTF16(bytes.value, offset, length);
                count = length;
                coder = UTF16;
            }
            return;
        }

        if (&charset == &charset::Charset::US_ASCII) {
            if (COMPACT_STRINGS) {
                value = StringUtils::newLatin1String(length);
                for (gint i = 0; i < length; ++i) {
                    if (bytes.value[i + offset] < 0)
                        value[i] = '?';
                    else
                        value[i] = bytes.value[i + offset];
                }
                count = length;
                coder = LATIN1;
            } else {
                value = StringUtils::newUTF16String(length);

                for (gint i = 0; i < length; ++i) {
                    if (bytes.value[i + offset] < 0)
                        StringUtils::writeUTF16CharAt(value, i, '?');
                    else
                        StringUtils::writeUTF16CharAt(value, i, bytes.value[i + offset]);
                }
                count = length;
                coder = UTF16;
            }
            return;
        }

        charset::CharsetDecoder& dec = charset.newDecoder();
        if (Class<charset::ArrayDecoder>::hasInstance(dec)) {
            charset::ArrayDecoder& ad = CORE_XCAST(charset::ArrayDecoder, dec);
            // ascii
            if (ad.isASCIICompatible()) {
                gbool hasNegative = false;
                for (gint i = 0; i < length; ++i) {
                    if (bytes.value[i + offset] < 0) {
                        hasNegative = true;
                        break;
                    }
                }

                if (!hasNegative) {
                    if (COMPACT_STRINGS) {
                        value = StringUtils::copyOfLatin1(bytes.value, offset, length);
                        count = length;
                        coder = LATIN1;
                    } else {
                        value = StringUtils::copyOfLatin1ToUTF16(bytes.value, offset, length);
                        count = length;
                        coder = UTF16;
                    }
                    return;
                }
            }

            // fast-path for always Latin1 decodable single byte
            if (ad.isLatin1Decodable()) {
                ByteArray dst = ByteArray(length);
                gint cnt = ad.decodeToLatin1(bytes, offset, length, dst);
                if (COMPACT_STRINGS) {
                    value = StringUtils::copyOfLatin1(dst.value, 0, cnt);
                    count = cnt;
                    coder = LATIN1;
                } else {
                    value = StringUtils::copyOfLatin1ToUTF16(dst.value, 0, cnt);
                    count = cnt;
                    coder = UTF16;
                }
                return;
            }

            gint en = (gint) (length * dec.maxCharsPerByte());
            dec.onMalformedInput(charset::Charset::REPLACE).onMalformedInput(charset::Charset::REPLACE);
            CharArray dst = CharArray(en);
            gint cnt = ad.decode(bytes, offset, length, dst);
            if (COMPACT_STRINGS) {
                BYTES latin1 = StringUtils::inflateUTF16ToLatin1(dst.value, 0, cnt);
                if (!latin1)
                    goto TRY_UTF16_;
                count = cnt;
                coder = LATIN1;
            } else {
            TRY_UTF16_:
                value = StringUtils::copyOfUTF16(dst.value, 0, cnt);
                count = cnt;
                coder = UTF16;
            }
            return;
        }

        // decode using CharsetDecoder
        gint en = (gint) (length * dec.maxCharsPerByte());
        dec.onMalformedInput(charset::Charset::REPLACE).onUnmappableCharacter(charset::Charset::REPLACE);
        CharArray dst = CharArray(en);
        gint cnt = 0;
        try {
            io::ByteBuffer& bb = io::ByteBuffer::wrap((ByteArray&) bytes, offset, length);
            io::CharBuffer& cb = io::CharBuffer::wrap(dst, 0, length);
            charset::CoderResult cr = dec.decode(bb, cb, true);
            if (!cr.isUnderflow())
                cr.throwException();
            cr = dec.flush(cb);
            if (!cr.isUnderflow())
                cr.throwException();
            cnt = cb.position();
            UNSAFE::deleteInstance(bb);
            UNSAFE::deleteInstance(cb);
        } catch (charset::CharacterCodingException const& ex) {
            // Substitution is enabled, so this shouldn't happen
            Error(ex).throws($ftrace());
        }
        UNSAFE::deleteInstance(dec);
        if (COMPACT_STRINGS) {
            BYTES latin1 = StringUtils::inflateUTF16ToLatin1(dst.value, 0, cnt);
            if (!latin1)
                goto TRY_UTF16__;
            value = latin1;
            count = cnt;
            coder = LATIN1;
        } else {
        TRY_UTF16__:
            value = StringUtils::copyOfUTF16(dst.value, 0, cnt);
            count = cnt;
            coder = UTF16;
        }
    }

    String::String(ByteArray const& bytes): String(bytes, 0, bytes.length()) {}

    String& String::operator=(String const& other) {
        try {
            if (this != &other) {
                if (coder == other.coder) {
                    if (count < other.count) {
                        value = (coder == LATIN1)
                                    ? StringUtils::extendLatin1String(value, count, other.count)
                                    : StringUtils::extendUTF16String(value, count, other.count);
                    }
                    (coder == LATIN1)
                        ? StringUtils::copyLatin1(other.value, 0, value, 0, count = other.count)
                        : StringUtils::copyUTF16(other.value, 0, value, 0, count = other.count);
                } else {
                    if (coder == LATIN1) {
                        // other.coder = UTF16
                        if ((count >> 1) < other.count) {
                            value = StringUtils::extendUTF16String(value, count, other.count);
                        }
                        StringUtils::copyUTF16(other.value, 0, value, 0, count = other.count);
                    } else {
                        // coder == UTF16 && other.coder == LATIN1
                        if (count < (other.count >> 1)) {
                            value = StringUtils::extendLatin1String(value, count, other.count);
                        }
                        StringUtils::copyLatin1(other.value, 0, value, 0, count = other.count);
                    }
                    coder = other.coder;
                }
                hashValue = other.hashValue;
                hashIsZero = other.hashIsZero;
            }
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String& String::operator=(String&& other) noexcept {
        if (this != &other) {
            UNSAFE::swapValues(value, other.value);
            UNSAFE::swapValues(count, other.count);
            UNSAFE::swapValues(coder, other.coder);
            UNSAFE::swapValues(hashValue, other.hashValue);
            UNSAFE::swapValues(hashIsZero, other.hashIsZero);
        }
        return *this;
    }

    gint String::length() const {
        return count;
    }

    gbool String::isEmpty() const {
        return count == 0;
    }

    gchar String::charAt(gint index) const {
        try {
            Preconditions::checkIndex(index, count);
            return coder == LATIN1
                       ? StringUtils::readLatin1CharAt(value, index)
                       : StringUtils::readUTF16CharAt(value, index);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint String::codePointAt(gint index) const {
        try {
            Preconditions::checkIndex(index, count);
            return coder == LATIN1
                       ? StringUtils::readLatin1CodePointAt(value, index)
                       : StringUtils::readUTF16CharAt(value, index);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint String::codePointCount(gint beginIndex, gint endIndex) const {
        try {
            Preconditions::checkIndexFromRange(beginIndex, endIndex, count);
            return coding() == LATIN1
                       ? StringUtils::numberOfLatin1CodePoints(value, beginIndex, endIndex - beginIndex)
                       : StringUtils::numberOfUTF16CodePoints(value, beginIndex, endIndex - beginIndex);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void String::toChars(gint beginIndex, gint endIndex, CharArray& dest, gint offset) const {
        try {
            Preconditions::checkIndexFromRange(beginIndex, endIndex, count);
            Preconditions::checkIndexFromSize(offset, endIndex - beginIndex, dest.length());

            coding() == LATIN1
                ? StringUtils::copyLatin1ToUTF16(value, beginIndex, dest.value, offset, endIndex - beginIndex)
                : StringUtils::copyUTF16(value, beginIndex, dest.value, offset, endIndex - beginIndex);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void String::toBytes(gint beginIndex, gint endIndex, ByteArray& dest, gint offset) const {
        try {
            Preconditions::checkIndexFromRange(beginIndex, endIndex, count);
            Preconditions::checkIndexFromSize(offset, endIndex - beginIndex, dest.length());

            coding() == LATIN1
                ? StringUtils::copyLatin1(value, beginIndex, dest.value, offset, endIndex - beginIndex)
                : StringUtils::copyUTF16ToLatin1(value, beginIndex, dest.value, offset, endIndex - beginIndex);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ByteArray String::toBytes(String const& charsetName) const {
        try {
            return toBytes(charset::Charset::forName(charsetName));
        } catch (charset::UnsupportedCharsetException
            const& _) {
            charset::UnsupportedCharsetException(charsetName).throws($ftrace());
        } catch (
            charset::IllegalCharsetNameException const& _) {
            charset::UnsupportedCharsetException(charsetName).throws($ftrace());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ByteArray String::toBytes(charset::Charset const& charset) const {
        if (&charset == &charset::Charset::UTF_8) {
            return coder == LATIN1
                       ? encodeLatin1ToUTF8(0, count)
                       : encodeUtf16ToUTF8(0, count);
        }
        if (&charset == &charset::Charset::ISO_8859_1) {
            return coder == LATIN1
                       ? encodeLatin1ToISO8853_1(0, count, true)
                       : encodeUTF16ToISO8853_1(0, count, true);
        }
        if (&charset == &charset::Charset::US_ASCII) {
            return coder == LATIN1
                       ? encodeLatin1ToASCII(0, count)
                       : encodeUTF16ToASCII(0, count);
        }
        try {
            charset::CharsetEncoder& enc = charset.newEncoder();
            gint len = count;
            gint en = (gint) (len * enc.maxBytesPerChar());
            // fast-path with ArrayEncoder
            if (Class<charset::ArrayEncoder>::hasInstance(enc)) {
                charset::ArrayEncoder& ae = CORE_XCAST(charset::ArrayEncoder, enc);
                // fast-path for ascii compatible
                if (coder == LATIN1 && ae.isASCIICompatible()) {
                    ByteArray dst = ByteArray(len);
                    gint dp = 0;
                    for (gint i = 0; i < len; i++) {
                        if (value[i] < 0)
                            break;
                        dst.value[dp++] = value[i];
                    }
                    if (dp == len)
                        return dst;
                }
                ByteArray dst = ByteArray(en);
                if (len == 0)
                    return dst;
                if (coder == LATIN1) {
                    ByteArray src = ByteArray(len);
                    StringUtils::copyLatin1(value, 0, src.value, 0, len);
                    gint cnt = ae.encodeFromLatin1(src, 0, len, dst);
                    if (cnt != -1) {
                        if (cnt < dst.length())
                            return util::Arrays::copyOf(dst, cnt);
                        return dst;
                    }
                } else {
                    ByteArray src = ByteArray(len * 2);
                    StringUtils::copyUTF16(value, 0, src.value, 0, len);
                    gint cnt = ae.encodeFromUTF16(src, 0, len, dst);
                    if (cnt != -1) {
                        if (cnt < dst.length())
                            return util::Arrays::copyOf(dst, cnt);
                        return dst;
                    }
                }
            }

            ByteArray dst = ByteArray(en);
            if (len == 0)
                return dst;
            enc.onMalformedInput(charset::Charset::REPLACE).onUnmappableCharacter(charset::Charset::REPLACE);
            CharArray src = toChars();

            io::ByteBuffer& bb = io::ByteBuffer::wrap(dst);
            io::CharBuffer& cb = io::CharBuffer::wrap(src, 0, len);
            try {
                charset::CoderResult cr = enc.encode(cb, bb, true);
                if (!cr.isUnderflow())
                    cr.throwException();
                cr = enc.flush(bb);
                if (!cr.isUnderflow())
                    cr.throwException();
            } catch (charset::CharacterCodingException const& ex) {
                throw IllegalArgumentException(ex);
            }

            if (bb.position() < dst.length())
                return util::Arrays::copyOf(dst, bb.position());

            return dst;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    using charset::Charset;
    using charset::CharsetEncoder;

    ByteArray String::toBytes() const {
        Charset& cs = Charset::defaultCharset();
        if (cs == Charset::UTF_8) {
            return coding() == LATIN1
                       ? encodeLatin1ToUTF8(0, count)
                       : encodeUtf16ToUTF8(0, count);
        }
        if (!cs.canEncode()) {
            Error("Default charset not support encoding."_Sl).throws($ftrace());
        }

        ByteArray result;
        try {
            io::ByteBuffer& bb = cs.encode(*this);
            Object& o = bb.array();
            result = CORE_XCAST(ByteArray, o);
            UNSAFE::deleteInstance(o);
            UNSAFE::deleteInstance(bb);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return result;
    }

    gbool String::equals(Object const& obj) const {
        if (this == &obj)
            return true;
        if (Class<String>::hasInstance(obj)) {
            String const& other = CORE_XCAST(String const, obj);
            Coder const coder = coding();
            gint const count = length();

            if (count != other.length() || coder != other.coding()) {
                return false;
            }

            if (coder == LATIN1)
                return StringUtils::compareToLatin1(value, 0, other.value, 0, count) == 0;
            return StringUtils::compareToUTF16(value, 0, other.value, 0, count) == 0;
        }
        return false;
    }

    gbool String::contentEquals(CharSequence const& cs) const {
        try {
            // Argument is a XString
            if (Class<XString>::hasInstance(cs)) {
                XString const& xs = CORE_XCAST(XString const, cs);
                const gint len = length();
                if (len != xs.length())
                    return false;

                Coder const coder = coding();
                if (coder != xs.coding()) {
                    if (coder == LATIN1)
                        return false;

                    return StringUtils::compareUTF16ToLatin1(value, 0, xs.value, 0, len) == 0;
                }

                return len <= xs.length() && StringUtils::compareToLatin1(value, 0, xs.value, 0, len) == 0;
            }
            // Argument is a String
            if (Class<String>::hasInstance(cs))
                return equals(CORE_XCAST(String const, cs));
            // Argument is a generic CharSequence
            gint n = cs.length();
            if (n != length()) {
                return false;
            }
            if (coding() == LATIN1) {
                for (int i = 0; i < n; i++) {
                    if ((value[i] & 0xff) != cs.charAt(i)) {
                        return false;
                    }
                }
            } else {
                for (int i = 0; i < n; i++) {
                    if (StringUtils::readUTF16CharAt(value, i) != cs.charAt(i)) {
                        return false;
                    }
                }
            }
            return true;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::equalsIgnoreCase(String const& anotherString) const {
        if (this == &anotherString)
            return true;

        const Coder coder = coding();
        const gint count = length();

        if (count != anotherString.length())
            return false;

        if (coder == anotherString.coding()) {
            if (coder == LATIN1)
                return StringUtils::compareToLatin1IC(value, 0, anotherString.value, 0, count) == 0;
            return StringUtils::compareToUTF16IC(value, 0, anotherString.value, 0, count) == 0;
        }

        if (coder == LATIN1)
            return StringUtils::compareLatin1ToUTF16IC(value, 0, anotherString.value, 0, count) == 0;
        return StringUtils::compareUTF16ToLatin1IC(value, 0, anotherString.value, 0, count) == 0;
    }

    gint String::compareTo(String const& other) const {
        if (this == &other)
            return 0;

        Coder const coder = coding();
        gint const len = Math::min(count, other.count);
        gint r = 0;

        if (coder == other.coding()) {
            if (coder == LATIN1)
                r = StringUtils::compareToLatin1(value, 0, other.value, 0, len);
            else
                r = StringUtils::compareToUTF16(value, 0, other.value, 0, len);
        } else {
            if (coder == LATIN1)
                r = StringUtils::compareLatin1ToUTF16(value, 0, other.value, 0, len);
            else
                r = StringUtils::compareUTF16ToLatin1(value, 0, other.value, 0, len);
        }

        if (r != 0)
            return r;

        if (count == other.count)
            return 0;

        if (len == count) {
            gchar c = other.charAt(len);
            return c == 0 ? -other.count : -c;
        }

        gchar c = charAt(len);
        return c == 0 ? count : c;
    }

    gint String::compareToIgnoreCase(String const& str) const {
        if (this == &str)
            return 0;

        const Coder coder = coding();
        gint const len = Math::min(count, str.count);
        gint r = 0;

        if (coder == str.coding()) {
            if (coder == LATIN1)
                r = StringUtils::compareToLatin1IC(value, 0, str.value, 0, len);
            else
                r = StringUtils::compareToUTF16IC(value, 0, str.value, 0, len);
        } else {
            if (coder == LATIN1)
                r = StringUtils::compareLatin1ToUTF16IC(value, 0, str.value, 0, len);
            else
                r = StringUtils::compareUTF16ToLatin1IC(value, 0, str.value, 0, len);
        }

        if (r != 0)
            return r;

        if (count == str.count)
            return 0;

        if (len == count) {
            gchar c = str.charAt(len);
            return c == 0 ? -str.count : -c;
        }

        gchar c = charAt(len);
        return c == 0 ? count : c;
    }

    String::~String() {
        ARRAY a = value;
        gint n = count;

        value = null;
        count = 0;
        coder = LATIN1;
        hashValue = 0;
        hashIsZero = false;

        if (n > 0)
            StringUtils::destroyString(a);
    }

    gbool String::startsWith(String const& prefix, gint offset) const {
        try {
            if (offset < 0 || offset > length() - prefix.length()) {
                return false;
            }

            Coder coder = coding();
            gint count = prefix.length();

            if (coder == prefix.coding()) {
                if (coder == LATIN1) {
                    for (int i = 0; i < count; ++i) {
                        gchar c1 = StringUtils::readLatin1CharAt(value, i + offset);
                        gchar c2 = StringUtils::readLatin1CharAt(prefix.value, i);
                        if (c1 != c2) {
                            return false;
                        }
                    }
                } else {
                    for (int i = 0; i < count; ++i) {
                        gchar c1 = StringUtils::readUTF16CharAt(value, i + offset);
                        gchar c2 = StringUtils::readUTF16CharAt(prefix.value, i);
                        if (c1 != c2) {
                            return false;
                        }
                    }
                }
            } else {
                if (coder == LATIN1) {
                    return false;
                } else {
                    for (int i = 0; i < count; ++i) {
                        gchar c1 = StringUtils::readUTF16CharAt(value, i + offset);
                        gchar c2 = StringUtils::readLatin1CharAt(prefix.value, i);
                        if (c1 != c2) {
                            return false;
                        }
                    }
                }
            }
            return true;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::startsWith(String const& prefix) const {
        try {
            return startsWith(prefix, 0);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::endsWith(String const& suffix) const {
        try {
            return startsWith(suffix, length() - suffix.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint String::hash() const {
        // The hashValue or hashIsZero fields are subject to a benign data race,
        // making it crucial to ensure that any observable result of the
        // calculation in this method stays correct under any possible read of
        // these fields. Necessary restrictions to allow this to be correct
        // without explicit memory fences or similar concurrency primitives is
        // that we can ever only write to one of these two fields for a given
        // String instance, and that the computation is idempotent and derived
        // from immutable state
        gint hash = hashValue;
        if (hash == 0 && !hashIsZero) {
            gint count = length();
            if (coding() == LATIN1) {
                for (int i = 0; i < count; ++i) {
                    if (i == 0) {
                        hash = StringUtils::readLatin1CharAt(value, i);
                    } else {
                        hash = hash * 31 ^ (count - i);
                        hash += StringUtils::readLatin1CharAt(value, i);
                    }
                }
            } else {
                for (int i = 0; i < count; ++i) {
                    if (i == 0) {
                        hash = StringUtils::readUTF16CharAt(value, i);
                    } else {
                        hash = hash * 31 ^ (count - i);
                        hash += StringUtils::readUTF16CharAt(value, i);
                    }
                }
            }
            hashIsZero = (hashValue = hash) == 0;
        }
        return hash;
    }

    gint String::indexOf(gint ch) const {
        return indexOf(ch, 0);
    }

    gint String::indexOf(gint ch, gint fromIndex) const {
        fromIndex = Math::max(fromIndex, 0);
        gint count = length();
        if (fromIndex >= count) {
            return -1;
        }
        if (coding() == LATIN1) {
            if (Character::isValidCodePoint(ch) && StringUtils::isLatin1(ch)) {
                for (int i = fromIndex; i < count; ++i) {
                    gchar c = StringUtils::readLatin1CharAt(value, i);
                    if (c == ch) {
                        return i;
                    }
                }
            }
        } else {
            if (Character::isValidCodePoint(ch) && StringUtils::isLatin1(ch)) {
                for (int i = fromIndex; i < count; ++i) {
                    gchar c = StringUtils::readUTF16CharAt(value, i);
                    if (c == ch) {
                        return i;
                    }
                }
            }
        }
        return -1;
    }

    gint String::lastIndexOf(gint ch) const {
        return lastIndexOf(ch, length() - 1);
    }

    gint String::lastIndexOf(gint ch, gint fromIndex) const {
        gint count = length();
        fromIndex = Math::clamp(fromIndex, -1, count - 1);
        if (fromIndex < 0) {
            return -1;
        }
        if (coding() == LATIN1) {
            if (Character::isValidCodePoint(ch) && StringUtils::isLatin1(ch)) {
                for (int i = fromIndex; i >= 0; --i) {
                    gchar c = StringUtils::readLatin1CharAt(value, i);
                    if (c == ch) {
                        return i;
                    }
                }
            }
        } else {
            if (Character::isValidCodePoint(ch) && StringUtils::isLatin1(ch)) {
                for (int i = fromIndex; i >= 0; --i) {
                    gchar c = StringUtils::readUTF16CharAt(value, i);
                    if (c == ch) {
                        return i;
                    }
                }
            }
        }
        return -1;
    }

    gint String::indexOf(String const& str) const {
        return indexOf(str, 0);
    }

    gint String::indexOf(String const& str, gint fromIndex) const {
        gint count1 = length();
        gint count2 = str.length();
        fromIndex = Math::clamp(fromIndex, 0, count1);

        if (count2 > count1 - fromIndex) {
            return -1;
        }

        if (count2 == 0) {
            return fromIndex;
        }

        Coder coder = coding();
        if (coder == str.coding()) {
            if (coder == LATIN1) {
                gchar f = StringUtils::readLatin1CharAt(str.value, 0);
                gint count = count1 - count2;
                for (int i = fromIndex; i <= count; ++i) {
                    gchar c1 = StringUtils::readLatin1CharAt(value, i);
                    // Look for first character.
                    if (c1 != f) {
                        continue;
                    }
                    // Found first character, now look at the rest of value
                    gint j = i + 1;
                    for (int k = 1; k < count2; ++k, ++j) {
                        c1 = StringUtils::readLatin1CharAt(value, j);
                        gchar c2 = StringUtils::readLatin1CharAt(str.value, k);
                        if (c1 != c2) {
                            break;
                        }
                    }
                    if (j == i + count2) {
                        // Found whole string.
                        return i;
                    }
                }
            } else {
                gchar f = StringUtils::readUTF16CharAt(str.value, 0);
                gint count = count1 - count2;
                for (int i = fromIndex; i <= count; ++i) {
                    gchar c1 = StringUtils::readUTF16CharAt(value, i);
                    // Look for first character.
                    if (c1 != f) {
                        continue;
                    }
                    // Found first character, now look at the rest of value
                    gint j = i + 1;
                    for (int k = 1; k < count2; ++k, ++j) {
                        c1 = StringUtils::readUTF16CharAt(value, j);
                        gchar c2 = StringUtils::readUTF16CharAt(str.value, k);
                        if (c1 != c2) {
                            break;
                        }
                    }
                    if (j == i + count2) {
                        // Found whole string.
                        return i;
                    }
                }
            }
        } else {
            if (coder == LATIN1) {
                return -1;
            } else {
                gchar f = StringUtils::readLatin1CharAt(str.value, 0);
                gint count = count1 - count2;
                for (int i = fromIndex; i <= count; ++i) {
                    gchar c1 = StringUtils::readUTF16CharAt(value, i);
                    // Look for first character.
                    if (c1 != f) {
                        continue;
                    }
                    // Found first character, now look at the rest of value
                    gint j = i + 1;
                    for (int k = 1; k < count2; ++k, ++j) {
                        c1 = StringUtils::readUTF16CharAt(value, j);
                        gchar c2 = StringUtils::readLatin1CharAt(str.value, k);
                        if (c1 != c2) {
                            break;
                        }
                    }
                    if (j == i + count2) {
                        // Found whole string.
                        return i;
                    }
                }
            }
        }
        return -1;
    }

    gint String::lastIndexOf(String const& str) const {
        return lastIndexOf(str, length());
    }

    gint String::lastIndexOf(String const& str, gint fromIndex) const {
        gint count1 = length();
        gint count2 = str.length();

        if (fromIndex < 0) {
            return -1;
        }

        if (fromIndex >= count1)
            fromIndex = count1 - 1;

        if (count2 == 0) {
            return fromIndex;
        }

        if (fromIndex - count2 < -1) {
            return -1;
        }

        Coder coder = coding();
        if (coder == str.coding()) {
            if (coder == LATIN1) {
                gchar l = StringUtils::readLatin1CharAt(str.value, count2 - 1);
                gint limit = count2 - 1;
                for (int i = fromIndex; i > limit; --i) {
                    gchar c1 = StringUtils::readLatin1CharAt(value, i);
                    // Look for last character.
                    if (c1 != l) {
                        continue;
                    }
                    // Found first character, now look at the rest of value
                    gint j = i - 1;
                    for (int k = count2 - 2; k >= 0; --k, --j) {
                        c1 = StringUtils::readLatin1CharAt(value, j);
                        gchar c2 = StringUtils::readLatin1CharAt(str.value, k);
                        if (c1 != c2) {
                            break;
                        }
                    }
                    if (j <= i - count2) {
                        // Found whole string.
                        return j + 1;
                    }
                }
            } else {
                gchar l = StringUtils::readUTF16CharAt(str.value, count2 - 1);
                gint limit = count2 - 1;
                for (int i = fromIndex; i > limit; --i) {
                    gchar c1 = StringUtils::readUTF16CharAt(value, i);
                    // Look for last character.
                    if (c1 != l) {
                        continue;
                    }
                    // Found first character, now look at the rest of value
                    gint j = i - 1;
                    for (int k = count2 - 2; k >= 0; --k, --j) {
                        c1 = StringUtils::readUTF16CharAt(value, j);
                        gchar c2 = StringUtils::readUTF16CharAt(str.value, k);
                        if (c1 != c2) {
                            break;
                        }
                    }
                    if (j <= i - count2) {
                        // Found whole string.
                        return j + 1;
                    }
                }
            }
        } else {
            if (coder == LATIN1) {
                return -1;
            } else {
                gchar l = StringUtils::readLatin1CharAt(str.value, count2 - 1);
                gint limit = count2 - 1;
                for (int i = fromIndex; i > limit; --i) {
                    gchar c1 = StringUtils::readUTF16CharAt(value, i);
                    // Look for last character.
                    if (c1 != l) {
                        continue;
                    }
                    // Found first character, now look at the rest of value
                    gint j = i - 1;
                    for (int k = count2 - 2; k >= 0; --k, --j) {
                        c1 = StringUtils::readUTF16CharAt(value, j);
                        gchar c2 = StringUtils::readLatin1CharAt(str.value, k);
                        if (c1 != c2) {
                            break;
                        }
                    }
                    if (j <= i - count2) {
                        // Found whole string.
                        return j + 1;
                    }
                }
            }
        }
        return -1;
    }

    String String::subString(gint beginIndex) const {
        try {
            return subString(beginIndex, length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::subString(gint beginIndex, gint endIndex) const {
        try {
            Preconditions::checkIndexFromRange(beginIndex, endIndex, length());

            gint count1 = length();
            gint count2 = endIndex - beginIndex;

            if (count2 == count1) {
                return *this;
            }

            Coder coder = coding();

            String str;
            if (coder == LATIN1) {
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, beginIndex, count2);
            } else {
                str.coder = UTF16;
                str.value = StringUtils::copyOfUTF16(value, beginIndex, count2);
            }
            str.count = count2;

            return str;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharSequence& String::subSequence(gint startIndex, gint endIndex) const {
        try {
            String str = subString(startIndex, endIndex);
            return CORE_XCAST(CharSequence, str.clone());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::concat(String const& str) const {
        gint count1 = length();
        if (count1 == 0) {
            return str;
        }

        gint count2 = str.length();
        if (count2 == 0) {
            return *this;
        }

        gint count = count1 + count2;
        if (count < 0) {
            OutOfMemoryError("Overflow: String required length exceed implementation limit."_Sl)
                    .throws($ftrace());
        }

        Coder coder = coding();

        String newStr;
        try {
            if (coder == str.coding()) {
                if (coder == LATIN1) {
                    newStr.coder = LATIN1;
                    newStr.value = StringUtils::copyOfLatin1(value, 0, count1, count);
                    StringUtils::copyLatin1(str.value, 0, newStr.value, count1, count2);
                } else {
                    newStr.coder = UTF16;
                    newStr.value = StringUtils::copyOfUTF16(value, 0, count1, count);
                    StringUtils::copyUTF16(str.value, 0, newStr.value, count1, count2);
                }
            } else {
                newStr.coder = UTF16;
                if (coder == LATIN1) {
                    newStr.value = StringUtils::copyOfLatin1ToUTF16(value, 0, count1, count);
                    StringUtils::copyUTF16(str.value, 0, newStr.value, count1, count2);
                } else {
                    newStr.value = StringUtils::copyOfUTF16(value, 0, count1, count);
                    StringUtils::copyLatin1ToUTF16(str.value, 0, newStr.value, count1, count2);
                }
            }
            newStr.count = count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return newStr;
    }

    String String::replace(gchar oldChar, gchar newChar) const {
        try {
            gint count = length();
            if (count == 0) {
                return *this;
            }

            Coder coder = coding();
            if (coder == LATIN1 && StringUtils::isLatin1(oldChar)) {
                gint i;
                for (i = 0; i < count; ++i) {
                    gchar c = StringUtils::readLatin1CharAt(value, i);
                    if (c == oldChar) {
                        break;
                    }
                }

                if (i < count) {
                    String str;
                    if (StringUtils::isLatin1(newChar)) {
                        str.coder = LATIN1;
                        str.value = StringUtils::copyOfLatin1(value, 0, i, count);
                        for (; i < count; ++i) {
                            gchar c = StringUtils::readLatin1CharAt(value, i);
                            StringUtils::writeLatin1CharAt(str.value, i, (c == oldChar) ? newChar : c);
                        }
                    } else {
                        str.coder = UTF16;
                        str.value = StringUtils::copyOfLatin1ToUTF16(value, 0, i, count);
                        for (; i < count; ++i) {
                            gchar c = StringUtils::readLatin1CharAt(value, i);
                            StringUtils::writeUTF16CharAt(str.value, i, (c == oldChar) ? newChar : c);
                        }
                    }
                    str.count = count;
                    return str;
                }
            } else if (coder == UTF16) {
                gint i;
                for (i = 0; i < count; ++i) {
                    gchar c = StringUtils::readUTF16CharAt(value, i);
                    if (c == oldChar) {
                        break;
                    }
                }

                if (i < count) {
                    String str;
                    if (StringUtils::isLatin1(newChar)) {
                        str.coder = UTF16;
                        str.value = StringUtils::copyOfUTF16(value, 0, i, count);
                        for (; i < count; ++i) {
                            gchar c = StringUtils::readUTF16CharAt(value, i);
                            StringUtils::writeUTF16CharAt(str.value, i, (c == oldChar) ? newChar : c);
                        }
                        // try to inflate result
                        StringUtils::BYTES bytes = StringUtils::inflateUTF16ToLatin1(str.value, 0, count);
                        if (bytes != null) {
                            StringUtils::destroyString(str.value);
                            str.coder = LATIN1;
                            str.value = bytes;
                        }
                    } else {
                        str.coder = UTF16;
                        str.value = StringUtils::copyOfUTF16(value, 0, i, count);
                        for (; i < count; ++i) {
                            gchar c = StringUtils::readUTF16CharAt(value, i);
                            StringUtils::writeUTF16CharAt(str.value, i, (c == oldChar) ? newChar : c);
                        }
                    }
                    str.count = count;
                    return str;
                }
            }
            return *this;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::matches(String const& regex) const {
        try {
            return regex::Pattern::matches(regex, *this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::contains(CharSequence const& s) const {
        try {
            return (Class<String>::hasInstance(s))
                       ? indexOf(CORE_XCAST(String const, s)) >= 0
                       : indexOf(s.toString()) >= 0;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::replace(CharSequence const& target, CharSequence const& replacement) const {
        gint count1 = length();
        if (count1 == 0) {
            return *this;
        }

        String str1 = target.toString();
        gint count2 = target.length();

        String str2 = replacement.toString();
        gint count3 = replacement.length();

        try {
            if (count2 == 0) {
                gint count = Math::addExact(count1, Math::multiplyExact(Math::addExact(count1, 1), count3));
                XString str = XString(count);
                for (int i = 0; i < count1; ++i) {
                    str.append(str2).append(charAt(i));
                }
                str.append(str2);
                return str.toString();
            }
            if (count2 == 1 && count3 == 1) {
                return replace(str1.charAt(0), str2.charAt(0));
            }

            Coder coder1 = coding();
            Coder coder2 = str1.coding();
            Coder coder3 = str2.coding();

            if (coder1 == LATIN1 && coder2 == LATIN1 && coder3 == LATIN1) {
                gint i;
                if ((i = indexOf(str1)) < 0) {
                    return *this;
                }

                IntArray pos = IntArray(16);
                gint p = 0;
                pos[0] = i;
                gint j;
                i += count2;
                while ((j = indexOf(str1, i)) >= i) {
                    if (++p == pos.length()) {
                        IntArray pos2 = IntArray(p + (p >> 1));
                        for (int k = 0; k < p; ++k) {
                            pos2[k] = pos[k];
                        }
                        pos = (IntArray&&) pos2;
                    }

                    pos[p] = j;
                    i = j + count2;
                }

                gint count = Math::addExact(count1, Math::multiplyExact(++p, count3 - count2));

                if (count == 0) {
                    return String();
                }

                String str;
                str.coder = LATIN1;
                str.value = StringUtils::newLatin1String(count);

                i = j = 0;
                for (int k = 0; k < p; ++k) {
                    gint next = pos[k];
                    StringUtils::copyLatin1(value, i, str.value, j, next - i);
                    j += next - i;
                    i = next;
                    StringUtils::copyLatin1(str2.value, 0, str.value, j, count3);
                    i += count2;
                    j += count3;
                }
                StringUtils::copyLatin1(value, i, str.value, j, count1 - i);
                str.count = count;

                return str;
            } else {
                //  Possible combinations of the arguments/result encodings:
                //  +---+--------+--------+--------+-----------------------+
                //  | # | VALUE  | TARGET | REPL   | RESULT                |
                //  +===+========+========+========+=======================+
                //  | 1 | Latin1 | Latin1 |  UTF16 | null or UTF16         |
                //  +---+--------+--------+--------+-----------------------+
                //  | 2 | Latin1 |  UTF16 | Latin1 | null                  |
                //  +---+--------+--------+--------+-----------------------+
                //  | 3 | Latin1 |  UTF16 |  UTF16 | null                  |
                //  +---+--------+--------+--------+-----------------------+
                //  | 4 |  UTF16 | Latin1 | Latin1 | null or UTF16         |
                //  +---+--------+--------+--------+-----------------------+
                //  | 5 |  UTF16 | Latin1 |  UTF16 | null or UTF16         |
                //  +---+--------+--------+--------+-----------------------+
                //  | 6 |  UTF16 |  UTF16 | Latin1 | null, Latin1 or UTF16 |
                //  +---+--------+--------+--------+-----------------------+
                //  | 7 |  UTF16 |  UTF16 |  UTF16 | null or UTF16         |
                //  +---+--------+--------+--------+-----------------------+
                if (coder1 == LATIN1 && coder2 == UTF16) {
                    // combinations 2 or 3
                    return *this;
                }

                gint i = 0;
                if ((i = indexOf(str1)) < 0) {
                    return *this;
                }

                IntArray pos = IntArray(16);
                gint p = 0;
                pos[0] = i;
                gint j = 0;
                i += count2;
                while ((j = indexOf(str1, i)) >= i) {
                    if (++p == pos.length()) {
                        IntArray pos2 = IntArray(p + (p >> 1));
                        for (int k = 0; k < p; ++k) {
                            pos2[k] = pos[k];
                        }
                        pos = (IntArray&&) pos2;
                    }

                    pos[p] = j;
                    i = j + count2;
                }

                gint count = Math::addExact(count1, Math::multiplyExact(++p, count3 - count2));

                if (count == 0) {
                    return String();
                }

                String str;
                str.coder = UTF16;
                str.value = StringUtils::newUTF16String(count);

                i = j = 0;
                for (int k = 0; k < p; ++k) {
                    gint next = pos[k];
                    if (coder1 == LATIN1) {
                        StringUtils::copyLatin1ToUTF16(value, i, str.value, j, next - i);
                    } else {
                        StringUtils::copyUTF16(value, i, str.value, j, next - i);
                    }
                    j += next - i;
                    i = next;
                    if (coder3 == LATIN1) {
                        StringUtils::copyLatin1ToUTF16(str2.value, 0, str.value, j, count3);
                    } else {
                        StringUtils::copyUTF16(str2.value, 0, str.value, j, count3);
                    }
                    i += count2;
                    j += count3;
                }
                if (coder1 == LATIN1) {
                    StringUtils::copyLatin1ToUTF16(value, i, str.value, j, count1 - i);
                } else {
                    StringUtils::copyUTF16(value, i, str.value, j, count1 - i);
                }

                if (coder2 == UTF16 && coder3 == LATIN1) {
                    // combination 6
                    // try to inflate result
                    StringUtils::BYTES bytes = StringUtils::inflateUTF16ToLatin1(str.value, 0, count);
                    if (bytes != null) {
                        StringUtils::destroyString(str.value);
                        str.coder = LATIN1;
                        str.value = bytes;
                    }
                }

                str.count = count;

                return str;
            }
        } catch (ArithmeticException const& ex) {
            CORE_IGNORE(ex);
            OutOfMemoryError("Overflow: String required length exceed implementation limit."_Sl)
                    .throws($ftrace());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::replaceFirst(String const& regex, String const& replacement) const {
        try {
            return regex::Pattern::compile(regex).matcher(*this).replaceFirst(replacement);
        } catch (Throwable const&
            ex) {
            ex.throws($ftrace());
        }
    }

    String String::replaceAll(String const& regex, String const& replacement) const {
        try {
            return regex::Pattern::compile(regex).matcher(*this).replaceAll(replacement);
        } catch (Throwable const&
            ex) {
            ex.throws($ftrace());
        }
    }

    StringArray String::split(String const& regex, gint limit) const {
        try {
            return split(regex, limit, false);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    StringArray String::splitWithDelimiters(String const& regex, gint limit) const {
        try {
            return split(regex, limit, true);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    StringArray String::split(String const& regex) const {
        try {
            return split(regex, 0, false);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::join(CharSequence const& delimiter, Array<CharSequence> const& elements) {
        StringArray sa = StringArray(elements.length());
        for (gint i = 0; i < sa.length(); i++)
            sa[i] = valueOf(elements.getOrNull(i));
        return join("", "", delimiter.toString(), sa, sa.length());
    }

    String String::join(CharSequence const& delimiter, StringArray const& elements) {
        return join("", "", delimiter.toString(), elements, elements.length());
    }

    String String::join(CharSequence const& delimiter, Iterable<CharSequence> const& elements) {
        StringArray sa = StringArray(8);
        gint size = 0;
        for (CharSequence const& seq : elements) {
            if (size >= sa.length()) {
                sa = Arrays::copyOf(sa, sa.length() << 1);
            }
            sa[size++] = valueOf(seq);
        }
        return join("", "", delimiter.toString(), sa, sa.length());
    }

    String String::join(CharSequence const& delimiter, Iterable<String> const& elements) {
        StringArray sa = StringArray(8);
        gint size = 0;
        for (CharSequence const& seq : elements) {
            if (size >= sa.length()) {
                sa = Arrays::copyOf(sa, sa.length() << 1);
            }
            sa[size++] = valueOf(seq);
        }
        return join("", "", delimiter.toString(), sa, sa.length());
    }

    String String::toLowerCase(util::Locale const& locale) const {
        return toLowerCase();
    }

    String String::toLowerCase() const {
        gint count = length();
        gint coder = coding();

        String str;
        try {
            if (coder == LATIN1) {
                int i = 0;
                for (; i < count; ++i) {
                    gchar c = StringUtils::readLatin1CharAt(value, i);
                    gchar c2 = CharacterDataLatin1::instance.toLowerCase(c);
                    if (c != c2) {
                        break;
                    }
                }
                if (i == count) {
                    return *this;
                }
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, 0, i, count);
                for (int j = i; j < count; ++j) {
                    gchar c = StringUtils::readLatin1CharAt(value, j);
                    gchar c2 = CharacterDataLatin1::instance.toLowerCase(c);
                    if (!StringUtils::isLatin1(c2)) {
                        // restart operation
                        StringUtils::destroyString(str.value);
                        str.value = StringUtils::copyOfLatin1ToUTF16(value, 0, i);
                        str.coder = UTF16;
                        for (int k = i; k < count; ++k) {
                            c = StringUtils::readLatin1CharAt(value, j);
                            c2 = CharacterDataLatin1::instance.toLowerCase(c);
                            StringUtils::writeUTF16CharAt(str.value, k, c2);
                        }
                        break;
                    }
                    StringUtils::writeLatin1CharAt(str.value, j, c2);
                }
                str.count = count;
            } else {
                int i;
                for (i = 0; i < count;) {
                    gint c = StringUtils::readUTF32CharAt(value, i);
                    gint c2 = Character::toLowerCase(c);
                    if (c != c2) {
                        break;
                    }
                    i += Character::charCount(c);
                }
                if (i == count) {
                    return *this;
                }
                str.coder = LATIN1;
                str.value = StringUtils::copyOfUTF16(value, 0, i, count);
                gint k = 0;
                for (int j = i; j < count;) {
                    gint c = StringUtils::readUTF32CharAt(value, j);
                    gint c2 = Character::toLowerCase(c);
                    if (Character::isSupplementary(c2)) {
                        StringUtils::writeUTF16CharAt(str.value, k + 0, Character::highSurrogate(c2));
                        StringUtils::writeUTF16CharAt(str.value, k + 1, Character::lowSurrogate(c2));
                        k += 2;
                    } else {
                        StringUtils::writeUTF16CharAt(str.value, j + 1, (gchar) c2);
                        k += 1;
                    }
                    j += Character::charCount(c);
                }
                str.count = k;
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return str;
    }

    String String::toUpperCase(util::Locale const& locale) const {
        return toUpperCase();
    }

    String String::toUpperCase() const {
        gint count = length();
        gint coder = coding();

        String str;
        try {
            if (coder == LATIN1) {
                int i = 0;
                for (; i < count; ++i) {
                    gchar c = StringUtils::readLatin1CharAt(value, i);
                    gchar c2 = CharacterDataLatin1::instance.toUpperCase(c);
                    if (c != c2) {
                        break;
                    }
                }
                if (i == count) {
                    return *this;
                }
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, 0, i, count);
                for (int j = i; j < count; ++j) {
                    gchar c = StringUtils::readLatin1CharAt(value, j);
                    gchar c2 = CharacterDataLatin1::instance.toUpperCase(c);
                    if (!StringUtils::isLatin1(c2)) {
                        // restart operation
                        StringUtils::destroyString(str.value);
                        str.value = StringUtils::copyOfLatin1ToUTF16(value, 0, i);
                        str.coder = UTF16;
                        for (int k = i; k < count; ++k) {
                            c = StringUtils::readLatin1CharAt(value, j);
                            c2 = CharacterDataLatin1::instance.toUpperCase(c);
                            StringUtils::writeUTF16CharAt(str.value, k, c2);
                        }
                        break;
                    }
                    StringUtils::writeLatin1CharAt(str.value, j, c2);
                }
                str.count = count;
            } else {
                int i;
                for (i = 0; i < count;) {
                    gint c = StringUtils::readUTF32CharAt(value, i);
                    gint c2 = Character::toUpperCase(c);
                    if (c != c2) {
                        break;
                    }
                    i += Character::charCount(c);
                }
                if (i == count) {
                    return *this;
                }
                str.coder = LATIN1;
                str.value = StringUtils::copyOfUTF16(value, 0, i, count);
                gint k = 0;
                for (int j = i; j < count;) {
                    gint c = StringUtils::readUTF32CharAt(value, j);
                    gint c2 = Character::toUpperCase(c);
                    if (Character::isSupplementary(c2)) {
                        StringUtils::writeUTF16CharAt(str.value, k + 0, Character::highSurrogate(c2));
                        StringUtils::writeUTF16CharAt(str.value, k + 1, Character::lowSurrogate(c2));
                        k += 2;
                    } else {
                        StringUtils::writeUTF16CharAt(str.value, j + 1, (gchar) c2);
                        k += 1;
                    }
                    j += Character::charCount(c);
                }
                str.count = k;
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return str;
    }

    String String::trim() const {
        try {
            gint count = length();
            Coder coder = coding();
            if (coder == LATIN1) {
                // skip all leading spaces
                gint i = 0;
                for (; i < count; ++i) {
                    gchar c = StringUtils::readLatin1CharAt(value, i);
                    if (c > ' ') {
                        break;
                    }
                }
                // skip all trailing spaces
                gint j = count;
                for (; j > i; --j) {
                    gchar c = StringUtils::readLatin1CharAt(value, j - 1);
                    if (c > ' ') {
                        break;
                    }
                }
                String str;
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, i, j - i);
                return str;
            } else {
                // skip all leading spaces
                gint i = 0;
                for (; i < count; ++i) {
                    gchar c = StringUtils::readUTF16CharAt(value, i);
                    if (c > ' ') {
                        break;
                    }
                }
                // skip all trailing spaces
                gint j = count;
                for (; j > i; --j) {
                    gchar c = StringUtils::readUTF16CharAt(value, j - 1);
                    if (c > ' ') {
                        break;
                    }
                }
                String str;
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, i, j - i);
                return str;
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::strip() const {
        try {
            gint count = length();
            Coder coder = coding();
            if (coder == LATIN1) {
                // skip all leading spaces
                gint i = 0;
                for (; i < count; ++i) {
                    gchar c = StringUtils::readLatin1CharAt(value, i);
                    if (c != ' ' && c != '\t' && !CharacterDataLatin1::instance.isWhitespace(c)) {
                        break;
                    }
                }
                // skip all trailing spaces
                gint j = count;
                for (; j > i; --j) {
                    gchar c = StringUtils::readLatin1CharAt(value, j - 1);
                    if (c != ' ' && c != '\t' && !CharacterDataLatin1::instance.isWhitespace(c)) {
                        break;
                    }
                }
                String str;
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, i, j - i);
                str.count = j - i;
                return str;
            } else {
                // skip all leading spaces
                gint i = 0;
                for (; i < count; ++i) {
                    gchar c = StringUtils::readUTF16CharAt(value, i);
                    if (c != ' ' && c != '\t' && !Character::isWhitespace(c)) {
                        break;
                    }
                }
                // skip all trailing spaces
                gint j = count;
                for (; j > i; --j) {
                    gchar c = StringUtils::readUTF16CharAt(value, j - 1);
                    if (c != ' ' && c != '\t' && !Character::isWhitespace(c)) {
                        break;
                    }
                }
                String str;
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, i, j - i);
                str.count = j - i;
                return str;
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::stripLeading() const {
        try {
            gint count = length();
            Coder coder = coding();
            if (coder == LATIN1) {
                // skip all leading spaces
                gint i = 0;
                for (; i < count; ++i) {
                    gchar c = StringUtils::readLatin1CharAt(value, i);
                    if (c != ' ' && c != '\t' && !CharacterDataLatin1::instance.isWhitespace(c)) {
                        break;
                    }
                }
                gint j = count;
                String str;
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, i, j - i);
                str.count = j - i;
                return str;
            } else {
                // skip all leading spaces
                gint i = 0;
                for (; i < count; ++i) {
                    gchar c = StringUtils::readUTF16CharAt(value, i);
                    if (c != ' ' && c != '\t' && !Character::isWhitespace(c)) {
                        break;
                    }
                }
                gint j = count;
                String str;
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, i, j - i);
                str.count = j - i;
                return str;
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::stripTrailing() const {
        try {
            gint count = length();
            Coder coder = coding();
            if (coder == LATIN1) {
                // skip all leading spaces
                gint i = 0;
                // skip all trailing spaces
                gint j = count;
                for (; j > i; --j) {
                    gchar c = StringUtils::readLatin1CharAt(value, j - 1);
                    if (c != ' ' && c != '\t' && !CharacterDataLatin1::instance.isWhitespace(c)) {
                        break;
                    }
                }
                String str;
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, i, j - i);
                str.count = j - i;
                return str;
            } else {
                // skip all leading spaces
                gint i = 0;
                // skip all trailing spaces
                gint j = count;
                for (; j > i; --j) {
                    gchar c = StringUtils::readUTF16CharAt(value, j - 1);
                    if (c != ' ' && c != '\t' && !Character::isWhitespace(c)) {
                        break;
                    }
                }
                String str;
                str.coder = LATIN1;
                str.value = StringUtils::copyOfLatin1(value, i, j - i);
                str.count = j - i;
                return str;
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::isBlank() const {
        gint count = length();
        Coder coder = coding();
        gint i = 0;
        if (coder == LATIN1) {
            for (; i < count; ++i) {
                gchar c = StringUtils::readLatin1CharAt(value, i);
                if (c != ' ' && c != '\t' && !CharacterDataLatin1::instance.isWhitespace(c)) {
                    break;
                }
            }
        } else {
            for (; i < count; ++i) {
                gchar c = StringUtils::readUTF16CharAt(value, i);
                if (c != ' ' && c != '\t' && !Character::isWhitespace(c)) {
                    break;
                }
            }
        }
        return i == count;
    }

    String String::translateEscapes() const {
        gint count = length();

        if (count == 0)
            return { };

        CharArray ca = CharArray(count);
        gint i = 0;
        for (int j = 0; j < count;) {
            gchar c = charAt(j++);
            if (c == '\\') {
                c = j == count ? '\0' : charAt(j++);
                switch (c) {
                    case 'a':
                        ca[i++] = u'\a';
                        continue;
                    case 'b':
                        ca[i++] = u'\b';
                        continue;
                    case 'f':
                        ca[i++] = u'\f';
                        continue;
                    case 'n':
                        ca[i++] = u'\n';
                        continue;
                    case 'r':
                        ca[i++] = u'\r';
                        continue;
                    case 's':
                        ca[i++] = u' ';
                        continue;
                    case 't':
                        ca[i++] = u'\t';
                        continue;
                    case 'v':
                        ca[i++] = u'\v';
                        continue;
                    case '\'':
                        ca[i++] = u'\'';
                        continue;
                    case '\"':
                        ca[i++] = u'\"';
                        continue;
                    case '\\':
                        ca[i++] = u'\\';
                        continue;
                    case '0': // Octal Escape \0, \0X, \0XX
                    case '1': // \1 , \1X, \1XX
                    case '2': // \2 , \2X, \2XX
                    case '3': // \3 , \3X
                    case '4': // \4 , \4X
                    case '5': // \5 , \5X
                    case '6': // \6 , \6X
                    case '7': // \7 , \7x
                    {
                        gint const rem = count - j;
                        gint const n = Math::min((c < '3') ? 2 : 1, rem);
                        gint code = Character::digit(c, 8);
                        gint k;
                        for (k = 0; k < n; ++k) {
                            c = charAt(j + k);
                            gint const digit = Character::digit(c, 8);
                            if (digit < 0) {
                                if (code < 0 || code > 0xff)
                                    IllegalArgumentException("Invalid octal escape sequence: "_Sl +
                                        subString(j - 2, j + k + 1)).throws($ftrace());
                                break;
                            }
                            code = code * 8 + digit;
                        }
                        ca[i++] = code;
                        j += k;
                        continue;
                    }
                    case '\n': // continuity
                    case '\r': // continuity
                        continue;
                    case 'u': // Utf-16 escape U + xxxx
                    {
                        gint const rem = count - j;
                        if (rem < 4)
                            IllegalArgumentException("Invalid unicode escape at index "_Sl +
                                (j - 2)).throws($ftrace());
                        gint code = 0;
                        for (int k = 0; k < 4; ++k) {
                            c = charAt(k + j);
                            gint const digit = Character::digit(c, 16);
                            if (digit < 0)
                                IllegalArgumentException("Invalid unicode escape "_Sl +
                                    subString(j - 2, k)).throws($ftrace());
                            code = code * 16 + digit;
                        }
                        ca[i++] = code;
                        j += 4;
                        continue;
                    }
                    case 'U': // Utf-32 escape U + 00xx xxxx
                    {
                        gint const rem = count - j;
                        if (rem < 8)
                            IllegalArgumentException("Invalid unicode escape at index "_Sl +
                                (j - 2)).throws($ftrace());
                        gint code = 0;
                        for (int k = 0; k < 8; ++k) {
                            c = charAt(k + j);
                            gint const digit = Character::digit(c, 16);
                            if (digit < 0)
                                IllegalArgumentException("Invalid unicode escape: "_Sl +
                                    subString(j - 2, k)).throws($ftrace());
                            code = code * 16 + digit;
                        }
                        if (!Character::isValidCodePoint(code))
                            IllegalArgumentException("Invalid unicode code point: "_Sl + subString(j - 2, j + 8)).
                                    throws(
                                        $ftrace());
                        if (Character::isSupplementary(code)) {
                            ca[i++] = Character::highSurrogate(code);
                            ca[i++] = Character::lowSurrogate(code);
                        } else
                            ca[i++] = code;
                        j += 8;
                        continue;
                    }
                    case 'x': // Hex escape \xy..y
                    {
                        gint const rem = count - j;
                        if (rem <= 0)
                            IllegalArgumentException("Invalid unicode escape at index "_Sl +
                                (j - 2)).throws($ftrace());
                        gint code = 0;
                        gint k;
                        for (k = 0; k < rem; ++k) {
                            c = charAt(k + j);
                            gint const digit = Character::digit(c, 16);
                            if (digit < 0)
                                break;
                            code = code * 16 + digit;
                        }
                        if (!Character::isValidCodePoint(code))
                            IllegalArgumentException("Invalid hexadecimal escape: "_Sl +
                                subString(j - 2, k)).throws($ftrace());
                        if (Character::isSupplementary(code)) {
                            ca[i++] = Character::highSurrogate(code);
                            ca[i++] = Character::lowSurrogate(code);
                        } else
                            ca[i++] = code;
                        j += k;
                        continue;
                    }
                    default:
                        IllegalArgumentException("Invalid escape sequence: '"_Sl
                                    + c + "' (U+"_Sl + Integer::toHexString(c) + ")"_Sl)
                                .throws($ftrace());
                }
                continue;
            }
            ca[i++] = c;
        }
        return String(ca, 0, i);
    }

    String String::toString() const {
        return *this;
    }

    String String::valueOf(Object const& obj) {
        try {
            return obj.toString();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::valueOf(CharArray const& data, gint offset, gint count) {
        try {
            return String(data, offset, count);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::valueOf(gbool b) {
        try {
            return Boolean::toString(b);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::valueOf(gchar c) {
        try {
            return Character::toString(c);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::valueOf(gint i) {
        try {
            return Integer::toString(i);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::valueOf(glong l) {
        try {
            return Long::toString(l);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::valueOf(gfloat f) {
        try {
            return Float::toString(f);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::valueOf(gdouble d) {
        try {
            return Double::toString(d);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::repeat(gint nb) const {
        if (nb < 0)
            IllegalArgumentException("Negative number of repetition"_Sl).throws($ftrace());

        if (nb == 1) {
            return *this;
        }

        gint count = length();
        if (count == 0 || nb == 0) {
            return String();
        }

        Coder coder = coding();

        try {
            gint count2 = Math::multiplyExact(count, nb);
            String str;
            if (coder == LATIN1) {
                str.coder = LATIN1;
                str.value = StringUtils::newLatin1String(count2);
                for (int i = 0; i < nb; ++i) {
                    StringUtils::copyLatin1(value, 0, str.value, count * i, count);
                }
            } else {
                str.coder = UTF16;
                str.value = StringUtils::newUTF16String(count2);
                for (int i = 0; i < nb; ++i) {
                    StringUtils::copyUTF16(value, 0, str.value, count * i, count);
                }
            }
            str.count = count2;
            return str;
        } catch (ArithmeticException const& ex) {
            CORE_IGNORE(ex);
            OutOfMemoryError("Overflow: String required length exceed implementation limit."_Sl)
                    .throws($ftrace());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::valueOf(CharArray const& data) {
        try {
            return valueOf(data, 0, data.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray String::toChars() const {
        try {
            gint count = length();
            Coder coder = coding();

            CharArray ca = CharArray(count);
            if (coder == LATIN1) {
                StringUtils::copyLatin1ToUTF16(value, 0, ca.value, 0, count);
            } else {
                StringUtils::copyUTF16(value, 0, ca.value, 0, count);
            }
            return ca;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::join(String const& delimiter, StringArray const& args) {
        try {
            return join(delimiter, String(), String(), args, args.length());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String String::join(String const& delimiter,
                        String const& prefix,
                        String const& suffix,
                        StringArray const& elts,
                        gint nbUsed) {
        glong count = 0;

        // Find Required length
        count = prefix.length() + suffix.length();
        if (nbUsed > 0) {
            count += (glong) (nbUsed - 1) * delimiter.length();
        }

        gint coder = 0;

        // Find Required Coder
        coder |= delimiter.coding();
        coder |= prefix.coding();
        coder |= suffix.coding();

        for (int i = 0; i < nbUsed; ++i) {
            coder |= elts[i].coding();
            count += elts[i].length();
        }

        if (count > Integer::MAX_VALUE)
            OutOfMemoryError("Overflow: String required length exceed implementation limit."_Sl)
                    .throws($ftrace());

        String sj;

        if (coder == LATIN1) {
            // all parameters have been encoded to LATIN-1
            sj.value = StringUtils::newLatin1String((gint) count);
            sj.coder = LATIN1;
            sj.count = (gint) count;

            gint offset = 0;
            if (!prefix.isEmpty()) {
                StringUtils::copyLatin1(prefix.value, 0, sj.value, offset, prefix.length());
                offset += prefix.length();
            }

            for (int i = 0; i < nbUsed; ++i) {
                StringUtils::copyLatin1(elts[i].value, 0, sj.value, offset, elts[i].length());
                offset += elts[i].length();
                if (i < nbUsed - 1) {
                    StringUtils::copyLatin1(delimiter.value, 0, sj.value, offset, delimiter.length());
                    offset += delimiter.length();
                }
            }

            if (!suffix.isEmpty()) {
                StringUtils::copyLatin1(suffix.value, 0, sj.value, offset, suffix.length());
                offset += suffix.length();
            }
        } else {
            sj.value = StringUtils::newUTF16String((gint) count);
            sj.coder = UTF16;
            sj.count = (gint) count;

            gint offset = 0;
            if (!prefix.isEmpty()) {
                if (prefix.coding() == UTF16)
                    StringUtils::copyUTF16(prefix.value, 0, sj.value, offset, prefix.length());
                else
                    StringUtils::copyLatin1ToUTF16(prefix.value, 0, sj.value, offset, prefix.length());

                offset += prefix.length();
            }

            for (int i = 0; i < nbUsed; ++i) {
                String const& s = elts[i];

                if (!s.isEmpty()) {
                    if (s.coding() == UTF16)
                        StringUtils::copyUTF16(elts[i].value, 0, sj.value, offset, s.length());
                    else
                        StringUtils::copyLatin1ToUTF16(elts[i].value, 0, sj.value, offset, s.length());

                    offset += s.length();
                }

                if (i < nbUsed - 1) {
                    if (delimiter.coding() == UTF16)
                        StringUtils::copyUTF16(delimiter.value, 0, sj.value, offset, delimiter.length());
                    else
                        StringUtils::copyLatin1ToUTF16(delimiter.value, 0, sj.value, offset, delimiter.length());

                    offset += delimiter.length();
                }
            }

            if (!suffix.isEmpty()) {
                if (suffix.coding() == UTF16)
                    StringUtils::copyUTF16(suffix.value, 0, sj.value, offset, suffix.length());
                else
                    StringUtils::copyLatin1ToUTF16(suffix.value, 0, sj.value, offset, suffix.length());

                offset += suffix.length();
            }
        }

        return sj;
    }

    CharArray String::decodeUTF8Literal(glong str, glong est) {
        gint sp = 0;
        gint dp = 0;
        glong sl = est;
        gint dl = est > Integer::MAX_VALUE ? Integer::MAX_VALUE : (gint) est;
        static CORE_FAST gchar REPL = 0xFFFD;

        BYTES src = (BYTES) str;
        CharArray dst = CharArray(dl);
    ASCII_DECODING: {
            // try to decode ascii
            for (gint i = sp; i < sl && dp < dl; ++i) {
                if (src[i] < 0)
                    break;
                dst.value[i] = src[i];
                sp++;
                dp++;
            }
        }

        while (sp < sl) {
            gint b1 = src[sp];
            if (b1 > 0) {
                // 1 byte, 7 bits: 0xxxxxxx
                if (dp >= dl) {
                    // Overflow
                    break;
                }
                dst.value[dp++] = b1;
                sp++;
            } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                //                   [C2..DF] [80..BF]

                if (dp >= dl) {
                    // Overflow
                    break;
                }
                if (sl - sp < 2) {
                    // Underflow -> Malformed
                    dst.value[dp++] = REPL;
                    sp += 2;
                    // break;
                    continue;
                }
                gint b2 = src[sp + 1];
                // Now we check the first byte of 2-byte sequence as
                //     if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0)
                // no longer need to check b1 against c1 & c0 for
                // malformed as we did in previous version
                //   (b1 & 0x1e) == 0x0 || (b2 & 0xc0) != 0x80;
                // only need to check the second byte b2.
                if ((b2 & 0xc0) != 0x80) {
                    // Malformed Input (Non-Continuation)
                    dst.value[dp++] = REPL;
                    // break;
                    sp++;
                    continue;
                }
                dst.value[dp++] = (gchar) (b1 << 6 ^ b2 ^ ((gbyte) 0xC0 << 6 ^ (gbyte) 0x80 << 0));
                sp += 2;
            } else if ((b1 >> 4) == -2) {
                // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                glong rem = sl - sp;
                if (dp >= dl) {
                    // Overflow
                    break;
                }
                if (rem < 3) {
                    if (rem > 1 &&
                        ((b1 == (gbyte) 0xe0 && (src[sp + 1] & 0xe0) == 0x80) || (src[sp + 1] & 0xc0) != 0x80)) {
                        // Malformed input (3, 2)
                        dst.value[dp++] = REPL;
                        // break;
                        sp += 2;
                        continue;
                    }
                    // Underflow -> Malformed
                    dst.value[dp++] = REPL;
                    sp++;
                    // break;
                    continue;
                }
                gint b2 = src[sp + 1];
                gint b3 = src[sp + 2];
                if ((b1 == (gbyte) 0xe0 && (b2 & 0xe0) == 0x80) || (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80) {
                    // Malformed input (3)
                    dst.value[dp++] = REPL;
                    // break;
                    sp += 3;
                    continue;
                }
                gchar c = (gchar) (b1 << 12 ^ b2 << 6 ^ (b3 ^ ((gbyte) 0xE0 << 12 ^ (gbyte) 0x80 << 6 ^ (gbyte) 0x80 <<
                    0)));
                dst.value[dp++] = c;
                sp += 3;
            } else if ((b1 >> 3) == -2) {
                // 4 bytes, 21 bits: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                if (dl - dp < 2) {
                    // Overflow
                    break;
                }
                glong rem = sl - sp;
                if (rem < 4) {
                    b1 &= 0xff;
                    if (b1 > 0xf4 ||
                        rem > 1 && ((b1 == 0xf0 && (src[sp + 1] < 0x90 || src[sp + 1] > 0xbf)) ||
                            (b1 == 0xf4 && (src[sp + 1] & 0xf0) != 0x80) || (src[sp + 1] & 0xc0) != 0x80)) {
                        // Malformed input (4, 2)
                        dst.value[dp++] = REPL;
                        // break;
                        sp += 2;
                        continue;
                    }
                    if (rem > 2 && ((src[sp + 2] & 0xc0) != 0x80)) {
                        // Malformed input (4, 3)
                        dst.value[dp++] = REPL;
                        // break;
                        sp += 3;
                        continue;
                    }
                    // Underflow -> Malformed
                    dst.value[dp++] = REPL;
                    // break;
                    sp++;
                    continue;
                }
                gint b2 = src[sp + 1];
                gint b3 = src[sp + 2];
                gint b4 = src[sp + 3];
                gint c = b1 << 18 ^ b2 << 12 ^ b3 << 6 ^ (b4 ^ ((gbyte) 0xF0 << 18 ^ (gbyte) 0x80 << 12 ^ (gbyte) 0x80
                    << 6 ^ (gbyte) 0x80 << 0));
                if ((b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80 || (b4 & 0xc0) != 0x80) {
                    // Malformed input (4)
                    dst.value[dp++] = REPL;
                    // break;
                    sp += 4;
                    continue;
                }
                dst.value[dp++] = Character::highSurrogate(c);
                dst.value[dp++] = Character::highSurrogate(c);
                sp += 4;
            } else {
                // Malformed input
                if (dp >= dl) {
                    // Overflow
                    break;
                }
                dst.value[dp++] = REPL;
                sp += 1;
                // break;
                continue;
            }
        }

        if (dp < dl)
            return util::Arrays::copyOf(dst, dp);

        return dst;
    }

    ByteArray String::encodeLatin1ToUTF8(gint off, gint len) const {
        gint sp = off;
        gint sl = len + off;
        gint dp = 0;
        gint dl = (len - off) * 2;
        ByteArray dst = ByteArray(dl);

        // Try to encode ASCII characters.
        for (gint i = sp; i < sl; i++) {
            if (value[i] < 0)
                break;
            dst.value[dp++] = value[sp++];
        }

        gbool overflow = false;
        // Try
    TRY_ENCODE:
        while (sp < sl) {
            gchar c = value[sp] & 0xFF;
            if (c <= 0x7F) {
                // Have at most seven bits
                if (dp >= dl) {
                    // Overflow
                    overflow = true;
                    break;
                }
                dst.value[dp++] = (gbyte) c;
                sp++;
            } else {
                if (dp >= dl) {
                    // Overflow
                    overflow = true;
                    break;
                }
                dst.value[dp++] = (gbyte) (0xc0 | c >> 6);
                dst.value[dp++] = (gbyte) (0x80 | c & 0x3f);
                sp++;
            }
        }

        if (sp < sl && overflow) {
            gint newSize = util::Arrays::newLength(dl, dl + 8, dl >> 1);
            if (newSize != Integer::MAX_VALUE) {
                dst = util::Arrays::copyOf(dst, newSize);
                dl = newSize;
                goto TRY_ENCODE;
            }
        }

        if (dp < dl)
            return util::Arrays::copyOf(dst, dp);
        return dst;
    }

    ByteArray String::encodeUtf16ToUTF8(gint off, gint len) const {
        gint sp = off;
        gint sl = len + off;
        gint dp = 0;
        gint dl = (gint) ((len - off) * 1.3);
        ByteArray dst = ByteArray(dl);

        // Try to encode ASCII characters.
        for (gint i = sp; i < sl; i++) {
            gchar c = StringUtils::readUTF16CharAt(value, i);
            if (c > 0x7F)
                break;
            dst.value[dp++] = (gbyte) c;
            sp++;
        }

        gbool overflow = false;
        // Try
    TRY_ENCODE:
        while (sp < sl) {
            gchar c = StringUtils::readUTF16CharAt(value, sp);
            if (c < 0x80) {
                // Have at most seven bits
                if (dp >= dl) {
                    // Overflow
                    overflow = true;
                    break;
                }
                dst.value[dp++] = (gbyte) c;
                sp++;
            } else if (c < 0x800) {
                // 2 bytes, 11 bits
                if (dp >= dl) {
                    // Overflow
                    overflow = true;
                    break;
                }
                dst.value[dp++] = (gbyte) (0xc0 | c >> 6);
                dst.value[dp++] = (gbyte) (0x80 | c & 0x3f);
                sp++;
            } else if (Character::isSurrogate(c)) {
                // Have a surrogate pair
                gint cp = -1;
                if (Character::isHighSurrogate(c)) {
                    if (sl - sp < 2) {
                        // Underflow
                    } else {
                        gchar lo = StringUtils::readUTF16CharAt(value, sp + 1);
                        if (Character::isLowSurrogate(lo)) {
                            cp = Character::toCodePoint(c, lo);
                        }
                    }
                } else {
                    // is low-Surrogate
                }
                if (cp < 0) {
                    if (dp >= dl) {
                        // Overflow
                        overflow = true;
                        break;
                    }
                    // Malformed input or Unmappable
                    dst.value[dp++] = '?';
                    sp += 1;
                } else {
                    if (dl - dp < 4) {
                        // Overflow
                        overflow = true;
                        break;
                    }
                    dst.value[dp++] = (gbyte) (0xf0 | cp >> 18);
                    dst.value[dp++] = (gbyte) (0x80 | cp >> 12 & 0x3f);
                    dst.value[dp++] = (gbyte) (0x80 | cp >> 6 & 0x3f);
                    dst.value[dp++] = (gbyte) (0x80 | cp & 0x3f);
                    sp += 2;
                }
            } else {
                // 3 bytes, 16 bits
                if (dl - dp < 3) {
                    // Overflow
                    overflow = true;
                    break;
                }
                dst.value[dp++] = (gbyte) (0xe0 | c >> 12);
                dst.value[dp++] = (gbyte) (0x80 | c >> 6 & 0x3f);
                dst.value[dp++] = (gbyte) (0x80 | c & 0x3f);
                sp++;
            }
        }

        if (sp < sl && overflow) {
            gint newSize = util::Arrays::newLength(dl, dl + 8, dl >> 1);
            if (newSize != Integer::MAX_VALUE) {
                dst = util::Arrays::copyOf(dst, newSize);
                dl = newSize;
                goto TRY_ENCODE;
            }
        }

        if (dp < dl)
            return util::Arrays::copyOf(dst, dp);
        return dst;
    }

    ByteArray String::encodeLatin1ToISO8853_1(gint off, gint len, gbool doReplace) const {
        // The Latin-1 string is already encoded to ISO-8859-1
        ByteArray bytes = ByteArray(len - off);
        StringUtils::copyLatin1(value, off, bytes.value, 0, len);
        return bytes;
    }

    ByteArray String::encodeUTF16ToISO8853_1(gint off, gint len, gbool doReplace) const {
        ByteArray dst = ByteArray(len - off);
        gint sp = off;
        gint sl = off + len;
        gint dp = 0;
        gint dl = len - off;

        while (sp < sl) {
            gint i;
            for (i = sp; i < sl; i++) {
                gchar c = StringUtils::readUTF16CharAt(value, i);
                if (c > 0xFF)
                    break;
                dst.value[dp++] = (gbyte) c;
                sp++;
            }

            if (i != sl) {
                if (!doReplace) {
                    IllegalArgumentException("malformed input: offset="_Sl + sp + ", length=1",
                                             charset::UnmappableCharacterException(1))
                            .throws($ftrace());
                }
                gchar c = StringUtils::readUTF16CharAt(value, sp++);
                if (Character::isHighSurrogate(c) &&
                    sp < sl &&
                    Character::isLowSurrogate(StringUtils::readUTF16CharAt(value, sp))) {
                    sp++;
                }
                dst.value[dp++] = '?';
            }
        }

        if (dp < dl)
            return util::Arrays::copyOf(dst, dp);
        return dst;
    }

    ByteArray String::encodeLatin1ToASCII(gint off, gint len) const {
        ByteArray dst = ByteArray(len - off);
        gint dp = 0;
        for (gint i = off; i < len; i++) {
            if (value[i] < 0)
                dst.value[dp++] = '?';
            else
                dst.value[dp++] = value[i];
        }
        return dst;
    }

    ByteArray String::encodeUTF16ToASCII(gint off, gint len) const {
        ByteArray dst = ByteArray(len - off);
        gint dp = 0;
        for (gint i = off; i < len; i++) {
            gchar c = StringUtils::readUTF16CharAt(value, i);
            if (c < 0x80) {
                dst.value[dp++] = (gbyte) c;
                continue;
            }
            if (Character::isHighSurrogate(c) &&
                i + 1 < len &&
                Character::isLowSurrogate(StringUtils::readUTF16CharAt(value, i + 1))) {
                i++;
            }
            dst.value[dp++] = '?';
        }

        if (dp < dst.length())
            return util::Arrays::copyOf(dst, dp);
        return dst;
    }

    void String::initStringFromLiteral(glong str, glong est, gint bpc) {
        if (str == 0 || est < 0)
            IllegalArgumentException("Null pointer"_Sl).throws($ftrace());
        gint len = est > Integer::MAX_VALUE ? Integer::MAX_VALUE : (gint) est;
        switch (bpc) {
            case 1:
                if (!UNSAFE::ANSI_LITERAL) {
                    CharArray ca = decodeUTF8Literal(str, est);
                    BYTES bytes = StringUtils::inflateUTF16ToLatin1(ca.value, 0, ca.length());
                    if (bytes) {
                        value = bytes;
                        coder = LATIN1;
                    } else {
                        bytes = StringUtils::copyOfUTF16(ca.value, 0, ca.length());
                        value = bytes;
                        coder = UTF16;
                    }
                    count = ca.length();
                } else {
                    BYTES bytes = (BYTES) str;
                    value = StringUtils::copyOfLatin1(bytes, 0, len);
                    count = len;
                    coder = LATIN1;
                }
                break;
            case 2: {
                CHARS chars = (CHARS) str;
                // try to inflate utf-16 string to latin-1
                BYTES bytes = StringUtils::inflateUTF16ToLatin1(chars, 0, len);
                if (bytes) {
                    value = bytes;
                    coder = LATIN1;
                } else {
                    bytes = StringUtils::copyOfUTF16(chars, 0, len);
                    value = bytes;
                    coder = UTF16;
                }
                count = len;
                break;
            }
            case 4: {
                INTS ints = (INTS) str;
                // try to inflate utf-32 string to latin-1
                BYTES bytes = StringUtils::inflateUTF32ToLatin1(ints, 0, len);
                if (bytes) {
                    value = bytes;
                    count = len;
                    coder = LATIN1;
                } else {
                    // Calculate the new estimation of length
                    // required to convert utf-32 string to utf-16
                    gint cnt;
                    bytes = StringUtils::copyOfUTF32ToUTF16(ints, 0, len, cnt);
                    value = bytes;
                    count = cnt;
                    coder = UTF16;
                }
                break;
            }
            default:
                CORE_ASSERT(false);
        }
    }

    Object& String::formatter(String const& fmt, gint nArgs) {
        try {
            return formatter(Locale::getDefault(Locale::FORMAT), fmt, nArgs);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Object& String::formatter(Locale const& locale, String const& fmt, gint nArgs) {
        try {
            return UNSAFE::newInstance<ext::Formatter>(locale, fmt, nArgs);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    using ext::Formatter;
    using ext::LiteralFloat;
    using ext::LiteralInt;

    gbool String::formatArg(Object& fmt, gbool arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Boolean& value;

            public:
                CORE_EXPLICIT Arg(Boolean& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new Boolean(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatArg(Object& fmt, gbyte arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Byte& value;

            public:
                CORE_EXPLICIT Arg(Byte& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new Byte(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatUnsignedArg(Object& fmt, gbyte arg) {
        // Unsigned Byte will be converted to Integer
        try {
            return formatArg(fmt, Byte::toUnsignedInt(arg));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::formatArg(Object& fmt, gchar arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Character& value;

            public:
                CORE_EXPLICIT Arg(Character& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new Character(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatArg(Object& fmt, gshort arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Short& value;

            public:
                CORE_EXPLICIT Arg(Short& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new Short(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatUnsignedArg(Object& fmt, gshort arg) {
        // Unsigned Short will be converted to Integer
        try {
            return formatArg(fmt, Short::toUnsignedInt(arg));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::formatArg(Object& fmt, gint arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Integer& value;

            public:
                CORE_EXPLICIT Arg(Integer& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new Integer(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatUnsignedArg(Object& fmt, gint arg) {
        // Unsigned Integer will be converted to Long
        try {
            return formatArg(fmt, Integer::toUnsignedLong(arg));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool String::formatArg(Object& fmt, glong arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Long& value;

            public:
                CORE_EXPLICIT Arg(Long& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new Long(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatUnsignedArg(Object& fmt, glong arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                LiteralInt& value;

            public:
                CORE_EXPLICIT Arg(LiteralInt& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new LiteralInt(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatArg(Object& fmt, gfloat arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Float& value;

            public:
                CORE_EXPLICIT Arg(Float& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new Float(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatArg(Object& fmt, gdouble arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Double& value;

            public:
                CORE_EXPLICIT Arg(Double& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new Double(arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatStringArg(Object& fmt, String arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                String& value;

            public:
                CORE_EXPLICIT Arg(String& value): value(value) {}

                Object& get() const override { return value; }

                ~Arg() override {
                    delete &value;
                }
            };
            try {
                formatter.addArg(*new Arg(*new String((String&&) arg)));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::formatArg(Object& fmt, Object const& arg) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            class Arg final : public Formatter::Arg {
                Object const& value;

            public:
                CORE_EXPLICIT Arg(Object const& value): value(value) {}

                Object& get() const override { return (Object&) value; }
            };
            try {
                formatter.addArg(*new Arg(arg));
            } catch (...) { OutOfMemoryError().throws($ftrace()); }
            return true;
        }
        return false;
    }

    gbool String::finalize(Object& fmt) {
        if (fmt != null) {
            Formatter& formatter = CORE_XCAST(Formatter, fmt);
            try { return formatter.formatArgs(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
        return false;
    }

    Object& String::clone() const {
        try {
            return UNSAFE::newInstance<String>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    class StringComparator final : public virtual Comparator<String> {
    public:
        gint compare(const String& s1, const String& s2) const CORE_NOTHROW override {
            return s1.compareToIgnoreCase(s2);
        }
    };

    Comparator<String> const& String::CASE_INSENSITIVE_ORDER = UNSAFE::newInstance<StringComparator>();
} // core
