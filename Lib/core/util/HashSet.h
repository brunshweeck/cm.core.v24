//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_HASHSET_H
#define CORE24_HASHSET_H

#include <core/util/LinkedHashMap.h>

namespace core {
    namespace util {
        /**
         * This class implements the @c Set interface, backed by a hash table
         * (actually a @c HashMap instance).  It makes no guarantees as to the
         * iteration order of the set; in particular, it does not guarantee that the
         * order will remain constant over time.  This class permits the @c null
         * element.
         *
         * <p>
         * This class offers constant time performance for the basic operations
         * (@c add, @c remove, @c contains and @c size),
         * assuming the hash function disperses the elements properly among the
         * buckets.  Iterating over this set requires time proportional to the sum of
         * the @c HashSet instance's size (the number of elements) plus the
         * "capacity" of the backing @c HashMap instance (the number of
         * buckets).  Thus, it's very important not to set the initial capacity too
         * high (or the load factor too low) if iteration performance is important.
         * </p>
         * <p>
         * <b>Note that this implementation is not synchronized.</b>
         * If multiple threads access a hash set concurrently, and at least one of
         * the threads modifies the set, it <i>must</i> be synchronized externally.
         * This is typically accomplished by synchronizing on some object that
         * naturally encapsulates the set.
         *
         * If no such object exists, the set should be "wrapped" using the
         * @b Collections.synchronizedSet
         * method.  This is best done at creation time, to prevent accidental
         * unsynchronized access to the set:
         * @code
         *   auto &s = Collections::synchronizedSet(Unsafe::newInstance<HashSet>(...));
         * @endcode
         * </p>
         * <p>
         * The iterators returned by this class's @c iterator method are
         * <i>fail-fast</i>: if the set is modified at any time after the iterator is
         * created, in any way except through the iterator's own @c remove
         * method, the Iterator throws a @b ConcurrentModificationException.
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
         * exception for its correctness: <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         * </p>
         * @tparam E the type of elements maintained by this set
         *
         * @see     Collection
         * @see     Set
         * @see     TreeSet
         * @see     HashMap
         */
        template <class E>
        class HashSet : public virtual Set<E> {
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(BackendMap, typename Class<HashMap<E, Object>>::Pointer);
            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_TEMPLATES_AS_FRIEND(LinkedHashSet);

            /**
             * The backing Map (The Dummy value associated with an
             * Object in backing Map is @c null)
             */
            BackendMap map = { };

        public:
            /**
             * Constructs a new, empty set; the backing @c HashMap instance has
             * default initial capacity (16) and load factor (0.75).
             */
            CORE_IMPLICIT HashSet() {
                try { map = new HashMap<E, Object>(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new set containing the elements in the specified
             * collection.  The @c HashMap is created with default load factor
             * (0.75) and an initial capacity sufficient to contain the elements in
             * the specified collection.
             *
             * @param c the collection whose elements are to be placed into this set
             * @throws NullPointerException if the specified collection is null
             */
            CORE_IMPLICIT HashSet(Collection<E> const& c): HashSet() {
                try { HashSet::addAll(c); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new, empty set; the backing @c HashMap instance has
             * the specified initial capacity and the specified load factor.
             *
             * @param      initialCapacity   the initial capacity of the hash map
             * @param      loadFactor        the load factor of the hash map
             * @throws     IllegalArgumentException if the initial capacity is less
             *             than zero, or if the load factor is non-positive
             */
            CORE_IMPLICIT HashSet(gint initialCapacity, gfloat loadFactor) {
                try {
                    map = new HashMap<E, Object>(initialCapacity, loadFactor);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new, empty set; the backing @c HashMap instance has
             * the specified initial capacity and default load factor (0.75).
             *
             * @param      initialCapacity   the initial capacity of the hash table
             * @throws     IllegalArgumentException if the initial capacity is less
             *             than zero
             */
            CORE_IMPLICIT HashSet(gint initialCapacity) {
                try {
                    map = new HashMap<E, Object>(initialCapacity);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            HashSet(HashSet const& other) {
                try {
                    map = new HashMap<E, Object>(other.getMap());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            HashSet(HashSet&& other) CORE_NOTHROW {
                if (Class<LinkedHashMap<E, Object>>::hasInstance(other.getMap())) {
                    try {
                        map = new HashMap<E, Object>(other.getMap());
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                } else
                    UNSAFE::swapValues(map, other.map);
            }

            HashSet& operator=(HashSet const& other) {
                if (this != &other) {
                    try { getMap() = other.getMap(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
                return *this;
            }

            HashSet& operator=(HashSet&& other) CORE_NOTHROW {
                if (this != &other) {
                    getMap() = UNSAFE::moveInstance(other.getMap());
                }
                return *this;
            }

            /**
             * Returns an iterator over the elements in this set.  The elements
             * are returned in no particular order.
             *
             * @return an Iterator over the elements in this set
             * @see ConcurrentModificationException
             */
            Iterator2<E>& iterator() const override {
                try { return getMap().keySet().iterator(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this set.  The elements
             * are returned in no particular order.
             *
             * @return an Iterator over the elements in this set
             * @see ConcurrentModificationException
             */
            Iterator<E>& iterator() override {
                try { return getMap().keySet().iterator(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the number of elements in this set (its cardinality).
             *
             * @return the number of elements in this set (its cardinality)
             */
            gint size() const override {
                try { return getMap().size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this set contains no elements.
             *
             * @return @c true if this set contains no elements
             */
            gbool isEmpty() const override {
                try { return getMap().isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this set contains the specified element.
             * More formally, returns @c true if and only if this set
             * contains an element @c e such that
             * @c Objects.equals(o, e).
             *
             * @param o element whose presence in this set is to be tested
             * @return @c true if this set contains the specified element
             */
            gbool contains(const E& o) const override {
                try { return getMap().containsKey(o); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Adds the specified element to this set if it is not already present.
             * More formally, adds the specified element @c e to this set if
             * this set contains no element @c e2 such that
             * @c Objects.equals(e, e2).
             * If this set already contains the element, the call leaves the set
             * unchanged and returns @c false.
             *
             * @param e element to be added to this set
             * @return @c true if this set did not already contain the specified
             * element
             */
            gbool add(const E& e) override {
                try {
                    gint size = getMap().size();
                    getMap().put(e, null);
                    return getMap().size() > size;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the specified element from this set if it is present.
             * More formally, removes an element @c e such that
             * @c Objects.equals(o, e),
             * if this set contains such an element.  Returns @c true if
             * this set contained the element (or equivalently, if this set
             * changed as a result of the call).  (This set will not contain the
             * element once the call returns.)
             *
             * @param o object to be removed from this set, if present
             * @return @c true if the set contained the specified element
             */
            gbool remove(const E& o) override {
                try { return getMap().remove(o, null); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all of the elements from this set.
             * The set will be empty after this call returns.
             */
            void clear() override {
                try { getMap().clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a shallow copy of this @c HashSet instance: the elements
             * themselves are not cloned.
             *
             * @return a shallow copy of this set
             */
            Object& clone() const override {
                try {
                    HashSet& clone = UNSAFE::newInstance<HashSet>();
                    clone.getMap() = getMap();
                    return clone;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return getMap().keySet().toArray();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<E&> const& action) override {
                try { return getMap().keySet().forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<E> const& action) const override {
                try { return getMap().keySet().forEach(action); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~HashSet() override {
                BackendMap tmp = map;
                map = { };
                delete tmp;
            }

        private:
            /**
             * Constructs a new, empty linked hash set.  (This package private
             * constructor is only used by LinkedHashSet.) The backing
             * HashMap instance is a LinkedHashMap with the specified initial
             * capacity and the specified load factor.
             *
             * @param      initialCapacity   the initial capacity of the hash map
             * @param      loadFactor        the load factor of the hash map
             * @param      dummy             ignored (distinguishes this
             *             constructor from other int, float constructor.)
             * @throws     IllegalArgumentException if the initial capacity is less
             *             than zero, or if the load factor is non-positive
             */
            CORE_EXPLICIT HashSet(gint initialCapacity, gfloat loadFactor, gbool dummy) {
                CORE_IGNORE(dummy);
                try {
                    map = new LinkedHashMap<E, Object>(initialCapacity, loadFactor);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            virtual HashMap<E, Object>& getMap() {
                CORE_ASSERT(map != null);
                return *map;
            }

            virtual HashMap<E, Object> const& getMap() const {
                CORE_ASSERT(map != null);
                return *map;
            }
        };
    } // util
} // core

#endif //CORE24_HASHSET_H
