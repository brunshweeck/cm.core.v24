//
// Created by brunshweeck on 23 juin 2024.
//

#include <core/lang/DoubleArray.h>
#include <core/lang/FloatArray.h>
#include <core/lang/LongArray.h>
#include <core/lang/ShortArray.h>
#include "DualPivotQuickSort.h"

namespace core {
    namespace util {
        void DualPivotQuickSort::sort(IntArray& a, gint low, gint high) {
            sort(a, 0, low, high);
        }

        void DualPivotQuickSort::sort(IntArray& a, gint bits, gint low, gint high) {
            while (true) {
                gint end = high - 1;
                gint size = high - low;

                /*
                 * Run mixed insertion sort on small non-leftmost parts.
                 */
                if (size < MAX_MIXED_INSERTION_SORT_SIZE + bits && (bits & 1) > 0) {
                    // mixedInsertionSort(a, low, high - 3 * ((size >> 5) << 3), high);
                    return;
                }

                /*
                         * Invoke insertion sort on small leftmost part.
                         */
                if (size < MAX_INSERTION_SORT_SIZE) {
                    insertionSort(a, low, high);
                    return;
                }

                /*
                         * Check if the whole array or large non-leftmost
                         * parts are nearly sorted and then merge runs.
                         */
                if ((bits == 0 || size > MIN_TRY_MERGE_SIZE && (bits & 1) > 0) /*&& tryMergeRuns(a, low, size)*/) {
                    return;
                }

                /*
                         * Switch to heap sort if execution
                         * time is becoming quadratic.
                         */
                if ((bits += DELTA) > MAX_RECURSION_DEPTH) {
                    heapSort(a, low, high);
                    return;
                }

                /*
                         * Use an inexpensive approximation of the golden ratio
                         * to select five sample elements and determine pivots.
                         */
                gint step = (size >> 3) * 3 + 3;

                /*
                         * Five elements around (and including) the central element
                         * will be used for pivot selection as described below. The
                         * unequal choice of spacing these elements was empirically
                         * determined to work well on a wide variety of inputs.
                         */
                gint e1 = low + step;
                gint e5 = end - step;
                gint e3 = (e1 + e5) >> 1;
                gint e2 = (e1 + e3) >> 1;
                gint e4 = (e3 + e5) >> 1;
                gint a3 = a[e3];

                /*
                         * Sort these elements in place by the combination
                         * of 4-element sorting network and insertion sort.
                         *
                         *    5 ------o-----------o------------
                         *            |           |
                         *    4 ------|-----o-----o-----o------
                         *            |     |           |
                         *    2 ------o-----|-----o-----o------
                         *                  |     |
                         *    1 ------------o-----o------------
                         */
                if (a[e5] < a[e2]) {
                    gint t = a[e5];
                    a[e5] = a[e2];
                    a[e2] = t;
                }
                if (a[e4] < a[e1]) {
                    gint t = a[e4];
                    a[e4] = a[e1];
                    a[e1] = t;
                }
                if (a[e5] < a[e4]) {
                    gint t = a[e5];
                    a[e5] = a[e4];
                    a[e4] = t;
                }
                if (a[e2] < a[e1]) {
                    gint t = a[e2];
                    a[e2] = a[e1];
                    a[e1] = t;
                }
                if (a[e4] < a[e2]) {
                    gint t = a[e4];
                    a[e4] = a[e2];
                    a[e2] = t;
                }

                if (a3 < a[e2]) {
                    if (a3 < a[e1]) {
                        a[e3] = a[e2];
                        a[e2] = a[e1];
                        a[e1] = a3;
                    } else {
                        a[e3] = a[e2];
                        a[e2] = a3;
                    }
                } else if (a3 > a[e4]) {
                    if (a3 > a[e5]) {
                        a[e3] = a[e4];
                        a[e4] = a[e5];
                        a[e5] = a3;
                    } else {
                        a[e3] = a[e4];
                        a[e4] = a3;
                    }
                }

                // Pointers
                gint lower = low; // The index of the last element of the left part
                gint upper = end; // The index of the first element of the right part

                /*
                         * Partitioning with 2 pivots in case of different elements.
                         */
                if (a[e1] < a[e2] && a[e2] < a[e3] && a[e3] < a[e4] && a[e4] < a[e5]) {
                    /*
                             * Use the first and fifth of the five sorted elements as
                             * the pivots. These values are inexpensive approximation
                             * of tertiles. Note, that pivot1 < pivot2.
                             */
                    gint pivot1 = a[e1];
                    gint pivot2 = a[e5];

                    /*
                             * The first and the last elements to be sorted are moved
                             * to the locations formerly occupied by the pivots. When
                             * partitioning is completed, the pivots are swapped back
                             * into their final positions, and excluded from the next
                             * subsequent sorting.
                             */
                    a[e1] = a[lower];
                    a[e5] = a[upper];

                    /*
                             * Skip elements, which are less or greater than the pivots.
                             */
                    while (a[++lower] < pivot1) {
                        ;
                    }
                    while (a[--upper] > pivot2) {
                        ;
                    }

                    /*
                             * Backward 3-interval partitioning
                             *
                             *   left part                 central part          right part
                             * +------------------------------------------------------------+
                             * |  < pivot1  |   ?   |  pivot1 <= && <= pivot2  |  > pivot2  |
                             * +------------------------------------------------------------+
                             *             ^       ^                            ^
                             *             |       |                            |
                             *           lower     k                          upper
                             *
                             * Invariants:
                             *
                             *              all in (low, lower] < pivot1
                             *    pivot1 <= all in (k, upper)  <= pivot2
                             *              all in [upper, end) > pivot2
                             *
                             * Pointer k is the last index of ?-part
                             */
                    for (gint unused = --lower, k = ++upper; --k > lower;) {
                        gint ak = a[k];

                        if (ak < pivot1) {
                            // Move a[k] to the left side
                            while (lower < k) {
                                if (a[++lower] >= pivot1) {
                                    if (a[lower] > pivot2) {
                                        a[k] = a[--upper];
                                        a[upper] = a[lower];
                                    } else {
                                        a[k] = a[lower];
                                    }
                                    a[lower] = ak;
                                    break;
                                }
                            }
                        } else if (ak > pivot2) {
                            // Move a[k] to the right side
                            a[k] = a[--upper];
                            a[upper] = ak;
                        }
                    }

                    /*
                             * Swap the pivots into their final positions.
                             */
                    a[low] = a[lower];
                    a[lower] = pivot1;
                    a[end] = a[upper];
                    a[upper] = pivot2;

                    /*
                             * Sort non-left parts recursively (possibly in parallel),
                             * excluding known pivots.
                             */
                    sort(a, bits | 1, lower + 1, upper);
                    sort(a, bits | 1, upper + 1, high);
                } else {
                    // Use single pivot in case of many equal elements

                    /*
                             * Use the third of the five sorted elements as the pivot.
                             * This value is inexpensive approximation of the median.
                             */
                    gint pivot = a[e3];

                    /*
                             * The first element to be sorted is moved to the
                             * location formerly occupied by the pivot. After
                             * completion of partitioning the pivot is swapped
                             * back into its final position, and excluded from
                             * the next subsequent sorting.
                             */
                    a[e3] = a[lower];

                    /*
                             * Traditional 3-way (Dutch National Flag) partitioning
                             *
                             *   left part                 central part    right part
                             * +------------------------------------------------------+
                             * |   < pivot   |     ?     |   == pivot   |   > pivot   |
                             * +------------------------------------------------------+
                             *              ^           ^                ^
                             *              |           |                |
                             *            lower         k              upper
                             *
                             * Invariants:
                             *
                             *   all in (low, lower] < pivot
                             *   all in (k, upper)  == pivot
                             *   all in [upper, end] > pivot
                             *
                             * Pointer k is the last index of ?-part
                             */
                    for (gint k = ++upper; --k > lower;) {
                        gint ak = a[k];

                        if (ak != pivot) {
                            a[k] = pivot;

                            if (ak < pivot) {
                                // Move a[k] to the left side
                                while (a[++lower] < pivot);

                                if (a[lower] > pivot) {
                                    a[--upper] = a[lower];
                                }
                                a[lower] = ak;
                            } else {
                                // ak > pivot - Move a[k] to the right side
                                a[--upper] = ak;
                            }
                        }
                    }

                    /*
                             * Swap the pivot into its final position.
                             */
                    a[low] = a[lower];
                    a[lower] = pivot;

                    /*
                             * Sort the right part (possibly in parallel), excluding
                             * known pivot. All elements from the central part are
                             * equal and therefore already sorted.
                             */
                    sort(a, bits | 1, upper, high);
                }
                high = lower; // Iterate along the left part
            }
        }

        void DualPivotQuickSort::insertionSort(IntArray& a, gint low, gint high) {
            for (gint i, k = low; ++k < high;) {
                gint ai = a[i = k];

                if (ai < a[i - 1]) {
                    while (--i >= low && ai < a[i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            }
        }

        void DualPivotQuickSort::mixedInsertionSort(IntArray& a, gint low, gint end, gint high) {
            if (end == high) {
                /*
                         * Invoke simple insertion sort on tiny array.
                         */
                for (gint i; ++low < end;) {
                    gint ai = a[i = low];

                    while (ai < a[--i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            } else {
                /*
                         * Start with pin insertion sort on small part.
                         *
                         * Pin insertion sort is extended simple insertion sort.
                         * The main idea of this sort is to put elements larger
                         * than an element called pin to the end of array (the
                         * proper area for such elements). It avoids expensive
                         * movements of these elements through the whole array.
                         */
                gint pin = a[end];

                for (gint i, p = high; ++low < end;) {
                    gint ai = a[i = low];

                    if (ai < a[i - 1]) {
                        // Small element

                        /*
                                 * Insert small element into sorted part.
                                 */
                        a[i] = a[--i];

                        while (ai < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = ai;
                    } else if (p > i && ai > pin) {
                        // Large element

                        /*
                                 * Find element smaller than pin.
                                 */
                        while (a[--p] > pin) {
                            ;
                        }

                        /*
                                 * Swap it with large element.
                                 */
                        if (p > i) {
                            ai = a[p];
                            a[p] = a[i];
                        }

                        /*
                                 * Insert small element into sorted part.
                                 */
                        while (ai < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = ai;
                    }
                }

                /*
                         * Continue with pair insertion sort on remain part.
                         */
                for (gint i; low < high; ++low) {
                    gint a1 = a[i = low], a2 = a[++low];

                    /*
                             * Insert two elements per iteration: at first, insert the
                             * larger element and then insert the smaller element, but
                             * from the position where the larger element was inserted.
                             */
                    if (a1 > a2) {
                        while (a1 < a[--i]) {
                            a[i + 2] = a[i];
                        }
                        a[++i + 1] = a1;

                        while (a2 < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = a2;
                    } else if (a1 < a[i - 1]) {
                        while (a2 < a[--i]) {
                            a[i + 2] = a[i];
                        }
                        a[++i + 1] = a2;

                        while (a1 < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = a1;
                    }
                }
            }
        }

        void DualPivotQuickSort::heapSort(IntArray& a, gint low, gint high) {
            for (gint k = (low + high) >> 1; k > low;) {
                pushDown(a, --k, a[k], low, high);
            }
            while (--high > low) {
                gint max = a[low];
                pushDown(a, low, a[high], low, high);
                a[high] = max;
            }
        }

        void DualPivotQuickSort::pushDown(IntArray& a, gint p, gint value, gint low, gint high) {
            for (gint k;; a[p] = a[p = k]) {
                k = (p << 1) - low + 2; // Index of the right child

                if (k > high) {
                    break;
                }
                if (k == high || a[k] < a[k - 1]) {
                    --k;
                }
                if (a[k] <= value) {
                    break;
                }
            }
            a[p] = value;
        }

        gbool DualPivotQuickSort::tryMergeRuns(IntArray& a, gint low, gint size) {
            /*
             * The run array is constructed only if initial runs are
             * glong enough to continue, run[i] then holds start index
             * of the i-th sequence of elements in non-descending order.
             */
            IntArray run = IntArray(0);
            gint high = low + size;
            gint count = 1, last = low;

            /*
                     * Identify all possible runs.
                     */
            for (gint k = low + 1; k < high;) {
                /*
                         * Find the end index of the current run.
                         */
                if (a[k - 1] < a[k]) {
                    // Identify ascending sequence
                    while (++k < high && a[k - 1] <= a[k]);
                } else if (a[k - 1] > a[k]) {
                    // Identify descending sequence
                    while (++k < high && a[k - 1] >= a[k]) {
                        ;
                    }

                    // Reverse into ascending order
                    for (gint i = last - 1, j = k; ++i < --j && a[i] > a[j];) {
                        gint ai = a[i];
                        a[i] = a[j];
                        a[j] = ai;
                    }
                } else {
                    // Identify constant sequence
                    for (gint ak = a[k]; ++k < high && ak == a[k];) {
                        ;
                    }

                    if (k < high) {
                        continue;
                    }
                }

                /*
                         * Check special cases.
                         */
                if (run == null) {
                    if (k == high) {
                        /*
                                 * The array is monotonous sequence,
                                 * and therefore already sorted.
                                 */
                        return true;
                    }

                    if (k - low < MIN_FIRST_RUN_SIZE) {
                        /*
                                 * The first run is too small
                                 * to proceed with scanning.
                                 */
                        return false;
                    }

                    run = IntArray(((size >> 10) | 0x7F) & 0x3FF);
                    run[0] = low;
                } else if (a[last - 1] > a[last]) {
                    if (count > (k - low) >> MIN_FIRST_RUNS_FACTOR) {
                        /*
                                 * The first runs are not glong
                                 * enough to continue scanning.
                                 */
                        return false;
                    }

                    if (++count == MAX_RUN_CAPACITY) {
                        /*
                                 * Array is not highly structured.
                                 */
                        return false;
                    }

                    if (count == run.length()) {
                        /*
                                 * Increase capacity of index array.
                                 */
                        run = Arrays::copyOf(run, count << 1);
                    }
                }
                run[count] = (last = k);
            }

            /*
                     * Merge runs of highly structured array.
                     */
            if (count > 1) {
                gint offset = low;
                IntArray b = IntArray(size);
                mergeRuns(a, b, offset, 1, run, 0, count);
            }
            return true;
        }

        IntArray DualPivotQuickSort::mergeRuns(IntArray& a, IntArray& b, gint offset,
                                               gint aim, IntArray& run, gint lo, gint hi) {
            if (hi - lo == 1) {
                if (aim >= 0) {
                    return a;
                }
                for (gint i = run[hi], j = i - offset, low = run[lo]; i > low; b[--j] = a[--i]) {
                    ;
                }
                return b;
            }

            /*
             * Split into approximately equal parts.
             */
            gint mi = lo, rmi = (run[lo] + run[hi]) >> 1;
            while (run[++mi + 1] <= rmi) {
                ;
            }

            /*
             * Merge the left and right parts.
             */
            IntArray a1(0), a2(0);

            a1 = mergeRuns(a, b, offset, -aim, run, lo, mi);
            a2 = mergeRuns(a, b, offset, 0, run, mi, hi);

            IntArray dst = a1 == a ? b : a;

            gint k = a1 == a ? run[lo] - offset : run[lo];
            gint lo1 = a1 == b ? run[lo] - offset : run[lo];
            gint hi1 = a1 == b ? run[mi] - offset : run[mi];
            gint lo2 = a2 == b ? run[mi] - offset : run[mi];
            gint hi2 = a2 == b ? run[hi] - offset : run[hi];

            mergeParts(dst, k, a1, lo1, hi1, a2, lo2, hi2);
            return dst;
        }

        void DualPivotQuickSort::mergeParts(IntArray& dst, gint k, IntArray& a1,
                                            gint lo1, gint hi1, IntArray& a2, gint lo2, gint hi2) {
            /*
             * Merge small parts sequentially.
             */
            while (lo1 < hi1 && lo2 < hi2) {
                dst[k++] = a1[lo1] < a2[lo2] ? a1[lo1++] : a2[lo2++];
            }
            if (dst != a1 || k < lo1) {
                while (lo1 < hi1) {
                    dst[k++] = a1[lo1++];
                }
            }
            if (dst != a2 || k < lo2) {
                while (lo2 < hi2) {
                    dst[k++] = a2[lo2++];
                }
            }
        }

        void DualPivotQuickSort::sort(LongArray& a, gint low, gint high) {
            sort(a, 0, low, high);
        }

        void DualPivotQuickSort::sort(LongArray& a, gint bits, gint low, gint high) {
            while (true) {
                gint end = high - 1, size = high - low;

                /*
             * Run mixed insertion sort on small non-leftmost parts.
             */
                if (size < MAX_MIXED_INSERTION_SORT_SIZE + bits && (bits & 1) > 0) {
                    mixedInsertionSort(a, low, high - 3 * ((size >> 5) << 3), high);
                    return;
                }

                /*
             * Invoke insertion sort on small leftmost part.
             */
                if (size < MAX_INSERTION_SORT_SIZE) {
                    insertionSort(a, low, high);
                    return;
                }

                /*
             * Check if the whole array or large non-leftmost
             * parts are nearly sorted and then merge runs.
             */
                if ((bits == 0 || size > MIN_TRY_MERGE_SIZE && (bits & 1) > 0) && tryMergeRuns(a, low, size)) {
                    return;
                }

                /*
             * Switch to heap sort if execution
             * time is becoming quadratic.
             */
                if ((bits += DELTA) > MAX_RECURSION_DEPTH) {
                    heapSort(a, low, high);
                    return;
                }

                /*
             * Use an inexpensive approximation of the golden ratio
             * to select five sample elements and determine pivots.
             */
                gint step = (size >> 3) * 3 + 3;

                /*
             * Five elements around (and including) the central element
             * will be used for pivot selection as described below. The
             * unequal choice of spacing these elements was empirically
             * determined to work well on a wide variety of inputs.
             */
                gint e1 = low + step;
                gint e5 = end - step;
                gint e3 = (e1 + e5) >> 1;
                gint e2 = (e1 + e3) >> 1;
                gint e4 = (e3 + e5) >> 1;
                glong a3 = a[e3];

                /*
             * Sort these elements in place by the combination
             * of 4-element sorting network and insertion sort.
             *
             *    5 ------o-----------o------------
             *            |           |
             *    4 ------|-----o-----o-----o------
             *            |     |           |
             *    2 ------o-----|-----o-----o------
             *                  |     |
             *    1 ------------o-----o------------
             */
                if (a[e5] < a[e2]) {
                    glong t = a[e5];
                    a[e5] = a[e2];
                    a[e2] = t;
                }
                if (a[e4] < a[e1]) {
                    glong t = a[e4];
                    a[e4] = a[e1];
                    a[e1] = t;
                }
                if (a[e5] < a[e4]) {
                    glong t = a[e5];
                    a[e5] = a[e4];
                    a[e4] = t;
                }
                if (a[e2] < a[e1]) {
                    glong t = a[e2];
                    a[e2] = a[e1];
                    a[e1] = t;
                }
                if (a[e4] < a[e2]) {
                    glong t = a[e4];
                    a[e4] = a[e2];
                    a[e2] = t;
                }

                if (a3 < a[e2]) {
                    if (a3 < a[e1]) {
                        a[e3] = a[e2];
                        a[e2] = a[e1];
                        a[e1] = a3;
                    } else {
                        a[e3] = a[e2];
                        a[e2] = a3;
                    }
                } else if (a3 > a[e4]) {
                    if (a3 > a[e5]) {
                        a[e3] = a[e4];
                        a[e4] = a[e5];
                        a[e5] = a3;
                    } else {
                        a[e3] = a[e4];
                        a[e4] = a3;
                    }
                }

                // Pointers
                gint lower = low; // The index of the last element of the left part
                gint upper = end; // The index of the first element of the right part

                /*
             * Partitioning with 2 pivots in case of different elements.
             */
                if (a[e1] < a[e2] && a[e2] < a[e3] && a[e3] < a[e4] && a[e4] < a[e5]) {
                    /*
                 * Use the first and fifth of the five sorted elements as
                 * the pivots. These values are inexpensive approximation
                 * of tertiles. Note, that pivot1 < pivot2.
                 */
                    glong pivot1 = a[e1];
                    glong pivot2 = a[e5];

                    /*
                 * The first and the last elements to be sorted are moved
                 * to the locations formerly occupied by the pivots. When
                 * partitioning is completed, the pivots are swapped back
                 * into their final positions, and excluded from the next
                 * subsequent sorting.
                 */
                    a[e1] = a[lower];
                    a[e5] = a[upper];

                    /*
                 * Skip elements, which are less or greater than the pivots.
                 */
                    while (a[++lower] < pivot1) {
                        ;
                    }
                    while (a[--upper] > pivot2) {
                        ;
                    }

                    /*
                 * Backward 3-interval partitioning
                 *
                 *   left part                 central part          right part
                 * +------------------------------------------------------------+
                 * |  < pivot1  |   ?   |  pivot1 <= && <= pivot2  |  > pivot2  |
                 * +------------------------------------------------------------+
                 *             ^       ^                            ^
                 *             |       |                            |
                 *           lower     k                          upper
                 *
                 * Invariants:
                 *
                 *              all in (low, lower] < pivot1
                 *    pivot1 <= all in (k, upper)  <= pivot2
                 *              all in [upper, end) > pivot2
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint unused = --lower, k = ++upper; --k > lower;) {
                        glong ak = a[k];

                        if (ak < pivot1) {
                            // Move a[k] to the left side
                            while (lower < k) {
                                if (a[++lower] >= pivot1) {
                                    if (a[lower] > pivot2) {
                                        a[k] = a[--upper];
                                        a[upper] = a[lower];
                                    } else {
                                        a[k] = a[lower];
                                    }
                                    a[lower] = ak;
                                    break;
                                }
                            }
                        } else if (ak > pivot2) {
                            // Move a[k] to the right side
                            a[k] = a[--upper];
                            a[upper] = ak;
                        }
                    }

                    /*
                 * Swap the pivots into their final positions.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot1;
                    a[end] = a[upper];
                    a[upper] = pivot2;

                    /*
                 * Sort non-left parts recursively (possibly in parallel),
                 * excluding known pivots.
                 */
                    {
                        sort(a, bits | 1, lower + 1, upper);
                        sort(a, bits | 1, upper + 1, high);
                    }
                } else {
                    // Use single pivot in case of many equal elements

                    /*
                 * Use the third of the five sorted elements as the pivot.
                 * This value is inexpensive approximation of the median.
                 */
                    glong pivot = a[e3];

                    /*
                 * The first element to be sorted is moved to the
                 * location formerly occupied by the pivot. After
                 * completion of partitioning the pivot is swapped
                 * back into its final position, and excluded from
                 * the next subsequent sorting.
                 */
                    a[e3] = a[lower];

                    /*
                 * Traditional 3-way (Dutch National Flag) partitioning
                 *
                 *   left part                 central part    right part
                 * +------------------------------------------------------+
                 * |   < pivot   |     ?     |   == pivot   |   > pivot   |
                 * +------------------------------------------------------+
                 *              ^           ^                ^
                 *              |           |                |
                 *            lower         k              upper
                 *
                 * Invariants:
                 *
                 *   all in (low, lower] < pivot
                 *   all in (k, upper)  == pivot
                 *   all in [upper, end] > pivot
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint k = ++upper; --k > lower;) {
                        glong ak = a[k];

                        if (ak != pivot) {
                            a[k] = pivot;

                            if (ak < pivot) {
                                // Move a[k] to the left side
                                while (a[++lower] < pivot) {
                                    ;
                                }

                                if (a[lower] > pivot) {
                                    a[--upper] = a[lower];
                                }
                                a[lower] = ak;
                            } else {
                                // ak > pivot - Move a[k] to the right side
                                a[--upper] = ak;
                            }
                        }
                    }

                    /*
                 * Swap the pivot into its final position.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot;

                    /*
                 * Sort the right part (possibly in parallel), excluding
                 * known pivot. All elements from the central part are
                 * equal and therefore already sorted.
                 */
                    {
                        sort(a, bits | 1, upper, high);
                    }
                }
                high = lower; // Iterate along the left part
            }
        }

        void DualPivotQuickSort::mixedInsertionSort(LongArray& a, gint low, gint end, gint high) {
            if (end == high) {
                /*
             * Invoke simple insertion sort on tiny array.
             */
                for (gint i; ++low < end;) {
                    glong ai = a[i = low];

                    while (ai < a[--i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            } else {
                /*
             * Start with pin insertion sort on small part.
             *
             * Pin insertion sort is extended simple insertion sort.
             * The main idea of this sort is to put elements larger
             * than an element called pin to the end of array (the
             * proper area for such elements). It avoids expensive
             * movements of these elements through the whole array.
             */
                glong pin = a[end];

                for (gint i, p = high; ++low < end;) {
                    glong ai = a[i = low];

                    if (ai < a[i - 1]) {
                        // Small element

                        /*
                     * Insert small element into sorted part.
                     */
                        a[i] = a[--i];

                        while (ai < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = ai;
                    } else if (p > i && ai > pin) {
                        // Large element

                        /*
                     * Find element smaller than pin.
                     */
                        while (a[--p] > pin) {
                            ;
                        }

                        /*
                     * Swap it with large element.
                     */
                        if (p > i) {
                            ai = a[p];
                            a[p] = a[i];
                        }

                        /*
                     * Insert small element into sorted part.
                     */
                        while (ai < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = ai;
                    }
                }

                /*
             * Continue with pair insertion sort on remain part.
             */
                for (gint i; low < high; ++low) {
                    glong a1 = a[i = low], a2 = a[++low];

                    /*
                 * Insert two elements per iteration: at first, insert the
                 * larger element and then insert the smaller element, but
                 * from the position where the larger element was inserted.
                 */
                    if (a1 > a2) {
                        while (a1 < a[--i]) {
                            a[i + 2] = a[i];
                        }
                        a[++i + 1] = a1;

                        while (a2 < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = a2;
                    } else if (a1 < a[i - 1]) {
                        while (a2 < a[--i]) {
                            a[i + 2] = a[i];
                        }
                        a[++i + 1] = a2;

                        while (a1 < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = a1;
                    }
                }
            }
        }

        void DualPivotQuickSort::insertionSort(LongArray& a, gint low, gint high) {
            for (gint i, k = low; ++k < high;) {
                glong ai = a[i = k];

                if (ai < a[i - 1]) {
                    while (--i >= low && ai < a[i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            }
        }

        void DualPivotQuickSort::heapSort(LongArray& a, gint low, gint high) {
            for (gint k = (low + high) >> 1; k > low;) {
                pushDown(a, --k, a[k], low, high);
            }
            while (--high > low) {
                glong max = a[low];
                pushDown(a, low, a[high], low, high);
                a[high] = max;
            }
        }

        void DualPivotQuickSort::pushDown(LongArray& a, gint p, glong value, gint low, gint high) {
            for (gint k;; a[p] = a[p = k]) {
                k = (p << 1) - low + 2; // Index of the right child

                if (k > high) {
                    break;
                }
                if (k == high || a[k] < a[k - 1]) {
                    --k;
                }
                if (a[k] <= value) {
                    break;
                }
            }
            a[p] = value;
        }

        gbool DualPivotQuickSort::tryMergeRuns(LongArray& a, gint low, gint size) {
            /*
         * The run array is constructed only if initial runs are
         * glong enough to continue, run[i] then holds start index
         * of the i-th sequence of elements in non-descending order.
         */
            IntArray run = IntArray(0);
            gint high = low + size;
            gint count = 1, last = low;

            /*
         * Identify all possible runs.
         */
            for (gint k = low + 1; k < high;) {
                /*
             * Find the end index of the current run.
             */
                if (a[k - 1] < a[k]) {
                    // Identify ascending sequence
                    while (++k < high && a[k - 1] <= a[k]);
                } else if (a[k - 1] > a[k]) {
                    // Identify descending sequence
                    while (++k < high && a[k - 1] >= a[k]) {
                        ;
                    }

                    // Reverse into ascending order
                    for (gint i = last - 1, j = k; ++i < --j && a[i] > a[j];) {
                        glong ai = a[i];
                        a[i] = a[j];
                        a[j] = ai;
                    }
                } else {
                    // Identify constant sequence
                    for (glong ak = a[k]; ++k < high && ak == a[k];) {
                        ;
                    }

                    if (k < high) {
                        continue;
                    }
                }

                /*
             * Check special cases.
             */
                if (run == null) {
                    if (k == high) {
                        /*
                     * The array is monotonous sequence,
                     * and therefore already sorted.
                     */
                        return true;
                    }

                    if (k - low < MIN_FIRST_RUN_SIZE) {
                        /*
                     * The first run is too small
                     * to proceed with scanning.
                     */
                        return false;
                    }

                    run = IntArray(((size >> 10) | 0x7F) & 0x3FF);
                    run[0] = low;
                } else if (a[last - 1] > a[last]) {
                    if (count > (k - low) >> MIN_FIRST_RUNS_FACTOR) {
                        /*
                     * The first runs are not glong
                     * enough to continue scanning.
                     */
                        return false;
                    }

                    if (++count == MAX_RUN_CAPACITY) {
                        /*
                     * Array is not highly structured.
                     */
                        return false;
                    }

                    if (count == run.length()) {
                        /*
                     * Increase capacity of index array.
                     */
                        run = Arrays::copyOf(run, count << 1);
                    }
                }
                run[count] = (last = k);
            }

            /*
         * Merge runs of highly structured array.
         */
            if (count > 1) {
                gint offset = low;
                LongArray b = LongArray(size);
                mergeRuns(a, b, offset, 1, run, 0, count);
            }
            return true;
        }

        LongArray& DualPivotQuickSort::mergeRuns(LongArray& a, LongArray& b, gint offset, gint aim, IntArray& run,
                                                 gint lo, gint hi) {
            if (hi - lo == 1) {
                if (aim >= 0) {
                    return a;
                }
                for (gint i = run[hi], j = i - offset, low = run[lo]; i > low; b[--j] = a[--i]) { ; }
                return b;
            }

            /*
         * Split into approximately equal parts.
         */
            gint mi = lo, rmi = (run[lo] + run[hi]) >> 1;
            while (run[++mi + 1] <= rmi) { ; }

            /*
         * Merge the left and right parts.
         */
            LongArray a1 = LongArray(0);
            LongArray a2 = LongArray(0);

            {
                a1 = mergeRuns(a, b, offset, -aim, run, lo, mi);
                a2 = mergeRuns(a, b, offset, 0, run, mi, hi);
            }

            LongArray& dst = a1 == a ? b : a;

            gint k = a1 == a ? run[lo] - offset : run[lo];
            gint lo1 = a1 == b ? run[lo] - offset : run[lo];
            gint hi1 = a1 == b ? run[mi] - offset : run[mi];
            gint lo2 = a2 == b ? run[mi] - offset : run[mi];
            gint hi2 = a2 == b ? run[hi] - offset : run[hi];

            {
                mergeParts(dst, k, a1, lo1, hi1, a2, lo2, hi2);
            }
            return dst;
        }

        void DualPivotQuickSort::mergeParts(LongArray& dst, gint k, LongArray& a1, gint lo1, gint hi1, LongArray& a2,
                                            gint lo2, gint hi2) {
            /*
         * Merge small parts sequentially.
         */
            while (lo1 < hi1 && lo2 < hi2) {
                dst[k++] = a1[lo1] < a2[lo2] ? a1[lo1++] : a2[lo2++];
            }
            if (dst != a1 || k < lo1) {
                while (lo1 < hi1) {
                    dst[k++] = a1[lo1++];
                }
            }
            if (dst != a2 || k < lo2) {
                while (lo2 < hi2) {
                    dst[k++] = a2[lo2++];
                }
            }
        }

        void DualPivotQuickSort::sort(ByteArray& a, gint low, gint high) {
            if (high - low > MIN_BYTE_COUNTING_SORT_SIZE) {
                countingSort(a, low, high);
            } else {
                insertionSort(a, low, high);
            }
        }

        void DualPivotQuickSort::insertionSort(ByteArray& a, gint low, gint high) {
            for (gint i, k = low; ++k < high;) {
                gbyte ai = a[i = k];

                if (ai < a[i - 1]) {
                    while (--i >= low && ai < a[i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            }
        }

        void DualPivotQuickSort::countingSort(ByteArray& a, gint low, gint high) {
            IntArray count = IntArray(NUM_BYTE_VALUES);

            /*
         * Compute a histogram with the number of each values.
         */
            for (gint i = high; i > low; ++count[a[--i] & 0xFF]) {
                ;
            }

            /*
         * Place values on their final positions.
         */
            if (high - low > NUM_BYTE_VALUES) {
                for (gint i = MAX_BYTE_INDEX; --i > Byte::MAX_VALUE;) {
                    gint value = i & 0xFF;

                    for (low = high - count[value]; high > low;
                         a[--high] = (gbyte) value
                    );
                }
            } else {
                for (gint i = MAX_BYTE_INDEX; high > low;) {
                    while (count[--i & 0xFF] == 0) { ; }

                    gint value = i & 0xFF;
                    gint c = count[value];

                    do {
                        a[--high] = (gbyte) value;
                    } while (--c > 0);
                }
            }
        }

        void DualPivotQuickSort::sort(CharArray& a, gint low, gint high) {
            if (high - low > MIN_CHAR_COUNTING_SORT_SIZE) {
                countingSort(a, low, high);
            } else {
                sort(a, 0, low, high);
            }
        }

        void DualPivotQuickSort::sort(CharArray& a, gint bits, gint low, gint high) {
            while (true) {
                gint end = high - 1, size = high - low;

                /*
             * Invoke insertion sort on small leftmost part.
             */
                if (size < MAX_INSERTION_SORT_SIZE) {
                    insertionSort(a, low, high);
                    return;
                }

                /*
             * Switch to counting sort if execution
             * time is becoming quadratic.
             */
                if ((bits += DELTA) > MAX_RECURSION_DEPTH) {
                    countingSort(a, low, high);
                    return;
                }

                /*
             * Use an inexpensive approximation of the golden ratio
             * to select five sample elements and determine pivots.
             */
                gint step = (size >> 3) * 3 + 3;

                /*
             * Five elements around (and including) the central element
             * will be used for pivot selection as described below. The
             * unequal choice of spacing these elements was empirically
             * determined to work well on a wide variety of inputs.
             */
                gint e1 = low + step;
                gint e5 = end - step;
                gint e3 = (e1 + e5) >> 1;
                gint e2 = (e1 + e3) >> 1;
                gint e4 = (e3 + e5) >> 1;
                gchar a3 = a[e3];

                /*
             * Sort these elements in place by the combination
             * of 4-element sorting network and insertion sort.
             *
             *    5 ------o-----------o------------
             *            |           |
             *    4 ------|-----o-----o-----o------
             *            |     |           |
             *    2 ------o-----|-----o-----o------
             *                  |     |
             *    1 ------------o-----o------------
             */
                if (a[e5] < a[e2]) {
                    gchar t = a[e5];
                    a[e5] = a[e2];
                    a[e2] = t;
                }
                if (a[e4] < a[e1]) {
                    gchar t = a[e4];
                    a[e4] = a[e1];
                    a[e1] = t;
                }
                if (a[e5] < a[e4]) {
                    gchar t = a[e5];
                    a[e5] = a[e4];
                    a[e4] = t;
                }
                if (a[e2] < a[e1]) {
                    gchar t = a[e2];
                    a[e2] = a[e1];
                    a[e1] = t;
                }
                if (a[e4] < a[e2]) {
                    gchar t = a[e4];
                    a[e4] = a[e2];
                    a[e2] = t;
                }

                if (a3 < a[e2]) {
                    if (a3 < a[e1]) {
                        a[e3] = a[e2];
                        a[e2] = a[e1];
                        a[e1] = a3;
                    } else {
                        a[e3] = a[e2];
                        a[e2] = a3;
                    }
                } else if (a3 > a[e4]) {
                    if (a3 > a[e5]) {
                        a[e3] = a[e4];
                        a[e4] = a[e5];
                        a[e5] = a3;
                    } else {
                        a[e3] = a[e4];
                        a[e4] = a3;
                    }
                }

                // Pointers
                gint lower = low; // The index of the last element of the left part
                gint upper = end; // The index of the first element of the right part

                /*
             * Partitioning with 2 pivots in case of different elements.
             */
                if (a[e1] < a[e2] && a[e2] < a[e3] && a[e3] < a[e4] && a[e4] < a[e5]) {
                    /*
                 * Use the first and fifth of the five sorted elements as
                 * the pivots. These values are inexpensive approximation
                 * of tertiles. Note, that pivot1 < pivot2.
                 */
                    gchar pivot1 = a[e1];
                    gchar pivot2 = a[e5];

                    /*
                 * The first and the last elements to be sorted are moved
                 * to the locations formerly occupied by the pivots. When
                 * partitioning is completed, the pivots are swapped back
                 * into their final positions, and excluded from the next
                 * subsequent sorting.
                 */
                    a[e1] = a[lower];
                    a[e5] = a[upper];

                    /*
                 * Skip elements, which are less or greater than the pivots.
                 */
                    while (a[++lower] < pivot1) { ; }
                    while (a[--upper] > pivot2) { ; }

                    /*
                 * Backward 3-interval partitioning
                 *
                 *   left part                 central part          right part
                 * +------------------------------------------------------------+
                 * |  < pivot1  |   ?   |  pivot1 <= && <= pivot2  |  > pivot2  |
                 * +------------------------------------------------------------+
                 *             ^       ^                            ^
                 *             |       |                            |
                 *           lower     k                          upper
                 *
                 * Invariants:
                 *
                 *              all in (low, lower] < pivot1
                 *    pivot1 <= all in (k, upper)  <= pivot2
                 *              all in [upper, end) > pivot2
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint unused = --lower, k = ++upper; --k > lower;) {
                        gchar ak = a[k];

                        if (ak < pivot1) {
                            // Move a[k] to the left side
                            while (lower < k) {
                                if (a[++lower] >= pivot1) {
                                    if (a[lower] > pivot2) {
                                        a[k] = a[--upper];
                                        a[upper] = a[lower];
                                    } else {
                                        a[k] = a[lower];
                                    }
                                    a[lower] = ak;
                                    break;
                                }
                            }
                        } else if (ak > pivot2) {
                            // Move a[k] to the right side
                            a[k] = a[--upper];
                            a[upper] = ak;
                        }
                    }

                    /*
                 * Swap the pivots into their final positions.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot1;
                    a[end] = a[upper];
                    a[upper] = pivot2;

                    /*
                 * Sort non-left parts recursively,
                 * excluding known pivots.
                 */
                    sort(a, bits | 1, lower + 1, upper);
                    sort(a, bits | 1, upper + 1, high);
                } else {
                    // Use single pivot in case of many equal elements

                    /*
                 * Use the third of the five sorted elements as the pivot.
                 * This value is inexpensive approximation of the median.
                 */
                    gchar pivot = a[e3];

                    /*
                 * The first element to be sorted is moved to the
                 * location formerly occupied by the pivot. After
                 * completion of partitioning the pivot is swapped
                 * back into its final position, and excluded from
                 * the next subsequent sorting.
                 */
                    a[e3] = a[lower];

                    /*
                 * Traditional 3-way (Dutch National Flag) partitioning
                 *
                 *   left part                 central part    right part
                 * +------------------------------------------------------+
                 * |   < pivot   |     ?     |   == pivot   |   > pivot   |
                 * +------------------------------------------------------+
                 *              ^           ^                ^
                 *              |           |                |
                 *            lower         k              upper
                 *
                 * Invariants:
                 *
                 *   all in (low, lower] < pivot
                 *   all in (k, upper)  == pivot
                 *   all in [upper, end] > pivot
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint k = ++upper; --k > lower;) {
                        gchar ak = a[k];

                        if (ak != pivot) {
                            a[k] = pivot;

                            if (ak < pivot) {
                                // Move a[k] to the left side
                                while (a[++lower] < pivot) { ; }

                                if (a[lower] > pivot) {
                                    a[--upper] = a[lower];
                                }
                                a[lower] = ak;
                            } else {
                                // ak > pivot - Move a[k] to the right side
                                a[--upper] = ak;
                            }
                        }
                    }

                    /*
                 * Swap the pivot into its final position.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot;

                    /*
                 * Sort the right part, excluding known pivot.
                 * All elements from the central part are
                 * equal and therefore already sorted.
                 */
                    sort(a, bits | 1, upper, high);
                }
                high = lower; // Iterate along the left part
            }
        }

        void DualPivotQuickSort::insertionSort(CharArray& a, gint low, gint high) {
            for (gint i, k = low; ++k < high;) {
                gchar ai = a[i = k];

                if (ai < a[i - 1]) {
                    while (--i >= low && ai < a[i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            }
        }

        void DualPivotQuickSort::countingSort(CharArray& a, gint low, gint high) {
            IntArray count = IntArray(NUM_CHAR_VALUES);

            /*
         * Compute a histogram with the number of each values.
         */
            for (gint i = high; i > low; ++count[a[--i]]) {
                ;
            }

            /*
         * Place values on their final positions.
         */
            if (high - low > NUM_CHAR_VALUES) {
                for (gint i = NUM_CHAR_VALUES; i > 0;) {
                    for (low = high - count[--i]; high > low;
                         a[--high] = (gchar) i
                    );
                }
            } else {
                for (gint i = NUM_CHAR_VALUES; high > low;) {
                    while (count[--i] == 0);
                    gint c = count[i];

                    do {
                        a[--high] = (gchar) i;
                    } while (--c > 0);
                }
            }
        }

        void DualPivotQuickSort::sort(ShortArray& a, gint low, gint high) {
            if (high - low > MIN_SHORT_COUNTING_SORT_SIZE) {
                countingSort(a, low, high);
            } else {
                sort(a, 0, low, high);
            }
        }

        void DualPivotQuickSort::sort(ShortArray& a, gint bits, gint low, gint high) {
            while (true) {
                gint end = high - 1, size = high - low;

                /*
             * Invoke insertion sort on small leftmost part.
             */
                if (size < MAX_INSERTION_SORT_SIZE) {
                    insertionSort(a, low, high);
                    return;
                }

                /*
             * Switch to counting sort if execution
             * time is becoming quadratic.
             */
                if ((bits += DELTA) > MAX_RECURSION_DEPTH) {
                    countingSort(a, low, high);
                    return;
                }

                /*
             * Use an inexpensive approximation of the golden ratio
             * to select five sample elements and determine pivots.
             */
                gint step = (size >> 3) * 3 + 3;

                /*
             * Five elements around (and including) the central element
             * will be used for pivot selection as described below. The
             * unequal choice of spacing these elements was empirically
             * determined to work well on a wide variety of inputs.
             */
                gint e1 = low + step;
                gint e5 = end - step;
                gint e3 = (e1 + e5) >> 1;
                gint e2 = (e1 + e3) >> 1;
                gint e4 = (e3 + e5) >> 1;
                gshort a3 = a[e3];

                /*
             * Sort these elements in place by the combination
             * of 4-element sorting network and insertion sort.
             *
             *    5 ------o-----------o------------
             *            |           |
             *    4 ------|-----o-----o-----o------
             *            |     |           |
             *    2 ------o-----|-----o-----o------
             *                  |     |
             *    1 ------------o-----o------------
             */
                if (a[e5] < a[e2]) {
                    gshort t = a[e5];
                    a[e5] = a[e2];
                    a[e2] = t;
                }
                if (a[e4] < a[e1]) {
                    gshort t = a[e4];
                    a[e4] = a[e1];
                    a[e1] = t;
                }
                if (a[e5] < a[e4]) {
                    gshort t = a[e5];
                    a[e5] = a[e4];
                    a[e4] = t;
                }
                if (a[e2] < a[e1]) {
                    gshort t = a[e2];
                    a[e2] = a[e1];
                    a[e1] = t;
                }
                if (a[e4] < a[e2]) {
                    gshort t = a[e4];
                    a[e4] = a[e2];
                    a[e2] = t;
                }

                if (a3 < a[e2]) {
                    if (a3 < a[e1]) {
                        a[e3] = a[e2];
                        a[e2] = a[e1];
                        a[e1] = a3;
                    } else {
                        a[e3] = a[e2];
                        a[e2] = a3;
                    }
                } else if (a3 > a[e4]) {
                    if (a3 > a[e5]) {
                        a[e3] = a[e4];
                        a[e4] = a[e5];
                        a[e5] = a3;
                    } else {
                        a[e3] = a[e4];
                        a[e4] = a3;
                    }
                }

                // Pointers
                gint lower = low; // The index of the last element of the left part
                gint upper = end; // The index of the first element of the right part

                /*
             * Partitioning with 2 pivots in case of different elements.
             */
                if (a[e1] < a[e2] && a[e2] < a[e3] && a[e3] < a[e4] && a[e4] < a[e5]) {
                    /*
                 * Use the first and fifth of the five sorted elements as
                 * the pivots. These values are inexpensive approximation
                 * of tertiles. Note, that pivot1 < pivot2.
                 */
                    gshort pivot1 = a[e1];
                    gshort pivot2 = a[e5];

                    /*
                 * The first and the last elements to be sorted are moved
                 * to the locations formerly occupied by the pivots. When
                 * partitioning is completed, the pivots are swapped back
                 * into their final positions, and excluded from the next
                 * subsequent sorting.
                 */
                    a[e1] = a[lower];
                    a[e5] = a[upper];

                    /*
                 * Skip elements, which are less or greater than the pivots.
                 */
                    while (a[++lower] < pivot1) { ; }
                    while (a[--upper] > pivot2) { ; }

                    /*
                 * Backward 3-interval partitioning
                 *
                 *   left part                 central part          right part
                 * +------------------------------------------------------------+
                 * |  < pivot1  |   ?   |  pivot1 <= && <= pivot2  |  > pivot2  |
                 * +------------------------------------------------------------+
                 *             ^       ^                            ^
                 *             |       |                            |
                 *           lower     k                          upper
                 *
                 * Invariants:
                 *
                 *              all in (low, lower] < pivot1
                 *    pivot1 <= all in (k, upper)  <= pivot2
                 *              all in [upper, end) > pivot2
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint unused = --lower, k = ++upper; --k > lower;) {
                        gshort ak = a[k];

                        if (ak < pivot1) {
                            // Move a[k] to the left side
                            while (lower < k) {
                                if (a[++lower] >= pivot1) {
                                    if (a[lower] > pivot2) {
                                        a[k] = a[--upper];
                                        a[upper] = a[lower];
                                    } else {
                                        a[k] = a[lower];
                                    }
                                    a[lower] = ak;
                                    break;
                                }
                            }
                        } else if (ak > pivot2) {
                            // Move a[k] to the right side
                            a[k] = a[--upper];
                            a[upper] = ak;
                        }
                    }

                    /*
                 * Swap the pivots into their final positions.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot1;
                    a[end] = a[upper];
                    a[upper] = pivot2;

                    /*
                 * Sort non-left parts recursively,
                 * excluding known pivots.
                 */
                    sort(a, bits | 1, lower + 1, upper);
                    sort(a, bits | 1, upper + 1, high);
                } else {
                    // Use single pivot in case of many equal elements

                    /*
                 * Use the third of the five sorted elements as the pivot.
                 * This value is inexpensive approximation of the median.
                 */
                    gshort pivot = a[e3];

                    /*
                 * The first element to be sorted is moved to the
                 * location formerly occupied by the pivot. After
                 * completion of partitioning the pivot is swapped
                 * back into its final position, and excluded from
                 * the next subsequent sorting.
                 */
                    a[e3] = a[lower];

                    /*
                 * Traditional 3-way (Dutch National Flag) partitioning
                 *
                 *   left part                 central part    right part
                 * +------------------------------------------------------+
                 * |   < pivot   |     ?     |   == pivot   |   > pivot   |
                 * +------------------------------------------------------+
                 *              ^           ^                ^
                 *              |           |                |
                 *            lower         k              upper
                 *
                 * Invariants:
                 *
                 *   all in (low, lower] < pivot
                 *   all in (k, upper)  == pivot
                 *   all in [upper, end] > pivot
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint k = ++upper; --k > lower;) {
                        gshort ak = a[k];

                        if (ak != pivot) {
                            a[k] = pivot;

                            if (ak < pivot) {
                                // Move a[k] to the left side
                                while (a[++lower] < pivot) { ; }

                                if (a[lower] > pivot) {
                                    a[--upper] = a[lower];
                                }
                                a[lower] = ak;
                            } else {
                                // ak > pivot - Move a[k] to the right side
                                a[--upper] = ak;
                            }
                        }
                    }

                    /*
                 * Swap the pivot into its final position.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot;

                    /*
                 * Sort the right part, excluding known pivot.
                 * All elements from the central part are
                 * equal and therefore already sorted.
                 */
                    sort(a, bits | 1, upper, high);
                }
                high = lower; // Iterate along the left part
            }
        }

        void DualPivotQuickSort::insertionSort(ShortArray& a, gint low, gint high) {
            for (gint i, k = low; ++k < high;) {
                gshort ai = a[i = k];

                if (ai < a[i - 1]) {
                    while (--i >= low && ai < a[i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            }
        }

        void DualPivotQuickSort::countingSort(ShortArray& a, gint low, gint high) {
            IntArray count = IntArray(NUM_SHORT_VALUES);

            /*
         * Compute a histogram with the number of each values.
         */
            for (gint i = high; i > low; ++count[a[--i] & 0xFFFF]) { ; }

            /*
         * Place values on their final positions.
         */
            if (high - low > NUM_SHORT_VALUES) {
                for (gint i = MAX_SHORT_INDEX; --i > Short::MAX_VALUE;) {
                    gint value = i & 0xFFFF;

                    for (low = high - count[value]; high > low;
                         a[--high] = (gshort) value
                    );
                }
            } else {
                for (gint i = MAX_SHORT_INDEX; high > low;) {
                    while (count[--i & 0xFFFF] == 0) { ; }

                    gint value = i & 0xFFFF;
                    gint c = count[value];

                    do {
                        a[--high] = (gshort) value;
                    } while (--c > 0);
                }
            }
        }

        void DualPivotQuickSort::sort(FloatArray& a, gint low, gint high) {
            /*
         * Phase 1. Count the number of negative zero -0.0f,
         * turn them into positive zero, and move all NaNs
         * to the end of the array.
         */
            gint numNegativeZero = 0;

            for (gint k = high; k > low;) {
                gfloat ak = a[--k];

                if (ak == 0.0f && Float::toRawIntBits(ak) < 0) {
                    // ak is -0.0f
                    numNegativeZero += 1;
                    a[k] = 0.0f;
                } else if (Float::isNaN(ak)) {
                    // ak is NaN
                    a[k] = a[--high];
                    a[high] = ak;
                }
            }

            /*
         * Phase 2. Sort everything except NaNs,
         * which are already in place.
         */
            gint size = high - low;

            {
                sort(a, 0, low, high);
            }

            /*
         * Phase 3. Turn positive zero 0.0f
         * back into negative zero -0.0f.
         */
            if (++numNegativeZero == 1) {
                return;
            }

            /*
         * Find the position one less than
         * the index of the first zero.
         */
            while (low <= high) {
                gint middle = (low + high) >> 1;

                if (a[middle] < 0) {
                    low = middle + 1;
                } else {
                    high = middle - 1;
                }
            }

            /*
         * Replace the required number of 0.0f by -0.0f.
         */
            while (--numNegativeZero > 0) {
                a[++high] = -0.0f;
            }
        }

        void DualPivotQuickSort::sort(FloatArray& a, gint bits, gint low, gint high) {
            while (true) {
                gint end = high - 1, size = high - low;

                /*
             * Run mixed insertion sort on small non-leftmost parts.
             */
                if (size < MAX_MIXED_INSERTION_SORT_SIZE + bits && (bits & 1) > 0) {
                    mixedInsertionSort(a, low, high - 3 * ((size >> 5) << 3), high);
                    return;
                }

                /*
             * Invoke insertion sort on small leftmost part.
             */
                if (size < MAX_INSERTION_SORT_SIZE) {
                    insertionSort(a, low, high);
                    return;
                }

                /*
             * Check if the whole array or large non-leftmost
             * parts are nearly sorted and then merge runs.
             */
                if ((bits == 0 || size > MIN_TRY_MERGE_SIZE && (bits & 1) > 0)
                    && tryMergeRuns(a, low, size)) {
                    return;
                }

                /*
             * Switch to heap sort if execution
             * time is becoming quadratic.
             */
                if ((bits += DELTA) > MAX_RECURSION_DEPTH) {
                    heapSort(a, low, high);
                    return;
                }

                /*
             * Use an inexpensive approximation of the golden ratio
             * to select five sample elements and determine pivots.
             */
                gint step = (size >> 3) * 3 + 3;

                /*
             * Five elements around (and including) the central element
             * will be used for pivot selection as described below. The
             * unequal choice of spacing these elements was empirically
             * determined to work well on a wide variety of inputs.
             */
                gint e1 = low + step;
                gint e5 = end - step;
                gint e3 = (e1 + e5) >> 1;
                gint e2 = (e1 + e3) >> 1;
                gint e4 = (e3 + e5) >> 1;
                gfloat a3 = a[e3];

                /*
             * Sort these elements in place by the combination
             * of 4-element sorting network and insertion sort.
             *
             *    5 ------o-----------o------------
             *            |           |
             *    4 ------|-----o-----o-----o------
             *            |     |           |
             *    2 ------o-----|-----o-----o------
             *                  |     |
             *    1 ------------o-----o------------
             */
                if (a[e5] < a[e2]) {
                    gfloat t = a[e5];
                    a[e5] = a[e2];
                    a[e2] = t;
                }
                if (a[e4] < a[e1]) {
                    gfloat t = a[e4];
                    a[e4] = a[e1];
                    a[e1] = t;
                }
                if (a[e5] < a[e4]) {
                    gfloat t = a[e5];
                    a[e5] = a[e4];
                    a[e4] = t;
                }
                if (a[e2] < a[e1]) {
                    gfloat t = a[e2];
                    a[e2] = a[e1];
                    a[e1] = t;
                }
                if (a[e4] < a[e2]) {
                    gfloat t = a[e4];
                    a[e4] = a[e2];
                    a[e2] = t;
                }

                if (a3 < a[e2]) {
                    if (a3 < a[e1]) {
                        a[e3] = a[e2];
                        a[e2] = a[e1];
                        a[e1] = a3;
                    } else {
                        a[e3] = a[e2];
                        a[e2] = a3;
                    }
                } else if (a3 > a[e4]) {
                    if (a3 > a[e5]) {
                        a[e3] = a[e4];
                        a[e4] = a[e5];
                        a[e5] = a3;
                    } else {
                        a[e3] = a[e4];
                        a[e4] = a3;
                    }
                }

                // Pointers
                gint lower = low; // The index of the last element of the left part
                gint upper = end; // The index of the first element of the right part

                /*
             * Partitioning with 2 pivots in case of different elements.
             */
                if (a[e1] < a[e2] && a[e2] < a[e3] && a[e3] < a[e4] && a[e4] < a[e5]) {
                    /*
                 * Use the first and fifth of the five sorted elements as
                 * the pivots. These values are inexpensive approximation
                 * of tertiles. Note, that pivot1 < pivot2.
                 */
                    gfloat pivot1 = a[e1];
                    gfloat pivot2 = a[e5];

                    /*
                 * The first and the last elements to be sorted are moved
                 * to the locations formerly occupied by the pivots. When
                 * partitioning is completed, the pivots are swapped back
                 * into their final positions, and excluded from the next
                 * subsequent sorting.
                 */
                    a[e1] = a[lower];
                    a[e5] = a[upper];

                    /*
                 * Skip elements, which are less or greater than the pivots.
                 */
                    while (a[++lower] < pivot1) { ; }
                    while (a[--upper] > pivot2) { ; }

                    /*
                 * Backward 3-interval partitioning
                 *
                 *   left part                 central part          right part
                 * +------------------------------------------------------------+
                 * |  < pivot1  |   ?   |  pivot1 <= && <= pivot2  |  > pivot2  |
                 * +------------------------------------------------------------+
                 *             ^       ^                            ^
                 *             |       |                            |
                 *           lower     k                          upper
                 *
                 * Invariants:
                 *
                 *              all in (low, lower] < pivot1
                 *    pivot1 <= all in (k, upper)  <= pivot2
                 *              all in [upper, end) > pivot2
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint unused = --lower, k = ++upper; --k > lower;) {
                        gfloat ak = a[k];

                        if (ak < pivot1) {
                            // Move a[k] to the left side
                            while (lower < k) {
                                if (a[++lower] >= pivot1) {
                                    if (a[lower] > pivot2) {
                                        a[k] = a[--upper];
                                        a[upper] = a[lower];
                                    } else {
                                        a[k] = a[lower];
                                    }
                                    a[lower] = ak;
                                    break;
                                }
                            }
                        } else if (ak > pivot2) {
                            // Move a[k] to the right side
                            a[k] = a[--upper];
                            a[upper] = ak;
                        }
                    }

                    /*
                 * Swap the pivots into their final positions.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot1;
                    a[end] = a[upper];
                    a[upper] = pivot2;

                    /*
                 * Sort non-left parts recursively (possibly in parallel),
                 * excluding known pivots.
                 */
                    {
                        sort(a, bits | 1, lower + 1, upper);
                        sort(a, bits | 1, upper + 1, high);
                    }
                } else {
                    // Use single pivot in case of many equal elements

                    /*
                 * Use the third of the five sorted elements as the pivot.
                 * This value is inexpensive approximation of the median.
                 */
                    gfloat pivot = a[e3];

                    /*
                 * The first element to be sorted is moved to the
                 * location formerly occupied by the pivot. After
                 * completion of partitioning the pivot is swapped
                 * back into its final position, and excluded from
                 * the next subsequent sorting.
                 */
                    a[e3] = a[lower];

                    /*
                 * Traditional 3-way (Dutch National Flag) partitioning
                 *
                 *   left part                 central part    right part
                 * +------------------------------------------------------+
                 * |   < pivot   |     ?     |   == pivot   |   > pivot   |
                 * +------------------------------------------------------+
                 *              ^           ^                ^
                 *              |           |                |
                 *            lower         k              upper
                 *
                 * Invariants:
                 *
                 *   all in (low, lower] < pivot
                 *   all in (k, upper)  == pivot
                 *   all in [upper, end] > pivot
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint k = ++upper; --k > lower;) {
                        gfloat ak = a[k];

                        if (ak != pivot) {
                            a[k] = pivot;

                            if (ak < pivot) {
                                // Move a[k] to the left side
                                while (a[++lower] < pivot) { ; }

                                if (a[lower] > pivot) {
                                    a[--upper] = a[lower];
                                }
                                a[lower] = ak;
                            } else {
                                // ak > pivot - Move a[k] to the right side
                                a[--upper] = ak;
                            }
                        }
                    }

                    /*
                 * Swap the pivot into its final position.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot;

                    /*
                 * Sort the right part (possibly in parallel), excluding
                 * known pivot. All elements from the central part are
                 * equal and therefore already sorted.
                 */
                    {
                        sort(a, bits | 1, upper, high);
                    }
                }
                high = lower; // Iterate along the left part
            }
        }

        void DualPivotQuickSort::mixedInsertionSort(FloatArray& a, gint low, gint end, gint high) {
            if (end == high) {
                /*
             * Invoke simple insertion sort on tiny array.
             */
                for (gint i; ++low < end;) {
                    gfloat ai = a[i = low];

                    while (ai < a[--i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            } else {
                /*
             * Start with pin insertion sort on small part.
             *
             * Pin insertion sort is extended simple insertion sort.
             * The main idea of this sort is to put elements larger
             * than an element called pin to the end of array (the
             * proper area for such elements). It avoids expensive
             * movements of these elements through the whole array.
             */
                gfloat pin = a[end];

                for (gint i, p = high; ++low < end;) {
                    gfloat ai = a[i = low];

                    if (ai < a[i - 1]) {
                        // Small element

                        /*
                     * Insert small element into sorted part.
                     */
                        a[i] = a[--i];

                        while (ai < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = ai;
                    } else if (p > i && ai > pin) {
                        // Large element

                        /*
                     * Find element smaller than pin.
                     */
                        while (a[--p] > pin);

                        /*
                     * Swap it with large element.
                     */
                        if (p > i) {
                            ai = a[p];
                            a[p] = a[i];
                        }

                        /*
                     * Insert small element into sorted part.
                     */
                        while (ai < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = ai;
                    }
                }

                /*
             * Continue with pair insertion sort on remain part.
             */
                for (gint i; low < high; ++low) {
                    gfloat a1 = a[i = low], a2 = a[++low];

                    /*
                 * Insert two elements per iteration: at first, insert the
                 * larger element and then insert the smaller element, but
                 * from the position where the larger element was inserted.
                 */
                    if (a1 > a2) {
                        while (a1 < a[--i]) {
                            a[i + 2] = a[i];
                        }
                        a[++i + 1] = a1;

                        while (a2 < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = a2;
                    } else if (a1 < a[i - 1]) {
                        while (a2 < a[--i]) {
                            a[i + 2] = a[i];
                        }
                        a[++i + 1] = a2;

                        while (a1 < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = a1;
                    }
                }
            }
        }

        void DualPivotQuickSort::insertionSort(FloatArray& a, gint low, gint high) {
            for (gint i, k = low; ++k < high;) {
                gfloat ai = a[i = k];

                if (ai < a[i - 1]) {
                    while (--i >= low && ai < a[i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            }
        }

        void DualPivotQuickSort::heapSort(FloatArray& a, gint low, gint high) {
            for (gint k = (low + high) >> 1; k > low;) {
                pushDown(a, --k, a[k], low, high);
            }
            while (--high > low) {
                gfloat max = a[low];
                pushDown(a, low, a[high], low, high);
                a[high] = max;
            }
        }

        void DualPivotQuickSort::pushDown(FloatArray& a, gint p, gfloat value, gint low, gint high) {
            for (gint k;; a[p] = a[p = k]) {
                k = (p << 1) - low + 2; // Index of the right child

                if (k > high) {
                    break;
                }
                if (k == high || a[k] < a[k - 1]) {
                    --k;
                }
                if (a[k] <= value) {
                    break;
                }
            }
            a[p] = value;
        }

        gbool DualPivotQuickSort::tryMergeRuns(FloatArray& a, gint low, gint size) {
            /*
         * The run array is constructed only if initial runs are
         * glong enough to continue, run[i] then holds start index
         * of the i-th sequence of elements in non-descending order.
         */
            IntArray run = IntArray(0);
            gint high = low + size;
            gint count = 1, last = low;

            /*
         * Identify all possible runs.
         */
            for (gint k = low + 1; k < high;) {
                /*
             * Find the end index of the current run.
             */
                if (a[k - 1] < a[k]) {
                    // Identify ascending sequence
                    while (++k < high && a[k - 1] <= a[k]);
                } else if (a[k - 1] > a[k]) {
                    // Identify descending sequence
                    while (++k < high && a[k - 1] >= a[k]) {
                        ;
                    }

                    // Reverse into ascending order
                    for (gint i = last - 1, j = k; ++i < --j && a[i] > a[j];) {
                        gfloat ai = a[i];
                        a[i] = a[j];
                        a[j] = ai;
                    }
                } else {
                    // Identify constant sequence
                    for (gfloat ak = a[k]; ++k < high && ak == a[k];) { ; }

                    if (k < high) {
                        continue;
                    }
                }

                /*
             * Check special cases.
             */
                if (run == null) {
                    if (k == high) {
                        /*
                     * The array is monotonous sequence,
                     * and therefore already sorted.
                     */
                        return true;
                    }

                    if (k - low < MIN_FIRST_RUN_SIZE) {
                        /*
                     * The first run is too small
                     * to proceed with scanning.
                     */
                        return false;
                    }

                    run = IntArray(((size >> 10) | 0x7F) & 0x3FF);
                    run[0] = low;
                } else if (a[last - 1] > a[last]) {
                    if (count > (k - low) >> MIN_FIRST_RUNS_FACTOR) {
                        /*
                     * The first runs are not glong
                     * enough to continue scanning.
                     */
                        return false;
                    }

                    if (++count == MAX_RUN_CAPACITY) {
                        /*
                     * Array is not highly structured.
                     */
                        return false;
                    }

                    if (count == run.length()) {
                        /*
                     * Increase capacity of index array.
                     */
                        run = Arrays::copyOf(run, count << 1);
                    }
                }
                run[count] = (last = k);
            }

            /*
         * Merge runs of highly structured array.
         */
            if (count > 1) {
                gint offset = low;
                FloatArray b = FloatArray(size);

                mergeRuns(a, b, offset, 1, run, 0, count);
            }
            return true;
        }

        FloatArray& DualPivotQuickSort::mergeRuns(FloatArray& a, FloatArray& b, gint offset, gint aim, IntArray& run,
                                                  gint lo, gint hi) {
            if (hi - lo == 1) {
                if (aim >= 0) {
                    return a;
                }
                for (gint i = run[hi], j = i - offset, low = run[lo]; i > low; b[--j] = a[--i]) { ; }
                return b;
            }

            /*
         * Split into approximately equal parts.
         */
            gint mi = lo, rmi = (run[lo] + run[hi]) >> 1;
            while (run[++mi + 1] <= rmi) { ; }

            /*
         * Merge the left and right parts.
         */
            FloatArray a1 = FloatArray(0);
            FloatArray a2 = FloatArray(0);

            {
                a1 = mergeRuns(a, b, offset, -aim, run, lo, mi);
                a2 = mergeRuns(a, b, offset, 0, run, mi, hi);
            }

            FloatArray& dst = a1 == a ? b : a;

            gint k = a1 == a ? run[lo] - offset : run[lo];
            gint lo1 = a1 == b ? run[lo] - offset : run[lo];
            gint hi1 = a1 == b ? run[mi] - offset : run[mi];
            gint lo2 = a2 == b ? run[mi] - offset : run[mi];
            gint hi2 = a2 == b ? run[hi] - offset : run[hi];

            {
                mergeParts(dst, k, a1, lo1, hi1, a2, lo2, hi2);
            }
            return dst;
        }

        void DualPivotQuickSort::mergeParts(FloatArray& dst, gint k, FloatArray& a1, gint lo1, gint hi1,
                                            FloatArray& a2, gint lo2, gint hi2) {
            /*
         * Merge small parts sequentially.
         */
            while (lo1 < hi1 && lo2 < hi2) {
                dst[k++] = a1[lo1] < a2[lo2] ? a1[lo1++] : a2[lo2++];
            }
            if (dst != a1 || k < lo1) {
                while (lo1 < hi1) {
                    dst[k++] = a1[lo1++];
                }
            }
            if (dst != a2 || k < lo2) {
                while (lo2 < hi2) {
                    dst[k++] = a2[lo2++];
                }
            }
        }

        void DualPivotQuickSort::sort(DoubleArray& a, gint low, gint high) {
            /*
             * Phase 1. Count the number of negative zero -0.0d,
             * turn them into positive zero, and move all NaNs
             * to the end of the array.
             */
            gint numNegativeZero = 0;

            for (gint k = high; k > low;) {
                gdouble ak = a[--k];

                if (ak == 0.0 && Double::toRawLongBits(ak) < 0) {
                    // ak is -0.0d
                    numNegativeZero += 1;
                    a[k] = 0.0;
                } else if (Double::isNaN(ak)) {
                    // ak is NaN
                    a[k] = a[--high];
                    a[high] = ak;
                }
            }

            /*
             * Phase 2. Sort everything except NaNs,
             * which are already in place.
             */
            gint size = high - low;

            {
                sort(a, 0, low, high);
            }

            /*
             * Phase 3. Turn positive zero 0.0d
             * back into negative zero -0.0d.
             */
            if (++numNegativeZero == 1) {
                return;
            }

            /*
             * Find the position one less than
             * the index of the first zero.
             */
            while (low <= high) {
                gint middle = (low + high) >> 1;

                if (a[middle] < 0) {
                    low = middle + 1;
                } else {
                    high = middle - 1;
                }
            }

            /*
             * Replace the required number of 0.0d by -0.0d.
             */
            while (--numNegativeZero > 0) {
                a[++high] = -0.0;
            }
        }

        void DualPivotQuickSort::sort(StringArray& a, gint low, gint high) {}

        void DualPivotQuickSort::sort(DoubleArray& a, gint bits, gint low, gint high) {
            while (true) {
                gint end = high - 1, size = high - low;

                /*
             * Run mixed insertion sort on small non-leftmost parts.
             */
                if (size < MAX_MIXED_INSERTION_SORT_SIZE + bits && (bits & 1) > 0) {
                    mixedInsertionSort(a, low, high - 3 * ((size >> 5) << 3), high);
                    return;
                }

                /*
             * Invoke insertion sort on small leftmost part.
             */
                if (size < MAX_INSERTION_SORT_SIZE) {
                    insertionSort(a, low, high);
                    return;
                }

                /*
             * Check if the whole array or large non-leftmost
             * parts are nearly sorted and then merge runs.
             */
                if ((bits == 0 || size > MIN_TRY_MERGE_SIZE && (bits & 1) > 0)
                    && tryMergeRuns(a, low, size)) {
                    return;
                }

                /*
             * Switch to heap sort if execution
             * time is becoming quadratic.
             */
                if ((bits += DELTA) > MAX_RECURSION_DEPTH) {
                    heapSort(a, low, high);
                    return;
                }

                /*
             * Use an inexpensive approximation of the golden ratio
             * to select five sample elements and determine pivots.
             */
                gint step = (size >> 3) * 3 + 3;

                /*
             * Five elements around (and including) the central element
             * will be used for pivot selection as described below. The
             * unequal choice of spacing these elements was empirically
             * determined to work well on a wide variety of inputs.
             */
                gint e1 = low + step;
                gint e5 = end - step;
                gint e3 = (e1 + e5) >> 1;
                gint e2 = (e1 + e3) >> 1;
                gint e4 = (e3 + e5) >> 1;
                gdouble a3 = a[e3];

                /*
             * Sort these elements in place by the combination
             * of 4-element sorting network and insertion sort.
             *
             *    5 ------o-----------o------------
             *            |           |
             *    4 ------|-----o-----o-----o------
             *            |     |           |
             *    2 ------o-----|-----o-----o------
             *                  |     |
             *    1 ------------o-----o------------
             */
                if (a[e5] < a[e2]) {
                    gdouble t = a[e5];
                    a[e5] = a[e2];
                    a[e2] = t;
                }
                if (a[e4] < a[e1]) {
                    gdouble t = a[e4];
                    a[e4] = a[e1];
                    a[e1] = t;
                }
                if (a[e5] < a[e4]) {
                    gdouble t = a[e5];
                    a[e5] = a[e4];
                    a[e4] = t;
                }
                if (a[e2] < a[e1]) {
                    gdouble t = a[e2];
                    a[e2] = a[e1];
                    a[e1] = t;
                }
                if (a[e4] < a[e2]) {
                    gdouble t = a[e4];
                    a[e4] = a[e2];
                    a[e2] = t;
                }

                if (a3 < a[e2]) {
                    if (a3 < a[e1]) {
                        a[e3] = a[e2];
                        a[e2] = a[e1];
                        a[e1] = a3;
                    } else {
                        a[e3] = a[e2];
                        a[e2] = a3;
                    }
                } else if (a3 > a[e4]) {
                    if (a3 > a[e5]) {
                        a[e3] = a[e4];
                        a[e4] = a[e5];
                        a[e5] = a3;
                    } else {
                        a[e3] = a[e4];
                        a[e4] = a3;
                    }
                }

                // Pointers
                gint lower = low; // The index of the last element of the left part
                gint upper = end; // The index of the first element of the right part

                /*
             * Partitioning with 2 pivots in case of different elements.
             */
                if (a[e1] < a[e2] && a[e2] < a[e3] && a[e3] < a[e4] && a[e4] < a[e5]) {
                    /*
                 * Use the first and fifth of the five sorted elements as
                 * the pivots. These values are inexpensive approximation
                 * of tertiles. Note, that pivot1 < pivot2.
                 */
                    gdouble pivot1 = a[e1];
                    gdouble pivot2 = a[e5];

                    /*
                 * The first and the last elements to be sorted are moved
                 * to the locations formerly occupied by the pivots. When
                 * partitioning is completed, the pivots are swapped back
                 * into their final positions, and excluded from the next
                 * subsequent sorting.
                 */
                    a[e1] = a[lower];
                    a[e5] = a[upper];

                    /*
                 * Skip elements, which are less or greater than the pivots.
                 */
                    while (a[++lower] < pivot1) { ; }
                    while (a[--upper] > pivot2) { ; }

                    /*
                 * Backward 3-interval partitioning
                 *
                 *   left part                 central part          right part
                 * +------------------------------------------------------------+
                 * |  < pivot1  |   ?   |  pivot1 <= && <= pivot2  |  > pivot2  |
                 * +------------------------------------------------------------+
                 *             ^       ^                            ^
                 *             |       |                            |
                 *           lower     k                          upper
                 *
                 * Invariants:
                 *
                 *              all in (low, lower] < pivot1
                 *    pivot1 <= all in (k, upper)  <= pivot2
                 *              all in [upper, end) > pivot2
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint unused = --lower, k = ++upper; --k > lower;) {
                        gdouble ak = a[k];

                        if (ak < pivot1) {
                            // Move a[k] to the left side
                            while (lower < k) {
                                if (a[++lower] >= pivot1) {
                                    if (a[lower] > pivot2) {
                                        a[k] = a[--upper];
                                        a[upper] = a[lower];
                                    } else {
                                        a[k] = a[lower];
                                    }
                                    a[lower] = ak;
                                    break;
                                }
                            }
                        } else if (ak > pivot2) {
                            // Move a[k] to the right side
                            a[k] = a[--upper];
                            a[upper] = ak;
                        }
                    }

                    /*
                 * Swap the pivots into their final positions.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot1;
                    a[end] = a[upper];
                    a[upper] = pivot2;

                    /*
                 * Sort non-left parts recursively (possibly in parallel),
                 * excluding known pivots.
                 */
                    {
                        sort(a, bits | 1, lower + 1, upper);
                        sort(a, bits | 1, upper + 1, high);
                    }
                } else {
                    // Use single pivot in case of many equal elements

                    /*
                 * Use the third of the five sorted elements as the pivot.
                 * This value is inexpensive approximation of the median.
                 */
                    gdouble pivot = a[e3];

                    /*
                 * The first element to be sorted is moved to the
                 * location formerly occupied by the pivot. After
                 * completion of partitioning the pivot is swapped
                 * back into its final position, and excluded from
                 * the next subsequent sorting.
                 */
                    a[e3] = a[lower];

                    /*
                 * Traditional 3-way (Dutch National Flag) partitioning
                 *
                 *   left part                 central part    right part
                 * +------------------------------------------------------+
                 * |   < pivot   |     ?     |   == pivot   |   > pivot   |
                 * +------------------------------------------------------+
                 *              ^           ^                ^
                 *              |           |                |
                 *            lower         k              upper
                 *
                 * Invariants:
                 *
                 *   all in (low, lower] < pivot
                 *   all in (k, upper)  == pivot
                 *   all in [upper, end] > pivot
                 *
                 * Pointer k is the last index of ?-part
                 */
                    for (gint k = ++upper; --k > lower;) {
                        gdouble ak = a[k];

                        if (ak != pivot) {
                            a[k] = pivot;

                            if (ak < pivot) {
                                // Move a[k] to the left side
                                while (a[++lower] < pivot) { ; }

                                if (a[lower] > pivot) {
                                    a[--upper] = a[lower];
                                }
                                a[lower] = ak;
                            } else {
                                // ak > pivot - Move a[k] to the right side
                                a[--upper] = ak;
                            }
                        }
                    }

                    /*
                 * Swap the pivot into its final position.
                 */
                    a[low] = a[lower];
                    a[lower] = pivot;

                    /*
                 * Sort the right part (possibly in parallel), excluding
                 * known pivot. All elements from the central part are
                 * equal and therefore already sorted.
                 */
                    {
                        sort(a, bits | 1, upper, high);
                    }
                }
                high = lower; // Iterate along the left part
            }
        }

        void DualPivotQuickSort::mixedInsertionSort(DoubleArray& a, gint low, gint end, gint high) {
            if (end == high) {
                /*
             * Invoke simple insertion sort on tiny array.
             */
                for (gint i; ++low < end;) {
                    gdouble ai = a[i = low];

                    while (ai < a[--i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            } else {
                /*
             * Start with pin insertion sort on small part.
             *
             * Pin insertion sort is extended simple insertion sort.
             * The main idea of this sort is to put elements larger
             * than an element called pin to the end of array (the
             * proper area for such elements). It avoids expensive
             * movements of these elements through the whole array.
             */
                gdouble pin = a[end];

                for (gint i, p = high; ++low < end;) {
                    gdouble ai = a[i = low];

                    if (ai < a[i - 1]) {
                        // Small element

                        /*
                     * Insert small element into sorted part.
                     */
                        a[i] = a[--i];

                        while (ai < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = ai;
                    } else if (p > i && ai > pin) {
                        // Large element

                        /*
                     * Find element smaller than pin.
                     */
                        while (a[--p] > pin);

                        /*
                     * Swap it with large element.
                     */
                        if (p > i) {
                            ai = a[p];
                            a[p] = a[i];
                        }

                        /*
                     * Insert small element into sorted part.
                     */
                        while (ai < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = ai;
                    }
                }

                /*
             * Continue with pair insertion sort on remain part.
             */
                for (gint i; low < high; ++low) {
                    gdouble a1 = a[i = low], a2 = a[++low];

                    /*
                 * Insert two elements per iteration: at first, insert the
                 * larger element and then insert the smaller element, but
                 * from the position where the larger element was inserted.
                 */
                    if (a1 > a2) {
                        while (a1 < a[--i]) {
                            a[i + 2] = a[i];
                        }
                        a[++i + 1] = a1;

                        while (a2 < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = a2;
                    } else if (a1 < a[i - 1]) {
                        while (a2 < a[--i]) {
                            a[i + 2] = a[i];
                        }
                        a[++i + 1] = a2;

                        while (a1 < a[--i]) {
                            a[i + 1] = a[i];
                        }
                        a[i + 1] = a1;
                    }
                }
            }
        }

        void DualPivotQuickSort::insertionSort(DoubleArray& a, gint low, gint high) {
            for (gint i, k = low; ++k < high;) {
                gdouble ai = a[i = k];

                if (ai < a[i - 1]) {
                    while (--i >= low && ai < a[i]) {
                        a[i + 1] = a[i];
                    }
                    a[i + 1] = ai;
                }
            }
        }

        void DualPivotQuickSort::heapSort(DoubleArray& a, gint low, gint high) {
            for (gint k = (low + high) >> 1; k > low;) {
                pushDown(a, --k, a[k], low, high);
            }
            while (--high > low) {
                gdouble max = a[low];
                pushDown(a, low, a[high], low, high);
                a[high] = max;
            }
        }

        void DualPivotQuickSort::pushDown(DoubleArray& a, gint p, gdouble value, gint low, gint high) {
            for (gint k;; a[p] = a[p = k]) {
                k = (p << 1) - low + 2; // Index of the right child

                if (k > high) {
                    break;
                }
                if (k == high || a[k] < a[k - 1]) {
                    --k;
                }
                if (a[k] <= value) {
                    break;
                }
            }
            a[p] = value;
        }

        gbool DualPivotQuickSort::tryMergeRuns(DoubleArray& a, gint low, gint size) {
            /*
         * The run array is constructed only if initial runs are
         * glong enough to continue, run[i] then holds start index
         * of the i-th sequence of elements in non-descending order.
         */
            IntArray run = IntArray(0);
            gint high = low + size;
            gint count = 1, last = low;

            /*
         * Identify all possible runs.
         */
            for (gint k = low + 1; k < high;) {
                /*
             * Find the end index of the current run.
             */
                if (a[k - 1] < a[k]) {
                    // Identify ascending sequence
                    while (++k < high && a[k - 1] <= a[k]);
                } else if (a[k - 1] > a[k]) {
                    // Identify descending sequence
                    while (++k < high && a[k - 1] >= a[k]) {
                        ;
                    }

                    // Reverse into ascending order
                    for (gint i = last - 1, j = k; ++i < --j && a[i] > a[j];) {
                        gdouble ai = a[i];
                        a[i] = a[j];
                        a[j] = ai;
                    }
                } else {
                    // Identify constant sequence
                    for (gdouble ak = a[k]; ++k < high && ak == a[k];);

                    if (k < high) {
                        continue;
                    }
                }

                /*
             * Check special cases.
             */
                if (run == null) {
                    if (k == high) {
                        /*
                     * The array is monotonous sequence,
                     * and therefore already sorted.
                     */
                        return true;
                    }

                    if (k - low < MIN_FIRST_RUN_SIZE) {
                        /*
                     * The first run is too small
                     * to proceed with scanning.
                     */
                        return false;
                    }

                    run = IntArray(((size >> 10) | 0x7F) & 0x3FF);
                    run[0] = low;
                } else if (a[last - 1] > a[last]) {
                    if (count > (k - low) >> MIN_FIRST_RUNS_FACTOR) {
                        /*
                     * The first runs are not glong
                     * enough to continue scanning.
                     */
                        return false;
                    }

                    if (++count == MAX_RUN_CAPACITY) {
                        /*
                     * Array is not highly structured.
                     */
                        return false;
                    }

                    if (count == run.length()) {
                        /*
                     * Increase capacity of index array.
                     */
                        run = Arrays::copyOf(run, count << 1);
                    }
                }
                run[count] = (last = k);
            }

            /*
         * Merge runs of highly structured array.
         */
            if (count > 1) {
                gint offset = low;
                DoubleArray b = DoubleArray(size);

                mergeRuns(a, b, offset, 1, run, 0, count);
            }
            return true;
        }

        DoubleArray& DualPivotQuickSort::mergeRuns(DoubleArray& a, DoubleArray& b, gint offset, gint aim,
                                                   IntArray& run, gint lo, gint hi) {
            if (hi - lo == 1) {
                if (aim >= 0) {
                    return a;
                }
                for (gint i = run[hi], j = i - offset, low = run[lo]; i > low; b[--j] = a[--i]) { ; }
                return b;
            }

            /*
         * Split into approximately equal parts.
         */
            gint mi = lo, rmi = (run[lo] + run[hi]) >> 1;
            while (run[++mi + 1] <= rmi) { ; }

            /*
         * Merge the left and right parts.
         */
            DoubleArray a1 = DoubleArray(0);
            DoubleArray a2 = DoubleArray(0);

            {
                a1 = mergeRuns(a, b, offset, -aim, run, lo, mi);
                a2 = mergeRuns(a, b, offset, 0, run, mi, hi);
            }

            DoubleArray& dst = a1 == a ? b : a;

            gint k = a1 == a ? run[lo] - offset : run[lo];
            gint lo1 = a1 == b ? run[lo] - offset : run[lo];
            gint hi1 = a1 == b ? run[mi] - offset : run[mi];
            gint lo2 = a2 == b ? run[mi] - offset : run[mi];
            gint hi2 = a2 == b ? run[hi] - offset : run[hi];

            {
                mergeParts(dst, k, a1, lo1, hi1, a2, lo2, hi2);
            }
            return dst;
        }

        void DualPivotQuickSort::mergeParts(DoubleArray& dst, gint k, DoubleArray& a1, gint lo1, gint hi1,
                                            DoubleArray& a2, gint lo2, gint hi2) {
            /*
         * Merge small parts sequentially.
         */
            while (lo1 < hi1 && lo2 < hi2) {
                dst[k++] = a1[lo1] < a2[lo2] ? a1[lo1++] : a2[lo2++];
            }
            if (dst != a1 || k < lo1) {
                while (lo1 < hi1) {
                    dst[k++] = a1[lo1++];
                }
            }
            if (dst != a2 || k < lo2) {
                while (lo2 < hi2) {
                    dst[k++] = a2[lo2++];
                }
            }
        }
    } // util
} // core
