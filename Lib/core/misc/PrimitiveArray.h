//
// Created by brunshweeck on 4 sept. 2024.
//

#ifndef CORE24_PRIMITIVEARRAY_H
#define CORE24_PRIMITIVEARRAY_H

#include <core/misc/ValueArray.h>
#include <core/lang/Class.h>

namespace core {
    namespace misc {
        /**
         * The base of any primitive array class
         * @tparam T the type of value supported by this array
         */
        template <class T>
        class PrimitiveArray : public BasicArray {
            CORE_ALIAS(VALUE, typename Class<T>::Prime);
            CORE_ALIAS(ARRAY, typename Class<VALUE>::Pointer);

            CORE_CHECK_SLIMMED_TYPE(T);
            CORE_FAST_ASSERT(Class<VALUE>::isPrimitive());

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_AS_FRIEND(String);
            CORE_ADD_AS_FRIEND(XString);
            CORE_ADD_AS_FRIEND(util::Arrays);

            CORE_ADD_AS_FRIEND(BooleanArray);
            CORE_ADD_AS_FRIEND(ByteArray);
            CORE_ADD_AS_FRIEND(ShortArray);
            CORE_ADD_AS_FRIEND(CharArray);
            CORE_ADD_AS_FRIEND(IntArray);
            CORE_ADD_AS_FRIEND(FloatArray);
            CORE_ADD_AS_FRIEND(LongArray);
            CORE_ADD_AS_FRIEND(DoubleArray);

            CORE_ADD_AS_FRIEND(BooleanArray2D);
            CORE_ADD_AS_FRIEND(ByteArray2D);
            CORE_ADD_AS_FRIEND(ShortArray2D);
            CORE_ADD_AS_FRIEND(CharArray2D);
            CORE_ADD_AS_FRIEND(IntArray2D);
            CORE_ADD_AS_FRIEND(FloatArray2D);
            CORE_ADD_AS_FRIEND(LongArray2D);
            CORE_ADD_AS_FRIEND(DoubleArray2D);

            CORE_ADD_AS_FRIEND(BooleanArray3D);
            CORE_ADD_AS_FRIEND(ByteArray3D);
            CORE_ADD_AS_FRIEND(ShortArray3D);
            CORE_ADD_AS_FRIEND(CharArray3D);
            CORE_ADD_AS_FRIEND(IntArray3D);
            CORE_ADD_AS_FRIEND(FloatArray3D);
            CORE_ADD_AS_FRIEND(LongArray3D);
            CORE_ADD_AS_FRIEND(DoubleArray3D);

            /**
             * The field used to store this array values
             */
            ARRAY value = {};

        protected:
            CORE_ALIAS(VarArgs, std::initializer_list<VALUE>);

            /**
             * Construct new primitive array.
             */
            CORE_IMPLICIT PrimitiveArray() {
            }

        public:
            /**
             * Return the value of elements at specified index
             *
             * @param i The index of desired element.
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            virtual VALUE& get(gint i) = 0;

            /**
             * Return the value of elements at specified index
             *
             * @param i The index of desired element.
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            virtual VALUE const& get(gint i) const = 0;

            /**
             * Set value of element at specified index with
             * specified new value
             *
             * @param i The index of desired element.
             * @param value The replacement value
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            virtual VALUE set(gint i, VALUE value) = 0;

            /**
             * The Array2D class represent the optimization of ValueArray for
             * treatment of 2-dimensional value array.
             *
             * @tparam A the Simple ValueArray (Array1D)
             */
            template <class A>
            class Array2D : public ValueArray<A> {
                CORE_ALIAS(VALUE, PrimitiveArray::VALUE);

            protected:
                /**
                 * Construct new instance of Array2D
                 */
                CORE_IMPLICIT Array2D() {
                };

            public:
                /**
                 * Return the value of elements at specified index (2D)
                 *
                 * @param ix The first coordinate of index of desired element.
                 * @param iy The second coordinate of index of desired element.
                 * @throws IndexOutOfBoundsException If the given index out of bounds
                 */
                virtual VALUE& get(gint ix, gint iy) = 0;

                /**
                 * Return the value of elements at specified index (2D)
                 *
                 * @param ix The index of desired row.
                 * @param iy The index of desired column.
                 * @throws IndexOutOfBoundsException If one of the given index out of bounds
                 */
                virtual VALUE const& get(gint ix, gint iy) const = 0;

                /**
                 * Return the value of elements at specified index (2D)
                 *
                 * @param ix The index of desired row.
                 * @param iy The index of desired column.
                 * @throws IndexOutOfBoundsException If one of the given index out of bounds
                 */
                virtual VALUE set(gint ix, gint iy, VALUE value) = 0;
            };

            /**
             * The Array3D class represent the optimization of ValueArray for
             * treatment of 3-dimensional value array.
             *
             * @tparam A the Array2D
             */
            template <class A, class B>
            class Array3D : public ValueArray<A> {
                CORE_ALIAS(VALUE, PrimitiveArray::VALUE);

            protected:
                CORE_ALIAS(VALUE1D, B);
                CORE_ALIAS(VALUE2D, A);

                /**
                 * Construct new instance of Array2D
                 */
                CORE_IMPLICIT Array3D() {
                };

            public:
                /**
                 * Return the value of elements at specified index (3D)
                 *
                 * @param ix The first coordinate of index of desired element.
                 * @param iy The second coordinate of index of desired element.
                 * @param iz The third coordinate of index of desired element.
                 * @throws IndexOutOfBoundsException If the given index out of bounds
                 */
                virtual VALUE& get(gint ix, gint iy, gint iz) = 0;

                /**
                 * Return the value of elements at specified index (3D)
                 *
                 * @param ix The first coordinate of index of desired element.
                 * @param iy The second coordinate of index of desired element.
                 * @param iz The third coordinate of index of desired element.
                 * @throws IndexOutOfBoundsException If the given index out of bounds
                 */
                virtual VALUE const& get(gint ix, gint iy, gint iz) const = 0;

                /**
                 * Return the value of elements at specified index (3D)
                 *
                 * @param ix The index of desired row.
                 * @param iy The index of desired column.
                 * @param iz The third coordinate of index of desired element.
                 * @throws IndexOutOfBoundsException If one of the given index out of bounds
                 */
                virtual VALUE set(gint ix, gint iy, gint iz, VALUE value) = 0;

                /**
                 * Return the array at specified index (2D)
                 *
                 * @param ix The first coordinate of index of desired element.
                 * @param iy The second coordinate of index of desired element.
                 * @throws IndexOutOfBoundsException If the given index out of bounds
                 */
                virtual VALUE1D& get(gint ix, gint iy) = 0;

                /**
                 * Return the array at specified index (2D)
                 *
                 * @param ix The first coordinate of index of desired element.
                 * @param iy The second coordinate of index of desired element.
                 * @throws IndexOutOfBoundsException If the given index out of bounds
                 */
                virtual VALUE1D const& get(gint ix, gint iy) const = 0;

                /**
                 * Return the array at specified index (2D)
                 *
                 * @param ix The index of desired row.
                 * @param iy The index of desired column.
                 * @throws IndexOutOfBoundsException If one of the given index out of bounds
                 */
                virtual VALUE1D set(gint ix, gint iy, VALUE1D value) = 0;
            };
        };
    } // misc
} // core

#endif //CORE24_PRIMITIVEARRAY_H
