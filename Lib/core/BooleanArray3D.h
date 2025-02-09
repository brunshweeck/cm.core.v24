//
// Created by brunshweeck on 2024/09/14.
//

#ifndef CORE24_BOOLEANARRAY3D_H
#define CORE24_BOOLEANARRAY3D_H

#include <core/BooleanArray2D.h>

namespace core {
    /**
     * The @c BooleanArray3D class wrap the 3-Dimensional array of gbool.
     * It's the update and optimized form of primitive type @c gbool[N][M][X].
     *
     * @see BooleanArray2D
     * @see BooleanArray
     */
    class BooleanArray3D final : public misc::PrimitiveArray<Boolean>::Array3D<BooleanArray2D, BooleanArray> {
    public:
        /**
         * Construct new empty @c BooleanArray3D
         */
        CORE_IMPLICIT BooleanArray3D();

        /**
         * Construct new @c BooleanArray3D instance able to contains
         * the given number of rows.
         *
         * @note All rows will be initialized as empty @c BooleanArray2D
         *
         * @throws IllegalArgumentException If the given number of rows is negative
         * @param nbRows The number of rows to be allocated
         */
        CORE_EXPLICIT BooleanArray3D(gint nbRows);

        /**
         * Construct new @c BooleanArray3D instance able to contains
         * the given number of rows.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated
         * @param initializer the object used to initialize all rows.
         */
        CORE_EXPLICIT BooleanArray3D(gint nbRows, BooleanArray2D initializer);

        /**
         * Construct new @c BooleanArray3D instance able to contains
         * the given number of rows and columns.
         *
         * @note The content will be initialized as empty @c BooleanArray
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per rows to be allocated on this array.
         *
         * @see BooleanArray2D(nbRows)
         */
        CORE_EXPLICIT BooleanArray3D(gint nbRows, gint nbCols);

        /**
         * Construct new @c BooleanArray3D instance able to contains
         * the given number of rows and columns.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per rows to be allocated on this array.
         * @param initializer the object used to initialize all columns per rows
         */
        CORE_EXPLICIT BooleanArray3D(gint nbRows, gint nbCols, BooleanArray initializer);

        /**
         * Construct new @c BooleanArray3D instance able to contains
         * the given number of rows, columns per row and columns per column.
         * <p>
         *  - If the number of rows is zero, all other length will be ignored. <br/>
         *  - If the number of columns per row is zero, the number of columns per
         *      column will be ignored. <br/>
         *  - After construct the number of value supported by this array will
         *      be: @code nbRows * nbCols * nbSubCols @endcode <br/>
         *  - All values will be initialized as false.
         * </p>
         *
         * @throws IllegalArgumentException If one of following conditions is true: <br/>
         *                             - @c nbRows is negative. <br/>
         *                             - @c nbCols is negative. <br/>
         *                             - @c nbSubCols is negative.
         *
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per row to be allocated on this array
         * @param nbSubCols The number of columns per column of row to be allocated on this array
         */
        CORE_EXPLICIT BooleanArray3D(gint nbRows, gint nbCols, gint nbSubCols);

        /**
         * Construct new @c BooleanArray3D instance able to contains
         * the given number of rows, columns per row and columns per column.
         * <p>
         *  - If the number of rows is zero, all other length will be ignored. <br/>
         *  - If the number of columns per row is zero, the number of columns per
         *      column will be ignored. <br/>
         *  - After construct the number of value supported by this array will
         *      be: @code nbRows * nbCols * nbSubCols @endcode
         * </p>
         *
         * @throws IllegalArgumentException If one of following conditions is true: <br/>
         *                             - @c nbRows is negative. <br/>
         *                             - @c nbCols is negative. <br/>
         *                             - @c nbSubCols is negative.
         *
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per row to be allocated on this array
         * @param nbSubCols The number of columns per column of row to be allocated on this array
         * @param initializer the value used to initialize all content of this array.
         */
        CORE_EXPLICIT BooleanArray3D(gint nbRows, gint nbCols, gint nbSubCols, gbool initializer);

        /**
         * Construct new @c BooleanArray3D instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT BooleanArray3D(BooleanArray3D const& array);

        /**
         * Construct new @c BooleanArray3D instance by swapping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT BooleanArray3D(BooleanArray3D&& array) CORE_NOTHROW;

        /**
         * Construct new @c BooleanArray3D with specified rows
         * such that one rows is an instance of BooleanArray.
         * <p>
         * Example:
         * @code
         *  BooleanArray3D ba = {
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
         * @c BooleanArray3D::of(...) methods, And not do
         * implicit cast of values.
         *
         * @param args the list of rows used to create array.
         */
        CORE_IMPLICIT BooleanArray3D(VarArgs const& args);

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
        BooleanArray2D& get(gint index) override;

        /**
         * Return The array representing the nth row (where n = index)
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        BooleanArray2D const& get(gint index) const override;

        /**
         * Set the row at specified position
         *
         * @param index The index of desired row.
         * @param newRow The replacement row
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        BooleanArray2D set(gint index, BooleanArray2D newRow) override;

        /**
         * Return the sub-column at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        BooleanArray& get(gint ix, gint iy) override;

        /**
         * Return the sub-column at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        BooleanArray const& get(gint ix, gint iy) const override;

        /**
         * Set the column of row at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param newCol the replacement column
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        BooleanArray set(gint ix, gint iy, BooleanArray newCol) override;

        /**
         * Return the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param iz The index of desired sub-column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gbool& get(gint ix, gint iy, gint iz) override;

        /**
         * Return the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param iz The index of desired sub-column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gbool const& get(gint ix, gint iy, gint iz) const override;

        /**
         * Set the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column (sub-row).
         * @param iz the index of desired sub-column.
         * @param newValue the replacement value
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gbool set(gint ix, gint iy, gint iz, gbool newValue) override;

        /**
         * Destroy this array
         */
        ~BooleanArray3D() override;
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
        BooleanArray2D const& operator[](gint index) const;

        /**
         * Return the row of elements at specified index
         *
         * @param index The index of desired row.
         * @return the row at specified index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        BooleanArray2D& operator[](gint index);

        /**
         * Set the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        BooleanArray3D& operator=(BooleanArray3D const& array);

        /**
         * Swap the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        BooleanArray3D& operator=(BooleanArray3D&& array) CORE_NOTHROW;

        /**
         * Obtain newly created instance of @c BooleanArray3D initialized with zero rows
         *
         * @return empty bool array
         */
        static BooleanArray3D of();

        /**
         * Obtain newly created instance of @c BooleanArray3D with single row
         *
         * @param r The first row
         */
        static BooleanArray3D of(BooleanArray2D const& r);

        /**
         * Obtain newly created instance of @c BooleanArray3D with two rows
         *
         * @param r0 The first row
         * @param r1 The second row
         */
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1);

        /**
         * Obtain newly created instance of @c BooleanArray3D with three rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         */
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2);

        /**
         * Obtain newly created instance of @c BooleanArray3D with four rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         */
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                              BooleanArray2D const& r3);

        /**
         * Obtain newly created instance of @c BooleanArray3D with five rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         */
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                              BooleanArray2D const& r3,
                              BooleanArray2D const& r4);

        /**
         * Obtain newly created instance of @c BooleanArray3D with six rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         */
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                              BooleanArray2D const& r3,
                              BooleanArray2D const& r4, BooleanArray2D const& r5);

        /**
         * Obtain newly created instance of @c BooleanArray3D with seven rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         */
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                              BooleanArray2D const& r3,
                              BooleanArray2D const& r4, BooleanArray2D const& r5, BooleanArray2D const& r6);

        /**
         * Obtain newly created instance of @c BooleanArray3D with eight rows
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
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                              BooleanArray2D const& r3,
                              BooleanArray2D const& r4, BooleanArray2D const& r5, BooleanArray2D const& r6,
                              BooleanArray2D const& r7);

        /**
         * Obtain newly created instance of @c BooleanArray3D with nine rows
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
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                              BooleanArray2D const& r3,
                              BooleanArray2D const& r4, BooleanArray2D const& r5, BooleanArray2D const& r6,
                              BooleanArray2D const& r7,
                              BooleanArray2D const& r8);

        /**
         * Obtain newly created instance of @c BooleanArray3D with ten rows
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
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                              BooleanArray2D const& r3, BooleanArray2D const& r4, BooleanArray2D const& r5,
                              BooleanArray2D const& r6, BooleanArray2D const& r7, BooleanArray2D const& r8,
                              BooleanArray2D const& r9);

        /**
         * Obtain newly created instance of @c BooleanArray3D with ten and more rows
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
        static BooleanArray3D of(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                              BooleanArray2D const& r3, BooleanArray2D const& r4, BooleanArray2D const& r5,
                              BooleanArray2D const& r6, BooleanArray2D const& r7, BooleanArray2D const& r8,
                              BooleanArray2D const& r9, Rows&&... rs) {
            CORE_FAST_ASSERT(Class<>::allIsTrue<Class<BooleanArray2D>::accept<Rows>()...>());

            CORE_FAST gint n = sizeof...(Rows);
            gint wi = 10;

            BooleanArray3D ba = BooleanArray3D(10 + n);

            ba.initializeFirstTens(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
            accumulate(ba.initialize(wi, CORE_CAST(Rows &&, rs))...);

            return ba;
        }

        /**
         * Obtain new BooleanArray3D instance from Array<BooleanArray>
         *
         * @param a the reference array 3D
         * @return the value array 3D
         */
        static BooleanArray3D copyOf(Array<BooleanArray2D> const& a);

    private:
        // ---------------------------- Iterator API ---------------------------------------------

        class Linear3DIterator final : public Object {
            BooleanArray3D& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_ADD_AS_FRIEND(BooleanArray3D);

            CORE_EXPLICIT Linear3DIterator(BooleanArray3D& array, gbool isEnd);

        public:
            Linear3DIterator(Linear3DIterator const&) = delete;
            Linear3DIterator(Linear3DIterator&&) = default;

            Linear3DIterator& operator=(Linear3DIterator const&) = delete;
            Linear3DIterator& operator=(Linear3DIterator&&) = delete;

            ~Linear3DIterator() override = default;


            Linear3DIterator& operator++();

            BooleanArray2D& operator*() const;

            gbool equals(const Object& o) const override;
        };

        class Linear3DIterator2 final : public Object {
            BooleanArray3D const& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_ADD_AS_FRIEND(BooleanArray3D);

            CORE_EXPLICIT Linear3DIterator2(BooleanArray3D const& array, gbool isEnd);

        public:
            Linear3DIterator2(Linear3DIterator2 const&) = delete;
            Linear3DIterator2(Linear3DIterator2&&) = default;

            Linear3DIterator2& operator=(Linear3DIterator2 const&) = delete;
            Linear3DIterator2& operator=(Linear3DIterator2&&) = delete;

            ~Linear3DIterator2() override = default;


            Linear3DIterator2& operator++();

            BooleanArray2D const& operator*() const;

            gbool equals(const Object& o) const override;
        };

    public:
        Linear3DIterator begin();

        Linear3DIterator2 begin() const;

        Linear3DIterator end();

        Linear3DIterator2 end() const;

    private:
        void initializeFirstTens(BooleanArray2D const& r0, BooleanArray2D const& r1, BooleanArray2D const& r2,
                                 BooleanArray2D const& r3, BooleanArray2D const& r4, BooleanArray2D const& r5,
                                 BooleanArray2D const& r6, BooleanArray2D const& r7, BooleanArray2D const& r8,
                                 BooleanArray2D const& r9) const;

        // ---------------------------- Varargs API ---------------------------------------------
        gint initialize(gint& i, BooleanArray2D const& r);
        gint initialize(gint& i, BooleanArray2D& r);
        gint initialize(gint& i, BooleanArray2D&& r);

        static void accumulate(...);
    };
} // core

#endif //CORE24_BOOLEANARRAY3D_H
