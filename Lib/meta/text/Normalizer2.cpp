//
// Created by brunshweeck on 28/11/24.
//

#include "Normalizer2.h"

#include <core/lang/Array.h>
#include <core/lang/XString.h>
#include <core/misc/Unsafe.h>
#include "Normalizer2.Property.h"

namespace core {
    namespace text {
        String Normalizer2::normalize(CharSequence const& src, Normalizer::Form mode) {
            XString xstr = XString(src.length());
            return normalize(src, xstr, mode).toString();
        }

        String Normalizer2::getDecomposition(gint c) {
            if (Hangul::isHangul(c)) {
                return Hangul::getDecomposition(c);
            }
            glong dec = Property::getDecomposition(c);
            if (dec == 0L) {
                return String();
            }
            gchar info = UNSAFE::getChar(null, dec);
            gint len = info >> 8;
            dec += 2;
            CharArray buffer = CharArray(len);
            for (int i = 0; i < len; ++i) {
                gchar ch = UNSAFE::getChar(null, dec + (i << 1));
                buffer[i] = ch;
            }
            return String(buffer);
        }

        Normalizer2::Decomposition Normalizer2::getDecompositionStyle(gint c) {
            if (Hangul::isHangul(c))
                return CANONICAL;
            glong dec = Property::getDecomposition(c);
            if (dec == 0L)
                return NONE;
            gchar info = UNSAFE::getChar(null, dec);
            return (Decomposition)(info & 0xFF);
        }

        gint Normalizer2::getCombiningClass(gint c) {
            return Property::getProperty(c).getCombiningClass();
        }

        gbool Normalizer2::isNormalized(CharSequence const& src, Normalizer::Form mode) {
            return spanQuickCheck(src, 0, mode) == src.length();
        }

        gint Normalizer2::composePair(gint a, gint b) {
            gint cp = Hangul::composePair(a, b);
            if (cp >= 0)
                return cp;
            glong ligature = Property::getLigature(b);
            if (ligature == 0)
                return -1;
            gchar len = UNSAFE::getChar(null, ligature);
            ligature += 2;
            if (Character::isSupplementary(a)) {
                // class SurrogatePair final {
                //     gchar pair1[2];
                //     gchar pair2[2];
                // };
                // Find the lower bounds for index
                gint start;
                {
                    start = 0;
                    gint end = len;
                    gchar c = 0;
                    while (start < end) {
                        gint mid = (start + end) / 2;
                        gchar hi = UNSAFE::getChar(null, ligature + (mid * 8LL));
                        gchar lo = UNSAFE::getChar(null, ligature + (mid * 8LL) + 2);
                        c = Character::toCodePoint(hi, lo);
                        if (c < a)
                            start = mid;
                        else
                            end = mid;
                    }
                }
                if (start != len) {
                    gchar hi = UNSAFE::getChar(null, ligature + (start * 8LL) + 4);
                    gchar lo = UNSAFE::getChar(null, ligature + (start * 8LL) + 6);
                    return Character::toCodePoint(hi, lo);
                }
            } else {
                // class Pair final {
                //     gchar c1;
                //     gchar c2;
                // };
                // Find the lower bounds for index
                gint start;
                {
                    start = 0;
                    gint end = len;
                    while (start + 1 < end) {
                        gint mid = (start + end) / 2;
                        gchar c = UNSAFE::getChar(null, ligature + (mid * 4LL));
                        if (c <= a)
                            start = mid;
                        else
                            end = mid;
                    }
                }
                if (start != len) {
                    gchar c = UNSAFE::getChar(null, ligature + (start * 4LL) + 2);
                    return c;
                }
            }
            return -1;
        }

        XString& Normalizer2::normalize(CharSequence const& src, XString& dest, Normalizer::Form mode) {
            gint len = src.length();
            gint index = 0;
            while (index < len) {
                if (src.charAt(index) & 0xFF80)
                    break;
                index++;
            }

            // We'll normalize the remaining content.
            gint n = spanQuickCheck(src, index, mode);
            index += n;
            dest.append(src, 0, index);

            if (index >= len)
                return dest;

            XString buffer = XString(len - index);
            decompose(src, index, buffer, mode == Normalizer::NFC || mode == Normalizer::NFKC);

            XString buffer2 = XString(buffer.length());
            canonicalOrder(buffer, 0, buffer2);

            if (mode == Normalizer::NFD || mode == Normalizer::NFKD)
                return dest.append(buffer2);

            buffer.setLength(0);
            compose(buffer2, 0, buffer);
            dest.append(buffer);
            return dest;
        }

        gint Normalizer2::spanQuickCheck(CharSequence const& src, gint offset, Normalizer::Form mode) {
            gint len = src.length();
            while (offset > 0 && Character::isHighSurrogate(src.charAt(offset)))
                offset--;

            gint index = 0;
            gint ccc = 0;
            for (gint sp = offset; sp < len; sp++) {
                gint c = src.charAt(sp);
                if (c < 0x80) {
                    // ASCII characters are stable code points
                    ccc = 0;
                    index = sp;
                    continue;
                }

                if (Character::isSurrogate(c)) {
                    if (Character::isHighSurrogate(c) && sp + 1 < len &&
                        Character::isLowSurrogate(src.charAt(sp + 1))) {
                        sp++;
                        c = Character::toCodePoint(c, src.charAt(sp));
                    } else {
                        // treat surrogate like stable code point
                        ccc = 0;
                        index = sp;
                        continue;
                    }
                }

                Property const& property = Property::getProperty(c);

                if (property.getCombiningClass() < ccc && property.getCombiningClass() > 0)
                    return index;

                gint qc = (property.nfQuickCheck >> (mode << 1)) & 0x3;
                if (qc != YES)
                    return index; // ### can we quickly check MAYBE ?
                ccc = property.getCombiningClass();
                if (ccc == 0)
                    index = sp;
            }

            return index;
        }

        void Normalizer2::decompose(CharSequence const& src, gint offset, XString& buffer, gbool canonical) {
            gint len = src.length();
            Decomposition tag = NONE;

            gint ch = 0;
            for (int i = offset; i < len; ++i) {
                ch = src.charAt(i);
                if (Character::isHighSurrogate(ch) && i + 1 < len) {
                    gint c = src.charAt(i + 1);
                    if (Character::isLowSurrogate(c)) {
                        ch = Character::toCodePoint(ch, c);
                        i++;
                    }
                }

                Property const& property = Property::getProperty(ch);
                if (property.getVersion().compareTo(Version::UNICODE_VERSION) > 0) {
                    buffer.appendCodePoint(ch);
                    continue;
                }

                tag = getDecompositionStyle(ch);
                if (canonical && tag == CANONICAL || !canonical && tag != NONE)
                    buffer.append(getDecomposition(ch));
                else
                    buffer.appendCodePoint(ch);
            }
        }

        void Normalizer2::canonicalOrder(CharSequence const& src, gint offset, XString& buffer) {
            gint len = src.length();

            gint lastCC = 0, lastCC2 = 0;
            gint ch = 0, ch2 = 0;

            gint i, j;
            for (i = offset, j; i < len - 1; i++) {
                j = i + 1;
                ch = src.charAt(i);
                if (Character::isHighSurrogate(ch) && i + 1 < len) {
                    gchar c = src.charAt(i + 1);
                    if (Character::isLowSurrogate(c)) {
                        ch = Character::toCodePoint(ch, c);
                        ++i;
                        j++;

                        if (j >= len) {
                            buffer.appendCodePoint(ch);
                            continue;
                        }
                    }
                }

                lastCC = 0;
                gint idx = buffer.length();
            ADVANCE:
                ch2 = src.charAt(j);
                if (Character::isHighSurrogate(ch2) && j + 1 < len) {
                    gchar c = src.charAt(j + 1);
                    if (Character::isLowSurrogate(c)) {
                        ch2 = Character::toCodePoint(ch2, c);
                        j++;
                    }
                }
                lastCC2 = 0;

                Property const& p2 = Property::getProperty(ch2);
                if (p2.getVersion().compareTo(Version::UNICODE_VERSION) <= 0)
                    lastCC2 = p2.getCombiningClass();

                if (lastCC2 == 0) {
                    i = j - 1;
                    buffer.appendCodePoint(ch);
                    continue;
                }

                Property const& p = Property::getProperty(ch);
                if (p.getVersion().compareTo(Version::UNICODE_VERSION) <= 0)
                    lastCC = p.getCombiningClass();

                if (lastCC > lastCC2) {
                    buffer.append(ch2);
                    buffer.append(ch);
                } else {
                    if (idx < buffer.length())
                        buffer.remove(idx, buffer.length());
                    buffer.appendCodePoint(ch);
                    lastCC = lastCC2;
                    ch = ch2;
                    j++;
                    if (j >= len)
                        break;

                    goto ADVANCE;
                }
            }
            if (i < len)
                buffer.append(src, i, len);
        }

        void Normalizer2::compose(CharSequence const& src, gint offset, XString& buffer) {
            gint len = src.length();
            gint ch = 0, lastCh = -1;
            gint lastCC = 255;
            for (int i = offset; i < len; ++i) {
                ch = src.charAt(i);

                if (Character::isHighSurrogate(ch) && i + 1 < len) {
                    gint c = src.charAt(i + 1);
                    if (Character::isLowSurrogate(c)) {
                        ch = Character::toCodePoint(ch, c);
                        i++;
                    }
                }

                Property const& property = Property::getProperty(ch);
                if (property.getVersion().compareTo(Version::UNICODE_VERSION) > 0) {
                    lastCC = 255;
                    buffer.appendCodePoint(ch);
                    continue;
                }

                gint cc = property.getCombiningClass();
                if (lastCh > 0 || i > 0 && cc > lastCC) {
                    // allowed to form ligature with S
                    gint ligature = lastCh < 0 ? -1 : composePair(lastCh, ch);
                    if (ligature > 0) {
                        lastCh = ligature;
                        continue;
                    }
                    if (lastCh >= 0) {
                        buffer.appendCodePoint(lastCh);
                        buffer.appendCodePoint(ch);
                        lastCh = -1;
                        continue;
                    }
                    buffer.appendCodePoint(ch);
                }
                lastCC = cc;
                lastCh = ch;
            }

            if (lastCh >= 0)
                buffer.appendCodePoint(lastCh);
        }

        gbool Normalizer2::Hangul::isHangul(gint c) {
            return c >= SBase && c <= SBase + SCount;
        }

        String Normalizer2::Hangul::getDecomposition(gint c) {
            if (!isHangul(c))
                return "";
            CharArray buffer = CharArray(3);
            // compute Hangul syllable decomposition as per UAX #15
            gint SIndex = c - SBase;
            buffer[0] = LBase + SIndex / NCount; // L
            buffer[1] = VBase + (SIndex % NCount) / TCount; // V
            buffer[2] = TBase + SIndex % TCount; // T
            return String(buffer, 0, buffer[2] == 0 ? 2 : 3);
        }

        gint Normalizer2::Hangul::composePair(gint a, gint b) {
            if (a >= LBase && a < SBase + SCount) {
                // hangul L-V pair
                gint LIndex = a - LBase;
                if (LIndex < LCount) {
                    gint VIndex = b - VBase;
                    if (VIndex < VCount)
                        return SBase + (LIndex * VCount + VIndex) * TCount;
                }
                // hangul LV-T pair
                gint SIndex = a - SBase;
                if (SIndex < SCount && (SIndex % TCount) == 0) {
                    gint TIndex = b - TBase;
                    if (TIndex < TCount && TIndex > 0)
                        return a + TIndex;
                }
            }
            return -1;
        }
    } // text
} // core
