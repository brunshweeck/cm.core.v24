//
// Created by brunshweeck on 15 sept. 2024.
//

#ifndef CORE24_FILEARRAY_H
#define CORE24_FILEARRAY_H

#include <core/io/File.h>

namespace core {
    namespace io {
        /**
         * The @c FileArray class wrap the array of File.
         * It's used by @c File to list child and root
         * files.
         */
        class FileArray final : public misc::ValueArray<File> {
        public:
            /**
             * Construct new empty @c FileArray
             */
            CORE_IMPLICIT FileArray() CORE_NOTHROW;

            /**
             * Construct new @c FileArray instance able to contains
             * the given number of values.
             *
             * @note All value will be initialized with value @c U+0000.
             *
             * @throws IllegalArgumentException If the given length is negative
             * @param length The number of value to be allocated on this array
             */
            CORE_EXPLICIT FileArray(gint length);

            /**
             * Construct new @c FileArray instance able to contains
             * the given number of values.
             *
             * @note All value will be initialized with given initial value.
             *
             * @throws IllegalArgumentException If the given length is negative
             * @param length The number of value to be allocated on this array
             * @param initialValue The value used to initialize all values of
             *          this array.
             */
            CORE_EXPLICIT FileArray(gint length, File initialValue);

            /**
             * Construct new @c FileArray instance by copy of values
             * of given array.
             *
             * @param array The array used to create this array.
             */
            CORE_IMPLICIT FileArray(FileArray const& array);

            /**
             * Construct new @c FileArray instance by swapping of content
             * of given array.
             *
             * @note After this construction, the given array will be
             * empty and this array will have the old length of given
             * array.
             *
             * @param array The array used to create this array.
             */
            CORE_IMPLICIT FileArray(FileArray&& array) CORE_NOTHROW;

            /**
             * Construct new @c FileArray with specified values.
             *
             * @param args the list of values used to create array.
             */
            CORE_IMPLICIT FileArray(VarArgs const& args);

            FileArray& operator=(const FileArray& other);

            FileArray& operator=(FileArray&& other) CORE_NOTHROW;

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
            File& get(gint index) override;

            /**
             * Return the value of elements at specified index
             *
             * @param index The index of desired element.
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            File const& get(gint index) const override;

            /**
             * Set value of element at specified index with
             * specified new value
             *
             * @param index The index of desired element.
             * @param newValue The replacement value
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            File set(gint index, File newValue) override;

            /**
             * Destroy this array data.
             *
             * @note This operation will clear content firstly.
             */
            ~FileArray() override;

            /**
             * Return new empty array
             *
             * @return the new empty array
             */
            static FileArray of();

            /**
             * Obtain new FileArray containing one value.
             *
             * @param f0 the first file
             * @return newly created array with one value.
             */
            static FileArray of(File f0);

            /**
             * Obtain new FileArray containing two values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @return newly created array with two values.
             */
            static FileArray of(File f0, File f1);

            /**
             * Obtain new FileArray containing three values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @return newly created array with three values.
             */
            static FileArray of(File f0, File f1, File f2);

            /**
             * Obtain new FileArray containing four values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @param f3 the fourth file
             * @return newly created array with four values.
             */
            static FileArray of(File f0, File f1, File f2, File f3);

            /**
             * Obtain new FileArray containing five values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @param f3 the fourth file
             * @param f4 the fifth file
             * @return newly created array with five values.
             */
            static FileArray of(File f0, File f1, File f2, File f3, File f4);

            /**
             * Obtain new FileArray containing six values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @param f3 the fourth file
             * @param f4 the fifth file
             * @param f5 the sixth file
             * @return newly created array with six values.
             */
            static FileArray of(File f0, File f1, File f2, File f3, File f4,
                                  File f5);

            /**
             * Obtain new FileArray containing seven values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @param f3 the fourth file
             * @param f4 the fifth file
             * @param f5 the sixth file
             * @param f6 the seventh file
             * @return newly created array with seven values.
             */
            static FileArray of(File f0, File f1, File f2, File f3, File f4,
                                  File f5, File f6);

            /**
             * Obtain new FileArray containing eight values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @param f3 the fourth file
             * @param f4 the fifth file
             * @param f5 the sixth file
             * @param f6 the seventh file
             * @param f7 the eigth file
             * @return newly created array with eight values.
             */
            static FileArray of(File f0, File f1, File f2, File f3, File f4,
                                  File f5, File f6, File f7);

            /**
             * Obtain new FileArray containing nine values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @param f3 the fourth file
             * @param f4 the fifth file
             * @param f5 the sixth file
             * @param f6 the seventh file
             * @param f7 the eigth file
             * @param f8 the ninth file
             * @return newly created array with nine values.
             */
            static FileArray of(File f0, File f1, File f2, File f3, File f4,
                                  File f5, File f6, File f7, File f8);

            /**
             * Obtain new FileArray containing ten values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @param f3 the fourth file
             * @param f4 the fifth file
             * @param f5 the sixth file
             * @param f6 the seventh file
             * @param f7 the eigth file
             * @param f8 the ninth file
             * @param f9 the tenth file
             * @return newly created array with ten values.
             */
            static FileArray of(File f0, File f1, File f2, File f3, File f4,
                                  File f5, File f6, File f7, File f8, File f9);

            /**
             * Obtain new FileArray containing ten+ values.
             *
             * @param f0 the first file
             * @param f1 the second file
             * @param f2 the third file
             * @param f3 the fourth file
             * @param f4 the fifth file
             * @param f5 the sixth file
             * @param f6 the seventh file
             * @param f7 the eigth file
             * @param f8 the ninth file
             * @param f9 the tenth file
             * @param f9 the tenth file
             * @param f the more files
             * @return newly created array with ten+ values.
             */
            template <class... Files>
            static FileArray of(File f0, File f1, File f2, File f3, File f4,
                                  File f5, File f6, File f7, File f8, File f9,
                                  Files&&... f) {
                FileArray sa = FileArray(10 + sizeof...(Files));
                sa.initializeFirstTeens(f0, f1, f2, f3, f4, f5, f6, f7, f8, f9);
                sa.writeFiles(10, CORE_CAST(Files &&, f)...);

                return sa;
            }

            File& operator[](gint index);

            File const& operator[](gint index) const;

        private:
            class LinearIterator final : public Object {
                CORE_ADD_AS_FRIEND(FileArray);

                FileArray& array;
                gint next = 0;
                gbool isEnd = false;

                CORE_EXPLICIT LinearIterator(FileArray& array, gbool isEnd);

            public:
                LinearIterator& operator++();

                File& operator*() const;

                gbool operator==(LinearIterator const& rhs) const;

                gbool operator!=(LinearIterator const& rhs) const;
            };

            class LinearIterator2 final : public Object {
                CORE_ADD_AS_FRIEND(FileArray);

                FileArray const& array;
                gint next = 0;
                gbool isEnd = false;

                CORE_EXPLICIT LinearIterator2(FileArray const& array, gbool isEnd);

            public:
                LinearIterator2& operator++();

                File const& operator*() const;

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

            void initializeFirstTeens(File& f0, File& f1, File& f2, File& f3, File& f4,
                                      File& f5, File& f6, File& f7, File& f8, File& f9);

            template <class F>
            void writeAsFile(gint idx, F&& f) {
                CORE_FAST_ASSERT(Class<File>::accept<F>() || Class<F>::isFile());
                writeAsFile(idx, ""_S + f);
            }

            void writeAsFile(gint idx, File const& f);

            void writeAsFile(gint idx, File& f);

            void writeAsFile(gint idx, File&& f);

            void writeFiles(gint idx);

            template <class F0>
            void writeFiles(gint idx, F0&& f0) { writeAsFile(idx + 0, CORE_CAST(F0 &&, f0)); }

            template <class F0, class F1>
            void writeFiles(gint idx, F0&& f0, F1&& f1) {
                writeAsFile(idx + 0, CORE_CAST(F0 &&, f0));
                writeAsFile(idx + 1, CORE_CAST(F1 &&, f1));
            }

            template <class F0, class F1, class F2>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
            }

            template <class F0, class F1, class F2, class F3>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2, F3&& f3) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
                writeFiles(idx + 3, CORE_CAST(F3 &&, f3));
            }

            template <class F0, class F1, class F2, class F3, class F4>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2, F3&& f3, F4&& f4) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
                writeFiles(idx + 3, CORE_CAST(F3 &&, f3));
                writeFiles(idx + 4, CORE_CAST(F4 &&, f4));
            }

            template <class F0, class F1, class F2, class F3, class F4, class F5>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2, F3&& f3, F4&& f4,
                              F5&& f5) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
                writeFiles(idx + 3, CORE_CAST(F3 &&, f3));
                writeFiles(idx + 4, CORE_CAST(F4 &&, f4));
                writeFiles(idx + 5, CORE_CAST(F5 &&, f5));
            }

            template <class F0, class F1, class F2, class F3, class F4,
                      class F5, class F6>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2, F3&& f3, F4&& f4,
                              F5&& f5, F6&& f6) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
                writeFiles(idx + 3, CORE_CAST(F3 &&, f3));
                writeFiles(idx + 4, CORE_CAST(F4 &&, f4));
                writeFiles(idx + 5, CORE_CAST(F5 &&, f5));
                writeFiles(idx + 6, CORE_CAST(F6 &&, f6));
            }

            template <class F0, class F1, class F2, class F3, class F4,
                      class F5, class F6, class F7>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2, F3&& f3, F4&& f4,
                              F5&& f5, F6&& f6, F7&& f7) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
                writeFiles(idx + 3, CORE_CAST(F3 &&, f3));
                writeFiles(idx + 4, CORE_CAST(F4 &&, f4));
                writeFiles(idx + 5, CORE_CAST(F5 &&, f5));
                writeFiles(idx + 6, CORE_CAST(F6 &&, f6));
                writeFiles(idx + 7, CORE_CAST(F7 &&, f7));
            }

            template <class F0, class F1, class F2, class F3, class F4,
                      class F5, class F6, class F7, class F8>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2, F3&& f3, F4&& f4,
                              F5&& f5, F6&& f6, F7&& f7, F8&& f8) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
                writeFiles(idx + 3, CORE_CAST(F3 &&, f3));
                writeFiles(idx + 4, CORE_CAST(F4 &&, f4));
                writeFiles(idx + 5, CORE_CAST(F5 &&, f5));
                writeFiles(idx + 6, CORE_CAST(F6 &&, f6));
                writeFiles(idx + 7, CORE_CAST(F7 &&, f7));
                writeFiles(idx + 8, CORE_CAST(F8 &&, f8));
            }

            template <class F0, class F1, class F2, class F3, class F4,
                      class F5, class F6, class F7, class F8, class F9>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2, F3&& f3, F4&& f4,
                              F5&& f5, F6&& f6, F7&& f7, F8&& f8, F9&& f9) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
                writeFiles(idx + 3, CORE_CAST(F3 &&, f3));
                writeFiles(idx + 4, CORE_CAST(F4 &&, f4));
                writeFiles(idx + 5, CORE_CAST(F5 &&, f5));
                writeFiles(idx + 6, CORE_CAST(F6 &&, f6));
                writeFiles(idx + 7, CORE_CAST(F7 &&, f7));
                writeFiles(idx + 8, CORE_CAST(F8 &&, f8));
                writeFiles(idx + 9, CORE_CAST(F9 &&, f9));
            }

            template <class F0, class F1, class F2, class F3, class F4,
                      class F5, class F6, class F7, class F8, class F9, class... F>
            void writeFiles(gint idx, F0&& f0, F1&& f1, F2&& f2, F3&& f3, F4&& f4,
                              F5&& f5, F6&& f6, F7&& f7, F8&& f8, F9&& f9, F&&... f) {
                writeFiles(idx + 0, CORE_CAST(F0 &&, f0));
                writeFiles(idx + 1, CORE_CAST(F1 &&, f1));
                writeFiles(idx + 2, CORE_CAST(F2 &&, f2));
                writeFiles(idx + 3, CORE_CAST(F3 &&, f3));
                writeFiles(idx + 4, CORE_CAST(F4 &&, f4));
                writeFiles(idx + 5, CORE_CAST(F5 &&, f5));
                writeFiles(idx + 6, CORE_CAST(F6 &&, f6));
                writeFiles(idx + 7, CORE_CAST(F7 &&, f7));
                writeFiles(idx + 8, CORE_CAST(F8 &&, f8));
                writeFiles(idx + 9, CORE_CAST(F9 &&, f9));
                writeFiles(idx + 10, CORE_CAST(F&&, f)...);
            }
        };
    } // io
} // core

#endif //CORE24_FILEARRAY_H
