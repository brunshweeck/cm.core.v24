//
// Created by brunshweeck on 23 juin 2024.
//

#ifndef CORE24_DUALPRIVOTQUICKSORT_H
#define CORE24_DUALPRIVOTQUICKSORT_H

#include <core/IntArray.h>
#include <core/util/Arrays.h>

namespace core {
    namespace util {
        /**
         * This class implements powerful and fully optimized versions, both
         * sequential, of the Dual-Pivot Quicksort algorithm. This algorithm
         * offers O(n log(n)) performance on all data sets, and is typically
         * faster than traditional (one-pivot) Quicksort implementations.
         *
         * There are also additional algorithms, invoked from the Dual-Pivot
         * Quicksort, such as mixed insertion sort, merging of runs and heap
         * sort, counting sort and parallel merge sort.
         */
        class DualPivotQuickSort final : public Object {
            CORE_ADD_AS_FRIEND(Arrays);

            static CORE_FAST gint MAX_MIXED_INSERTION_SORT_SIZE = (1 << 6 | 1LL << 1) - 1;
            static CORE_FAST gint MAX_INSERTION_SORT_SIZE = 1 << 5 | 1 << 3 | 1 << 2;
            static CORE_FAST gint MIN_PARALLEL_SORT_SIZE = 1 << 12;
            static CORE_FAST gint MIN_TRY_MERGE_SIZE = 1 << 12;
            static CORE_FAST gint MIN_FIRST_RUN_SIZE = 1 << 4;
            static CORE_FAST gint MIN_FIRST_RUNS_FACTOR = (1 << 3) - 1;
            static CORE_FAST gint MAX_RUN_CAPACITY = 1 << 12 | 1 << 10;
            static CORE_FAST gint MIN_RUN_COUNT = 1 << 12;
            static CORE_FAST gint MIN_PARALLEL_MERGE_PARTS_SIZE = 1 << 12;
            static CORE_FAST gint MIN_BYTE_COUNTING_SORT_SIZE = 1 << 6;
            static CORE_FAST gint MIN_SHORT_COUNTING_SORT_SIZE = 1 << 10 | 1 << 9 | 1 << 8;
            static CORE_FAST gint MIN_CHAR_COUNTING_SORT_SIZE = 1 << 10 | 1 << 9 | 1 << 8;
            static CORE_FAST gint DELTA = 1 << 2 | 1 << 1;
            static CORE_FAST gint MAX_RECURSION_DEPTH = 1 << 8 | 1 << 7; // 64 * DELTA

            // [gint]

            /**
             * Sorts the specified range of the array using parallel merge
             * sort and/or Dual-Pivot Quicksort.
             *
             * To balance the faster splitting and parallelism of merge sort
             * with the faster element partitioning of Quicksort, ranges are
             * subdivided in tiers such that, if there is enough parallelism,
             * the four-way parallel merge is started, still ensuring enough
             * parallelism to process the partitions.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(IntArray &a, gint low, gint high);

            /**
             * Sorts the specified array using the Dual-Pivot Quicksort and/or
             * other sorts in special-cases.
             *
             * @param a The array to be sorted
             * @param bits the combination of recursion depth and bit flag, where
             *        the right bit "0" indicates that array is the leftmost part
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(IntArray &a, gint bits, gint low, gint high);

            /**
             * Sorts the specified range of the array using insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void insertionSort(IntArray &a, gint low, gint high);

            /**
             * Sorts the specified range of the array using mixed insertion sort.
             *
             * Mixed insertion sort is combination of simple insertion sort,
             * pin insertion sort and pair insertion sort.
             *
             * In the context of Dual-Pivot Quicksort, the pivot element
             * from the left part plays the role of sentinel, because it
             * is less than any elements from the given part. Therefore,
             * expensive check of the left range can be skipped on each
             * iteration unless it is the leftmost call.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param end the index of the last element for simple insertion sort
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void mixedInsertionSort(IntArray &a, gint low, gint end, gint high);

            /**
             * Sorts the specified range of the array using heap sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void heapSort(IntArray &a, gint low, gint high);

            /**
             * Pushes specified element down during heap sort.
             *
             * @param a the given array
             * @param p the start index
             * @param value the given element
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void pushDown(IntArray &a, gint p, gint value, gint low, gint high);

            /**
             * Tries to sort the specified range of the array.
             *
             * @param a the array to be sorted
             * @param low the index of the first element to be sorted
             * @param size the array size
             * @return true if finally sorted, false otherwise
             */
            static gbool tryMergeRuns(IntArray &a, gint low, gint size);

            /**
             * Merges the specified runs.
             *
             * @param a the source array
             * @param b the temporary buffer used in merging
             * @param offset the start index in the source, inclusive
             * @param aim specifies merging: to source ( > 0), buffer ( < 0) or any ( == 0)
             * @param run the start indexes of the runs, inclusive
             * @param lo the start index of the first run, inclusive
             * @param hi the start index of the last run, inclusive
             * @return the destination where runs are merged
             */
            static IntArray mergeRuns(IntArray &a, IntArray &b, gint offset, gint aim, IntArray &run, gint lo, gint hi);

            /**
             * Merges the sorted parts.
             *
             * @param dst the destination where parts are merged
             * @param k the start index of the destination, inclusive
             * @param a1 the first part
             * @param lo1 the start index of the first part, inclusive
             * @param hi1 the end index of the first part, exclusive
             * @param a2 the second part
             * @param lo2 the start index of the second part, inclusive
             * @param hi2 the end index of the second part, exclusive
             */
            static void mergeParts(IntArray &dst, gint k,
                                   IntArray &a1, gint lo1, gint hi1, IntArray &a2, gint lo2, gint hi2);


            // [glong]

            /**
             * Sorts the specified range of the array using parallel merge
             * sort and/or Dual-Pivot Quicksort.
             *
             * To balance the faster splitting and parallelism of merge sort
             * with the faster element partitioning of Quicksort, ranges are
             * subdivided in tiers such that, if there is enough parallelism,
             * the four-way parallel merge is started, still ensuring enough
             * parallelism to process the partitions.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(LongArray &a, gint low, gint high);

            /**
             * Sorts the specified array using the Dual-Pivot Quicksort and/or
             * other sorts in special-cases.
             *
             * @param a the array to be sorted
             * @param bits the combination of recursion depth and bit flag, where
             *        the right bit "0" indicates that array is the leftmost part
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(LongArray &a, gint bits, gint low, gint high);

            /**
             * Sorts the specified range of the array using mixed insertion sort.
             *
             * Mixed insertion sort is combination of simple insertion sort,
             * pin insertion sort and pair insertion sort.
             *
             * In the context of Dual-Pivot Quicksort, the pivot element
             * from the left part plays the role of sentinel, because it
             * is less than any elements from the given part. Therefore,
             * expensive check of the left range can be skipped on each
             * iteration unless it is the leftmost call.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param end the index of the last element for simple insertion sort
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void mixedInsertionSort(LongArray &a, gint low, gint end, gint high);

            /**
             * Sorts the specified range of the array using insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void insertionSort(LongArray &a, gint low, gint high);

            /**
             * Sorts the specified range of the array using heap sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void heapSort(LongArray &a, gint low, gint high);

            /**
             * Pushes specified element down during heap sort.
             *
             * @param a the given array
             * @param p the start index
             * @param value the given element
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void pushDown(LongArray &a, gint p, glong value, gint low, gint high);

            /**
             * Tries to sort the specified range of the array.
             *
             * @param a the array to be sorted
             * @param low the index of the first element to be sorted
             * @param size the array size
             * @return true if finally sorted, false otherwise
             */
            static gbool tryMergeRuns(LongArray &a, gint low, gint size);

            /**
             * Merges the specified runs.
             *
             * @param a the source array
             * @param b the temporary buffer used in merging
             * @param offset the start index in the source, inclusive
             * @param aim specifies merging: to source ( > 0), buffer ( < 0) or any ( == 0)
             * @param run the start indexes of the runs, inclusive
             * @param lo the start index of the first run, inclusive
             * @param hi the start index of the last run, inclusive
             * @return the destination where runs are merged
             */
            static LongArray &mergeRuns(LongArray &a, LongArray &b, gint offset,
                                        gint aim, IntArray &run, gint lo, gint hi);

            /**
             * Merges the sorted parts.
             *
             * @param dst the destination where parts are merged
             * @param k the start index of the destination, inclusive
             * @param a1 the first part
             * @param lo1 the start index of the first part, inclusive
             * @param hi1 the end index of the first part, exclusive
             * @param a2 the second part
             * @param lo2 the start index of the second part, inclusive
             * @param hi2 the end index of the second part, exclusive
             */
            static void mergeParts(LongArray &dst, gint k,
                                   LongArray &a1, gint lo1, gint hi1, LongArray &a2, gint lo2, gint hi2);

            // [gbyte]

            /**
             * Sorts the specified range of the array using
             * counting sort or insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(ByteArray &a, gint low, gint high);

            /**
             * Sorts the specified range of the array using insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void insertionSort(ByteArray &a, gint low, gint high);

            /**
             * The number of distinct gbyte values.
             */
            static CORE_FAST gint NUM_BYTE_VALUES = 1 << 8;

            /**
                     * Max index of gbyte counter.
                     */
            static CORE_FAST gint MAX_BYTE_INDEX = Byte::MAX_VALUE + NUM_BYTE_VALUES + 1;

            /**
                     * Sorts the specified range of the array using counting sort.
                     *
                     * @param a the array to be sorted
                     * @param low the index of the first element, inclusive, to be sorted
                     * @param high the index of the last element, exclusive, to be sorted
                     */
            static void countingSort(ByteArray &a, gint low, gint high);

            // [gchar]

            /**
                     * Sorts the specified range of the array using
                     * counting sort or Dual-Pivot Quicksort.
                     *
                     * @param a the array to be sorted
                     * @param low the index of the first element, inclusive, to be sorted
                     * @param high the index of the last element, exclusive, to be sorted
                     */
            static void sort(CharArray &a, gint low, gint high);

            /**
             * Sorts the specified array using the Dual-Pivot Quicksort and/or
             * other sorts in special-cases.
             *
             * @param a the array to be sorted
             * @param bits the combination of recursion depth and bit flag, where
             *        the right bit "0" indicates that array is the leftmost part
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(CharArray &a, gint bits, gint low, gint high);

            /**
             * Sorts the specified range of the array using insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void insertionSort(CharArray &a, gint low, gint high);

            /**
             * The number of distinct gchar values.
             */
            static CORE_FAST gint NUM_CHAR_VALUES = 1 << 16;

            /**
                     * Sorts the specified range of the array using counting sort.
                     *
                     * @param a the array to be sorted
                     * @param low the index of the first element, inclusive, to be sorted
                     * @param high the index of the last element, exclusive, to be sorted
                     */
            static void countingSort(CharArray &a, gint low, gint high);

            // [gshort]

            /**
                     * Sorts the specified range of the array using
                     * counting sort or Dual-Pivot Quicksort.
                     *
                     * @param a the array to be sorted
                     * @param low the index of the first element, inclusive, to be sorted
                     * @param high the index of the last element, exclusive, to be sorted
                     */
            static void sort(ShortArray &a, gint low, gint high);

            /**
             * Sorts the specified array using the Dual-Pivot Quicksort and/or
             * other sorts in special-cases.
             *
             * @param a the array to be sorted
             * @param bits the combination of recursion depth and bit flag, where
             *        the right bit "0" indicates that array is the leftmost part
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(ShortArray &a, gint bits, gint low, gint high);

            /**
             * Sorts the specified range of the array using insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void insertionSort(ShortArray &a, gint low, gint high);

            /**
             * The number of distinct gshort values.
             */
            static CORE_FAST gint NUM_SHORT_VALUES = 1 << 16;

            /**
             * Max index of gshort counter.
             */
            static CORE_FAST gint MAX_SHORT_INDEX = Short::MAX_VALUE + NUM_SHORT_VALUES + 1;

            /**
             * Sorts the specified range of the array using counting sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void countingSort(ShortArray &a, gint low, gint high);

            // [gfloat]

            /**
             * Sorts the specified range of the array using parallel merge
             * sort and/or Dual-Pivot Quicksort.
             *
             * To balance the faster splitting and parallelism of merge sort
             * with the faster element partitioning of Quicksort, ranges are
             * subdivided in tiers such that, if there is enough parallelism,
             * the four-way parallel merge is started, still ensuring enough
             * parallelism to process the partitions.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(FloatArray &a, gint low, gint high);

            /**
             * Sorts the specified array using the Dual-Pivot Quicksort and/or
             * other sorts in special-cases.
             *
             * @param a the array to be sorted
             * @param bits the combination of recursion depth and bit flag, where
             *        the right bit "0" indicates that array is the leftmost part
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(FloatArray &a, gint bits, gint low, gint high);

            /**
             * Sorts the specified range of the array using mixed insertion sort.
             *
             * Mixed insertion sort is combination of simple insertion sort,
             * pin insertion sort and pair insertion sort.
             *
             * In the context of Dual-Pivot Quicksort, the pivot element
             * from the left part plays the role of sentinel, because it
             * is less than any elements from the given part. Therefore,
             * expensive check of the left range can be skipped on each
             * iteration unless it is the leftmost call.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param end the index of the last element for simple insertion sort
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void mixedInsertionSort(FloatArray &a, gint low, gint end, gint high);

            /**
             * Sorts the specified range of the array using insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void insertionSort(FloatArray &a, gint low, gint high);

            /**
             * Sorts the specified range of the array using heap sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void heapSort(FloatArray &a, gint low, gint high);

            /**
             * Pushes specified element down during heap sort.
             *
             * @param a the given array
             * @param p the start index
             * @param value the given element
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void pushDown(FloatArray &a, gint p, gfloat value, gint low, gint high);

            /**
             * Tries to sort the specified range of the array.
             *
             * @param a the array to be sorted
             * @param low the index of the first element to be sorted
             * @param size the array size
             * @return true if finally sorted, false otherwise
             */
            static gbool tryMergeRuns(FloatArray &a, gint low, gint size);

            /**
             * Merges the specified runs.
             *
             * @param a the source array
             * @param b the temporary buffer used in merging
             * @param offset the start index in the source, inclusive
             * @param aim specifies merging: to source ( > 0), buffer ( < 0) or any ( == 0)
             * @param run the start indexes of the runs, inclusive
             * @param lo the start index of the first run, inclusive
             * @param hi the start index of the last run, inclusive
             * @return the destination where runs are merged
             */
            static FloatArray &mergeRuns(FloatArray &a, FloatArray &b, gint offset,
                                         gint aim, IntArray &run, gint lo, gint hi);

            /**
             * Merges the sorted parts.
             *
             * @param dst the destination where parts are merged
             * @param k the start index of the destination, inclusive
             * @param a1 the first part
             * @param lo1 the start index of the first part, inclusive
             * @param hi1 the end index of the first part, exclusive
             * @param a2 the second part
             * @param lo2 the start index of the second part, inclusive
             * @param hi2 the end index of the second part, exclusive
             */
            static void mergeParts(FloatArray &dst, gint k,
                                   FloatArray &a1, gint lo1, gint hi1, FloatArray &a2, gint lo2, gint hi2);

            // [gdouble]

            /**
             * Sorts the specified range of the array using parallel merge
             * sort and/or Dual-Pivot Quicksort.
             *
             * To balance the faster splitting and parallelism of merge sort
             * with the faster element partitioning of Quicksort, ranges are
             * subdivided in tiers such that, if there is enough parallelism,
             * the four-way parallel merge is started, still ensuring enough
             * parallelism to process the partitions.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(DoubleArray &a, gint low, gint high);

            static void sort(StringArray &a, gint low, gint high);

            /**
             * Sorts the specified array using the Dual-Pivot Quicksort and/or
             * other sorts in special-cases.
             *
             * @param a the array to be sorted
             * @param bits the combination of recursion depth and bit flag, where
             *        the right bit "0" indicates that array is the leftmost part
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void sort(DoubleArray &a, gint bits, gint low, gint high);

            /**
             * Sorts the specified range of the array using mixed insertion sort.
             *
             * Mixed insertion sort is combination of simple insertion sort,
             * pin insertion sort and pair insertion sort.
             *
             * In the context of Dual-Pivot Quicksort, the pivot element
             * from the left part plays the role of sentinel, because it
             * is less than any elements from the given part. Therefore,
             * expensive check of the left range can be skipped on each
             * iteration unless it is the leftmost call.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param end the index of the last element for simple insertion sort
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void mixedInsertionSort(DoubleArray &a, gint low, gint end, gint high);

            /**
             * Sorts the specified range of the array using insertion sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void insertionSort(DoubleArray &a, gint low, gint high);

            /**
             * Sorts the specified range of the array using heap sort.
             *
             * @param a the array to be sorted
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void heapSort(DoubleArray &a, gint low, gint high);

            /**
             * Pushes specified element down during heap sort.
             *
             * @param a the given array
             * @param p the start index
             * @param value the given element
             * @param low the index of the first element, inclusive, to be sorted
             * @param high the index of the last element, exclusive, to be sorted
             */
            static void pushDown(DoubleArray &a, gint p, gdouble value, gint low, gint high);

            /**
             * Tries to sort the specified range of the array.
             *
             * @param a the array to be sorted
             * @param low the index of the first element to be sorted
             * @param size the array size
             * @return true if finally sorted, false otherwise
             */
            static gbool tryMergeRuns(DoubleArray &a, gint low, gint size);

            /**
             * Merges the specified runs.
             *
             * @param a the source array
             * @param b the temporary buffer used in merging
             * @param offset the start index in the source, inclusive
             * @param aim specifies merging: to source ( > 0), buffer ( < 0) or any ( == 0)
             * @param run the start indexes of the runs, inclusive
             * @param lo the start index of the first run, inclusive
             * @param hi the start index of the last run, inclusive
             * @return the destination where runs are merged
             */
            static DoubleArray &mergeRuns(DoubleArray &a, DoubleArray &b, gint offset,
                                          gint aim, IntArray &run, gint lo, gint hi);

            /**
             * Merges the sorted parts.
             *
             * @param dst the destination where parts are merged
             * @param k the start index of the destination, inclusive
             * @param a1 the first part
             * @param lo1 the start index of the first part, inclusive
             * @param hi1 the end index of the first part, exclusive
             * @param a2 the second part
             * @param lo2 the start index of the second part, inclusive
             * @param hi2 the end index of the second part, exclusive
             */
            static void mergeParts(DoubleArray &dst, gint k,
                                   DoubleArray &a1, gint lo1, gint hi1, DoubleArray &a2, gint lo2, gint hi2);
        };
    } // util
} // core

#endif //CORE24_DUALPRIVOTQUICKSORT_H
