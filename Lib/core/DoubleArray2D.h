//
// Created by brunshweeck on 2024/09/14.
//

#ifndef CORE24_DOUBLEARRAY2D_H
#define CORE24_DOUBLEARRAY2D_H

#include <core/DoubleArray.h>

namespace core {
    /**
     * The @c DoubleArray class wrap the 2-Dimensional array of @c gdouble. it's
     * the update and optimized form of primitive type @c gdouble[N][M].
     */
    class DoubleArray2D final
        : public misc::PrimitiveArray<Double>::Array2D<DoubleArray> {
    public:
        /**
         * Construct new empty @c DoubleArray2D
         */
        CORE_IMPLICIT DoubleArray2D();

        /**
         * Construct new @c DoubleArray2D instance able to contains
         * the given number of rows. The row of this array represent
         * another array (@c DoubleArray), And all rows will be initializer
         * as empty array.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated
         */
        CORE_EXPLICIT DoubleArray2D(gint nbRows);

        /**
         * Construct new @c DoubleArray2D instance able to contains
         * the given number of rows and initialize all rows with
         * given initializer. The row of this array represent another
         * array (@c DoubleArray).
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows
         * @param initializer the array used to initialize all rows
         */
        CORE_EXPLICIT DoubleArray2D(gint nbRows, DoubleArray initializer);

        /**
         * Construct new @c DoubleArray2D instance able to contains
         * the given number of rows and given number of columns
         * per row. The row of this array represent another array
         * (@c DoubleArray).
         *
         * <p>
         *  During operation, every row will be initializer as
         *  array with length equals to given number of columns.
         *  And him values will be initializer with value @c 0.0 .
         * </p>
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per row to be allocated on this array
         */
        CORE_EXPLICIT DoubleArray2D(gint nbRows, gint nbCols);

        /**
         * Construct new @c DoubleArray2D instance able to contains
         * the given number of rows and given number of columns
         * per row. The row of this array represent another array
         * (@c DoubleArray).
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
        CORE_EXPLICIT DoubleArray2D(gint nbRows, gint nbCols, gdouble initializer);

        /**
         * Construct new @c DoubleArray2D instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT DoubleArray2D(DoubleArray2D const& array);

        /**
         * Construct new @c DoubleArray2D instance by swapping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT DoubleArray2D(DoubleArray2D&& array) CORE_NOTHROW;

        /**
         * Construct new @c DoubleArray2D with specified list of rows
         * such that one rows is an instance of DoubleArray.
         * <p>
         * Example:
         * @code
         *  DoubleArray2D da = {
         *     {'a', 'b', ..., 'z'}, // 1st row.
         *     ...
         *     {'a', 'b', ..., 'z'}, // nth row.
         *  }
         * @endcode
         * </p>
         *
         * @note This constructor is Equivalent to call of
         * @c DoubleArray2D::of(...) methods, And not do
         * implicit cast of values.
         *
         * @param args the list of rows used to create array.
         */
        CORE_IMPLICIT DoubleArray2D(VarArgs const& args);

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
        DoubleArray& get(gint index) override;

        /**
         * Return The array representing the nth row (where n = index)
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        DoubleArray const& get(gint index) const override;

        /**
         * Set the row at specified position
         *
         * @param index The index of desired element.
         * @param newRow The replacement row
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        DoubleArray set(gint index, DoubleArray newRow) override;

        /**
         * Return the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         DoubleArray2D da = ...;
         *         return da[ix][iy];
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @return the value of elements at specified index [ix, iy]
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gdouble& get(gint ix, gint iy) override;

        /**
         * Return the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         DoubleArray2D da = ...;
         *         return da[ix][iy];
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @return the value of elements at specified index [ix, iy]
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gdouble const& get(gint ix, gint iy) const override;

        /**
         * Set the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         DoubleArray2D da = ...;
         *         da[ix].set(iy, newValue);
         *         // or
         *         da[ix][iy] = newValue;
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param newValue the replacement value.
         * @return the old value at given index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gdouble set(gint ix, gint iy, gdouble newValue) override;

        /**
         * Destroy this array
         */
        ~DoubleArray2D() override;

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
        DoubleArray const& operator[](gint index) const;

        /**
         * Return the row of elements at specified index
         *
         * @param index The index of desired row.
         * @return the row at specified index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        DoubleArray& operator[](gint index);

        /**
         * Set the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        DoubleArray2D& operator=(DoubleArray2D const& array);

        /**
         * Swap the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        DoubleArray2D& operator=(DoubleArray2D&& array) CORE_NOTHROW;

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with zero rows
         *
         * @return empty double array
         */
        static DoubleArray of();

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with single row
         *
         * @param r The first row
         */
        static DoubleArray2D of(DoubleArray const& r);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with two rows
         *
         * @param r0 The first row
         * @param r1 The second row
         */
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with three rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         */
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with four rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         */
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with five rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         */
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3,
                              DoubleArray const& r4);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with six rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         */
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3,
                              DoubleArray const& r4, DoubleArray const& r5);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with seven rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         */
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3,
                              DoubleArray const& r4, DoubleArray const& r5, DoubleArray const& r6);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with eight rows
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
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3,
                              DoubleArray const& r4, DoubleArray const& r5, DoubleArray const& r6, DoubleArray const& r7);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with nine rows
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
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3,
                              DoubleArray const& r4, DoubleArray const& r5, DoubleArray const& r6, DoubleArray const& r7,
                              DoubleArray const& r8);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with ten rows
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
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3,
                              DoubleArray const& r4, DoubleArray const& r5, DoubleArray const& r6, DoubleArray const& r7,
                              DoubleArray const& r8, DoubleArray const& r9);

        /**
         * Obtain newly created instance of @c DoubleArray2D initialized with ten and more rows
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
        static DoubleArray2D of(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3,
                              DoubleArray const& r4, DoubleArray const& r5, DoubleArray const& r6, DoubleArray const& r7,
                              DoubleArray const& r8, DoubleArray const& r9, Rows&&... rows) {
            CORE_FAST_ASSERT(Class<>::allIsTrue<Class<DoubleArray>::isSame<Rows>()...>());

            CORE_FAST gint n = sizeof...(Rows);
            gint wi = 10;

            DoubleArray2D da = DoubleArray2D(10 + n);

            da.initializeFirstTens(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
            accumulate(da.initialize(wi, CORE_CAST(Rows &&, rows))...);

            return da;
        }

        /**
         * Obtain new DoubleArray2D instance from Array<DoubleArray>
         *
         * @param a the reference array 2D
         * @return the value array 2D
         */
     static DoubleArray2D copyOf(Array<DoubleArray> const& a);

    private:
     // ---------------------------- Iterator API ---------------------------------------------

     class Linear2DIterator final : public Object {
      DoubleArray2D& array;
      gint next = 0;
      gbool isEnd = false;

      CORE_ADD_AS_FRIEND(DoubleArray2D);

      CORE_EXPLICIT Linear2DIterator(DoubleArray2D& array, gbool isEnd);

     public:
      Linear2DIterator(Linear2DIterator const&) = delete;
      Linear2DIterator(Linear2DIterator&&) = default;

      Linear2DIterator& operator=(Linear2DIterator const&) = delete;
      Linear2DIterator& operator=(Linear2DIterator&&) = delete;

      ~Linear2DIterator() override = default;


      Linear2DIterator& operator++();

      DoubleArray& operator*() const;

      gbool equals(const Object& o) const override;
     };

     class Linear2DIterator2 final : public Object {
      DoubleArray2D const& array;
      gint next = 0;
      gbool isEnd = false;

      CORE_ADD_AS_FRIEND(DoubleArray2D);

      CORE_EXPLICIT Linear2DIterator2(DoubleArray2D const& array, gbool isEnd);

     public:
      Linear2DIterator2(Linear2DIterator2 const&) = delete;
      Linear2DIterator2(Linear2DIterator2&&) = default;

      Linear2DIterator2& operator=(Linear2DIterator2 const&) = delete;
      Linear2DIterator2& operator=(Linear2DIterator2&&) = delete;

      ~Linear2DIterator2() override = default;


      Linear2DIterator2& operator++();

      DoubleArray const& operator*() const;

      gbool equals(const Object& o) const override;
     };

    public:
     Linear2DIterator begin();

     Linear2DIterator2 begin() const;

     Linear2DIterator end();

     Linear2DIterator2 end() const;

    private:
        void initializeFirstTens(DoubleArray const& r0, DoubleArray const& r1, DoubleArray const& r2, DoubleArray const& r3,
                                 DoubleArray const& r4, DoubleArray const& r5, DoubleArray const& r6, DoubleArray const& r7,
                                 DoubleArray const& r8, DoubleArray const& r9);

        // ---------------------------- Varargs API ---------------------------------------------
        gint initialize(gint& i, DoubleArray const& r);
        gint initialize(gint& i, DoubleArray& r);
        gint initialize(gint& i, DoubleArray&& r);

        static void accumulate(...);
    };
} // core

#endif //CORE24_DOUBLEARRAY2D_H
