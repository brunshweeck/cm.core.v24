//
// Created by admin on 07/01/25.
//

#include "OffsetDateTime.h"

#include <core/lang/Enum.h>
#include <core/time/OffsetTime.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZonedDateTime.h>
#include <core/util/Optional.h>

#include "Instant.h"
#include "ValueRange.h"
#include "ZoneRules.h"
#include "meta/time/TemporalUtils.h"

namespace core {
  using namespace util;
  CORE_ALIAS(Fields, time::TemporalUtils::Fields);
  CORE_ALIAS(Units, time::TemporalUtils::Units);

  namespace time {
    OffsetDateTime const OffsetDateTime::MIN = LocalDateTime::MIN.atOffset(ZoneOffset::MAX);
    OffsetDateTime const OffsetDateTime::MAX = LocalDateTime::MAX.atOffset(ZoneOffset::MIN);

    OffsetDateTime::OffsetDateTime(LocalDateTime const& dateTime, ZoneOffset const& offset)
      : dateTime(dateTime), offset(offset) {}

    OffsetDateTime::Comparator& OffsetDateTime::timeLineOrder() {
      class TimeLineComparator final : public Comparator {
      public:
        gint compare(OffsetDateTime const& t1, OffsetDateTime const& t2) const CORE_NOTHROW override {
          try {
            return compareInstant(t1, t2);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      static Comparator& comparator = UNSAFE::newInstance<TimeLineComparator>();
      return comparator;
    }

    OffsetDateTime OffsetDateTime::now() {
      return now(ZoneId::systemZone());
    }

    OffsetDateTime OffsetDateTime::now(ZoneId const& zone) {
      Instant now = Instant::now();
      ZoneOffset offset = zone.getRules().offset(now);
      return ofInstant(now, offset);
    }

    OffsetDateTime OffsetDateTime::of(LocalDate const& date, LocalTime const& time, ZoneOffset const& offset) {
      try {
        LocalDateTime ldt = LocalDateTime::of(date, time);
        return OffsetDateTime(ldt, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::of(LocalDateTime const& dateTime, ZoneOffset const& offset) {
      try {
        return OffsetDateTime(dateTime, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::of(gint year, gint month, gint dayOfMonth, gint hour, gint minute, gint second,
                                      gint nanoOfSecond, ZoneOffset const& offset) {
      try {
        LocalDateTime ldt = LocalDateTime::of(year, month, dayOfMonth, hour, minute, second, nanoOfSecond);
        return OffsetDateTime(ldt, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::ofInstant(Instant const& instant, ZoneId const& zone) {
      try {
        ZoneRules rules = zone.getRules();
        ZoneOffset offset = rules.offset(instant);

        LocalDateTime ldt = LocalDateTime::ofEpochSecond(instant.epochSecond(), instant.nano(), offset);
        return OffsetDateTime(ldt, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::from(TemporalAccessor const& temporal) {
      if (Class<OffsetDateTime>::hasInstance(temporal))
        return CORE_XCAST(OffsetDateTime const, temporal);
      try {
        ZoneOffset offset = ZoneOffset::from(temporal);
        OptionalDate date = temporal.query(TemporalQuery::localDate()).tryCast<LocalDate>();
        OptionalTime time = temporal.query(TemporalQuery::localTime()).tryCast<LocalTime>();
        if (date.isPresent() && time.isPresent()) {
          return OffsetDateTime::of(date.get(), time.get(), offset);
        } else {
          Instant instant = Instant::from(temporal);
          return OffsetDateTime::ofInstant(instant, offset);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::parse(CharSequence const& text) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    OffsetDateTime OffsetDateTime::parse(CharSequence const& text, DateTimeFormatter const& formatter) {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    gbool OffsetDateTime::isSupported(TemporalField field) const {
      return dateTime.isSupported(field) || offset.isSupported(field) || Fields::isSupportedBy(*this, field);
    }

    gbool OffsetDateTime::isSupported(TemporalUnit unit) const {
      return dateTime.isSupported(unit) ||
          (unit != TemporalUnit::FOREVER && Units::isSupportedBy(*this, unit));
    }

    ValueRange OffsetDateTime::range(TemporalField field) const {
      try {
        if (field == TemporalField::INSTANT_SECONDS || field == TemporalField::OFFSET_SECONDS)
          return Fields::range(*this, field);
        return dateTime.range(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint OffsetDateTime::get(TemporalField field) const {
      glong result = 0;
      try {
        result = getLong(field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
      if (result < Integer::MIN_VALUE || result > Integer::MAX_VALUE)
        TemporalException("Value of field "_Sl + field + " out of range"_Sl).throws($ftrace());
      return result;
    }

    glong OffsetDateTime::getLong(TemporalField field) const {
      try {
        switch (field) {
          case TemporalField::INSTANT_SECONDS: return toEpochSecond();
          case TemporalField::OFFSET_SECONDS: return offset.totalSeconds();
          default: return (dateTime.isSupported(field))
                            ? dateTime.getLong(field)
                            : Fields::getFrom(*this, field);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffset OffsetDateTime::toOffset() const {
      return offset;
    }

    OffsetDateTime OffsetDateTime::withOffsetSameLocal(ZoneOffset const& offset) const {
      return with(dateTime, offset);
    }

    OffsetDateTime OffsetDateTime::withOffsetSameInstant(ZoneOffset const& offset) const {
      gint secondsUntil = offset.totalSeconds() - toOffset().totalSeconds();
      try {
        LocalDateTime ldt = toLocalDateTime().plusSeconds(secondsUntil);
        return OffsetDateTime(ldt, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime OffsetDateTime::toLocalDateTime() const {
      return dateTime;
    }

    LocalDate OffsetDateTime::toLocalDate() const {
      return dateTime.toLocalDate();
    }

    gint OffsetDateTime::year() const {
      return dateTime.year();
    }

    Month OffsetDateTime::month() const {
      return dateTime.month();
    }

    gint OffsetDateTime::dayOfMonth() const {
      return dateTime.dayOfMonth();
    }

    gint OffsetDateTime::dayOfYear() const {
      return dateTime.dayOfYear();
    }

    DayOfWeek OffsetDateTime::dayOfWeek() const {
      return dateTime.dayOfWeek();
    }

    LocalTime OffsetDateTime::toLocalTime() const {
      return dateTime.toLocalTime();
    }

    gint OffsetDateTime::hour() const {
      return dateTime.hour();
    }

    gint OffsetDateTime::minute() const {
      return dateTime.minute();
    }

    gint OffsetDateTime::second() const {
      return dateTime.second();
    }

    gint OffsetDateTime::nano() const {
      return dateTime.nano();
    }

    OffsetDateTime OffsetDateTime::with(TemporalAdjuster const& adjuster) const {
      try {
        if (Class<LocalDate>::hasInstance(adjuster) ||
          Class<LocalTime>::hasInstance(adjuster) ||
          Class<LocalDateTime>::hasInstance(adjuster))
          return with(dateTime.with(adjuster), offset);
        if (Class<Instant>::hasInstance(adjuster))
          return ofInstant(CORE_XCAST(Instant const, adjuster), offset);
        if (Class<ZoneOffset>::hasInstance(adjuster))
          return with(dateTime, CORE_XCAST(ZoneOffset const, adjuster));
        if (Class<OffsetDateTime>::hasInstance(adjuster))
          return CORE_XCAST(OffsetDateTime const, adjuster);

        Temporal& t = adjuster.adjustInto(*this);
        OffsetDateTime odt = CORE_XCAST(OffsetDateTime const, t);
        if (this != &t)
          UNSAFE::deleteRegInstance(t);

        return odt;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::with(TemporalField field, glong newValue) const {
      try {
        switch (field) {
          case TemporalField::INSTANT_SECONDS:
            return ofInstant(Instant::ofEpochSecond(newValue, nano()), offset);
          case TemporalField::OFFSET_SECONDS:
            Fields::checkValue(newValue, field);
            return with(dateTime, ZoneOffset::ofTotalSeconds(newValue));
          default:
            return with(dateTime.with(field, newValue), offset);
        }
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::withYear(gint year) const {
      try {
        return with(dateTime.withYear(year), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::withMonth(gint month) const {
      try {
        return with(dateTime.withMonth(month), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::withDayOfMonth(gint dayOfMonth) const {
      try {
        return with(dateTime.withDayOfMonth(dayOfMonth), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::withDayOfYear(gint dayOfYear) const {
      try {
        return with(dateTime.withDayOfYear(dayOfYear), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::withHour(gint hour) const {
      try {
        return with(dateTime.withHour(hour), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::withMinute(gint minute) const {
      try {
        return with(dateTime.withMinute(minute), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::withSecond(gint second) const {
      try {
        return with(dateTime.withSecond(second), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::withNano(gint nanoOfSecond) const {
      try {
        return with(dateTime.withNano(nanoOfSecond), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::truncatedTo(TemporalUnit unit) const {
      try {
        return with(dateTime.truncatedTo(unit), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plus(TemporalAmount const& amountToAdd) const {
      try {
        Temporal& t = amountToAdd.addTo(*this);
        OffsetDateTime odt = CORE_XCAST(OffsetDateTime const, t);
        if (this != &t)
          UNSAFE::deleteRegInstance(t);
        return odt;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plus(glong amountToAdd, TemporalUnit unit) const {
      try {
        return with(dateTime.plus(amountToAdd, unit), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plusYears(glong years) const {
      try {
        return with(dateTime.plusYears(years), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plusMonths(glong months) const {
      try {
        return with(dateTime.plusMonths(months), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plusWeeks(glong weeks) const {
      try {
        return with(dateTime.plusWeeks(weeks), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plusDays(glong days) const {
      try {
        return with(dateTime.plusDays(days), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plusHours(glong hours) const {
      try {
        return with(dateTime.plusHours(hours), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plusMinutes(glong minutes) const {
      try {
        return with(dateTime.plusMinutes(minutes), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plusSeconds(glong seconds) const {
      try {
        return with(dateTime.plusSeconds(seconds), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::plusNanos(glong nanos) const {
      try {
        return with(dateTime.plusNanos(nanos), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minus(TemporalAmount const& amountToSubtract) const {
      try {
        Temporal& t = amountToSubtract.subtractFrom(*this);
        OffsetDateTime odt = CORE_XCAST(OffsetDateTime const, t);
        if (this != &t)
          UNSAFE::deleteRegInstance(t);
        return odt;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minus(glong amountToSubtract, TemporalUnit unit) const {
      try {
        return with(dateTime.minus(amountToSubtract, unit), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minusYears(glong years) const {
      try {
        return with(dateTime.minusYears(years), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minusMonths(glong months) const {
      try {
        return with(dateTime.minusMonths(months), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minusWeeks(glong weeks) const {
      try {
        return with(dateTime.minusWeeks(weeks), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minusDays(glong days) const {
      try {
        return with(dateTime.minusDays(days), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minusHours(glong hours) const {
      try {
        return with(dateTime.minusHours(hours), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minusMinutes(glong minutes) const {
      try {
        return with(dateTime.minusMinutes(minutes), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minusSeconds(glong seconds) const {
      try {
        return with(dateTime.minusSeconds(seconds), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetDateTime OffsetDateTime::minusNanos(glong nanos) const {
      try {
        return with(dateTime.minusNanos(nanos), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    TemporalAdjuster::Optional OffsetDateTime::query(TemporalQuery const& query) const {
      if (query == TemporalQuery::offset() || query == TemporalQuery::zone())
        return toOffset();
      if (query == TemporalQuery::localDate())
        return toLocalDate();
      if (query == TemporalQuery::localTime())
        return toLocalTime();
      if (query == TemporalQuery::precision())
        return (Enum<TemporalUnit>) TemporalUnit::NANOS;
      if (query == TemporalQuery::zoneId() || query == TemporalQuery::chronology())
        return Optional::empty();
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& OffsetDateTime::adjustInto(Temporal const& temporal) const {
      try {
        return adjustFieldTo(
          TemporalField::EPOCH_DAY, toLocalDate().toEpochDay(),
          adjustFieldTo(
            TemporalField::NANO_OF_DAY, toLocalTime().toNanoOfDay(),
            adjustFieldTo(
              TemporalField::OFFSET_SECONDS, toOffset().totalSeconds(),
              temporal)));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong OffsetDateTime::until(Temporal const& endExclusive, TemporalUnit unit) const {
      try {
        OffsetDateTime end = OffsetDateTime::from(endExclusive);
        OffsetDateTime start = *this;
        try {
          end = end.withOffsetSameInstant(offset);
        } catch (DateTimeException const& ex) {
          start = withOffsetSameInstant(end.offset);
        }
        return start.dateTime.until(end.dateTime, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String OffsetDateTime::format(DateTimeFormatter const& formatter) const {
      // TODO: Implement this method!
      CORE_ASSERT2(false, "Unimplemented method.");
    }

    ZonedDateTime OffsetDateTime::atZoneSameInstant(ZoneId const& zone) const {
      try {
        return ZonedDateTime::ofInstant(dateTime, offset, zone);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime OffsetDateTime::atZoneSimilarLocal(ZoneId const& zone) const {
      try {
        return ZonedDateTime::ofLocal(dateTime, zone, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    OffsetTime OffsetDateTime::toOffsetTime() const {
      try {
        return OffsetTime::of(dateTime.toLocalTime(), offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime OffsetDateTime::toZonedDateTime() const {
      try {
        return ZonedDateTime::of(dateTime, offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Instant OffsetDateTime::toInstant() const {
      try {
        return dateTime.toInstant(offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong OffsetDateTime::toEpochSecond() const {
      try {
        return dateTime.toEpochSecond(offset);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint OffsetDateTime::compareTo(OffsetDateTime const& other) const {
      try {
        gint cmp = toOffset().compareTo(other.toOffset());
        if (cmp != 0) {
          cmp = Long::compare(toEpochSecond(), other.toEpochSecond());
          if (cmp == 0) {
            cmp = toLocalTime().nano() - other.toLocalTime().nano();
          }
        }
        if (cmp == 0) {
          cmp = toLocalDateTime().compareTo(other.toLocalDateTime());
        }
        return cmp;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool OffsetDateTime::isAfter(OffsetDateTime const& other) const {
      return compareTo(other) > 0;
    }

    gbool OffsetDateTime::isBefore(OffsetDateTime const& other) const {
      return compareTo(other) < 0;
    }

    gbool OffsetDateTime::equals(Object const& other) const {
      return this == &other ||
          Class<OffsetDateTime>::hasInstance(other) && compareTo(CORE_XCAST(OffsetDateTime const, other)) == 0;
    }

    gint OffsetDateTime::hash() const {
      return dateTime.hash() ^ offset.hash();
    }

    String OffsetDateTime::toString() const {
      return dateTime.toString() + offset.toString();
    }

    Object& OffsetDateTime::clone() const {
      return UNSAFE::newInstance<OffsetDateTime>(*this);
    }

    gint OffsetDateTime::compareInstant(OffsetDateTime const& datetime1, OffsetDateTime const& datetime2) {
      if (datetime1.toOffset().equals(datetime2.toOffset())) {
        return datetime1.toLocalDateTime().compareTo(datetime2.toLocalDateTime());
      }
      gint cmp = Long::compare(datetime1.toEpochSecond(), datetime2.toEpochSecond());
      if (cmp == 0) {
        cmp = datetime1.toLocalTime().nano() - datetime2.toLocalTime().nano();
      }
      return cmp;
    }

    OffsetDateTime OffsetDateTime::with(LocalDateTime const& dateTime, ZoneOffset const& offset) const {
      return OffsetDateTime(dateTime, offset);
    }
  } // time
} // core
