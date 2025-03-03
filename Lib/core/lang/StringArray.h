//
// Created by brunshweeck on 3 sept. 2024.
//

#ifndef CORE24_STRINGARRAY_H
#define CORE24_STRINGARRAY_H

#include <core/lang/String.h>
#include <core/misc/ValueArray.h>

namespace core {
    /**
     * The StringArray class wrap the static array of values from native type
     * (generic) String in an object. It's equivalent in C to String[N]
     * <p>
     * This class provide the instantaneous access from items
     */
    class StringArray final : public misc::ValueArray<String> {

    public:
        /**
         * Construct new empty @c StringArray
         */
        CORE_IMPLICIT StringArray() CORE_NOTHROW;

        /**
         * Construct new @c StringArray instance able to contains
         * the given number of values.
         *
         * @note All value will be initialized with value @c U+0000.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param length The number of value to be allocated on this array
         */
        CORE_EXPLICIT StringArray(gint length);

        /**
         * Construct new @c StringArray instance able to contains
         * the given number of values.
         *
         * @note All value will be initialized with given initial value.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param length The number of value to be allocated on this array
         * @param initialValue The value used to initialize all values of
         *          this array.
         */
        CORE_EXPLICIT StringArray(gint length, String initialValue);

        /**
         * Construct new @c StringArray instance by copy of values
         * of given array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT StringArray(StringArray const &array);

        /**
         * Construct new @c StringArray instance by swapping of content
         * of given array.
         *
         * @note After this construction, the given array will be
         * empty and this array will have the old length of given
         * array.
         *
         * @param array The array used to create this array.
         */
        CORE_IMPLICIT StringArray(StringArray &&array) CORE_NOTHROW;

        /**
         * Construct new @c StringArray with specified values.
         *
         * @param args the list of values used to create array.
         */
        CORE_IMPLICIT StringArray(VarArgs const &args);

        StringArray & operator=(const StringArray &other);

        StringArray & operator=(StringArray &&other) CORE_NOTHROW;

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
        String &get(gint index) override;

        /**
         * Return the value of elements at specified index
         *
         * @param index The index of desired element.
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        String const &get(gint index) const override;

        /**
         * Set value of element at specified index with
         * specified new value
         *
         * @param index The index of desired element.
         * @param newValue The replacement value
         * @throws IndexOutOfBoundsException If the given index out of bounds
         */
        String set(gint index, String newValue) override;

        /**
         * Destroy this array data.
         *
         * @note This operation will clear content firstly.
         */
        ~StringArray() override;

        static StringArray copyOf(Array<String> const& a);

        /**
         * Obtain newly created @c StringArray instance with primitive String array.
         *
         * @tparam T The array type
         * @param array The array to be copied
         * @return The new StringArray that contains all values of given array
         */
        template<class T, Class<gbool>::OnlyIf<Class<T>::isArray()>  = true,
            Class<gbool>::OnlyIf<Class<String>::isSame<typename Class<T>::ArrayElement>()>  = true>
        static StringArray copyOf(T &&array) {
            CORE_FAST gint count = Class<T>::count();
            StringArray strings = StringArray(count);
            for (int i = 0; i < count; ++i) strings[i] = array[i];
            return strings;
        }

        /**
         * Return new empty array
         *
         * @return the new empty array
         */
        static StringArray of();

        /**
         * Obtain new StringArray containing one value.
         *
         * @param s0 the first string
         * @return newly created array with one value.
         */
        static StringArray of(String s0);

        /**
         * Obtain new StringArray containing two values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @return newly created array with two values.
         */
        static StringArray of(String s0, String s1);

        /**
         * Obtain new StringArray containing three values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @return newly created array with three values.
         */
        static StringArray of(String s0, String s1, String s2);

        /**
         * Obtain new StringArray containing four values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @param s3 the fourth string
         * @return newly created array with four values.
         */
        static StringArray of(String s0, String s1, String s2, String s3);

        /**
         * Obtain new StringArray containing five values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @param s3 the fourth string
         * @param s4 the fifth string
         * @return newly created array with five values.
         */
        static StringArray of(String s0, String s1, String s2, String s3, String s4);

        /**
         * Obtain new StringArray containing six values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @param s3 the fourth string
         * @param s4 the fifth string
         * @param s5 the sixth string
         * @return newly created array with six values.
         */
        static StringArray of(String s0, String s1, String s2, String s3, String s4,
                              String s5);

        /**
         * Obtain new StringArray containing seven values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @param s3 the fourth string
         * @param s4 the fifth string
         * @param s5 the sixth string
         * @param s6 the seventh string
         * @return newly created array with seven values.
         */
        static StringArray of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6);

        /**
         * Obtain new StringArray containing eight values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @param s3 the fourth string
         * @param s4 the fifth string
         * @param s5 the sixth string
         * @param s6 the seventh string
         * @param s7 the eigth string
         * @return newly created array with eight values.
         */
        static StringArray of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6, String s7);

        /**
         * Obtain new StringArray containing nine values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @param s3 the fourth string
         * @param s4 the fifth string
         * @param s5 the sixth string
         * @param s6 the seventh string
         * @param s7 the eigth string
         * @param s8 the ninth string
         * @return newly created array with nine values.
         */
        static StringArray of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6, String s7, String s8);

        /**
         * Obtain new StringArray containing ten values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @param s3 the fourth string
         * @param s4 the fifth string
         * @param s5 the sixth string
         * @param s6 the seventh string
         * @param s7 the eigth string
         * @param s8 the ninth string
         * @param s9 the tenth string
         * @return newly created array with ten values.
         */
        static StringArray of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6, String s7, String s8, String s9);

        /**
         * Obtain new StringArray containing ten+ values.
         *
         * @param s0 the first string
         * @param s1 the second string
         * @param s2 the third string
         * @param s3 the fourth string
         * @param s4 the fifth string
         * @param s5 the sixth string
         * @param s6 the seventh string
         * @param s7 the eigth string
         * @param s8 the ninth string
         * @param s9 the tenth string
         * @param s9 the tenth string
         * @param s the more strings
         * @return newly created array with ten+ values.
         */
        template<class... Strings>
        static StringArray of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6, String s7, String s8, String s9,
                              Strings &&... s) {
            StringArray sa = StringArray(10 + sizeof...(Strings));
            sa.initializeFirstTeens(s0, s1, s2, s3, s4, s5, s6, s7, s8, s9);
            sa.writeStrings(10, CORE_CAST(Strings &&, s)...);

            return sa;
        }

        String &operator[](gint index);

        String const &operator[](gint index) const;

    private:
        class LinearIterator final : public Object {
            CORE_ADD_AS_FRIEND(StringArray);

            StringArray &array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator(StringArray &array, gbool isEnd);

        public:
            LinearIterator &operator++();

            String &operator*() const;

            gbool operator==(LinearIterator const &rhs) const;

            gbool operator!=(LinearIterator const &rhs) const;
        };

        class LinearIterator2 final : public Object {
            CORE_ADD_AS_FRIEND(StringArray);

            StringArray const &array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator2(StringArray const &array, gbool isEnd);

        public:
            LinearIterator2 &operator++();

            String const &operator*() const;

            gbool operator==(LinearIterator2 const &rhs) const;

            gbool operator!=(LinearIterator2 const &rhs) const;
        };

    public:
        LinearIterator begin();

        LinearIterator2 begin() const;

        LinearIterator end();

        LinearIterator2 end() const;

        gbool equals(const Object &obj) const override;

        gint hash() const override;

        String toString() const override;

        Object &clone() const override;

    private:
        // -------------------------- Varargs API ---------------------------------------------

        void initializeFirstTeens(String &s0, String &s1, String &s2, String &s3, String &s4,
                                  String &s5, String &s6, String &s7, String &s8, String &s9);

        template<class S>
        void writeAsString(gint idx, S &&s) {
            CORE_FAST_ASSERT(Class<String>::accept<S>() || Class<S>::isString());
            writeAsString(idx, ""_S + s);
        }

        void writeAsString(gint idx, String const &s);

        void writeAsString(gint idx, String &s);

        void writeAsString(gint idx, String &&s);

        void writeStrings(gint idx);

        template<class S0>
        void writeStrings(gint idx, S0 &&s0) { writeAsString(idx + 0, CORE_CAST(S0 &&, s0)); }

        template<class S0, class S1>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1) {
            writeAsString(idx + 0, CORE_CAST(S0 &&, s0));
            writeAsString(idx + 1, CORE_CAST(S1 &&, s1));
        }

        template<class S0, class S1, class S2>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
        }

        template<class S0, class S1, class S2, class S3>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2, S3 &&s3) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
            writeStrings(idx + 3, CORE_CAST(S3 &&, s3));
        }

        template<class S0, class S1, class S2, class S3, class S4>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2, S3 &&s3, S4 &&s4) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
            writeStrings(idx + 3, CORE_CAST(S3 &&, s3));
            writeStrings(idx + 4, CORE_CAST(S4 &&, s4));
        }

        template<class S0, class S1, class S2, class S3, class S4, class S5>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2, S3 &&s3, S4 &&s4,
                          S5 &&s5) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
            writeStrings(idx + 3, CORE_CAST(S3 &&, s3));
            writeStrings(idx + 4, CORE_CAST(S4 &&, s4));
            writeStrings(idx + 5, CORE_CAST(S5 &&, s5));
        }

        template<class S0, class S1, class S2, class S3, class S4,
            class S5, class S6>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2, S3 &&s3, S4 &&s4,
                          S5 &&s5, S6 &&s6) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
            writeStrings(idx + 3, CORE_CAST(S3 &&, s3));
            writeStrings(idx + 4, CORE_CAST(S4 &&, s4));
            writeStrings(idx + 5, CORE_CAST(S5 &&, s5));
            writeStrings(idx + 6, CORE_CAST(S6 &&, s6));
        }

        template<class S0, class S1, class S2, class S3, class S4,
            class S5, class S6, class S7>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2, S3 &&s3, S4 &&s4,
                          S5 &&s5, S6 &&s6, S7 &&s7) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
            writeStrings(idx + 3, CORE_CAST(S3 &&, s3));
            writeStrings(idx + 4, CORE_CAST(S4 &&, s4));
            writeStrings(idx + 5, CORE_CAST(S5 &&, s5));
            writeStrings(idx + 6, CORE_CAST(S6 &&, s6));
            writeStrings(idx + 7, CORE_CAST(S7 &&, s7));
        }

        template<class S0, class S1, class S2, class S3, class S4,
            class S5, class S6, class S7, class S8>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2, S3 &&s3, S4 &&s4,
                          S5 &&s5, S6 &&s6, S7 &&s7, S8 &&s8) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
            writeStrings(idx + 3, CORE_CAST(S3 &&, s3));
            writeStrings(idx + 4, CORE_CAST(S4 &&, s4));
            writeStrings(idx + 5, CORE_CAST(S5 &&, s5));
            writeStrings(idx + 6, CORE_CAST(S6 &&, s6));
            writeStrings(idx + 7, CORE_CAST(S7 &&, s7));
            writeStrings(idx + 8, CORE_CAST(S8 &&, s8));
        }

        template<class S0, class S1, class S2, class S3, class S4,
            class S5, class S6, class S7, class S8, class S9>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2, S3 &&s3, S4 &&s4,
                          S5 &&s5, S6 &&s6, S7 &&s7, S8 &&s8, S9 &&s9) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
            writeStrings(idx + 3, CORE_CAST(S3 &&, s3));
            writeStrings(idx + 4, CORE_CAST(S4 &&, s4));
            writeStrings(idx + 5, CORE_CAST(S5 &&, s5));
            writeStrings(idx + 6, CORE_CAST(S6 &&, s6));
            writeStrings(idx + 7, CORE_CAST(S7 &&, s7));
            writeStrings(idx + 8, CORE_CAST(S8 &&, s8));
            writeStrings(idx + 9, CORE_CAST(S9 &&, s9));
        }

        template<class S0, class S1, class S2, class S3, class S4,
            class S5, class S6, class S7, class S8, class S9, class... S>
        void writeStrings(gint idx, S0 &&s0, S1 &&s1, S2 &&s2, S3 &&s3, S4 &&s4,
                          S5 &&s5, S6 &&s6, S7 &&s7, S8 &&s8, S9 &&s9, S &&... s) {
            writeStrings(idx + 0, CORE_CAST(S0 &&, s0));
            writeStrings(idx + 1, CORE_CAST(S1 &&, s1));
            writeStrings(idx + 2, CORE_CAST(S2 &&, s2));
            writeStrings(idx + 3, CORE_CAST(S3 &&, s3));
            writeStrings(idx + 4, CORE_CAST(S4 &&, s4));
            writeStrings(idx + 5, CORE_CAST(S5 &&, s5));
            writeStrings(idx + 6, CORE_CAST(S6 &&, s6));
            writeStrings(idx + 7, CORE_CAST(S7 &&, s7));
            writeStrings(idx + 8, CORE_CAST(S8 &&, s8));
            writeStrings(idx + 9, CORE_CAST(S9 &&, s9));
            writeStrings(idx + 10, CORE_CAST(S&&, s)...);
        }
    };
} // core

#endif //CORE24_STRINGARRAY_H
