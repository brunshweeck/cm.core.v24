//
// Created by admin on 18/01/25.
//

#include "TimeZone.h"

#include "core/StringArray.h"
#include "core/UnsupportedOperationException.h"

namespace core {
    namespace time {
        gint TimeZone::offset(glong date) const {
            return 0;
        }

        String TimeZone::getID() const {
            return ID;
        }

        String TimeZone::displayName() const {
            return String();
        }

        String TimeZone::displayName(util::Locale const& locale) const {
            return String();
        }

        String TimeZone::displayName(gbool daylight, Style style, util::Locale const& locale) const {
            return String();
        }

        gint TimeZone::dstSavings() const {
            return 0;
        }

        gbool TimeZone::useDaylightTime() const {
            return false;
        }

        gbool TimeZone::observesDayLightTime() const {
            return false;
        }

        TimeZone& TimeZone::of(String const& ID) {
            UnsupportedOperationException().throws($ftrace());
        }

        TimeZone& TimeZone::of(ZoneId const& zoneId) {
            UnsupportedOperationException().throws($ftrace());
        }

        ZoneId& TimeZone::toZoneId() const {
            return zoneId.get();
        }

        StringArray TimeZone::availableIDs(gint rawOffset) {
            return StringArray();
        }

        StringArray TimeZone::availableIDs() {
            return StringArray();
        }

        TimeZone& TimeZone::getDefault() {
            UnsupportedOperationException().throws($ftrace());
        }

        void TimeZone::setDefault(TimeZone const& zone) {}

        gbool TimeZone::hasSameRules(TimeZone const& other) const {
            return false;
        }

        Object& TimeZone::clone() const {
            CloneNotSupportedException().throws($ftrace());
        }

        gint TimeZone::offsets(glong date, IntArray& offsets) {
            return 0;
        }

        StringArray TimeZone::displayNames(String const& ID, util::Locale const& locale) {
            return StringArray();
        }

        ZoneId& TimeZone::toZoneID() const {
            return zoneId.get();
        }

        TimeZone& TimeZone::forID(String const& ID, gbool fallback) {
            UnsupportedOperationException().throws($ftrace());
        }

        util::Optional<TimeZone> TimeZone::parseID(String const& id) {
            UnsupportedOperationException().throws($ftrace());
        }
    } // time
} // core
