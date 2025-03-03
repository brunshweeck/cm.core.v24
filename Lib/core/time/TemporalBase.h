//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_TEMPORALBASE_H
#define CORE24_CORE_24_TEMPORALBASE_H

#include <core/time/Temporal.h>
#include <core/time/TemporalAmount.h>
#include <core/time/TemporalException.h>

namespace core {
  namespace time {
    template <class T>
    class TemporalBase : public virtual Temporal {
    public:
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
      virtual T with(TemporalAdjuster const& adjuster) const = 0;

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
       * @implSpec
       * Implementations must check and handle all fields defined in @b TemporalField.
       * If the field is supported, then the adjustment must be performed.
       * If unsupported, then an @c TemporalException must be thrown.
       * <p>
       * Implementations must not alter this object.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable implementations.
       * </p>
       * @param field  the field to set in the result, not null
       * @param newValue  the new value of the field in the result
       * @return an object of the same type with the specified field set, not null
       * @throws DateTimeException if the field cannot be set
       * @throws TemporalException if the field is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual T with(TemporalField field, glong newValue) const = 0;

      /**
       * Returns an object of the same type as this object with an amount added.
       * <p>
       * This adjusts this temporal, adding according to the rules of the specified amount.
       * The amount is typically a @b Period but may be any other type implementing
       * the @b TemporalAmount interface, such as @b Duration.
       * <p>
       * Some example code indicating how and why this method is used:
       * <pre>
       *  date = date.plus(period);                // add a Period instance
       *  date = date.plus(duration);              // add a Duration instance
       *  date = date.plus(workingDays(6));        // example user-written workingDays method
       * </pre>
       * <p>
       * Note that calling @c plus followed by @c minus is not guaranteed to
       * return the same date-time.
       *
       * @implSpec
       * <p>
       * Implementations must not alter either this object or the specified temporal object.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable implementations.
       * <p>
       * The default implementation must behave equivalent to this code:
       * <pre>
       *  return amount.addTo(this);
       * </pre>
       *
       * @param amount  the amount to add, not null
       * @return an object of the same type with the specified adjustment made, not null
       * @throws DateTimeException if the addition cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual T plus(TemporalAmount const& amount) const = 0;

      /**
       * Returns an object of the same type as this object with the specified period added.
       * <p>
       * This method returns a new object based on this one with the specified period added.
       * For example, on a @c LocalDate, this could be used to add a number of years, months or days.
       * The returned object will have the same observable type as this object.
       * <p>
       * In some cases, changing a field is not fully defined. For example, if the target object is
       * a date representing the 31st January, then adding one month would be unclear.
       * In cases like this, the field is responsible for resolving the result. Typically, it will choose
       * the previous valid date, which would be the last valid day of February in this example.
       *
       * @implSpec
       * Implementations must check and handle all units defined in @b TemporalUnit.
       * If the unit is supported, then the addition must be performed.
       * If unsupported, then an @c TemporalException must be thrown.
       * <p>
       * Implementations must not alter this object.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable implementations.
       * </p>
       * @param amountToAdd  the amount of the specified unit to add, may be negative
       * @param unit  the unit of the amount to add, not null
       * @return an object of the same type with the specified period added, not null
       * @throws DateTimeException if the unit cannot be added
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual T plus(glong amountToAdd, TemporalUnit unit) const = 0;

      /**
       * Returns an object of the same type as this object with an amount subtracted.
       * <p>
       * This adjusts this temporal, subtracting according to the rules of the specified amount.
       * The amount is typically a @b Period but may be any other type implementing
       * the @b TemporalAmount interface, such as @b Duration.
       * <p>
       * Some example code indicating how and why this method is used:
       * <pre>
       *  date = date.minus(period);               // subtract a Period instance
       *  date = date.minus(duration);             // subtract a Duration instance
       *  date = date.minus(workingDays(6));       // example user-written workingDays method
       * </pre>
       * <p>
       * Note that calling @c plus followed by @c minus is not guaranteed to
       * return the same date-time.
       *
       * @implSpec
       * <p>
       * Implementations must not alter either this object or the specified temporal object.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable implementations.
       * <p>
       * The default implementation must behave equivalent to this code:
       * <pre>
       *  return amount.subtractFrom(this);
       * </pre>
       *
       * @param amount  the amount to subtract, not null
       * @return an object of the same type with the specified adjustment made, not null
       * @throws DateTimeException if the subtraction cannot be made
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual T minus(TemporalAmount const& amount) const = 0;

      /**
       * Returns an object of the same type as this object with the specified period subtracted.
       * <p>
       * This method returns a new object based on this one with the specified period subtracted.
       * For example, on a @c LocalDate, this could be used to subtract a number of years, months or days.
       * The returned object will have the same observable type as this object.
       * <p>
       * In some cases, changing a field is not fully defined. For example, if the target object is
       * a date representing the 31st March, then subtracting one month would be unclear.
       * In cases like this, the field is responsible for resolving the result. Typically, it will choose
       * the previous valid date, which would be the last valid day of February in this example.
       *
       * @implSpec
       * Implementations must behave in a manor equivalent to the default method behavior.
       * <p>
       * Implementations must not alter this object.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable implementations.
       * <p>
       * The default implementation must behave equivalent to this code:
       * <pre>
       *  return (amountToSubtract == Long.MIN_VALUE ?
       *      plus(Long.MAX_VALUE, unit).plus(1, unit) : plus(-amountToSubtract, unit));
       * </pre>
       *
       * @param amountToSubtract  the amount of the specified unit to subtract, may be negative
       * @param unit  the unit of the amount to subtract, not null
       * @return an object of the same type with the specified period subtracted, not null
       * @throws DateTimeException if the unit cannot be subtracted
       * @throws TemporalException if the unit is not supported
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual T minus(glong amountToSubtract, TemporalUnit unit) const = 0;

    private:
      Temporal& add(glong amountToAdd, TemporalUnit unit) const final {
        CORE_FAST_ASSERT(Class<Temporal>::isSuper<T>());
        try {
          T result = plus(amountToAdd, unit);
          return CORE_XCAST(Temporal, result.clone());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      Temporal& subtract(glong amountToSubtract, TemporalUnit unit) const final {
        CORE_FAST_ASSERT(Class<Temporal>::isSuper<T>());
        try {
          T result = minus(amountToSubtract, unit);
          return CORE_XCAST(Temporal, result.clone());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      Temporal& adjust(TemporalField field, glong newValue) const final {
        CORE_FAST_ASSERT(Class<Temporal>::isSuper<T>());
        try {
          T result = with(field, newValue);
          return CORE_XCAST(Temporal, result.clone());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      Temporal& adjust(TemporalAdjuster const& adjuster) const final {
        CORE_FAST_ASSERT(Class<Temporal>::isSuper<T>());
        try {
          T result = with(adjuster);
          return CORE_XCAST(Temporal, result.clone());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
    };
  } // time
} // core

#endif //CORE24_CORE_24_TEMPORALBASE_H
