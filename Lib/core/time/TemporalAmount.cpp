//
// Created by brunshweeck on 12/02/25.
//

#include "TemporalAmount.h"

#include <core/time/Temporal.h>
#include <core/lang/Enum.h>
#include <core/lang/RuntimeException.h>
#include <core/util/List.h>

namespace core {
  namespace time {
    glong TemporalAmount::get(TemporalUnit unit) const {
      RuntimeException("Undefined operation"_Sl).throws($ftrace());
    }

    TemporalAmount::UnitList& TemporalAmount::getUnits() const {
      static UnitList& units = UnitList::emptyList();
      return units;
    }

    Temporal& TemporalAmount::addTo(Temporal const& temporal) const {
      RuntimeException("Undefined operation"_Sl).throws($ftrace());
    }

    Temporal& TemporalAmount::subtractFrom(Temporal const& temporal) const {
      RuntimeException("Undefined operation"_Sl).throws($ftrace());
    }

    Temporal& TemporalAmount::addAmountTo(glong amountToAdd, TemporalUnit unit, Temporal const& temporal) {
      try {
        return temporal.add(amountToAdd, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& TemporalAmount::subtractAmountFrom(glong amountToSubtract, TemporalUnit unit, Temporal const& temporal) {
      try {
        return temporal.subtract(amountToSubtract, unit);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
  } // time
} // core
