//
// Created by brunshweeck on 12/02/25.
//

#ifndef CORE24_CORE_24_FORMATSTYLE_H
#define CORE24_CORE_24_FORMATSTYLE_H
#include <core/lang/String.h>

namespace core {
  namespace time {
    /**
     * Enumeration of the style of a localized date, time or date-time formatter.
     * <p>
     * These styles are used when obtaining a date-time style from configuration.
     * See @em DateTimeFormatter for usage.
     * </p>
     */
    enum class FormatStyle {
      // ordered from large to small

      /**
       * Full text style, with the most detail.
       * For example, the format might be <em>'Saturday, April 12, 1952 AD'</em> or <em>'3:30:42pm PST'</em>.
       */
      FULL,

      /**
       * Long text style, with lots of detail.
       * For example, the format might be 'January 12, 1952'.
       */
      LONG,

      /**
       * Medium text style, with some detail.
       * For example, the format might be 'Jan 12, 1952'.
       */
      MEDIUM,

      /**
       * Short text style, typically numeric.
       * For example, the format might be '12.13.52' or '3:30pm'.
       */
      SHORT
    };
  } // time

  inline namespace literals {
    extern String operator+(String const& text, time::FormatStyle style);

    extern String operator+(time::FormatStyle style, String const& text);
  }
} // core

#endif //CORE24_CORE_24_FORMATSTYLE_H
