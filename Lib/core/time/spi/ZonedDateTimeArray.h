//
// Created by brunshweeck on 01/03/25.
//

#ifndef CORE24_ZONEDDATETIMEARRAY_H
#define CORE24_ZONEDDATETIMEARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/ZonedDateTime.h>

namespace core {
  namespace time {
    /**
     * The ZonedDateTimeArray class wrap the static array of values from native type
     * (generic) ZonedDateTime in an object. It's equivalent in C to ZonedDateTime[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class ZonedDateTimeArray final : public misc::ValueArray<ZonedDateTime> {
    public:
      /**
       * Construct new empty @c ZonedDateTimeArray
       */
      CORE_IMPLICIT ZonedDateTimeArray() CORE_NOTHROW;

      /**
       * Construct new @c ZonedDateTimeArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with value @c U+0000.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       */
      CORE_EXPLICIT ZonedDateTimeArray(gint length);

      /**
       * Construct new @c ZonedDateTimeArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with given initial value.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       * @param initialValue The value used to initialize all values of
       *          this array.
       */
      CORE_EXPLICIT ZonedDateTimeArray(gint length, ZonedDateTime const& initialValue);

      /**
       * Construct new @c ZonedDateTimeArray instance by copy of values
       * of given array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT ZonedDateTimeArray(ZonedDateTimeArray const& array);

      /**
       * Construct new @c ZonedDateTimeArray instance by swapping of content
       * of given array.
       *
       * @note After this construction, the given array will be
       * empty and this array will have the old length of given
       * array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT ZonedDateTimeArray(ZonedDateTimeArray&& array) CORE_NOTHROW;

      /**
       * Construct new @c ZonedDateTimeArray with specified values.
       *
       * @param args the list of values used to create array.
       */
      CORE_IMPLICIT ZonedDateTimeArray(VarArgs const& args);

      ZonedDateTimeArray& operator=(const ZonedDateTimeArray& other);

      ZonedDateTimeArray& operator=(ZonedDateTimeArray&& other) CORE_NOTHROW;

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
      ZonedDateTime& get(gint index) override;

      /**
       * Return the value of elements at specified index
       *
       * @param index The index of desired element.
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      ZonedDateTime const& get(gint index) const override;

      /**
       * Set value of element at specified index with
       * specified new value
       *
       * @param index The index of desired element.
       * @param newValue The replacement value
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      ZonedDateTime set(gint index, ZonedDateTime newValue) override;

      /**
       * Destroy this array data.
       *
       * @note This operation will clear content firstly.
       */
      ~ZonedDateTimeArray() override;

      static ZonedDateTimeArray copyOf(Array<ZonedDateTime> const& a);

      /**
       * Obtain newly created @c ZonedDateTimeArray instance with primitive ZonedDateTime array.
       *
       * @tparam T The array type
       * @param array The array to be copied
       * @return The new ZonedDateTimeArray that contains all values of given array
       */
      template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                Class<gbool>::OnlyIf<Class<ZonedDateTime>::isSame<typename Class<T>::ArrayElement>()>  = true>
      static ZonedDateTimeArray copyOf(T&& array) {
        CORE_FAST gint count = Class<T>::count();
        ZonedDateTimeArray zonedDateTimes = ZonedDateTimeArray(count);
        for (int i = 0; i < count; ++i) zonedDateTimes[i] = array[i];
        return zonedDateTimes;
      }

      /**
       * Return new empty array
       *
       * @return the new empty array
       */
      static ZonedDateTimeArray of();

      /**
       * Obtain new ZonedDateTimeArray containing one value.
       *
       * @param s0 the first zonedDateTime
       * @return newly created array with one value.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0);

      /**
       * Obtain new ZonedDateTimeArray containing two values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @return newly created array with two values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1);

      /**
       * Obtain new ZonedDateTimeArray containing three values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @return newly created array with three values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2);

      /**
       * Obtain new ZonedDateTimeArray containing four values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @param s3 the fourth zonedDateTime
       * @return newly created array with four values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                   ZonedDateTime const& s3);

      /**
       * Obtain new ZonedDateTimeArray containing five values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @param s3 the fourth zonedDateTime
       * @param s4 the fifth zonedDateTime
       * @return newly created array with five values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                   ZonedDateTime const& s3, ZonedDateTime const& s4);

      /**
       * Obtain new ZonedDateTimeArray containing six values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @param s3 the fourth zonedDateTime
       * @param s4 the fifth zonedDateTime
       * @param s5 the sixth zonedDateTime
       * @return newly created array with six values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                   ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5);

      /**
       * Obtain new ZonedDateTimeArray containing seven values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @param s3 the fourth zonedDateTime
       * @param s4 the fifth zonedDateTime
       * @param s5 the sixth zonedDateTime
       * @param s6 the seventh zonedDateTime
       * @return newly created array with seven values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                   ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                   ZonedDateTime const& s6);

      /**
       * Obtain new ZonedDateTimeArray containing eight values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @param s3 the fourth zonedDateTime
       * @param s4 the fifth zonedDateTime
       * @param s5 the sixth zonedDateTime
       * @param s6 the seventh zonedDateTime
       * @param s7 the eigth zonedDateTime
       * @return newly created array with eight values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                   ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                   ZonedDateTime const& s6, ZonedDateTime const& s7);

      /**
       * Obtain new ZonedDateTimeArray containing nine values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @param s3 the fourth zonedDateTime
       * @param s4 the fifth zonedDateTime
       * @param s5 the sixth zonedDateTime
       * @param s6 the seventh zonedDateTime
       * @param s7 the eigth zonedDateTime
       * @param s8 the ninth zonedDateTime
       * @return newly created array with nine values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                   ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                   ZonedDateTime const& s6, ZonedDateTime const& s7, ZonedDateTime const& s8);

      /**
       * Obtain new ZonedDateTimeArray containing ten values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @param s3 the fourth zonedDateTime
       * @param s4 the fifth zonedDateTime
       * @param s5 the sixth zonedDateTime
       * @param s6 the seventh zonedDateTime
       * @param s7 the eigth zonedDateTime
       * @param s8 the ninth zonedDateTime
       * @param s9 the tenth zonedDateTime
       * @return newly created array with ten values.
       */
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                   ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                   ZonedDateTime const& s6, ZonedDateTime const& s7, ZonedDateTime const& s8,
                                   ZonedDateTime const& s9);

      /**
       * Obtain new ZonedDateTimeArray containing ten+ values.
       *
       * @param s0 the first zonedDateTime
       * @param s1 the second zonedDateTime
       * @param s2 the third zonedDateTime
       * @param s3 the fourth zonedDateTime
       * @param s4 the fifth zonedDateTime
       * @param s5 the sixth zonedDateTime
       * @param s6 the seventh zonedDateTime
       * @param s7 the eigth zonedDateTime
       * @param s8 the ninth zonedDateTime
       * @param s9 the tenth zonedDateTime
       * @param s9 the tenth zonedDateTime
       * @param s the more zonedDateTimes
       * @return newly created array with ten+ values.
       */
      template <class... ZonedDateTimes>
      static ZonedDateTimeArray of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                   ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                   ZonedDateTime const& s6, ZonedDateTime const& s7, ZonedDateTime const& s8,
                                   ZonedDateTime const& s9, ZonedDateTimes&&... s) {
        ZonedDateTimeArray sa = ZonedDateTimeArray(10 + sizeof...(ZonedDateTimes));
        sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
        sa.writeZonedDateTimes(10, CORE_CAST(ZonedDateTimes &&, s)...);

        return sa;
      }

      ZonedDateTime& operator[](gint index);

      ZonedDateTime const& operator[](gint index) const;

    private:
      class LinearIterator final : public Object {
        CORE_ADD_AS_FRIEND(ZonedDateTimeArray);

        ZonedDateTimeArray& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator(ZonedDateTimeArray& array, gbool isEnd);

      public:
        LinearIterator& operator++();

        ZonedDateTime& operator*() const;

        gbool operator==(LinearIterator const& rhs) const;

        gbool operator!=(LinearIterator const& rhs) const;
      };

      class LinearIterator2 final : public Object {
        CORE_ADD_AS_FRIEND(ZonedDateTimeArray);

        ZonedDateTimeArray const& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator2(ZonedDateTimeArray const& array, gbool isEnd);

      public:
        LinearIterator2& operator++();

        ZonedDateTime const& operator*() const;

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

      void initializeFirstTeens(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                ZonedDateTime const& s3, ZonedDateTime const& s4,
                                ZonedDateTime const& s5, ZonedDateTime const& s6, ZonedDateTime const& s7,
                                ZonedDateTime const& s8, ZonedDateTime const& s9);

      template <class S>
      void writeAsZonedDateTime(gint idx, S&& s) {
        CORE_FAST_ASSERT(Class<ZonedDateTime>::accept<S>() || Class<S>::isZonedDateTime());
        writeAsZonedDateTime(idx, ""_S + s);
      }

      void writeAsZonedDateTime(gint idx, ZonedDateTime const& s);

      void writeZonedDateTimes(gint idx);

      template <class S0>
      void writeZonedDateTimes(gint idx, S0&& s0) { writeAsZonedDateTime(idx + 0, CORE_CAST(S0 &&, s0)); }

      template <class S0, class S1>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1) {
        writeAsZonedDateTime(idx + 0, CORE_CAST(S0 &&, s0));
        writeAsZonedDateTime(idx + 1, CORE_CAST(S1 &&, s1));
      }

      template <class S0, class S1, class S2>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
      }

      template <class S0, class S1, class S2, class S3>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeZonedDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
      }

      template <class S0, class S1, class S2, class S3, class S4>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeZonedDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeZonedDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
      }

      template <class S0, class S1, class S2, class S3, class S4, class S5>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeZonedDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeZonedDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeZonedDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeZonedDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeZonedDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeZonedDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeZonedDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeZonedDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeZonedDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeZonedDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeZonedDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeZonedDateTimes(idx + 7, CORE_CAST(S7 &&, s7));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeZonedDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeZonedDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeZonedDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeZonedDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeZonedDateTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeZonedDateTimes(idx + 8, CORE_CAST(S8 &&, s8));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeZonedDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeZonedDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeZonedDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeZonedDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeZonedDateTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeZonedDateTimes(idx + 8, CORE_CAST(S8 &&, s8));
        writeZonedDateTimes(idx + 9, CORE_CAST(S9 &&, s9));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9, class... S>
      void writeZonedDateTimes(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
        writeZonedDateTimes(idx + 0, CORE_CAST(S0 &&, s0));
        writeZonedDateTimes(idx + 1, CORE_CAST(S1 &&, s1));
        writeZonedDateTimes(idx + 2, CORE_CAST(S2 &&, s2));
        writeZonedDateTimes(idx + 3, CORE_CAST(S3 &&, s3));
        writeZonedDateTimes(idx + 4, CORE_CAST(S4 &&, s4));
        writeZonedDateTimes(idx + 5, CORE_CAST(S5 &&, s5));
        writeZonedDateTimes(idx + 6, CORE_CAST(S6 &&, s6));
        writeZonedDateTimes(idx + 7, CORE_CAST(S7 &&, s7));
        writeZonedDateTimes(idx + 8, CORE_CAST(S8 &&, s8));
        writeZonedDateTimes(idx + 9, CORE_CAST(S9 &&, s9));
        writeZonedDateTimes(idx + 10, CORE_CAST(S&&, s)...);
      }
    };
  } // time
} // core

#endif //CORE24_ZONEDDATETIMEARRAY_H
