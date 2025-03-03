//
// Created by bruns on 10/05/2024.
//

#ifndef CORE24_BOOLEANARRAY_H
#define CORE24_BOOLEANARRAY_H

#include <core/lang/Boolean.h>
#include <core/misc/PrimitiveArray.h>

namespace core {
    /**
     * The class @c BooleanArray wrap the array of primitive
     * type @c gbool in the object.
     *
     * @note This class provide the random access to elements.
     *
     */
    class BooleanArray final : public misc::PrimitiveArray<Boolean> {
    public:
        /**
         * Construct new empty @c BooleanArray
         */
        CORE_IMPLICIT BooleanArray();

        /**
         * Construct new @c BooleanArray instance able to contains
         * the given number of values.
         *
         * @note All value will be initialized with value @c U+0000.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param length The number of value to be allocated on this array
         */
        CORE_EXPLICIT BooleanArray(gint length);

        /**
         * Construct new @c BooleanArray instance able to contains
         * the given number of values.
         *
         * @note All value will be initialized with given initial value.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param length The number of value to be allocated on this array
         * @param initialValue The value used to initialize all values of
         *          this array.
         */
        CORE_EXPLICIT BooleanArray(gint length, gbool initialValue);

        /**
         * Construct new @c BooleanArray instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT BooleanArray(BooleanArray const &array);

        /**
         * Construct new @c BooleanArray instance by swaping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT BooleanArray(BooleanArray &&array) CORE_NOTHROW;

        /**
         * Construct new @c BooleanArray with specified values.
         *
         * @param args the list of values used to create array.
         */
        CORE_IMPLICIT BooleanArray(VarArgs const &args);

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
        gbool &get(gint index) override;

        /**
         * Return the value of elements at specified index
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        gbool const &get(gint index) const override;

        /**
         * Set value of element at specified index with
         * specified new value
         *
         * @param index The index of desired element.
         * @param newValue The replacement value
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        gbool set(gint index, gbool newValue) override;

        /**
         * Destroy this array data.
         *
         * @note This operation will clear content firstly.
         */
        ~BooleanArray() override;

        /**
         * Obtains newly created empty @c BooleanArray
         *
         * @return new empty BooleanArray
         */
        static BooleanArray of();

        /**
         * Obtain newly created @c BooleanArray instance with arbitrary number of booleans values.
         *
         * @param args the booleans values.
         * @return The new BooleanArray that contains all given values
         */
        template<class... T>
        static BooleanArray of(T &&... args) {
            // Check if the given arguments are valid
            CORE_FAST_ASSERT(Class<>::allIsTrue<Class<Boolean>::accept<T>() ...>());
            return {CORE_CAST(gbool, args)...};
        }

        /**
         * Obtain newly created @c BooleanArray instance with primitive boolean array
         *
         * @note If the given array length exceed implementation limit (Integer::MAX_VALUE),
         *     The length of result array will be Integer::MAX_VALUE
         *
         * @tparam A The array type
         * @param a The array to be copied
         * @return The new BooleanArray that contains all values of given array
         */
        template<class A,
            ClassOf(1)::OnlyIf<Class<A>::isArray()> CaptureArray = 1,
            ClassOf(1)::OnlyIf<Class<Boolean>::accept<typename Class<A>::ArrayElement>()> CaptureElements = 1>
        static BooleanArray copyOf(A &&a) {
            // Get it size
            gint len = Class<A>::count();
            // Create the destination array
            BooleanArray ba = BooleanArray(len);
            for (int i = 0; i < len; ++i) ba[i] = a[i];
            return ba;
        }

        /**
         * Obtain newly created @c BooleanArray with Array<Boolean> instance.
         *
         * @param a the array reference.
         * @return the new BooleanArray containing all values of given array.
         */
        static BooleanArray copyOf(Array<Boolean> const &a);

        /**
         * Obtain newly created @c BooleanArray representing the sequence of values
         * generated by specified @c generator.
         *
         * @note the remaining values will be generated by function generator.
         * @param length the length sequence.
         * @param generator the function used generated the value at specific index.
         * @return the newly created array.
         */
        static BooleanArray ofSequence(gint length, function::IntUnaryOperator const &generator);

        /**
         * Obtain newly created @c BooleanArray representing the sequence of value
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
        static BooleanArray ofSequence(gint length, function::BooleanSupplier const &generator);

        gbool const &operator[](gint index) const;

        gbool &operator[](gint index);

        BooleanArray &operator=(BooleanArray const &array);

        BooleanArray &operator=(BooleanArray &&array) CORE_NOTHROW;

    private:
        class LinearIterator final : public Object {
            CORE_ADD_AS_FRIEND(BooleanArray);

            BooleanArray &array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator(BooleanArray &array, gbool isEnd);

        public:
            LinearIterator &operator++();

            gbool &operator*() const;

            gbool operator==(LinearIterator const &rhs) const;

            gbool operator!=(LinearIterator const &rhs) const;
        };

        class LinearIterator2 final : public Object {
            CORE_ADD_AS_FRIEND(BooleanArray);

            BooleanArray const &array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator2(BooleanArray const &array, gbool isEnd);

        public:
            LinearIterator2 &operator++();

            gbool const &operator*() const;

            gbool operator==(LinearIterator2 const &rhs) const;

            gbool operator!=(LinearIterator2 const &rhs) const;
        };

    public:
        LinearIterator begin();

        LinearIterator2 begin() const;

        LinearIterator end();

        LinearIterator2 end() const;

        gbool equals(const Object &obj) const override;

        gint hash() const override;

        String toString() const override;

        Object &clone() const override;
    };
} // core

#endif // CORE24_BOOLEANARRAY_H
