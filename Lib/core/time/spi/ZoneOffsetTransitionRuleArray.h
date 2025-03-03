//
// Created by brunshweeck on 24/02/25.
//

#ifndef CORE24_ZONEOFFSETTRANSITIONRULEARRAY_H
#define CORE24_ZONEOFFSETTRANSITIONRULEARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/ZoneOffsetTransitionRule.h>

namespace core {
  namespace time {
    /**
     * The ZoneOffsetTransitionRuleArray class wrap the static array of values from native type
     * (generic) ZoneOffsetTransitionRule in an object. It's equivalent in C to ZoneOffsetTransitionRule[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class ZoneOffsetTransitionRuleArray final : public misc::ValueArray<ZoneOffsetTransitionRule> {
    public:
      /**
       * Construct new empty @c ZoneOffsetTransitionRuleArray
       */
      CORE_IMPLICIT ZoneOffsetTransitionRuleArray() CORE_NOTHROW;

      /**
       * Construct new @c ZoneOffsetTransitionRuleArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with value @c U+0000.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       */
      CORE_EXPLICIT ZoneOffsetTransitionRuleArray(gint length);

      /**
       * Construct new @c ZoneOffsetTransitionRuleArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with given initial value.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       * @param initialValue The value used to initialize all values of
       *          this array.
       */
      CORE_EXPLICIT ZoneOffsetTransitionRuleArray(gint length, ZoneOffsetTransitionRule const& initialValue);

      /**
       * Construct new @c ZoneOffsetTransitionRuleArray instance by copy of values
       * of given array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT ZoneOffsetTransitionRuleArray(ZoneOffsetTransitionRuleArray const& array);

      /**
       * Construct new @c ZoneOffsetTransitionRuleArray instance by swapping of content
       * of given array.
       *
       * @note After this construction, the given array will be
       * empty and this array will have the old length of given
       * array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT ZoneOffsetTransitionRuleArray(ZoneOffsetTransitionRuleArray&& array) CORE_NOTHROW;

      /**
       * Construct new @c ZoneOffsetTransitionRuleArray with specified values.
       *
       * @param args the list of values used to create array.
       */
      CORE_IMPLICIT ZoneOffsetTransitionRuleArray(VarArgs const& args);

      ZoneOffsetTransitionRuleArray& operator=(const ZoneOffsetTransitionRuleArray& other);

      ZoneOffsetTransitionRuleArray& operator=(ZoneOffsetTransitionRuleArray&& other) CORE_NOTHROW;

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
      ZoneOffsetTransitionRule& get(gint index) override;

      /**
       * Return the value of elements at specified index
       *
       * @param index The index of desired element.
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      ZoneOffsetTransitionRule const& get(gint index) const override;

      /**
       * Set value of element at specified index with
       * specified new value
       *
       * @param index The index of desired element.
       * @param newValue The replacement value
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      ZoneOffsetTransitionRule set(gint index, ZoneOffsetTransitionRule newValue) override;

      /**
       * Destroy this array data.
       *
       * @note This operation will clear content firstly.
       */
      ~ZoneOffsetTransitionRuleArray() override;

      static ZoneOffsetTransitionRuleArray copyOf(Array<ZoneOffsetTransitionRule> const& a);

      /**
       * Obtain newly created @c ZoneOffsetTransitionRuleArray instance with primitive ZoneOffsetTransitionRule array.
       *
       * @tparam T The array type
       * @param array The array to be copied
       * @return The new ZoneOffsetTransitionRuleArray that contains all values of given array
       */
      template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                Class<gbool>::OnlyIf<Class<ZoneOffsetTransitionRule>::isSame<typename Class<T>::ArrayElement>()>  =
                    true>
      static ZoneOffsetTransitionRuleArray copyOf(T&& array) {
        CORE_FAST gint count = Class<T>::count();
        ZoneOffsetTransitionRuleArray zoneOffsetTransitionRules = ZoneOffsetTransitionRuleArray(count);
        for (int i = 0; i < count; ++i) zoneOffsetTransitionRules[i] = array[i];
        return zoneOffsetTransitionRules;
      }

      /**
       * Return new empty array
       *
       * @return the new empty array
       */
      static ZoneOffsetTransitionRuleArray of();

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing one value.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @return newly created array with one value.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing two values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @return newly created array with two values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing three values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @return newly created array with three values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing four values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @param s3 the fourth zoneOffsetTransitionRule
       * @return newly created array with four values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2,
                                              ZoneOffsetTransitionRule const& s3);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing five values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @param s3 the fourth zoneOffsetTransitionRule
       * @param s4 the fifth zoneOffsetTransitionRule
       * @return newly created array with five values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2,
                                              ZoneOffsetTransitionRule const& s3, ZoneOffsetTransitionRule const& s4);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing six values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @param s3 the fourth zoneOffsetTransitionRule
       * @param s4 the fifth zoneOffsetTransitionRule
       * @param s5 the sixth zoneOffsetTransitionRule
       * @return newly created array with six values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2,
                                              ZoneOffsetTransitionRule const& s3, ZoneOffsetTransitionRule const& s4,
                                              ZoneOffsetTransitionRule const& s5);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing seven values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @param s3 the fourth zoneOffsetTransitionRule
       * @param s4 the fifth zoneOffsetTransitionRule
       * @param s5 the sixth zoneOffsetTransitionRule
       * @param s6 the seventh zoneOffsetTransitionRule
       * @return newly created array with seven values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2,
                                              ZoneOffsetTransitionRule const& s3, ZoneOffsetTransitionRule const& s4,
                                              ZoneOffsetTransitionRule const& s5,
                                              ZoneOffsetTransitionRule const& s6);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing eight values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @param s3 the fourth zoneOffsetTransitionRule
       * @param s4 the fifth zoneOffsetTransitionRule
       * @param s5 the sixth zoneOffsetTransitionRule
       * @param s6 the seventh zoneOffsetTransitionRule
       * @param s7 the eigth zoneOffsetTransitionRule
       * @return newly created array with eight values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2,
                                              ZoneOffsetTransitionRule const& s3, ZoneOffsetTransitionRule const& s4,
                                              ZoneOffsetTransitionRule const& s5,
                                              ZoneOffsetTransitionRule const& s6, ZoneOffsetTransitionRule const& s7);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing nine values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @param s3 the fourth zoneOffsetTransitionRule
       * @param s4 the fifth zoneOffsetTransitionRule
       * @param s5 the sixth zoneOffsetTransitionRule
       * @param s6 the seventh zoneOffsetTransitionRule
       * @param s7 the eigth zoneOffsetTransitionRule
       * @param s8 the ninth zoneOffsetTransitionRule
       * @return newly created array with nine values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2,
                                              ZoneOffsetTransitionRule const& s3, ZoneOffsetTransitionRule const& s4,
                                              ZoneOffsetTransitionRule const& s5,
                                              ZoneOffsetTransitionRule const& s6, ZoneOffsetTransitionRule const& s7,
                                              ZoneOffsetTransitionRule const& s8);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing ten values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @param s3 the fourth zoneOffsetTransitionRule
       * @param s4 the fifth zoneOffsetTransitionRule
       * @param s5 the sixth zoneOffsetTransitionRule
       * @param s6 the seventh zoneOffsetTransitionRule
       * @param s7 the eigth zoneOffsetTransitionRule
       * @param s8 the ninth zoneOffsetTransitionRule
       * @param s9 the tenth zoneOffsetTransitionRule
       * @return newly created array with ten values.
       */
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2,
                                              ZoneOffsetTransitionRule const& s3, ZoneOffsetTransitionRule const& s4,
                                              ZoneOffsetTransitionRule const& s5,
                                              ZoneOffsetTransitionRule const& s6, ZoneOffsetTransitionRule const& s7,
                                              ZoneOffsetTransitionRule const& s8,
                                              ZoneOffsetTransitionRule const& s9);

      /**
       * Obtain new ZoneOffsetTransitionRuleArray containing ten+ values.
       *
       * @param s0 the first zoneOffsetTransitionRule
       * @param s1 the second zoneOffsetTransitionRule
       * @param s2 the third zoneOffsetTransitionRule
       * @param s3 the fourth zoneOffsetTransitionRule
       * @param s4 the fifth zoneOffsetTransitionRule
       * @param s5 the sixth zoneOffsetTransitionRule
       * @param s6 the seventh zoneOffsetTransitionRule
       * @param s7 the eigth zoneOffsetTransitionRule
       * @param s8 the ninth zoneOffsetTransitionRule
       * @param s9 the tenth zoneOffsetTransitionRule
       * @param s9 the tenth zoneOffsetTransitionRule
       * @param s the more zoneOffsetTransitionRules
       * @return newly created array with ten+ values.
       */
      template <class... ZoneOffsetTransitionRules>
      static ZoneOffsetTransitionRuleArray of(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                              ZoneOffsetTransitionRule const& s2,
                                              ZoneOffsetTransitionRule const& s3, ZoneOffsetTransitionRule const& s4,
                                              ZoneOffsetTransitionRule const& s5,
                                              ZoneOffsetTransitionRule const& s6, ZoneOffsetTransitionRule const& s7,
                                              ZoneOffsetTransitionRule const& s8,
                                              ZoneOffsetTransitionRule const& s9, ZoneOffsetTransitionRules&&... s) {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(10 + sizeof...(ZoneOffsetTransitionRules));
        sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
        sa.writeZoneOffsetTransitionRules(10, CORE_CAST(ZoneOffsetTransitionRules &&, s)...);

        return sa;
      }

      ZoneOffsetTransitionRule& operator[](gint index);

      ZoneOffsetTransitionRule const& operator[](gint index) const;

    private:
      class LinearIterator final : public Object {
        CORE_ADD_AS_FRIEND(ZoneOffsetTransitionRuleArray);

        ZoneOffsetTransitionRuleArray& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator(ZoneOffsetTransitionRuleArray& array, gbool isEnd);

      public:
        LinearIterator& operator++();

        ZoneOffsetTransitionRule& operator*() const;

        gbool operator==(LinearIterator const& rhs) const;

        gbool operator!=(LinearIterator const& rhs) const;
      };

      class LinearIterator2 final : public Object {
        CORE_ADD_AS_FRIEND(ZoneOffsetTransitionRuleArray);

        ZoneOffsetTransitionRuleArray const& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator2(ZoneOffsetTransitionRuleArray const& array, gbool isEnd);

      public:
        LinearIterator2& operator++();

        ZoneOffsetTransitionRule const& operator*() const;

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

      void initializeFirstTeens(ZoneOffsetTransitionRule const& s0, ZoneOffsetTransitionRule const& s1,
                                ZoneOffsetTransitionRule const& s2,
                                ZoneOffsetTransitionRule const& s3, ZoneOffsetTransitionRule const& s4,
                                ZoneOffsetTransitionRule const& s5, ZoneOffsetTransitionRule const& s6,
                                ZoneOffsetTransitionRule const& s7,
                                ZoneOffsetTransitionRule const& s8, ZoneOffsetTransitionRule const& s9);

      template <class S>
      void writeAsZoneOffsetTransitionRule(gint idx, S&& s) {
        CORE_FAST_ASSERT(Class<ZoneOffsetTransitionRule>::accept<S>() || Class<S>::isZoneOffsetTransitionRule());
        writeAsZoneOffsetTransitionRule(idx, ""_S + s);
      }

      void writeAsZoneOffsetTransitionRule(gint idx, ZoneOffsetTransitionRule const& s);

      void writeZoneOffsetTransitionRules(gint idx);

      template <class S0>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0) {
        writeAsZoneOffsetTransitionRule(idx + 0, CORE_CAST(S0 &&, s0));
      }

      template <class S0, class S1>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1) {
        writeAsZoneOffsetTransitionRule(idx + 0, CORE_CAST(S0 &&, s0));
        writeAsZoneOffsetTransitionRule(idx + 1, CORE_CAST(S1 &&, s1));
      }

      template <class S0, class S1, class S2>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
      }

      template <class S0, class S1, class S2, class S3>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitionRules(idx + 3, CORE_CAST(S3 &&, s3));
      }

      template <class S0, class S1, class S2, class S3, class S4>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitionRules(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitionRules(idx + 4, CORE_CAST(S4 &&, s4));
      }

      template <class S0, class S1, class S2, class S3, class S4, class S5>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitionRules(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitionRules(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitionRules(idx + 5, CORE_CAST(S5 &&, s5));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitionRules(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitionRules(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitionRules(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitionRules(idx + 6, CORE_CAST(S6 &&, s6));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6, S7&& s7) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitionRules(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitionRules(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitionRules(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitionRules(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsetTransitionRules(idx + 7, CORE_CAST(S7 &&, s7));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitionRules(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitionRules(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitionRules(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitionRules(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsetTransitionRules(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsetTransitionRules(idx + 8, CORE_CAST(S8 &&, s8));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitionRules(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitionRules(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitionRules(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitionRules(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsetTransitionRules(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsetTransitionRules(idx + 8, CORE_CAST(S8 &&, s8));
        writeZoneOffsetTransitionRules(idx + 9, CORE_CAST(S9 &&, s9));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9, class... S>
      void writeZoneOffsetTransitionRules(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                          S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
        writeZoneOffsetTransitionRules(idx + 0, CORE_CAST(S0 &&, s0));
        writeZoneOffsetTransitionRules(idx + 1, CORE_CAST(S1 &&, s1));
        writeZoneOffsetTransitionRules(idx + 2, CORE_CAST(S2 &&, s2));
        writeZoneOffsetTransitionRules(idx + 3, CORE_CAST(S3 &&, s3));
        writeZoneOffsetTransitionRules(idx + 4, CORE_CAST(S4 &&, s4));
        writeZoneOffsetTransitionRules(idx + 5, CORE_CAST(S5 &&, s5));
        writeZoneOffsetTransitionRules(idx + 6, CORE_CAST(S6 &&, s6));
        writeZoneOffsetTransitionRules(idx + 7, CORE_CAST(S7 &&, s7));
        writeZoneOffsetTransitionRules(idx + 8, CORE_CAST(S8 &&, s8));
        writeZoneOffsetTransitionRules(idx + 9, CORE_CAST(S9 &&, s9));
        writeZoneOffsetTransitionRules(idx + 10, CORE_CAST(S&&, s)...);
      }
    };
  } // time
} // core

#endif //CORE24_ZONEOFFSETTRANSITIONRULEARRAY_H
