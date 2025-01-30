//
// Created by admin on 07/01/25.
//

#ifndef CORE24_ZONEDDATETIME_H
#define CORE24_ZONEDDATETIME_H

#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>

namespace core {
    namespace time {
        /**
         * A date-time with a time-zone in the ISO-8601 calendar system,
         * such as @c 2007-12-03T10:15:30+01:00 Europe/Paris.
         * <p>
         * @c ZonedDateTime is an immutable representation of a date-time with a time-zone.
         * This class stores all date and time fields, to a precision of nanoseconds,
         * and a time-zone, with a zone offset used to handle ambiguous local date-times.
         * For example, the value
         * "2nd October 2007 at 13:45.30.123456789 +02:00 in the Europe/Paris time-zone"
         * can be stored in a @c ZonedDateTime.
         * </p>
         * <p>
         * This class handles conversion from the local time-line of @c LocalDateTime
         * to the instant time-line of @c Instant.
         * The difference between the two time-lines is the offset from UTC/Greenwich,
         * represented by a @c ZoneOffset.
         * </p>
         * <p>
         * Converting between the two time-lines involves calculating the offset using the
         * @b rules accessed from the @c ZoneId.
         * Obtaining the offset for an instant is simple, as there is exactly one valid
         * offset for each instant. By contrast, obtaining the offset for a local date-time
         * is not straightforward. There are three cases:
         *
         * @li Normal, with one valid offset. For the vast majority of the year, the normal
         *  case applies, where there is a single valid offset for the local date-time.
         * @li Gap, with zero valid offsets. This is when clocks jump forward typically
         *  due to the spring daylight savings change from "winter" to "summer".
         *  In a gap there are local date-time values with no valid offset.
         * @li Overlap, with two valid offsets. This is when clocks are set back typically
         *  due to the autumn daylight savings change from "summer" to "winter".
         *  In an overlap there are local date-time values with two valid offsets.
         *
         * </p>
         * <p>
         * Any method that converts directly or implicitly from a local date-time to an
         * instant by obtaining the offset has the potential to be complicated.
         * </p>
         * <p>
         * For Gaps, the general strategy is that if the local date-time falls in the
         * middle of a Gap, then the resulting zoned date-time will have a local date-time
         * shifted forwards by the length of the Gap, resulting in a date-time in the later
         * offset, typically "summer" time.
         * </p>
         * <p>
         * For Overlaps, the general strategy is that if the local date-time falls in the
         * middle of an Overlap, then the previous offset will be retained. If there is no
         * previous offset, or the previous offset is invalid, then the earlier offset is
         * used, typically "summer" time.. Two additional methods,
         * @b withEarlierOffsetAtOverlap() and @b withLaterOffsetAtOverlap(),
         * help manage the case of an overlap.
         * </p>
         * <p>
         * In terms of design, this class should be viewed primarily as the combination
         * of a @c LocalDateTime and a @c ZoneId. The @c ZoneOffset is
         * a vital, but secondary, piece of information, used to ensure that the class
         * represents an instant, especially during a daylight savings overlap.
         * </p>
         * @note
         * A @c ZonedDateTime holds state equivalent to three separate objects,
         * a @c LocalDateTime, a @c ZoneId and the resolved @c ZoneOffset.
         * The offset and local date-time are used to define an instant when necessary.
         * The zone ID is used to obtain the rules for how and when the offset changes.
         * The offset cannot be freely set, as the zone controls which offsets are valid.
         */
        class ZonedDateTime final : public virtual Temporal, public virtual Comparable<ZonedDateTime> {
            CORE_ALIAS(ZoneID, Class<ZoneId>::Pointer);

            /**
             * The local date-time.
             */
            LocalDateTime dateTime;

            /**
             * The offset from UTC/Greenwich.
             */
            ZoneOffset zOffset;

            /**
             * The time-zone.
             */
            ZoneID zId = { };

        public:
            /**
             * Obtains the current date-time from the system clock in the default time-zone.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             * </p>
             * @return the current date-time using the system clock
             */
            static ZonedDateTime now();

            /**
             * Obtains the current date-time from the system clock in the specified time-zone.
             * <p>
             * Using this method will prevent the ability to use an alternate clock for testing
             * because the clock is hard-coded.
             * </p>
             * @param zone  the zone ID to use
             * @return the current date-time using the system clock
             */
            static ZonedDateTime now(ZoneId const& zone);

            /**
             * Obtains an instance of @c ZonedDateTime from a local date and time.
             * <p>
             * This creates a zoned date-time matching the input local date and time as closely as possible.
             * Time-zone rules, such as daylight savings, mean that not every local date-time
             * is valid for the specified zone, thus the local date-time may be adjusted.
             * </p>
             * <p>
             * The local date time and first combined to form a local date-time.
             * The local date-time is then resolved to a single instant on the time-line.
             * This is achieved by finding a valid offset from UTC/Greenwich for the local
             * date-time as defined by the @b rules of the zone ID.
             * </p>
             * <p>
             * In most cases, there is only one valid offset for a local date-time.
             * In the case of an overlap, when clocks are set back, there are two valid offsets.
             * This method uses the earlier offset typically corresponding to "summer".
             * </p>
             * <p>
             * In the case of a gap, when clocks jump forward, there is no valid offset.
             * Instead, the local date-time is adjusted to be later by the length of the gap.
             * For a typical one-hour daylight savings change, the local date-time will be
             * moved one-hour later into the offset typically corresponding to "summer".
             * </p>
             * @param date  the local date
             * @param time  the local time
             * @param zone  the time-zone
             * @return the offset date-time
             */
            CORE_EXPLICIT ZonedDateTime(LocalDate const& date, LocalTime const& time, ZoneId const& zone);

            /**
             * Obtains an instance of @c ZonedDateTime from a local date-time.
             * <p>
             * This creates a zoned date-time matching the input local date-time as closely as possible.
             * Time-zone rules, such as daylight savings, mean that not every local date-time
             * is valid for the specified zone, thus the local date-time may be adjusted.
             * </p>
             * <p>
             * The local date-time is resolved to a single instant on the time-line.
             * This is achieved by finding a valid offset from UTC/Greenwich for the local
             * date-time as defined by the @b rules of the zone ID.
             * </p>
             * <p>
             * In most cases, there is only one valid offset for a local date-time.
             * In the case of an overlap, when clocks are set back, there are two valid offsets.
             * This method uses the earlier offset typically corresponding to "summer".
             * </p>
             * <p>
             * In the case of a gap, when clocks jump forward, there is no valid offset.
             * Instead, the local date-time is adjusted to be later by the length of the gap.
             * For a typical one-hour daylight savings change, the local date-time will be
             * moved one-hour later into the offset typically corresponding to "summer".
             * </p>
             * @param dateTime  the local date-time
             * @param zone  the time-zone
             * @return the zoned date-time
             */
            CORE_EXPLICIT ZonedDateTime(LocalDateTime const& dateTime, ZoneId const& zone);

            /**
             * Obtains an instance of @c ZonedDateTime from a year, month, day,
             * hour, minute and time-zone.
             * <p>
             * This creates a zoned date-time matching the local date-time of the seven
             * specified fields as closely as possible.
             * Time-zone rules, such as daylight savings, mean that not every local date-time
             * is valid for the specified zone, thus the local date-time may be adjusted.
             * </p>
             * <p>
             * The local date-time is resolved to a single instant on the time-line.
             * This is achieved by finding a valid offset from UTC/Greenwich for the local
             * date-time as defined by the @b rules of the zone ID.
             * </p>
             * <p>
             * In most cases, there is only one valid offset for a local date-time.
             * In the case of an overlap, when clocks are set back, there are two valid offsets.
             * This method uses the earlier offset typically corresponding to "summer".
             * </p>
             * <p>
             * In the case of a gap, when clocks jump forward, there is no valid offset.
             * Instead, the local date-time is adjusted to be later by the length of the gap.
             * For a typical one-hour daylight savings change, the local date-time will be
             * moved one-hour later into the offset typically corresponding to "summer".
             * </p>
             * <p>
             * This method exists primarily for writing test cases.
             * Non test-code will typically use other methods to create an offset time.
             * @c LocalDateTime has five additional convenience variants of the
             * equivalent factory method taking fewer arguments.
             * They are not provided here to reduce the footprint of the API.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param zone  the time-zone
             * @return the offset date-time
             * @throws DateTimeException if the value of any field is out of range, or
             *  if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT ZonedDateTime(gint year, gint month, gint dayOfMonth,
                                        gint hour, gint minute,
                                        ZoneId const& zone);

            /**
             * Obtains an instance of @c ZonedDateTime from a year, month, day,
             * hour, minute, second and time-zone.
             * <p>
             * This creates a zoned date-time matching the local date-time of the seven
             * specified fields as closely as possible.
             * Time-zone rules, such as daylight savings, mean that not every local date-time
             * is valid for the specified zone, thus the local date-time may be adjusted.
             * </p>
             * <p>
             * The local date-time is resolved to a single instant on the time-line.
             * This is achieved by finding a valid offset from UTC/Greenwich for the local
             * date-time as defined by the @b rules of the zone ID.
             * </p>
             * <p>
             * In most cases, there is only one valid offset for a local date-time.
             * In the case of an overlap, when clocks are set back, there are two valid offsets.
             * This method uses the earlier offset typically corresponding to "summer".
             * </p>
             * <p>
             * In the case of a gap, when clocks jump forward, there is no valid offset.
             * Instead, the local date-time is adjusted to be later by the length of the gap.
             * For a typical one-hour daylight savings change, the local date-time will be
             * moved one-hour later into the offset typically corresponding to "summer".
             * </p>
             * <p>
             * This method exists primarily for writing test cases.
             * Non test-code will typically use other methods to create an offset time.
             * @c LocalDateTime has five additional convenience variants of the
             * equivalent factory method taking fewer arguments.
             * They are not provided here to reduce the footprint of the API.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param zone  the time-zone
             * @return the offset date-time
             * @throws DateTimeException if the value of any field is out of range, or
             *  if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT ZonedDateTime(gint year, gint month, gint dayOfMonth,
                                        gint hour, gint minute, gint second,
                                        ZoneId const& zone);

            /**
             * Obtains an instance of @c ZonedDateTime from a year, month, day,
             * hour, minute, second, nanosecond and time-zone.
             * <p>
             * This creates a zoned date-time matching the local date-time of the seven
             * specified fields as closely as possible.
             * Time-zone rules, such as daylight savings, mean that not every local date-time
             * is valid for the specified zone, thus the local date-time may be adjusted.
             * </p>
             * <p>
             * The local date-time is resolved to a single instant on the time-line.
             * This is achieved by finding a valid offset from UTC/Greenwich for the local
             * date-time as defined by the @b rules of the zone ID.
             * </p>
             * <p>
             * In most cases, there is only one valid offset for a local date-time.
             * In the case of an overlap, when clocks are set back, there are two valid offsets.
             * This method uses the earlier offset typically corresponding to "summer".
             * </p>
             * <p>
             * In the case of a gap, when clocks jump forward, there is no valid offset.
             * Instead, the local date-time is adjusted to be later by the length of the gap.
             * For a typical one-hour daylight savings change, the local date-time will be
             * moved one-hour later into the offset typically corresponding to "summer".
             * </p>
             * <p>
             * This method exists primarily for writing test cases.
             * Non test-code will typically use other methods to create an offset time.
             * @c LocalDateTime has five additional convenience variants of the
             * equivalent factory method taking fewer arguments.
             * They are not provided here to reduce the footprint of the API.
             * </p>
             * @param year  the year to represent, from MIN_YEAR to MAX_YEAR
             * @param month  the month-of-year to represent, from 1 (January) to 12 (December)
             * @param dayOfMonth  the day-of-month to represent, from 1 to 31
             * @param hour  the hour-of-day to represent, from 0 to 23
             * @param minute  the minute-of-hour to represent, from 0 to 59
             * @param second  the second-of-minute to represent, from 0 to 59
             * @param nanoOfSecond  the nano-of-second to represent, from 0 to 999,999,999
             * @param zone  the time-zone
             * @return the offset date-time
             * @throws DateTimeException if the value of any field is out of range, or
             *  if the day-of-month is invalid for the month-year
             */
            CORE_EXPLICIT ZonedDateTime(gint year, gint month, gint dayOfMonth,
                                        gint hour, gint minute, gint second, gint nanoOfSecond,
                                        ZoneId const& zone);

            ~ZonedDateTime() override;

            /**
             * Obtains an instance of @c ZonedDateTime from a local date-time
             * using the preferred offset if possible.
             * <p>
             * The local date-time is resolved to a single instant on the time-line.
             * This is achieved by finding a valid offset from UTC/Greenwich for the local
             * date-time as defined by the @b rules of the zone ID.
             * </p>
             * <p>
             * In most cases, there is only one valid offset for a local date-time.
             * In the case of an overlap, where clocks are set back, there are two valid offsets.
             * If the preferred offset is one of the valid offsets then it is used.
             * Otherwise the earlier valid offset is used, typically corresponding to "summer".
             * </p>
             * <p>
             * In the case of a gap, where clocks jump forward, there is no valid offset.
             * Instead, the local date-time is adjusted to be later by the length of the gap.
             * For a typical one-hour daylight savings change, the local date-time will be
             * moved one-hour later into the offset typically corresponding to "summer".
             * </p>
             * @param dateTime  the local date-time
             * @param zone  the time-zone
             * @param preferredOffset  the zone offset, null if no preference
             * @return the zoned date-time
             */
            static ZonedDateTime ofLocal(LocalDateTime const& dateTime, ZoneId const& zone,
                                         ZoneOffset const& preferredOffset);

            /**
             * Obtains an instance of @c ZonedDateTime from the instant formed by combining
             * the local date-time and offset.
             * <p>
             * This creates a zoned date-time by @b combining the @c LocalDateTime and @c ZoneOffset.
             * This combination uniquely specifies an instant without ambiguity.
             * </p>
             * <p>
             * Converting an instant to a zoned date-time is simple as there is only one valid
             * offset for each instant. If the valid offset is different to the offset specified,
             * then the date-time and offset of the zoned date-time will differ from those specified.
             * </p>
             * @param dateTime  the local date-time
             * @param offset  the zone offset
             * @param zone  the time-zone
             * @return the zoned date-time
             */
            static ZonedDateTime ofInstant(LocalDateTime const& dateTime, ZoneOffset const& offset, ZoneId const& zone);

            /**
             * Obtains an instance of @c ZonedDateTime strictly validating the
             * combination of local date-time, offset and zone ID.
             * <p>
             * This creates a zoned date-time ensuring that the offset is valid for the
             * local date-time according to the rules of the specified zone.
             * If the offset is invalid, an exception is thrown.
             * </p>
             * @param dateTime  the local date-time
             * @param offset  the zone offset
             * @param zone  the time-zone
             * @return the zoned date-time
             * @throws DateTimeException if the combination of arguments is invalid
             */
            static ZonedDateTime ofStrict(LocalDateTime const& dateTime, ZoneOffset const& offset, ZoneId const& zone);

            /**
             * Obtains an instance of @c ZonedDateTime leniently, for advanced use cases,
             * allowing any combination of local date-time, offset and zone ID.
             * <p>
             * This creates a zoned date-time with no checks other than no nulls.
             * This means that the resulting zoned date-time may have an offset that is in conflict
             * with the zone ID.
             * </p>
             * <p>
             * This method is intended for advanced use cases.
             * For example, consider the case where a zoned date-time with valid fields is created
             * and then stored in a database or serialization-based store. At some later point,
             * the object is then re-loaded. However, between those points in time, the government
             * that defined the time-zone has changed the rules, such that the originally stored
             * local date-time now does not occur. This method can be used to create the object
             * in an "invalid" state, despite the change in rules.
             * </p>
             * @param dateTime  the local date-time
             * @param offset  the zone offset
             * @param zone  the time-zone
             * @return the zoned date-time
             */
            static ZonedDateTime ofLenient(LocalDateTime const& dateTime, ZoneOffset const& offset, ZoneId const& zone);

            /**
             * Obtains an instance of @c ZonedDateTime from a temporal object.
             * <p>
             * This obtains a zoned date-time based on the specified temporal.
             * A @c Temporal represents an arbitrary set of date and time information,
             * which this factory converts to an instance of @c ZonedDateTime.
             * </p>
             * <p>
             * The conversion will first obtain a @c ZoneId from the temporal object,
             * falling back to a @c ZoneOffset if necessary. It will then try to obtain
             * an @c Instant, falling back to a @c LocalDateTime if necessary.
             * The result will be either the combination of @c ZoneId or @c ZoneOffset
             * with @c Instant or @c LocalDateTime.
             * Implementations are permitted to perform optimizations such as accessing
             * those fields that are equivalent to the relevant objects.
             * </p>
             * <p>
             * This method matches the signature of the functional interface @b TemporalQuery
             * allowing it to be used as a query via method reference, @c ZonedDateTime::from.
             * </p>
             * @param temporal  the temporal object to convert
             * @return the zoned date-time
             * @throws DateTimeException if unable to convert to an @c ZonedDateTime
             */
            static ZonedDateTime from(Temporal const& temporal);

            /**
             * Checks if the specified field is supported.
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The supported fields are:
             *
             * @li @c NANO_OF_SECOND
             * @li @c NANO_OF_DAY
             * @li @c MICRO_OF_SECOND
             * @li @c MICRO_OF_DAY
             * @li @c MILLI_OF_SECOND
             * @li @c MILLI_OF_DAY
             * @li @c SECOND_OF_MINUTE
             * @li @c SECOND_OF_DAY
             * @li @c MINUTE_OF_HOUR
             * @li @c MINUTE_OF_DAY
             * @li @c HOUR_OF_AMPM
             * @li @c CLOCK_HOUR_OF_AMPM
             * @li @c HOUR_OF_DAY
             * @li @c CLOCK_HOUR_OF_DAY
             * @li @c AMPM_OF_DAY
             * @li @c DAY_OF_WEEK
             * @li @c ALIGNED_DAY_OF_WEEK_IN_MONTH
             * @li @c ALIGNED_DAY_OF_WEEK_IN_YEAR
             * @li @c DAY_OF_MONTH
             * @li @c DAY_OF_YEAR
             * @li @c EPOCH_DAY
             * @li @c ALIGNED_WEEK_OF_MONTH
             * @li @c ALIGNED_WEEK_OF_YEAR
             * @li @c MONTH_OF_YEAR
             * @li @c PROLEPTIC_MONTH
             * @li @c YEAR_OF_ERA
             * @li @c YEAR
             * @li @c ERA
             * @li @c INSTANT_SECONDS
             * @li @c OFFSET_SECONDS
             * </p>
             * All other @c ChronoField instances will return false.
             *
             * @param field  the field to check, null returns false
             * @return true if the field is supported on this date-time, false if not
             */
            gbool isSupported(ChronoField field) const override;

            /**
             * Checks if the specified unit is supported.
             * <p>
             * If the unit is a @b ChronoUnit then the query is implemented here.
             * The supported units are:
             *
             * @li @c NANOS
             * @li @c MICROS
             * @li @c MILLIS
             * @li @c SECONDS
             * @li @c MINUTES
             * @li @c HOURS
             * @li @c HALF_DAYS
             * @li @c DAYS
             * @li @c WEEKS
             * @li @c MONTHS
             * @li @c YEARS
             * @li @c DECADES
             * @li @c CENTURIES
             * @li @c MILLENNIA
             * @li @c ERAS
             * </p>
             * All other @c ChronoUnit instances will return false.
             *
             * @param unit  the unit to check, null returns false
             * @return true if the unit can be added/subtracted, false if not
             */
            gbool isSupported(ChronoUnit unit) const override;

            /**
             * Gets the value of the specified field from this date-time as an @c int.
             * <p>
             * This queries this date-time for the value of the specified field.
             * The returned value will always be within the valid range of values for the field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The supported fields will return valid
             * values based on this date-time, except @c NANO_OF_DAY, @c MICRO_OF_DAY,
             * @c EPOCH_DAY, @c PROLEPTIC_MONTH and @c INSTANT_SECONDS which are too
             * large to fit in an @c int and throw an @c TemporalException.
             * All other @c ChronoField instances will throw an @c TemporalException.
             * </p>
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained or
             *         the value is outside the range of valid values for the field
             * @throws TemporalException if the field is not supported or
             *         the range of values exceeds an @c int
             * @throws ArithmeticException if numeric overflow occurs
             */
            gint get(ChronoField field) const override;

            /**
             * Gets the value of the specified field from this date-time as a @c long.
             * <p>
             * This queries this date-time for the value of the specified field.
             * If it is not possible to return the value, because the field is not supported
             * or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the query is implemented here.
             * The supported fields will return valid
             * values based on this date-time.
             * All other @c ChronoField instances will throw an @c TemporalException.
             * </p>
             * @param field  the field to get
             * @return the value for the field
             * @throws DateTimeException if a value for the field cannot be obtained
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong getLong(ChronoField field) const override;

            /**
             * Gets the zone offset, such as '+01:00'.
             * <p>
             * This is the offset of the local date-time from UTC/Greenwich.
             * </p>
             * @return the zone offset
             */
            ZoneOffset offset() const;

            /**
             * Returns a copy of this date-time changing the zone offset to the
             * earlier of the two valid offsets at a local time-line overlap.
             * <p>
             * This method only has any effect when the local time-line overlaps, such as
             * at an autumn daylight savings cutover. In this scenario, there are two
             * valid offsets for the local date-time. Calling this method will return
             * a zoned date-time with the earlier of the two selected.
             * </p>
             * <p>
             * If this method is called when it is not an overlap, @c this
             * is returned.
             * </p>
             * @return a @c ZonedDateTime based on this date-time with the earlier offset
             */
            ZonedDateTime withEarlierOffsetAtOverlap() const;

            /**
             * Returns a copy of this date-time changing the zone offset to the
             * later of the two valid offsets at a local time-line overlap.
             * <p>
             * This method only has any effect when the local time-line overlaps, such as
             * at an autumn daylight savings cutover. In this scenario, there are two
             * valid offsets for the local date-time. Calling this method will return
             * a zoned date-time with the later of the two selected.
             * </p>
             * <p>
             * If this method is called when it is not an overlap, @c this
             * is returned.
             * </p>
             * @return a @c ZonedDateTime based on this date-time with the later offset
             */
            ZonedDateTime withLaterOffsetAtOverlap() const;

            /**
             * Gets the time-zone, such as 'Europe/Paris'.
             * <p>
             * This returns the zone ID. This identifies the time-zone @b rules
             * that determine when and how the offset from UTC/Greenwich changes.
             * </p>
             * <p>
             * The zone ID may be same as the @em offset.
             * If this is true, then any future calculations, such as addition or subtraction,
             * have no complex edge cases due to time-zone rules.
             * See also @b withFixedOffsetZone().
             * </p>
             * @return the time-zone
             */
            ZoneId const& zone() const;

            /**
             * Returns a copy of this date-time with a different time-zone,
             * retaining the local date-time if possible.
             * <p>
             * This method changes the time-zone and retains the local date-time.
             * The local date-time is only changed if it is invalid for the new zone,
             * determined using the same approach as
             * @b ofLocal(LocalDateTime, ZoneId, ZoneOffset).
             * </p>
             * <p>
             * To change the zone and adjust the local date-time,
             * use @b withZoneSameInstant(ZoneId).
             * </p>
             * @param zone  the time-zone to change to
             * @return a @c ZonedDateTime based on this date-time with the requested zone
             */
            ZonedDateTime withZoneSameLocal(ZoneId const& zone) const;

            /**
             * Returns a copy of this date-time with a different time-zone,
             * retaining the instant.
             * <p>
             * This method changes the time-zone and retains the instant.
             * This normally results in a change to the local date-time.
             * </p>
             * <p>
             * This method is based on retaining the same instant, thus gaps and overlaps
             * in the local time-line have no effect on the result.
             * </p>
             * <p>
             * To change the offset while keeping the local time,
             * use @b withZoneSameLocal(ZoneId).
             * </p>
             * @param zone  the time-zone to change to
             * @return a @c ZonedDateTime based on this date-time with the requested zone
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime withZoneSameInstant(ZoneId const& zone) const;

            /**
             * Returns a copy of this date-time with the zone ID set to the offset.
             * <p>
             * This returns a zoned date-time where the zone ID is the same as @b offset().
             * The local date-time, offset and instant of the result will be the same as in this date-time.
             * </p>
             * <p>
             * Setting the date-time to a fixed single offset means that any future
             * calculations, such as addition or subtraction, have no complex edge cases
             * due to time-zone rules.
             * This might also be useful when sending a zoned date-time across a network,
             * as most protocols, such as ISO-8601, only handle offsets,
             * and not region-based zone IDs.
             * </p>
             * @return a @c ZonedDateTime with the zone ID set to the offset
             */
            ZonedDateTime withFixedOffsetZone() const;

            /**
             * Gets the @c LocalDateTime part of this date-time.
             * <p>
             * This returns a @c LocalDateTime with the same year, month, day and time
             * as this date-time.
             * </p>
             * @return the local date-time part of this date-time
             */
            LocalDateTime toLocalDateTime() const;

            /**
             * Gets the @c LocalDate part of this date-time.
             * <p>
             * This returns a @c LocalDate with the same year, month and day
             * as this date-time.
             * </p>
             * @return the date part of this date-time
             */
            LocalDate toLocalDate() const;

            /**
             * Gets the year field.
             * <p>
             * This method returns the primitive @c int value for the year.
             * </p>
             * <p>
             * The year returned by this method is proleptic as per @c get(YEAR).
             * To obtain the year-of-era, use @c get(YEAR_OF_ERA).
             * </p>
             * @return the year, from MIN_YEAR to MAX_YEAR
             */
            gint year() const;

            /**
             * Gets the month-of-year field using the @c Month enum.
             * <p>
             * This method returns the enum @b Month for the month.
             * This avoids confusion as to what @c int values mean.
             * If you need access to the primitive @c int value then the enum
             * provides the @b int value.
             * </p>
             * @return the month-of-year
             */
            LocalDate::Month month() const;

            /**
             * Gets the day-of-month field.
             * <p>
             * This method returns the primitive @c int value for the day-of-month.
             * </p>
             * @return the day-of-month, from 1 to 31
             */
            gint dayOfMonth() const;

            /**
             * Gets the day-of-year field.
             * <p>
             * This method returns the primitive @c int value for the day-of-year.
             * </p>
             * @return the day-of-year, from 1 to 365, or 366 in a leap year
             */
            gint dayOfYear() const;

            /**
             * Gets the day-of-week field, which is an enum @c DayOfWeek.
             * <p>
             * This method returns the enum @b DayOfWeek for the day-of-week.
             * This avoids confusion as to what @c int values mean.
             * If you need access to the primitive @c int value then the enum
             * provides the @b int value.
             * </p>
             * <p>
             * Additional information can be obtained from the @c DayOfWeek.
             * This includes textual names of the values.
             * </p>
             * @return the day-of-week
             */
            LocalDate::DayOfWeek dayOfWeek() const;

            /**
             * Gets the @c LocalTime part of this date-time.
             * <p>
             * This returns a @c LocalTime with the same hour, minute, second and
             * nanosecond as this date-time.
             * </p>
             * @return the time part of this date-time
             */
            LocalTime toLocalTime() const;

            /**
             * Gets the hour-of-day field.
             *
             * @return the hour-of-day, from 0 to 23
             */
            gint hour() const;

            /**
             * Gets the minute-of-hour field.
             *
             * @return the minute-of-hour, from 0 to 59
             */
            gint minute() const;

            /**
             * Gets the second-of-minute field.
             *
             * @return the second-of-minute, from 0 to 59
             */
            gint second() const;

            /**
             * Gets the nano-of-second field.
             *
             * @return the nano-of-second, from 0 to 999,999,999
             */
            gint nano() const;

            /**
             * Returns a copy of this date-time with the specified field set to a new value.
             * <p>
             * This returns a @c ZonedDateTime, based on this one, with the value
             * for the specified field changed.
             * This can be used to change any supported field, such as the year, month or day-of-month.
             * If it is not possible to set the value, because the field is not supported or for
             * some other reason, an exception is thrown.
             * </p>
             * <p>
             * In some cases, changing the specified field can cause the resulting date-time to become invalid,
             * such as changing the month from 31st January to February would make the day-of-month invalid.
             * In cases like this, the field is responsible for resolving the date. Typically it will choose
             * the previous valid date, which would be the last valid day of February in this example.
             * </p>
             * <p>
             * If the field is a @b ChronoField then the adjustment is implemented here.
             * </p>
             * <p>
             * The @c INSTANT_SECONDS field will return a date-time with the specified instant.
             * The zone and nano-of-second are unchanged.
             * The result will have an offset derived from the new instant and original zone.
             * If the new instant value is outside the valid range then a @c DateTimeException will be thrown.
             * </p>
             * <p>
             * The @c OFFSET_SECONDS field will typically be ignored.
             * The offset of a @c ZonedDateTime is controlled primarily by the time-zone.
             * As such, changing the offset does not generally make sense, because there is only
             * one valid offset for the local date-time and zone.
             * If the zoned date-time is in a daylight savings overlap, then the offset is used
             * to switch between the two valid offsets. In all other cases, the offset is ignored.
             * If the new offset value is outside the valid range then a @c DateTimeException will be thrown.
             * </p>
             * <p>
             * The other supported fields will behave as per
             * the matching method on @b LocalDateTime.
             * The zone is not part of the calculation and will be unchanged.
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * <p>
             * All other @c ChronoField instances will throw an @c TemporalException.
             * </p>
             * @param field  the field to set in the result
             * @param newValue  the new value of the field in the result
             * @return a @c ZonedDateTime based on @c this with the specified field set
             * @throws DateTimeException if the field cannot be set
             * @throws TemporalException if the field is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            ZonedDateTime with(ChronoField field, glong newValue) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the year altered.
             * <p>
             * This operates on the local time-line, changing the year of the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param year  the year to set in the result, from MIN_YEAR to MAX_YEAR
             * @return a @c ZonedDateTime based on this date-time with the requested year
             * @throws DateTimeException if the year value is invalid
             */
            ZonedDateTime withYear(gint year) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the month-of-year altered.
             * <p>
             * This operates on the local time-line, changing the month of the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param month  the month-of-year to set in the result, from 1 (January) to 12 (December)
             * @return a @c ZonedDateTime based on this date-time with the requested month
             * @throws DateTimeException if the month-of-year value is invalid
             */
            ZonedDateTime withMonth(gint month) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the day-of-month altered.
             * <p>
             * This operates on the local time-line, changing the day-of-month of the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param dayOfMonth  the day-of-month to set in the result, from 1 to 28-31
             * @return a @c ZonedDateTime based on this date-time with the requested day
             * @throws DateTimeException if the day-of-month value is invalid,
             *  or if the day-of-month is invalid for the month-year
             */
            ZonedDateTime withDayOfMonth(gint dayOfMonth) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the hour-of-day altered.
             * <p>
             * This operates on the local time-line, changing the time of the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param hour  the hour-of-day to set in the result, from 0 to 23
             * @return a @c ZonedDateTime based on this date-time with the requested hour
             * @throws DateTimeException if the hour value is invalid
             */
            ZonedDateTime withHour(gint hour) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the minute-of-hour altered.
             * <p>
             * This operates on the local time-line, changing the time of the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param minute  the minute-of-hour to set in the result, from 0 to 59
             * @return a @c ZonedDateTime based on this date-time with the requested minute
             * @throws DateTimeException if the minute value is invalid
             */
            ZonedDateTime withMinute(gint minute) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the second-of-minute altered.
             * <p>
             * This operates on the local time-line, changing the time of the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param second  the second-of-minute to set in the result, from 0 to 59
             * @return a @c ZonedDateTime based on this date-time with the requested second
             * @throws DateTimeException if the second value is invalid
             */
            ZonedDateTime withSecond(gint second) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the nano-of-second altered.
             * <p>
             * This operates on the local time-line, changing the time of the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param nanoOfSecond  the nano-of-second to set in the result, from 0 to 999,999,999
             * @return a @c ZonedDateTime based on this date-time with the requested nanosecond
             * @throws DateTimeException if the nano value is invalid
             */
            ZonedDateTime withNano(gint nanoOfSecond) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the time truncated.
             * <p>
             * Truncation returns a copy of the original date-time with fields
             * smaller than the specified unit set to zero.
             * For example, truncating with the @b minutes unit
             * will set the second-of-minute and nano-of-second field to zero.
             * </p>
             * <p>
             * The unit must have a @em duration
             * that divides into the length of a standard day without remainder.
             * This includes all supplied time units on @b ChronoUnit and
             * @b DAYS. Other units throw an exception.
             * </p>
             * <p>
             * This operates on the local time-line, truncating
             * the underlying local date-time. This is then converted back to a
             * @c ZonedDateTime, using the zone ID to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param unit  the unit to truncate to
             * @return a @c ZonedDateTime based on this date-time with the time truncated
             * @throws DateTimeException if unable to truncate
             * @throws TemporalException if the unit is not supported
             */
            ZonedDateTime truncateTo(ChronoUnit unit) const;

            /**
             * Returns a copy of this date-time with the specified amount added.
             * <p>
             * This returns a @c ZonedDateTime, based on this one, with the amount
             * in terms of the unit added. If it is not possible to add the amount, because the
             * unit is not supported or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * If the field is a @b ChronoUnit then the addition is implemented here.
             * The zone is not part of the calculation and will be unchanged in the result.
             * The calculation for date and time units differ.
             * </p>
             * <p>
             * Date units operate on the local time-line.
             * The period is first added to the local date-time, then converted back
             * to a zoned date-time using the zone ID.
             * The conversion uses @b ofLocal(LocalDateTime, ZoneId, ZoneOffset)
             * with the offset before the addition.
             * </p>
             * <p>
             * Time units operate on the instant time-line.
             * The period is first added to the local date-time, then converted back to
             * a zoned date-time using the zone ID.
             * The conversion uses @b ofInstant(LocalDateTime, ZoneOffset, ZoneId)
             * with the offset before the addition.
             * </p>
             * @param amountToAdd  the amount of the unit to add to the result, may be negative
             * @param unit  the unit of the amount to add
             * @return a @c ZonedDateTime based on this date-time with the specified amount added
             * @throws DateTimeException if the addition cannot be made
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            ZonedDateTime plus(glong amountToAdd, ChronoUnit unit) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of years added.
             * <p>
             * This operates on the local time-line, adding years to the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param years  the years to add, may be negative
             * @return a @c ZonedDateTime based on this date-time with the years added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime plusYears(gint years) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of months added.
             * <p>
             * This operates on the local time-line, adding months to the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param months  the months to add, may be negative
             * @return a @c ZonedDateTime based on this date-time with the months added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime plusMonths(gint months) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of weeks added.
             * <p>
             * This operates on the local time-line, adding weeks to the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param weeks  the weeks to add, may be negative
             * @return a @c ZonedDateTime based on this date-time with the weeks added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime plusWeeks(gint weeks) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of days added.
             * <p>
             * This operates on the local time-line, adding days to the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param days  the days to add, may be negative
             * @return a @c ZonedDateTime based on this date-time with the days added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime plusDays(gint days) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of hours added.
             * <p>
             * This operates on the instant time-line, such that adding one-hour will
             * always be a duration of one-hour later.
             * This may cause the local date-time to change by an amount other than one-hour.
             * Note that this is a different approach to that used by days, months and years,
             * thus adding one day is not the same as adding 24 hours.
             * </p>
             * <p>
             * For example, consider a time-zone, such as 'Europe/Paris', where the
             * Autumn DST cutover means that the local times 02:00 to 02:59 occur twice
             * changing from offset +02:00 in summer to +01:00 in winter.
             *
             * @li Adding one-hour to 01:30+02:00 will result in 02:30+02:00
             *     (both in summer time)
             * @li Adding one-hour to 02:30+02:00 will result in 02:30+01:00
             *     (moving from summer to winter time)
             * @li Adding one-hour to 02:30+01:00 will result in 03:30+01:00
             *     (both in winter time)
             * @li Adding three hours to 01:30+02:00 will result in 03:30+01:00
             *     (moving from summer to winter time)
             * </p>
             *
             * @param hours  the hours to add, may be negative
             * @return a @c ZonedDateTime based on this date-time with the hours added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime plusHours(gint hours) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of minutes added.
             * <p>
             * This operates on the instant time-line, such that adding one minute will
             * always be a duration of one minute later.
             * This may cause the local date-time to change by an amount other than one minute.
             * Note that this is a different approach to that used by days, months and years.
             * </p>
             * @param minutes  the minutes to add, may be negative
             * @return a @c ZonedDateTime based on this date-time with the minutes added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime plusMinutes(gint minutes) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of seconds added.
             * <p>
             * This operates on the instant time-line, such that adding one second will
             * always be a duration of one second later.
             * This may cause the local date-time to change by an amount other than one second.
             * Note that this is a different approach to that used by days, months and years.
             * </p>
             * @param seconds  the seconds to add, may be negative
             * @return a @c ZonedDateTime based on this date-time with the seconds added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime plusSeconds(gint seconds) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of nanoseconds added.
             * <p>
             * This operates on the instant time-line, such that adding one nano will
             * always be a duration of one nano later.
             * This may cause the local date-time to change by an amount other than one nano.
             * Note that this is a different approach to that used by days, months and years.
             * </p>
             * @param nanos  the nanos to add, may be negative
             * @return a @c ZonedDateTime based on this date-time with the nanoseconds added
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime plusNanos(gint nanos) const;

            /**
             * Returns a copy of this date-time with the specified amount subtracted.
             * <p>
             * This returns a @c ZonedDateTime, based on this one, with the amount
             * in terms of the unit subtracted. If it is not possible to subtract the amount,
             * because the unit is not supported or for some other reason, an exception is thrown.
             * </p>
             * <p>
             * The calculation for date and time units differ.
             * </p>
             * <p>
             * Date units operate on the local time-line.
             * The period is first subtracted from the local date-time, then converted back
             * to a zoned date-time using the zone ID.
             * The conversion uses @b ofLocal(LocalDateTime, ZoneId, ZoneOffset)
             * with the offset before the subtraction.
             * </p>
             * <p>
             * Time units operate on the instant time-line.
             * The period is first subtracted from the local date-time, then converted back to
             * a zoned date-time using the zone ID.
             * The conversion uses @b ofInstant(LocalDateTime, ZoneOffset, ZoneId)
             * with the offset before the subtraction.
             * </p>
             * <p>
             * This method is equivalent to @b plus(long, ChronoUnit) with the amount negated.
             * See that method for a full description of how addition, and thus subtraction, works.
             * </p>
             * @param amountToSubtract  the amount of the unit to subtract from the result, may be negative
             * @param unit  the unit of the amount to subtract
             * @return a @c ZonedDateTime based on this date-time with the specified amount subtracted
             * @throws DateTimeException if the subtraction cannot be made
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            ZonedDateTime minus(glong amountToSubtract, ChronoUnit unit) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of years subtracted.
             * <p>
             * This operates on the local time-line, subtracting years to the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param years  the years to subtract, may be negative
             * @return a @c ZonedDateTime based on this date-time with the years subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime minusYears(gint years) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of months subtracted.
             * <p>
             * This operates on the local time-line, subtracting months to the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param months  the months to subtract, may be negative
             * @return a @c ZonedDateTime based on this date-time with the months subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime minusMonths(gint months) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of weeks subtracted.
             * <p>
             * This operates on the local time-line, subtracting weeks to the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param weeks  the weeks to subtract, may be negative
             * @return a @c ZonedDateTime based on this date-time with the weeks subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime minusWeeks(gint weeks) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of days subtracted.
             * <p>
             * This operates on the local time-line, subtracting days to the local date-time.
             * This is then converted back to a @c ZonedDateTime, using the zone ID
             * to obtain the offset.
             * </p>
             * <p>
             * When converting back to @c ZonedDateTime, if the local date-time is in an overlap,
             * then the offset will be retained if possible, otherwise the earlier offset will be used.
             * If in a gap, the local date-time will be adjusted forward by the length of the gap.
             * </p>
             * @param days  the days to subtract, may be negative
             * @return a @c ZonedDateTime based on this date-time with the days subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime minusDays(gint days) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of hours subtracted.
             * <p>
             * This operates on the instant time-line, such that subtracting one-hour will
             * always be a duration of one-hour earlier.
             * This may cause the local date-time to change by an amount other than one-hour.
             * Note that this is a different approach to that used by days, months and years,
             * thus subtracting one day is not the same as adding 24 hours.
             * </p>
             * <p>
             * For example, consider a time-zone, such as 'Europe/Paris', where the
             * Autumn DST cutover means that the local times 02:00 to 02:59 occur twice
             * changing from offset +02:00 in summer to +01:00 in winter.
             *
             * @li Subtracting one-hour from 03:30+01:00 will result in 02:30+01:00
             *     (both in winter time)
             * @li Subtracting one-hour from 02:30+01:00 will result in 02:30+02:00
             *     (moving from winter to summer time)
             * @li Subtracting one-hour from 02:30+02:00 will result in 01:30+02:00
             *     (both in summer time)
             * @li Subtracting three hours from 03:30+01:00 will result in 01:30+02:00
             *     (moving from winter to summer time)
             * </p>
             *
             * @param hours  the hours to subtract, may be negative
             * @return a @c ZonedDateTime based on this date-time with the hours subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime minusHours(gint hours) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of minutes subtracted.
             * <p>
             * This operates on the instant time-line, such that subtracting one minute will
             * always be a duration of one minute earlier.
             * This may cause the local date-time to change by an amount other than one minute.
             * Note that this is a different approach to that used by days, months and years.
             * </p>
             * @param minutes  the minutes to subtract, may be negative
             * @return a @c ZonedDateTime based on this date-time with the minutes subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime minusMinutes(gint minutes) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of seconds subtracted.
             * <p>
             * This operates on the instant time-line, such that subtracting one second will
             * always be a duration of one second earlier.
             * This may cause the local date-time to change by an amount other than one second.
             * Note that this is a different approach to that used by days, months and years.
             * </p>
             * @param seconds  the seconds to subtract, may be negative
             * @return a @c ZonedDateTime based on this date-time with the seconds subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime minusSeconds(gint seconds) const;

            /**
             * Returns a copy of this @c ZonedDateTime with the specified number of nanoseconds subtracted.
             * <p>
             * This operates on the instant time-line, such that subtracting one nano will
             * always be a duration of one nano earlier.
             * This may cause the local date-time to change by an amount other than one nano.
             * Note that this is a different approach to that used by days, months and years.
             * </p>
             * @param nanos  the nanos to subtract, may be negative
             * @return a @c ZonedDateTime based on this date-time with the nanoseconds subtracted
             * @throws DateTimeException if the result exceeds the supported date range
             */
            ZonedDateTime minusNanos(gint nanos) const;

            /**
             * Queries this date-time using the specified query.
             * <p>
             * This queries this date-time using the specified query strategy object.
             * The @c TemporalQuery object defines the logic to be used to
             * obtain the result. Read the documentation of the query to understand
             * what the result of this method will be.
             * </p>
             * <p>
             * The result of this method is obtained by invoking the
             * @b TemporalQuery::queryFrom(Temporal) method on the
             * specified query passing @c this as the argument.
             * </p>
             * @param query  the query to invoke
             * @return the query result, null may be returned (defined by the query)
             * @throws DateTimeException if unable to query (defined by the query)
             * @throws ArithmeticException if numeric overflow occurs (defined by the query)
             */
            util::Optional<> query(TemporalQuery const& query) const override;

            /**
             * Calculates the amount of time until another date-time in terms of the specified unit.
             * <p>
             * This calculates the amount of time between two @c ZonedDateTime
             * objects in terms of a single @c ChronoUnit.
             * The start and end points are @c this and the specified date-time.
             * The result will be negative if the end is before the start.
             * For example, the amount in days between two date-times can be calculated
             * using @c startDateTime.until(endDateTime, DAYS).
             * </p>
             * <p>
             * The @c Temporal passed to this method is converted to a
             * @c ZonedDateTime using @b from(Temporal).
             * If the time-zone differs between the two zoned date-times, the specified
             * end date-time is normalized to have the same zone as this date-time.
             * </p>
             * <p>
             * The calculation returns a whole number, representing the number of
             * complete units between the two date-times.
             * For example, the amount in months between 2012-06-15T00:00Z and 2012-08-14T23:59Z
             * will only be one month as it is one minute short of two months.
             * </p>
             * <p>
             * The calculation is implemented in this method for @b ChronoUnit.
             * The units @c NANOS, @c MICROS, @c MILLIS, @c SECONDS,
             * @c MINUTES, @c HOURS and @c HALF_DAYS, @c DAYS,
             * @c WEEKS, @c MONTHS, @c YEARS, @c DECADES,
             * @c CENTURIES, @c MILLENNIA and @c ERAS are supported.
             * Other @c ChronoUnit values will throw an exception.
             * </p>
             * <p>
             * The calculation for date and time units differ.
             * </p>
             * <p>
             * Date units operate on the local time-line, using the local date-time.
             * For example, the period from noon on day 1 to noon the following day
             * in days will always be counted as exactly one day, irrespective of whether
             * there was a daylight savings change or not.
             * </p>
             * <p>
             * Time units operate on the instant time-line.
             * The calculation effectively converts both zoned date-times to instants
             * and then calculates the period between the instants.
             * For example, the period from noon on day 1 to noon the following day
             * in hours may be 23, 24 or 25 hours (or some other amount) depending on
             * whether there was a daylight savings change or not.
             * </p>
             * @param endExclusive  the end date, exclusive, which is converted to a @c ZonedDateTime
             * @param unit  the unit to measure the amount in
             * @return the amount of time between this date-time and the end date-time
             * @throws DateTimeException if the amount cannot be calculated, or the end
             *  temporal cannot be converted to a @c ZonedDateTime
             * @throws TemporalException if the unit is not supported
             * @throws ArithmeticException if numeric overflow occurs
             */
            glong until(Temporal const& endExclusive, ChronoUnit unit) const override;

            /**
             * Converts this date-time to the number of seconds from the epoch
             * of 1970-01-01T00:00:00Z.
             * <p>
             * This uses the <em> local date-time</em> and
             * @em offset to calculate the epoch-second value,
             * which is the number of elapsed seconds from 1970-01-01T00:00:00Z.
             * Instants on the time-line after the epoch are positive, earlier are negative.
             *
             * @return the number of seconds from the epoch of 1970-01-01T00:00:00Z
             */
            glong toEpochSecond() const;

            /**
             * Converts this date-time to an @c OffsetDateTime.
             * <p>
             * This creates an offset date-time using the local date-time and offset.
             * The zone ID is ignored.
             * </p>
             * @return an offset date-time representing the same local date-time and offset
             */
            OffsetDateTime toOffsetDateTime() const;

            /**
             * Compares this date-time to another date-time.
             * <p>
             * The comparison is based first on the instant, then on the local date-time,
             * then on the zone ID, then on the chronology.
             * It is "consistent with equals", as defined by @b Comparable.
             * </p>
             * <p>
             * If all the date-time objects being compared are in the same chronology, then the
             * additional chronology stage is not required.
             * </p>
             * <p>
             * This default implementation performs the comparison defined above.
             * </p>
             * @param other  the other date-time to compare to
             * @return the comparator value, that is the comparison of this with the @c other values for the instant,
             *          the local date-time, the zone ID, and the chronology, in order, returning the first non-zero result,
             *          and otherwise returning zero
             * @see isBefore
             * @see isAfter
             */
            gint compareTo(ZonedDateTime const& other) const override;

            /**
             * Checks if the instant of this date-time is after that of the specified date-time.
             * <p>
             * This method differs from the comparison in @b compareTo in that it
             * only compares the instant of the date-time. This is equivalent to using
             * @c dateTime1.toInstant().isAfter(dateTime2.toInstant());.
             * </p>
             * <p>
             * This default implementation performs the comparison based on the epoch-second
             * and nano-of-second.
             * </p>
             * @param other  the other date-time to compare to
             * @return true if this is after the specified date-time
             */
            gbool isAfter(ZonedDateTime const& other) const;

            /**
             * Checks if the instant of this date-time is before that of the specified date-time.
             * <p>
             * This method differs from the comparison in @b compareTo in that it
             * only compares the instant of the date-time. This is equivalent to using
             * @c dateTime1.toInstant().isBefore(dateTime2.toInstant());.
             * </p>
             * <p>
             * This default implementation performs the comparison based on the epoch-second
             * and nano-of-second.
             * </p>
             * @param other  the other date-time to compare to
             * @return true if this point is before the specified date-time
             */
            gbool isBefore(ZonedDateTime const& other) const;

            /**
             * Checks if this date-time is equal to another date-time.
             * <p>
             * The comparison is based on the offset date-time and the zone.
             * Only objects of type @c ZonedDateTime are compared, other types return false.
             * </p>
             * @param other  the object to check, null returns false
             * @return true if this is equal to the other date-time
             */
            gbool equals(Object const& other) const override;

            /**
             * A hash code for this date-time.
             *
             * @return a suitable hash code
             */
            gint hash() const override;

            /**
             * Outputs this date-time as a @c String, such as
             * @c 2007-12-03T10:15:30+01:00[Europe/Paris].
             * <p>
             * The format consists of the @c LocalDateTime followed by the @c ZoneOffset.
             * If the @c ZoneId is not the same as the offset, then the ID is output.
             * The output is compatible with ISO-8601 if the offset and ID are the same,
             * and the seconds in the offset are zero.
             * </p>
             * @return a string representation of this date-time
             */
            String toString() const override;

            /**
             * Return the shadow copy of this object.
             *
             * @return the shadow copy of this object
             */
            Object& clone() const override;

        private:

            /**
             * Obtains an instance of @c ZonedDateTime using seconds from the
             * epoch of 1970-01-01T00:00:00Z.
             *
             * @param epochSecond  the number of seconds from the epoch of 1970-01-01T00:00:00Z
             * @param nanoOfSecond  the nanosecond within the second, from 0 to 999,999,999
             * @param zone  the time-zone
             * @return the zoned date-time
             * @throws DateTimeException if the result exceeds the supported range
             */
            static ZonedDateTime create(glong epochSecond, gint nanoOfSecond, ZoneId const& zone);

            /**
             * Constructor.
             *
             * @param dateTime  the date-time, validated as not null
             * @param offset  the zone offset, validated as not null
             * @param zone  the time-zone, validated as not null
             */
            CORE_EXPLICIT ZonedDateTime(LocalDateTime const &dateTime, ZoneOffset const &offset, ZoneId const &zone);

            /**
             * Obtains an instance of @c ZonedDateTime from a local date-time
             * using the preferred offset if possible.
             * <p>
             * The local date-time is resolved to a single instant on the time-line.
             * This is achieved by finding a valid offset from UTC/Greenwich for the local
             * date-time as defined by the @b rules of the zone ID.
             * </p>
             * <p>
             * In most cases, there is only one valid offset for a local date-time.
             * In the case of an overlap, where clocks are set back, there are two valid offsets.
             * If the preferred offset is one of the valid offsets then it is used.
             * Otherwise the earlier valid offset is used, typically corresponding to "summer".
             * </p>
             * <p>
             * In the case of a gap, where clocks jump forward, there is no valid offset.
             * Instead, the local date-time is adjusted to be later by the length of the gap.
             * For a typical one-hour daylight savings change, the local date-time will be
             * moved one-hour later into the offset typically corresponding to "summer".
             * </p>
             * @param dateTime  the local date-time
             * @param zone  the time-zone
             * @return the zoned date-time
             */
            static ZonedDateTime ofLocal(LocalDateTime const& dateTime, ZoneId const& zone);

            /**
             * Resolves the offset into this zoned date-time for the with methods.
             * <p>
             * This typically ignores the offset, unless it can be used to switch offset in a DST overlap.
             *
             * @param offset  the offset
             * @return the zoned date-time
             */
            ZonedDateTime resolveOffset(ZoneOffset const &offset) const;

            /**
             * Resolves the new local date-time using this zone ID, retaining the offset if possible.
             *
             * @param newDateTime  the new local date-time, not null
             * @return the zoned date-time, not null
             */
            ZonedDateTime resolveLocal(LocalDateTime const &newDateTime) const;

            /**
             * Resolves the new local date-time using the offset to identify the instant.
             *
             * @param newDateTime  the new local date-time, not null
             * @return the zoned date-time, not null
             */
            ZonedDateTime resolveInstant(LocalDateTime const &newDateTime) const;
        };
    } // time
} // core

#endif //CORE24_ZONEDDATETIME_H
