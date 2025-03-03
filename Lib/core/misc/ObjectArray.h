//
// Created by brunshweeck on 4 sept. 2024.
//

#ifndef CORE24_OBJECTARRAY_H
#define CORE24_OBJECTARRAY_H

#include <core/misc/BasicArray.h>
#include <core/lang/Class.h>

namespace core {
    namespace misc {
        /**
         * The base of reference array class (@c Array<E>)
         */
        class ObjectArray : public BasicArray {
            CORE_ALIAS(VALUE, Class<Object>::Pointer);
            CORE_ALIAS(ARRAY, Class<VALUE>::Pointer);

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_AS_FRIEND(util::Arrays);
            CORE_ADD_AS_FRIEND(Array<Object>);

            CORE_ADD_TEMPLATES_AS_FRIEND(Array, T);

            /**
             * The field used to store this array values
             */
            ARRAY value = {};

        protected:
            template<class T>
            class Wrapper final : public Object {
                T const &value;

            public:
                CORE_FAST CORE_IMPLICIT Wrapper(T const &arg) : value(arg) {}

                T const &get() const { return value; }
            };

            template<class T, class Obj, ClassOf(1)::OnlyIf<Class<Obj>::template accept<T>()> CaptureArgs = 1>
            CORE_ALIAS(VarArgs, std::initializer_list<T>);

            /**
             * Construct new primitive array.
             */
            CORE_IMPLICIT ObjectArray();

            /**
             * Return the value of elements at specified index
             *
             * @param i The index of desired element.
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            Object &get0(gint i);

            /**
             * Return the value of elements at specified index
             *
             * @param i The index of desired element.
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            Object const &get0(gint i) const;

            /**
             * Set value of element at specified index with
             * specified new value
             *
             * @param i The index of desired element.
             * @param value The replacement value
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            Object &set0(gint i, Object &value);

            void setAll0(ObjectArray const &oa);

            void swapAll0(ObjectArray &oa);

            void reserve(gint n);

            gbool check0(gint i) const;

        public:
            gint length() const override;

            gbool equals(const Object &o) const override;

            gint hash() const override;

            gbool isEmpty() const override;

            String toString() const override;
        };
    } // misc
} // core

#endif //CORE24_OBJECTARRAY_H
