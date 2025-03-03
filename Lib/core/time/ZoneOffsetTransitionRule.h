//
// Created by admin on 29/12/24.
//

#ifndef CORE24_ZONEOFFSETTRANSITIONRULE_H
#define CORE24_ZONEOFFSETTRANSITIONRULE_H

#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>

namespace core {
  namespace time {
    /**
     * A rule expressing how to create a transition.
     * <p>
     * This class allows rules for identifying future transitions to be expressed.
     * A rule might be written in many forms:
     *
     * @li the 16th March
     * @li the Sunday on or after the 16th March
     * @li the Sunday on or before the 16th March
     * @li the last Sunday in February
     *
     * These different rule types can be expressed and queried.
     * </p>
     * @note
     * This class is immutable and thread-safe.
     */
    class ZoneOffsetTransitionRule final : public Object {
      CORE_ADD_AS_FRIEND(ZoneOffsetTransitionRuleArray);
    public:
      enum class TimeDefinition;

    private:
      /**
       * The month of the month-day of the first day of the cutover week.
       * The actual date will be adjusted by the dowChange field.
       */
      Month month_;
      /**
       * The day-of-month of the month-day of the cutover week.
       * If positive, it is the start of the week where the cutover can occur.
       * If negative, it represents the end of the week where cutover can occur.
       * The value is the number of days from the end of the month, such that
       * @c -1 is the last day of the month, @c -2 is the second
       * to last day, and so on.
       */
      gint dom;
      /**
       * The cutover day-of-week, null to retain the day-of-month.
       */
      DayOfWeek dow;
      /**
       * The cutover time in the 'before' offset.
       */
      LocalTime time;
      /**
       * Whether the cutover time is midnight at the end of day.
       */
      gbool timeEndOfDay;
      /**
       * The definition of how the local time should be interpreted.
       */
      TimeDefinition timeDefinition_;

      /**
       * The standard offset at the cutover.
       */
      ZoneOffset standardOffset_;
      /**
       * The offset before the cutover.
       */
      ZoneOffset offsetBefore_;
      /**
       * The offset after the cutover.
       */
      ZoneOffset offsetAfter_;

      /**
       * Creates an instance defining the yearly rule to create transitions between two offsets.
       *
       * @param month  the month of the month-day of the first day of the cutover week, not null
       * @param dayOfMonthIndicator  the day of the month-day of the cutover week, positive if the week is that
       *  day or later, negative if the week is that day or earlier, counting from the last day of the month,
       *  from -28 to 31 excluding 0
       * @param dayOfWeek  the required day-of-week, null if the month-day should not be changed
       * @param time  the cutover time in the 'before' offset, not null
       * @param timeEndOfDay  whether the time is midnight at the end of day
       * @param timeDefinition  how to interpret the cutover
       * @param standardOffset  the standard offset in force at the cutover, not null
       * @param offsetBefore  the offset before the cutover, not null
       * @param offsetAfter  the offset after the cutover, not null
       * @throws IllegalArgumentException if the day of month indicator is invalid
       * @throws IllegalArgumentException if the end of day flag is true when the time is not midnight
       */
      ZoneOffsetTransitionRule(
        Month month,
        gint dayOfMonthIndicator,
        DayOfWeek dayOfWeek,
        const LocalTime& time,
        gbool timeEndOfDay,
        TimeDefinition timeDefinition,
        const ZoneOffset& standardOffset,
        const ZoneOffset& offsetBefore,
        const ZoneOffset& offsetAfter);

    public:
      /**
       * Obtains an instance defining the yearly rule to create transitions between two offsets.
       * <p>
       * Applications should normally obtain an instance from {@link ZoneRules}.
       * This factory is only intended for use when creating {@link ZoneRules}.
       * </p>
       * @param month  the month of the month-day of the first day of the cutover week
       * @param dayOfMonthIndicator  the day of the month-day of the cutover week, positive if the week is that
       *  day or later, negative if the week is that day or earlier, counting from the last day of the month,
       *  from -28 to 31 excluding 0
       * @param dayOfWeek  the required day-of-week, null if the month-day should not be changed
       * @param time  the cutover time in the 'before' offset
       * @param timeEndOfDay  whether the time is midnight at the end of day
       * @param timeDefinition  how to interpret the cutover
       * @param standardOffset  the standard offset in force at the cutover
       * @param offsetBefore  the offset before the cutover
       * @param offsetAfter  the offset after the cutover
       * @return the rule
       * @throws IllegalArgumentException if the day of month indicator is invalid
       * @throws IllegalArgumentException if the end of day flag is true when the time is not midnight
       * @throws IllegalArgumentException if @c time.nano() returns non-zero value
       */
      static ZoneOffsetTransitionRule of(Month month,
                                         gint dayOfMonthIndicator,
                                         DayOfWeek dayOfWeek,
                                         LocalTime const& time,
                                         gbool timeEndOfDay,
                                         TimeDefinition timeDefinition,
                                         ZoneOffset const& standardOffset,
                                         ZoneOffset const& offsetBefore,
                                         ZoneOffset const& offsetAfter);

      /**
       * Gets the month of the transition.
       * <p>
       * If the rule defines an exact date then the month is the month of that date.
       * </p>
       * <p>
       * If the rule defines a week where the transition might occur, then the month
       * if the month of either the earliest or latest possible date of the cutover.
       * </p>
       * @return the month of the transition
       */
      Month month() const;

      /**
       * Gets the indicator of the day-of-month of the transition.
       * <p>
       * If the rule defines an exact date then the day is the month of that date.
       * </p>
       * <p>
       * If the rule defines a week where the transition might occur, then the day
       * defines either the start of the end of the transition week.
       * </p>
       * <p>
       * If the value is positive, then it represents a normal day-of-month, and is the
       * earliest possible date that the transition can be.
       * The date may refer to 29th February which should be treated as 1st March in non-leap years.
       * </p>
       * <p>
       * If the value is negative, then it represents the number of days back from the
       * end of the month where @c -1 is the last day of the month.
       * In this case, the day identified is the latest possible date that the transition can be.
       * </p>
       * @return the day-of-month indicator, from -28 to 31 excluding 0
       */
      gint dayOfMonthIndicator() const;

      /**
       * Gets the day-of-week of the transition.
       * <p>
       * If the rule defines an exact date then this returns null.
       * </p>
       * <p>
       * If the rule defines a week where the cutover might occur, then this method
       * returns the day-of-week that the month-day will be adjusted to.
       * If the day is positive then the adjustment is later.
       * If the day is negative then the adjustment is earlier.
       * </p>
       * @return the day-of-week that the transition occurs, null if the rule defines an exact date
       */
      DayOfWeek dayOfWeek() const;

      /**
       * Gets the local time of day of the transition which must be checked with
       * @b isMidnightEndOfDay().
       * <p>
       * The time is converted into an instant using the time definition.
       * </p>
       * @return the local time of day of the transition
       */
      LocalTime localTime() const;

      /**
       * Is the transition local time midnight at the end of day.
       * <p>
       * The transition may be represented as occurring at 24:00.
       * </p>
       * @return whether a local time of midnight is at the start or end of the day
       */
      gbool isMidnightEndOfDay() const;

      /**
       * Gets the time definition, specifying how to convert the time to an instant.
       * <p>
       * The local time can be converted to an instant using the standard offset,
       * the wall offset or UTC.
       * </p>
       * @return the time definition
       */
      TimeDefinition timeDefinition() const;

      /**
       * Gets the standard offset in force at the transition.
       *
       * @return the standard offset
       */
      ZoneOffset standardOffset() const;

      /**
       * Gets the offset before the transition.
       *
       * @return the offset before
       */
      ZoneOffset offsetBefore() const;

      /**
       * Gets the offset after the transition.
       *
       * @return the offset after
       */
      ZoneOffset offsetAfter() const;

      /**
       * Creates a transition instance for the specified year.
       * <p>
       * Calculations are performed using the ISO-8601 chronology.
       * </p>
       * @param year  the year to create a transition for
       * @return the transition instance
       */
      ZoneOffsetTransition createTransition(gint year) const;

      /**
       * Checks if this object equals another.
       * <p>
       * The entire state of the object is compared.
       * </p>
       * @param otherRule  the other object to compare to, null returns false
       * @return true if equal
       */
      gbool equals(Object const& otherRule) const override;

      /**
       * Returns a suitable hash code.
       *
       * @return the hash code
       */
      gint hash() const override;

      /**
       * Returns a string describing this object.
       *
       * @return a string for debugging
       */
      String toString() const override;

      /**
       * Returns a shadow copy of this object.
       *
       * @return a shadow copy for debugging
       */
      Object& clone() const override;

      /**
       * A definition of the way a local time can be converted to the actual
       * transition date-time.
       * <p>
       * Time zone rules are expressed in one of three ways:
       *
       * @li Relative to UTC
       * @li Relative to the standard offset in force
       * @li Relative to the wall offset (what you would see on a clock on the wall)
       * </p>
       */
      enum class TimeDefinition {
        /**
         * The local date-time is expressed in terms of the UTC offset.
         */
        UTC,

        /**
         * The local date-time is expressed in terms of the wall offset.
         */
        WALL,

        /**
         * The local date-time is expressed in terms of the standard offset.
         */
        STANDARD,
      };

    private:
      /**
       * Converts the specified local date-time to the local date-time actually
       * seen on a wall clock.
       * <p>
       * This method converts using the type of this enum.
       * The output is defined relative to the 'before' offset of the transition.
       * </p>
       * <p>
       * The UTC type uses the UTC offset.
       * The STANDARD type uses the standard offset.
       * The WALL type returns the input date-time.
       * The result is intended for use with the wall-offset.
       * </p>
       * @param dateTime  the local date-time
       * @param standardOffset  the standard offset
       * @param wallOffset  the wall offset
       * @return the date-time relative to the wall/before offset
       */
      static LocalDateTime createDateTime(TimeDefinition timeDefinition,
                                          LocalDateTime const& dateTime,
                                          ZoneOffset const& standardOffset,
                                          ZoneOffset const& wallOffset);
    };
  } // time

  inline namespace literals {
    extern String operator+(String const& text, time::ZoneOffsetTransitionRule::TimeDefinition timeDefinition);

    extern String operator+(time::ZoneOffsetTransitionRule::TimeDefinition timeDefinition, String const& text);
  }
} // core

#endif //CORE24_ZONEOFFSETTRANSITIONRULE_H
