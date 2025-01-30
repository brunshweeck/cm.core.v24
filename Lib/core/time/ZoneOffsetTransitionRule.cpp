//
// Created by admin on 29/12/24.
//

#include "ZoneOffsetTransitionRule.h"

#include "ZoneOffsetTransition.h"
#include "core/IllegalArgumentException.h"
#include "core/XString.h"
#include "core/misc/Unsafe.h"

namespace core {
    namespace time {
        ZoneOffsetTransitionRule::ZoneOffsetTransitionRule(LocalDate::Month month,
                                                           gint dayOfMonthIndicator,
                                                           LocalDate::DayOfWeek dayOfWeek,
                                                           LocalTime const& time,
                                                           gbool timeEndOfDay,
                                                           TimeDefinition timeDefinition,
                                                           ZoneOffset const& standardOffset,
                                                           ZoneOffset const& offsetBefore,
                                                           ZoneOffset const& offsetAfter)
            : m(month),
              dom(dayOfMonthIndicator),
              dow(dayOfWeek),
              time(time),
              timeEndOfDay(timeEndOfDay),
              timeDef(timeDefinition),
              standard(standardOffset),
              before(offsetBefore),
              after(offsetAfter) {
            if (dayOfMonthIndicator < -28 || dayOfMonthIndicator > 31 || dayOfMonthIndicator == 0) {
                IllegalArgumentException("Day of month indicator must be between -28 "
                    "and 31 inclusive excluding zero"_Sl).throws($ftrace());
            }
            if (timeEndOfDay && time.equals(LocalTime::MIDNIGHT) == false) {
                IllegalArgumentException("Time must be midnight when end of day flag is true"_Sl).throws($ftrace());
            }
            if (time.nano() != 0) {
                IllegalArgumentException("Time's nano-of-second must be zero"_Sl).throws($ftrace());
            }
        }

        LocalDate::Month ZoneOffsetTransitionRule::month() const { return m; }

        gint ZoneOffsetTransitionRule::dayOfMonthIndicator() const { return dom; }

        LocalDate::DayOfWeek ZoneOffsetTransitionRule::dayOfWeek() const { return dow; }

        LocalTime ZoneOffsetTransitionRule::localTime() const { return time; }

        gbool ZoneOffsetTransitionRule::isMidnightEndOfDay() const { return timeEndOfDay; }

        ZoneOffsetTransitionRule::TimeDefinition ZoneOffsetTransitionRule::timeDefinition() const { return timeDef; }

        ZoneOffset ZoneOffsetTransitionRule::standardOffset() const { return standard; }

        ZoneOffset ZoneOffsetTransitionRule::offsetBefore() const { return before; }

        ZoneOffset ZoneOffsetTransitionRule::offsetAfter() const { return after; }

        ZoneOffsetTransition ZoneOffsetTransitionRule::createTransition(gint year) const {
            LocalDate date = LocalDate::EPOCH;
            LocalDate::Month month = m;
            if (dom < 0) {
                date = LocalDate(year, month, LocalDate::lengthOfMonth(month, LocalDate::isLeapYear(year)) + 1 + dom);
                LocalDate::DayOfWeek dayOfWeek = date.dayOfWeek();
                if (dow == dayOfWeek) {
                    // Same
                } else {
                    // Previous
                    gint daysDiff = dow - dayOfWeek;
                    date = date.minusDays(daysDiff < 0 ? 7 - daysDiff : -daysDiff);
                }
            } else {
                date = LocalDate(year, month, dom);
                LocalDate::DayOfWeek dayOfWeek = date.dayOfWeek();
                if (dow == dayOfWeek) {
                    // Same
                } else {
                    // Previous
                    gint daysDiff = dayOfWeek - dow;
                    date = date.plusDays(daysDiff < 0 ? 7 - daysDiff : -daysDiff);
                }
            }
            if (timeEndOfDay) {
                date = date.plusDays(1);
            }
            try {
                LocalDateTime localDT = LocalDateTime(date, time);
                LocalDateTime transition = createDateTime(timeDef, localDT, standard, before);
                return ZoneOffsetTransition(transition, before, after);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool ZoneOffsetTransitionRule::equals(Object const& other) const {
            if (this == &other)
                return true;
            if (Class<ZoneOffsetTransitionRule>::hasInstance(other)) {
                ZoneOffsetTransitionRule const& otherRule = CORE_XCAST(ZoneOffsetTransitionRule const, other);
                return m == otherRule.m &&
                        dom == otherRule.dom &&
                        dow == otherRule.dow &&
                        timeDef == otherRule.timeDef &&
                        timeEndOfDay == otherRule.timeEndOfDay &&
                        time.equals(otherRule.time) &&
                        standard.equals(otherRule.standard) &&
                        before.equals(otherRule.before) &&
                        after.equals(otherRule.after);
            }
            return false;
        }

        gint ZoneOffsetTransitionRule::hash() const {
            gint hash = ((time.toSecondOfDay() + (timeEndOfDay ? 1 : 0)) << 15) +
                    (m << 11) + ((dom + 32) << 5) + (dow << 2) + (gint) timeDef;
            return hash ^ standard.hash() ^ before.hash() ^ after.hash();
        }

        static CORE_FAST const char* DAYS[] = {
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday",
            "Sunday"
        };

        static CORE_FAST const char* MONTHS[] = {
            "January",
            "February",
            "March",
            "April",
            "May",
            "June",
            "July",
            "August",
            "September",
            "October",
            "November",
            "December",
        };

        String ZoneOffsetTransitionRule::toString() const {
            XString str;
            str.append("TransitionRule[")
               .append(before.compareTo(after) > 0 ? "Gap "_Sl : "Overlap "_Sl)
               .append(before).append(" to ").append(after).append(", ");
            if (dom == -1) {
                str.append(DAYS[dow]).append(" on or before last day of ").append(MONTHS[m]);
            } else if (dom < 0) {
                str.append(DAYS[dow]).append(" on or before last day minus ").append(-dom - 1).append(" of ").append(MONTHS[m]);
            } else {
                str.append(DAYS[dow]).append(" on or after ").append(MONTHS[m]).append(' ').append(dom);
            }
            str.append(" at "_Sl)
            .append(timeEndOfDay ? "24:00"_Sl : time.toString())
            .append(timeDef == TimeDefinition::WALL
            ? " WALL"_Sl : timeDef == TimeDefinition::UTC
            ? " UTC"_Sl : " STANDARD"_Sl)
            .append(", standard offset ").append(standard)
            .append(']');
            return str.toString();
        }

        Object& ZoneOffsetTransitionRule::clone() const {
            try {
                return UNSAFE::newInstance<ZoneOffsetTransitionRule>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDateTime ZoneOffsetTransitionRule::createDateTime(TimeDefinition timeDefinition,
                                                               LocalDateTime const& dateTime,
                                                               ZoneOffset const& standardOffset,
                                                               ZoneOffset const& wallOffset) {
            switch (timeDefinition) {
                case TimeDefinition::UTC: {
                    gint difference = wallOffset.totalSeconds() - ZoneOffset::UTC.totalSeconds();
                    return dateTime.plusSeconds(difference);
                }
                case TimeDefinition::STANDARD: {
                    int difference = wallOffset.totalSeconds() - standardOffset.totalSeconds();
                    return dateTime.plusSeconds(difference);
                }
                default: // WALL
                    return dateTime;
            }
        }
    } // time
} // core
