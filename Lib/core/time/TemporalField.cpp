//
// Created by brunshweeck on 12/02/25.
//

#include "TemporalField.h"
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


    ENUM_CLASS(Enum, TemporalField) {
      Enum e = (TemporalField) 0;
      static Object& table = labels(null);

      LABEL(TemporalField, NANO_OF_SECOND);
      LABEL(TemporalField, NANO_OF_DAY);
      LABEL(TemporalField, MICRO_OF_SECOND);
      LABEL(TemporalField, MICRO_OF_DAY);
      LABEL(TemporalField, MILLI_OF_SECOND);
      LABEL(TemporalField, MILLI_OF_DAY);
      LABEL(TemporalField, SECOND_OF_MINUTE);
      LABEL(TemporalField, SECOND_OF_DAY);
      LABEL(TemporalField, MINUTE_OF_HOUR);
      LABEL(TemporalField, MINUTE_OF_DAY);
      LABEL(TemporalField, HOUR_OF_AMPM);
      LABEL(TemporalField, CLOCK_HOUR_OF_AMPM);
      LABEL(TemporalField, HOUR_OF_DAY);
      LABEL(TemporalField, CLOCK_HOUR_OF_DAY);
      LABEL(TemporalField, AMPM_OF_DAY);
      LABEL(TemporalField, DAY_OF_WEEK);
      LABEL(TemporalField, ALIGNED_DAY_OF_WEEK_IN_MONTH);
      LABEL(TemporalField, ALIGNED_DAY_OF_WEEK_IN_YEAR);
      LABEL(TemporalField, DAY_OF_MONTH);
      LABEL(TemporalField, DAY_OF_YEAR);
      LABEL(TemporalField, EPOCH_DAY);
      LABEL(TemporalField, ALIGNED_WEEK_OF_MONTH);
      LABEL(TemporalField, ALIGNED_WEEK_OF_YEAR);
      LABEL(TemporalField, MONTH_OF_YEAR);
      LABEL(TemporalField, PROLEPTIC_MONTH);
      LABEL(TemporalField, YEAR_OF_ERA);
      LABEL(TemporalField, YEAR);
      LABEL(TemporalField, ERA);
      LABEL(TemporalField, INSTANT_SECONDS);
      LABEL(TemporalField, OFFSET_SECONDS);
      LABEL(TemporalField, DAY_OF_QUARTER);
      LABEL(TemporalField, QUARTER_OF_YEAR);
      LABEL(TemporalField, WEEK_OF_WEEK_BASED_YEAR);
      LABEL(TemporalField, WEEK_BASED_YEAR);
      LABEL(TemporalField, JULIAN_DAY);
      LABEL(TemporalField, MODIFIED_JULIAN_DAY);
      LABEL(TemporalField, RATA_DIE);

      labels(table);
      return true;
    }


#undef LABEL
#undef ENUM_CLASS
  } // time

  using namespace time;

  inline namespace literals {
    String operator+(String const& text, TemporalField field) {
      Enum<TemporalField> e = field;
      return text + e;
    }

    String operator+(TemporalField field, String const& text) {
      Enum<TemporalField> e = field;
      return e + text;
    }
  }
} // core
