//
// Created by brunshweeck on 22 juin 2024.
//

#include "Arrays.h"

#include <core/lang/BooleanArray.h>
#include <core/lang/DoubleArray.h>
#include <core/lang/FloatArray.h>
#include <core/lang/LongArray.h>
#include <core/lang/OutOfMemoryError.h>
#include <core/lang/ShortArray.h>
#include <core/lang/StringArray.h>
#include <core/function/DoubleUnaryOperator.h>
#include <core/function/IntUnaryOperator.h>
#include <core/function/LongUnaryOperator.h>
#include "meta/lang/DualPivotQuickSort.h"

namespace core {
    using namespace misc;

    namespace util {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        CORE_ALIAS(ArrayE, ObjectArray);

        template <class T, class V>
        static T check0(T t, V fn) {
            if (t) fn(*t);
            return t;
        }

        void Arrays::sort(IntArray& a) {
            try {
                DualPivotQuickSort::sort(a, 0, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(IntArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                DualPivotQuickSort::sort(a, fromIndex, toIndex);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(LongArray& a) {
            try {
                DualPivotQuickSort::sort(a, 0, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(LongArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                DualPivotQuickSort::sort(a, fromIndex, toIndex);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(ShortArray& a) {
            try {
                DualPivotQuickSort::sort(a, 0, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(ShortArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                DualPivotQuickSort::sort(a, fromIndex, toIndex);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(CharArray& a) {
            try {
                DualPivotQuickSort::sort(a, 0, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(CharArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                DualPivotQuickSort::sort(a, fromIndex, toIndex);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(ByteArray& a) {
            try {
                DualPivotQuickSort::sort(a, 0, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(ByteArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                DualPivotQuickSort::sort(a, fromIndex, toIndex);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(FloatArray& a) {
            try {
                DualPivotQuickSort::sort(a, 0, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(FloatArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                DualPivotQuickSort::sort(a, fromIndex, toIndex);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(DoubleArray& a) {
            try {
                DualPivotQuickSort::sort(a, 0, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(DoubleArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                DualPivotQuickSort::sort(a, fromIndex, toIndex);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(StringArray& a) {
            try {
                DualPivotQuickSort::sort(a, 0, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::sort(StringArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                DualPivotQuickSort::sort(a, fromIndex, toIndex);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::legacyMergeSort(StringArray& a, gint fromIndex, gint toIndex) {
            mergeSort(a, a, fromIndex, toIndex, -fromIndex);
        }

        void Arrays::mergeSort(StringArray& src, StringArray& dest, gint low, gint high, gint offset) {
            gint count = high - low;

            // Insertion sort on smallest arrays
            if (count < INSERTIONSORT_THRESHOLD) {
                for (int i = low; i < high; i++) {
                    for (gint j = i; j > low && dest[j - 1].compareTo(dest[j]) > 0; j--) {
                        dest[j] = UNSAFE::moveInstance(dest[j - 1]);
                    }
                }
                return;
            }

            // Recursively sort halves of dest into src
            gint destLow = low;
            gint destHigh = high;
            low += offset;
            high += offset;
            gint mid = (low + high) >> 1;
            mergeSort(dest, src, low, mid, -offset);
            mergeSort(dest, src, mid, high, -offset);

            // If list is already sorted, just copy from src to dest.  This is an
            // optimization that results in faster sorts for nearly ordered lists.
            if (src[mid - 1].compareTo(src[mid]) <= 0) {
                for (int i = 0; i < count; ++i) {
                    dest.value[i + destLow] = UNSAFE::moveInstance(src.value[i + low]);
                }
                return;
            }

            // Merge sorted halves (now in src) into dest
            for (int i = destLow, p = low, q = mid; i < destHigh; i++) {
                if (q >= high || p < mid && src[p].compareTo(src[q]) <= 0) {
                    dest.value[i] = UNSAFE::moveInstance(src.value[p++]);
                } else {
                    dest.value[i] = UNSAFE::moveInstance(src.value[q++]);
                }
            }
        }

        gint Arrays::binarySearch(ByteArray const& a, gbyte key) {
            try {
                gint low = 0;
                gint high = a.length() - 1;
                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gbyte midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(ByteArray const& a, gint fromIndex, gint toIndex, gbyte key) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());

                gint low = fromIndex;
                gint high = toIndex - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gbyte midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(ShortArray const& a, gshort key) {
            try {
                gint low = 0;
                gint high = a.length() - 1;
                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gshort midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(ShortArray const& a, gint fromIndex, gint toIndex, gshort key) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                gint low = fromIndex;
                gint high = toIndex - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gshort midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(CharArray const& a, gchar key) {
            try {
                gint low = 0;
                gint high = a.length() - 1;
                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gchar midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(CharArray const& a, gint fromIndex, gint toIndex, gchar key) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                gint low = fromIndex;
                gint high = toIndex - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gchar midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(IntArray const& a, gint key) {
            try {
                gint low = 0;
                gint high = a.length() - 1;
                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gint midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(IntArray const& a, gint fromIndex, gint toIndex, gint key) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                gint low = fromIndex;
                gint high = toIndex - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gint midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(LongArray const& a, glong key) {
            try {
                gint low = 0;
                gint high = a.length() - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    glong midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(LongArray const& a, gint fromIndex, gint toIndex, glong key) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                gint low = fromIndex;
                gint high = toIndex - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    glong midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(FloatArray const& a, gfloat key) {
            gint const keyBits = Float::toIntBits(key);

            try {
                gint low = 0;
                gint high = a.length() - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gfloat midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else {
                        gint const midBits = Float::toIntBits(midVal);
                        if (midBits < keyBits)
                            low = mid + 1;
                        else if (midBits > keyBits)
                            high = mid - 1;
                        else
                            return mid;
                    }
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(FloatArray const& a, gint fromIndex, gint toIndex, gfloat key) {
            gint const keyBits = Float::toIntBits(key);

            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                gint low = fromIndex;
                gint high = toIndex - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gfloat midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else {
                        gint const midBits = Float::toIntBits(midVal);
                        if (midBits < keyBits)
                            low = mid + 1;
                        else if (midBits > keyBits)
                            high = mid - 1;
                        else
                            return mid;
                    }
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(DoubleArray const& a, gdouble key) {
            glong const keyBits = Double::toLongBits(key);

            try {
                gint low = 0;
                gint high = a.length() - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gdouble midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else {
                        glong const midBits = Double::toLongBits(midVal);
                        if (midBits < keyBits)
                            low = mid + 1;
                        else if (midBits > keyBits)
                            high = mid - 1;
                        else
                            return mid;
                    }
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(DoubleArray const& a, gint fromIndex, gint toIndex, gdouble key) {
            glong const keyBits = Double::toLongBits(key);

            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());

                gint low = fromIndex;
                gint high = toIndex - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    gdouble midVal = a[mid];
                    if (midVal < key)
                        low = mid + 1;
                    else if (midVal > key)
                        high = mid - 1;
                    else {
                        glong const midBits = Double::toLongBits(midVal);
                        if (midBits < keyBits)
                            low = mid + 1;
                        else if (midBits > keyBits)
                            high = mid - 1;
                        else
                            return mid;
                    }
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(StringArray const& a, String const& key) {
            try {
                gint low = 0;
                gint high = a.length() - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    String const& midVal = a[mid];
                    gint r = midVal.compareTo(key);
                    if (r < 0)
                        low = mid + 1;
                    else if (r > 0)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::binarySearch(StringArray const& a, gint fromIndex, gint toIndex, String const& key) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                gint low = fromIndex;
                gint high = toIndex - 1;

                while (low <= high) {
                    gint mid = (high + low) >> 1;
                    String const& midVal = a[mid];
                    gint r = midVal.compareTo(key);
                    if (r < 0)
                        low = mid + 1;
                    else if (r > 0)
                        high = mid - 1;
                    else
                        return mid;
                }
                return -(low + 1);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::fill(ByteArray& a, gbyte val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(ByteArray& a, gint fromIndex, gint toIndex, gbyte val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill(ShortArray& a, gshort val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(ShortArray& a, gint fromIndex, gint toIndex, gshort val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill(CharArray& a, gchar val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(CharArray& a, gint fromIndex, gint toIndex, gchar val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill(IntArray& a, gint val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(IntArray& a, gint fromIndex, gint toIndex, gint val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill(LongArray& a, glong val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(LongArray& a, gint fromIndex, gint toIndex, glong val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill(FloatArray& a, gfloat val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(FloatArray& a, gint fromIndex, gint toIndex, gfloat val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill(DoubleArray& a, gdouble val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(DoubleArray& a, gint fromIndex, gint toIndex, gdouble val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill(BooleanArray& a, gbool val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(BooleanArray& a, gint fromIndex, gint toIndex, gbool val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill(StringArray& a, String const& val) {
            for (int i = 0; i < a.count; ++i) a.value[i] = val;
        }

        void Arrays::fill(StringArray& a, gint fromIndex, gint toIndex, String const& val) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) a.value[i] = val;
        }

        void Arrays::fill0(Object& a, Object const& val) {
            ArrayE& oa = CORE_XCAST(ArrayE, a);
            for (int i = 0; i < oa.count; ++i) oa.value[i] = &CORE_CAST(Object &, val);
        }

        void Arrays::fill0(Object& a, gint fromIndex, gint toIndex, Object const& val) {
            ArrayE& oa = CORE_XCAST(ArrayE, a);
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, oa.count);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (int i = fromIndex; i < toIndex; ++i) oa.value[i] = &CORE_CAST(Object &, val);
        }

        ByteArray Arrays::copyOf(ByteArray const& original, gint newLength) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            try {
                ByteArray ba = ByteArray(newLength);

                for (int i = 0, to = Math::min(newLength, original.count); i < to; ++i)
                    ba.value[i] = original.value[i];

                return ba;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copyOf0(Object const& src, Object& dest, gint newLength, Validator validator) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            ArrayE const& oa1 = CORE_XCAST(ArrayE const, src);
            ArrayE& oa2 = CORE_XCAST(ArrayE, dest);
            oa2.reserve(newLength);

            for (int i = 0, to = Math::min(newLength, oa1.count); i < to; ++i)
                oa2.value[i] = check0(oa1.value[i], validator);
        }

        void Arrays::copyOfRange0(Object const& src, gint fromIndex, gint toIndex, Object& dest, Validator validator) {
            ArrayE const& oa1 = CORE_XCAST(ArrayE const, src);
            ArrayE& oa2 = CORE_XCAST(ArrayE, dest);

            Preconditions::checkIndexFromRange(fromIndex, toIndex, oa1.count);

            gint newLength = toIndex - fromIndex;

            oa2.reserve(newLength);

            for (int i = fromIndex, j = 0; i < toIndex; ++i)
                oa2.value[j++] = check0(oa1.value[i], validator);
        }

        void Arrays::classChecked(Object const&) {
            // Do Nothing
        }

        ByteArray Arrays::copyOfRange(ByteArray const& original, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, original.count);
                gint const n = toIndex - fromIndex;
                ByteArray ba = ByteArray(n);
                for (int i = fromIndex, j = 0; i < toIndex; ++i) ba.value[j++] = original.value[i];
                return ba;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortArray Arrays::copyOf(ShortArray const& original, gint newLength) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            try {
                ShortArray sa = ShortArray(newLength);

                for (int i = 0, to = Math::min(newLength, original.count); i < to; ++i)
                    sa.value[i] = original.value[i];

                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortArray Arrays::copyOfRange(ShortArray const& original, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, original.count);
                gint const n = toIndex - fromIndex;
                ShortArray sa = ShortArray(n);
                for (int i = fromIndex, j = 0; i < toIndex; ++i) sa.value[j++] = original.value[i];
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray Arrays::copyOf(CharArray const& original, gint newLength) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            try {
                CharArray ca = CharArray(newLength);

                for (int i = 0, to = Math::min(newLength, original.count); i < to; ++i)
                    ca.value[i] = original.value[i];

                return ca;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray Arrays::copyOfRange(CharArray const& original, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, original.count);
                gint const n = toIndex - fromIndex;
                CharArray ca = CharArray(n);
                for (int i = fromIndex, j = 0; i < toIndex; ++i) ca.value[j++] = original.value[i];
                return ca;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntArray Arrays::copyOf(IntArray const& original, gint newLength) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            try {
                IntArray ia = IntArray(newLength);

                for (int i = 0, to = Math::min(newLength, original.count); i < to; ++i)
                    ia.value[i] = original.value[i];

                return ia;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntArray Arrays::copyOfRange(IntArray const& original, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, original.count);
                gint const n = toIndex - fromIndex;
                IntArray ia = IntArray(n);
                for (int i = fromIndex, j = 0; i < toIndex; ++i) ia.value[j++] = original.value[i];
                return ia;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongArray Arrays::copyOf(LongArray const& original, gint newLength) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            try {
                LongArray la = LongArray(newLength);

                for (int i = 0, to = Math::min(newLength, original.count); i < to; ++i)
                    la.value[i] = original.value[i];

                return la;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongArray Arrays::copyOfRange(LongArray const& original, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, original.count);
                gint const n = toIndex - fromIndex;
                LongArray la = LongArray(n);
                for (int i = fromIndex, j = 0; i < toIndex; ++i) la.value[j++] = original.value[i];
                return la;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatArray Arrays::copyOf(FloatArray const& original, gint newLength) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            try {
                FloatArray fa = FloatArray(newLength);

                for (int i = 0, to = Math::min(newLength, original.count); i < to; ++i)
                    fa.value[i] = original.value[i];

                return fa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatArray Arrays::copyOfRange(FloatArray const& original, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, original.count);
                gint const n = toIndex - fromIndex;
                FloatArray fa = FloatArray(n);
                for (int i = fromIndex, j = 0; i < toIndex; ++i) fa.value[j++] = original.value[i];
                return fa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleArray Arrays::copyOf(DoubleArray const& original, gint newLength) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            try {
                DoubleArray da = DoubleArray(newLength);

                for (int i = 0, to = Math::min(newLength, original.count); i < to; ++i)
                    da.value[i] = original.value[i];

                return da;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleArray Arrays::copyOfRange(DoubleArray const& original, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, original.count);
                gint const n = toIndex - fromIndex;
                DoubleArray da = DoubleArray(n);
                for (int i = fromIndex, j = 0; i < toIndex; ++i) da.value[j++] = original.value[i];
                return da;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        StringArray Arrays::copyOf(StringArray const& a, gint newLength) {
            if (newLength < 0)
                IllegalArgumentException("Negative array size: "_S + newLength).throws($ftrace());

            try {
                StringArray sa = StringArray(newLength);

                for (int i = 0, to = Math::min(newLength, a.count); i < to; ++i)
                    sa.value[i] = a.value[i];

                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        StringArray Arrays::copyOfRange(StringArray const& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.count);
                gint const n = toIndex - fromIndex;
                StringArray sa = StringArray(n);
                for (int i = fromIndex, j = 0; i < toIndex; ++i) sa.value[j++] = a.value[i];
                return sa;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }


        void Arrays::copy(BooleanArray const& src, gint srcOff, BooleanArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy(ByteArray const& src, gint srcOff, ByteArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy(ShortArray const& src, gint srcOff, ShortArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy(CharArray const& src, gint srcOff, CharArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy(IntArray const& src, gint srcOff, IntArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy(LongArray const& src, gint srcOff, LongArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy(FloatArray const& src, gint srcOff, FloatArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy(DoubleArray const& src, gint srcOff, DoubleArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy(StringArray const& src, gint srcOff, StringArray& dest, gint destOff, gint length) {
            try {
                Preconditions::checkIndexFromSize(srcOff, length, src.count);
                Preconditions::checkIndexFromSize(destOff, length, dest.count);

                for (gint i = 0; i < length; i++) dest.value[i + destOff] = src.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy0(Object const& src, gint srcOff, Object& dest, gint destOff, gint length) {
            ArrayE const& oa1 = CORE_XCAST(ArrayE const, src);
            ArrayE& oa2 = CORE_XCAST(ArrayE, dest);

            try {
                Preconditions::checkIndexFromSize(srcOff, length, oa1.count);
                Preconditions::checkIndexFromSize(destOff, length, oa2.count);

                for (gint i = 0; i < length; i++) oa2.value[i + destOff] = oa1.value[i + srcOff];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::copy0(Object const& src, gint srcOff,
                           Object& dest, gint destOff,
                           gint length, Validator validator) {
            ArrayE const& oa1 = CORE_XCAST(ArrayE const, src);
            ArrayE& oa2 = CORE_XCAST(ArrayE, dest);

            try {
                Preconditions::checkIndexFromSize(srcOff, length, oa1.count);
                Preconditions::checkIndexFromSize(destOff, length, oa2.count);

                for (gint i = 0; i < length; i++) oa2.value[i + destOff] = check0(oa1.value[i + srcOff], validator);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::newLength(gint oldLength, gint minGrowth, gint prefGrowth) {
            // preconditions not checked because of inlining
            // assert oldLength >= 0
            // assert minGrowth > 0

            gint prefLength = oldLength + Math::max(minGrowth, prefGrowth); // might overflow
            if (0 < prefLength && prefLength <= SOFT_MAX_ARRAY_LENGTH)
                return prefLength;
            // put code cold in a separate method
            // hugeLength:
            {
                int minLength = oldLength + minGrowth;
                if (minLength < 0) {
                    // overflow
                    OutOfMemoryError("Required array length "_S + oldLength + " + "_S + minGrowth + " is too large"_S)
                            .throws($ftrace());
                }
                if (minLength <= SOFT_MAX_ARRAY_LENGTH)
                    return SOFT_MAX_ARRAY_LENGTH;
                return minLength;
            }
        }

        void Arrays::reverse(BooleanArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(BooleanArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, toIndex--);
        }

        void Arrays::reverse(ByteArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(ByteArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, toIndex--);
        }

        void Arrays::reverse(CharArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(CharArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, toIndex--);
        }

        void Arrays::reverse(ShortArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(ShortArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, toIndex--);
        }

        void Arrays::reverse(IntArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(IntArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, toIndex--);
        }

        void Arrays::reverse(FloatArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(FloatArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, toIndex--);
        }

        void Arrays::reverse(LongArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(LongArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, toIndex--);
        }

        void Arrays::reverse(DoubleArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(DoubleArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, toIndex--);
        }

        void Arrays::reverse(StringArray& a) {
            return reverse(a, 0, a.length());
        }

        void Arrays::reverse(StringArray& a, gint fromIndex, gint toIndex) {
            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            while (toIndex - fromIndex > 1) swap(a, fromIndex++, --toIndex);
        }

        gint Arrays::mismatch(BooleanArray const& a, BooleanArray const& b) {
            if (&a == &b)
                return -1;
            gint length = Math::min(a.length(), b.length());
            gint i = mismatch(a, 0, b, 0, length);
            return (i < 0 && a.length() != b.length()) ? length : i;
        }

        gint Arrays::mismatch(BooleanArray const& a, gint aFrom, BooleanArray const& b, gint bFrom, gint length) {
            try {
                Preconditions::checkIndexFromSize(aFrom, length, a.length());
                Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint i = 0;
            if (length > 7) {
                if (a.value[aFrom] != b.value[bFrom])
                    return 0;
                gint aOffset = UNSAFE::ARRAY_BOOLEAN_BASE_OFFSET + aFrom;
                gint bOffset = UNSAFE::ARRAY_BOOLEAN_BASE_OFFSET + bFrom;
                i = vectorizedMismatch(a, aOffset, b, bOffset, length, LOG2_BOOLEAN_ARRAY_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFrom + i] != b[bFrom + i])
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(BooleanArray const& a, gint aFrom, gint aTo,
                              BooleanArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            gint i = mismatch(a, aFrom, b, bFrom, length);
            return (i < 0 && aLength != bLength) ? length : i;
        }

        gint Arrays::mismatch(ByteArray const& a, ByteArray const& b) {
            if (&a == &b)
                return -1;
            gint length = Math::min(a.length(), b.length());
            gint i = mismatch(a, 0, b, 0, length);
            return (i < 0 && a.length() != b.length()) ? length : i;
        }

        gint Arrays::mismatch(ByteArray const& a, gint aFrom, ByteArray const& b, gint bFrom, gint length) {
            try {
                Preconditions::checkIndexFromSize(aFrom, length, a.length());
                Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint i = 0;
            if (length > 7) {
                if (a.value[aFrom] != b.value[bFrom])
                    return 0;
                gint aOffset = UNSAFE::ARRAY_BYTE_BASE_OFFSET + aFrom;
                gint bOffset = UNSAFE::ARRAY_BYTE_BASE_OFFSET + bFrom;
                i = vectorizedMismatch(a, aOffset, b, bOffset, length, LOG2_BYTE_ARRAY_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFrom + i] != b[bFrom + i])
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(ByteArray const& a, gint aFrom, gint aTo,
                              ByteArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            gint i = mismatch(a, aFrom, b, bFrom, length);
            return (i < 0 && aLength != bLength) ? length : i;
        }

        gint Arrays::mismatch(CharArray const& a, CharArray const& b) {
            if (&a == &b)
                return -1;
            gint length = Math::min(a.length(), b.length());
            gint i = mismatch(a, 0, b, 0, length);
            return (i < 0 && a.length() != b.length()) ? length : i;
        }

        gint Arrays::mismatch(CharArray const& a, gint aFrom, CharArray const& b, gint bFrom, gint length) {
            try {
                Preconditions::checkIndexFromSize(aFrom, length, a.length());
                Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint i = 0;
            if (length > 7) {
                if (a.value[aFrom] != b.value[bFrom])
                    return 0;
                gint aOffset = UNSAFE::ARRAY_CHAR_BASE_OFFSET + aFrom;
                gint bOffset = UNSAFE::ARRAY_CHAR_BASE_OFFSET + bFrom;
                i = vectorizedMismatch(a, aOffset, b, bOffset, length, LOG2_CHAR_ARRAY_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFrom + i] != b[bFrom + i])
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(CharArray const& a, gint aFrom, gint aTo,
                              CharArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            gint i = mismatch(a, aFrom, b, bFrom, length);
            return (i < 0 && aLength != bLength) ? length : i;
        }

        gint Arrays::mismatch(ShortArray const& a, ShortArray const& b) {
            if (&a == &b)
                return -1;
            gint length = Math::min(a.length(), b.length());
            gint i = mismatch(a, 0, b, 0, length);
            return (i < 0 && a.length() != b.length()) ? length : i;
        }

        gint Arrays::mismatch(ShortArray const& a, gint aFrom, ShortArray const& b, gint bFrom, gint length) {
            try {
                Preconditions::checkIndexFromSize(aFrom, length, a.length());
                Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint i = 0;
            if (length > 7) {
                if (a.value[aFrom] != b.value[bFrom])
                    return 0;
                gint aOffset = UNSAFE::ARRAY_SHORT_BASE_OFFSET + aFrom;
                gint bOffset = UNSAFE::ARRAY_SHORT_BASE_OFFSET + bFrom;
                i = vectorizedMismatch(a, aOffset, b, bOffset, length, LOG2_SHORT_ARRAY_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFrom + i] != b[bFrom + i])
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(ShortArray const& a, gint aFrom, gint aTo,
                              ShortArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            gint i = mismatch(a, aFrom, b, bFrom, length);
            return (i < 0 && aLength != bLength) ? length : i;
        }

        gint Arrays::mismatch(IntArray const& a, IntArray const& b) {
            if (&a == &b)
                return -1;
            gint length = Math::min(a.length(), b.length());
            gint i = mismatch(a, 0, b, 0, length);
            return (i < 0 && a.length() != b.length()) ? length : i;
        }

        gint Arrays::mismatch(IntArray const& a, gint aFrom, IntArray const& b, gint bFrom, gint length) {
            try {
                Preconditions::checkIndexFromSize(aFrom, length, a.length());
                Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint i = 0;
            if (length > 7) {
                if (a.value[aFrom] != b.value[bFrom])
                    return 0;
                gint aOffset = UNSAFE::ARRAY_INT_BASE_OFFSET + aFrom;
                gint bOffset = UNSAFE::ARRAY_INT_BASE_OFFSET + bFrom;
                i = vectorizedMismatch(a, aOffset, b, bOffset, length, LOG2_INT_ARRAY_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFrom + i] != b[bFrom + i])
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(IntArray const& a, gint aFrom, gint aTo,
                              IntArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            gint i = mismatch(a, aFrom, b, bFrom, length);
            return (i < 0 && aLength != bLength) ? length : i;
        }

        gint Arrays::mismatch(FloatArray const& a, FloatArray const& b) {
            if (&a == &b)
                return -1;
            gint length = Math::min(a.length(), b.length());
            gint i = mismatch(a, 0, b, 0, length);
            return (i < 0 && a.length() != b.length()) ? length : i;
        }

        gint Arrays::mismatch(FloatArray const& a, gint aFrom, FloatArray const& b, gint bFrom, gint length) {
            try {
                Preconditions::checkIndexFromSize(aFrom, length, a.length());
                Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint i = 0;
            if (length > 7) {
                if (Float::toRawIntBits(a.value[aFrom]) == Float::toRawIntBits(b.value[bFrom])) {
                    gint aOffset = UNSAFE::ARRAY_FLOAT_BASE_OFFSET + aFrom;
                    gint bOffset = UNSAFE::ARRAY_FLOAT_BASE_OFFSET + bFrom;
                    i = vectorizedMismatch(a, aOffset, b, bOffset, length, LOG2_FLOAT_ARRAY_INDEX_SCALE);
                }
                // Mismatched
                if (i >= 0) {
                    // Check if mismatch is not associated with two NaN values
                    if (!Float::isNaN(a[aFrom + i]) || !Float::isNaN(b[bFrom + i]))
                        return i;

                    // Mismatch on two different NaN values that are normalized to match
                    // Fall back to slow mechanism
                    // ISSUE: Consider looping over vectorizedMismatch adjusting ranges
                    // However, requires that returned value be relative to input ranges
                    i++;
                }
            } else
                i = length - ~i;

            for (; i < length; i++) {
                if (Float::toIntBits(a[aFrom + i]) != Float::toIntBits(b[bFrom + i]))
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(FloatArray const& a, gint aFrom, gint aTo,
                              FloatArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            gint i = mismatch(a, aFrom, b, bFrom, length);
            return (i < 0 && aLength != bLength) ? length : i;
        }

        gint Arrays::mismatch(LongArray const& a, LongArray const& b) {
            if (&a == &b)
                return -1;
            gint length = Math::min(a.length(), b.length());
            gint i = mismatch(a, 0, b, 0, length);
            return (i < 0 && a.length() != b.length()) ? length : i;
        }

        gint Arrays::mismatch(LongArray const& a, gint aFrom, LongArray const& b, gint bFrom, gint length) {
            try {
                Preconditions::checkIndexFromSize(aFrom, length, a.length());
                Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint i = 0;
            if (length > 7) {
                if (a.value[aFrom] != b.value[bFrom])
                    return 0;
                gint aOffset = UNSAFE::ARRAY_LONG_BASE_OFFSET + aFrom;
                gint bOffset = UNSAFE::ARRAY_LONG_BASE_OFFSET + bFrom;
                i = vectorizedMismatch(a, aOffset, b, bOffset, length, LOG2_LONG_ARRAY_INDEX_SCALE);
                if (i >= 0)
                    return i;
                i = length - ~i;
            }
            for (; i < length; i++) {
                if (a[aFrom + i] != b[bFrom + i])
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(LongArray const& a, gint aFrom, gint aTo,
                              LongArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            gint i = mismatch(a, aFrom, b, bFrom, length);
            return (i < 0 && aLength != bLength) ? length : i;
        }

        gint Arrays::mismatch(DoubleArray const& a, DoubleArray const& b) {
            if (&a == &b)
                return -1;
            gint length = Math::min(a.length(), b.length());
            gint i = mismatch(a, 0, b, 0, length);
            return (i < 0 && a.length() != b.length()) ? length : i;
        }

        gint Arrays::mismatch(DoubleArray const& a, gint aFrom, DoubleArray const& b, gint bFrom, gint length) {
            try {
                Preconditions::checkIndexFromSize(aFrom, length, a.length());
                Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint i = 0;
            if (length > 7) {
                if (Double::toRawLongBits(a.value[aFrom]) == Double::toRawLongBits(b.value[bFrom])) {
                    gint aOffset = UNSAFE::ARRAY_FLOAT_BASE_OFFSET + aFrom;
                    gint bOffset = UNSAFE::ARRAY_FLOAT_BASE_OFFSET + bFrom;
                    i = vectorizedMismatch(a, aOffset, b, bOffset, length, LOG2_FLOAT_ARRAY_INDEX_SCALE);
                }
                // Mismatched
                if (i >= 0) {
                    // Check if mismatch is not associated with two NaN values
                    if (!Double::isNaN(a[aFrom + i]) || !Double::isNaN(b[bFrom + i]))
                        return i;

                    // Mismatch on two different NaN values that are normalized to match
                    // Fall back to slow mechanism
                    // ISSUE: Consider looping over vectorizedMismatch adjusting ranges
                    // However, requires that returned value be relative to input ranges
                    i++;
                }
            } else
                i = length - ~i;

            for (; i < length; i++) {
                if (Double::toLongBits(a[aFrom + i]) != Double::toLongBits(b[bFrom + i]))
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(DoubleArray const& a, gint aFrom, gint aTo,
                              DoubleArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            gint i = mismatch(a, aFrom, b, bFrom, length);
            return (i < 0 && aLength != bLength) ? length : i;
        }

        gint Arrays::mismatch(StringArray const& a, StringArray const& b) {
            if (&a == &b)
                return -1;
            gint aLength = a.length();
            gint bLength = b.length();
            gint length = Math::min(aLength, bLength);
            for (gint i = 0; i < length; ++i) {
                if (a.value[i] != b.value[i])
                    return i;
            }
            return aLength != bLength ? length : -1;
        }

        gint Arrays::mismatch(StringArray const& a, gint aFrom, StringArray const& b, gint bFrom, gint length) {
            try {
                misc::Preconditions::checkIndexFromSize(aFrom, length, a.length());
                misc::Preconditions::checkIndexFromSize(bFrom, length, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (&a == &b)
                return -1;
            for (gint i = 0; i < length; ++i) {
                if (a.value[aFrom + i] != b.value[bFrom + i])
                    return i;
            }
            return -1;
        }

        gint Arrays::mismatch(StringArray const& a, gint aFrom, gint aTo, StringArray const& b, gint bFrom, gint bTo) {
            try {
                misc::Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                misc::Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (&a == &b)
                return -1;
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint length = Math::min(aLength, bLength);
            for (gint i = 0; i < length; ++i) {
                if (a.value[aFrom + i] != b.value[bFrom + i])
                    return i;
            }
            return aLength != bLength ? length : -1;
        }

        gint Arrays::hash(BooleanArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + Boolean::hash(a.value[i]);
            return result;
        }

        gint Arrays::hash(ByteArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + a.value[i];
            return result;
        }

        gint Arrays::hash(CharArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + a.value[i];
            return result;
        }

        gint Arrays::hash(ShortArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + a.value[i];
            return result;
        }

        gint Arrays::hash(IntArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + a.value[i];
            return result;
        }

        gint Arrays::hash(FloatArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + Float::hash(a.value[i]);
            return result;
        }

        gint Arrays::hash(LongArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + Long::hash(a.value[i]);
            return result;
        }

        gint Arrays::hash(DoubleArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + Double::hash(a.value[i]);
            return result;
        }

        gint Arrays::hash(StringArray const& a) {
            gint result = 0;
            gint length = a.length();
            for (gint i = 0; i < length; ++i)
                result = result * 31 + a.value[i].hash();
            return result;
        }

        void Arrays::setAll(IntArray& array, function::IntUnaryOperator const& generator) {
            gint length = array.length();
            try {
                for (gint i = 0; i < length; i++)
                    array.value[i] = generator.apply(i);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::setAll(LongArray& array, function::LongUnaryOperator const& generator) {
            gint length = array.length();
            try {
                for (gint i = 0; i < length; i++)
                    array.value[i] = generator.apply(i);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void Arrays::setAll(DoubleArray& array, function::DoubleUnaryOperator const& generator) {
            gint length = array.length();
            try {
                for (gint i = 0; i < length; i++)
                    array.value[i] = generator.apply(i);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::compare(BooleanArray const& a, BooleanArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Boolean::compare(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compare(BooleanArray const& a, gint aFrom, gint aTo,
                             BooleanArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Boolean::compare(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compare(ByteArray const& a, ByteArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Byte::compare(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compare(ByteArray const& a, gint aFrom, gint aTo, ByteArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Byte::compare(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compareUnsigned(ByteArray const& a, ByteArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Byte::compareUnsigned(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compareUnsigned(ByteArray const& a, gint aFrom, gint aTo,
                                     ByteArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Byte::compareUnsigned(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compare(CharArray const& a, CharArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Character::compare(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compare(CharArray const& a, gint aFrom, gint aTo,
                             CharArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Character::compare(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compare(ShortArray const& a, ShortArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Short::compare(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compare(ShortArray const& a, gint aFrom, gint aTo,
                             ShortArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Short::compare(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compareUnsigned(ShortArray const& a, ShortArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Short::compareUnsigned(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compareUnsigned(ShortArray const& a, gint aFrom, gint aTo,
                                     ShortArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Short::compareUnsigned(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compare(IntArray const& a, IntArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Integer::compare(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compare(IntArray const& a, gint aFrom, gint aTo,
                             IntArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Integer::compare(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compareUnsigned(IntArray const& a, IntArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Integer::compareUnsigned(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compareUnsigned(IntArray const& a, gint aFrom, gint aTo,
                                     IntArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Integer::compareUnsigned(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compare(FloatArray const& a, FloatArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Float::compare(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compare(FloatArray const& a, gint aFrom, gint aTo,
                             FloatArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Float::compare(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compare(LongArray const& a, LongArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Long::compare(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compare(LongArray const& a, gint aFrom, gint aTo,
                             LongArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Long::compare(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compareUnsigned(LongArray const& a, LongArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Long::compareUnsigned(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compareUnsigned(LongArray const& a, gint aFrom, gint aTo,
                                     LongArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Long::compareUnsigned(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gint Arrays::compare(DoubleArray const& a, DoubleArray const& b) {
            if (&a == &b)
                return 0;
            gint aLength = a.length();
            gint bLength = b.length();
            gint i = mismatch(a, 0, b, 0, Math::min(aLength, bLength));
            if (i >= 0)
                return Double::compare(a.value[i], b.value[i]);
            return aLength - bLength;
        }

        gint Arrays::compare(DoubleArray const& a, gint aFrom, gint aTo, DoubleArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            gint i = mismatch(a, aFrom, b, bFrom, Math::min(aLength, bLength));
            if (i >= 0)
                return Double::compare(a.value[aFrom + i], b.value[bFrom + i]);
            return aLength - bLength;
        }

        gbool Arrays::equals(BooleanArray const& a, BooleanArray const& a2) {
            if (&a == &a2)
                return true;
            if (a.length() != a2.length())
                return false;
            return mismatch(a, a2) < 0;
        }

        gbool Arrays::equals(BooleanArray const& a, gint aFrom, gint aTo,
                             BooleanArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            return mismatch(a, aFrom, b, bFrom, aLength) < 0;
        }

        gbool Arrays::equals(ByteArray const& a, ByteArray const& a2) {
            if (&a == &a2)
                return true;
            if (a.length() != a2.length())
                return false;
            return mismatch(a, a2) < 0;
        }

        gbool Arrays::equals(ByteArray const& a, gint aFrom, gint aTo,
                             ByteArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            return mismatch(a, aFrom, b, bFrom, aLength) < 0;
        }

        gbool Arrays::equals(CharArray const& a, CharArray const& a2) {
            if (&a == &a2)
                return true;
            if (a.length() != a2.length())
                return false;
            return mismatch(a, a2) < 0;
        }

        gbool Arrays::equals(CharArray const& a, gint aFrom, gint aTo,
                             CharArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            return mismatch(a, aFrom, b, bFrom, aLength) < 0;
        }

        gbool Arrays::equals(ShortArray const& a, ShortArray const& a2) {
            if (&a == &a2)
                return true;
            if (a.length() != a2.length())
                return false;
            return mismatch(a, a2) < 0;
        }

        gbool Arrays::equals(ShortArray const& a, gint aFrom, gint aTo,
                             ShortArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            return mismatch(a, aFrom, b, bFrom, aLength) < 0;
        }

        gbool Arrays::equals(IntArray const& a, IntArray const& a2) {
            if (&a == &a2)
                return true;
            if (a.length() != a2.length())
                return false;
            return mismatch(a, a2) < 0;
        }

        gbool Arrays::equals(IntArray const& a, gint aFrom, gint aTo,
                             IntArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            return mismatch(a, aFrom, b, bFrom, aLength) < 0;
        }

        gbool Arrays::equals(FloatArray const& a, FloatArray const& a2) {
            if (&a == &a2)
                return true;
            if (a.length() != a2.length())
                return false;
            return mismatch(a, a2) < 0;
        }

        gbool Arrays::equals(FloatArray const& a, gint aFrom, gint aTo,
                             FloatArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            return mismatch(a, aFrom, b, bFrom, aLength) < 0;
        }

        gbool Arrays::equals(LongArray const& a, LongArray const& a2) {
            if (&a == &a2)
                return true;
            if (a.length() != a2.length())
                return false;
            return mismatch(a, a2) < 0;
        }

        gbool Arrays::equals(LongArray const& a, gint aFrom, gint aTo,
                             LongArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            return mismatch(a, aFrom, b, bFrom, aLength) < 0;
        }

        gbool Arrays::equals(DoubleArray const& a, DoubleArray const& a2) {
            if (&a == &a2)
                return true;
            if (a.length() != a2.length())
                return false;
            return mismatch(a, a2) < 0;
        }

        gbool Arrays::equals(DoubleArray const& a, gint aFrom, gint aTo,
                             DoubleArray const& b, gint bFrom, gint bTo) {
            try {
                Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            return mismatch(a, aFrom, b, bFrom, aLength) < 0;
        }

        void Arrays::reverse0(Object& a, gint fromIndex, gint toIndex) {
            ArrayE& oa = CORE_XCAST(ArrayE, a);

            try {
                Preconditions::checkIndexFromRange(fromIndex, toIndex, oa.count);

                while (fromIndex < toIndex) {
                    ArrayE::VALUE t = oa.value[fromIndex];

                    oa.value[fromIndex] = oa.value[toIndex - 1];
                    oa.value[toIndex - 1] = t;

                    fromIndex += 1;
                    toIndex -= 1;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Arrays::vectorizedMismatch(Object const& a, glong aOffset,
                                        Object const& b, glong bOffset,
                                        gint length, gint log2ArrayIndexScale) {
            glong wi = 0;
            glong len = length << log2ArrayIndexScale;
            for (; wi < len >> LOG2_LONG_BIT_SIZE; wi++) {
                glong bi = wi << LOG2_LONG_BIT_SIZE;
                glong av = UNSAFE::getLongUnaligned(a, aOffset + bi);
                glong bv = UNSAFE::getLongUnaligned(b, bOffset + bi);
                if (av != bv) {
                    glong x = av ^ bv;
                    glong i = 0;
                    do {
                        x >>= LOG2_LONG_BIT_SIZE;
                        i++;
                    } while (x);
                    return CORE_CAST(gint, (bi + (~i & 0x7)) >> log2ArrayIndexScale);
                }
            }

            glong off = wi << LOG2_LONG_BIT_SIZE;
            len -= off;
            aOffset += off;
            bOffset += off;
            if (log2ArrayIndexScale < LOG2_INT_BIT_SIZE) {
                glong xi = 0;
                for (; xi < len >> LOG2_INT_BIT_SIZE; xi++) {
                    glong bi = xi << LOG2_INT_BIT_SIZE;
                    gint av = UNSAFE::getIntUnaligned(a, aOffset + bi);
                    gint bv = UNSAFE::getIntUnaligned(b, bOffset + bi);
                    if (av != bv) {
                        gint x = av ^ bv;
                        gint i = 0;
                        do {
                            x >>= LOG2_INT_BIT_SIZE;
                            i += 1;
                        } while (x);
                        return CORE_CAST(gint, (off + bi + (~i & 0x3)) >> log2ArrayIndexScale);
                    }
                }

                glong off2 = xi << LOG2_INT_BIT_SIZE;
                len -= off2;
            }

            return ~CORE_CAST(gint, len >> log2ArrayIndexScale);
        }

        glong Arrays::array2DirectAccess(ArrayE const& a) { return CORE_CAST(glong, a.value); }

        glong Arrays::array2DirectAccess(BooleanArray const& a) { return CORE_CAST(glong, a.value); }

        glong Arrays::array2DirectAccess(ByteArray const& a) { return CORE_CAST(glong, a.value); }

        glong Arrays::array2DirectAccess(ShortArray const& a) { return CORE_CAST(glong, a.value); }

        glong Arrays::array2DirectAccess(CharArray const& a) { return CORE_CAST(glong, a.value); }

        glong Arrays::array2DirectAccess(IntArray const& a) { return CORE_CAST(glong, a.value); }

        glong Arrays::array2DirectAccess(FloatArray const& a) { return CORE_CAST(glong, a.value); }

        glong Arrays::array2DirectAccess(LongArray const& a) { return CORE_CAST(glong, a.value); }

        glong Arrays::array2DirectAccess(DoubleArray const& a) { return CORE_CAST(glong, a.value); }

        Object& Arrays::fastSet0(ArrayE& a, gint index, Object& x) { return a.set0(index, x); }

        gbool Arrays::equals(const ArrayE& a, const ArrayE& b) {
            if (&a == &b)
                return true;
            gint length = a.length();
            if (length != b.length())
                return false;

            for (gint i = 0; i < length; ++i) {
                if (a.get0(i) != b.get0(i))
                    return false;
            }
            return true;
        }

        gbool Arrays::equals(const ArrayE& a, gint aFrom, gint aTo,
                             const ArrayE& b, gint bFrom, gint bTo) {
            gint aLength = aTo - aFrom;
            gint bLength = bTo - bFrom;
            if (aLength != bLength)
                return false;
            for (gint i = 0; i < aLength; ++i) {
                if (a.get0(aFrom + i) != b.get0(bFrom + i))
                    return false;
            }
            return true;
        }

        void Arrays::swap0(ArrayE& a, gint i, gint j) {
            ArrayE::VALUE tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(BooleanArray& a, gint i, gint j) {
            try {
                gint length = a.length();
                Preconditions::checkIndex(i, length);
                Preconditions::checkIndex(j, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gbool tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(ByteArray& a, gint i, gint j) {
            try {
                gint length = a.length();
                Preconditions::checkIndex(i, length);
                Preconditions::checkIndex(j, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gbyte tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(CharArray& a, gint i, gint j) {
            try {
                gint length = a.length();
                Preconditions::checkIndex(i, length);
                Preconditions::checkIndex(j, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gchar tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(ShortArray& a, gint i, gint j) {
            try {
                gint length = a.length();
                Preconditions::checkIndex(i, length);
                Preconditions::checkIndex(j, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gshort tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(IntArray& a, gint i, gint j) {
            try {
                gint length = a.length();
                Preconditions::checkIndex(i, length);
                Preconditions::checkIndex(j, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(FloatArray& a, gint i, gint j) {
            try {
                gint length = a.length();
                Preconditions::checkIndex(i, length);
                Preconditions::checkIndex(j, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gfloat tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(LongArray& a, gint i, gint j) {
            try {
                gint length = a.length();
                Preconditions::checkIndex(i, length);
                Preconditions::checkIndex(j, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            glong tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(DoubleArray& a, gint i, gint j) {
            try {
                gint length = a.length();
                Preconditions::checkIndex(i, length);
                Preconditions::checkIndex(j, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gdouble tmp = a.value[i];
            a.value[i] = a.value[j];
            a.value[j] = tmp;
        }

        void Arrays::swap(StringArray& a, gint i, gint j) {
            a.value[i] = UNSAFE::moveInstance(a.value[j]);
        }

        CORE_WARNING_POP
    } // utils
} // core
