//
// Created by brunshweeck on 12/02/25.
//

#include "ValueRange.h"

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Unsafe.h>
#include <core/time/DateTimeException.h>

namespace core {
  using namespace util;

  namespace time {
    ValueRange::ValueRange(glong minSmallest, glong minLargest, glong maxSmallest, glong maxLargest)
      : minSmallest(minSmallest), minLargest(minLargest), maxSmallest(maxSmallest), maxLargest(maxLargest) {}

    ValueRange ValueRange::of(glong min, glong max) {
      if (min > max)
        IllegalArgumentException("Minimum value must be less than maximum value"_Sl).throws($ftrace());
      return ValueRange(min, min, max, max);
    }

    ValueRange ValueRange::of(glong min, glong maxSmallest, glong maxLargest) {
      if (min > maxSmallest)
        IllegalArgumentException("Minimum value must be less than smallest maximum value"_Sl).throws($ftrace());
      try {
        return ValueRange::of(min, min, maxSmallest, maxLargest);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ValueRange ValueRange::of(glong minSmallest, glong minLargest, glong maxSmallest, glong maxLargest) {
      if (minSmallest > minLargest) {
        IllegalArgumentException("Smallest minimum value must be less than largest minimum value"_Sl)
            .throws($ftrace());
      }
      if (maxSmallest > maxLargest) {
        IllegalArgumentException("Smallest maximum value must be less than largest maximum value"_Sl)
            .throws($ftrace());
      }
      if (minLargest > maxLargest) {
        IllegalArgumentException("Largest minimum value must be less than largest maximum value"_Sl)
            .throws($ftrace());
      }
      if (minSmallest > maxSmallest) {
        IllegalArgumentException("Smallest minimum value must be less than smallest maximum value"_Sl)
            .throws($ftrace());
      }
      return ValueRange(minSmallest, minLargest, maxSmallest, maxLargest);
    }

    gbool ValueRange::isFixed() const {
      return minSmallest == minLargest && maxSmallest == maxLargest;
    }

    glong ValueRange::minimum() const {
      return minSmallest;
    }

    glong ValueRange::largestMinimum() const {
      return minLargest;
    }

    glong ValueRange::smallestMaximum() const {
      return maxSmallest;
    }

    glong ValueRange::maximum() const {
      return maxLargest;
    }

    gbool ValueRange::isIntValue() const {
      return minimum() >= Integer::MIN_VALUE && maximum() <= Integer::MAX_VALUE;
    }

    gbool ValueRange::isValidValue(glong value) const {
      return value >= minimum() && value <= maximum();
    }

    gbool ValueRange::isValidIntValue(glong value) const {
      return isIntValue() && isValidValue(value);
    }

    glong ValueRange::checkValidValue(glong value, TemporalField field) const {
      if (isValidValue(value) == false) {
        DateTimeException("Invalid value for "_Sl + field + " (valid values "_Sl + *this + "): "_Sl + value)
            .throws($ftrace());
      }
      return value;
    }

    gint ValueRange::checkValidIntValue(glong value, TemporalField field) const {
      if (isValidIntValue(value) == false) {
        DateTimeException("Invalid value for "_Sl + field + " (valid values "_Sl + *this + "): "_Sl + value)
            .throws($ftrace());
      }
      return (gint) value;
    }

    gbool ValueRange::equals(const Object& other) const {
      if (this == &other)
        return true;
      if (!Class<ValueRange>::hasInstance(other))
        return false;
      ValueRange const& that = CORE_XCAST(ValueRange const, other);
      return minSmallest == that.minSmallest
          && minLargest == that.minLargest
          && maxSmallest == that.maxSmallest
          && maxLargest == that.maxLargest;
    }

    gint ValueRange::hash() const {
      glong hash = minSmallest + (minLargest << 16) + (minLargest >> 48) +
          (maxSmallest << 32) + (maxSmallest >> 32) + (maxLargest << 48) +
          (maxLargest >> 16);
      return Long::hash(hash);
    }

    String ValueRange::toString() const {
      XString buf;
      buf.append(minSmallest);
      if (minSmallest != minLargest) {
        buf.append('/').append(minLargest);
      }
      buf.append(" - ").append(maxSmallest);
      if (maxSmallest != maxLargest) {
        buf.append('/').append(maxLargest);
      }
      return buf.toString();
    }

    Object& ValueRange::clone() const {
      return UNSAFE::newInstance<ValueRange>(*this);
    }
  } // time
} // core
