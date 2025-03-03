//
// Created by brunshweeck on 20/11/24.
//

#include <core/regex/Matcher.h>
#include <core/text/Normalizer.h>
#include <meta/regex/Pattern.ASCII.h>
#include <meta/regex/Pattern.Predicates.h>
#include <meta/regex/Pattern.Grapheme.h>
#include <meta/regex/Pattern.Node.h>
#include <meta/regex/Pattern.Self.h>

namespace core {
    namespace regex {
        Pattern::Node& Pattern::accept = UNSAFE::newInstance<Node>();
        Pattern::Node& Pattern::lastAccept = UNSAFE::newInstance<LastNode>();

        static CORE_FAST gint MAX_REPS = 0x7FFFFFFF;

        Pattern::TreeInfo::TreeInfo() {
            reset();
        }

        void Pattern::TreeInfo::reset() {
            minLength = 0;
            maxLength = 0;
            maxValid = true;
            deterministic = true;
        }

        Object& Pattern::Analyzer::clone() const {
            return (Object&)*this;
        }

        Pattern::Node::Node() {
            next.value = &Pattern::accept;
        }

        gbool Pattern::Node::match(Matcher& matcher, gint i, CharSequence const& seq) {
            matcher.last = i;
            matcher.groups[0] = matcher.first;
            matcher.groups[1] = matcher.last;
            return true;
        }

        gbool Pattern::Node::study(TreeInfo& info) {
            if (next.isPresent())
                try { return next.get().study(info); } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return info.deterministic;
        }

        gbool Pattern::LastNode::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (matcher.acceptMode == Matcher::ENDANCHOR && i != matcher.to)
                return false;
            matcher.last = i;
            matcher.groups[0] = matcher.first;
            matcher.groups[1] = matcher.last;
            return true;
        }

        Pattern::Start::Start(const Optional<Node>& node) {
            next = node;
            TreeInfo info;
            try {
                if (next.isPresent())
                    next.get().study(info);
                minLength = info.minLength;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool Pattern::Start::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (i > matcher.to - minLength) {
                matcher.hitEnd_ = true;
                return false;
            }
            gint guard = matcher.to - minLength;
            for (; i < guard; i++) {
                if (next.isPresent() && next.get().match(matcher, i, seq)) {
                    matcher.first = i;
                    matcher.groups[0] = matcher.first;
                    matcher.groups[1] = matcher.last;
                    return true;
                }
            }
            matcher.hitEnd_ = true;
            return false;
        }

        gbool Pattern::Start::study(TreeInfo& info) {
            if (next.isPresent())
                next.get().study(info);
            info.deterministic = false;
            info.maxValid = false;
            return false;
        }

        Pattern::StartS::StartS(const Optional<Node>& node): Start(node) {}

        gbool Pattern::StartS::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (i > matcher.to - minLength) {
                matcher.hitEnd_ = true;
                return false;
            }
            gint guard = matcher.to - minLength;
            while (i < guard) {
                if (next.isPresent() && next.get().match(matcher, i, seq)) {
                    matcher.first = i;
                    matcher.groups[0] = matcher.first;
                    matcher.groups[1] = matcher.last;
                    return true;
                }
                if (i == guard)
                    break;
                if (Character::isHighSurrogate(seq.charAt(i++)))
                    if (i < seq.length() && Character::isLowSurrogate(seq.charAt(i)))
                        i++;
            }
            matcher.hitEnd_ = true;
            return false;
        }

        gbool Pattern::Begin::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint fromIndex = (matcher.anchoringBounds)
                                 ? matcher.from
                                 : 0;
            if (i == fromIndex && next.isPresent() && next.get().match(matcher, i, seq)) {
                matcher.first = i;
                matcher.groups[0] = matcher.first;
                matcher.groups[1] = matcher.last;
                return true;
            }

            return false;
        }

        gbool Pattern::End::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint endIndex = (matcher.anchoringBounds)
                                ? matcher.to
                                : matcher.getTextLength();
            if (i == endIndex) {
                matcher.hitEnd_ = true;
                return next.isPresent() && next.get().match(matcher, i, seq);
            }
            return false;
        }

        gbool Pattern::Caret::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint startIndex = matcher.from;
            gint endIndex = matcher.to;
            if (!matcher.anchoringBounds) {
                startIndex = 0;
                endIndex = matcher.getTextLength();
            }
            // Perl does not match ^ at end of input even after newline
            if (i == endIndex) {
                matcher.hitEnd_ = true;
                return false;
            }
            if (i > startIndex) {
                gchar ch = seq.charAt(i - 1);
                if (ch != '\n' && ch != '\r' && (ch | 1) != u'\u2029' && ch != u'\u0085')
                    return false;
                // Should treat /r/n as one newline
                if (ch == '\r' && seq.charAt(i) == '\n')
                    return false;
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::UnixCaret::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint startIndex = matcher.from;
            gint endIndex = matcher.to;
            if (!matcher.anchoringBounds) {
                startIndex = 0;
                endIndex = matcher.getTextLength();
            }
            // Perl does not match ^ at end of input even after newline
            if (i == endIndex) {
                matcher.hitEnd_ = true;
                return false;
            }
            if (i > startIndex) {
                gchar ch = seq.charAt(i - 1);
                if (ch != '\n')
                    return false;
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::LastMatch::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (i != matcher.oldLast)
                return false;
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        Pattern::Dollar::Dollar(gbool multiline): multiline(multiline) {}

        gbool Pattern::Dollar::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint endIndex = (matcher.anchoringBounds)
                                ? matcher.to
                                : matcher.getTextLength();
            if (!multiline) {
                if (i < endIndex - 2)
                    return false;
                if (i == endIndex - 2) {
                    gchar ch = seq.charAt(i);
                    if (ch != '\r')
                        return false;
                    ch = seq.charAt(i + 1);
                    if (ch != '\n')
                        return false;
                }
            }
            // Matches before any line terminator; also matches at the
            // end of input
            // Before line terminator:
            // If multiline, we match here no matter what
            // If not multiline, fall through so that the end
            // is marked as hit; this must be a /r/n or a /n
            // at the very end so the end was hit; more input
            // could make this not match here
            if (i < endIndex) {
                gchar ch = seq.charAt(i);
                if (ch == '\n') {
                    // No match between \r\n
                    if (i > 0 && seq.charAt(i - 1) == '\r')
                        return false;
                    if (multiline)
                        return next.isPresent() && next.get().match(matcher, i, seq);
                } else
                    return false;
            }
            // Matched at current end so hit end
            matcher.hitEnd_ = true;
            // If a $ matches because of end of input, then more input
            // could cause it to fail!
            matcher.requireEnd = true;
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::Dollar::study(TreeInfo& info) {
            if (next.isPresent())
                next.get().study(info);
            return info.deterministic;
        }

        Pattern::UnixDollar::UnixDollar(gbool multiline): multiline(multiline) {}

        gbool Pattern::UnixDollar::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint endIndex = (matcher.anchoringBounds)
                                ? matcher.to
                                : matcher.getTextLength();
            if (i < endIndex) {
                gchar ch = seq.charAt(i);
                if (ch == '\n') {
                    // If not multiline, then only possible to
                    // match at very end or one before end
                    if (!multiline && i != endIndex - 1)
                        return false;
                    // If multiline return next.isPresent() && next.get().match without setting
                    // matcher.hitEnd
                    if (multiline)
                        return next.isPresent() && next.get().match(matcher, i, seq);
                } else
                    return false;
            }
            // Matching because at the end or 1 before the end;
            // more input could change this so set hitEnd
            matcher.hitEnd_ = true;
            // If a $ matches because of end of input, then more input
            // could cause it to fail!
            matcher.requireEnd = true;
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::UnixDollar::study(TreeInfo& info) {
            if (next.isPresent())
                next.get().study(info);
            return info.deterministic;
        }

        gbool Pattern::LineEnding::match(Matcher& matcher, gint i, const CharSequence& seq) {
            // (u+000Du+000A|[u+000Au+000Bu+000Cu+000Du+0085u+2028u+2029])
            if (i < matcher.to) {
                gint ch = seq.charAt(i);
                if (ch == 0x0A || ch == 0x0B || ch == 0x0C ||
                    ch == 0x85 || ch == 0x2028 || ch == 0x2029)
                    return next.isPresent() && next.get().match(matcher, i + 1, seq);
                if (ch == 0x0D) {
                    i++;
                    if (i < matcher.to) {
                        if (seq.charAt(i) == 0x0A && next.isPresent() &&
                            next.get().match(matcher, i + 1, seq)) {
                            return true;
                        }
                    } else
                        matcher.hitEnd_ = true;
                    return next.isPresent() && next.get().match(matcher, i, seq);
                }
            } else
                matcher.hitEnd_ = true;
            return false;
        }

        gbool Pattern::LineEnding::study(TreeInfo& info) {
            info.minLength++;
            info.maxLength += 2;
            return next.isPresent() && next.get().study(info);
        }

        Pattern::CharProperty::CharProperty(const Optional<CharPredicate>& predicate): predicate(predicate) {}

        gbool Pattern::CharProperty::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (i < matcher.to) {
                gint ch = Character::codePointAt(seq, i);
                i += Character::charCount(ch);
                if (i <= matcher.to)
                    return predicate.isPresent() && predicate.get().test(ch) &&
                            next.isPresent() && next.get().match(matcher, i, seq);
            }
            matcher.hitEnd_ = true;
            return false;
        }

        gbool Pattern::CharProperty::study(TreeInfo& info) {
            info.minLength++;
            info.maxLength++;
            return next.isPresent() && next.get().study(info);
        }

        Pattern::BmpCharProperty::BmpCharProperty(const Optional<CharPredicate>& predicate): CharProperty(predicate) {}

        gbool Pattern::BmpCharProperty::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (i < matcher.to) {
                return predicate.isPresent() && predicate.get().test(seq.charAt(i)) &&
                        next.isPresent() && next.get().match(matcher, i + 1, seq);
            }
            matcher.hitEnd_ = true;
            return false;
        }

        Pattern::NFCCharProperty::NFCCharProperty(const Optional<CharPredicate>& predicate): predicate(predicate) {}

        gbool Pattern::NFCCharProperty::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (i < matcher.to) {
                gint ch0 = Character::codePointAt(seq, i);
                gint n = Character::charCount(ch0);
                gint j = Grapheme::nextBoundary(seq, i, matcher.to);
                if (i + n == j) {
                    // single cp grapheme, assume nfc
                    if (predicate.isPresent() && predicate.get().test(ch0))
                        return next.isPresent() && next.get().match(matcher, j, seq);
                } else {
                    while (i + n < j) {
                        String nfc = Normalizer::normalize(
                            seq.toString().subSequence(i, j), Normalizer::Form::NFC);
                        if (nfc.codePointCount(0, nfc.length()) == 1) {
                            if (predicate.isPresent() && predicate.get().test(nfc.codePointAt(0)) &&
                                next.isPresent() && next.get().match(matcher, j, seq)) {
                                return true;
                            }
                        }

                        ch0 = Character::codePointBefore(seq, j);
                        j -= Character::charCount(ch0);
                    }
                }
                if (j < matcher.to)
                    return false;
            } else {
                matcher.hitEnd_ = true;
            }
            return false;
        }

        gbool Pattern::NFCCharProperty::study(TreeInfo& info) {
            info.minLength++;
            info.deterministic = false;
            return next.isPresent() && next.get().study(info);
        }

        gbool Pattern::XGrapheme::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (i < matcher.to) {
                i = Grapheme::nextBoundary(seq, i, matcher.to);
                return next.isPresent() && next.get().match(matcher, i, seq);
            }
            matcher.hitEnd_ = true;
            return false;
        }

        gbool Pattern::XGrapheme::study(TreeInfo& info) {
            info.minLength++;
            info.deterministic = false;
            return next.isPresent() && next.get().study(info);
        }

        gbool Pattern::GraphemeBound::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint startIndex = matcher.from;
            gint endIndex = matcher.to;
            if (matcher.transparentBounds) {
                startIndex = 0;
                endIndex = matcher.getTextLength();
            }
            if (i == startIndex) {
                // continue with return below
            } else if (i < endIndex) {
                if (Character::isSurrogatePair(seq.charAt(i - 1), seq.charAt(i))) {
                    return false;
                }
                if (Grapheme::nextBoundary(seq, matcher.last, endIndex) > i) {
                    return false;
                }
            } else {
                matcher.hitEnd_ = true;
                matcher.requireEnd = true;
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        Pattern::SliceNode::SliceNode(IntArray buffer): buffer(UNSAFE::moveInstance(buffer)) {}

        gbool Pattern::SliceNode::study(TreeInfo& info) {
            info.minLength += buffer.length();
            info.maxLength += buffer.length();
            return next.isPresent() && next.get().study(info);
        }

        Pattern::Slice::Slice(const IntArray& buffer): SliceNode(buffer) {}

        gbool Pattern::Slice::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint len = buffer.length();
            for (gint j = 0; j < len; j++) {
                if ((i + j) >= matcher.to) {
                    matcher.hitEnd_ = true;
                    return false;
                }
                if (buffer[j] != seq.charAt(i + j))
                    return false;
            }
            return next.isPresent() && next.get().match(matcher, i + len, seq);
        }

        Pattern::SliceI::SliceI(const IntArray& buffer): SliceNode(buffer) {}

        gbool Pattern::SliceI::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint len = buffer.length();
            for (gint j = 0; j < len; j++) {
                if ((i + j) >= matcher.to) {
                    matcher.hitEnd_ = true;
                    return false;
                }
                gint c = seq.charAt(i + j);
                if (buffer[j] != c &&
                    buffer[j] != ASCII::toLower(c))
                    return false;
            }
            return next.isPresent() && next.get().match(matcher, i + len, seq);
        }

        Pattern::SliceU::SliceU(const IntArray& buffer): SliceNode(buffer) {}

        gbool Pattern::SliceU::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint len = buffer.length();
            for (gint j = 0; j < len; j++) {
                if ((i + j) >= matcher.to) {
                    matcher.hitEnd_ = true;
                    return false;
                }
                gint c = seq.charAt(i + j);
                if (buffer[j] != c &&
                    buffer[j] != Character::toLowerCase(Character::toUpperCase(c)))
                    return false;
            }
            return next.isPresent() && next.get().match(matcher, i + len, seq);
        }

        Pattern::SliceS::SliceS(const IntArray& buffer): Slice(buffer) {}

        gbool Pattern::SliceS::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint x = i;
            for (gint j = 0; j < buffer.length(); j++) {
                if (x >= matcher.to) {
                    matcher.hitEnd_ = true;
                    return false;
                }
                gint c = Character::codePointAt(seq, x);
                if (buffer[j] != c)
                    return false;
                x += Character::charCount(c);
                if (x > matcher.to) {
                    matcher.hitEnd_ = true;
                    return false;
                }
            }
            return next.isPresent() && next.get().match(matcher, x, seq);
        }

        Pattern::SliceIS::SliceIS(const IntArray& buffer): SliceNode(buffer) {}

        gint Pattern::SliceIS::toLower(gint c) {
            return ASCII::toLower(c);
        }

        gbool Pattern::SliceIS::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint x = i;
            for (gint j = 0; j < buffer.length(); j++) {
                if (x >= matcher.to) {
                    matcher.hitEnd_ = true;
                    return false;
                }
                gint c = Character::codePointAt(seq, x);
                if (buffer[j] != c && buffer[j] != toLower(c))
                    return false;
                x += Character::charCount(c);
                if (x > matcher.to) {
                    matcher.hitEnd_ = true;
                    return false;
                }
            }
            return next.isPresent() && next.get().match(matcher, x, seq);
        }

        Pattern::SliceUS::SliceUS(const IntArray& buffer): SliceIS(buffer) {
        }

        gint Pattern::SliceUS::toLower(gint c) {
            return Character::toLowerCase(Character::toUpperCase(c));
        }

        Pattern::Ques::Ques(const Optional<Node>& atom, Qtype type): atom(atom), type(type) {}

        gbool Pattern::Ques::match(Matcher& matcher, gint i, const CharSequence& seq) {
            switch (type) {
                case Qtype::GREEDY:
                    return (atom.isPresent() && atom.get().match(matcher, i, seq) &&
                                next.isPresent() && next.get().match(matcher, matcher.last, seq))
                            || next.isPresent() && next.get().match(matcher, i, seq);
                case Qtype::LAZY:
                    return next.isPresent() && next.get().match(matcher, i, seq)
                            || (atom.isPresent() && atom.get().match(matcher, i, seq) &&
                                next.isPresent() && next.get().match(matcher, matcher.last, seq));
                case Qtype::POSSESSIVE:
                    if (atom.isPresent() && atom.get().match(matcher, i, seq)) i = matcher.last;
                    return next.isPresent() && next.get().match(matcher, i, seq);
                default:
                    return atom.isPresent() && atom.get().match(matcher, i, seq) &&
                            next.isPresent() && next.get().match(matcher, matcher.last, seq);
            }
        }

        gbool Pattern::Ques::study(TreeInfo& info) {
            if (type != Qtype::INDEPENDENT) {
                gint minL = info.minLength;
                if (atom.isPresent())
                    atom.get().study(info);
                info.minLength = minL;
                info.deterministic = false;
                return next.isPresent() && next.get().study(info);
            }
            if (atom.isPresent())
                atom.get().study(info);
            return next.isPresent() && next.get().study(info);
        }

        Pattern::CharPropertyGreedy::CharPropertyGreedy(const Optional<CharProperty>& cp, gint cmin)
            : predicate(cp.get().predicate), cmin(cmin) {}

        gbool Pattern::CharPropertyGreedy::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint starti = i;
            gint n = 0;
            gint to = matcher.to;
            // greedy, all the way down
            while (i < to) {
                gint ch = Character::codePointAt(seq, i);
                gint len = Character::charCount(ch);
                if (i + len > to) {
                    // the region cut off the high half of a surrogate pair
                    matcher.hitEnd_ = true;
                    ch = seq.charAt(i);
                    len = 1;
                }
                if (predicate.isPresent() && !predicate.get().test(ch))
                    break;
                i += len;
                n++;
            }
            if (i >= to) {
                matcher.hitEnd_ = true;
            }
            while (n >= cmin) {
                if (next.isPresent() && next.get().match(matcher, i, seq))
                    return true;
                if (n == cmin)
                    return false;
                // backing off if match fails
                gint ch = Character::codePointBefore(seq, i);
                // check if the region cut off the low half of a surrogate pair
                i = Math::max(starti, i - Character::charCount(ch));
                n--;
            }
            return false;
        }

        gbool Pattern::CharPropertyGreedy::study(TreeInfo& info) {
            info.minLength += cmin;
            if (info.maxValid) {
                info.maxLength += MAX_REPS;
            }
            info.deterministic = false;
            return next.isPresent() && next.get().study(info);
        }

        Pattern::BmpCharPropertyGreedy::BmpCharPropertyGreedy(const Optional<BmpCharProperty>& predicate, gint cmin)
            : CharPropertyGreedy(predicate, cmin) {}

        gbool Pattern::BmpCharPropertyGreedy::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint n = 0;
            gint to = matcher.to;
            while (i < to && predicate.isPresent() && predicate.get().test(seq.charAt(i))) {
                i++;
                n++;
            }
            if (i >= to) {
                matcher.hitEnd_ = true;
            }
            while (n >= cmin) {
                if (next.isPresent() && next.get().match(matcher, i, seq))
                    return true;
                i--;
                n--; // backing off if match fails
            }
            return false;
        }

        Pattern::Curly::Curly(const Optional<Node>& atom, gint cmin, gint cmax, Qtype type)
            : atom(atom), type(type), cmin(cmin), cmax(cmax) {}

        gbool Pattern::Curly::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint j;
            for (j = 0; j < cmin; j++) {
                if (atom.isPresent() && atom.get().match(matcher, i, seq)) {
                    i = matcher.last;
                    continue;
                }
                return false;
            }
            if (type == Qtype::GREEDY)
                return match0(matcher, i, j, seq);
            if (type == Qtype::LAZY)
                return match1(matcher, i, j, seq);
            return match2(matcher, i, j, seq);
        }

        gbool Pattern::Curly::match0(Matcher& matcher, gint i, gint j, const CharSequence& seq) {
            if (j >= cmax) {
                // We have matched the maximum... continue with the rest of
                // the regular expression
                return next.isPresent() && next.get().match(matcher, i, seq);
            }
            gint backLimit = j;
            while (atom.isPresent() && atom.get().match(matcher, i, seq)) {
                // k is the length of this match
                gint k = matcher.last - i;
                if (k == 0) // Zero length match
                    break;
                // Move up index and number matched
                i = matcher.last;
                j++;
                // We are greedy so match as many as we can
                while (j < cmax) {
                    if (atom.isPresent() && !atom.get().match(matcher, i, seq))
                        break;
                    if (i + k != matcher.last) {
                        if (match0(matcher, matcher.last, j + 1, seq))
                            return true;
                        break;
                    }
                    i += k;
                    j++;
                }
                // Handle backing off if match fails
                while (j >= backLimit) {
                    if (next.isPresent() && next.get().match(matcher, i, seq))
                        return true;
                    i -= k;
                    j--;
                }
                return false;
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::Curly::match1(Matcher& matcher, gint i, gint j, const CharSequence& seq) {
            for (;;) {
                // Try finishing match without consuming any more
                if (next.isPresent() && next.get().match(matcher, i, seq))
                    return true;
                // At the maximum, no match found
                if (j >= cmax)
                    return false;
                // Okay, must try one more atom
                if (atom.isPresent() && !atom.get().match(matcher, i, seq))
                    return false;
                // If we haven't moved forward then must break out
                if (i == matcher.last)
                    return false;
                // Move up index and number matched
                i = matcher.last;
                j++;
            }
        }

        gbool Pattern::Curly::match2(Matcher& matcher, gint i, gint j, const CharSequence& seq) {
            for (; j < cmax; j++) {
                if (atom.isPresent() && !atom.get().match(matcher, i, seq))
                    break;
                if (i == matcher.last)
                    break;
                i = matcher.last;
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::Curly::study(TreeInfo& info) {
            // Save original info
            gint minL = info.minLength;
            gint maxL = info.maxLength;
            gbool maxV = info.maxValid;
            gbool detm = info.deterministic;
            info.reset();

            if (atom.isPresent())
                atom.get().study(info);

            gint temp = info.minLength * cmin + minL;
            if (temp < minL) {
                temp = 0xFFFFFFF; // arbitrary large number
            }
            info.minLength = temp;

            if (maxV & info.maxValid) {
                temp = info.maxLength * cmax + maxL;
                info.maxLength = temp;
                if (temp < maxL) {
                    info.maxValid = false;
                }
            } else {
                info.maxValid = false;
            }

            if (info.deterministic && cmin == cmax)
                info.deterministic = detm;
            else
                info.deterministic = false;
            return next.isPresent() && next.get().study(info);
        }

        Pattern::GroupCurly::GroupCurly(const Optional<Node>& atom, gint cmin, gint cmax, Qtype type,
                                        gint local, gint group, gbool capture)
            : atom(atom),
              type(type),
              cmin(cmin),
              cmax(cmax),
              localIndex(local),
              groupIndex(group),
              capture(capture) {}

        gbool Pattern::GroupCurly::match(Matcher& matcher, gint i, const CharSequence& seq) {
            IntArray& groups = matcher.groups;
            IntArray& locals = matcher.locals;
            gint save0 = locals[localIndex];
            gint save1 = 0;
            gint save2 = 0;

            if (capture) {
                save1 = groups[groupIndex];
                save2 = groups[groupIndex + 1];
            }

            // Notify GroupTail there is no need to setup group info
            // because it will be set here
            locals[localIndex] = -1;

            gbool ret = true;
            for (gint j = 0; j < cmin; j++) {
                if (atom.isPresent() && atom.get().match(matcher, i, seq)) {
                    if (capture) {
                        groups[groupIndex] = i;
                        groups[groupIndex + 1] = matcher.last;
                    }
                    i = matcher.last;
                } else {
                    ret = false;
                    break;
                }
            }
            if (ret) {
                if (type == Qtype::GREEDY) {
                    ret = match0(matcher, i, cmin, seq);
                } else if (type == Qtype::LAZY) {
                    ret = match1(matcher, i, cmin, seq);
                } else {
                    ret = match2(matcher, i, cmin, seq);
                }
            }
            if (!ret) {
                locals[localIndex] = save0;
                if (capture) {
                    groups[groupIndex] = save1;
                    groups[groupIndex + 1] = save2;
                }
            }
            return ret;
        }

        gbool Pattern::GroupCurly::match0(Matcher& matcher, gint i, gint j, const CharSequence& seq) {
            // don't back off passing the starting "j"
            gint min = j;
            IntArray& groups = matcher.groups;
            gint save0 = 0;
            gint save1 = 0;
            if (capture) {
                save0 = groups[groupIndex];
                save1 = groups[groupIndex + 1];
            }
            for (;;) {
                if (j >= cmax)
                    break;
                if (atom.isPresent() && !atom.get().match(matcher, i, seq))
                    break;
                gint k = matcher.last - i;
                if (k <= 0) {
                    if (capture) {
                        groups[groupIndex] = i;
                        groups[groupIndex + 1] = i + k;
                    }
                    i = i + k;
                    break;
                }
                for (;;) {
                    if (capture) {
                        groups[groupIndex] = i;
                        groups[groupIndex + 1] = i + k;
                    }
                    i = i + k;
                    if (++j >= cmax)
                        break;
                    if (atom.isPresent() && !atom.get().match(matcher, i, seq))
                        break;
                    if (i + k != matcher.last) {
                        if (match0(matcher, i, j, seq))
                            return true;
                        break;
                    }
                }
                while (j > min) {
                    if (next.isPresent() && next.get().match(matcher, i, seq)) {
                        if (capture) {
                            groups[groupIndex + 1] = i;
                            groups[groupIndex] = i - k;
                        }
                        return true;
                    }
                    // backing off
                    i = i - k;
                    if (capture) {
                        groups[groupIndex + 1] = i;
                        groups[groupIndex] = i - k;
                    }
                    j--;
                }
                break;
            }
            if (capture) {
                groups[groupIndex] = save0;
                groups[groupIndex + 1] = save1;
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::GroupCurly::match1(Matcher& matcher, gint i, gint j, const CharSequence& seq) {
            for (;;) {
                if (next.isPresent() && next.get().match(matcher, i, seq))
                    return true;
                if (j >= cmax)
                    return false;
                if (atom.isPresent() && !atom.get().match(matcher, i, seq))
                    return false;
                if (i == matcher.last)
                    return false;
                if (capture) {
                    matcher.groups[groupIndex] = i;
                    matcher.groups[groupIndex + 1] = matcher.last;
                }
                i = matcher.last;
                j++;
            }
        }

        gbool Pattern::GroupCurly::match2(Matcher& matcher, gint i, gint j, const CharSequence& seq) {
            for (; j < cmax; j++) {
                if (atom.isPresent() && !atom.get().match(matcher, i, seq)) {
                    break;
                }
                if (capture) {
                    matcher.groups[groupIndex] = i;
                    matcher.groups[groupIndex + 1] = matcher.last;
                }
                if (i == matcher.last) {
                    break;
                }
                i = matcher.last;
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::GroupCurly::study(TreeInfo& info) {
            // Save original info
            gint minL = info.minLength;
            gint maxL = info.maxLength;
            gbool maxV = info.maxValid;
            gbool detm = info.deterministic;
            info.reset();

            if (atom.isPresent())
                atom.get().study(info);

            gint temp = info.minLength * cmin + minL;
            if (temp < minL) {
                temp = 0xFFFFFFF; // Arbitrary large number
            }
            info.minLength = temp;

            if (maxV & info.maxValid) {
                temp = info.maxLength * cmax + maxL;
                info.maxLength = temp;
                if (temp < maxL) {
                    info.maxValid = false;
                }
            } else {
                info.maxValid = false;
            }

            if (info.deterministic && cmin == cmax) {
                info.deterministic = detm;
            } else {
                info.deterministic = false;
            }
            return next.isPresent() && next.get().study(info);
        }

        gbool Pattern::BranchConn::match(Matcher& matcher, gint i, const CharSequence& seq) {
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::BranchConn::study(TreeInfo& info) {
            return info.deterministic;
        }

        Pattern::Branch::Branch(Optional<Node> first, Optional<Node> second, const Optional<Node>& branchConn) {
            CORE_IGNORE_DEPRECATIONS(atoms = Array<Node>(2);)
            if (first.isPresent())
                Arrays::fastSet(atoms, 0, first.get());
            if (second.isPresent())
                Arrays::fastSet(atoms, 1, second.get());
            conn = branchConn;
        }

        void Pattern::Branch::addNode(Optional<Node> node) {
            if (size >= atoms.length()) {
                atoms = Arrays::copyOf(atoms, atoms.length() * 2);
            }
            if (node.isPresent())
                Arrays::fastSet(atoms, size++, node.get());
        }

        gbool Pattern::Branch::match(Matcher& matcher, gint i, const CharSequence& seq) {
            for (gint n = 0; n < size; n++) {
                if (atoms.getOrNull(n) == null) {
                    if (conn.isPresent() &&
                        conn.get().next.isPresent() &&
                        conn.get().next.get().match(matcher, i, seq))
                        return true;
                } else if (atoms[n].match(matcher, i, seq)) {
                    return true;
                }
            }
            return false;
        }

        gbool Pattern::Branch::study(TreeInfo& info) {
            gint minL = info.minLength;
            gint maxL = info.maxLength;
            gbool maxV = info.maxValid;

            gint minL2 = Integer::MAX_VALUE; //arbitrary large enough num
            gint maxL2 = -1;
            for (gint n = 0; n < size; n++) {
                info.reset();
                if (atoms.getOrNull(n) != null)
                    atoms[n].study(info);
                minL2 = Math::min(minL2, info.minLength);
                maxL2 = Math::max(maxL2, info.maxLength);
                maxV = (maxV & info.maxValid);
            }

            minL += minL2;
            maxL += maxL2;

            info.reset();
            conn.get().next.get().study(info);

            info.minLength += minL;
            info.maxLength += maxL;
            info.maxValid &= maxV;
            info.deterministic = false;
            return false;
        }

        Pattern::GroupHead::GroupHead(gint localCount): localIndex(localCount) {}

        gbool Pattern::GroupHead::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint save = matcher.locals[localIndex];
            matcher.locals[localIndex] = i;
            gbool ret = next.isPresent() && next.get().match(matcher, i, seq);
            matcher.locals[localIndex] = save;
            return ret;
        }

        Pattern::GroupTail::GroupTail(gint localCount, gint groupCount)
            : localIndex(localCount), groupIndex(groupCount + groupCount) {}

        gbool Pattern::GroupTail::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint tmp = matcher.locals[localIndex];
            if (tmp >= 0) {
                // This is the normal group case.
                // Save the group so we can unset it if it
                // backs off of a match.
                gint groupStart = matcher.groups[groupIndex];
                gint groupEnd = matcher.groups[groupIndex + 1];

                matcher.groups[groupIndex] = tmp;
                matcher.groups[groupIndex + 1] = i;
                if (next.isPresent() && next.get().match(matcher, i, seq)) {
                    return true;
                }
                matcher.groups[groupIndex] = groupStart;
                matcher.groups[groupIndex + 1] = groupEnd;
                return false;
            }
            // This is a group reference case. We don't need to save any
            // group info because it isn't really a group.
            matcher.last = i;
            return true;
        }

        Pattern::Prolog::Prolog(const Optional<Loop>& loop): loop(loop) {}

        gbool Pattern::Prolog::match(Matcher& matcher, gint i, const CharSequence& seq) {
            return loop.isPresent() && loop.get().match(matcher, i, seq);
        }

        gbool Pattern::Prolog::study(TreeInfo& info) {
            return loop.isPresent() && loop.get().study(info);
        }

        Pattern::Loop::Loop(gint countIndex, gint beginIndex)
            : countIndex(countIndex), beginIndex(beginIndex), posIndex(-1) {}

        gbool Pattern::Loop::match(Matcher& matcher, gint i, const CharSequence& seq) {
            // Avoid infinite loop in zero-length case.
            if (i > matcher.locals[beginIndex]) {
                gint count = matcher.locals[countIndex];

                // This block is for before we reach the minimum
                // iterations required for the loop to match
                if (count < cmin) {
                    matcher.locals[countIndex] = count + 1;
                    gbool b = body.isPresent() && body.get().match(matcher, i, seq);
                    // If match failed we must backtrack, so
                    // the loop count should NOT be incremented
                    if (!b)
                        matcher.locals[countIndex] = count;
                    // Return success or failure since we are under
                    // minimum
                    return b;
                }
                // This block is for after we have the minimum
                // iterations required for the loop to match
                if (count < cmax) {
                    // Let's check if we have already tried and failed
                    // at this starting position "i" in the past.
                    // If yes, then just return false without trying
                    // again, to stop the exponential backtracking.
                    if (posIndex != -1 &&
                        matcher.localsPos[posIndex].contains(i)) {
                        return next.isPresent() && next.get().match(matcher, i, seq);
                    }
                    matcher.locals[countIndex] = count + 1;
                    gbool b = body.isPresent() && body.get().match(matcher, i, seq);
                    // If match failed we must backtrack, so
                    // the loop count should NOT be incremented
                    if (b)
                        return true;
                    matcher.locals[countIndex] = count;
                    // save the failed position
                    if (posIndex != -1) {
                        matcher.localsPos[posIndex].add(i);
                    }
                }
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::Loop::matchInit(Matcher& matcher, gint i, const CharSequence& seq) {
            gint save = matcher.locals[countIndex];
            gbool ret;
            if (posIndex != -1 && matcher.localsPos.getOrNull(posIndex) == null)
                Arrays::fastSet(matcher.localsPos, posIndex, UNSAFE::newInstance<Matcher::HashSet>());
            if (0 < cmin) {
                matcher.locals[countIndex] = 1;
                ret = body.isPresent() && body.get().match(matcher, i, seq);
            } else if (0 < cmax) {
                matcher.locals[countIndex] = 1;
                ret = body.isPresent() && body.get().match(matcher, i, seq);
                if (!ret)
                    ret = next.isPresent() && next.get().match(matcher, i, seq);
            } else {
                ret = next.isPresent() && next.get().match(matcher, i, seq);
            }
            matcher.locals[countIndex] = save;
            return ret;
        }

        gbool Pattern::Loop::study(TreeInfo& info) {
            info.maxValid = false;
            info.deterministic = false;
            return false;
        }

        Pattern::LazyLoop::LazyLoop(gint countIndex, gint beginIndex): Loop(countIndex, beginIndex) {}

        gbool Pattern::LazyLoop::match(Matcher& matcher, gint i, const CharSequence& seq) {
            // Check for zero length group
            if (i > matcher.locals[beginIndex]) {
                gint count = matcher.locals[countIndex];
                if (count < cmin) {
                    matcher.locals[countIndex] = count + 1;
                    gbool result = body.isPresent() && body.get().match(matcher, i, seq);
                    // If match failed we must backtrack, so
                    // the loop count should NOT be incremented
                    if (!result)
                        matcher.locals[countIndex] = count;
                    return result;
                }
                if (next.isPresent() && next.get().match(matcher, i, seq))
                    return true;
                if (count < cmax) {
                    matcher.locals[countIndex] = count + 1;
                    gbool result = body.isPresent() && body.get().match(matcher, i, seq);
                    // If match failed we must backtrack, so
                    // the loop count should NOT be incremented
                    if (!result)
                        matcher.locals[countIndex] = count;
                    return result;
                }
                return false;
            }
            return next.isPresent() && next.get().match(matcher, i, seq);
        }

        gbool Pattern::LazyLoop::matchInit(Matcher& matcher, gint i, const CharSequence& seq) {
            gint save = matcher.locals[countIndex];
            gbool ret = false;
            if (0 < cmin) {
                matcher.locals[countIndex] = 1;
                ret = body.isPresent() && body.get().match(matcher, i, seq);
            } else if (next.isPresent() && next.get().match(matcher, i, seq)) {
                ret = true;
            } else if (0 < cmax) {
                matcher.locals[countIndex] = 1;
                ret = body.isPresent() && body.get().match(matcher, i, seq);
            }
            matcher.locals[countIndex] = save;
            return ret;
        }

        gbool Pattern::LazyLoop::study(TreeInfo& info) {
            info.maxValid = false;
            info.deterministic = false;
            return false;
        }

        Pattern::BackRef::BackRef(gint groupCount): groupIndex(groupCount + groupCount) {}

        gbool Pattern::BackRef::match(Matcher& matcher, gint i, const CharSequence& seq) {
            // reference to not existing group must never match
            // group does not exist if matcher didn't allocate space for it
            if (groupIndex >= matcher.groups.length()) {
                return false;
            }

            gint j = matcher.groups[groupIndex];
            gint k = matcher.groups[groupIndex + 1];

            gint groupSize = k - j;
            // If the referenced group didn't match, neither can this
            if (j < 0)
                return false;

            // If there isn't enough input left no match
            if (i + groupSize > matcher.to) {
                matcher.hitEnd_ = true;
                return false;
            }
            // Check each new char to make sure it matches what the group
            // referenced matched last time around
            for (gint index = 0; index < groupSize; index++)
                if (seq.charAt(i + index) != seq.charAt(j + index))
                    return false;

            return next.isPresent() && next.get().match(matcher, i + groupSize, seq);
        }

        gbool Pattern::BackRef::study(TreeInfo& info) {
            info.maxValid = false;
            return next.isPresent() && next.get().study(info);
        }

        Pattern::CIBackRef::CIBackRef(gint groupCount, gint doUnicodeCase)
            : doUnicodeCase(doUnicodeCase), groupIndex(groupCount + groupCount) {}

        gbool Pattern::CIBackRef::match(Matcher& matcher, gint i, const CharSequence& seq) {
            // reference to not existing group must never match
            // group does not exist if matcher didn't allocate space for it
            if (groupIndex >= matcher.groups.length()) {
                return false;
            }

            gint j = matcher.groups[groupIndex];
            gint k = matcher.groups[groupIndex + 1];

            gint groupSizeChars = k - j; //Group size in chars

            // If the referenced group didn't match, neither can this
            if (j < 0)
                return false;

            // If there isn't enough input left no match
            if (i + groupSizeChars > matcher.to) {
                matcher.hitEnd_ = true;
                return false;
            }

            // Check each new char to make sure it matches what the group
            // referenced matched last time around
            gint x = i;

            // We set groupCodepoints to the number of chars
            // in the given subsequence but this is an upper bound estimate
            // we reduce by one if we spot 2-char codepoints.
            gint groupCodepoints = groupSizeChars;

            for (gint index = 0; index < groupCodepoints; index++) {
                gint c1 = Character::codePointAt(seq, x);
                gint c2 = Character::codePointAt(seq, j);
                if (c1 != c2) {
                    if (doUnicodeCase) {
                        gint cc1 = Character::toUpperCase(c1);
                        gint cc2 = Character::toUpperCase(c2);
                        if (cc1 != cc2 &&
                            Character::toLowerCase(cc1) !=
                            Character::toLowerCase(cc2))
                            return false;
                    } else {
                        if (ASCII::toLower(c1) != ASCII::toLower(c2))
                            return false;
                    }
                }
                x += Character::charCount(c1);
                j += Character::charCount(c2);

                if (c1 >= Character::MIN_SUPPLEMENTARY) {
                    //Group size is guessed in terms of chars, but we need to
                    //adjust if we spot a 2-char codePoint.
                    groupCodepoints--;
                }
            }

            return next.isPresent() && next.get().match(matcher, i + groupSizeChars, seq);
        }

        gbool Pattern::CIBackRef::study(TreeInfo& info) {
            info.maxValid = false;
            return next.isPresent() && next.get().study(info);
        }

        Pattern::First::First(const Optional<Node>& atom): atom(BnM::optimize(atom)) {}

        gbool Pattern::First::match(Matcher& matcher, gint i, const CharSequence& seq) {
            if (atom.isPresent() && Class<BnM>::hasInstance(atom.get())) {
                return atom.get().match(matcher, i, seq)
                        && next.isPresent() && next.get().match(matcher, matcher.last, seq);
            }
            for (;;) {
                if (i > matcher.to) {
                    matcher.hitEnd_ = true;
                    return false;
                }
                if (atom.isPresent() && atom.get().match(matcher, i, seq)) {
                    return next.isPresent() && next.get().match(matcher, matcher.last, seq);
                }
                i += Self::countChars(seq, i, 1);
                matcher.first++;
            }
        }

        gbool Pattern::First::study(TreeInfo& info) {
            if (atom.isPresent())
                atom.get().study(info);
            info.maxValid = false;
            info.deterministic = false;
            return next.isPresent() && next.get().study(info);
        }

        Pattern::Pos::Pos(const Optional<Node>& cond): cond(cond) {}

        gbool Pattern::Pos::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint savedTo = matcher.to;
            gbool conditionMatched;

            // Relax transparent region boundaries for lookahead
            if (matcher.transparentBounds)
                matcher.to = matcher.getTextLength();
            try {
                conditionMatched = cond.isPresent() && cond.get().match(matcher, i, seq);
                // Reinstate region boundaries
                matcher.to = savedTo;
            } catch (Throwable const& ex) {
                // Reinstate region boundaries
                matcher.to = savedTo;
                ex.throws($ftrace());
            }
            return conditionMatched && next.isPresent() && next.get().match(matcher, i, seq);
        }

        Pattern::Neg::Neg(const Optional<Node>& cond): cond(cond) {}

        gbool Pattern::Neg::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint savedTo = matcher.to;
            gbool conditionMatched;

            // Relax transparent region boundaries for lookahead
            if (matcher.transparentBounds)
                matcher.to = matcher.getTextLength();
            try {
                if (i < matcher.to) {
                    conditionMatched = cond.isPresent() && !cond.get().match(matcher, i, seq);
                } else {
                    // If a negative lookahead succeeds then more input
                    // could cause it to fail!
                    matcher.requireEnd = true;
                    conditionMatched = cond.isPresent() && !cond.get().match(matcher, i, seq);
                }

                // Reinstate region boundaries
                matcher.to = savedTo;
            } catch (Throwable const& ex) {
                // Reinstate region boundaries
                matcher.to = savedTo;
                ex.throws($ftrace());
            }
            return conditionMatched && next.isPresent() && next.get().match(matcher, i, seq);
        }

        Pattern::LookBehindEndNode& Pattern::LookBehindEndNode::INSTANCE = UNSAFE::newInstance<LookBehindEndNode>();

        gbool Pattern::LookBehindEndNode::match(Matcher& matcher, gint i, const CharSequence& seq) {
            return i == matcher.lookbehindTo;
        }

        Pattern::Behind::Behind(const Optional<Node>& cond, gint rmax, gint rmin): cond(cond),
            rmax(rmax),
            rmin(rmin) {}

        gbool Pattern::Behind::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint savedFrom = matcher.from;
            gbool conditionMatched = false;
            gint startIndex = (!matcher.transparentBounds) ? matcher.from : 0;
            gint from = Math::max(i - rmax, startIndex);
            // Set end boundary
            gint savedLBT = matcher.lookbehindTo;
            matcher.lookbehindTo = i;
            // Relax transparent region boundaries for lookbehind
            if (matcher.transparentBounds)
                matcher.from = 0;
            for (gint j = i - rmin; !conditionMatched && j >= from; j--) {
                conditionMatched = cond.isPresent() && cond.get().match(matcher, j, seq);
            }
            matcher.from = savedFrom;
            matcher.lookbehindTo = savedLBT;
            return conditionMatched && next.isPresent() && next.get().match(matcher, i, seq);
        }

        Pattern::BehindS::BehindS(const Optional<Node>& cond, gint rmax, gint rmin): Behind(cond, rmax, rmin) {}

        gbool Pattern::BehindS::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint rmaxChars = Self::countChars(seq, i, -rmax);
            gint rminChars = Self::countChars(seq, i, -rmin);
            gint savedFrom = matcher.from;
            gint startIndex = (!matcher.transparentBounds) ? matcher.from : 0;
            gbool conditionMatched = false;
            gint from = Math::max(i - rmaxChars, startIndex);
            // Set end boundary
            gint savedLBT = matcher.lookbehindTo;
            matcher.lookbehindTo = i;
            // Relax transparent region boundaries for lookbehind
            if (matcher.transparentBounds)
                matcher.from = 0;

            for (gint j = i - rminChars;
                 !conditionMatched && j >= from;
                 j -= j > from ? Self::countChars(seq, j, -1) : 1) {
                conditionMatched = cond.isPresent() && cond.get().match(matcher, j, seq);
            }
            matcher.from = savedFrom;
            matcher.lookbehindTo = savedLBT;
            return conditionMatched && next.isPresent() && next.get().match(matcher, i, seq);
        }

        Pattern::NotBehind::NotBehind(const Optional<Node>& cond, gint rmax, gint rmin): cond(cond),
            rmax(rmax),
            rmin(rmin) {}

        gbool Pattern::NotBehind::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint savedLBT = matcher.lookbehindTo;
            gint savedFrom = matcher.from;
            gbool conditionMatched = false;
            gint startIndex = (!matcher.transparentBounds) ? matcher.from : 0;
            gint from = Math::max(i - rmax, startIndex);
            matcher.lookbehindTo = i;
            // Relax transparent region boundaries for lookbehind
            if (matcher.transparentBounds)
                matcher.from = 0;
            for (gint j = i - rmin; !conditionMatched && j >= from; j--) {
                conditionMatched = cond.isPresent() && cond.get().match(matcher, j, seq);
            }
            // Reinstate region boundaries
            matcher.from = savedFrom;
            matcher.lookbehindTo = savedLBT;
            return !conditionMatched && next.isPresent() && next.get().match(matcher, i, seq);
        }

        Pattern::NotBehindS::NotBehindS(const Optional<Node>& cond, gint rmax, gint rmin): NotBehind(cond, rmax, rmin) {
        }

        gbool Pattern::NotBehindS::match(Matcher& matcher, gint i, const CharSequence& seq) {
            gint rmaxChars = Self::countChars(seq, i, -rmax);
            gint rminChars = Self::countChars(seq, i, -rmin);
            gint savedFrom = matcher.from;
            gint savedLBT = matcher.lookbehindTo;
            gbool conditionMatched = false;
            gint startIndex = (!matcher.transparentBounds) ? matcher.from : 0;
            gint from = Math::max(i - rmaxChars, startIndex);
            matcher.lookbehindTo = i;
            // Relax transparent region boundaries for lookbehind
            if (matcher.transparentBounds)
                matcher.from = 0;
            for (gint j = i - rminChars;
                 !conditionMatched && j >= from;
                 j -= j > from ? Self::countChars(seq, j, -1) : 1) {
                conditionMatched = cond.isPresent() && cond.get().match(matcher, j, seq);
            }
            //Reinstate region boundaries
            matcher.from = savedFrom;
            matcher.lookbehindTo = savedLBT;
            return !conditionMatched && next.isPresent() && next.get().match(matcher, i, seq);
        }

        Pattern::Bound::Bound(gint type, gbool useUWORD): type(type),
                                                          useUWORD(useUWORD) {}

        gbool Pattern::Bound::isWord(gint ch) const {
            return useUWORD
                       ? Predicates::WORD().get().test(ch)
                       : Predicates::ASCII_WORD().get().test(ch);
        }

        gbool Pattern::Bound::hasBaseCharacter(Matcher& matcher, gint i, const CharSequence& seq) {
            gint start = (!matcher.transparentBounds)
                             ? matcher.from
                             : 0;
            for (gint x = i; x >= start; x--) {
                gint ch = Character::codePointAt(seq, x);
                if (Character::isLetterOrDigit(ch))
                    return true;
                if (Character::category(ch) == Character::Category::NON_SPACING_MARK)
                    continue;
                return false;
            }
            return false;
        }

        gint Pattern::Bound::check(Matcher& matcher, gint i, const CharSequence& seq) const {
            gint ch;
            gbool left = false;
            gint startIndex = matcher.from;
            gint endIndex = matcher.to;
            if (matcher.transparentBounds) {
                startIndex = 0;
                endIndex = matcher.getTextLength();
            }
            if (i > startIndex) {
                ch = Character::codePointBefore(seq, i);
                left = (isWord(ch) ||
                    ((Character::category(ch) == Character::Category::NON_SPACING_MARK)
                        && hasBaseCharacter(matcher, i - 1, seq)));
            }
            gbool right = false;
            if (i < endIndex) {
                ch = Character::codePointAt(seq, i);
                right = (isWord(ch) ||
                    ((Character::category(ch) == Character::Category::NON_SPACING_MARK)
                        && hasBaseCharacter(matcher, i, seq)));
            } else {
                // Tried to access char past the end
                matcher.hitEnd_ = true;
                // The addition of another char could wreck a boundary
                matcher.requireEnd = true;
            }
            return ((left ^ right) ? (right ? LEFT : RIGHT) : NONE);
        }

        gbool Pattern::Bound::match(Matcher& matcher, gint i, const CharSequence& seq) {
            return (check(matcher, i, seq) & type) > 0
                    && next.isPresent() && next.get().match(matcher, i, seq);
        }

        Optional<Pattern::Node> Pattern::BnM::optimize(Optional<Node> node) {
            if (node.isPresent() && !(Class<Slice>::hasInstance(node.get()))) {
                return node;
            }

            IntArray& src = ((Slice&)node).buffer;
            gint patternLength = src.length();
            // The BM algorithm requires a bit of overhead;
            // If the pattern is short don't use it, since
            // a shift larger than the pattern length cannot
            // be used anyway.
            if (patternLength < 4) {
                return node;
            }
            gint i, j;
            IntArray lastOcc = IntArray(128);
            IntArray optoSft = IntArray(patternLength);
            // Precalculate part of the bad character shift
            // It is a table for where in the pattern each
            // lower 7-bit value occurs
            for (i = 0; i < patternLength; i++) {
                lastOcc[src[i] & 0x7F] = i + 1;
            }
            // Precalculate the good suffix shift
            // i is the shift amount being considered
        NEXT:
            for (i = patternLength; i > 0; i--) {
                // j is the beginning index of suffix being considered
                for (j = patternLength - 1; j >= i; j--) {
                    // Testing for good suffix
                    if (src[j] == src[j - i]) {
                        // src[j..len] is a good suffix
                        optoSft[j - 1] = i;
                    } else {
                        // No match. The array has already been
                        // filled up with correct values before.
                        goto NEXT;
                    }
                }
                // This fills up the remaining of optoSft
                // any suffix can not have larger shift amount
                // then its sub-suffix. Why???
                while (j > 0) {
                    optoSft[--j] = i;
                }
            }
            // Set the guard value because of unicode compression
            optoSft[patternLength - 1] = 1;
            if (node.isPresent() && Class<SliceS>::hasInstance(node.get()))
                return UNSAFE::newInstance<BnMS>(src, lastOcc, optoSft, (Optional<Node>)node.get().next);
            return UNSAFE::newInstance<BnM>(src, lastOcc, optoSft, (Optional<Node>)node.get().next);
        }

        Pattern::BnM::BnM(IntArray buffer, IntArray lastOcc, IntArray optoSft, Optional<Node> next)
            : buffer(UNSAFE::moveInstance(buffer)),
              lastOcc(UNSAFE::moveInstance(lastOcc)),
              optoSft(UNSAFE::moveInstance(optoSft)) {
            this->next = UNSAFE::moveInstance(next);
        }

        gbool Pattern::BnM::match(Matcher& matcher, gint i, const CharSequence& seq) {
            IntArray& src = buffer;
            gint patternLength = src.length();
            gint last = matcher.to - patternLength;

            // Loop over all possible match positions in text
        NEXT:
            while (i <= last) {
                // Loop over pattern from right to left
                for (gint j = patternLength - 1; j >= 0; j--) {
                    gint ch = seq.charAt(i + j);
                    if (ch != src[j]) {
                        // Shift search to the right by the maximum of the
                        // bad character shift and the good suffix shift
                        i += Math::max(j + 1 - lastOcc[ch & 0x7F], optoSft[j]);
                        goto NEXT;
                    }
                }
                // Entire pattern matched starting at i
                matcher.first = i;
                gbool ret = next.isPresent() && next.get().match(matcher, i + patternLength, seq);
                if (ret) {
                    matcher.first = i;
                    matcher.groups[0] = matcher.first;
                    matcher.groups[1] = matcher.last;
                    return true;
                }
                i++;
            }
            // BnM is only used as the leading node in the unanchored case,
            // and it replaced its Start() which always searches to the end
            // if it doesn't find what it's looking for, so hitEnd is true.
            matcher.hitEnd_ = true;
            return false;
        }

        gbool Pattern::BnM::study(TreeInfo& info) {
            info.minLength += buffer.length();
            info.maxValid = false;
            return next.isPresent() && next.get().study(info);
        }

        Pattern::BnMS::BnMS(const IntArray& buffer, const IntArray& lastOcc, const IntArray& optoSft,
                            const Optional<Node>& next): BnM(buffer, lastOcc, optoSft, next), lengthInChars(0) {
            for (gint cp : buffer) {
                lengthInChars += Character::charCount(cp);
            }
        }

        gbool Pattern::BnMS::match(Matcher& matcher, gint i, const CharSequence& seq) {
            IntArray& src = buffer;
            gint patternLength = src.length();
            gint last = matcher.to - lengthInChars;

            // Loop over all possible match positions in text
        NEXT:
            while (i <= last) {
                // Loop over pattern from right to left
                gint ch;
                for (gint j = Self::countChars(seq, i, patternLength), x = patternLength - 1;
                     j > 0; j -= Character::charCount(ch), x--) {
                    ch = Character::codePointBefore(seq, i + j);
                    if (ch != src[x]) {
                        // Shift search to the right by the maximum of the
                        // bad character shift and the good suffix shift
                        gint n = Math::max(x + 1 - lastOcc[ch & 0x7F], optoSft[x]);
                        i += Self::countChars(seq, i, n);
                        goto NEXT;
                    }
                }
                // Entire pattern matched starting at i
                matcher.first = i;
                gbool ret = next.isPresent() && next.get().match(matcher, i + lengthInChars, seq);
                if (ret) {
                    matcher.first = i;
                    matcher.groups[0] = matcher.first;
                    matcher.groups[1] = matcher.last;
                    return true;
                }
                i += Self::countChars(seq, i, 1);
            }
            matcher.hitEnd_ = true;
            return false;
        }
    } // regex
} // core
