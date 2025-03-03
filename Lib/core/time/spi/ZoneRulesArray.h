//
// Created by brunshweeck on 02/03/25.
//

#ifndef CORE24_ZONERULESARRAY_H
#define CORE24_ZONERULESARRAY_H

#include <core/misc/ValueArray.h>
#include <core/time/ZoneRules.h>

namespace core {
    namespace time {
        /**
         * The ZoneRulesArray class wrap the static array of values from native type
         * (generic) ZoneRules in an object. It's equivalent in C to ZoneRules[N]
         * <p>
         * This class provide the instantaneous access from items
         */
        class ZoneRulesArray final : public misc::ValueArray<ZoneRules> {
        public:
            /**
             * Construct new empty @c ZoneRulesArray
             */
            CORE_IMPLICIT ZoneRulesArray() CORE_NOTHROW;

            /**
             * Construct new @c ZoneRulesArray instance able to contains
             * the given number of values.
             *
             * @note All value will be initialized with value @c U+0000.
             *
             * @throws IllegalArgumentException If the given length is negative
             * @param length The number of value to be allocated on this array
             */
            CORE_EXPLICIT ZoneRulesArray(gint length);

            /**
             * Construct new @c ZoneRulesArray instance able to contains
             * the given number of values.
             *
             * @note All value will be initialized with given initial value.
             *
             * @throws IllegalArgumentException If the given length is negative
             * @param length The number of value to be allocated on this array
             * @param initialValue The value used to initialize all values of
             *          this array.
             */
            CORE_EXPLICIT ZoneRulesArray(gint length, ZoneRules const& initialValue);

            /**
             * Construct new @c ZoneRulesArray instance by copy of values
             * of given array.
             *
             * @param array The array used to create this array.
             */
            CORE_IMPLICIT ZoneRulesArray(ZoneRulesArray const& array);

            /**
             * Construct new @c ZoneRulesArray instance by swapping of content
             * of given array.
             *
             * @note After this construction, the given array will be
             * empty and this array will have the old length of given
             * array.
             *
             * @param array The array used to create this array.
             */
            CORE_IMPLICIT ZoneRulesArray(ZoneRulesArray&& array) CORE_NOTHROW;

            /**
             * Construct new @c ZoneRulesArray with specified values.
             *
             * @param args the list of values used to create array.
             */
            CORE_IMPLICIT ZoneRulesArray(VarArgs const& args);

            ZoneRulesArray& operator=(const ZoneRulesArray& other);

            ZoneRulesArray& operator=(ZoneRulesArray&& other) CORE_NOTHROW;

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
            ZoneRules& get(gint index) override;

            /**
             * Return the value of elements at specified index
             *
             * @param index The index of desired element.
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            ZoneRules const& get(gint index) const override;

            /**
             * Set value of element at specified index with
             * specified new value
             *
             * @param index The index of desired element.
             * @param newValue The replacement value
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            ZoneRules set(gint index, ZoneRules newValue) override;

            /**
             * Destroy this array data.
             *
             * @note This operation will clear content firstly.
             */
            ~ZoneRulesArray() override;

            static ZoneRulesArray copyOf(Array<ZoneRules> const& a);

            /**
             * Obtain newly created @c ZoneRulesArray instance with primitive ZoneRules array.
             *
             * @tparam T The array type
             * @param array The array to be copied
             * @return The new ZoneRulesArray that contains all values of given array
             */
            template <class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
                      Class<gbool>::OnlyIf<Class<ZoneRules>::isSame<typename Class<T>::ArrayElement>()>  = true>
            static ZoneRulesArray copyOf(T&& array) {
                CORE_FAST gint count = Class<T>::count();
                ZoneRulesArray zoneRuless = ZoneRulesArray(count);
                for (int i = 0; i < count; ++i) zoneRuless[i] = array[i];
                return zoneRuless;
            }

            /**
             * Return new empty array
             *
             * @return the new empty array
             */
            static ZoneRulesArray of();

            /**
             * Obtain new ZoneRulesArray containing one value.
             *
             * @param s0 the first zoneRules
             * @return newly created array with one value.
             */
            static ZoneRulesArray of(ZoneRules const& s0);

            /**
             * Obtain new ZoneRulesArray containing two values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @return newly created array with two values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1);

            /**
             * Obtain new ZoneRulesArray containing three values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @return newly created array with three values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2);

            /**
             * Obtain new ZoneRulesArray containing four values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @param s3 the fourth zoneRules
             * @return newly created array with four values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3);

            /**
             * Obtain new ZoneRulesArray containing five values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @param s3 the fourth zoneRules
             * @param s4 the fifth zoneRules
             * @return newly created array with five values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3, ZoneRules const& s4);

            /**
             * Obtain new ZoneRulesArray containing six values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @param s3 the fourth zoneRules
             * @param s4 the fifth zoneRules
             * @param s5 the sixth zoneRules
             * @return newly created array with six values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5);

            /**
             * Obtain new ZoneRulesArray containing seven values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @param s3 the fourth zoneRules
             * @param s4 the fifth zoneRules
             * @param s5 the sixth zoneRules
             * @param s6 the seventh zoneRules
             * @return newly created array with seven values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                      ZoneRules const& s6);

            /**
             * Obtain new ZoneRulesArray containing eight values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @param s3 the fourth zoneRules
             * @param s4 the fifth zoneRules
             * @param s5 the sixth zoneRules
             * @param s6 the seventh zoneRules
             * @param s7 the eigth zoneRules
             * @return newly created array with eight values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                      ZoneRules const& s6, ZoneRules const& s7);

            /**
             * Obtain new ZoneRulesArray containing nine values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @param s3 the fourth zoneRules
             * @param s4 the fifth zoneRules
             * @param s5 the sixth zoneRules
             * @param s6 the seventh zoneRules
             * @param s7 the eigth zoneRules
             * @param s8 the ninth zoneRules
             * @return newly created array with nine values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                      ZoneRules const& s6, ZoneRules const& s7, ZoneRules const& s8);

            /**
             * Obtain new ZoneRulesArray containing ten values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @param s3 the fourth zoneRules
             * @param s4 the fifth zoneRules
             * @param s5 the sixth zoneRules
             * @param s6 the seventh zoneRules
             * @param s7 the eigth zoneRules
             * @param s8 the ninth zoneRules
             * @param s9 the tenth zoneRules
             * @return newly created array with ten values.
             */
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                      ZoneRules const& s6, ZoneRules const& s7, ZoneRules const& s8,
                                      ZoneRules const& s9);

            /**
             * Obtain new ZoneRulesArray containing ten+ values.
             *
             * @param s0 the first zoneRules
             * @param s1 the second zoneRules
             * @param s2 the third zoneRules
             * @param s3 the fourth zoneRules
             * @param s4 the fifth zoneRules
             * @param s5 the sixth zoneRules
             * @param s6 the seventh zoneRules
             * @param s7 the eigth zoneRules
             * @param s8 the ninth zoneRules
             * @param s9 the tenth zoneRules
             * @param s9 the tenth zoneRules
             * @param s the more zoneRuless
             * @return newly created array with ten+ values.
             */
            template <class... ZoneRuless>
            static ZoneRulesArray of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                      ZoneRules const& s6, ZoneRules const& s7, ZoneRules const& s8,
                                      ZoneRules const& s9, ZoneRuless&&... s) {
                ZoneRulesArray sa = ZoneRulesArray(10 + sizeof...(ZoneRuless));
                sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
                sa.writeZoneRuless(10, CORE_CAST(ZoneRuless &&, s)...);

                return sa;
            }

            ZoneRules& operator[](gint index);

            ZoneRules const& operator[](gint index) const;

        private:
            class LinearIterator final : public Object {
                CORE_ADD_AS_FRIEND(ZoneRulesArray);

                ZoneRulesArray& array;
                gint next = 0;
                gbool isEnd = false;

                CORE_EXPLICIT LinearIterator(ZoneRulesArray& array, gbool isEnd);

            public:
                LinearIterator& operator++();

                ZoneRules& operator*() const;

                gbool operator==(LinearIterator const& rhs) const;

                gbool operator!=(LinearIterator const& rhs) const;
            };

            class LinearIterator2 final : public Object {
                CORE_ADD_AS_FRIEND(ZoneRulesArray);

                ZoneRulesArray const& array;
                gint next = 0;
                gbool isEnd = false;

                CORE_EXPLICIT LinearIterator2(ZoneRulesArray const& array, gbool isEnd);

            public:
                LinearIterator2& operator++();

                ZoneRules const& operator*() const;

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

            void initializeFirstTeens(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                      ZoneRules const& s3, ZoneRules const& s4,
                                      ZoneRules const& s5, ZoneRules const& s6, ZoneRules const& s7,
                                      ZoneRules const& s8, ZoneRules const& s9);

            template <class S>
            void writeAsZoneRules(gint idx, S&& s) {
                CORE_FAST_ASSERT(Class<ZoneRules>::accept<S>() || Class<S>::isZoneRules());
                writeAsZoneRules(idx, ""_S + s);
            }

            void writeAsZoneRules(gint idx, ZoneRules const& s);

            void writeZoneRuless(gint idx);

            template <class S0>
            void writeZoneRuless(gint idx, S0&& s0) { writeAsZoneRules(idx + 0, CORE_CAST(S0 &&, s0)); }

            template <class S0, class S1>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1) {
                writeAsZoneRules(idx + 0, CORE_CAST(S0 &&, s0));
                writeAsZoneRules(idx + 1, CORE_CAST(S1 &&, s1));
            }

            template <class S0, class S1, class S2>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
            }

            template <class S0, class S1, class S2, class S3>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
                writeZoneRuless(idx + 3, CORE_CAST(S3 &&, s3));
            }

            template <class S0, class S1, class S2, class S3, class S4>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
                writeZoneRuless(idx + 3, CORE_CAST(S3 &&, s3));
                writeZoneRuless(idx + 4, CORE_CAST(S4 &&, s4));
            }

            template <class S0, class S1, class S2, class S3, class S4, class S5>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                  S5&& s5) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
                writeZoneRuless(idx + 3, CORE_CAST(S3 &&, s3));
                writeZoneRuless(idx + 4, CORE_CAST(S4 &&, s4));
                writeZoneRuless(idx + 5, CORE_CAST(S5 &&, s5));
            }

            template <class S0, class S1, class S2, class S3, class S4,
                      class S5, class S6>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                  S5&& s5, S6&& s6) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
                writeZoneRuless(idx + 3, CORE_CAST(S3 &&, s3));
                writeZoneRuless(idx + 4, CORE_CAST(S4 &&, s4));
                writeZoneRuless(idx + 5, CORE_CAST(S5 &&, s5));
                writeZoneRuless(idx + 6, CORE_CAST(S6 &&, s6));
            }

            template <class S0, class S1, class S2, class S3, class S4,
                      class S5, class S6, class S7>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                  S5&& s5, S6&& s6, S7&& s7) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
                writeZoneRuless(idx + 3, CORE_CAST(S3 &&, s3));
                writeZoneRuless(idx + 4, CORE_CAST(S4 &&, s4));
                writeZoneRuless(idx + 5, CORE_CAST(S5 &&, s5));
                writeZoneRuless(idx + 6, CORE_CAST(S6 &&, s6));
                writeZoneRuless(idx + 7, CORE_CAST(S7 &&, s7));
            }

            template <class S0, class S1, class S2, class S3, class S4,
                      class S5, class S6, class S7, class S8>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                  S5&& s5, S6&& s6, S7&& s7, S8&& s8) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
                writeZoneRuless(idx + 3, CORE_CAST(S3 &&, s3));
                writeZoneRuless(idx + 4, CORE_CAST(S4 &&, s4));
                writeZoneRuless(idx + 5, CORE_CAST(S5 &&, s5));
                writeZoneRuless(idx + 6, CORE_CAST(S6 &&, s6));
                writeZoneRuless(idx + 7, CORE_CAST(S7 &&, s7));
                writeZoneRuless(idx + 8, CORE_CAST(S8 &&, s8));
            }

            template <class S0, class S1, class S2, class S3, class S4,
                      class S5, class S6, class S7, class S8, class S9>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                  S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
                writeZoneRuless(idx + 3, CORE_CAST(S3 &&, s3));
                writeZoneRuless(idx + 4, CORE_CAST(S4 &&, s4));
                writeZoneRuless(idx + 5, CORE_CAST(S5 &&, s5));
                writeZoneRuless(idx + 6, CORE_CAST(S6 &&, s6));
                writeZoneRuless(idx + 7, CORE_CAST(S7 &&, s7));
                writeZoneRuless(idx + 8, CORE_CAST(S8 &&, s8));
                writeZoneRuless(idx + 9, CORE_CAST(S9 &&, s9));
            }

            template <class S0, class S1, class S2, class S3, class S4,
                      class S5, class S6, class S7, class S8, class S9, class... S>
            void writeZoneRuless(gint idx, S0&& s0, S1&& s1, S2&& s2, S3&& s3, S4&& s4,
                                  S5&& s5, S6&& s6, S7&& s7, S8&& s8, S9&& s9, S&&... s) {
                writeZoneRuless(idx + 0, CORE_CAST(S0 &&, s0));
                writeZoneRuless(idx + 1, CORE_CAST(S1 &&, s1));
                writeZoneRuless(idx + 2, CORE_CAST(S2 &&, s2));
                writeZoneRuless(idx + 3, CORE_CAST(S3 &&, s3));
                writeZoneRuless(idx + 4, CORE_CAST(S4 &&, s4));
                writeZoneRuless(idx + 5, CORE_CAST(S5 &&, s5));
                writeZoneRuless(idx + 6, CORE_CAST(S6 &&, s6));
                writeZoneRuless(idx + 7, CORE_CAST(S7 &&, s7));
                writeZoneRuless(idx + 8, CORE_CAST(S8 &&, s8));
                writeZoneRuless(idx + 9, CORE_CAST(S9 &&, s9));
                writeZoneRuless(idx + 10, CORE_CAST(S&&, s)...);
            }
        };
    } // time
} // core

#endif //CORE24_ZONERULESARRAY_H
