//
// Created by admin on 03/01/25.
//

#include "MonthDay.h"

#include <core/lang/XString.h>
#include <core/time/DateTimeException.h>
#include <core/time/LocalDate.h>
#include <core/time/Month.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ValueRange.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  using namespace util;
  CORE_ALIAS(Fields, time::TemporalUtils::Fields);
  CORE_ALIAS(Units, time::TemporalUtils::Units);

  namespace time {
    MonthDay::MonthDay(gint month, gint dayOfMonth): month_(month), day_(dayOfMonth) {}

    MonthDay MonthDay::now() {
      try {
        LocalDate now = LocalDate::now();
        return MonthDay::of(now.month(), now.dayOfMonth());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    MonthDay MonthDay::now(const ZoneId& zone) {
      try {
        LocalDate now = LocalDate::now(zone);
        return MonthDay::of(now.month(), now.dayOfMonth());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    MonthDay MonthDay::of(Month month, gint dayOfMonth) {
      gint monthOfYear = (gint) month;
      try {
        Fields::checkValue(monthOfYear, TemporalField::MONTH_OF_YEAR);
        Fields::checkValue(dayOfMonth, TemporalField::DAY_OF_MONTH);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }

      if (dayOfMonth < 0 || dayOfMonth > TemporalUtils::lengthOfMonth(monthOfYear, true))
        DateTimeException("Illegal value for field "_Sl + TemporalField::DAY_OF_MONTH
          + ", value "_Sl + dayOfMonth + " is not valid for month "_Sl + month).throws($ftrace());
      return MonthDay(monthOfYear, dayOfMonth);
    }

    MonthDay MonthDay::of(gint month, gint dayOfMonth) {
      try {
        return MonthDay::of((Month) month, dayOfMonth);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    MonthDay MonthDay::from(TemporalAccessor const& temporal) {
      if (Class<MonthDay>::hasInstance(temporal))
        return CORE_XCAST(MonthDay const, temporal);
      try {
        gint month = temporal.get(TemporalField::MONTH_OF_YEAR);
        gint dayOfMonth = temporal.get(TemporalField::DAY_OF_MONTH);
        return MonthDay::of(month, dayOfMonth);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    MonthDay MonthDay::parse(CharSequence const& text) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    MonthDay MonthDay::parse(CharSequence const& text, DateTimeFormatter const& formatter) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool MonthDay::isSupported(TemporalField field) const {
      return field == TemporalField::MONTH_OF_YEAR
          || field == TemporalField::DAY_OF_MONTH
          || Fields::isSupportedBy(*this, field);
    }

    ValueRange MonthDay::range(TemporalField field) const {
      try {
        switch (field) {
          case TemporalField::MONTH_OF_YEAR: return Fields::range(field);
          case TemporalField::DAY_OF_MONTH:
            return ValueRange::of(1, TemporalUtils::lengthOfMonth(month_, false),
                                  TemporalUtils::lengthOfMonth(month_, true));
          default: return Fields::range(*this, field);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint MonthDay::get(TemporalField field) const {
      try {
        return range(field).checkValidIntValue(getLong(field), field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong MonthDay::getLong(TemporalField field) const {
      try {
        switch (field) {
          case TemporalField::MONTH_OF_YEAR: return month_;
          case TemporalField::DAY_OF_MONTH: return day_;
          default: return Fields::getFrom(*this, field);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Month MonthDay::month() const {
      return (Month) month_;
    }

    gint MonthDay::dayOfMonth() const {
      return day_;
    }

    gbool MonthDay::isValidYear(gint year) const {
      return !(day_ == 29 && month_ == 2 && !TemporalUtils::isLeapYear(year));
    }

    MonthDay MonthDay::withMonth(gint month) const {
      try {
        Fields::checkValue(month, TemporalField::MONTH_OF_YEAR);
        return MonthDay::with((Month) month);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    MonthDay MonthDay::with(Month month) const {
      return MonthDay((gint) month, day_);
    }

    MonthDay MonthDay::withDayOfMonth(gint dayOfMonth) const {
      try {
        Fields::checkValue(dayOfMonth, TemporalField::DAY_OF_MONTH);
        return MonthDay::of(month_, dayOfMonth);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    TemporalAccessor::Optional MonthDay::query(TemporalQuery const& query) const {
      if (query == TemporalQuery::localDate()
        || query == TemporalQuery::localTime()
        || query == TemporalQuery::zone()
        || query == TemporalQuery::zoneId()
        || query == TemporalQuery::offset()
        || query == TemporalQuery::precision())
        return Optional::empty();
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& MonthDay::adjustInto(Temporal const& temporal) const {
      try {
        Temporal& t1 = adjustFieldTo(TemporalField::MONTH_OF_YEAR, month_, temporal);
        Temporal& t2 = adjustFieldTo(TemporalField::DAY_OF_MONTH, day_, t1);
        if (&t1 != &t2 && &t1 != &temporal)
          UNSAFE::deleteRegInstance(t1);
        return t2;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String MonthDay::format(DateTimeFormatter const& formatter) const {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    LocalDate MonthDay::atYear(gint year) const {
      try {
        return LocalDate::of(year, month_, isValidYear(year) ? day_ : 28);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint MonthDay::compareTo(MonthDay const& other) const {
      gint result = month_ - other.month_;
      if (result == 0)
        result = day_ - other.day_;
      return result;
    }

    gbool MonthDay::isAfter(MonthDay const& other) const {
      return compareTo(other) > 0;
    }

    gbool MonthDay::isBefore(MonthDay const& other) const {
      return compareTo(other) < 0;
    }

    gbool MonthDay::equals(Object const& other) const {
      return this == &other
          || Class<MonthDay>::hasInstance(other) && compareTo(CORE_XCAST(MonthDay const, other)) == 0;
    }

    gint MonthDay::hash() const {
      return (month_ << 6) + day_;
    }

    String MonthDay::toString() const {
      return XString(10).append("--"_Sl)
                        .append(month_ < 10 ? "0"_Sl : ""_Sl).append(month_)
                        .append(day_ < 10 ? "-0"_Sl : "-"_Sl).append(day_)
                        .toString();
    }

    Object& MonthDay::clone() const {
      return UNSAFE::newInstance<MonthDay>(*this);
    }
  } // time
} // core
