//
// Created by admin on 24/12/24.
//

#ifndef CORE24_COLLECTIONS_H
#define CORE24_COLLECTIONS_H

#include <core/random/Random.h>
#include <core/util/List.h>
#include <core/util/Deque.h>
#include <core/util/NavigableMap.h>
#include <core/util/NavigableSet.h>
#include <core/util/Optional.h>

namespace core {
    namespace util {
        /**
         * This class consists exclusively of static methods that operate on or return
         * collections.  It contains polymorphic algorithms that operate on
         * collections, "wrappers", which return a new collection backed by a
         * specified collection, and a few other odds and ends.
         * <p>
         * The documentation for the polymorphic algorithms contained in this class
         * generally includes a brief description of the <i>implementation</i>.  Such
         * descriptions should be regarded as <i>implementation notes</i>, rather than
         * parts of the <i>specification</i>.  Implementors should feel free to
         * substitute other algorithms, so long as the specification itself is adhered
         * to.  (For example, the algorithm used by @c sort does not have to be
         * a mergesort, but it does have to be <i>stable</i>.)
         * </p>
         * <p>
         * The "destructive" algorithms contained in this class, that is, the
         * algorithms that modify the collection on which they operate, are specified
         * to throw @c UnsupportedOperationException if the collection does not
         * support the appropriate mutation primitive(s), such as the @c set
         * method.  These algorithms may, but are not required to, throw this
         * exception if an invocation would have no effect on the collection.  For
         * example, invoking the @c sort method on an unmodifiable list that is
         * already sorted may or may not throw @c UnsupportedOperationException.
         * </p>
         *
         * @see     Collection
         * @see     Set
         * @see     List
         * @see     Map
         */
        class Collections final : public Object {
            // Suppresses default constructor, ensuring non-instantiability.
            Collections() = default;

            // Algorithms

            /*
             * Tuning parameters for algorithms - Many of the List algorithms have
             * two implementations, one of which is appropriate for RandomAccess
             * lists, the other for "sequential."  Often, the random access variant
             * yields better performance on small sequential access lists.  The
             * tuning parameters below determine the cutoff point for what constitutes
             * a "small" sequential access list for each algorithm.  The values below
             * were empirically determined to work well for LinkedList. Hopefully
             * they should be reasonable for other sequential access List
             * implementations.  Those doing performance work on this code would
             * do well to validate the values of these parameters from time to time.
             * (The first word of each tuning parameter name is the algorithm to which
             * it applies.)
            */
            static CORE_FAST int BINARY_SEARCH_THRESHOLD = 5000;
            static CORE_FAST int REVERSE_THRESHOLD = 18;
            static CORE_FAST int SHUFFLE_THRESHOLD = 5;
            static CORE_FAST int FILL_THRESHOLD = 25;
            static CORE_FAST int ROTATE_THRESHOLD = 100;
            static CORE_FAST int COPY_THRESHOLD = 10;
            static CORE_FAST int REPLACE_ALL_THRESHOLD = 11;
            static CORE_FAST int INDEX_OF_SUBLIST_THRESHOLD = 35;

            static Optional<random::Random> rnd;

        public:
            /**
             * Sorts the specified list into ascending order, according to the
             * <em> Comparable natural ordering</em> of its elements.
             * All elements in the list must implement the @b Comparable
             * interface.  Furthermore, all elements in the list must be
             * <i>mutually comparable</i> (that is, @c e1.compareTo(e2)
             * must not throw a @c ClassCastException for any elements
             * @c e1 and @c e2 in the list).
             *
             * <p>
             * This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             * </p>
             * <p>
             * The specified list must be modifiable, but need not be resizable.
             * </p>
             * @tparam  T the class of the objects in the list
             * @param  list the list to be sorted.
             * @throws ClassCastException if the list contains elements that are not
             *         <i>mutually comparable</i> (for example, strings and integers).
             * @throws UnsupportedOperationException if the specified list's
             *         list-iterator does not support the @c set operation.
             * @throws IllegalArgumentException (optional) if the implementation
             *         detects that the natural ordering of the list elements is
             *         found to violate the @b Comparable contract
             * @see List::sort(Comparator)
             */
            template <class T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<Comparable<T>>()>  = 1>
            static void sort(List<T>& list) {
                try { list.sort(Comparator<T>::naturalOrder()); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Sorts the specified list according to the order induced by the
             * specified comparator.  All elements in the list must be <i>mutually
             * comparable</i> using the specified comparator (that is,
             * @c c.compare(e1, e2) must not throw a @c ClassCastException
             * for any elements @c e1 and @c e2 in the list).
             *
             * <p>
             * This sort is guaranteed to be <i>stable</i>:  equal elements will
             * not be reordered as a result of the sort.
             * </p>
             * <p>
             * The specified list must be modifiable, but need not be resizable.
             * </p>
             * @note
             * This implementation defers to the @b List::sort(Comparator)
             * method using the specified list and comparator.
             *
             * @tparam  T the class of the objects in the list
             * @param  list the list to be sorted.
             * @param  c the comparator to determine the order of the list.
             * @throws ClassCastException if the list contains elements that are not
             *         <i>mutually comparable</i> using the specified comparator.
             * @throws UnsupportedOperationException if the specified list's
             *         list-iterator does not support the @c set operation.
             * @throws IllegalArgumentException (optional) if the comparator is
             *         found to violate the @b Comparator contract
             * @see List::sort(Comparator)
             */
            template <class T>
            static void sort(List<T>& list, Comparator<T> const& c) {
                try { list.sort(c); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }


            /**
             * Searches the specified list for the specified object using the binary
             * search algorithm.  The list must be sorted into ascending order
             * according to the <em> Comparable natural ordering</em> of its
             * elements (as by the @b sort(List) method) prior to making this
             * call.  If it is not sorted, the results are undefined.  If the list
             * contains multiple elements equal to the specified object, there is no
             * guarantee which one will be found.
             *
             * <p>
             * This method runs in log(n) time for a "random access" list (which
             * provides near-constant-time positional access).  If the specified list
             * does not implement the @b RandomAccess interface and is large,
             * this method will do an iterator-based binary search that performs
             * O(n) link traversals and O(log n) element comparisons.
             * </p>
             * @tparam  T the class of the objects in the list
             * @param  list the list to be searched.
             * @param  key the key to be searched for.
             * @return the index of the search key, if it is contained in the list;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the list: the index of the first
             *         element greater than the key, or @c list.size() if all
             *         elements in the list are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the list contains elements that are not
             *         <i>mutually comparable</i> (for example, strings and
             *         integers), or the search key is not mutually comparable
             *         with the elements of the list.
             */
            template <class T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<Comparable<T>>()>  = 1>
            static gint binarySearch(List<T> const& list, T const& key) {
                try {
                    if (Class<RandomAccess>::hasInstance(list) || list.size() <= BINARY_SEARCH_THRESHOLD)
                        return binarySearch0(list, key);
                    return binarySearchWithIterator(list, key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Searches the specified list for the specified object using the binary
             * search algorithm.  The list must be sorted into ascending order
             * according to the specified comparator (as by the @b sort(List, Comparator)
             * method), prior to making this call.  If it is
             * not sorted, the results are undefined.  If the list contains multiple
             * elements equal to the specified object, there is no guarantee which one
             * will be found.
             *
             * <p>
             * This method runs in log(n) time for a "random access" list (which
             * provides near-constant-time positional access).  If the specified list
             * does not implement the @b RandomAccess interface and is large,
             * this method will do an iterator-based binary search that performs
             * O(n) link traversals and O(log n) element comparisons.
             * </p>
             * @tparam  T the class of the objects in the list
             * @param  list the list to be searched.
             * @param  key the key to be searched for.
             * @param  c the comparator by which the list is ordered.
             * @return the index of the search key, if it is contained in the list;
             *         otherwise, <code>(-(<i>insertion point</i>) - 1)</code>.  The
             *         <i>insertion point</i> is defined as the point at which the
             *         key would be inserted into the list: the index of the first
             *         element greater than the key, or @c list.size() if all
             *         elements in the list are less than the specified key.  Note
             *         that this guarantees that the return value will be &gt;= 0 if
             *         and only if the key is found.
             * @throws ClassCastException if the list contains elements that are not
             *         <i>mutually comparable</i> using the specified comparator,
             *         or the search key is not mutually comparable with the
             *         elements of the list using this comparator.
             */
            template <class T>
            static gint binarySearch(List<T> const& list, T const& key, Comparator<T> const& c) {
                try {
                    if (Class<RandomAccess>::hasInstance(list) || list.size() <= BINARY_SEARCH_THRESHOLD)
                        return binarySearch0(list, key, c);
                    return binarySearchWithIterator(list, key, c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Reverses the order of the elements in the specified list.
             * <p>
             * This method runs in linear time.
             * </p>
             * @note
             * This method mutates the specified list in-place. To obtain a
             * reverse-ordered view of a list without mutating it, use the
             * @b List::reversed method.
             *
             * @param  list the list whose elements are to be reversed.
             * @throws UnsupportedOperationException if the specified list or
             *         its list-iterator does not support the @c set operation.
             * @see    List::reversed
             */
            template <class T>
            static void reverse(List<T>& list) {
                gint size = list.size();
                if (size < REVERSE_THRESHOLD || Class<RandomAccess>::hasInstance(list)) {
                    for (gint i = 0, j = size; i < size >> 1;)
                        try { swap(list, i++, --j); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                } else {
                    // instead of using a raw type here, it's possible to capture
                    // the wildcard, but it will require a call to a supplementary
                    // private method
                    ListIterator<T>& it1 = list.listIterator();
                    ListIterator<T>& it2 = list.listIterator(size);
                    for (gint i = 0; i < size >> 1; i++) {
                        try {
                            T& tmp = it1.next();
                            it1.set(it2.previous());
                            it2.set(tmp);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                    UNSAFE::deleteRegInstance(it1);
                    UNSAFE::deleteRegInstance(it2);
                }
            }

            /**
             * Randomly permutes the specified list using a default source of
             * randomness.  All permutations occur with approximately equal
             * likelihood.
             *
             * <p>
             * The hedge "approximately" is used in the foregoing description because
             * default source of randomness is only approximately an unbiased source
             * of independently chosen bits. If it were a perfect source of randomly
             * chosen bits, then the algorithm would choose permutations with perfect
             * uniformity.
             * </p>
             * <p>
             * This implementation traverses the list backwards, from the last
             * element up to the second, repeatedly swapping a randomly selected element
             * into the "current position".  Elements are randomly selected from the
             * portion of the list that runs from the first element to the current
             * position, inclusive.
             * </p>
             * @note This method runs in linear time.  If the specified list does
             * not implement the @b RandomAccess interface and is large, this
             * implementation dumps the specified list into an array before shuffling
             * it, and dumps the shuffled array back into the list.  This avoids the
             * quadratic behavior that would result from shuffling a "sequential
             * access" list in place.
             *
             * @param  list the list to be shuffled.
             * @throws UnsupportedOperationException if the specified list or
             *         its list-iterator does not support the @c set operation.
             */
            template <class T>
            static void shuffle(List<T>& list) {
                if (!rnd.isPresent())
                    rnd = UNSAFE::newInstance<random::Random>();
                random::Random& rng = rnd.get();
                try { shuffle(list, rng); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Randomly permute the specified list using the specified source of
             * randomness.
             * <p>
             * This method is equivalent to @b shuffle(List, RandomGenerator)
             * and exists for backward compatibility. The @b shuffle(List, RandomGenerator)
             * method is preferred, as it is not limited to random generators
             * that extend the @b Random class.
             * </p>
             * @param  list the list to be shuffled.
             * @param  rnd the source of randomness to use to shuffle the list.
             * @throws UnsupportedOperationException if the specified list or its
             *         list-iterator does not support the @c set operation.
             */
            template <class T>
            static void shuffle(List<T>& list, random::Random& rnd) {
                try {
                    shuffle(list, CORE_XCAST(random::RandomGenerator const, rnd));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Randomly permute the specified list using the specified source of
             * randomness.  All permutations occur with equal likelihood
             * assuming that the source of randomness is fair.
             * <p>
             * This implementation traverses the list backwards, from the last element
             * up to the second, repeatedly swapping a randomly selected element into
             * the "current position".  Elements are randomly selected from the
             * portion of the list that runs from the first element to the current
             * position, inclusive.
             * </p>
             * @note This method runs in linear time.  If the specified list does
             * not implement the @b RandomAccess interface and is large, this
             * implementation dumps the specified list into an array before shuffling
             * it, and dumps the shuffled array back into the list.  This avoids the
             * quadratic behavior that would result from shuffling a "sequential
             * access" list in place.
             *
             * @param  list the list to be shuffled.
             * @param  rng the source of randomness to use to shuffle the list.
             * @throws UnsupportedOperationException if the specified list or its
             *         list-iterator does not support the @c set operation.
             */
            template <class T>
            static void shuffle(List<T>& list, random::RandomGenerator& rng) {
                gint size = list.size();
                if (size < SHUFFLE_THRESHOLD || Class<RandomAccess>::hasInstance(list)) {
                    for (gint i = size; i > 1; --i)
                        try { swap(list, i - 1, rng.nextInt(i)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                } else {
                    Array<T> a = list.toArray();

                    // Shuffle array
                    for (gint i = size; i > 1; --i)
                        try { swap(a, i - 1, rng.nextInt(i)); } catch (Throwable const& ex) { ex.throws($ftrace()); }

                    // Dump array back into list
                    // instead of using a raw type here, it's possible to capture
                    // the wildcard, but it will require a call to a supplementary
                    // private method
                    ListIterator<T>& it = list.listIterator();
                    for (T& tmp : a) {
                        try {
                            it.next();
                            it.set(tmp);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                    UNSAFE::deleteRegInstance(it);
                }
            }

            /**
             * Swaps the elements at the specified positions in the specified list.
             * (If the specified positions are equal, invoking this method leaves
             * the list unchanged.)
             *
             * @param list The list in which to swap elements.
             * @param i the index of one element to be swapped.
             * @param j the index of the other element to be swapped.
             * @throws IndexOutOfBoundsException if either @c i or @c j
             *         is out of range (i &lt; 0 || i &gt;= list.size()
             *         || j &lt; 0 || j &gt;= list.size()).
             */
            template <class T>
            static void swap(List<T>& list, gint i, gint j) {
                // instead of using a raw type here, it's possible to capture
                // the wildcard, but it will require a call to a supplementary
                // private method
                try { list.set(i, list.set(j, list.get(i))); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Replaces all of the elements of the specified list with the specified
             * element.
             * <p>
             * This method runs in linear time.
             * </p>
             * @tparam  T the class of the objects in the list
             * @param  list the list to be filled with the specified element.
             * @param  obj The element with which to fill the specified list.
             * @throws UnsupportedOperationException if the specified list or its
             *         list-iterator does not support the @c set operation.
             */
            template <class T>
            static void fill(List<T>& list, T const& obj) {
                gint size = list.size();

                if (size < FILL_THRESHOLD || Class<RandomAccess>::hasInstance(list)) {
                    for (gint i = 0; i < size; i++)
                        try { list.set(i, obj); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                } else {
                    ListIterator<T>& it = list.listIterator();
                    for (gint i = 0; i < size; i++) {
                        try {
                            it.next();
                            it.set(obj);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                    UNSAFE::deleteRegInstance(it);
                }
            }

            /**
             * Copies all of the elements from one list into another.  After the
             * operation, the index of each copied element in the destination list
             * will be identical to its index in the source list.  The destination
             * list's size must be greater than or equal to the source list's size.
             * If it is greater, the remaining elements in the destination list are
             * unaffected.
             * <p>
             * This method runs in linear time.
             * </p>
             * @tparam  T the class of the objects in the lists
             * @param  dest The destination list.
             * @param  src The source list.
             * @throws IndexOutOfBoundsException if the destination list is too small
             *         to contain the entire source List.
             * @throws UnsupportedOperationException if the destination list's
             *         list-iterator does not support the @c set operation.
             */
            template <class T, class U = T, ClassOf(1)::OnlyIf<Class<U>::template isExtends<T>()>  = 1>
            static void copy(List<T>& dest, List<U> const& src) {
                gint srcSize = src.size();
                gint destSize = dest.size();
                if (srcSize > destSize)
                    IndexOutOfBoundsException("Source does not fit in dest").throws($ftrace());
                if (srcSize < COPY_THRESHOLD ||
                    Class<RandomAccess>::hasInstance(src) && Class<RandomAccess>::hasInstance(dest)) {
                    for (gint i = 0; i < srcSize; i++)
                        try { dest.set(i, src.get(i)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                } else {
                    ListIterator<U>& it1 = src.listIterator();
                    ListIterator<T>& it2 = dest.listIterator();
                    for (gint i = 0; i < srcSize; i++) {
                        try {
                            it2.next();
                            it2.set(it1.next());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                    UNSAFE::deleteRegInstance(it1);
                    UNSAFE::deleteRegInstance(it2);
                }
            }

            /**
             * Returns the minimum element of the given collection, according to the
             * <i>natural ordering</i> of its elements.  All elements in the
             * collection must implement the @c Comparable interface.
             * Furthermore, all elements in the collection must be <i>mutually
             * comparable</i> (that is, @c e1.compareTo(e2) must not throw a
             * @c ClassCastException for any elements @c e1 and
             * @c e2 in the collection).
             * <p>
             * This method iterates over the entire collection, hence it requires
             * time proportional to the size of the collection.
             * </p>
             * @tparam  T the class of the objects in the collection
             * @param  coll the collection whose minimum element is to be determined.
             * @return the minimum element of the given collection, according
             *         to the <i>natural ordering</i> of its elements.
             * @throws ClassCastException if the collection contains elements that are
             *         not <i>mutually comparable</i> (for example, strings and
             *         integers).
             * @throws NoSuchElementException if the collection is empty.
             * @see Comparable
             */
            template <class T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<Comparable<T>>()>  = 0>
            static T const& min(Collection<T> const& coll) {
                Iterator2<T>& it = coll.iterator();
                try {
                    Optional<T> candidate = it.next();
                    while (it.hasNext()) {
                        T const& tmp = it.next();
                        if (tmp.compareTo(candidate.get()) < 0)
                            candidate = tmp;
                    }
                    return candidate.get();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the minimum element of the given collection, according to the
             * order induced by the specified comparator.  All elements in the
             * collection must be <i>mutually comparable</i> by the specified
             * comparator (that is, @c comp.compare(e1, e2) must not throw a
             * @c ClassCastException for any elements @c e1 and
             * @c e2 in the collection).
             * <p>
             * This method iterates over the entire collection, hence it requires
             * time proportional to the size of the collection.
             * </p>
             * @tparam  T the class of the objects in the collection
             * @param  coll the collection whose minimum element is to be determined.
             * @param  comp the comparator with which to determine the minimum element.
             * @return the minimum element of the given collection, according
             *         to the specified comparator.
             * @throws ClassCastException if the collection contains elements that are
             *         not <i>mutually comparable</i> using the specified comparator.
             * @throws NoSuchElementException if the collection is empty.
             * @see Comparable
             */
            template <class T>
            static T const& min(Collection<T> const& coll, Comparator<T> const& comp) {
                Iterator2<T>& it = coll.iterator();
                try {
                    Optional<T> candidate = it.next();
                    while (it.hasNext()) {
                        T const& tmp = it.next();
                        if (comp.compare(tmp, candidate.get()) < 0)
                            candidate = tmp;
                    }
                    return candidate.get();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the maximum element of the given collection, according to the
             * <i>natural ordering</i> of its elements.  All elements in the
             * collection must implement the @c Comparable interface.
             * Furthermore, all elements in the collection must be <i>mutually
             * comparable</i> (that is, @c e1.compareTo(e2) must not throw a
             * @c ClassCastException for any elements @c e1 and
             * @c e2 in the collection).
             * <p>
             * This method iterates over the entire collection, hence it requires
             * time proportional to the size of the collection.
             * </p>
             * @tparam  T the class of the objects in the collection
             * @param  coll the collection whose maximum element is to be determined.
             * @return the maximum element of the given collection, according
             *         to the <i>natural ordering</i> of its elements.
             * @throws ClassCastException if the collection contains elements that are
             *         not <i>mutually comparable</i> (for example, strings and
             *         integers).
             * @throws NoSuchElementException if the collection is empty.
             * @see Comparable
             */
            template <class T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<Comparable<T>>()>  = 0>
            static T const& max(Collection<T> const& coll) {
                Iterator2<T>& it = coll.iterator();
                try {
                    Optional<T> candidate = it.next();
                    while (it.hasNext()) {
                        T const& tmp = it.next();
                        if (tmp.compareTo(candidate.get()) > 0)
                            candidate = tmp;
                    }
                    return candidate.get();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the maximum element of the given collection, according to the
             * order induced by the specified comparator.  All elements in the
             * collection must be <i>mutually comparable</i> by the specified
             * comparator (that is, @c comp.compare(e1, e2) must not throw a
             * @c ClassCastException for any elements @c e1 and
             * @c e2 in the collection).
             * <p>
             * This method iterates over the entire collection, hence it requires
             * time proportional to the size of the collection.
             * </p>
             * @tparam  T the class of the objects in the collection
             * @param  coll the collection whose maximum element is to be determined.
             * @param  comp the comparator with which to determine the maximum element.
             * @return the maximum element of the given collection, according
             *         to the specified comparator.
             * @throws ClassCastException if the collection contains elements that are
             *         not <i>mutually comparable</i> using the specified comparator.
             * @throws NoSuchElementException if the collection is empty.
             * @see Comparable
             */
            template <class T>
            static T const& max(Collection<T> const& coll, Comparator<T> const& comp) {
                Iterator2<T>& it = coll.iterator();
                try {
                    Optional<T> candidate = it.next();
                    while (it.hasNext()) {
                        T const& tmp = it.next();
                        if (comp.compare(tmp, candidate.get()) > 0)
                            candidate = tmp;
                    }
                    return candidate.get();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Rotates the elements in the specified list by the specified distance.
             * After calling this method, the element at index @c i will be
             * the element previously at index @c (i - distance) mod
             * @c list.size(), for all values of @c i between @c 0
             * and @c list.size()-1, inclusive.  (This method has no effect on
             * the size of the list.)
             *
             * <p>
             * For example, suppose @c list comprises@c  [t, a, n, k, s].
             * After invoking @c Collections.rotate(list, 1) (or
             * @c Collections.rotate(list, -4)), @c list will comprise
             * @c [s, t, a, n, k].
             * </p>
             * <p>
             * Note that this method can usefully be applied to sublists to
             * move one or more elements within a list while preserving the
             * order of the remaining elements.  For example, the following idiom
             * moves the element at index @c j forward to position
             * @c k (which must be greater than or equal to @c j):
             * @code
             *     Collections.rotate(list.subList(j, k+1), -1);
             * @endcode
             * To make this concrete, suppose @c list comprises
             * @c [a, b, c, d, e].  To move the element at index @c 1
             * (@c b) forward two positions, perform the following invocation:
             * @code
             *     Collections.rotate(l.subList(1, 4), -1);
             * @endcode
             * The resulting list is @c [a, c, d, b, e].
             * </p>
             * <p>
             * To move more than one element forward, increase the absolute value
             * of the rotation distance.  To move elements backward, use a positive
             * shift distance.
             * </p>
             * <p>
             * If the specified list is small or implements the
             * @b RandomAccess interface, this implementation exchanges the first
             * element into the location it should go, and then repeatedly exchanges
             * the displaced element into the location it should go until a displaced
             * element is swapped into the first element.  If necessary, the process
             * is repeated on the second and successive elements, until the rotation
             * is complete.  If the specified list is large and doesn't implement the
             * @c RandomAccess interface, this implementation breaks the
             * list into two sublist views around index @c -distance mod size.
             * Then the @b reverse(List) method is invoked on each sublist view,
             * and finally it is invoked on the entire list.  For a more complete
             * description of both algorithms, see Section 2.3 of Jon Bentley's
             * <i>Programming Pearls</i> (Addison-Wesley, 1986).
             * </p>
             * @param list the list to be rotated.
             * @param distance the distance to rotate the list.  There are no
             *        constraints on this value; it may be zero, negative, or
             *        greater than @c list.size().
             * @throws UnsupportedOperationException if the specified list or
             *         its list-iterator does not support the @c set operation.
             */
            template <class T>
            static void rotate(List<T>& list, gint distance) {
                try {
                    if (list.size() < ROTATE_THRESHOLD || Class<RandomAccess>::hasInstance(list))
                        rotate1(list, distance);
                    else
                        rotate2(list, distance);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Replaces all occurrences of one specified value in a list with another.
             * More formally, replaces with @c newVal each element @c e
             * in @c list such that @c (oldVal.equals(e)).
             * (This method has no effect on the size of the list.)
             *
             * @tparam  T the class of the objects in the list
             * @param list the list in which replacement is to occur.
             * @param oldVal the old value to be replaced.
             * @param newVal the new value with which @c oldVal is to be
             *        replaced.
             * @return @c true if @c list contained one or more elements
             *         @c e such that @c (oldVal.equals(e)).
             * @throws UnsupportedOperationException if the specified list or
             *         its list-iterator does not support the @c set operation.
             */
            template <class T>
            static gbool replaceAll(List<T>& list, T const& oldVal, T const& newVal) {
                gbool result = false;
                gint size = list.size();
                if (size < REPLACE_ALL_THRESHOLD || Class<RandomAccess>::hasInstance(list)) {
                    for (gint i = 0; i < size; i++) {
                        if (oldVal == list.get(i)) {
                            list.set(i, newVal);
                            result = true;
                        }
                    }
                } else {
                    ListIterator<T>& it = list.listIterator();
                    for (gint i = 0; i < size; i++) {
                        try {
                            if (oldVal == it.next()) {
                                it.set(newVal);
                                result = true;
                            }
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                    UNSAFE::deleteRegInstance(it);
                }
                return result;
            }

            /**
             * Returns the starting position of the first occurrence of the specified
             * target list within the specified source list, or -1 if there is no
             * such occurrence.  More formally, returns the lowest index @c i
             * such that @c source.subList(i, i+target.size()).equals(target),
             * or -1 if there is no such index.  (Returns -1 if
             * @c target.size() > source.size())
             *
             * <p>
             * This implementation uses the "brute force" technique of scanning
             * over the source list, looking for a match with the target at each
             * location in turn.
             * </p>
             * @param source the list in which to search for the first occurrence
             *        of @c target.
             * @param target the list to search for as a subList of @c source.
             * @return the starting position of the first occurrence of the specified
             *         target list within the specified source list, or -1 if there
             *         is no such occurrence.
             */
            template <class T, class U = T, ClassOf(1)::OnlyIf<Class<U>::template isExtends<T>()>  = 1>
            static gint indexOfSubList(List<T> const& source, List<U> const& target) {
                gint sourceSize = source.size();
                gint targetSize = target.size();
                gint maxCandidate = sourceSize - targetSize;
                if (sourceSize < INDEX_OF_SUBLIST_THRESHOLD ||
                    Class<RandomAccess>::hasInstance(source) && Class<RandomAccess>::hasInstance(target)) {
                NEXT_CANDIDATE:
                    for (gint candidate = 0; candidate <= maxCandidate; candidate++) {
                        for (gint i = 0, j = candidate; i < targetSize; i++, j++) {
                            if (source.get(i) == target.get(j))
                                goto NEXT_CANDIDATE; // Element mismatch, try next cand
                        }
                        return candidate; // All elements of candidate matched target
                    }
                } else {
                    ListIterator2<T>& it1 = source.listIterator();
                NEXT_IT_CANDIDATE:
                    for (gint candidate = 0; candidate <= maxCandidate; candidate++) {
                        ListIterator2<U> it2 = target.listIterator();
                        for (gint i = 0; i < targetSize; i++) {
                            try {
                                if (it1.next() == it2.next()) {
                                    // Back up source iterator to next candidate
                                    for (gint j = 0; j < i; j++, i++)
                                        it1.previous();
                                    UNSAFE::deleteRegInstance(it2);
                                    goto NEXT_IT_CANDIDATE;
                                }
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }
                        UNSAFE::deleteRegInstance(it1);
                        UNSAFE::deleteRegInstance(it2);
                        return candidate;
                    }
                }
                return -1;
            }

            /**
             * Returns the starting position of the last occurrence of the specified
             * target list within the specified source list, or -1 if there is no such
             * occurrence.  More formally, returns the highest index @c i
             * such that @c source.subList(i, i+target.size()).equals(target),
             * or -1 if there is no such index.  (Returns -1 if
             * @c target.size() > source.size())
             *
             * <p>
             * This implementation uses the "brute force" technique of iterating
             * over the source list, looking for a match with the target at each
             * location in turn.
             * </p>
             * @param source the list in which to search for the last occurrence
             *        of @c target.
             * @param target the list to search for as a subList of @c source.
             * @return the starting position of the last occurrence of the specified
             *         target list within the specified source list, or -1 if there
             *         is no such occurrence.
             */
            template <class T, class U = T, ClassOf(1)::OnlyIf<Class<U>::template isExtends<T>()>  = 1>
            static gint lastIndexOfSubList(List<T> const& source, List<U> const& target) {
                gint sourceSize = source.size();
                gint targetSize = target.size();
                gint maxCandidate = sourceSize - targetSize;
                if (sourceSize < INDEX_OF_SUBLIST_THRESHOLD ||
                    Class<RandomAccess>::hasInstance(source) && Class<RandomAccess>::hasInstance(target)) {
                NEXT_CANDIDATE:
                    for (gint candidate = maxCandidate; candidate >= 0; --candidate) {
                        for (gint i = 0, j = candidate; i < targetSize; i++, ++j) {
                            if (source.get(i) == target.get(j))
                                goto NEXT_CANDIDATE; // Element mismatch, try next cand
                        }
                        return candidate; // All elements of candidate matched target
                    }
                } else {
                    if (maxCandidate < 0)
                        return -1;
                    ListIterator2<T>& it1 = source.listIterator(maxCandidate);
                NEXT_IT_CANDIDATE:
                    for (gint candidate = maxCandidate; candidate >= 0; --candidate) {
                        ListIterator2<U> it2 = target.listIterator();
                        for (gint i = 0; i < targetSize; i++) {
                            if (it1.next() == it2.next()) {
                                // Back up source iterator to next candidate
                                for (gint j = 0; j <= i + 1; j++, i++)
                                    it1.previous();
                                UNSAFE::deleteRegInstance(it2);
                                goto NEXT_IT_CANDIDATE;
                            }
                        }
                        UNSAFE::deleteRegInstance(it1);
                        UNSAFE::deleteRegInstance(it2);
                        return candidate;
                    }
                }
                return -1;
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified collection. Query operations on the returned collection "read through"
             * to the specified collection, and attempts to modify the returned
             * collection, whether direct or via its iterator, result in an
             * @c UnsupportedOperationException.
             * <p>
             * The returned collection does <i>not</i> pass the hashCode and equals
             * operations through to the backing collection, but relies on
             * @c Object's @c equals and @c hash methods.  This
             * is necessary to preserve the contracts of these operations in the case
             * that the backing collection is a set or a list.
             * </p>
             * <p>
             * The returned collection will be serializable if the specified collection
             * is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam  T the class of the objects in the collection
             * @param  c the collection for which an unmodifiable view is to be
             *         returned.
             * @return an unmodifiable view of the specified collection.
             */
            template <class T>
            static Collection<T>& unmodifiableCollection(Collection<T>& c) {
                if (Class<UnmodifiableCollection<T>>::hasInstance(c))
                    return c;
                return UNSAFE::newInstance<UnmodifiableCollection<T>>(c);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified @c SequencedCollection. Query operations on the returned collection
             * "read through" to the specified collection, and attempts to modify the returned
             * collection, whether direct or via its iterator, result in an
             * @c UnsupportedOperationException.
             * <p>
             * The returned collection does <i>not</i> pass the @c hash and
             * @c equals operations through to the backing collection, but relies on
             * @c Object's @c equals and @c hash methods.  This
             * is necessary to preserve the contracts of these operations in the case
             * that the backing collection is a set or a list.
             * </p>
             * <p>
             * The returned collection will be serializable if the specified collection
             * is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam  T the class of the objects in the collection
             * @param  c the collection for which an unmodifiable view is to be
             *         returned.
             * @return an unmodifiable view of the specified collection.
             */
            template <class T>
            static SequencedCollection<T>& unmodifiableSequencedCollection(SequencedCollection<T>& c) {
                if (Class<UnmodifiableSequencedCollection<T>>::hasInstance(c))
                    return c;
                return UNSAFE::newInstance<UnmodifiableSequencedCollection<T>>(c);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified set. Query operations on the returned set "read through" to the specified
             * set, and attempts to modify the returned set, whether direct or via its
             * iterator, result in an @c UnsupportedOperationException.
             * <p>
             * The returned set will be serializable if the specified set
             * is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam  T the class of the objects in the set
             * @param  s the set for which an unmodifiable view is to be returned.
             * @return an unmodifiable view of the specified set.
             */
            template <class T>
            static Set<T>& unmodifiableSet(Set<T>& s) {
                if (Class<UnmodifiableSet<T>>::hasInstance(s))
                    return s;
                return UNSAFE::newInstance<UnmodifiableSet<T>>(s);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified @c SequencedSet. Query operations on the returned set
             * "read through" to the specified set, and attempts to modify the returned
             * set, whether direct or via its iterator, result in an
             * @c UnsupportedOperationException.
             * <p>
             * The returned set will be serializable if the specified set
             * is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam  T the class of the objects in the set
             * @param  s the set for which an unmodifiable view is to be returned.
             * @return an unmodifiable view of the specified sequenced set.
             */
            template <class T>
            static SequencedSet<T>& unmodifiableSequencedSet(SequencedSet<T>& s) {
                if (Class<UnmodifiableSequencedSet<T>>::hasInstance(s))
                    return s;
                return UNSAFE::newInstance<UnmodifiableSequencedSet<T>>(s);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified sorted set. Query operations on the returned sorted set "read
             * through" to the specified sorted set.  Attempts to modify the returned
             * sorted set, whether direct, via its iterator, or via its
             * @c subSet, @c headSet, or @c tailSet views, result in
             * an @c UnsupportedOperationException.
             * <p>
             * The returned sorted set will be serializable if the specified sorted set
             * is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam  T the class of the objects in the set
             * @param s the sorted set for which an unmodifiable view is to be
             *        returned.
             * @return an unmodifiable view of the specified sorted set.
             */
            template <class T>
            static SortedSet<T>& unmodifiableSortedSet(SortedSet<T>& s) {
                if (Class<UnmodifiableSortedSet<T>>::hasInstance(s))
                    return s;
                return UNSAFE::newInstance<UnmodifiableSortedSet<T>>(s);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified navigable set. Query operations on the returned navigable set "read
             * through" to the specified navigable set.  Attempts to modify the returned
             * navigable set, whether direct, via its iterator, or via its
             * @c subSet, @c headSet, or @c tailSet views, result in
             * an @c UnsupportedOperationException.
             * <p>
             * The returned navigable set will be serializable if the specified
             * navigable set is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam  T the class of the objects in the set
             * @param s the navigable set for which an unmodifiable view is to be
             *        returned
             * @return an unmodifiable view of the specified navigable set
             */
            template <class T>
            static NavigableSet<T>& unmodifiableNavigableSet(NavigableSet<T>& s) {
                if (Class<UnmodifiableNavigableSet<T>>::hasInstance(s))
                    return s;
                return UNSAFE::newInstance<UnmodifiableNavigableSet<T>>(s);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified list. Query operations on the returned list "read through" to the
             * specified list, and attempts to modify the returned list, whether
             * direct or via its iterator, result in an
             * @c UnsupportedOperationException.
             * <p>
             * The returned list will be serializable if the specified list
             * is serializable. Similarly, the returned list will implement
             * @b RandomAccess if the specified list does.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam  T the class of the objects in the list
             * @param  list the list for which an unmodifiable view is to be returned.
             * @return an unmodifiable view of the specified list.
             */
            template <class T>
            static List<T>& unmodifiableList(List<T>& list) {
                if (Class<UnmodifiableList<T>>::hasInstance(list) ||
                    Class<UnmodifiableRandomAccessList<T>>::hasInstance(list))
                    return list;
                if (Class<RandomAccess>::hasInstance(list))
                    return UNSAFE::newInstance<UnmodifiableRandomAccessList<T>>(list);
                return UNSAFE::newInstance<UnmodifiableList<T>>(list);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified map. Query operations on the returned map "read through"
             * to the specified map, and attempts to modify the returned
             * map, whether direct or via its collection views, result in an
             * @c UnsupportedOperationException.
             * <p>
             * The returned map will be serializable if the specified map
             * is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam K the class of the map keys
             * @tparam V the class of the map values
             * @param  map the map for which an unmodifiable view is to be returned.
             * @return an unmodifiable view of the specified map.
             */
            template <class K, class V>
            static Map<K, V>& unmodifiableMap(Map<K, V>& map) {
                if (Class<UnmodifiableMap<K, V>>::hasInstance(map))
                    return map;
                return UNSAFE::newInstance<UnmodifiableMap<K, V>>(map);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified @c SequencedMap. Query operations on the returned map
             * "read through" to the specified map, and attempts to modify the returned
             * map, whether direct or via its collection views, result in an
             * @c UnsupportedOperationException.
             * <p>
             * The returned map will be serializable if the specified map
             * is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam K the class of the map keys
             * @tparam V the class of the map values
             * @param  map the map for which an unmodifiable view is to be returned.
             * @return an unmodifiable view of the specified map.
             */
            template <class K, class V>
            static SequencedMap<K, V>& unmodifiableSequencedMap(SequencedMap<K, V>& map) {
                if (Class<UnmodifiableSequencedMap<K, V>>::hasInstance(map))
                    return map;
                return UNSAFE::newInstance<UnmodifiableSequencedMap<K, V>>(map);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified sorted map. Query operations on the returned sorted map "read through"
             * to the specified sorted map.  Attempts to modify the returned
             * sorted map, whether direct, via its collection views, or via its
             * @c subMap, @c headMap, or @c tailMap views, result in
             * an @c UnsupportedOperationException.
             * <p>
             * The returned sorted map will be serializable if the specified sorted map
             * is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam K the class of the map keys
             * @tparam V the class of the map values
             * @param map the sorted map for which an unmodifiable view is to be
             *        returned.
             * @return an unmodifiable view of the specified sorted map.
             */
            template <class K, class V>
            static SortedMap<K, V>& unmodifiableSortedMap(SortedMap<K, V>& map) {
                if (Class<UnmodifiableSortedMap<K, V>>::hasInstance(map))
                    return map;
                return UNSAFE::newInstance<UnmodifiableSortedMap<K, V>>(map);
            }

            /**
             * Returns an <em>unmodifiable view</em> of the
             * specified navigable map. Query operations on the returned navigable map "read
             * through" to the specified navigable map.  Attempts to modify the returned
             * navigable map, whether direct, via its collection views, or via its
             * @c subMap, @c headMap, or @c tailMap views, result in
             * an @c UnsupportedOperationException.
             * <p>
             * The returned navigable map will be serializable if the specified
             * navigable map is serializable.
             * </p>
             * @note This method may return its argument if the argument is already unmodifiable.
             * @tparam K the class of the map keys
             * @tparam V the class of the map values
             * @param map the navigable map for which an unmodifiable view is to be
             *        returned
             * @return an unmodifiable view of the specified navigable map
             */
            template <class K, class V>
            static NavigableMap<K, V>& unmodifiableNavigableMap(NavigableMap<K, V>& map) {
                if (Class<UnmodifiableNavigableMap<K, V>>::hasInstance(map))
                    return map;
                return UNSAFE::newInstance<UnmodifiableNavigableMap<K, V>>(map);
            }

            /**
             * Returns a synchronized (thread-safe) collection backed by the specified
             * collection.  In order to guarantee serial access, it is critical that
             * @b all access to the backing collection is accomplished
             * through the returned collection.<p>
             *
             * It is imperative that the user manually synchronize on the returned
             * collection when traversing it via @b Iterator or @b Spliterator
             * @code
             *  auto& c = Collections::synchronizedCollection(myCollection);
             *     ...
             *  synchronized (c) {
             *      auto& i = c.iterator(); // Must be in the synchronized block
             *      while (i.hasNext())
             *         foo(i.next());
             *  }
             * @endcode
             * Failure to follow this advice may result in non-deterministic behavior.
             *
             * <p>The returned collection does <i>not</i> pass the @c hash
             * and @c equals operations through to the backing collection, but
             * relies on @c Object's equals and hashCode methods.  This is
             * necessary to preserve the contracts of these operations in the case
             * that the backing collection is a set or a list.<p>
             *
             * The returned collection will be serializable if the specified collection
             * is serializable.
             *
             * @tparam  T the class of the objects in the collection
             * @param  c the collection to be "wrapped" in a synchronized collection.
             * @return a synchronized view of the specified collection.
             */
            template <class T>
            static Collection<T>& synchronizedCollection(Collection<T>& c) {}

            /**
             * Returns a synchronized (thread-safe) set backed by the specified
             * set.  In order to guarantee serial access, it is critical that
             * @b all access to the backing set is accomplished
             * through the returned set.<p>
             *
             * It is imperative that the user manually synchronize on the returned
             * collection when traversing it via @b Iterator or @b Spliterator
             * @code
             *  Set s = Collections::synchronizedSet(new HashSet());
             *      ...
             *  synchronized (s) {
             *      auto& i = s.iterator(); // Must be in the synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * Failure to follow this advice may result in non-deterministic behavior.
             *
             * <p>The returned set will be serializable if the specified set is
             * serializable.
             *
             * @tparam  T the class of the objects in the set
             * @param  s the set to be "wrapped" in a synchronized set.
             * @return a synchronized view of the specified set.
             */
            template <class T>
            static Set<T>& synchronizedSet(Set<T>& s) {}

            /**
             * Returns a synchronized (thread-safe) sorted set backed by the specified
             * sorted set.  In order to guarantee serial access, it is critical that
             * @b all access to the backing sorted set is accomplished
             * through the returned sorted set (or its views).<p>
             *
             * It is imperative that the user manually synchronize on the returned
             * sorted set when traversing it or any of its @c subSet,
             * @c headSet, or @c tailSet views via @b Iterator,
             * or @b Spliterator:
             * @code
             *  SortedSet s = Collections::synchronizedSortedSet(new TreeSet());
             *      ...
             *  synchronized (s) {
             *      auto& i = s.iterator(); // Must be in the synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * or:
             * @code
             *  SortedSet s = Collections::synchronizedSortedSet(new TreeSet());
             *  SortedSet s2 = s.headSet(foo);
             *      ...
             *  synchronized (s) {  // Note: s, not s2!!!
             *      auto& i = s2.iterator(); // Must be in the synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * Failure to follow this advice may result in non-deterministic behavior.
             *
             * <p>The returned sorted set will be serializable if the specified
             * sorted set is serializable.
             *
             * @tparam  T the class of the objects in the set
             * @param  s the sorted set to be "wrapped" in a synchronized sorted set.
             * @return a synchronized view of the specified sorted set.
             */
            template <class T>
            static SortedSet<T>& synchronizedSortedSet(SortedSet<T>& s) {}

            /**
             * Returns a synchronized (thread-safe) navigable set backed by the
             * specified navigable set.  In order to guarantee serial access, it is
             * critical that @b all access to the backing navigable set is
             * accomplished through the returned navigable set (or its views).<p>
             *
             * It is imperative that the user manually synchronize on the returned
             * navigable set when traversing it, or any of its @c subSet,
             * @c headSet, or @c tailSet views, via @b Iterator,
             * or @b Spliterator:
             * @code
             *  NavigableSet s = Collections::synchronizedNavigableSet(new TreeSet());
             *      ...
             *  synchronized (s) {
             *      auto& i = s.iterator(); // Must be in the synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * or:
             * @code
             *  NavigableSet s = Collections::synchronizedNavigableSet(new TreeSet());
             *  NavigableSet s2 = s.headSet(foo, true);
             *      ...
             *  synchronized (s) {  // Note: s, not s2!!!
             *      auto& i = s2.iterator(); // Must be in the synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * Failure to follow this advice may result in non-deterministic behavior.
             *
             * <p>The returned navigable set will be serializable if the specified
             * navigable set is serializable.
             *
             * @tparam  T the class of the objects in the set
             * @param  s the navigable set to be "wrapped" in a synchronized navigable
             * set
             * @return a synchronized view of the specified navigable set
             */
            template <class T>
            static NavigableSet<T>& synchronizedNavigableSet(NavigableSet<T>& s) {}

            /**
             * Returns a synchronized (thread-safe) list backed by the specified
             * list.  In order to guarantee serial access, it is critical that
             * @b all access to the backing list is accomplished
             * through the returned list.<p>
             *
             * It is imperative that the user manually synchronize on the returned
             * list when traversing it via @b Iterator or @b Spliterator
             * @code
             *  auto& list = Collections::synchronizedList(new ArrayList());
             *      ...
             *  synchronized (list) {
             *      auto& i = list.iterator(); // Must be in synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * Failure to follow this advice may result in non-deterministic behavior.
             *
             * <p>The returned list will be serializable if the specified list is
             * serializable.
             *
             * @tparam  T the class of the objects in the list
             * @param  list the list to be "wrapped" in a synchronized list.
             * @return a synchronized view of the specified list.
             */
            template <class T>
            static List<T>& synchronizedList(List<T>& list) {}

            /**
             * Returns a synchronized (thread-safe) map backed by the specified
             * map.  In order to guarantee serial access, it is critical that
             * @b all access to the backing map is accomplished
             * through the returned map.<p>
             *
             * It is imperative that the user manually synchronize on the returned
             * map when traversing any of its collection views via @b Iterator,
             * or @b Spliterator:
             * @code
             *  auto& m = Collections::synchronizedMap(new HashMap());
             *      ...
             *  auto& s = m.keySet();  // Needn't be in synchronized block
             *      ...
             *  synchronized (m) {  // Synchronizing on m, not s!
             *      auto& i = s.iterator(); // Must be in synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * Failure to follow this advice may result in non-deterministic behavior.
             *
             * <p>The returned map will be serializable if the specified map is
             * serializable.
             *
             * @tparam K the class of the map keys
             * @tparam V the class of the map values
             * @param  map the map to be "wrapped" in a synchronized map.
             * @return a synchronized view of the specified map.
             */
            template <class K, class V>
            static Map<K, V>& synchronizedMap(Map<K, V>& map) {
                if (Class<SynchronizedMap<K, V>>::hasInstance(map))
                    return map;
                return UNSAFE::newInstance<SynchronizedMap<K, V>>(map);
            }

            /**
             * Returns a synchronized (thread-safe) sorted map backed by the specified
             * sorted map.  In order to guarantee serial access, it is critical that
             * @b all access to the backing sorted map is accomplished
             * through the returned sorted map (or its views).<p>
             *
             * It is imperative that the user manually synchronize on the returned
             * sorted map when traversing any of its collection views, or the
             * collections views of its @c subMap, @c headMap or
             * @c tailMap views, via @b Iterator or @b Spliterator:
             * @code
             *  auto& m = Collections::synchronizedSortedMap(new TreeMap());
             *      ...
             *  auto& s = m.keySet();  // Needn't be in synchronized block
             *      ...
             *  synchronized (m) {  // Synchronizing on m, not s!
             *      auto& i = s.iterator(); // Must be in synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * or:
             * @code
             *  SortedMap m = Collections::synchronizedSortedMap(new TreeMap());
             *  auto& m2 = m.subMap(foo, bar);
             *      ...
             *  auto& s2 = m2.keySet();  // Needn't be in synchronized block
             *      ...
             *  synchronized (m) {  // Synchronizing on m, not m2 or s2!
             *      auto& i = s2.iterator(); // Must be in synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * Failure to follow this advice may result in non-deterministic behavior.
             *
             * <p>The returned sorted map will be serializable if the specified
             * sorted map is serializable.
             *
             * @tparam K the class of the map keys
             * @tparam V the class of the map values
             * @param  map the sorted map to be "wrapped" in a synchronized sorted map.
             * @return a synchronized view of the specified sorted map.
             */
            template <class K, class V>
            static SortedMap<K, V>& synchronizedSortedMap(SortedMap<K, V>& map) {
                if (Class<SynchronizedSortedMap<K, V>>::hasInstance(map))
                    return map;
                return UNSAFE::newInstance<SynchronizedSortedMap<K, V>>(map);
            }

            /**
             * Returns a synchronized (thread-safe) navigable map backed by the
             * specified navigable map.  In order to guarantee serial access, it is
             * critical that @b all access to the backing navigable map is
             * accomplished through the returned navigable map (or its views).<p>
             *
             * It is imperative that the user manually synchronize on the returned
             * navigable map when traversing any of its collection views, or the
             * collections views of any of its @c subMap, @c headMap or
             * @c tailMap views, via @b Iterator or @b Spliterator:
             * @code
             *  NavigableMap m = Collections::synchronizedNavigableMap(new TreeMap());
             *      ...
             *  Set s = m.keySet();  // Needn't be in synchronized block
             *      ...
             *  synchronized (m) {  // Synchronizing on m, not s!
             *      auto& i = s.iterator(); // Must be in synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * or:
             * @code
             *  NavigableMap m = Collections::synchronizedNavigableMap(new TreeMap());
             *  NavigableMap m2 = m.subMap(foo, true, bar, false);
             *      ...
             *  Set s2 = m2.keySet();  // Needn't be in synchronized block
             *      ...
             *  synchronized (m) {  // Synchronizing on m, not m2 or s2!
             *      auto& i = s2.iterator(); // Must be in synchronized block
             *      while (i.hasNext())
             *          foo(i.next());
             *  }
             * @endcode
             * Failure to follow this advice may result in non-deterministic behavior.
             *
             * <p>The returned navigable map will be serializable if the specified
             * navigable map is serializable.
             *
             * @tparam K the class of the map keys
             * @tparam V the class of the map values
             * @param  map the navigable map to be "wrapped" in a synchronized navigable
             *              map
             * @return a synchronized view of the specified navigable map.
             */
            template <class K, class V>
            static NavigableMap<K, V>& synchronizedNavigableMap(NavigableMap<K, V>& map) {
                if (Class<SynchronizedNavigableMap<K, V>>::hasInstance(map))
                    return map;
                return UNSAFE::newInstance<SynchronizedNavigableMap<K, V>>(map);
            }

            /**
             * Returns an immutable list consisting of @c n copies of the
             * specified object.  The newly allocated data object is tiny (it contains
             * a single reference to the data object).  This method is useful in
             * combination with the @c List::addAll method to grow lists.
             * The returned list is serializable.
             *
             * @tparam  T the class of the object to copy and of the objects
             *         in the returned list.
             * @param  n the number of elements in the returned list.
             * @param  o the element to appear repeatedly in the returned list.
             * @return an immutable list consisting of @c n copies of the
             *         specified object.
             * @throws IllegalArgumentException if @c n < 0
             * @see    List::addAll(Collection)
             * @see    List::addAll(int, Collection)
             */
            template <class T, ClassOf(1)::OnlyIf<Class<T>::isClass()>  = 1>
            static List<T>& nCopies(gint n, T const& o) {
                CORE_CHECK_SLIMMED_TYPE(T);
                class CopiesList final : public List<T> {
                    gint n;
                    T& elmt;

                public:
                    CORE_EXPLICIT CopiesList(gint n, T& element): n(n), elmt(element) {}

                    gint size() const override { return n; }

                    gbool isEmpty() const override { return n == 0; }

                    gbool contains(const T& val) const override { return val == elmt; }

                    Iterator<T>&iterator() override { return listIterator(); }

                    Iterator2<T>&iterator() const override { return listIterator(); }

                    Array<T> toArray() const override { return Array<T>(n, elmt); }

                    gbool add(const T& val) override { UnsupportedOperationException().throws($ftrace()); }

                    gbool remove(const T& val) override { UnsupportedOperationException().throws($ftrace()); }

                    gbool containsAll(const Collection<T>& c) const override {
                        if (n == 0) return false;
                        if (!Class<CopiesList>::hasInstance(c))
                            return List<T>::containsAll(c);
                        CopiesList const& list = CORE_XCAST(CopiesList const, c);
                        return list.n > 0 && elmt == list.elmt;
                    }

                    gbool addAll(const Collection<T>& c) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    gbool addAll(gint index, const Collection<T>& c) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    gbool removeAll(const Collection<T>& c) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    gbool retainAll(const Collection<T>& c) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    void replaceAll(const function::UnaryOperator<T>& op) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    void sort(const Comparator<T>& c) override {}

                    void clear() override { UnsupportedOperationException().throws($ftrace()); }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (Class<CopiesList>::hasInstance(o)) {
                            CopiesList const& list = CORE_XCAST(CopiesList const, o);
                            return n == list.n && elmt == list.elmt;
                        }
                        return List<T>::equals(o);
                    }

                    gint hash() const override {
                        if (n == 0) return 1;
                        // hashCode of n repeating elements is 31^n + elementHash * Sum(31^k, k = 0..n-1)
                        // this implementation completes in O(log(n)) steps taking advantage of
                        // 31^(2*n) = (31^n)^2 and Sum(31^k, k = 0..(2*n-1)) = Sum(31^k, k = 0..n-1) * (31^n + 1)
                        gint pow = 31;
                        gint sum = 1;
                        for (int i = Integer::numberOfLeadingZeros(n) + 1; i < Integer::SIZE; i++) {
                            sum *= pow + 1;
                            pow *= pow;
                            if ((n << i) < 0) {
                                pow *= 31;
                                sum = sum * 31 + 1;
                            }
                        }
                        return pow + sum * (elmt.hash());
                    }

                    T&get(gint index) override {
                        try {
                            misc::Preconditions::checkIndex(index, n);
                            return elmt;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    const T&get(gint index) const override {
                        try {
                            misc::Preconditions::checkIndex(index, n);
                            return elmt;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    T&set(gint index, const T& newValue) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    void add(gint index, const T& val) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    T&remove(gint index) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    gint indexOf(const T& val) const override {
                        return val == elmt ? 0 : -1;
                    }

                    gint lastIndexOf(const T& val) const override {
                        return val == elmt ? n - 1 : -1;
                    }

                    ListIterator<T>&listIterator() override {
                        return listIterator(0);
                    }

                    ListIterator2<T>&listIterator() const override {
                        return listIterator(0);
                    }

                    ListIterator<T>&listIterator(gint index) override {
                        class Itr final : public ListIterator<T> {
                            gint cursor = 0;
                            CopiesList& list;

                        public:
                            CORE_EXPLICIT Itr(gint index, CopiesList& list)
                                : cursor(index), list(list) {}

                            gbool equals(const Object& o) const override {
                                if (this == &o)
                                    return true;
                                if (Class<Itr>::hasInstance(o)) {
                                    Itr const& it = CORE_XCAST(Itr const, o);
                                    return &list == &it.list && cursor == it.cursor;
                                }
                                return false;
                            }

                            gbool hasNext() const override {
                                return cursor < list.n;
                            }

                            T&next() override {
                                if (cursor++ >= list.n)
                                    NoSuchElementException().throws($ftrace());
                                return list.elmt;
                            }

                            void forEach(const function::Consumer<T&>& action) override {
                                while (cursor++ < list.n) action.accept(list.elmt);
                            }

                            gbool hasPrevious() const override {
                                return cursor > 0;
                            }

                            T&previous() override {
                                if (cursor-- <= 0)
                                    NoSuchElementException().throws($ftrace());
                                return list.elmt;
                            }

                            gint nextIndex() const override {
                                return cursor;
                            }

                            gint previousIndex() const override {
                                return cursor - 1;
                            }
                        };

                        try {
                            misc::Preconditions::checkIndexForAdding(index, n);
                            return UNSAFE::newInstance<Itr>(index, *this);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    ListIterator2<T>&listIterator(gint index) const override {
                        class Itr final : public ListIterator2<T> {
                            gint cursor = 0;
                            CopiesList const& list;

                        public:
                            CORE_EXPLICIT Itr(gint index, CopiesList const& list)
                                : cursor(index), list(list) {}

                            gbool equals(const Object& o) const override {
                                if (this == &o)
                                    return true;
                                if (Class<Itr>::hasInstance(o)) {
                                    Itr const& it = CORE_XCAST(Itr const, o);
                                    return &list == &it.list && cursor == it.cursor;
                                }
                                return false;
                            }

                            gbool hasNext() const override {
                                return cursor < list.n;
                            }

                            T const&next() override {
                                if (cursor >= list.n)
                                    NoSuchElementException().throws($ftrace());
                                return list.elmt;
                            }

                            void forEach(const function::Consumer<T>& action) override {
                                while (cursor++ < list.n) action.accept(list.elmt);
                            }

                            gbool hasPrevious() const override {
                                return cursor > 0;
                            }

                            T const&previous() override {
                                if (cursor-- <= 0)
                                    NoSuchElementException().throws($ftrace());
                                return list.elmt;
                            }

                            gint nextIndex() const override {
                                return cursor;
                            }

                            gint previousIndex() const override {
                                return cursor - 1;
                            }
                        };

                        try {
                            misc::Preconditions::checkIndexForAdding(index, n);
                            return UNSAFE::newInstance<Itr>(index, *this);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    const List<T>&subList(gint fromIndex, gint toIndex) const override {
                        try {
                            misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, n);
                            gint size = toIndex - fromIndex;
                            if (size == n) return *this;
                            return nCopies(size, elmt);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    List<T>&subList(gint fromIndex, gint toIndex) override {
                        try {
                            misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, n);
                            gint size = toIndex - fromIndex;
                            if (size == n) return *this;
                            return nCopies(size, elmt);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void addFirst(const T& val) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    void addLast(const T& val) override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    T&firstElement() override {
                        if (n == 0) NoSuchElementException().throws($ftrace());
                        return elmt;
                    }

                    const T&firstElement() const override {
                        if (n == 0) NoSuchElementException().throws($ftrace());
                        return elmt;
                    }

                    T&lastElement() override {
                        if (n == 0) NoSuchElementException().throws($ftrace());
                        return elmt;
                    }

                    const T&lastElement() const override {
                        if (n == 0) NoSuchElementException().throws($ftrace());
                        return elmt;
                    }

                    T&removeFirst() override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    T&removeLast() override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    const List<T>&reversed() const override { return *this; }

                    List<T>&reversed() override { return *this; }
                };

                try {
                    T& elmt = UNSAFE::copyInstance(o);
                    return UNSAFE::newInstance<CopiesList>(n, elmt);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an immutable list consisting of @c n copies of the
             * specified object.  The newly allocated data object is tiny (it contains
             * a single reference to the data object).  This method is useful in
             * combination with the @c List.addAll method to grow lists.
             * The returned list is serializable.
             *
             * @tparam  T The type of input element
             * @tparam  E the class of the object to copy and of the objects
             *         in the returned list.
             * @param  n the number of elements in the returned list.
             * @param  o the element to appear repeatedly in the returned list.
             * @return an immutable list consisting of @c n copies of the
             *         specified object.
             * @throws IllegalArgumentException if @c n < 0
             * @see    List::addAll(Collection)
             * @see    List::addAll(int, Collection)
             */
            template <class T, ClassOf(1)::OnlyIf<Class<T>::isPrimitive()>  = 1, class E = typename Class<T>::Object>
            static List<E>& nCopies(gint n, T const& o) {
                CORE_CHECK_SLIMMED_TYPE(E);
                E& elmt = Array<E>::of(o)[0];
                return nCopies(n, elmt);
            }

            /**
             * Returns the number of elements in the specified collection equal to the
             * specified object.
             *
             * @param c the collection in which to determine the frequency
             *     of @c o
             * @param o the object whose frequency is to be determined
             * @return the number of elements in @c c equal to @c o
             */
            template <class T>
            static gint frequency(Collection<T> const& c, T const& o) {
                int result = 0;
                for (Object const& e : c)
                    if (o.equals(e))
                        result++;
                return result;
            }

            /**
             * Returns @c true if the two specified collections have no
             * elements in common.
             *
             * <p>Care must be exercised if this method is used on collections that
             * do not comply with the general contract for @c Collection.
             * Implementations may elect to iterate over either collection and test
             * for containment in the other collection (or to perform any equivalent
             * computation).  If either collection uses a nonstandard equality test
             * (as does a @b SortedSet whose ordering is not <em>compatible with
             * equals</em>, or the key set of an @b IdentityHashMap), both
             * collections must use the same nonstandard equality test, or the
             * result of this method is undefined.
             *
             * <p>Care must also be exercised when using collections that have
             * restrictions on the elements that they may contain. Collection
             * implementations are allowed to throw exceptions for any operation
             * involving elements they deem ineligible. For absolute safety the
             * specified collections should contain only elements which are
             * eligible elements for both collections.
             *
             * <p>Note that it is permissible to pass the same collection in both
             * parameters, in which case the method will return @c true if and
             * only if the collection is empty.
             *
             * @param c1 a collection
             * @param c2 a collection
             * @return @c true if the two specified collections have no
             * elements in common.
             * @throws ClassCastException if one collection contains an element that is
             * of a type which is ineligible for the other collection. (optional)
             */
            template <class T, class U = T, ClassOf(1)::OnlyIf<Class<U>::template isExtends<T>()>  = 1>
            static gbool disjoint(Collection<T> const& c1, Collection<U> const& c2) {
                // The collection to be used for contains(). Preference is given to
                // the collection who's contains() has lower O() complexity.
                Collection<T> const* contains = &c1;
                // The collection to be iterated. If the collections' contains() impl
                // are of different O() complexity, the collection with slower
                // contains() will be used for iteration. For collections who's
                // contains() are of the same complexity then best performance is
                // achieved by iterating the smaller collection.
                Collection<T> const* iterate = &c2;

                // Performance optimization cases. The heuristics:
                //   1. Generally iterate over c1.
                //   2. If c1 is a Set then iterate over c2.
                //   3. If either collection is empty then result is always true.
                //   4. Iterate over the smaller Collection.
                if (Class<Set<T>>::hasInstance(c1)) {
                    // Use c1 for contains as a Set's contains() is expected to perform
                    // better than O(N/2)
                    iterate = c2;
                    contains = c1;
                } else if (!Class<Set<T>>::hasInstance(c2)) {
                    // Both are mere Collections. Iterate over smaller collection.
                    // Example: If c1 contains 3 elements and c2 contains 50 elements and
                    // assuming contains() requires ceiling(N/2) comparisons then
                    // checking for all c1 elements in c2 would require 75 comparisons
                    // (3 * ceiling(50/2)) vs. checking all c2 elements in c1 requiring
                    // 100 comparisons (50 * ceiling(3/2)).
                    int c1size = c1.size();
                    int c2size = c2.size();
                    if (c1size == 0 || c2size == 0) {
                        // At least one collection is empty. Nothing will match.
                        return true;
                    }

                    if (c1size > c2size) {
                        iterate = c2;
                        contains = c1;
                    }
                }

                for (T const& obj : *iterate) {
                    if (contains->contains(obj)) {
                        // Found a common element. Collections are not disjoint.
                        return false;
                    }
                }

                // No common elements were found.
                return true;
            }

            /**
             * Adds all of the specified elements to the specified collection.
             * Elements to be added may be specified individually or as an array.
             *
             * <p>When elements are specified individually, this method provides a
             * convenient way to add a few elements to an existing collection:
             * @code
             *     Collections.addAll(flavors, "Peaches 'n Plutonium", "Rocky Racoon");
             * @endcode
             *
             * @tparam  T the class of the elements to add and of the collection
             * @param c the collection into which @c elements are to be inserted
             * @param args the elements to insert into @c c
             * @return @c true if the collection changed as a result of the call
             * @throws UnsupportedOperationException if @c c does not support
             *         the @c add operation
             * @throws IllegalArgumentException if some property of a value in
             *         @c elements prevents it from being added to @c c
             * @see Collection#addAll(Collection)
             */
            template <class T, class... Args>
            static gbool addAll(Collection<T> const& c, Args const&... args) {
                try {
                    return addAll(c, Array<T>::of(args...));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            template <class T, class U = T, ClassOf(1)::OnlyIf<Class<U>::template isExtends<T>()>  = 1>
            static gbool addAll(Collection<T>& c, Array<U> const& args) {
                gbool result = false;
                for (U const& arg : args)
                    try { result |= c.add(arg); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return result;
            }

            /**
             * Returns a view of a @b Deque as a Last-in-first-out (Lifo)
             * @b Queue. Method @c add is mapped to @c push,
             * @c remove is mapped to @c pop and so on. This
             * view can be useful when you would like to use a method
             * requiring a @c Queue, but you need Lifo ordering.
             *
             * <p>Each method invocation on the queue returned by this method
             * results in exactly one method invocation on the backing deque, with
             * one exception.  The @b addAll method is
             * implemented as a sequence of @b addFirst
             * invocations on the backing deque.
             *
             * @note
             * This method provides a view that inverts the sense of certain operations,
             * but it doesn't reverse the encounter order. To obtain a reverse-ordered
             * view, use the @b Deque::reversed method.
             *
             * @tparam  T the class of the objects in the deque
             * @param deque the deque
             * @return the queue
             */
            template <class T>
            static Queue<T>& asLifoQueue(Deque<T>& deque) {
                class Lifo final : public Queue<T> {
                    Deque<T>& deque;

                public:
                    CORE_EXPLICIT Lifo(Deque<T>& deque) : deque(deque) {}

                    gint size() const override {
                        return deque.size();
                    }

                    gbool isEmpty() const override {
                        return deque.isEmpty();
                    }

                    gbool contains(const T& val) const override {
                        return deque.contains(val);
                    }

                    Iterator<T>&iterator() override {
                        return deque.iterator();
                    }

                    Iterator2<T>&iterator() const override {
                        return ((Deque<T> const&) deque).iterator();
                    }

                    Array<T> toArray() const override {
                        return deque.toArray();
                    }

                    gbool remove(const T& val) override {
                        return deque.remove(val);
                    }

                    gbool containsAll(const Collection<T>& c) const override {
                        return deque.containsAll(c);
                    }

                    gbool removeAll(const Collection<T>& c) override {
                        return deque.removeAll(c);
                    }

                    gbool removeIf(const function::Predicate<T>& filter) override {
                        return deque.removeIf(filter);
                    }

                    gbool retainAll(const Collection<T>& c) override {
                        return deque.retainAll(c);
                    }

                    gbool add(const T& val) override {
                        return deque.offerFirst(val);
                    }

                    gbool offer(const T& val) override {
                        return deque.offerFirst(val);
                    }

                    T&remove() override {
                        return deque.removeFirst();
                    }

                    Object&poll() override {
                        return isEmpty() ? (Object&) null : deque.removeLast();
                    }

                    T&element() override {
                        return deque.firstElement();
                    }

                    const T&element() const override {
                        return deque.firstElement();
                    }

                    Object&peek() override {
                        return deque.peekFirst();
                    }

                    const Object&peek() const override {
                        return deque.peekFirst();
                    }

                    void clear() override {
                        deque.clear();
                    }

                    gbool addAll(const Collection<T>& c) override {
                        return deque.addAll(c);
                    }

                    void forEach(const function::Consumer<T&>& action) override {
                        deque.forEach(action);
                    }

                    void forEach(const function::Consumer<T>& action) const override {
                        ((Deque<T> const&) deque).forEach(action);
                    }

                    Spliterator<T>&spliterator() override {
                        return deque.spliterator();
                    }

                    Spliterator2<T>&spliterator() const override {
                        return ((Deque<T> const&) deque).spliterator();
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (Class<Lifo>::hasInstance(o))
                            return deque.equals(CORE_XCAST(Lifo const, o).deque);
                        return false;
                    }

                    gint hash() const override {
                        return deque.hash();
                    }

                    String toString() const override {
                        return deque.toString();
                    }
                };

                return UNSAFE::newInstance<Lifo>(deque);
            }

        private:
            template <class T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<Comparable<T>>()>  = 1>
            static gint binarySearch0(List<T> const& list, T const& key) {
                gint low = 0;
                gint high = list.size() - 1;

                gint mid = 0;
                gint cmp = 0;
                while (low <= high) {
                    mid = (low + high) >> 1;
                    Comparable<T> const& midVal = list.get(mid);
                    try { cmp = midVal.compareTo(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }

                    if (cmp < 0)
                        low = mid + 1;
                    else if (cmp > 0)
                        high = mid - 1;
                    else
                        return mid; // key found
                }
                return -(low + 1); // key not found
            }

            template <class T>
            static gint binarySearch0(List<T> const& list, T const& key, Comparator<T> const& c) {
                gint low = 0;
                gint high = list.size() - 1;

                gint mid = 0;
                gint cmp = 0;
                while (low <= high) {
                    mid = (low + high) >> 1;
                    T const& midVal = list.get(mid);
                    try { cmp = c.compare(midVal, key); } catch (Throwable const& ex) { ex.throws($ftrace()); }

                    if (cmp < 0)
                        low = mid + 1;
                    else if (cmp > 0)
                        high = mid - 1;
                    else
                        return mid; // key found
                }
                return -(low + 1); // key not found
            }

            template <class T, ClassOf(1)::OnlyIf<Class<T>::template isExtends<Comparable<T>>()>  = 1>
            static gint binarySearchWithIterator(List<T> const& list, T const& key) {
                gint low = 0;
                gint high = list.size() - 1;
                ListIterator2<T>& it = list.iterator();

                gint mid = 0;
                gint cmp = 0;
                while (low <= high) {
                    mid = (low + high) >> 1;
                    try {
                        Comparable<T> const& midVal = get(it, mid);
                        cmp = midVal.compareTo(key);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }

                    if (cmp < 0)
                        low = mid + 1;
                    else if (cmp > 0)
                        high = mid - 1;
                    else {
                        UNSAFE::deleteRegInstance(it);
                        return mid; // key found
                    }
                }

                UNSAFE::deleteRegInstance(it);
                return -(low + 1); // key not found
            }

            template <class T>
            static gint binarySearchWithIterator(List<T> const& list, T const& key, Comparator<T> const& c) {
                gint low = 0;
                gint high = list.size() - 1;
                ListIterator2<T>& it = list.iterator();

                gint mid = 0;
                gint cmp = 0;
                while (low <= high) {
                    mid = (low + high) >> 1;
                    try {
                        T const& midVal = get(it, mid);
                        cmp = c.compare(midVal, key);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }

                    if (cmp < 0)
                        low = mid + 1;
                    else if (cmp > 0)
                        high = mid - 1;
                    else {
                        UNSAFE::deleteRegInstance(it);
                        return mid; // key found
                    }
                }
                UNSAFE::deleteRegInstance(it);
                return -(low + 1); // key not found
            }

            /**
             * Gets the ith element from the given list by repositioning the specified
             * list listIterator.
             */
            template <class T>
            static T const& get(ListIterator2<T>& it, gint index) {
                Optional<T> obj;
                gint pos = it.nextIndex();
                if (pos <= index) {
                    do {
                        try { obj = it.next(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    } while (pos++ < index);
                } else {
                    do {
                        try { obj = it.previous(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    } while (--pos > index);
                }
                return obj.get();
            }

            /**
             * Swaps the two specified elements in the specified array.
             */
            template <class T>
            static void swap(Array<T>& a, gint i, gint j) { Arrays::swap(a, i, j); }

            template <class T>
            static void rotate1(List<T>& list, gint distance) {
                gint size = list.size();
                if (size == 0)
                    return;
                distance = distance % size;
                if (distance < 0)
                    distance += size;
                if (distance == 0)
                    return;
                for (gint i = 0, n = 0; n != size; i++) {
                    Optional<T> displaced = list.get(i);
                    gint j = i;
                    do {
                        j += distance;
                        if (j >= size)
                            j -= size;
                        displaced = list.set(j, displaced.get());
                        n++;
                    } while (j != i);
                }
            }

            template <class T>
            static void rotate2(List<T>& list, gint distance) {
                gint size = list.size();
                if (size == 0)
                    return;
                gint mid = -distance % size;
                if (mid < 0)
                    mid += size;
                if (distance == 0)
                    return;
                List<T>& list1 = list.subList(0, mid);
                List<T>& list2 = list.subList(mid, size);
                try {
                    reverse(list1);
                    reverse(list2);
                    reverse(list);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                UNSAFE::deleteRegInstance(list1);
                UNSAFE::deleteRegInstance(list2);
            }

            template <class E>
            class UnmodifiableCollection;

            template <class E>
            class UnmodifiableSequencedCollection;

            template <class E>
            class UnmodifiableSet;

            template <class E>
            class UnmodifiableSequencedSet;

            template <class E>
            class UnmodifiableSortedSet;

            template <class E>
            class UnmodifiableNavigableSet;

            template <class E>
            class UnmodifiableList;

            template <class E>
            class UnmodifiableRandomAccessList;

            template <class K, class V>
            class UnmodifiableMap;

            template <class K, class V>
            class UnmodifiableEntrySet;

            template <class K, class V>
            class UnmodifiableEntry;

            template <class K, class V>
            class UnmodifiableSequencedMap;

            template <class K, class V>
            class UnmodifiableSortedMap;

            template <class K, class V>
            class UnmodifiableNavigableMap;

            template <class E>
            class SynchronizedCollection;

            template <class E>
            class SynchronizedSet;

            template <class E>
            class SynchronizedSortedSet;

            template <class E>
            class SynchronizedNavigableSet;

            template <class E>
            class SynchronizedList;

            template <class E>
            class SynchronizedRandomAccessList;

            template <class K, class V>
            class SynchronizedMap;

            template <class K, class V>
            class SynchronizedSortedMap;

            template <class K, class V>
            class SynchronizedNavigableMap;
        };

        template <class E>
        class Collections::UnmodifiableCollection : public Collection<E> {
        public:
            Collection<E>& collection;

            CORE_EXPLICIT UnmodifiableCollection(Collection<E> const& collection)
                : collection((Collection<E>&) collection) {}

            gint size() const override {
                try { return collection.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return collection.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(E const& val) const override {
                try { return collection.contains(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try { return collection.toArray(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try { return collection.toString(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& iterator() override {
                class Itr final : public Iterator<E> {
                    Iterator<E>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<E>& it) : it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    E&next() override {
                        try { return it.next(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    void forEach(function::Consumer<E&> const& action) override {
                        try { it.forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(Object const& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteRegInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(collection.iterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& iterator() const override {
                class Itr final : public Iterator2<E> {
                    Iterator2<E>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator2<E>& it) : it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    E const&next() override {
                        try { return it.next(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        UnsupportedOperationException().throws($ftrace());
                    }

                    void forEach(function::Consumer<E> const& action) override {
                        try { it.forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(Object const& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteRegInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(((Collection<E> const&) collection).iterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(E const& val) override { UnsupportedOperationException().throws($ftrace()); }

            gbool remove(E const& val) override { UnsupportedOperationException().throws($ftrace()); }

            gbool containsAll(Collection<E> const& c) const override {
                try {
                    return collection.containsAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(Collection<E> const& c) override { UnsupportedOperationException().throws($ftrace()); }

            gbool removeAll(Collection<E> const& c) override { UnsupportedOperationException().throws($ftrace()); }

            gbool retainAll(Collection<E> const& c) override { UnsupportedOperationException().throws($ftrace()); }

            void clear() override { UnsupportedOperationException().throws($ftrace()); }

            void forEach(function::Consumer<E&> const& action) override {
                try { collection.forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<E> const& action) const override {
                try { collection.forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(function::Predicate<E> const& filter) override {
                try { return collection.removeIf(filter); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator<E>& spliterator() override {
                try { return collection.spliterator(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator2<E>& spliterator() const override {
                try {
                    return ((Collection<E> const&) collection).spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool equals(Object const& o) const override {
                try { return this == &o || collection == o; } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try { return collection.hash(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class E>
        class Collections::UnmodifiableSequencedCollection : public virtual UnmodifiableCollection<E>,
                                                             public virtual SequencedCollection<E> {
            CORE_IMPORT_FIELD_OR_METHOD(UnmodifiableCollection<E>, collection);

        public:
            CORE_EXPLICIT UnmodifiableSequencedCollection(SequencedCollection<E> const& c)
                : UnmodifiableCollection<E>(c), SequencedCollection<E>() {}

            SequencedCollection<E>& sc() { return CORE_XCAST(SequencedCollection<E>, collection); }

            SequencedCollection<E> const& sc() const { return CORE_XCAST(SequencedCollection<E>, collection); }

            gint size() const override {
                try {
                    return UnmodifiableCollection<E>::size();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    return UnmodifiableCollection<E>::isEmpty();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const E& val) const override {
                return UnmodifiableCollection<E>::contains(val);
            }

            Array<E> toArray() const override {
                try {
                    return UnmodifiableCollection<E>::toArray();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    return UnmodifiableCollection<E>::toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& iterator() override {
                try {
                    return UnmodifiableCollection<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& iterator() const override {
                try {
                    return UnmodifiableCollection<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const E& val) override {
                try {
                    return UnmodifiableCollection<E>::add(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const E& val) override {
                try {
                    return UnmodifiableCollection<E>::remove(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(const Collection<E>& c) const override {
                try {
                    return UnmodifiableCollection<E>::containsAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableCollection<E>::addAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableCollection<E>::removeAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableCollection<E>::retainAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    UnmodifiableCollection<E>::clear();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E&>& action) override {
                try {
                    UnmodifiableCollection<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E>& action) const override {
                try {
                    UnmodifiableCollection<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(const function::Predicate<E>& filter) override {
                try {
                    return UnmodifiableCollection<E>::removeIf(filter);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator<E>& spliterator() override {
                try {
                    return UnmodifiableCollection<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator2<E>& spliterator() const override {
                try {
                    return UnmodifiableCollection<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool equals(const Object& o) const override {
                try {
                    return UnmodifiableCollection<E>::equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try {
                    return UnmodifiableCollection<E>::hash();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<E> const& reversed() const override {
                try {
                    return UNSAFE::newInstance<UnmodifiableSequencedCollection>(sc().reversed());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<E>& reversed() override {
                try {
                    return UNSAFE::newInstance<UnmodifiableSequencedCollection>(sc().reversed());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void addFirst(E const& val) override { UnsupportedOperationException().throws($ftrace()); }

            void addLast(E const& val) override { UnsupportedOperationException().throws($ftrace()); }

            E& firstElement() override {
                try { return sc().firstElement(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E const& firstElement() const override {
                try { return sc().firstElement(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& lastElement() override {
                try { return sc().lastElement(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E const& lastElement() const override {
                try { return sc().lastElement(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& removeFirst() override { UnsupportedOperationException().throws($ftrace()); }

            E& removeLast() override { UnsupportedOperationException().throws($ftrace()); }
        };

        template <class E>
        class Collections::UnmodifiableSet : public virtual UnmodifiableCollection<E>, public virtual Set<E> {
        public:
            CORE_EXPLICIT UnmodifiableSet(Set<E> const& s) : UnmodifiableCollection<E>(s), Set<E>() {}

            gint size() const override {
                try {
                    return UnmodifiableCollection<E>::size();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    return UnmodifiableCollection<E>::isEmpty();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const E& val) const override {
                try {
                    return UnmodifiableCollection<E>::contains(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return UnmodifiableCollection<E>::toArray();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    return UnmodifiableCollection<E>::toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& iterator() override {
                try {
                    return UnmodifiableCollection<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& iterator() const override {
                try {
                    return UnmodifiableCollection<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const E& val) override {
                try {
                    return UnmodifiableCollection<E>::add(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const E& val) override {
                try {
                    return UnmodifiableCollection<E>::remove(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(const Collection<E>& c) const override {
                try {
                    return UnmodifiableCollection<E>::containsAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableCollection<E>::addAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableCollection<E>::removeAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableCollection<E>::retainAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    UnmodifiableCollection<E>::clear();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E&>& action) override {
                try {
                    UnmodifiableCollection<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E>& action) const override {
                try {
                    UnmodifiableCollection<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(const function::Predicate<E>& filter) override {
                try {
                    return UnmodifiableCollection<E>::removeIf(filter);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator<E>& spliterator() override {
                try {
                    return UnmodifiableCollection<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator2<E>& spliterator() const override {
                try {
                    return UnmodifiableCollection<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool equals(const Object& o) const override {
                try {
                    return UnmodifiableCollection<E>::equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try {
                    return UnmodifiableCollection<E>::hash();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class E>
        class Collections::UnmodifiableSequencedSet : public virtual UnmodifiableSet<E>,
                                                      public virtual SequencedSet<E> {
        public:
            CORE_EXPLICIT UnmodifiableSequencedSet(SequencedSet<E> const& s)
                : UnmodifiableCollection<E>(s), UnmodifiableSet<E>(s), SequencedSet<E>() {}

            SequencedSet<E>& ss() { return CORE_XCAST(SequencedSet<E>, UnmodifiableSet<E>::collection); }

            SequencedSet<E> const& ss() const { return CORE_XCAST(SequencedSet<E>, UnmodifiableSet<E>::collection); }

            gint size() const override {
                try {
                    return UnmodifiableSet<E>::size();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    return UnmodifiableSet<E>::isEmpty();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const E& val) const override {
                try {
                    return UnmodifiableSet<E>::contains(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return UnmodifiableSet<E>::toArray();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    return UnmodifiableSet<E>::toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& iterator() override {
                try {
                    return UnmodifiableSet<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& iterator() const override {
                try {
                    return UnmodifiableSet<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const E& val) override {
                try {
                    return UnmodifiableSet<E>::add(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const E& val) override {
                try {
                    return UnmodifiableSet<E>::remove(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(const Collection<E>& c) const override {
                try {
                    return UnmodifiableSet<E>::containsAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSet<E>::addAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSet<E>::removeAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSet<E>::retainAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    UnmodifiableSet<E>::clear();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E&>& action) override {
                try {
                    UnmodifiableSet<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E>& action) const override {
                try {
                    UnmodifiableSet<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(const function::Predicate<E>& filter) override {
                try {
                    return UnmodifiableSet<E>::removeIf(filter);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator<E>& spliterator() override {
                try {
                    return UnmodifiableSet<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator2<E>& spliterator() const override {
                try {
                    return UnmodifiableSet<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool equals(const Object& o) const override {
                try {
                    return UnmodifiableSet<E>::equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try {
                    return UnmodifiableSet<E>::hash();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<E> const& reversed() const override {
                try {
                    return UNSAFE::newInstance<UnmodifiableSequencedSet>(ss().reversed());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<E>& reversed() override {
                try {
                    return UNSAFE::newInstance<UnmodifiableSequencedSet>(ss().reversed());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class E>
        class Collections::UnmodifiableSortedSet : public virtual UnmodifiableSet<E>,
                                                   public virtual SortedSet<E> {
        public:
            CORE_EXPLICIT UnmodifiableSortedSet(SortedSet<E> const& s)
                : UnmodifiableCollection<E>(s), UnmodifiableSet<E>(s), SortedSet<E>() {}

            SortedSet<E>& ss() { return CORE_XCAST(SortedSet<E>, UnmodifiableSet<E>::collection); }

            SortedSet<E> const& ss() const { return CORE_XCAST(SortedSet<E>, UnmodifiableSet<E>::collection); }

            gint size() const override {
                try {
                    return UnmodifiableSet<E>::size();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    return UnmodifiableSet<E>::isEmpty();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const E& val) const override {
                try {
                    return UnmodifiableSet<E>::contains(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return UnmodifiableSet<E>::toArray();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    return UnmodifiableSet<E>::toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& iterator() override {
                try {
                    return UnmodifiableSet<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& iterator() const override {
                try {
                    return UnmodifiableSet<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const E& val) override {
                try {
                    return UnmodifiableSet<E>::add(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const E& val) override {
                try {
                    return UnmodifiableSet<E>::remove(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(const Collection<E>& c) const override {
                try {
                    return UnmodifiableSet<E>::containsAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSet<E>::addAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSet<E>::removeAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSet<E>::retainAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    UnmodifiableSet<E>::clear();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E&>& action) override {
                try {
                    UnmodifiableSet<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E>& action) const override {
                try {
                    UnmodifiableSet<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(const function::Predicate<E>& filter) override {
                try {
                    return UnmodifiableSet<E>::removeIf(filter);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator<E>& spliterator() override {
                try {
                    return UnmodifiableSet<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator2<E>& spliterator() const override {
                try {
                    return UnmodifiableSet<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool equals(const Object& o) const override {
                try {
                    return UnmodifiableSet<E>::equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try {
                    return UnmodifiableSet<E>::hash();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Comparator<E>& comparator() const override {
                try {
                    return ss().comparator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedSet<E>& subSet(const E& fromElement, const E& toElement) const override {
                try {
                    return ss().subSet(fromElement, toElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedSet<E>& subSet(const E& fromElement, const E& toElement) override {
                try {
                    return ss().subSet(fromElement, toElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedSet<E>& headSet(const E& toElement) const override {
                try {
                    return ss().headSet(toElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedSet<E>& headSet(const E& toElement) override {
                try {
                    return ss().headSet(toElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedSet<E>& tailSet(const E& fromElement) const override {
                try {
                    return ss().tailSet(fromElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedSet<E>& tailSet(const E& fromElement) override {
                try {
                    return ss().tailSet(fromElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& first() const override {
                try {
                    return ss().first();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& first() override {
                try {
                    return ss().first();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& last() const override {
                try {
                    return ss().last();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& last() override {
                try {
                    return ss().last();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class E>
        class Collections::UnmodifiableNavigableSet : public virtual UnmodifiableSortedSet<E>,
                                                      public virtual NavigableSet<E> {
        public:
            CORE_EXPLICIT UnmodifiableNavigableSet(NavigableSet<E> const& s)
                : UnmodifiableCollection<E>(s), UnmodifiableSet<E>(s), UnmodifiableSortedSet<E>(s), NavigableSet<E>() {}

            NavigableSet<E>& ns() {
                return CORE_XCAST(NavigableSet<E>, UnmodifiableSortedSet<E>::collection);
            }

            NavigableSet<E> const& ns() const {
                return CORE_XCAST(NavigableSet<E>, UnmodifiableSortedSet<E>::collection);
            }

            gint size() const override {
                try {
                    return UnmodifiableSortedSet<E>::size();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    return UnmodifiableSortedSet<E>::isEmpty();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const E& val) const override {
                try {
                    return UnmodifiableSortedSet<E>::contains(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return UnmodifiableSortedSet<E>::toArray();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    return UnmodifiableSortedSet<E>::toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& iterator() override {
                try {
                    return UnmodifiableSortedSet<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& iterator() const override {
                try {
                    return UnmodifiableSortedSet<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const E& val) override {
                try {
                    return UnmodifiableSortedSet<E>::add(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const E& val) override {
                try {
                    return UnmodifiableSortedSet<E>::remove(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(const Collection<E>& c) const override {
                try {
                    return UnmodifiableSortedSet<E>::containsAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSortedSet<E>::addAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSortedSet<E>::removeAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(const Collection<E>& c) override {
                try {
                    return UnmodifiableSortedSet<E>::retainAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    UnmodifiableSortedSet<E>::clear();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E&>& action) override {
                try {
                    UnmodifiableSortedSet<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E>& action) const override {
                try {
                    UnmodifiableSortedSet<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(const function::Predicate<E>& filter) override {
                try {
                    return UnmodifiableSortedSet<E>::removeIf(filter);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator<E>& spliterator() override {
                try {
                    return UnmodifiableSortedSet<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator2<E>& spliterator() const override {
                try {
                    return UnmodifiableSortedSet<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool equals(const Object& o) const override {
                try {
                    return UnmodifiableSortedSet<E>::equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try {
                    return UnmodifiableSortedSet<E>::hash();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Comparator<E>& comparator() const override {
                try {
                    return UnmodifiableSortedSet<E>::comparator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedSet<E>& subSet(const E& fromElement, const E& toElement) const override {
                try {
                    return UnmodifiableSortedSet<E>::subSet(fromElement, toElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedSet<E>& subSet(const E& fromElement, const E& toElement) override {
                try {
                    return UnmodifiableSortedSet<E>::subSet(fromElement, toElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedSet<E>& headSet(const E& toElement) const override {
                try {
                    return UnmodifiableSortedSet<E>::headSet(toElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedSet<E>& headSet(const E& toElement) override {
                try {
                    return UnmodifiableSortedSet<E>::headSet(toElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedSet<E>& tailSet(const E& fromElement) const override {
                try {
                    return UnmodifiableSortedSet<E>::tailSet(fromElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedSet<E>& tailSet(const E& fromElement) override {
                try {
                    return UnmodifiableSortedSet<E>::tailSet(fromElement);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& first() const override {
                try {
                    return UnmodifiableSortedSet<E>::first();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& first() override {
                try {
                    return UnmodifiableSortedSet<E>::first();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& last() const override {
                try {
                    return UnmodifiableSortedSet<E>::last();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& last() override {
                try {
                    return UnmodifiableSortedSet<E>::last();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& lower(const E& e) const override {
                try { return ns().lower(e); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& lower(const E& e) override {
                try { return ns().lower(e); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& floor(const E& e) const override {
                try { return ns().floor(e); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& floor(const E& e) override {
                try { return ns().floor(e); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& ceiling(const E& e) const override {
                try { return ns().ceiling(e); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& ceiling(const E& e) override {
                try { return ns().ceiling(e); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& higher(const E& e) const override {
                try { return ns().higher(e); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& higher(const E& e) override {
                try { return ns().higher(e); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& pollFirst() override { UnsupportedOperationException().throws($ftrace()); }

            Object& pollLast() override { UnsupportedOperationException().throws($ftrace()); }

            const NavigableSet<E>& descendingSet() const override {
                try {
                    return UNSAFE::newInstance<UnmodifiableNavigableSet>(ns().descendingSet());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<E>& descendingSet() override {
                try {
                    return UNSAFE::newInstance<UnmodifiableNavigableSet>(ns().descendingSet());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& descendingIterator() override {
                try {
                    return descendingSet().iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& descendingIterator() const override {
                try {
                    return descendingSet().iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<E>& subSet(const E& fromElement, gbool fromInclusive,
                                          const E& toElement, gbool toInclusive) const override {
                try {
                    return ns().subSet(fromElement, fromInclusive, toElement, toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<E>& subSet(const E& fromElement, gbool fromInclusive,
                                    const E& toElement, gbool toInclusive) override {
                try {
                    return ns().subSet(fromElement, fromInclusive, toElement, toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<E>& headSet(const E& toElement, gbool inclusive) const override {
                try {
                    return ns().headSet(toElement, inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<E>& headSet(const E& toElement, gbool inclusive) override {
                try {
                    return ns().headSet(toElement, inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<E>& tailSet(const E& fromElement, gbool inclusive) const override {
                try {
                    return ns().tailSet(fromElement, inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<E>& tailSet(const E& fromElement, gbool inclusive) override {
                try {
                    return ns().tailSet(fromElement, inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class E>
        class Collections::UnmodifiableList : public virtual UnmodifiableCollection<E>, public virtual List<E> {
        public:
            CORE_EXPLICIT UnmodifiableList(List<E> const& list) : UnmodifiableCollection<E>(list), List<E>() {}

            virtual List<E>& l() { return CORE_XCAST(List<E>, UnmodifiableCollection<E>::collection); }

            virtual List<E> const& l() const { return CORE_XCAST(List<E>, UnmodifiableCollection<E>::collection); }

            gint size() const override {
                try {
                    return UnmodifiableCollection<E>::size();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    return UnmodifiableCollection<E>::isEmpty();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(E const& val) const override {
                try {
                    return UnmodifiableCollection<E>::contains(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return UnmodifiableCollection<E>::toArray();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    return UnmodifiableCollection<E>::toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(E const& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool remove(E const& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool addAll(Collection<E> const& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool removeAll(Collection<E> const& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool retainAll(Collection<E> const& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                UnsupportedOperationException().throws($ftrace());
            }

            void forEach(function::Consumer<E&> const& action) override {
                try {
                    return UnmodifiableCollection<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<E> const& action) const override {
                try {
                    return UnmodifiableCollection<E>::forEach(action);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(function::Predicate<E> const& filter) override {
                UnsupportedOperationException().throws($ftrace());
            }

            Spliterator<E>& spliterator() override {
                try {
                    return UnmodifiableCollection<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator2<E>& spliterator() const override {
                try {
                    return UnmodifiableCollection<E>::spliterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool equals(Object const& o) const override {
                try {
                    return UnmodifiableCollection<E>::equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try {
                    return UnmodifiableCollection<E>::hash();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& iterator() final {
                try {
                    return UnmodifiableCollection<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& iterator() const final {
                try {
                    return UnmodifiableCollection<E>::iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(gint index, Collection<E> const& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void add(gint index, E const& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void addFirst(E const& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void addLast(E const& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            E& remove(gint index) override {
                UnsupportedOperationException().throws($ftrace());
            }

            E& removeFirst() override {
                UnsupportedOperationException().throws($ftrace());
            }

            E& removeLast() override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool containsAll(Collection<E> const& c) const override {
                try {
                    return UnmodifiableCollection<E>::containsAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& get(gint index) override {
                try { return l().get(index); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const E& get(gint index) const override {
                try { return l().get(index); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& set(gint index, const E& newValue) override { UnsupportedOperationException().throws($ftrace()); }

            gint indexOf(const E& val) const override {
                try { return l().indexOf(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint lastIndexOf(const E& val) const override {
                try { return l().lastIndexOf(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void replaceAll(const function::UnaryOperator<E>& op) override {
                UnsupportedOperationException().throws($ftrace());
            }

            ListIterator<E>& listIterator() override {
                try { return listIterator(0); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ListIterator2<E>& listIterator() const override {
                try { return listIterator(0); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ListIterator<E>& listIterator(gint index) override {
                class Itr final : public ListIterator<E> {
                    ListIterator<E>& it;

                public:
                    CORE_EXPLICIT Itr(ListIterator<E>& it) : it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    E&next() override {
                        try { return it.next(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool hasPrevious() const override {
                        try { return it.hasPrevious(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    E&previous() override {
                        try { return it.previous(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gint nextIndex() const override {
                        try { return it.nextIndex(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gint previousIndex() const override {
                        try { return it.previousIndex(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override { UnsupportedOperationException().throws($ftrace()); }

                    void set(const E& e) override { UnsupportedOperationException().throws($ftrace()); }

                    void add(const E& e) override { UnsupportedOperationException().throws($ftrace()); }

                    void forEach(const function::Consumer<E&>& action) override {
                        try { it.forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    ~Itr() override { UNSAFE::deleteRegInstance(it); }
                };

                try {
                    return UNSAFE::newInstance<Itr>(l().listIterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ListIterator2<E>& listIterator(gint index) const override {
                class Itr final : public ListIterator2<E> {
                    ListIterator2<E>& it;

                public:
                    CORE_EXPLICIT Itr(ListIterator2<E>& it) : it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    E const&next() override {
                        try { return it.next(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool hasPrevious() const override {
                        try { return it.hasPrevious(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    E const&previous() override {
                        try { return it.previous(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gint nextIndex() const override {
                        try { return it.nextIndex(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gint previousIndex() const override {
                        try { return it.previousIndex(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override { UnsupportedOperationException().throws($ftrace()); }

                    void set(const E& e) override { UnsupportedOperationException().throws($ftrace()); }

                    void add(const E& e) override { UnsupportedOperationException().throws($ftrace()); }

                    void forEach(const function::Consumer<E>& action) override {
                        try { it.forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    ~Itr() override { UNSAFE::deleteRegInstance(it); }
                };

                try {
                    return UNSAFE::newInstance<Itr>(l().listIterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const List<E>& subList(gint fromIndex, gint toIndex) const override {
                try {
                    return UNSAFE::newInstance<UnmodifiableList<E>>(l().subList(fromIndex, toIndex));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            List<E>& subList(gint fromIndex, gint toIndex) override {
                try {
                    return UNSAFE::newInstance<UnmodifiableList<E>>(l().subList(fromIndex, toIndex));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const List<E>& reversed() const override {
                try {
                    return UNSAFE::newInstance<UnmodifiableList<E>>(l().reversed());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            List<E>& reversed() override {
                try {
                    return UNSAFE::newInstance<UnmodifiableList<E>>(l().reversed());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class E>
        class Collections::UnmodifiableRandomAccessList : public virtual UnmodifiableList<E>,
                                                          public virtual RandomAccess {
        public:
            CORE_EXPLICIT UnmodifiableRandomAccessList(List<E> const& list)
                : UnmodifiableCollection<E>(list), UnmodifiableList<E>(list) {}

            List<E>& l() override { return CORE_XCAST(List<E>, UnmodifiableCollection<E>::collection); }

            List<E> const& l() const override { return CORE_XCAST(List<E>, UnmodifiableCollection<E>::collection); }

            const List<E>& subList(gint fromIndex, gint toIndex) const override {
                try {
                    return UNSAFE::newInstance<UnmodifiableRandomAccessList<E>>(l().subList(fromIndex, toIndex));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            List<E>& subList(gint fromIndex, gint toIndex) override {
                try {
                    return UNSAFE::newInstance<UnmodifiableRandomAccessList<E>>(l().subList(fromIndex, toIndex));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const List<E>& reversed() const override {
                try {
                    return UNSAFE::newInstance<UnmodifiableRandomAccessList<E>>(l().reversed());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            List<E>& reversed() override {
                try {
                    return UNSAFE::newInstance<UnmodifiableRandomAccessList<E>>(l().reversed());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class K, class V>
        class Collections::UnmodifiableMap : public Map<K, V> {
        public:
            CORE_ALIAS(Keys, typename Class<Set<K>>::Pointer);
            CORE_ALIAS(Vals, typename Class<Collection<V>>::Pointer);
            CORE_ALIAS(Entries, typename Class<Set<Entry<K, V>>>::Pointer);

            Map<K, V>& map;

            CORE_IMPORT_FIELD_OR_METHOD($(Map<K, V>), keys);
            CORE_IMPORT_FIELD_OR_METHOD($(Map<K, V>), vals);
            Entries mutable entries = null;

            CORE_EXPLICIT UnmodifiableMap(Map<K, V>& map): map(map) {}

            gint size() const override {
                try { return map.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return map.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsKey(K const& key) const override {
                try { return map.containsKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsValue(V const& value) const override {
                try { return map.containsValue(value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& get(K const& key) override {
                try { return map.get(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V const& get(K const& key) const override {
                try { return map.get(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& put(K const& key, V const& value) override { UnsupportedOperationException().throws($ftrace()); }

            V& remove(K const& key) override { UnsupportedOperationException().throws($ftrace()); }

            void putAll(Map<K, V> const& m) override { UnsupportedOperationException().throws($ftrace()); }

            void clear() override { UnsupportedOperationException().throws($ftrace()); }

            Set<K>& keySet() override {
                try {
                    if (keys == null)
                        keys = &unmodifiableSet(map.keySet());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                return *keys;
            }

            Set<K> const& keySet() const override {
                try {
                    if (keys == null)
                        keys = &unmodifiableSet(map.keySet());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                return *keys;
            }

            Collection<V>& values() override {
                try {
                    if (vals == null)
                        vals = &unmodifiableCollection(map.values());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                return *vals;
            }

            Collection<V> const& values() const override {
                try {
                    if (vals == null)
                        vals = &unmodifiableCollection(map.values());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                return *vals;
            }

            Set<Entry<K, V>>& entrySet() override {
                try {
                    if (entries == null)
                        entries = &unmodifiableSet(map.entrySet());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                return *entries;
            }

            Set<Entry<K, V>> const& entrySet() const override {
                try {
                    if (entries == null)
                        entries = &unmodifiableSet(map.entrySet());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                return *entries;
            }

            gbool equals(Object const& o) const override {
                try { return this == &o || map == o; } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try { return map.hash(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try { return map.toString(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& getOrDefault(K const& key, V const& defaultValue) override {
                try {
                    return map.getOrDefault(key, defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V const& getOrDefault(K const& key, V const& defaultValue) const override {
                try {
                    return map.getOrDefault(key, defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& getOrNull(K const& key) override {
                try { return map.getOrNull(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object const& getOrNull(K const& key) const override {
                try { return map.getOrNull(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::BiConsumer<K&, V&> const& action) override {
                try { return map.forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::BiConsumer<K, V> const& action) const override {
                try { return map.forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void replaceAll(function::BiFunction<K&, V&, V> const& function) override {
                UnsupportedOperationException().throws($ftrace());
            }

            Object& putIfAbsent(K const& key, V const& value) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool remove(K const& key, V const& value) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool replace(K const& key, V const& oldValue, V const& newValue) override {
                UnsupportedOperationException().throws($ftrace());
            }

            Object& replace(K const& key, V const& value) override {
                UnsupportedOperationException().throws($ftrace());
            }

            ~UnmodifiableMap() override {
                if (keys != null) {
                    UNSAFE::deleteRegInstance(*keys);
                    keys = null;
                }

                if (vals != null) {
                    UNSAFE::deleteRegInstance(*vals);
                    vals = null;
                }

                if (entries != null) {
                    UNSAFE::deleteRegInstance(*entries);
                    entries = null;
                }
            }
        };

        template <class K, class V>
        class Collections::UnmodifiableEntrySet final : public UnmodifiableSet<Entry<K, V>> {
        public:
            CORE_EXPLICIT UnmodifiableEntrySet(Set<Entry<K, V>>& s): UnmodifiableSet<Entry<K, V>>(s) {}

            Set<Entry<K, V>>& set() {
                return CORE_XCAST($(Set<Entry<K, V>>), UnmodifiableSet<Entry<K, V>>::collection);
            }

            Set<Entry<K, V>> const& set() const {
                return CORE_XCAST($(Set<Entry<K, V>>), UnmodifiableSet<Entry<K, V>>::collection);
            }

            static function::Consumer<Entry<K, V>> entryConsumer(function::Consumer<Entry<K, V>> const& action) {
                return [&](Entry<K, V> const& e) {
                    Entry<K, V>& entry = UNSAFE::newInstance<UnmodifiableEntry<K, V>>(e);
                    try { action.accept(entry); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    UNSAFE::deleteRegInstance(entry);
                };
            }

            static function::Consumer<Entry<K, V>&> entryConsumer(function::Consumer<Entry<K, V>&> const& action) {
                return [&](Entry<K, V>& e) {
                    Entry<K, V>& entry = UNSAFE::newInstance<UnmodifiableEntry<K, V>>(e);
                    try { action.accept(entry); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    UNSAFE::deleteRegInstance(entry);
                };
            }

            void forEach(function::Consumer<Entry<K, V>&> const& action) override {
                try { set().forEach(entryConsumer(action)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<Entry<K, V>> const& action) const override {
                try { set().forEach(entryConsumer(action)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<Entry<K, V>>& iterator() override {
                class Itr final : public Iterator<Entry<K, V>> {
                    Iterator<Entry<K, V>>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry<K, V>>& it): it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Entry<K, V>& next() override {
                        try {
                            return UNSAFE::newInstance<UnmodifiableEntry<K, V>>(it.next());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override { UnsupportedOperationException().throws($ftrace()); }

                    void forEach(function::Consumer<Entry<K, V>&> const& action) override {
                        try {
                            it.forEach(entryConsumer(action));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(Object const& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteRegInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(set().iterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry<K, V>>& iterator() const override {
                class Itr final : public Iterator2<Entry<K, V>> {
                    Iterator2<Entry<K, V>>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry<K, V>>& it): it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Entry<K, V> const& next() override {
                        try {
                            return UNSAFE::newInstance<UnmodifiableEntry<K, V>>(it.next());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override { UnsupportedOperationException().throws($ftrace()); }

                    void forEach(function::Consumer<Entry<K, V>> const& action) override {
                        try {
                            it.forEach(entryConsumer(action));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(Object const& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteRegInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(set().iterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator<Entry<K, V>>& spliterator() override {
                class Spl final : public Spliterator<Entry<K, V>> {
                    Spliterator<Entry<K, V>>& spl;

                public:
                    CORE_EXPLICIT Spl(Spliterator<Entry<K, V>>& spl): spl(spl) {}

                    gbool tryAdvance(function::Consumer<Entry<K, V>&> const& action) override {
                        try {
                            return spl.tryAdvance(entryConsumer(action));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void forEach(function::Consumer<Entry<K, V>&> const& action) override {
                        try {
                            spl.forEach(entryConsumer(action));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Spliterator<Entry<K, V>>& trySplit() override {
                        try {
                            return spl.trySplit();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Spliterator<Entry<K, V>> const& trySplit() const override {
                        try {
                            Spliterator<Entry<K, V>>& split = spl.trySplit();
                            return &split == &spl ? *this : UNSAFE::newInstance<Spl>(split);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    glong estimateSize() const override {
                        try {
                            return spl.estimateSize();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    glong getExactSizeIfKnown() const override {
                        try {
                            return spl.getExactSizeIfKnown();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gint characteristics() const override {
                        try {
                            return spl.characteristics();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool hasCharacteristics(gint characteristics) const override {
                        try {
                            return spl.hasCharacteristics(characteristics);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Comparator<Entry<K, V>> const& comparator() const override {
                        try {
                            return spl.comparator();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(Object const& o) const override {
                        return this == &o || Class<Spl>::hasInstance(o) && spl == CORE_XCAST(Spl const, o).spl;
                    }
                };

                return UNSAFE::newInstance<Spl>(set().spliterator());
            }

            Spliterator2<Entry<K, V>>& spliterator() const override {
                class Spl final : public Spliterator2<Entry<K, V>> {
                    Spliterator2<Entry<K, V>>& spl;

                public:
                    CORE_EXPLICIT Spl(Spliterator2<Entry<K, V>>& spl): spl(spl) {}

                    gbool tryAdvance(function::Consumer<Entry<K, V>> const& action) override {
                        try {
                            return spl.tryAdvance(entryConsumer(action));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void forEach(function::Consumer<Entry<K, V>> const& action) override {
                        try {
                            spl.forEach(entryConsumer(action));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Spliterator2<Entry<K, V>>& trySplit() override {
                        try {
                            return spl.trySplit();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Spliterator<Entry<K, V>> const& trySplit() const override {
                        try {
                            Spliterator<Entry<K, V>>& split = spl.trySplit();
                            return &split == &spl ? *this : UNSAFE::newInstance<Spl>(split);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    glong estimateSize() const override {
                        try {
                            return spl.estimateSize();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    glong getExactSizeIfKnown() const override {
                        try {
                            return spl.getExactSizeIfKnown();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gint characteristics() const override {
                        try {
                            return spl.characteristics();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool hasCharacteristics(gint characteristics) const override {
                        try {
                            return spl.hasCharacteristics(characteristics);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Comparator<Entry<K, V>> const& comparator() const override {
                        try {
                            return spl.comparator();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(Object const& o) const override {
                        return this == &o || Class<Spl>::hasInstance(o) && spl == CORE_XCAST(Spl const, o).spl;
                    }
                };

                return UNSAFE::newInstance<Spl>(set().spliterator());
            }

            Array<Entry<K, V>> toArray() const override {
                try {
                    Array<Entry<K, V>> a = set().toArray();
                    for (gint i = 0; i < a.length(); i++)
                        Arrays::fastSet(a, i, UNSAFE::newInstance<UnmodifiableEntry<K, V>>(a[i]));
                    return a;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                // try {  } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool contains(Entry<K, V> const& val) const override {
                try {
                    Entry<K, V>& e = *new UnmodifiableEntry<K, V>((Entry<K, V>&) val);
                    gbool r = set().contains(e);
                    delete &e;
                    return r;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(Collection<Entry<K, V>> const& c) const override {
                for (Entry<K, V> const& e : set()) {
                    try {
                        if (!contains(e))
                            return false;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
                return true;
            }

            gbool equals(Object const& o) const override {
                if (this == &o) return true;
                if (!Class<Set<Entry<K, V>>>::hasInstance(o)) return false;
                Set<Entry<K, V>> const& s = CORE_XCAST($(Set<Entry<K, V>>) const, o);
                try {
                    return s.size() == set().size() && containsAll(s);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        /**
         * This "wrapper class" serves two purposes: it prevents
         * the client from modifying the backing Map, by short-circuiting
         * the setValue method, and it protects the backing Map against
         * an ill-behaved Map.Entry that attempts to modify another
         * Map Entry when asked to perform an equality check.
         */
        template <class K, class V>
        class Collections::UnmodifiableEntry final : public Entry<K, V> {
            Entry<K, V>& entry;

        public:
            CORE_EXPLICIT UnmodifiableEntry(Entry<K, V> const& entry): entry((Entry<K, V>&) entry) {}

            K& getKey() override {
                try { return entry.getKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K const& getKey() const override {
                try { return entry.getKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& getValue() override {
                try { return entry.getValue(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V const& getValue() const override {
                try { return entry.getValue(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& setValue(V const& value) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool equals(Object const& other) const override {
                try { return this == &other || entry == other; } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try { return entry.hash(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try { return entry.toString(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& clone() const override {
                return UNSAFE::newInstance<UnmodifiableEntry>(*this);
            }
        };

        template <class K, class V>
        class Collections::UnmodifiableSequencedMap : public virtual UnmodifiableMap<K, V>,
                                                      public virtual SequencedMap<K, V> {
            SequencedMap<K, V>& sequencedMap() {
                return CORE_XCAST($(SequencedMap<K, V>), UnmodifiableMap<K, V>::map);
            }

            SequencedMap<K, V> const& sequencedMap() const {
                return CORE_XCAST($(SequencedMap<K, V>), UnmodifiableMap<K, V>::map);
            }

        public:
            CORE_EXPLICIT UnmodifiableSequencedMap(SequencedMap<K, V>& map): UnmodifiableMap<K, V>(map) {}

            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), clear);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), containsKey);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), containsValue);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), entrySet);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), isEmpty);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), keySet);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), put);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), putAll);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), putIfAbsent);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), remove);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), replace);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), replaceAll);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), size);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), toString);
            CORE_IMPORT_FIELD_OR_METHOD($(UnmodifiableMap<K, V>), values);

            SequencedMap<K, V>& reversed() override {
                return UNSAFE::newInstance<UnmodifiableSequencedMap>(sequencedMap().reversed());
            }

            SequencedMap<K, V> const& reversed() const override {
                return UNSAFE::newInstance<UnmodifiableSequencedMap>(sequencedMap().reversed());
            }

            Object& pollFirstEntry() override { UnsupportedOperationException().throws($ftrace()); }

            Object& pollLastEntry() override { UnsupportedOperationException().throws($ftrace()); }

            Object& putFirst(K const& k, V const& v) override { UnsupportedOperationException().throws($ftrace()); }

            Object& putLast(K const& k, V const& v) override { UnsupportedOperationException().throws($ftrace()); }
        };

        template <class K, class V>
        class Collections::UnmodifiableSortedMap : public UnmodifiableMap<K, V> {};

        template <class K, class V>
        class Collections::UnmodifiableNavigableMap : public UnmodifiableMap<K, V> {};
    } // util
} // core

#endif //CORE24_COLLECTIONS_H
