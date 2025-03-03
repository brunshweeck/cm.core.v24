//
// Created by brunshweeck on 01/03/25.
//

#ifndef CORE24_LOCALTIMEARRAY_H
#define CORE24_LOCALTIMEARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/LocalTime.h>

namespace core {
  namespace time {
    /**
     * The LocalTimeArray class wrap the static array of values from native type
     * (generic) LocalTime in an object. It's equivalent in C to LocalTime[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class LocalTimeArray final : public misc::ValueArray<LocalTime> {
    public:
      /**
       * Construct new empty @c LocalTimeArray
       */
      CORE_IMPLICIT LocalTimeArray() CORE_NOTHROW;

      /**
       * Construct new @c LocalTimeArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with value @c U+0000.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       */
      CORE_EXPLICIT LocalTimeArray(gint length);

      /**
       * Construct new @c LocalTimeArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with given initial value.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       * @param initialValue The value used to initialize all values of
       *          this array.
       */
      CORE_EXPLICIT LocalTimeArray(gint length, LocalTime const& initialValue);

      /**
       * Construct new @c LocalTimeArray instance by copy of values
       * of given array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT LocalTimeArray(LocalTimeArray const& array);

      /**
       * Construct new @c LocalTimeArray instance by swapping of content
       * of given array.
       *
       * @note After this construction, the given array will be
       * empty and this array will have the old length of given
       * array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT LocalTimeArray(LocalTimeArray&& array) CORE_NOTHROW;

      /**
       * Construct new @c LocalTimeArray with specified values.
       *
       * @param args the list of values used to create array.
       */
      CORE_IMPLICIT LocalTimeArray(VarArgs const& args);

      LocalTimeArray& operator=(const LocalTimeArray& other);

      LocalTimeArray& operator=(LocalTimeArray&& other) CORE_NOTHROW;

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
      LocalTime& get(gint index) override;

      /**
       * Return the value of elements at specified index
       *
       * @param index The index of desired element.
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      LocalTime const& get(gint index) const override;

      /**
       * Set value of element at specified index with
       * specified new value
       *
       * @param index The index of desired element.
       * @param newValue The replacement value
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      LocalTime set(gint index, LocalTime newValue) override;

      /**
       * Destroy this array data.
       *
       * @note This operation will clear content firstly.
       */
      ~LocalTimeArray() override;

      static LocalTimeArray copyOf(Array<LocalTime> const& a);

      /**
       * Obtain newly created @c LocalTimeArray instance with primitive LocalTime array.
       *
       * @tparam T The array type
       * @param array The array to be copied
       * @return The new LocalTimeArray that contains all values of given array
       */
      template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                Class<gbool>::OnlyIf<Class<LocalTime>::isSame<typename Class<T>::ArrayElement>()>  = true>
      static LocalTimeArray copyOf(T&& array) {
        CORE_FAST gint count = Class<T>::count();
        LocalTimeArray localTimes = LocalTimeArray(count);
        for (int i = 0; i < count; ++i) localTimes[i] = array[i];
        return localTimes;
      }

      /**
       * Return new empty array
       *
       * @return the new empty array
       */
      static LocalTimeArray of();

      /**
       * Obtain new LocalTimeArray containing one value.
       *
       * @param s0 the first localTime
       * @return newly created array with one value.
       */
      static LocalTimeArray of(LocalTime const& s0);

      /**
       * Obtain new LocalTimeArray containing two values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @return newly created array with two values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1);

      /**
       * Obtain new LocalTimeArray containing three values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @return newly created array with three values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2);

      /**
       * Obtain new LocalTimeArray containing four values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @param s3 the fourth localTime
       * @return newly created array with four values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                   LocalTime const& s3);

      /**
       * Obtain new LocalTimeArray containing five values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @param s3 the fourth localTime
       * @param s4 the fifth localTime
       * @return newly created array with five values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                   LocalTime const& s3, LocalTime const& s4);

      /**
       * Obtain new LocalTimeArray containing six values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @param s3 the fourth localTime
       * @param s4 the fifth localTime
       * @param s5 the sixth localTime
       * @return newly created array with six values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                   LocalTime const& s3, LocalTime const& s4, LocalTime const& s5);

      /**
       * Obtain new LocalTimeArray containing seven values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @param s3 the fourth localTime
       * @param s4 the fifth localTime
       * @param s5 the sixth localTime
       * @param s6 the seventh localTime
       * @return newly created array with seven values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                   LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                   LocalTime const& s6);

      /**
       * Obtain new LocalTimeArray containing eight values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @param s3 the fourth localTime
       * @param s4 the fifth localTime
       * @param s5 the sixth localTime
       * @param s6 the seventh localTime
       * @param s7 the eigth localTime
       * @return newly created array with eight values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                   LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                   LocalTime const& s6, LocalTime const& s7);

      /**
       * Obtain new LocalTimeArray containing nine values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @param s3 the fourth localTime
       * @param s4 the fifth localTime
       * @param s5 the sixth localTime
       * @param s6 the seventh localTime
       * @param s7 the eigth localTime
       * @param s8 the ninth localTime
       * @return newly created array with nine values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                   LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                   LocalTime const& s6, LocalTime const& s7, LocalTime const& s8);

      /**
       * Obtain new LocalTimeArray containing ten values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @param s3 the fourth localTime
       * @param s4 the fifth localTime
       * @param s5 the sixth localTime
       * @param s6 the seventh localTime
       * @param s7 the eigth localTime
       * @param s8 the ninth localTime
       * @param s9 the tenth localTime
       * @return newly created array with ten values.
       */
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                   LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                   LocalTime const& s6, LocalTime const& s7, LocalTime const& s8,
                                   LocalTime const& s9);

      /**
       * Obtain new LocalTimeArray containing ten+ values.
       *
       * @param s0 the first localTime
       * @param s1 the second localTime
       * @param s2 the third localTime
       * @param s3 the fourth localTime
       * @param s4 the fifth localTime
       * @param s5 the sixth localTime
       * @param s6 the seventh localTime
       * @param s7 the eigth localTime
       * @param s8 the ninth localTime
       * @param s9 the tenth localTime
       * @param s9 the tenth localTime
       * @param s the more localTimes
       * @return newly created array with ten+ values.
       */
      template <class... LocalTimes>
      static LocalTimeArray of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                   LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                   LocalTime const& s6, LocalTime const& s7, LocalTime const& s8,
                                   LocalTime const& s9, LocalTimes&&... s) {
        LocalTimeArray sa = LocalTimeArray(10 + sizeof...(LocalTimes));
        sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
        sa.writeLocalTimes(10, CORE_CAST(LocalTimes &&, s)...);

        return sa;
      }

      LocalTime& operator[](gint index);

      LocalTime const& operator[](gint index) const;

    private:
      class LinearIterator final : public Object {
        CORE_ADD_AS_FRIEND(LocalTimeArray);

        LocalTimeArray& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator(LocalTimeArray& array, gbool isEnd);

      public:
        LinearIterator& operator++();

        LocalTime& operator*() const;

        gbool operator==(LinearIterator const& rhs) const;

        gbool operator!=(LinearIterator const& rhs) const;
      };

      class LinearIterator2 final : public Object {
        CORE_ADD_AS_FRIEND(LocalTimeArray);

        LocalTimeArray const& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator2(LocalTimeArray const& array, gbool isEnd);

      public:
        LinearIterator2& operator++();

        LocalTime const& operator*() const;

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

      void initializeFirstTeens(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                LocalTime const& s3, LocalTime const& s4,
                                LocalTime const& s5, LocalTime const& s6, LocalTime const& s7,
                                LocalTime const& s8, LocalTime const& s9);

      template <class S>
      void writeAsLocalTime(gint idx, S&& s) {
        CORE_FAST_ASSERT(Class<LocalTime>::accept<S>() || Class<S>::isLocalTime());
        writeAsLocalTime(idx, ""_S + s);
      }

      void writeAsLocalTime(gint idx, LocalTime const& s);

      void writeLocalTimes(gint idx);

      template <class S0>
      void writeLocalTimes(gint idx, S0&& s0) { writeAsLocalTime(idx + 0, CORE_CAST(S0 &&, s0)); }

      template <class S0, class S1>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1) {
        writeAsLocalTime(idx + 0, CORE_CAST(S0 &&, s0));
        writeAsLocalTime(idx + 1, CORE_CAST(S1 &&, s1));
      }

      template <class S0, class S1, class S2>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
      }

      template <class S0, class S1, class S2, class S3>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalTimes(idx + 3, CORE_CAST(S3 &&, s3));
      }

      template <class S0, class S1, class S2, class S3, class S4>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalTimes(idx + 4, CORE_CAST(S4 &&, s4));
      }

      template <class S0, class S1, class S2, class S3, class S4, class S5>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalTimes(idx + 5, CORE_CAST(S5 &&, s5));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalTimes(idx + 6, CORE_CAST(S6 &&, s6));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalTimes(idx + 7, CORE_CAST(S7 &&, s7));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalTimes(idx + 8, CORE_CAST(S8 &&, s8));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalTimes(idx + 8, CORE_CAST(S8 &&, s8));
        writeLocalTimes(idx + 9, CORE_CAST(S9 &&, s9));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9, class... S>
      void writeLocalTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
        writeLocalTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeLocalTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeLocalTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeLocalTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeLocalTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeLocalTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeLocalTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeLocalTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeLocalTimes(idx + 8, CORE_CAST(S8 &&, s8));
        writeLocalTimes(idx + 9, CORE_CAST(S9 &&, s9));
        writeLocalTimes(idx + 10, CORE_CAST(S&&, s)...);
      }
    };
  } // time
} // core

#endif //CORE24_LOCALTIMEARRAY_H
