//
// Created by brunshweeck on 01/03/25.
//

#ifndef CORE24_LOCALDATEARRAY_H
#define CORE24_LOCALDATEARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/LocalDate.h>

namespace core {
  namespace time {
    /**
     * The LocalDateArray class wrap the static array of values from native type
     * (generic) LocalDate in an object. It's equivalent in C to LocalDate[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class LocalDateArray final : public misc::ValueArray<LocalDate> {
    public:
      /**
       * Construct new empty @c LocalDateArray
       */
      CORE_IMPLICIT LocalDateArray() CORE_NOTHROW;

      /**
       * Construct new @c LocalDateArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with value @c U+0000.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       */
      CORE_EXPLICIT LocalDateArray(gint length);

      /**
       * Construct new @c LocalDateArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with given initial value.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       * @param initialValue The value used to initialize all values of
       *          this array.
       */
      CORE_EXPLICIT LocalDateArray(gint length, LocalDate const& initialValue);

      /**
       * Construct new @c LocalDateArray instance by copy of values
       * of given array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT LocalDateArray(LocalDateArray const& array);

      /**
       * Construct new @c LocalDateArray instance by swapping of content
       * of given array.
       *
       * @note After this construction, the given array will be
       * empty and this array will have the old length of given
       * array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT LocalDateArray(LocalDateArray&& array) CORE_NOTHROW;

      /**
       * Construct new @c LocalDateArray with specified values.
       *
       * @param args the list of values used to create array.
       */
      CORE_IMPLICIT LocalDateArray(VarArgs const& args);

      LocalDateArray& operator=(const LocalDateArray& other);

      LocalDateArray& operator=(LocalDateArray&& other) CORE_NOTHROW;

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
      LocalDate& get(gint index) override;

      /**
       * Return the value of elements at specified index
       *
       * @param index The index of desired element.
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      LocalDate const& get(gint index) const override;

      /**
       * Set value of element at specified index with
       * specified new value
       *
       * @param index The index of desired element.
       * @param newValue The replacement value
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      LocalDate set(gint index, LocalDate newValue) override;

      /**
       * Destroy this array data.
       *
       * @note This operation will clear content firstly.
       */
      ~LocalDateArray() override;

      static LocalDateArray copyOf(Array<LocalDate> const& a);

      /**
       * Obtain newly created @c LocalDateArray instance with primitive LocalDate array.
       *
       * @tparam T The array type
       * @param array The array to be copied
       * @return The new LocalDateArray that contains all values of given array
       */
      template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                Class<gbool>::OnlyIf<Class<LocalDate>::isSame<typename Class<T>::ArrayElement>()>  = true>
      static LocalDateArray copyOf(T&& array) {
        CORE_FAST gint count = Class<T>::count();
        LocalDateArray localDates = LocalDateArray(count);
        for (int i = 0; i < count; ++i) localDates[i] = array[i];
        return localDates;
      }

      /**
       * Return new empty array
       *
       * @return the new empty array
       */
      static LocalDateArray of();

      /**
       * Obtain new LocalDateArray containing one value.
       *
       * @param s0 the first localDate
       * @return newly created array with one value.
       */
      static LocalDateArray of(LocalDate const& s0);

      /**
       * Obtain new LocalDateArray containing two values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @return newly created array with two values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1);

      /**
       * Obtain new LocalDateArray containing three values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @return newly created array with three values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2);

      /**
       * Obtain new LocalDateArray containing four values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @param s3 the fourth localDate
       * @return newly created array with four values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                   LocalDate const& s3);

      /**
       * Obtain new LocalDateArray containing five values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @param s3 the fourth localDate
       * @param s4 the fifth localDate
       * @return newly created array with five values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                   LocalDate const& s3, LocalDate const& s4);

      /**
       * Obtain new LocalDateArray containing six values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @param s3 the fourth localDate
       * @param s4 the fifth localDate
       * @param s5 the sixth localDate
       * @return newly created array with six values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                   LocalDate const& s3, LocalDate const& s4, LocalDate const& s5);

      /**
       * Obtain new LocalDateArray containing seven values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @param s3 the fourth localDate
       * @param s4 the fifth localDate
       * @param s5 the sixth localDate
       * @param s6 the seventh localDate
       * @return newly created array with seven values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                   LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                   LocalDate const& s6);

      /**
       * Obtain new LocalDateArray containing eight values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @param s3 the fourth localDate
       * @param s4 the fifth localDate
       * @param s5 the sixth localDate
       * @param s6 the seventh localDate
       * @param s7 the eigth localDate
       * @return newly created array with eight values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                   LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                   LocalDate const& s6, LocalDate const& s7);

      /**
       * Obtain new LocalDateArray containing nine values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @param s3 the fourth localDate
       * @param s4 the fifth localDate
       * @param s5 the sixth localDate
       * @param s6 the seventh localDate
       * @param s7 the eigth localDate
       * @param s8 the ninth localDate
       * @return newly created array with nine values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                   LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                   LocalDate const& s6, LocalDate const& s7, LocalDate const& s8);

      /**
       * Obtain new LocalDateArray containing ten values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @param s3 the fourth localDate
       * @param s4 the fifth localDate
       * @param s5 the sixth localDate
       * @param s6 the seventh localDate
       * @param s7 the eigth localDate
       * @param s8 the ninth localDate
       * @param s9 the tenth localDate
       * @return newly created array with ten values.
       */
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                   LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                   LocalDate const& s6, LocalDate const& s7, LocalDate const& s8,
                                   LocalDate const& s9);

      /**
       * Obtain new LocalDateArray containing ten+ values.
       *
       * @param s0 the first localDate
       * @param s1 the second localDate
       * @param s2 the third localDate
       * @param s3 the fourth localDate
       * @param s4 the fifth localDate
       * @param s5 the sixth localDate
       * @param s6 the seventh localDate
       * @param s7 the eigth localDate
       * @param s8 the ninth localDate
       * @param s9 the tenth localDate
       * @param s9 the tenth localDate
       * @param s the more localDates
       * @return newly created array with ten+ values.
       */
      template <class... LocalDates>
      static LocalDateArray of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                   LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                   LocalDate const& s6, LocalDate const& s7, LocalDate const& s8,
                                   LocalDate const& s9, LocalDates&&... s) {
        LocalDateArray sa = LocalDateArray(10 + sizeof...(LocalDates));
        sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
        sa.writeLocalDates(10, CORE_CAST(LocalDates &&, s)...);

        return sa;
      }

      LocalDate& operator[](gint index);

      LocalDate const& operator[](gint index) const;

    private:
      class LinearIterator final : public Object {
        CORE_ADD_AS_FRIEND(LocalDateArray);

        LocalDateArray& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator(LocalDateArray& array, gbool isEnd);

      public:
        LinearIterator& operator++();

        LocalDate& operator*() const;

        gbool operator==(LinearIterator const& rhs) const;

        gbool operator!=(LinearIterator const& rhs) const;
      };

      class LinearIterator2 final : public Object {
        CORE_ADD_AS_FRIEND(LocalDateArray);

        LocalDateArray const& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator2(LocalDateArray const& array, gbool isEnd);

      public:
        LinearIterator2& operator++();

        LocalDate const& operator*() const;

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

    private:
      // -------------------------- Varargs API ---------------------------------------------

      void initializeFirstTeens(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                LocalDate const& s3, LocalDate const& s4,
                                LocalDate const& s5, LocalDate const& s6, LocalDate const& s7,
                                LocalDate const& s8, LocalDate const& s9);

      template <class S>
      void writeAsLocalDate(gint idx, S&& s) {
        CORE_FAST_ASSERT(Class<LocalDate>::accept<S>() || Class<S>::isLocalDate());
        writeAsLocalDate(idx, ""_S + s);
      }

      void writeAsLocalDate(gint idx, LocalDate const& s);

      void writeLocalDates(gint idx);

      template <class S0>
      void writeLocalDates(gint idx, S0&& s0) { writeAsLocalDate(idx + 0, CORE_CAST(S0 &&, s0)); }

      template <class S0, class S1>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1) {
        writeAsLocalDate(idx + 0, CORE_CAST(S0 &&, s0));
        writeAsLocalDate(idx + 1, CORE_CAST(S1 &&, s1));
      }

      template <class S0, class S1, class S2>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
      }

      template <class S0, class S1, class S2, class S3>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDates(idx + 3, CORE_CAST(S3 &&, s3));
      }

      template <class S0, class S1, class S2, class S3, class S4>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDates(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDates(idx + 4, CORE_CAST(S4 &&, s4));
      }

      template <class S0, class S1, class S2, class S3, class S4, class S5>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDates(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDates(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDates(idx + 5, CORE_CAST(S5 &&, s5));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDates(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDates(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDates(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDates(idx + 6, CORE_CAST(S6 &&, s6));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDates(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDates(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDates(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDates(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalDates(idx + 7, CORE_CAST(S7 &&, s7));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDates(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDates(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDates(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDates(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalDates(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalDates(idx + 8, CORE_CAST(S8 &&, s8));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDates(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDates(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDates(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDates(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalDates(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalDates(idx + 8, CORE_CAST(S8 &&, s8));
        writeLocalDates(idx + 9, CORE_CAST(S9 &&, s9));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9, class... S>
      void writeLocalDates(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
        writeLocalDates(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDates(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDates(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDates(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDates(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDates(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDates(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalDates(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalDates(idx + 8, CORE_CAST(S8 &&, s8));
        writeLocalDates(idx + 9, CORE_CAST(S9 &&, s9));
        writeLocalDates(idx + 10, CORE_CAST(S&&, s)...);
      }
    };
  } // time
} // core

#endif //CORE24_LOCALDATEARRAY_H
