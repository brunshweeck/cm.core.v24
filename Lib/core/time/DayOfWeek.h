//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_DAYOFWEEK_H
#define CORE24_CORE_24_DAYOFWEEK_H

#include <core/lang/String.h>

namespace core {
  namespace time {
    /**
     * A day-of-week, such as 'Tuesday'.
     * <p>
     * @c DayOfWeek is an enum representing the 7 days of the week -
     * Monday, Tuesday, Wednesday, Thursday, Friday, Saturday and Sunday.
     * </p>
     * <p>
     * In addition to the textual enum name, each day-of-week has an @c int value.
     * The @c int value follows the ISO-8601 standard, from 1 (Monday) to 7 (Sunday).
     * It is recommended that applications use the enum rather than the @c int value
     * to ensure code clarity.
     * </p>
     * <p>
     * This enum provides access to the localized textual form of the day-of-week.
     * Some locales also assign different numeric values to the days, declaring
     * Sunday to have the value 1, however this class provides no support for this.
     * See @b WeekFields for localized week-numbering.
     * </p>
     * <p>
     * This enum represents a common concept that is found in many calendar systems.
     * As such, this enum may be used by any calendar system that has the day-of-week
     * concept defined exactly equivalent to the ISO calendar system.
     * </p>
     * @note
     * This is an immutable and thread-safe enum.
     */
    enum class DayOfWeek {
      /**
       * The singleton instance for the day-of-week of Monday.
       * This has the numeric value of @c 1.
       */
      MONDAY = 1,
      /**
       * The singleton instance for the day-of-week of Tuesday.
       * This has the numeric value of @c 2.
       */
      TUESDAY,
      /**
       * The singleton instance for the day-of-week of Wednesday.
       * This has the numeric value of @c 3.
       */
      WEDNESDAY,
      /**
       * The singleton instance for the day-of-week of Thursday.
       * This has the numeric value of @c 4.
       */
      THURSDAY,
      /**
       * The singleton instance for the day-of-week of Friday.
       * This has the numeric value of @c 5.
       */
      FRIDAY,
      /**
       * The singleton instance for the day-of-week of Saturday.
       * This has the numeric value of @c 6.
       */
      SATURDAY,
      /**
       * The singleton instance for the day-of-week of Sunday.
       * This has the numeric value of @c 7.
       */
      SUNDAY,
    };
  } // time

  inline namespace literals {
    extern String operator+(String const& text, time::DayOfWeek dayOfWeek);

    extern String operator+(time::DayOfWeek dayOfWeek, String const& text);
  }
} // core

#endif //CORE24_CORE_24_DAYOFWEEK_H
