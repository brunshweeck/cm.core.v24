//
// Created by bruns on 14/10/2024.
//

#include <core/UnsupportedOperationException.h>
#include <core/regex/MatchResult.h>
#include <core/util/Map.h>

namespace core {
    namespace regex {
        gint MatchResult::start(String const &name) const {
            return start(groupNumber(name));
        }

        gint MatchResult::end(String const &name) const {
            return end(groupNumber(name));
        }

        String MatchResult::group(String const &name) const {
            return group(groupNumber(name));
        }

        util::Map<String, Integer> const &MatchResult::namedGroups() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gbool MatchResult::hasMatch() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint MatchResult::groupNumber(String const &name) const {
            Object const &number = namedGroups().getOrNull(name);
            if (number != null)
                return CORE_XCAST(Integer const, number);
            IllegalArgumentException("No group with name <"_S + name + ">"_S).throws($ftrace());
        }
    } // regex
} // core
