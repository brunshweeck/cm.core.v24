//
// Created by brunshweeck on 20/11/24.
//

#include <core/lang/ArithmeticException.h>
#include <core/lang/OutOfMemoryError.h>
#include <core/lang/StringArray.h>
#include <core/regex/Matcher.h>
#include <core/text/Normalizer.h>
#include <core/util/ArrayList.h>
#include <core/util/HashMap.h>
#include <core/util/LinkedHashSet.h>
#include <meta/regex/Pattern.ASCII.h>
#include <meta/regex/Pattern.Grapheme.h>
#include <meta/regex/Pattern.Predicates.h>
#include <meta/regex/Pattern.Self.h>

namespace core {
    namespace regex {
        static CORE_FAST gint MAX_REPS = 0x7FFFFFFF;

        String Pattern::Self::normalize(String const& pattern) {
            gint plen = pattern.length();
            XString pbuf = XString(plen);
            gchar last = 0;
            gint lastStart = 0;
            gchar cc = 0;
            for (gint i = 0; i < plen;) {
                gchar c = pattern.charAt(i);
                if (cc == 0 && // top level
                    c == '\\' && i + 1 < plen && pattern.charAt(i + 1) == '\\') {
                    i += 2;
                    last = 0;
                    continue;
                }
                if (c == '[' && last != '\\') {
                    if (cc == 0) {
                        if (lastStart < i)
                            normalizeSlice(pattern, lastStart, i, pbuf);
                        lastStart = i;
                    }
                    cc++;
                } else if (c == ']' && last != '\\') {
                    cc--;
                    if (cc == 0) {
                        normalizeClazz(pattern, lastStart, i + 1, pbuf);
                        lastStart = i + 1;
                    }
                }
                last = c;
                i++;
            }
            CORE_ASSERT(cc == 0);
            if (lastStart < plen)
                normalizeSlice(pattern, lastStart, plen, pbuf);
            return pbuf.toString();
        }

        void Pattern::Self::normalizeSlice(String const& src, gint off, gint limit, XString& dst) {
            gint len = src.length();
            gint off0 = off;
            while (off < limit && ASCII::isAscii(src.charAt(off))) {
                off++;
            }
            if (off == limit) {
                dst.append(src, off0, limit);
                return;
            }
            off--;
            if (off < off0)
                off = off0;
            else
                dst.append(src, off0, off);
            while (off < limit) {
                gint ch0 = src.codePointAt(off);
                if (R"(.$|()[]{}^?*+\)"_Sl.indexOf(ch0) != -1) {
                    dst.append((gchar)ch0);
                    off++;
                    continue;
                }
                gint j = Grapheme::nextBoundary(src, off, limit);
                gint ch1;
                String seq = src.subString(off, j);
                String nfd = Normalizer::normalize(seq, Normalizer::Form::NFD);
                off = j;
                if (nfd.codePointCount(0, nfd.length()) > 1) {
                    ch0 = nfd.codePointAt(0);
                    ch1 = nfd.codePointAt(Character::charCount(ch0));
                    if (Character::category(ch1) == Character::Category::NON_SPACING_MARK) {
                        Set<String>& altns = UNSAFE::newInstance<LinkedHashSet<String>>();
                        altns.add(seq);
                        produceEquivalentAlternation(nfd, altns);
                        dst.append("(?:");
                        altns.forEach([&](String const& s) -> void { dst.append(s).append('|'); });
                        dst.remove(dst.length() - 1, dst.length());
                        dst.append(")");
                        continue;
                    }
                }
                String nfc = Normalizer::normalize(seq, Normalizer::Form::NFC);
                if (!seq.equals(nfc) && !nfd.equals(nfc))
                    dst.append("(?:" + seq + "|" + nfd + "|" + nfc + ")");
                else if (!seq.equals(nfd))
                    dst.append("(?:" + seq + "|" + nfd + ")");
                else
                    dst.append(seq);
            }
        }

        void Pattern::Self::normalizeClazz(String const& src, gint off, gint limit, XString& dst) {
            try {
                dst.append(Normalizer::normalize(src.subString(off, limit), Normalizer::Form::NFC));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Pattern::Self::produceEquivalentAlternation(String const& src, Set<String>& dst) {
            try {
                gint len = countChars(src, 0, 1);
                if (src.length() == len) {
                    dst.add(src); // source has one character.
                    return;
                }
                String base = src.subString(0, len);
                String combiningMarks = src.subString(len);
                StringArray perms = producePermutations(combiningMarks);
                // Add combined permutations
                for (gint x = 0; x < perms.length(); x++) {
                    String next = base + perms[x];
                    dst.add(next);
                    next = composeOneStep(next);
                    if (next != null) {
                        produceEquivalentAlternation(next, dst);
                    }
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        StringArray Pattern::Self::producePermutations(String const& input) {
            try {
                if (input.length() == countChars(input, 0, 1))
                    return StringArray(1, input);

                if (input.length() == countChars(input, 0, 2)) {
                    gint c0 = Character::codePointAt(input, 0);
                    gint c1 = Character::codePointAt(input, Character::charCount(c0));
                    if (getClass(c1) == getClass(c0)) {
                        return StringArray(1, input);
                    }
                    StringArray result = StringArray(2);
                    result[0] = input;
                    XString sb = XString(2);
                    sb.appendCodePoint(c1);
                    sb.appendCodePoint(c0);
                    result[1] = sb.toString();
                    return result;
                }

                /*
                 * Since
                 *      12! =   479'001'600 < Integer.MAX_VALUE
                 *      13! = 6'227'020'800 > Integer.MAX_VALUE
                 * the computation of n! using gint arithmetic will overflow iff
                 *      n < 0 or n > 12
                 *
                 * Here, nCodePoints! is computed in the next for-loop below.
                 * As nCodePoints >= 0, the computation overflows iff nCodePoints > 12.
                 * In that case, throw OOME to simulate length > Integer.MAX_VALUE.
                 */
                gint nCodePoints = countCodePoints(input);
                if (nCodePoints > 12) {
                    OutOfMemoryError("Pattern too complex").throws($ftrace());
                }

                /* Compute length = nCodePoints! */
                gint length = 1;
                for (gint x = 2; x <= nCodePoints; ++x) {
                    length *= x;
                }
                StringArray temp = StringArray(length);

                IntArray combClass = IntArray(nCodePoints);
                for (gint x = 0, i = 0; x < nCodePoints; x++) {
                    gint c = Character::codePointAt(input, i);
                    combClass[x] = getClass(c);
                    i += Character::charCount(c);
                }

                // For each char, take it out and add the permutations
                // of the remaining chars
                gint index = 0;
                gint len;
                // offset maintains the index in code units.
            loop:
                for (gint x = 0, offset = 0; x < nCodePoints; x++, offset += len) {
                    len = countChars(input, offset, 1);
                    for (gint y = x - 1; y >= 0; y--) {
                        if (combClass[y] == combClass[x]) {
                            goto loop;
                        }
                    }
                    XString sb = XString(input);
                    String otherChars = sb.remove(offset, offset + len).toString();
                    StringArray subResult = producePermutations(otherChars);

                    String prefix = input.subString(offset, offset + len);
                    for (String const& sre : subResult)
                        temp[index++] = prefix + sre;
                }
                StringArray result = StringArray(index);
                Arrays::copy(temp, 0, result, 0, index);
                return result;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Pattern::Self::getClass(gint c) {
            return 0;
        }

        String Pattern::Self::composeOneStep(String const& input) {
            try {
                gint len = countChars(input, 0, 2);
                String firstTwoCharacters = input.subString(0, len);
                String result = Normalizer::normalize(firstTwoCharacters, Normalizer::Form::NFC);
                if (result.equals(firstTwoCharacters))
                    return "";
                String remainder = input.subString(len);
                return result + remainder;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Pattern::Self::RemoveQEQuoting() {
            // try {  } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pLen = patternLength;
            gint i = 0;
            while (i < pLen - 1) {
                if (temp[i] != '\\')
                    i += 1;
                else if (temp[i + 1] != 'Q')
                    i += 2;
                else
                    break;
            }
            if (i >= pLen - 1) // No \Q sequence found
                return;
            gint j = i;
            i += 2;
            gint newTempLen;
            try {
                newTempLen = Math::addExact(j + 2, Math::multiplyExact(3, pLen - i));
            } catch (ArithmeticException const& _) {
                OutOfMemoryError("Required pattern length too large").throws($ftrace());
            }
            IntArray newtemp = IntArray(newTempLen);
            Arrays::copy(temp, 0, newtemp, 0, j);

            gbool inQuote = true;
            gbool beginQuote = true;
            while (i < pLen) {
                gint c = temp[i++];
                if (!ASCII::isAscii(c) || ASCII::isAlpha(c)) {
                    newtemp[j++] = c;
                } else if (ASCII::isDigit(c)) {
                    if (beginQuote) {
                        /*
                         * A unicode escape \[0xu] could be before this quote,
                         * and we don't want this numeric char to processed as
                         * part of the escape.
                         */
                        newtemp[j++] = '\\';
                        newtemp[j++] = 'x';
                        newtemp[j++] = '3';
                    }
                    newtemp[j++] = c;
                } else if (c != '\\') {
                    if (inQuote) newtemp[j++] = '\\';
                    newtemp[j++] = c;
                } else if (inQuote) {
                    if (temp[i] == 'E') {
                        i++;
                        inQuote = false;
                    } else {
                        newtemp[j++] = '\\';
                        newtemp[j++] = '\\';
                    }
                } else {
                    if (temp[i] == 'Q') {
                        i++;
                        inQuote = true;
                        beginQuote = true;
                        continue;
                    } else {
                        newtemp[j++] = c;
                        if (i != pLen)
                            newtemp[j++] = temp[i++];
                    }
                }

                beginQuote = false;
            }

            patternLength = j;
            temp = Arrays::copyOf(newtemp, j + 2); // double zero termination
        }

        void Pattern::Self::compile(String const& regex) {
            // try {  } catch (Throwable const& ex) { ex.throws($ftrace()); }
            // Handle canonical equivalences
            if (has(CANON_EQ) && !has(LITERAL)) {
                pattern = normalize(regex);
            } else {
                pattern = regex;
            }
            patternLength = pattern.length();

            // Copy pattern to gint array for convenience
            // Use double zero to terminate pattern
            temp = IntArray(patternLength + 2);

            hasSupplementary = false;
            gint c, count = 0;
            // Convert all chars into code points
            for (gint x = 0; x < patternLength; x += Character::charCount(c)) {
                c = pattern.codePointAt(x);
                if (isSupplementary(c)) {
                    hasSupplementary = true;
                }
                temp[count++] = c;
            }

            patternLength = count; // patternLength now in code points

            if (!has(LITERAL))
                RemoveQEQuoting();

            // Allocate all temporary objects here.
            buffer = IntArray(32);
            CORE_IGNORE_DEPRECATIONS(groupNodes = Array<GroupHead>(10);)
            namedGroups = { };
            topClosureNodes = UNSAFE::newInstance<ArrayList<Node>>(10);

            if (has(LITERAL)) {
                // Literal pattern handling
                matchRoot = newSlice(temp, patternLength, hasSupplementary);
                matchRoot.get().next = lastAccept;
            } else {
                // Start recursive descent parsing
                try { matchRoot = expr(lastAccept); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                // Check extra pattern characters
                if (patternLength != position) {
                    if (peek() == ')') {
                        error("Unmatched closing ')'").throws($ftrace());
                    }
                    if (position == patternLength + 1 && temp[patternLength - 1] == '\\') {
                        error("Unescaped trailing backslash").throws($ftrace());
                    }
                    {
                        error("Unexpected internal error").throws($ftrace());
                    }
                }
            }

            // Peephole optimization
            if (Class<Slice>::hasInstance(matchRoot.get())) {
                root = BnM::optimize(matchRoot);
                if (root == matchRoot) {
                    if (hasSupplementary)
                        root = UNSAFE::newInstance<StartS>(matchRoot);
                    else
                        root = UNSAFE::newInstance<Start>(matchRoot);
                }
            } else if (!Class<Begin>::hasInstance(matchRoot.get()) && !Class<First>::hasInstance(matchRoot.get()))
                if (hasSupplementary)
                    root = UNSAFE::newInstance<StartS>(matchRoot);
                else
                    root = UNSAFE::newInstance<Start>(matchRoot);
            else
                root = matchRoot;

            // Optimize the greedy Loop to prevent exponential backtracking, IF there
            // is no group ref in this pattern. With a non-negative localTCNCount value,
            // the greedy type Loop, Curly will skip the backtracking for any starting
            // position "i" that failed in the past.
            if (!hasGroupRef) {
                for (Node& node : topClosureNodes.get()) {
                    if (Class<Loop>::hasInstance(node)) {
                        // non-deterministic-greedy-group
                        ((Loop&)node).posIndex = localTCNCount++;
                    }
                }
            }

            // Release temporary storage
            temp = { };
            buffer = { };
            groupNodes = { };
            patternLength = 0;
            compiled = true;
            UNSAFE::deleteRegInstance(topClosureNodes.get());
            topClosureNodes = { };
        }

        Optional<Map<String, Integer>> Pattern::Self::namedGroupsMap() {
            Optional<Map<String, Integer>> groups = namedGroups;
            try {
                if (groups.isEmpty())
                    groups = namedGroups = UNSAFE::newInstance<HashMap<String, Integer>>();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return groups;
        }

        gbool Pattern::Self::has(gint f) const {
            return (flags & f) != 0;
        }

        void Pattern::Self::accept(gint ch, String const& s) {
            gint testChar = temp[position++];
            if (has(COMMENTS))
                testChar = parsePastWhitespace(testChar);
            if (ch != testChar) {
                error(s).throws($ftrace());
            }
        }

        void Pattern::Self::mark(gint c) {
            temp[patternLength] = c;
        }

        gint Pattern::Self::peek() {
            gint ch = temp[position];
            if (has(COMMENTS))
                ch = peekPastWhitespace(ch);
            return ch;
        }

        gint Pattern::Self::read() {
            gint ch = temp[position++];
            if (has(COMMENTS))
                ch = parsePastWhitespace(ch);
            return ch;
        }

        gint Pattern::Self::readEscaped() {
            gint ch = temp[position++];
            return ch;
        }

        gint Pattern::Self::next() {
            gint ch = temp[++position];
            if (has(COMMENTS))
                ch = peekPastWhitespace(ch);
            return ch;
        }

        gint Pattern::Self::nextEscaped() {
            gint ch = temp[++position];
            return ch;
        }

        gint Pattern::Self::peekPastWhitespace(gint ch) {
            while (ASCII::isSpace(ch) || ch == '#') {
                while (ASCII::isSpace(ch))
                    ch = temp[++position];
                if (ch == '#') {
                    ch = peekPastLine();
                }
            }
            return ch;
        }

        gint Pattern::Self::parsePastWhitespace(gint ch) {
            while (ASCII::isSpace(ch) || ch == '#') {
                while (ASCII::isSpace(ch))
                    ch = temp[position++];
                if (ch == '#')
                    ch = parsePastLine();
            }
            return ch;
        }

        gint Pattern::Self::parsePastLine() {
            gint ch = temp[position++];
            while (ch != 0 && !isLineSeparator(ch))
                ch = temp[position++];
            if (ch == 0 && position > patternLength) {
                position = patternLength;
                ch = temp[position++];
            }
            return ch;
        }

        gint Pattern::Self::peekPastLine() {
            gint ch = temp[++position];
            while (ch != 0 && !isLineSeparator(ch))
                ch = temp[++position];
            if (ch == 0 && position > patternLength) {
                position = patternLength;
                ch = temp[position];
            }
            return ch;
        }

        gbool Pattern::Self::isLineSeparator(gint ch) const {
            if (has(UNIX_LINES))
                return ch == '\n';

            return (ch == '\n' ||
                ch == '\r' ||
                (ch | 1) == u'\u2029' ||
                ch == u'\u0085');
        }

        gint Pattern::Self::skip() {
            gint i = position;
            gint ch = temp[i + 1];
            position = i + 2;
            return ch;
        }

        void Pattern::Self::unread() {
            position--;
        }

        PatternSyntaxException Pattern::Self::error(String const& s) const {
            return PatternSyntaxException(s, pattern, position - 1);
        }

        gbool Pattern::Self::findSupplementary(gint start, gint end) {
            for (gint i = start; i < end; i++) {
                if (isSupplementary(temp[i]))
                    return true;
            }
            return false;
        }

        gbool Pattern::Self::isSupplementary(gint ch) {
            return ch >= Character::MIN_SUPPLEMENTARY ||
                    Character::isSurrogate((gchar)ch);
        }

        Optional<Pattern::Node> Pattern::Self::expr(const Optional<Node>& end) {
            Optional<Node> prev = { };
            Optional<Node> firstTail = { };
            Optional<Branch> branch = { };
            Optional<Node> branchConn = { };

            for (;;) {
                Optional<Node> node;
                try { node = sequence(end); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                Optional<Node> nodeTail = root; //double return
                if (prev.isEmpty()) {
                    prev = node;
                    firstTail = nodeTail;
                } else {
                    // Branch
                    if (branchConn.isEmpty()) {
                        branchConn = UNSAFE::newInstance<BranchConn>();
                        branchConn.get().next = end;
                    }
                    if (node == end) {
                        // if the node returned from sequence() is "end"
                        // we have an empty expr, set a null atom into
                        // the branch to indicate to go "next" directly.
                        node = { };
                    } else {
                        // the "tail.next" of each atom goes to branchConn
                        nodeTail.get().next = branchConn;
                    }
                    if (prev == branch) {
                        branch.get().addNode(node);
                    } else {
                        if (prev == end) {
                            prev = { };
                        } else {
                            // replace the "end" with "branchConn" at its tail.next
                            // when put the "prev" into the branch as the first atom.
                            firstTail.get().next = branchConn;
                        }
                        prev = branch = UNSAFE::newInstance<Branch>(prev, node, branchConn);
                    }
                }
                if (peek() != '|') {
                    return prev;
                }
                next();
            }
        }

        Optional<Pattern::Node> Pattern::Self::sequence(const Optional<Node>& end) {
            Optional<Node> head = { };
            Optional<Node> tail = { };
            Optional<Node> node;
            for (;;) {
                gint ch = peek();
                switch (ch) {
                    case '(':
                        // Because group handles its own closure,
                        // we need to treat it differently
                        try { node = group0(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    // Check for comment or flag group
                        if (node.isEmpty())
                            continue;
                        if (head.isEmpty())
                            head = node;
                        else
                            tail.get().next = node;
                    // Double return: Tail was returned in root
                        tail = root;
                        continue;
                    case '[':
                        if (has(CANON_EQ) && !has(LITERAL))
                            node = UNSAFE::newInstance<NFCCharProperty>(clazz(true));
                        else
                            node = newCharProperty(clazz(true));
                        break;
                    case '\\':
                        ch = nextEscaped();
                        if (ch == 'p' || ch == 'P') {
                            gbool oneLetter = true;
                            gbool comp = (ch == 'P');
                            ch = next(); // Consume { if present
                            if (ch != '{') {
                                unread();
                            } else {
                                oneLetter = false;
                            }
                            // node = newCharProperty(family(oneLetter, comp));
                            if (has(CANON_EQ) && !has(LITERAL))
                                node = UNSAFE::newInstance<NFCCharProperty>(family(oneLetter, comp));
                            else
                                node = newCharProperty(family(oneLetter, comp));
                        } else {
                            unread();
                            node = atom();
                        }
                        break;
                    case '^':
                        next();
                        if (has(MULTILINE)) {
                            if (has(UNIX_LINES))
                                node = UNSAFE::newInstance<UnixCaret>();
                            else
                                node = UNSAFE::newInstance<Caret>();
                        } else {
                            node = UNSAFE::newInstance<Begin>();
                        }
                        break;
                    case '$':
                        next();
                        if (has(UNIX_LINES))
                            node = UNSAFE::newInstance<UnixDollar>(has(MULTILINE));
                        else
                            node = UNSAFE::newInstance<Dollar>(has(MULTILINE));
                        break;
                    case '.':
                        next();
                        if (has(DOTALL)) {
                            node = UNSAFE::newInstance<CharProperty>(ALL());
                        } else {
                            if (has(UNIX_LINES)) {
                                node = UNSAFE::newInstance<CharProperty>(UNIXDOT());
                            } else {
                                node = UNSAFE::newInstance<CharProperty>(DOT());
                            }
                        }
                        break;
                    case '|':
                    case ')':
                        goto BREAK_LOOP;
                    case ']': // Now interpreting dangling ] and } as literals
                    case '}':
                        node = atom();
                        break;
                    case '?':
                    case '*':
                    case '+':
                        next();
                        error("Dangling meta character '"_Sl + ((gchar)ch) + "'").throws($ftrace());
                    case 0:
                        if (position >= patternLength) {
                            goto BREAK_LOOP;
                        }
                    // Fall through
                    default:
                        node = atom();
                        break;
                }

                node = closure(node);
                /* save the top dot-greedy nodes (.*, .+) as well
                if (node instanceof GreedyCharProperty &&
                    ((GreedyCharProperty)node).cp instanceof Dot) {
                    topClosureNodes.add(node);
                }
                */
                if (head.isEmpty()) {
                    head = tail = node;
                } else {
                    tail.get().next = node;
                    tail = node;
                }
            }
        BREAK_LOOP:
            if (head.isEmpty()) {
                return end;
            }
            tail.get().next = end;
            root = tail; //double return
            return head;
        }

        Optional<Pattern::Node> Pattern::Self::atom() {
            gint first = 0;
            gint prev = -1;
            gbool hasSupplementary = false;
            gint ch = peek();
            for (;;) {
                switch (ch) {
                    case '*':
                    case '+':
                    case '?':
                    case '{':
                        if (first > 1) {
                            position = prev; // Unwind one character
                            first--;
                        }
                        break;
                    case '$':
                    case '.':
                    case '^':
                    case '(':
                    case '[':
                    case '|':
                    case ')':
                        break;
                    case '\\':
                        ch = nextEscaped();
                        if (ch == 'p' || ch == 'P') {
                            // Property
                            if (first > 0) {
                                // Slice is waiting; handle it first
                                unread();
                                break;
                            } else {
                                // No slice; just return the family node
                                gbool comp = (ch == 'P');
                                gbool oneLetter = true;
                                ch = next(); // Consume { if present
                                if (ch != '{')
                                    unread();
                                else
                                    oneLetter = false;
                                if (has(CANON_EQ) && !has(LITERAL))
                                    return UNSAFE::newInstance<NFCCharProperty>(family(oneLetter, comp));
                                return newCharProperty(family(oneLetter, comp));
                            }
                        }
                        unread();
                        prev = position;
                        ch = escape(false, first == 0, false);
                        if (ch >= 0) {
                            append(ch, first);
                            first++;
                            if (isSupplementary(ch)) {
                                hasSupplementary = true;
                            }
                            ch = peek();
                            continue;
                        } else if (first == 0) {
                            return root;
                        }
                    // Unwind meta escape sequence
                        position = prev;
                        break;
                    case 0:
                        if (position >= patternLength) {
                            break;
                        }
                    // Fall through
                    default:
                        prev = position;
                        append(ch, first);
                        first++;
                        if (isSupplementary(ch)) {
                            hasSupplementary = true;
                        }
                        ch = next();
                        continue;
                }
                break;
            }
            if (first == 1)
                return newCharProperty(single(buffer[0]));

            return newSlice(buffer, first, hasSupplementary);
        }

        void Pattern::Self::append(gint ch, gint index) {
            gint len = buffer.length();
            if (index - len >= 0) {
                len = Arrays::newLength(len,
                                        1 + index - len, /* minimum growth */
                                        len /* preferred growth */);
                buffer = Arrays::copyOf(buffer, len);
            }
            buffer[index] = ch;
        }

        Optional<Pattern::Node> Pattern::Self::ref(gint refNum) {
            gbool done = false;
            while (!done) {
                gint ch = peek();
                switch (ch) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        gint newRefNum = (refNum * 10) + (ch - '0');
                        // Add another number if it doesn't make a group
                        // that doesn't exist
                        if (capturingGroupCount - 1 < newRefNum) {
                            done = true;
                            break;
                        }
                        refNum = newRefNum;
                        read();
                    }
                    default:
                        done = true;
                }
            }
            hasGroupRef = true;
            if (has(CASE_INSENSITIVE))
                return UNSAFE::newInstance<CIBackRef>(refNum, has(UNICODE_CASE));
            return UNSAFE::newInstance<BackRef>(refNum);
        }

        gint Pattern::Self::escape(gbool inclass, gbool create, gbool isrange) {
            gint ch = skip();
            switch (ch) {
                case '0':
                    return o();
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    if (inclass) break;
                    if (create) {
                        root = ref((ch - '0'));
                    }
                    return -1;
                case 'A':
                    if (inclass) break;
                    if (create) root = UNSAFE::newInstance<Begin>();
                    return -1;
                case 'B':
                    if (inclass) break;
                    if (create) root = UNSAFE::newInstance<Bound>(Bound::NONE, has(UNICODE_CHARACTER_CLASS));
                    return -1;
                case 'C':
                    break;
                case 'D':
                    if (create) {
                        predicate = has(UNICODE_CHARACTER_CLASS) ? Predicates::DIGIT() : Predicates::ASCII_DIGIT();
                        predicate = predicate.get().negate();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'E':
                case 'F':
                    break;
                case 'G':
                    if (inclass) break;
                    if (create) root = UNSAFE::newInstance<LastMatch>();
                    return -1;
                case 'H':
                    if (create) {
                        predicate = HorizWS().get().negate();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'I':
                case 'J':
                case 'K':
                case 'L':
                case 'M':
                    break;
                case 'N':
                    return N();
                case 'O':
                case 'P':
                case 'Q':
                    break;
                case 'R':
                    if (inclass) break;
                    if (create) root = UNSAFE::newInstance<LineEnding>();
                    return -1;
                case 'S':
                    if (create) {
                        predicate = has(UNICODE_CHARACTER_CLASS)
                                        ? Predicates::WHITE_SPACE()
                                        : Predicates::ASCII_SPACE();
                        predicate = predicate.get().negate();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'T':
                case 'U':
                    break;
                case 'V':
                    if (create) {
                        predicate = VertWS().get().negate();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'W':
                    if (create) {
                        predicate = has(UNICODE_CHARACTER_CLASS) ? Predicates::WORD() : Predicates::ASCII_WORD();
                        predicate = predicate.get().negate();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'X':
                    if (inclass) break;
                    if (create) {
                        root = UNSAFE::newInstance<XGrapheme>();
                    }
                    return -1;
                case 'Y':
                    break;
                case 'Z':
                    if (inclass) break;
                    if (create) {
                        if (has(UNIX_LINES))
                            root = UNSAFE::newInstance<UnixDollar>(false);
                        else
                            root = UNSAFE::newInstance<Dollar>(false);
                    }
                    return -1;
                case 'a':
                    return '\007';
                case 'b':
                    if (inclass) break;
                    if (create) {
                        if (peek() == '{') {
                            if (skip() == 'g') {
                                if (read() == '}') {
                                    root = UNSAFE::newInstance<GraphemeBound>();
                                    return -1;
                                }
                                break; // error missing trailing }
                            }
                            unread();
                            unread();
                        }
                        root = UNSAFE::newInstance<Bound>(Bound::BOTH, has(UNICODE_CHARACTER_CLASS));
                    }
                    return -1;
                case 'c':
                    return c();
                case 'd':
                    if (create) {
                        predicate = has(UNICODE_CHARACTER_CLASS) ? Predicates::DIGIT() : Predicates::ASCII_DIGIT();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'e':
                    return '\033';
                case 'f':
                    return '\f';
                case 'g':
                    break;
                case 'h':
                    if (create) {
                        predicate = HorizWS();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'i':
                case 'j':
                    break;
                case 'k': {
                    if (inclass)
                        break;
                    if (read() != '<')
                        error("\\k is not followed by '<' for named capturing group").throws($ftrace());
                    String name = groupname(read());
                    Object& number = namedGroupsMap().get().getOrNull(name);
                    if (number == null)
                        error("named capturing group <" + name + "> does not exist").throws($ftrace());
                    if (create) {
                        hasGroupRef = true;
                        if (has(CASE_INSENSITIVE))
                            root = UNSAFE::newInstance<CIBackRef>(CORE_XCAST(Integer, number), has(UNICODE_CASE));
                        else
                            root = UNSAFE::newInstance<BackRef>(CORE_XCAST(Integer, number));
                    }
                    return -1;
                }
                case 'l':
                case 'm':
                    break;
                case 'n':
                    return '\n';
                case 'o':
                case 'p':
                case 'q':
                    break;
                case 'r':
                    return '\r';
                case 's':
                    if (create) {
                        predicate = has(UNICODE_CHARACTER_CLASS)
                                        ? Predicates::WHITE_SPACE()
                                        : Predicates::ASCII_SPACE();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 't':
                    return '\t';
                case 'u':
                    return u();
                case 'v':
                    // '\v' was implemented as VT/0x0B in releases < 1.8 (though
                    // undocumented). In JDK8 '\v' is specified as a predefined
                    // character class for all vertical whitespace characters.
                    // So [-1, root=VertWS node] pair is returned (instead of a
                    // single 0x0B). This breaks the range if '\v' is used as
                    // the start or end value, such as [\v-...] or [...-\v], in
                    // which a single definite value (0x0B) is expected. For
                    // compatibility concern '\013'/0x0B is returned if isrange.
                    if (isrange)
                        return '\013';
                    if (create) {
                        predicate = VertWS();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'w':
                    if (create) {
                        predicate = has(UNICODE_CHARACTER_CLASS) ? Predicates::WORD() : Predicates::ASCII_WORD();
                        if (!inclass)
                            root = newCharProperty(predicate);
                    }
                    return -1;
                case 'x':
                    return x();
                case 'y':
                    break;
                case 'z':
                    if (inclass) break;
                    if (create) root = UNSAFE::newInstance<End>();
                    return -1;
                default:
                    return ch;
            }
            error("Illegal/unsupported escape sequence").throws($ftrace());
        }

        Optional<Pattern::CharPredicate> Pattern::Self::clazz(gbool consume) {
            Optional<Pattern::CharPredicate> prev = { };
            Optional<Pattern::CharPredicate> curr = { };
            Optional<BitClass> bits = UNSAFE::newInstance<BitClass>();

            gbool isNeg = false;
            gbool hasBits = false;
            gint ch = next();

            // Negates if first char in a class, otherwise literal
            if (ch == '^' && temp[position - 1] == '[') {
                ch = next();
                isNeg = true;
            }
            for (;;) {
                switch (ch) {
                    case '[':
                        curr = clazz(true);
                        if (prev.isEmpty())
                            prev = curr;
                        else
                            prev = prev.get().logicalOr(curr);
                        ch = peek();
                        continue;
                    case '&':
                        ch = next();
                        if (ch == '&') {
                            ch = next();
                            Optional<Pattern::CharPredicate> right = { };
                            while (ch != ']' && ch != '&') {
                                if (ch == '[') {
                                    if (right.isEmpty())
                                        right = clazz(true);
                                    else
                                        right = right.get().logicalOr(clazz(true));
                                } else {
                                    // abc&&def
                                    unread();
                                    if (right.isEmpty()) {
                                        right = clazz(false);
                                    } else {
                                        right = right.get().logicalOr(clazz(false));
                                    }
                                }
                                ch = peek();
                            }
                            if (hasBits) {
                                // bits used, union has high precedence
                                if (prev.isEmpty()) {
                                    prev = curr = bits;
                                } else {
                                    prev = prev.get().logicalOr(bits);
                                }
                                hasBits = false;
                            }
                            if (right != null)
                                curr = right;
                            if (prev.isEmpty()) {
                                if (right.isEmpty())
                                    error("Bad class syntax").throws($ftrace());
                                else
                                    prev = right;
                            } else {
                                if (curr.isEmpty())
                                    error("Bad intersection syntax").throws($ftrace());
                                prev = prev.get().logicalAnd(curr);
                            }
                        } else {
                            // treat as a literal &
                            unread();
                            break;
                        }
                        continue;
                    case 0:
                        if (position >= patternLength)
                            error("Unclosed character class").throws($ftrace());
                        break;
                    case ']':
                        if (prev != null || hasBits) {
                            if (consume)
                                next();
                            if (prev.isEmpty())
                                prev = bits;
                            else if (hasBits)
                                prev = prev.get().logicalOr(bits);
                            if (isNeg)
                                return prev.get().negate();
                            return prev;
                        }
                        break;
                    default:
                        break;
                }
                curr = range(bits);
                if (curr.isEmpty()) {
                    // the bits used
                    hasBits = true;
                } else {
                    if (prev.isEmpty())
                        prev = curr;
                    else if (prev != curr)
                        prev = prev.get().logicalOr(curr);
                }
                ch = peek();
            }
        }

        Optional<Pattern::CharPredicate> Pattern::Self::bitsOrSingle(Optional<BitClass> bits, gint ch) const {
            /* Bits can only handle codepoints in [u+0000-u+00ff] range.
           Use "single" node instead of bits when dealing with unicode
           case folding for codepoints listed below.
           (1)Uppercase out of range: u+00ff, u+00b5
              toUpperCase(u+00ff) -> u+0178
              toUpperCase(u+00b5) -> u+039c
           (2)LatinSmallLetterLongS u+17f
              toUpperCase(u+017f) -> u+0053
           (3)LatinSmallLetterDotlessI u+131
              toUpperCase(u+0131) -> u+0049
           (4)LatinCapitalLetterIWithDotAbove u+0130
              toLowerCase(u+0130) -> u+0069
           (5)KelvinSign u+212a
              toLowerCase(u+212a) ==> u+006B
           (6)AngstromSign u+212b
              toLowerCase(u+212b) ==> u+00e5
        */
            if (ch < 256 &&
                !(has(CASE_INSENSITIVE) && has(UNICODE_CASE) &&
                    (ch == 0xff || ch == 0xb5 ||
                        ch == 0x49 || ch == 0x69 || //I and i
                        ch == 0x53 || ch == 0x73 || //S and s
                        ch == 0x4b || ch == 0x6b || //K and k
                        ch == 0xc5 || ch == 0xe5))) {
                //A+ring
                bits.get().add(ch, flags);
                return { };
            }
            return single(ch);
        }

        Optional<Pattern::CharPredicate> Pattern::Self::single(gint ch) const {
            if (has(CASE_INSENSITIVE)) {
                gint lower, upper;
                if (has(UNICODE_CASE)) {
                    upper = Character::toUpperCase(ch);
                    lower = Character::toLowerCase(upper);
                    // Unicode case insensitive matches
                    if (upper != lower)
                        return SingleU(lower);
                } else if (ASCII::isAscii(ch)) {
                    lower = ASCII::toLower(ch);
                    upper = ASCII::toUpper(ch);
                    // Case insensitive matches a given BMP character
                    if (lower != upper)
                        return SingleI(lower, upper);
                }
            }
            if (isSupplementary(ch))
                return SingleS(ch);
            return Single(ch); // Match a given BMP character
        }

        Optional<Pattern::CharPredicate> Pattern::Self::range(const Optional<BitClass>& bits) {
            gint ch = peek();
            if (ch == '\\') {
                ch = nextEscaped();
                if (ch == 'p' || ch == 'P') {
                    // A property
                    gbool comp = (ch == 'P');
                    gbool oneLetter = true;
                    // Consume { if present
                    ch = next();
                    if (ch != '{')
                        unread();
                    else
                        oneLetter = false;
                    return family(oneLetter, comp);
                } else {
                    // ordinary escape
                    gbool isrange = temp[position + 1] == '-';
                    unread();
                    ch = escape(true, true, isrange);
                    if (ch == -1)
                        return predicate;
                }
            } else {
                next();
            }
            if (ch >= 0) {
                if (peek() == '-') {
                    gint endRange = temp[position + 1];
                    if (endRange == '[') {
                        return bitsOrSingle(bits, ch);
                    }
                    if (endRange != ']') {
                        next();
                        gint m = peek();
                        if (m == '\\') {
                            m = escape(true, false, true);
                        } else {
                            next();
                        }
                        if (m < ch) {
                            error("Illegal character range").throws($ftrace());
                        }
                        if (has(CASE_INSENSITIVE)) {
                            if (has(UNICODE_CASE))
                                return CIRangeU(ch, m);
                            return CIRange(ch, m);
                        } else {
                            return Range(ch, m);
                        }
                    }
                }
                return bitsOrSingle(bits, ch);
            }
            error("Unexpected character '"_Sl + ((gchar)ch) + "'").throws($ftrace());
        }

        Optional<Pattern::CharPredicate> Pattern::Self::family(gbool singleLetter, gbool isComplement) {
            next();
            String name;
            Optional<Pattern::CharPredicate> p = { };

            if (singleLetter) {
                gint c = temp[position];
                if (!Character::isSupplementary(c)) {
                    name = String::valueOf((gchar)c);
                } else {
                    name = String(temp, position, 1);
                }
                read();
            } else {
                gint i = position;
                mark('}');
                while (read() != '}') {}
                mark('\000');
                gint j = position;
                if (j > patternLength)
                    error("Unclosed character family").throws($ftrace());
                if (i + 1 >= j)
                    error("Empty character family").throws($ftrace());
                name = String(temp, i, j - i - 1);
            }

            gint i = name.indexOf('=');
            if (i != -1) {
                // property construct \p{name=value}
                String value = name.subString(i + 1);
                name = name.subString(0, i).toLowerCase(Locale::ENGLISH);
                if (name == "sc" || name == "script")
                    p = Predicates::forUnicodeScript(value);
                else if (name == "blk" || name == "block")
                    p = Predicates::forUnicodeBlock(value);
                else if (name == "gc" || name == "general_category")
                    p = Predicates::forProperty(value, has(CASE_INSENSITIVE));
                if (p.isEmpty())
                    error("Unknown Unicode property {name=<" + name + ">, "
                        + "value=<" + value + ">}").throws($ftrace());
            } else {
                if (name.startsWith("In")) {
                    // \p{InBlockName}
                    p = Predicates::forUnicodeBlock(name.subString(2));
                } else if (name.startsWith("Is")) {
                    // \p{IsGeneralCategory} and \p{IsScriptName}
                    String shortName = name.subString(2);
                    p = Predicates::forUnicodeProperty(shortName, has(CASE_INSENSITIVE));
                    if (p.isEmpty())
                        p = Predicates::forProperty(shortName, has(CASE_INSENSITIVE));
                    if (p.isEmpty())
                        p = Predicates::forUnicodeScript(shortName);
                } else {
                    if (has(UNICODE_CHARACTER_CLASS))
                        p = Predicates::forPOSIXName(name, has(CASE_INSENSITIVE));
                    if (p.isEmpty())
                        p = Predicates::forProperty(name, has(CASE_INSENSITIVE));
                }
                if (p.isEmpty())
                    error("Unknown character property name {" + name + "}").throws($ftrace());
            }
            if (isComplement) {
                // it might be too expensive to detect if a complement of
                // CharProperty can match "certain" supplementary. So just
                // go with StartS.
                hasSupplementary = true;
                p = p.get().negate();
            }
            return p;
        }

        Optional<Pattern::CharProperty> Pattern::Self::newCharProperty(const Optional<Pattern::CharPredicate>& p) {
            if (p.isEmpty())
                return { };
            if (Class<BmpCharPredicate>::hasInstance(p.get()))
                return UNSAFE::newInstance<BmpCharProperty>(p);
            else {
                hasSupplementary = true;
                return UNSAFE::newInstance<CharProperty>(p);
            }
        }

        String Pattern::Self::groupname(gint ch) {
            XString sb = XString();
            if (!ASCII::isAlpha(ch))
                error("capturing group name does not start with a Latin letter").throws($ftrace());
            do {
                sb.append((gchar)ch);
            } while (ASCII::isAlnum(ch = read()));
            if (ch != '>')
                error("named capturing group is missing trailing '>'").throws($ftrace());
            return sb.toString();
        }

        Optional<Pattern::Node> Pattern::Self::group0() {
            gbool capturingGroup = false;
            Optional<Node> head;
            Optional<Node> tail;
            gint save = flags;
            gint saveTCNCount = topClosureNodes.get().size();
            root = { };
            gint ch = next();
            if (ch == '?') {
                ch = skip();
                switch (ch) {
                    case ':': {
                        //  (?:xxx) pure group
                        head = createGroup(true);
                        tail = root;
                        head.get().next = expr(tail);
                    }
                    case '=':
                    case '!': {
                        // (?=xxx) and (?!xxx) lookahead
                        head = createGroup(true);
                        tail = root;
                        head.get().next = expr(tail);
                        if (ch == '=') {
                            head = tail = UNSAFE::newInstance<Pos>(head);
                        } else {
                            head = tail = UNSAFE::newInstance<Neg>(head);
                        }
                    }
                    case '>': {
                        // (?>xxx)  independent group
                        head = createGroup(true);
                        tail = root;
                        head.get().next = expr(tail);
                        head = tail = UNSAFE::newInstance<Ques>(head, Qtype::INDEPENDENT);
                    }
                    case '<': {
                        // (?<xxx)  look behind
                        ch = read();
                        if (ch != '=' && ch != '!') {
                            // named captured group
                            String name = groupname(ch);
                            if (namedGroupsMap().get().containsKey(name))
                                error("Named capturing group <" + name
                                    + "> is already defined").throws($ftrace());
                            capturingGroup = true;
                            head = createGroup(false);
                            tail = root;
                            namedGroupsMap().get().put(name, capturingGroupCount - 1);
                            head.get().next = expr(tail);
                            break;
                        }
                        gint start = position;
                        head = createGroup(true);
                        tail = root;
                        head.get().next = expr(tail);
                        tail.get().next = LookBehindEndNode::INSTANCE;
                        TreeInfo info = TreeInfo();
                        head.get().study(info);
                        if (!info.maxValid) {
                            error("Look-behind group does not have "
                                "an obvious maximum length").throws($ftrace());
                        }
                        gbool hasSupplementary = findSupplementary(start, patternLength);
                        if (ch == '=') {
                            head = tail = (hasSupplementary
                                               ? UNSAFE::newInstance<BehindS>(head, info.maxLength,
                                                                              info.minLength)
                                               : UNSAFE::newInstance<Behind>(head, info.maxLength,
                                                                             info.minLength));
                        } else {
                            // if (ch == '!')
                            head = tail = (hasSupplementary
                                               ? UNSAFE::newInstance<NotBehindS>(head, info.maxLength,
                                                   info.minLength)
                                               : UNSAFE::newInstance<NotBehind>(head, info.maxLength,
                                                   info.minLength));
                        }
                        // clear all top-closure-nodes inside lookbehind
                        if (saveTCNCount < topClosureNodes.get().size())
                            topClosureNodes.get().subList(saveTCNCount, topClosureNodes.get().size()).clear();
                    }
                    case '$':
                    case '@':
                        error("Unknown group type").throws($ftrace());
                    default: {
                        // (?xxx:) inlined match flags
                        unread();
                        addFlag();
                        ch = read();
                        if (ch == ')') {
                            return { }; // Inline modifier only
                        }
                        if (ch != ':') {
                            error("Unknown inline modifier").throws($ftrace());
                        }
                        head = createGroup(true);
                        tail = root;
                        head.get().next = expr(tail);
                    }
                }
            } else {
                // (xxx) a regular group
                capturingGroup = true;
                try { head = createGroup(false); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                tail = root;
                head.get().next = expr(tail);
            }

            accept(')', "Unclosed group");
            flags = save;

            // Check for quantifiers
            Optional<Node> node = closure(head);
            if (node == head) { // No closure
                root = tail;
                return node; // Dual return
            }
            if (head == tail) { // Zero length assertion
                root = node;
                return node; // Dual return
            }

            // have group closure, clear all inner closure nodes from the
            // top list (no backtracking stopper optimization for inner
            if (saveTCNCount < topClosureNodes.get().size())
                topClosureNodes.get().subList(saveTCNCount, topClosureNodes.get().size()).clear();

            if (Class<Ques>::hasInstance(node.get())) {
                Optional<Ques> ques = (Optional<Ques>)node;
                if (ques.get().type == Qtype::POSSESSIVE) {
                    root = node;
                    return node;
                }
                tail.get().next = UNSAFE::newInstance<BranchConn>();
                tail = (Optional<Node>)tail.get().next;
                if (ques.get().type == Qtype::GREEDY) {
                    head = UNSAFE::newInstance<Branch>(head, Optional<Node>(), tail);
                } else { // Reluctant quantifier
                    head = UNSAFE::newInstance<Branch>(Optional<Node>(), head, tail);
                }
                root = tail;
                return head;
            } else if (Class<Curly>::hasInstance(node)) {
                Optional<Curly> curly = (Optional<Curly>)node;
                if (curly.get().type == Qtype::POSSESSIVE) {
                    root = node;
                    return node;
                }
                // Discover if the group is deterministic
                TreeInfo info = TreeInfo();
                if (head.get().study(info)) { // Deterministic
                    Optional<GroupTail> temp = (Optional<GroupTail>)tail;
                    head = root = UNSAFE::newInstance<GroupCurly>((Optional<Node>)head.get().next,
                                                                  curly.get().cmin,
                                                                  curly.get().cmax,
                                                                  curly.get().type,
                                                                  temp.get().localIndex,
                                                                  temp.get().groupIndex,
                                                                  capturingGroup);
                    return head;
                } else { // Non-deterministic
                    gint temp = ((Optional<GroupHead>)head).get().localIndex;
                    Optional<Loop> loop;
                    if (curly.get().type == Qtype::GREEDY) {
                        loop = UNSAFE::newInstance<Loop>(localCount, temp);
                        // add the max_reps greedy to the top-closure-node list
                        if (curly.get().cmax == MAX_REPS)
                            topClosureNodes.get().add(loop.get());
                    } else { // Reluctant Curly
                        loop = UNSAFE::newInstance<LazyLoop>(localCount, temp);
                    }
                    Prolog prolog = UNSAFE::newInstance<Prolog>(loop);
                    localCount += 1;
                    loop.get().cmin = curly.get().cmin;
                    loop.get().cmax = curly.get().cmax;
                    loop.get().body = head;
                    tail.get().next = loop;
                    root = loop;
                    return prolog; // Dual return
                }
            }
            error("Internal logic error").throws($ftrace());
        }

        Optional<Pattern::Node> Pattern::Self::createGroup(gbool anonymous) {
            gint localIndex = localCount++;
            gint groupIndex = 0;
            if (!anonymous)
                groupIndex = capturingGroupCount++;
            Optional<GroupHead> head = UNSAFE::newInstance<GroupHead>(localIndex);
            root = UNSAFE::newInstance<GroupTail>(localIndex, groupIndex);

            // for debug/print only, head.match does NOT need the "tail" info
            head.get().tail = (GroupTail&)root.get();

            if (!anonymous && groupIndex < 10)
                Arrays::fastSet(groupNodes, groupIndex, head.get());
            return head;
        }

        void Pattern::Self::addFlag() {
            gint ch = peek();
            for (;;) {
                switch (ch) {
                    case 'i':
                        flags |= CASE_INSENSITIVE;
                        break;
                    case 'm':
                        flags |= MULTILINE;
                        break;
                    case 's':
                        flags |= DOTALL;
                        break;
                    case 'd':
                        flags |= UNIX_LINES;
                        break;
                    case 'u':
                        flags |= UNICODE_CASE;
                        break;
                    case 'c':
                        flags |= CANON_EQ;
                        break;
                    case 'x':
                        flags |= COMMENTS;
                        break;
                    case 'U':
                        flags |= (UNICODE_CHARACTER_CLASS | UNICODE_CASE);
                        break;
                    case '-': // subFlag then fall through
                        ch = next();
                        subFlag();
                    default:
                        return;
                }
                ch = next();
            }
        }

        void Pattern::Self::subFlag() {
            gint ch = peek();
            for (;;) {
                switch (ch) {
                    case 'i':
                        flags &= ~CASE_INSENSITIVE;
                        break;
                    case 'm':
                        flags &= ~MULTILINE;
                        break;
                    case 's':
                        flags &= ~DOTALL;
                        break;
                    case 'd':
                        flags &= ~UNIX_LINES;
                        break;
                    case 'u':
                        flags &= ~UNICODE_CASE;
                        break;
                    case 'c':
                        flags &= ~CANON_EQ;
                        break;
                    case 'x':
                        flags &= ~COMMENTS;
                        break;
                    case 'U':
                        flags &= ~(UNICODE_CHARACTER_CLASS | UNICODE_CASE);
                        break;
                    default:
                        return;
                }
                ch = next();
            }
        }

        Qtype Pattern::Self::qtype() {
            gint ch = next();
            if (ch == '?') {
                next();
                return Qtype::LAZY;
            } else if (ch == '+') {
                next();
                return Qtype::POSSESSIVE;
            }
            return Qtype::GREEDY;
        }

        Optional<Pattern::Node> Pattern::Self::curly(const Optional<Node>& prev, gint cmin) {
            Qtype type = qtype();
            if (type == Qtype::GREEDY) {
                if (Class<BmpCharProperty>::hasInstance(prev.get())) {
                    return UNSAFE::newInstance<BmpCharPropertyGreedy>((BmpCharProperty&)prev.get(), cmin);
                } else if (Class<CharProperty>::hasInstance(prev.get())) {
                    return UNSAFE::newInstance<CharPropertyGreedy>((CharProperty&)prev.get(), cmin);
                }
            }
            return UNSAFE::newInstance<Curly>(prev, cmin, MAX_REPS, type);
        }

        Optional<Pattern::Node> Pattern::Self::closure(Optional<Node> prev) {
            gint ch = peek();
            switch (ch) {
                case '?':
                    return UNSAFE::newInstance<Ques>(prev, qtype());
                case '*':
                    return curly(prev, 0);
                case '+':
                    return curly(prev, 1);
                case '{':
                    ch = skip();
                    if (ASCII::isDigit(ch)) {
                        gint cmin = 0, cmax;
                        try {
                            do {
                                cmin = Math::addExact(Math::multiplyExact(cmin, 10), ch - '0');
                            } while (ASCII::isDigit(ch = read()));
                            if (ch == ',') {
                                ch = read();
                                if (ch == '}') {
                                    unread();
                                    return curly(prev, cmin);
                                } else {
                                    cmax = 0;
                                    while (ASCII::isDigit(ch)) {
                                        cmax = Math::addExact(Math::multiplyExact(cmax, 10),
                                                              ch - '0');
                                        ch = read();
                                    }
                                }
                            } else {
                                cmax = cmin;
                            }
                        } catch (ArithmeticException const& ae) {
                            error("Illegal repetition range").throws($ftrace());
                        }
                        if (ch != '}')
                            error("Unclosed counted closure").throws($ftrace());
                        if (cmax < cmin)
                            error("Illegal repetition range").throws($ftrace());
                        unread();
                        return (cmin == 0 && cmax == 1)
                                   ? (Node&)UNSAFE::newInstance<Ques>(prev, qtype())
                                   : (Node&)UNSAFE::newInstance<Curly>(prev, cmin, cmax, qtype());
                    } else {
                        error("Illegal repetition").throws($ftrace());
                    }
                default:
                    return prev;
            }
        }

        gint Pattern::Self::c() {
            if (position < patternLength) {
                return read() ^ 64;
            }
            error("Illegal control escape sequence").throws($ftrace());
        }

        gint Pattern::Self::o() {
            gint n = read();
            if (((n - '0') | ('7' - n)) >= 0) {
                gint m = read();
                if (((m - '0') | ('7' - m)) >= 0) {
                    gint o = read();
                    if ((((o - '0') | ('7' - o)) >= 0) && (((n - '0') | ('3' - n)) >= 0)) {
                        return (n - '0') * 64 + (m - '0') * 8 + (o - '0');
                    }
                    unread();
                    return (n - '0') * 8 + (m - '0');
                }
                unread();
                return (n - '0');
            }
            error("Illegal octal escape sequence").throws($ftrace());
        }

        gint Pattern::Self::x() {
            gint n = read();
            if (ASCII::isHexDigit(n)) {
                gint m = read();
                if (ASCII::isHexDigit(m)) {
                    return ASCII::toDigit(n) * 16 + ASCII::toDigit(m);
                }
            } else if (n == '{' && ASCII::isHexDigit(peek())) {
                gint ch = 0;
                while (ASCII::isHexDigit(n = read())) {
                    ch = (ch << 4) + ASCII::toDigit(n);
                    if (ch > Character::MAX_CODE_POINT)
                        error("Hexadecimal codepoint is too big").throws($ftrace());
                }
                if (n != '}')
                    error("Unclosed hexadecimal escape sequence").throws($ftrace());
                return ch;
            }
            error("Illegal hexadecimal escape sequence").throws($ftrace());
        }

        gint Pattern::Self::cursor() const {
            return position;
        }

        void Pattern::Self::setcursor(gint pos) {
            position = pos;
        }

        gint Pattern::Self::uxxxx() {
            gint n = 0;
            for (gint i = 0; i < 4; i++) {
                gint ch = read();
                if (!ASCII::isHexDigit(ch)) {
                    error("Illegal Unicode escape sequence").throws($ftrace());
                }
                n = n * 16 + ASCII::toDigit(ch);
            }
            return n;
        }

        gint Pattern::Self::u() {
            gint n = uxxxx();
            if (Character::isHighSurrogate((gchar)n)) {
                gint cur = cursor();
                if (read() == '\\' && read() == 'u') {
                    gint n2 = uxxxx();
                    if (Character::isLowSurrogate((gchar)n2))
                        return Character::toCodePoint((gchar)n, (gchar)n2);
                }
                setcursor(cur);
            }
            return n;
        }

        gint Pattern::Self::N() {
            if (read() == '{') {
                gint i = position;
                while (read() != '}') {
                    if (position >= patternLength)
                        error("Unclosed character name escape sequence").throws($ftrace());
                }
                String name = String(temp, i, position - i - 1);
                try {
                    return Character::codePointOf(name);
                } catch (IllegalArgumentException const& x) {
                    error("Unknown character name [" + name + "]").throws($ftrace());
                }
            }
            error("Illegal character name escape sequence").throws($ftrace());
        }

        gint Pattern::Self::countChars(CharSequence const& seq, gint index, gint lengthInCodePoints) {
            // optimization
            if (lengthInCodePoints == 1 && index >= 0 && index < seq.length() &&
                !Character::isHighSurrogate(seq.charAt(index))) {
                return 1;
            }
            gint length = seq.length();
            gint x = index;
            if (lengthInCodePoints >= 0) {
                CORE_ASSERT((length == 0 && index == 0) || index >= 0 && index < length);
                for (gint i = 0; x < length && i < lengthInCodePoints; i++) {
                    if (Character::isHighSurrogate(seq.charAt(x++))) {
                        if (x < length && Character::isLowSurrogate(seq.charAt(x))) {
                            x++;
                        }
                    }
                }
                return x - index;
            }

            CORE_ASSERT(index >= 0 && index <= length);
            if (index == 0) {
                return 0;
            }
            gint len = -lengthInCodePoints;
            for (gint i = 0; x > 0 && i < len; i++) {
                if (Character::isLowSurrogate(seq.charAt(--x))) {
                    if (x > 0 && Character::isHighSurrogate(seq.charAt(x - 1))) {
                        x--;
                    }
                }
            }
            return index - x;
        }

        gint Pattern::Self::countCodePoints(CharSequence const& seq) {
            gint length = seq.length();
            gint n = 0;
            for (gint i = 0; i < length;) {
                n++;
                if (Character::isHighSurrogate(seq.charAt(i++))) {
                    if (i < length && Character::isLowSurrogate(seq.charAt(i))) {
                        i++;
                    }
                }
            }
            return n;
        }

        Optional<Pattern::Node> Pattern::Self::newSlice(IntArray buf, gint count, gbool hasSupplemenatry) const {
            IntArray tmp = IntArray(count);
            if (has(CASE_INSENSITIVE)) {
                if (has(UNICODE_CASE)) {
                    for (gint i = 0; i < count; i++) {
                        tmp[i] = Character::toLowerCase(
                            Character::toUpperCase(buf[i]));
                    }
                    return hasSupplementary
                               ? (Node&)UNSAFE::newInstance<SliceUS>(tmp)
                               : UNSAFE::newInstance<SliceU>(tmp);
                }
                for (gint i = 0; i < count; i++) {
                    tmp[i] = ASCII::toLower(buf[i]);
                }
                return hasSupplementary
                           ? (Node&)UNSAFE::newInstance<SliceIS>(tmp)
                           : UNSAFE::newInstance<SliceI>(tmp);
            }
            for (gint i = 0; i < count; i++) {
                tmp[i] = buf[i];
            }
            return hasSupplementary
                       ? UNSAFE::newInstance<SliceS>(tmp)
                       : UNSAFE::newInstance<Slice>(tmp);
        }

        void Pattern::Self::clear() {
            compiled = false;
            Optional<Node> node = UNSAFE::moveInstance(root);
            root = { };
            matchRoot = { };
            while (node.isPresent()) {
                Optional<Node> next = (Optional<Node>)node.get().next;
                if (node.get() == Pattern::accept) {
                    break;
                }
                if (node.get() == Pattern::lastAccept) {
                    node = next;
                    continue;
                }
                if (node.get() == LookBehindEndNode::INSTANCE) {
                    node = next;
                    continue;
                }
                UNSAFE::deleteRegInstance(node.get());
                node = next;
            }
        }

        Optional<Pattern::CharPredicate> Pattern::Self::VertWS() {
            return UNSAFE::newInstance<BmpCharPredicate>([](gint cp) {
                return (cp >= 0x0A && cp <= 0x0D) ||
                        cp == 0x85 || cp == 0x2028 || cp == 0x2029;
            });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::HorizWS() {
            return UNSAFE::newInstance<BmpCharPredicate>([](gint cp) {
                return cp == 0x09 || cp == 0x20 || cp == 0xa0 || cp == 0x1680 ||
                        cp == 0x180e || cp >= 0x2000 && cp <= 0x200a || cp == 0x202f ||
                        cp == 0x205f || cp == 0x3000;
            });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::ALL() {
            return UNSAFE::newInstance<CharPredicate>([](gint cp) { return true; });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::DOT() {
            return UNSAFE::newInstance<CharPredicate>([](gint ch) {
                return (ch != '\n' && ch != '\r'
                    && (ch | 1) != u'\u2029'
                    && ch != u'\u0085');
            });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::UNIXDOT() {
            return UNSAFE::newInstance<CharPredicate>([](gint ch) { return ch != '\n'; });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::SingleS(gint c) {
            return UNSAFE::newInstance<CharPredicate>([c](gint ch) { return ch == c; });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::Single(gint c) {
            return UNSAFE::newInstance<BmpCharPredicate>([c](gint ch) { return ch == c; });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::SingleI(gint lower, gint upper) {
            return UNSAFE::newInstance<BmpCharPredicate>(
                [lower, upper](gint ch) { return ch == lower || ch == upper; });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::SingleU(gint lower) {
            return UNSAFE::newInstance<CharPredicate>(
                [lower](gint ch) { return lower == ch || lower == Character::toLowerCase(Character::toUpperCase(ch)); }
            );
        }

        gbool Pattern::Self::inRange(gint lower, gint ch, gint upper) {
            return lower <= ch && ch <= upper;
        }

        Optional<Pattern::CharPredicate> Pattern::Self::Range(gint lower, gint upper) {
            if (upper < Character::MIN_HIGH_SURROGATE ||
                lower > Character::MAX_LOW_SURROGATE && upper < Character::MIN_SUPPLEMENTARY)
                return UNSAFE::newInstance<BmpCharPredicate>(
                    [lower, upper](gint ch) { return inRange(lower, ch, upper); });
            return UNSAFE::newInstance<CharPredicate>([lower, upper](gint ch) { return inRange(lower, ch, upper); });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::CIRange(gint lower, gint upper) {
            return UNSAFE::newInstance<CharPredicate>([lower, upper](gint ch) {
                    return inRange(lower, ch, upper) || ASCII::isAscii(ch) && (
                        inRange(lower, ASCII::toUpper(ch), upper) || inRange(lower, ASCII::toLower(ch), upper));
                });
        }

        Optional<Pattern::CharPredicate> Pattern::Self::CIRangeU(gint lower, gint upper) {
            return UNSAFE::newInstance<CharPredicate>([lower, upper](gint ch) {
                    if (inRange(lower, ch, upper))
                        return true;
                    gint up = Character::toUpperCase(ch);
                    return inRange(lower, up, upper) ||
                            inRange(lower, Character::toLowerCase(up), upper);
                });
        }

        Pattern::Self::~Self() {
            clear();
        }
    }
}
