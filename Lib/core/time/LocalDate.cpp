//
// Created by brunshweeck on 3 août 2024.
//

#include "LocalDate.h"

#include <core/lang/Enum.h>
#include <core/time/DateTimeException.h>
#include <core/time/Instant.h>
#include <core/time/Month.h>
#include <core/time/LocalDateTime.h>
#include <core/time/Period.h>
#include <core/time/TemporalAmount.h>
#include <core/time/TemporalException.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ValueRange.h>
#include <core/time/ZonedDateTime.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneOffsetTransition.h>
#include <core/time/ZoneRules.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  using namespace util;

  namespace time {
    CORE_ALIAS(Fields, TemporalUtils::Fields);
    CORE_ALIAS(Units, TemporalUtils::Units);

    LocalDate const LocalDate::MIN = LocalDate::of(-999999999, Month::JANUARY, 01);

    LocalDate const LocalDate::MAX = LocalDate::of(+999999999, Month::DECEMBER, 31);

    LocalDate const LocalDate::EPOCH = LocalDate::of(1970, Month::JANUARY, 31);


    LocalDate::LocalDate(gint year, gint month, gint dayOfMonth): year_(year), month_(month), day_(dayOfMonth) {}

    LocalDate LocalDate::now() {
      return now(ZoneId::systemZone());
    }

    LocalDate LocalDate::now(ZoneId const& zone) {
      return LocalDateTime::now(zone).toLocalDate();
    }

    LocalDate LocalDate::of(gint year, Month month, gint dayOfMonth) {
      try {
        Fields::checkValue(year, TemporalField::YEAR);
        Fields::checkValue(dayOfMonth, TemporalField::DAY_OF_MONTH);
        return create(year, (gint) month, dayOfMonth);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::of(gint year, gint month, gint dayOfMonth) {
      try {
        Fields::checkValue(year, TemporalField::YEAR);
        Fields::checkValue(month, TemporalField::MONTH_OF_YEAR);
        Fields::checkValue(dayOfMonth, TemporalField::DAY_OF_MONTH);
        return create(year, month, dayOfMonth);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::ofYearDay(gint year, gint dayOfYear) {
      try {
        Fields::checkValue(year, TemporalField::YEAR);
        Fields::checkValue(dayOfYear, TemporalField::DAY_OF_YEAR);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      gbool leap = TemporalUtils::isLeapYear(year);
      if (dayOfYear == 366 && !leap) {
        DateTimeException("Invalid date 'DayOfYear 366' as '"_Sl + year + "' is not a leap year"_Sl)
            .throws($ftrace());
      }
      gint moy = (dayOfYear - 1) / 31 + 1;
      int monthEnd = TemporalUtils::firstDayOfMonthInYear(moy, leap)
          + TemporalUtils::lengthOfMonth(moy, leap) - 1;
      if (dayOfYear > monthEnd) {
        moy = moy == 12 ? 1 : moy + 1;
      }
      int dom = dayOfYear - TemporalUtils::firstDayOfMonthInYear(moy, leap) + 1;
      return LocalDate(year, moy, dom);
    }

    LocalDate LocalDate::ofInstant(Instant const& instant, ZoneId const& zone) {
      try {
        ZoneRules rules = zone.getRules();
        ZoneOffset offset = rules.offset(instant);
        glong localSecond = instant.epochSecond() + offset.totalSeconds();
        glong localEpochDay = Math::floorDiv(localSecond, SECONDS_PER_DAY);
        return ofEpochDay(localEpochDay);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::ofEpochDay(glong epochDay) {
      try {
        Fields::checkValue(epochDay, TemporalField::EPOCH_DAY);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      glong zeroDay = epochDay + DAYS_0000_TO_1970;
      // find the march-based year
      zeroDay -= 60; // adjust to 0000-03-01 so leap day is at end of four-year cycle
      glong adjust = 0;
      if (zeroDay < 0) {
        // adjust negative years to positive for calculation
        glong adjustCycles = (zeroDay + 1) / DAYS_PER_CYCLE - 1;
        adjust = adjustCycles * 400;
        zeroDay += -adjustCycles * DAYS_PER_CYCLE;
      }
      glong yearEst = (400 * zeroDay + 591) / DAYS_PER_CYCLE;
      glong doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
      if (doyEst < 0) {
        // fix estimate
        yearEst--;
        doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
      }
      yearEst += adjust; // reset any negative year
      gint marchDoy0 = (gint) doyEst;

      // convert march-based values back to january-based
      gint marchMonth0 = (marchDoy0 * 5 + 2) / 153;
      gint month = marchMonth0 + 3;
      if (month > 12) {
        month -= 12;
      }
      gint dom = marchDoy0 - (marchMonth0 * 306 + 5) / 10 + 1;
      if (marchDoy0 >= 306) {
        yearEst++;
      }

      return LocalDate((gint) yearEst, month, dom);
    }

    LocalDate LocalDate::from(TemporalAccessor const& temporal) {
      OptionalDate date = temporal.query(TemporalQuery::localDate()).tryCast<LocalDate>();
      if (date.isEmpty()) {
        DateTimeException("Unable to obtain LocalDate from TemporalAccessor: "_Sl +
                  temporal + " of type " + typeName(temporal)).throws($ftrace());
      }
      return date.get();
    }

    LocalDate LocalDate::parse(CharSequence const& text) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    LocalDate LocalDate::parse(CharSequence const& text, DateTimeFormatter const& formatter) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool LocalDate::isSupported(TemporalField field) const {
      try {
        return Fields::isDateBased(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool LocalDate::isSupported(TemporalUnit unit) const {
      try {
        return Units::isDateBased(unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ValueRange LocalDate::range(TemporalField field) const {
      if (Fields::isDateBased(field)) {
        switch (field) {
          case TemporalField::DAY_OF_MONTH:
            return ValueRange::of(1, lengthOfMonth());
          case TemporalField::DAY_OF_YEAR:
            return ValueRange::of(1, lengthOfYear());
          case TemporalField::ALIGNED_WEEK_OF_MONTH:
            return ValueRange::of(1, month() == Month::FEBRUARY && isLeapYear() ? 4 : 5);
          case TemporalField::YEAR_OF_ERA:
            return year() <= 0
                     ? ValueRange::of(1, TemporalUtils::MAX_YEAR + 1)
                     : ValueRange::of(1, TemporalUtils::MAX_YEAR);
          default: return Fields::range(field);
        }
      }
      TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
    }

    gint LocalDate::get(TemporalField field) const {
      glong result = 0;
      try {
        result = getLong(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      if (result > Integer::MAX_VALUE || result < Integer::MIN_VALUE) {
        TemporalException("Value of field '"_Sl + field + "' out of range."_Sl).throws($ftrace());
      }
      return (gint) result;
    }

    glong LocalDate::getLong(TemporalField field) const {
      switch (field) {
        case TemporalField::DAY_OF_WEEK: return (gint) dayOfWeek();
        case TemporalField::ALIGNED_DAY_OF_WEEK_IN_MONTH: return ((day_ - 1) % 7) + 1;
        case TemporalField::ALIGNED_DAY_OF_WEEK_IN_YEAR: return ((dayOfYear() - 1) % 7) + 1;
        case TemporalField::DAY_OF_MONTH: return day_;
        case TemporalField::DAY_OF_YEAR: return dayOfYear();
        case TemporalField::EPOCH_DAY: return toEpochDay();
        case TemporalField::ALIGNED_WEEK_OF_MONTH: return ((day_ - 1) / 7) + 1;
        case TemporalField::ALIGNED_WEEK_OF_YEAR: return ((dayOfYear() - 1) / 7) + 1;
        case TemporalField::MONTH_OF_YEAR: return month_;
        case TemporalField::PROLEPTIC_MONTH: return prolepticMonth();
        case TemporalField::YEAR_OF_ERA: return year_ >= 1 ? year_ : 1 - year_;
        case TemporalField::YEAR: return year_;
        case TemporalField::ERA: return year_ >= 1 ? 1 : 0;
        case TemporalField::DAY_OF_QUARTER: return Fields::getDayOfQuarterFrom(*this);
        case TemporalField::QUARTER_OF_YEAR: return Fields::getQuarterOfYearFrom(*this);
        case TemporalField::WEEK_OF_WEEK_BASED_YEAR: return Fields::getWeekOfWeekBasedYearFrom(*this);
        case TemporalField::WEEK_BASED_YEAR: return Fields::getWeekBasedYearFrom(*this);
        case TemporalField::JULIAN_DAY: return Fields::getJulianDayFrom(*this);
        case TemporalField::MODIFIED_JULIAN_DAY: return Fields::getModifiedJulianDayFrom(*this);
        case TemporalField::RATA_DIE: return Fields::getRataDieFrom(*this);
        default: TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
      }
    }

    gint LocalDate::year() const {
      return year_;
    }

    Month LocalDate::month() const {
      return (Month) month_;
    }

    gint LocalDate::dayOfMonth() const {
      return day_;
    }

    gint LocalDate::dayOfYear() const {
      return TemporalUtils::firstDayOfMonthInYear(month_, isLeapYear()) + day_ - 1;
    }

    DayOfWeek LocalDate::dayOfWeek() const {
      gint dow = Math::floorMod(toEpochDay() + 3, 7);
      return (DayOfWeek) dow;
    }

    gbool LocalDate::isLeapYear() const {
      return TemporalUtils::isLeapYear(year_);
    }

    gint LocalDate::lengthOfMonth() const {
      return TemporalUtils::lengthOfMonth(month_, isLeapYear());
    }

    gint LocalDate::lengthOfYear() const {
      return isLeapYear() ? 366 : 365;
    }

    LocalDate LocalDate::with(TemporalAdjuster const& adjuster) const {
      if (Class<LocalDate>::hasInstance(adjuster))
        return CORE_XCAST(LocalDate const, adjuster);
      try {
        Temporal& t = adjuster.adjustInto(*this);
        LocalDate date = CORE_XCAST(LocalDate, t);
        if (&t != this)
          UNSAFE::deleteRegInstance(t);
        return date;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::with(TemporalField field, glong newValue) const {
      try {
        Fields::checkValue(newValue, field);
        switch (field) {
          case TemporalField::DAY_OF_WEEK: return plusDays(newValue - (gint) dayOfWeek());
          case TemporalField::ALIGNED_DAY_OF_WEEK_IN_MONTH:
            return plusDays(newValue - getLong(TemporalField::ALIGNED_DAY_OF_WEEK_IN_MONTH));
          case TemporalField::ALIGNED_DAY_OF_WEEK_IN_YEAR:
            return plusDays(newValue - getLong(TemporalField::ALIGNED_DAY_OF_WEEK_IN_YEAR));
          case TemporalField::DAY_OF_MONTH: return withDayOfMonth((gint) newValue);
          case TemporalField::DAY_OF_YEAR: return withDayOfYear((gint) newValue);
          case TemporalField::EPOCH_DAY: return LocalDate::ofEpochDay(newValue);
          case TemporalField::ALIGNED_WEEK_OF_MONTH:
            return plusWeeks(newValue - getLong(TemporalField::ALIGNED_WEEK_OF_MONTH));
          case TemporalField::ALIGNED_WEEK_OF_YEAR:
            return plusWeeks(newValue - getLong(TemporalField::ALIGNED_WEEK_OF_YEAR));
          case TemporalField::MONTH_OF_YEAR: return withMonth((gint) newValue);
          case TemporalField::PROLEPTIC_MONTH: return plusMonths(newValue - prolepticMonth());
          case TemporalField::YEAR_OF_ERA: return withYear((gint) (year_ >= 1 ? newValue : 1 - newValue));
          case TemporalField::YEAR: return withYear((gint) newValue);
          case TemporalField::ERA:
            return getLong(TemporalField::ERA) == newValue ? *this : withYear(1 - year_);
          case TemporalField::DAY_OF_QUARTER: return withDayOfYear(getLong(TemporalField::DAY_OF_YEAR)
              + (newValue - getLong(TemporalField::DAY_OF_QUARTER)));
          case TemporalField::QUARTER_OF_YEAR: return withDayOfYear(getLong(TemporalField::MONTH_OF_YEAR)
              + (newValue - getLong(TemporalField::QUARTER_OF_YEAR)) * 3);
          case TemporalField::WEEK_OF_WEEK_BASED_YEAR: return plusWeeks(Math::subtractExact(newValue,
              getLong(TemporalField::WEEK_OF_WEEK_BASED_YEAR)));
          case TemporalField::WEEK_BASED_YEAR: {
            int newWby = (gint) newValue;
            int dow = get(TemporalField::DAY_OF_WEEK);
            int week = Fields::getWeek(*this);
            if (week == 53 && Fields::getWeekRange(newWby) == 52) {
              week = 52;
            }
            LocalDate resolved = LocalDate::of(newWby, 1, 4); // 4th is guaranteed to be in week one
            int days = (dow - resolved.get(TemporalField::DAY_OF_WEEK)) + ((week - 1) * 7);
            return resolved.plusDays(days);
          }
          case TemporalField::JULIAN_DAY:
            return with(TemporalField::EPOCH_DAY, Math::subtractExact(newValue, JULIAN_DAY_OFFSET));
          case TemporalField::MODIFIED_JULIAN_DAY:
            return with(TemporalField::EPOCH_DAY, Math::subtractExact(newValue, 40587L));
          case TemporalField::RATA_DIE:
            return with(TemporalField::EPOCH_DAY, Math::subtractExact(newValue, 719163L));
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
    }

    LocalDate LocalDate::withYear(gint year) const {
      try {
        Fields::checkValue(year, TemporalField::YEAR);
        return resolve(year, month_, day_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::withMonth(gint month) const {
      try {
        Fields::checkValue(month, TemporalField::MONTH_OF_YEAR);
        return resolve(year_, month, day_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::withDayOfMonth(gint dayOfMonth) const {
      try {
        Fields::checkValue(dayOfMonth, TemporalField::DAY_OF_MONTH);
        return of(year_, month_, dayOfMonth);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::withDayOfYear(gint dayOfYear) const {
      try {
        return ofYearDay(year_, dayOfYear);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::plus(TemporalAmount const& amount) const {
      try {
        Temporal& temporal = amount.addTo(*this);
        LocalDate date = CORE_XCAST(LocalDate, temporal);
        UNSAFE::deleteRegInstance(temporal);
        return date;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::plus(glong amountToAdd, TemporalUnit unit) const {
      try {
        switch (unit) {
          case TemporalUnit::DAYS: return plusDays(amountToAdd);
          case TemporalUnit::WEEKS: return plusWeeks(amountToAdd);
          case TemporalUnit::MONTHS: return plusMonths(amountToAdd);
          case TemporalUnit::YEARS: return plusYears(amountToAdd);
          case TemporalUnit::DECADES: return plusYears(Math::multiplyExact(amountToAdd, 10));
          case TemporalUnit::CENTURIES: return plusYears(Math::multiplyExact(amountToAdd, 100));
          case TemporalUnit::MILLENNIA: return plusYears(Math::multiplyExact(amountToAdd, 1000));
          case TemporalUnit::ERAS: return with(TemporalField::ERA, amountToAdd);
          case TemporalUnit::WEEK_BASED_YEARS:
            return with(TemporalField::WEEK_BASED_YEAR,
                        Math::addExact(getLong(TemporalField::WEEK_BASED_YEAR), amountToAdd));
          case TemporalUnit::QUARTER_YEARS: return plus(amountToAdd / 4, TemporalUnit::YEARS)
                .plus((amountToAdd % 4) * 3, TemporalUnit::MONTHS);
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
    }

    LocalDate LocalDate::plusYears(glong yearsToAdd) const {
      try {
        gint newYear = (gint) Fields::checkValue(year_ + yearsToAdd, TemporalField::YEAR);
        return resolve(newYear, month_, day_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::plusMonths(glong monthsToAdd) const {
      try {
        glong newYear = year_ + Math::floorDiv(monthsToAdd, 12);
        gint newMonth = month_ + Math::floorMod(monthsToAdd, 12);
        if (newMonth > 12) {
          newYear += 1;
          newMonth -= 12;
        } else if (newMonth < 1) {
          newYear -= 1;
          newMonth += 12;
        }
        return resolve(newYear, newMonth, day_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::plusWeeks(glong weeksToAdd) const {
      try {
        return plusDays(Math::multiplyExact(weeksToAdd, 7));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::plusDays(glong daysToAdd) const {
      try {
        glong newDay = day_ + daysToAdd;
        if (newDay > 0) {
          if (newDay <= 28)
            return LocalDate::of(year_, month_, (gint) newDay);
          else if (newDay <= 59) {
            gint n = lengthOfMonth();
            if (newDay <= n)
              return LocalDate::of(year_, month_, (gint) newDay);
            gint newMonth = (month_ == 12) ? 1 : month_ + 1;
            gint newYear = (month_ == 12) ? year_ + 1 : year_;
            return LocalDate::of(newYear, newMonth, (gint) (newDay - n));
          }
        }
        return LocalDate::ofEpochDay(Math::addExact(toEpochDay(), daysToAdd));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::minus(TemporalAmount const& amount) const {
      try {
        Temporal& temporal = amount.subtractFrom(*this);
        LocalDate date = CORE_XCAST(LocalDate, temporal);
        UNSAFE::deleteRegInstance(temporal);
        return date;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::minus(glong amountToSubtract, TemporalUnit unit) const {
      try {
        return (amountToSubtract == Long::MIN_VALUE)
                 ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                 : plus(-amountToSubtract, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::minusYears(glong yearsToSubtract) const {
      try {
        return (yearsToSubtract == Long::MIN_VALUE)
                 ? plusYears(Long::MAX_VALUE).plusYears(1)
                 : plusYears(-yearsToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::minusMonths(glong monthsToSubtract) const {
      try {
        return (monthsToSubtract == Long::MIN_VALUE)
                 ? plusMonths(Long::MAX_VALUE).plusMonths(1)
                 : plusMonths(-monthsToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::minusWeeks(glong weeksToSubtract) const {
      try {
        return (weeksToSubtract == Long::MIN_VALUE)
                 ? plusWeeks(Long::MAX_VALUE).plusWeeks(1)
                 : plusWeeks(-weeksToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDate::minusDays(glong daysToSubtract) const {
      try {
        return (daysToSubtract == Long::MIN_VALUE)
                 ? plusDays(Long::MAX_VALUE).plusDays(1)
                 : plusDays(-daysToSubtract);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    TemporalAccessor::Optional LocalDate::query(TemporalQuery const& query) const {
      if (query == TemporalQuery::localDate())
        return *this;
      if (query == TemporalQuery::precision())
        return (Enum<TemporalUnit>) TemporalUnit::DAYS;
      if (query == TemporalQuery::localTime()
        || query == TemporalQuery::zone()
        || query == TemporalQuery::zoneId()
        || query == TemporalQuery::offset())
        return Optional::empty();
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& LocalDate::adjustInto(Temporal const& temporal) const {
      try {
        return adjustFieldTo(TemporalField::EPOCH_DAY, toEpochDay(), temporal);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong LocalDate::until(const Temporal& endExclusive, TemporalUnit unit) const {
      try {
        LocalDate end = LocalDate::from(endExclusive);
        switch (unit) {
          case TemporalUnit::DAYS: return daysUntil(end);
          case TemporalUnit::WEEKS: return daysUntil(end) / 7;
          case TemporalUnit::MONTHS: return monthsUntil(end);
          case TemporalUnit::YEARS: return monthsUntil(end) / 12;
          case TemporalUnit::DECADES: return monthsUntil(end) / 120;
          case TemporalUnit::CENTURIES: return monthsUntil(end) / 1200;
          case TemporalUnit::MILLENNIA: return monthsUntil(end) / 12000;
          case TemporalUnit::ERAS: return end.getLong(TemporalField::ERA) - getLong(TemporalField::ERA);
          case TemporalUnit::WEEK_BASED_YEARS:
            return Math::subtractExact(end.getLong(TemporalField::WEEK_BASED_YEAR),
                                       getLong(TemporalField::WEEK_BASED_YEAR));
          case TemporalUnit::QUARTER_YEARS: return monthsUntil(end) / 3;
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
    }

    Period LocalDate::until(LocalDate const& endDateExclusive) const {
      try {
        LocalDate end = LocalDate::from(endDateExclusive);
        glong totalMonths = end.prolepticMonth() - prolepticMonth(); // safe
        gint days = end.day_ - this->day_;
        if (totalMonths > 0 && days < 0) {
          totalMonths--;
          LocalDate calcDate = plusMonths(totalMonths);
          days = (gint) (end.toEpochDay() - calcDate.toEpochDay()); // safe
        } else if (totalMonths < 0 && days > 0) {
          totalMonths++;
          days -= end.lengthOfMonth();
        }
        glong years = totalMonths / 12; // safe
        gint months = (gint) (totalMonths % 12); // safe
        return Period::of(Math::toIntExact(years), months, days);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String LocalDate::format(DateTimeFormatter const& formatter) const {
      // TODO: implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    LocalDateTime LocalDate::atTime(LocalTime const& time) const {
      return LocalDateTime::of(*this, time);
    }

    LocalDateTime LocalDate::atTime(gint hour, gint minute) const {
      return LocalDateTime::of(*this, LocalTime::of(hour, minute));
    }

    LocalDateTime LocalDate::atTime(gint hour, gint minute, gint second) const {
      return LocalDateTime::of(*this, LocalTime::of(hour, minute, second));
    }

    LocalDateTime LocalDate::atTime(gint hour, gint minute, gint second, gint nanoOfSecond) const {
      return LocalDateTime::of(*this, LocalTime::of(hour, minute, second, nanoOfSecond));
    }

    LocalDateTime LocalDate::atStartOfDay() const {
      return LocalDateTime::of(*this, LocalTime::MIDNIGHT);
    }

    ZonedDateTime LocalDate::atStartOfDay(ZoneId const& zone) const {
      try {
        LocalDateTime localDT = atStartOfDay();
        if (!Class<ZoneOffset>::hasInstance(zone)) {
          ZoneRules rules = zone.getRules();
          auto transition = rules.transition(localDT);
          if (transition.isPresent() && transition.get().isGap()) {
            return ZonedDateTime::of(transition.get().dateTimeAfter(), zone);
          }
        }
        return ZonedDateTime::of(localDT, zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong LocalDate::toEpochDay() const {
      glong y = year_;
      glong m = month_;
      glong total = 0;
      total += 365 * y;
      if (y >= 0) {
        total += (y + 3) / 4 - (y + 99) / 100 + (y + 399) / 400;
      } else {
        total -= y / -4 - y / -100 + y / -400;
      }
      total += (367 * m - 362) / 12;
      total += dayOfMonth() - 1;
      if (m > 2) {
        total--;
        if (!isLeapYear()) {
          total--;
        }
      }
      return total - DAYS_0000_TO_1970;
    }

    glong LocalDate::toEpochSecond(LocalTime const& time, ZoneOffset const& offset) const {
      glong secs = toEpochDay() * SECONDS_PER_DAY + time.toSecondOfDay();
      secs -= offset.totalSeconds();
      return secs;
    }

    gint LocalDate::compareTo(const LocalDate& other) const {
      gint result = year_ - other.year_;
      if (result == 0) {
        result = month_ - other.month_;
        if (result == 0) {
          result = day_ - other.day_;
        }
      }
      return result;
    }

    gbool LocalDate::isAfter(LocalDate const& other) const {
      return compareTo(other) > 0;
    }

    gbool LocalDate::isBefore(LocalDate const& other) const {
      return compareTo(other) < 0;
    }

    gbool LocalDate::equals(const Object& obj) const {
      return this == &obj ||
          (Class<LocalDate>::hasInstance(obj) && compareTo(CORE_XCAST(LocalDate const, obj)) == 0);
    }

    gint LocalDate::hash() const {
      gint y = year_;
      gint m = month_;
      gint d = day_;
      return ((y & 0xFFFFF800) ^ (y << 11)) | (m << 6) | d;
    }

    String LocalDate::toString() const {
      gint y = year_;
      gint m = month_;
      gint d = day_;
      gint absYear = Math::abs(y);
      XString str = XString(10);
      if (absYear < 1000) {
        if (y < 0) {
          str.append(y - 10000).deleteCharAt(1);
        } else {
          str.append(y + 10000).deleteCharAt(0);
        }
      } else {
        if (y > 9999) {
          str.append(u'+');
        }
        str.append(y);
      }
      return str.append(m < 10 ? "-0"_Sl : "-"_Sl).append(m)
                .append(d < 10 ? "-0"_Sl : "-"_Sl).append(d)
                .toString();
    }

    Object& LocalDate::clone() const {
      return UNSAFE::newInstance<LocalDate>(*this);
    }

    LocalDate LocalDate::create(gint year, gint month, gint dayOfMonth) {
      if (dayOfMonth > 28) {
        gint dom = 0;
        switch (month) {
          case 2:
            dom = (TemporalUtils::isLeapYear(year) ? 29 : 28);
            break;
          case 4:
          case 6:
          case 9:
          case 11: dom = 30;
            break;
          default: dom = 31;
            break;
        }
        if (dayOfMonth > dom) {
          if (dayOfMonth == 29) {
            DateTimeException("Invalid date 'February 29' as '"_Sl
              + year + "' is not a leap year").throws($ftrace());
          } else {
            DateTimeException("Invalid date '"_Sl + (Month) month
              + " " + dayOfMonth + "'").throws($ftrace());
          }
        }
      }
      return LocalDate(year, month, dayOfMonth);
    }

    LocalDate LocalDate::resolve(gint year, gint month, gint day) {
      switch (month) {
        case 2:
          day = Math::min(day, TemporalUtils::isLeapYear(year) ? 29 : 28);
          break;
        case 4:
        case 6:
        case 9:
        case 11:
          day = Math::min(day, 30);
        default:
          break;
      }
      return LocalDate(year, month, day);
    }

    gint LocalDate::prolepticMonth() const {
      return (year_ * 12L + month_ - 1);
    }

    glong LocalDate::daysUntil(LocalDate const& end) const {
      return end.toEpochDay() - toEpochDay(); // no overflow
    }

    glong LocalDate::monthsUntil(LocalDate const& end) const {
      glong packed1 = prolepticMonth() * 32L + dayOfMonth(); // no overflow
      glong packed2 = end.prolepticMonth() * 32L + end.dayOfMonth(); // no overflow
      return (packed2 - packed1) / 32;
    }
  } // util
} // core
