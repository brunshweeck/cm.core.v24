//
// Created by brunshweeck on 2024/09/14.
//

#ifndef CORE24_FLOATARRAY2D_H
#define CORE24_FLOATARRAY2D_H

#include <core/lang/FloatArray.h>

namespace core {
    /**
     * The @c FloatArray class wrap the 2-Dimensional array of @c gfloat. it's
     * the update and optimized form of primitive type @c gfloat[N][M].
     */
    class FloatArray2D final
        : public misc::PrimitiveArray<Float>::Array2D<FloatArray> {
    public:
        /**
         * Construct new empty @c FloatArray2D
         */
        CORE_IMPLICIT FloatArray2D();

        /**
         * Construct new @c FloatArray2D instance able to contains
         * the given number of rows. The row of this array represent
         * another array (@c FloatArray), And all rows will be initializer
         * as empty array.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated
         */
        CORE_EXPLICIT FloatArray2D(gint nbRows);

        /**
         * Construct new @c FloatArray2D instance able to contains
         * the given number of rows and initialize all rows with
         * given initializer. The row of this array represent another
         * array (@c FloatArray).
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows
         * @param initializer the array used to initialize all rows
         */
        CORE_EXPLICIT FloatArray2D(gint nbRows, FloatArray initializer);

        /**
         * Construct new @c FloatArray2D instance able to contains
         * the given number of rows and given number of columns
         * per row. The row of this array represent another array
         * (@c FloatArray).
         *
         * <p>
         *  During operation, every row will be initializer as
         *  array with length equals to given number of columns.
         *  And him values will be initializer with value @c 0.0f .
         * </p>
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per row to be allocated on this array
         */
        CORE_EXPLICIT FloatArray2D(gint nbRows, gint nbCols);

        /**
         * Construct new @c FloatArray2D instance able to contains
         * the given number of rows and given number of columns
         * per row. The row of this array represent another array
         * (@c FloatArray).
         *
         * <p>
         *  During operation, every row will be initializer as
         *  array with length equals to given number of columns.
         *  And him values will be initializer with zero.
         * </p>
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per row to be allocated on this array
         * @param initializer the value used to initialize values of all rows.
         */
        CORE_EXPLICIT FloatArray2D(gint nbRows, gint nbCols, gfloat initializer);

        /**
         * Construct new @c FloatArray2D instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT FloatArray2D(FloatArray2D const& array);

        /**
         * Construct new @c FloatArray2D instance by swapping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT FloatArray2D(FloatArray2D&& array) CORE_NOTHROW;

        /**
         * Construct new @c FloatArray2D with specified list of rows
         * such that one rows is an instance of FloatArray.
         * <p>
         * Example:
         * @code
         *  FloatArray2D fa = {
         *     {'a', 'b', ..., 'z'}, // 1st row.
         *     ...
         *     {'a', 'b', ..., 'z'}, // nth row.
         *  }
         * @endcode
         * </p>
         *
         * @note This constructor is Equivalent to call of
         * @c FloatArray2D::of(...) methods, And not do
         * implicit cast of values.
         *
         * @param args the list of rows used to create array.
         */
        CORE_IMPLICIT FloatArray2D(VarArgs const& args);

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
         * Return The array representing the nth row (where n = index)
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        FloatArray& get(gint index) override;

        /**
         * Return The array representing the nth row (where n = index)
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        FloatArray const& get(gint index) const override;

        /**
         * Set the row at specified position
         *
         * @param index The index of desired element.
         * @param newRow The replacement row
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        FloatArray set(gint index, FloatArray newRow) override;

        /**
         * Return the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         FloatArray2D fa = ...;
         *         return fa[ix][iy];
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @return the value of elements at specified index [ix, iy]
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gfloat& get(gint ix, gint iy) override;

        /**
         * Return the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         FloatArray2D fa = ...;
         *         return fa[ix][iy];
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @return the value of elements at specified index [ix, iy]
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gfloat const& get(gint ix, gint iy) const override;

        /**
         * Set the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         FloatArray2D fa = ...;
         *         fa[ix].set(iy, newValue);
         *         // or
         *         fa[ix][iy] = newValue;
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param newValue the replacement value.
         * @return the old value at given index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gfloat set(gint ix, gint iy, gfloat newValue) override;

        /**
         * Destroy this array
         */
        ~FloatArray2D() override;

        /**
         * Return the string representation of this array.
         *
         * @return the string representation of this array.
         */
        String toString() const override;

        /**
         * Return the shadow copy of this array.
         *
         * @return The shadow copy of this array.
         */
        Object& clone() const override;

        /**
         * Return the row of elements at specified index
         *
         * @param index The index of desired row.
         * @return the row at specified index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        FloatArray const& operator[](gint index) const;

        /**
         * Return the row of elements at specified index
         *
         * @param index The index of desired row.
         * @return the row at specified index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        FloatArray& operator[](gint index);

        /**
         * Set the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        FloatArray2D& operator=(FloatArray2D const& array);

        /**
         * Swap the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        FloatArray2D& operator=(FloatArray2D&& array) CORE_NOTHROW;

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with zero rows
         *
         * @return empty float array
         */
        static FloatArray of();

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with single row
         *
         * @param r The first row
         */
        static FloatArray2D of(FloatArray const& r);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with two rows
         *
         * @param r0 The first row
         * @param r1 The second row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with three rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with four rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with five rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3,
                              FloatArray const& r4);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with six rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3,
                              FloatArray const& r4, FloatArray const& r5);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with seven rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3,
                              FloatArray const& r4, FloatArray const& r5, FloatArray const& r6);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with eight rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         * @param r7 The eighth row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3,
                              FloatArray const& r4, FloatArray const& r5, FloatArray const& r6, FloatArray const& r7);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with nine rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         * @param r7 The eighth row
         * @param r8 The ninth row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3,
                              FloatArray const& r4, FloatArray const& r5, FloatArray const& r6, FloatArray const& r7,
                              FloatArray const& r8);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with ten rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         * @param r7 The eighth row
         * @param r8 The ninth row
         * @param r9 The tenth row
         */
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3,
                              FloatArray const& r4, FloatArray const& r5, FloatArray const& r6, FloatArray const& r7,
                              FloatArray const& r8, FloatArray const& r9);

        /**
         * Obtain newly created instance of @c FloatArray2D initialized with ten and more rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         * @param r7 The eighth row
         * @param r8 The ninth row
         * @param r9 The tenth row
         * @param rows rest of rows
         */
        template <class... Rows>
        static FloatArray2D of(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3,
                              FloatArray const& r4, FloatArray const& r5, FloatArray const& r6, FloatArray const& r7,
                              FloatArray const& r8, FloatArray const& r9, Rows&&... rows) {
            CORE_FAST_ASSERT(Class<>::allIsTrue<Class<FloatArray>::isSame<Rows>()...>());

            CORE_FAST gint n = sizeof...(Rows);
            gint wi = 10;

            FloatArray2D fa = FloatArray2D(10 + n);

            fa.initializeFirstTens(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
            accumulate(fa.initialize(wi, CORE_CAST(Rows &&, rows))...);

            return fa;
        }

        /**
         * Obtain new FloatArray2D instance from Array<FloatArray>
         *
         * @param a the reference array 2D
         * @return the value array 2D
         */
     static FloatArray2D copyOf(Array<FloatArray> const& a);

    private:
     // ---------------------------- Iterator API ---------------------------------------------

     class Linear2DIterator final : public Object {
      FloatArray2D& array;
      gint next = 0;
      gbool isEnd = false;

      CORE_ADD_AS_FRIEND(FloatArray2D);

      CORE_EXPLICIT Linear2DIterator(FloatArray2D& array, gbool isEnd);

     public:
      Linear2DIterator(Linear2DIterator const&) = delete;
      Linear2DIterator(Linear2DIterator&&) = default;

      Linear2DIterator& operator=(Linear2DIterator const&) = delete;
      Linear2DIterator& operator=(Linear2DIterator&&) = delete;

      ~Linear2DIterator() override = default;


      Linear2DIterator& operator++();

      FloatArray& operator*() const;

      gbool equals(const Object& o) const override;
     };

     class Linear2DIterator2 final : public Object {
      FloatArray2D const& array;
      gint next = 0;
      gbool isEnd = false;

      CORE_ADD_AS_FRIEND(FloatArray2D);

      CORE_EXPLICIT Linear2DIterator2(FloatArray2D const& array, gbool isEnd);

     public:
      Linear2DIterator2(Linear2DIterator2 const&) = delete;
      Linear2DIterator2(Linear2DIterator2&&) = default;

      Linear2DIterator2& operator=(Linear2DIterator2 const&) = delete;
      Linear2DIterator2& operator=(Linear2DIterator2&&) = delete;

      ~Linear2DIterator2() override = default;


      Linear2DIterator2& operator++();

      FloatArray const& operator*() const;

      gbool equals(const Object& o) const override;
     };

    public:
     Linear2DIterator begin();

     Linear2DIterator2 begin() const;

     Linear2DIterator end();

     Linear2DIterator2 end() const;

    private:
        void initializeFirstTens(FloatArray const& r0, FloatArray const& r1, FloatArray const& r2, FloatArray const& r3,
                                 FloatArray const& r4, FloatArray const& r5, FloatArray const& r6, FloatArray const& r7,
                                 FloatArray const& r8, FloatArray const& r9);

        // ---------------------------- Varargs API ---------------------------------------------
        gint initialize(gint& i, FloatArray const& r);
        gint initialize(gint& i, FloatArray& r);
        gint initialize(gint& i, FloatArray&& r);

        static void accumulate(...);
    };
} // core

#endif //CORE24_FLOATARRAY2D_H
