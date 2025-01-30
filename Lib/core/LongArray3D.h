//
// Created by brunshweeck on 2024/09/14.
//

#ifndef CORE24_LONGARRAY3D_H
#define CORE24_LONGARRAY3D_H

#include <core/LongArray2D.h>

namespace core {
    /**
     * The @c LongArray3D class wrap the 3-Dimensional array of glong.
     * It's the update and optimized form of primitive type @c glong[N][M][X].
     *
     * @see LongArray2D
     * @see LongArray
     */
    class LongArray3D final : public misc::PrimitiveArray<Long>::Array3D<LongArray2D, LongArray> {
    public:
        /**
         * Construct new empty @c LongArray3D
         */
        CORE_IMPLICIT LongArray3D();

        /**
         * Construct new @c LongArray3D instance able to contains
         * the given number of rows.
         *
         * @note All rows will be initialized as empty @c LongArray2D
         *
         * @throws IllegalArgumentException If the given number of rows is negative
         * @param nbRows The number of rows to be allocated
         */
        CORE_EXPLICIT LongArray3D(gint nbRows);

        /**
         * Construct new @c LongArray3D instance able to contains
         * the given number of rows.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated
         * @param initializer the object used to initialize all rows.
         */
        CORE_EXPLICIT LongArray3D(gint nbRows, LongArray2D initializer);

        /**
         * Construct new @c LongArray3D instance able to contains
         * the given number of rows and columns.
         *
         * @note The content will be initialized as empty @c LongArray
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per rows to be allocated on this array.
         *
         * @see LongArray2D(nbRows)
         */
        CORE_EXPLICIT LongArray3D(gint nbRows, gint nbCols);

        /**
         * Construct new @c LongArray3D instance able to contains
         * the given number of rows and columns.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per rows to be allocated on this array.
         * @param initializer the object used to initialize all columns per rows
         */
        CORE_EXPLICIT LongArray3D(gint nbRows, gint nbCols, LongArray initializer);

        /**
         * Construct new @c LongArray3D instance able to contains
         * the given number of rows, columns per row and columns per column.
         * <p>
         *  @li If the number of rows is zero, all other length will be ignored.
         *  @li If the number of columns per row is zero, the number of columns per
         *      column will be ignored.
         *  @li After construct the number of value supported by this array will
         *      be: @code nbRows * nbCols * nbSubCols @endcode
         *  @li All values will be initialized as zero.
         * </p>
         *
         * @throws IllegalArgumentException If one of following conditions is true:
         *                             @li @c nbRows is negative.
         *                             @li @c nbCols is negative.
         *                             @li @c nbSubCols is negative.
         *
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per row to be allocated on this array
         * @param nbSubCols The number of columns per column of row to be allocated on this array
         */
        CORE_EXPLICIT LongArray3D(gint nbRows, gint nbCols, gint nbSubCols);

        /**
         * Construct new @c LongArray3D instance able to contains
         * the given number of rows, columns per row and columns per column.
         * <p>
         *  @li If the number of rows is zero, all other length will be ignored.
         *  @li If the number of columns per row is zero, the number of columns per
         *      column will be ignored.
         *  @li After construct the number of value supported by this array will
         *      be: @code nbRows * nbCols * nbSubCols @endcode
         * </p>
         *
         * @throws IllegalArgumentException If one of following conditions is true:
         *                             @li @c nbRows is negative.
         *                             @li @c nbCols is negative.
         *                             @li @c nbSubCols is negative.
         *
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per row to be allocated on this array
         * @param nbSubCols The number of columns per column of row to be allocated on this array
         * @param initializer the value used to initialize all content of this array.
         */
        CORE_EXPLICIT LongArray3D(gint nbRows, gint nbCols, gint nbSubCols, glong initializer);

        /**
         * Construct new @c LongArray3D instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT LongArray3D(LongArray3D const& array);

        /**
         * Construct new @c LongArray3D instance by swapping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT LongArray3D(LongArray3D&& array) CORE_NOTHROW;

        /**
         * Construct new @c LongArray3D with specified rows
         * such that one rows is an instance of LongArray.
         * <p>
         * Example:
         * @code
         *  LongArray3D la = {
         *     {
         *       {'a', 'b', ..., 'z'}, // 1st column
         *       ...
         *       {'a', 'b', ..., 'z'} // nth column
         *     }, // 1st row.
         *     ...
         *     {
         *       {'a', 'b', ..., 'z'}, // 1st column
         *       ...
         *       {'a', 'b', ..., 'z'} // nth column
         *     }, // nth row.
         *  }
         * @endcode
         * </p>
         *
         * @note This constructor is Equivalent to call of
         * @c LongArray3D::of(...) methods, And not do
         * implicit cast of values.
         *
         * @param args the list of rows used to create array.
         */
        CORE_IMPLICIT LongArray3D(VarArgs const& args);

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
        LongArray2D& get(gint index) override;

        /**
         * Return The array representing the nth row (where n = index)
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        LongArray2D const& get(gint index) const override;

        /**
         * Set the row at specified position
         *
         * @param index The index of desired row.
         * @param newRow The replacement row
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        LongArray2D set(gint index, LongArray2D newRow) override;

        /**
         * Return the sub-column at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        LongArray& get(gint ix, gint iy) override;

        /**
         * Return the sub-column at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        LongArray const& get(gint ix, gint iy) const override;

        /**
         * Set the column of row at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param newCol the replacement column
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        LongArray set(gint ix, gint iy, LongArray newCol) override;

        /**
         * Return the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param iz The index of desired sub-column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        glong& get(gint ix, gint iy, gint iz) override;

        /**
         * Return the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param iz The index of desired sub-column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        glong const& get(gint ix, gint iy, gint iz) const override;

        /**
         * Set the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column (sub-row).
         * @param iz the index of desired sub-column.
         * @param newValue the replacement value
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        glong set(gint ix, gint iy, gint iz, glong newValue) override;

        /**
         * Destroy this array
         */
        ~LongArray3D() override;
        /**
         * Return the string representation of this array.
         *
         * @return the string representation of this array.
         */
        String toString() const override;

        /**
         * Return the shadow copy of this array.
         *
         * @return the shadow copy of this array
         */
        Object& clone() const override;

        /**
         * Return the row of elements at specified index
         *
         * @param index The index of desired row.
         * @return the row at specified index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        LongArray2D const& operator[](gint index) const;

        /**
         * Return the row of elements at specified index
         *
         * @param index The index of desired row.
         * @return the row at specified index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        LongArray2D& operator[](gint index);

        /**
         * Set the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        LongArray3D& operator=(LongArray3D const& array);

        /**
         * Swap the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        LongArray3D& operator=(LongArray3D&& array) CORE_NOTHROW;

        /**
         * Obtain newly created instance of @c LongArray3D initialized with zero rows
         *
         * @return empty long array
         */
        static LongArray3D of();

        /**
         * Obtain newly created instance of @c LongArray3D with single row
         *
         * @param r The first row
         */
        static LongArray3D of(LongArray2D const& r);

        /**
         * Obtain newly created instance of @c LongArray3D with two rows
         *
         * @param r0 The first row
         * @param r1 The second row
         */
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1);

        /**
         * Obtain newly created instance of @c LongArray3D with three rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         */
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2);

        /**
         * Obtain newly created instance of @c LongArray3D with four rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         */
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                              LongArray2D const& r3);

        /**
         * Obtain newly created instance of @c LongArray3D with five rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         */
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                              LongArray2D const& r3,
                              LongArray2D const& r4);

        /**
         * Obtain newly created instance of @c LongArray3D with six rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         */
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                              LongArray2D const& r3,
                              LongArray2D const& r4, LongArray2D const& r5);

        /**
         * Obtain newly created instance of @c LongArray3D with seven rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         */
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                              LongArray2D const& r3,
                              LongArray2D const& r4, LongArray2D const& r5, LongArray2D const& r6);

        /**
         * Obtain newly created instance of @c LongArray3D with eight rows
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
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                              LongArray2D const& r3,
                              LongArray2D const& r4, LongArray2D const& r5, LongArray2D const& r6,
                              LongArray2D const& r7);

        /**
         * Obtain newly created instance of @c LongArray3D with nine rows
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
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                              LongArray2D const& r3,
                              LongArray2D const& r4, LongArray2D const& r5, LongArray2D const& r6,
                              LongArray2D const& r7,
                              LongArray2D const& r8);

        /**
         * Obtain newly created instance of @c LongArray3D with ten rows
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
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                              LongArray2D const& r3, LongArray2D const& r4, LongArray2D const& r5,
                              LongArray2D const& r6, LongArray2D const& r7, LongArray2D const& r8,
                              LongArray2D const& r9);

        /**
         * Obtain newly created instance of @c LongArray3D with ten and more rows
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
         * @param rs rest of rows
         */
        template <class... Rows>
        static LongArray3D of(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                              LongArray2D const& r3, LongArray2D const& r4, LongArray2D const& r5,
                              LongArray2D const& r6, LongArray2D const& r7, LongArray2D const& r8,
                              LongArray2D const& r9, Rows&&... rs) {
            CORE_FAST_ASSERT(Class<>::allIsTrue<Class<LongArray2D>::accept<Rows>()...>());

            CORE_FAST gint n = sizeof...(Rows);
            gint wi = 10;

            LongArray3D la = LongArray3D(10 + n);

            la.initializeFirstTens(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
            accumulate(la.initialize(wi, CORE_CAST(Rows &&, rs))...);

            return la;
        }

        /**
         * Obtain new LongArray3D instance from Array<LongArray>
         *
         * @param a the reference array 3D
         * @return the value array 3D
         */
        static LongArray3D copyOf(Array<LongArray2D> const& a);

    private:
        // ---------------------------- Iterator API ---------------------------------------------

        class Linear3DIterator final : public Object {
            LongArray3D& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_ADD_AS_FRIEND(LongArray3D);

            CORE_EXPLICIT Linear3DIterator(LongArray3D& array, gbool isEnd);

        public:
            Linear3DIterator(Linear3DIterator const&) = delete;
            Linear3DIterator(Linear3DIterator&&) = default;

            Linear3DIterator& operator=(Linear3DIterator const&) = delete;
            Linear3DIterator& operator=(Linear3DIterator&&) = delete;

            ~Linear3DIterator() override = default;


            Linear3DIterator& operator++();

            LongArray2D& operator*() const;

            gbool equals(const Object& o) const override;
        };

        class Linear3DIterator2 final : public Object {
            LongArray3D const& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_ADD_AS_FRIEND(LongArray3D);

            CORE_EXPLICIT Linear3DIterator2(LongArray3D const& array, gbool isEnd);

        public:
            Linear3DIterator2(Linear3DIterator2 const&) = delete;
            Linear3DIterator2(Linear3DIterator2&&) = default;

            Linear3DIterator2& operator=(Linear3DIterator2 const&) = delete;
            Linear3DIterator2& operator=(Linear3DIterator2&&) = delete;

            ~Linear3DIterator2() override = default;


            Linear3DIterator2& operator++();

            LongArray2D const& operator*() const;

            gbool equals(const Object& o) const override;
        };

    public:
        Linear3DIterator begin();

        Linear3DIterator2 begin() const;

        Linear3DIterator end();

        Linear3DIterator2 end() const;

    private:
        void initializeFirstTens(LongArray2D const& r0, LongArray2D const& r1, LongArray2D const& r2,
                                 LongArray2D const& r3, LongArray2D const& r4, LongArray2D const& r5,
                                 LongArray2D const& r6, LongArray2D const& r7, LongArray2D const& r8,
                                 LongArray2D const& r9) const;

        // ---------------------------- Varargs API ---------------------------------------------
        gint initialize(gint& i, LongArray2D const& r);
        gint initialize(gint& i, LongArray2D& r);
        gint initialize(gint& i, LongArray2D&& r);

        static void accumulate(...);
    };
} // core

#endif //CORE24_LONGARRAY3D_H
