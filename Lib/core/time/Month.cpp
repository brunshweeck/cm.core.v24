//
// Created by brunshweeck on 12/02/25.
//

#include "Month.h"

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


    ENUM_CLASS(Enum, Month) {
      Enum e = (Month) 0;
      static Object& table = labels(null);

      LABEL(Month, JANUARY);
      LABEL(Month, FEBRUARY);
      LABEL(Month, MARCH);
      LABEL(Month, APRIL);
      LABEL(Month, MAY);
      LABEL(Month, JUNE);
      LABEL(Month, JULY);
      LABEL(Month, AUGUST);
      LABEL(Month, SEPTEMBER);
      LABEL(Month, OCTOBER);
      LABEL(Month, NOVEMBER);
      LABEL(Month, DECEMBER);

      labels(table);
      return true;
    }


#undef LABEL
#undef ENUM_CLASS
  } // time
  using namespace time;

  inline namespace literals {
    String operator+(String const& text, Month month) {
      Enum<Month> e = month;
      return text + e;
    }

    String operator+(Month month, String const& text) {
      Enum<Month> e = month;
      return e + text;
    }
  }
} // core
