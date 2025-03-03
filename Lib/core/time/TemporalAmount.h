//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_TEMPORALAMOUNT_H
#define CORE24_CORE_24_TEMPORALAMOUNT_H

#include <core/time/TemporalUnit.h>

namespace core {
  namespace time {
    /**
     * Framework-level interface defining an amount of time, such as
     * "6 hours", "8 days" or "2 years and 3 months".
     * <p>
     * This is the base interface type for amounts of time.
     * An amount is distinct from a date or time-of-day in that it is not tied
     * to any specific point on the time-line.
     * <p>
     * The amount can be thought of as a @c Map of @b TemporalUnit to
     * @c long, exposed via @b getUnits() and @b get(TemporalUnit).
     * A simple case might have a single unit-value pair, such as "6 hours".
     * A more complex case may have multiple unit-value pairs, such as
     * "7 years, 3 months and 5 days".
     * <p>
     * There are two common implementations.
     * @b Period is a date-based implementation, storing years, months and days.
     * @b Duration is a time-based implementation, storing seconds and nanoseconds,
     * but providing some access using other duration based units such as minutes,
     * hours and fixed 24-hour days.
     * <p>
     * This interface is a framework-level interface that should not be widely
     * used in application code. Instead, applications should create and pass
     * around instances of concrete types, such as @c Period and @c Duration.
     *
     * @implSpec
     * This interface places no restrictions on the mutability of implementations,
     * however immutability is strongly recommended.
     */
    class TemporalAmount : public virtual Object {
    public:
      CORE_ALIAS(Optional, util::Optional<>);
      CORE_ALIAS(OptionalTemporal, util::Optional<Temporal>);
      CORE_ALIAS(OptionalDate, util::Optional<LocalDate>);
      CORE_ALIAS(OptionalTime, util::Optional<LocalTime>);
      CORE_ALIAS(OptionalZone, util::Optional<ZoneId>);
      CORE_ALIAS(OptionalOffset, util::Optional<ZoneOffset>);
      CORE_ALIAS(OptionalDateTime, util::Optional<LocalDateTime>);
      CORE_ALIAS(OptionalOffsetTime, util::Optional<OffsetTime>);
      CORE_ALIAS(OptionalOffsetDateTime, util::Optional<OffsetDateTime>);
      CORE_ALIAS(UnitEnum, Enum<TemporalUnit>);
      CORE_ALIAS(UnitList, util::List<UnitEnum>);

      /**
       * Returns the value of the requested unit.
       * The units returned from @b getUnits() uniquely define the
       * value of the @c TemporalAmount.  A value must be returned
       * for each unit listed in @c getUnits.
       *
       * @implSpec
       * Implementations may declare support for units not listed by @b getUnits().
       * Typically, the implementation would define additional units
       * as conversions for the convenience of developers.
       *
       * @param unit the @c TemporalUnit for which to return the value
       * @return the long value of the unit
       * @throws DateTimeException if a value for the unit cannot be obtained
       * @throws TemporalException if the @c unit is not supported
       */
      virtual glong get(TemporalUnit unit) const = 0;

      /**
       * Returns the list of units uniquely defining the value of this TemporalAmount.
       * The list of {@code TemporalUnits} is defined by the implementation class.
       * The list is a snapshot of the units at the time @c getUnits
       * is called and is not mutable.
       * The units are ordered from longest duration to the shortest duration
       * of the unit.
       *
       * @implSpec
       * The list of units completely and uniquely represents the
       * state of the object without omissions, overlaps or duplication.
       * The units are in order from longest duration to shortest.
       *
       * @return the List of {@code TemporalUnits}; not null
       */
      virtual UnitList& getUnits() const = 0;

      /**
       * Adds to the specified temporal object.
       * <p>
       * Adds the amount to the specified temporal object using the logic
       * encapsulated in the implementing class.
       * <p>
       * There are two equivalent ways of using this method.
       * The first is to invoke this method directly.
       * The second is to use @b Temporal::plus(TemporalAmount):
       * <pre>
       *   // These two lines are equivalent, but the second approach is recommended
       *   dateTime = amount.addTo(dateTime);
       *   dateTime = dateTime.plus(adder);
       * </pre>
       * It is recommended to use the second approach, @c plus(TemporalAmount),
       * as it is a lot clearer to read in code.
       *
       * @implSpec
       * The implementation must take the input object and add to it.
       * The implementation defines the logic of the addition and is responsible for
       * documenting that logic. It may use any method on @c Temporal to
       * query the temporal object and perform the addition.
       * The returned object must have the same observable type as the input object
       * <p>
       * The input object must not be altered.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable temporal objects.
       * <p>
       * The input temporal object may be in a calendar system other than ISO.
       * Implementations may choose to document compatibility with other calendar systems,
       * or reject non-ISO temporal objects by <b> querying the chronology</b>.
       * <p>
       * This method may be called from multiple threads in parallel.
       * It must be thread-safe when invoked.
       *
       * @param temporal  the temporal object to add the amount to, not null
       * @return an object of the same observable type with the addition made, not null
       * @throws DateTimeException if unable to add
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Temporal& addTo(Temporal const& temporal) const = 0;

      /**
       * Subtracts this object from the specified temporal object.
       * <p>
       * Subtracts the amount from the specified temporal object using the logic
       * encapsulated in the implementing class.
       * <p>
       * There are two equivalent ways of using this method.
       * The first is to invoke this method directly.
       * The second is to use @b Temporal::minus(TemporalAmount):
       * <pre>
       *   // these two lines are equivalent, but the second approach is recommended
       *   dateTime = amount.subtractFrom(dateTime);
       *   dateTime = dateTime.minus(amount);
       * </pre>
       * It is recommended to use the second approach, @c minus(TemporalAmount),
       * as it is a lot clearer to read in code.
       *
       * @implSpec
       * The implementation must take the input object and subtract from it.
       * The implementation defines the logic of the subtraction and is responsible for
       * documenting that logic. It may use any method on @c Temporal to
       * query the temporal object and perform the subtraction.
       * The returned object must have the same observable type as the input object
       * <p>
       * The input object must not be altered.
       * Instead, an adjusted copy of the original must be returned.
       * This provides equivalent, safe behavior for immutable and mutable temporal objects.
       * <p>
       * The input temporal object may be in a calendar system other than ISO.
       * Implementations may choose to document compatibility with other calendar systems,
       * or reject non-ISO temporal objects by <b> querying the chronology</b>.
       * <p>
       * This method may be called from multiple threads in parallel.
       * It must be thread-safe when invoked.
       *
       * @param temporal  the temporal object to subtract the amount from, not null
       * @return an object of the same observable type with the subtraction made, not null
       * @throws DateTimeException if unable to subtract
       * @throws ArithmeticException if numeric overflow occurs
       */
      virtual Temporal& subtractFrom(Temporal const& temporal) const = 0;

    protected:
      static Temporal& addAmountTo(glong amountToAdd, TemporalUnit unit, Temporal const& temporal);

      static Temporal& subtractAmountFrom(glong amountToSubtract, TemporalUnit unit, Temporal const& temporal);
    };
  } // time
} // core

#endif //CORE24_CORE_24_TEMPORALAMOUNT_H
