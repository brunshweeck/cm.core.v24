//
// Created by bruns on 22/09/2024.
//

#ifndef CORE24_LINKEDHASHSET_H
#define CORE24_LINKEDHASHSET_H

#include <core/util/HashSet.h>
#include <core/util/SequencedSet.h>

namespace core {
    namespace util {
        /**
         * <p>
         * Hash table and linked list implementation of the @c Set interface,
         * with well-defined encounter order.  This implementation differs from
         * @c HashSet in that it maintains a doubly-linked list running through
         * all of its entries.  This linked list defines the encounter order (iteration
         * order), which is the order in which elements were inserted into the set
         * (<i>insertion-order</i>). The least recently inserted element (the eldest) is
         * first, and the youngest element is last. Note that encounter order is <i>not</i> affected
         * if an element is <i>re-inserted</i> into the set with the @c add method.
         * (An element @c e is reinserted into a set @c s if @c s.add(e) is
         * invoked when @c s.contains(e) would return @c true immediately prior to
         * the invocation.) The reverse-ordered view of this set is in the opposite order, with
         * the youngest element appearing first and the eldest element appearing last. The encounter
         * order of elements already in the set can be changed by using the
         * @b addFirst and @b addLast methods.
         * </p>
         * <p>
         * This implementation spares its clients from the unspecified, generally
         * chaotic ordering provided by @b HashSet, without incurring the
         * increased cost associated with @b TreeSet.  It can be used to
         * produce a copy of a set that has the same order as the original, regardless
         * of the original set's implementation:
         * @code
         *     void foo(Set<String> const &s) {
         *         auto &copy = Unsafe::newInstance<LinkedHashSet<String>>(s);
         *         ...
         *     }
         * @endcode
         * This technique is particularly useful if a module takes a set on input,
         * copies it, and later returns results whose order is determined by that of
         * the copy.  (Clients generally appreciate having things returned in the same
         * order they were presented.)
         * </p>
         * <p>
         * This class provides all of the optional @b Set and @b SequencedSet
         * operations, and it permits null elements. Like @c HashSet, it provides constant-time
         * performance for the basic operations (@c add, @c contains and
         * @c remove), assuming the hash function disperses elements
         * properly among the buckets.  Performance is likely to be just slightly
         * below that of @c HashSet, due to the added expense of maintaining the
         * linked list, with one exception: Iteration over a @c LinkedHashSet
         * requires time proportional to the <i>size</i> of the set, regardless of
         * its capacity.  Iteration over a @c HashSet is likely to be more
         * expensive, requiring time proportional to its <i>capacity</i>.
         * </p>
         * <p>
         * A linked hash set has two parameters that affect its performance:
         * <i>initial capacity</i> and <i>load factor</i>.  They are defined precisely
         * as for @c HashSet.  Note, however, that the penalty for choosing an
         * excessively high value for initial capacity is less severe for this class
         * than for @c HashSet, as iteration times for this class are unaffected
         * by capacity.
         * </p>
         * <p>
         * <b>Note that this implementation is not synchronized.</b>
         * If multiple threads access a linked hash set concurrently, and at least
         * one of the threads modifies the set, it <em>must</em> be synchronized
         * externally.  This is typically accomplished by synchronizing on some
         * object that naturally encapsulates the set.
         *
         * If no such object exists, the set should be "wrapped" using the
         * @b Collections.synchronizedSet
         * method.  This is best done at creation time, to prevent accidental
         * unsynchronized access to the set:
         * @code
         *   Set s = Collections.synchronizedSet(new LinkedHashSet(...));
         * @endcode
         * </p>
         * <p>
         * The iterators returned by this class's @c iterator method are
         * <em>fail-fast</em>: if the set is modified at any time after the iterator
         * is created, in any way except through the iterator's own @c remove
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
         *
         * @tparam E the type of elements maintained by this set
         *
         * @see     Object::hash()
         * @see     Collection
         * @see     Set
         * @see     HashSet
         * @see     TreeSet
         * @see     Hashtable
         */
        template<class E>
        class LinkedHashSet final : public virtual HashSet<E>, public virtual SequencedSet<E> {
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(BackendMap, typename Class<LinkedHashMap<E, Object>>::Pointer);
            CORE_ADD_GLOBAL_FRIENDS();

        public:
            /**
             * Constructs a new, empty linked hash set with the specified initial
             * capacity and load factor.
             *
             * @param      initialCapacity the initial capacity of the linked hash set
             * @param      loadFactor      the load factor of the linked hash set
             * @throws     IllegalArgumentException  if the initial capacity is less
             *               than zero, or if the load factor is non-positive
             */
            CORE_EXPLICIT LinkedHashSet(gint initialCapacity, gfloat loadFactor)
                : HashSet<>(initialCapacity, loadFactor, true){}

            /**
             * Constructs a new, empty linked hash set with the specified initial
             * capacity and the default load factor (0.75).
             *
             * @param   initialCapacity   the initial capacity of the LinkedHashSet
             * @throws  IllegalArgumentException if the initial capacity is less
             *              than zero
             */
            CORE_EXPLICIT LinkedHashSet(gint initialCapacity): HashSet<E>(initialCapacity, 0.75F, true) {}

            /**
             * Constructs a new, empty linked hash set with the default initial
             * capacity (16) and load factor (0.75).
             */
            CORE_IMPLICIT LinkedHashSet(): HashSet<E>(16, 0.75F, true) {}

            /**
             * Constructs a new linked hash set with the same elements as the
             * specified collection.  The linked hash set is created with an initial
             * capacity sufficient to hold the elements in the specified collection
             * and the default load factor (0.75).
             *
             * @param c  the collection whose elements are to be placed into
             *           this set
             */
            CORE_EXPLICIT LinkedHashSet(Collection<E> const &c): LinkedHashSet() {
                try { LinkedHashSet::addAll(c); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            LinkedHashSet(LinkedHashSet const &other) {
                try { getMap() = other.getMap(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            LinkedHashSet(LinkedHashSet &&other) CORE_NOTHROW {
                getMap() = UNSAFE::moveInstance(other.getMap());
            }

            LinkedHashSet & operator=(LinkedHashSet const &other) {
                if (this != &other) {
                    try { getMap() = other.getMap(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
                }
                return *this;
            }

            LinkedHashSet & operator=(LinkedHashSet &&other) CORE_NOTHROW {
                if (this != &other) {
                    getMap() = UNSAFE::moveInstance(other.getMap());
                }
                return *this;
            }

            /**
             *
             * <p>
             * If this set already contains the element, it is relocated if necessary so that it is
             * first in encounter order.
             *
             *
             */
            void addFirst(const E &val) override {
                try { getMap().putFirst(val, null); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             * <p>
             * If this set already contains the element, it is relocated if necessary so that it is
             * last in encounter order.
             *
             *
             */
            void addLast(const E &val) override {
                try { getMap().putLast(val, null); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @throws NoSuchElementException
             *
             */
            E &removeFirst() override {
                try { return getMap().pollFirstEntry().getKey(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @throws NoSuchElementException
             *
             */
            E &removeLast() override {
                try { return getMap().pollLastEntry().getKey(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             * <p>
             * Modifications to the reversed view are permitted and will be propagated to this set.
             * In addition, modifications to this set will be visible in the reversed view.
             *
             */
            const SequencedSet<E> &reversed() const override {
                try { return getMap().reversed().sequencedKeySet(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             * <p>
             * Modifications to the reversed view are permitted and will be propagated to this set.
             * In addition, modifications to this set will be visible in the reversed view.
             *
             */
            SequencedSet<E> &reversed() override {
                try { return getMap().reversed().sequencedKeySet(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            E & firstElement() override {
                try { return getMap().firstEntry().getKey(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            E const & firstElement() const override {
                try { return getMap().firstEntry().getKey(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            E & lastElement() override {
                try { return getMap().lastEntry().getKey(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            E const & lastElement() const override {
                try { return getMap().lastEntry().getKey(); } catch(Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~LinkedHashSet() override = default;

        private:
            LinkedHashMap<E, Object> &getMap() override {
                return *CORE_DCAST(BackendMap, HashSet<E>::map);
            }

            LinkedHashMap<E, Object> const & getMap() const override {
                return *CORE_DCAST(BackendMap, HashSet<E>::map);
            }
        };
    } // util
} // core

#endif //CORE24_LINKEDHASHSET_H
