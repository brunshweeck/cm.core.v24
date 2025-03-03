//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_TEMPORAL_H
#define CORE24_TEMPORAL_H

#include <core/time/TemporalAccessor.h>
#include <core/time/TemporalAdjuster.h>

namespace core {
  namespace time {
    /**
     * Framework-level interface defining read-write access to a temporal object,
     * such as a date, time, offset or some combination of these.
     * <p>
     * This is the base interface type for date, time and offset objects that
     * are complete enough to be manipulated using plus and minus.
     * It is implemented by those classes that can provide and manipulate information
     * as @em fields.
     * </p>
     * <p>
     * Most date and time information can be represented as a number.
     * These are modeled using @c TemporalField with the number held using
     * a @c glong to handle large values. Year, month and day-of-month are
     * simple examples of fields, but they also include instant and offsets.
     * See @c TemporalField for the standard set of fields.
     * </p>
     * <p>
     * Two pieces of date/time information cannot be represented by numbers,
     * the @em chronology and the @em time-zone.
     * </p>
     * <p>
     * This interface is a framework-level interface that should not be widely
     * used in application code. Instead, applications should create and pass
     * around instances of concrete types, such as @c LocalDate.
     * There are many reasons for this, part of which is that implementations
     * of this interface may be in calendar systems other than ISO.
     * </p>
     * <h2>When to implement</h2>
     * <p>
     * A class should implement this interface if it meets three criteria:
     * - it provides access to date/time/offset information
     * - the set of fields are contiguous from the largest to the smallest
     * - the set of fields are complete, such that no other field is needed to define the
     *  valid range of values for the fields that are represented
     * </p>
     * <p>
     * Two examples make this clear:
     * - @c LocalDate implements this interface as it represents a set of fields
     *  that are contiguous from days to forever and require no external information to determine
     *  the validity of each date. It is therefore able to implement plus/minus correctly.
     * - @c LocalTime implements this interface as it represents a set of fields
     *  that are contiguous from nanos to within days and require no external information to determine
     *  validity. It is able to implement plus/minus correctly, by wrapping around the day.
     * </p>
     * @note
     * This interface places no restrictions on the mutability of implementations,
     * however immutability is strongly recommended.
     * All implementations must be @c Comparable.
     *
     * @warning It is not recommended to extends directly this class,
     *              extends TemporalBase instead.
     */
    class Temporal : public virtual TemporalAccessor {
    public:
      /**
       * Checks if the specified unit is supported.
       * <p>
       * This checks if the specified unit can be added to, or subtracted from, this date-time.
       * If false, then calling the @em plus and @em minus methods will throw an exception.
       *
       * @param unit  the unit to check
       * @return true if the unit can be added/subtracted, false if not
       */
      virtual gbool isSupported(TemporalUnit unit) const = 0;

      CORE_IMPORT_FIELD_OR_METHOD(TemporalAccessor, isSupported);

      /**
       * Calculates the amount of time until another temporal in terms of the specified unit.
       * <p>
       * This calculates the amount of time between two temporal objects
       * in terms of a single @c TemporalUnit.
       * The start and end points are @c this and the specified temporal.
       * The end point is converted to be of the same type as the start point if different.
       * The result will be negative if the end is before the start.
       * For example, the amount in hours between two temporal objects can be
       * calculated using @c startTime.until(endTime,HOURS).
       * </p>
       * <p>
       * The calculation returns a whole number, representing the number of
       * complete units between the two temporal.
       * For example, the amount in hours between the times 11:30 and 13:29
       * will only be one hour as it is one minute short of two hours.
       * </p>
       *
       * @note
       * Implementations must begin by checking to ensure that the input temporal
       * object is of the same observable type as the implementation.
       * They must then perform the calculation for all instances of @em TemporalUnit.
       * An @c TemporalException must be thrown for @c TemporalUnit
       * instances that are unsupported.
       * <p>
       * Implementations must ensure that no observable state is altered when this
       * read-only method is invoked.
       * </p>
       * @param endExclusive  the end temporal, exclusive, converted to be of the
       *  same type as this object, not null
       * @param unit  the unit to measure the amount in, not null
       * @return the amount of time between this temporal object and the specified one
       *  in terms of the unit; positive if the specified object is later than this one,
       *  negative if it is earlier than this one
       * @throws DateTimeException if the amount cannot be calculated, or the end
       *  temporal cannot be converted to the same type as this temporal
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual glong until(Temporal const& endExclusive, TemporalUnit unit) const = 0;

    private:
      /**
       * Returns an object of the same type as this object with the specified period added.
       * <p>
       * This method returns a new object based on this one with the specified period added.
       * For example, on a @c LocalDate, this could be used to add a number of years, months or days.
       * The returned object will have the same observable type as this object.
       * </p>
       * <p>
       * In some cases, changing a field is not fully defined. For example, if the target object is
       * a date representing the 31st January, then adding one month would be unclear.
       * In cases like this, the field is responsible for resolving the result. Typically, it will choose
       * the previous valid date, which would be the last valid day of February in this example.
       * </p>
       *
       * @param amountToAdd  the amount of the specified unit to add, may be negative
       * @param unit  the unit of the amount to add, not null
       * @return an object of the same type with the specified period added, not null
       * @throws DateTimeException if the unit cannot be added
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Temporal& add(glong amountToAdd, TemporalUnit unit) const = 0;

      /**
       * Returns an object of the same type as this object with the specified period subtracted.
       * <p>
       * This method returns a new object based on this one with the specified period subtracted.
       * For example, on a @c LocalDate, this could be used to subtract a number of years, months or days.
       * The returned object will have the same observable type as this object.
       * </p>
       * <p>
       * In some cases, changing a field is not fully defined. For example, if the target object is
       * a date representing the 31st March, then subtracting one month would be unclear.
       * In cases like this, the field is responsible for resolving the result. Typically, it will choose
       * the previous valid date, which would be the last valid day of February in this example.
       * </p>
       * @param amountToSubtract  the amount of the specified unit to subtract, may be negative
       * @param unit  the unit of the amount to subtract, not null
       * @return an object of the same type with the specified period subtracted, not null
       * @throws DateTimeException if the unit cannot be subtracted
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Temporal& subtract(glong amountToSubtract, TemporalUnit unit) const = 0;

      /**
       * Returns an object of the same type as this object with the specified field altered.
       * <p>
       * This returns a new object based on this one with the value for the specified field changed.
       * For example, on a @c LocalDate, this could be used to set the year, month or day-of-month.
       * The returned object will have the same observable type as this object.
       * <p>
       * In some cases, changing a field is not fully defined. For example, if the target object is
       * a date representing the 31st January, then changing the month to February would be unclear.
       * In cases like this, the field is responsible for resolving the result. Typically, it will choose
       * the previous valid date, which would be the last valid day of February in this example.
       *
       * @param field  the field to set in the result, not null
       * @param newValue  the new value of the field in the result
       * @return an object of the same type with the specified field set, not null
       * @throws DateTimeException if the field cannot be set
       * @throws UnsupportedTemporalTypeException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Temporal& adjust(TemporalField field, glong newValue) const = 0;

      /**
       * Returns an adjusted object of the same type as this object with the adjustment made.
       * <p>
       * This adjusts this date-time according to the rules of the specified adjuster.
       * A simple adjuster might simply set the one of the fields, such as the year field.
       * A more complex adjuster might set the date to the last day of the month.
       * A selection of common adjustments is provided in @b TemporalAdjuster.
       * These include finding the "last day of the month" and "next Wednesday".
       * The adjuster is responsible for handling special cases, such as the varying
       * lengths of month and leap years.
       * <p>
       * Some example code indicating how and why this method is used:
       * @code
       *  date = date.with(TemporalAdjuster::lastDayOfMonth());
       *  date = date.with(TemporalAdjuster::next(WEDNESDAY));
       * @endcode
       *
       * @note
       * <p>
       * Implementations must not alter either this object or the specified temporal object.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable implementations.
       * </p>
       * <p>
       * The default implementation must behave equivalent to this code:
       * @code
       *  return adjuster.adjustInto(this);
       * @endcode
       * </p>
       *
       * @param adjuster  the adjuster to use, not null
       * @return an object of the same type with the specified adjustment made, not null
       * @throws DateTimeException if unable to make the adjustment
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Temporal& adjust(TemporalAdjuster const& adjuster) const = 0;


      CORE_ADD_AS_FRIEND(TemporalAmount);
      CORE_ADD_AS_FRIEND(TemporalAdjuster);
    };
  } // time
} // core

#endif //CORE24_TEMPORAL_H
