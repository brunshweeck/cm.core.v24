//
// Created by brunshweeck on 3 août 2024.
//

#include "Temporal.h"

#include <core/lang/UnsupportedOperationException.h>
#include <core/time/TemporalQuery.h>
#include <core/util/Optional.h>

#include "TemporalAdjuster.h"

namespace core {
  namespace time {
    gbool Temporal::isSupported(TemporalUnit unit) const {
      return false;
    }

    Temporal& Temporal::adjust(TemporalAdjuster const& adjuster) const {
      try {
        return adjuster.adjustInto(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
  } // time
} // core
