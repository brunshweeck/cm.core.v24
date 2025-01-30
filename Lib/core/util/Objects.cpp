//
// Created by admin on 18/01/25.
//

#include "Objects.h"

#include <core/AssertionError.h>
#include <core/Boolean.h>
#include <core/Float.h>
#include <core/Long.h>
#include <core/Short.h>
#include <core/misc/Preconditions.h>

namespace core {
    namespace util {
        Objects::Objects() {
            AssertionError("No util::Objects instances for you!").throws($ftrace());
        }

        gbool Objects::equals(Object const& a, Object const& b) {
            return &a == &b || (&a != &null && a.equals(b));
        }

        gint Objects::hash(Object const& obj) {
            return Hasher::hash(&obj);
        }

        String Objects::toString(Object const& obj) {
            try {
                return String::valueOf(obj);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String Objects::toString(Object const& obj, String const& nullDefault) {
            return obj != null ? toString(obj) : nullDefault;
        }

        gint Objects::checkIndex(gint index, gint length) {
            try {
                return misc::Preconditions::checkIndex(index, length);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Objects::checkIndexForRange(gint fromIndex, gint toIndex, gint length) {
            try {
                return misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, length);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Objects::checkIndexForSize(gint fromIndex, gint size, gint length) {
            try {
                return misc::Preconditions::checkIndexFromSize(fromIndex, size, length);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        glong Objects::checkIndex(glong index, glong length) {
            try {
                return misc::Preconditions::checkIndex(index, length);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        glong Objects::checkIndexForRange(glong fromIndex, glong toIndex, glong length) {
            try {
                return misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, length);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        glong Objects::checkIndexForSize(glong fromIndex, glong size, glong length) {
            try {
                return misc::Preconditions::checkIndexFromSize(fromIndex, size, length);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        Objects::Pointers::Pointers() {
            AssertionError("No util::Objects::Pointers instances for you!").throws($ftrace());
        }

        gbool Objects::Pointers::isNull(Object const& obj) {
            return obj == null;
        }

        Objects::Hasher::Hasher() {
            AssertionError("No util::Objects::Hasher instances for you!").throws($ftrace());
        }

        gint Objects::Hasher::hash(Object const& obj) { return obj != null ? 0 : obj.hash(); }

        gint Objects::Hasher::hash(gbool b) { return Boolean::hash(b); }

        gint Objects::Hasher::hash(gbyte b) { return Byte::hash(b); }

        gint Objects::Hasher::hash(gshort s) { return Short::hash(s); }

        gint Objects::Hasher::hash(gint i) { return Integer::hash(i); }

        gint Objects::Hasher::hash(glong i) { return Long::hash(i); }

        gint Objects::Hasher::hash(gfloat f) { return Float::hash(f); }

        gint Objects::Hasher::hash(gdouble d) { return Double::hash(d); }

        gint Objects::Hasher::hash(gchar c) { return Character::hash(c); }

        Objects::Stringifier::Stringifier() {
            AssertionError("No util::Objects::Stringifier instances for you!").throws($ftrace());
        }

        String Objects::Stringifier::toString(Object const& obj) {
            try {
                return Objects::toString(obj);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String Objects::Stringifier::toString(gbool b) {
            return Boolean::toString(b);
        }

        String Objects::Stringifier::toString(gbyte b) {
            return Byte::toString(b);
        }

        String Objects::Stringifier::toString(gshort s) {
            return Short::toString(s);
        }

        String Objects::Stringifier::toString(gint i) {
            return Integer::toString(i);
        }

        String Objects::Stringifier::toString(glong i) {
            return Long::toString(i);
        }

        String Objects::Stringifier::toString(gfloat f) {
            return Float::toString(f);
        }

        String Objects::Stringifier::toString(gdouble d) {
            return Double::toString(d);
        }

        String Objects::Stringifier::toString(gchar c) {
            return Character::toString(c);
        }
    } // util
} // core
