//
// Created by bruns on 08/05/2024.
//

#include <typeinfo>
#include <core/lang/Class.h>
#include <core/lang/CloneNotSupportedException.h>
#include <core/lang/Long.h>
#include "Object.h"
#include <core/lang/String.h>

#if defined(CORE_COMPILER_MSVC) && !__has_include(<cxxabi.h>)
#else
#include <cxxabi.h>
#endif


namespace core {
  gbool Object::equals(Object const& other) const {
    return this == &other;
  }

  gint Object::hash() const {
    return identityHash(*this);
  }

  String Object::toString() const {
    if (this == &null)
      return "null"_S;

    String s = typeName(*this);
    s += "@"_S;
    s += Integer::toHexString(identityHash(*this));
    return s;
  }

  Object& Object::clone() const {
    CloneNotSupportedException().throws($ftrace());
  }

  String Object::typeName(Object const& obj) {
#if defined(CORE_COMPILER_MSVC) && !__has_include(<core/lang/cxxabi.h>)
        gbyte const *rawName = CORE_CAST(gbyte *, typeid(obj).name());
        gint n = -1;
        if (rawName == null)
            return "Object"_S;

        while (rawName[++n] != '\0') {
            //
        }

        ByteArray ba = ByteArray(n);
        for(int i = 0; i < n; i++) ba[i] = rawName[i];

        String s = String(ba, 0);
        gint i = 0;

        if((i = s.indexOf("class "_S)) >= 0) {
            if(i == 0)
                s = s.subString(6);
            s = s.replace(" class "_S, ""_S);
        }

        if((i = s.indexOf("struct "_S)) >= 0) {
            if(i == 0)
                s = s.subString(7);
            s = s.replace(" struct "_S, ""_S);
        }

        if((i = s.indexOf("enum "_S)) >= 0) {
            if(i == 0)
                s = s.subString(7);
            s = s.replace(" enum "_S, ""_S);
        }

        if((i = s.indexOf("union "_S)) >= 0) {
            if(i == 0)
                s = s.subString(7);
            s = s.replace(" union "_S, ""_S);
        }

        return s.strip();
#else
    gbyte const* rawName = CORE_CAST(gbyte *, typeid(obj).name());
    gint n = -1;
    if (rawName == null)
      return "Object"_S;

    while (rawName[++n] != '\0') {
      //
    }
    n = Math::max(n, 64);

    size_t len = n;
    gint st = 0;
    ByteArray ba = ByteArray(n);
    __cxxabiv1::__cxa_demangle(CORE_CAST(char*, rawName), CORE_CAST(char*, &ba[0]), &len, &st);
    while (st == -1) {
      n *= 2;
      ba = ByteArray(n);
      len = n;
      st = 0;
      __cxxabiv1::__cxa_demangle(CORE_CAST(char*, rawName), CORE_CAST(char*, &ba[0]), &len, &st);
    }
    if (st != 0)
      return "Object"_S;

    n = -1;
    while (ba[++n] != '\0') {
      //
    }

    return String(ba, 0, 0, n);
#endif
  }

  gint Object::identityHash(const Object& x) {
    if (x == null)
      return 0;
    // Identity hash represent the hash code of object address
    glong address = CORE_CAST(glong, &x);
    return Long::hash(address);
  }
} // core
