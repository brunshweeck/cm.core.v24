//
// Created by brunshweeck on 17 sept. 2024.
//

#include "StringJoiner.h"

#include <core/OutOfMemoryError.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace util {
        StringJoiner::StringJoiner(CharSequence const &delimiter)
                : StringJoiner(delimiter, ""_S, ""_S) {
        }

        StringJoiner::StringJoiner(CharSequence const &delimiter, CharSequence const &prefix,
                                   CharSequence const &suffix)
                : delimiter(delimiter.toString()),
                  prefix(prefix.toString()),
                  suffix(suffix.toString()),
                  count(0),
                  capacity(0) {
            CORE_IGNORE(checkAddLength(0, 0));
        }

        StringJoiner &StringJoiner::setEmptyValue(CharSequence const &emptyValue) {
            empty = emptyValue.toString();
            return *this;
        }

        String StringJoiner::toString() const {
            if (count == 0)
                return empty;

            return String::join(delimiter, prefix, suffix, elts, count);
        }

        StringJoiner &StringJoiner::add(CharSequence const &newElement) {
            try {
                String elt = String::valueOf(newElement);
                if (elts.isEmpty()) {
                    elts = StringArray(8);
                } else {
                    if (count == elts.length()) {
                        StringArray sa = StringArray(2 * count);
                        for (int i = 0; i < count; ++i)
                            sa[i] = UNSAFE::moveInstance(elts[i]);

                        elts = UNSAFE::moveInstance(sa);
                    }
                    capacity = checkAddLength(capacity, delimiter.length());
                }
                capacity = checkAddLength(capacity, elt.length());
                elts[count++] = UNSAFE::moveInstance(elt);
                return *this;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        StringJoiner &StringJoiner::merge(StringJoiner const &other) {
            try {
                if (other.count == 0) return *this;

                return add(other.compactElements());
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint StringJoiner::length() const {
            return (count == 0 && !empty.isEmpty()) ? empty.length() : capacity + prefix.length() + suffix.length();
        }

        gint StringJoiner::checkAddLength(gint oldLength, gint inc) const {
            glong newLen = (glong) oldLength + (glong) inc;
            glong tmpLen = newLen + (glong) prefix.length() + (glong) suffix.length();

            if (tmpLen != (gint) tmpLen)
                OutOfMemoryError("Requested array size exceeds limit"_S).throws($ftrace());

            return (gint) newLen;
        }

        String StringJoiner::compactElements() const {
            int sz = count;


            try {
                if (sz > 1)
                    return String::join(delimiter, ""_S, ""_S, elts, sz);

                if (elts.length() > 0)
                    return elts[0];
            } catch (Throwable const &ex) { ex.throws($ftrace()); }

            return String();
        }
    } // util
} // core
