//
// Created by brunshweeck on 12/02/25.
//

#include "TemporalAccessor.h"

#include <core/time/TemporalException.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ValueRange.h>
#include <meta/time/TemporalUtils.h>

namespace core {
  namespace time {
    using namespace util;
    CORE_ALIAS(Fields, TemporalUtils::Fields);
    CORE_ALIAS(Units, TemporalUtils::Units);

    gbool TemporalAccessor::isSupported(TemporalField field) const {
      try {
        return Fields::isSupportedBy(*this, field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ValueRange TemporalAccessor::range(TemporalField field) const {
      try {
        return Fields::range(*this, field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint TemporalAccessor::get(TemporalField field) const {
      try {
        ValueRange r = range(field);
        if (r.isIntValue() == false)
          TemporalException("Value of field "_Sl +
            field + " is too large. use getLong() instead"_Sl).throws($ftrace());
        glong value = getLong(field);
        if (r.isValidValue(value) == false)
          DateTimeException("Invalid value for field "_Sl +
            field + " (valid values "_Sl + r + ": "_Sl + value).throws($ftrace());
        return (gint) value;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    glong TemporalAccessor::getLong(TemporalField field) const {
      try {
        return Fields::getFrom(*this, field);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    TemporalAccessor::Optional TemporalAccessor::query(const TemporalQuery& query) const {
      if (query == TemporalQuery::localDate() ||
        query == TemporalQuery::localTime() ||
        query == TemporalQuery::chronology() ||
        query == TemporalQuery::zone() ||
        query == TemporalQuery::zoneId() ||
        query == TemporalQuery::offset())
        return Optional::empty();
      try {
        return query.queryFrom(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
  } // time
} // core
