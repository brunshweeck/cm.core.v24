//
// Created by brunshweeck on 22 juin 2024.
//

#ifndef CORE24_ARRAYS_H
#define CORE24_ARRAYS_H

#include "Comparator.h"

#include <core/Array.h>

namespace core {
    namespace util {
        /**
         * This class contains various methods for manipulating arrays (such as
         * sorting and searching). This class also contains a static factory
         * that allows arrays to be viewed as lists.
         *
         * <p>
         * The documentation for the methods contained in this class includes
         * brief descriptions of the <i>implementations</i>. Such descriptions should
         * be regarded as <i>implementation notes</i>, rather than parts of the
         * <i>specification</i>. Implementors should feel free to substitute other
         * algorithms, so long as the specification itself is adhered to. (For
         * example, the algorithm used by @c Arrays::sort(Array<?>) does not have to be
         * a MergeSort, but it does have to be <i>stable</i>.)
         * </p>
         */
        class Arrays final : public Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            CORE_ADD_GLOBAL_FRIENDS();

            CORE_ADD_AS_FRIEND(io::Buffer);

            CORE_IMPLICIT Arrays() = default;

        public:
            static CORE_FAST gint LOG2_BOOLEAN_BIT_SIZE = 0;
            static CORE_FAST gint LOG2_BYTE_BIT_SIZE = 0;
            static CORE_FAST gint LOG2_CHAR_BIT_SIZE = 1;
            static CORE_FAST gint LOG2_SHORT_BIT_SIZE = 1;
            static CORE_FAST gint LOG2_INT_BIT_SIZE = 2;
            static CORE_FAST gint LOG2_FLOAT_BIT_SIZE = 2;
            static CORE_FAST gint LOG2_LONG_BIT_SIZE = 3;
            static CORE_FAST gint LOG2_DOUBLE_BIT_SIZE = 3;

            static gint CORE_FAST LOG2_BOOLEAN_ARRAY_INDEX_SCALE = LOG2_BOOLEAN_BIT_SIZE;
            static gint CORE_FAST LOG2_BYTE_ARRAY_INDEX_SCALE = LOG2_BYTE_BIT_SIZE;
            static gint CORE_FAST LOG2_CHAR_ARRAY_INDEX_SCALE = LOG2_CHAR_BIT_SIZE;
            static gint CORE_FAST LOG2_SHORT_ARRAY_INDEX_SCALE = LOG2_SHORT_BIT_SIZE;
            static gint CORE_FAST LOG2_INT_ARRAY_INDEX_SCALE = LOG2_INT_BIT_SIZE;
            static gint CORE_FAST LOG2_FLOAT_ARRAY_INDEX_SCALE = LOG2_FLOAT_BIT_SIZE;
            static gint CORE_FAST LOG2_LONG_ARRAY_INDEX_SCALE = LOG2_LONG_BIT_SIZE;
            static gint CORE_FAST LOG2_DOUBLE_ARRAY_INDEX_SCALE = LOG2_DOUBLE_BIT_SIZE;

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(IntArray& a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index @c fromIndex, inclusive, to
             * the index @c toIndex, exclusive. If @code fromIndex == toIndex @endcode,
             * the range to be sorted is empty.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             */
            static void sort(IntArray& a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(LongArray& a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index @c fromIndex, inclusive, to
             * the index @c toIndex, exclusive. If @code fromIndex == toIndex @endcode,
             * the range to be sorted is empty.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             */
            static void sort(LongArray& a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(ShortArray& a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index @c fromIndex, inclusive, to
             * the index @c toIndex, exclusive. If @code fromIndex == toIndex @endcode,
             * the range to be sorted is empty.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             */
            static void sort(ShortArray& a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(CharArray& a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index @c fromIndex, inclusive, to
             * the index @c toIndex, exclusive. If @code fromIndex == toIndex @endcode,
             * the range to be sorted is empty.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             */
            static void sort(CharArray& a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(ByteArray& a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index @c fromIndex, inclusive, to
             * the index @c toIndex, exclusive. If @code fromIndex == toIndex @endcode,
             * the range to be sorted is empty.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             */
            static void sort(ByteArray& a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             * <p>
             * The @c < relation does not provide a total order on all float
             * values: @c -0.0F == 0.0F is @c true and a @c Float::NaN
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * @c Float::compareTo: @c -0.0F is treated as less than value
             * @c 0.0F and @c Float::NaN is considered greater than any
             * other value and all @c Float::NaN values are considered equal.
             * </p>
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(FloatArray& a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index @c fromIndex, inclusive, to
             * the index @c toIndex, exclusive. If @code fromIndex == toIndex @endcode,
             * the range to be sorted is empty.
             * <p>
             * The @c < relation does not provide a total order on all float
             * values: @c -0.0F == 0.0F is @c true and a @c Float::NaN
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * @c Float::compareTo: @c -0.0F is treated as less than value
             * @c 0.0F and @c Float::NaN is considered greater than any
             * other value and all @c Float::NaN values are considered equal.
             * </p>
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             */
            static void sort(FloatArray& a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending numerical order.
             * <p>
             * The @c < relation does not provide a total order on all float
             * values: @c -0.0 == 0.0 is @c true and a @c Float::NaN
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * @c Double::compareTo: @c -0.0 is treated as less than value
             * @c 0.0 and @c Double::NaN is considered greater than any
             * other value and all @c Double::NaN values are considered equal.
             * </p>
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(DoubleArray& a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index @c fromIndex, inclusive, to
             * the index @c toIndex, exclusive. If @code fromIndex == toIndex @endcode,
             * the range to be sorted is empty.
             * <p>
             * The @c < relation does not provide a total order on all float
             * values: @c -0.0 == 0.0 is @c true and a @c Double::NaN
             * value compares neither less than, greater than, nor equal to any value,
             * even itself. This method uses the total order imposed by the method
             * @c Double::compareTo: @c -0.0 is treated as less than value
             * @c 0.0 and @c Double::NaN is considered greater than any
             * other value and all @c Double::NaN values are considered equal.
             * </p>
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             */
            static void sort(DoubleArray& a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array into ascending lexicographic order.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             */
            static void sort(StringArray& a);

            /**
             * Sorts the specified range of the array into ascending order. The range
             * to be sorted extends from the index @c fromIndex, inclusive, to
             * the index @c toIndex, exclusive. If @code fromIndex == toIndex @endcode,
             * the range to be sorted is empty.
             *
             * @note The sorting algorithm is a Dual-Pivot Quicksort. This algorithm
             *       offers <i> O(n log(n))</i> performance on all data sets, and is typically
             *       faster than traditional (one-pivot) Quicksort implementations.
             *
             * @param a the array to be sorted
             * @param fromIndex the index of the first element, inclusive, to be sorted
             * @param toIndex the index of the last element, exclusive, to be sorted
             */
            static void sort(StringArray& a, gint fromIndex, gint toIndex);

            /**
             * Sorts the specified array of objects into ascending order, according
             * to the <em> Natural ordering </em> of its elements.
             * All elements in the array must implement the @c Comparable
             * interface.  Furthermore, all elements in the array must be
             * <i>mutually comparable</i> (that is, @c e1.compareTo(e2) must
             * not throw a @c ClassCastException for any elements @c e1
             * and @c e2 in the array).
             * <p>
             * This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             * </p>
             * <p>
             * Implementation note: This implementation is a stable, adaptive,
             * iterative mergesort that requires far fewer than n lg(n) comparisons
             * when the input array is partially sorted, while offering the
             * performance of a traditional mergesort when the input array is
             * randomly ordered.  If the input array is nearly sorted, the
             * implementation requires approximately n comparisons.  Temporary
             * storage requirements vary from a small constant for nearly sorted
             * input arrays to n/2 object references for randomly ordered input
             * arrays.
             * </p>
             * <p>
             * The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merge two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             * </p>
             * <p>
             * The implementation was adapted from Tim Peters's list sort for Python
             * (TimSort).  It uses techniques from Peter MCILROY's "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             * </p>
             * @param a the array to be sorted
             * @throws IllegalArgumentException (optional) if the natural
             *         ordering of the array elements is found to violate the
             *         @c Comparable contract
             */
            template <class T, ClassOf(1)::OnlyIfAll<Class<Comparable<T>>::template isSuper<T>()>  = 1>
            static void sort(Array<T>& a) {
                try {
                    legacyMergeSort(a, 0, a.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Sorts the specified range of the specified array of objects into
             * ascending order, according to the
             * <em> Natural ordering </em> of its
             * elements.  The range to be sorted extends from index
             * @c fromIndex, inclusive, to index @c toIndex, exclusive.
             * (If @code fromIndex == toIndex @endcode, the range to be sorted is empty.)  All
             * elements in this range must implement the @c Comparable
             * interface.  Furthermore, all elements in this range must be <i>mutually
             * comparable</i> (that is, @c e1.compareTo(e2) must not throw a
             * @c ClassCastException for any elements @c e1 and
             * @c e2 in the array).
             *
             * <p>
             * This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             * </p>
             * <p>
             * Implementation note: This implementation is a stable, adaptive,
             * iterative mergesort that requires far fewer than n lg(n) comparisons
             * when the input array is partially sorted, while offering the
             * performance of a traditional mergesort when the input array is
             * randomly ordered.  If the input array is nearly sorted, the
             * implementation requires approximately n comparisons.  Temporary
             * storage requirements vary from a small constant for nearly sorted
             * input arrays to n/2 object references for randomly ordered input
             * arrays.
             * </p>
             * <p>
             * The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merge two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             * </p>
             * <p>
             * The implementation was adapted from Tim Peters's list sort for Python
             * (TimSort).  It uses techniques from Peter MCILROY's "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             * </p>
             * @param a the array to be sorted
             * @param fromIndex the index of the first element (inclusive) to be
             *        sorted
             * @param toIndex the index of the last element (exclusive) to be sorted
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode or
             *         (optional) if the natural ordering of the array elements is
             *         found to violate the @c Comparable contract
             */
            template <class T, ClassOf(1)::OnlyIfAll<Class<Comparable<T>>::template isSuper<T>()>  = 1>
            static void sort(Array<T>& a, gint fromIndex, gint toIndex) {
                try {
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                    legacyMergeSort(a, fromIndex, toIndex);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Sorts the specified array of objects according to the order induced by
             * the specified comparator.  All elements in the array must be
             * <i>mutually comparable</i> by the specified comparator (that is,
             * @code c.compare(e1, e2) @endcode must not throw a @c ClassCastException
             * for any elements @c e1 and @c e2 in the array).
             *
             * <p>
             * This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             * </p>
             * <p>
             * Implementation note: This implementation is a stable, adaptive,
             * iterative mergesort that requires far fewer than n lg(n) comparisons
             * when the input array is partially sorted, while offering the
             * performance of a traditional mergesort when the input array is
             * randomly ordered.  If the input array is nearly sorted, the
             * implementation requires approximately n comparisons.  Temporary
             * storage requirements vary from a small constant for nearly sorted
             * input arrays to n/2 object references for randomly ordered input
             * arrays.
             * </p>
             * <p>
             * The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merge two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             * </p>
             * <p>
             * The implementation was adapted from Tim Peters's list sort for Python
             * (TimSort).  It uses techniques from Peter MCILROY's "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             * </p>
             * @tparam T the class of the objects to be sorted
             * @param a the array to be sorted
             * @param comparator the comparator to determine the order of the array.  A
             *        @c null value indicates that the elements'
             *        <em> Natural ordering </em> should be used.
             * @throws IllegalArgumentException (optional) if the comparator is
             *         found to violate the @c Comparator contract
             */
            template <class T>
            static void sort(Array<T>& a, Comparator<T> const& comparator) {
                try {
                    legacyMergeSort(a, 0, a.length(), comparator);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Sorts the specified range of the specified array of objects according
             * to the order induced by the specified comparator.  The range to be
             * sorted extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be sorted is empty.)  All elements in the range must be
             * <i>mutually comparable</i> by the specified comparator (that is,
             * @code c.compare(e1, e2) @endcode must not throw a @c ClassCastException
             * for any elements @c e1 and @c e2 in the range).
             *
             * <p>
             * This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             * </p>
             * <p>
             * Implementation note: This implementation is a stable, adaptive,
             * iterative mergesort that requires far fewer than n lg(n) comparisons
             * when the input array is partially sorted, while offering the
             * performance of a traditional mergesort when the input array is
             * randomly ordered.  If the input array is nearly sorted, the
             * implementation requires approximately n comparisons.  Temporary
             * storage requirements vary from a small constant for nearly sorted
             * input arrays to n/2 object references for randomly ordered input
             * arrays.
             * </p>
             * <p>
             * The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merge two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             * </p>
             * <p>
             * The implementation was adapted from Tim Peters's list sort for Python
             * (TimSort).  It uses techniques from Peter MCILROY's "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             * </p>
             * @tparam T the class of the objects to be sorted
             * @param a the array to be sorted
             * @param fromIndex the index of the first element (inclusive) to be
             *        sorted
             * @param toIndex the index of the last element (exclusive) to be sorted
             * @param comparator the comparator to determine the order of the array.  A
             *        @c null value indicates that the elements'
             *        <em> Natural ordering </em> should be used.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode or
             *         (optional) if the comparator is found to violate the
             *         @c Comparator contract
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            template <class T>
            static void sort(Array<T>& a, gint fromIndex, gint toIndex, Comparator<T> const& comparator) {
                try {
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, a.length());
                    legacyMergeSort(a, fromIndex, toIndex, comparator);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            // Swapping

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(BooleanArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(ByteArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(CharArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(ShortArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(IntArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(FloatArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(LongArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(DoubleArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            static void swap(StringArray& a, gint i, gint j);

            /**
             * Swaps a[i] with a[j].
             */
            template <class T>
            static void swap(Array<T>& a, gint i, gint j) {
                try {
                    gint length = a.length();
                    misc::Preconditions::checkIndex(i, length);
                    misc::Preconditions::checkIndex(j, length);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                swap0(a, i, j);
            }

            // Searching

            /**
             * Searches the specified array of bytes for the specified value using the
             * binary search algorithm.  The array must be sorted (as
             * by the @c arrays::sort(ByteArray) method) prior to making this call.  If it
             * is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(ByteArray const& a, gbyte key);

            /**
             * Searches a range of
             * the specified array of bytes for the specified value using the
             * binary search algorithm.
             * The range must be sorted (as by the @code arrays::sort(ByteArray, int, int) @endcode method)
             * prior to making this call.  If it
             * is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            static gint binarySearch(ByteArray const& a, gint fromIndex, gint toIndex, gbyte key);

            /**
             * Searches the specified array of shorts for the specified value using
             * the binary search algorithm.  The array must be sorted
             * (as by the @c arrays::sort(ShortArray) method) prior to making this call.  If
             * it is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(ShortArray const& a, gshort key);

            /**
             * Searches a range of
             * the specified array of shorts for the specified value using
             * the binary search algorithm.
             * The range must be sorted
             * (as by the @code arrays::sort(ShortArray, int, int) @endcode method)
             * prior to making this call.  If
             * it is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            static gint binarySearch(ShortArray const& a, gint fromIndex, gint toIndex, gshort key);

            /**
             * Searches the specified array of chars for the specified value using the
             * binary search algorithm.  The array must be sorted (as
             * by the @c arrays::sort(CharArray) method) prior to making this call.  If it
             * is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(CharArray const& a, gchar key);

            /**
             * Searches a range of
             * the specified array of chars for the specified value using the
             * binary search algorithm.
             * The range must be sorted (as
             * by the @code arrays::sort(CharArray, int, int) @endcode method)
             * prior to making this call.  If it
             * is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            static gint binarySearch(CharArray const& a, gint fromIndex, gint toIndex, gchar key);

            /**
             * Searches the specified array of ints for the specified value using the
             * binary search algorithm.  The array must be sorted (as
             * by the @c arrays::sort(IntArray) method) prior to making this call.  If it
             * is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(IntArray const& a, gint key);

            /**
             * Searches a range of
             * the specified array of ints for the specified value using the
             * binary search algorithm.
             * The range must be sorted (as
             * by the @code arrays::sort(IntArray, int, int) @endcode method)
             * prior to making this call.  If it
             * is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            static gint binarySearch(IntArray const& a, gint fromIndex, gint toIndex, gint key);

            /**
             * Searches the specified array of longs for the specified value using the
             * binary search algorithm.  The array must be sorted (as
             * by the @c arrays::sort(LongArray) method) prior to making this call.  If it
             * is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(LongArray const& a, glong key);

            /**
             * Searches a range of
             * the specified array of longs for the specified value using the
             * binary search algorithm.
             * The range must be sorted (as
             * by the @code arrays::sort(LongArray, int, int) @endcode method)
             * prior to making this call.  If it
             * is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             */
            static gint binarySearch(LongArray const& a, gint fromIndex, gint toIndex, glong key);

            /**
             * Searches the specified array of floats for the specified value using
             * the binary search algorithm. The array must be sorted
             * (as by the @c arrays::sort(FloatArray) method) prior to making this call. If
             * it is not sorted, the results are undefined. If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found. This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>. The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key. Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(FloatArray const& a, gfloat key);

            /**
             * Searches a range of
             * the specified array of floats for the specified value using
             * the binary search algorithm.
             * The range must be sorted
             * (as by the @code arrays::sort(FloatArray, int, int) @endcode method)
             * prior to making this call. If
             * it is not sorted, the results are undefined. If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found. This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a The array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(- (insertion point) - 1)</code>. The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key. Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            static gint binarySearch(FloatArray const& a, gint fromIndex, gint toIndex, gfloat key);

            /**
             * Searches the specified array of doubles for the specified value using
             * the binary search algorithm.  The array must be sorted
             * (as by the @c arrays::sort(DoubleArray) method) prior to making this call.
             * If it is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.  This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(DoubleArray const& a, gdouble key);

            /**
             * Searches a range of
             * the specified array of doubles for the specified value using
             * the binary search algorithm.
             * The range must be sorted
             * (as by the @code arrays::sort(DoubleArray, int, int) @endcode method)
             * prior to making this call.
             * If it is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.  This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            static gint binarySearch(DoubleArray const& a, gint fromIndex, gint toIndex, gdouble key);

            /**
             * Searches the specified array of strings for the specified value using
             * the binary search algorithm.  The array must be sorted
             * (as by the @c arrays::sort(StringArray) method) prior to making this call.
             * If it is not sorted, the results are undefined.  If the array contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.  This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             */
            static gint binarySearch(StringArray const& a, String const& key);

            /**
             * Searches a range of
             * the specified array of strings for the specified value using
             * the binary search algorithm.
             * The range must be sorted
             * (as by the @code arrays::sort(StringArray, int, int) @endcode method)
             * prior to making this call.
             * If it is not sorted, the results are undefined.  If the range contains
             * multiple elements with the specified value, there is no guarantee which
             * one will be found.  This method considers all NaN values to be
             * equivalent and equal.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            static gint binarySearch(StringArray const& a, gint fromIndex, gint toIndex, String const& key);

            /**
             * Searches the specified array for the specified object using the binary
             * search algorithm. The array must be sorted into ascending order
             * according to the
             * <em> Natural ordering </em>
             * of its elements (as by the @c arrays::sort(Array<?>) method) prior to making this call.
             * If it is not sorted, the results are undefined.
             * (If the array contains elements that are not mutually comparable (for
             * example, strings and integers), it <i>cannot</i> be sorted according
             * to the natural ordering of its elements, hence results are undefined.)
             * If the array contains multiple
             * elements equal to the specified object, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the search key is not comparable to the
             *         elements of the array.
             */
            template <class T, ClassOf(1)::OnlyIfAll<Class<Comparable<T>>::template isSuper<T>()>  = 1>
            static gint binarySearch(Array<T> const& a, T const& key) {
                return binarySearch(a, 0, a.length(), key);
            }

            /**
             * Searches a range of
             * the specified array for the specified object using the binary
             * search algorithm.
             * The range must be sorted into ascending order
             * according to the
             * <em> Natural ordering </em>
             * of its elements (as by the @code arrays::sort(Array<?>, int, int) @endcode method) prior to making this
             * call.  If it is not sorted, the results are undefined.
             * (If the range contains elements that are not mutually comparable (for
             * example, strings and integers), it <i>cannot</i> be sorted according
             * to the natural ordering of its elements, hence results are undefined.)
             * If the range contains multiple
             * elements equal to the specified object, there is no guarantee which
             * one will be found.
             *
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the search key is not comparable to the
             *         elements of the array within the specified range.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            template <class T>
            static gint binarySearch(Array<T> const& a, gint fromIndex, gint toIndex, T const& key) {
                try {
                    int low = fromIndex;
                    int high = toIndex - 1;

                    while (low <= high) {
                        int mid = (low + high) >> 1;
                        Comparable<T> const &midVal = CORE_XCAST(Comparable<T> const, a[mid]);
                        int cmp = midVal.compareTo(key);

                        if (cmp < 0)
                            low = mid + 1;
                        else if (cmp > 0)
                            high = mid - 1;
                        else
                            return mid; // key found
                    }
                    return -(low + 1); // key not found.
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Searches the specified array for the specified object using the binary
             * search algorithm.  The array must be sorted into ascending order
             * according to the specified comparator (as by the
             * @code arrays::sort(Array<?>, Comparator) @endcode method) prior to making this call.  If it is
             * not sorted, the results are undefined.
             * If the array contains multiple
             * elements equal to the specified object, there is no guarantee which one
             * will be found.
             *
             * @tparam T the class of the objects in the array
             * @param a the array to be searched
             * @param key the value to be searched for
             * @param comparator the comparator by which the array is ordered.  A
             *        @c null value indicates that the elements'
             *        <em> Natural ordering </em> should be used.
             * @return index of the search key, if it is contained in the array;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element greater than the key, or @c a.length() if all
             *         elements in the array are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the array contains elements that are not
             *         <i>mutually comparable</i> using the specified comparator,
             *         or the search key is not comparable to the
             *         elements of the array using this comparator.
             */
            template <class T>
            static gint binarySearch(Array<T> const& a, T const& key, Comparator<T> const& comparator) {
                return binarySearch(a, 0, a.length(), key, comparator);
            }

            /**
             * Searches a range of
             * the specified array for the specified object using the binary
             * search algorithm.
             * The range must be sorted into ascending order
             * according to the specified comparator (as by the
             * @code arrays::sort(Array<?>, int, int, Comparator) @endcode method) prior to making this call.
             * If it is not sorted, the results are undefined.
             * If the range contains multiple elements equal to the specified object,
             * there is no guarantee which one will be found.
             *
             * @tparam T the class of the objects in the array
             * @param a the array to be searched
             * @param fromIndex the index of the first element (inclusive) to be
             *          searched
             * @param toIndex the index of the last element (exclusive) to be searched
             * @param key the value to be searched for
             * @param comparator the comparator by which the array is ordered.
             * @return index of the search key, if it is contained in the array
             *         within the specified range;
             *         otherwise, <code>(-(insertion point) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the array: the index of the first
             *         element in the range greater than the key,
             *         or @c toIndex if all
             *         elements in the range are less than the specified key.  Note
             *         that this guarantees that the return value will be >= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the range contains elements that are not
             *         <i>mutually comparable</i> using the specified comparator,
             *         or the search key is not comparable to the
             *         elements in the range using this comparator.
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 or toIndex > a.length() @endcode
             *
             */
            template <class T>
            static gint binarySearch(Array<T> const& a, gint fromIndex, gint toIndex, T const& key,
                                     Comparator<T> const& comparator) {
                try {
                    int low = fromIndex;
                    int high = toIndex - 1;

                    while (low <= high) {
                        int mid = (low + high) >> 1;
                        T midVal = a[mid];
                        int cmp = comparator.compare(midVal, key);
                        if (cmp < 0)
                            low = mid + 1;
                        else if (cmp > 0)
                            high = mid - 1;
                        else
                            return mid; // key found
                    }
                    return -(low + 1); // key not found.
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            // Filling

            /**
             * Assigns the specified byte value to each element of the specified array
             * of bytes.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(ByteArray& a, gbyte val);

            /**
             * Assigns the specified byte value to each element of the specified
             * range of the specified array of bytes.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(ByteArray& a, gint fromIndex, gint toIndex, gbyte val);

            /**
             * Assigns the specified short value to each element of the specified array
             * of shorts.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(ShortArray& a, gshort val);

            /**
             * Assigns the specified short value to each element of the specified
             * range of the specified array of shorts.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(ShortArray& a, gint fromIndex, gint toIndex, gshort val);

            /**
             * Assigns the specified char value to each element of the specified array
             * of chars.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(CharArray& a, gchar val);

            /**
             * Assigns the specified char value to each element of the specified
             * range of the specified array of chars.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(CharArray& a, gint fromIndex, gint toIndex, gchar val);

            /**
             * Assigns the specified int value to each element of the specified array
             * of ints.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(IntArray& a, gint val);

            /**
             * Assigns the specified int value to each element of the specified
             * range of the specified array of ints.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(IntArray& a, gint fromIndex, gint toIndex, gint val);

            /**
             * Assigns the specified long value to each element of the specified array
             * of longs.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(LongArray& a, glong val);

            /**
             * Assigns the specified long value to each element of the specified
             * range of the specified array of longs.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(LongArray& a, gint fromIndex, gint toIndex, glong val);

            /**
             * Assigns the specified float value to each element of the specified array
             * of floats.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(FloatArray& a, gfloat val);

            /**
             * Assigns the specified float value to each element of the specified
             * range of the specified array of floats.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(FloatArray& a, gint fromIndex, gint toIndex, gfloat val);

            /**
             * Assigns the specified double value to each element of the specified
             * array of doubles.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(DoubleArray& a, gdouble val);

            /**
             * Assigns the specified double value to each element of the specified
             * range of the specified array of doubles.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(DoubleArray& a, gint fromIndex, gint toIndex, gdouble val);

            /**
             * Assigns the specified boolean value to each element of the specified
             * array of booleans.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(BooleanArray& a, gbool val);

            /**
             * Assigns the specified boolean value to each element of the specified
             * range of the specified array of booleans.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(BooleanArray& a, gint fromIndex, gint toIndex, gbool val);

            /**
             * Assigns the specified string value to each element of the specified
             * array of strings.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             */
            static void fill(StringArray& a, String const& val);

            /**
             * Assigns the specified string value to each element of the specified
             * range of the specified array of strings.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            static void fill(StringArray& a, gint fromIndex, gint toIndex, String const& val);

            /**
             * Assigns the specified Object reference to each element of the specified
             * array of Objects.
             *
             * @param a the array to be filled
             * @param val the value to be stored in all elements of the array
             * @throws ArrayStoreException if the specified value is not of a
             *         runtime type that can be stored in the specified array
             */
            template <class T, class U = T, ClassOf(1)::OnlyIf<Class<U>::template isExtends<T>()>  = 1>
            static void fill(Array<T>& a, U const& val) {
                try { fill0(a, val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Assigns the specified Object reference to each element of the specified
             * range of the specified array of Objects.  The range to be filled
             * extends from index @c fromIndex, inclusive, to index
             * @c toIndex, exclusive.  (If @code fromIndex == toIndex @endcode, the
             * range to be filled is empty.)
             *
             * @param a the array to be filled
             * @param fromIndex the index of the first element (inclusive) to be
             *        filled with the specified value
             * @param toIndex the index of the last element (exclusive) to be
             *        filled with the specified value
             * @param val the value to be stored in all elements of the array
             * @throws IllegalArgumentException if @code fromIndex > toIndex @endcode
             * @throws IndexOutOfBoundsException if @code fromIndex < 0 @endcode or
             *         @code toIndex > a.length() @endcode
             */
            template <class T, class U = T, ClassOf(1)::OnlyIf<Class<U>::template isExtends<T>()>  = 1>
            static void fill(Array<T>& a, gint fromIndex, gint toIndex, U const& val) {
                try { fill0(a, fromIndex, toIndex, val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            // Cloning

            /**
             * Copies the specified array, truncating or padding with nulls (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain @c null.
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             * The resulting array is of exactly the same class as the original array.
             *
             * @tparam T the class of the objects in the array
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with nulls
             *     to obtain the specified length
             */
            template <class T>
            static Array<T> copyOf(Array<T> const& original, gint newLength) {
                // Create new empty array
                Array<T> newArray;
                // Initialize with newLength and copy all elements (Using copy Helpers)
                copyOf0(original, newArray, newLength, classChecked);
                return newArray;
            }

            /**
             * Copies the specified array, truncating or padding with nulls (if necessary)
             * so the copy has the specified length.  For all indices that are
             * valid in both the original array and the copy, the two arrays will
             * contain identical values.  For any indices that are valid in the
             * copy but not the original, the copy will contain @c null.
             * Such indices will exist if and only if the specified length
             * is greater than that of the original array.
             * The resulting array is of exactly the same class as the original array.
             *
             * @tparam T the class of the objects in the output array
             * @tparam U the class of the objects in the input array
             * @param original the array to be copied
             * @param newLength the length of the copy to be returned
             * @return a copy of the original array, truncated or padded with nulls
             *     to obtain the specified length
             * @throws ClassCastException If The output array is not compatible with
             *          any elements of input array.
             */
            template <class T, class U, ClassOf(1)::OnlyIf<!Class<T>::template isSame<U>()>  = 1>
            static Array<T> copyOf(Array<U> const& original, gint newLength) {
                try {
                    // Create new empty array
                    Array<T> newArray;
                    // Initialize with newLength and copy all elements (Using copy Helpers)
                    (Class<T>::template isSuper<U>())
                        ? copyOf0(original, newArray, newLength, classChecked)
                        : copyOf0(original, newArray, newLength, classChecker<T>);
                    return newArray;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            static ByteArray copyOf(ByteArray const& original, gint newLength);

            template <class T>
            static Array<T> copyOfRange(Array<T> const& original, gint fromIndex, gint toIndex) {
                // Create new empty array
                Array<T> newArray;
                // Initialize with newLength and copy all elements from Range [fromIndex, toIndex[ (Using copy Helpers)
                copyOfRange0(original, fromIndex, toIndex, newArray, classChecked);
                return newArray;
            }

            template <class T, class U, ClassOf(1)::OnlyIf<!Class<T>::template isSame<U>()>  = 1>
            static Array<T> copyOfRange(Array<U> const& original, gint fromIndex, gint toIndex) {
                Array<T> newArray;
                // Initialize with newLength and copy all elements from Range [fromIndex, toIndex[ (Using copy Helpers)
                if (Class<T>::template isSuper<U>())
                    copyOfRange0(original, fromIndex, toIndex, newArray, classChecked);
                else
                    copyOfRange0(original, fromIndex, toIndex, newArray, classChecker<T>);
                return newArray;
            }

            static ByteArray copyOfRange(ByteArray const& original, gint fromIndex, gint toIndex);

            static ShortArray copyOf(ShortArray const& a, gint newLength);

            static ShortArray copyOfRange(ShortArray const& original, gint fromIndex, gint toIndex);

            static CharArray copyOf(CharArray const& a, gint newLength);

            static CharArray copyOfRange(CharArray const& a, gint fromIndex, gint toIndex);

            static IntArray copyOf(IntArray const& a, gint newLength);

            static IntArray copyOfRange(IntArray const& a, gint fromIndex, gint toIndex);

            static LongArray copyOf(LongArray const& a, gint newLength);

            static LongArray copyOfRange(LongArray const& a, gint fromIndex, gint toIndex);

            static FloatArray copyOf(FloatArray const& a, gint newLength);

            static FloatArray copyOfRange(FloatArray const& a, gint fromIndex, gint toIndex);

            static DoubleArray copyOf(DoubleArray const& a, gint newLength);

            static DoubleArray copyOfRange(DoubleArray const& a, gint fromIndex, gint toIndex);

            static StringArray copyOf(StringArray const& a, gint newLength);

            static StringArray copyOfRange(StringArray const& a, gint fromIndex, gint toIndex);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(BooleanArray const& src, gint srcOff, BooleanArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(ByteArray const& src, gint srcOff, ByteArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(ShortArray const& src, gint srcOff, ShortArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(CharArray const& src, gint srcOff, CharArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(IntArray const& src, gint srcOff, IntArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(LongArray const& src, gint srcOff, LongArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(FloatArray const& src, gint srcOff, FloatArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(DoubleArray const& src, gint srcOff, DoubleArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            static void copy(StringArray const& src, gint srcOff, StringArray& dest, gint destOff, gint length);

            /**
             * Copies an array from the specified source array, beginning at the
             * specified position, to the specified position of the destination array.
             * A subsequence of array components are copied from the source array referenced
             * by @c src to the destination array referenced by @c dest. The number of values copied is
             * equal to the @c length argument. The values at positions @c srcOff through @c srcOff+length-1
             * in the source array are copied into positions @c destOff through @c destOff+length-1 ,
             * respectively, of the destination array.
             * <p>
             * If the @c src and @c dest arguments refer to the same array object, then the copying is performed
             * as if the values at positions @c srcOff through @c srcOff+length-1 were first copied to a temporary
             * array with @c length values and then the contents of the temporary array were copied into positions
             * @c destOff through @c destOff+length-1 of the destination array.
             * </p>
             * <p>
             * An @c IndexOutOfBoundsException is thrown and the destination is not modified:
             * - The @c srcOff argument is negative.
             * - The @c destOff argument is negative.
             * - The @c length argument is negative.
             * - @c srcOff+length is greater than @c src.length(), the length of the source array.
             * - @c destOff+length is greater than @c dest.length(), the length of the destination array.
             * </p>
             * In this case, let @c k be the smallest non-negative integer less than
             * length such that @c src[srcOff+k] cannot be converted to the component type of the destination
             * array; when the exception is thrown, source array values from positions @c srcOff through
             * @c srcOff+k-1 will already have been copied to destination array positions @c destOff through
             * @c destOff+k-1 and no other positions of the destination array will have been modified.
             * (Because of the restrictions already itemized, this paragraph effectively applies only to the
             * situation where both arrays have component types that are reference types.)
             *
             * @param      src      the source array.
             * @param      srcOff   starting position in the source array.
             * @param      dest     the destination array.
             * @param      destOff  starting position in the destination data.
             * @param      length   the number of array elements to be copied.
             * @throws     IndexOutOfBoundsException  if copying would cause
             *             access of data outside array bounds.
             */
            template <class T, class U>
            static void copy(Array<T> const& src, gint srcOff, Array<U>& dest, gint destOff, gint length) {
                try {
                    Class<U>::template isSuper<T>()
                        ? copy0(src, srcOff, dest, destOff, length)
                        : copy0(src, srcOff, dest, destOff, length, classChecker<T>);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * A soft maximum array length imposed by array growth computations.
             * For preserve compatibility, we have an implementation limit that will cause
             *
             *     OutOfMemoryError("Requested array size exceeds limit")
             *
             * to be thrown if a request is made to allocate an array of some length near
             * Integer::MAX_VALUE, even if there is sufficient heap available. The soft
             * maximum value is chosen conservatively to be smaller than any implementation
             * limit that is likely to be encountered.
             */
            static CORE_FAST gint SOFT_MAX_ARRAY_LENGTH = (Integer::MAX_VALUE - 8);

            /**
             * Computes a new array length given an array's current length, a minimum growth
             * amount, and a preferred growth amount. The computation is done in an overflow-safe
             * fashion.
             *
             * This method is used by objects that contain an array that might need to be grown
             * in order to fulfill some immediate need (the minimum growth amount) but would also
             * like to request more space (the preferred growth amount) in order to accommodate
             * potential future needs. The returned length is usually clamped at the soft maximum
             * length in order to avoid hitting the JVM implementation limit. However, the soft
             * maximum will be exceeded if the minimum growth amount requires it.
             *
             * If the preferred growth amount is less than the minimum growth amount, the
             * minimum growth amount is used as the preferred growth amount.
             *
             * The preferred length is determined by adding the preferred growth amount to the
             * current length. If the preferred length does not exceed the soft maximum length
             * (SOFT_MAX_ARRAY_LENGTH) then the preferred length is returned.
             *
             * If the preferred length exceeds the soft maximum, we use the minimum growth
             * amount. The minimum required length is determined by adding the minimum growth
             * amount to the current length. If the minimum required length exceeds Integer.MAX_VALUE,
             * then this method throws OutOfMemoryError. Otherwise, this method returns the greater of
             * the soft maximum or the minimum required length.
             *
             * Note that this method does not do any array allocation itself; it only does array
             * length growth computations. However, it will throw OutOfMemoryError as noted above.
             *
             * @param oldLength   current length of the array (must be non-negative)
             * @param minGrowth   minimum required growth amount (must be positive)
             * @param prefGrowth  preferred growth amount
             * @return the new array length
             * @throws OutOfMemoryError if the new length would exceed Integer::MAX_VALUE
             */
            static gint newLength(gint oldLength, gint minGrowth, gint prefGrowth);

            /**
             * Reverses the elements of an array in-place.
             *
             * @tparam T the array component type
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            template <class T>
            static Array<T>& reverse(Array<T>& a) {
                try {
                    reverse0(a, 0, a.length());
                    return a;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Reverses the elements of an array in-place.
             *
             * @tparam T the array component type
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            template <class T>
            static Array<T>&& reverse(Array<T>&& a) {
                try {
                    reverse0(a, 0, a.length());
                    return UNSAFE::moveInstance(a);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Reverses the elements of an array in-place.
             *
             * @tparam T the array component type
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            template <class T>
            static Array<T>& reverse(Array<T>& a, gint fromIndex, gint toIndex) {
                try {
                    reverse0(a, fromIndex, toIndex);
                    return a;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Reverses the elements of an array in-place.
             *
             * @tparam T the array component type
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            template <class T>
            static Array<T>&& reverse(Array<T>&& a, gint fromIndex, gint toIndex) {
                try {
                    reverse0(a, fromIndex, toIndex);
                    return UNSAFE::moveInstance(a);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(BooleanArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(BooleanArray& a, gint fromIndex, gint toIndex);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(ByteArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(ByteArray& a, gint fromIndex, gint toIndex);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(CharArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(CharArray& a, gint fromIndex, gint toIndex);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(ShortArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(ShortArray& a, gint fromIndex, gint toIndex);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(IntArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(IntArray& a, gint fromIndex, gint toIndex);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(FloatArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(FloatArray& a, gint fromIndex, gint toIndex);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(LongArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(LongArray& a, gint fromIndex, gint toIndex);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(DoubleArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(DoubleArray& a, gint fromIndex, gint toIndex);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(StringArray& a);

            /**
             * Reverses the elements of an array in-place.
             *
             * @param a the array to be reversed
             * @param fromIndex the start (inclusive) index
             * @param toIndex the end (exclusive) index
             * @return the reversed array, always the same array as the argument
             */
            static void reverse(StringArray& a, gint fromIndex, gint toIndex);


            // Mismatch methods

            // Mismatch boolean

            /**
             * Finds and returns the index of the first mismatch between two
             * @c boolean arrays, otherwise return -1 if no mismatch is found.  The
             * index will be in the range of 0 (inclusive) up to the length (inclusive)
             * of the smaller array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(BooleanArray const& a, BooleanArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(BooleanArray const& a, gint aFrom, BooleanArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c boolean arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     a[aFrom + pl] != b[bFrom + pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(BooleanArray const& a, gint aFrom, gint aTo,
                                 BooleanArray const& b, gint bFrom, gint bTo);

            // Mismatch byte

            /**
             * Finds and returns the index of the first mismatch between two @c byte
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(ByteArray const& a, ByteArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(ByteArray const& a, gint aFrom, ByteArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c byte arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     a[aFrom + pl] != b[bFrom + pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(ByteArray const& a, gint aFrom, gint aTo,
                                 ByteArray const& b, gint bFrom, gint bTo);

            // Mismatch char

            /**
             * Finds and returns the index of the first mismatch between two @c char
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(CharArray const& a, CharArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(CharArray const& a, gint aFrom, CharArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c char arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     a[aFrom + pl] != b[bFrom + pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(CharArray const& a, gint aFrom, gint aTo,
                                 CharArray const& b, gint bFrom, gint bTo);

            // Mismatch short

            /**
             * Finds and returns the index of the first mismatch between two @c short
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(ShortArray const& a, ShortArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(ShortArray const& a, gint aFrom, ShortArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c short arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     a[aFrom + pl] != b[bFrom + pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(ShortArray const& a, gint aFrom, gint aTo,
                                 ShortArray const& b, gint bFrom, gint bTo);

            // Mismatch int

            /**
             * Finds and returns the index of the first mismatch between two @c int
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(IntArray const& a, IntArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(IntArray const& a, gint aFrom, IntArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c int arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     a[aFrom + pl] != b[bFrom + pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(IntArray const& a, gint aFrom, gint aTo,
                                 IntArray const& b, gint bFrom, gint bTo);

            // Mismatch float

            /**
             * Finds and returns the index of the first mismatch between two @c float
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     Float.compare(a[pl], b[pl]) != 0
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(FloatArray const& a, FloatArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(FloatArray const& a, gint aFrom, FloatArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c float arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     Float.compare(a[aFrom + pl], b[bFrom + pl]) != 0
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(FloatArray const& a, gint aFrom, gint aTo,
                                 FloatArray const& b, gint bFrom, gint bTo);

            // Mismatch long

            /**
             * Finds and returns the index of the first mismatch between two @c long
             * arrays, otherwise return -1 if no mismatch is found.  The index will be
             * in the range of 0 (inclusive) up to the length (inclusive) of the smaller
             * array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     a[pl] != b[pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(LongArray const& a, LongArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(LongArray const& a, gint aFrom, LongArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c long arrays over the specified ranges, otherwise return -1 if no
             * mismatch is found.  The index will be in the range of 0 (inclusive) up to
             * the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     a[aFrom + pl] != b[bFrom + pl]
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(LongArray const& a, gint aFrom, gint aTo,
                                 LongArray const& b, gint bFrom, gint bTo);

            // Mismatch double

            /**
             * Finds and returns the index of the first mismatch between two
             * @c double arrays, otherwise return -1 if no mismatch is found.  The
             * index will be in the range of 0 (inclusive) up to the length (inclusive)
             * of the smaller array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     Double::compare(a[pl], b[pl]) != 0
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(DoubleArray const& a, DoubleArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(DoubleArray const& a, gint aFrom, DoubleArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c double arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     Double::compare(a[aFrom + pl], b[bFrom + pl]) != 0
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(DoubleArray const& a, gint aFrom, gint aTo,
                                 DoubleArray const& b, gint bFrom, gint bTo);

            // Mismatch String

            /**
             * Finds and returns the index of the first mismatch between two
             * @c string arrays, otherwise return -1 if no mismatch is found.  The
             * index will be in the range of 0 (inclusive) up to the length (inclusive)
             * of the smaller array.
             *
             * <p>
             * If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             * </p>
             * <p>
             * Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, b, 0, pl) &&
             *     a[pl].compareTo(b[pl]) != 0
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             * </p>
             * <p>
             * Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, b, 0, Math::min(a.length(), b.length()))
             * @endcode
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            static gint mismatch(StringArray const& a, StringArray const& b);

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            static gint mismatch(StringArray const& a, gint aFrom, StringArray const& b, gint bFrom, gint length);

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c string arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>
             * If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             * </p>
             * <p>
             * Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, b, bFrom, pl) &&
             *     a[aFrom + pl].compareTo(b[bFrom + pl]) != 0
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint mismatch(StringArray const& a, gint aFrom, gint aTo,
                                 StringArray const& b, gint bFrom, gint bTo);

            // Mismatch objects

            /**
             * Finds and returns the index of the first mismatch between two
             * @c Object arrays, otherwise return -1 if no mismatch is found.  The
             * index will be in the range of 0 (inclusive) up to the length (inclusive)
             * of the smaller array.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl) &&
             *     !Objects.equals(a[pl], b[pl])
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            template <class T>
            static gint mismatch(Array<T> const& a, Array<T> const& b) {
                if (&a == &b)
                    return -1;
                gint aLength = a.length();
                gint bLength = b.length();
                gint length = Math::min(aLength, bLength);
                for (gint i = 0; i < length; ++i) {
                    if (a.get0(i) != b.get0(i))
                        return i;
                }
                return aLength != bLength ? length : -1;
            }

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c Object arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl) &&
             *     !Objects.equals(a[aFrom + pl], b[bFrom + pl])
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom))
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            template <class T>
            static gint mismatch(Array<T> const& a, gint aFrom, gint aTo,
                                 Array<T> const& b, gint bFrom, gint bTo) {
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
                    if (a.get0(aFrom + i) != b.get0(bFrom + i))
                        return i;
                }
                return aLength != bLength ? length : -1;
            }

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            template <class T>
            static gint mismatch(Array<T> const& a, gint aFrom,
                                 Array<T> const& b, gint bFrom, gint length) {
                try {
                    misc::Preconditions::checkIndexFromSize(aFrom, length, a.length());
                    misc::Preconditions::checkIndexFromSize(bFrom, length, b.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (&a == &b)
                    return -1;
                for (gint i = 0; i < length; ++i) {
                    if (a.get0(aFrom + i) != b.get0(bFrom + i))
                        return i;
                }
                return -1;
            }

            /**
             * Finds and returns the index of the first mismatch between two
             * @c Object arrays, otherwise return -1 if no mismatch is found.
             * The index will be in the range of 0 (inclusive) up to the length
             * (inclusive) of the smaller array.
             *
             * <p>The specified comparator is used to determine if two array elements
             * from the each array are not equal.
             *
             * <p>If the two arrays share a common prefix then the returned index is the
             * length of the common prefix and it follows that there is a mismatch
             * between the two elements at that index within the respective arrays.
             * If one array is a proper prefix of the other then the returned index is
             * the length of the smaller array and it follows that the index is only
             * valid for the larger array.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(a.length(), b.length()) &&
             *     Arrays.equals(a, 0, pl, b, 0, pl, cmp)
             *     cmp.compare(a[pl], b[pl]) != 0
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b, share a proper
             * prefix if the following expression is true:
             * @code
             *     a.length() != b.length() &&
             *     Arrays.equals(a, 0, Math::min(a.length(), b.length()),
             *                   b, 0, Math::min(a.length(), b.length()),
             *                   cmp)
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param b the second array to be tested for a mismatch
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return the index of the first mismatch between the two arrays,
             *         otherwise @c -1.
             */
            template <class T, class S = T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<S>()>  = 1>
            static gint mismatch(Array<T> const& a, Array<T> const& b, Comparator<T> const& cmp) {
                if (&a == &b)
                    return -1;
                gint aLength = a.length();
                gint bLength = b.length();
                gint length = Math::min(aLength, bLength);
                gbool supportNull = Class<S>::hasInstance(null);
                try {
                    for (gint i = 0; i < length; ++i) {
                        Object const& av = a.get0(i);
                        Object const& bv = b.get0(i);
                        if (av != bv) {
                            if (!supportNull && (av == null || bv == null))
                                return i;
                            gint r = cmp.compare(CORE_XCAST(S const, av), CORE_XCAST(S const, bv));
                            if (r != 0)
                                return i;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return aLength != bLength ? length : -1;
            }

            /**
             * Finds and returns the relative index of the first mismatch between two
             * @c Object arrays over the specified ranges, otherwise return -1 if
             * no mismatch is found.  The index will be in the range of 0 (inclusive) up
             * to the length (inclusive) of the smaller range.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the returned relative index is the length of the common prefix and
             * it follows that there is a mismatch between the two elements at that
             * relative index within the respective arrays.
             * If one array is a proper prefix of the other, over the specified ranges,
             * then the returned relative index is the length of the smaller range and
             * it follows that the relative index is only valid for the array with the
             * larger range.
             * Otherwise, there is no mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a common
             * prefix of length @c pl if the following expression is true:
             * @code
             *     pl >= 0 &&
             *     pl < Math::min(aTo - aFrom, bTo - bFrom) &&
             *     Arrays.equals(a, aFrom, aFrom + pl, b, bFrom, bFrom + pl, cmp) &&
             *     cmp.compare(a[aFrom + pl], b[bFrom + pl]) != 0
             * @endcode
             * Note that a common prefix length of @c 0 indicates that the first
             * elements from each array mismatch.
             *
             * <p>Two arrays, @c a and @c b with specified
             * ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively, share a proper
             * prefix if the following expression is true:
             * @code
             *     (aTo - aFrom) != (bTo - bFrom) &&
             *     Arrays.equals(a, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   b, 0, Math::min(aTo - aFrom, bTo - bFrom),
             *                   cmp)
             * @endcode
             *
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return the relative index of the first mismatch between the two arrays
             *         over the specified ranges, otherwise @c -1.
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws ArrayIndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            template <class T, class S = T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<S>()>  = 1>
            static gint mismatch(Array<T> const& a, gint aFrom, gint aTo,
                                 Array<T> const& b, gint bFrom, gint bTo, Comparator<T> const& cmp) {
                try {
                    misc::Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                    misc::Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                gint aLength = aTo - aFrom;
                gint bLength = bTo - bFrom;
                gint length = Math::min(aLength, bLength);
                gbool supportNull = Class<S>::hasInstance(null);
                try {
                    for (gint i = 0; i < length; ++i) {
                        Object const& av = a.get0(aFrom + i);
                        Object const& bv = b.get0(bFrom + i);
                        if (av != bv) {
                            if (!supportNull && (av == null || bv == null))
                                return i;
                            gint r = cmp.compare(CORE_XCAST(S const, av), CORE_XCAST(S const, bv));
                            if (r != 0)
                                return i;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return aLength != bLength ? length : -1;
            }

            /**
             * Find the relative index of a mismatch between two arrays starting from
             * given indexes.
             *
             * <p>
             * This method does not perform bounds checks. It is the responsibility
             * of the caller to perform such bounds checks before calling this method.
             * </p>
             * @param a the first array to be tested for a mismatch
             * @param aFrom the index of the first element (inclusive) in the first
             * array to be compared
             * @param b the second array to be tested for a mismatch
             * @param bFrom the index of the first element (inclusive) in the
             * second array to be compared
             * @param length the number of bytes from each array to check
             * @return the relative index of a mismatch between the two arrays,
             * otherwise -1 if no mismatch.  The index will be within the range of
             * (inclusive) 0 to (exclusive) the smaller of the two array bounds.
             */
            template <class T, class S = T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<S>()>  = 1>
            static gint mismatch(Array<T> const& a, gint aFrom,
                                 Array<T> const& b, gint bFrom,
                                 gint length, Comparator<T> const& cmp) {
                try {
                    misc::Preconditions::checkIndexFromRange(aFrom, length, a.length());
                    misc::Preconditions::checkIndexFromRange(bFrom, length, b.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                gbool supportNull = Class<S>::hasInstance(null);
                try {
                    for (gint i = 0; i < length; ++i) {
                        Object const& av = a.get0(aFrom + i);
                        Object const& bv = b.get0(bFrom + i);
                        if (av != bv) {
                            if (!supportNull && (av == null || bv == null))
                                return i;
                            gint r = cmp.compare(CORE_XCAST(S const, av), CORE_XCAST(S const, bv));
                            if (r != 0)
                                return i;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return -1;
            }

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c boolean arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Boolean
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(BooleanArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c byte arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Byte
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(ByteArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c char arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Character
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(CharArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c short arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Short
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(ShortArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two non-null @c int arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Integer
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(IntArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c float arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Float
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(FloatArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c long arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Long
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(LongArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c double arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Double
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(DoubleArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c string arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Double
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            static gint hash(StringArray const& a);

            /**
             * Returns a hash code based on the contents of the specified array.
             * For any two @c object arrays @c a and @c b
             * such that @c Arrays::equals(a, b), it is also the case that
             * @c Arrays::hash(a) == Arrays::hash(b).
             *
             * <p>
             * The value returned by this method is the same value that would be
             * obtained by invoking the @b hash
             * method on a @b List containing a sequence of @b Double
             * instances representing the elements of @c a in the same order.
             * </p>
             * @param a the array whose hash value to compute
             * @return a content-based hash code for @c a
             */
            template <class T>
            static gint hash(Array<T> const& a) {
                gint result = 0;
                gint length = a.length();
                for (gint i = 0; i < length; ++i) {
                    result = result * 31 + a.getOrNull(i).hash();
                }
                return result;
            }

            /**
             * Set all elements of the specified array, using the provided
             * generator function to compute each element.
             *
             * <p>
             * If the generator function throws an exception, it is relayed to
             * the caller and the array is left in an indeterminate state.
             * </p>
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            static void setAll(IntArray& array, function::IntUnaryOperator const& generator);

            /**
             * Set all elements of the specified array, using the provided
             * generator function to compute each element.
             *
             * <p>
             * If the generator function throws an exception, it is relayed to
             * the caller and the array is left in an indeterminate state.
             * </p>
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            static void setAll(LongArray& array, function::LongUnaryOperator const& generator);

            /**
             * Set all elements of the specified array, using the provided
             * generator function to compute each element.
             *
             * <p>
             * If the generator function throws an exception, it is relayed to
             * the caller and the array is left in an indeterminate state.
             * </p>
             * @param array array to be initialized
             * @param generator a function accepting an index and producing the desired
             *        value for that position
             */
            static void setAll(DoubleArray& array, function::DoubleUnaryOperator const& generator);


            // Comparison methods

            // Compare boolean

            /**
             * Compares two @c boolean arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Boolean::compare(gbool, gbool) @endcode, at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See <b> #mismatch(BooleanArray, BooleanArray)</b> for the definition of a
             * common and proper prefix.)
             *
             * <p>The comparison is consistent with @b equals},
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Boolean.compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(BooleanArray const& a, BooleanArray const& b);

            /**
             * Compares two @c boolean arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by @b Boolean::compare(gbool, gbool) @endcode, at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b> mismatch(BooleanArray, int, int, BooleanArray, int, int) </b> for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Boolean.compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint compare(BooleanArray const& a, gint aFrom, gint aTo,
                                BooleanArray const& b, gint bFrom, gint bTo);

            // Compare byte

            /**
             * Compares two @c byte arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b> Byte::compare(gbyte, gbyte)</b>, at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(ByteArray, ByteArray) for the definition of a common and
             * proper prefix.)
             *
             * <p>The comparison is consistent with @b equals,
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Byte.compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(ByteArray const& a, ByteArray const& b);

            /**
             * Compares two @c byte arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b> Byte::compare(gbyte, gbyte)</b>, at a relative index
             * within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b> mismatch(ByteArray, gint, gint, ByteArray, gint, gint)</b> for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Byte.compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint compare(ByteArray const& a, gint aFrom, gint aTo,
                                ByteArray const& b, gint bFrom, gint bTo);

            /**
             * Compares two @c byte arrays lexicographically, numerically treating
             * elements as unsigned.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Byte::compareUnsigned(gbyte, gbyte), at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(ByteArray, ByteArray) for the definition of a common
             * and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Byte.compareUnsigned(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are
             *         equal and contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compareUnsigned(ByteArray const& a, ByteArray const& b);


            /**
             * Compares two @c byte arrays lexicographically over the specified
             * ranges, numerically treating elements as unsigned.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b> Byte::compareUnsigned(gbyte, gbyte)</b>, at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b> mismatch(ByteArray, gint, gint, ByteArray, gint, gint)</b> for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Byte.compareUnsigned(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             * @throws NullPointerException
             *         if either array is null
             */
            static gint compareUnsigned(ByteArray const& a, gint aFrom, gint aTo,
                                        ByteArray const& b, gint bFrom, gint bTo);

            // Compare char

            /**
             * Compares two @c char arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * <b> Character::compare(gchar, gchar) </b>, at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(CharArray, CharArray) for the definition of a common and
             * proper prefix.)
             *
             * <p>The comparison is consistent with @b equals,
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Character.compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(CharArray const& a, CharArray const& b);

            /**
             * Compares two @c char arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b> Character::compare(gchar, gchar) </b>, at a relative
             * index within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See <b> mismatch(CharArray, int, int, CharArray, int, int)</b> for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Character.compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint compare(CharArray const& a, gint aFrom, gint aTo,
                                CharArray const& b, gint bFrom, gint bTo);

            // Compare short

            /**
             * Compares two @c short arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Short::compare(gshort, gshort)</b>, at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(ShortArray, ShortArray) for the definition of a common
             * and proper prefix.)
             *
             * <p>The comparison is consistent with @b equals,
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Short.compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(ShortArray const& a, ShortArray const& b);

            /**
             * Compares two @c short arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by <b> Short::compare(gshort, gshort)</b>, at a relative
             * index within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(ShortArray, int, int, ShortArray, int, int) for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Short.compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint compare(ShortArray const& a, gint aFrom, gint aTo,
                                ShortArray const& b, gint bFrom, gint bTo);

            /**
             * Compares two @c short arrays lexicographically, numerically treating
             * elements as unsigned.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Short::compareUnsigned(short, short), at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(ShortArray, ShortArray) for the definition of a common
             * and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Short.compareUnsigned(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are
             *         equal and contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compareUnsigned(ShortArray const& a, ShortArray const& b);

            /**
             * Compares two @c short arrays lexicographically over the specified
             * ranges, numerically treating elements as unsigned.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by @b Short::compareUnsigned(short, short), at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(ShortArray, int, int, ShortArray, int, int) for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Short.compareUnsigned(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             * @throws NullPointerException
             *         if either array is null
             */
            static gint compareUnsigned(ShortArray const& a, gint aFrom, gint aTo,
                                        ShortArray const& b, gint bFrom, gint bTo);

            // Compare int

            /**
             * Compares two @c int arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Integer::compare(int, int), at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(IntArray, IntArray) for the definition of a common and
             * proper prefix.)
             *
             * <p>The comparison is consistent with @b equals,
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Integer.compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(IntArray const& a, IntArray const& b);

            /**
             * Compares two @c int arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by @b Integer::compare(int, int), at a relative index
             * within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(IntArray, int, int, IntArray, int, int) for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Integer.compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint compare(IntArray const& a, gint aFrom, gint aTo,
                                IntArray const& b, gint bFrom, gint bTo);

            /**
             * Compares two @c int arrays lexicographically, numerically treating
             * elements as unsigned.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Integer::compareUnsigned(int, int), at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(IntArray, IntArray) for the definition of a common
             * and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Integer.compareUnsigned(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are
             *         equal and contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compareUnsigned(IntArray const& a, IntArray const& b);

            /**
             * Compares two @c int arrays lexicographically over the specified
             * ranges, numerically treating elements as unsigned.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by @b Integer::compareUnsigned(int, int), at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(IntArray, int, int, IntArray, int, int) for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Integer.compareUnsigned(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             * @throws NullPointerException
             *         if either array is null
             */
            static gint compareUnsigned(IntArray const& a, gint aFrom, gint aTo,
                                        IntArray const& b, gint bFrom, gint bTo);

            // Compare float

            /**
             * Compares two @c float arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Float::compare(float, float), at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(FloatArray, FloatArray) for the definition of a common
             * and proper prefix.)
             *
             * <p>The comparison is consistent with @b equals,
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Float.compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(FloatArray const& a, FloatArray const& b);

            /**
             * Compares two @c float arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by @b Float::compare(float, float), at a relative
             * index within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(FloatArray, int, int, FloatArray, int, int) for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Float.compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint compare(FloatArray const& a, gint aFrom, gint aTo,
                                FloatArray const& b, gint bFrom, gint bTo);

            // Compare long

            /**
             * Compares two @c long arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Long::compare(long, long), at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(LongArray, LongArray) for the definition of a common and
             * proper prefix.)
             *
             * <p>The comparison is consistent with @b equals,
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Long.compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(LongArray const& a, LongArray const& b);

            /**
             * Compares two @c long arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by @b Long::compare(long, long), at a relative index
             * within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(LongArray, int, int, LongArray, int, int) for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Long.compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint compare(LongArray const& a, gint aFrom, gint aTo,
                                LongArray const& b, gint bFrom, gint bTo);

            /**
             * Compares two @c long arrays lexicographically, numerically treating
             * elements as unsigned.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Long::compareUnsigned(long, long), at an index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(LongArray, LongArray) for the definition of a common
             * and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Long.compareUnsigned(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are
             *         equal and contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compareUnsigned(LongArray const& a, LongArray const& b);

            /**
             * Compares two @c long arrays lexicographically over the specified
             * ranges, numerically treating elements as unsigned.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by @b Long::compareUnsigned(long, long), at a
             * relative index within the respective arrays that is the length of the
             * prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(LongArray, int, int, LongArray, int, int) for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Long.compareUnsigned(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             * @throws NullPointerException
             *         if either array is null
             */
            static gint compareUnsigned(LongArray const& a, gint aFrom, gint aTo,
                                        LongArray const& b, gint bFrom, gint bTo);

            // Compare double

            /**
             * Compares two @c double arrays lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements, as if by
             * @b Double::compare(double, double), at an index within the respective
             * arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(DoubleArray, DoubleArray) for the definition of a common
             * and proper prefix.)
             *
             * <p>The comparison is consistent with @b equals,
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return Double::compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            static gint compare(DoubleArray const& a, DoubleArray const& b);

            /**
             * Compares two @c double arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements, as if by @b Double::compare(double, double), at a relative
             * index within the respective arrays that is the length of the prefix.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(DoubleArray, int, int, DoubleArray, int, int) for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if:
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return Double::compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gint compare(DoubleArray const& a, gint aFrom, gint aTo,
                                DoubleArray const& b, gint bFrom, gint bTo);

            // Compare objects

            /**
             * Compares two @c Object arrays, within comparable elements,
             * lexicographically.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing two elements of type @c T at
             * an index @c i within the respective arrays that is the prefix
             * length, as if by:
             * @code
             *     Comparator::nullsFirst(Comparator<T>::naturalOrder()).
             *         compare(a[i], b[i])
             * @endcode
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(Array<?>, Array<?>) for the definition of a common
             * and proper prefix.)
             *
             * <p>The comparison is consistent with @b equals,
             * more specifically the following holds for arrays @c a and @c b:
             * @code
             *     Arrays.equals(a, b) == (Arrays.compare(a, b) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return a[i].compareTo(b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @tparam T the type of comparable array elements
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            template <class T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<Comparable<T>>()>  = 1>
            static gint compare(Array<T> const& a, Array<T> const& b) {
                if (&a == &b)
                    return true;
                gint aLength = a.length();
                gint bLength = b.length();
                gint length = Math::min(aLength, bLength);
                try {
                    for (gint i = 0; i < length; ++i) {
                        Object const& av = a.get0(i);
                        Object const& bv = b.get0(i);
                        if (&av != &bv) {
                            if (av == null)
                                return -1;
                            if (bv == null)
                                return 1;
                            gint r = CORE_XCAST(Comparable<T> const, av).compareTo(CORE_XCAST(T const, bv));
                            if (r != 0)
                                return r;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return 0;
            }

            /**
             * Compares two @c Object arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing two
             * elements of type @c T at a relative index @c i within the
             * respective arrays that is the prefix length, as if by:
             * @code
             *     Comparator::nullsFirst(Comparator<T>::naturalOrder()).
             *         compare(a[aFrom + i, b[bFrom + i])
             * @endcode
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(Array<?>, int, int, Array<?>, int, int) for the
             * definition of a common and proper prefix.)
             *
             * <p>The comparison is consistent with
             * @b equals, more
             * specifically the following holds for arrays @c a and @c b with
             * specified ranges [@c aFrom, @c aTo) and
             * [@c bFrom, @c bTo) respectively:
             * @code
             *     Arrays.equals(a, aFrom, aTo, b, bFrom, bTo) ==
             *         (Arrays.compare(a, aFrom, aTo, b, bFrom, bTo) == 0)
             * @endcode
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return a[aFrom + i].compareTo(b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @tparam T the type of comparable array elements
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            template <class T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<Comparable<T>>()>  = 1>
            static gint compare(Array<T> const& a, gint aFrom, gint aTo,
                                Array<T> const& b, gint bFrom, gint bTo) {
                try {
                    misc::Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                    misc::Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                gint aLength = aTo - aFrom;
                gint bLength = bTo - bFrom;
                gint length = Math::min(aLength, bLength);
                try {
                    for (gint i = 0; i < length; ++i) {
                        Object const& av = a.get0(aFrom + i);
                        Object const& bv = b.get0(bFrom + i);
                        if (&av != &bv) {
                            if (av == null)
                                return -1;
                            if (bv == null)
                                return 1;
                            gint r = CORE_XCAST(Comparable<T> const, av).compareTo(CORE_XCAST(T const, bv));
                            if (r != 0)
                                return r;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return 0;
            }

            /**
             * Compares two @c Object arrays lexicographically using a specified
             * comparator.
             *
             * <p>If the two arrays share a common prefix then the lexicographic
             * comparison is the result of comparing with the specified comparator two
             * elements at an index within the respective arrays that is the prefix
             * length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two array lengths.
             * (See @b mismatch(Array<?>, Array<?>) for the definition of a common
             * and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, b, cmp);
             *     if (i >= 0 && i < Math::min(a.length(), b.length()))
             *         return cmp.compare(a[i], b[i]);
             *     return a.length() - b.length;
             * @endcode
             *
             * @param a the first array to compare
             * @param b the second array to compare
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return the value @c 0 if the first and second array are equal and
             *         contain the same elements in the same order;
             *         a value less than @c 0 if the first array is
             *         lexicographically less than the second array; and
             *         a value greater than @c 0 if the first array is
             *         lexicographically greater than the second array
             */
            template <class T, class S = T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<S>()>  = 1>
            static gint compare(Array<T> const& a, Array<T> const& b, Comparator<S> const& cmp) {
                gint aLength = a.length();
                gint bLength = b.length();
                gint length = Math::min(aLength, bLength);
                gbool supportNull = Class<S>::hasInstance(null);
                try {
                    for (gint i = 0; i < length; ++i) {
                        Object const& av = a.get0(i);
                        Object const& bv = b.get0(i);
                        if (&av != &bv) {
                            if (!supportNull) {
                                if (av == null)
                                    return -1;

                                if (bv == null)
                                    return 1;
                            }
                            gint r = cmp.compare(CORE_XCAST(S const, av), CORE_XCAST(S const, bv));
                            if (r != 0)
                                return r;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return 0;
            }

            /**
             * Compares two @c Object arrays lexicographically over the specified
             * ranges.
             *
             * <p>If the two arrays, over the specified ranges, share a common prefix
             * then the lexicographic comparison is the result of comparing with the
             * specified comparator two elements at a relative index within the
             * respective arrays that is the prefix length.
             * Otherwise, one array is a proper prefix of the other and, lexicographic
             * comparison is the result of comparing the two range lengths.
             * (See @b mismatch(Array<?>, int, int, Array<?>, int, int) for the
             * definition of a common and proper prefix.)
             *
             * @apiNote
             * <p>This method behaves as if :
             * @code
             *     int i = Arrays.mismatch(a, aFrom, aTo,
             *                             b, bFrom, bTo, cmp);
             *     if (i >= 0 && i < Math::min(aTo - aFrom, bTo - bFrom))
             *         return cmp.compare(a[aFrom + i], b[bFrom + i]);
             *     return (aTo - aFrom) - (bTo - bFrom);
             * @endcode
             *
             * @param a the first array to compare
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be compared
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be compared
             * @param b the second array to compare
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be compared
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be compared
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return the value @c 0 if, over the specified ranges, the first and
             *         second array are equal and contain the same elements in the same
             *         order;
             *         a value less than @c 0 if, over the specified ranges, the
             *         first array is lexicographically less than the second array; and
             *         a value greater than @c 0 if, over the specified ranges, the
             *         first array is lexicographically greater than the second array
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            template <class T, class S = T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<S>()>  = 1>
            static gint compare(Array<T> const& a, gint aFrom, gint aTo,
                                Array<T> const& b, gint bFrom, gint bTo, Comparator<S> const& cmp) {
                try {
                    misc::Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                    misc::Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                gint aLength = aTo - aFrom;
                gint bLength = bTo - bFrom;
                gint length = Math::min(aLength, bLength);
                gbool supportNull = Class<S>::hasInstance(null);
                try {
                    for (gint i = 0; i < length; ++i) {
                        Object const& av = a.get0(aFrom + i);
                        Object const& bv = b.get0(bFrom + i);
                        if (&av != &bv) {
                            if (!supportNull) {
                                if (av == null)
                                    return -1;
                                if (bv == null)
                                    return 1;
                            }
                            gint r = cmp.compare(CORE_XCAST(S const, av), CORE_XCAST(S const, bv));
                            if (r != 0)
                                return r;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return 0;
            }

            // Equality Testing

            /**
             * Returns @c true if the two specified arrays of booleans are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             */
            static gbool equals(BooleanArray const& a, BooleanArray const& a2);

            /**
             * Returns true if the two specified arrays of booleans, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gbool equals(BooleanArray const& a, gint aFrom, gint aTo,
                                BooleanArray const& b, gint bFrom, gint bTo);

            /**
             * Returns @c true if the two specified arrays of bytes are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             */
            static gbool equals(ByteArray const& a, ByteArray const& a2);

            /**
             * Returns true if the two specified arrays of bytes, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gbool equals(ByteArray const& a, gint aFrom, gint aTo,
                                ByteArray const& b, gint bFrom, gint bTo);

            /**
             * Returns @c true if the two specified arrays of chars are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             */
            static gbool equals(CharArray const& a, CharArray const& a2);

            /**
             * Returns true if the two specified arrays of chars, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gbool equals(CharArray const& a, gint aFrom, gint aTo,
                                CharArray const& b, gint bFrom, gint bTo);

            /**
             * Returns @c true if the two specified arrays of shorts are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             */
            static gbool equals(ShortArray const& a, ShortArray const& a2);

            /**
             * Returns true if the two specified arrays of shorts, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gbool equals(ShortArray const& a, gint aFrom, gint aTo,
                                ShortArray const& b, gint bFrom, gint bTo);

            /**
             * Returns @c true if the two specified arrays of ints are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             */
            static gbool equals(IntArray const& a, IntArray const& a2);

            /**
             * Returns true if the two specified arrays of ints, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gbool equals(IntArray const& a, gint aFrom, gint aTo,
                                IntArray const& b, gint bFrom, gint bTo);

            /**
             * Returns @c true if the two specified arrays of floats are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.
             *
             * Two floats @c f1 and @c f2 are considered equal if:
             * <pre>    @code Float::valueOf(f1).equals(Float.valueOf(f2))@endcode
             * (Unlike the @c == operator, this method considers
             * @c NaN equal to itself, and 0.0f unequal to -0.0f.)
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             * @see Float#equals(Object)
             */
            static gbool equals(FloatArray const& a, FloatArray const& a2);

            /**
             * Returns true if the two specified arrays of floats, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * <p>Two floats @c f1 and @c f2 are considered equal if:
             * <pre>    @code Float::valueOf(f1).equals(Float.valueOf(f2))@endcode
             * (Unlike the @c == operator, this method considers
             * @c NaN equal to itself, and 0.0f unequal to -0.0f.)
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gbool equals(FloatArray const& a, gint aFrom, gint aTo,
                                FloatArray const& b, gint bFrom, gint bTo);

            /**
             * Returns @c true if the two specified arrays of longs are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             */
            static gbool equals(LongArray const& a, LongArray const& a2);

            /**
             * Returns true if the two specified arrays of longs, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            static gbool equals(LongArray const& a, gint aFrom, gint aTo,
                                LongArray const& b, gint bFrom, gint bTo);

            /**
             * Returns @c true if the two specified arrays of doubles are
             * <i>equal</i> to one another.  Two arrays are considered equal if both
             * arrays contain the same number of elements, and all corresponding pairs
             * of elements in the two arrays are equal.  In other words, two arrays
             * are equal if they contain the same elements in the same order.
             *
             * Two doubles @c d1 and @c d2 are considered equal if:
             * <pre>    @code Double::valueOf(d1).equals(Double::valueOf(d2))@endcode
             * (Unlike the @c == operator, this method considers
             * @c NaN equal to itself, and 0.0d unequal to -0.0d.)
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             * @see Double::equals(Object)
             */
            static gbool equals(DoubleArray const& a, DoubleArray const& a2);

            /**
             * Returns true if the two specified arrays of doubles, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * <p>Two doubles @c d1 and @c d2 are considered equal if:
             * <pre>    @code Double::valueOf(d1).equals(Double::valueOf(d2)) @endcode
             * (Unlike the @c == operator, this method considers
             * @c NaN equal to itself, and 0.0d unequal to -0.0d.)
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             * @see Double::equals(Object)
             */
            static gbool equals(DoubleArray const& a, gint aFrom, gint aTo,
                                DoubleArray const& b, gint bFrom, gint bTo);

            /**
             * Returns @c true if the two specified arrays of Objects are
             * <i>equal</i> to one another.  The two arrays are considered equal if
             * both arrays contain the same number of elements, and all corresponding
             * pairs of elements in the two arrays are equal.  Two objects @c e1
             * and @c e2 are considered <i>equal</i> if
             * @c Objects::equals(e1, e2).
             * In other words, the two arrays are equal if
             * they contain the same elements in the same order.
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @return @c true if the two arrays are equal
             */
            template <class T>
            static gbool equals(Array<T> const& a, Array<T> const& a2) {
                if (&a == &a2)
                    return true;
                gint aLength = a.length();
                if (aLength != a2.length())
                    return false;
                for (int i = 0; i < aLength; ++i) {
                    if (a.get0(i) != a2.get0(i))
                        return false;
                }
                return true;
            }

            /**
             * Returns true if the two specified arrays of Objects, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * <p>Two objects @c e1 and @c e2 are considered <i>equal</i> if
             * @c Objects::equals(e1, e2).
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            template <class T>
            static gbool equals(Array<T> const& a, gint aFrom, gint aTo,
                                Array<T> const& b, gint bFrom, gint bTo) {
                try {
                    misc::Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                    misc::Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                gint aLength = aTo - aFrom;
                gint bLength = bTo - bFrom;
                if (aLength != bLength)
                    return false;
                for (int i = 0; i < aLength; ++i) {
                    if (a.get0(aFrom + i) != b.get0(bFrom + i))
                        return false;
                }
                return true;
            }

            /**
             * Returns @c true if the two specified arrays of Objects are
             * <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if both arrays contain the same number
             * of elements, and all corresponding pairs of elements in the two arrays
             * are equal.  In other words, the two arrays are equal if they contain the
             * same elements in the same order.
             *
             * <p>Two objects @c e1 and @c e2 are considered <i>equal</i> if,
             * given the specified comparator, @code cmp.compare(e1, e2) == 0 @endcode.
             *
             * @param a one array to be tested for equality
             * @param a2 the other array to be tested for equality
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return @c true if the two arrays are equal
             */
            template <class T, class S = T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<S>()>  = 1>
            static gbool equals(Array<T> const& a, Array<T> const& a2, Comparator<S> const& cmp) {
                if (&a == &a2)
                    return true;
                gint aLength = a.length();
                if (a.length() != a2.length())
                    return false;
                gbool supportNull = Class<S>::hasInstance(null);
                for (int i = 0; i < aLength; ++i) {
                    Object const& av = a.get0(i);
                    Object const& bv = a2.get0(i);
                    if (&av != &bv) {
                        if (!supportNull && (av == null || bv == null))
                            return false;
                        gint r = cmp.compare(CORE_XCAST(S const, av), CORE_XCAST(S const, bv));
                        if (r != 0)
                            return false;
                    }
                }
                return true;
            }

            /**
             * Returns true if the two specified arrays of Objects, over the specified
             * ranges, are <i>equal</i> to one another.
             *
             * <p>Two arrays are considered equal if the number of elements covered by
             * each range is the same, and all corresponding pairs of elements over the
             * specified ranges in the two arrays are equal.  In other words, two arrays
             * are equal if they contain, over the specified ranges, the same elements
             * in the same order.
             *
             * <p>Two objects @c e1 and @c e2 are considered <i>equal</i> if,
             * given the specified comparator, @code cmp.compare(e1, e2) == 0 @endcode.
             *
             * @param a the first array to be tested for equality
             * @param aFrom the index (inclusive) of the first element in the
             *                   first array to be tested
             * @param aTo the index (exclusive) of the last element in the
             *                 first array to be tested
             * @param b the second array to be tested for equality
             * @param bFrom the index (inclusive) of the first element in the
             *                   second array to be tested
             * @param bTo the index (exclusive) of the last element in the
             *                 second array to be tested
             * @param cmp the comparator to compare array elements
             * @tparam T the type of array elements
             * @return @c true if the two arrays, over the specified ranges, are
             *         equal
             * @throws IllegalArgumentException
             *         if @code aFrom > aTo @endcode or
             *         if @code bFrom > bTo @endcode
             * @throws IndexOutOfBoundsException
             *         if @code aFrom < 0 or aTo > a.length() @endcode or
             *         if @code bFrom < 0 or bTo > b.length() @endcode
             */
            template <class T, class S = T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<S>()>  = 1>
            static gbool equals(Array<T> const& a, gint aFrom, gint aTo,
                                Array<T> const& b, gint bFrom, gint bTo, Comparator<S> const& cmp) {
                try {
                    misc::Preconditions::checkIndexFromRange(aFrom, aTo, a.length());
                    misc::Preconditions::checkIndexFromRange(bFrom, bTo, b.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                gint aLength = aTo - aFrom;
                gint bLength = bTo - bFrom;
                if (aLength != bLength)
                    return false;
                gbool supportNull = Class<S>::hasInstance(null);
                for (int i = 0; i < aLength; ++i) {
                    Object const& av = a.get0(aFrom + i);
                    Object const& bv = b.get0(bFrom + i);
                    if (&av != &bv) {
                        if (!supportNull && (av == null || bv == null))
                            return false;
                        gint r = cmp.compare(CORE_XCAST(S const, av), CORE_XCAST(S const, bv));
                        if (r != 0)
                            return false;
                    }
                }
                return true;
            }

            /**
             * Set the reference on specified array without unity verification.
             *
             * @param a the array reference
             * @param index the index
             * @param x the value to set at specified index
             * @return the old value at specified index
             * @throws IndexOutOfBoundsException if given index out of bounds.
             */
            template <class T, class E = T, ClassOf(1)::OnlyIf<Class<E>::template isExtends<T>()>  = 1>
            static Object& fastSet(Array<T>& a, gint index, E& x) {
                CORE_FAST_ASSERT(!Class<E>::isConstant());
                CORE_FAST_ASSERT(!Class<E>::isRvalue());
                try {
                    misc::Preconditions::checkIndex(index, a.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return fastSet0(a, index, x);
            }

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(misc::ObjectArray const& a);

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             * This method is used for create direct memory access buffers.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(BooleanArray const& a);

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             * This method is used for create direct memory access buffers.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(ByteArray const& a);

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             * This method is used for create direct memory access buffers.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(ShortArray const& a);

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             * This method is used for create direct memory access buffers.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(CharArray const& a);

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             * This method is used for create direct memory access buffers.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(IntArray const& a);

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             * This method is used for create direct memory access buffers.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(FloatArray const& a);

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             * This method is used for create direct memory access buffers.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(LongArray const& a);

            /**
             * Convert specified array instance to C-pointer for direct memory access.
             * This method is used for create direct memory access buffers.
             *
             * @param a the target array
             * @return the address of first value on specified object.
             */
            static glong array2DirectAccess(DoubleArray const& a);

        private:
            static void reverse0(Object& a, gint fromIndex, gint toIndex);

            /**
             * Find the relative index of the first mismatching pair of elements in two
             * primitive arrays of the same component type.  Pairs of elements will be
             * tested in order relative to given offsets into both arrays.
             *
             * <p>
             * This method does not perform type checks or bounds checks.  It is the
             * responsibility of the caller to perform such checks before calling this
             * method.
             * </p>
             * <p>
             * The given offsets, in bytes, need not be aligned according to the
             * given log<sub>2</sub> size the array elements.  More specifically, an
             * offset modulus the size need not be zero.
             * </p>
             * @param a the first array to be tested for mismatch, or @c null for
             * direct memory access
             * @param aOffset the relative offset, in bytes, from the base address of
             * the first array to test from, otherwise if the first array is
             * @c null, an absolute address pointing to the first element to test.
             * @param b the second array to be tested for mismatch, or @c null for
             * direct memory access
             * @param bOffset the relative offset, in bytes, from the base address of
             * the second array to test from, otherwise if the second array is
             * @c null, an absolute address pointing to the first element to test.
             * @param length the number of array elements to test
             * @param log2ArrayIndexScale log<sub>2</sub> of the array index scale, that
             * corresponds to the size, in bytes, of an array element.
             * @return if a mismatch is found a relative index, between 0 (inclusive)
             * and @c length (exclusive), of the first mismatching pair of elements
             * in the two arrays.  Otherwise, if a mismatch is not found the bitwise
             * compliment of the number of remaining pairs of elements to be checked in
             * the tail of the two arrays.
             */
            static gint vectorizedMismatch(Object const& a, glong aOffset,
                                           Object const& b, glong bOffset,
                                           gint length,
                                           gint log2ArrayIndexScale);

            static Object& fastSet0(misc::ObjectArray& a, gint index, Object& x);

            static void copy0(Object const& src, gint srcOff, Object& dest, gint destOff, gint length);

            static void copy0(Object const& src, gint srcOff,
                              Object& dest, gint destOff,
                              gint length, void (*validator)(Object const& o));

            static void fill0(Object& a, Object const& val);

            static void fill0(Object& a, gint fromIndex, gint toIndex, Object const& val);

            CORE_ALIAS(Validator, Class<void(Object const &)>::Pointer);

            static void copyOf0(Object const& src, Object& dest, gint newLength, Validator validator);

            static void copyOfRange0(Object const& src, gint fromIndex, gint toIndex, Object& dest,
                                     Validator validator);

            template <class T>
            static void classChecker(Object const& o) {
                if (!Class<T>::hasInstance(o))
                    ClassCastException().throws($ftrace());
            }

            static void classChecked(Object const& o);

            template <class T>
            class NaturalOrder final : public Comparator<> {
            public:
                gint compare(const Object& t1, const Object& t2) const CORE_NOTHROW override {
                    Comparable<T> const& c = CORE_XCAST(Comparable<T> const, t1);
                    T const& t = CORE_XCAST(T const, t2);
                    return c.compareTo(t);
                }
            };

            template <class T>
            static void legacyMergeSort(Array<T>& a, gint fromIndex, gint toIndex) {
                Array<T> copy = copyOfRange(a, fromIndex, toIndex);
                mergeSort(copy, a, fromIndex, toIndex, -fromIndex);
            }

            static void legacyMergeSort(StringArray& a, gint fromIndex, gint toIndex);

            template <class T>
            static void legacyMergeSort(Array<T>& a, gint fromIndex, gint toIndex, Comparator<T> const& comparator) {
                Array<T> copy = copyOfRange(a, fromIndex, toIndex);
                if (Class<Comparable<T>>::template isSuper<T>() && (comparator == Comparator<T>::naturalOrder()) ||
                    Class<NaturalOrder<T>>::hasInstance(comparator))
                    mergeSort(copy, a, fromIndex, toIndex, -fromIndex);
                else
                    mergeSort(copy, a, fromIndex, toIndex, -fromIndex, comparator);
            }

            /**
             * Tuning parameter: list size at or below which insertion sort will be
             * used in preference to mergesort.
             * To be removed in a future release.
             */
            static CORE_FAST gint INSERTIONSORT_THRESHOLD = 7;

            /**
             * Src is the source array that starts at index 0
             * Dest is the (possibly larger) array destination with a possible offset
             * low is the index in dest to start sorting
             * high is the end index in dest to end sorting
             * off is the offset to generate corresponding low, high in src
             * To be removed in a future release.
             */
            template <class T>
            static void mergeSort(Array<T>& src, Array<T>& dest, gint low, gint high, gint offset) {
                gint count = high - low;

                // Insertion sort on smallest arrays
                if (count < INSERTIONSORT_THRESHOLD) {
                    for (int i = low; i < high; i++) {
                        for (gint j = i;
                             j > low && CORE_XCAST(Comparable<T> const, dest[j - 1]).compareTo(dest[j]) > 0; j
                             --) {
                            swap(dest, j, j - 1);
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
                if (CORE_XCAST(Comparable<T> const, src[mid - 1]).compareTo(src[mid]) <= 0) {
                    for (int i = 0; i < count; ++i) {
                        dest.value[i + destLow] = src.value[i + low];
                    }
                    return;
                }

                // Merge sorted halves (now in src) into dest
                for (int i = destLow, p = low, q = mid; i < destHigh; i++) {
                    if (q >= high || p < mid && CORE_XCAST(Comparable<T> const, src[p]).compareTo(src[q]) <= 0) {
                        dest.value[i] = src.value[p++];
                    } else {
                        dest.value[i] = src.value[q++];
                    }
                }
            }

            static void mergeSort(StringArray& src, StringArray& dest, gint low, gint high, gint offset);

            template <class T>
            static void mergeSort(Array<T>& src, Array<T>& dest, gint low, gint high, gint offset,
                                  Comparator<T> const& comparator) {
                gint count = high - low;

                // Insertion sort on smallest arrays
                if (count < INSERTIONSORT_THRESHOLD) {
                    for (int i = low; i < high; i++) {
                        for (gint j = i; j > low && comparator.compare(dest[j - 1], dest[j]) > 0; j--) {
                            swap(dest, j, j - 1);
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
                mergeSort(dest, src, low, mid, -offset, comparator);
                mergeSort(dest, src, mid, high, -offset, comparator);

                // If list is already sorted, just copy from src to dest.  This is an
                // optimization that results in faster sorts for nearly ordered lists.
                if (comparator.compare(src[mid - 1], src[mid]) <= 0) {
                    for (int i = 0; i < count; ++i) {
                        dest.value[i + destLow] = src.value[i + low];
                    }
                    return;
                }

                // Merge sorted halves (now in src) into dest
                for (int i = destLow, p = low, q = mid; i < destHigh; i++) {
                    if (q >= high || p < mid && comparator.compare(src[p], src[q]) <= 0) {
                        dest.value[i] = src.value[p++];
                    } else {
                        dest.value[i] = src.value[q++];
                    }
                }
            }

            static gbool equals(misc::ObjectArray const& a, misc::ObjectArray const& b);

            static gbool equals(misc::ObjectArray const& a, gint aFrom, gint aTo,
                                misc::ObjectArray const& b, gint bFrom, gint bTo);

            static void swap0(misc::ObjectArray& a, gint i, gint j);
        };
    } // utils
} // core


#endif //CORE24_ARRAYS_H
