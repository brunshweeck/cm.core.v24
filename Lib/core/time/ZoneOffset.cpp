//
// Created by admin on 13/12/24.
//

#include "ZoneOffset.h"

#include <core/lang/Enum.h>
#include <core/lang/XString.h>
#include <core/time/DateTimeException.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZoneRules.h>
#include <core/time/ValueRange.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>

#include "TemporalUnit.h"

namespace core {
    using namespace util;

    namespace time {
        CORE_ALIAS(Fields, TemporalUtils::Fields);
        CORE_ALIAS(Units, TemporalUtils::Units);

        ZoneOffset const ZoneOffset::UTC = ZoneOffset(0);
        ZoneOffset const ZoneOffset::MIN = ZoneOffset(-MAX_SECONDS);
        ZoneOffset const ZoneOffset::MAX = ZoneOffset(+MAX_SECONDS);

        ZoneOffset::ZoneOffset(gint totalSeconds): totalSeconds_(totalSeconds), id(buildId(totalSeconds)) {}

        ZoneOffset ZoneOffset::of(String const& offsetId) {
            if (offsetId.equals("Z"_Sl))
                return ZoneOffset::UTC;
            // parse - +h, +hh, +hhmm, +hh:mm, +hhmmss, +hh:mm:ss
            gint hours = Integer::MIN_VALUE, minutes = Integer::MIN_VALUE, seconds = Integer::MIN_VALUE;
            try {
                switch (offsetId.length()) {
                    case 2: {
                        String tmp = offsetId.charAt(0) + "0"_Sl + offsetId.charAt(1);
                        hours = parseNumber(tmp, 1, false);
                        minutes = 0;
                        seconds = 0;
                    }
                    case 3: {
                        hours = parseNumber(offsetId, 1, false);
                        minutes = 0;
                        seconds = 0;
                        break;
                    }
                    case 5: {
                        hours = parseNumber(offsetId, 1, false);
                        minutes = parseNumber(offsetId, 3, false);
                        seconds = 0;
                        break;
                    }
                    case 6: {
                        hours = parseNumber(offsetId, 1, false);
                        minutes = parseNumber(offsetId, 4, true);
                        break;
                    }
                    case 7: {
                        hours = parseNumber(offsetId, 1, false);
                        minutes = parseNumber(offsetId, 3, false);
                        seconds = parseNumber(offsetId, 5, false);
                        break;
                    }
                    case 9: {
                        hours = parseNumber(offsetId, 1, false);
                        minutes = parseNumber(offsetId, 4, true);
                        seconds = parseNumber(offsetId, 7, true);
                        break;
                    }
                    default: break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            if (hours == Integer::MIN_VALUE || minutes == Integer::MIN_VALUE || seconds == Integer::MIN_VALUE) {
                DateTimeException("Invalid ID for ZoneOffset, invalid format: "_Sl + offsetId).throws($ftrace());
            }

            gchar first = offsetId.charAt(0);
            if (first != '+' && first != '-') {
                DateTimeException("Invalid ID for ZoneOffset, plus/minus not found when expected: " + offsetId)
                        .throws($ftrace());
            }
            try {
                if (first == '-') {
                    return ofHoursMinutesSeconds(-hours, -minutes, -seconds);
                } else {
                    return ofHoursMinutesSeconds(hours, minutes, seconds);
                }
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
            try {
                validate(hours, minutes, seconds);
                gint totalSeconds = ZoneOffset::totalSeconds(hours, minutes, seconds);
                return ofTotalSeconds(totalSeconds);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneOffset ZoneOffset::from(TemporalAccessor const& temporal) {
            Optional result = temporal.query(TemporalQuery::offset());
            if (result.isEmpty()) {
                DateTimeException("Unable to obtain ZoneOffset from Temporal: "_Sl +
                    temporal + " of type " + typeName(temporal)).throws($ftrace());
            }
            ZoneOffset offset = ((OptionalOffset) result).get();
            if (&result.get() != &temporal)
                UNSAFE::deleteRegInstance(result.get());
            return offset;
        }

        ZoneOffset ZoneOffset::ofTotalSeconds(gint totalSeconds) {
            if (totalSeconds < -MAX_SECONDS || totalSeconds > MAX_SECONDS) {
                DateTimeException("Zone offset not in valid range: -18:00 to +18:00").throws($ftrace());
            }
            return ZoneOffset(totalSeconds);
        }

        gint ZoneOffset::totalSeconds() const {
            return totalSeconds_;
        }

        String ZoneOffset::getId() const {
            return id;
        }

        ZoneRules ZoneOffset::getRules() const {
            try {
                return ZoneRules::of(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneOffset const& ZoneOffset::normalized() const {
            return *this;
        }

        gbool ZoneOffset::isSupported(TemporalField field) const {
            return field == TemporalField::OFFSET_SECONDS || Fields::isSupportedBy(*this, field);
        }

        ValueRange ZoneOffset::range(TemporalField field) const {
            try {
                return Fields::range(*this, field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ZoneOffset::get(TemporalField field) const {
            try {
                if (field == TemporalField::OFFSET_SECONDS)
                    return totalSeconds_;
                return range(field).checkValidIntValue(getLong(field), field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong ZoneOffset::getLong(TemporalField field) const {
            try {
                if (field == TemporalField::OFFSET_SECONDS)
                    return totalSeconds_;
                return Fields::getFrom(*this, field);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ZoneId::Optional ZoneOffset::query(TemporalQuery const& query) const {
            if (query == TemporalQuery::offset() || query == TemporalQuery::zone())
                return *this;
            if (query == TemporalQuery::precision())
                return (Enum<TemporalUnit>) TemporalUnit::SECONDS;
            if (query == TemporalQuery::localDate() ||
                query == TemporalQuery::localTime() ||
                query == TemporalQuery::zoneId())
                return Optional::empty();
            try {
                return query.queryFrom(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ZoneOffset::compareTo(ZoneOffset const& otherOffset) const {
            return totalSeconds_ - otherOffset.totalSeconds_;
        }

        gbool ZoneOffset::equals(Object const& other) const {
            return this == &other ||
                    (Class<ZoneOffset>::hasInstance(other) &&
                    totalSeconds_ == CORE_XCAST(ZoneOffset const, other).totalSeconds_);
        }

        gint ZoneOffset::hash() const {
            return totalSeconds_;
        }

        String ZoneOffset::toString() const {
            return getId();
        }

        Object& ZoneOffset::clone() const {
            return UNSAFE::newInstance<ZoneOffset>(*this);
        }

        gint ZoneOffset::parseNumber(CharSequence const& offsetId, gint pos, gbool precededByColon) {
            if (precededByColon && offsetId.charAt(pos - 1) != ':') {
                DateTimeException("Invalid ID for ZoneOffset, colon not found when expected: "_Sl + offsetId)
                        .throws($ftrace());
            }
            gchar ch1 = offsetId.charAt(pos);
            gchar ch2 = offsetId.charAt(pos + 1);
            if (ch1 < '0' || ch1 > '9' || ch2 < '0' || ch2 > '9') {
                DateTimeException("Invalid ID for ZoneOffset, non numeric characters found: "_Sl + offsetId)
                        .throws($ftrace());
            }
            return (ch1 - 48) * 10 + (ch2 - 48);
        }

        void ZoneOffset::validate(gint hours, gint minutes, gint seconds) {}

        gint ZoneOffset::totalSeconds(gint hours, gint minutes, gint seconds) {
            return hours * SECONDS_PER_HOUR + minutes * SECONDS_PER_MINUTE + seconds;
        }

        String ZoneOffset::buildId(gint totalSeconds) {
            if (totalSeconds == 0) {
                return "Z"_Sl;
            } else {
                int absTotalSeconds = Math::abs(totalSeconds);
                XString buf;
                int absHours = absTotalSeconds / SECONDS_PER_HOUR;
                int absMinutes = (absTotalSeconds / SECONDS_PER_MINUTE) % LocalTime::MINUTES_PER_HOUR;
                buf.append(totalSeconds < 0 ? "-"_Sl : "+"_Sl)
                    .append(absHours < 10 ? "0" : "").append(absHours)
                    .append(absMinutes < 10 ? ":0" : ":").append(absMinutes);
                int absSeconds = absTotalSeconds % SECONDS_PER_MINUTE;
                if (absSeconds != 0) {
                    buf.append(absSeconds < 10 ? ":0" : ":").append(absSeconds);
                }
                return buf.toString();
            }
        }

        ZoneOffset ZoneOffset::getOffset(glong epochSecond) const {
            return *this;
        }
    } // time
} // core
