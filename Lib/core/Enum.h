//
// Created by admin on 09/01/25.
//

#ifndef CORE24_ENUM_H
#define CORE24_ENUM_H

#include <core/util/HashMap.h>
#include <core/util/Optional.h>

namespace core {
    /**
     * This is the common base class of all enumeration.
     *
     * @param E The type of the enum subclass
     */
    template <class E>
    class Enum : public virtual Comparable<Enum<E>> {
        CORE_CHECK_PRIME_TYPE(E);
        CORE_FAST_ASSERT(Class<E>::isEnum());
        CORE_ALIAS(UNSAFE, Object::UNSAFE);
        CORE_ALIAS(MAP, util::Map<Enum, String>);
        CORE_ALIAS(TABLE, util::HashMap<Enum, String>);
        CORE_ALIAS(CACHE, util::Optional<MAP>);

        /**
         * The name of this enum constant, as declared in the enum declaration.
         */
        String label;

        /**
         * The value of this enumeration constant
         */
        E value;

    public:
        CORE_IMPLICIT Enum(E value): value(value) {}

        /**
         * Returns the name of this enum constant, exactly as declared in its
         * enum declaration.
         *
         * <b>
         * Most programmers should use the toString method in
         * preference to this one, as the toString method may return
         * a more user-friendly name.</b>  This method is designed primarily for
         * use in specialized situations where correctness depends on getting the
         * exact name, which will not vary from release to release.
         * @return the name of this enum constant
         */
        String name() const {
            return label;
        }

        /**
         * Returns the ordinal of this enumeration constant (its position
         * in its enum declaration, where the initial constant is assigned
         * an ordinal of zero).
         *
         * @return the ordinal of this enumeration constant
         */
        gint ordinal() const {
            return (gint) value;
        }

        /**
         * Returns the name of this enum constant, as contained in the
         * declaration.  This method may be overridden, though it typically
         * isn't necessary or desirable.  An enum class should override this
         * method when a more "programmer-friendly" string form exists.
         *
         * @return the name of this enum constant
         */
        String toString() const override {
            if (!label.isEmpty())
                return label;
            Object& defaultLabel = labels(null).getOrNull(*this);
            if (defaultLabel != null)
                return CORE_XCAST(String, defaultLabel);
            return String::valueOf(ordinal());
        }

        /**
         * Returns true if the specified object is equal to this
         * enum constant.
         *
         * @param other the object to be compared for equality with this object.
         * @return  true if the specified object is equal to this
         *          enum constant.
         */
        gbool equals(Object const& other) const override {
            return this == &other || Class<Enum>::hasInstance(other) && value == CORE_XCAST(Enum const, other).value;
        }

        /**
         * Returns a hash code for this enum constant.
         *
         * @return a hash code for this enum constant.
         */
        gint hash() const override {
            return ordinal();
        }

        Object& clone() const override {
            try {
                return UNSAFE::newInstance<Enum>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Compares this enum with the specified object for order.  Returns a
         * negative integer, zero, or a positive integer as this object is less
         * than, equal to, or greater than the specified object.
         *
         * Enum constants are only comparable to other enum constants of the
         * same enum type.  The natural order implemented by this
         * method is the order in which the constants are declared.
         */
        gint compareTo(Enum const& other) const override {
            return ordinal() - other.ordinal();
        }

        CORE_FAST operator E() const {
            return value;
        }

    protected:
        /**
         * Sole constructor.
         *
         * @param name The name of this enum constant, which is the identifier
         *               used to declare it.
         * @param ordinal The ordinal of this enumeration constant (its position
         *         in the enum declaration, where the initial constant is assigned
         *         an ordinal of zero).
         */
        CORE_EXPLICIT Enum(String const& name, gint ordinal) {
            label = name;
            value = (E) ordinal;
            setDefaultLabel(label, *this);
        }

        static void setDefaultLabel(String const& label, E value) {
            Object& oldLabel = labels(null).put(Enum(value), label);
            if (oldLabel != null)
                UNSAFE::deleteRegInstance(oldLabel);
        }

        /**
         * The labels cache
         */
        static MAP& labels(Object& table) {
            static CACHE LABELS = {};
            if (LABELS.isEmpty()) {
                if (table == null)
                    LABELS = UNSAFE::newInstance<TABLE>();
                else
                    LABELS = CORE_XCAST(MAP, table);
            }
            return LABELS.get();
        };
    };
} // core

#endif //CORE24_ENUM_H
