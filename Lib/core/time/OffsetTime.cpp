//
// Created by admin on 07/01/25.
//

#include "OffsetTime.h"

#include <core/lang/Enum.h>
#include <core/time/Duration.h>
#include <core/time/Instant.h>
#include <core/time/OffsetDateTime.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ValueRange.h>
#include <core/time/ZoneRules.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  using namespace util;
  CORE_ALIAS(Fields, time::TemporalUtils::Fields);
  CORE_ALIAS(Units, time::TemporalUtils::Units);

  namespace time {
    OffsetTime const OffsetTime::MIN = LocalTime::MIN.atOffset(ZoneOffset::MAX);
    OffsetTime const OffsetTime::MAX = LocalTime::MAX.atOffset(ZoneOffset::MIN);

    OffsetTime::OffsetTime(LocalTime const& time, ZoneOffset const& offset): time(time), offset(offset) {}

    OffsetTime OffsetTime::now() {
      return now(ZoneId::systemZone());
    }

    OffsetTime OffsetTime::now(ZoneId const& zone) {
      Instant now = Instant::now();
      ZoneOffset offset = zone.getRules().offset(now);
      return ofInstant(now, offset);
    }

    OffsetTime OffsetTime::of(LocalTime const& time, ZoneOffset const& offset) {
      return OffsetTime(time, offset);
    }

    OffsetTime OffsetTime::of(gint hour, gint minute, gint second, gint nanoOfSecond, ZoneOffset const& offset) {
      try {
        LocalTime time = LocalTime::of(hour, minute, second, nanoOfSecond);
        return OffsetTime(time, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::ofInstant(Instant const& instant, ZoneId const& zone) {
      try {
        ZoneRules rules = zone.getRules();
        ZoneOffset offset = rules.offset(instant);
        glong totalSeconds = instant.epochSecond() + offset.totalSeconds();
        glong secondOfDay = Math::floorMod(totalSeconds, SECONDS_PER_DAY);

        LocalTime time = LocalTime::ofNanoOfDay(secondOfDay * NANOS_PER_SECOND + instant.nano());
        return OffsetTime(time, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::from(TemporalAccessor const& temporal) {
      if (Class<OffsetTime>::hasInstance(temporal))
        return CORE_XCAST(OffsetTime const, temporal);
      try {
        LocalTime time = LocalTime::from(temporal);
        ZoneOffset offset = ZoneOffset::from(temporal);
        return OffsetTime(time, offset);
      } catch (Throwable const& ex) {
        DateTimeException("Unable to obtain OffsetTime from Temporal: " +
                          temporal + " of type " + typeName(temporal), ex).throws($ftrace());
      }
    }

    OffsetTime OffsetTime::parse(CharSequence const& text) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    OffsetTime OffsetTime::parse(CharSequence const& text, DateTimeFormatter const& formatter) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool OffsetTime::isSupported(TemporalField field) const {
      return time.isSupported(field) || offset.isSupported(field) || Fields::isSupportedBy(*this, field);
    }

    gbool OffsetTime::isSupported(TemporalUnit unit) const {
      return time.isSupported(unit) || Units::isSupportedBy(*this, unit);
    }

    ValueRange OffsetTime::range(TemporalField field) const {
      try {
        if (field == TemporalField::OFFSET_SECONDS)
          return Fields::range(*this, field);
        return time.range(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint OffsetTime::get(TemporalField field) const {
      try {
        return Temporal::get(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong OffsetTime::getLong(TemporalField field) const {
      try {
        if (field == TemporalField::OFFSET_SECONDS)
          return offset.totalSeconds();
        if (time.isSupported(field))
          return time.getLong(field);
        return Fields::getFrom(*this, field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffset OffsetTime::toOffset() const {
      return offset;
    }

    OffsetTime OffsetTime::withOffsetSameLocal(ZoneOffset const& offset) const {
      return OffsetTime(time, offset);
    }

    OffsetTime OffsetTime::withOffsetSameInstant(ZoneOffset const& offset) const {
      try {
        glong secondsUntil = offset.totalSeconds() - toOffset().totalSeconds();
        LocalTime t = time.plusSeconds(secondsUntil);
        return OffsetTime(t, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime OffsetTime::toLocalTime() const {
      return time;
    }

    gint OffsetTime::hour() const {
      return time.hash();
    }

    gint OffsetTime::minute() const {
      return time.minute();
    }

    gint OffsetTime::second() const {
      return time.second();
    }

    gint OffsetTime::nano() const {
      return time.nano();
    }

    OffsetTime OffsetTime::with(TemporalAdjuster const& adjuster) const {
      if (Class<LocalTime>::hasInstance(adjuster))
        return with(CORE_XCAST(LocalTime const, adjuster), offset);
      if (Class<ZoneOffset>::hasInstance(adjuster))
        return with(time, CORE_XCAST(ZoneOffset const, adjuster));
      if (Class<OffsetTime>::hasInstance(adjuster))
        return CORE_XCAST(OffsetTime const, adjuster);
      try {
        return CORE_XCAST(OffsetTime, adjuster.adjustInto(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::with(TemporalField field, glong newValue) const {
      try {
        if (field == TemporalField::OFFSET_SECONDS) {
          Fields::checkValue(newValue, field);
          return with(time, ZoneOffset::ofTotalSeconds(newValue));
        }
        return with(time.with(field, newValue), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::withHour(gint hour) const {
      try {
        return with(time.withHour(hour), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::withMinute(gint minute) const {
      try {
        return with(time.withMinute(minute), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::withSecond(gint second) const {
      try {
        return with(time.withSecond(second), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::withNano(gint nanoOfSecond) const {
      try {
        return with(time.withNano(nanoOfSecond), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::truncatedTo(TemporalUnit unit) const {
      try {
        return with(time.truncatedTo(unit), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::plus(TemporalAmount const& amountToAdd) const {
      try {
        return CORE_XCAST(OffsetTime, amountToAdd.addTo(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::plus(glong amountToAdd, TemporalUnit unit) const {
      try {
        return with(time.plus(amountToAdd, unit), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::plusHours(glong hours) const {
      try {
        return with(time.plusHours(hours), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::plusMinutes(glong minutes) const {
      try {
        return with(time.plusMinutes(minutes), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::plusSeconds(glong seconds) const {
      try {
        return with(time.plusSeconds(seconds), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::plusNanos(glong nanos) const {
      try {
        return with(time.plusNanos(nanos), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::minus(TemporalAmount const& amountToSubtract) const {
      try {
        return CORE_XCAST(OffsetTime, amountToSubtract.subtractFrom(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::minus(glong amountToSubtract, TemporalUnit unit) const {
      try {
        return with(time.minus(amountToSubtract, unit), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::minusHours(glong hours) const {
      try {
        return with(time.minusHours(hours), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::minusMinutes(glong minutes) const {
      try {
        return with(time.minusMinutes(minutes), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::minusSeconds(glong seconds) const {
      try {
        return with(time.minusSeconds(seconds), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetTime::minusNanos(gint nanos) const {
      try {
        return with(time.minusNanos(nanos), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    TemporalAdjuster::Optional OffsetTime::query(TemporalQuery const& query) const {
      if (query == TemporalQuery::offset() || query == TemporalQuery::zone())
        return toOffset();
      if (query == TemporalQuery::localTime())
        return toLocalTime();
      if (query == TemporalQuery::precision())
        return (Enum<TemporalUnit>) TemporalUnit::NANOS;
      if (query == TemporalQuery::zoneId() ||
        query == TemporalQuery::chronology() ||
        query == TemporalQuery::localDate())
        return toOffset();

      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& OffsetTime::adjustInto(Temporal const& temporal) const {
      try {
        return adjustFieldTo(
          TemporalField::NANO_OF_DAY, time.toNanoOfDay(),
          adjustFieldTo(
            TemporalField::OFFSET_SECONDS, toOffset().totalSeconds(), temporal));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong OffsetTime::until(Temporal const& endExclusive, TemporalUnit unit) const {
      try {
        OffsetTime end = OffsetTime::from(endExclusive);
        glong nanosUntil = end.toEpochNano() - toEpochNano();
        switch (unit) {
          case TemporalUnit::NANOS: return nanosUntil;
          case TemporalUnit::MICROS: return nanosUntil / 1000;
          case TemporalUnit::MILLIS: return nanosUntil / NANOS_PER_MILLI;
          case TemporalUnit::SECONDS: return nanosUntil / NANOS_PER_SECOND;
          case TemporalUnit::MINUTES: return nanosUntil / NANOS_PER_MINUTE;
          case TemporalUnit::HOURS: return nanosUntil / NANOS_PER_HOUR;
          case TemporalUnit::HALF_DAYS: return nanosUntil / (12L * NANOS_PER_HOUR);
          default: break;
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      TemporalException("Unsupported unit "_Sl + unit).throws($ftrace());
    }

    String OffsetTime::format(DateTimeFormatter const& formatter) const {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    OffsetDateTime OffsetTime::atDate(LocalDate const& date) const {
      try {
        return OffsetDateTime::of(date, time, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong OffsetTime::toEpochSecond(LocalDate const& date) const {
      try {
        glong epochDay = date.toEpochDay();
        glong secs = epochDay * 86400L + time.toSecondOfDay();
        secs -= offset.totalSeconds();
        return secs;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint OffsetTime::compareTo(OffsetTime const& other) const {
      if (offset.equals(other.offset)) {
        return time.compareTo(other.time);
      }
      gint compare = Long::compare(toEpochNano(), other.toEpochNano());
      if (compare == 0) {
        compare = time.compareTo(other.time);
      }
      return compare;
    }

    gbool OffsetTime::isAfter(OffsetTime const& other) const {
      return toEpochNano() > other.toEpochNano();
    }

    gbool OffsetTime::isBefore(OffsetTime const& other) const {
      return toEpochNano() < other.toEpochNano();
    }

    gbool OffsetTime::equals(Object const& other) const {
      return this == &other ||
          (Class<OffsetTime>::hasInstance(other) && compareTo(CORE_XCAST(OffsetTime const, other)) == 0);
    }

    gint OffsetTime::hash() const {
      return time.hash() ^ offset.hash();
    }

    String OffsetTime::toString() const {
      return time.toString() + offset.toString();
    }

    Object& OffsetTime::clone() const {
      return UNSAFE::newInstance<OffsetTime>(*this);
    }

    OffsetTime OffsetTime::with(LocalTime const& time, ZoneOffset const& offset) const {
      if (time == toLocalTime() && offset == toOffset())
        return *this;
      return OffsetTime(time, offset);
    }

    glong OffsetTime::toEpochNano() const {
      glong nod = time.toNanoOfDay();
      glong offsetNanos = offset.totalSeconds() * NANOS_PER_SECOND;
      return nod - offsetNanos;
    }
  } // time
} // core
