//
// Created by bruns on 13/05/2024.
//

#include <core/Byte.h>
#include <core/Integer.h>
#include <core/NumberFormatException.h>
#include <core/String.h>
#include <core/misc/Unsafe.h>

namespace core {
    String Byte::toString(gbyte value) {
        return Integer::toString(value);
    }

    Byte Byte::valueOf(gbyte b) {
        return b;
    }

    gbyte Byte::parseByte(String const &s, gint radix) {
        gint i;
        try {
            i = Integer::parseInt(s, radix);
        } catch (Exception const &ex) {
            ex.throws($ftrace());
        }
        if (i < MIN_VALUE || i > MAX_VALUE) {
            NumberFormatException("value out of range"_S).throws($ftrace());
        }
        return CORE_CAST(gbyte, i);
    }

    gbyte Byte::parseByte(String const &s) {
        try {
            return parseByte(s, 10);
        } catch (Exception const &ex) {
            ex.throws($ftrace());
        }
    }

    Byte Byte::valueOf(String const &s, gint radix) {
        try {
            return valueOf(parseByte(s, radix));
        } catch (Exception const &ex) {
            ex.throws($ftrace());
        }
    }

    Byte Byte::valueOf(String const &s) {
        try {
            return valueOf(s, 10);
        } catch (Exception const &ex) {
            ex.throws($ftrace());
        }
    }

    Byte Byte::decode(String const &nm) {
        gint i;
        try {
            i = Integer::decode(nm);
        } catch (Exception const &ex) {
            ex.throws($ftrace());
        }
        if (i < MIN_VALUE || i > MAX_VALUE) {
            NumberFormatException("value out of range"_S).throws($ftrace());
        }
        return CORE_CAST(gbyte, i);
    }

    Byte::Byte() CORE_NOTHROW {
        CORE_IGNORE(this);
    }

    Byte::Byte(gbyte value) : value(value) {
    }

    gbyte Byte::byteValue() const {
        return value;
    }

    gshort Byte::shortValue() const {
        return value;
    }

    gint Byte::intValue() const {
        return value;
    }

    glong Byte::longValue() const {
        return value;
    }

    gfloat Byte::floatValue() const {
        return value;
    }

    gdouble Byte::doubleValue() const {
        return value;
    }

    String Byte::toString() const {
        return toString(value);
    }

    gint Byte::hash() const {
        return hash(value);
    }

    gint Byte::hash(gbyte value) {
        return (gint) value;
    }

    gbool Byte::equals(Object const &obj) const {
        return this == &obj || Class<Byte>::hasInstance(obj) && value == CORE_XCAST(Byte const, obj).value;
    }

    gint Byte::compareTo(Byte const &anotherByte) const {
        return compare(value, anotherByte.value);
    }

    gint Byte::compare(gbyte x, gbyte y) {
        return x - y;
    }

    gint Byte::compareUnsigned(gbyte x, gbyte y) {
        return toUnsignedInt(x) - toUnsignedInt(y);
    }

    gint Byte::toUnsignedInt(gbyte x) {
        return x & 0xff;
    }

    glong Byte::toUnsignedLong(gbyte x) {
        return x & 0xffLL;
    }

    Byte::operator gbyte() const {
        return byteValue();
    }

    Byte::operator gbyte &() {
        return value;
    }

    Object &Byte::clone() const {
        try {
            return UNSAFE::newInstance<Byte>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
