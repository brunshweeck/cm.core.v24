//
// Created by brunshweeck on 8 sept. 2024.
//

#ifndef CORE24_CHARARRAY2D_H
#define CORE24_CHARARRAY2D_H

#include <core/lang/CharArray.h>

namespace core {
  /**
   * The @c CharArray class wrap the 2-Dimensional array of @c gchar.
   * It's the update and optimized form of primitive type @c gchar[N][M].
   */
  class CharArray2D final
      : public misc::PrimitiveArray<Character>::Array2D<CharArray> {
  public:
    /**
     * Construct new empty @c CharArray2D
     */
    CORE_IMPLICIT CharArray2D();

    /**
     * Construct new @c CharArray2D instance able to contains
     * the given number of rows. The row of this array represent
     * another array (@c CharArray), And all rows will be initializer
     * as empty array.
     *
     * @throws IllegalArgumentException If the given length is negative
     * @param nbRows The number of rows to be allocated
     */
    CORE_EXPLICIT CharArray2D(gint nbRows);

    /**
     * Construct new @c CharArray2D instance able to contains
     * the given number of rows and initialize all rows with
     * given initializer. The row of this array represent another
     * array (@c CharArray).
     *
     * @throws IllegalArgumentException If the given length is negative
     * @param nbRows The number of rows
     * @param initializer the array used to initialize all rows
     */
    CORE_EXPLICIT CharArray2D(gint nbRows, CharArray initializer);

    /**
     * Construct new @c CharArray2D instance able to contains
     * the given number of rows and given number of columns
     * per row. The row of this array represent another array
     * (@c CharArray).
     *
     * <p>
     *  During operation, every row will be initializer as
     *  array with length equals to given number of columns.
     *  And him values will be initializer with value @c '\u0000'
     * </p>
     *
     * @throws IllegalArgumentException If the given length is negative
     * @param nbRows The number of rows to be allocated on this array
     * @param nbCols The number of columns per row to be allocated on this array
     */
    CORE_EXPLICIT CharArray2D(gint nbRows, gint nbCols);

    /**
     * Construct new @c CharArray2D instance able to contains
     * the given number of rows and given number of columns
     * per row. The row of this array represent another array
     * (@c CharArray).
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
    CORE_EXPLICIT CharArray2D(gint nbRows, gint nbCols, gchar initializer);

    /**
     * Construct new @c CharArray2D instance by copy of values
     * of given array.
     *
     * @param array The array used to create this array.
     */
    CORE_IMPLICIT CharArray2D(CharArray2D const& array);

    /**
     * Construct new @c CharArray2D instance by swapping of content
     * of given array.
     *
     * @note After this construction, the given array will be
     * empty and this array will have the old length of given
     * array.
     *
     * @param array The array used to create this array.
     */
    CORE_IMPLICIT CharArray2D(CharArray2D&& array) CORE_NOTHROW;

    /**
     * Construct new @c CharArray2D with specified list of rows
     * such that one rows is an instance of CharArray.
     * <p>
     * Example:
     * @code
     *  CharArray2D ca = {
     *     {'a', 'b', ..., 'z'}, // 1st row.
     *     ...
     *     {'a', 'b', ..., 'z'}, // nth row.
     *  }
     * @endcode
     * </p>
     *
     * @note This constructor is Equivalent to call of
     * @c CharArray2D::of(...) methods, And not do
     * implicit cast of values.
     *
     * @param args the list of rows used to create array.
     */
    CORE_IMPLICIT CharArray2D(VarArgs const& args);

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
    CharArray& get(gint index) override;

    /**
     * Return The array representing the nth row (where n = index)
     *
     * @param index The index of desired element.
     * @throws IndexOutOfBoundsException If the given index out of bounds
     */
    CharArray const& get(gint index) const override;

    /**
     * Set the row at specified position
     *
     * @param index The index of desired element.
     * @param newRow The replacement row
     * @throws IndexOutOfBoundsException If the given index out of bounds
     */
    CharArray set(gint index, CharArray newRow) override;

    /**
     * Return the value of elements at specified index [ix, iy]
     *
     * @note This calling is equivalent to do:
     *       @code
     *         CharArray2D ca = ...;
     *         return ca[ix][iy];
     *       @endcode
     *
     * @param ix The index of desired row.
     * @param iy The index of desired column.
     * @return the value of elements at specified index [ix, iy]
     * @throws IndexOutOfBoundsException If one of the given index out of bounds
     */
    gchar& get(gint ix, gint iy) override;

    /**
     * Return the value of elements at specified index [ix, iy]
     *
     * @note This calling is equivalent to do:
     *       @code
     *         CharArray2D ca = ...;
     *         return ca[ix][iy];
     *       @endcode
     *
     * @param ix The index of desired row.
     * @param iy The index of desired column.
     * @return the value of elements at specified index [ix, iy]
     * @throws IndexOutOfBoundsException If one of the given index out of bounds
     */
    gchar const& get(gint ix, gint iy) const override;

    /**
     * Set the value of elements at specified index [ix, iy]
     *
     * @note This calling is equivalent to do:
     *       @code
     *         CharArray2D ca = ...;
     *         ca[ix].set(iy, newValue);
     *         // or
     *         ca[ix][iy] = newValue;
     *       @endcode
     *
     * @param ix The index of desired row.
     * @param iy The index of desired column.
     * @param newValue the replacement value.
     * @return the old value at given index
     * @throws IndexOutOfBoundsException If one of the given index out of bounds
     */
    gchar set(gint ix, gint iy, gchar newValue) override;

    /**
     * Destroy this array
     */
    ~CharArray2D() override;

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
    CharArray const& operator[](gint index) const;

    /**
     * Return the row of elements at specified index
     *
     * @param index The index of desired row.
     * @return the row at specified index
     * @throws IndexOutOfBoundsException If one of the given index out of bounds
     */
    CharArray& operator[](gint index);

    /**
     * Set the content of this array with content of another array.
     *
     * @param array the array that content will be used as replacement
     *        content.
     * @return this array reference.
     */
    CharArray2D& operator=(CharArray2D const& array);

    /**
     * Swap the content of this array with content of another array.
     *
     * @param array the array that content will be used as replacement
     *        content.
     * @return this array reference.
     */
    CharArray2D& operator=(CharArray2D&& array) CORE_NOTHROW;

    /**
     * Obtain newly created instance of @c CharArray2D initialized with zero rows
     *
     * @return empty char array
     */
    static CharArray of();

    /**
     * Obtain newly created instance of @c CharArray2D initialized with single row
     *
     * @param r The first row
     */
    static CharArray2D of(CharArray const& r);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with two rows
     *
     * @param r0 The first row
     * @param r1 The second row
     */
    static CharArray2D of(CharArray const& r0, CharArray const& r1);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with three rows
     *
     * @param r0 The first row
     * @param r1 The second row
     * @param r2 The third row
     */
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with four rows
     *
     * @param r0 The first row
     * @param r1 The second row
     * @param r2 The third row
     * @param r3 The fourth row
     */
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with five rows
     *
     * @param r0 The first row
     * @param r1 The second row
     * @param r2 The third row
     * @param r3 The fourth row
     * @param r4 The fiftieth row
     */
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                          CharArray const& r4);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with six rows
     *
     * @param r0 The first row
     * @param r1 The second row
     * @param r2 The third row
     * @param r3 The fourth row
     * @param r4 The fiftieth row
     * @param r5 The sixth row
     */
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                          CharArray const& r4, CharArray const& r5);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with seven rows
     *
     * @param r0 The first row
     * @param r1 The second row
     * @param r2 The third row
     * @param r3 The fourth row
     * @param r4 The fiftieth row
     * @param r5 The sixth row
     * @param r6 The seventh row
     */
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                          CharArray const& r4, CharArray const& r5, CharArray const& r6);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with eight rows
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
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                          CharArray const& r4, CharArray const& r5, CharArray const& r6, CharArray const& r7);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with nine rows
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
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                          CharArray const& r4, CharArray const& r5, CharArray const& r6, CharArray const& r7,
                          CharArray const& r8);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with ten rows
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
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                          CharArray const& r4, CharArray const& r5, CharArray const& r6, CharArray const& r7,
                          CharArray const& r8, CharArray const& r9);

    /**
     * Obtain newly created instance of @c CharArray2D initialized with ten and more rows
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
    static CharArray2D of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                          CharArray const& r4, CharArray const& r5, CharArray const& r6, CharArray const& r7,
                          CharArray const& r8, CharArray const& r9, Rows&&... rows) {
      CORE_FAST_ASSERT(Class<>::allIsTrue<Class<CharArray>::isSame<Rows>()...>());

      CORE_FAST gint n = sizeof...(Rows);
      gint wi = 10;

      CharArray2D ca = CharArray2D(10 + n);

      ca.initializeFirstTens(r0, r1, r2, r3, r4, r5, r6, r7, r8, r9);
      accumulate(ca.initialize(wi, CORE_CAST(Rows &&, rows))...);

      return ca;
    }

    /**
     * Obtain new CharArray2D instance from Array<CharArray>
     *
     * @param a the reference array 2D
     * @return the value array 2D
     */
    static CharArray2D copyOf(Array<CharArray> const& a);

  private:
    // ---------------------------- Iterator API ---------------------------------------------

    class Linear2DIterator final : public Object {
      CharArray2D& array;
      gint next = 0;
      gbool isEnd = false;

      CORE_ADD_AS_FRIEND(CharArray2D);

      CORE_EXPLICIT Linear2DIterator(CharArray2D& array, gbool isEnd);

    public:
      Linear2DIterator(Linear2DIterator const&) = delete;

      Linear2DIterator(Linear2DIterator&&) = default;

      Linear2DIterator& operator=(Linear2DIterator const&) = delete;

      Linear2DIterator& operator=(Linear2DIterator&&) = delete;

      ~Linear2DIterator() override = default;


      Linear2DIterator& operator++();

      CharArray& operator*() const;

      gbool equals(const Object& o) const override;
    };

    class Linear2DIterator2 final : public Object {
      CharArray2D const& array;
      gint next = 0;
      gbool isEnd = false;

      CORE_ADD_AS_FRIEND(CharArray2D);

      CORE_EXPLICIT Linear2DIterator2(CharArray2D const& array, gbool isEnd);

    public:
      Linear2DIterator2(Linear2DIterator2 const&) = delete;

      Linear2DIterator2(Linear2DIterator2&&) = default;

      Linear2DIterator2& operator=(Linear2DIterator2 const&) = delete;

      Linear2DIterator2& operator=(Linear2DIterator2&&) = delete;

      ~Linear2DIterator2() override = default;


      Linear2DIterator2& operator++();

      CharArray const& operator*() const;

      gbool equals(const Object& o) const override;
    };

  public:
    Linear2DIterator begin();

    Linear2DIterator2 begin() const;

    Linear2DIterator end();

    Linear2DIterator2 end() const;

  private:
    void initializeFirstTens(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                             CharArray const& r4, CharArray const& r5, CharArray const& r6, CharArray const& r7,
                             CharArray const& r8, CharArray const& r9);

    // ---------------------------- Varargs API ---------------------------------------------
    gint initialize(gint& i, CharArray const& r);

    gint initialize(gint& i, CharArray& r);

    gint initialize(gint& i, CharArray&& r);

    static void accumulate(...);
  };
} // core

#endif //CORE24_CHARARRAY2D_H
