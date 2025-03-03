//
// Created by brunshweeck on 01/03/25.
//

#ifndef CORE24_INSTANTARRAY_H
#define CORE24_INSTANTARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/Instant.h>

namespace core {
  namespace time {
    /**
     * The InstantArray class wrap the static array of values from native type
     * (generic) Instant in an object. It's equivalent in C to Instant[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class InstantArray final : public misc::ValueArray<Instant> {
    public:
      /**
       * Construct new empty @c InstantArray
       */
      CORE_IMPLICIT InstantArray() CORE_NOTHROW;

      /**
       * Construct new @c InstantArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with value @c U+0000.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       */
      CORE_EXPLICIT InstantArray(gint length);

      /**
       * Construct new @c InstantArray instance able to contains
       * the given number of values.
       *
       * @note All value will be initialized with given initial value.
       *
       * @throws IllegalArgumentException If the given length is negative
       * @param length The number of value to be allocated on this array
       * @param initialValue The value used to initialize all values of
       *          this array.
       */
      CORE_EXPLICIT InstantArray(gint length, Instant const& initialValue);

      /**
       * Construct new @c InstantArray instance by copy of values
       * of given array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT InstantArray(InstantArray const& array);

      /**
       * Construct new @c InstantArray instance by swapping of content
       * of given array.
       *
       * @note After this construction, the given array will be
       * empty and this array will have the old length of given
       * array.
       *
       * @param array The array used to create this array.
       */
      CORE_IMPLICIT InstantArray(InstantArray&& array) CORE_NOTHROW;

      /**
       * Construct new @c InstantArray with specified values.
       *
       * @param args the list of values used to create array.
       */
      CORE_IMPLICIT InstantArray(VarArgs const& args);

      InstantArray& operator=(const InstantArray& other);

      InstantArray& operator=(InstantArray&& other) CORE_NOTHROW;

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
      Instant& get(gint index) override;

      /**
       * Return the value of elements at specified index
       *
       * @param index The index of desired element.
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      Instant const& get(gint index) const override;

      /**
       * Set value of element at specified index with
       * specified new value
       *
       * @param index The index of desired element.
       * @param newValue The replacement value
       * @throws IndexOutOfBoundsException If the given index out of bounds
       */
      Instant set(gint index, Instant newValue) override;

      /**
       * Destroy this array data.
       *
       * @note This operation will clear content firstly.
       */
      ~InstantArray() override;

      static InstantArray copyOf(Array<Instant> const& a);

      /**
       * Obtain newly created @c InstantArray instance with primitive Instant array.
       *
       * @tparam T The array type
       * @param array The array to be copied
       * @return The new InstantArray that contains all values of given array
       */
      template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                Class<gbool>::OnlyIf<Class<Instant>::isSame<typename Class<T>::ArrayElement>()>  = true>
      static InstantArray copyOf(T&& array) {
        CORE_FAST gint count = Class<T>::count();
        InstantArray instants = InstantArray(count);
        for (int i = 0; i < count; ++i) instants[i] = array[i];
        return instants;
      }

      /**
       * Return new empty array
       *
       * @return the new empty array
       */
      static InstantArray of();

      /**
       * Obtain new InstantArray containing one value.
       *
       * @param s0 the first instant
       * @return newly created array with one value.
       */
      static InstantArray of(Instant const& s0);

      /**
       * Obtain new InstantArray containing two values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @return newly created array with two values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1);

      /**
       * Obtain new InstantArray containing three values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @return newly created array with three values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2);

      /**
       * Obtain new InstantArray containing four values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @param s3 the fourth instant
       * @return newly created array with four values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2,
                                   Instant const& s3);

      /**
       * Obtain new InstantArray containing five values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @param s3 the fourth instant
       * @param s4 the fifth instant
       * @return newly created array with five values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2,
                                   Instant const& s3, Instant const& s4);

      /**
       * Obtain new InstantArray containing six values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @param s3 the fourth instant
       * @param s4 the fifth instant
       * @param s5 the sixth instant
       * @return newly created array with six values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2,
                                   Instant const& s3, Instant const& s4, Instant const& s5);

      /**
       * Obtain new InstantArray containing seven values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @param s3 the fourth instant
       * @param s4 the fifth instant
       * @param s5 the sixth instant
       * @param s6 the seventh instant
       * @return newly created array with seven values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2,
                                   Instant const& s3, Instant const& s4, Instant const& s5,
                                   Instant const& s6);

      /**
       * Obtain new InstantArray containing eight values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @param s3 the fourth instant
       * @param s4 the fifth instant
       * @param s5 the sixth instant
       * @param s6 the seventh instant
       * @param s7 the eigth instant
       * @return newly created array with eight values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2,
                                   Instant const& s3, Instant const& s4, Instant const& s5,
                                   Instant const& s6, Instant const& s7);

      /**
       * Obtain new InstantArray containing nine values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @param s3 the fourth instant
       * @param s4 the fifth instant
       * @param s5 the sixth instant
       * @param s6 the seventh instant
       * @param s7 the eigth instant
       * @param s8 the ninth instant
       * @return newly created array with nine values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2,
                                   Instant const& s3, Instant const& s4, Instant const& s5,
                                   Instant const& s6, Instant const& s7, Instant const& s8);

      /**
       * Obtain new InstantArray containing ten values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @param s3 the fourth instant
       * @param s4 the fifth instant
       * @param s5 the sixth instant
       * @param s6 the seventh instant
       * @param s7 the eigth instant
       * @param s8 the ninth instant
       * @param s9 the tenth instant
       * @return newly created array with ten values.
       */
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2,
                                   Instant const& s3, Instant const& s4, Instant const& s5,
                                   Instant const& s6, Instant const& s7, Instant const& s8,
                                   Instant const& s9);

      /**
       * Obtain new InstantArray containing ten+ values.
       *
       * @param s0 the first instant
       * @param s1 the second instant
       * @param s2 the third instant
       * @param s3 the fourth instant
       * @param s4 the fifth instant
       * @param s5 the sixth instant
       * @param s6 the seventh instant
       * @param s7 the eigth instant
       * @param s8 the ninth instant
       * @param s9 the tenth instant
       * @param s9 the tenth instant
       * @param s the more instants
       * @return newly created array with ten+ values.
       */
      template <class... Instants>
      static InstantArray of(Instant const& s0, Instant const& s1, Instant const& s2,
                                   Instant const& s3, Instant const& s4, Instant const& s5,
                                   Instant const& s6, Instant const& s7, Instant const& s8,
                                   Instant const& s9, Instants&&... s) {
        InstantArray sa = InstantArray(10 + sizeof...(Instants));
        sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
        sa.writeInstants(10, CORE_CAST(Instants &&, s)...);

        return sa;
      }

      Instant& operator[](gint index);

      Instant const& operator[](gint index) const;

    private:
      class LinearIterator final : public Object {
        CORE_ADD_AS_FRIEND(InstantArray);

        InstantArray& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator(InstantArray& array, gbool isEnd);

      public:
        LinearIterator& operator++();

        Instant& operator*() const;

        gbool operator==(LinearIterator const& rhs) const;

        gbool operator!=(LinearIterator const& rhs) const;
      };

      class LinearIterator2 final : public Object {
        CORE_ADD_AS_FRIEND(InstantArray);

        InstantArray const& array;
        gint next = 0;
        gbool isEnd = false;

        CORE_EXPLICIT LinearIterator2(InstantArray const& array, gbool isEnd);

      public:
        LinearIterator2& operator++();

        Instant const& operator*() const;

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

      void initializeFirstTeens(Instant const& s0, Instant const& s1, Instant const& s2,
                                Instant const& s3, Instant const& s4,
                                Instant const& s5, Instant const& s6, Instant const& s7,
                                Instant const& s8, Instant const& s9);

      template <class S>
      void writeAsInstant(gint idx, S&& s) {
        CORE_FAST_ASSERT(Class<Instant>::accept<S>() || Class<S>::isInstant());
        writeAsInstant(idx, ""_S + s);
      }

      void writeAsInstant(gint idx, Instant const& s);

      void writeInstants(gint idx);

      template <class S0>
      void writeInstants(gint idx, S0&& s0) { writeAsInstant(idx + 0, CORE_CAST(S0 &&, s0)); }

      template <class S0, class S1>
      void writeInstants(gint idx, S0&& s0, S1&& s1) {
        writeAsInstant(idx + 0, CORE_CAST(S0 &&, s0));
        writeAsInstant(idx + 1, CORE_CAST(S1 &&, s1));
      }

      template <class S0, class S1, class S2>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
      }

      template <class S0, class S1, class S2, class S3>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
        writeInstants(idx + 3, CORE_CAST(S3 &&, s3));
      }

      template <class S0, class S1, class S2, class S3, class S4>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
        writeInstants(idx + 3, CORE_CAST(S3 &&, s3));
        writeInstants(idx + 4, CORE_CAST(S4 &&, s4));
      }

      template <class S0, class S1, class S2, class S3, class S4, class S5>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
        writeInstants(idx + 3, CORE_CAST(S3 &&, s3));
        writeInstants(idx + 4, CORE_CAST(S4 &&, s4));
        writeInstants(idx + 5, CORE_CAST(S5 &&, s5));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
        writeInstants(idx + 3, CORE_CAST(S3 &&, s3));
        writeInstants(idx + 4, CORE_CAST(S4 &&, s4));
        writeInstants(idx + 5, CORE_CAST(S5 &&, s5));
        writeInstants(idx + 6, CORE_CAST(S6 &&, s6));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
        writeInstants(idx + 3, CORE_CAST(S3 &&, s3));
        writeInstants(idx + 4, CORE_CAST(S4 &&, s4));
        writeInstants(idx + 5, CORE_CAST(S5 &&, s5));
        writeInstants(idx + 6, CORE_CAST(S6 &&, s6));
        writeInstants(idx + 7, CORE_CAST(S7 &&, s7));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
        writeInstants(idx + 3, CORE_CAST(S3 &&, s3));
        writeInstants(idx + 4, CORE_CAST(S4 &&, s4));
        writeInstants(idx + 5, CORE_CAST(S5 &&, s5));
        writeInstants(idx + 6, CORE_CAST(S6 &&, s6));
        writeInstants(idx + 7, CORE_CAST(S7 &&, s7));
        writeInstants(idx + 8, CORE_CAST(S8 &&, s8));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
        writeInstants(idx + 3, CORE_CAST(S3 &&, s3));
        writeInstants(idx + 4, CORE_CAST(S4 &&, s4));
        writeInstants(idx + 5, CORE_CAST(S5 &&, s5));
        writeInstants(idx + 6, CORE_CAST(S6 &&, s6));
        writeInstants(idx + 7, CORE_CAST(S7 &&, s7));
        writeInstants(idx + 8, CORE_CAST(S8 &&, s8));
        writeInstants(idx + 9, CORE_CAST(S9 &&, s9));
      }

      template <class S0, class S1, class S2, class S3, class S4,
                class S5, class S6, class S7, class S8, class S9, class... S>
      void writeInstants(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                               S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
        writeInstants(idx + 0, CORE_CAST(S0 &&, s0));
        writeInstants(idx + 1, CORE_CAST(S1 &&, s1));
        writeInstants(idx + 2, CORE_CAST(S2 &&, s2));
        writeInstants(idx + 3, CORE_CAST(S3 &&, s3));
        writeInstants(idx + 4, CORE_CAST(S4 &&, s4));
        writeInstants(idx + 5, CORE_CAST(S5 &&, s5));
        writeInstants(idx + 6, CORE_CAST(S6 &&, s6));
        writeInstants(idx + 7, CORE_CAST(S7 &&, s7));
        writeInstants(idx + 8, CORE_CAST(S8 &&, s8));
        writeInstants(idx + 9, CORE_CAST(S9 &&, s9));
        writeInstants(idx + 10, CORE_CAST(S&&, s)...);
      }
    };
  } // time
} // core

#endif //CORE24_INSTANTARRAY_H
