//
// Created by brunshweeck on 12/02/25.
//

#include "FormatStyle.h"
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


    ENUM_CLASS(Enum, FormatStyle) {
      Enum e = (FormatStyle) 0;
      static Object& table = labels(null);

      LABEL(FormatStyle, FULL);
      LABEL(FormatStyle, LONG);
      LABEL(FormatStyle, MEDIUM);
      LABEL(FormatStyle, SHORT);

      labels(table);
      return true;
    }


#undef LABEL
#undef ENUM_CLASS
  } // time

  using namespace time;

  inline namespace literals {
    String operator+(String const& text, FormatStyle style) {
      Enum<FormatStyle> e = style;
      return text + e;
    }

    String operator+(FormatStyle style, String const& text) {
      Enum<FormatStyle> e = style;
      return e + text;
    }
  }
} // core
