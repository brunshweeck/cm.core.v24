//
// Created by brunshweeck on 4 sept. 2024.
//

#include <core/lang/XString.h>
#include <core/misc/ObjectArray.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace misc {
        ObjectArray::ObjectArray() { CORE_IGNORE(this); }

        Object &ObjectArray::get0(gint i) { return (value && value[i]) ? *value[i] : null; }

        Object const &ObjectArray::get0(gint i) const { return (value && value[i]) ? *value[i] : null; }

        Object &ObjectArray::set0(gint i, Object &newValue) {
            Object &oldValue = get0(i);
            value[i] = &newValue;
            return oldValue;
        }

        void ObjectArray::setAll0(ObjectArray const &oa) {
            for (int i = 0; i < count; ++i) value[i] = oa.value[i];
        }

        void ObjectArray::swapAll0(ObjectArray &oa) {
            UNSAFE::swapValues(value, oa.value);
            UNSAFE::swapValues(count, oa.count);
        }

        void ObjectArray::reserve(gint n) {
            if (n == 0 && count > 0) {
                UNSAFE::freeMemory(CORE_CAST(glong, value));
                count = 0;
                return;
            }
            if (count > n) {
                count = n;
                return;
            }
            if (!value)
                value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(n * sizeof(VALUE)));
            else
                value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value), n * sizeof(VALUE)));
            count = n;
        }

        gbool ObjectArray::check0(gint i) const { return value && value[i]; }

        gint ObjectArray::length() const { return count; }

        gbool ObjectArray::equals(const Object &o) const {
            if (this == &o)
                return true;
            if (!Class<ObjectArray>::hasInstance(o))
                return false;
            ObjectArray const &oa = CORE_XCAST(ObjectArray const, o);
            if (count != oa.count)
                return false;
            if (!value)
                return !oa.value;
            for (int i = 0; i < count; i++) {
                if (get0(i) != oa.get0(i))
                    return false;
            }
            return true;
        }

        gint ObjectArray::hash() const {
            gint h = 0;
            for (int i = 0; i < count; i++) {
                h = (h * 31) ^ (count - 1 - i) + get0(i).hash();
            }
            return h;
        }

        gbool ObjectArray::isEmpty() const {
            return count == 0;
        }

        String ObjectArray::toString() const {
            if (count == 0)
                return "[]"_S;
            XString str;
            str.append('[');
            for (int i = 0; i < count - 1; i++) {
                str.append(get0(i)).append(',').append(' ');
            }
            return str.append(get0(count - 1)).append(']').toString();
        }
    } // misc
} // core
