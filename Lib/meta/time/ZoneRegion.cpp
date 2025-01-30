//
// Created by admin on 29/12/24.
//

#include "ZoneRegion.h"

#include <core/time/ZoneRules.h>

namespace core {

    namespace time {
        ZoneRegion ZoneRegion::ofID(String const& zoneId, gbool checkAvailable) {
            try { checkName(zoneId); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            try {
                // always attempt load for better behavior after deserialization
                ZoneRules rules = regionToRules(zoneId);
                return ZoneRegion(zoneId, rules);
            } catch (Throwable const& ex) {
                if (checkAvailable)
                    ex.throws($ftrace());
            }
            return ZoneRegion(zoneId);
        }

        ZoneRegion::ZoneRegion(String const& id): id(id) {}

        ZoneRegion::ZoneRegion(String const& id, ZoneRules const& rules): id(id) {
            ZoneRegion::rules = &UNSAFE::newInstance<ZoneRules>(rules);
        }

        String ZoneRegion::getId() const { return id; }

        ZoneRules ZoneRegion::getRules() const {
            if (rules == null)
                return ZoneOffset::UTC.getRules();
            return *rules;
        }

        Object& ZoneRegion::clone() const {
            try {
                ZoneRegion& clone = UNSAFE::newInstance<ZoneRegion>(*this);
                if (rules != null)
                    clone.rules = new ZoneRules(getRules());
                return clone;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        ZoneOffset ZoneRegion::getOffset(glong epochSecond) const {
            return getRules().offset(LocalDateTime::ofEpochSecond(epochSecond, 0));
        }

        void ZoneRegion::checkName(const String& zoneId) {
            int n = zoneId.length();
            if (n < 2) {
                DateTimeException("Invalid ID for region-based ZoneId, invalid format: "_Sl + zoneId).throws($ftrace());
            }
            for (int i = 0; i < n; i++) {
                gchar c = zoneId.charAt(i);
                if (c >= 'a' && c <= 'z') continue;
                if (c >= 'A' && c <= 'Z') continue;
                if (c == '/' && i != 0) continue;
                if (c >= '0' && c <= '9' && i != 0) continue;
                if (c == '~' && i != 0) continue;
                if (c == '.' && i != 0) continue;
                if (c == '_' && i != 0) continue;
                if (c == '+' && i != 0) continue;
                if (c == '-' && i != 0) continue;
                DateTimeException("Invalid ID for region-based ZoneId, invalid format: " + zoneId).throws($ftrace());
            }
        }
    } // time
} // core
