//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_TEMPORALACCESSOR_H
#define CORE24_CORE_24_TEMPORALACCESSOR_H

#include <core/time/TemporalField.h>

namespace core {
  namespace time {
    /**
     * Framework-level interface defining read-only access to a temporal object,
     * such as a date, time, offset or some combination of these.
     * <p>
     * This is the base interface type for date, time and offset objects.
     * It is implemented by those classes that can provide information
     * as @em fields or @em queries.
     * <p>
     * Most date and time information can be represented as a number.
     * These are modeled using @c TemporalField with the number held using
     * a @c long to handle large values. Year, month and day-of-month are
     * simple examples of fields, but they also include instant and offsets.
     * See @b TemporalField for the standard set of fields.
     * <p>
     * Two pieces of date/time information cannot be represented by numbers,
     * the @b chronology and the @em time-zone.
     * These can be accessed via @em queries using
     * the static methods defined on @b TemporalQuery.
     * <p>
     * A sub-interface, @b Temporal, extends this definition to one that also
     * supports adjustment and manipulation on more complete temporal objects.
     * <p>
     * This interface is a framework-level interface that should not be widely
     * used in application code. Instead, applications should create and pass
     * around instances of concrete types, such as @c LocalDate.
     * There are many reasons for this, part of which is that implementations
     * of this interface may be in calendar systems other than ISO.
     *
     * @note
     * This interface places no restrictions on the mutability of implementations,
     * however immutability is strongly recommended.
     */
    class TemporalAccessor : public virtual Object {
    public:
      CORE_ALIAS(Optional, util::Optional<>);
      CORE_ALIAS(OptionalDate, util::Optional<LocalDate>);
      CORE_ALIAS(OptionalTime, util::Optional<LocalTime>);
      CORE_ALIAS(OptionalZone, util::Optional<ZoneId>);
      CORE_ALIAS(OptionalOffset, util::Optional<ZoneOffset>);
      CORE_ALIAS(OptionalDateTime, util::Optional<LocalDateTime>);
      CORE_ALIAS(OptionalOffsetTime, util::Optional<OffsetTime>);
      CORE_ALIAS(OptionalOffsetDateTime, util::Optional<OffsetDateTime>);

      /**
       * Checks if the specified field is supported.
       * <p>
       * This checks if the date-time can be queried for the specified field.
       * If false, then calling the @b range and @b get
       * methods will throw an exception.
       *
       * @note
       * Implementations must check and handle all fields defined in @b TemporalField.
       * If the field is supported, then true must be returned, otherwise false must be returned.
       * <p>
       * Implementations must ensure that no observable state is altered when this
       * read-only method is invoked.
       *
       * @param field  the field to check, null returns false
       * @return true if this date-time can be queried for the field, false if not
       */
      virtual gbool isSupported(TemporalField field) const = 0;

      /**
       * Gets the range of valid values for the specified field.
       * <p>
       * All fields can be expressed as a @c long integer.
       * This method returns an object that describes the valid range for that value.
       * The value of this temporal object is used to enhance the accuracy of the returned range.
       * If the date-time cannot return the range, because the field is unsupported or for
       * some other reason, an exception will be thrown.
       * <p>
       * Note that the result only describes the minimum and maximum valid values
       * and it is important not to read too much into them. For example, there
       * could be values within the range that are invalid for the field.
       *
       * @note
       * Implementations must check and handle all fields defined in @b TemporalField.
       * If the field is supported, then the range of the field must be returned.
       * If unsupported, then an @c TemporalException must be thrown.
       * <p>
       * Implementations must ensure that no observable state is altered when this
       * read-only method is invoked.
       *
       * @param field  the field to query the range for, not null
       * @return the range of valid values for the field, not null
       * @throws DateTimeException if the range for the field cannot be obtained
       * @throws TemporalException if the field is not supported
       */
      virtual ValueRange range(TemporalField field) const = 0;

      /**
       * Gets the value of the specified field as an @c int.
       * <p>
       * This queries the date-time for the value of the specified field.
       * The returned value will always be within the valid range of values for the field.
       * If the date-time cannot return the value, because the field is unsupported or for
       * some other reason, an exception will be thrown.
       *
       * @note
       * Implementations must check and handle all fields defined in @b TemporalField.
       * If the field is supported and has an @c int range, then the value of
       * the field must be returned.
       * If unsupported, then an @c TemporalException must be thrown.
       * <p>
       * Implementations must ensure that no observable state is altered when this
       * read-only method is invoked.
       * <p>
       * The default implementation must behave equivalent to this code:
       * @code
       *  if (range(field).isIntValue()) {
       *    return range(field).checkValidIntValue(getLong(field), field);
       *  }
       *  throw new TemporalException("Invalid field " + field + " + for get() method, use getLong() instead");
       * @endcode
       *
       * @param field  the field to get, not null
       * @return the value for the field, within the valid range of values
       * @throws DateTimeException if a value for the field cannot be obtained or
       *         the value is outside the range of valid values for the field
       * @throws TemporalException if the field is not supported or
       *         the range of values exceeds an @c int
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual gint get(TemporalField field) const = 0;

      /**
       * Gets the value of the specified field as a @c long.
       * <p>
       * This queries the date-time for the value of the specified field.
       * The returned value may be outside the valid range of values for the field.
       * If the date-time cannot return the value, because the field is unsupported or for
       * some other reason, an exception will be thrown.
       *
       * @note
       * Implementations must check and handle all fields defined in @b TemporalField.
       * If the field is supported, then the value of the field must be returned.
       * If unsupported, then an @c TemporalException must be thrown.
       * <p>
       * Implementations must ensure that no observable state is altered when this
       * read-only method is invoked.
       *
       * @param field  the field to get, not null
       * @return the value for the field
       * @throws DateTimeException if a value for the field cannot be obtained
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual glong getLong(TemporalField field) const = 0;

      /**
       * Queries this date-time.
       * <p>
       * This queries this date-time using the specified query strategy object.
       * <p>
       * Queries are a key tool for extracting information from date-times.
       * They exists to externalize the process of querying, permitting different
       * approaches, as per the strategy design pattern.
       * Examples might be a query that checks if the date is the day before February 29th
       * in a leap year, or calculates the number of days to your next birthday.
       * <p>
       * The most common query implementations are method references, such as
       * @c LocalDate::from and @c ZoneId::from.
       * Additional implementations are provided as static methods on @b TemporalQuery.
       *
       * @note
       * The default implementation must behave equivalent to this code:
       * @code
       *  if (query == TemporalQueries.zoneId() ||
       *        query == TemporalQueries.chronology() || query == TemporalQueries.precision()) {
       *    return null;
       *  }
       *  return query.queryFrom(this);
       * @endcode
       * Future versions are permitted to add further queries to the if statement.
       * <p>
       * Implementations must ensure that no observable state is altered when this
       * read-only method is invoked.
       *
       * @param query  the query to invoke, not null
       * @return the query result, null may be returned (defined by the query)
       * @throws DateTimeException if unable to query
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Optional query(const TemporalQuery& query) const = 0;
    };
  } // time
} // core

#endif //CORE24_CORE_24_TEMPORALACCESSOR_H
