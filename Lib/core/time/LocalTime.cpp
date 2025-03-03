//
// Created by brunshweeck on 3 août 2024.
//

#include "LocalTime.h"

#include <core/lang/Enum.h>
#include <core/lang/XString.h>
#include <core/time/DateTimeException.h>
#include <core/time/Duration.h>
#include <core/time/Instant.h>
#include <core/time/LocalDateTime.h>
#include <core/time/OffsetTime.h>
#include <core/time/TemporalAmount.h>
#include <core/time/TemporalException.h>
#include <core/time/TemporalQuery.h>
#include <core/time/TemporalUnit.h>
#include <core/time/ValueRange.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneRules.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  using namespace util;

  namespace time {
    CORE_ALIAS(Fields, TemporalUtils::Fields);
    CORE_ALIAS(Units, TemporalUtils::Units);

    LocalTime const LocalTime::MIN = LocalTime::of(0, 0);

    LocalTime const LocalTime::MAX = LocalTime::of(23, 59, 59, 999999999);

    LocalTime const LocalTime::MIDNIGHT = LocalTime::of(0, 0);

    LocalTime const LocalTime::NOON = LocalTime::of(12, 0);

    LocalTime::LocalTime(gint hour, gint minute, gint second, gint nanoOfSecond)
      : hour_(hour), minute_(minute), second_(second), nano_(nanoOfSecond) {}

    LocalTime LocalTime::now() {
      return ofInstant(Instant::now(), ZoneOffset::UTC);
    }

    LocalTime LocalTime::now(ZoneId const& zone) {
      return ofInstant(Instant::now(), zone);
    }

    LocalTime LocalTime::of(gint hour, gint minute) {
      try {
        Fields::checkValue(hour, TemporalField::HOUR_OF_DAY);
        Fields::checkValue(minute, TemporalField::MINUTE_OF_HOUR);
        return create(hour, minute, 0, 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::of(gint hour, gint minute, gint second) {
      try {
        Fields::checkValue(hour, TemporalField::HOUR_OF_DAY);
        Fields::checkValue(minute, TemporalField::MINUTE_OF_HOUR);
        Fields::checkValue(second, TemporalField::SECOND_OF_MINUTE);
        return create(hour, minute, second, 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::of(gint hour, gint minute, gint second, gint nanoOfSecond) {
      try {
        Fields::checkValue(hour, TemporalField::HOUR_OF_DAY);
        Fields::checkValue(minute, TemporalField::MINUTE_OF_HOUR);
        Fields::checkValue(second, TemporalField::SECOND_OF_MINUTE);
        Fields::checkValue(nanoOfSecond, TemporalField::NANO_OF_SECOND);
        return create(hour, minute, second, nanoOfSecond);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::ofInstant(Instant const& instant, ZoneId const& zone) {
      ZoneOffset offset = zone.getRules().offset(instant);
      glong localSecond = instant.epochSecond() + offset.totalSeconds();
      gint secsOfDay = Math::floorMod(localSecond, SECONDS_PER_DAY);
      return ofNanoOfDay(secsOfDay * NANOS_PER_SECOND + instant.nano());
    }

    LocalTime LocalTime::ofSecondOfDay(glong secondOfDay) {
      try {
        Fields::checkValue(secondOfDay, TemporalField::SECOND_OF_DAY);
        gint hours = (gint) (secondOfDay / SECONDS_PER_HOUR);
        secondOfDay -= hours * SECONDS_PER_HOUR;
        gint minutes = (gint) (secondOfDay / SECONDS_PER_MINUTE);
        secondOfDay -= minutes * SECONDS_PER_MINUTE;
        return create(hours, minutes, (gint) secondOfDay, 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::ofNanoOfDay(glong nanoOfDay) {
      try {
        Fields::checkValue(nanoOfDay, TemporalField::NANO_OF_DAY);
        gint hours = (gint) (nanoOfDay / NANOS_PER_HOUR);
        nanoOfDay -= hours * NANOS_PER_HOUR;
        gint minutes = (gint) (nanoOfDay / NANOS_PER_MINUTE);
        nanoOfDay -= minutes * NANOS_PER_MINUTE;
        gint seconds = (gint) (nanoOfDay / NANOS_PER_SECOND);
        nanoOfDay -= seconds * NANOS_PER_SECOND;
        return create(hours, minutes, seconds, (gint) nanoOfDay);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::from(TemporalAccessor const& temporal) {
      OptionalTime time = temporal.query(TemporalQuery::localTime()).tryCast<LocalTime>();
      if (time.isEmpty()) {
        DateTimeException("Unable to obtain LocalTime from TemporalAccessor: "_Sl +
          temporal + " of type " + typeName(temporal)).throws($ftrace());
      }
      return time.get();
    }

    LocalTime LocalTime::parse(CharSequence const& text) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    LocalTime LocalTime::parse(CharSequence const& text, DateTimeFormatter const& formatter) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool LocalTime::isSupported(TemporalField field) const {
      return Fields::isTimeBased(field);
    }

    gbool LocalTime::isSupported(TemporalUnit unit) const {
      return Units::isTimeBased(unit);
    }

    ValueRange LocalTime::range(TemporalField field) const {
      if (isSupported(field))
        TemporalException("Unsupported field: "_Sl + field).throws($ftrace());
      return Fields::range(field);
    }

    gint LocalTime::get(TemporalField field) const {
      glong result = 0;
      try {
        result = getLong(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      if (result > Integer::MAX_VALUE || result < Integer::MIN_VALUE) {
        TemporalException("Value of field '"_Sl + field + "' out of range."_Sl).throws($ftrace());
      }
      return (gint) result;
    }

    glong LocalTime::getLong(TemporalField field) const {
      switch (field) {
        case TemporalField::NANO_OF_SECOND: return nano_;
        case TemporalField::NANO_OF_DAY: return toNanoOfDay();
        case TemporalField::MICRO_OF_SECOND: return nano_ / 1000;
        case TemporalField::MICRO_OF_DAY: return toNanoOfDay() / 1000;
        case TemporalField::MILLI_OF_SECOND: return nano_ / NANOS_PER_MILLI;
        case TemporalField::MILLI_OF_DAY: return toNanoOfDay() / NANOS_PER_MILLI;
        case TemporalField::SECOND_OF_MINUTE: return second_;
        case TemporalField::SECOND_OF_DAY: return toSecondOfDay();
        case TemporalField::MINUTE_OF_HOUR: return minute_;
        case TemporalField::MINUTE_OF_DAY: return hour_ * 60 + minute_;
        case TemporalField::HOUR_OF_AMPM: return hour_ % 12;
        case TemporalField::CLOCK_HOUR_OF_AMPM: return (hour_ % 12 == 0) ? 12 : hour_ % 12;
        case TemporalField::HOUR_OF_DAY: return hour_;
        case TemporalField::CLOCK_HOUR_OF_DAY: return (hour_ == 0) ? 24 : hour_;
        case TemporalField::AMPM_OF_DAY: return (hour_ < 12) ? 0 : 1;
        default:
          TemporalException("Unsupported field: " + field).throws($ftrace());
      }
    }

    gint LocalTime::hour() const {
      return hour_;
    }

    gint LocalTime::minute() const {
      return minute_;
    }

    gint LocalTime::second() const {
      return second_;
    }

    gint LocalTime::nano() const {
      return nano_;
    }

    LocalTime LocalTime::with(TemporalAdjuster const& adjuster) const {
      if (Class<LocalTime>::hasInstance(adjuster))
        return CORE_XCAST(LocalTime const, adjuster);
      try {
        Temporal& t = adjuster.adjustInto(*this);
        LocalTime time = CORE_XCAST(LocalTime, t);
        if (this != &t)
          UNSAFE::deleteRegInstance(t);
        return time;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::with(TemporalField field, glong newValue) const {
      try {
        Fields::checkValue(newValue, field);
        switch (field) {
          case TemporalField::NANO_OF_SECOND: return withNano((gint) newValue);
          case TemporalField::NANO_OF_DAY: return LocalTime::ofNanoOfDay(newValue);
          case TemporalField::MICRO_OF_SECOND: return withNano((gint) newValue * 1000);
          case TemporalField::MICRO_OF_DAY: return LocalTime::ofNanoOfDay(newValue * 1000);
          case TemporalField::MILLI_OF_SECOND: return withNano((gint) newValue * NANOS_PER_MILLI);
          case TemporalField::MILLI_OF_DAY: return LocalTime::ofNanoOfDay(newValue * NANOS_PER_MILLI);
          case TemporalField::SECOND_OF_MINUTE: return withSecond((gint) newValue);
          case TemporalField::SECOND_OF_DAY: return LocalTime::ofSecondOfDay(newValue);
          case TemporalField::MINUTE_OF_HOUR: return withMinute((gint) newValue);
          case TemporalField::MINUTE_OF_DAY: return LocalTime::ofSecondOfDay(newValue * SECONDS_PER_MINUTE);
          case TemporalField::HOUR_OF_AMPM: return plusHours(newValue - (hour_ % 12));
          case TemporalField::CLOCK_HOUR_OF_AMPM: return plusHours((newValue % 12) - (hour_ % 12));
          case TemporalField::HOUR_OF_DAY: return withHour((gint) newValue);
          case TemporalField::CLOCK_HOUR_OF_DAY: return withHour((gint) newValue % 24);
          case TemporalField::AMPM_OF_DAY: return plusHours((newValue - (hour_ / 12)) * 12);
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported field: " + field).throws($ftrace());
    }

    LocalTime LocalTime::withHour(gint hour) const {
      try {
        Fields::checkValue(hour, TemporalField::HOUR_OF_DAY);
        return create(hour, minute_, second_, nano_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::withMinute(gint minute) const {
      try {
        Fields::checkValue(minute, TemporalField::MINUTE_OF_HOUR);
        return create(hour_, minute, second_, nano_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::withSecond(gint second) const {
      try {
        Fields::checkValue(second, TemporalField::SECOND_OF_MINUTE);
        return create(hour_, minute_, second, nano_);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::withNano(gint nanoOfSecond) const {
      try {
        Fields::checkValue(nanoOfSecond, TemporalField::NANO_OF_SECOND);
        return create(hour_, minute_, second_, nanoOfSecond);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::truncatedTo(TemporalUnit unit) const {
      if (unit == TemporalUnit::NANOS)
        return *this;
      Duration duration = Units::getDuration(unit);
      if (duration.seconds() > SECONDS_PER_DAY)
        TemporalException("Unit is too large for truncation"_Sl).throws($ftrace());
      gint nanos = duration.toNanos();
      if ((nanos % NANOS_PER_DAY) != 0)
        TemporalException("Unit must divide into a standard day without remainder"_Sl).throws($ftrace());
      glong nanoOfDay = toNanoOfDay();
      return LocalTime::ofNanoOfDay((nanoOfDay / nanos) * nanos);
    }

    LocalTime LocalTime::plus(TemporalAmount const& amountToAdd) const {
      try {
        auto& temporal = amountToAdd.addTo(*this);
        LocalTime time = CORE_XCAST(LocalTime, temporal);
        UNSAFE::deleteRegInstance(temporal);
        return time;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::plus(glong amountToAdd, TemporalUnit unit) const {
      switch (unit) {
        case TemporalUnit::NANOS: return plusNanos(amountToAdd);
        case TemporalUnit::MICROS: return plusNanos((amountToAdd % MICROS_PER_DAY) * 1000);
        case TemporalUnit::MILLIS: return plusNanos((amountToAdd % MILLIS_PER_DAY) * NANOS_PER_MILLI);
        case TemporalUnit::SECONDS: return plusSeconds(amountToAdd);
        case TemporalUnit::MINUTES: return plusMinutes(amountToAdd);
        case TemporalUnit::HOURS: return plusHours(amountToAdd);
        case TemporalUnit::HALF_DAYS: return plusHours((amountToAdd % 2) * 12);
        default:
          TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
      }
    }

    LocalTime LocalTime::plusHours(glong hoursToAdd) const {
      hoursToAdd %= HOURS_PER_DAY;
      gint newHour = (hour_ + hoursToAdd) % HOURS_PER_DAY;
      return create(newHour, minute_, second_, nano_);
    }

    LocalTime LocalTime::plusMinutes(glong minutesToAdd) const {
      minutesToAdd %= MINUTES_PER_DAY;
      gint minutes = (hour_ * MINUTES_PER_HOUR + minute_ + minutesToAdd) % MINUTES_PER_DAY;
      gint newHour = Math::floorDiv(minutes, MINUTES_PER_HOUR);
      gint newMinute = Math::floorMod(minutes, MINUTES_PER_HOUR);
      return create(newHour, newMinute, second_, nano_);
    }

    LocalTime LocalTime::plusSeconds(glong secondsToAdd) const {
      secondsToAdd %= SECONDS_PER_DAY;
      gint seconds = (toSecondOfDay() + secondsToAdd) % SECONDS_PER_DAY;
      gint newHour = Math::floorDiv(seconds, SECONDS_PER_HOUR);
      gint newMinute = Math::floorDiv(seconds = Math::floorMod(seconds, SECONDS_PER_HOUR), SECONDS_PER_MINUTE);
      gint newSecond = Math::floorMod(seconds, SECONDS_PER_MINUTE);
      return create(newHour, newMinute, newSecond, nano_);
    }

    LocalTime LocalTime::plusNanos(glong nanosToAdd) const {
      nanosToAdd %= NANOS_PER_DAY;
      glong nanos = (toNanoOfDay() + nanosToAdd) % NANOS_PER_DAY;
      gint newHour = (gint) Math::floorDiv(nanos, NANOS_PER_HOUR);
      gint newMinute = (gint) Math::floorDiv(nanos = Math::floorMod(nanos, NANOS_PER_HOUR), NANOS_PER_MINUTE);
      gint newSecond = (gint) Math::floorDiv(nanos = Math::floorMod(nanos, NANOS_PER_MINUTE), NANOS_PER_SECOND);
      gint newNano = (gint) Math::floorMod(nanos, NANOS_PER_SECOND);
      return create(newHour, newMinute, newSecond, newNano);
    }

    LocalTime LocalTime::minus(TemporalAmount const& amountToSubtract) const {
      try {
        auto& temporal = amountToSubtract.subtractFrom(*this);
        LocalTime time = CORE_XCAST(LocalTime, temporal);
        UNSAFE::deleteRegInstance(temporal);
        return time;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTime::minus(glong amountToSubtract, TemporalUnit unit) const {
      return (amountToSubtract == Long::MIN_VALUE)
               ? plus(Long::MAX_VALUE, unit).plus(1, unit)
               : plus(-amountToSubtract, unit);
    }

    LocalTime LocalTime::minusHours(glong hoursToSubtract) const {
      return (hoursToSubtract == Long::MIN_VALUE)
               ? plusHours(Long::MAX_VALUE).plusHours(1)
               : plusHours(-hoursToSubtract);
    }

    LocalTime LocalTime::minusMinutes(glong minutesToSubtract) const {
      return (minutesToSubtract == Long::MIN_VALUE)
               ? plusMinutes(Long::MAX_VALUE).plusMinutes(1)
               : plusMinutes(-minutesToSubtract);
    }

    LocalTime LocalTime::minusSeconds(glong secondsToSubtract) const {
      return (secondsToSubtract == Long::MIN_VALUE)
               ? plusSeconds(Long::MAX_VALUE).plusSeconds(1)
               : plusSeconds(-secondsToSubtract);
    }

    LocalTime LocalTime::minusNanos(glong nanosToSubtract) const {
      return (nanosToSubtract == Long::MIN_VALUE)
               ? plusNanos(Long::MAX_VALUE).plusNanos(1)
               : plusNanos(-nanosToSubtract);
    }

    TemporalAccessor::Optional LocalTime::query(TemporalQuery const& query) const {
      if (query == TemporalQuery::localTime())
        return *this;
      if (query == TemporalQuery::precision())
        return (Enum<TemporalUnit>) TemporalUnit::NANOS;
      if (query == TemporalQuery::localDate() ||
        query == TemporalQuery::zone() ||
        query == TemporalQuery::zoneId() ||
        query == TemporalQuery::offset())
        return Optional::empty();
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& LocalTime::adjustInto(Temporal const& temporal) const {
      try {
        return adjustFieldTo(TemporalField::NANO_OF_DAY, toNanoOfDay(), temporal);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong LocalTime::until(const Temporal& endExclusive, TemporalUnit unit) const {
      try {
        LocalTime end = LocalTime::from(endExclusive);
        glong nanosUntil = end.toNanoOfDay() - toNanoOfDay();
        switch (unit) {
          case TemporalUnit::NANOS: return nanosUntil;
          case TemporalUnit::MICROS: return nanosUntil / 1000;
          case TemporalUnit::MILLIS: return nanosUntil / NANOS_PER_MILLI;
          case TemporalUnit::SECONDS: return nanosUntil / NANOS_PER_SECOND;
          case TemporalUnit::MINUTES: return nanosUntil / NANOS_PER_MINUTE;
          case TemporalUnit::HOURS: return nanosUntil / NANOS_PER_HOUR;
          case TemporalUnit::HALF_DAYS: return nanosUntil / (NANOS_PER_HOUR * 12L);
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit: "_Sl + unit).throws($ftrace());
    }

    String LocalTime::format(DateTimeFormatter const& formatter) const {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    LocalDateTime LocalTime::atDate(LocalDate const& date) const {
      return LocalDateTime::of(date, *this);
    }

    OffsetTime LocalTime::atOffset(ZoneOffset const& offset) const {
      return OffsetTime::of(*this, offset);
    }

    gint LocalTime::toSecondOfDay() const {
      return hour_ * SECONDS_PER_HOUR
          + minute_ * SECONDS_PER_MINUTE
          + second_;
    }

    glong LocalTime::toNanoOfDay() const {
      return hour_ * NANOS_PER_HOUR
          + minute_ * NANOS_PER_MINUTE
          + second_ * NANOS_PER_SECOND
          + nano_;
    }

    glong LocalTime::toEpochSecond(LocalDate const& date, ZoneOffset const& offset) const {
      return date.toEpochDay() * SECONDS_PER_DAY
          + (toSecondOfDay() - offset.totalSeconds());
    }

    gint LocalTime::compareTo(const LocalTime& other) const {
      gint result = hour_ - other.hour_;
      if (result == 0) {
        result = minute_ - other.minute_;
        if (result == 0) {
          result = second_ - other.second_;
          if (result == 0) {
            result = nano_ - other.nano_;
          }
        }
      }
      return result;
    }

    gbool LocalTime::isAfter(LocalTime const& other) const {
      return compareTo(other) > 0;
    }

    gbool LocalTime::isBefore(LocalTime const& other) const {
      return compareTo(other) < 0;
    }

    gbool LocalTime::equals(const Object& other) const {
      return this == &other ||
          (Class<LocalTime>::hasInstance(other) && compareTo(CORE_XCAST(LocalTime const, other)) == 0);
    }

    gint LocalTime::hash() const {
      return Long::hash(toNanoOfDay());
    }

    String LocalTime::toString() const {
      gint hour = LocalTime::hour();
      gint minute = LocalTime::minute();
      gint second = LocalTime::second();
      gint nano = LocalTime::nano();
      XString str = XString(18);
      str.append(hour < 10 ? "0"_Sl : ""_Sl).append(hour)
         .append(minute < 10 ? ":0"_Sl : ":"_Sl).append(minute);
      if (second > 0 || nano > 0) {
        str.append(second < 10 ? ":0"_Sl : ":"_Sl).append(second);
        if (nano > 0) {
          str.append('.');
          if (nano % 1000000 == 0)
            str.append(Integer::toString(nano / 1000000 + 1000).subString(1));
          else if (nano % 1000 == 0)
            str.append(Integer::toString(nano / 1000 + 1000000).subString(1));
          else
            str.append(Integer::toString(nano + 1000000000).subString(1));
        }
      }
      return str.toString();
    }

    Object& LocalTime::clone() const {
      return UNSAFE::newInstance<LocalTime>(*this);
    }

    LocalTime LocalTime::create(gint hour, gint minute, gint second, gint nanoOfSecond) {
      try {
        Fields::checkValue(hour, TemporalField::HOUR_OF_DAY);
        Fields::checkValue(minute, TemporalField::MINUTE_OF_HOUR);
        Fields::checkValue(second, TemporalField::SECOND_OF_MINUTE);
        Fields::checkValue(nanoOfSecond, TemporalField::NANO_OF_SECOND);
        return LocalTime(hour, minute, second, nanoOfSecond);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
  } // time
} // core
