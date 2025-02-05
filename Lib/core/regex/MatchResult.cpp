//
// Created by bruns on 14/10/2024.
//

#include "MatchResult.h"

#include <core/UnsupportedOperationException.h>
#include <core/util/Map.h>

namespace core {
    namespace regex {
        gint MatchResult::start(String const &name) const {
            CORE_TRY_RETHROW(return start(groupNumber(name)));
        }

        gint MatchResult::end(String const &name) const {
            CORE_TRY_RETHROW(return end(groupNumber(name)))
        }

        String MatchResult::group(String const &name) const {
            CORE_TRY_RETHROW(return group(groupNumber(name)));
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
            IllegalArgumentException("No group with name <"_Sl + name + ">"_Sl).throws($ftrace());
        }
    } // regex
} // core
