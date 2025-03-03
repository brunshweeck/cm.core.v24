//
// Created by admin on 29/12/24.
//

#include "ZoneOffsetTransitionRule.h"

#include "Year.h"
#include "ZoneOffsetTransition.h"
#include <core/lang/Enum.h>
#include <core/lang/IllegalArgumentException.h>
#include "meta/time/TemporalUtils.h"

namespace core {
  namespace time {
    CORE_ALIAS(Fields, TemporalUtils::Fields);

    static CORE_FAST DayOfWeek INVALID_DAY_OF_WEEK = (DayOfWeek) 0;

    ZoneOffsetTransitionRule::ZoneOffsetTransitionRule(Month month,
                                                       gint dayOfMonthIndicator,
                                                       DayOfWeek dayOfWeek,
                                                       const LocalTime& time,
                                                       gbool timeEndOfDay,
                                                       TimeDefinition timeDefinition,
                                                       const ZoneOffset& standardOffset,
                                                       const ZoneOffset& offsetBefore,
                                                       const ZoneOffset& offsetAfter)
      : month_(month),
        dom(dayOfMonthIndicator),
        dow(dayOfWeek),
        time(time),
        timeEndOfDay(timeEndOfDay),
        timeDefinition_(timeDefinition),
        standardOffset_(standardOffset),
        offsetBefore_(offsetBefore),
        offsetAfter_(offsetAfter) {}

    ZoneOffsetTransitionRule ZoneOffsetTransitionRule::of(Month month,
                                                          gint dayOfMonthIndicator,
                                                          DayOfWeek dayOfWeek,
                                                          LocalTime const& time,
                                                          gbool timeEndOfDay,
                                                          TimeDefinition timeDefinition,
                                                          ZoneOffset const& standardOffset,
                                                          ZoneOffset const& offsetBefore,
                                                          ZoneOffset const& offsetAfter) {
      if (dayOfMonthIndicator < -28 || dayOfMonthIndicator > 31 || dayOfMonthIndicator == 0) {
        IllegalArgumentException("Day of month indicator must be between -28 "
          "and 31 inclusive excluding zero"_Sl).throws($ftrace());
      }
      if (timeEndOfDay && !time.equals(LocalTime::MIDNIGHT)) {
        IllegalArgumentException("Time must be midnight when end of day flag is true"_Sl).throws($ftrace());
      }
      if (time.nano() != 0) {
        IllegalArgumentException("Time's nano-of-second must be zero"_Sl).throws($ftrace());
      }
      return ZoneOffsetTransitionRule(month, dayOfMonthIndicator, dayOfWeek, time, timeEndOfDay, timeDefinition,
                                      standardOffset, offsetBefore, offsetAfter);
    }

    Month ZoneOffsetTransitionRule::month() const {
      return month_;
    }

    gint ZoneOffsetTransitionRule::dayOfMonthIndicator() const {
      return dom;
    }

    DayOfWeek ZoneOffsetTransitionRule::dayOfWeek() const {
      return dow;
    }

    LocalTime ZoneOffsetTransitionRule::localTime() const {
      return time;
    }

    gbool ZoneOffsetTransitionRule::isMidnightEndOfDay() const {
      return timeEndOfDay;
    }

    ZoneOffsetTransitionRule::TimeDefinition ZoneOffsetTransitionRule::timeDefinition() const {
      return timeDefinition_;
    }

    ZoneOffset ZoneOffsetTransitionRule::standardOffset() const {
      return standardOffset_;
    }

    ZoneOffset ZoneOffsetTransitionRule::offsetBefore() const {
      return offsetBefore_;
    }

    ZoneOffset ZoneOffsetTransitionRule::offsetAfter() const {
      return offsetAfter_;
    }

    ZoneOffsetTransition ZoneOffsetTransitionRule::createTransition(gint year) const {
      LocalDate date = LocalDate::EPOCH;
      if (dom < 0) {
        date = LocalDate::of(year, month_, TemporalUtils::lengthOfMonth((gint) month_, Year::isLeap(year)) + 1 + dom);
        if (dow != INVALID_DAY_OF_WEEK) {
          date = date.with(TemporalAdjuster::previousOrSame(dow));
        }
      } else {
        date = LocalDate::of(year, month_, dom);
        if (dow != INVALID_DAY_OF_WEEK) {
          date = date.with(TemporalAdjuster::nextOrSame(dow));
        }
      }
      if (timeEndOfDay) {
        date = date.plusDays(1);
      }
      LocalDateTime localDT = LocalDateTime::of(date, time);
      LocalDateTime transition = createDateTime(timeDefinition_, localDT, standardOffset_, offsetBefore_);
      return ZoneOffsetTransition(transition, offsetBefore_, offsetAfter_);
    }

    gbool ZoneOffsetTransitionRule::equals(Object const& otherRule) const {
      if (this == &otherRule)
        return true;
      if (Class<ZoneOffsetTransitionRule>::hasInstance(otherRule)) {
        ZoneOffsetTransitionRule const& other = CORE_XCAST(ZoneOffsetTransitionRule const, otherRule);
        return month_ == other.month_ &&
            dom == other.dom &&
            dow == other.dow &&
            timeDefinition_ == other.timeDefinition_ &&
            timeEndOfDay == other.timeEndOfDay &&
            time.equals(other.time) &&
            standardOffset().equals(other.standardOffset()) &&
            offsetBefore_.equals(other.offsetBefore_) &&
            offsetAfter_.equals(other.offsetAfter_);
      }
      return false;
    }

    gint ZoneOffsetTransitionRule::hash() const {
      gint hash = ((time.toSecondOfDay() + (timeEndOfDay ? 1 : 0)) << 15) +
          ((gint) month_ << 11) + ((dom + 32) << 5) + ((gint) dow << 2) + (gint) timeDefinition_;
      return hash ^ standardOffset_.hash() ^ offsetBefore_.hash() ^ offsetAfter_.hash();
    }

    String ZoneOffsetTransitionRule::toString() const {
      XString buf;
      buf.append("TransitionRule[")
         .append(offsetBefore_.compareTo(offsetAfter_) > 0 ? "Gap " : "Overlap ")
         .append(offsetBefore_).append(" to ").append(offsetAfter_).append(", ");
      if (dow != INVALID_DAY_OF_WEEK) {
        if (dom == -1) {
          buf.append(dow).append(" on or before last day of ")
          .append(month_);
        } else if (dom < 0) {
          buf.append(dow).append(" on or before last day minus ")
          .append(-dom - 1).append(" of ").append(month_);
        } else {
          buf.append(dow).append(" on or after ")
          .append(month_).append(' ').append(dom);
        }
      } else {
        buf.append(month_).append(' ').append(dom);
      }
      buf.append(" at ").append(timeEndOfDay ? "24:00" : time.toString())
         .append(" ").append(timeDefinition_)
         .append(", standard offset ").append(standardOffset_)
         .append(']');
      return buf.toString();
    }

    Object& ZoneOffsetTransitionRule::clone() const {
      return UNSAFE::newInstance<ZoneOffsetTransitionRule>(*this);
    }

    LocalDateTime ZoneOffsetTransitionRule::createDateTime(TimeDefinition timeDefinition, LocalDateTime const& dateTime,
                                                           ZoneOffset const& standardOffset,
                                                           ZoneOffset const& wallOffset) {
      try {
        switch (timeDefinition) {
          case TimeDefinition::UTC: {
            int difference = wallOffset.totalSeconds() - ZoneOffset::UTC.totalSeconds();
            return dateTime.plusSeconds(difference);
          }
          case TimeDefinition::STANDARD: {
            int difference = wallOffset.totalSeconds() - standardOffset.totalSeconds();
            return dateTime.plusSeconds(difference);
          }
          default: // WALL
            return dateTime;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
  } // time


  namespace time {
    CORE_ALIAS(TimeDefinition, ZoneOffsetTransitionRule::TimeDefinition);

#define LABEL(CLASS, NAME) setDefaultLabel(#NAME ## _Sl, CLASS::NAME)
#define ENUM_CLASS(CLASS, ENUM) \
    class CLASS ## _ ## ENUM ## Enum final: public Enum<ENUM> { \
    public:\
        CLASS ## _ ## ENUM ## Enum (String const &label, ENUM value): Enum/*<ENUM>*/(label, (gint)value) {}\
        static gbool init();\
    };\
    static gint $_ ## CLASS ## _ ## ENUM ## Enum = CLASS ## _ ## ENUM ## Enum::init(); \
    gbool CLASS ## _ ## ENUM ## Enum::init()


    ENUM_CLASS(Enum, TimeDefinition) {
      Enum e = (TimeDefinition) 0;
      static Object& table = labels(null);

      LABEL(TimeDefinition , UTC);
      LABEL(TimeDefinition , WALL);
      LABEL(TimeDefinition , STANDARD);

      labels(table);
      return true;
    }


#undef LABEL
#undef ENUM_CLASS
  } // time

  inline namespace literals {
    using namespace time;
    String operator+(String const& text, TimeDefinition timeDefinition) {
      return text + (Enum<TimeDefinition>) timeDefinition;
    }

    String operator+(TimeDefinition timeDefinition, String const& text) {
      return (Enum<TimeDefinition>) timeDefinition + text;
    }
  }
} // core
