//
// Created by admin on 06/01/25.
//

#include "YearMonth.h"

#include <core/lang/Enum.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ValueRange.h>
#include <core/time/ZoneId.h>
#include <meta/time/TemporalUtils.h>
#include <core/util/Optional.h>

namespace core {
  using namespace util;

  namespace time {
    CORE_ALIAS(Fields, TemporalUtils::Fields);
    CORE_ALIAS(Units, TemporalUtils::Units);

    YearMonth::YearMonth(gint year, gint month) : year_(year), month_(month) {}

    YearMonth YearMonth::now() {
      return YearMonth::now(ZoneId::systemZone());
    }

    YearMonth YearMonth::now(ZoneId const& zone) {
      LocalDate now = LocalDate::now(zone);
      return YearMonth::of(now.year(), now.month());
    }

    YearMonth YearMonth::of(gint year, Month month) {
      try {
        return of(year, (gint) month);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::of(gint year, gint month) {
      try {
        Fields::checkValue(year, TemporalField::YEAR);
        Fields::checkValue(month, TemporalField::MONTH_OF_YEAR);
        return YearMonth(year, month);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::from(TemporalAccessor const& temporal) {
      if (Class<YearMonth>::hasInstance(temporal))
        return CORE_XCAST(YearMonth const, temporal);
      try {
        return of(temporal.get(TemporalField::YEAR), temporal.get(TemporalField::MONTH_OF_YEAR));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::parse(CharSequence const& text) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    YearMonth YearMonth::parse(CharSequence const& text, DateTimeFormatter const& formatter) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool YearMonth::isSupported(TemporalField field) const {
      return field == TemporalField::YEAR ||
          field == TemporalField::MONTH_OF_YEAR ||
          field == TemporalField::PROLEPTIC_MONTH ||
          field == TemporalField::YEAR_OF_ERA ||
          field == TemporalField::ERA;
    }

    gbool YearMonth::isSupported(TemporalUnit unit) const {
      return unit == TemporalUnit::MONTHS ||
          unit == TemporalUnit::YEARS ||
          unit == TemporalUnit::DECADES ||
          unit == TemporalUnit::CENTURIES ||
          unit == TemporalUnit::MILLENNIA ||
          unit == TemporalUnit::ERAS || Units::isSupportedBy(*this, unit);
    }

    ValueRange YearMonth::range(TemporalField field) const {
      if (field == TemporalField::YEAR_OF_ERA)
        return year_ <= 0
                 ? ValueRange::of(1, TemporalUtils::MAX_YEAR + 1)
                 : ValueRange::of(1, TemporalUtils::MAX_YEAR);
      try {
        return Fields::range(*this, field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint YearMonth::get(TemporalField field) const {
      try {
        return range(field).checkValidIntValue(getLong(field), field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong YearMonth::getLong(TemporalField field) const {
      try {
        switch (field) {
          case TemporalField::MONTH_OF_YEAR:
            return month_;
          case TemporalField::PROLEPTIC_MONTH:
            return prolepticMonth();
          case TemporalField::YEAR_OF_ERA:
            return year_ < 1 ? 1 - year_ : year_;
          case TemporalField::YEAR:
            return year_;
          case TemporalField::ERA:
            return year_ < 1 ? 0 : 1;
          default:
            return Fields::getFrom(*this, field);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint YearMonth::year() const {
      return year_;
    }

    Month YearMonth::month() const {
      return (Month) month_;
    }

    gbool YearMonth::isLeapYear() const {
      return TemporalUtils::isLeapYear(year_);
    }

    gbool YearMonth::isValidDay(gint dayOfMonth) const {
      return dayOfMonth >= 1 && dayOfMonth <= lengthOfMonth();
    }

    gint YearMonth::lengthOfMonth() const {
      return TemporalUtils::lengthOfMonth(month_, isLeapYear());
    }

    gint YearMonth::lengthOfYear() const {
      return isLeapYear() ? 366 : 365;
    }

    YearMonth YearMonth::with(TemporalAdjuster const& adjuster) const {
      try {
        return CORE_XCAST(YearMonth, adjuster.adjustInto(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::with(TemporalField field, glong newValue) const {
      try {
        Fields::checkValue(newValue, field);
        switch (field) {
          case TemporalField::MONTH_OF_YEAR:
            return withMonth(newValue);
          case TemporalField::PROLEPTIC_MONTH:
            return plusMonths(newValue - prolepticMonth());
          case TemporalField::YEAR_OF_ERA:
            return withYear(year_ < 0 ? 1 - year_ : year_);
          case TemporalField::YEAR:
            return withYear(newValue);
          default:
            break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported field "_Sl + field).throws($ftrace());
    }

    YearMonth YearMonth::withYear(gint year) const {
      try {
        return with(year, month_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::withMonth(gint month) const {
      try {
        return with(year_, month);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::plus(TemporalAmount const& amountToAdd) const {
      try {
        return CORE_XCAST(YearMonth, amountToAdd.addTo(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::plus(glong amountToAdd, TemporalUnit unit) const {
      try {
        switch (unit) {
          case TemporalUnit::MONTHS: return plusMonths(amountToAdd);
          case TemporalUnit::YEARS: return plusYears(amountToAdd);
          case TemporalUnit::DECADES: return plusYears(Math::multiplyExact(amountToAdd, 10));
          case TemporalUnit::CENTURIES: return plusYears(Math::multiplyExact(amountToAdd, 100));
          case TemporalUnit::MILLENNIA: return plusYears(Math::multiplyExact(amountToAdd, 1000));
          case TemporalUnit::ERAS:
            return with(TemporalField::ERA, Math::addExact(getLong(TemporalField::ERA), amountToAdd));
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit "_Sl + unit).throws($ftrace());
    }

    YearMonth YearMonth::plusYears(glong yearsToAdd) const {
      try {
        glong newYear = Math::addExact((glong) year_, yearsToAdd);
        Fields::checkValue(newYear, TemporalField::YEAR);
        return with(newYear, month_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::plusMonths(glong monthsToAdd) const {
      try {
        glong yearsToAdd = Math::floorDiv(monthsToAdd, 12);
        monthsToAdd = Math::floorMod(monthsToAdd, 12);

        glong newYear = Math::addExact((glong) year_, yearsToAdd);
        glong newMonth = Math::addExact((glong) month_, monthsToAdd);

        if (newMonth > 12) {
          newYear += 1;
          newMonth -= 12;
        } else if (newMonth < 1) {
          newYear -= 1;
          newMonth += 12;
        }

        Fields::checkValue(newYear, TemporalField::YEAR);
        Fields::checkValue(newMonth, TemporalField::MONTH_OF_YEAR);
        return with(newYear, newMonth);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::minus(TemporalAmount const& amountToSubtract) const {
      try {
        return CORE_XCAST(YearMonth, amountToSubtract.subtractFrom(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::minus(glong amountToSubtract, TemporalUnit unit) const {
      try {
        return (amountToSubtract == Long::MIN_VALUE)
                 ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                 : plus(-amountToSubtract, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::minusYears(glong yearsToSubtract) const {
      try {
        return (yearsToSubtract == Long::MIN_VALUE)
                 ? plusYears(Long::MAX_VALUE).plusYears(1)
                 : plusYears(-yearsToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    YearMonth YearMonth::minusMonths(glong monthsToSubtract) const {
      try {
        return (monthsToSubtract == Long::MIN_VALUE)
                 ? plusMonths(Long::MAX_VALUE).plusMonths(1)
                 : plusMonths(-monthsToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    TemporalAdjuster::Optional YearMonth::query(TemporalQuery const& query) const {
      if (query == TemporalQuery::localDate() ||
        query == TemporalQuery::localTime() ||
        query == TemporalQuery::zone() ||
        query == TemporalQuery::zoneId() ||
        query == TemporalQuery::offset() ||
        query == TemporalQuery::chronology())
        return Optional::empty();
      if (query == TemporalQuery::precision())
        return (Enum<TemporalUnit>) TemporalUnit::MONTHS;
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& YearMonth::adjustInto(Temporal const& temporal) const {
      try {
        return adjustFieldTo(TemporalField::PROLEPTIC_MONTH, prolepticMonth(), temporal);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong YearMonth::until(Temporal const& endExclusive, TemporalUnit unit) const {
      try {
        YearMonth end = YearMonth::from(endExclusive);
        glong monthsUntil = end.prolepticMonth() - prolepticMonth();
        switch (unit) {
          case TemporalUnit::MONTHS: return monthsUntil;
          case TemporalUnit::YEARS: return monthsUntil / 12;
          case TemporalUnit::DECADES: return monthsUntil / 120;
          case TemporalUnit::CENTURIES: return monthsUntil / 1200;
          case TemporalUnit::MILLENNIA: return monthsUntil / 12000;
          case TemporalUnit::ERAS: return end.getLong(TemporalField::ERA) - getLong(TemporalField::ERA);
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit "_Sl + unit).throws($ftrace());
    }

    String YearMonth::format(DateTimeFormatter const& formatter) const {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    LocalDate YearMonth::atDay(gint dayOfMonth) const {
      try {
        return LocalDate::of(year_, month_, dayOfMonth);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate YearMonth::atEndOfMonth() const {
      try {
        return LocalDate::of(year_, month_, lengthOfMonth());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint YearMonth::compareTo(YearMonth const& other) const {
      gint cmp = (year_ - other.year_);
      if (cmp == 0) {
        cmp = (month_ - other.month_);
      }
      return cmp;
    }

    gbool YearMonth::isAfter(YearMonth const& other) const {
      return compareTo(other) > 0;
    }

    gbool YearMonth::isBefore(YearMonth const& other) const {
      return compareTo(other) < 0;
    }

    gbool YearMonth::equals(Object const& other) const {
      return this == &other ||
          (Class<YearMonth>::hasInstance(other) && compareTo(CORE_XCAST(YearMonth const, other)) == 0);
    }

    gint YearMonth::hash() const {
      return year_ ^ (month_ << 27);
    }

    String YearMonth::toString() const {
      int absYear = Math::abs(year_);
      XString buf = XString(9);
      if (absYear < 1000) {
        if (year_ < 0) {
          buf.append(year_ - 10000).deleteCharAt(1);
        } else {
          buf.append(year_ + 10000).deleteCharAt(0);
        }
      } else {
        buf.append(year_);
      }
      return buf.append(month_ < 10 ? "-0" : "-")
                .append(month_)
                .toString();
    }

    Object& YearMonth::clone() const {
      return UNSAFE::newInstance<YearMonth>(*this);
    }

    YearMonth YearMonth::with(gint newYear, gint newMonth) const {
      if (newYear == year_ && newMonth == month_) {
        return *this;
      }
      return YearMonth(newYear, newMonth);
    }

    glong YearMonth::prolepticMonth() const {
      return (glong) year_ * 12 + month_ - 1;
    }
  } // time
} // core
