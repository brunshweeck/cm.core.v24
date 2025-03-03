//
// Created by brunshweeck on 12/02/25.
//

#include "DayOfWeek.h"
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


    ENUM_CLASS(Enum, DayOfWeek) {
      Enum e = (DayOfWeek) 0;
      static Object& table = labels(null);

      LABEL(DayOfWeek, MONDAY);
      LABEL(DayOfWeek, TUESDAY);
      LABEL(DayOfWeek, WEDNESDAY);
      LABEL(DayOfWeek, THURSDAY);
      LABEL(DayOfWeek, FRIDAY);
      LABEL(DayOfWeek, SATURDAY);
      LABEL(DayOfWeek, SUNDAY);

      labels(table);
      return true;
    }


#undef LABEL
#undef ENUM_CLASS
  } // util

  using namespace time;

  inline namespace literals {
    String operator+(String const& text, DayOfWeek dayOfWeek) {
      Enum<DayOfWeek> e = dayOfWeek;
      return text + e;
    }

    String operator+(DayOfWeek dayOfWeek, String const& text) {
      Enum<DayOfWeek> e = dayOfWeek;
      return e + text;
    }
  }
} // core
