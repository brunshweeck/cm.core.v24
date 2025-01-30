//
// Created by bruns on 10/05/2024.
//

#ifndef CORE24_FLOATARRAY_H
#define CORE24_FLOATARRAY_H

#include <core/Float.h>
#include <core/misc/PrimitiveArray.h>

namespace core {
    /**
     * The class @c FloatArray wrap the array of primitive
     * type @c gfloat in the object.
     *
     * @note This class provide the random access to elements.
     *
     */
    class FloatArray final : public misc::PrimitiveArray<Float> {
    public:
        /**
         * Construct new empty @c FloatArray
         */
        CORE_IMPLICIT FloatArray() CORE_NOTHROW;

        /**
         * Construct new @c FloatArray instance able to contains
         * the given number of values.
         *
         * @note All value will be initialized with value @c U+0000.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param length The number of value to be allocated on this array
         */
        CORE_EXPLICIT FloatArray(gint length);

        /**
         * Construct new @c FloatArray instance able to contains
         * the given number of values.
         *
         * @note All value will be initialized with given initial value.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param length The number of value to be allocated on this array
         * @param initialValue The value used to initialize all values of
         *          this array.
         */
        CORE_EXPLICIT FloatArray(gint length, gfloat initialValue);

        /**
         * Construct new @c FloatArray instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT FloatArray(FloatArray const& array);

        /**
         * Construct new @c FloatArray instance by swapping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT FloatArray(FloatArray&& array) CORE_NOTHROW;

        /**
         * Construct new @c FloatArray with specified values.
         *
         * @param args the list of values used to create array.
         */
        CORE_IMPLICIT FloatArray(VarArgs const& args);

        /**
         * Return the number of values on this array
         *
         * @return The number of values on this array
         */
        gint length() const override;

        /**
         * Test if this array has no element.
         *
         * @return @c true if this array has no element
         */
        gbool isEmpty() const override;

        /**
         * Return the value of elements at specified index
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        gfloat& get(gint index) override;

        /**
         * Return the value of elements at specified index
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        gfloat const& get(gint index) const override;

        /**
         * Set value of element at specified index with
         * specified new value
         *
         * @param index The index of desired element.
         * @param newValue The replacement value
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        gfloat set(gint index, gfloat newValue) override;

        /**
         * Destroy this array data.
         *
         * @note This operation will clear content firstly.
         */
        ~FloatArray() override;

        gfloat const& operator[](gint index) const;

        gfloat& operator[](gint index);

        FloatArray& operator=(FloatArray const& array);

        FloatArray& operator=(FloatArray&& array) CORE_NOTHROW;

        /**
         * Obtains newly created empty @c FloatArray
         *
         * @return new empty FloatArray
         */
        static FloatArray of();

        /**
         * Obtain newly created @c FloatArray instance with arbitrary
         * number of values.
         *
         * @param args the values to be contained by newly created array.
         * @return The new FloatArray that contains all given values
         */
        template <class... T>
        static FloatArray of(T&&... args) {
            CORE_FAST_ASSERT(Class<>::allIsTrue<Class<Float>::accept<T>() ...>());
            return {CORE_CAST(gfloat, args)...};
        }

        /**
         * Obtain newly created @c FloatArray instance with primitive array
         *
         * @tparam A The array type
         * @param a The array to be copied
         * @return The new FloatArray that contains all values of given array
         */
        template <class A,
                  ClassOf(1)::OnlyIf<Class<A>::isArray()> CaptureArray = 1,
                  ClassOf(1)::OnlyIf<Class<Float>::accept<typename Class<A>::ArrayElement>()> CaptureElements = 1>
        static FloatArray copyOf(A&& a) {
            CORE_FAST gint len = Class<A>::count();
            FloatArray dest = FloatArray(len);
            for (int i = 0; i < len; ++i) dest[i] = a[i];
            return dest;
        }

        /**
         * Obtain newly created @c FloatArray with Array<Float> instance.
         *
         * @param a the array reference.
         * @return the new FloatArray containing all values of given array.
         */
        static FloatArray copyOf(Array<Float> const& a);

        /**
         * Obtain newly created @c FloatArray instance representing the sequence of integers
         * from @c 0 to @c limit ( @a exclusive) by @a step @c 1.
         *
         * @note The call of @c FloatArray::ofRange(l) produces [0, 1, 2, ..., l-1].
         * For example: @c FloatArray::ofRange(4) produces [0, 1, 2, 3].
         *
         * @param limit the max bound of range (exclusive)
         */
        static FloatArray ofRange(gfloat limit);

        /**
         * Obtain newly created @c FloatArray instance representing the sequence of value
         * from @c firstValue ( @a inclusive) to @c limit ( @a exclusive) by @a step @c 1.
         *
         * @note The call of @c FloatArray::ofRange(i,l) produces [i, i+1, i+2, ..., l-1].
         * For example: @c FloatArray::ofRange(1,5) produces [1, 2, 3, 4].
         *
         * @param firstValue the min bound of range (inclusive)
         * @param limit the max bound of range (exclusive)
         */
        static FloatArray ofRange(gfloat firstValue, gfloat limit);

        /**
         * Obtain newly created @c FloatArray instance representing the sequence of integers
         * from @c firstValue ( @a inclusive) to @c limit ( @a exclusive) by @a step @c offsetByValue.
         *
         * @note The call of @c FloatArray::ofRange(i,l,k) produces [i, i+k, i+2k, ..., i+nk] (where i+nk < l).
         * For example: @c FloatArray::ofRange(1,8,2) produces [1, 3, 5, 7].
         *
         * @param firstValue the min bound of range (inclusive)
         * @param limit the max bound of range (exclusive)
         * @param offsetByValue the step between the values of range
         */
        static FloatArray ofRange(gfloat firstValue, gfloat limit, gdouble offsetByValue);

        /**
         * Obtain newly created @c FloatArray representing the sequence of values
         * generated by specified @c generator.
         *
         * @note the remaining values will be generated by function generator.
         * @param length the length sequence.
         * @param generator the function used generated the value at specific index.
         * @return the newly created array.
         */
        static FloatArray ofSequence(gint length, function::DoubleUnaryOperator const& generator);

        /**
         * Obtain newly created @c FloatArray representing the sequence of value
         * generated by specified @c generator.
         * <p>
         *  The nth call of generator corresponding return the value at index n-1.
         * </p>
         *
         * @note the remaining values will be generated by function generator.
         * @param length the length of sequence.
         * @param generator the function used generated the value at specific index.
         * @return the newly created array.
         */
        static FloatArray ofSequence(gint length, function::DoubleSupplier const& generator);

    private:
        class LinearIterator final : public Object {
            CORE_ADD_AS_FRIEND(FloatArray);

            FloatArray& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator(FloatArray& array, gbool isEnd);

        public:
            LinearIterator& operator++();

            gfloat& operator*() const;

            gbool operator==(LinearIterator const& rhs) const;

            gbool operator!=(LinearIterator const& rhs) const;
        };

        class LinearIterator2 final : public Object {
            CORE_ADD_AS_FRIEND(FloatArray);

        private:
            FloatArray const& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator2(FloatArray const& array, gbool isEnd);

        public:
            LinearIterator2& operator++();

            gfloat const& operator*() const;

            gbool operator==(LinearIterator2 const& rhs) const;

            gbool operator!=(LinearIterator2 const& rhs) const;
        };

    public:
        LinearIterator begin();

        LinearIterator2 begin() const;

        LinearIterator end();

        LinearIterator2 end() const;

        gbool equals(const Object& obj) const override;

        gint hash() const override;

        String toString() const override;

        Object& clone() const override;
    };
} // core

#endif // CORE24_FLOATARRAY_H
