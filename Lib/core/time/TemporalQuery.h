//
// Created by admin on 13/12/24.
//

#ifndef CORE24_TEMPORALQUERY_H
#define CORE24_TEMPORALQUERY_H

#include <core/lang/String.h>

namespace core {
  namespace time {
    /**
     * Strategy for querying a temporal object.
     * <p>
     * Queries are a key tool for extracting information from temporal objects.
     * They exist to externalize the process of querying, permitting different
     * approaches, as per the strategy design pattern.
     * Examples might be a query that checks if the date is the day before February 29th
     * in a leap year, or calculates the number of days to your next birthday.
     * </p>
     * <p>
     * The @b TemporalField interface provides another mechanism for querying
     * temporal objects. That interface is limited to returning a @c long.
     * By contrast, queries can return any type.
     * </p>
     * <p>
     * There are two equivalent ways of using a @c TemporalQuery.
     * The first is to invoke the method on this interface directly.
     * The second is to use @b Temporal::query(TemporalQuery):
     * @code
     *   // these two lines are equivalent, but the second approach is recommended
     *   auto& t = thisQuery.queryFrom(temporal);
     *   auto& t = temporal.query(thisQuery);
     * @endcode
     * It is recommended to use the second approach, @c query(TemporalQuery),
     * as it is a lot clearer to read in code.
     * <p>
     * The most common implementations are method references, such as
     * @c LocalDate::from and @c ZoneId::from.
     * </p>
     */
    class TemporalQuery : public virtual Object {
    public:
      CORE_ALIAS(Optional, util::Optional<>);

      /**
       * Queries the specified temporal object.
       * <p>
       * This queries the specified temporal object to return an object using the logic
       * encapsulated in the implementing class.
       * Examples might be a query that checks if the date is the day before February 29th
       * in a leap year, or calculates the number of days to your next birthday.
       * </p>
       * <p>
       * There are two equivalent ways of using this method.
       * The first is to invoke this method directly.
       * The second is to use @b Temporal::query(TemporalQuery):
       * @code
       *   // these two lines are equivalent, but the second approach is recommended
       *   auto &t = thisQuery.queryFrom(temporal);
       *   auto &t = temporal.query(thisQuery);
       * @endcode
       * It is recommended to use the second approach, @c query(TemporalQuery),
       * as it is a lot clearer to read in code.
       * </p>
       * @note
       * The implementation must take the input object and query it.
       * The implementation defines the logic of the query and is responsible for
       * documenting that logic.
       * It may use any method on @c Temporal to determine the result.
       * The input object must not be altered.
       * <p>
       * The input temporal object may be in a calendar system other than ISO.
       * Implementations may choose to document compatibility with other calendar systems,
       * or reject non-ISO temporal objects by <b> querying the chronology</b>.
       * </p>
       * <p>
       * This method may be called from multiple threads in parallel.
       * It must be thread-safe when invoked.
       * </p>
       * @param temporal  the temporal object to query
       * @return the queried value, may return empty optional to indicate not found
       * @throws DateTimeException if unable to query
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Optional queryFrom(TemporalAccessor const& temporal) const = 0;

      /**
       * A strict query for the @c ZoneId.
       * <p>
       * This queries a @c TemporalAccessor for the zone.
       * The zone is only returned if the date-time conceptually contains a @c ZoneId.
       * It will not be returned if the date-time only conceptually has an @c ZoneOffset.
       * Thus a @b ZonedDateTime will return the result of @c zone(),
       * but an @b OffsetDateTime will return empty optional.
       * <p>
       * In most cases, applications should use @b zone() as this query is too strict.
       * <p>
       * The result from JDK classes implementing @c TemporalAccessor is as follows:<br>
       * @c LocalDate returns empty option<br>
       * @c LocalTime returns empty option<br>
       * @c LocalDateTime returns empty option<br>
       * @c ZonedDateTime returns the associated zone<br>
       * @c OffsetTime returns empty option<br>
       * @c OffsetDateTime returns empty option<br>
       * @c ChronoLocalDate returns empty option<br>
       * @c ChronoLocalDateTime returns empty option<br>
       * @c ChronoZonedDateTime returns the associated zone<br>
       * @c Era returns empty option<br>
       * @c YearMonth returns empty option<br>
       * @c MonthDay returns empty option<br>
       * @c ZoneOffset returns empty option<br>
       * @c Instant returns empty option<br>
       *
       * @return a query that can obtain the zone ID of a temporal, not null
       */
      static TemporalQuery& zoneId();

      /**
       * A query for the @c Chronology.
       * <p>
       * This queries a @c TemporalAccessor for the chronology.
       * If the target @c TemporalAccessor represents a date, or part of a date,
       * then it should return the chronology that the date is expressed in.
       * As a result of this definition, objects only representing time, such as
       * @c LocalTime, will return empty optional.
       * <p>
       * The result from classes implementing @c TemporalAccessor is as follows:<br>
       * @c LocalDate returns @c IsoChronology::INSTANCE<br>
       * @c LocalTime returns empty option (does not represent a date)<br>
       * @c LocalDateTime returns @c IsoChronology::INSTANCE<br>
       * @c ZonedDateTime returns @c IsoChronology::INSTANCE<br>
       * @c OffsetTime returns empty option (does not represent a date)<br>
       * @c OffsetDateTime returns @c IsoChronology::INSTANCE<br>
       * @c ChronoLocalDate returns the associated chronology<br>
       * @c ChronoLocalDateTime returns the associated chronology<br>
       * @c ChronoZonedDateTime returns the associated chronology<br>
       * @c Era returns the associated chronology<br>
       * @c YearMonth returns @c IsoChronology::INSTANCE<br>
       * @c MonthDay returns empty option @c IsoChronology::INSTANCE<br>
       * @c ZoneOffset returns empty option (does not represent a date)<br>
       * @c Instant returns empty option (does not represent a date)<br>
       * <p>
       * The method @b Chronology::from(TemporalAccessor) can be used as a
       * @c TemporalQuery via a method reference, @c Chronology::from.
       * That method is equivalent to this query, except that it throws an
       * exception if a chronology cannot be obtained.
       *
       * @return a query that can obtain the chronology of a temporal, not null
       */
      static TemporalQuery& chronology();

      /**
       * A query for the smallest supported unit.
       * <p>
       * This queries a @c TemporalAccessor for the time precision.
       * If the target @c TemporalAccessor represents a consistent or complete date-time,
       * date or time then this must return the smallest precision actually supported.
       * Note that fields such as @c NANO_OF_DAY and @c NANO_OF_SECOND
       * are defined to always return ignoring the precision, thus this is the only
       * way to find the actual smallest supported unit.
       * For example, were @c GregorianCalendar to implement @c TemporalAccessor
       * it would return a precision of @c MILLIS.
       * <p>
       * The result from JDK classes implementing @c TemporalAccessor is as follows:<br>
       * @c LocalDate returns @c DAYS<br>
       * @c LocalTime returns @c NANOS<br>
       * @c LocalDateTime returns @c NANOS<br>
       * @c ZonedDateTime returns @c NANOS<br>
       * @c OffsetTime returns @c NANOS<br>
       * @c OffsetDateTime returns @c NANOS<br>
       * @c ChronoLocalDate returns @c DAYS<br>
       * @c ChronoLocalDateTime returns @c NANOS<br>
       * @c ChronoZonedDateTime returns @c NANOS<br>
       * @c Era returns @c ERAS<br>
       * @c YearMonth returns @c MONTHS<br>
       * @c MonthDay returns empty option (does not represent a complete date or time)<br>
       * @c ZoneOffset returns empty option (does not represent a date or time)<br>
       * @c Instant returns @c NANOS<br>
       *
       * @return a query that can obtain the precision of a temporal, not null
       */
      static TemporalQuery& precision();

      // non-special constants are standard queries that derive information from other information
      /**
       * A lenient query for the @c ZoneId, falling back to the @c ZoneOffset.
       * <p>
       * This queries a @c TemporalAccessor for the zone.
       * It first tries to obtain the zone, using @b zoneId().
       * If that is not found it tries to obtain the @b offset().
       * Thus a @b ZonedDateTime will return the result of @c zone(),
       * while an @b OffsetDateTime will return the result of @c getOffset().
       * <p>
       * In most cases, applications should use this query rather than @c zoneId().
       * <p>
       * The method @b ZoneId::from(TemporalAccessor) can be used as a
       * @c TemporalQuery via a method reference, @c ZoneId::from.
       * That method is equivalent to this query, except that it throws an
       * exception if a zone cannot be obtained.
       *
       * @return a query that can obtain the zone ID or offset of a temporal, not null
       */
      static TemporalQuery& zone();

      /**
       * A query for @c ZoneOffset returning null if not found.
       * <p>
       * This returns a @c TemporalQuery that can be used to query a temporal
       * object for the offset. The query will return empty optional if the temporal
       * object cannot supply an offset.
       * <p>
       * The query implementation examines the @b OFFSET_SECONDS
       * field and uses it to create a @c ZoneOffset.
       * <p>
       * The method @b ZoneOffset::from(TemporalAccessor) can be used as a
       * @c TemporalQuery via a method reference, @c ZoneOffset::from.
       * This query and @c ZoneOffset::from will return the same result if the
       * temporal object contains an offset. If the temporal object does not contain
       * an offset, then the method reference will throw an exception, whereas this
       * query will return empty optional.
       *
       * @return a query that can obtain the offset of a temporal, not null
       */
      static TemporalQuery& offset();

      /**
       * A query for @c LocalDate returning null if not found.
       * <p>
       * This returns a @c TemporalQuery that can be used to query a temporal
       * object for the local date. The query will return empty optional if the temporal
       * object cannot supply a local date.
       * <p>
       * The query implementation examines the @b EPOCH_DAY
       * field and uses it to create a @c LocalDate.
       * <p>
       * The method @b LocalDate::from(TemporalAccessor) can be used as a
       * @c TemporalQuery via a method reference, @c LocalDate::from.
       * This query and @c LocalDate::from will return the same result if the
       * temporal object contains a date. If the temporal object does not contain
       * a date, then the method reference will throw an exception, whereas this
       * query will return empty optional.
       *
       * @return a query that can obtain the date of a temporal, not null
       */
      static TemporalQuery& localDate();

      /**
       * A query for @c LocalTime returning null if not found.
       * <p>
       * This returns a @c TemporalQuery that can be used to query a temporal
       * object for the local time. The query will return empty optional if the temporal
       * object cannot supply a local time.
       * <p>
       * The query implementation examines the @b NANO_OF_DAY
       * field and uses it to create a @c LocalTime.
       * <p>
       * The method @b LocalTime::from(TemporalAccessor) can be used as a
       * @c TemporalQuery via a method reference, @c LocalTime::from.
       * This query and @c LocalTime::from will return the same result if the
       * temporal object contains a time. If the temporal object does not contain
       * a time, then the method reference will throw an exception, whereas this
       * query will return empty optional.
       *
       * @return a query that can obtain the time of a temporal, not null
       */
      static TemporalQuery& localTime();
    };
  } // time
} // core

#endif //CORE24_TEMPORALQUERY_H
