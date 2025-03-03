//
// Created by brunshweeck on 24/02/25.
//

#include "Year.h"

#include <core/time/ValueRange.h>

#include "LocalDate.h"
#include "MonthDay.h"
#include "TemporalQuery.h"
#include "YearMonth.h"
#include "ZoneId.h"
#include <core/lang/Enum.h>
#include <core/util/Optional.h>
#include "meta/time/TemporalUtils.h"

namespace core {
  namespace time {
    CORE_ALIAS(Fields, TemporalUtils::Fields);
    CORE_ALIAS(Units, TemporalUtils::Units);

    Year::Year(gint year): year(year) {}

    Year Year::now() {
      try {
        return now(ZoneId::systemZone());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Year Year::now(ZoneId const& zone) {
      try {
        LocalDate now = LocalDate::now(zone);
        return Year::of(now.year());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Year Year::of(gint isoYear) {
      try {
        Fields::checkValue(isoYear, TemporalField::YEAR);
        return Year(isoYear);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Year Year::from(TemporalAccessor const& temporal) {
      if (Class<Year>::hasInstance(temporal))
        return CORE_XCAST(Year const, temporal);
      try {
        return Year::of(temporal.get(TemporalField::YEAR));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Year Year::parse(CharSequence const& text) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    Year Year::parse(CharSequence const& text, DateTimeFormatter const& formatter) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool Year::isLeap(gint year) {
      // A year that is a multiple of 100, 200 and 300 is not divisible by 16, but 400 is.
      // So for a year that's divisible by 4, checking that it's also divisible by 16
      // is sufficient to determine it must be a leap year.
      return (year & 15) == 0 ? (year & 3) == 0 : (year & 3) == 0 && year % 100 != 0;
    }

    gint Year::get() const {
      return year;
    }

    gbool Year::isSupported(TemporalField field) const {
      return field == TemporalField::YEAR ||
          field == TemporalField::YEAR_OF_ERA ||
          field == TemporalField::ERA ||
          Fields::isSupportedBy(*this, field);
    }

    gbool Year::isSupported(TemporalUnit unit) const {
      return unit == TemporalUnit::YEARS ||
          unit == TemporalUnit::DECADES ||
          unit == TemporalUnit::CENTURIES ||
          unit == TemporalUnit::MILLENNIA ||
          unit == TemporalUnit::ERAS ||
          Units::isSupportedBy(*this, unit);
    }

    ValueRange Year::range(TemporalField field) const {
      if (field == TemporalField::YEAR_OF_ERA)
        return year <= 0 ? ValueRange::of(1, MAX_VALUE + 1) : ValueRange::of(1, MAX_VALUE);
      try {
        return Fields::range(*this, field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint Year::get(TemporalField field) const {
      try {
        return range(field).checkValidIntValue(getLong(field), field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong Year::getLong(TemporalField field) const {
      try {
        switch (field) {
          case TemporalField::YEAR: return year;
          case TemporalField::YEAR_OF_ERA: return year < 0 ? 1 - year : year;
          case TemporalField::ERA: return year < 0 ? 0 : 1;
          default: return Fields::getFrom(*this, field);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool Year::isLeap() const {
      return isLeap(year);
    }

    gbool Year::isValidMonthDay(MonthDay const& monthDay) const {
      return monthDay.isValidYear(year);
    }

    gint Year::length() const {
      return isLeap() ? 366 : 365;
    }

    Year Year::with(TemporalAdjuster const& adjuster) const {
      return CORE_XCAST(Year, adjuster.adjustInto(*this));
    }

    Year Year::with(TemporalField field, glong newValue) const {
      try {
        Fields::checkValue(newValue, field);
        switch (field) {
          case TemporalField::YEAR: return Year::of((gint) newValue);
          case TemporalField::YEAR_OF_ERA: return Year::of(newValue < 1 ? 1 - newValue : newValue);
          case TemporalField::ERA: getLong(TemporalField::ERA) == newValue ? *this : Year::of(1 - year);
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported field "_Sl + field).throws($ftrace());
    }

    Year Year::plus(TemporalAmount const& amount) const {
      try {
        return CORE_XCAST(Year, amount.addTo(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Year Year::plus(glong amountToAdd, TemporalUnit unit) const {
      try {
        switch (unit) {
          case TemporalUnit::YEARS: return plusYears(amountToAdd);
          case TemporalUnit::DECADES: return plusYears(Math::multiplyExact(amountToAdd, 10));
          case TemporalUnit::CENTURIES: return plusYears(Math::multiplyExact(amountToAdd, 100));
          case TemporalUnit::MILLENNIA: return plusYears(Math::multiplyExact(amountToAdd, 1000));
          case TemporalUnit::ERAS: return with(TemporalField::ERA,
                                               Math::addExact(getLong(TemporalField::ERA), amountToAdd));
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported field "_Sl + unit).throws($ftrace());
    }

    Year Year::plusYears(glong yearsToAdd) const {
      try {
        glong newYear = Math::addExact((glong) year, yearsToAdd);
        Fields::checkValue(newYear, TemporalField::YEAR);
        return Year::of((gint) newYear);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Year Year::minus(TemporalAmount const& amount) const {
      try {
        return CORE_XCAST(Year, amount.subtractFrom(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Year Year::minus(glong amountToSubtract, TemporalUnit unit) const {
      try {
        return (amountToSubtract == Long::MIN_VALUE)
                 ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                 : plus(-amountToSubtract, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Year Year::minusYears(glong yearsToSubtract) const {
      try {
        return (yearsToSubtract == Long::MIN_VALUE)
                 ? plusYears(yearsToSubtract).plusYears(1)
                 : plusYears(-yearsToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    TemporalAdjuster::Optional Year::query(const TemporalQuery& query) const {
      if (query == TemporalQuery::precision())
        return (Enum<TemporalUnit>) TemporalUnit::YEARS;
      if (query == TemporalQuery::localDate() ||
        query == TemporalQuery::localTime() ||
        query == TemporalQuery::offset() ||
        query == TemporalQuery::zoneId() ||
        query == TemporalQuery::zone() ||
        query == TemporalQuery::chronology())
        return Optional::empty();
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& Year::adjustInto(Temporal const& temporal) const {
      try {
        return adjustFieldTo(TemporalField::YEAR, year, temporal);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong Year::until(Temporal const& endExclusive, TemporalUnit unit) const {
      try {
        Year end = Year::from(endExclusive);
        glong yearsUntil = end.year - year;
        switch (unit) {
          case TemporalUnit::YEARS: return yearsUntil;
          case TemporalUnit::DECADES: return yearsUntil / 10;
          case TemporalUnit::CENTURIES: return yearsUntil / 100;
          case TemporalUnit::MILLENNIA: return yearsUntil / 1000;
          case TemporalUnit::ERAS: return end.getLong(TemporalField::ERA) - getLong(TemporalField::ERA);
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit "_Sl + unit).throws($ftrace());
    }

    String Year::format(DateTimeFormatter const& formatter) const {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    LocalDate Year::atDay(gint dayOfYear) const {
      try {
        return LocalDate::ofYearDay(year, dayOfYear);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth Year::atMonth(Month month) const {
      try {
        return YearMonth::of(year, month);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth Year::atMonth(gint month) const {
      try {
        return YearMonth::of(year, month);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate Year::atMonthDay(MonthDay const& monthDay) const {
      try {
        return LocalDate::of(year, monthDay.month(), monthDay.dayOfMonth());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint Year::compareTo(Year const& other) const {
      return year - other.year;
    }

    gbool Year::isAfter(Year const& other) const {
      return compareTo(other) > 0;
    }

    gbool Year::isBefore(Year const& other) const {
      return compareTo(other) < 0;
    }

    gbool Year::equals(Object const& other) const {
      return this == &other ||
          Class<Year>::hasInstance(other) && compareTo(CORE_XCAST(Year const, other)) == 0;
    }

    gint Year::hash() const {
      return year;
    }

    String Year::toString() const {
      return Integer::toString(year);
    }
  } // time
} // core
