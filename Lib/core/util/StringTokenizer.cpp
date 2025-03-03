//
// Created by brunshweeck on 17 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <core/util/StringTokenizer.h>
#include <core/util/NoSuchElementException.h>

namespace core {
    namespace util {
        StringTokenizer::StringTokenizer(String str, String delim, gbool returnDelims) {
            currentPosition = 0;
            newPosition = -1;
            delimsChanged = false;
            maxPosition = str.length();
            StringTokenizer::str = UNSAFE::moveInstance(str);
            delimiters = UNSAFE::moveInstance(delim);
            retDelims = returnDelims;
            setMaxDelimiterCodePoint();
        }

        StringTokenizer::StringTokenizer(String str, String delim)
            : StringTokenizer(UNSAFE::moveInstance(str), UNSAFE::moveInstance(delim), false) {
        }

        StringTokenizer::StringTokenizer(String str)
            : StringTokenizer(UNSAFE::moveInstance(str), "\t\n\r\f"_S, false) {
        }

        gbool StringTokenizer::hasMoreTokens() const {
            /*
             * Temporarily store this position and use it in the following
             * nextToken() method only if the delimiters haven't been changed in
             * that nextToken() invocation.
             */
            newPosition = skipDelimiters(currentPosition);
            return (newPosition < maxPosition);
        }

        String StringTokenizer::nextToken() {
            /*
             * If next position already computed in hasMoreElements() and
             * delimiters have changed between the computation and this invocation,
             * then use the computed value.
             */

            currentPosition = (newPosition >= 0 && !delimsChanged)
                                  ? newPosition
                                  : skipDelimiters(currentPosition);

            /* Reset these anyway */
            delimsChanged = false;
            newPosition = -1;

            if (currentPosition >= maxPosition)
                NoSuchElementException().throws($ftrace());
            gint start = currentPosition;
            currentPosition = scanToken(currentPosition, false);
            return str.subString(start, currentPosition);
        }

        String StringTokenizer::nextToken(String delim) {
            delimiters = UNSAFE::moveInstance(delim);

            /* delimiter string specified, so set the appropriate flag. */
            delimsChanged = true;

            setMaxDelimiterCodePoint();
            return nextToken();
        }

        gint StringTokenizer::countTokens() const {
            gint count = 0;
            gint currPos = currentPosition;
            while (currPos < maxPosition) {
                currPos = skipDelimiters(currPos);
                if (currPos >= maxPosition)
                    break;
                currPos = scanToken(currPos, false);
                count++;
            }
            return count;
        }

        gbool StringTokenizer::goBack() {
            /* Reset these anyway */
            delimsChanged = false;
            newPosition = -1;

            if (currentPosition <= 0)
                return false;
            currentPosition = scanToken(currentPosition, true);
            return true;
        }

        void StringTokenizer::setMaxDelimiterCodePoint() {
            if (delimiters.isEmpty()) {
                maxDelimCodePoint = 0;
                return;
            }

            gint m = 0;
            gint c;
            gint count = 0;
            for (gint i = 0; i < delimiters.length(); i += Character::charCount(c)) {
                c = delimiters.charAt(i);
                if (c >= Character::MIN_HIGH_SURROGATE && c <= Character::MAX_LOW_SURROGATE) {
                    c = delimiters.codePointAt(i);
                    hasSurrogates = true;
                }
                if (m < c)
                    m = c;
                count++;
            }
            maxDelimCodePoint = m;

            if (hasSurrogates) {
                delimiterCodePoints = IntArray(count);
                for (gint i = 0, j = 0; i < count; i++, j += Character::charCount(c)) {
                    c = delimiters.codePointAt(j);
                    delimiterCodePoints[i] = c;
                }
            }
        }

        gint StringTokenizer::skipDelimiters(gint startPos) const {
            gint position = startPos;
            while (!retDelims && position < maxPosition) {
                if (!hasSurrogates) {
                    gchar c = str.charAt(position);
                    if ((c > maxDelimCodePoint) || (delimiters.indexOf(c) < 0))
                        break;
                    position++;
                } else {
                    gint c = str.codePointAt(position);
                    if ((c > maxDelimCodePoint) || !isDelimiter(c)) {
                        break;
                    }
                    position += Character::charCount(c);
                }
            }
            return position;
        }

        gint StringTokenizer::scanToken(gint startPos, gbool toBack) const {
            gint position = startPos;
            if (!toBack) {
                while (position < maxPosition) {
                    if (!hasSurrogates) {
                        gchar c = str.charAt(position);
                        if ((c <= maxDelimCodePoint) && (delimiters.indexOf(c) >= 0))
                            break;
                        position++;
                    } else {
                        gint c = str.codePointAt(position);
                        if ((c <= maxDelimCodePoint) && isDelimiter(c))
                            break;
                        position += Character::charCount(c);
                    }
                }
                if (retDelims && (startPos == position)) {
                    if (!hasSurrogates) {
                        gchar c = str.charAt(position);
                        if ((c <= maxDelimCodePoint) && (delimiters.indexOf(c) >= 0))
                            position++;
                    } else {
                        gint c = str.codePointAt(position);
                        if ((c <= maxDelimCodePoint) && isDelimiter(c))
                            position += Character::charCount(c);
                    }
                }
            } else {
                position -= 1;
                while (position >= 0) {
                    if (!hasSurrogates || position > 0) {
                        gchar c = str.charAt(position);
                        if ((c <= maxDelimCodePoint) && (delimiters.indexOf(c) >= 0))
                            break;
                        position--;
                    } else {
                        gint c = str.codePointAt(position - 1);
                        if(Character::isSupplementary(c)) {
                            if ((c <= maxDelimCodePoint) && isDelimiter(c))
                                break;
                            position -= Character::charCount(c);
                        }else {
                            c = str.charAt(position);
                            if ((c <= maxDelimCodePoint) && (delimiters.indexOf(c) >= 0))
                                break;
                            position--;
                        }
                    }
                }
                if (retDelims && (startPos == position)) {
                    if (!hasSurrogates || position > 0) {
                        gchar c = str.charAt(position);
                        if ((c <= maxDelimCodePoint) && (delimiters.indexOf(c) >= 0))
                            position--;
                    } else {
                        gint c = str.codePointAt(position);
                        if(Character::isSupplementary(c)) {
                            if ((c <= maxDelimCodePoint) && isDelimiter(c))
                                position -= Character::charCount(c);
                        }else {
                            c = str.charAt(position);
                            if ((c <= maxDelimCodePoint) && (delimiters.indexOf(c) >= 0))
                                position--;
                        }
                    }
                }
            }
            return position;
        }

        gbool StringTokenizer::isDelimiter(gint codePoint) const {
            for (gint delimiterCodePoint: delimiterCodePoints) {
                if (delimiterCodePoint == codePoint) {
                    return true;
                }
            }
            return false;
        }
    } // util
} // core
