//
// Created by brunshweeck on 24/02/25.
//

#ifndef CORE24_LOCALDATETIMEARRAY_H
#define CORE24_LOCALDATETIMEARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/LocalDateTime.h>

namespace core {
  namespace time {
    /**
     * The LocalDateTimeArray class wrap the static array of values from native type
     * (generic) LocalDateTime in an object. It's equivalent in C to LocalDateTime[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class LocalDateTimeArray final : public misc::ValueArray<LocalDateTime> {
    public:
      /**
       * Construct new empty @c LocalDateTimeArray
       */
      CORE_IMPLICIT LocalDateTimeArray() CORE_NOTHROW;

      /**
       * Construct new @c LocalDateTimeArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with value @c U+0000.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       */
      CORE_EXPLICIT LocalDateTimeArray(gint length);

      /**
       * Construct new @c LocalDateTimeArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with given initial value.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       * @param initialValue The value used to initialize all values of
       *          this array.
       */
      CORE_EXPLICIT LocalDateTimeArray(gint length, LocalDateTime const& initialValue);

      /**
       * Construct new @c LocalDateTimeArray instance by copy of values
       * of given array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT LocalDateTimeArray(LocalDateTimeArray const& array);

      /**
       * Construct new @c LocalDateTimeArray instance by swapping of content
       * of given array.
       *
       * @note After this construction, the given array will be
       * empty and this array will have the old length of given
       * array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT LocalDateTimeArray(LocalDateTimeArray&& array) CORE_NOTHROW;

      /**
       * Construct new @c LocalDateTimeArray with specified values.
       *
       * @param args the list of values used to create array.
       */
      CORE_IMPLICIT LocalDateTimeArray(VarArgs const& args);

      LocalDateTimeArray& operator=(const LocalDateTimeArray& other);

      LocalDateTimeArray& operator=(LocalDateTimeArray&& other) CORE_NOTHROW;

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
      LocalDateTime& get(gint index) override;

      /**
       * Return the value of elements at specified index
       *
       * @param index The index of desired element.
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      LocalDateTime const& get(gint index) const override;

      /**
       * Set value of element at specified index with
       * specified new value
       *
       * @param index The index of desired element.
       * @param newValue The replacement value
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      LocalDateTime set(gint index, LocalDateTime newValue) override;

      /**
       * Destroy this array data.
       *
       * @note This operation will clear content firstly.
       */
      ~LocalDateTimeArray() override;

      static LocalDateTimeArray copyOf(Array<LocalDateTime> const& a);

      /**
       * Obtain newly created @c LocalDateTimeArray instance with primitive LocalDateTime array.
       *
       * @tparam T The array type
       * @param array The array to be copied
       * @return The new LocalDateTimeArray that contains all values of given array
       */
      template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                Class<gbool>::OnlyIf<Class<LocalDateTime>::isSame<typename Class<T>::ArrayElement>()>  = true>
      static LocalDateTimeArray copyOf(T&& array) {
        CORE_FAST gint count = Class<T>::count();
        LocalDateTimeArray localDateTimes = LocalDateTimeArray(count);
        for (int i = 0; i < count; ++i) localDateTimes[i] = array[i];
        return localDateTimes;
      }

      /**
       * Return new empty array
       *
       * @return the new empty array
       */
      static LocalDateTimeArray of();

      /**
       * Obtain new LocalDateTimeArray containing one value.
       *
       * @param s0 the first localDateTime
       * @return newly created array with one value.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0);

      /**
       * Obtain new LocalDateTimeArray containing two values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @return newly created array with two values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1);

      /**
       * Obtain new LocalDateTimeArray containing three values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @return newly created array with three values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2);

      /**
       * Obtain new LocalDateTimeArray containing four values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @param s3 the fourth localDateTime
       * @return newly created array with four values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                   LocalDateTime const& s3);

      /**
       * Obtain new LocalDateTimeArray containing five values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @param s3 the fourth localDateTime
       * @param s4 the fifth localDateTime
       * @return newly created array with five values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                   LocalDateTime const& s3, LocalDateTime const& s4);

      /**
       * Obtain new LocalDateTimeArray containing six values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @param s3 the fourth localDateTime
       * @param s4 the fifth localDateTime
       * @param s5 the sixth localDateTime
       * @return newly created array with six values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                   LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5);

      /**
       * Obtain new LocalDateTimeArray containing seven values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @param s3 the fourth localDateTime
       * @param s4 the fifth localDateTime
       * @param s5 the sixth localDateTime
       * @param s6 the seventh localDateTime
       * @return newly created array with seven values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                   LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                   LocalDateTime const& s6);

      /**
       * Obtain new LocalDateTimeArray containing eight values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @param s3 the fourth localDateTime
       * @param s4 the fifth localDateTime
       * @param s5 the sixth localDateTime
       * @param s6 the seventh localDateTime
       * @param s7 the eigth localDateTime
       * @return newly created array with eight values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                   LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                   LocalDateTime const& s6, LocalDateTime const& s7);

      /**
       * Obtain new LocalDateTimeArray containing nine values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @param s3 the fourth localDateTime
       * @param s4 the fifth localDateTime
       * @param s5 the sixth localDateTime
       * @param s6 the seventh localDateTime
       * @param s7 the eigth localDateTime
       * @param s8 the ninth localDateTime
       * @return newly created array with nine values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                   LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                   LocalDateTime const& s6, LocalDateTime const& s7, LocalDateTime const& s8);

      /**
       * Obtain new LocalDateTimeArray containing ten values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @param s3 the fourth localDateTime
       * @param s4 the fifth localDateTime
       * @param s5 the sixth localDateTime
       * @param s6 the seventh localDateTime
       * @param s7 the eigth localDateTime
       * @param s8 the ninth localDateTime
       * @param s9 the tenth localDateTime
       * @return newly created array with ten values.
       */
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                   LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                   LocalDateTime const& s6, LocalDateTime const& s7, LocalDateTime const& s8,
                                   LocalDateTime const& s9);

      /**
       * Obtain new LocalDateTimeArray containing ten+ values.
       *
       * @param s0 the first localDateTime
       * @param s1 the second localDateTime
       * @param s2 the third localDateTime
       * @param s3 the fourth localDateTime
       * @param s4 the fifth localDateTime
       * @param s5 the sixth localDateTime
       * @param s6 the seventh localDateTime
       * @param s7 the eigth localDateTime
       * @param s8 the ninth localDateTime
       * @param s9 the tenth localDateTime
       * @param s9 the tenth localDateTime
       * @param s the more localDateTimes
       * @return newly created array with ten+ values.
       */
      template <class... LocalDateTimes>
      static LocalDateTimeArray of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                   LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                   LocalDateTime const& s6, LocalDateTime const& s7, LocalDateTime const& s8,
                                   LocalDateTime const& s9, LocalDateTimes&&... s) {
        LocalDateTimeArray sa = LocalDateTimeArray(10 + sizeof...(LocalDateTimes));
        sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
        sa.writeLocalDateTimes(10, CORE_CAST(LocalDateTimes &&, s)...);

        return sa;
      }

      LocalDateTime& operator[](gint index);

      LocalDateTime const& operator[](gint index) const;

    private:
      class LinearIterator final : public Object {
        CORE_ADD_AS_FRIEND(LocalDateTimeArray);

        LocalDateTimeArray& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator(LocalDateTimeArray& array, gbool isEnd);

      public:
        LinearIterator& operator++();

        LocalDateTime& operator*() const;

        gbool operator==(LinearIterator const& rhs) const;

        gbool operator!=(LinearIterator const& rhs) const;
      };

      class LinearIterator2 final : public Object {
        CORE_ADD_AS_FRIEND(LocalDateTimeArray);

        LocalDateTimeArray const& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator2(LocalDateTimeArray const& array, gbool isEnd);

      public:
        LinearIterator2& operator++();

        LocalDateTime const& operator*() const;

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

      void initializeFirstTeens(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                LocalDateTime const& s3, LocalDateTime const& s4,
                                LocalDateTime const& s5, LocalDateTime const& s6, LocalDateTime const& s7,
                                LocalDateTime const& s8, LocalDateTime const& s9);

      template <class S>
      void writeAsLocalDateTime(gint idx, S&& s) {
        CORE_FAST_ASSERT(Class<LocalDateTime>::accept<S>() || Class<S>::isLocalDateTime());
        writeAsLocalDateTime(idx, ""_S + s);
      }

      void writeAsLocalDateTime(gint idx, LocalDateTime const& s);

      void writeLocalDateTimes(gint idx);

      template <class S0>
      void writeLocalDateTimes(gint idx, S0&& s0) { writeAsLocalDateTime(idx + 0, CORE_CAST(S0 &&, s0)); }

      template <class S0, class S1>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1) {
        writeAsLocalDateTime(idx + 0, CORE_CAST(S0 &&, s0));
        writeAsLocalDateTime(idx + 1, CORE_CAST(S1 &&, s1));
      }

      template <class S0, class S1, class S2>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
      }

      template <class S0, class S1, class S2, class S3>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
      }

      template <class S0, class S1, class S2, class S3, class S4>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
      }

      template <class S0, class S1, class S2, class S3, class S4, class S5>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalDateTimes(idx + 7, CORE_CAST(S7 &&, s7));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalDateTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalDateTimes(idx + 8, CORE_CAST(S8 &&, s8));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalDateTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalDateTimes(idx + 8, CORE_CAST(S8 &&, s8));
        writeLocalDateTimes(idx + 9, CORE_CAST(S9 &&, s9));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9, class... S>
      void writeLocalDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
        writeLocalDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalDateTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalDateTimes(idx + 8, CORE_CAST(S8 &&, s8));
        writeLocalDateTimes(idx + 9, CORE_CAST(S9 &&, s9));
        writeLocalDateTimes(idx + 10, CORE_CAST(S&&, s)...);
      }
    };
  } // time
} // core

#endif //CORE24_LOCALDATETIMEARRAY_H
