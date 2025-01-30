//
// Created by bruns on 15/10/2024.
//

#include <core/regex/Matcher.h>

#include "core/io/IOException.h"
#include "core/util/Map.h"
#include "meta/regex/Pattern.ASCII.h"
#include "meta/regex/Pattern.Node.h"
#include "meta/regex/Pattern.Self.h"
#include "core/IndexOutOfBoundsException.h"
#include "core/util/List.h"

namespace core {
    using namespace util;
    using namespace text;
    using namespace function;

    namespace regex {

        Matcher::Matcher() : parent("", 0) {
        }

        Matcher::Matcher(Pattern const &parent, CharSequence const &cs)
                : parent(parent), text(UNSAFE::copyInstance(cs)) {
            gint capturingGroupCount = Math::max(parent.self->capturingGroupCount, 10);
            // Allocate state storage
            groups = IntArray(capturingGroupCount * 2);
            locals = IntArray(parent.self->localCount);
            CORE_IGNORE_DEPRECATIONS(localsPos = Array<HashSet>(parent.self->localTCNCount);)

            // Put fields into initial states
            reset();
        }

        Matcher::Matcher(Matcher const &other) : parent(other.parent) {
            groups = other.groups;
            from = other.from;
            to = other.to;
            lookbehindTo = other.lookbehindTo;
            text = other.text;
            first = other.first;
            last = other.last;
            oldLast = other.oldLast;
            lastAppendPosition = other.lastAppendPosition;
            locals = other.locals;
            localsPos = other.localsPos;
            hitEnd_ = other.hitEnd_;
            requireEnd = other.requireEnd;
            transparentBounds = other.transparentBounds;
            anchoringBounds = other.anchoringBounds;
            namedGroups_ = other.namedGroups_;
        }

        Matcher::Matcher(Matcher&& other) CORE_NOTHROW: parent(UNSAFE::moveInstance(other.parent)) {
            groups = UNSAFE::moveInstance(other.groups);
            UNSAFE::swapValues(from, other.from);
            UNSAFE::swapValues(to, other.to);
            UNSAFE::swapValues(lookbehindTo, other.lookbehindTo);
            text = UNSAFE::moveInstance(other.text);
            UNSAFE::swapValues(first, other.first);
            UNSAFE::swapValues(last, other.last);
            UNSAFE::swapValues(oldLast, other.oldLast);
            UNSAFE::swapValues(lastAppendPosition, other.lastAppendPosition);
            locals = UNSAFE::moveInstance(other.locals);
            UNSAFE::swapValues(localsPos, other.localsPos);
            UNSAFE::swapValues(hitEnd_, other.hitEnd_);
            UNSAFE::swapValues(requireEnd, other.requireEnd);
            UNSAFE::swapValues(transparentBounds, other.transparentBounds);
            UNSAFE::swapValues(anchoringBounds, other.anchoringBounds);
            namedGroups_ = UNSAFE::moveInstance(other.namedGroups_);
            other.modCount++;
        }

        Matcher &Matcher::operator=(Matcher const &other) {
            if (this != &other) {
                modCount++;
                other.modCount++;
                parent = other.parent;
                text = other.text;
                reset();
            }
            return *this;
        }

        Matcher& Matcher::operator=(Matcher&& other) CORE_NOTHROW {
            if (this != &other) {
                groups = UNSAFE::moveInstance(other.groups);
                UNSAFE::swapValues(from, other.from);
                UNSAFE::swapValues(to, other.to);
                UNSAFE::swapValues(lookbehindTo, other.lookbehindTo);
                text = UNSAFE::moveInstance(other.text);
                UNSAFE::swapValues(first, other.first);
                UNSAFE::swapValues(last, other.last);
                UNSAFE::swapValues(oldLast, other.oldLast);
                UNSAFE::swapValues(lastAppendPosition, other.lastAppendPosition);
                locals = UNSAFE::moveInstance(other.locals);
                UNSAFE::swapValues(localsPos, other.localsPos);
                UNSAFE::swapValues(hitEnd_, other.hitEnd_);
                UNSAFE::swapValues(requireEnd, other.requireEnd);
                UNSAFE::swapValues(transparentBounds, other.transparentBounds);
                UNSAFE::swapValues(anchoringBounds, other.anchoringBounds);
                namedGroups_ = UNSAFE::moveInstance(other.namedGroups_);
                other.modCount++;
            }
            return *this;
        }

        Pattern Matcher::getPattern() const {
            return parent;
        }

        MatchResult &Matcher::toMatchResult() const {
            gint minStart;
            String capturedText;
            if (hasMatch()) {
                minStart = this->minStart();
                if (Class<String>::hasInstance(text.get()))
                    capturedText = CORE_XCAST(String const, text.get()).subString(minStart, maxEnd());
                else
                    capturedText = text.get().subSequence(minStart, maxEnd()).toString();
            } else {
                minStart = -1;
                capturedText = {};
            }

            class ImmutableMatchResult : public MatchResult {
                gint first;
                gint last;
                gint groupCount_;
                IntArray groups;
                String text;
                Map<String, Integer> const &namedGroups_;
                gint minStart;

            public:
                CORE_EXPLICIT ImmutableMatchResult(gint first, gint last, gint groupCount,
                                                   IntArray groups, String text,
                                                   Map<String, Integer> const &namedGroups, gint minStart)
                        : first(first),
                          last(last),
                          groupCount_(groupCount),
                          groups(UNSAFE::moveInstance(groups)),
                          text(UNSAFE::moveInstance(text)),
                          namedGroups_(namedGroups),
                          minStart(minStart) {
                }

                gint start() const override {
                    try { checkMatch(); } catch (Throwable const &ex) { ex.throws($xtrace()); }
                    return first;
                }

                gint start(gint group) const override {
                    try {
                        checkMatch();
                        checkGroup(group);
                    } catch (Throwable const &ex) { ex.throws($xtrace()); }
                    return groups[group << 1];
                }

                gint end() const override {
                    try { checkMatch(); } catch (Throwable const &ex) { ex.throws($xtrace()); }
                    return last;
                }

                gint end(gint group) const override {
                    try {
                        checkMatch();
                        checkGroup(group);
                    } catch (Throwable const &ex) { ex.throws($xtrace()); }
                    return groups[(group << 1) - 1];
                }

                String group() const override {
                    try {
                        checkMatch();
                        return group(0);
                    } catch (Throwable const &ex) { ex.throws($xtrace()); }
                }

                String group(gint group) const override {
                    try {
                        checkMatch();
                        checkGroup(group);
                    } catch (Throwable const &ex) { ex.throws($xtrace()); }

                    if ((groups[group * 2] == -1) || (groups[group * 2 + 1] == -1))
                        IllegalArgumentException().throws($xtrace());
                    return text.subString(groups[group * 2] - minStart, groups[group * 2 + 1] - minStart);
                }

                gint groupCount() const override {
                    return groupCount_;
                }

                util::Map<String, Integer> const &namedGroups() const override {
                    return namedGroups_;
                }

                gbool hasMatch() const override {
                    return first >= 0;
                }

            private:
                void checkGroup(gint group) const {
                    if (group < 0 || group > groupCount_)
                        IndexOutOfBoundsException("No group "_S + group).throws($xtrace());
                }

                void checkMatch() const {
                    if (!hasMatch())
                        IllegalStateException("No match found").throws($xtrace());
                }
            };


            return UNSAFE::newInstance<ImmutableMatchResult>(first, last, groupCount(), groups, capturedText,
                                                             namedGroups(), minStart);
        }

        Matcher &Matcher::setPattern(Pattern const &newPattern) {
            parent = newPattern;
            namedGroups_ = {};

            // Reallocate state storage
            groups = IntArray(newPattern.self->capturingGroupCount * 2);
            locals = IntArray(newPattern.self->localCount);
            for (gint i = 0; i < groups.length(); i++)
                groups[i] = -1;
            for (gint i = 0; i < locals.length(); i++)
                locals[i] = -1;
            CORE_IGNORE_DEPRECATIONS(localsPos = Array<HashSet>(parent.self->localTCNCount);)
            modCount++;
            return *this;
        }

        Matcher &Matcher::reset() {
            first = -1;
            last = 0;
            oldLast = -1;
            for (gint i = 0; i < groups.length(); i++)
                groups[i] = -1;
            for (gint i = 0; i < locals.length(); i++)
                locals[i] = -1;
            for (gint i = 0; i < localsPos.length(); i++) {
                if (localsPos[i] != null)
                    localsPos[i].clear();
            }
            lastAppendPosition = 0;
            from = 0;
            to = getTextLength();
            modCount++;
            return *this;
        }

        Matcher &Matcher::reset(CharSequence const &input) {
            text = input;
            return reset();
        }

        gint Matcher::start() const {
            try { checkMatch(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            return first;
        }

        gint Matcher::start(gint group) const {
            try { checkMatch(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            checkGroup(group);
            return groups[group * 2];
        }

        gint Matcher::start(String const &name) const {
            return groups[getMatchedGroupIndex(name) * 2];
        }

        gint Matcher::end() const {

            try { checkMatch(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            return last;
        }

        gint Matcher::end(gint group) const {
            try { checkMatch(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            checkGroup(group);
            return groups[group * 2 + 1];
        }

        gint Matcher::end(String const &name) const {
            return groups[getMatchedGroupIndex(name) * 2 + 1];
        }

        String Matcher::group() const {
            return group(0);
        }

        String Matcher::group(gint group) const {
            try { checkMatch(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            checkGroup(group);
            if ((groups[group * 2] == -1) || (groups[group * 2 + 1] == -1))
                return ""_S;
            return subSequence(groups[group * 2], groups[group * 2 + 1]).toString();
        }

        String Matcher::group(String const &name) const {
            gint group;
            try { group = getMatchedGroupIndex(name); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            if ((groups[group * 2] == -1) || (groups[group * 2 + 1] == -1))
                return ""_S;
            return subSequence(groups[group * 2], groups[group * 2 + 1]).toString();
        }

        gint Matcher::groupCount() const {
            return parent.self->capturingGroupCount - 1;
        }

        gbool Matcher::matches() {
            return match(from, ENDANCHOR);
        }

        gbool Matcher::find() {
            gint nextSearchIndex = last;
            if (nextSearchIndex == first)
                nextSearchIndex++;

            // If next search starts before region, start it at region
            if (nextSearchIndex < from)
                nextSearchIndex = from;

            // If next search starts beyond region then it fails
            if (nextSearchIndex > to) {
                for (gint i = 0; i < groups.length(); i++)
                    groups[i] = -1;
                return false;
            }
            return search(nextSearchIndex);
        }

        gbool Matcher::find(gint start) {
            gint limit = getTextLength();
            if ((start < 0) || (start > limit))
                IndexOutOfBoundsException("Illegal start index"_S).throws($ftrace());
            reset();
            return search(start);
        }

        gbool Matcher::lookingAt() {
            return match(from, NOANCHOR);
        }

        String Matcher::quoteReplacement(String const &s) {
            if ((s.indexOf('\\') == -1) && (s.indexOf('$') == -1))
                return s;
            XString sb;
            for (gint i = 0; i < s.length(); i++) {
                gchar c = s.charAt(i);
                if (c == '\\' || c == '$') {
                    sb.append('\\');
                }
                sb.append(c);
            }
            return sb.toString();
        }

        Matcher &Matcher::appendReplacement(XString &sb, String const &replacement) {
            try { checkMatch(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            gint curLen = sb.length();
            try {
                // Append the intervening text
                sb.append(text.get(), lastAppendPosition, first);
                // Append the match substitution
                appendExpandedReplacement(sb, replacement);
            } catch (IllegalArgumentException const &ex) {
                sb.setLength(curLen);
                ex.throws($ftrace());
            }
            lastAppendPosition = last;
            modCount++;
            return *this;
        }

        XString &Matcher::appendTail(XString &sb) {
            sb.append(text.get(), lastAppendPosition, getTextLength());
            return sb;
        }

        String Matcher::replaceAll(String const &replacement) {
            reset();
            gbool result = find();
            if (result) {
                XString sb;
                do {
                    appendReplacement(sb, replacement);
                    result = find();
                } while (result);
                appendTail(sb);
                return sb.toString();
            }
            return text.toString();
        }

        String Matcher::replaceAll(function::Function<MatchResult, String> const &replacer) {
            reset();
            gbool result = find();
            if (result) {
                XString sb;
                do {
                    gint ec = modCount;
                    String replacement = replacer.apply(*this);
                    if (ec != modCount)
                        util::ConcurrentModificationException().throws($ftrace());
                    appendReplacement(sb, replacement);
                    result = find();
                } while (result);
                appendTail(sb);
                return sb.toString();
            }
            return text.toString();
        }

        String Matcher::replaceFirst(String const &replacement) {
            reset();
            if (!find())
                return text.toString();
            XString sb;
            appendReplacement(sb, replacement);
            appendTail(sb);
            return sb.toString();
        }

        String Matcher::replaceFirst(function::Function<MatchResult, String> const &replacer) {
            reset();
            if (!find())
                return text.toString();
            XString sb;
            gint ec = modCount;
            String replacement = replacer.apply(*this);
            if (ec != modCount)
                util::ConcurrentModificationException().throws($ftrace());
            appendReplacement(sb, replacement);
            appendTail(sb);
            return sb.toString();
        }

        Matcher &Matcher::region(gint start, gint end) {
            if ((start < 0) || (start > getTextLength()))
                IndexOutOfBoundsException("start").throws($ftrace());
            if ((end < 0) || (end > getTextLength()))
                IndexOutOfBoundsException("end").throws($ftrace());
            if (start > end)
                IndexOutOfBoundsException("start > end").throws($ftrace());
            reset();
            from = start;
            to = end;
            return *this;
        }

        gint Matcher::regionStart() const {
            return from;
        }

        gint Matcher::regionEnd() const {
            return to;
        }

        gbool Matcher::hasTransparentBounds() const {
            return transparentBounds;
        }

        Matcher &Matcher::useTransparentBounds(gbool b) {
            transparentBounds = b;
            return *this;
        }

        gbool Matcher::hasAnchoringBounds() const {
            return anchoringBounds;
        }

        Matcher &Matcher::useAnchoringBounds(gbool b) {
            anchoringBounds = b;
            return *this;
        }

        String Matcher::toString() const {
            XString sb;
            sb.append(Object::toString())
                    .append("[pattern=").append(parent)
                    .append(" region=")
                    .append(regionStart()).append(',').append(regionEnd())
                    .append(" lastmatch=");
            if ((first >= 0) && (group() != null)) {
                sb.append(group());
            }
            sb.append(']');
            return sb.toString();
        }

        gbool Matcher::hitEnd() const {
            return hitEnd_;
        }

        gbool Matcher::requiresEnd() const {
            return requireEnd;
        }

        Map<String, Integer> &Matcher::namedGroups() const {
            if (namedGroups_.isEmpty()) {
                return (namedGroups_ = parent.namedGroups()).get();
            }
            return namedGroups_.get();
        }

        gbool Matcher::hasMatch() const {
            return first >= 0;
        }

        gbool Matcher::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<Matcher>::hasInstance(o))
                return false;
            Matcher const &rhs = CORE_XCAST(Matcher const, o);
            return parent == rhs.parent
                   && groups == rhs.groups
                   && from == rhs.from
                   && to == rhs.to
                   && lookbehindTo == rhs.lookbehindTo
                   && text == rhs.text
                   && acceptMode == rhs.acceptMode
                   && first == rhs.first
                   && last == rhs.last
                   && oldLast == rhs.oldLast
                   && lastAppendPosition == rhs.lastAppendPosition
                   && locals == rhs.locals
                   && hitEnd_ == rhs.hitEnd_
                   && requireEnd == rhs.requireEnd
                   && transparentBounds == rhs.transparentBounds
                   && anchoringBounds == rhs.anchoringBounds
                   && modCount == rhs.modCount
                   && localsPos == rhs.localsPos
                   && namedGroups() == rhs.namedGroups();
        }

        namespace {
            class Holder final : public Object {
            };
        }

        Object &Matcher::clone() const {
            try {
                return UNSAFE::newInstance<Matcher>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Matcher::minStart() const {
            gint r = text.get().length();
            for (gint group = 0; group <= groupCount(); ++group) {
                gint start = groups[group * 2];
                if (start >= 0) {
                    r = Math::min(r, start);
                }
            }
            return r;
        }

        gint Matcher::maxEnd() const {
            gint r = 0;
            for (gint group = 0; group <= groupCount(); ++group) {
                gint end = groups[group * 2 + 1];
                if (end >= 0) {
                    r = Math::max(r, end);
                }
            }
            return r;
        }

        void Matcher::appendExpandedReplacement(Appendable &app, String const &replacement) const {
            try {
                gint cursor = 0;
                while (cursor < replacement.length()) {
                    gchar nextChar = replacement.charAt(cursor);
                    if (nextChar == '\\') {
                        cursor++;
                        if (cursor == replacement.length())
                            IllegalArgumentException("character to be escaped is missing"_S)
                                    .throws($ftrace());
                        nextChar = replacement.charAt(cursor);
                        app.append(nextChar);
                        cursor++;
                    } else if (nextChar == '$') {
                        // Skip past $
                        cursor++;
                        // Throw IAE if this "$" is the last character in replacement
                        if (cursor == replacement.length())
                            IllegalArgumentException("Illegal group reference: group index is missing"_S)
                                    .throws($ftrace());
                        nextChar = replacement.charAt(cursor);
                        gint refNum = -1;
                        if (nextChar == '{') {
                            cursor++;
                            gint begin = cursor;
                            while (cursor < replacement.length()) {
                                nextChar = replacement.charAt(cursor);
                                if (Pattern::ASCII::isLower(nextChar) ||
                                    Pattern::ASCII::isUpper(nextChar) ||
                                    Pattern::ASCII::isDigit(nextChar)) {
                                    cursor++;
                                } else {
                                    break;
                                }
                            }
                            if (begin == cursor)
                                IllegalArgumentException("named capturing group has 0 length name")
                                        .throws($ftrace());
                            if (nextChar != '}')
                                IllegalArgumentException("named capturing group is missing trailing '}'"_S)
                                        .throws($ftrace());
                            String gname = replacement.subString(begin, cursor);
                            if (Pattern::ASCII::isDigit(gname.charAt(0)))
                                IllegalArgumentException("capturing group name {"_S + gname
                                                         + "} starts with digit character"_S)
                                        .throws($ftrace());
                            Object &number = namedGroups().getOrNull(gname);
                            if (number == null)
                                IllegalArgumentException("No group with name {"_S + gname + "}")
                                        .throws($ftrace());
                            refNum = CORE_XCAST(Integer, number);
                            cursor++;
                        } else {
                            // The first number is always a group
                            refNum = nextChar - '0';
                            if ((refNum < 0) || (refNum > 9))
                                IllegalArgumentException("Illegal group reference"_S)
                                        .throws($ftrace());
                            cursor++;
                            // Capture the largest legal group string
                            gbool done = false;
                            while (!done) {
                                if (cursor >= replacement.length()) {
                                    break;
                                }
                                gint nextDigit = replacement.charAt(cursor) - '0';
                                if ((nextDigit < 0) || (nextDigit > 9)) {
                                    // not a number
                                    break;
                                }
                                gint newRefNum = (refNum * 10) + nextDigit;
                                if (groupCount() < newRefNum) {
                                    done = true;
                                } else {
                                    refNum = newRefNum;
                                    cursor++;
                                }
                            }
                        }
                        // Append group
                        if (start(refNum) != -1 && end(refNum) != -1)
                            app.append(text.get(), start(refNum), end(refNum));
                    } else {
                        app.append(nextChar);
                        cursor++;
                    }
                }
            } catch (io::IOException const &ex) {
                // cannot happen on XString
                AssertionError(ex.message()).throws($ftrace());
            }
        }

        gbool Matcher::search(gint from) {
            hitEnd_ = false;
            requireEnd = false;
            from = from < 0 ? 0 : from;
            first = from;
            oldLast = oldLast < 0 ? from : oldLast;
            for (gint i = 0; i < groups.length(); i++)
                groups[i] = -1;
            for (gint i = 0; i < localsPos.length(); i++) {
                if (localsPos[i] != null)
                    localsPos[i].clear();
            }
            acceptMode = NOANCHOR;
            gbool result = parent.self->root.get().match(*this, from, text.get());
            if (!result)
                first = -1;
            oldLast = last;
            modCount++;
            return result;
        }

        gbool Matcher::match(gint from, gint anchor) {
            hitEnd_ = false;
            requireEnd = false;
            from = from < 0 ? 0 : from;
            first = from;
            oldLast = oldLast < 0 ? from : oldLast;
            for (gint i = 0; i < groups.length(); i++)
                groups[i] = -1;
            for (gint i = 0; i < localsPos.length(); i++) {
                if (localsPos[i] != null)
                    localsPos[i].clear();
            }
            acceptMode = anchor;
            gbool result = parent.self->matchRoot.get().match(*this, from, text.get());
            if (!result)
                first = -1;
            oldLast = last;
            modCount++;
            return result;
        }

        gint Matcher::getTextLength() const {
            return text.get().length();
        }

        CharSequence &Matcher::subSequence(gint beginIndex, gint endIndex) const {
            return text.get().subSequence(beginIndex, endIndex);
        }

        gchar Matcher::charAt(gint i) const {
            return text.get().charAt(i);
        }

        gint Matcher::getMatchedGroupIndex(String const &name) const {
            try { checkMatch(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            Object &number = namedGroups().getOrNull(name);
            if (number == null)
                IllegalArgumentException("No group with name <"_S + name + ">")
                        .throws($ftrace());
            return CORE_XCAST(Integer, number);
        }

        void Matcher::checkGroup(gint group) const {
            if (group < 0 || group > groupCount())
                IndexOutOfBoundsException("No group "_S + group).throws($ftrace());
        }

        void Matcher::checkMatch() const {
            if (!hasMatch())
                IllegalStateException("No match found"_S).throws($ftrace());
        }

        Matcher::HashSet::HashSet() {
            entries = IntArray(16 << 1, -1); // initCapacity = 16;
            hashes = IntArray((16 / 2) | 1, -1); // odd -> fewer collisions
        }

        gbool Matcher::HashSet::contains(gint i) {
            gint h = hashes[i % hashes.length()];
            while (h != -1) {
                if (entries[h] == i)
                    return true;
                h = entries[h + 1];
            }
            return false;
        }

        void Matcher::HashSet::add(gint i) {
            gint h0 = i % hashes.length();
            gint next = hashes[h0];
            //  if invoker guarantees contains(i) checked before add(i)
            //  the following check is not needed.
            gint next0 = next;
            while (next0 != -1) {
                if (entries[next0] == i)
                    return;
                next0 = entries[next0 + 1];
            }
            hashes[h0] = pos;
            entries[pos++] = i;
            entries[pos++] = next;
            if (pos == entries.length())
                expand();
        }

        void Matcher::HashSet::clear() {
            Arrays::fill(entries, -1);
            Arrays::fill(hashes, -1);
            pos = 0;
        }

        void Matcher::HashSet::expand() {
            IntArray &old = entries;
            IntArray es = IntArray(old.length() << 1);
            gint hlen = (old.length() / 2) | 1;
            IntArray hs = IntArray(hlen);
            Arrays::fill(es, -1);
            Arrays::fill(hs, -1);
            for (gint n = 0; n < pos;) {
                // re-hashing
                gint i = old[n];
                gint hsh = i % hlen;
                gint next = hs[hsh];
                hs[hsh] = n;
                es[n++] = i;
                es[n++] = next;
            }
            entries = es;
            hashes = hs;
        }

        Object &Matcher::HashSet::clone() const {
            return Object::clone();
        }
    } // regex
} // core
