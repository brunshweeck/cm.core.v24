//
// Created by admin on 13/12/24.
//

#include "ZoneOffset.h"

#include <core/Enum.h>
#include <core/XString.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZoneRules.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;

    namespace time {
        ZoneOffset const ZoneOffset::UTC = ZoneOffset(0);
        ZoneOffset const ZoneOffset::MIN = ZoneOffset(-MAX_SECONDS);
        ZoneOffset const ZoneOffset::MAX = ZoneOffset(+MAX_SECONDS);

        ZoneOffset ZoneOffset::of(String const& offsetId) {
            if (offsetId == "Z"_Sl)
                return UTC;
            // parse - +h, +hh, +hhmm, +hh:mm, +hhmmss, +hh:mm:ss
            gint hours = 0, minutes = 0, seconds = 0;
            String id = offsetId;
            switch (id.length()) {
                case 2:
                    id = id.charAt(0) + "0"_Sl + id.charAt(1);
                case 3:
                    try {
                        hours = parseID(id, 1, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    minutes = 0;
                    seconds = 0;
                    break;
                case 5:
                    try {
                        hours = parseID(id, 1, false);
                        minutes = parseID(id, 3, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    seconds = 0;
                    break;
                case 6:
                    try {
                        hours = parseID(id, 1, false);
                        minutes = parseID(id, 4, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    seconds = 0;
                    break;
                case 7:
                    try {
                        hours = parseID(id, 1, false);
                        minutes = parseID(id, 3, false);
                        seconds = parseID(id, 5, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    break;
                case 9:
                    try {
                        hours = parseID(id, 1, false);
                        minutes = parseID(id, 4, true);
                        seconds = parseID(id, 7, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    break;
                default:
                    DateTimeException("Invalid ID for ZoneOffset, invalid format: "_Sl + offsetId).throws($ftrace());
            }
            gchar first = id.charAt(0);
            if (first != '+' && first != '-') {
                DateTimeException("Invalid ID for ZoneOffset, plus/minus "
                    "not found when expected: "_Sl + offsetId).throws($ftrace());
            }
            try {
                if (first == '-')
                    return ofHoursMinutesSeconds(-hours, -minutes, -seconds);
                return ofHoursMinutesSeconds(hours, minutes, seconds);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneOffset ZoneOffset::ofHours(gint hours) {
            try {
                return ofHoursMinutesSeconds(hours, 0, 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneOffset ZoneOffset::ofHoursMinutes(gint hours, gint minutes) {
            try {
                return ofHoursMinutesSeconds(hours, minutes, 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneOffset ZoneOffset::ofHoursMinutesSeconds(gint hours, gint minutes, gint seconds) {
            try { validate(hours, minutes, seconds); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint totalSeconds = hours * LocalTime::SECONDS_PER_HOUR
                    + minutes * LocalTime::SECONDS_PER_MINUTE
                    + seconds;
            return ZoneOffset(totalSeconds);
        }

        ZoneOffset ZoneOffset::from(Temporal const& temporal) {
            Optional<ZoneOffset> opt = (Optional<ZoneOffset>) temporal.query(TemporalQuery::OFFSET);
            if (opt.isEmpty()) {
                DateTimeException("Unable to obtain ZoneOffset from Temporal: "_Sl +
                    temporal + " of type " + typeName(temporal)).throws($ftrace());
            }
            ZoneOffset offset = opt.get();
            UNSAFE::deleteRegInstance(opt.get());
            return ZoneOffset(offset);
        }

        ZoneOffset::ZoneOffset(gint totalSeconds) {
            if (totalSeconds < -MAX_SECONDS || totalSeconds > MAX_SECONDS) {
                DateTimeException("Zone offset not in valid range: -18:00 to +18:00").throws($ftrace());
            }
            totalSecs = totalSeconds;
            id = createID(totalSeconds);
        }

        ZoneOffset::ZoneOffset(ZoneOffset const& other): totalSecs(other.totalSecs),
                                                         id(other.id) {
            if (other.rules != null)
                rules = new ZoneRules(other.getRules());
        }

        ZoneOffset::ZoneOffset(ZoneOffset&& other) CORE_NOTHROW:
            totalSecs(other.totalSecs), id(UNSAFE::moveInstance(other.id)) {
            UNSAFE::swapValues(rules, other.rules);
        }

        ZoneOffset& ZoneOffset::operator=(ZoneOffset const& other) {
            if (this == &other || this == &UTC || this == &MAX || this == &MIN)
                return *this;
            totalSecs = other.totalSecs;
            id = other.id;
            rules = other.rules;
            return *this;
        }

        ZoneOffset& ZoneOffset::operator=(ZoneOffset&& other) noexcept {
            if (this == &other || this == &UTC || this == &MAX || this == &MIN)
                return *this;
            UNSAFE::swapValues(totalSecs, other.totalSecs);
            UNSAFE::swapValues(id, other.id);
            UNSAFE::swapValues(rules, other.rules);
            return *this;
        }

        ZoneOffset::~ZoneOffset() {
            if (rules != null) {
                delete rules;
                rules = null;
            }
        }

        gint ZoneOffset::totalSeconds() const { return totalSecs; }

        String ZoneOffset::getId() const { return id; }

        ZoneRules ZoneOffset::getRules() const {
            CORE_ASSERT(rules != null);
            if (rules == null) {
                rules = new ZoneRules(*this);
            }
            return *rules;
        }

        ZoneOffset const& ZoneOffset::normalized() const { return *this; }

        gbool ZoneOffset::isSupported(ChronoUnit unit) const { return false; }

        gbool ZoneOffset::isSupported(ChronoField field) const {
            return field == OFFSET_SECONDS;
        }

        gint ZoneOffset::get(ChronoField field) const {
            if (field == OFFSET_SECONDS)
                return totalSecs;
            TemporalException("Unsupported field "_Sl + Temporal::toString(field)).throws($ftrace());
        }

        glong ZoneOffset::getLong(ChronoField field) const {
            if (field == OFFSET_SECONDS)
                return totalSecs;
            TemporalException("Unsupported field "_Sl + Temporal::toString(field)).throws($ftrace());
        }

        Optional<> ZoneOffset::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::OFFSET || query == TemporalQuery::ZONE)
                return *this;
            if (query == TemporalQuery::PRECISION)
                return Optional<Enum<ChronoUnit>>(SECONDS);
            if (query == TemporalQuery::ZONE_ID ||
                query == TemporalQuery::LOCAL_DATE ||
                query == TemporalQuery::LOCAL_TIME)
                return Optional<>();

            try {
                return query.queryFrom(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint ZoneOffset::compareTo(ZoneOffset const& otherOffset) const {
            return otherOffset.totalSecs - totalSecs;
        }

        gbool ZoneOffset::equals(Object const& other) const {
            return this == &other ||
                    Class<ZoneOffset>::hasInstance(other) && totalSecs == CORE_XCAST(ZoneOffset const, other).totalSecs;
        }

        gint ZoneOffset::hash() const {
            return totalSecs;
        }

        String ZoneOffset::toString() const {
            return id;
        }

        Object& ZoneOffset::clone() const {
            return UNSAFE::newInstance<ZoneOffset>(*this);
        }

        ZoneOffset ZoneOffset::getOffset(glong epochSecond) const {
            return *this;
        }

        void ZoneOffset::validate(gint hours, gint minutes, gint seconds) {
            if (hours < -18 || hours > 18) {
                DateTimeException("Zone offset hours not in valid range: value "_Sl + hours +
                    " is not in the range -18 to 18"_Sl).throws($ftrace());
            }
            if (hours > 0) {
                if (minutes < 0 || seconds < 0) {
                    DateTimeException("Zone offset minutes and seconds must be "
                        "positive because hours is positive"_Sl).throws($ftrace());
                }
            } else if (hours < 0) {
                if (minutes > 0 || seconds > 0) {
                    DateTimeException("Zone offset minutes and seconds must be "
                        "negative because hours is negative").throws($ftrace());
                }
            } else if ((minutes > 0 && seconds < 0) || (minutes < 0 && seconds > 0)) {
                DateTimeException("Zone offset minutes and seconds must have the same sign").throws($ftrace());
            }
            if (minutes < -59 || minutes > 59) {
                DateTimeException("Zone offset minutes not in valid range: value "_Sl +
                    minutes + " is not in the range -59 to 59"_Sl).throws($ftrace());
            }
            if (seconds < -59 || seconds > 59) {
                DateTimeException("Zone offset seconds not in valid range: value "_Sl +
                    seconds + " is not in the range -59 to 59"_Sl).throws($ftrace());
            }
            if (Math::abs(hours) == 18 && (minutes | seconds) != 0) {
                DateTimeException("Zone offset not in valid range: -18:00 to +18:00").throws($ftrace());
            }
        }

        String ZoneOffset::createID(gint totalSeconds) {
            if (totalSeconds != 0) {
                gint absTotalSeconds = Math::abs(totalSeconds);
                XString str;
                gint absHours = absTotalSeconds / LocalTime::SECONDS_PER_HOUR;
                gint absMinutes = absTotalSeconds / LocalTime::SECONDS_PER_MINUTE % LocalTime::MINUTES_PER_HOUR;
                str.append(totalSeconds < 0 ? "-" : "+")
                   .append(absHours < 10 ? "0" : "").append(absHours)
                   .append(absMinutes < 10 ? ":0" : ":").append(absMinutes);
                gint absSeconds = absTotalSeconds % LocalTime::SECONDS_PER_MINUTE;
                if (absSeconds != 0) {
                    str.append(absSeconds < 10 ? ":0" : ":").append(absSeconds);
                }
                return str.toString();
            }

            return "Z";
        }

        gint ZoneOffset::parseID(String const& id, gint pos, gbool precededByColon) {
            //try {  } catch (Throwable const &ex) { ex.throws($ftrace()); }
            if (precededByColon && id.charAt(pos - 1) != ':') {
                DateTimeException("Invalid ID for ZoneOffset, colon "
                    "not found when expected: "_Sl + id).throws($ftrace());
            }
            char ch1 = id.charAt(pos);
            char ch2 = id.charAt(pos + 1);
            if (ch1 < '0' || ch1 > '9' || ch2 < '0' || ch2 > '9') {
                DateTimeException("Invalid ID for ZoneOffset, non "
                    "numeric characters found: "_Sl + id).throws($ftrace());
            }
            return (ch1 - 48) * 10 + (ch2 - 48);
        }
    } // time
} // core
