//
// Created by brunshweeck on 01/03/25.
//

#ifndef CORE24_ZONEOFFSETTRANSITIONARRAY_H
#define CORE24_ZONEOFFSETTRANSITIONARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/ZoneOffsetTransition.h>

namespace core {
  namespace time {
    /**
     * The ZoneOffsetTransitionArray class wrap the static array of values from native type
     * (generic) ZoneOffsetTransition in an object. It's equivalent in C to ZoneOffsetTransition[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class ZoneOffsetTransitionArray final : public misc::ValueArray<ZoneOffsetTransition> {
    public:
      /**
       * Construct new empty @c ZoneOffsetTransitionArray
       */
      CORE_IMPLICIT ZoneOffsetTransitionArray() CORE_NOTHROW;

      /**
       * Construct new @c ZoneOffsetTransitionArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with value @c U+0000.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       */
      CORE_EXPLICIT ZoneOffsetTransitionArray(gint length);

      /**
       * Construct new @c ZoneOffsetTransitionArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with given initial value.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       * @param initialValue The value used to initialize all values of
       *          this array.
       */
      CORE_EXPLICIT ZoneOffsetTransitionArray(gint length, ZoneOffsetTransition const& initialValue);

      /**
       * Construct new @c ZoneOffsetTransitionArray instance by copy of values
       * of given array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT ZoneOffsetTransitionArray(ZoneOffsetTransitionArray const& array);

      /**
       * Construct new @c ZoneOffsetTransitionArray instance by swapping of content
       * of given array.
       *
       * @note After this construction, the given array will be
       * empty and this array will have the old length of given
       * array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT ZoneOffsetTransitionArray(ZoneOffsetTransitionArray&& array) CORE_NOTHROW;

      /**
       * Construct new @c ZoneOffsetTransitionArray with specified values.
       *
       * @param args the list of values used to create array.
       */
      CORE_IMPLICIT ZoneOffsetTransitionArray(VarArgs const& args);

      ZoneOffsetTransitionArray& operator=(const ZoneOffsetTransitionArray& other);

      ZoneOffsetTransitionArray& operator=(ZoneOffsetTransitionArray&& other) CORE_NOTHROW;

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
      ZoneOffsetTransition& get(gint index) override;

      /**
       * Return the value of elements at specified index
       *
       * @param index The index of desired element.
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      ZoneOffsetTransition const& get(gint index) const override;

      /**
       * Set value of element at specified index with
       * specified new value
       *
       * @param index The index of desired element.
       * @param newValue The replacement value
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      ZoneOffsetTransition set(gint index, ZoneOffsetTransition newValue) override;

      /**
       * Destroy this array data.
       *
       * @note This operation will clear content firstly.
       */
      ~ZoneOffsetTransitionArray() override;

      static ZoneOffsetTransitionArray copyOf(Array<ZoneOffsetTransition> const& a);

      /**
       * Obtain newly created @c ZoneOffsetTransitionArray instance with primitive ZoneOffsetTransition array.
       *
       * @tparam T The array type
       * @param array The array to be copied
       * @return The new ZoneOffsetTransitionArray that contains all values of given array
       */
      template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                Class<gbool>::OnlyIf<Class<ZoneOffsetTransition>::isSame<typename Class<T>::ArrayElement>()>  =
                    true>
      static ZoneOffsetTransitionArray copyOf(T&& array) {
        CORE_FAST gint count = Class<T>::count();
        ZoneOffsetTransitionArray zoneOffsetTransitions = ZoneOffsetTransitionArray(count);
        for (int i = 0; i < count; ++i) zoneOffsetTransitions[i] = array[i];
        return zoneOffsetTransitions;
      }

      /**
       * Return new empty array
       *
       * @return the new empty array
       */
      static ZoneOffsetTransitionArray of();

      /**
       * Obtain new ZoneOffsetTransitionArray containing one value.
       *
       * @param s0 the first zoneOffsetTransition
       * @return newly created array with one value.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0);

      /**
       * Obtain new ZoneOffsetTransitionArray containing two values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @return newly created array with two values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1);

      /**
       * Obtain new ZoneOffsetTransitionArray containing three values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @return newly created array with three values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2);

      /**
       * Obtain new ZoneOffsetTransitionArray containing four values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @param s3 the fourth zoneOffsetTransition
       * @return newly created array with four values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2,
                                              ZoneOffsetTransition const& s3);

      /**
       * Obtain new ZoneOffsetTransitionArray containing five values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @param s3 the fourth zoneOffsetTransition
       * @param s4 the fifth zoneOffsetTransition
       * @return newly created array with five values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2,
                                              ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4);

      /**
       * Obtain new ZoneOffsetTransitionArray containing six values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @param s3 the fourth zoneOffsetTransition
       * @param s4 the fifth zoneOffsetTransition
       * @param s5 the sixth zoneOffsetTransition
       * @return newly created array with six values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2,
                                              ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4,
                                              ZoneOffsetTransition const& s5);

      /**
       * Obtain new ZoneOffsetTransitionArray containing seven values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @param s3 the fourth zoneOffsetTransition
       * @param s4 the fifth zoneOffsetTransition
       * @param s5 the sixth zoneOffsetTransition
       * @param s6 the seventh zoneOffsetTransition
       * @return newly created array with seven values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2,
                                              ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4,
                                              ZoneOffsetTransition const& s5,
                                              ZoneOffsetTransition const& s6);

      /**
       * Obtain new ZoneOffsetTransitionArray containing eight values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @param s3 the fourth zoneOffsetTransition
       * @param s4 the fifth zoneOffsetTransition
       * @param s5 the sixth zoneOffsetTransition
       * @param s6 the seventh zoneOffsetTransition
       * @param s7 the eigth zoneOffsetTransition
       * @return newly created array with eight values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2,
                                              ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4,
                                              ZoneOffsetTransition const& s5,
                                              ZoneOffsetTransition const& s6, ZoneOffsetTransition const& s7);

      /**
       * Obtain new ZoneOffsetTransitionArray containing nine values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @param s3 the fourth zoneOffsetTransition
       * @param s4 the fifth zoneOffsetTransition
       * @param s5 the sixth zoneOffsetTransition
       * @param s6 the seventh zoneOffsetTransition
       * @param s7 the eigth zoneOffsetTransition
       * @param s8 the ninth zoneOffsetTransition
       * @return newly created array with nine values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2,
                                              ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4,
                                              ZoneOffsetTransition const& s5,
                                              ZoneOffsetTransition const& s6, ZoneOffsetTransition const& s7,
                                              ZoneOffsetTransition const& s8);

      /**
       * Obtain new ZoneOffsetTransitionArray containing ten values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @param s3 the fourth zoneOffsetTransition
       * @param s4 the fifth zoneOffsetTransition
       * @param s5 the sixth zoneOffsetTransition
       * @param s6 the seventh zoneOffsetTransition
       * @param s7 the eigth zoneOffsetTransition
       * @param s8 the ninth zoneOffsetTransition
       * @param s9 the tenth zoneOffsetTransition
       * @return newly created array with ten values.
       */
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2,
                                              ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4,
                                              ZoneOffsetTransition const& s5,
                                              ZoneOffsetTransition const& s6, ZoneOffsetTransition const& s7,
                                              ZoneOffsetTransition const& s8,
                                              ZoneOffsetTransition const& s9);

      /**
       * Obtain new ZoneOffsetTransitionArray containing ten+ values.
       *
       * @param s0 the first zoneOffsetTransition
       * @param s1 the second zoneOffsetTransition
       * @param s2 the third zoneOffsetTransition
       * @param s3 the fourth zoneOffsetTransition
       * @param s4 the fifth zoneOffsetTransition
       * @param s5 the sixth zoneOffsetTransition
       * @param s6 the seventh zoneOffsetTransition
       * @param s7 the eigth zoneOffsetTransition
       * @param s8 the ninth zoneOffsetTransition
       * @param s9 the tenth zoneOffsetTransition
       * @param s9 the tenth zoneOffsetTransition
       * @param s the more zoneOffsetTransitions
       * @return newly created array with ten+ values.
       */
      template <class... ZoneOffsetTransitions>
      static ZoneOffsetTransitionArray of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                              ZoneOffsetTransition const& s2,
                                              ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4,
                                              ZoneOffsetTransition const& s5,
                                              ZoneOffsetTransition const& s6, ZoneOffsetTransition const& s7,
                                              ZoneOffsetTransition const& s8,
                                              ZoneOffsetTransition const& s9, ZoneOffsetTransitions&&... s) {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(10 + sizeof...(ZoneOffsetTransitions));
        sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
        sa.writeZoneOffsetTransitions(10, CORE_CAST(ZoneOffsetTransitions &&, s)...);

        return sa;
      }

      ZoneOffsetTransition& operator[](gint index);

      ZoneOffsetTransition const& operator[](gint index) const;

    private:
      class LinearIterator final : public Object {
        CORE_ADD_AS_FRIEND(ZoneOffsetTransitionArray);

        ZoneOffsetTransitionArray& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator(ZoneOffsetTransitionArray& array, gbool isEnd);

      public:
        LinearIterator& operator++();

        ZoneOffsetTransition& operator*() const;

        gbool operator==(LinearIterator const& rhs) const;

        gbool operator!=(LinearIterator const& rhs) const;
      };

      class LinearIterator2 final : public Object {
        CORE_ADD_AS_FRIEND(ZoneOffsetTransitionArray);

        ZoneOffsetTransitionArray const& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator2(ZoneOffsetTransitionArray const& array, gbool isEnd);

      public:
        LinearIterator2& operator++();

        ZoneOffsetTransition const& operator*() const;

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

      void initializeFirstTeens(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1,
                                ZoneOffsetTransition const& s2,
                                ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4,
                                ZoneOffsetTransition const& s5, ZoneOffsetTransition const& s6,
                                ZoneOffsetTransition const& s7,
                                ZoneOffsetTransition const& s8, ZoneOffsetTransition const& s9);

      template <class S>
      void writeAsZoneOffsetTransition(gint idx, S&& s) {
        CORE_FAST_ASSERT(Class<ZoneOffsetTransition>::accept<S>() || Class<S>::isZoneOffsetTransition());
        writeAsZoneOffsetTransition(idx, ""_S + s);
      }

      void writeAsZoneOffsetTransition(gint idx, ZoneOffsetTransition const& s);

      void writeZoneOffsetTransitions(gint idx);

      template <class S0>
      void writeZoneOffsetTransitions(gint idx, S0&& s0) {
        writeAsZoneOffsetTransition(idx + 0, CORE_CAST(S0 &&, s0));
      }

      template <class S0, class S1>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1) {
        writeAsZoneOffsetTransition(idx + 0, CORE_CAST(S0 &&, s0));
        writeAsZoneOffsetTransition(idx + 1, CORE_CAST(S1 &&, s1));
      }

      template <class S0, class S1, class S2>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
      }

      template <class S0, class S1, class S2, class S3>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitions(idx + 3, CORE_CAST(S3 &&, s3));
      }

      template <class S0, class S1, class S2, class S3, class S4>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitions(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitions(idx + 4, CORE_CAST(S4 &&, s4));
      }

      template <class S0, class S1, class S2, class S3, class S4, class S5>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitions(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitions(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitions(idx + 5, CORE_CAST(S5 &&, s5));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitions(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitions(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitions(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitions(idx + 6, CORE_CAST(S6 &&, s6));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6, S7&& s7) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitions(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitions(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitions(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitions(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsetTransitions(idx + 7, CORE_CAST(S7 &&, s7));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitions(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitions(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitions(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitions(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsetTransitions(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsetTransitions(idx + 8, CORE_CAST(S8 &&, s8));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitions(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitions(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitions(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitions(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsetTransitions(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsetTransitions(idx + 8, CORE_CAST(S8 &&, s8));
        writeZoneOffsetTransitions(idx + 9, CORE_CAST(S9 &&, s9));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9, class... S>
      void writeZoneOffsetTransitions(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
        writeZoneOffsetTransitions(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitions(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitions(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitions(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitions(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitions(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitions(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsetTransitions(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsetTransitions(idx + 8, CORE_CAST(S8 &&, s8));
        writeZoneOffsetTransitions(idx + 9, CORE_CAST(S9 &&, s9));
        writeZoneOffsetTransitions(idx + 10, CORE_CAST(S&&, s)...);
      }
    };
  } // time
} // core

#endif //CORE24_ZONEOFFSETTRANSITIONARRAY_H
