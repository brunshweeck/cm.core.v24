//
// Created by brunshweeck on 24/02/25.
//

#ifndef CORE24_ZONEOFFSETARRAY_H
#define CORE24_ZONEOFFSETARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/ZoneOffset.h>

namespace core {
  namespace time {
    /**
     * The ZoneOffsetArray class wrap the static array of values from native type
     * (generic) ZoneOffset in an object. It's equivalent in C to ZoneOffset[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class ZoneOffsetArray final : public misc::ValueArray<ZoneOffset> {
    public:
      /**
       * Construct new empty @c ZoneOffsetArray
       */
      CORE_IMPLICIT ZoneOffsetArray() CORE_NOTHROW;

      /**
       * Construct new @c ZoneOffsetArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with value @c U+0000.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       */
      CORE_EXPLICIT ZoneOffsetArray(gint length);

      /**
       * Construct new @c ZoneOffsetArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with given initial value.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       * @param initialValue The value used to initialize all values of
       *          this array.
       */
      CORE_EXPLICIT ZoneOffsetArray(gint length, ZoneOffset const& initialValue);

      /**
       * Construct new @c ZoneOffsetArray instance by copy of values
       * of given array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT ZoneOffsetArray(ZoneOffsetArray const& array);

      /**
       * Construct new @c ZoneOffsetArray instance by swapping of content
       * of given array.
       *
       * @note After this construction, the given array will be
       * empty and this array will have the old length of given
       * array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT ZoneOffsetArray(ZoneOffsetArray&& array) CORE_NOTHROW;

      /**
       * Construct new @c ZoneOffsetArray with specified values.
       *
       * @param args the list of values used to create array.
       */
      CORE_IMPLICIT ZoneOffsetArray(VarArgs const& args);

      ZoneOffsetArray& operator=(const ZoneOffsetArray& other);

      ZoneOffsetArray& operator=(ZoneOffsetArray&& other) CORE_NOTHROW;

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
      ZoneOffset& get(gint index) override;

      /**
       * Return the value of elements at specified index
       *
       * @param index The index of desired element.
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      ZoneOffset const& get(gint index) const override;

      /**
       * Set value of element at specified index with
       * specified new value
       *
       * @param index The index of desired element.
       * @param newValue The replacement value
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      ZoneOffset set(gint index, ZoneOffset newValue) override;

      /**
       * Destroy this array data.
       *
       * @note This operation will clear content firstly.
       */
      ~ZoneOffsetArray() override;

      static ZoneOffsetArray copyOf(Array<ZoneOffset> const& a);

      /**
       * Obtain newly created @c ZoneOffsetArray instance with primitive ZoneOffset array.
       *
       * @tparam T The array type
       * @param array The array to be copied
       * @return The new ZoneOffsetArray that contains all values of given array
       */
      template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                Class<gbool>::OnlyIf<Class<ZoneOffset>::isSame<typename Class<T>::ArrayElement>()>  = true>
      static ZoneOffsetArray copyOf(T&& array) {
        CORE_FAST gint count = Class<T>::count();
        ZoneOffsetArray zoneOffsets = ZoneOffsetArray(count);
        for (int i = 0; i < count; ++i) zoneOffsets[i] = array[i];
        return zoneOffsets;
      }

      /**
       * Return new empty array
       *
       * @return the new empty array
       */
      static ZoneOffsetArray of();

      /**
       * Obtain new ZoneOffsetArray containing one value.
       *
       * @param s0 the first zoneOffset
       * @return newly created array with one value.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0);

      /**
       * Obtain new ZoneOffsetArray containing two values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @return newly created array with two values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1);

      /**
       * Obtain new ZoneOffsetArray containing three values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @return newly created array with three values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2);

      /**
       * Obtain new ZoneOffsetArray containing four values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @param s3 the fourth zoneOffset
       * @return newly created array with four values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3);

      /**
       * Obtain new ZoneOffsetArray containing five values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @param s3 the fourth zoneOffset
       * @param s4 the fifth zoneOffset
       * @return newly created array with five values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3, ZoneOffset const& s4);

      /**
       * Obtain new ZoneOffsetArray containing six values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @param s3 the fourth zoneOffset
       * @param s4 the fifth zoneOffset
       * @param s5 the sixth zoneOffset
       * @return newly created array with six values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5);

      /**
       * Obtain new ZoneOffsetArray containing seven values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @param s3 the fourth zoneOffset
       * @param s4 the fifth zoneOffset
       * @param s5 the sixth zoneOffset
       * @param s6 the seventh zoneOffset
       * @return newly created array with seven values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                ZoneOffset const& s6);

      /**
       * Obtain new ZoneOffsetArray containing eight values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @param s3 the fourth zoneOffset
       * @param s4 the fifth zoneOffset
       * @param s5 the sixth zoneOffset
       * @param s6 the seventh zoneOffset
       * @param s7 the eigth zoneOffset
       * @return newly created array with eight values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                ZoneOffset const& s6, ZoneOffset const& s7);

      /**
       * Obtain new ZoneOffsetArray containing nine values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @param s3 the fourth zoneOffset
       * @param s4 the fifth zoneOffset
       * @param s5 the sixth zoneOffset
       * @param s6 the seventh zoneOffset
       * @param s7 the eigth zoneOffset
       * @param s8 the ninth zoneOffset
       * @return newly created array with nine values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                ZoneOffset const& s6, ZoneOffset const& s7, ZoneOffset const& s8);

      /**
       * Obtain new ZoneOffsetArray containing ten values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @param s3 the fourth zoneOffset
       * @param s4 the fifth zoneOffset
       * @param s5 the sixth zoneOffset
       * @param s6 the seventh zoneOffset
       * @param s7 the eigth zoneOffset
       * @param s8 the ninth zoneOffset
       * @param s9 the tenth zoneOffset
       * @return newly created array with ten values.
       */
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                ZoneOffset const& s6, ZoneOffset const& s7, ZoneOffset const& s8,
                                ZoneOffset const& s9);

      /**
       * Obtain new ZoneOffsetArray containing ten+ values.
       *
       * @param s0 the first zoneOffset
       * @param s1 the second zoneOffset
       * @param s2 the third zoneOffset
       * @param s3 the fourth zoneOffset
       * @param s4 the fifth zoneOffset
       * @param s5 the sixth zoneOffset
       * @param s6 the seventh zoneOffset
       * @param s7 the eigth zoneOffset
       * @param s8 the ninth zoneOffset
       * @param s9 the tenth zoneOffset
       * @param s9 the tenth zoneOffset
       * @param s the more zoneOffsets
       * @return newly created array with ten+ values.
       */
      template <class... ZoneOffsets>
      static ZoneOffsetArray of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                ZoneOffset const& s6, ZoneOffset const& s7, ZoneOffset const& s8,
                                ZoneOffset const& s9, ZoneOffsets&&... s) {
        ZoneOffsetArray sa = ZoneOffsetArray(10 + sizeof...(ZoneOffsets));
        sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
        sa.writeZoneOffsets(10, CORE_CAST(ZoneOffsets &&, s)...);

        return sa;
      }

      ZoneOffset& operator[](gint index);

      ZoneOffset const& operator[](gint index) const;

    private:
      class LinearIterator final : public Object {
        CORE_ADD_AS_FRIEND(ZoneOffsetArray);

        ZoneOffsetArray& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator(ZoneOffsetArray& array, gbool isEnd);

      public:
        LinearIterator& operator++();

        ZoneOffset& operator*() const;

        gbool operator==(LinearIterator const& rhs) const;

        gbool operator!=(LinearIterator const& rhs) const;
      };

      class LinearIterator2 final : public Object {
        CORE_ADD_AS_FRIEND(ZoneOffsetArray);

        ZoneOffsetArray const& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator2(ZoneOffsetArray const& array, gbool isEnd);

      public:
        LinearIterator2& operator++();

        ZoneOffset const& operator*() const;

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

      void initializeFirstTeens(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                ZoneOffset const& s3, ZoneOffset const& s4,
                                ZoneOffset const& s5, ZoneOffset const& s6, ZoneOffset const& s7,
                                ZoneOffset const& s8, ZoneOffset const& s9);

      template <class S>
      void writeAsZoneOffset(gint idx, S&& s) {
        CORE_FAST_ASSERT(Class<ZoneOffset>::accept<S>() || Class<S>::isZoneOffset());
        writeAsZoneOffset(idx, ""_S + s);
      }

      void writeAsZoneOffset(gint idx, ZoneOffset const& s);

      void writeZoneOffsets(gint idx);

      template <class S0>
      void writeZoneOffsets(gint idx, S0&& s0) { writeAsZoneOffset(idx + 0, CORE_CAST(S0 &&, s0)); }

      template <class S0, class S1>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1) {
        writeAsZoneOffset(idx + 0, CORE_CAST(S0 &&, s0));
        writeAsZoneOffset(idx + 1, CORE_CAST(S1 &&, s1));
      }

      template <class S0, class S1, class S2>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
      }

      template <class S0, class S1, class S2, class S3>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsets(idx + 3, CORE_CAST(S3 &&, s3));
      }

      template <class S0, class S1, class S2, class S3, class S4>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsets(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsets(idx + 4, CORE_CAST(S4 &&, s4));
      }

      template <class S0, class S1, class S2, class S3, class S4, class S5>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                            S5&& s5) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsets(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsets(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsets(idx + 5, CORE_CAST(S5 &&, s5));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                            S5&& s5, S6&& s6) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsets(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsets(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsets(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsets(idx + 6, CORE_CAST(S6 &&, s6));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                            S5&& s5, S6&& s6, S7&& s7) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsets(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsets(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsets(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsets(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsets(idx + 7, CORE_CAST(S7 &&, s7));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                            S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsets(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsets(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsets(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsets(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsets(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsets(idx + 8, CORE_CAST(S8 &&, s8));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                            S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsets(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsets(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsets(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsets(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsets(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsets(idx + 8, CORE_CAST(S8 &&, s8));
        writeZoneOffsets(idx + 9, CORE_CAST(S9 &&, s9));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9, class... S>
      void writeZoneOffsets(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                            S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
        writeZoneOffsets(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsets(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsets(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsets(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsets(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsets(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsets(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsets(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsets(idx + 8, CORE_CAST(S8 &&, s8));
        writeZoneOffsets(idx + 9, CORE_CAST(S9 &&, s9));
        writeZoneOffsets(idx + 10, CORE_CAST(S&&, s)...);
      }
    };
  } // time
} // core

#endif //CORE24_ZONEOFFSETARRAY_H
