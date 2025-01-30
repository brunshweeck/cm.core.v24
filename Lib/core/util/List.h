//
// Created by brunshweeck on 30 juin 2024.
//

#ifndef CORE24_LIST_H
#define CORE24_LIST_H

#include "RandomAccess.h"
#include "SequencedCollection.h"

#include <core/IllegalStateException.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/function/UnaryOperator.h>
#include <core/util/Arrays.h>
#include <core/util/Comparator.h>
#include <core/util/ConcurrentModificationException.h>
#include <core/util/ListIterator.h>

namespace core {
    namespace util {
        /**
         * An ordered collection, where the user has precise control over where in the
         * list each element is inserted.  The user can access elements by their integer
         * index (position in the list), and search for elements in the list.
         * <p>
         * Unlike sets, lists typically allow duplicate elements.  More formally,
         * lists typically allow pairs of elements @c e1 and @c e2
         * such that @c e1.equals(e2), and they typically allow multiple
         * null elements if they allow null elements at all.  It is not inconceivable
         * that someone might wish to implement a list that prohibits duplicates, by
         * throwing runtime exceptions when the user attempts to insert them, but we
         * expect this usage to be rare.
         * </p>
         * <p>
         * The @c List interface places additional stipulations, beyond those
         * specified in the @c Collection interface, on the contracts of the
         * @c iterator, @c add, @c remove, @c equals, and
         * @c hash methods.  Declarations for other inherited methods are
         * also included here for convenience.
         * </p>
         * <p>
         * The @c List interface provides four methods for positional (indexed)
         * access to list elements.  Lists (like arrays classes) are zero based.  Note
         * that these operations may execute in time proportional to the index value
         * for some implementations (the @c LinkedList class, for
         * example). Thus, iterating over the elements in a list is typically
         * preferable to indexing through it if the caller does not know the
         * implementation.
         * </p>
         * <p>
         * The @c List interface provides a special iterator, called a
         * @c ListIterator, that allows element insertion and replacement, and
         * bidirectional access in addition to the normal operations that the
         * @c Iterator interface provides.  A method is provided to obtain a
         * list iterator that starts at a specified position in the list.
         * </p>
         * <p>
         * The @c List interface provides two methods to search for a specified
         * object.  From a performance standpoint, these methods should be used with
         * caution.  In many implementations they will perform costly linear
         * searches.
         * </p>
         * <p>
         * The @c List interface provides two methods to efficiently insert and
         * remove multiple elements at an arbitrary point in the list.
         * </p>
         * <p>
         * Note: While it is permissible for lists to contain themselves as elements,
         * extreme caution is advised: the @c equals and @c hash
         * methods are no longer well-defined on such a list.
         * </p>
         * <p>
         * Some list implementations have restrictions on the elements that
         * they may contain.  For example, some implementations prohibit null elements,
         * and some have restrictions on the types of their elements.  Attempting
         * to query the presence of an ineligible element may throw an exception,
         * or it may simply return false; some implementations will exhibit the former
         * behavior and some will exhibit the latter.  More generally, attempting an
         * operation on an ineligible element whose completion would not result in
         * the insertion of an ineligible element into the list may throw an
         * exception, or it may succeed, at the option of the implementation.
         * Such exceptions are marked as "optional" in the specification for this
         * interface.
         * </p>
         * <h2><em>Unmodifiable Lists</em></h2>
         * <p>
         * The @c List::of and  @c List::copyOf static factory methods
         * provide a convenient way to create unmodifiable lists. The @c List
         * instances created by these methods have the following characteristics:
         *
         * - They are <i>unmodifiable</i>. Elements cannot
         * be added, removed, or replaced. Calling any mutator method on the List
         * will always cause @c UnsupportedOperationException to be thrown.
         * However, if the contained elements are themselves mutable,
         * this may cause the List's contents to appear to change.
         * - They are serializable if all elements are serializable.
         * - The order of elements in the list is the same as the order of the
         * provided arguments, or of the elements in the provided array.
         * - The lists and their @c subList views implement the @c RandomAccess interface.
         * - They are value-based.
         * Programmers should treat instances that are @c equal
         * as interchangeable and should not use them for synchronization, or
         * unpredictable behavior may occur. For example, in a future release,
         * synchronization may fail. Callers should make no assumptions about the
         * identity of the returned instances. Factories are free to
         * create new instances or reuse existing ones.
         * - They are serialized as specified on the Serialized Form page.
         *
         * </p>
         * @tparam E the type of elements in this list
         *
         * @see Collection
         * @see Set
         * @see ArrayList
         * @see LinkedList
         * @see Vector
         */
        template<class E>
        class List : public virtual SequencedCollection<E> {
            CORE_CHECK_SLIMMED_TYPE(E);
            CORE_ADD_GLOBAL_FRIENDS();

            CORE_ALIAS(ListView, typename Class<List<E>>::Pointer);
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(PRE, misc::Preconditions);

        public:
            // Query Operations

            /**
             * Returns the number of elements in this list.  If this list contains
             * more than @c Integer::MAX_VALUE elements, returns @c Integer::MAX_VALUE.
             *
             * @return the number of elements in this list
             */
            gint size() const override = 0;

            /**
             * Returns @c true if this list contains no elements.
             *
             * @return @c true if this list contains no elements
             */
            gbool isEmpty() const override {
                try { return size() == 0; } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this list contains the specified element.
             * More formally, returns @c true if and only if this list contains
             * at least one element @c e such that @code Objects.equals(o, e) @endcode.
             *
             * @param val element whose presence in this list is to be tested
             * @return @c true if this list contains the specified element
             */
            gbool contains(const E &val) const override {
                try {
                    return SequencedCollection<E>::contains(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }


            /**
             * Returns an iterator over the elements in this list in proper sequence.
             *
             * @return an iterator over the elements in this list in proper sequence
             */
            Iterator<E> &iterator() override {
                try { return listIterator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this list in proper sequence.
             *
             * @return an iterator over the elements in this list in proper sequence
             */
            Iterator2<E> &iterator() const override {
                try { return listIterator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an array containing all the elements in this list in proper
             * sequence (from first to last element).
             *
             * <p>
             * The returned array will be "safe" in that no references to it are
             * maintained by this list.  (In other words, this method must
             * allocate a new array even if this list is backed by an array).
             * The caller is thus free to modify the returned array.
             * </p>
             * <p>
             * This method acts as bridge between array-based and collection-based
             * APIs.
             * </p>
             * @return an array containing all the elements in this list in proper
             *         sequence
             */
            Array<E> toArray() const override {
                try { return SequencedCollection<E>::toArray(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }


            // Modification Operations

            /**
             * Appends the specified element to the end of this list (optional
             * operation).
             *
             * <p>
             * Lists that support this operation may place limitations on what
             * elements may be added to this list.  In particular, some
             * lists will refuse to add null elements, and others will impose
             * restrictions on the type of elements that may be added.  List
             * classes should clearly specify in their documentation any restrictions
             * on what elements may be added.
             * </p>
             * @param val element to be appended to this list
             * @return @c true (as specified by @c Collection::add)
             * @throws UnsupportedOperationException if the @c add operation
             * @throws IllegalArgumentException if some property of this element
             *         prevents it from being added to this list
             */
            gbool add(E const &val) override {
                try { add(size(), val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return true;
            }

            /**
             * Removes the first occurrence of the specified element from this list,
             * if it is present (optional operation).  If this list does not contain
             * the element, it is unchanged.  More formally, removes the element with
             * the lowest index @c i such that @code Objects.equals(o, get(i)) @endcode
             * (if such an element exists).  Returns @c true if this list
             * contained the specified element (or equivalently, if this list changed
             * as a result of the call).
             *
             * @param val element to be removed from this list, if present
             * @return @c true if this list contained the specified element
             * @throws UnsupportedOperationException if the @c remove operation
             *         is not supported by this list
             */
            gbool remove(E const &val) override {
                try {
                    return SequencedCollection<E>::remove(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }


            // Bulk Modification Operations

            /**
             * Returns @c true if this list contains all the elements of the
             * specified collection.
             *
             * @param  c collection to be checked for containment in this list
             * @return @c true if this list contains all the elements of the
             *         specified collection
             */
            gbool containsAll(const Collection<E> &c) const override {
                try {
                    return SequencedCollection<E>::containsAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Appends all the elements in the specified collection to the end of
             * this list, in the order that they are returned by the specified
             * collection's iterator (optional operation).  The behavior of this
             * operation is undefined if the specified collection is modified while
             * the operation is in progress.  (Note that this will occur if the
             * specified collection is this list, and it's nonempty.)
             *
             * @param c collection containing elements to be added to this list
             * @return @c true if this list changed as a result of the call
             * @throws UnsupportedOperationException if the @c addAll operation
             *         is not supported by this list
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this list
             */
            gbool addAll(const Collection<E> &c) override {
                try { return addAll(size(), c); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts all the elements in the specified collection into this
             * list at the specified position (optional operation).  Shifts the
             * element currently at that position (if any) and any subsequent
             * elements to the right (increases their indices).  The new elements
             * will appear in this list in the order that they are returned by the
             * specified collection's iterator.  The behavior of this operation is
             * undefined if the specified collection is modified while the
             * operation is in progress.  (Note that this will occur if the specified
             * collection is this list, and it's nonempty.)
             *
             * @param index index at which to insert the first element from the
             *              specified collection
             * @param c collection containing elements to be added to this list
             * @return @c true if this list changed as a result of the call
             * @throws UnsupportedOperationException if the @c addAll operation
             *         is not supported by this list
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this list
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index > size() @endcode)
             */
            virtual gbool addAll(gint index, const Collection<E> &c) {
                try {
                    PRE::checkIndex(index, size());
                    gbool modified = false;
                    for (E const &val: c) {
                        add(index++, val);
                        modified = true;
                    }
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes from this list all of its elements that are contained in the
             * specified collection (optional operation).
             *
             * @param c collection containing elements to be removed from this list
             * @return @c true if this list changed as a result of the call
             * @throws UnsupportedOperationException if the @c removeAll operation
             *         is not supported by this list
             */
            gbool removeAll(const Collection<E> &c) override {
                try {
                    return SequencedCollection<E>::removeAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Retains only the elements in this list that are contained in the
             * specified collection (optional operation).  In other words, removes
             * from this list all of its elements that are not contained in the
             * specified collection.
             *
             * @param c collection containing elements to be retained in this list
             * @return @c true if this list changed as a result of the call
             * @throws UnsupportedOperationException if the @c retainAll operation
             *         is not supported by this list
             */
            gbool retainAll(const Collection<E> &c) override {
                try {
                    return SequencedCollection<E>::retainAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Replaces each element of this list with the result of applying the
             * operator to that element (optional operation).  Errors or runtime
             * exceptions thrown by the operator are relayed to the caller.
             *
             * @note
             * The default implementation is equivalent to, for this @c list:
             * @code
             *     ListIterator<E> &li = list.listIterator();
             *     while (li.hasNext()) {
             *         li.set(operator.apply(li.next()));
             *     }
             * @endcode
             *
             * If the list's list-iterator does not support the @c set operation
             * then an @c UnsupportedOperationException will be thrown when
             * replacing the first element.
             *
             * @param op the operator to apply to each element
             * @throws UnsupportedOperationException if the @c replaceAll operation
             *         is not supported by this list
             */
            virtual void replaceAll(function::UnaryOperator<E> const &op) {
                try {
                    ListIterator<E> &it = listIterator();
                    while (it.hasNext()) it.set(op.apply(it.next()));
                    UNSAFE::deleteInstance(it);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Sorts this list according to the order induced by the specified
             * @c Comparator (optional operation).  The sort is <i>stable</i>:
             * this method must not reorder equal elements.
             *
             * <p>
             * All elements in this list must be <i>mutually comparable</i> using the
             * specified comparator (that is, @code c.compare(e1, e2) @endcode must not throw
             * a @c ClassCastException for any elements @c e1 and @c e2 in the list).
             * </p>
             * <p>
             * This list must be modifiable, but need not be resizable.
             * </p>
             * @note
             * The default implementation obtains an array containing all elements in
             * this list, sorts the array, and iterates over this list resetting each
             * element from the corresponding position in the array. (This avoids the
             * n<sup>2</sup> log(n) performance that would result from attempting
             * to sort a linked list in place.)
             *
             * @implNote
             * This implementation is a stable, adaptive, iterative mergesort that
             * requires far fewer than n lg(n) comparisons when the input array is
             * partially sorted, while offering the performance of a traditional
             * mergesort when the input array is randomly ordered.  If the input array
             * is nearly sorted, the implementation requires approximately n
             * comparisons.  Temporary storage requirements vary from a small constant
             * for nearly sorted input arrays to n/2 object references for randomly
             * ordered input arrays.
             *
             * <p>
             * The implementation takes equal advantage of ascending and
             * descending order in its input array, and can take advantage of
             * ascending and descending order in different parts of the same
             * input array.  It is well-suited to merge two or more sorted arrays:
             * simply concatenate the arrays and sort the resulting array.
             * </p>
             * <p>
             * The implementation was adapted from Tim Peters's list sort for Python
             * (TimSort).  It uses techniques from PETER MCILROY is "Optimistic
             * Sorting and Information Theoretic Complexity", in Proceedings of the
             * Fourth Annual ACM-SIAM Symposium on Discrete Algorithms, pp 467-474,
             * January 1993.
             * </p>
             *
             * @param c the @c Comparator used to compare list elements.
             * @throws UnsupportedOperationException if the @c sort operation
             *         is not supported by this list
             * @throws IllegalArgumentException (@em optional)
             *         if the comparator is found to violate the @c Comparator
             *         contract
             */
            virtual void sort(Comparator<E> const &c) {
                try {
                    Array<E> array = toArray();
                    Arrays::sort(array, c);
                    ListIterator<E> &it = listIterator();

                    for (E const &e: array) {
                        it.next();
                        it.set(e);
                    }

                    UNSAFE::deleteInstance(it);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all the elements from this list (optional operation).
             * The list will be empty after this call returns.
             *
             * @throws UnsupportedOperationException if the @c clear operation
             *         is not supported by this list
             */
            void clear() override {
                try {
                    ListIterator<E> &it = listIterator();

                    while (it.hasNext()) {
                        it.next();
                        it.remove();
                    }

                    UNSAFE::deleteInstance(it);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }


            // Comparison and hashing

            /**
             * Compares the specified object with this list for equality.  Returns
             * @c true if and only if the specified object is also a list, both
             * lists have the same size, and all corresponding pairs of elements in
             * the two lists are <i>equal</i>.  (Two elements @c e1 and
             * @c e2 are <i>equal</i> if @code Objects.equals(e1, e2) @endcode.)
             * In other words, two lists are defined to be
             * equal if they contain the same elements in the same order.  This
             * definition ensures that the equals method works properly across
             * different implementations of the @c List interface.
             *
             * @param o the object to be compared for equality with this list
             * @return @c true if the specified object is equal to this list
             */
            gbool equals(const Object &o) const override {
                if (this == &o) {
                    return true;
                }
                if (Class<List>::hasInstance(o)) {
                    List const &list = CORE_XCAST(List const, o);
                    if (size() != list.size()) {
                        return false;
                    }
                    try {
                        ListIterator2<E> &it1 = listIterator();
                        ListIterator2<E> &it2 = list.listIterator();

                        while (it1.hasNext() && it2.hasNext()) {
                            E const &e1 = it1.next();
                            E const &e2 = it2.next();

                            if (!e1.equals(e2)) {
                                UNSAFE::deleteInstance(it1);
                                UNSAFE::deleteInstance(it2);
                                return false;
                            }

                            UNSAFE::deleteInstance(it1);
                            UNSAFE::deleteInstance(it2);
                            return true;
                        }
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                }
                return false;
            }

            /**
             * Returns the hash code value for this list.  The hash code of a list
             * is defined to be the result of the following calculation:
             * @code
             *     int hash = 1;
             *     for (E const &e : list)
             *         hash = 31 * hash + e.hash();
             * @endcode
             * This ensures that @c list1.equals(list2) implies that
             * @code list1.hash() == list2.hash() @endcode for any two lists,
             * @c list1 and @c list2, as required by the general
             * contract of @c Object::hash.
             *
             * @return the hash code value for this list
             */
            gint hash() const override {
                if (isEmpty())
                    return 0;

                try {
                    ListIterator2<E> &it = listIterator();
                    gint n = size();
                    gint hash = 0;

                    while (it.hasNext()) {
                        Object const &o = it.next();
                        hash = 31 * hash + (o.hash() ^ (--n));
                    }

                    UNSAFE::deleteInstance(it);
                    return hash;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // Positional Access Operations

            /**
             * Returns the element at the specified position in this list.
             *
             * @param index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index >= size() @endcode )
             */
            virtual E &get(gint index) = 0;

            /**
             * Returns the element at the specified position in this list.
             *
             * @param index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index >= size() @endcode )
             */
            virtual E const &get(gint index) const = 0;

            /**
             * Replaces the element at the specified position in this list with the
             * specified element (optional operation).
             *
             * @param index index of the element to replace
             * @param newValue element to be stored at the specified position
             * @return the element previously at the specified position
             * @throws UnsupportedOperationException if the @c set operation
             *         is not supported by this list
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this list
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index >= size() @endcode )
             */
            virtual E &set(gint index, E const &newValue) {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Inserts the specified element at the specified position in this list
             * (optional operation).  Shifts the element currently at that position
             * (if any) and any subsequent elements to the right (adds one to their
             * indices).
             *
             * @param index index at which the specified element is to be inserted
             * @param val Element to be inserted
             * @throws UnsupportedOperationException if the @c add operation
             *         is not supported by this list
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this list
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index > size() @endcode )
             */
            virtual void add(gint index, E const &val) {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Removes the element at the specified position in this list (optional
             * operation).  Shifts any subsequent elements to the left (subtracts one
             * from their indices).  Returns the element that was removed from the
             * list.
             *
             * @param index the index of the element to be removed
             * @return the element previously at the specified position
             * @throws UnsupportedOperationException if the @c remove operation
             *         is not supported by this list
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index >= size() @endcode )
             */
            virtual E &remove(gint index) {
                UnsupportedOperationException().throws($ftrace());
            }


            // Search Operations

            /**
             * Returns the index of the first occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the lowest index @c i such that
             * @c Objects.equals(o, get(i)), or -1 if there is no such index.
             *
             * @param val element to search for
             * @return the index of the first occurrence of the specified element in
             *         this list, or -1 if this list does not contain the element
             */
            virtual gint indexOf(E const &val) const {
                try {
                    ListIterator2<E> &it = listIterator();
                    gint i = -1;

                    while (it.hasNext()) {
                        if (val.equals(it.next())) {
                            i = it.previousIndex();
                            break;
                        }
                    }

                    UNSAFE::deleteInstance(it);
                    return i;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the index of the last occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the highest index @c i such that
             * @c Objects.equals(o, get(i)), or -1 if there is no such index.
             *
             * @param val element to search for
             * @return the index of the last occurrence of the specified element in
             *         this list, or -1 if this list does not contain the element
             */
            virtual gint lastIndexOf(E const &val) const {
                try {
                    ListIterator2<E> &it = listIterator(size());
                    gint i = -1;

                    while (it.hasPrevious()) {
                        if (val.equals(it.previous())) {
                            i = it.nextIndex();
                            break;
                        }
                    }

                    UNSAFE::deleteInstance(it);
                    return i;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // List Iterators

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence).
             *
             * @return a list iterator over the elements in this list (in proper
             *         sequence)
             */
            virtual ListIterator<E> &listIterator() {
                try { return listIterator(0); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence).
             *
             * @return a list iterator over the elements in this list (in proper
             *         sequence)
             */
            virtual ListIterator2<E> &listIterator() const {
                try { return listIterator(0); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to @c ListIterator::next.
             * An initial call to @c ListIterator::previous would
             * return the element with the specified index minus one.
             *
             * @param index index of the first element to be returned from the
             *        list iterator (by a call to @c ListIterator::next)
             * @return a list iterator over the elements in this list (in proper
             *         sequence), starting at the specified position in the list
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index > size() @endcode)
             */
            virtual ListIterator<E> &listIterator(gint index) {
                class Itr final : public ListIterator<E> {
                    List<E> &list;
                    gint cursor;
                    gint last = -1;
                    gint modCount;

                public:
                    CORE_EXPLICIT Itr(List<E> &list, gint cursor)
                        : list(list), cursor(cursor), modCount(list.modCount) {
                    }

                    gbool hasNext() const override { return cursor < list.size(); }

                    E & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());
                        if (modCount != list.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor++;
                        try { return list.get(last); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool hasPrevious() const override { return cursor > 0; }

                    E & previous() override {
                        if (!hasPrevious())
                            NoSuchElementException().throws($ftrace());
                        if (modCount != list.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = --cursor;
                        try { return list.get(last); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gint nextIndex() const override { return cursor; }

                    gint previousIndex() const override { return cursor - 1; }

                    void remove() override {
                        if (last < 0)
                            IllegalStateException().throws($ftrace());
                        if (modCount != list.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        try { list.remove(last); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        if (cursor >= last)
                            cursor--;
                        last = -1;
                        modCount = list.modCount;
                    }

                    void set(const E &e) override {
                        if (last < 0)
                            IllegalStateException().throws($ftrace());
                        if (modCount != list.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        try { list.set(last, e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    void add(const E &e) override {
                        if (last < 0)
                            IllegalStateException().throws($ftrace());
                        if (modCount != list.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        try { list.add(last, e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        last = -1;
                        modCount = list.modCount;
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;
                        Itr const &it = CORE_XCAST(Itr const, o);
                        return &list == &it.list && cursor == it.cursor;
                    }
                };

                try {
                    PRE::checkIndexForAdding(index, size());
                    return UNSAFE::newInstance<Itr>(*this, index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to @c ListIterator::next.
             * An initial call to @c ListIterator::previous would
             * return the element with the specified index minus one.
             *
             * @param index index of the first element to be returned from the
             *        list iterator (by a call to @c ListIterator::next)
             * @return a list iterator over the elements in this list (in proper
             *         sequence), starting at the specified position in the list
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index > size() @endcode)
             */
            virtual ListIterator2<E> &listIterator(gint index) const {
                class Itr final : public ListIterator2<E> {
                    List<E> const &list;
                    gint cursor;
                    gint last = -1;
                    gint modCount;

                public:
                    CORE_EXPLICIT Itr(List<E> const &list, gint cursor)
                        : list(list), cursor(cursor), modCount(list.modCount) {
                    }

                    gbool hasNext() const override { return cursor < list.size(); }

                    E const & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());
                        if (modCount != list.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor++;
                        try { return list.get(last); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool hasPrevious() const override { return cursor > 0; }

                    E const & previous() override {
                        if (!hasPrevious())
                            NoSuchElementException().throws($ftrace());
                        if (modCount != list.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = --cursor;
                        try { return list.get(last); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gint nextIndex() const override { return cursor; }

                    gint previousIndex() const override { return cursor - 1; }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;
                        Itr const &it = CORE_XCAST(Itr const, o);
                        return &list == &it.list && cursor == it.cursor;
                    }
                };

                try {
                    PRE::checkIndexForAdding(index, size());
                    return UNSAFE::newInstance<Itr>(*this, index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // View

            /**
             * Returns a view of the portion of this list between the specified
             * @c fromIndex, inclusive, and @c toIndex, exclusive.  (If
             * @c fromIndex and @c toIndex are equal, the returned list is
             * empty.)  The returned list is backed by this list, so non-structural
             * changes in the returned list are reflected in this list, and vice versa.
             * The returned list supports all the optional list operations supported
             * by this list.
             * <p>
             * This method eliminates the need for explicit range operations (of
             * the sort that commonly exist for arrays).  Any operation that expects
             * a list can be used as a range operation by passing a subList view
             * instead of a whole list.  For example, the following idiom
             * removes a range of elements from a list:
             * @code
             *      list.subList(from, to).clear();
             * @endcode
             * Similar idioms may be constructed for @c indexOf and
             * @c lastIndexOf, and all the algorithms in the
             * @c Collections class can be applied to a subList.
             * </p>
             * <p>
             * The semantics of the list returned by this method become undefined if
             * the backing list (i.e., this list) is <i>structurally modified</i> in
             * any way other than via the returned list.  (Structural modifications are
             * those that change the size of this list, or otherwise perturb it in such
             * a fashion that iterations in progress may yield incorrect results.)
             * </p>
             * @param fromIndex low endpoint (inclusive) of the subList
             * @param toIndex high endpoint (exclusive) of the subList
             * @return a view of the specified range within this list
             * @throws IndexOutOfBoundsException for an illegal endpoint index value
             *         (@code fromIndex < 0 || toIndex > size || fromIndex > toIndex @endcode)
             */
            virtual List const &subList(gint fromIndex, gint toIndex) const {
                try {
                    PRE::checkIndexFromRange(fromIndex, toIndex, size());
                    return UNSAFE::newInstance<SubList>(CORE_CAST(List&, *this), fromIndex, toIndex - fromIndex);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this list between the specified
             * @c fromIndex, inclusive, and @c toIndex, exclusive.  (If
             * @c fromIndex and @c toIndex are equal, the returned list is
             * empty.)  The returned list is backed by this list, so non-structural
             * changes in the returned list are reflected in this list, and vice versa.
             * The returned list supports all the optional list operations supported
             * by this list.
             * <p>
             * This method eliminates the need for explicit range operations (of
             * the sort that commonly exist for arrays).  Any operation that expects
             * a list can be used as a range operation by passing a subList view
             * instead of a whole list.  For example, the following idiom
             * removes a range of elements from a list:
             * @code
             *      list.subList(from, to).clear();
             * @endcode
             * Similar idioms may be constructed for @c indexOf and
             * @c lastIndexOf, and all the algorithms in the
             * @c Collections class can be applied to a subList.
             * </p>
             * <p>
             * The semantics of the list returned by this method become undefined if
             * the backing list (i.e., this list) is <i>structurally modified</i> in
             * any way other than via the returned list.  (Structural modifications are
             * those that change the size of this list, or otherwise perturb it in such
             * a fashion that iterations in progress may yield incorrect results.)
             * </p>
             * @param fromIndex low endpoint (inclusive) of the subList
             * @param toIndex high endpoint (exclusive) of the subList
             * @return a view of the specified range within this list
             * @throws IndexOutOfBoundsException for an illegal endpoint index value
             *         (@code fromIndex < 0 || toIndex > size || fromIndex > toIndex @endcode)
             */
            virtual List &subList(gint fromIndex, gint toIndex) {
                try {
                    PRE::checkIndexFromRange(fromIndex, toIndex, size());
                    return UNSAFE::newInstance<SubList>(*this, fromIndex, toIndex - fromIndex);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // ========== SequencedCollection ==========

            /**
             * Adds an element as the first element of this collection (optional operation).
             * After this operation completes normally, the given element will be a member of
             * this collection, and it will be the first element in encounter order.
             *
             * @note
             * The implementation in this interface always throws @c UnsupportedOperationException.
             *
             * @param val the element to be added
             * @throws UnsupportedOperationException if this collection implementation
             *         does not support this operation
             */
            void addFirst(E const &val) override {
                try { add(0, val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Adds an element as the last element of this collection (optional operation).
             * After this operation completes normally, the given element will be a member of
             * this collection, and it will be the last element in encounter order.
             *
             * @note
             * The implementation in this interface always throws @c UnsupportedOperationException.
             *
             * @param val the element to be added.
             * @throws UnsupportedOperationException if this collection implementation
             *         does not support this operation
             */
            void addLast(E const &val) override {
                try { add(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Gets the first element of this collection.
             *
             * @note
             * The implementation in this interface obtains an iterator of this collection, and
             * then it obtains an element by calling the iterator's @c next method. Any
             * @c NoSuchElementException thrown is propagated. Otherwise, it returns
             * the element.
             *
             * @return the retrieved element
             * @throws NoSuchElementException if this collection is empty
             */
            E &firstElement() override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());

                try { return get(0); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Gets the first element of this collection.
             *
             * @note
             * The implementation in this interface obtains an iterator of this collection, and
             * then it obtains an element by calling the iterator's @c next method. Any
             * @c NoSuchElementException thrown is propagated. Otherwise, it returns
             * the element.
             *
             * @return the retrieved element
             * @throws NoSuchElementException if this collection is empty
             */
            E const &firstElement() const override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());

                try { return get(0); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Gets the last element of this collection.
             *
             * @note
             * The implementation in this interface obtains an iterator of the reversed view
             * of this collection, and then it obtains an element by calling the iterator's
             * @c next method. Any @c NoSuchElementException thrown is propagated.
             * Otherwise, it returns the element.
             *
             * @return the retrieved element
             * @throws NoSuchElementException if this collection is empty
             */
            E &lastElement() override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());

                try { return get(size() - 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Gets the last element of this collection.
             *
             * @note
             * The implementation in this interface obtains an iterator of the reversed view
             * of this collection, and then it obtains an element by calling the iterator's
             * @c next method. Any @c NoSuchElementException thrown is propagated.
             * Otherwise, it returns the element.
             *
             * @return the retrieved element
             * @throws NoSuchElementException if this collection is empty
             */
            E const &lastElement() const override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());

                try { return get(size() - 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes and returns the first element of this collection (optional operation).
             *
             * @note
             * The implementation in this interface obtains an iterator of this collection, and then
             * it obtains an element by calling the iterator's @c next method. Any
             * @c NoSuchElementException thrown is propagated. It then calls the iterator's
             * @c remove method. Any @c UnsupportedOperationException thrown is propagated.
             * Then, it returns the element.
             *
             * @return the removed element
             * @throws NoSuchElementException if this collection is empty
             * @throws UnsupportedOperationException if this collection implementation
             *         does not support this operation
             */
            E &removeFirst() override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());

                try { return remove(0); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes and returns the last element of this collection (optional operation).
             *
             * @note
             * The implementation in this interface obtains an iterator of the reversed view of this
             * collection, and then it obtains an element by calling the iterator's @c next method.
             * Any @c NoSuchElementException thrown is propagated. It then calls the iterator's
             * @c remove method. Any @c UnsupportedOperationException thrown is propagated.
             * Then, it returns the element.
             *
             * @return the removed element
             * @throws NoSuchElementException if this collection is empty
             * @throws UnsupportedOperationException if this collection implementation
             *         does not support this operation
             */
            E &removeLast() override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());

                try { return remove(size() - 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a reverse-ordered @em view of this collection.
             * The encounter order of elements in the returned view is the inverse of the encounter
             * order of elements in this collection. The reverse ordering affects all order-sensitive
             * operations, including those on the view collections of the returned view. If the collection
             * implementation permits modifications to this view, the modifications "write through" to the
             * underlying collection. Changes to the underlying collection might or might not be visible
             * in this reversed view, depending upon the implementation.
             *
             * @note
             * The implementation in this interface returns a reverse-ordered List
             * view. The @c reversed() method of the view returns a reference
             * to this List. Other operations on the view are implemented via calls to
             * public methods on this List. The exact relationship between calls on the
             * view and calls on this List is unspecified. However, order-sensitive
             * operations generally behave as if they delegate to the appropriate method
             * with the opposite orientation. For example, calling firstElement on
             * the view might result in a call to @c lastElement on this List.
             *
             * @return a reverse-ordered view of this collection, as a @c List
             */
            List const &reversed() const override {
                try {
                    if (!reverse) {
                        if (Class<RandomAccess>::hasInstance(*this))
                            return UNSAFE::newInstance<RandomAccessReverse>(CORE_CAST(List<E> &, *this));

                        return UNSAFE::newInstance<Reverse>(CORE_CAST(List<E> &, *this));
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                return *reverse;
            }

            /**
             * Returns a reverse-ordered @em view of this collection.
             * The encounter order of elements in the returned view is the inverse of the encounter
             * order of elements in this collection. The reverse ordering affects all order-sensitive
             * operations, including those on the view collections of the returned view. If the collection
             * implementation permits modifications to this view, the modifications "write through" to the
             * underlying collection. Changes to the underlying collection might or might not be visible
             * in this reversed view, depending upon the implementation.
             *
             * @note
             * The implementation in this interface returns a reverse-ordered List
             * view. The @c reversed() method of the view returns a reference
             * to this List. Other operations on the view are implemented via calls to
             * public methods on this List. The exact relationship between calls on the
             * view and calls on this List is unspecified. However, order-sensitive
             * operations generally behave as if they delegate to the appropriate method
             * with the opposite orientation. For example, calling firstElement on
             * the view might result in a call to @c lastElement on this List.
             *
             * @return a reverse-ordered view of this collection, as a @c List
             */
            List &reversed() override {
                try {
                    if (!reverse) {
                        if (Class<RandomAccess>::hasInstance(*this))
                            return UNSAFE::newInstance<RandomAccessReverse>(*this);

                        return UNSAFE::newInstance<Reverse>(*this);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                return *reverse;
            }

            /**
             * Destroy this list and it reversed view
             */
            ~List() override {
                if (reverse != null) {
                    UNSAFE::deleteRegInstance(*reverse);
                    reverse = null;
                }
            }

            // ::::::::::::::::::::::: Statics List Methods :::::::::::::::::::::::::::::::::

            /**
             * Returns an unmodifiable list containing an arbitrary number of elements.
             * See <em>Unmodifiable Lists</em> for details.
             *
             * This will cause the @c List::of(E) method to be invoked instead.
             *
             * @param args the elements to be contained in the list
             * @return a @c List containing the specified elements
             */
            template<class... Es>
            static List &of(Es &&... args) {
                try {
                    Array<E> a = Array<E>::of(UNSAFE::forwardInstance<Es>(args)...);
                    switch (a.length()) {
                        case 0: return emptyList();
                        case 1: return UNSAFE::newInstance<Singleton>(a[0]);
                        default: return UNSAFE::newInstance<ListN>(a);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an unmodifiable list containing an arbitrary number of elements.
             * See <em>Unmodifiable Lists</em> for details.
             *
             * @code
             *     Array<String> a = ... ;
             *     auto list = List<Array<String>>::of(a);
             * @endcode
             *
             * This will cause the @c List::of(E) method to be invoked instead.
             *
             * @param args the elements to be contained in the list
             * @return a @c List containing the specified elements
             */
            template<class T = E, ClassOf(1)::OnlyIf<Class<E>::template isSuper<T>()> CaptureArray = 1>
            static List &of(Array<T> args) {
                try {
                    Array<E> a = Arrays::copyOf<E>(args, args.length());
                    switch (a.length()) {
                        case 0: return emptyList();
                        case 1: return UNSAFE::newInstance<Singleton>(a[0]);
                        default: return UNSAFE::newInstance<ListN>(a);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an <em>unmodifiable List</em> containing the elements of
             * the given Collection, in its iteration order. The given Collection must not be null,
             * and it must not contain any null elements. If the given Collection is subsequently
             * modified, the returned List will not reflect such modifications.
             *
             * @implNote
             * If the given Collection is an <em>unmodifiable List</em>,
             * calling copyOf will generally not create a copy.
             *
             * @tparam T the @c Collection's element type
             * @param c a @c Collection from which elements are drawn, must be non-null
             * @return a @c List containing the elements of the given @c Collection
             */
            template<class T = E, ClassOf(1)::OnlyIf<Class<E>::template isSuper<T>()> CaptureCollection = 1>
            static List &copyOf(Collection<T> const &c) {
                try {
                    Array<T> ca = c.toArray();
                    Array<E> a = Arrays::copyOf<E>(ca, ca.length());
                    switch (a.length()) {
                        case 0: return emptyList();
                        case 1: return UNSAFE::newInstance<Singleton>(a[0]);
                        default: return UNSAFE::newInstance<ListN>(a);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an empty list (immutable).  This list is serializable.
             *
             * <p>
             * This example illustrates the type-safe way to obtain an empty list:
             * @code
             *     auto& s = List<String>::emptyList();
             * @endcode
             * </p>
             * @note
             * Implementations of this method need not create a separate @c List
             * object for each call.   Using this method is likely to have comparable
             * cost to using the like-named field.  (Unlike this method, the field does
             * not provide type safety.)
             *
             * @tparam E type of elements, if there were any, in the list
             * @return an empty immutable list
             */
            static List &emptyList() CORE_NOTHROW {
                try {
                    return UNSAFE::newInstance<EmptyList>();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        protected:
            /**
             * The number of times this list has been <i>structurally modified</i>.
             * Structural modifications are those that change the size of the
             * list, or otherwise perturb it in such a fashion that iterations in
             * progress may yield incorrect results.
             *
             * <p>
             * This field is used by the iterator and list iterator implementation
             * returned by the @code iterator @endcode and @code listIterator @endcode methods.
             * If the value of this field changes unexpectedly, the iterator (or list
             * iterator) will throw a @code ConcurrentModificationException @endcode in
             * response to the @code next @endcode, @code remove @endcode, @code previous @endcode,
             * @code set @endcode or @code add @endcode operations.  This provides
             * <i>fail-fast</i> behavior, rather than non-deterministic behavior in
             * the face of concurrent modification during iteration.
             * </p>
             * <p>
             * <b>Use of this field by subclasses is optional.</b> If a subclass
             * wishes to provide fail-fast iterators (and list iterators), then it
             * merely has to increment this field in its @code add(int, E) @endcode and
             * @code remove(int) @endcode methods (and any other methods that it overrides
             * that result in structural modifications to the list).  A single call to
             * @code add(int, E) @endcode or @code remove(int) @endcode must add no more than
             * one to this field, or the iterators (and list iterators) will throw
             * bogus @code ConcurrentModificationExceptions @endcode.  If an implementation
             * does not wish to provide fail-fast iterators, this field may be
             * ignored.
             * </p>
             */
            gint mutable modCount = 0;

            // ::::::::::::::::::::::: List View Field :::::::::::::::::::::::::::::::::::::::::

            /**
             * The reversed list view
             */
            ListView mutable reverse = null;

            // ::::::::::::::::::::::: List View Class :::::::::::::::::::::::::::::::::::::::::

        private:
            /**
             * The reversed order list (order is defined by list indexes)
             */
            class Reverse;

            /**
             * The random access version of reversed order list.
             */
            class RandomAccessReverse;

            /**
             * The sublist view
             */
            class SubList;

            // ::::::::::::::::::::::: Unmodifiable List Class :::::::::::::::::::::::::::::::::

            /**
             * The unmodifiable list base class
             */
            class UnmodifiableList;

            /**
             * The unmodifiable empty list.
             */
            class EmptyList;

            /**
             * The unmodifiable list of single element.
             */
            class Singleton;

            /**
             * The unmodifiable list of arbitrary number of elements.
             */
            class ListN;
        };

        template<class E>
        class List<E>::Reverse : public virtual List<E> {
            List<E> &list;

        public:
            /**
             * Create new reversed view of given list.
             * This view will be linked automatically with original
             * version.
             * @param list the original list
             */
            CORE_EXPLICIT Reverse(List<E> &list): list(list) {
                CORE_ASSERT(list.reverse == null);
                list.reverse = this;
            }

            gint size() const override {
                try { return list.size(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return list.isEmpty(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool contains(const E &val) const override {
                try { return list.contains(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return Arrays::reverse(list.toArray());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool add(const E &val) override {
                try { list.addFirst(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return true;
            }

            gbool containsAll(const Collection<E> &c) const override {
                try { return list.containsAll(c); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool addAll(const Collection<E> &c) override {
                try { return list.addAll(c); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool addAll(gint index, const Collection<E> &c) override {
                try { return list.addAll(index, c); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(const Collection<E> &c) override {
                try { return list.removeAll(c); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(const Collection<E> &c) override {
                try { return list.retainAll(c); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &get(gint index) override {
                try {
                    PRE::checkIndex(index, size());
                    return list.get(size() - (index + 1));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &get(gint index) const override {
                try {
                    PRE::checkIndex(index, size());
                    return list.get(size() - (index + 1));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &set(gint index, const E &newValue) override {
                try {
                    PRE::checkIndex(index, size());
                    return list.set(size() - (index + 1), newValue);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void add(gint index, const E &val) override {
                try {
                    PRE::checkIndexForAdding(index, size());
                    return list.add(size() - index, val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &remove(gint index) override {
                try {
                    PRE::checkIndex(index, size());
                    return list.remove(size() - (index + 1));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gint indexOf(const E &val) const override {
                try { return list.lastIndexOf(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gint lastIndexOf(const E &val) const override {
                try { return list.indexOf(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            ListIterator<E> &listIterator(gint index) override {
                class Itr : public ListIterator<E> {
                    ListIterator<E> &it;

                public:
                    CORE_EXPLICIT Itr(ListIterator<E> &it) : it(it) {
                    }

                    gbool hasNext() const override {
                        try { return it.hasPrevious(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    E & next() override {
                        try { return it.previous(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool hasPrevious() const override {
                        try { return it.hasNext(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    E & previous() override {
                        try { return it.next(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gint nextIndex() const override {
                        try { return it.previousIndex(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gint previousIndex() const override {
                        try { return it.nextIndex(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        try { return it.remove(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    void set(const E &e) override {
                        try { return it.set(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    void add(const E &e) override {
                        try { return it.add(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object &o) const override {
                        try {
                            return this == &o or Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    ~Itr() override { UNSAFE::deleteInstance(it); }
                };

                try {
                    PRE::checkIndexForAdding(index, size());
                    return UNSAFE::newInstance<Itr>(list.listIterator(size() - index));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            ListIterator2<E> &listIterator(gint index) const override {
                class Itr : public ListIterator2<E> {
                    ListIterator<E> &it;

                public:
                    CORE_EXPLICIT Itr(ListIterator<E> &it) : it(it) {
                    }

                    gbool hasNext() const override {
                        try { return it.hasPrevious(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    E const & next() override {
                        try { return it.previous(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool hasPrevious() const override {
                        try { return it.hasNext(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    E const & previous() override {
                        try { return it.next(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gint nextIndex() const override {
                        try { return it.previousIndex(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gint previousIndex() const override {
                        try { return it.nextIndex(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object &o) const override {
                        try {
                            return this == &o or Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    ~Itr() override { UNSAFE::deleteInstance(it); }
                };

                try {
                    PRE::checkIndexForAdding(index, size());
                    return UNSAFE::newInstance<Itr>(list.listIterator(size() - index));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const List<E> &subList(gint fromIndex, gint toIndex) const override {
                try {
                    gint sz = size();
                    PRE::checkIndexFromRange(fromIndex, toIndex, sz);
                    fromIndex = sz - fromIndex;
                    toIndex = sz - toIndex;
                    return list.subList(toIndex, fromIndex).reversed();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            List<E> &subList(gint fromIndex, gint toIndex) override {
                try {
                    gint sz = size();
                    PRE::checkIndexFromRange(fromIndex, toIndex, sz);
                    fromIndex = sz - fromIndex;
                    toIndex = sz - toIndex;
                    return list.subList(toIndex, fromIndex).reversed();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void addFirst(const E &val) override {
                try { return list.addLast(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void addLast(const E &val) override {
                try { return list.addFirst(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &firstElement() override {
                try { return list.lastElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &firstElement() const override {
                try { return list.lastElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &lastElement() override {
                try { return list.firstElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &lastElement() const override {
                try { return list.firstElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &removeFirst() override {
                try { return list.removeLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &removeLast() override {
                try { return list.removeFirst(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const List<E> &reversed() const override { return list; }

            List<E> &reversed() override { return list; }

            /**
             * Destroy this view and unlink it from original list.
             */
            ~Reverse() override { list.reverse = null; }
        };

        template<class E>
        class List<E>::RandomAccessReverse final : public virtual Reverse, public virtual RandomAccess {
        public:
            CORE_EXPLICIT RandomAccessReverse(List<E> &list): Reverse(list) {
            }
        };

        template<class E>
        class List<E>::SubList final : public virtual List {
            CORE_ALIAS(SubView, typename Class<SubList>::Pointer);

            List &list;
            SubList &sublist = *this;
            gint offset = 0;
            gint count = 0;

        public:
            CORE_EXPLICIT SubList(List &list, gint offset, gint count)
                : list(list), offset(offset), count(count) {
                modCount = list.modCount;
            }

            CORE_EXPLICIT SubList(SubList &sublist, gint offset, gint count)
                : list(sublist.list), sublist(sublist.sublist), offset(offset + sublist.offset), count(count) {
                modCount = list.modCount;
            }

            gint size() const override {
                try {
                    check();
                    return count;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    check();
                    return count == 0;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool add(const E &val) override {
                try {
                    check();
                    addLast(val);
                    return true;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool addAll(const Collection<E> &c) override {
                try {
                    check();
                    gint sz = list.size();
                    gbool modified = list.addAll(offset, c);
                    update(list.size() - sz);
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool addAll(gint index, const Collection<E> &c) override {
                try {
                    PRE::checkIndex(index, size());
                    check();
                    gint sz = list.size();
                    gbool modified = list.addAll(offset + index, c);
                    update(list.size() - sz);
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &set(gint index, const E &newValue) override {
                try {
                    PRE::checkIndex(index, size());
                    check();
                    return list.set(offset + index, newValue);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void add(gint index, const E &val) override {
                try {
                    PRE::checkIndex(index, size());
                    check();
                    list.add(offset + index, val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &remove(gint index) override {
                try {
                    PRE::checkIndex(index, size());
                    check();
                    return list.remove(offset + index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            ListIterator<E> &listIterator(gint index) override {
                class Itr final : public ListIterator<E> {
                    SubList &list;
                    ListIterator<E> &it;
                    gint cursor;
                    gint last = -1;

                public:
                    CORE_EXPLICIT Itr(SubList &list, ListIterator<E> &it, gint index)
                        : list(list), it(it), cursor(index) {
                    }

                    gbool hasNext() const override { return cursor < list.size(); }

                    E & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        try {
                            list.check();
                            last = cursor++;
                            return it.next();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool hasPrevious() const override { return cursor > 0; }

                    E & previous() override {
                        if (!hasPrevious())
                            NoSuchElementException().throws($ftrace());

                        try {
                            list.check();
                            last = --cursor;
                            return it.previous();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gint nextIndex() const override { return cursor; }

                    gint previousIndex() const override { return cursor - 1; }

                    void remove() override {
                        if (last < 0)
                            IllegalStateException().throws($ftrace());

                        try {
                            list.check();
                            it.remove();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        if (cursor >= last)
                            cursor--;
                        last = -1;
                        list.update(-1);
                    }

                    void set(const E &e) override {
                        if (last < 0)
                            IllegalStateException().throws($ftrace());

                        try {
                            list.check();
                            it.set(e);
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    void add(const E &e) override {
                        if (last < 0)
                            IllegalStateException().throws($ftrace());

                        try {
                            list.check();
                            it.add(e);
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        if (cursor >= last)
                            cursor++;
                        last = -1;
                        list.update(1);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;
                        Itr const &itr = CORE_XCAST(Itr const, o);
                        return cursor == itr.cursor && &list == &itr.list && it == itr.it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(it);
                    }
                };

                try {
                    PRE::checkIndexForAdding(index, size());
                    check();
                    return UNSAFE::newInstance<Itr>(*this,
                                                    list.listIterator(offset + index),
                                                    index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            ListIterator2<E> &listIterator(gint index) const override {
                class Itr final : public ListIterator2<E> {
                    SubList &list;
                    ListIterator<E> &it;
                    gint cursor;
                    gint last = -1;

                public:
                    CORE_EXPLICIT Itr(SubList &list, ListIterator<E> &it, gint index)
                        : list(list), it(it), cursor(index) {
                    }

                    gbool hasNext() const override { return cursor < list.size(); }

                    E const & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        try {
                            list.check();
                            last = cursor++;
                            return it.next();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool hasPrevious() const override { return cursor > 0; }

                    E const & previous() override {
                        if (!hasPrevious())
                            NoSuchElementException().throws($ftrace());

                        try {
                            list.check();
                            last = --cursor;
                            return it.previous();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gint nextIndex() const override { return cursor; }

                    gint previousIndex() const override { return cursor - 1; }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;
                        Itr const &itr = CORE_XCAST(Itr const, o);
                        return cursor == itr.cursor && &list == &itr.list && it == itr.it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(it);
                    }
                };

                try {
                    PRE::checkIndexForAdding(index, size());
                    check();
                    return UNSAFE::newInstance<Itr>(CORE_CAST(SubList &, *this),
                                                    list.listIterator(offset + index),
                                                    index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const List<E> &subList(gint fromIndex, gint toIndex) const override {
                try {
                    PRE::checkIndexFromRange(fromIndex, toIndex, size());
                    check();
                    return UNSAFE::newInstance<SubList>(sublist, fromIndex, toIndex - fromIndex);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            List<E> &subList(gint fromIndex, gint toIndex) override {
                try {
                    PRE::checkIndexFromRange(fromIndex, toIndex, size());
                    check();
                    return UNSAFE::newInstance<SubList>(sublist, fromIndex, toIndex - fromIndex);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void addFirst(const E &val) override {
                try {
                    check();
                    list.add(offset, val);
                    update(1);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void addLast(const E &val) override {
                try {
                    check();
                    list.add(offset + count, val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &firstElement() override {
                try { check(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try { return list.get(offset); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &firstElement() const override {
                try { check(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try { return list.get(offset); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &lastElement() override {
                try { check(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try { return list.get(offset + count - 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &lastElement() const override {
                try { check(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try { return list.get(offset + count - 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &removeFirst() override {
                try { check(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try {
                    E &val = list.remove(offset);
                    update(-1);
                    return val;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &removeLast() override {
                try { check(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try {
                    E &val = list.remove(offset);
                    update(-1);
                    return val;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const List<E> &reversed() const override {
                try {
                    check();
                    return List<E>::reversed();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            List<E> &reversed() override {
                try {
                    check();
                    return List<E>::reversed();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &get(gint index) override {
                try {
                    PRE::checkIndex(index, size());
                    return list.get(offset + index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &get(gint index) const override {
                try {
                    PRE::checkIndex(index, size());
                    return list.get(offset + index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            // :::::::::::::::::::::::: List Utility :::::::::::::::::::::::::::::::::::::

            /**
             * Update this view
             */
            void update(gint adj) {
                SubView view = this;
                do {
                    view->modCount = list.modCount;
                    view->count += adj;
                    view = &view->sublist;
                } while (view != &view->sublist);
            }

            /**
             * Check if this list is updated
             */
            void check() const {
                if (modCount != list.modCount)
                    ConcurrentModificationException().throws($ftrace());
            }
        };

        template<class E>
        class List<E>::UnmodifiableList : public List<E> {
        public:
            gbool removeIf(const function::Predicate<E> &filter) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool add(const E &val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool remove(const E &val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool addAll(const Collection<E> &c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool addAll(gint index, const Collection<E> &c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool removeAll(const Collection<E> &c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool retainAll(const Collection<E> &c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void replaceAll(const function::UnaryOperator<E> &op) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void sort(const Comparator<E> &c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                UnsupportedOperationException().throws($ftrace());
            }

            E &set(gint index, const E &newValue) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void add(gint index, const E &val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            E &remove(gint index) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void addFirst(const E &val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void addLast(const E &val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            E &removeFirst() override {
                UnsupportedOperationException().throws($ftrace());
            }

            E &removeLast() override {
                UnsupportedOperationException().throws($ftrace());
            }
        };

        template<class E>
        class List<E>::EmptyList final : public List<E>::UnmodifiableList {
        public:
            gint size() const override { return 0; }

            E &get(gint index) override {
                IndexOutOfBoundsException(index).throws($ftrace());
            }

            const E &get(gint index) const override {
                IndexOutOfBoundsException(index).throws($ftrace());
            }

            void forEach(const function::Consumer<E &> &action) override {
            }

            void forEach(const function::Consumer<E> &action) const override {
            }

            String toString() const override { return "[]"_S; }

            gbool isEmpty() const override { return true; }

            gbool contains(const E &val) const override { return false; }

            Array<E> toArray() const override { return Array<E>(); }

            gbool containsAll(const Collection<E> &c) const override { return c.isEmpty(); }

            gbool equals(const Object &o) const override {
                return this == &o || Class<List>::hasInstance(o) && CORE_XCAST(List<E> const, o).isEmpty();
            }

            gint hash() const override { return 0; }

            gint indexOf(const E &val) const override { return -1; }

            gint lastIndexOf(const E &val) const override { return -1; }

            E &firstElement() override { NoSuchElementException().throws($ftrace()); }

            const E &firstElement() const override { NoSuchElementException().throws($ftrace()); }

            E &lastElement() override { NoSuchElementException().throws($ftrace()); }

            const E &lastElement() const override { NoSuchElementException().throws($ftrace()); }

            const List<E> &reversed() const override { return *this; }

            List<E> &reversed() override { return *this; }
        };

        template<class E>
        class List<E>::Singleton final : public List<E>::UnmodifiableList {
            E &value;

        public:
            CORE_EXPLICIT Singleton(E &value) : value(value) {
            }

            gint size() const override { return 1; }

            E &get(gint index) override {
                try {
                    PRE::checkIndex(index, 1);
                    return value;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &get(gint index) const override {
                try {
                    PRE::checkIndex(index, 1);
                    return value;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E &> &action) override {
                try { action.accept(value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E> &action) const override {
                try { action.accept(value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    return XString().append("[").append(value).append("]").toString();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override { return false; }

            gbool contains(const E &val) const override {
                try { return val == value; } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override { return Array<E>(1, value); }

            gint hash() const override { return value.hash(); }

            gint indexOf(const E &val) const override {
                try { return val == value ? 0 : -1; } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gint lastIndexOf(const E &val) const override {
                try { return val == value ? 0 : -1; } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &firstElement() override { return value; }

            const E &firstElement() const override { return value; }

            E &lastElement() override { return value; }

            const E &lastElement() const override { return value; }

            const List<E> &reversed() const override { return *this; }

            List<E> &reversed() override { return *this; }
        };

        template<class E>
        class List<E>::ListN final : public List<E>::UnmodifiableList {
            Array<E> elts;

        public:
            CORE_EXPLICIT ListN(const Array<E> &elts): elts(elts) {
            }

            gint size() const override { return elts.length(); }

            E &get(gint index) override {
                try {
                    PRE::checkIndex(index, elts.length());
                    return elts[index];
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &get(gint index) const override {
                try {
                    PRE::checkIndex(index, elts.length());
                    return elts[index];
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E &> &action) override {
                try {
                    for (E &elt: elts) action.accept(elt);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E> &action) const override {
                try {
                    for (E const &elt: elts) action.accept(elt);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    return elts.toString();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override { return elts.isEmpty(); }

            gbool contains(const E &val) const override {
                try {
                    for (E const &elt: elts)
                        if (val == elt)
                            return true;

                    return false;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override { return elts; }

            gint hash() const override { return elts.hash(); }

            gint indexOf(const E &val) const override {
                try {
                    gint i = 0;
                    for (E const &elt: elts) {
                        if (val == elt) return i;
                        i += 1;
                    }
                    return -1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gint lastIndexOf(const E &val) const override {
                try {
                    for (int i = elts.length() - 1; i >= 0; --i) {
                        E const &elt = elts[i];
                        if (val == elt) return i;
                    }
                    return -1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &firstElement() override {
                if (elts.length() == 0)
                    NoSuchElementException().throws($ftrace());

                return elts[0];
            }

            const E &firstElement() const override {
                if (elts.length() == 0)
                    NoSuchElementException().throws($ftrace());

                return elts[0];
            }

            E &lastElement() override {
                if (elts.length() == 0)
                    NoSuchElementException().throws($ftrace());

                return elts[elts.length() - 1];
            }

            const E &lastElement() const override {
                if (elts.length() == 0)
                    NoSuchElementException().throws($ftrace());

                return elts[elts.length() - 1];
            }
        };
    } // util
} // core

#endif //CORE24_LIST_H
