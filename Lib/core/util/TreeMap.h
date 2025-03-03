//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_TREEMAP_H
#define CORE24_TREEMAP_H

#include <core/util/NavigableMap.h>
#include <core/util/NavigableSet.h>

namespace core {
    namespace util {
        /**
         * A Red-Black tree based @b NavigableMap implementation.
         * The map is sorted according to the <em> natural
         * ordering</em> of its keys, or by a @c Comparator provided at map
         * creation time, depending on which constructor is used.
         *
         * <p>
         * This implementation provides guaranteed log(n) time cost for the
         * @c containsKey, @c get, @c put and @c remove
         * operations.  Algorithms are adaptations of those in Cormen, Leiserson, and
         * Rivest's <em>Introduction to Algorithms</em>.
         * </p>
         * <p>
         * Note that the ordering maintained by a tree map, like any sorted map, and
         * whether an explicit comparator is provided, must be <em>consistent
         * with</em> @c equals if this sorted map is to correctly implement the
         * @c Map interface.  (See @c Comparable or @c Comparator for a
         * precise definition of <em>consistent with equals</em>.)  This is so because
         * the @c Map interface is defined in terms of the @c equals
         * operation, but a sorted map performs all key comparisons using its
         * @c compareTo (or @c compare) method, so two keys that are deemed equal by
         * this method are, from the standpoint of the sorted map, equal.  The behavior
         * of a sorted map <em>is</em> well-defined even if its ordering is
         * inconsistent with @c equals; it just fails to obey the general contract
         * of the @c Map interface.
         * </p>
         * <p>
         * <b>Note that this implementation is not synchronized.</b>
         * If multiple threads access a map concurrently, and at least one of the
         * threads modifies the map structurally, it <em>must</em> be synchronized
         * externally.  (A structural modification is any operation that adds or
         * deletes one or more mappings; merely changing the value associated
         * with an existing key is not a structural modification.)  This is
         * typically accomplished by synchronizing on some object that naturally
         * encapsulates the map.
         * If no such object exists, the map should be "wrapped" using the
         * @b Collections::synchronizedSortedMap method.
         * This is best done at creation time, to prevent accidental
         * unsynchronized access to the map:
         * @code
         *   SortedMap &m = Collections.synchronizedSortedMap(TreeMap(...));
         * @endcode
         * </p>
         * <p>
         * The iterators returned by the @c iterator method of the collections
         * returned by all of this class's "collection view methods" are
         * <em>fail-fast</em>: if the map is structurally modified at any time after
         * the iterator is created, in any way except through the iterator's own
         * @c remove method, the iterator will throw a @b ConcurrentModificationException.
         * Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the future.
         * </p>
         * <p>
         * Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw @c ConcurrentModificationException on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <em>the fail-fast behavior of iterators
         * should be used only to detect bugs.</em>
         * </p>
         * <p>
         * The methods @b ceilingEntry, @b firstEntry, @b floorEntry, @b higherEntry,
         * @b lastEntry, @b lowerEntry, @b pollFirstEntry, and @b pollLastEntry
         * return @b Map::Entry instances that represent snapshots of mappings as
         * of the time of the call. They do <em>not</em> support mutation of the
         * underlying map via the optional @b setValue method.
         * </p>
         * <p>
         * The @b putFirst and @b putLast methods of this class
         * throw @c UnsupportedOperationException. The encounter order of mappings is determined
         * by the comparison method; therefore, explicit positioning is not supported.
         * </p>
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         *
         * @see Map
         * @see HashMap
         * @see Hashtable
         * @see Comparable
         * @see Comparator
         * @see Collection
         */
        template <class K, class V>
        class TreeMap final : public virtual NavigableMap<K, V> {
            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_TEMPLATES_AS_FRIEND(TreeSet);

            CORE_ALIAS(Entry, util::Entry<K, V>);
            CORE_ALIAS(Map, util::Map<K, V>);
            CORE_ALIAS(SequencedMap, util::SequencedMap<K, V>);
            CORE_ALIAS(SortedMap, util::SortedMap<K, V>);
            CORE_ALIAS(NavigableMap, util::NavigableMap<K, V>);

            CORE_ALIAS(UNSAFE, Object::UNSAFE);
            CORE_ALIAS(KEY, typename Class<K>::Pointer);
            CORE_ALIAS(VALUE, typename Class<V>::Pointer);
            CORE_ALIAS(COMPARATOR, typename Class<Comparator<K>>::Pointer);
            CORE_ALIAS(NavigableView, typename Class<NavigableMap>::Pointer);
            CORE_ALIAS(NavigableKeys, typename Class<NavigableSet<K>>::Pointer);
            CORE_ALIAS(SequencedKeys, typename SequencedMap::SequencedKeys);
            CORE_ALIAS(SequencedVals, typename SequencedMap::SequencedVals);
            CORE_ALIAS(SequencedEntries, typename SequencedMap::SequencedEntries);

            class Node;
            CORE_ALIAS(NODE, typename Class<Node>::Pointer);

            /**
             * The comparator used to maintain order in this tree map, or
             * null if it uses the natural ordering of its keys.
             */
            COMPARATOR cpr = { };

            /**
             * The root of this tree map
             */
            NODE root = { };

            /**
             * The number of entries in the tree
             */
            gint count = 0;

            /**
             * The number of structural modifications to the tree.
             */
            gint modCount = 0;

        public:
            /**
             * Constructs a new, empty tree map, using the natural ordering of its
             * keys.  All keys inserted into the map must implement the @b Comparable interface.
             * Furthermore, all such keys must be <em>mutually comparable</em>: @c k1.compareTo(k2) must not throw
             * a @c ClassCastException for any keys @c k1 and
             * @c k2 in the map.  If the user attempts to put a key into the
             * map that violates this constraint (for example, the user attempts to
             * put a string key into a map whose keys are integers), the
             * @code put(Object key, Object value) @endcode call will throw a
             * @c ClassCastException.
             */
            TreeMap() = default;

            /**
             * Constructs a new, empty tree map, ordered according to the given
             * comparator.  All keys inserted into the map must be <em>mutually
             * comparable</em> by the given comparator: @code comparator.compare(k1,
             * k2) @endcode must not throw a @c ClassCastException for any keys
             * @c k1 and @c k2 in the map.  If the user attempts to put
             * a key into the map that violates this constraint, the @code put(Object
             * key, Object value) @endcode call will throw a @c ClassCastException.
             *
             * @param comparator the comparator that will be used to order this map.
             *        If is equals to @c Comparator::naturalOrder(), the <b>natural
             *        ordering</b> of the keys will be used.
             */
            template <class X,
                      ClassOf(1)::OnlyIf<Class<X>::template isSuper<K>()> CaptureComparator = 1>
            CORE_EXPLICIT TreeMap(Comparator<X> const& comparator):
                cpr() {
                try {
                    Comparator<K>& kc = Comparator<K>::wrap(comparator);
                    if (kc != Comparator<K>::naturalOrder())
                        cpr = &kc;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new tree map containing the same mappings as the given
             * map, ordered according to the <em>natural ordering</em> of its keys.
             * All keys inserted into the new map must implement the @b Comparable
             * interface.  Furthermore, all such keys must be
             * <em>mutually comparable</em>: @c k1.compareTo(k2) must not throw
             * a @c ClassCastException for any keys @c k1 and
             * @c k2 in the map.  This method runs in n*log(n) time.
             *
             * @param  m the map whose mappings are to be placed in this map
             * @throws ClassCastException if the keys in m are not @b Comparable,
             *         or are not mutually comparable
             */
            template <class X, class Y,
                      ClassOf(1)::OnlyIf<Class<K>::template isSuper<X>()> CaptureKey = 1,
                      ClassOf(1)::OnlyIf<Class<V>::template isSuper<Y>()> CaptureValue = 1>
            CORE_EXPLICIT TreeMap(util::Map<X, Y> const& m) {
                try {
                    if (Class<util::SortedMap<X, Y>>::hasInstance(m)) {
                        util::SortedMap<X, Y> const& map = CORE_XCAST($(util::SortedMap<X, Y>) const, m);
                        Comparator<K> const& kc = Comparator<K>::wrap(m.comparator());
                        if (kc != Comparator<K>::naturalOrder())
                            cpr = &kc;

                        initializeTreeFromSortedTree(m.size(), map.entrySet().iterator());
                    } else if (Class<Map>::hasInstance(m)) {
                        TreeMap::putAll(CORE_XCAST(Map const, m));
                    } else {
                        for (util::Entry<X, Y> entry : m.entrySet())
                            TreeMap::put(entry.getKey(), entry.getValue());
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new tree map containing the same mappings and
             * using the same ordering as the specified sorted map.  This
             * method runs in linear time.
             *
             * @param  m the sorted map whose mappings are to be placed in this map,
             *         and whose comparator is to be used to sort this map
             */
            template <class X, class Y,
                      ClassOf(1)::OnlyIf<Class<K>::template isSuper<X>()> CaptureKey = 1,
                      ClassOf(1)::OnlyIf<Class<V>::template isSuper<Y>()> CaptureValue = 1>
            CORE_EXPLICIT TreeMap(util::SortedMap<X, Y> const& m) {
                try {
                    Comparator<K> const& kc = Comparator<K>::wrap(m.comparator());
                    if (kc != Comparator<K>::naturalOrder())
                        cpr = &kc;

                    initializeTreeFromSortedTree(m.size(), m.entrySet().iterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new tree map containing the same mappings and
             * using the same ordering as the specified tree map.  This
             * method runs in linear time.
             *
             * @param  m the tree map whose mappings are to be placed in this map,
             *         and whose comparator is to be used to sort this map
             */
            CORE_IMPLICIT TreeMap(TreeMap const& m) :
                cpr(m.cpr) {
                try {
                    initializeTreeFromSortedTree(m.size(), ((TreeMap&) m).entrySet().iterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a new tree map containing the same mappings and
             * using the same ordering as the specified tree map.  This
             * method runs in linear time.
             *
             * @param  m the tree map whose mappings are to be placed in this map,
             *         and whose comparator is to be used to sort this map
             */
            CORE_IMPLICIT TreeMap(TreeMap&& m) CORE_NOTHROW:
                cpr(m.cpr) {
                m.modCount += 1;
                UNSAFE::swapValues(count, m.count);
                UNSAFE::swapValues(root, m.root);
            }

            /**
             * Set this tree map mappings and ordering to the same mappings and
             * using the same ordering as the specified tree map.  This
             * method runs in linear time.
             *
             * @param  m the tree map whose mappings are to be placed in this map,
             *         and whose comparator is to be used to sort this map
             */
            TreeMap& operator=(TreeMap const& m) {
                if (this != &m) {
                    try {
                        TreeMap tmp(m);
                        TreeMap<K, V>::operator=(UNSAFE::moveInstance(tmp));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
                return *this;
            }

            /**
             * Set this tree map mappings and ordering to the same mappings and
             * using the same ordering as the specified tree map.  This
             * method runs in linear time.
             *
             * @param  m the tree map whose mappings are to be placed in this map,
             *         and whose comparator is to be used to sort this map
             */
            TreeMap& operator=(TreeMap&& m) CORE_NOTHROW {
                if (this != &m) {
                    m.modCount += 1;
                    modCount += 1;
                    UNSAFE::swapValues(cpr, m.cpr);
                    UNSAFE::swapValues(count, m.count);
                    UNSAFE::swapValues(root, m.root);
                }
                return *this;
            }

            /**
             * Destroy this tree map and him dependant views such as
             * keys, values and entries views
             */
            ~TreeMap() override {
                TreeMap::clear();

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

                if (reverse != null) {
                    UNSAFE::deleteRegInstance(*reverse);
                    reverse = null;
                }
            }

            // Query Operations

            /**
             * Returns the number of key-value mappings in this map.
             *
             * @return the number of key-value mappings in this map
             */
            gint size() const override { return count; }

            /**
             * Returns @c true if this map contains a mapping for the specified
             * key.
             *
             * @param key key whose presence in this map is to be tested
             * @return @c true if this map contains a mapping for the
             *         specified key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map uses natural ordering, or its comparator
             *         does not permit null keys
             */
            gbool containsKey(const K& key) const override {
                try { return exactNode(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this map maps one or more keys to the
             * specified value.  More formally, returns @c true if and only if
             * this map contains at least one mapping to a value @c v such
             * that @c value.equals(v).
             * This operation will probably require time linear in the map size
             * for most implementations.
             *
             * @param value value whose presence in this map is to be tested
             * @return @c true if a mapping to @c value exists;
             *         @c false otherwise
             */
            gbool containsValue(const V& value) const override {
                for (NODE node = firstNode(); node != null; node = successor(node)) {
                    V& v = valueOf(node);
                    if (value == v)
                        return true;
                }

                return false;
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or @c null if this map contains no mapping for the key.
             *
             * <p>
             * More formally, if this map contains a mapping from a key
             * @c k to a value @c v such that @c key compares
             * equal to @c k according to the map's ordering, then this
             * method returns @c v; otherwise it returns @c null.
             * (There can be at most one such mapping.)
             * </p>
             * <p>
             * A return value of @c null does not <em>necessarily</em>
             * indicate that the map contains no mapping for the key; it's also
             * possible that the map explicitly maps the key to @c null.
             * The @em containsKey operation may be used to
             * distinguish these two cases.
             * </p>
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map uses natural ordering, or its comparator
             *         does not permit null keys
             */
            V& get(const K& key) override {
                NODE node = exactNode(key);
                if (node == null)
                    return valueOf(node);
                MissingKeyException(key).throws($ftrace());
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or @c null if this map contains no mapping for the key.
             *
             * <p>
             * More formally, if this map contains a mapping from a key
             * @c k to a value @c v such that @c key compares
             * equal to @c k according to the map's ordering, then this
             * method returns @c v; otherwise it returns @c null.
             * (There can be at most one such mapping.)
             * </p>
             * <p>
             * A return value of @c null does not <em>necessarily</em>
             * indicate that the map contains no mapping for the key; it's also
             * possible that the map explicitly maps the key to @c null.
             * The @em containsKey operation may be used to
             * distinguish these two cases.
             * </p>
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map uses natural ordering, or its comparator
             *         does not permit null keys
             */
            const V& get(const K& key) const override {
                NODE node = exactNode(key);
                if (node == null)
                    return valueOf(node);
                MissingKeyException(key).throws($ftrace());
            }

            /**
             * Returns the comparator used to order the keys in this map.
             *
             * @return the comparator used to order the keys in this map.
             */
            const Comparator<K>& comparator() const override {
                if (cpr)
                    return *cpr;

                return Comparator<K>::naturalOrder();
            }

            const K& firstKey() const override {
                NODE f = firstNode();
                if (f)
                    return f->getKey();
                NoSuchElementException().throws($ftrace());
            }

            K& firstKey() override {
                NODE f = firstNode();
                if (f)
                    return f->getKey();
                NoSuchElementException().throws($ftrace());
            }

            const K& lastKey() const override {
                NODE l = lastNode();
                if (l)
                    return l->getKey();
                NoSuchElementException().throws($ftrace());
            }

            K& lastKey() override {
                NODE l = lastNode();
                if (l)
                    return l->getKey();
                NoSuchElementException().throws($ftrace());
            }

            /**
             * Copies all the mappings from the specified map to this map.
             * These mappings replace any mappings that this map had for any
             * of the keys currently in the specified map.
             *
             * @param  map mappings to be stored in this map
             * @throws ClassCastException if the class of a key or value in
             *         the specified map prevents it from being stored in this map
             * @throws IllegalArgumentException if the specified map is null or
             *         the specified map contains a null key and this map does not
             *         permit null keys
             */
            void putAll(const Map& map) override {
                gint size = map.size();
                if (count == 0 && size != 0 && Class<SortedMap>::hasInstance(map)) {
                    SortedMap const& m = CORE_XCAST(SortedMap const, m);
                    if (comparator() == m.comparator()) {
                        modCount += 1;
                        try {
                            SortedMap& m2 = CORE_CAST(SortedMap &, m);
                            initializeTreeFromSortedTree(m.size(), m2.entrySet().iterator());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        return;
                    }
                }
                Map::putAll(map);
            }

            /**
             * Associates the specified value with the specified key in this map.
             * If the map previously contained a mapping for the key, the old
             * value is replaced.
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             *
             * @return the previous value associated with @c key, or
             *         @c null if there was no mapping for @c key.
             *         (A @c null return can also indicate that the map
             *         previously associated @c null with @c key.)
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map uses natural ordering, or its comparator
             *         does not permit null keys
             */
            Object& put(const K& key, const V& value) override {
                try { return put(key, value, true); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& putIfAbsent(const K& key, const V& value) override {
                try { return put(key, value, false); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the mapping for this key from this TreeMap if present.
             *
             * @param  key key for which mapping should be removed
             * @return the previous value associated with @c key, or
             *         @c null if there was no mapping for @c key.
             *         (A @c null return can also indicate that the map
             *         previously associated @c null with @c key.)
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map uses natural ordering, or its comparator
             *         does not permit null keys
             */
            V& remove(const K& key) override {
                NODE node = exactNode(key);
                if (node != null) {
                    V& value = valueOf(node);
                    deleteNode(node);
                    return value;
                }
                MissingKeyException(key).throws($ftrace());
            }

            gbool remove(const K& key, const V& value) override {
                NODE node = exactNode(key);
                if (node != null && value == valueOf(node)) {
                    deleteNode(node);
                    return true;
                }
                return false;
            }

            /**
             * Removes all the mappings from this map.
             * The map will be empty after this call returns.
             */
            void clear() override {
                modCount += 1;
                count = 0;
                root = null;
            }

            /**
             * Returns a shallow copy of this @c TreeMap instance. (The keys and
             * values themselves are not cloned.)
             *
             * @return a shallow copy of this map
             */
            Object& clone() const override {
                TreeMap& clone = UNSAFE::newInstance<TreeMap>();
                try {
                    // Initialize clone with our mappings
                    TreeMap& map = CORE_CAST(TreeMap &, *this);
                    clone.initializeTreeFromSortedTree(count, map.entrySet().iterator());
                } catch (Throwable const& _) {
                    UNSAFE::deleteRegInstance(clone);
                    CloneNotSupportedException().throws($ftrace());
                }
                return clone;
            }

            // NavigableMap API methods

            /**
             *
             */
            const Entry& firstEntry() const override {
                NODE x = firstNode();
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            Entry& firstEntry() override {
                NODE x = firstNode();
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            const Entry& lastEntry() const override {
                NODE x = lastNode();
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            Entry& lastEntry() override {
                NODE x = lastNode();
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            const Entry& lowerEntry(const K& key) const override {
                NODE x = lowerNode(key);
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            Entry& lowerEntry(const K& key) override {
                NODE x = lowerNode(key);
                if (x != null)
                    return *x;
                MissingKeyException().throws($ftrace());
            }

            const K& lowerKey(const K& key) const override {
                NODE x = lowerNode(key);
                if (x != null)
                    return *x->key;
                MissingKeyException().throws($ftrace());
            }

            K& lowerKey(const K& key) override {
                NODE x = lowerNode(key);
                if (x != null)
                    return *x->key;
                NoSuchElementException().throws($ftrace());
            }

            const Entry& floorEntry(const K& key) const override {
                NODE x = floorNode(key);
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            Entry& floorEntry(const K& key) override {
                NODE x = floorNode(key);
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            const K& floorKey(const K& key) const override {
                NODE x = floorNode(key);
                if (x != null)
                    return *x->key;
                MissingKeyException().throws($ftrace());
            }

            K& floorKey(const K& key) override {
                NODE x = floorNode(key);
                if (x != null)
                    return *x->key;
                MissingKeyException().throws($ftrace());
            }

            const Entry& ceilingEntry(const K& key) const override {
                NODE x = ceilingNode(key);
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            Entry& ceilingEntry(const K& key) override {
                NODE x = ceilingNode(key);
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            const K& ceilingKey(const K& key) const override {
                NODE x = ceilingNode(key);
                if (x != null)
                    return *x->key;
                MissingKeyException().throws($ftrace());
            }

            K& ceilingKey(const K& key) override {
                NODE x = ceilingNode(key);
                if (x != null)
                    return *x->key;
                MissingKeyException().throws($ftrace());
            }

            const Entry& higherEntry(const K& key) const override {
                NODE x = higherNode(key);
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            Entry& higherEntry(const K& key) override {
                NODE x = higherNode(key);
                if (x != null)
                    return *x;
                NoSuchElementException().throws($ftrace());
            }

            const K& higherKey(const K& key) const override {
                NODE x = higherNode(key);
                if (x != null)
                    return *x->key;
                MissingKeyException().throws($ftrace());
            }

            K& higherKey(const K& key) override {
                NODE x = higherNode(key);
                if (x != null)
                    return *x->key;
                MissingKeyException().throws($ftrace());
            }

            /**
             * Returns a @b Set view of the keys contained in this map.
             *
             * <p>The set's iterator returns the keys in ascending order.
             * <p>The set is backed by the map, so changes to the map are
             * reflected in the set, and vice versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own @c remove operation), the results of
             * the iteration are undefined.  The set supports element removal,
             * which removes the corresponding mapping from the map, via the
             * @c Iterator::remove, @c Set::remove,
             * @c removeAll, @c retainAll, and @c clear
             * operations.  It does not support the @c add or @c addAll
             * operations.
             */
            Set<K>& keySet() override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Set<K>& keySet() const override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<K>& navigableKeySet() const override {
                NavigableKeys view = CORE_DCAST(NavigableKeys, keys);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<KeySet>((TreeMap&) *this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    keys = view;
                }
                return *view;
            }

            NavigableSet<K>& navigableKeySet() override {
                NavigableKeys view = CORE_DCAST(NavigableKeys, keys);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<KeySet>(*this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    keys = view;
                }
                return *view;
            }

            const NavigableSet<K>& descendingKeySet() const override {
                try { return reversed().navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<K>& descendingKeySet() override {
                try { return reversed().navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a @b Collection view of the values contained in this map.
             *
             * <p>The collection's iterator returns the values in ascending order
             * of the corresponding keys.
             *
             * <p>The collection is backed by the map, so changes to the map are
             * reflected in the collection, and vice versa.  If the map is
             * modified while an iteration over the collection is in progress
             * (except through the iterator's own @c remove operation),
             * the results of the iteration are undefined.  The collection
             * supports element removal, which removes the corresponding
             * mapping from the map, via the @c Iterator::remove,
             * @c Collection::remove, @c removeAll,
             * @c retainAll and @c clear operations.  It does not
             * support the @c add or @c addAll operations.
             */
            Collection<V>& values() override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Collection<V>& values() const override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a @b Set view of the mappings contained in this map.
             *
             * <p>The set's iterator returns the entries in ascending key order.
             *
             * <p>The set is backed by the map, so changes to the map are
             * reflected in the set, and vice versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own @c remove operation, or through the
             * @c setValue operation on a map entry returned by the
             * iterator) the results of the iteration are undefined.  The set
             * supports element removal, which removes the corresponding
             * mapping from the map, via the @c Iterator::remove,
             * @c Set::remove, @c removeAll, @c retainAll and
             * @c clear operations.  It does not support the
             * @c add or @c addAll operations.
             */
            Set<Entry>& entrySet() override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Set<Entry>& entrySet() const override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& descendingMap() const override {
                try { return reversed(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableMap& descendingMap() override {
                try { return reversed(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                       const K& toKey, gbool toInclusive) const override {
                try {
                    return UNSAFE::newInstance<BoundedMap>(CORE_CAST(TreeMap &, *this),
                                                           UNSAFE::copyInstance(fromKey), fromInclusive,
                                                           UNSAFE::copyInstance(toKey), toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                 const K& toKey, gbool toInclusive) override {
                try {
                    return UNSAFE::newInstance<BoundedMap>(*this,
                                                           UNSAFE::copyInstance(fromKey), fromInclusive,
                                                           UNSAFE::copyInstance(toKey), toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& headMap(const K& toKey, gbool inclusive) const override {
                try {
                    return UNSAFE::newInstance<HeadMap>(CORE_CAST(TreeMap &, *this),
                                                        UNSAFE::copyInstance(toKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableMap& headMap(const K& toKey, gbool inclusive) override {
                try {
                    return UNSAFE::newInstance<HeadMap>(*this,
                                                        UNSAFE::copyInstance(toKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& tailMap(K const& fromKey, gbool inclusive) const override {
                try {
                    return UNSAFE::newInstance<TailMap>(CORE_CAST(TreeMap &, *this),
                                                        UNSAFE::copyInstance(fromKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableMap& tailMap(K const& fromKey, gbool inclusive) override {
                try {
                    return UNSAFE::newInstance<TailMap>(*this,
                                                        UNSAFE::copyInstance(fromKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedMap& subMap(const K& fromKey, const K& toKey) const override {
                try {
                    return subMap(fromKey, true, toKey, false);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedMap& subMap(const K& fromKey, const K& toKey) override {
                try {
                    return subMap(fromKey, true, toKey, false);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedMap& headMap(const K& toKey) const override {
                try { return headMap(toKey, false); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedMap& headMap(const K& toKey) override {
                try { return headMap(toKey, false); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedMap& tailMap(const K& fromKey) const override {
                try { return tailMap(fromKey, true); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedMap& tailMap(const K& fromKey) override {
                try { return tailMap(fromKey, true); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool replace(const K& key, const V& oldValue, const V& newValue) override {
                try {
                    NODE node = exactNode(key);
                    if (node != null && oldValue == valueOf(node)) {
                        if (&oldValue != &newValue)
                            node->value = &UNSAFE::copyInstance(newValue);
                        return true;
                    }
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& replace(const K& key, const V& value) override {
                NODE node = exactNode(key);
                if (node == null) {
                    V& old = valueOf(node);
                    node->value = &UNSAFE::copyInstance(value);
                    return old;
                }
                return null;
            }

            void forEach(const function::BiConsumer<K&, V&>& action) override {
                gint mc = modCount;
                try {
                    for (NODE x = firstNode(); x != null && mc == modCount; x = successor(x))
                        action.accept(x->getKey(), x->getValue());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                if (mc != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void forEach(const function::BiConsumer<K, V>& action) const override {
                gint mc = modCount;
                try {
                    for (NODE x = firstNode(); x != null && mc == modCount; x = successor(x))
                        action.accept(x->getKey(), x->getValue());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                if (mc != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void replaceAll(const function::BiFunction<K&, V&, V>& function) override {
                gint mc = modCount;
                for (NODE x = firstNode(); x && mc == modCount; x = successor(x))
                    x->value = &UNSAFE::copyInstance(function.apply(x->getKey(), x->getValue()));

                if (mc != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            SequencedSet<K>& sequencedKeySet() override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedSet<K>& sequencedKeySet() const override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<V>& sequencedValues() override {
                SequencedVals view = CORE_DCAST(SequencedVals, vals);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<Values>(*this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    vals = view;
                }
                return *view;
            }

            const SequencedCollection<V>& sequencedValues() const override {
                SequencedVals view = CORE_DCAST(SequencedVals, vals);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<Values>((TreeMap&) *this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    vals = view;
                }
                return *view;
            }

            SequencedSet<Entry>& sequencedEntrySet() override {
                SequencedEntries view = CORE_DCAST(SequencedEntries, entries);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<EntrySet>(*this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    entries = view;
                }
                return *view;
            }

            const SequencedSet<Entry>& sequencedEntrySet() const override {
                SequencedEntries view = CORE_DCAST(SequencedEntries, entries);
                if (!view) {
                    try { view = &UNSAFE::newInstance<EntrySet>((TreeMap&) *this, false); } catch (Throwable const&
                        ex) { ex.throws($ftrace()); }
                    entries = view;
                }
                return *view;
            }

            NavigableMap const& reversed() const override {
                NavigableView view = CORE_DCAST(NavigableView, reverse);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<Reverse>((TreeMap&) *this);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    reverse = view;
                }
                return *view;
            }

            NavigableMap& reversed() override {
                NavigableView view = CORE_DCAST(NavigableView, reverse);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<Reverse>((TreeMap&) *this);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    reverse = view;
                }
                return *view;
            }

        private:
            enum Color {
                BLACK, RED
            };

            class Node final : public Entry {
            public:
                KEY key = null;
                VALUE value = null;
                NODE left = null;
                NODE right = null;
                NODE parent = null;
                Color color = BLACK;

                /**
                 * Make a new cell with given key, value, and parent, and with
                 * @c null child links, and BLACK color.
                 */
                CORE_EXPLICIT Node(K& key, V& value, NODE parent) :
                    key(&key),
                    value(&value),
                    parent(parent) {}

                /**
                 * Returns the key.
                 *
                 * @return the key
                 */
                K& getKey() override { return *key; }

                /**
                 * Returns the key.
                 *
                 * @return the key
                 */
                const K& getKey() const override { return *key; }

                /**
                 * Returns the value associated with the key.
                 *
                 * @return the value associated with the key
                 */
                V& getValue() override { return *value; }

                /**
                 * Returns the value associated with the key.
                 *
                 * @return the value associated with the key
                 */
                const V& getValue() const override { return *value; }

                /**
                 * Replaces the value currently associated with the key with the given
                 * value.
                 *
                 * @return the value associated with the key before this method was
                 *         called
                 */
                V& setValue(const V& newValue) override {
                    VALUE oldValue = value;
                    value = &UNSAFE::copyInstance(newValue);
                    return *oldValue;
                }

                gint hash() const override {
                    return (key == null ? 0 : key->hash()) ^ (value == null ? 0 : value->hash());
                }

                ~Node() override {
                    left = right = parent = null;
                    key = null;
                    value = null;
                }
            };

            /**
             * Returns this map's entry for the given key, or @c null if the map
             * does not contain an entry for the key.
             *
             * @return this map's entry for the given key, or @c null if the map
             *         does not contain an entry for the key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map uses natural ordering, or its comparator
             *         does not permit null keys
             */
            NODE exactNode(K const& key) const {
                NODE p = root;
                while (p != null) {
                    gint r = compare(key, *p->key);
                    if (r == 0)
                        return p;
                    p = r < 0 ? p->left : p->right;
                }
                return null;
            }

            /**
             * Gets the entry corresponding to the specified key; if no such entry
             * exists, returns the entry for the least key greater than the specified
             * key; if no such entry exists (i.e., the greatest key in the Tree is less
             * than the specified key), returns @c null.
             */
            NODE ceilingNode(K const& key) const {
                NODE p = root;
                while (p != null) {
                    gint r = compare(key, *p->key);
                    if (r < 0) {
                        if (p->left != null)
                            p = p->left;
                        else
                            return p;
                    } else if (r > 0) {
                        if (p->right != null)
                            p = p->right;
                        else {
                            NODE parent = p->parent;
                            NODE ch = p;
                            while (parent != null && ch == parent->right) {
                                ch = parent;
                                parent = parent->parent;
                            }
                            return parent;
                        }
                    } else
                        return p;
                }
                return null;
            }

            /**
             * Gets the entry corresponding to the specified key; if no such entry
             * exists, returns the entry for the greatest key less than the specified
             * key; if no such entry exists (i.e., the least key in the Tree is greater
             * than the specified key), returns @c null.
             */
            NODE floorNode(K const& key) const {
                NODE p = root;
                while (p != null) {
                    gint r = compare(key, *p->key);
                    if (r < 0) {
                        if (p->right != null)
                            p = p->right;
                        else
                            return p;
                    } else if (r > 0) {
                        if (p->left != null)
                            p = p->left;
                        else {
                            NODE parent = p->parent;
                            NODE ch = p;
                            while (parent != null && ch == parent->left) {
                                ch = parent;
                                parent = parent->parent;
                            }
                            return parent;
                        }
                    } else
                        return p;
                }
                return null;
            }

            /**
             * Returns the entry for the least key greater than the specified key; if
             * no such entry exists (i.e., the greatest key in the Tree is less than
             * or equal to the specified key), returns @c null.
             */
            NODE higherNode(K const& key) const {
                NODE p = root;
                while (p != null) {
                    const gint r = compare(key, *p->key);
                    if (r < 0) {
                        if (p->left != null)
                            p = p->left;
                        else
                            return p;
                    } else {
                        if (p->right != null)
                            p = p->right;
                        else {
                            NODE parent = p->parent;
                            NODE ch = p;
                            while (parent != null && ch == parent->right) {
                                ch = parent;
                                parent = parent->parent;
                            }
                            return parent;
                        }
                    }
                }
                return null;
            }

            /**
             * Returns the entry for the greatest key less than the specified key; if
             * no such entry exists (i.e., the least key in the Tree is greater than
             * or equal to the specified key), returns @c null.
             */
            NODE lowerNode(K const& key) const {
                NODE p = root;
                while (p != null) {
                    const gint r = compare(key, *p->key);
                    if (r > 0) {
                        if (p->right != null)
                            p = p->right;
                        else
                            return p;
                    } else {
                        if (p->left != null)
                            p = p->left;
                        else {
                            NODE parent = p->parent;
                            NODE ch = p;
                            while (parent != null && ch == parent->left) {
                                ch = parent;
                                parent = parent->parent;
                            }
                            return parent;
                        }
                    }
                }
                return null;
            }

            /**
             * Returns the first Entry in the TreeMap (according to the TreeMap's
             * key-sort function).  Returns null if the TreeMap is empty.
             */
            NODE firstNode() const {
                NODE p = root;
                if (p != null)
                    while (p->left != null)
                        p = p->left;
                return p;
            }

            /**
             * Returns the last Entry in the TreeMap (according to the TreeMap's
             * key-sort function).  Returns null if the TreeMap is empty.
             */
            NODE lastNode() const {
                NODE p = root;
                if (p != null)
                    while (p->right != null)
                        p = p->right;
                return p;
            }

            /**
             * Returns the successor of the specified Entry, or null if no such.
             */
            static NODE successor(NODE t) {
                if (t == null)
                    return null;
                if (t->right != null) {
                    NODE p = t->right;
                    while (p->left != null)
                        p = p->left;
                    return p;
                }
                NODE p = t->parent;
                NODE ch = t;
                while (p != null && ch == p->right) {
                    ch = p;
                    p = p->parent;
                }
                return p;
            }

            /**
             * Returns the predecessor of the specified Entry, or null if no such.
             */
            static NODE predecessor(NODE t) {
                if (t == null)
                    return null;
                if (t->left != null) {
                    NODE p = t->left;
                    while (p->right != null)
                        p = p->right;
                    return p;
                }
                NODE p = t->parent;
                NODE ch = t;
                while (p != null && ch == p->left) {
                    ch = p;
                    p = p->parent;
                }
                return p;
            }

            Object& put(K const& key, V const& value, gbool replace) {
                NODE t = root;
                if (t == null) {
                    putNode(key, value);
                    return null;
                }
                gint r = 0;
                NODE parent = null;
                if (cpr == null) {
                    Comparable<K> const& k = CORE_XCAST(Comparable<K> const, key);
                    do {
                        parent = t;
                        r = k.compareTo(*t->key);
                        if (r == 0) {
                            // mapping found (starting replacement)
                            VALUE val = t->value;
                            if (replace)
                                t->value = &UNSAFE::copyInstance(value);
                            return *val;
                        }
                        t = (r < 0) ? t->left : t->right;
                    } while (t != null);
                } else {
                    do {
                        parent = t;
                        r = cpr->compare(key, *t->key);
                        if (r == 0) {
                            // mapping found (starting replacement)
                            VALUE val = t->value;
                            if (replace)
                                t->value = &UNSAFE::copyInstance(value);
                            return *val;
                        }
                        t = (r < 0) ? t->left : t->right;
                    } while (t != null);
                }
                putNode(key, value, parent, r < 0);
                return null;
            }

            void putNode(K const& key, V const& value) {
                CORE_IGNORE(compare(key, key));
                root = &UNSAFE::newInstance<Node>(UNSAFE::copyInstance(key), UNSAFE::copyInstance(value), null);
                count += 1;
                modCount += 1;
            }

            void putNode(K const& key, V const& value, NODE parent, gbool toLeft) {
                NODE x = &UNSAFE::newInstance<Node>(UNSAFE::copyInstance(key), UNSAFE::copyInstance(value), parent);
                (toLeft ? parent->left : parent->right) = x;
                afterInsertion(x);
                count += 1;
                modCount += 1;
            }

            gint compare(K const& key1, K const& key2) const {
                if (cpr == null) {
                    if (!Class<Comparable<K>>::hasInstance(key1))
                        ClassCastException().throws($ftrace());
                    Comparable<K> const& key = CORE_XCAST(Comparable<K> const, key1);
                    return key.compareTo(key2);
                }
                return cpr->compare(key1, key2);
            }

            /**
             * Delete node p, and then rebalance the tree.
             */
            void deleteNode(NODE p) {
                modCount += 1;
                count -= 1;

                // If strictly internal, move successor's element to p and then make p
                // point to successor.
                if (p->left != null && p->right != null) {
                    // p has 2 children
                    NODE s = successor(p);
                    NODE pr1 = p->parent;
                    NODE pr2 = s->parent;
                    if (pr1 != null) {
                        if (pr1->left == p) {
                            pr1->left = s;
                            if (pr2 != null) {
                                if (pr2->left == s)
                                    pr2->left = p;
                                else
                                    pr2->right = p;
                                p->parent = pr2;
                            } else
                                p->parent = null;
                        } else {
                            pr1->right = s;
                            if (pr2 != null) {
                                if (pr2->left == s)
                                    pr2->left = p;
                                else
                                    pr2->right = p;
                                p->parent = pr2;
                            } else
                                p->parent = null;
                        }
                        s->parent = pr1;
                    } else if (pr2 != null) {
                        if (pr2->left == s) {
                            pr2->left = p;
                            if (pr1 != null) {
                                if (pr1->left == p)
                                    pr1->left = s;
                                else
                                    pr1->right = s;
                                s->parent = pr1;
                            } else
                                s->parent = null;
                        } else {
                            pr2->right = p;
                            if (pr1 != null) {
                                if (pr1->left == p)
                                    pr1->left = s;
                                else
                                    pr1->right = s;
                                s->parent = pr1;
                            } else
                                s->parent = null;
                        }
                        p->parent = pr2;
                    }
                    if (pr1 == null)
                        root = s;
                    if (pr2 == null)
                        root = p;
                    UNSAFE::swapValues(s->left, p->left);
                    UNSAFE::swapValues(s->right, p->right);
                    UNSAFE::swapValues(s->color, p->color);
                    if (s->left != 0)
                        s->left->parent = s;
                    if (s->right != 0)
                        s->right->parent = s;
                    if (p->left != null)
                        p->left->parent = p;
                    if (p->right != null)
                        p->right->parent = p;
                }

                // Start fixup at replacement node, if i exists.
                NODE repl = (p->left != null ? p->left : p->right);

                if (repl != null) {
                    // Link replacement to parent
                    repl->parent = p->parent;
                    if (p->parent == null)
                        root = repl;
                    else if (p == p->parent->left)
                        p->parent->left = repl;
                    else
                        p->parent->right = repl;

                    // Null out links so they are OK to use by afterDeletion.
                    p->left = p->right = p->parent = null;

                    // Fix replacement
                    if (p->color == BLACK)
                        afterDeletion(p);
                } else if (p->parent == null) {
                    // return if we are the only node.
                    root = null;
                } else {
                    //  No children. Use self as phantom replacement and unlink.
                    if (p->color == BLACK)
                        afterDeletion(p);

                    if (p->parent != null) {
                        if (p == p->parent->left)
                            p->parent->left = null;
                        else if (p == p->parent->right)
                            p->parent->right = null;
                        p->parent = null;
                    }
                }
            }

            /**
             * Balancing operations.
             *
             * Implementations of rebalancings during insertion and deletion are
             * slightly different from the CLR version.  Rather than using dummy
             * nil-nodes, we use a set of accessors that deal properly with null.  They
             * are used to avoid messiness surrounding null-ness checks in the main
             * algorithms.
             */

            /**
             *
             */
            static Color colorOf(NODE p) { return (p == null) ? BLACK : p->color; }

            static void setColor(NODE p, Color color) {
                if (p != null)
                    p->color = color;
            }

            static NODE parentOf(NODE p) { return (p == null) ? null : p->parent; }

            static NODE leftOf(NODE p) { return (p == null) ? null : p->left; }

            static NODE rightOf(NODE p) { return (p == null) ? null : p->right; }

            static K& keyOf(NODE node) { return node->getKey(); }

            static V& valueOf(NODE node) { return node->getValue(); }

            static NODE nextOf(NODE node) { return successor(node); }

            static NODE prevOf(NODE node) { return predecessor(node); }

            // From CLR
            void rotateLeft(NODE p) {
                if (p != null) {
                    NODE r = p->right;
                    p->right = r->left;
                    if (r->left != null)
                        r->left->parent = p;
                    r->parent = p->parent;
                    if (p->parent == null)
                        root = r;
                    else if (p->parent->left == p)
                        p->parent->left = r;
                    else
                        p->parent->right = r;
                    r->left = p;
                    p->parent = r;
                }
            }

            // From CLR
            void rotateRight(NODE p) {
                if (p != null) {
                    NODE l = p->left;
                    p->left = l->right;
                    if (l->right != null)
                        l->right->parent = p;
                    l->parent = p->parent;
                    if (p->parent == null)
                        root = l;
                    else if (p->parent->right == p)
                        p->parent->right = l;
                    else
                        p->parent->left = l;
                    l->right = p;
                    p->parent = l;
                }
            }

            void afterInsertion(NODE x) {
                x->color = RED;

                while (x != null && x != root && x->parent->color == RED) {
                    if (parentOf(x) == leftOf(parentOf(parentOf(x)))) {
                        NODE y = rightOf(parentOf(parentOf(x)));
                        if (colorOf(y) == RED) {
                            setColor(parentOf(x), BLACK);
                            setColor(y, BLACK);
                            setColor(parentOf(parentOf(x)), RED);
                            x = parentOf(parentOf(x));
                        } else {
                            if (x == rightOf(parentOf(x))) {
                                x = parentOf(x);
                                rotateLeft(x);
                            }
                            setColor(parentOf(x), BLACK);
                            setColor(parentOf(parentOf(x)), RED);
                            rotateRight(parentOf(parentOf(x)));
                        }
                    } else {
                        NODE y = leftOf(parentOf(parentOf(x)));
                        if (colorOf(y) == RED) {
                            setColor(parentOf(x), BLACK);
                            setColor(y, BLACK);
                            setColor(parentOf(parentOf(x)), RED);
                            x = parentOf(parentOf(x));
                        } else {
                            if (x == leftOf(parentOf(x))) {
                                x = parentOf(x);
                                rotateRight(x);
                            }
                            setColor(parentOf(x), BLACK);
                            setColor(parentOf(parentOf(x)), RED);
                            rotateLeft(parentOf(parentOf(x)));
                        }
                    }
                }
                root->color = BLACK;
            }

            void afterDeletion(NODE x) {
                while (x != null && x != root && colorOf(x) == BLACK) {
                    if (x == leftOf(parentOf(x))) {
                        NODE sib = rightOf(parentOf(x));

                        if (colorOf(sib) == RED) {
                            setColor(sib, BLACK);
                            setColor(parentOf(x), RED);
                            rotateLeft(parentOf(x));
                            sib = rightOf(parentOf(x));
                        }

                        if (colorOf(leftOf(sib)) == BLACK &&
                            colorOf(rightOf(sib)) == BLACK) {
                            setColor(sib, RED);
                            x = parentOf(x);
                        } else {
                            if (colorOf(rightOf(sib)) == BLACK) {
                                setColor(leftOf(sib), BLACK);
                                setColor(sib, RED);
                                rotateRight(sib);
                                sib = rightOf(parentOf(x));
                            }
                            setColor(sib, colorOf(parentOf(x)));
                            setColor(parentOf(x), BLACK);
                            setColor(rightOf(sib), BLACK);
                            rotateLeft(parentOf(x));
                            x = root;
                        }
                    } else {
                        // symmetric
                        NODE sib = leftOf(parentOf(x));

                        if (colorOf(sib) == RED) {
                            setColor(sib, BLACK);
                            setColor(parentOf(x), RED);
                            rotateRight(parentOf(x));
                            sib = leftOf(parentOf(x));
                        }

                        if (colorOf(rightOf(sib)) == BLACK &&
                            colorOf(leftOf(sib)) == BLACK) {
                            setColor(sib, RED);
                            x = parentOf(x);
                        } else {
                            if (colorOf(leftOf(sib)) == BLACK) {
                                setColor(rightOf(sib), BLACK);
                                setColor(sib, RED);
                                rotateLeft(sib);
                                sib = leftOf(parentOf(x));
                            }
                            setColor(sib, colorOf(parentOf(x)));
                            setColor(parentOf(x), BLACK);
                            setColor(leftOf(sib), BLACK);
                            rotateRight(parentOf(x));
                            x = root;
                        }
                    }
                }

                setColor(x, BLACK);
            }

            /**
             * Linear time tree building algorithm from sorted data.  Can accept keys
             * and/or values from iterator.
             *
             * It is assumed that the comparator of the TreeMap is already set prior
             * to calling this method.
             *
             * @param size the number of keys (or key-value pairs) to be read from
             *        the iterator or stream
             * @param it If non-null, new entries are created from entries
             *        or keys read from this iterator.
             */
            template <class X, class Y>
            void initializeTreeFromSortedTree(gint size, Iterator<util::Entry<X, Y>>& it) {
                root = initializeNodesFromSortedTree(0, 0, size - 1, redLevel(size), it);
                count = size;
                UNSAFE::deleteRegInstance(it, true);
            }

            /** Intended to be called only from @c TreeSet::addAll */
            template <class X>
            void initializeTreeFromSortedSet(SortedSet<X>& set, V& dummy) {
                try {
                    Iterator<X>& it = set.iterator();
                    gint size = set.size();
                    root = initializeNodesFromSortedSet(0, 0, size - 1, redLevel(size), it, dummy);
                    count = size;
                    UNSAFE::deleteRegInstance(it, true);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Recursive "helper method" that does the real work of the
             * previous method.  Identically named parameters have
             * identical definitions.  Additional parameters are documented below.
             * It is assumed that the comparator and size fields of the TreeMap are
             * already set prior to calling this method.  (It ignores both fields.)
             *
             * @param level the current level of tree. Initial call should be 0.
             * @param lo the first element index of this subtree. Initial should be 0.
             * @param hi the last element index of this subtree.  Initial should be
             *        size-1.
             * @param redLevel the level at which nodes should be red.
             *        Must be equal to computeRedLevel for tree of this size.
             * @param it the entries iterator
             */
            template <class X, class Y>
            NODE initializeNodesFromSortedTree(gint level, gint lo, gint hi, gint redLevel,
                                               Iterator<util::Entry<X, Y>>& it) {
                /*
                 * Strategy: The root is the middlemost element. To get to it, we
                 * have to first recursively construct the entire left subtree,
                 * to grab all of its elements. We can then proceed with right
                 * subtree.
                 *
                 * The lo and hi arguments are the minimum and maximum
                 * indices to pull out of the iterator or stream for current subtree.
                 * They are not actually indexed, we just proceed sequentially,
                 * ensuring that items are extracted in corresponding order.
                 */
                if (hi < lo)
                    return null;

                gint mid = (lo + hi) >> 1;

                NODE left = null;
                if (lo < mid)
                    left = initializeNodesFromSortedTree(level + 1, lo, mid, redLevel, it);

                // extract key and/or value from iterator
                util::Entry<X, Y>& entry = it.next();
                K& key = entry.getKey();
                V& value = entry.getValue();

                NODE middle = &UNSAFE::newInstance<Node>(key, value, null);

                // color nodes in non-full bottommost level red
                if (level == redLevel)
                    middle->color = RED;

                if (left != null) {
                    middle->left = left;
                    left->parent = middle;
                }

                if (mid < hi) {
                    NODE right = initializeNodesFromSortedTree(level + 1, mid + 1, hi, redLevel, it);
                    middle->right = right;
                    right->parent = middle;
                }

                return middle;
            }

            /**
             * Recursive "helper method" that does the real work of the
             * previous method.  Identically named parameters have
             * identical definitions.  Additional parameters are documented below.
             * It is assumed that the comparator and size fields of the TreeMap are
             * already set prior to calling this method.  (It ignores both fields.)
             *
             * @param level the current level of tree. Initial call should be 0.
             * @param lo the first element index of this subtree. Initial should be 0.
             * @param hi the last element index of this subtree.  Initial should be
             *        size-1.
             * @param redLevel the level at which nodes should be red.
             *        Must be equal to computeRedLevel for tree of this size.
             * @param it the set iterator
             * @param dummy the dummy value used by set
             */
            template <class X>
            NODE initializeNodesFromSortedSet(gint level, gint lo, gint hi, gint redLevel,
                                              Iterator<X>& it, V& dummy) {
                /*
                 * Strategy: The root is the middlemost element. To get to it, we
                 * have to first recursively construct the entire left subtree,
                 * to grab all of its elements. We can then proceed with right
                 * subtree.
                 *
                 * The lo and hi arguments are the minimum and maximum
                 * indices to pull out of the iterator or stream for current subtree.
                 * They are not actually indexed, we just proceed sequentially,
                 * ensuring that items are extracted in corresponding order.
                 */
                if (hi < lo)
                    return null;

                gint mid = (lo + hi) >> 1;

                NODE left = null;
                if (lo < mid)
                    left = initializeNodesFromSortedSet(level + 1, lo, mid, redLevel, it, dummy);

                // extract key and/or value from iterator
                K& key = it.next();
                V& value = dummy;

                NODE middle = &UNSAFE::newInstance<Node>(key, value, null);

                // color nodes in non-full bottommost level red
                if (level == redLevel)
                    middle->color = RED;

                if (left != null) {
                    middle->left = left;
                    left->parent = middle;
                }

                if (mid < hi) {
                    NODE right = initializeNodesFromSortedSet(level + 1, mid + 1, hi, redLevel, it, dummy);
                    middle->right = right;
                    right->parent = middle;
                }

                return middle;
            }

            /**
             * Finds the level down to which to assign all nodes BLACK.  This is the
             * last full level of the complete binary tree produced by buildTree.
             * The remaining nodes are colored RED. (This makes a nice set of
             * color assignments wrt future insertions.) This level number is
             * computed by finding the number of splits needed to reach the zeroth
             * node.
             *
             * @param size the (non-negative) number of keys in the tree to be built
             */
            static gint redLevel(gint size) { return 31 - Integer::numberOfLeadingZeros(size + 1); }

            // ::::::::::::::::::::: TreeMap views Fields :::::::::::::::::::::::::::::

            /**
             * Fields initialized to contain an instance of the entry set view
             * the first time this view is requested.  Views are stateless, so
             * there's no reason to create more than one.
             */

            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);
            CORE_IMPORT_FIELD_OR_METHOD(SequencedMap, reverse);

            // ::::::::::::::::::::: TreeMap views Destructors Utilities :::::::::::::::::::::::::::::

            static void detachReverse(NavigableMap const& map) { NavigableMap::detachReverse(map); }

            static void detachEntries(NavigableMap const& map, gbool reverseOrder) {
                if (!reverseOrder)
                    NavigableMap::detachEntries(map);
                else
                    NavigableMap::detachEntries(map.reversed());
            }

            static void detachKeys(NavigableMap const& map, gbool reverseOrder) {
                if (!reverseOrder)
                    NavigableMap::detachKeys(map);
                else
                    NavigableMap::detachKeys(map.reversed());
            }

            static void detachVals(NavigableMap const& map, gbool reverseOrder) {
                if (!reverseOrder)
                    NavigableMap::detachVals(map);
                else
                    NavigableMap::detachVals(map.reversed());
            }

            // ::::::::::::::::::::: TreeMap views Utilities :::::::::::::::::::::::::::::

            Iterator<K>& keyIterator(gbool reverseOrder) {
                class Itr final : public Iterator<K> {
                    TreeMap<K, V>& map;
                    NODE cursor = { };
                    NODE last = { };
                    gint mc = 0;
                    gbool reverseOrder = false;

                public:
                    CORE_EXPLICIT Itr(TreeMap<K, V>& map, gbool reverseOrder):
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? map.lastNode() : map.firstNode();
                        mc = map.modCount;
                    }

                    gbool hasNext() const override { return cursor; }

                    K& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(last) : successor(last);
                        return last->getKey();
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.deleteNode(x);
                        mc = map.modCount;
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &map == &it.map && reverseOrder == it.reverseOrder && cursor == it.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& keyIterator(gbool reverseOrder) const {
                class Itr final : public Iterator2<K> {
                    TreeMap<K, V> const& map;
                    NODE cursor = { };
                    NODE last = { };
                    gint mc = 0;
                    gbool reverseOrder = false;

                public:
                    CORE_EXPLICIT Itr(TreeMap<K, V> const& map, gbool reverseOrder):
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? map.lastNode() : map.firstNode();
                        mc = map.modCount;
                    }

                    gbool hasNext() const override { return cursor; }

                    K const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(last) : successor(last);
                        return last->getKey();
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &map == &it.map && reverseOrder == it.reverseOrder && cursor == it.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<V>& valueIterator(gbool reverseOrder) {
                class Itr final : public Iterator<V> {
                    TreeMap<K, V>& map;
                    NODE cursor = { };
                    NODE last = { };
                    gint mc = 0;
                    gbool reverseOrder = false;

                public:
                    CORE_EXPLICIT Itr(TreeMap<K, V>& map, gbool reverseOrder):
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? map.lastNode() : map.firstNode();
                        mc = map.modCount;
                    }

                    gbool hasNext() const override { return cursor; }

                    V& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(last) : successor(last);
                        return last->getValue();
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.deleteNode(x);
                        mc = map.modCount;
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &map == &it.map && reverseOrder == it.reverseOrder && cursor == it.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& valueIterator(gbool reverseOrder) const {
                class Itr final : public Iterator2<V> {
                    TreeMap<K, V> const& map;
                    NODE cursor = { };
                    NODE last = { };
                    gint mc = 0;
                    gbool reverseOrder = false;

                public:
                    CORE_EXPLICIT Itr(TreeMap<K, V> const& map, gbool reverseOrder):
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? map.lastNode() : map.firstNode();
                        mc = map.modCount;
                    }

                    gbool hasNext() const override { return cursor; }

                    V const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(last) : successor(last);
                        return last->getValue();
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &map == &it.map && reverseOrder == it.reverseOrder && cursor == it.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<Entry>& entryIterator(gbool reverseOrder) {
                class Itr final : public Iterator<Entry> {
                    TreeMap<K, V>& map;
                    NODE cursor = { };
                    NODE last = { };
                    gint mc = 0;
                    gbool reverseOrder = false;

                public:
                    CORE_EXPLICIT Itr(TreeMap<K, V>& map, gbool reverseOrder):
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? map.lastNode() : map.firstNode();
                        mc = map.modCount;
                    }

                    gbool hasNext() const override { return cursor; }

                    Entry& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(last) : successor(last);
                        return *last;
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.deleteNode(x);
                        mc = map.modCount;
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &map == &it.map && reverseOrder == it.reverseOrder && cursor == it.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry>& entryIterator(gbool reverseOrder) const {
                class Itr final : public Iterator2<Entry> {
                    TreeMap<K, V> const& map;
                    NODE cursor = { };
                    NODE last = { };
                    gint mc = 0;
                    gbool reverseOrder = false;

                public:
                    CORE_EXPLICIT Itr(TreeMap<K, V> const& map, gbool reverseOrder):
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? map.lastNode() : map.firstNode();
                        mc = map.modCount;
                    }

                    gbool hasNext() const override { return cursor; }

                    Entry const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(last) : successor(last);
                        return *last;
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &map == &it.map && reverseOrder == it.reverseOrder && cursor == it.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            // ::::::::::::::::::::: TreeMap views Classes :::::::::::::::::::::::::::::

            /**
             * The keys view class
             */
            class KeySet;

            /**
             * The values view class
             */
            class Values;

            /**
             * The entries view
             */
            class EntrySet;

            /**
             * The reverse view (descending view)
             */
            class Reverse;

            /**
             * The portion of map base view (ascending view)
             */
            class SubMap;

            /**
             * The portion of map view (ascending view)
             * limited by the max key (tail) value.
             */
            class HeadMap;

            /**
             * The portion of map view (ascending view)
             * limited by the min key (head) value.
             */
            class TailMap;

            /**
             * The portion of map view (ascending view)
             * limited by the min key (head) value and
             * max key (tail) value.
             */
            class BoundedMap;
        };

        template <class K, class V>
        class TreeMap<K, V>::KeySet final : public NavigableSet<K> {
            NavigableMap& map;
            gbool reverseOrder;

        public:
            CORE_EXPLICIT KeySet(NavigableMap const& map, gbool reverseOrder) :
                map((NavigableMap&) map),
                reverseOrder(reverseOrder) {}

            gint size() const override {
                try { return map.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Comparator<K>& comparator() const override {
                try { return !reverseOrder ? map.comparator() : map.comparator().reversed(); } catch (Throwable const&
                    ex) { ex.throws($ftrace()); }
            }

            const K& lower(const K& e) const override {
                try {
                    return !reverseOrder ? map.lowerKey(e) : map.higherKey(e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& lower(const K& e) override {
                try {
                    return !reverseOrder ? map.lowerKey(e) : map.higherKey(e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& floor(const K& e) const override {
                try {
                    return !reverseOrder ? map.floorKey(e) : map.ceilingKey(e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& floor(const K& e) override {
                try {
                    return !reverseOrder ? map.floorKey(e) : map.ceilingKey(e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& ceiling(const K& e) const override {
                try {
                    return reverseOrder ? map.floorKey(e) : map.ceilingKey(e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& ceiling(const K& e) override {
                try {
                    return reverseOrder ? map.floorKey(e) : map.ceilingKey(e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& higher(const K& e) const override {
                try {
                    return reverseOrder ? map.lowerKey(e) : map.higherKey(e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& higher(const K& e) override {
                try {
                    return reverseOrder ? map.lowerKey(e) : map.higherKey(e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<K>& iterator() override {
                try {
                    if (Class<TreeMap>::hasInstance(map))
                        return CORE_XCAST(TreeMap, map).keyIterator(reverseOrder);

                    return CORE_XCAST(SubMap, map).keyIterator(reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& iterator() const override {
                try {
                    if (Class<TreeMap>::hasInstance(map))
                        return CORE_XCAST(TreeMap const, map).keyIterator(reverseOrder);

                    return CORE_XCAST(SubMap const, map).keyIterator(reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return map.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const K& val) const override {
                try { return map.containsKey(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const K& val) override { UnsupportedOperationException().throws($ftrace()); }

            void addFirst(const K& e) override { UnsupportedOperationException().throws($ftrace()); }

            void addLast(const K& e) override { UnsupportedOperationException().throws($ftrace()); }

            gbool remove(const K& val) override {
                try {
                    gint size = map.size();
                    map.remove(val);
                    return map.size() < size;
                } catch (Throwable const&) { return false; }
            }

            gbool addAll(const Collection<K>& c) override { UnsupportedOperationException().throws($ftrace()); }

            void clear() override { try { map.clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); } }

            K& firstElement() override {
                try {
                    return !reverseOrder ? map.firstKey() : map.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& firstElement() const override {
                try {
                    return !reverseOrder ? map.firstKey() : map.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& lastElement() override {
                try {
                    return reverseOrder ? map.firstKey() : map.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& lastElement() const override {
                try {
                    return reverseOrder ? map.firstKey() : map.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& first() const override {
                try {
                    return !reverseOrder ? map.firstKey() : map.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& first() override {
                try {
                    return !reverseOrder ? map.firstKey() : map.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& last() const override {
                try {
                    return reverseOrder ? map.firstKey() : map.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& last() override {
                try {
                    return reverseOrder ? map.firstKey() : map.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& removeFirst() override {
                Object& obj = !reverseOrder ? map.pollFirstEntry() : map.pollLastEntry();
                if (obj == null)
                    NoSuchElementException().throws($ftrace());
                return CORE_XCAST(Entry, obj).getKey();
            }

            K& removeLast() override {
                Object& obj = reverseOrder ? map.pollFirstEntry() : map.pollLastEntry();
                if (obj == null)
                    NoSuchElementException().throws($ftrace());
                return CORE_XCAST(Entry, obj).getKey();
            }

            Iterator<K>& descendingIterator() override {
                try {
                    return (!reverseOrder ? map.descendingKeySet() : map.navigableKeySet()).iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& descendingIterator() const override {
                try {
                    if (!reverseOrder)
                        return ((Set<K> const&) map.descendingKeySet()).iterator();

                    return ((Set<K> const&) map.navigableKeySet()).iterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<K>& subSet(const K& fromElement, gbool fromInclusive,
                                          const K& toElement, gbool toInclusive) const override {
                try {
                    if (!reverseOrder)
                        return map.subMap(fromElement, fromInclusive, toElement, toInclusive).navigableKeySet();

                    return map.subMap(fromElement, fromInclusive, toElement, toInclusive).descendingKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<K>& subSet(const K& fromElement, gbool fromInclusive,
                                    const K& toElement, gbool toInclusive) override {
                try {
                    if (!reverseOrder)
                        return map.subMap(fromElement, fromInclusive, toElement, toInclusive).navigableKeySet();

                    return map.subMap(fromElement, fromInclusive, toElement, toInclusive).descendingKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<K>& headSet(const K& toElement, gbool inclusive) const override {
                try {
                    if (!reverseOrder)
                        return map.headMap(toElement, inclusive).navigableKeySet();

                    return map.headMap(toElement, inclusive).descendingKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<K>& headSet(const K& toElement, gbool inclusive) override {
                try {
                    if (!reverseOrder)
                        return map.headMap(toElement, inclusive).navigableKeySet();

                    return map.headMap(toElement, inclusive).descendingKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<K>& tailSet(const K& fromElement, gbool inclusive) const override {
                try {
                    if (!reverseOrder)
                        return map.tailMap(fromElement, inclusive).navigableKeySet();

                    return map.tailMap(fromElement, inclusive).descendingKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<K>& tailSet(const K& fromElement, gbool inclusive) override {
                try {
                    if (!reverseOrder)
                        return map.tailMap(fromElement, inclusive).navigableKeySet();

                    return map.tailMap(fromElement, inclusive).descendingKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<K>& reversed() const override {
                try {
                    return !reverseOrder ? map.descendingKeySet() : map.navigableKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<K>& reversed() override {
                try { return !reverseOrder ? map.descendingKeySet() : map.navigableKeySet(); } catch (Throwable const&
                    ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<K&>& action) override {
                if (Class<TreeMap>::hasInstance(map)) {
                    TreeMap& tree = CORE_XCAST(TreeMap, map);
                    gint modCount = tree.modCount;

                    if (!reverseOrder) {
                        for (NODE node = tree.firstNode(); node != null; node = successor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            K& key = keyOf(node);
                            action.accept(key);
                        }
                    } else {
                        for (NODE node = tree.lastNode(); node != null; node = predecessor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            K& key = keyOf(node);
                            action.accept(key);
                        }
                    }
                } else {
                    SubMap& sub = CORE_XCAST(SubMap, map);

                    if (!reverseOrder) {
                        for (NODE node = sub.lowestNode(), end = successor(sub.highestNode()); node != end; node =
                             successor(node)) {
                            sub.check();

                            K& key = keyOf(node);
                            action.accept(key);
                        }
                    } else {
                        for (NODE node = sub.highestNode(), end = predecessor(sub.lowestNode()); node != end; node =
                             predecessor(node)) {
                            sub.check();

                            K& key = keyOf(node);
                            action.accept(key);
                        }
                    }
                }
            }

            void forEach(const function::Consumer<K>& action) const override {
                if (Class<TreeMap>::hasInstance(map)) {
                    TreeMap& tree = CORE_XCAST(TreeMap, map);
                    gint modCount = tree.modCount;

                    if (!reverseOrder) {
                        for (NODE node = tree.firstNode(); node != null; node = successor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            K& key = keyOf(node);
                            action.accept(key);
                        }
                    } else {
                        for (NODE node = tree.lastNode(); node != null; node = predecessor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            K& key = keyOf(node);
                            action.accept(key);
                        }
                    }
                } else {
                    SubMap& sub = CORE_XCAST(SubMap, map);

                    if (!reverseOrder) {
                        for (NODE node = sub.lowestNode(), end = successor(sub.highestNode()); node != end; node =
                             successor(node)) {
                            sub.check();

                            K& key = keyOf(node);
                            action.accept(key);
                        }
                    } else {
                        for (NODE node = sub.highestNode(), end = predecessor(sub.lowestNode()); node != end; node =
                             predecessor(node)) {
                            sub.check();

                            K& key = keyOf(node);
                            action.accept(key);
                        }
                    }
                }
            }

            ~KeySet() override { detachKeys(map, reverseOrder); }
        };

        template <class K, class V>
        class TreeMap<K, V>::Values final : public SequencedCollection<V> {
            NavigableMap& map;
            gbool reverseOrder;

        public:
            Values(NavigableMap const& map, gbool reverseOrder):
                map((NavigableMap&) map),
                reverseOrder(reverseOrder) {}

            gint size() const override {
                try { return map.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return map.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const V& val) const override {
                try { return map.containsValue(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const V& val) override { UnsupportedOperationException().throws($ftrace()); }

            gbool addAll(const Collection<V>& c) override { UnsupportedOperationException().throws($ftrace()); }

            void clear() override { try { map.clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); } }

            void addFirst(const V& val) override { UnsupportedOperationException().throws($ftrace()); }

            void addLast(const V& val) override { UnsupportedOperationException().throws($ftrace()); }

            V& firstElement() override {
                try { return (!reverseOrder ? map.firstEntry() : map.lastEntry()).getValue(); } catch (Throwable const&
                    ex) { ex.throws($ftrace()); }
            }

            const V& firstElement() const override {
                try { return (!reverseOrder ? map.firstEntry() : map.lastEntry()).getValue(); } catch (Throwable const&
                    ex) { ex.throws($ftrace()); }
            }

            V& lastElement() override {
                try { return (reverseOrder ? map.firstEntry() : map.lastEntry()).getValue(); } catch (Throwable const&
                    ex) { ex.throws($ftrace()); }
            }

            const V& lastElement() const override {
                try { return (reverseOrder ? map.firstEntry() : map.lastEntry()).getValue(); } catch (Throwable const&
                    ex) { ex.throws($ftrace()); }
            }

            V& removeFirst() override {
                Object& obj = !reverseOrder ? map.pollFirstEntry() : map.pollLastEntry();
                if (obj == null)
                    NoSuchElementException().throws($ftrace());
                return CORE_XCAST(Entry, obj).getValue();
            }

            V& removeLast() override {
                Object& obj = reverseOrder ? map.pollFirstEntry() : map.pollLastEntry();
                if (obj == null)
                    NoSuchElementException().throws($ftrace());
                return CORE_XCAST(Entry, obj).getValue();
            }

            Iterator<V>& iterator() override {
                try {
                    if (Class<TreeMap>::hasInstance(map))
                        return CORE_XCAST(TreeMap, map).valueIterator(reverseOrder);

                    return CORE_XCAST(SubMap, map).valueIterator(reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& iterator() const override {
                try {
                    if (Class<TreeMap>::hasInstance(map))
                        return CORE_XCAST(TreeMap const, map).valueIterator(reverseOrder);

                    return CORE_XCAST(SubMap const, map).valueIterator(reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedCollection<V>& reversed() const override {
                try {
                    return (!reverseOrder ? map.reversed() : map).sequencedValues();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<V>& reversed() override {
                try {
                    return (!reverseOrder ? map.reversed() : map).sequencedValues();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<V&>& action) override {
                if (Class<TreeMap>::hasInstance(map)) {
                    TreeMap& tree = CORE_XCAST(TreeMap, map);
                    gint modCount = tree.modCount;
                    if (!reverseOrder) {
                        for (NODE node = tree.firstNode(); node != 0; node = successor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            V& value = valueOf(node);
                            action.accept(value);
                        }
                    } else {
                        for (NODE node = tree.lastNode(); node != 0; node = predecessor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            V& value = valueOf(node);
                            action.accept(value);
                        }
                    }
                } else {
                    SubMap& sub = CORE_XCAST(SubMap, map);
                    if (!reverseOrder) {
                        for (NODE node = sub.lowestNode(), end = successor(sub.highestNode()); node != end; node =
                             successor(node)) {
                            sub.check();

                            V& value = valueOf(node);
                            action.accept(value);
                        }
                    } else {
                        for (NODE node = sub.lowestNode(), end = predecessor(sub.highestNode()); node != end; node =
                             predecessor(node)) {
                            sub.check();

                            V& value = valueOf(node);
                            action.accept(value);
                        }
                    }
                }
            }

            void forEach(const function::Consumer<V>& action) const override {
                if (Class<TreeMap>::hasInstance(map)) {
                    TreeMap& tree = CORE_XCAST(TreeMap, map);
                    gint modCount = tree.modCount;
                    if (!reverseOrder) {
                        for (NODE node = tree.firstNode(); node != 0; node = successor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            V& value = valueOf(node);
                            action.accept(value);
                        }
                    } else {
                        for (NODE node = tree.lastNode(); node != 0; node = predecessor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            V& value = valueOf(node);
                            action.accept(value);
                        }
                    }
                } else {
                    SubMap& sub = CORE_XCAST(SubMap, map);
                    if (!reverseOrder) {
                        for (NODE node = sub.lowestNode(), end = successor(sub.highestNode()); node != end; node =
                             successor(node)) {
                            sub.check();

                            V& value = valueOf(node);
                            action.accept(value);
                        }
                    } else {
                        for (NODE node = sub.lowestNode(), end = predecessor(sub.highestNode()); node != end; node =
                             predecessor(node)) {
                            sub.check();

                            V& value = valueOf(node);
                            action.accept(value);
                        }
                    }
                }
            }

            ~Values() override { detachVals(map, reverseOrder); }
        };

        template <class K, class V>
        class TreeMap<K, V>::EntrySet final : public SequencedSet<Entry> {
            NavigableMap& map;
            gbool reverseOrder;

        public:
            CORE_EXPLICIT EntrySet(NavigableMap const& map, gbool reverseOrder) :
                map((NavigableMap&) map),
                reverseOrder(reverseOrder) {}

            gint size() const override {
                try { return map.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return map.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<Entry>& iterator() override {
                try {
                    if (Class<TreeMap>::hasInstance(map))
                        return CORE_XCAST(TreeMap, map).entryIterator(reverseOrder);

                    return CORE_XCAST(SubMap, map).entryIterator(reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry>& iterator() const override {
                try {
                    if (Class<TreeMap>::hasInstance(map))
                        return CORE_XCAST(TreeMap const, map).entryIterator(reverseOrder);

                    return CORE_XCAST(SubMap const, map).entryIterator(reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override { try { map.clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); } }

            Entry& firstElement() override {
                try {
                    return !reverseOrder ? map.firstEntry() : map.lastEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& firstElement() const override {
                try {
                    return !reverseOrder ? map.firstEntry() : map.lastEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& lastElement() override {
                try {
                    return reverseOrder ? map.firstEntry() : map.lastEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& lastElement() const override {
                try {
                    return reverseOrder ? map.firstEntry() : map.lastEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void addFirst(const Entry& e) override { UnsupportedOperationException().throws($ftrace()); }

            void addLast(const Entry& e) override { UnsupportedOperationException().throws($ftrace()); }

            Entry& removeFirst() override {
                Object& obj = !reverseOrder ? map.pollFirstEntry() : map.pollLastEntry();
                if (obj == null)
                    NoSuchElementException().throws($ftrace());
                return CORE_XCAST(Entry, obj);
            }

            Entry& removeLast() override {
                Object& obj = reverseOrder ? map.pollFirstEntry() : map.pollLastEntry();
                if (obj == null)
                    NoSuchElementException().throws($ftrace());
                return CORE_XCAST(Entry, obj);
            }

            const SequencedSet<Entry>& reversed() const override {
                try {
                    return (!reverseOrder ? map.reversed() : map).sequencedEntrySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<Entry>& reversed() override {
                try {
                    return (!reverseOrder ? map.reversed() : map).sequencedEntrySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const Entry& val) const override {
                try {
                    if (Class<TreeMap>::hasInstance(map)) {
                        TreeMap<K, V> const& tree = CORE_XCAST(TreeMap const, map);
                        NODE node = tree.exactNode(val.getKey());
                        if (!node || val.getValue() != valueOf(node))
                            return false;
                        return true;
                    }

                    SubMap const& sub = CORE_XCAST(SubMap const, map);
                    TreeMap<K, V> const& tree = sub.tree();
                    if (!sub.inRange(val.getKey()))
                        return false;
                    NODE node = tree.exactNode(val.getKey());
                    if (!node || val.getValue() != valueOf(node))
                        return false;
                    return true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<Entry&>& action) override {
                if (Class<TreeMap>::hasInstance(map)) {
                    TreeMap& tree = CORE_XCAST(TreeMap, map);
                    gint modCount = tree.modCount;
                    if (!reverseOrder) {
                        for (NODE node = tree.firstNode(); node != 0; node = successor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            action.accept(*node);
                        }
                    } else {
                        for (NODE node = tree.lastNode(); node != 0; node = predecessor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            action.accept(*node);
                        }
                    }
                } else {
                    SubMap& sub = CORE_XCAST(SubMap, map);
                    if (!reverseOrder) {
                        for (NODE node = sub.lowestNode(), end = successor(sub.highestNode()); node != end; node =
                             successor(node)) {
                            sub.check();

                            action.accept(*node);
                        }
                    } else {
                        for (NODE node = sub.lowestNode(), end = predecessor(sub.highestNode()); node != end; node =
                             predecessor(node)) {
                            sub.check();

                            action.accept(*node);
                        }
                    }
                }
            }

            void forEach(const function::Consumer<Entry>& action) const override {
                if (Class<TreeMap>::hasInstance(map)) {
                    TreeMap& tree = CORE_XCAST(TreeMap, map);
                    gint modCount = tree.modCount;
                    if (!reverseOrder) {
                        for (NODE node = tree.firstNode(); node != 0; node = successor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            action.accept(*node);
                        }
                    } else {
                        for (NODE node = tree.lastNode(); node != 0; node = predecessor(node)) {
                            if (modCount != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());

                            action.accept(*node);
                        }
                    }
                } else {
                    SubMap& sub = CORE_XCAST(SubMap, map);
                    if (!reverseOrder) {
                        for (NODE node = sub.lowestNode(), end = successor(sub.highestNode()); node != end; node =
                             successor(node)) {
                            sub.check();

                            action.accept(*node);
                        }
                    } else {
                        for (NODE node = sub.lowestNode(), end = predecessor(sub.highestNode()); node != end; node =
                             predecessor(node)) {
                            sub.check();

                            action.accept(*node);
                        }
                    }
                }
            }

            ~EntrySet() override { detachEntries(map, reverseOrder); }
        };

        template <class K, class V>
        class TreeMap<K, V>::Reverse final : public NavigableMap {
            NavigableMap& map;

        public:
            CORE_EXPLICIT Reverse(NavigableMap const& map):
                map((NavigableMap&) map) {}

            gint size() const override {
                try { return map.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return map.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Comparator<K>& comparator() const override {
                try { return map.comparator().reversed(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Set<Entry>& entrySet() override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Set<Entry>& entrySet() const override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& lowerEntry(const K& key) const override {
                try { return map.higherEntry(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& lowerEntry(const K& key) override {
                try { return map.higherEntry(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& lowerKey(const K& key) const override {
                try { return map.higherKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& lowerKey(const K& key) override {
                try { return map.higherKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& floorEntry(const K& key) const override {
                try { return map.ceilingEntry(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& floorEntry(const K& key) override {
                try { return map.ceilingEntry(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& floorKey(const K& key) const override {
                try { return map.ceilingKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& floorKey(const K& key) override {
                try { return map.ceilingKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& ceilingEntry(const K& key) const override {
                try { return map.floorEntry(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& ceilingEntry(const K& key) override {
                try { return map.floorEntry(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& ceilingKey(const K& key) const override {
                try { return map.floorKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& ceilingKey(const K& key) override {
                try { return map.floorKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& higherEntry(const K& key) const override {
                try { return map.lowerEntry(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& higherEntry(const K& key) override {
                try { return map.lowerEntry(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& higherKey(const K& key) const override {
                try { return map.lowerKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& higherKey(const K& key) override {
                try { return map.lowerKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& firstEntry() const override {
                try { return map.lastEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& firstEntry() override {
                try { return map.lastEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& firstKey() const override {
                try { return map.lastKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& firstKey() override {
                try { return map.lastKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& lastEntry() const override {
                try { return map.firstEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& lastEntry() override {
                try { return map.firstEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& lastKey() const override {
                try { return map.firstKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& lastKey() override {
                try { return map.firstKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& descendingMap() const override { return map; }

            NavigableMap& descendingMap() override { return map; }

            const NavigableSet<K>& navigableKeySet() const override {
                NavigableKeys view = CORE_DCAST(NavigableKeys, keys);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<KeySet>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    keys = view;
                }
                return *view;
            }

            NavigableSet<K>& navigableKeySet() override {
                NavigableKeys view = CORE_DCAST(NavigableKeys, keys);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<KeySet>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    keys = view;
                }
                return *view;
            }

            const NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                       const K& toKey, gbool toInclusive) const override {
                try { return map.subMap(toKey, toInclusive, fromKey, fromInclusive).reversed(); } catch (Throwable const
                    & ex) { ex.throws($ftrace()); }
            }

            NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                 const K& toKey, gbool toInclusive) override {
                try { return map.subMap(toKey, toInclusive, fromKey, fromInclusive).reversed(); } catch (Throwable const
                    & ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& headMap(const K& toKey, gbool inclusive) const override {
                try {
                    return map.tailMap(toKey, inclusive).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableMap& headMap(const K& toKey, gbool inclusive) override {
                try {
                    return map.tailMap(toKey, inclusive).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& tailMap(const K& fromKey, gbool inclusive) const override {
                try {
                    return map.headMap(fromKey, inclusive).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableMap& tailMap(const K& fromKey, gbool inclusive) override {
                try {
                    return map.headMap(fromKey, inclusive).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& pollFirstEntry() override {
                try { return map.pollLastEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& pollLastEntry() override {
                try { return map.pollFirstEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Set<K>& keySet() override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Set<K>& keySet() const override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<K>& descendingKeySet() const override {
                try { return map.navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<K>& descendingKeySet() override {
                try { return map.navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override { try { return map.clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); } }

            gbool containsKey(const K& key) const override {
                try { return map.containsKey(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsValue(const V& value) const override {
                try { return map.containsValue(value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& get(const K& key) override {
                try { return map.get(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& get(const K& key) const override {
                try { return map.get(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& put(const K& key, const V& value) override {
                try { return map.put(key, value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& remove(const K& key) override {
                try { return map.remove(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void putAll(const Map& m) override {
                try { map.putAll(m); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& getOrDefault(const K& key, const V& defaultValue) override {
                try {
                    return map.getOrDefault(key, defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& getOrDefault(const K& key, const V& defaultValue) const override {
                try {
                    return map.getOrDefault(key, defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& getOrNull(const K& key) override {
                try { return map.getOrNull(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Object& getOrNull(const K& key) const override {
                try { return map.getOrNull(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& putIfAbsent(const K& key, const V& value) override {
                try { return map.putIfAbsent(key, value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const K& key, const V& value) override {
                try { return map.remove(key, value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool replace(const K& key, const V& oldValue, const V& newValue) override {
                try {
                    return map.replace(key, oldValue, newValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& replace(const K& key, const V& value) override {
                try { return map.replace(key, value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<K>& sequencedKeySet() override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedSet<K>& sequencedKeySet() const override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<V>& sequencedValues() override {
                SequencedVals view = CORE_DCAST(SequencedVals, vals);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<Values>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    vals = view;
                }
                return *view;
            }

            const SequencedCollection<V>& sequencedValues() const override {
                SequencedVals view = CORE_DCAST(SequencedVals, vals);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<Values>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    vals = view;
                }
                return *view;
            }

            SequencedSet<Entry>& sequencedEntrySet() override {
                SequencedEntries view = CORE_DCAST(SequencedEntries, entries);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<EntrySet>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    entries = view;
                }
                return *view;
            }

            const SequencedSet<Entry>& sequencedEntrySet() const override {
                SequencedEntries view = CORE_DCAST(SequencedEntries, entries);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<EntrySet>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    entries = view;
                }
                return *view;
            }

            void forEach(const function::BiConsumer<K&, V&>& action) override {
                if (Class<TreeMap>::hasInstance(map)) {
                    TreeMap& tree = CORE_XCAST(TreeMap, map);
                    gint mc = tree.modCount;
                    for (NODE x = tree.lastNode(); x != 0; x = predecessor(x)) {
                        if (mc != tree.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            action.accept(x->getKey(), x->getValue());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                } else {
                    SubMap& sub = CORE_XCAST(SubMap, map);
                    for (NODE x = sub.highestNode(), y = predecessor(sub.lowestNode()); x != y; x = predecessor(x)) {
                        sub.check();
                        action.accept(x->getKey(), x->getValue());
                    }
                }
            }

            void forEach(const function::BiConsumer<K, V>& action) const override {
                if (Class<TreeMap>::hasInstance(map)) {
                    TreeMap& tree = CORE_XCAST(TreeMap, map);
                    gint mc = tree.modCount;
                    for (NODE x = tree.lastNode(); x != 0; x = predecessor(x)) {
                        if (mc != tree.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            action.accept(x->getKey(), x->getValue());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                } else {
                    SubMap& sub = CORE_XCAST(SubMap, map);
                    for (NODE x = sub.highestNode(), y = predecessor(sub.lowestNode()); x != y; x = predecessor(x)) {
                        sub.check();
                        action.accept(x->getKey(), x->getValue());
                    }
                }
            }

            void replaceAll(const function::BiFunction<K&, V&, V>& function) override {
                try {
                    if (Class<TreeMap>::hasInstance(map)) {
                        TreeMap& tree = CORE_XCAST(TreeMap, map);
                        gint mc = tree.modCount;
                        for (NODE x = tree.lastNode(); x != 0; x = predecessor(x)) {
                            if (mc != tree.modCount)
                                ConcurrentModificationException().throws($ftrace());
                            x->setValue(function.apply(x->getKey(), x->getValue()));
                        }
                    } else {
                        SubMap& sub = CORE_XCAST(SubMap, map);
                        for (NODE x = sub.highestNode(), y = predecessor(sub.lowestNode());
                             x != y; x = predecessor(x)) {
                            sub.check();
                            x->setValue(function.apply(x->getKey(), x->getValue()));
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Collection<V>& values() override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Collection<V>& values() const override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& reversed() const override { return map; }

            NavigableMap& reversed() override { return map; }

            ~Reverse() override {
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

                detachReverse(map);
            }

        private:
            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);
        };

        template <class K, class V>
        class TreeMap<K, V>::SubMap : public NavigableMap {
            CORE_ALIAS(SubView, typename Class<SubMap>::Pointer);
            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_AS_FRIEND(KeySet); // For SubMap::keyIterator
            CORE_ADD_AS_FRIEND(Values); // For SubMap::valueIterator
            CORE_ADD_AS_FRIEND(EntrySet); // For SubMap::entryIterator
            CORE_ADD_AS_FRIEND(BoundedMap); // For SubMap::update
            CORE_ADD_AS_FRIEND(Reverse); // For Map::forEach

        public:
            ~SubMap() override {
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

                if (reverse != null) {
                    UNSAFE::deleteRegInstance(*reverse);
                    reverse = null;
                }
            }

            const Comparator<K>& comparator() const override { return tree().comparator(); }

            const Entry& lowerEntry(const K& key) const override {
                try {
                    NODE node = tooHigh(key) ? highestNode() : tree().lowerNode(key);
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            Entry& lowerEntry(const K& key) override {
                try {
                    NODE node = tooHigh(key) ? highestNode() : tree().lowerNode(key);
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const K& lowerKey(const K& key) const override {
                try {
                    NODE node = tooHigh(key) ? highestNode() : tree().lowerNode(key);
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            K& lowerKey(const K& key) override {
                try {
                    NODE node = tooHigh(key) ? highestNode() : tree().lowerNode(key);
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const Entry& floorEntry(const K& key) const override {
                try {
                    NODE node = tooHigh(key) ? highestNode() : tree().floorNode(key);
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            Entry& floorEntry(const K& key) override {
                try {
                    NODE node = tooHigh(key) ? highestNode() : tree().floorNode(key);
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const K& floorKey(const K& key) const override {
                try {
                    NODE node = tooHigh(key) ? highestNode() : tree().floorNode(key);
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            K& floorKey(const K& key) override {
                try {
                    NODE node = tooHigh(key) ? highestNode() : tree().floorNode(key);
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const Entry& ceilingEntry(const K& key) const override {
                try {
                    NODE node = tooLow(key) ? lowestNode() : tree().ceilingNode(key);
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            Entry& ceilingEntry(const K& key) override {
                try {
                    NODE node = tooLow(key) ? lowestNode() : tree().ceilingNode(key);
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const K& ceilingKey(const K& key) const override {
                try {
                    NODE node = tooLow(key) ? lowestNode() : tree().ceilingNode(key);
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            K& ceilingKey(const K& key) override {
                try {
                    NODE node = tooLow(key) ? lowestNode() : tree().ceilingNode(key);
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const Entry& higherEntry(const K& key) const override {
                try {
                    NODE node = tooLow(key) ? lowestNode() : tree().higherNode(key);
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            Entry& higherEntry(const K& key) override {
                try {
                    NODE node = tooLow(key) ? lowestNode() : tree().higherNode(key);
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const K& higherKey(const K& key) const override {
                try {
                    NODE node = tooLow(key) ? lowestNode() : tree().higherNode(key);
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            K& higherKey(const K& key) override {
                try {
                    NODE node = tooLow(key) ? lowestNode() : tree().higherNode(key);
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const Entry& firstEntry() const override {
                try {
                    NODE node = lowestNode();
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            Entry& firstEntry() override {
                try {
                    NODE node = lowestNode();
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const Entry& lastEntry() const override {
                try {
                    NODE node = highestNode();
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            Entry& lastEntry() override {
                try {
                    NODE node = highestNode();
                    if (node == null)
                        return *node;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const NavigableMap& descendingMap() const override {
                try {
                    NavigableView rev = CORE_DCAST(NavigableView, reverse);
                    if (!rev) {
                        rev = &UNSAFE::newInstance<Reverse>(CORE_CAST(SubMap &, *this));
                        reverse = rev;
                    }

                    return *rev;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableMap& descendingMap() override {
                try {
                    NavigableView rev = CORE_DCAST(NavigableView, reverse);
                    if (!rev) {
                        rev = &UNSAFE::newInstance<Reverse>(*this);
                        reverse = rev;
                    }

                    return *rev;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<K>& navigableKeySet() const override {
                try {
                    NavigableKeys navKeys = CORE_DCAST(NavigableKeys, keys);
                    if (!navKeys) {
                        navKeys = &UNSAFE::newInstance<KeySet>((SubMap&) *this, false);
                        keys = navKeys;
                    }

                    return *navKeys;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<K>& navigableKeySet() override {
                try {
                    NavigableKeys navKeys = CORE_DCAST(NavigableKeys, keys);
                    if (!navKeys) {
                        navKeys = &UNSAFE::newInstance<KeySet>(*this, false);
                        keys = navKeys;
                    }

                    return *navKeys;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<K>& sequencedKeySet() override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedSet<K>& sequencedKeySet() const override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<V>& sequencedValues() override {
                try {
                    SequencedVals seqVals = CORE_DCAST(SequencedVals, vals);
                    if (!seqVals) {
                        seqVals = &UNSAFE::newInstance<Values>(*this, false);
                        vals = seqVals;
                    }
                    return *seqVals;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedCollection<V>& sequencedValues() const override {
                try {
                    SequencedVals seqVals = CORE_DCAST(SequencedVals, vals);
                    if (!seqVals) {
                        seqVals = &UNSAFE::newInstance<Values>((SubMap&) *this, false);
                        vals = seqVals;
                    }
                    return *seqVals;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<Entry>& sequencedEntrySet() override {
                try {
                    if (!entries)
                        entries = &UNSAFE::newInstance<EntrySet>(*this, false);
                    return *CORE_DCAST(SequencedEntries, entries);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedSet<Entry>& sequencedEntrySet() const override {
                try {
                    if (!entries)
                        entries = &UNSAFE::newInstance<EntrySet>((SubMap&) *this, false);
                    return *CORE_DCAST(SequencedEntries, entries);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& firstKey() const override {
                try {
                    NODE node = lowestNode();
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            K& firstKey() override {
                try {
                    NODE node = lowestNode();
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            const K& lastKey() const override {
                try {
                    NODE node = highestNode();
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            K& lastKey() override {
                try {
                    NODE node = highestNode();
                    if (node == null)
                        return keyOf(node);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            Set<K>& keySet() override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Set<K>& keySet() const override {
                try { return navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Collection<V>& values() override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Collection<V>& values() const override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Set<Entry>& entrySet() override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Set<Entry>& entrySet() const override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableSet<K>& descendingKeySet() const override {
                try { return reversed().navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableSet<K>& descendingKeySet() override {
                try { return reversed().navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsKey(const K& key) const override {
                try {
                    if (inRange(key))
                        return tree().containsKey(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return false;
            }

            V& get(const K& key) override {
                try {
                    if (inRange(key))
                        return tree().get(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                MissingKeyException(key).throws($ftrace());
            }

            const V& get(const K& key) const override {
                try {
                    if (inRange(key))
                        return tree().get(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                MissingKeyException(key).throws($ftrace());
            }

            Object& put(const K& key, const V& value) override {
                try {
                    if (inRange(key))
                        return tree().put(key, value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key out of range").throws($ftrace());
            }

            V& remove(const K& key) override {
                try {
                    if (inRange(key))
                        return tree().remove(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                MissingKeyException(key).throws($ftrace());
            }

            V& getOrDefault(const K& key, const V& defaultValue) override {
                try {
                    if (inRange(key))
                        return tree().getOrDefault(key, defaultValue);

                    return UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& getOrDefault(const K& key, const V& defaultValue) const override {
                try {
                    if (inRange(key))
                        return tree().getOrDefault(key, defaultValue);

                    return UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& getOrNull(const K& key) override {
                try {
                    if (inRange(key))
                        return tree().getOrNull(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return null;
            }

            const Object& getOrNull(const K& key) const override {
                try {
                    if (inRange(key))
                        return tree().getOrNull(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return null;
            }

            const NavigableMap& reversed() const override {
                try { return descendingMap(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NavigableMap& reversed() override {
                try { return descendingMap(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::BiConsumer<K&, V&>& action) override {
                try {
                    check();
                    for (NODE x = lowestNode(), end = successor(highestNode()); x != end; x = successor(x)) {
                        action.accept(x->getKey(), x->getValue());
                        check();
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::BiConsumer<K, V>& action) const override {
                try {
                    check();
                    for (NODE x = lowestNode(), end = successor(highestNode()); x != end; x = successor(x)) {
                        action.accept(x->getKey(), x->getValue());
                        check();
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void replaceAll(const function::BiFunction<K&, V&, V>& function) override {
                check();
                for (NODE x = lowestNode(), end = successor(highestNode()); x != end; x = successor(x)) {
                    x->setValue(function.apply(x->getKey(), x->getValue()));
                    check();
                }
            }

            Object& putIfAbsent(const K& key, const V& value) override {
                try {
                    if (inRange(key))
                        return tree().putIfAbsent(key, value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key out of range").throws($ftrace());
            }

            gbool remove(const K& key, const V& value) override {
                try {
                    if (inRange(key))
                        return tree().remove(key, value);

                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool replace(const K& key, const V& oldValue, const V& newValue) override {
                try {
                    if (inRange(key))
                        return tree().replace(key, oldValue, newValue);

                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& replace(const K& key, const V& value) override {
                try {
                    if (inRange(key))
                        return tree().replace(key, value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key out of range").throws($ftrace());
            }

            Object& pollFirstEntry() override {
                try {
                    NODE node = lowestNode();
                    if (node == null) {
                        tree().deleteNode(node);
                        return *node;
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            Object& pollLastEntry() override {
                try {
                    NODE node = highestNode();
                    if (node == null) {
                        tree().deleteNode(node);
                        return *node;
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                NoSuchElementException().throws($ftrace());
            }

            virtual gbool tooLow(K const& key) const = 0;

            virtual gbool tooHigh(K const& key) const = 0;

            virtual gbool inRange(K const& key) const { return !tooLow(key) && !tooHigh(key); }

            virtual NODE highestNode() const = 0;

            virtual NODE lowestNode() const = 0;

            virtual void check() const = 0;

            virtual void update(gint len) = 0;

            virtual TreeMap& tree() const = 0;

            virtual SubMap& parent() const = 0;

            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);
            CORE_IMPORT_FIELD_OR_METHOD(SequencedMap, reverse);

            virtual Iterator<K>& keyIterator(gbool reverseOrder) = 0;

            virtual Iterator2<K>& keyIterator(gbool reverseOrder) const = 0;

            virtual Iterator<V>& valueIterator(gbool reverseOrder) = 0;

            virtual Iterator2<V>& valueIterator(gbool reverseOrder) const = 0;

            virtual Iterator<Entry>& entryIterator(gbool reverseOrder) = 0;

            virtual Iterator2<Entry>& entryIterator(gbool reverseOrder) const = 0;
        };

        template <class K, class V>
        class TreeMap<K, V>::HeadMap final : public SubMap {
            TreeMap<K, V>& map;
            HeadMap& sub;
            K const& tail;
            gbool inclusive;

            gint mutable estSize = 0;
            gint mutable lastEst = 0;
            gint modCount = 0;

        public:
            HeadMap(TreeMap<K, V> const& map, K const& tail, gbool inclusive) :
                map((TreeMap&) map),
                sub(*this),
                tail(tail),
                inclusive(inclusive) { modCount = map.modCount; }

            HeadMap(TreeMap<K, V> const& map, HeadMap const& sub, K const& tail, gbool inclusive) :
                map((TreeMap&) map),
                sub((HeadMap&) sub),
                tail(tail),
                inclusive(inclusive) { modCount = map.modCount; }

            gint size() const override {
                try {
                    check();
                    if (lastEst != modCount) {
                        estSize = 0;
                        // We'll do iteration of all entries in reverse sens (to minimize complexity)
                        for (NODE x = highestNode(); x != null; x = predecessor(x))
                            estSize++;

                        lastEst = modCount;
                    }
                    return estSize;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsValue(const V& value) const override {
                try {
                    check();
                    // We'll iterate all entries in reverse sens
                    for (NODE x = highestNode(); x != null; x = predecessor(x))
                        if (value == x->getValue())
                            return true;
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                       const K& toKey, gbool toInclusive) const override {
                gbool f = false, t = false;
                try {
                    check();
                    if (((f = inRange(fromKey, fromInclusive))) &&
                        ((t = inRange(toKey, toInclusive))) && map.compare(fromKey, toKey) >= 0)
                        return UNSAFE::newInstance<BoundedMap>(map, CORE_CAST(HeadMap &, *this),
                                                               fromKey, fromInclusive,
                                                               toKey, toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (!f)
                    IllegalArgumentException("Start key too high").throws($ftrace());
                if (!t)
                    IllegalArgumentException("End key too high").throws($ftrace());
                IllegalArgumentException("Invalid keys range").throws($ftrace());
            }

            NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                 const K& toKey, gbool toInclusive) override {
                gbool f = false, t = false;
                try {
                    check();
                    if (((f = inRange(fromKey, fromInclusive))) &&
                        ((t = inRange(toKey, toInclusive))) && map.compare(fromKey, toKey) >= 0)
                        return UNSAFE::newInstance<BoundedMap>(map, *this,
                                                               UNSAFE::copyInstance(fromKey), fromInclusive,
                                                               toKey, toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (!f)
                    IllegalArgumentException("Start key too high").throws($ftrace());
                if (!t)
                    IllegalArgumentException("End key too high").throws($ftrace());
                IllegalArgumentException("Invalid keys range").throws($ftrace());
            }

            const NavigableMap& headMap(const K& toKey, gbool inclusive) const override {
                try {
                    check();
                    if (inRange(toKey, inclusive))
                        return UNSAFE::newInstance<HeadMap>(map, CORE_CAST(HeadMap &, *this),
                                                            UNSAFE::copyInstance(toKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key too high").throws($ftrace());
            }

            NavigableMap& headMap(const K& toKey, gbool inclusive) override {
                try {
                    check();
                    if (inRange(toKey, inclusive))
                        return UNSAFE::newInstance<HeadMap>(map, *this,
                                                            UNSAFE::copyInstance(toKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key too high").throws($ftrace());
            }

            const NavigableMap& tailMap(const K& fromKey, gbool inclusive) const override {
                try {
                    check();
                    if (inRange(fromKey, inclusive))
                        return UNSAFE::newInstance<BoundedMap>(map, CORE_CAST(HeadMap &, *this),
                                                               UNSAFE::copyInstance(fromKey), inclusive,
                                                               tail, HeadMap::inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key too high").throws($ftrace());
            }

            NavigableMap& tailMap(const K& fromKey, gbool inclusive) override {
                try {
                    check();
                    if (inRange(fromKey, inclusive))
                        return UNSAFE::newInstance<BoundedMap>(map, *this,
                                                               UNSAFE::copyInstance(fromKey), inclusive,
                                                               tail, HeadMap::inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key too high").throws($ftrace());
            }

        protected:
            gbool tooLow(const K& key) const override { return false; }

            gbool tooHigh(const K& key) const override {
                try {
                    gint result = map.compare(key, tail);
                    return inclusive ? result >= 0 : result > 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool closeRange(const K& key) const {
                try { return map.compare(key, tail) <= 0; } catch (Throwable const& ex) { ex.throws($ftrace()); }
            };

            gbool inRange(const K& key) const override {
                try { return !tooHigh(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool inRange(const K& key, gbool inclusive) const {
                try {
                    return inclusive ? inRange(key) : closeRange(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NODE highestNode() const override {
                try {
                    return inclusive ? map.floorNode(tail) : map.lowerNode(tail);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NODE lowestNode() const override { return map.firstNode(); }

            void check() const override {
                if (modCount != map.modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void update(gint len) override {
                CORE_ALIAS(HeadView, typename Class<HeadMap>::Pointer);
                HeadView view = this;
                do {
                    view->modCount = view->lastEst = map.modCount;
                    view->estSize += len;
                } while (view != this);
            }

            TreeMap<K, V>& tree() const override { return map; }

            SubMap& parent() const override { return sub; }

            Iterator<K>& keyIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<K> {
                    HeadMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(HeadMap& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : map.tree().firstNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    K& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getKey();
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.tree().deleteNode(x);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<K&>& action) override {
                        try {
                            if (reverseOrder)
                                for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                    action.accept(cursor->getKey());
                            else
                                for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                    action.accept(cursor->getKey());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = highestNode();
                    if (!reverseOrder) // End of Ascending iterator is exclusive
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& keyIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<K> {
                    HeadMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(HeadMap const& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : map.tree().firstNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    K const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getKey();
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<K>& action) override {
                        try {
                            if (reverseOrder)
                                for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                    action.accept(cursor->getKey());
                            else
                                for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                    action.accept(cursor->getKey());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = highestNode();
                    if (!reverseOrder) // End of Ascending iterator is exclusive
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<V>& valueIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<V> {
                    HeadMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(HeadMap& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : map.tree().firstNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    V& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getValue();
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.tree().deleteNode(x);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<V&>& action) override {
                        try {
                            if (reverseOrder)
                                for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                    action.accept(cursor->getValue());
                            else
                                for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                    action.accept(cursor->getValue());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = highestNode();
                    if (!reverseOrder) // End of Ascending iterator is exclusive
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& valueIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<V> {
                    HeadMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(HeadMap const& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : map.tree().firstNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    V const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getValue();
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<V>& action) override {
                        try {
                            if (reverseOrder)
                                for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                    action.accept(cursor->getValue());
                            else
                                for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                    action.accept(cursor->getValue());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = highestNode();
                    if (!reverseOrder) // End of Ascending iterator is exclusive
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<Entry>& entryIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<Entry> {
                    HeadMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(HeadMap& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : map.tree().firstNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    Entry& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return *last;
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.tree().deleteNode(x);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<Entry&>& action) override {
                        try {
                            if (reverseOrder)
                                for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                    action.accept(*cursor);
                            else
                                for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                    action.accept(*cursor);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = highestNode();
                    if (!reverseOrder) // End of Ascending iterator is exclusive
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry>& entryIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<Entry> {
                    HeadMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(HeadMap const& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : map.tree().firstNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    Entry const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return *last;
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<Entry>& action) override {
                        try {
                            if (reverseOrder)
                                for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                    action.accept(*cursor);
                            else
                                for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                    action.accept(*cursor);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = highestNode();
                    if (!reverseOrder) // End of Ascending iterator is exclusive
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class K, class V>
        class TreeMap<K, V>::TailMap final : public SubMap {
            TreeMap<K, V>& map;
            TailMap& sub;
            K const& head;
            gbool inclusive;

            gint mutable estSize = 0;
            gint mutable lastEst = 0;
            gint modCount = 0;

        public:
            TailMap(TreeMap<K, V> const& map, K const& head, gbool inclusive) :
                map((TreeMap &) map),
                sub(*this),
                head(head),
                inclusive(inclusive) { modCount = map.modCount; }

            TailMap(TreeMap<K, V> const& map, TailMap const& sub, K const& head, gbool inclusive) :
                map((TreeMap&) map),
                sub((TailMap&) sub),
                head(head),
                inclusive(inclusive) { modCount = map.modCount; }

            gint size() const override {
                try {
                    check();
                    if (lastEst != modCount) {
                        estSize = 0;
                        // We'll do iteration of all entries in normal sens (to minimize complexity)
                        for (NODE x = lowestNode(); x != null; x = successor(x))
                            estSize++;

                        lastEst = modCount;
                    }
                    return estSize;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsValue(const V& value) const override {
                try {
                    check();
                    // We'll iterate all entries in reverse sens
                    for (NODE x = lowestNode(); x != null; x = successor(x))
                        if (value == x->getValue())
                            return true;
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                       const K& toKey, gbool toInclusive) const override {
                gbool f = false, t = false;
                try {
                    check();
                    if (((f = inRange(fromKey, fromInclusive))) &&
                        ((t = inRange(toKey, toInclusive))) && map.compare(fromKey, toKey) >= 0)
                        return UNSAFE::newInstance<BoundedMap>(map, CORE_CAST(TailMap &, *this),
                                                               UNSAFE::copyInstance(fromKey), fromInclusive,
                                                               UNSAFE::copyInstance(toKey), toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (!f)
                    IllegalArgumentException("Start key too low").throws($ftrace());
                if (!t)
                    IllegalArgumentException("End key too low").throws($ftrace());
                IllegalArgumentException("Invalid keys range").throws($ftrace());
            }

            NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                 const K& toKey, gbool toInclusive) override {
                gbool f = false, t = false;
                try {
                    check();
                    if (((f = inRange(fromKey, fromInclusive))) &&
                        ((t = inRange(toKey, toInclusive))) && map.compare(fromKey, toKey) >= 0)
                        return UNSAFE::newInstance<BoundedMap>(map, *this,
                                                               UNSAFE::copyInstance(fromKey), fromInclusive,
                                                               UNSAFE::copyInstance(toKey), toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (!f)
                    IllegalArgumentException("Start key too low").throws($ftrace());
                if (!t)
                    IllegalArgumentException("End key too low").throws($ftrace());
                IllegalArgumentException("Invalid keys range").throws($ftrace());
            }

            const NavigableMap& tailMap(const K& fromKey, gbool inclusive) const override {
                try {
                    check();
                    if (inRange(fromKey, inclusive))
                        return UNSAFE::newInstance<TailMap>(map, CORE_CAST(TailMap &, *this),
                                                            UNSAFE::copyInstance(fromKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key too low").throws($ftrace());
            }

            NavigableMap& tailMap(const K& fromKey, gbool inclusive) override {
                try {
                    check();
                    if (inRange(fromKey, inclusive))
                        return UNSAFE::newInstance<TailMap>(map, *this,
                                                            UNSAFE::copyInstance(fromKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key too low").throws($ftrace());
            }

            const NavigableMap& headMap(const K& toKey, gbool inclusive) const override {
                try {
                    check();
                    if (inRange(toKey, inclusive))
                        return UNSAFE::newInstance<BoundedMap>(map, CORE_CAST(TailMap &, *this),
                                                               head, TailMap::inclusive,
                                                               UNSAFE::copyInstance(toKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key too low").throws($ftrace());
            }

            NavigableMap& headMap(const K& toKey, gbool inclusive) override {
                try {
                    check();
                    if (inRange(toKey, inclusive))
                        return UNSAFE::newInstance<BoundedMap>(map, *this,
                                                               head, TailMap::inclusive,
                                                               UNSAFE::copyInstance(toKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key too low").throws($ftrace());
            }

            void forEach(const function::BiConsumer<K&, V&>& action) override {
                gint mc = modCount;
                for (NODE x = lowestNode(); x != null; x = successor(x)) {
                    if (mc != tree().modCount)
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        action.accept(x->getKey(), x->getValue());
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            }

            void forEach(const function::BiConsumer<K, V>& action) const override {
                gint mc = modCount;
                for (NODE x = lowestNode(); x != null; x = successor(x)) {
                    if (mc != tree().modCount)
                        ConcurrentModificationException().throws($ftrace());
                    action.accept(x->getKey(), x->getValue());
                }
            }

            void replaceAll(const function::BiFunction<K&, V&, V>& function) override {
                try {
                    gint mc = modCount;
                    for (NODE x = lowestNode(); x != null; x = successor(x)) {
                        if (mc != tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                        x->setValue(function.apply(x->getKey(), x->getValue()));
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        protected:
            gbool tooHigh(const K& key) const override { return false; }

            gbool tooLow(const K& key) const override {
                try {
                    gint result = map.compare(key, head);
                    return inclusive ? result <= 0 : result < 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool closeRange(const K& key) const {
                try { return map.compare(key, head) <= 0; } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool inRange(const K& key) const override {
                try { return !tooLow(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool inRange(const K& key, gbool inclusive) const {
                try {
                    return inclusive ? inRange(key) : closeRange(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NODE lowestNode() const override {
                try {
                    return inclusive ? map.ceilingNode(head) : map.higherNode(head);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NODE highestNode() const override { return map.lastNode(); }

            void check() const override {
                if (modCount != map.modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void update(gint len) override {
                CORE_ALIAS(TailView, typename Class<TailMap>::Pointer);
                TailView view = this;
                do {
                    view->modCount = view->lastEst = map.modCount;
                    view->estSize += len;
                } while (view != this);
            }

            TreeMap<K, V>& tree() const override { return map; }

            SubMap& parent() const override { return sub; }

            Iterator<K>& keyIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<K> {
                    TailMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(TailMap& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? end : map.tree().lastNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (!reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    K& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getKey();
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.tree().deleteNode(x);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<K&>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(cursor->getKey());
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(cursor->getKey());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = lowestNode();
                    if (reverseOrder) // End of Descending iterator is exclusive
                        end = predecessor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& keyIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<K> {
                    TailMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(TailMap const& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? end : map.tree().lastNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (!reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    K const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getKey();
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<K>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(cursor->getKey());
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(cursor->getKey());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = lowestNode();
                    if (!reverseOrder) // End of Descending iterator is exclusive
                        end = predecessor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<V>& valueIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<V> {
                    TailMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(TailMap& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? end : map.tree().lastNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (!reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    V& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getValue();
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.tree().deleteNode(x);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<V&>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(cursor->getValue());
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(cursor->getValue());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = lowestNode();
                    if (!reverseOrder) // End of Descending iterator is exclusive
                        end = predecessor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& valueIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<V> {
                    TailMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(TailMap const& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? end : map.tree().lastNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (!reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    V const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getValue();
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<V>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(cursor->getValue());
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(cursor->getValue());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = lowestNode();
                    if (!reverseOrder) // End of Descending iterator is exclusive
                        end = predecessor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<Entry>& entryIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<Entry> {
                    TailMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(TailMap& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? end : map.tree().lastNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (!reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    Entry& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return *last;
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.tree().deleteNode(x);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<Entry&>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(*cursor);
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(*cursor);

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = lowestNode();
                    if (!reverseOrder) // End of Descending iterator is exclusive
                        end = predecessor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry>& entryIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<Entry> {
                    TailMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(TailMap const& map, NODE end, gbool reverseOrder) :
                        map(map),
                        end(end),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? end : map.tree().lastNode();
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override {
                        if (!reverseOrder)
                            return cursor != null;
                        return cursor != end;
                    }

                    Entry const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return *last;
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<Entry>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(*cursor);
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(*cursor);

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE end = lowestNode();
                    if (!reverseOrder) // End of Descending iterator is exclusive
                        end = predecessor(end);
                    return UNSAFE::newInstance<Itr>(*this, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class K, class V>
        class TreeMap<K, V>::BoundedMap final : public SubMap {
            TreeMap<K, V>& map;
            SubMap& sub;
            K const& head;
            gbool includeHead;
            K const& tail;
            gbool includeTail;

            gint mutable estSize = 0;
            gint mutable lastEst = 0;
            gint modCount = 0;

        public:
            BoundedMap(TreeMap<K, V> const& map,
                       K const& head, gbool includeHead,
                       K const& tail, gbool includeTail) :
                map((TreeMap&) map),
                sub(*this),
                head(head),
                includeHead(includeHead),
                tail(tail),
                includeTail(includeTail) { modCount = map.modCount; }

            BoundedMap(TreeMap<K, V> const& map, SubMap const& sub,
                       K const& head, gbool includeHead,
                       K const& tail, gbool includeTail) :
                map((TreeMap&) map),
                sub((SubMap&) sub),
                head(head),
                includeHead(includeHead),
                tail(tail),
                includeTail(includeTail) { modCount = map.modCount; }

            gint size() const override {
                try {
                    check();
                    if (lastEst != modCount) {
                        estSize = 0;
                        // We'll do iteration of all entries in normal sens (to minimize complexity)
                        for (NODE x = lowestNode(), y = successor(highestNode()); x != y; x = successor(x))
                            estSize++;

                        lastEst = modCount;
                    }
                    return estSize;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsValue(const V& value) const override {
                try {
                    check();
                    for (NODE x = lowestNode(), y = successor(highestNode()); x != y; x = successor(x))
                        if (value == x->getValue())
                            return true;
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                       const K& toKey, gbool toInclusive) const override {
                gbool f = false, t = false;
                try {
                    check();
                    if (((f = inRange(fromKey, fromInclusive))) &&
                        ((t = inRange(toKey, toInclusive))) && map.compare(fromKey, toKey) >= 0)
                        return UNSAFE::newInstance<BoundedMap>(map, CORE_CAST(BoundedMap &, *this),
                                                               UNSAFE::copyInstance(fromKey), fromInclusive,
                                                               UNSAFE::copyInstance(toKey), toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (!f)
                    IllegalArgumentException("Start key out of range").throws($ftrace());
                if (!t)
                    IllegalArgumentException("End key out of range").throws($ftrace());
                IllegalArgumentException("Invalid keys range").throws($ftrace());
            }

            NavigableMap& subMap(const K& fromKey, gbool fromInclusive,
                                 const K& toKey, gbool toInclusive) override {
                gbool f = false, t = false;
                try {
                    check();
                    if (((f = inRange(fromKey, fromInclusive))) &&
                        ((t = inRange(toKey, toInclusive))) && map.compare(fromKey, toKey) >= 0)
                        return UNSAFE::newInstance<BoundedMap>(map, *this,
                                                               UNSAFE::copyInstance(fromKey), fromInclusive,
                                                               UNSAFE::copyInstance(toKey), toInclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (!f)
                    IllegalArgumentException("Start key out of range").throws($ftrace());
                if (!t)
                    IllegalArgumentException("End key out of range").throws($ftrace());
                IllegalArgumentException("Invalid keys range").throws($ftrace());
            }

            const NavigableMap& tailMap(const K& fromKey, gbool inclusive) const override {
                try {
                    check();
                    if (inRange(fromKey, inclusive))
                        return UNSAFE::newInstance<BoundedMap>(map, CORE_CAST(BoundedMap &, *this),
                                                               UNSAFE::copyInstance(fromKey), inclusive,
                                                               tail, includeTail);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key out of range").throws($ftrace());
            }

            NavigableMap& tailMap(const K& fromKey, gbool inclusive) override {
                try {
                    check();
                    if (inRange(fromKey, inclusive))
                        return UNSAFE::newInstance<BoundedMap>(map, *this,
                                                               UNSAFE::copyInstance(fromKey), inclusive,
                                                               tail, includeTail);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key out of range").throws($ftrace());
            }

            const NavigableMap& headMap(const K& toKey, gbool inclusive) const override {
                try {
                    check();
                    if (inRange(toKey, inclusive))
                        return UNSAFE::newInstance<BoundedMap>(map, CORE_CAST(BoundedMap &, *this),
                                                               head, includeHead,
                                                               UNSAFE::copyInstance(toKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key out of range").throws($ftrace());
            }

            NavigableMap& headMap(const K& toKey, gbool inclusive) override {
                try {
                    check();
                    if (inRange(toKey, inclusive))
                        return UNSAFE::newInstance<BoundedMap>(map, *this,
                                                               head, includeHead,
                                                               UNSAFE::copyInstance(toKey), inclusive);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                IllegalArgumentException("Key out of range").throws($ftrace());
            }

        protected:
            gbool tooHigh(const K& key) const override {
                try {
                    gint result = map.compare(key, tail);
                    return includeTail ? result >= 0 : result > 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool tooLow(const K& key) const override {
                try {
                    gint result = map.compare(key, head);
                    return includeHead ? result <= 0 : result < 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool closeRange(const K& key) const {
                try {
                    return map.compare(key, head) >= 0 && map.compare(key, tail) <= 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool inRange(const K& key) const override {
                try { return !tooLow(key) && !tooHigh(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool inRange(const K& key, gbool inclusive) const {
                try {
                    return inclusive ? inRange(key) : closeRange(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NODE lowestNode() const override {
                try {
                    return includeHead ? map.ceilingNode(head) : map.higherNode(head);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            NODE highestNode() const override {
                try {
                    return includeTail ? map.floorNode(tail) : map.lowerNode(tail);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void check() const override {
                if (modCount != map.modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void update(gint len) override {
                CORE_ALIAS(SubView, typename Class<SubMap>::Pointer);
                CORE_ALIAS(BoundedView, typename Class<BoundedMap>::Pointer);
                BoundedView view = this;
                SubView p = view;
                do {
                    view->modCount = view->lastEst = map.modCount;
                    view->estSize += len;
                    p = &view->parent();
                    if (!CORE_DCAST(BoundedView, p)) {
                        // The parent is not BoundedMap (HeadMap or TailMap)
                        p->update(len);
                        break;
                    }
                    view = CORE_DCAST(BoundedView, p);
                } while (view != this);
            }

            TreeMap<K, V>& tree() const override { return map; }

            SubMap& parent() const override { return sub; }

            Iterator<K>& keyIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<K> {
                    BoundedMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(BoundedMap& map, NODE start, NODE end, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : start;
                        Itr::end = reverseOrder ? start : end;
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override { return cursor != end; }

                    K& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getKey();
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.tree().deleteNode(x);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<K&>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(cursor->getKey());
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(cursor->getKey());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE start = lowestNode();
                    NODE end = highestNode();
                    if (reverseOrder)
                        start = predecessor(start);
                    else
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, start, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& keyIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<K> {
                    BoundedMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(BoundedMap const& map, NODE start, NODE end, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : start;
                        Itr::end = reverseOrder ? start : end;
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override { return cursor != end; }

                    K const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getKey();
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<K>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(cursor->getKey());
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(cursor->getKey());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE start = lowestNode();
                    NODE end = highestNode();
                    if (reverseOrder)
                        start = predecessor(start);
                    else
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, start, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<V>& valueIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<V> {
                    BoundedMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(BoundedMap& map, NODE start, NODE end, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : start;
                        Itr::end = reverseOrder ? start : end;
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override { return cursor != end; }

                    V& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getValue();
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE x = last;
                        last = null;
                        map.tree().deleteNode(x);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<V&>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(cursor->getValue());
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(cursor->getValue());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE start = lowestNode();
                    NODE end = highestNode();
                    if (reverseOrder)
                        start = predecessor(start);
                    else
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, start, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& valueIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<V> {
                    BoundedMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(BoundedMap const& map, NODE start, NODE end, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : start;
                        Itr::end = reverseOrder ? start : end;
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override { return cursor != end; }

                    V const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return last->getValue();
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<V>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(cursor->getValue());
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(cursor->getValue());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE start = lowestNode();
                    NODE end = highestNode();
                    if (reverseOrder)
                        start = predecessor(start);
                    else
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, start, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<Entry>& entryIterator(gbool reverseOrder) override {
                class Itr final : public Iterator<Entry> {
                    BoundedMap& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(BoundedMap& map, NODE start, NODE end, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : start;
                        Itr::end = reverseOrder ? start : end;
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override { return cursor != end; }

                    Entry& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return *last;
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        NODE node = last;
                        last = null;
                        map.tree().deleteNode(node);
                        map.update(-1);
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<Entry&>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(*cursor);
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(*cursor);

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE start = lowestNode();
                    NODE end = highestNode();
                    if (reverseOrder)
                        start = predecessor(start);
                    else
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, start, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry>& entryIterator(gbool reverseOrder) const override {
                class Itr final : public Iterator2<Entry> {
                    BoundedMap const& map;
                    NODE cursor = { };
                    NODE end = { };
                    NODE last = { };
                    gbool reverseOrder = false;
                    gint mc = 0;

                public:
                    CORE_EXPLICIT Itr(BoundedMap const& map, NODE start, NODE end, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = reverseOrder ? end : start;
                        Itr::end = reverseOrder ? start : end;
                        mc = map.tree().modCount;
                    }

                    gbool hasNext() const override { return cursor != end; }

                    Entry const& next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());

                        last = cursor;
                        cursor = reverseOrder ? predecessor(cursor) : successor(cursor);
                        return *last;
                    }

                    gbool equals(const Object& other) const override {
                        if (this != &other)
                            return false;

                        if (!Class<Itr>::hasInstance(other))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &map == &it.map && cursor == it.cursor && reverseOrder == it.reverseOrder;
                    }

                    void forEach(const function::Consumer<Entry>& action) override {
                        if (reverseOrder)
                            for (; cursor != null && mc == map.tree().modCount; cursor = predecessor(cursor))
                                action.accept(*cursor);
                        else
                            for (; cursor != end && mc == map.tree().modCount; cursor = successor(cursor))
                                action.accept(*cursor);

                        if (mc != map.tree().modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    check();
                    NODE start = lowestNode();
                    NODE end = highestNode();
                    if (reverseOrder)
                        start = predecessor(start);
                    else
                        end = successor(end);
                    return UNSAFE::newInstance<Itr>(*this, start, end, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };
    } // util
} // core

#endif //CORE24_TREEMAP_H
