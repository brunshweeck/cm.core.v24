//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_MONTH_H
#define CORE24_CORE_24_MONTH_H

#include <core/lang/String.h>

namespace core {
  namespace time {
    /**
     * A month-of-year, such as 'July'.
     * <p>
     * @c Month is an enum representing the 12 months of the year -
     * January, February, March, April, May, June, July, August, September, October,
     * November and December.
     * </p>
     * <p>
     * In addition to the textual enum name, each month-of-year has an @c int value.
     * The @c int value follows normal usage and the ISO-8601 standard,
     * from 1 (January) to 12 (December). It is recommended that applications use the enum
     * rather than the @c int value to ensure code clarity.
     * </p>
     * <p>
     * This enum represents a common concept that is found in many calendar systems.
     * As such, this enum may be used by any calendar system that has the month-of-year
     * concept defined exactly equivalent to the ISO-8601 calendar system.
     * </p>
     * @note
     * This is an immutable and thread-safe enum.
     */
    enum class Month {
      /**
       * The singleton instance for the month of January with 31 days.
       * This has the numeric value of @c 1.
       */
      JANUARY = 1,
      /**
       * The singleton instance for the month of February with 28 days, or 29 in a leap year.
       * This has the numeric value of @c 2.
       */
      FEBRUARY,
      /**
       * The singleton instance for the month of March with 31 days.
       * This has the numeric value of @c 3.
       */
      MARCH,
      /**
       * The singleton instance for the month of April with 30 days.
       * This has the numeric value of @c 4.
       */
      APRIL,
      /**
       * The singleton instance for the month of May with 31 days.
       * This has the numeric value of @c 5.
       */
      MAY,
      /**
       * The singleton instance for the month of June with 30 days.
       * This has the numeric value of @c 6.
       */
      JUNE,
      /**
       * The singleton instance for the month of July with 31 days.
       * This has the numeric value of @c 7.
       */
      JULY,
      /**
       * The singleton instance for the month of August with 31 days.
       * This has the numeric value of @c 8.
       */
      AUGUST,
      /**
       * The singleton instance for the month of September with 30 days.
       * This has the numeric value of @c 9.
       */
      SEPTEMBER,
      /**
       * The singleton instance for the month of October with 31 days.
       * This has the numeric value of @c 10.
       */
      OCTOBER,
      /**
       * The singleton instance for the month of November with 30 days.
       * This has the numeric value of @c 11.
       */
      NOVEMBER,
      /**
       * The singleton instance for the month of December with 31 days.
       * This has the numeric value of @c 12.
       */
      DECEMBER,
    };
  } // time

  inline namespace literals {
    extern String operator+(String const& text, time::Month month);

    extern String operator+(time::Month month, String const& text);
  }
} // core

#endif //CORE24_CORE_24_MONTH_H
