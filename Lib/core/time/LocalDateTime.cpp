//
// Created by brunshweeck on 3 août 2024.
//

#include "LocalDateTime.h"

#include <core/lang/Enum.h>
#include <core/misc/Unsafe.h>
#include <core/time/DateTimeException.h>
#include <core/time/Instant.h>
#include <core/time/Month.h>
#include <core/time/OffsetDateTime.h>
#include <core/time/TemporalAmount.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ValueRange.h>
#include <core/time/ZonedDateTime.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneRules.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  using namespace util;

  namespace time {
    CORE_ALIAS(Fields, TemporalUtils::Fields);
    CORE_ALIAS(Units, TemporalUtils::Units);

    LocalDateTime const LocalDateTime::MIN =
        LocalDateTime::of(TemporalUtils::MIN_YEAR, Month::JANUARY, 1, 0, 0);
    LocalDateTime const LocalDateTime::MAX =
        LocalDateTime::of(TemporalUtils::MAX_YEAR, Month::DECEMBER, 31, 23, 59, 59, 999999999);

    LocalDateTime::LocalDateTime(LocalDate const& date, LocalTime const& time): date(date), time(time) {}

    LocalDateTime LocalDateTime::now() {
      return now(ZoneId::systemZone());
    }

    LocalDateTime LocalDateTime::now(ZoneId const& zone) {
      Instant now = Instant::now();
      ZoneOffset offset = zone.getRules().offset(now);
      return LocalDateTime::ofEpochSecond(now.epochSecond(), now.nano(), offset);
    }

    LocalDateTime LocalDateTime::of(gint year, Month month, gint dayOfMonth, gint hour, gint minute) {
      try {
        LocalDate date = LocalDate::of(year, month, dayOfMonth);
        LocalTime time = LocalTime::of(hour, minute);
        return LocalDateTime(date, time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::of(gint year, Month month, gint dayOfMonth, gint hour, gint minute, gint second) {
      try {
        LocalDate date = LocalDate::of(year, month, dayOfMonth);
        LocalTime time = LocalTime::of(hour, minute, second);
        return LocalDateTime(date, time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::of(gint year, Month month, gint dayOfMonth,
                                    gint hour, gint minute, gint second, gint nanoOfSecond) {
      try {
        LocalDate date = LocalDate::of(year, month, dayOfMonth);
        LocalTime time = LocalTime::of(hour, minute, second, nanoOfSecond);
        return LocalDateTime(date, time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::of(gint year, gint month, gint dayOfMonth, gint hour, gint minute) {
      try {
        LocalDate date = LocalDate::of(year, month, dayOfMonth);
        LocalTime time = LocalTime::of(hour, minute);
        return LocalDateTime(date, time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::of(gint year, gint month, gint dayOfMonth, gint hour, gint minute, gint second) {
      try {
        LocalDate date = LocalDate::of(year, month, dayOfMonth);
        LocalTime time = LocalTime::of(hour, minute, second);
        return LocalDateTime(date, time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::of(gint year, gint month, gint dayOfMonth, gint hour, gint minute, gint second,
                                    gint nanoOfSecond) {
      try {
        LocalDate date = LocalDate::of(year, month, dayOfMonth);
        LocalTime time = LocalTime::of(hour, minute, second, nanoOfSecond);
        return LocalDateTime(date, time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::of(LocalDate const& date, LocalTime const& time) {
      return LocalDateTime(date, time);
    }

    LocalDateTime LocalDateTime::ofInstant(Instant const& instant, ZoneId const& zone) {
      try {
        ZoneRules rules = zone.getRules();
        ZoneOffset offset = rules.offset(instant);
        return ofEpochSecond(instant.epochSecond(), instant.nano(), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::ofEpochSecond(glong epochSecond, gint nanoOfSecond, ZoneOffset const& offset) {
      try {
        Fields::checkValue(nanoOfSecond, TemporalField::NANO_OF_SECOND);
        glong localSecond = epochSecond + offset.totalSeconds(); // overflow caught later
        glong localEpochDay = Math::floorDiv(localSecond, SECONDS_PER_DAY);
        int secsOfDay = Math::floorMod(localSecond, SECONDS_PER_DAY);
        LocalDate date = LocalDate::ofEpochDay(localEpochDay);
        LocalTime time = LocalTime::ofNanoOfDay(secsOfDay * NANOS_PER_SECOND + nanoOfSecond);
        return LocalDateTime(date, time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::from(TemporalAccessor const& temporal) {
      if (Class<LocalDateTime>::hasInstance(temporal))
        return CORE_XCAST(LocalDateTime const, temporal);
      if (Class<ZonedDateTime>::hasInstance(temporal))
        return CORE_XCAST(ZonedDateTime const, temporal).toLocalDateTime();
      if (Class<OffsetDateTime>::hasInstance(temporal))
        return CORE_XCAST(OffsetDateTime const, temporal).toLocalDateTime();
      try {
        LocalDate date = LocalDate::from(temporal);
        LocalTime time = LocalTime::from(temporal);
        return LocalDateTime(date, time);
      } catch (Throwable const& ex) {
        DateTimeException("Unable to obtain LocalDateTime from TemporalAccessor: "_Sl +
                          temporal + " of type "_Sl + typeName(temporal), ex).throws($ftrace());
      }
    }

    LocalDateTime LocalDateTime::parse(CharSequence const& text) {
      // TODO: implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    LocalDateTime LocalDateTime::parse(CharSequence const& text, DateTimeFormatter const& formatter) {
      // TODO: implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool LocalDateTime::isSupported(TemporalField field) const {
      return Fields::isDateBased(field) && Fields::isTimeBased(field);
    }

    gbool LocalDateTime::isSupported(TemporalUnit unit) const {
      return Units::isDateBased(unit) && Units::isTimeBased(unit);
    }

    ValueRange LocalDateTime::range(TemporalField field) const {
      return (Fields::isTimeBased(field)) ? time.range(field) : date.range(field);
    }

    gint LocalDateTime::get(TemporalField field) const {
      try {
        return Fields::isTimeBased(field) ? time.get(field) : date.get(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong LocalDateTime::getLong(TemporalField field) const {
      try {
        return Fields::isTimeBased(field) ? time.getLong(field) : date.getLong(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDateTime::toLocalDate() const {
      return date;
    }

    gint LocalDateTime::year() const {
      return date.year();
    }

    Month LocalDateTime::month() const {
      return date.month();
    }

    gint LocalDateTime::dayOfMonth() const {
      return date.dayOfMonth();
    }

    gint LocalDateTime::dayOfYear() const {
      return date.dayOfYear();
    }

    DayOfWeek LocalDateTime::dayOfWeek() const {
      return date.dayOfWeek();
    }

    LocalTime LocalDateTime::toLocalTime() const {
      return time;
    }

    gint LocalDateTime::hour() const {
      return time.hour();
    }

    gint LocalDateTime::minute() const {
      return time.minute();
    }

    gint LocalDateTime::second() const {
      return time.second();
    }

    gint LocalDateTime::nano() const {
      return time.nano();
    }

    LocalDateTime LocalDateTime::with(TemporalAdjuster const& adjuster) const {
      if (Class<LocalDate>::hasInstance(adjuster))
        return with(CORE_XCAST(LocalDate const, adjuster), time);
      if (Class<LocalTime>::hasInstance(adjuster))
        return with(date, CORE_XCAST(LocalTime const, adjuster));
      if (Class<LocalDateTime>::hasInstance(adjuster))
        return CORE_XCAST(LocalDateTime const, adjuster);
      try {
        Temporal& t = adjuster.adjustInto(*this);
        LocalDateTime ldt = CORE_XCAST(LocalDateTime, t);
        if (&t != this)
          UNSAFE::deleteRegInstance(t);
        return ldt;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::with(TemporalField field, glong newValue) const {
      try {
        return Fields::isTimeBased(field)
                 ? with(date, time.with(field, newValue))
                 : with(date.with(field, newValue), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::withYear(gint year) const {
      try {
        return with(date.withYear(year), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::withMonth(gint month) const {
      try {
        return with(date.withMonth(month), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::withDayOfMonth(gint dayOfMonth) const {
      try {
        return with(date.withDayOfMonth(dayOfMonth), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::withDayOfYear(gint dayOfYear) const {
      try {
        return with(date.withDayOfYear(dayOfYear), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::withHour(gint hour) const {
      try {
        return with(date, time.withHour(hour));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::withMinute(gint minute) const {
      try {
        return with(date, time.withMinute(minute));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::withSecond(gint second) const {
      try {
        return with(date, time.withSecond(second));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::withNano(gint nanoOfSecond) const {
      try {
        return with(date, time.withNano(nanoOfSecond));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::truncatedTo(TemporalUnit unit) const {
      try {
        return with(date, time.truncatedTo(unit));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plus(TemporalAmount const& amountToAdd) const {
      try {
        Temporal& temporal = amountToAdd.addTo(*this);
        LocalDateTime localDT = CORE_XCAST(LocalDateTime, temporal);
        if (this != &temporal)
          UNSAFE::deleteRegInstance(temporal);
        return localDT;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plus(glong amountToAdd, TemporalUnit unit) const {
      try {
        switch (unit) {
          case TemporalUnit::NANOS: return plusNanos(amountToAdd);
          case TemporalUnit::MICROS: return plusDays(amountToAdd / MICROS_PER_DAY)
                .plusNanos((amountToAdd % MICROS_PER_DAY) * 1000);
          case TemporalUnit::MILLIS: return plusDays(amountToAdd / MILLIS_PER_DAY)
                .plusNanos((amountToAdd % MILLIS_PER_DAY) * 1000000);
          case TemporalUnit::SECONDS: return plusSeconds(amountToAdd);
          case TemporalUnit::MINUTES: return plusMinutes(amountToAdd);
          case TemporalUnit::HOURS: return plusHours(amountToAdd);
          case TemporalUnit::HALF_DAYS: return plusDays(amountToAdd / 2).plusHours((amountToAdd % 2) * 12);
          default: return with(date.plus(amountToAdd, unit), time);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plusYears(glong years) const {
      try {
        return with(date.plusYears(years), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plusMonths(glong months) const {
      try {
        return with(date.plusMonths(months), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plusWeeks(glong weeks) const {
      try {
        return with(date.plusWeeks(weeks), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plusDays(glong days) const {
      try {
        return with(date.plusDays(days), time);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plusHours(glong hours) const {
      try {
        return plus(date, hours, 0, 0, 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plusMinutes(glong minutes) const {
      try {
        return plus(date, 0, minutes, 0, 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plusSeconds(glong seconds) const {
      try {
        return plus(date, 0, 0, seconds, 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::plusNanos(glong nanos) const {
      try {
        return plus(date, 0, 0, 0, nanos);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minus(TemporalAmount const& amountToSubtract) const {
      try {
        Temporal& temporal = amountToSubtract.subtractFrom(*this);
        LocalDateTime localDT = CORE_XCAST(LocalDateTime, temporal);
        if (this != &temporal)
          UNSAFE::deleteRegInstance(temporal);
        return localDT;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minus(glong amountToSubtract, TemporalUnit unit) const {
      try {
        return (amountToSubtract == Long::MIN_VALUE)
                 ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                 : plus(-amountToSubtract, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minusYears(glong years) const {
      try {
        return (years == Long::MIN_VALUE)
                 ? plusYears(Long::MAX_VALUE).plusYears(1)
                 : plusYears(-years);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minusMonths(glong months) const {
      try {
        return (months == Long::MIN_VALUE)
                 ? plusMonths(Long::MAX_VALUE).plusMonths(1)
                 : plusMonths(-months);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minusWeeks(glong weeks) const {
      try {
        return (weeks == Long::MIN_VALUE)
                 ? plusWeeks(Long::MAX_VALUE).plusWeeks(1)
                 : plusWeeks(-weeks);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minusDays(glong days) const {
      try {
        return (days == Long::MIN_VALUE)
                 ? plusDays(Long::MAX_VALUE).plusDays(1)
                 : plusDays(-days);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minusHours(glong hours) const {
      try {
        return (hours == Long::MIN_VALUE)
                 ? plusHours(Long::MAX_VALUE).plusHours(1)
                 : plusHours(-hours);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minusMinutes(glong minutes) const {
      try {
        return (minutes == Long::MIN_VALUE)
                 ? plusMinutes(Long::MAX_VALUE).plusMinutes(1)
                 : plusMinutes(-minutes);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minusSeconds(glong seconds) const {
      try {
        return (seconds == Long::MIN_VALUE)
                 ? plusSeconds(Long::MAX_VALUE).plusSeconds(1)
                 : plusSeconds(-seconds);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTime::minusNanos(glong nanos) const {
      try {
        return (nanos == Long::MIN_VALUE)
                 ? plusNanos(Long::MAX_VALUE).plusNanos(1)
                 : plusNanos(-nanos);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    TemporalAccessor::Optional LocalDateTime::query(TemporalQuery const& query) const {
      if (query == TemporalQuery::localDate())
        return toLocalDate();
      if (query == TemporalQuery::localTime())
        return toLocalTime();
      if (query == TemporalQuery::precision())
        return (Enum<TemporalUnit>) TemporalUnit::NANOS;
      if (query == TemporalQuery::zone() ||
        query == TemporalQuery::zoneId() ||
        query == TemporalQuery::offset())
        return Optional::empty();
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& LocalDateTime::adjustInto(Temporal const& temporal) const {
      try {
        Temporal& t1 = adjustFieldTo(TemporalField::EPOCH_DAY, date.toEpochDay(), temporal);
        Temporal& t2 = adjustFieldTo(TemporalField::NANO_OF_DAY, time.toNanoOfDay(), t1);
        if (&t1 != &t2 && &t1 != &temporal)
          UNSAFE::deleteRegInstance(t1);
        return t2;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong LocalDateTime::until(const Temporal& endExclusive, TemporalUnit unit) const {
      try {
        LocalDateTime end = LocalDateTime::from(endExclusive);
        if (Units::isTimeBased(unit)) {
          glong daysUntil = date.daysUntil(end.date);
          if (daysUntil == 0L)
            return time.until(end.time, unit);
          glong timeUntil = end.time.toNanoOfDay() - time.toNanoOfDay();
          daysUntil += timeUntil / NANOS_PER_DAY;
          switch (unit) {
            case TemporalUnit::NANOS:
              daysUntil = Math::multiplyExact(daysUntil, NANOS_PER_DAY);
              break;
            case TemporalUnit::MICROS:
              daysUntil = Math::multiplyExact(daysUntil, MICROS_PER_DAY);
              timeUntil = timeUntil / 1000;
              break;
            case TemporalUnit::MILLIS:
              daysUntil = Math::multiplyExact(daysUntil, MILLIS_PER_DAY);
              timeUntil = timeUntil / NANOS_PER_MILLI;
              break;
            case TemporalUnit::SECONDS:
              daysUntil = Math::multiplyExact(daysUntil, SECONDS_PER_DAY);
              timeUntil = timeUntil / NANOS_PER_SECOND;
              break;
            case TemporalUnit::MINUTES:
              daysUntil = Math::multiplyExact(daysUntil, MINUTES_PER_DAY);
              timeUntil = timeUntil / NANOS_PER_MINUTE;
              break;
            case TemporalUnit::HOURS:
              daysUntil = Math::multiplyExact(daysUntil, HOURS_PER_DAY);
              timeUntil = timeUntil / NANOS_PER_HOUR;
              break;
            case TemporalUnit::HALF_DAYS:
              daysUntil = Math::multiplyExact(daysUntil, 2);
              timeUntil = timeUntil / (NANOS_PER_HOUR * 12);
              break;
            default: break;
          }
          return Math::addExact(daysUntil, timeUntil);
        }
        LocalDate endDate = end.date;
        LocalTime endTime = end.time;
        if (endDate.isAfter(date) && endTime.isBefore(time))
          return date.until(endDate.minusDays(1), unit);
        else if (endDate.isBefore(date) && endTime.isAfter(time))
          return date.until(endDate.plusDays(1), unit);
        else
          return date.until(endDate, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String LocalDateTime::format(DateTimeFormatter const& formatter) const {
      // TODO: implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    OffsetDateTime LocalDateTime::atOffset(ZoneOffset const& offset) const {
      try {
        return OffsetDateTime::of(*this, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime LocalDateTime::atZone(ZoneId const& zone) const {
      try {
        return ZonedDateTime::of(*this, zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Instant LocalDateTime::toInstant(ZoneOffset const& offset) const {
      try {
        return Instant::ofEpochSecond(toEpochSecond(offset), time.nano());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong LocalDateTime::toEpochSecond(ZoneOffset const& offset) const {
      return date.toEpochSecond(time, offset);
    }

    gint LocalDateTime::compareTo(const LocalDateTime& other) const {
      gint result = date.compareTo(other.date);
      if (result == 0)
        result = time.compareTo(other.time);
      return result;
    }

    gbool LocalDateTime::isAfter(LocalDateTime const& other) const {
      return compareTo(other) > 0;
    }

    gbool LocalDateTime::isBefore(LocalDateTime const& other) const {
      return compareTo(other) < 0;
    }

    gbool LocalDateTime::equals(const Object& obj) const {
      return this == &obj ||
          (Class<LocalDateTime>::hasInstance(obj) && compareTo(CORE_XCAST(LocalDateTime const, obj)) == 0);
    }

    gint LocalDateTime::hash() const {
      return date.hash() ^ time.hash();
    }

    String LocalDateTime::toString() const {
      return date + "T"_Sl + time;
    }

    Object& LocalDateTime::clone() const {
      return UNSAFE::newInstance<LocalDateTime>(*this);
    }

    LocalDateTime LocalDateTime::with(LocalDate const& newDate, LocalTime const& newTime) const {
      CORE_IGNORE(this);
      return LocalDateTime(newDate, newTime);
    }

    LocalDateTime LocalDateTime::plus(LocalDate const& newDate,
                                      gint hours, gint minutes, gint seconds, gint nanos) const {
      if ((hours | minutes | seconds | nanos) == 0) {
        return with(newDate, time);
      }
      glong daysToAdd = (nanos / NANOS_PER_DAY) //   max/24*60*60*1B
          + (seconds / SECONDS_PER_DAY) //       max/24*60*60
          + (minutes / MINUTES_PER_DAY) //       max/24*60
          + (hours / HOURS_PER_DAY); //          max/24
      glong nanoOfDay = (nanos % NANOS_PER_DAY)
          + (seconds % SECONDS_PER_DAY) * NANOS_PER_SECOND
          + (minutes % MINUTES_PER_DAY) * NANOS_PER_MINUTE
          + (hours % HOURS_PER_DAY) * NANOS_PER_HOUR;
      nanoOfDay += time.toNanoOfDay();
      daysToAdd += Math::floorDiv(nanoOfDay, NANOS_PER_DAY);
      nanoOfDay = Math::floorMod(nanoOfDay, NANOS_PER_DAY);

      try {
        return with(date.plusDays(daysToAdd), LocalTime::ofNanoOfDay(nanoOfDay));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
  } // time
} // core
