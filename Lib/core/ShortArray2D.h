//
// Created by brunshweeck on 2024/09/14.
//

#ifndef CORE24_SHORTARRAY2D_H
#define CORE24_SHORTARRAY2D_H

#include <core/ShortArray.h>

namespace core {
    /**
     * The @c ShortArray class wrap the 2-Dimensional array of @c gshort. it's
     * the update and optimized form of primitive type @c gshort[N][M].
     */
    class ShortArray2D final
        : public misc::PrimitiveArray<Short>::Array2D<ShortArray> {
    public:
        /**
         * Construct new empty @c ShortArray2D
         */
        CORE_IMPLICIT ShortArray2D();

        /**
         * Construct new @c ShortArray2D instance able to contains
         * the given number of rows. The row of this array represent
         * another array (@c ShortArray), And all rows will be initializer
         * as empty array.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated
         */
        CORE_EXPLICIT ShortArray2D(gint nbRows);

        /**
         * Construct new @c ShortArray2D instance able to contains
         * the given number of rows and initialize all rows with
         * given initializer. The row of this array represent another
         * array (@c ShortArray).
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows
         * @param initializer the array used to initialize all rows
         */
        CORE_EXPLICIT ShortArray2D(gint nbRows, ShortArray initializer);

        /**
         * Construct new @c ShortArray2D instance able to contains
         * the given number of rows and given number of columns
         * per row. The row of this array represent another array
         * (@c ShortArray).
         *
         * <p>
         *  During operation, every row will be initializer as
         *  array with length equals to given number of columns.
         *  And him values will be initializer with value @c 0.
         * </p>
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per row to be allocated on this array
         */
        CORE_EXPLICIT ShortArray2D(gint nbRows, gint nbCols);

        /**
         * Construct new @c ShortArray2D instance able to contains
         * the given number of rows and given number of columns
         * per row. The row of this array represent another array
         * (@c ShortArray).
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
        CORE_EXPLICIT ShortArray2D(gint nbRows, gint nbCols, gshort initializer);

        /**
         * Construct new @c ShortArray2D instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT ShortArray2D(ShortArray2D const& array);

        /**
         * Construct new @c ShortArray2D instance by swapping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT ShortArray2D(ShortArray2D&& array) CORE_NOTHROW;

        /**
         * Construct new @c ShortArray2D with specified list of rows
         * such that one rows is an instance of ShortArray.
         * <p>
         * Example:
         * @code
         *  ShortArray2D sa = {
         *     {'a', 'b', ..., 'z'}, // 1st row.
         *     ...
         *     {'a', 'b', ..., 'z'}, // nth row.
         *  }
         * @endcode
         * </p>
         *
         * @note This constructor is Equivalent to call of
         * @c ShortArray2D::of(...) methods, And not do
         * implicit cast of values.
         *
         * @param args the list of rows used to create array.
         */
        CORE_IMPLICIT ShortArray2D(VarArgs const& args);

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
        ShortArray& get(gint index) override;

        /**
         * Return The array representing the nth row (where n = index)
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        ShortArray const& get(gint index) const override;

        /**
         * Set the row at specified position
         *
         * @param index The index of desired element.
         * @param newRow The replacement row
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        ShortArray set(gint index, ShortArray newRow) override;

        /**
         * Return the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         ShortArray2D sa = ...;
         *         return sa[ix][iy];
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @return the value of elements at specified index [ix, iy]
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gshort& get(gint ix, gint iy) override;

        /**
         * Return the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         ShortArray2D sa = ...;
         *         return sa[ix][iy];
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @return the value of elements at specified index [ix, iy]
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gshort const& get(gint ix, gint iy) const override;

        /**
         * Set the value of elements at specified index [ix, iy]
         *
         * @note This calling is equivalent to do:
         *       @code
         *         ShortArray2D sa = ...;
         *         sa[ix].set(iy, newValue);
         *         // or
         *         sa[ix][iy] = newValue;
         *       @endcode
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param newValue the replacement value.
         * @return the old value at given index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gshort set(gint ix, gint iy, gshort newValue) override;

        /**
         * Destroy this array
         */
        ~ShortArray2D() override;

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
        ShortArray const& operator[](gint index) const;

        /**
         * Return the row of elements at specified index
         *
         * @param index The index of desired row.
         * @return the row at specified index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        ShortArray& operator[](gint index);

        /**
         * Set the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        ShortArray2D& operator=(ShortArray2D const& array);

        /**
         * Swap the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        ShortArray2D& operator=(ShortArray2D&& array) CORE_NOTHROW;

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with zero rows
         *
         * @return empty short array
         */
        static ShortArray of();

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with single row
         *
         * @param r The first row
         */
        static ShortArray2D of(ShortArray const& r);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with two rows
         *
         * @param r0 The first row
         * @param r1 The second row
         */
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with three rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         */
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with four rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         */
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with five rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         */
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3,
                              ShortArray const& r4);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with six rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         */
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3,
                              ShortArray const& r4, ShortArray const& r5);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with seven rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         */
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3,
                              ShortArray const& r4, ShortArray const& r5, ShortArray const& r6);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with eight rows
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
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3,
                              ShortArray const& r4, ShortArray const& r5, ShortArray const& r6, ShortArray const& r7);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with nine rows
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
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3,
                              ShortArray const& r4, ShortArray const& r5, ShortArray const& r6, ShortArray const& r7,
                              ShortArray const& r8);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with ten rows
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
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3,
                              ShortArray const& r4, ShortArray const& r5, ShortArray const& r6, ShortArray const& r7,
                              ShortArray const& r8, ShortArray const& r9);

        /**
         * Obtain newly created instance of @c ShortArray2D initialized with ten and more rows
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
        static ShortArray2D of(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3,
                              ShortArray const& r4, ShortArray const& r5, ShortArray const& r6, ShortArray const& r7,
                              ShortArray const& r8, ShortArray const& r9, Rows&&... rows) {
            CORE_FAST_ASSERT(Class<>::allIsTrue<Class<ShortArray>::isSame<Rows>()...>());

            CORE_FAST gint n = sizeof...(Rows);
            gint wi = 10;

            ShortArray2D sa = ShortArray2D(10 + n);

            sa.initializeFirstTens(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
            accumulate(sa.initialize(wi, CORE_CAST(Rows &&, rows))...);

            return sa;
        }

        /**
         * Obtain new ShortArray2D instance from Array<ShortArray>
         *
         * @param a the reference array 2D
         * @return the value array 2D
         */
     static ShortArray2D copyOf(Array<ShortArray> const& a);

    private:
     // ---------------------------- Iterator API ---------------------------------------------

     class Linear2DIterator final : public Object {
      ShortArray2D& array;
      gint next = 0;
      gbool isEnd = false;

      CORE_ADD_AS_FRIEND(ShortArray2D);

      CORE_EXPLICIT Linear2DIterator(ShortArray2D& array, gbool isEnd);

     public:
      Linear2DIterator(Linear2DIterator const&) = delete;
      Linear2DIterator(Linear2DIterator&&) = default;

      Linear2DIterator& operator=(Linear2DIterator const&) = delete;
      Linear2DIterator& operator=(Linear2DIterator&&) = delete;

      ~Linear2DIterator() override = default;


      Linear2DIterator& operator++();

      ShortArray& operator*() const;

      gbool equals(const Object& o) const override;
     };

     class Linear2DIterator2 final : public Object {
      ShortArray2D const& array;
      gint next = 0;
      gbool isEnd = false;

      CORE_ADD_AS_FRIEND(ShortArray2D);

      CORE_EXPLICIT Linear2DIterator2(ShortArray2D const& array, gbool isEnd);

     public:
      Linear2DIterator2(Linear2DIterator2 const&) = delete;
      Linear2DIterator2(Linear2DIterator2&&) = default;

      Linear2DIterator2& operator=(Linear2DIterator2 const&) = delete;
      Linear2DIterator2& operator=(Linear2DIterator2&&) = delete;

      ~Linear2DIterator2() override = default;


      Linear2DIterator2& operator++();

      ShortArray const& operator*() const;

      gbool equals(const Object& o) const override;
     };

    public:
     Linear2DIterator begin();

     Linear2DIterator2 begin() const;

     Linear2DIterator end();

     Linear2DIterator2 end() const;

    private:
        void initializeFirstTens(ShortArray const& r0, ShortArray const& r1, ShortArray const& r2, ShortArray const& r3,
                                 ShortArray const& r4, ShortArray const& r5, ShortArray const& r6, ShortArray const& r7,
                                 ShortArray const& r8, ShortArray const& r9);

        // ---------------------------- Varargs API ---------------------------------------------
        gint initialize(gint& i, ShortArray const& r);
        gint initialize(gint& i, ShortArray& r);
        gint initialize(gint& i, ShortArray&& r);

        static void accumulate(...);
    };
} // core

#endif //CORE24_SHORTARRAY2D_H
