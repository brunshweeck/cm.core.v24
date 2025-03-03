//
// Created by brunshweeck on 22/02/25.
//

#include "Period.h"

#include <core/lang/ArithmeticException.h>
#include <core/lang/Enum.h>
#include <core/time/DateTimeException.h>
#include <core/time/LocalDate.h>
#include <core/time/TemporalQuery.h>
#include <core/util/List.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  namespace time {
    Period const Period::ZERO = Period(0, 0, 0);

    Period::Period(gint years, gint months, gint days): years_(years), months_(months), days_(days) {}

    Period Period::ofYears(gint years) {
      try {
        return create(years, 0, 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::ofMonths(gint months) {
      try {
        return create(0, months, 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::ofWeeks(gint weeks) {
      try {
        return create(0, 0, Math::multiplyExact(weeks, 7));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::ofDays(gint days) {
      try {
        return create(0, 0, days);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::of(gint years, gint months, gint days) {
      try {
        return create(years, months, days);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::from(TemporalAmount const& amount) {
      if (Class<Period>::hasInstance(amount))
        return CORE_XCAST(Period const, amount);
      gint years = 0;
      gint months = 0;
      gint days = 0;
      glong unitAmount = 0;
      for (TemporalUnit unit : amount.getUnits()) {
        try {
          unitAmount = amount.get(unit);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        try {
          if (unit == TemporalUnit::YEARS)
            years = Math::toIntExact(unitAmount);
          else if (unit == TemporalUnit::MONTHS)
            months = Math::toIntExact(unitAmount);
          else if (unit == TemporalUnit::DAYS)
            days = Math::toIntExact(unitAmount);
          else
            DateTimeException("Unit must be Years, Months or Days, but was "_Sl + unit).throws($ftrace());
        } catch (ArithmeticException const& ex) { ex.throws($ftrace()); }
      }
      try {
        return create(years, months, days);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::parse(CharSequence const& text) {
      try {
        return TemporalUtils::Periods::parse(text);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::between(LocalDate const& startDateInclusive, LocalDate const& endDateExclusive) {
      try {
        return startDateInclusive.until(endDateExclusive);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong Period::get(TemporalUnit unit) const {
      if (unit == TemporalUnit::YEARS)
        return years_;
      else if (unit == TemporalUnit::MONTHS)
        return months_;
      else if (unit == TemporalUnit::DAYS)
        return days_;
      else
        TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
    }

    TemporalAmount::UnitList& Period::getUnits() const {
      static UnitList& units = UnitList::of(TemporalUnit::YEARS, TemporalUnit::MONTHS, TemporalUnit::DAYS);
      return units;
    }

    gbool Period::isZero() const {
      return (years_ | months_ | days_) == 0;
    }

    gbool Period::isNegative() const {
      return (years_ | months_ | days_) < 0;
    }

    gint Period::years() const {
      return years_;
    }

    gint Period::months() const {
      return months_;
    }

    gint Period::days() const {
      return days_;
    }

    Period Period::withYears(gint years) const {
      try {
        return create(years, months(), days());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::withMonths(gint months) const {
      try {
        return create(years(), months, days());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::withDays(gint days) const {
      try {
        return create(years(), months(), days);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::plus(TemporalAmount const& amountToAdd) const {
      try {
        Period period = Period::from(amountToAdd);
        return create(
          Math::addExact(years_, period.years_),
          Math::addExact(months_, period.months_),
          Math::addExact(days_, period.days_));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::plusYears(glong yearsToAdd) const {
      try {
        return create(
          Math::toIntExact(Math::addExact((glong) years(), yearsToAdd)),
          months(),
          days());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::plusMonths(glong monthsToAdd) const {
      try {
        glong yearsToAdd = Math::floorDiv(monthsToAdd, 12);
        monthsToAdd = Math::floorMod(monthsToAdd, 12);
        return create(
          Math::toIntExact(Math::addExact((glong) years(), yearsToAdd)),
          Math::toIntExact(Math::addExact((glong) months(), monthsToAdd)),
          days());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::plusDays(glong daysToAdd) const {
      try {
        glong yearsToAdd = Math::floorDiv(daysToAdd, 1461 /* 365.250 * 4 */) * 4L;
        daysToAdd = Math::floorMod(daysToAdd, 1461);
        return create(
          Math::toIntExact(Math::addExact((glong) years(), yearsToAdd)),
          months(),
          Math::toIntExact(Math::addExact((glong) days(), daysToAdd)));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::minus(TemporalAmount const& amountToSubtract) const {
      try {
        Period period = Period::from(amountToSubtract);
        return create(
          Math::subtractExact(years_, period.years_),
          Math::subtractExact(months_, period.months_),
          Math::subtractExact(days_, period.days_));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::minusYears(glong yearsToSubtract) const {
      try {
        return (yearsToSubtract == Long::MIN_VALUE)
                 ? plusYears(Long::MAX_VALUE).plusYears(1)
                 : plusYears(-yearsToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::minusMonths(glong monthsToSubtract) const {
      try {
        return (monthsToSubtract == Long::MIN_VALUE)
                 ? plusMonths(Long::MAX_VALUE).plusMonths(1)
                 : plusMonths(-monthsToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::minusDays(glong daysToSubtract) const {
      try {
        return (daysToSubtract == Long::MIN_VALUE)
                 ? plusDays(Long::MAX_VALUE).plusDays(1)
                 : plusDays(-daysToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::multiplyBy(gint scalar) const {
      try {
        static CORE_FAST gint MAX_CYCLES = Integer::MAX_VALUE / 1461;

        glong years = Math::multiplyExact((glong) years_, scalar);
        glong months = Math::multiplyExact((glong) months_, scalar);
        glong days = Math::multiplyExact((glong) days_, scalar);

        glong cycles = days / 1461;
        years = Math::addExact(years, (cycles - MAX_CYCLES) * 4);
        days -= (cycles - MAX_CYCLES) * 1461;

        static CORE_FAST gint MAX_YEARS = Integer::MAX_VALUE / 12;
        glong yearsToAdd = months / 12;
        years = Math::addExact(years, yearsToAdd - MAX_YEARS);
        months -= (yearsToAdd - MAX_YEARS) * 12;

        return create(
          Math::toIntExact(years),
          Math::toIntExact(months),
          Math::toIntExact(days));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::negated() const {
      try {
        return multiplyBy(-1);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Period Period::normalized() const {
      try {
        glong totalMonths = toTotalMonths();
        glong splitYears = totalMonths / 12;
        gint splitMonths = totalMonths % 12;

        return create(splitYears, splitMonths, days());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong Period::toTotalMonths() const {
      try {
        return (glong) years() * 12L + months();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& Period::addTo(Temporal const& temporal) const {
      OptionalTemporal t1, t2;
      try {
        validateChronology(temporal);
        if (months() == 0) {
          if (years() != 0) {
            t1 = addAmountTo(years(), TemporalUnit::YEARS, temporal);
          }
        } else {
          glong totalMonths = toTotalMonths();
          if (totalMonths != 0) {
            t1 = addAmountTo(totalMonths, TemporalUnit::MONTHS, temporal);
          }
        }
        if (days() != 0) {
          if (t1.isPresent())
            t2 = addAmountTo(days(), TemporalUnit::DAYS, t1.get());
          else
            t2 = addAmountTo(days(), TemporalUnit::DAYS, temporal);
        }

        if (t1.isPresent()) {
          if (t2.isPresent()) {
            if (&t1.get() != &t2.get() && &t1.get() != &temporal)
              UNSAFE::deleteRegInstance(t1.get());
          }
          return t1.get();
        }
        if (t2.isPresent())
          return t2.get();
        return CORE_XCAST(Temporal, temporal.clone());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& Period::subtractFrom(Temporal const& temporal) const {
      OptionalTemporal t1, t2;
      try {
        validateChronology(temporal);
        if (months() == 0) {
          if (years() != 0) {
            t1 = subtractAmountFrom(years(), TemporalUnit::YEARS, temporal);
          }
        } else {
          glong totalMonths = toTotalMonths();
          if (totalMonths != 0) {
            t1 = subtractAmountFrom(totalMonths, TemporalUnit::MONTHS, temporal);
          }
        }
        if (days() != 0) {
          if (t1.isPresent())
            t2 = subtractAmountFrom(days(), TemporalUnit::DAYS, t1.get());
          else
            t2 = subtractAmountFrom(days(), TemporalUnit::DAYS, temporal);
        }

        if (t1.isPresent()) {
          if (t2.isPresent()) {
            if (&t1.get() != &t2.get() && &t1.get() != &temporal)
              UNSAFE::deleteRegInstance(t1.get());
          }
          return t1.get();
        }
        if (t2.isPresent())
          return t2.get();
        return CORE_XCAST(Temporal, temporal.clone());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool Period::equals(Object const& other) const {
      if (this == &other)
        return true;
      if (!Class<Period>::hasInstance(other))
        return false;
      Period const& that = CORE_XCAST(Period const, other);
      return years() == that.years() &&
          months() == that.months() &&
          days() == that.days();
    }

    gint Period::hash() const {
      return years() + Integer::rotateLeft(months(), 8) + Integer::rotateLeft(days(), 16);
    }

    String Period::toString() const {
      if (isZero()) {
        return "P0D";
      } else {
        XString buf;
        buf.append(u'P');
        if (years_ != 0) {
          buf.append(years_).append(u'Y');
        }
        if (months_ != 0) {
          buf.append(months_).append(u'M');
        }
        if (days_ != 0) {
          buf.append(days_).append(u'D');
        }
        return buf.toString();
      }
    }

    Object& Period::clone() const {
      return UNSAFE::newInstance<Period>(*this);
    }

    Period Period::create(gint years, gint months, gint days) {
      return Period(years, months, days);
    }

    void Period::validateChronology(TemporalAccessor const& temporal) {
      Optional chronology;
      try {
        chronology = temporal.query(TemporalQuery::chronology());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      if (chronology.isPresent() /* && chronology.get() ==IsoChronology::getInstance()*/)
        DateTimeException("Chronology mismatch, expected: ISO, actual: "_Sl +
          chronology.get()).throws($ftrace());
    }
  } // time
} // core
