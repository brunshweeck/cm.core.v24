//
// Created by brunshweeck on 12/02/25.
//

#include "TemporalUnit.h"
#include <core/lang/Enum.h>

namespace core {
  namespace time {
#define LABEL(CLASS, NAME) setDefaultLabel(#NAME ## _Sl, CLASS::NAME)
#define ENUM_CLASS(CLASS, ENUM) \
    class CLASS ## _ ## ENUM ## Enum final: public Enum<ENUM> { \
    public:\
        CLASS ## _ ## ENUM ## Enum (String const &label, ENUM value): Enum/*<ENUM>*/(label, (gint)value) {}\
        static gbool init();\
    };\
    static gint $_ ## CLASS ## _ ## ENUM ## Enum = CLASS ## _ ## ENUM ## Enum::init(); \
    gbool CLASS ## _ ## ENUM ## Enum::init()


    ENUM_CLASS(Enum, TemporalUnit) {
      Enum e = (TemporalUnit) 0;
      static Object& table = labels(null);

      LABEL(TemporalUnit, NANOS);
      LABEL(TemporalUnit, MICROS);
      LABEL(TemporalUnit, MILLIS);
      LABEL(TemporalUnit, SECONDS);
      LABEL(TemporalUnit, MINUTES);
      LABEL(TemporalUnit, HOURS);
      LABEL(TemporalUnit, HALF_DAYS);
      LABEL(TemporalUnit, DAYS);
      LABEL(TemporalUnit, WEEKS);
      LABEL(TemporalUnit, MONTHS);
      LABEL(TemporalUnit, YEARS);
      LABEL(TemporalUnit, DECADES);
      LABEL(TemporalUnit, CENTURIES);
      LABEL(TemporalUnit, MILLENNIA);
      LABEL(TemporalUnit, ERAS);
      LABEL(TemporalUnit, FOREVER);
      LABEL(TemporalUnit, WEEK_BASED_YEARS);
      LABEL(TemporalUnit, QUARTER_YEARS);

      labels(table);
      return true;
    }


#undef LABEL
#undef ENUM_CLASS
  } // time

  using namespace time;

  inline namespace literals {
    String operator+(String const& text, TemporalUnit unit) {
      Enum<TemporalUnit> e = unit;
      return text + e;
    }

    String operator+(TemporalUnit unit, String const& text) {
      Enum<TemporalUnit> e = unit;
      return e + text;
    }
  }
} // core
