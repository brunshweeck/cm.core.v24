//
// Created by brunshweeck on 2024/09/14.
//

#ifndef CORE24_BYTEARRAY3D_H
#define CORE24_BYTEARRAY3D_H

#include <core/lang/ByteArray2D.h>

namespace core {
    /**
     * The @c ByteArray3D class wrap the 3-Dimensional array of gbyte.
     * It's the update and optimized form of primitive type @c gbyte[N][M][X].
     *
     * @see ByteArray2D
     * @see ByteArray
     */
    class ByteArray3D final : public misc::PrimitiveArray<Byte>::Array3D<ByteArray2D, ByteArray> {
    public:
        /**
         * Construct new empty @c ByteArray3D
         */
        CORE_IMPLICIT ByteArray3D();

        /**
         * Construct new @c ByteArray3D instance able to contains
         * the given number of rows.
         *
         * @note All rows will be initialized as empty @c ByteArray2D
         *
         * @throws IllegalArgumentException If the given number of rows is negative
         * @param nbRows The number of rows to be allocated
         */
        CORE_EXPLICIT ByteArray3D(gint nbRows);

        /**
         * Construct new @c ByteArray3D instance able to contains
         * the given number of rows.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated
         * @param initializer the object used to initialize all rows.
         */
        CORE_EXPLICIT ByteArray3D(gint nbRows, ByteArray2D initializer);

        /**
         * Construct new @c ByteArray3D instance able to contains
         * the given number of rows and columns.
         *
         * @note The content will be initialized as empty @c ByteArray
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per rows to be allocated on this array.
         *
         * @see ByteArray2D(nbRows)
         */
        CORE_EXPLICIT ByteArray3D(gint nbRows, gint nbCols);

        /**
         * Construct new @c ByteArray3D instance able to contains
         * the given number of rows and columns.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param nbRows The number of rows to be allocated on this array
         * @param nbCols The number of columns per rows to be allocated on this array.
         * @param initializer the object used to initialize all columns per rows
         */
        CORE_EXPLICIT ByteArray3D(gint nbRows, gint nbCols, ByteArray initializer);

        /**
         * Construct new @c ByteArray3D instance able to contains
         * the given number of rows, columns per row and columns per column.
         * <p>
         *  - If the number of rows is zero, all other length will be ignored. <br/>
         *  - If the number of columns per row is zero, the number of columns per
         *      column will be ignored. <br/>
         *  - After construct the number of value supported by this array will
         *      be: @code nbRows * nbCols * nbSubCols @endcode <br/>
         *  - All values will be initialized as zero.
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
        CORE_EXPLICIT ByteArray3D(gint nbRows, gint nbCols, gint nbSubCols);

        /**
         * Construct new @c ByteArray3D instance able to contains
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
        CORE_EXPLICIT ByteArray3D(gint nbRows, gint nbCols, gint nbSubCols, gbyte initializer);

        /**
         * Construct new @c ByteArray3D instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT ByteArray3D(ByteArray3D const& array);

        /**
         * Construct new @c ByteArray3D instance by swapping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT ByteArray3D(ByteArray3D&& array) CORE_NOTHROW;

        /**
         * Construct new @c ByteArray3D with specified rows
         * such that one rows is an instance of ByteArray.
         * <p>
         * Example:
         * @code
         *  ByteArray3D ba = {
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
         * @c ByteArray3D::of(...) methods, And not do
         * implicit cast of values.
         *
         * @param args the list of rows used to create array.
         */
        CORE_IMPLICIT ByteArray3D(VarArgs const& args);

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
        ByteArray2D& get(gint index) override;

        /**
         * Return The array representing the nth row (where n = index)
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        ByteArray2D const& get(gint index) const override;

        /**
         * Set the row at specified position
         *
         * @param index The index of desired row.
         * @param newRow The replacement row
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        ByteArray2D set(gint index, ByteArray2D newRow) override;

        /**
         * Return the sub-column at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        ByteArray& get(gint ix, gint iy) override;

        /**
         * Return the sub-column at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        ByteArray const& get(gint ix, gint iy) const override;

        /**
         * Set the column of row at specified index [ix, iy]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param newCol the replacement column
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        ByteArray set(gint ix, gint iy, ByteArray newCol) override;

        /**
         * Return the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param iz The index of desired sub-column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gbyte& get(gint ix, gint iy, gint iz) override;

        /**
         * Return the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column.
         * @param iz The index of desired sub-column.
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gbyte const& get(gint ix, gint iy, gint iz) const override;

        /**
         * Set the value of sub-column at specified index [ix, iy, iz]
         *
         * @param ix The index of desired row.
         * @param iy The index of desired column (sub-row).
         * @param iz the index of desired sub-column.
         * @param newValue the replacement value
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        gbyte set(gint ix, gint iy, gint iz, gbyte newValue) override;

        /**
         * Destroy this array
         */
        ~ByteArray3D() override;
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
        ByteArray2D const& operator[](gint index) const;

        /**
         * Return the row of elements at specified index
         *
         * @param index The index of desired row.
         * @return the row at specified index
         * @throws IndexOutOfBoundsException If one of the given index out of bounds
         */
        ByteArray2D& operator[](gint index);

        /**
         * Set the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        ByteArray3D& operator=(ByteArray3D const& array);

        /**
         * Swap the content of this array with content of another array.
         *
         * @param array the array that content will be used as replacement
         *        content.
         * @return this array reference.
         */
        ByteArray3D& operator=(ByteArray3D&& array) CORE_NOTHROW;

        /**
         * Obtain newly created instance of @c ByteArray3D initialized with zero rows
         *
         * @return empty byte array
         */
        static ByteArray3D of();

        /**
         * Obtain newly created instance of @c ByteArray3D with single row
         *
         * @param r The first row
         */
        static ByteArray3D of(ByteArray2D const& r);

        /**
         * Obtain newly created instance of @c ByteArray3D with two rows
         *
         * @param r0 The first row
         * @param r1 The second row
         */
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1);

        /**
         * Obtain newly created instance of @c ByteArray3D with three rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         */
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2);

        /**
         * Obtain newly created instance of @c ByteArray3D with four rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         */
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                              ByteArray2D const& r3);

        /**
         * Obtain newly created instance of @c ByteArray3D with five rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         */
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                              ByteArray2D const& r3,
                              ByteArray2D const& r4);

        /**
         * Obtain newly created instance of @c ByteArray3D with six rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         */
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                              ByteArray2D const& r3,
                              ByteArray2D const& r4, ByteArray2D const& r5);

        /**
         * Obtain newly created instance of @c ByteArray3D with seven rows
         *
         * @param r0 The first row
         * @param r1 The second row
         * @param r2 The third row
         * @param r3 The fourth row
         * @param r4 The fiftieth row
         * @param r5 The sixth row
         * @param r6 The seventh row
         */
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                              ByteArray2D const& r3,
                              ByteArray2D const& r4, ByteArray2D const& r5, ByteArray2D const& r6);

        /**
         * Obtain newly created instance of @c ByteArray3D with eight rows
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
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                              ByteArray2D const& r3,
                              ByteArray2D const& r4, ByteArray2D const& r5, ByteArray2D const& r6,
                              ByteArray2D const& r7);

        /**
         * Obtain newly created instance of @c ByteArray3D with nine rows
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
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                              ByteArray2D const& r3,
                              ByteArray2D const& r4, ByteArray2D const& r5, ByteArray2D const& r6,
                              ByteArray2D const& r7,
                              ByteArray2D const& r8);

        /**
         * Obtain newly created instance of @c ByteArray3D with ten rows
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
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                              ByteArray2D const& r3, ByteArray2D const& r4, ByteArray2D const& r5,
                              ByteArray2D const& r6, ByteArray2D const& r7, ByteArray2D const& r8,
                              ByteArray2D const& r9);

        /**
         * Obtain newly created instance of @c ByteArray3D with ten and more rows
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
        static ByteArray3D of(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                              ByteArray2D const& r3, ByteArray2D const& r4, ByteArray2D const& r5,
                              ByteArray2D const& r6, ByteArray2D const& r7, ByteArray2D const& r8,
                              ByteArray2D const& r9, Rows&&... rs) {
            CORE_FAST_ASSERT(Class<>::allIsTrue<Class<ByteArray2D>::accept<Rows>()...>());

            CORE_FAST gint n = sizeof...(Rows);
            gint wi = 10;

            ByteArray3D ba = ByteArray3D(10 + n);

            ba.initializeFirstTens(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
            accumulate(ba.initialize(wi, CORE_CAST(Rows &&, rs))...);

            return ba;
        }

        /**
         * Obtain new ByteArray3D instance from Array<ByteArray>
         *
         * @param a the reference array 3D
         * @return the value array 3D
         */
        static ByteArray3D copyOf(Array<ByteArray2D> const& a);

    private:
        // ---------------------------- Iterator API ---------------------------------------------

        class Linear3DIterator final : public Object {
            ByteArray3D& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_ADD_AS_FRIEND(ByteArray3D);

            CORE_EXPLICIT Linear3DIterator(ByteArray3D& array, gbool isEnd);

        public:
            Linear3DIterator(Linear3DIterator const&) = delete;
            Linear3DIterator(Linear3DIterator&&) = default;

            Linear3DIterator& operator=(Linear3DIterator const&) = delete;
            Linear3DIterator& operator=(Linear3DIterator&&) = delete;

            ~Linear3DIterator() override = default;


            Linear3DIterator& operator++();

            ByteArray2D& operator*() const;

            gbool equals(const Object& o) const override;
        };

        class Linear3DIterator2 final : public Object {
            ByteArray3D const& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_ADD_AS_FRIEND(ByteArray3D);

            CORE_EXPLICIT Linear3DIterator2(ByteArray3D const& array, gbool isEnd);

        public:
            Linear3DIterator2(Linear3DIterator2 const&) = delete;
            Linear3DIterator2(Linear3DIterator2&&) = default;

            Linear3DIterator2& operator=(Linear3DIterator2 const&) = delete;
            Linear3DIterator2& operator=(Linear3DIterator2&&) = delete;

            ~Linear3DIterator2() override = default;


            Linear3DIterator2& operator++();

            ByteArray2D const& operator*() const;

            gbool equals(const Object& o) const override;
        };

    public:
        Linear3DIterator begin();

        Linear3DIterator2 begin() const;

        Linear3DIterator end();

        Linear3DIterator2 end() const;

    private:
        void initializeFirstTens(ByteArray2D const& r0, ByteArray2D const& r1, ByteArray2D const& r2,
                                 ByteArray2D const& r3, ByteArray2D const& r4, ByteArray2D const& r5,
                                 ByteArray2D const& r6, ByteArray2D const& r7, ByteArray2D const& r8,
                                 ByteArray2D const& r9) const;

        // ---------------------------- Varargs API ---------------------------------------------
        gint initialize(gint& i, ByteArray2D const& r);
        gint initialize(gint& i, ByteArray2D& r);
        gint initialize(gint& i, ByteArray2D&& r);

        static void accumulate(...);
    };
} // core

#endif //CORE24_BYTEARRAY3D_H
