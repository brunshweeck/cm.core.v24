//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_TREESET_H
#define CORE24_TREESET_H

#include <core/util/NavigableSet.h>
#include <core/util/TreeMap.h>

namespace core {
    namespace util {
        /**
         * A @b NavigableSet implementation based on a @b TreeMap.
         * The elements are ordered using their <em> natural
         * ordering</em>, or by a @b Comparator provided at set creation
         * time, depending on which constructor is used.
         *
         * <p>
         * This implementation provides guaranteed log(n) time cost for the basic
         * operations (@c add, @c remove and @c contains).
         * </p>
         * <p>
         * Note that the ordering maintained by a set (whether an explicit
         * comparator is provided) must be <i>consistent with equals</i> if it is to
         * correctly implement the @c Set interface.  (See @c Comparable
         * or @c Comparator for a precise definition of <i>consistent with
         * equals</i>.)  This is so because the @c Set interface is defined in
         * terms of the @c equals operation, but a @c TreeSet instance
         * performs all element comparisons using its @c compareTo (or
         * @c compare) method, so two elements that are deemed equal by this method
         * are, from the standpoint of the set, equal.  The behavior of a set
         * <i>is</i> well-defined even if its ordering is inconsistent with equals; it
         * just fails to obey the general contract of the @c Set interface.
         * </p>
         * <p>
         * <b>Note that this implementation is not synchronized.</b>
         * If multiple threads access a tree set concurrently, and at least one
         * of the threads modifies the set, it <i>must</i> be synchronized
         * externally.  This is typically accomplished by synchronizing on some
         * object that naturally encapsulates the set.
         * If no such object exists, the set should be "wrapped" using the
         * @b Collections::synchronizedSortedSet
         * method.  This is best done at creation time, to prevent accidental
         * unsynchronized access to the set:
         * @code
         *   SortedSet &s = Collections.synchronizedSortedSet(Unsafe::newInstance<TreeSet>(...));
         * @endcode
         * </p>
         * <p>
         * The iterators returned by this class's @c iterator method are
         * <i>fail-fast</i>: if the set is modified at any time after the iterator is
         * created, in any way except through the iterator's own @c remove
         * method, the iterator will throw a @b ConcurrentModificationException.
         * Thus, in the face of concurrent modification, the iterator fails quickly
         * and cleanly, rather than risking arbitrary, non-deterministic behavior at
         * an undetermined time in the future.
         * </p>
         * <p>
         * Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw @c ConcurrentModificationException on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         * </p>
         * <p>
         * The @b addFirst and @b addLast methods of this class
         * throw @c UnsupportedOperationException. The encounter order of elements is determined
         * by the comparison method; therefore, explicit positioning is not supported.
         * </p>
         *
         * @param E the type of elements maintained by this set
         *
         * @see     Collection
         * @see     Set
         * @see     HashSet
         * @see     Comparable
         * @see     Comparator
         * @see     TreeMap
         */
        template <class E>
        class TreeSet final : public virtual NavigableSet<E> {
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ADD_GLOBAL_FRIENDS();

            /**
             * The backing Map
             */
            TreeMap<E, Object> map;

        public:
            /**
             * Constructs a new, empty tree set, sorted according to the
             * natural ordering of its elements.  All elements inserted into
             * the set must implement the @b Comparable interface.
             * Furthermore, all such elements must be <i>mutually
             * comparable</i>: @code e1.compareTo(e2) @endcode must not throw a
             * @c ClassCastException for any elements @c e1 and
             * @c e2 in the set.  If the user attempts to add an element
             * to the set that violates this constraint (for example, the user
             * attempts to add a string element to a set whose elements are
             * integers), the @c add call will throw a
             * @c ClassCastException.
             */
            CORE_IMPLICIT TreeSet() = default;

            /**
             * Constructs a new, empty tree set, sorted according to the specified
             * comparator.  All elements inserted into the set must be <i>mutually
             * comparable</i> by the specified comparator: @code comparator.compare(e1,
             * e2) @endcode must not throw a @c ClassCastException for any elements
             * @c e1 and @c e2 in the set.  If the user attempts to add
             * an element to the set that violates this constraint, the
             * @c add call will throw a @c ClassCastException.
             *
             * @param comparator the comparator that will be used to order this set.
             */
            CORE_EXPLICIT TreeSet(Comparator<E> const &comparator) {
                try { map = TreeMap<E, Object>(comparator); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new tree set containing the elements in the specified
             * collection, sorted according to the <i>natural ordering</i> of its
             * elements.  All elements inserted into the set must implement the
             * @b Comparable interface.  Furthermore, all such elements must be
             * <i>mutually comparable</i>: @code e1.compareTo(e2) @endcode must not throw a
             * @c ClassCastException for any elements @c e1 and
             * @c e2 in the set.
             *
             * @param c collection whose elements will comprise the new set
             * @throws ClassCastException if the elements in @c c are
             *         not @b Comparable, or are not mutually comparable
             */
            CORE_EXPLICIT TreeSet(Collection<E> const &c) {
                try {
                    if (Class<SortedSet<E>>::hasInstance(c))
                        map = TreeMap<E, Object>(CORE_XCAST(SortedSet<E> const, c).comparator());

                    TreeSet::addAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new tree set containing the same elements and
             * using the same ordering as the specified sorted set.
             *
             * @param s sorted set whose elements will comprise the new set.
             */
            CORE_EXPLICIT TreeSet(SortedSet<E> const &s): TreeSet(s.comparator()) {
                try { TreeSet::addAll(s); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            CORE_IMPLICIT TreeSet(TreeSet const &other) {
                try { map = other.map; } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            CORE_IMPLICIT TreeSet(TreeSet &&other) CORE_NOEXCEPT {
                try { map = UNSAFE::moveInstance(other.map); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this set in ascending order.
             *
             * @return an iterator over the elements in this set in ascending order
             */
            Iterator2<E> &iterator() const override {
                try { return map.keySet().iterator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this set in ascending order.
             *
             * @return an iterator over the elements in this set in ascending order
             */
            Iterator<E> &iterator() override {
                try { return map.keySet().iterator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this set in descending order.
             *
             * @return an iterator over the elements in this set in descending order
             */
            Iterator2<E> &descendingIterator() const override {
                try { return map.descendingKeySet().iterator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this set in descending order.
             *
             * @return an iterator over the elements in this set in descending order
             */
            Iterator<E> &descendingIterator() override {
                try { return map.descendingKeySet().iterator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the number of elements in this set (its cardinality).
             *
             * @return the number of elements in this set (its cardinality)
             */
            gint size() const override {
                try { return map.size(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this set contains no elements.
             *
             * @return @c true if this set contains no elements
             */
            gbool isEmpty() const override {
                try { return map.isEmpty(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this set contains the specified element.
             * More formally, returns @c true if and only if this set
             * contains an element @c e such that
             * @code Objects::equals(o, e) @endcode.
             *
             * @param e object to be checked for containment in this set
             * @return @c true if this set contains the specified element
             * @throws ClassCastException if the specified object cannot be compared
             *         with the elements currently in the set
             */
            gbool contains(E const &e) const override {
                try { return map.containsKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Adds the specified element to this set if it is not already present.
             * More formally, adds the specified element @c e to this set if
             * the set contains no element @c e2 such that
             * @code Objects::equals(e, e2) @endcode .
             * If this set already contains the element, the call leaves the set
             * unchanged and returns @c false.
             *
             * @param e element to be added to this set
             * @return @c true if this set did not already contain the specified
             *         element
             * @throws ClassCastException if the specified object cannot be compared
             *         with the elements currently in this set
             */
            gbool add(E const &e) override {
                try {
                    gint size = map.size();
                    map.put(e, null);
                    return map.size() > size;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the specified element from this set if it is present.
             * More formally, removes an element @c e such that
             * @code Objects::equals(o, e) @endcode,
             * if this set contains such an element.  Returns @c true if
             * this set contained the element (or equivalently, if this set
             * changed as a result of the call).  (This set will not contain the
             * element once the call returns.)
             *
             * @param e object to be removed from this set, if present
             * @return @c true if this set contained the specified element
             * @throws ClassCastException if the specified object cannot be compared
             *         with the elements currently in this set.
             */
            gbool remove(E const &e) override {
                try { return map.remove(e, null); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all the elements from this set.
             * The set will be empty after this call returns.
             */
            void clear() override { try { map.clear(); } catch (Throwable const &ex) { ex.throws($ftrace()); } }

            /**
             * Adds all the elements in the specified collection to this set.
             *
             * @param c collection containing elements to be added to this set
             * @return @c true if this set changed as a result of the call
             * @throws ClassCastException if the elements provided cannot be compared
             *         with the elements currently in the set.
             */
            gbool addAll(Collection<E> const &c) override {
                try {
                    if (c.isEmpty())
                        return false;

                    // Use linear-time version if applicable
                    if (map.isEmpty() && Class<SortedSet<E>>::hasInstance(c)) {
                        SortedSet<E> const &set = CORE_XCAST(SortedSet<E> const, c);
                        if (map.comparator() == set.comparator()) {
                            map.initializeTreeFromSortedSet((SortedSet<E> &)set, null);
                            return map.size() > 0;
                        }
                    }

                    return NavigableSet<E>::addAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            NavigableSet<E> const &subSet(E const &fromElement, gbool fromInclusive,
                                          E const &toElement, gbool toInclusive) const override {
                try { return map.subMap(fromElement, fromInclusive, toElement, toInclusive).navigableKeySet(); } catch (
                    Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            NavigableSet<E> &subSet(E const &fromElement, gbool fromInclusive,
                                    E const &toElement, gbool toInclusive) override {
                try { return map.subMap(fromElement, fromInclusive, toElement, toInclusive).navigableKeySet(); } catch (
                    Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            NavigableSet<E> const &headSet(E const &toElement, gbool toInclusive) const override {
                try { return map.headMap(toElement, toInclusive).navigableKeySet(); } catch (Throwable const &ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            NavigableSet<E> &headSet(E const &toElement, gbool toInclusive) override {
                try { return map.headMap(toElement, toInclusive).navigableKeySet(); } catch (Throwable const &ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            NavigableSet<E> const &tailSet(E const &fromElement, gbool fromInclusive) const override {
                try { return map.tailMap(fromElement, fromInclusive).navigableKeySet(); } catch (Throwable const &ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            NavigableSet<E> &tailSet(E const &fromElement, gbool fromInclusive) override {
                try { return map.tailMap(fromElement, fromInclusive).navigableKeySet(); } catch (Throwable const &ex) {
                    ex.throws($ftrace());
                }
            }

            Comparator<E> const &comparator() const override {
                try { return map.comparator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws NoSuchElementException
             */
            E const &first() const override {
                try { return map.firstKey(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws NoSuchElementException
             */
            E &first() override { try { return map.firstKey(); } catch (Throwable const &ex) { ex.throws($ftrace()); } }

            /**
             * @throws NoSuchElementException
             */
            E const &last() const override {
                try { return map.lastKey(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws NoSuchElementException
             */
            E &last() override { try { return map.lastKey(); } catch (Throwable const &ex) { ex.throws($ftrace()); } }

            // NavigableSet API methods

            /**
             * @throws ClassCastException
             */
            E const &lower(E const &e) const override {
                try { return map.lowerKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             */
            E &lower(E const &e) override {
                try { return map.lowerKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             */
            E const &floor(E const &e) const override {
                try { return map.floorKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             */
            E &floor(E const &e) override {
                try { return map.floorKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             */
            E const &ceiling(E const &e) const override {
                try { return map.ceilingKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             */
            E &ceiling(E const &e) override {
                try { return map.ceilingKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             */
            E const &higher(E const &e) const override {
                try { return map.higherKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @throws ClassCastException
             */
            E &higher(E const &e) override {
                try { return map.higherKey(e); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Throws @c UnsupportedOperationException. The encounter order induced by this
             * set's comparison method determines the position of elements, so explicit positioning
             * is not supported.
             *
             * @throws UnsupportedOperationException always
             */
            void addFirst(E const &e) override { UnsupportedOperationException().throws($ftrace()); }

            /**
             * Throws @c UnsupportedOperationException. The encounter order induced by this
             * set's comparison method determines the position of elements, so explicit positioning
             * is not supported.
             *
             * @throws UnsupportedOperationException always
             */
            void addLast(E const &e) override { UnsupportedOperationException().throws($ftrace()); }

            /**
             * Returns a shallow copy of this @c TreeSet instance. (The elements
             * themselves are not cloned.)
             *
             * @return a shallow copy of this set
             */
            Object &clone() const override {
                try {
                    TreeSet &clone = UNSAFE::newInstance<TreeSet>();
                    clone.map = map;
                    return clone;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return map.keySet().toArray();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };
    } // util
} // core

#endif //CORE24_TREESET_H
