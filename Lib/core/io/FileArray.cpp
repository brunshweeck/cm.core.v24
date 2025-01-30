//
// Created by brunshweeck on 15 sept. 2024.
//

#include <core/io/FileArray.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>
#include <core/util/NoSuchElementException.h>

namespace core {
    namespace io {
        FileArray::FileArray() CORE_NOTHROW: FileArray(0) {
        }

        FileArray::FileArray(gint length) {
            if (length < 0)
                IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

            try {
                value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(File)));
                new(value) File[length];
                count = length;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray::FileArray(gint length, File initialValue): ValueArray() {
            if (length < 0)
                IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

            try {
                value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(File)));
                value = new(value) File[length];
                count = length;
                if (initialValue.length() > 0) {
                    value[0] = UNSAFE::moveInstance(initialValue);
                    for (int i = 1; i < length; ++i) value[i] = value[0];
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray::FileArray(FileArray const& array): ValueArray(array) {
            try {
                value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(File)));
                count = array.count;

                for (int i = 0; i < count; ++i) value[i] = array.value[i];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray::FileArray(FileArray&& array) CORE_NOTHROW {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }

        FileArray::FileArray(VarArgs const& args): FileArray() {
            try {
                gint length = args.size();
                value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(File)));
                count = length;
                for (int i = 0; i < length; ++i)
                    new(value + i) File(UNSAFE::moveInstance(args.begin()[i]));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray& FileArray::operator=(const FileArray& other) {
            if (this != &other) {
                try {
                    if (count < other.count) {
                        value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                              other.count * sizeof(File)));
                        new(value + count) File[other.count - count];
                    }
                    count = other.count;

                    for (int i = 0; i < other.count; ++i) value[i] = other.value[i];
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
            return *this;
        }

        FileArray& FileArray::operator=(FileArray&& other) CORE_NOTHROW {
            if (this != &other) {
                UNSAFE::swapValues(value, other.value);
                UNSAFE::swapValues(count, other.count);
            }
            return *this;
        }

        gint FileArray::length() const { return count; }

        gbool FileArray::isEmpty() const { return (count == 0); }

        File& FileArray::get(gint index) {
            try {
                misc::Preconditions::checkIndex(index, count);
                return value[index];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        File const& FileArray::get(gint index) const {
            try {
                misc::Preconditions::checkIndex(index, count);
                return value[index];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        File FileArray::set(gint index, File newValue) {
            try {
                misc::Preconditions::checkIndex(index, count);
                value[index] = UNSAFE::moveInstance(newValue);
                return UNSAFE::moveInstance(newValue);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray::~FileArray() {
            const ARRAY a = value;
            const gint n = count;

            value = null;
            count = 0;

            if (n > 0) {
                for (int i = n - 1; i >= 0; --i)
                    a[i].~File();

                UNSAFE::freeMemory(CORE_CAST(glong, a));
            }
        }

        FileArray FileArray::of() { return {}; }

        FileArray FileArray::of(File s0) { return FileArray(1, UNSAFE::moveInstance(s0)); }

        FileArray FileArray::of(File s0, File s1) {
            try {
                FileArray sa = FileArray(2);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray FileArray::of(File s0, File s1, File s2) {
            try {
                FileArray sa = FileArray(3);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                sa.value[2] = UNSAFE::moveInstance(s2);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray FileArray::of(File s0, File s1, File s2, File s3) {
            try {
                FileArray sa = FileArray(4);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                sa.value[2] = UNSAFE::moveInstance(s2);
                sa.value[3] = UNSAFE::moveInstance(s3);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray FileArray::of(File s0, File s1, File s2, File s3, File s4) {
            try {
                FileArray sa = FileArray(5);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                sa.value[2] = UNSAFE::moveInstance(s2);
                sa.value[3] = UNSAFE::moveInstance(s3);
                sa.value[4] = UNSAFE::moveInstance(s4);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray FileArray::of(File s0, File s1, File s2, File s3, File s4,
                                    File s5) {
            try {
                FileArray sa = FileArray(6);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                sa.value[2] = UNSAFE::moveInstance(s2);
                sa.value[3] = UNSAFE::moveInstance(s3);
                sa.value[4] = UNSAFE::moveInstance(s4);
                sa.value[5] = UNSAFE::moveInstance(s5);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray FileArray::of(File s0, File s1, File s2, File s3, File s4,
                                    File s5, File s6) {
            try {
                FileArray sa = FileArray(7);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                sa.value[2] = UNSAFE::moveInstance(s2);
                sa.value[3] = UNSAFE::moveInstance(s3);
                sa.value[4] = UNSAFE::moveInstance(s4);
                sa.value[5] = UNSAFE::moveInstance(s5);
                sa.value[6] = UNSAFE::moveInstance(s6);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray FileArray::of(File s0, File s1, File s2, File s3, File s4,
                                    File s5, File s6, File s7) {
            try {
                FileArray sa = FileArray(8);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                sa.value[2] = UNSAFE::moveInstance(s2);
                sa.value[3] = UNSAFE::moveInstance(s3);
                sa.value[4] = UNSAFE::moveInstance(s4);
                sa.value[5] = UNSAFE::moveInstance(s5);
                sa.value[6] = UNSAFE::moveInstance(s6);
                sa.value[7] = UNSAFE::moveInstance(s7);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray FileArray::of(File s0, File s1, File s2, File s3, File s4,
                                    File s5, File s6, File s7, File s8) {
            try {
                FileArray sa = FileArray(9);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                sa.value[2] = UNSAFE::moveInstance(s2);
                sa.value[3] = UNSAFE::moveInstance(s3);
                sa.value[4] = UNSAFE::moveInstance(s4);
                sa.value[5] = UNSAFE::moveInstance(s5);
                sa.value[6] = UNSAFE::moveInstance(s6);
                sa.value[7] = UNSAFE::moveInstance(s7);
                sa.value[8] = UNSAFE::moveInstance(s8);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FileArray FileArray::of(File s0, File s1, File s2, File s3, File s4,
                                    File s5, File s6, File s7, File s8, File s9) {
            try {
                FileArray sa = FileArray(10);
                sa.value[0] = UNSAFE::moveInstance(s0);
                sa.value[1] = UNSAFE::moveInstance(s1);
                sa.value[2] = UNSAFE::moveInstance(s2);
                sa.value[3] = UNSAFE::moveInstance(s3);
                sa.value[4] = UNSAFE::moveInstance(s4);
                sa.value[5] = UNSAFE::moveInstance(s5);
                sa.value[6] = UNSAFE::moveInstance(s6);
                sa.value[7] = UNSAFE::moveInstance(s7);
                sa.value[8] = UNSAFE::moveInstance(s8);
                sa.value[9] = UNSAFE::moveInstance(s9);
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        File& FileArray::operator[](gint index) { return get(index); }

        File const& FileArray::operator[](gint index) const { return get(index); }

        FileArray::LinearIterator::LinearIterator(FileArray& array, gbool isEnd)
            : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
        }

        FileArray::LinearIterator& FileArray::LinearIterator::operator++() {
            isEnd = isEnd || ++next >= array.count;

            return *this;
        }

        File& FileArray::LinearIterator::operator*() const {
            if (!isEnd) return array[next];

            util::NoSuchElementException().throws($ftrace());
        }

        gbool FileArray::LinearIterator::operator==(LinearIterator const& rhs) const {
            return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
        }

        gbool FileArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

        FileArray::LinearIterator2::LinearIterator2(FileArray const& array, gbool isEnd)
            : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
        }

        FileArray::LinearIterator2& FileArray::LinearIterator2::operator++() {
            isEnd = isEnd || ++next >= array.count;

            return *this;
        }

        File const& FileArray::LinearIterator2::operator*() const {
            if (!isEnd) return array[next];

            util::NoSuchElementException().throws($ftrace());
        }

        gbool FileArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
            return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
        }

        gbool FileArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

        FileArray::LinearIterator FileArray::begin() { return LinearIterator(*this, false); }

        FileArray::LinearIterator2 FileArray::begin() const { return LinearIterator2(*this, false); }

        FileArray::LinearIterator FileArray::end() { return LinearIterator(*this, true); }

        FileArray::LinearIterator2 FileArray::end() const { return LinearIterator2(*this, true); }

        gbool FileArray::equals(const Object& obj) const {
            if (this == &obj) {
                return true;
            }

            if (!Class<FileArray>::hasInstance(obj)) {
                return false;
            }

            FileArray const& array = CORE_XCAST(FileArray const, obj);

            if (count != array.count) return false;

            for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

            return true;
        }

        gint FileArray::hash() const {
            gint const count = length();
            gint hash = 0;
            for (int i = 0; i < count; ++i) {
                hash = hash * 31 ^ (count - i - 1) + value[i].hash();
            }
            return hash;
        }

        String FileArray::toString() const {
            gint const count = length();

            XString str = XString(Math::max(count * (1 + 2) + 2, 16));
            str.append(u'[');
            for (int i = 0; i < count; ++i) {
                str.append(value[i]);
                if (i < count - 1) {
                    str.append(", "_S);
                }
            }
            str.append(u']');
            return str.toString();
        }

        Object& FileArray::clone() const {
            try {
                return UNSAFE::newInstance < FileArray > (*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void FileArray::initializeFirstTeens(File& f0, File& f1, File& f2, File& f3, File& f4,
                                               File& f5, File& f6, File& f7, File& f8, File& f9) {
            value[0] = UNSAFE::moveInstance(f0);
            value[1] = UNSAFE::moveInstance(f1);
            value[2] = UNSAFE::moveInstance(f2);
            value[3] = UNSAFE::moveInstance(f3);
            value[4] = UNSAFE::moveInstance(f4);
            value[5] = UNSAFE::moveInstance(f5);
            value[6] = UNSAFE::moveInstance(f6);
            value[7] = UNSAFE::moveInstance(f7);
            value[8] = UNSAFE::moveInstance(f8);
            value[9] = UNSAFE::moveInstance(f9);
        }

        void FileArray::writeAsFile(gint idx, File const& f) { value[idx] = f; }
        void FileArray::writeAsFile(gint idx, File& f) { value[idx] = f; }
        void FileArray::writeAsFile(gint idx, File&& f) { value[idx] = UNSAFE::moveInstance(f); }

        void FileArray::writeFiles(gint idx) {
            CORE_ASSERT(idx == count && idx > 10);
        }
    } // io
} // core
