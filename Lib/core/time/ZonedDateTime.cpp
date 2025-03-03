//
// Created by admin on 07/01/25.
//

#include "ZonedDateTime.h"

#include <core/lang/Enum.h>
#include <core/time/Duration.h>
#include <core/time/Instant.h>
#include <core/time/OffsetDateTime.h>
#include <core/time/Period.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ValueRange.h>
#include <core/time/ZoneOffsetTransition.h>
#include <core/time/ZoneRules.h>
#include <core/util/List.h>
#include <core/util/Optional.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  using namespace util;
  CORE_ALIAS(Fields, time::TemporalUtils::Fields);
  CORE_ALIAS(Units, time::TemporalUtils::Units);

  namespace time {
    ZonedDateTime::ZonedDateTime(LocalDateTime const& dateTime,
                                 ZoneOffset const& offset, ZoneId const& zone)
      : dateTime(dateTime), offset(offset), zone(zone) {}

    ZonedDateTime ZonedDateTime::now() {
      return now(ZoneId::systemZone());
    }

    ZonedDateTime ZonedDateTime::now(ZoneId const& zone) {
      Instant now = Instant::now();
      return ofInstant(now, zone);
    }

    ZonedDateTime ZonedDateTime::of(LocalDate const& date,
                                    LocalTime const& time, ZoneId const& zone) {
      try {
        return of(LocalDateTime::of(date, time), zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::of(LocalDateTime const& dateTime,
                                    ZoneId const& zone) {
      try {
        return ofLocal(dateTime, zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::of(gint year, gint month, gint dayOfMonth,
                                    gint hour, gint minute, gint second,
                                    gint nanoOfSecond, ZoneId const& zone) {
      try {
        LocalDateTime ldt = LocalDateTime::of(year, month, dayOfMonth, hour,
                                              minute, second, nanoOfSecond);
        return ofLocal(ldt, zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::ofLocal(LocalDateTime const& dateTime,
                                         ZoneId const& zone) {
      try {
        if (Class<ZoneOffset>::hasInstance(zone)) {
          return ZonedDateTime(dateTime, CORE_XCAST(ZoneOffset const, zone), zone);
        }
        ZoneRules rules = zone.getRules();
        ZoneOffsetArray validOffsets = rules.validOffsets(dateTime);
        ZoneOffset offset = ZoneOffset::UTC;
        LocalDateTime ldt = dateTime;
        if (validOffsets.length() == 1) {
          offset = validOffsets.get(0);
        } else if (validOffsets.length() == 0) {
          OptionalTransition trans = rules.transition(dateTime);
          ldt = ldt.plusSeconds(trans.get().duration().seconds());
          offset = trans.get().offsetAfter();
        } else {
          offset = validOffsets.get(0); // protect against bad ZoneRules
        }
        return ZonedDateTime(ldt, offset, zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::ofLocal(LocalDateTime const& dateTime,
                                         ZoneId const& zone,
                                         ZoneOffset const& preferredOffset) {
      try {
        if (Class<ZoneOffset>::hasInstance(zone)) {
          return ZonedDateTime(dateTime, CORE_XCAST(ZoneOffset const, zone), zone);
        }
        ZoneRules rules = zone.getRules();
        ZoneOffsetArray validOffsets = rules.validOffsets(dateTime);
        ZoneOffset offset = ZoneOffset::UTC;
        LocalDateTime ldt = dateTime;
        if (validOffsets.length() == 1) {
          offset = validOffsets.get(0);
        } else if (validOffsets.length() == 0) {
          OptionalTransition trans = rules.transition(dateTime);
          ldt = ldt.plusSeconds(trans.get().duration().seconds());
          offset = trans.get().offsetAfter();
        } else {
          if (Arrays::binarySearch(validOffsets, preferredOffset) >= 0) {
            offset = preferredOffset;
          } else {
            offset = validOffsets.get(0); // protect against bad ZoneRules
          }
        }
        return ZonedDateTime(ldt, offset, zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::ofInstant(Instant const& instant,
                                           ZoneId const& zone) {
      try {
        return create(instant.epochSecond(), instant.nano(), zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::ofInstant(LocalDateTime const& dateTime,
                                           ZoneOffset const& offset,
                                           ZoneId const& zone) {
      try {
        if (zone.getRules().isValidOffset(dateTime, offset))
          return ZonedDateTime(dateTime, offset, zone);
        return create(dateTime.toEpochSecond(offset), dateTime.nano(), zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::ofStrict(LocalDateTime const& dateTime,
                                          ZoneOffset const& offset,
                                          ZoneId const& zone) {
      ZoneRules rules = zone.getRules();
      if (!rules.isValidOffset(dateTime, offset)) {
        OptionalTransition trans = rules.transition(dateTime);
        if (trans.isPresent() && trans.get().isGap()) {
          // error message says daylight savings for simplicity
          // even though there are other kinds of gaps
          DateTimeException(
                "LocalDateTime '"_Sl + dateTime +
                "' does not exist in zone '"_Sl +
                zone +
                "' due to a gap in the local time-line, typically caused by daylight savings"_Sl)
              .throws($ftrace());
        }
        DateTimeException(
          "ZoneOffset '"_Sl + offset + "' is not valid for LocalDateTime '"_Sl +
          dateTime + "' in zone '"_Sl + zone + "'"_Sl).throws($ftrace());
      }
      return ZonedDateTime(dateTime, offset, zone);
    }

    ZonedDateTime ZonedDateTime::from(TemporalAccessor const& temporal) {
      if (Class<ZonedDateTime>::hasInstance(temporal)) {
        return CORE_XCAST(ZonedDateTime const, temporal);
      }
      try {
        ZoneId const& zone = ZoneId::from(temporal);
        if (temporal.isSupported(TemporalField::INSTANT_SECONDS)) {
          glong epochSecond = temporal.getLong(TemporalField::INSTANT_SECONDS);
          gint nanoOfSecond = temporal.get(TemporalField::NANO_OF_SECOND);
          return create(epochSecond, nanoOfSecond, zone);
        } else {
          LocalDate date = LocalDate::from(temporal);
          LocalTime time = LocalTime::from(temporal);
          return of(date, time, zone);
        }
      } catch (DateTimeException const& ex) {
        DateTimeException(
          "Unable to obtain ZonedDateTime from TemporalAccessor: "_Sl +
          temporal + " of type " + typeName(temporal),
          ex).throws($ftrace());
      }
    }

    ZonedDateTime ZonedDateTime::parse(CharSequence const& text) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    ZonedDateTime ZonedDateTime::parse(CharSequence const& text,
                                       DateTimeFormatter const& formatter) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool ZonedDateTime::isSupported(TemporalField field) const {
      return Fields::isSupportedBy(*this, field) ||
          dateTime.isSupported(field) ||
          offset.isSupported(field);
    }

    gbool ZonedDateTime::isSupported(TemporalUnit unit) const {
      return unit != TemporalUnit::FOREVER || Units::isSupportedBy(*this, unit);
    }

    ValueRange ZonedDateTime::range(TemporalField field) const {
      try {
        if (field == TemporalField::INSTANT_SECONDS || field ==
          TemporalField::OFFSET_SECONDS)
          return Fields::range(*this, field);
        return dateTime.range(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint ZonedDateTime::get(TemporalField field) const {
      try {
        return range(field).checkValidIntValue(getLong(field), field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong ZonedDateTime::getLong(TemporalField field) const {
      try {
        switch (field) {
          case TemporalField::INSTANT_SECONDS: return toEpochSecond();
          case TemporalField::OFFSET_SECONDS: return offset.totalSeconds();
          default: return dateTime.getLong(field);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffset ZonedDateTime::toOffset() const {
      return offset;
    }

    ZonedDateTime ZonedDateTime::withEarlierOffsetAtOverlap() const {
      OptionalTransition trans = toZone().getRules().transition(dateTime);
      if (trans.isPresent() && trans.get().isOverlap()) {
        ZoneOffset earlierOffset = trans.get().offsetBefore();
        if (!earlierOffset.equals(offset)) {
          return ZonedDateTime(dateTime, earlierOffset, zone.get());
        }
      }
      return *this;
    }

    ZonedDateTime ZonedDateTime::withLaterOffsetAtOverlap() const {
      OptionalTransition trans = toZone().getRules().transition(
        toLocalDateTime());
      if (trans.isPresent()) {
        ZoneOffset laterOffset = trans.get().offsetAfter();
        if (!laterOffset.equals(offset)) {
          return ZonedDateTime(dateTime, laterOffset, zone.get());
        }
      }
      return *this;
    }

    ZoneId const& ZonedDateTime::toZone() const {
      return zone.get();
    }

    ZonedDateTime ZonedDateTime::withZoneSameLocal(ZoneId const& zone) const {
      try {
        return ofLocal(dateTime, zone, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withZoneSameInstant(ZoneId const& zone) const {
      try {
        return create(dateTime.toEpochSecond(offset), dateTime.nano(), zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withFixedOffsetZone() const {
      try {
        return ZonedDateTime(dateTime, offset, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime ZonedDateTime::toLocalDateTime() const {
      return dateTime;
    }

    LocalDate ZonedDateTime::toLocalDate() const {
      return dateTime.toLocalDate();
    }

    gint ZonedDateTime::year() const {
      return dateTime.year();
    }

    Month ZonedDateTime::month() const {
      return dateTime.month();
    }

    gint ZonedDateTime::dayOfMonth() const {
      return dateTime.dayOfMonth();
    }

    gint ZonedDateTime::dayOfYear() const {
      return dateTime.dayOfYear();
    }

    DayOfWeek ZonedDateTime::dayOfWeek() const {
      return dateTime.dayOfWeek();
    }

    LocalTime ZonedDateTime::toLocalTime() const {
      return dateTime.toLocalTime();
    }

    gint ZonedDateTime::hour() const {
      return dateTime.hour();
    }

    gint ZonedDateTime::minute() const {
      return dateTime.minute();
    }

    gint ZonedDateTime::second() const {
      return dateTime.second();
    }

    gint ZonedDateTime::nano() const {
      return dateTime.nano();
    }

    ZonedDateTime ZonedDateTime::with(TemporalAdjuster const& adjuster) const {
      try {
        // optimizations
        if (Class<LocalDate>::hasInstance(adjuster)) {
          return resolveLocal(LocalDateTime::of(
            CORE_XCAST(LocalDate const, adjuster), dateTime.toLocalTime()));
        } else if (Class<LocalTime>::hasInstance(adjuster)) {
          return resolveLocal(LocalDateTime::of(dateTime.toLocalDate(),
                                                CORE_XCAST(
                                                  LocalTime const, adjuster)));
        } else if (Class<LocalDateTime>::hasInstance(adjuster)) {
          return resolveLocal(CORE_XCAST(LocalDateTime const, adjuster));
        } else if (Class<OffsetDateTime>::hasInstance(adjuster)) {
          OffsetDateTime const& odt =
              CORE_XCAST(OffsetDateTime const, adjuster);
          return ofLocal(odt.toLocalDateTime(), zone.get(), odt.toOffset());
        } else if (Class<Instant>::hasInstance(adjuster)) {
          Instant const& instant = CORE_XCAST(Instant const, adjuster);
          return create(instant.epochSecond(), instant.nano(), zone.get());
        } else if (Class<ZoneOffset>::hasInstance(adjuster)) {
          return resolveOffset(CORE_XCAST(ZoneOffset const, adjuster));
        }
        return CORE_XCAST(ZonedDateTime, adjuster.adjustInto(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime
    ZonedDateTime::with(TemporalField field, glong newValue) const {
      try {
        switch (field) {
          case TemporalField::INSTANT_SECONDS: return create(
              newValue, nano(), zone.get());
          case TemporalField::OFFSET_SECONDS:
            Fields::checkValue(newValue, field);
            return resolveOffset(ZoneOffset::ofTotalSeconds(newValue));
          default: return resolveLocal(dateTime.with(field, newValue));
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withYear(gint year) const {
      try {
        return resolveLocal(dateTime.withYear(year));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withMonth(gint month) const {
      try {
        return resolveLocal(dateTime.withMonth(month));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withDayOfMonth(gint dayOfMonth) const {
      try {
        return resolveLocal(dateTime.withDayOfMonth(dayOfMonth));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withHour(gint hour) const {
      try {
        return resolveLocal(dateTime.withHour(hour));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withMinute(gint minute) const {
      try {
        return resolveLocal(dateTime.withMinute(minute));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withSecond(gint second) const {
      try {
        return resolveLocal(dateTime.withSecond(second));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::withNano(gint nanoOfSecond) const {
      try {
        return resolveLocal(dateTime.withNano(nanoOfSecond));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::truncatedTo(TemporalUnit unit) const {
      try {
        return resolveLocal(dateTime.truncatedTo(unit));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plus(TemporalAmount const& amountToAdd) const {
      try {
        if (Class<Period>::hasInstance(amountToAdd)) {
          Period const& periodToAdd = CORE_XCAST(Period const, amountToAdd);
          return resolveLocal(dateTime.plus(periodToAdd));
        }
        return CORE_XCAST(ZonedDateTime const, amountToAdd.addTo(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plus(glong amountToAdd,
                                      TemporalUnit unit) const {
      try {
        return Units::isDateBased(unit)
                 ? resolveLocal(dateTime.plus(amountToAdd, unit))
                 : resolveInstant(dateTime.plus(amountToAdd, unit));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plusYears(glong years) const {
      try {
        return resolveLocal(dateTime.plusYears(years));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plusMonths(glong months) const {
      try {
        return resolveLocal(dateTime.plusMonths(months));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plusWeeks(glong weeks) const {
      try {
        return resolveLocal(dateTime.plusWeeks(weeks));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plusDays(glong days) const {
      try {
        return resolveLocal(dateTime.plusDays(days));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plusHours(glong hours) const {
      try {
        return resolveInstant(dateTime.plusHours(hours));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plusMinutes(glong minutes) const {
      try {
        return resolveInstant(dateTime.plusMinutes(minutes));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plusSeconds(glong seconds) const {
      try {
        return resolveInstant(dateTime.plusSeconds(seconds));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::plusNanos(glong nanos) const {
      try {
        return resolveInstant(dateTime.plusNanos(nanos));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minus(TemporalAmount const& amountToSubtract) const {
      try {
        if (Class<Period>::hasInstance(amountToSubtract)) {
          Period const& periodToAdd =
              CORE_XCAST(Period const, amountToSubtract);
          return resolveLocal(dateTime.plus(periodToAdd));
        }
        return CORE_XCAST(ZonedDateTime const,
                          amountToSubtract.subtractFrom(*this));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minus(glong amountToSubtract,
                                       TemporalUnit unit) const {
      try {
        return (amountToSubtract == Long::MIN_VALUE)
                 ? plus(Long::MAX_VALUE, unit).plus(1, unit)
                 : plus(-amountToSubtract, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minusYears(glong years) const {
      try {
        return (years == Long::MIN_VALUE)
                 ? plusYears(Long::MAX_VALUE).plusYears(1)
                 : plusYears(-years);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minusMonths(glong months) const {
      try {
        return (months == Long::MIN_VALUE)
                 ? plusMonths(Long::MAX_VALUE).plusMonths(1)
                 : plusMonths(-months);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minusWeeks(glong weeks) const {
      try {
        return (weeks == Long::MIN_VALUE)
                 ? plusWeeks(Long::MAX_VALUE).plusWeeks(1)
                 : plusWeeks(-weeks);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minusDays(glong days) const {
      try {
        return (days == Long::MIN_VALUE)
                 ? plusDays(Long::MAX_VALUE).plusDays(1)
                 : plusDays(-days);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minusHours(glong hours) const {
      try {
        return (hours == Long::MIN_VALUE)
                 ? plusHours(Long::MAX_VALUE).plusHours(1)
                 : plusHours(-hours);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minusMinutes(glong minutes) const {
      try {
        return (minutes == Long::MIN_VALUE)
                 ? plusMinutes(Long::MAX_VALUE).plusMinutes(1)
                 : plusMinutes(-minutes);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minusSeconds(glong seconds) const {
      try {
        return (seconds == Long::MIN_VALUE)
                 ? plusSeconds(Long::MAX_VALUE).plusSeconds(1)
                 : plusSeconds(-seconds);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::minusNanos(glong nanos) const {
      try {
        return (nanos == Long::MIN_VALUE)
                 ? plusNanos(Long::MAX_VALUE).plusNanos(1)
                 : plusNanos(-nanos);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Optional<> ZonedDateTime::query(TemporalQuery const& query) const {
      if (query == TemporalQuery::localDate()) return toLocalDate();
      if (query == TemporalQuery::localTime()) return toLocalTime();
      if (query == TemporalQuery::offset()) return toOffset();
      if (query == TemporalQuery::zoneId()) return toZone();
      if (query == TemporalQuery::zone()) return toZone();
      if (query == TemporalQuery::precision())
        return (Enum<TemporalUnit>) TemporalUnit::NANOS;
      if (query == TemporalQuery::chronology())
        return Optional::empty();
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong ZonedDateTime::until(Temporal const& endExclusive,
                               TemporalUnit unit) const {
      try {
        ZonedDateTime end = ZonedDateTime::from(endExclusive);
        ZonedDateTime start = *this;
        try {
          end = end.withZoneSameInstant(zone.get());
        } catch (DateTimeException const& ex) {
          // end may be out of valid range. Adjust to end's zone.
          start = withZoneSameInstant(end.zone.get());
        }

        if (Units::isDateBased(unit)) {
          return start.dateTime.until(end.dateTime, unit);
        } else {
          return start.toOffsetDateTime().until(end.toOffsetDateTime(), unit);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String ZonedDateTime::format(DateTimeFormatter const& formatter) const {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    glong ZonedDateTime::toEpochSecond() const {
      glong epochDay = toLocalDate().toEpochDay();
      glong secs = epochDay * 86400L + toLocalTime().toSecondOfDay();
      secs -= toOffset().totalSeconds();
      return secs;
    }

    OffsetDateTime ZonedDateTime::toOffsetDateTime() const {
      return OffsetDateTime::of(dateTime, offset);
    }

    gint ZonedDateTime::compareTo(ZonedDateTime const& other) const {
      gint cmp = Long::compare(toEpochSecond(), other.toEpochSecond());
      if (cmp == 0) {
        cmp = toLocalTime().nano() - other.toLocalTime().nano();
        if (cmp == 0) {
          cmp = toLocalDateTime().compareTo(other.toLocalDateTime());
          if (cmp == 0) {
            cmp = toZone().getId().compareTo(other.toZone().getId());
            // if (cmp == 0) {
            //   cmp = chronology().compareTo(other.chronology());
            // }
          }
        }
      }
      return cmp;
    }

    gbool ZonedDateTime::isAfter(ZonedDateTime const& other) const {
      glong thisEpochSec = toEpochSecond();
      glong otherEpochSec = other.toEpochSecond();
      return thisEpochSec > otherEpochSec ||
      (thisEpochSec == otherEpochSec &&
        toLocalTime().nano() > other.toLocalTime().nano());
    }

    gbool ZonedDateTime::isBefore(ZonedDateTime const& other) const {
      glong thisEpochSec = toEpochSecond();
      glong otherEpochSec = other.toEpochSecond();
      return thisEpochSec < otherEpochSec ||
      (thisEpochSec == otherEpochSec &&
        toLocalTime().nano() < other.toLocalTime().nano());
    }

    gbool ZonedDateTime::equals(Object const& other) const {
      if (this == &other) return true;
      if (!Class<ZonedDateTime>::hasInstance(other))
        return false;

      ZonedDateTime const& zdt = CORE_XCAST(ZonedDateTime const, other);
      return dateTime.equals(zdt.dateTime) &&
          offset.equals(zdt.offset) &&
          zone.equals(zdt.zone);
    }

    gint ZonedDateTime::hash() const {
      return dateTime.hash() ^
          offset.hash() ^
          Integer::rotateLeft(zone.hash(), 3);
    }

    String ZonedDateTime::toString() const {
      String str = dateTime.toString() + offset.toString();
      if (offset != zone) {
        str += '[' + zone.toString() + ']';
      }
      return str;
    }

    Object& ZonedDateTime::clone() const {
      return UNSAFE::newInstance<ZonedDateTime>(*this);
    }

    ZonedDateTime ZonedDateTime::create(glong epochSecond, gint nanoOfSecond,
                                        ZoneId const& zone) {
      ZoneOffset offset = zone.getOffset(epochSecond);
      LocalDateTime ldt = LocalDateTime::ofEpochSecond(epochSecond,
        nanoOfSecond, offset);
      return ZonedDateTime(ldt, offset, zone);
    }

    ZonedDateTime ZonedDateTime::ofLenient(LocalDateTime const& ldt,
                                           ZoneOffset const& offset,
                                           ZoneId const& zone) {
      if (Class<ZoneOffset>::hasInstance(zone) && !offset.equals(zone)) {
        IllegalArgumentException("ZoneId must match ZoneOffset"_Sl).throws($ftrace());
      }
      return ZonedDateTime(ldt, offset, zone);
    }

    ZonedDateTime ZonedDateTime::resolveLocal(
      LocalDateTime const& newDateTime) const {
      try {
        return ofLocal(newDateTime, zone.get(), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::resolveInstant(
      LocalDateTime const& newDateTime) const {
      try {
        return ofInstant(newDateTime, offset, zone.get());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTime::resolveOffset(ZoneOffset const& offset) const {
      if (!offset.equals(this->offset) && zone.get().getRules().
        isValidOffset(dateTime, offset)) {
        return ZonedDateTime(dateTime, offset, zone.get());
      }
      return *this;
    }
  } // time
} // core
