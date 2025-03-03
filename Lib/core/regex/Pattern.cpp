//
// Created by bruns on 13/10/2024.
//

#include "Pattern.h"

#include <core/lang/StringArray.h>
#include <core/lang/XString.h>
#include <core/regex/Matcher.h>
#include <core/util/ArrayList.h>
#include <core/util/Map.h>
#include <meta/regex/Pattern.Self.h>

namespace core {
    using namespace util;
    using namespace text;
    using namespace io;

    namespace regex {
        Pattern::Pattern(String p, gint f): regex(p), flags(f) {
            if ((f & ~ALL_FLAGS) != 0) {
                IllegalArgumentException("Unknown flag 0x" + Integer::toHexString(f)).throws($ftrace());
            }
            regex = UNSAFE::moveInstance(p);
            flags = f;

            // to use UNICODE_CASE if UNICODE_CHARACTER_CLASS present
            if ((flags & UNICODE_CHARACTER_CLASS) != 0)
                flags |= UNICODE_CASE;

            try { self = new Self; } catch (Throwable const &ex) { ex.throws($ftrace()); }
            // 'flags' for compiling
            self->flags = flags;

            // Reset group index count
            self->capturingGroupCount = 1;
            self->localCount = 0;
            self->localTCNCount = 0;

            if (!regex.isEmpty())
                try { self->compile(regex); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            else {
                self->root = UNSAFE::newInstance<Start>(lastAccept);
                self->matchRoot = lastAccept;
            }
            //
        }

        Pattern Pattern::compile(String const &regex) {
            try { return Pattern(regex, 0); } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        Pattern Pattern::compile(String const &regex, gint flags) {
            try { return Pattern(regex, flags); } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String Pattern::pattern() const { return regex; }

        Pattern::Pattern(Pattern const &other): regex(other.regex), flags(other.flags) {
            if (!other.self) {
                self = new Self;
                self->compile(regex);
            } else {
                other.self->state.getAndIncrement();
                self = other.self;
            }
        }

        Pattern::Pattern(Pattern &&other) CORE_NOTHROW {
            regex = UNSAFE::moveInstance(other.regex);
            UNSAFE::swapValues(flags, other.flags);
            UNSAFE::swapValues(self, other.self);
        }

        Pattern &Pattern::operator=(Pattern const &other) {
            if (this != &other) {
                regex = other.regex;
                flags = other.flags;
                // Destroy old Data
                {
                    SelfData data = self;
                    self = {};
                    if (data->state.getAndDecrement() == 0)
                        delete data;
                }
                // Init with new Data
                if (!other.self) {
                    self = new Self;
                    self->compile(regex);
                } else {
                    other.self->state.getAndIncrement();
                    self = other.self;
                }
            }
            return *this;
        }

        Pattern &Pattern::operator=(Pattern &&other) CORE_NOTHROW {
            if (this != &other) {
                regex = UNSAFE::moveInstance(other.regex);
                UNSAFE::swapValues(flags, other.flags);
                UNSAFE::swapValues(self, other.self);
            }
            return *this;
        }

        Pattern::~Pattern() {
            SelfData data = self;
            self = {};
            if (data->state.getAndDecrement() == 0)
                delete data;
        }

        String Pattern::toString() const { return regex; }

        Matcher Pattern::matcher(CharSequence const &input) const {
            if (!self->compiled) {
                // Synchronized (this) {
                if (!self->compiled)
                    self->compile(regex);
                // }
            }
            Matcher matcher = Matcher(*this, input);
            return matcher;
        }

        gint Pattern::getFlags() const { return flags; }

        gbool Pattern::matches(String const &regex, CharSequence const &input) {
            try {
                return compile(regex).matcher(input).matches();
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        StringArray Pattern::split(CharSequence const &input, gint limit) const {
            try { return split(input, limit, false); } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        StringArray Pattern::splitWithDelimiters(CharSequence const &input, gint limit) const {
            try { return split(input, limit, true); } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        StringArray Pattern::split(CharSequence const &input) const {
            try { return split(input, 0, false); } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String Pattern::quote(String const &s) {
            //try {  } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint slashEIndex = s.indexOf("\\E");
            if (slashEIndex == -1)
                return "\\Q" + s + "\\E";

            gint lenHint = s.length();
            lenHint = (lenHint < Integer::MAX_VALUE - 8 - lenHint) ? (lenHint << 1) : (Integer::MAX_VALUE - 8);

            XString xs = XString(lenHint);
            xs.append("\\Q");
            gint current = 0;
            do {
                xs.append(s, current, slashEIndex).append(R"(\E\\E\Q)"_Sl);
                current = slashEIndex + 2;
            } while ((slashEIndex = s.indexOf("\\E", current)) != -1);

            return xs.append(s, current, s.length())
                    .append("\\E")
                    .toString();
        }

        Map<String, Integer> &Pattern::namedGroups() const {
            try {
                return Map<String, Integer>::copyOf(self->namedGroupsMap().get());
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        Predicate<String> Pattern::asPredicate() const {
            return [&](String const& s) -> gbool { return matcher(s).find(); };
        }

        Predicate<String> Pattern::asMatchPredicate() const {
            return [&](String const& s) -> gbool { return matcher(s).matches(); };
        }

        StringArray Pattern::split(CharSequence const &input, gint limit, gbool withDelimiters) const  {
            gint matchCount = 0;
            gint index = 0;
            gbool matchLimited = limit > 0;
            ArrayList<String> matchList = {};
            Matcher m = matcher(input);

            // Add segments before each match found
            while(m.find()) {
                if (!matchLimited || matchCount < limit - 1) {
                    if (index == 0 && index == m.start() && m.start() == m.end()) {
                        // no empty leading substring included for zero-width match
                        // at the beginning of the input char sequence.
                        continue;
                    }
                    String match = input.subSequence(index, m.start()).toString();
                    matchList.add(match);
                    index = m.end();
                    if (withDelimiters) {
                        matchList.add(input.subSequence(m.start(), index).toString());
                    }
                    ++matchCount;
                } else if (matchCount == limit - 1) { // last one
                    String match = input.subSequence(index, input.length()).toString();
                    matchList.add(match);
                    index = m.end();
                    ++matchCount;
                }
            }

            // If no match was found, return this
            if (index == 0)
                return StringArray(1, input.toString());

            // Add remaining segment
            if (!matchLimited || matchCount < limit)
                matchList.add(input.subSequence(index, input.length()).toString());

            // Construct result
            gint resultSize = matchList.size();
            if (limit == 0) {
                while (resultSize > 0 && matchList.get(resultSize-1).isEmpty()) {
                    resultSize--;
                }
            }
            StringArray result =  StringArray(resultSize);
            for (gint i = 0; i < resultSize; i++) {
                String& var = matchList.get(i);
                result[i] = UNSAFE::moveInstance(var);
                UNSAFE::deleteInstance(var);
            }
            return result;
        }
    } // regex
} // core
