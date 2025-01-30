//
// Created by brunshweeck on 14 août 2024.
//

#ifndef CORE24_LINKEDHASHMAP_H
#define CORE24_LINKEDHASHMAP_H

#include <core/util/HashMap.h>
#include <core/util/SequencedMap.h>

namespace core {
    namespace util {
        /**
         * <p>
         * Hash table and linked list implementation of the @c Map interface,
         * with well-defined encounter order.  This implementation differs from
         * @c HashMap in that it maintains a doubly-linked list running through all of
         * its entries.  This linked list defines the encounter order (the order of iteration),
         * which is normally the order in which keys were inserted into the map
         * (<i>insertion-order</i>). The least recently inserted entry (the eldest) is
         * first, and the youngest entry is last. Note that encounter order is not affected
         * if a key is <i>re-inserted</i> into the map with the @c put method. (A key
         * @c k is reinserted into a map @c m if @code m.put(k, v) @endcode is invoked when
         * @c m.containsKey(k) would return @c true immediately prior to
         * the invocation.) The reverse-ordered view of this map is in the opposite order, with
         * the youngest entry appearing first and the eldest entry appearing last.
         * The encounter order of entries already in the map can be changed by using
         * the @b putFirst and @b putLast methods.
         * </p>
         * <p>
         * This implementation spares its clients from the unspecified, generally
         * chaotic ordering provided by @b HashMap,
         * without incurring the increased cost associated with @b TreeMap.  It
         * can be used to produce a copy of a map that has the same order as the
         * original, regardless of the original map's implementation:
         * @code
         *     void foo(Map<String, Integer> const& m) {
         *         Map<String, Integer> &copy = UNSAFE::newInstance<LinkedHashMap<String, Integer>>(m);
         *         ...
         *     }
         * @endcode
         * This technique is particularly useful if a module takes a map on input,
         * copies it, and later returns results whose order is determined by that of
         * the copy.  (Clients generally appreciate having things returned in the same
         * order they were presented.)
         * </p>
         * <p>
         * A special @b constructor is
         * provided to create a linked hash map whose encounter order is the order
         * in which its entries were last accessed, from least-recently accessed to
         * most-recently (<i>access-order</i>).  This kind of map is well-suited to
         * building LRU caches.  Invoking the @c put, @b putIfAbsent,
         * @c get, @c getOrDefault, @c getOrNull methods results
         * in access to the corresponding entry (assuming it exists after the
         * invocation completes). The @c replace methods only result in access
         * of the entry if the value is replaced.  The @c putAll method generates one
         * entry access for each mapping in the specified map, in the order that
         * key-value mappings are provided by the specified map's entry set iterator.
         * <i>No other methods generate entry accesses.</i> Invoking these methods on the
         * reversed view generates accesses to entries on the backing map. Note that in the
         * reversed view, access to an entry moves it first in encounter order.
         * Explicit-positioning methods such as @c putFirst or @c lastEntry, whether on
         * the map or on its reverse-ordered view, perform the positioning operation and
         * do not generate entry accesses. Operations on the @c keySet, @c values,
         * and @c entrySet views or on their sequenced counterparts do <i>not</i> affect
         * the encounter order of the backing map.
         * </p>
         * <p>
         * The @b deleteLastNode(Entry) method may be overridden to
         * impose a policy for removing stale mappings automatically when new mappings
         * are added to the map. Alternatively, since the "eldest" entry is the first
         * entry in encounter order, programs can inspect and remove stale mappings through
         * use of the @b firstEntry and @b pollFirstEntry
         * methods.
         * </p>
         * <p>
         * This class provides all the optional @c Map and @c SequencedMap operations,
         * and it permits null elements.  Like @c HashMap, it provides constant-time
         * performance for the basic operations (@c add, @c contains and
         * @c remove), assuming the hash function disperses elements
         * properly among the buckets.  Performance is likely to be just slightly
         * below that of @c HashMap, due to the added expense of maintaining the
         * linked list, with one exception: Iteration over the collection-views
         * of a @c LinkedHashMap requires time proportional to the <i>size</i>
         * of the map, regardless of its capacity.  Iteration over a @c HashMap
         * is likely to be more expensive, requiring time proportional to its
         * <i>capacity</i>.
         * </p>
         * <p>
         * A linked hash map has two parameters that affect its performance:
         * <i>initial capacity</i> and <i>load factor</i>.  They are defined precisely
         * as for @c HashMap.  Note, however, that the penalty for choosing an
         * excessively high value for initial capacity is less severe for this class
         * than for @c HashMap, as iteration times for this class are unaffected
         * by capacity.
         * </p>
         * <p>
         * <b>Note that this implementation is not synchronized.</b>
         * If multiple threads access a linked hash map concurrently, and at least
         * one of the threads modifies the map structurally, it <em>must</em> be
         * synchronized externally.  This is typically accomplished by
         * synchronizing on some object that naturally encapsulates the map.
         *
         * If no such object exists, the map should be "wrapped" using the
         * @b Collections.synchronizedMap method.
         * This is best done at creation time, to prevent accidental
         * unsynchronized access to the map:@code
         *   auto& m = Collections.synchronizedMap(UNSAFE::newInstance<LinkedHashMap<>>(...));
         * @endcode
         *
         * A structural modification is any operation that adds or deletes one or more
         * mappings or, in the case of access-ordered linked hash maps, affects
         * iteration order.  In insertion-ordered linked hash maps, merely changing
         * the value associated with a key that is already contained in the map is not
         * a structural modification.  <b>In access-ordered linked hash maps,
         * merely querying the map with @c get is a structural modification.
         * </b>
         * </p>
         * <p>
         * The iterators returned by the @c iterator method of the collections
         * returned by all of this class's collection view methods are
         * <em>fail-fast</em>: if the map is structurally modified at any time after
         * the iterator is created, in any way except through the iterator's own
         * @c remove method, the iterator will throw a
         * @b ConcurrentModificationException.  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the future.
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
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         *
         * @see     Object::hash()
         * @see     Collection
         * @see     Map
         * @see     HashMap
         * @see     TreeMap
         */
        template <class K, class V>
        class LinkedHashMap final : public virtual HashMap<K, V>, public virtual SequencedMap<K, V> {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            CORE_ALIAS(Entry, util::Entry<K, V>);
            CORE_ALIAS(Map, util::Map<K, V>);
            CORE_ALIAS(SequencedMap, util::SequencedMap<K, V>);
            CORE_ALIAS(HashMap, util::HashMap<K, V>);

            CORE_ALIAS(SequencedKeys, typename Class<SequencedSet<K>>::Pointer);
            CORE_ALIAS(SequencedVals, typename Class<SequencedCollection<V>>::Pointer);
            CORE_ALIAS(SequencedEntries, typename Class<SequencedSet<Entry>>::Pointer);
            CORE_ALIAS(SequencedView, typename Class<SequencedMap>::Pointer);

            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, Node);
            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, LinkedNode);
            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, TreeNode);
            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, NODE);
            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, LINKEDNODE);
            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, TREENODE);
            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, Keys);
            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, Vals);
            CORE_IMPORT_FIELD_OR_METHOD(typename HashMap, Entries);

            /**
             * The head (eldest) of the doubly linked list.
             */
            LINKEDNODE head = null;

            /**
             * The tail (youngest) of the doubly linked list.
             */
            LINKEDNODE tail = null;

            /**
             * The iteration ordering method for this linked hash map: @c true
             * for access-order, @c false for insertion-order.
             */
            enum AccessOrder { INSERTION, ACCESS } accessOrder = INSERTION;

        public:
            /**
             * Constructs an empty insertion-ordered @c LinkedHashMap instance
             * with the specified initial capacity and load factor.
             *
             * @param  initialCapacity the initial capacity
             * @param  loadFactor      the load factor
             * @throws IllegalArgumentException if the initial capacity is negative
             *         or the load factor is non-positive
             */
            CORE_EXPLICIT LinkedHashMap(gint initialCapacity, gfloat loadFactor) :
                HashMap(initialCapacity, loadFactor),
                accessOrder(ACCESS) {}

            /**
             * Constructs an empty insertion-ordered @c LinkedHashMap instance
             * with the specified initial capacity and a default load factor (0.75).
             *
             * @param  initialCapacity the initial capacity
             * @throws IllegalArgumentException if the initial capacity is negative
             */
            CORE_EXPLICIT LinkedHashMap(gint initialCapacity) :
                HashMap(initialCapacity),
                accessOrder(ACCESS) {}

            /**
             * Constructs an empty insertion-ordered @c LinkedHashMap instance
             * with the default initial capacity (16) and load factor (0.75).
             */
            CORE_IMPLICIT LinkedHashMap() :
                HashMap(),
                accessOrder(ACCESS) {}

            /**
             * Constructs an insertion-ordered @c LinkedHashMap instance with
             * the same mappings as the specified map.  The @c LinkedHashMap
             * instance is created with a default load factor (0.75) and an initial
             * capacity sufficient to hold the mappings in the specified map.
             *
             * @param  m the map whose mappings are to be placed in this map
             */
            CORE_EXPLICIT LinkedHashMap(Map const& m) : HashMap(), accessOrder(ACCESS) {
                try {
                    HashMap::putNodes(m, false);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            template <class X, class Y,
                      ClassOf(1)::OnlyIf<Class<K>::template isSuper<X>() && Class<V>::template isSuper<Y>()>  = 1>
            CORE_EXPLICIT LinkedHashMap(util::Map<X, Y> const& m) : HashMap(), accessOrder(ACCESS) {
                try {
                    HashMap::putNodes(m, false);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            LinkedHashMap(LinkedHashMap const& m) :
                HashMap(),
                accessOrder(ACCESS) { HashMap::putNodes(m, false); }

            LinkedHashMap(LinkedHashMap&& m) CORE_NOTHROW :
                HashMap(UNSAFE::moveInstance(m)) {
                if (this != &m) {
                    UNSAFE::swapValues(accessOrder, m.accessOrder);
                    UNSAFE::swapValues(head, m.head);
                    UNSAFE::swapValues(tail, m.tail);
                }
            }

            /**
             * Constructs an empty @c LinkedHashMap instance with the
             * specified initial capacity, load factor and ordering mode.
             *
             * @param  initialCapacity the initial capacity
             * @param  loadFactor      the load factor
             * @param  accessOrder     the ordering mode - @c true for
             *         access-order, @c false for insertion-order
             * @throws IllegalArgumentException if the initial capacity is negative
             *         or the load factor is non-positive
             */
            CORE_EXPLICIT LinkedHashMap(gint initialCapacity, gfloat loadFactor, gbool accessOrder) :
                HashMap(initialCapacity, loadFactor),
                accessOrder(accessOrder ? ACCESS : INSERTION) {}

            LinkedHashMap& operator=(LinkedHashMap const& m) {
                if (this != &m) {
                    LinkedHashMap m2 = m;
                    initialize(m2);
                }
                return *this;
            }

            /**
             * Returns @c true if this map maps one or more keys to the
             * specified value.
             *
             * @param value value whose presence in this map is to be tested
             * @return @c true if this map maps one or more keys to the
             *         specified value
             */
            gbool containsValue(const V& value) const override {
                for (LINKEDNODE x = head; x != null; x = HashMap::afterOf(x))
                    if (value == HashMap::valueOf(x))
                        return true;
                return false;
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or thrown @c MissingKeyException if this map contains no mapping for the key.
             *
             * <p>
             * More formally, if this map contains a mapping from a key
             * @c k to a value @c v such that
             * @c key.equals(k), then this method returns @c v; otherwise
             * it returns thrown @c MissingKeyException.  (There can be at most one such mapping.)
             * </p>
             */
            V& get(const K& key) override {
                NODE x = HashMap::exactNode(key);
                if (x == null)
                    MissingKeyException(key).throws($ftrace());
                if (accessOrder == ACCESS)
                    afterAccess(x);
                return HashMap::valueOf(x);
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or thrown @c MissingKeyException if this map contains no mapping for the key.
             *
             * <p>More formally, if this map contains a mapping from a key
             * @c k to a value @c v such that
             * @c key.equals(k), then this method returns @c v; otherwise
             * it returns thrown @c MissingKeyException.  (There can be at most one such mapping.)
             */
            const V& get(const K& key) const override {
                NODE x = HashMap::exactNode(key);
                if (x == null)
                    MissingKeyException(key).throws($ftrace());
                if (accessOrder == ACCESS)
                    CORE_CAST(LinkedHashMap &, *this).afterAccess(x);
                return HashMap::valueOf(x);
            }

            V& getOrDefault(const K& key, const V& defaultValue) override {
                NODE x = HashMap::exactNode(key);
                if (x == null)
                    return UNSAFE::copyInstance(defaultValue);
                if (accessOrder == ACCESS)
                    afterAccess(x);
                return HashMap::valueOf(x);
            }

            const V& getOrDefault(const K& key, const V& defaultValue) const override {
                NODE x = HashMap::exactNode(key);
                if (x == null)
                    return UNSAFE::copyInstance(defaultValue);
                if (accessOrder == ACCESS)
                    CORE_CAST(LinkedHashMap &, *this).afterAccess(x);
                return HashMap::valueOf(x);
            }

            Object& getOrNull(const K& key) override {
                NODE x = HashMap::exactNode(key);
                if (x == null)
                    return null;
                if (accessOrder == ACCESS)
                    afterAccess(x);
                return HashMap::valueOf(x);
            }

            const Object& getOrNull(const K& key) const override {
                NODE x = HashMap::exactNode(key);
                if (x == null)
                    return null;
                if (accessOrder == ACCESS)
                    CORE_CAST(LinkedHashMap &, *this).afterAccess(x);
                return HashMap::valueOf(x);
            }

            void clear() override {
                HashMap::clear();
                head = tail = null;
            }

            /**
             * Returns a @b Set view of the keys contained in this map. The encounter
             * order of the keys in the view matches the encounter order of mappings of
             * this map. The set is backed by the map, so changes to the map are
             * reflected in the set, and vice versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own @c remove operation), the results of
             * the iteration are undefined.  The set supports element removal,
             * which removes the corresponding mapping from the map, via the
             * @c Iterator::remove, @c Set::remove,
             * @c removeAll, @c retainAll, and @c clear
             * operations.  It does not support the @c add or @c addAll
             * operations.
             *
             * @return a set view of the keys contained in this map
             */
            Set<K>& keySet() override {
                try { return sequencedKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a @b Set view of the keys contained in this map. The encounter
             * order of the keys in the view matches the encounter order of mappings of
             * this map. The set is backed by the map, so changes to the map are
             * reflected in the set, and vice versa.  If the map is modified
             * while an iteration over the set is in progress (except through
             * the iterator's own @c remove operation), the results of
             * the iteration are undefined.  The set supports element removal,
             * which removes the corresponding mapping from the map, via the
             * @c Iterator::remove, @c Set::remove,
             * @c removeAll, @c retainAll, and @c clear
             * operations.  It does not support the @c add or @c addAll
             * operations.
             *
             * @return a set view of the keys contained in this map
             */
            const Set<K>& keySet() const override {
                try { return sequencedKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<K>& sequencedKeySet() override {
                SequencedKeys view = CORE_DCAST(SequencedKeys, keys);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<KeySet>(*this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    keys = view;
                }
                return *view;
            }

            const SequencedSet<K>& sequencedKeySet() const override {
                SequencedKeys view = CORE_DCAST(SequencedKeys, keys);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<KeySet>((LinkedHashMap&) *this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    keys = view;
                }
                return *view;
            }

            /**
             * Returns a @b Collection view of the values contained in this map. The
             * encounter order of values in the view matches the encounter order of entries in
             * this map. The collection is backed by the map, so changes to the map are
             * reflected in the collection, and vice versa.  If the map is
             * modified while an iteration over the collection is in progress
             * (except through the iterator's own @c remove operation),
             * the results of the iteration are undefined.  The collection
             * supports element removal, which removes the corresponding
             * mapping from the map, via the @c Iterator::remove,
             * @c Collection::remove, @c removeAll,
             * @c retainAll and @c clear operations.  It does not
             * support the @c add or @c addAll operations.
             *
             * @return a view of the values contained in this map
             */
            Collection<V>& values() override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a @b Collection view of the values contained in this map. The
             * encounter order of values in the view matches the encounter order of entries in
             * this map. The collection is backed by the map, so changes to the map are
             * reflected in the collection, and vice versa.  If the map is
             * modified while an iteration over the collection is in progress
             * (except through the iterator's own @c remove operation),
             * the results of the iteration are undefined.  The collection
             * supports element removal, which removes the corresponding
             * mapping from the map, via the @c Iterator::remove,
             * @c Collection::remove, @c removeAll,
             * @c retainAll and @c clear operations.  It does not
             * support the @c add or @c addAll operations.
             *
             * @return a view of the values contained in this map
             */
            const Collection<V>& values() const override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
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
                        view = &UNSAFE::newInstance<Values>((LinkedHashMap&) *this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    vals = view;
                }
                return *view;
            }

            /**
             * Returns a @b Set view of the mappings contained in this map. The encounter
             * order of the view matches the encounter order of entries of this map.
             * The set is backed by the map, so changes to the map are
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
             *
             * @return a set view of the mappings contained in this map
             */
            Set<Entry>& entrySet() override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a @b Set view of the mappings contained in this map. The encounter
             * order of the view matches the encounter order of entries of this map.
             * The set is backed by the map, so changes to the map are
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
             *
             * @return a set view of the mappings contained in this map
             */
            const Set<Entry>& entrySet() const override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
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
                    try {
                        view = &UNSAFE::newInstance<EntrySet>((LinkedHashMap&) *this, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    entries = view;
                }
                return *view;
            }

            // Map overrides

            /**
             *
             */
            void foreach(const function::BiConsumer<K&, V&>& action) override {
                gint mc = HashMap::modCount;
                try {
                    for (LINKEDNODE x = head; x != null && mc == HashMap::modCount; x = HashMap::afterOf(x))
                        action.accept(HashMap::keyOf(x), HashMap::valueOf(x));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (mc != HashMap::modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void foreach(const function::BiConsumer<K, V>& action) const override {
                gint mc = HashMap::modCount;
                for (LINKEDNODE x = head; x != null && mc == HashMap::modCount; x = HashMap::afterOf(x))
                    action.accept(HashMap::keyOf(x), HashMap::valueOf(x));
                if (mc != HashMap::modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void replaceAll(const function::BiFunction<K&, V&, V>& function) override {
                gint mc = HashMap::modCount;
                for (LINKEDNODE x = head; x != null && mc == HashMap::modCount; x = HashMap::afterOf(x))
                    x->value =
                            &UNSAFE::copyInstance(function.apply(HashMap::keyOf(x), HashMap::valueOf(x)));
                if (mc != HashMap::modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            Object& clone() const override {
                try {
                    return UNSAFE::newInstance<LinkedHashMap>(*this);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedMap& reversed() override {
                if (!reverse) {
                    try {
                        reverse = &UNSAFE::newInstance<Reverse>(*this);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
                return *reverse;
            }

            SequencedMap const& reversed() const override {
                if (!reverse) {
                    try {
                        reverse = &UNSAFE::newInstance<Reverse>((LinkedHashMap&) *this);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
                return *reverse;
            }

            Entry& firstEntry() override {
                if (!head)
                    NoSuchElementException().throws($ftrace());
                return *head;
            }

            Entry const& firstEntry() const override {
                if (!head)
                    NoSuchElementException().throws($ftrace());
                return *head;
            }

            Entry& lastEntry() override {
                if (!tail)
                    NoSuchElementException().throws($ftrace());
                return *tail;
            }

            Entry const& lastEntry() const override {
                if (!tail)
                    NoSuchElementException().throws($ftrace());
                return *tail;
            }

            Entry& pollFirstEntry() override {
                if (!head)
                    NoSuchElementException().throws($ftrace());
                LINKEDNODE node = head;
                deleteNode(hashOf(node), keyOf(node), null, false, false);
                return *node;
            }

            Entry& pollLastEntry() override {
                if (!tail)
                    NoSuchElementException().throws($ftrace());
                LINKEDNODE node = tail;
                deleteNode(hashOf(node), keyOf(node), null, false, false);
                return *node;
            }

            Object& putFirst(K const& k, V const& v) override {
                try {
                    mode = INSERT_FIRST;
                    Object& old = HashMap::put(k, v);
                    mode = INSERT_DEFAULT;
                    return old;
                } catch (Throwable const& ex) {
                    mode = INSERT_DEFAULT;
                    ex.throws($ftrace());
                }
            }

            Object& putLast(K const& k, V const& v) override {
                try {
                    mode = INSERT_LAST;
                    Object& old = HashMap::put(k, v);
                    mode = INSERT_DEFAULT;
                    return old;
                } catch (Throwable const& ex) {
                    mode = INSERT_DEFAULT;
                    ex.throws($ftrace());
                }
            }

            ~LinkedHashMap() override {
                LinkedHashMap::clear();

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

        protected:
            /**
             * Returns @c true if this map should remove its eldest entry.
             * This method is invoked by @c put and @c putAll after
             * inserting a new entry into the map.  It provides the implementor
             * with the opportunity to remove the eldest entry each time a new one
             * is added.  This is useful if the map represents a cache: it allows
             * the map to reduce memory consumption by deleting stale entries.
             *
             * <p>Sample use: this override will allow the map to grow up to 100
             * entries and then delete the eldest entry each time a new entry is
             * added, maintaining a steady state of 100 entries.
             * <pre>
             *     private static final int MAX_ENTRIES = 100;
             *
             *     protected boolean removeEldestEntry(Map.Entry eldest) {
             *        return size() &gt; MAX_ENTRIES;
             *     }
             * </pre>
             *
             * <p>This method typically does not modify the map in any way,
             * instead allowing the map to modify itself as directed by its
             * return value.  It <i>is</i> permitted for this method to modify
             * the map directly, but if it does so, it <i>must</i> return
             * @c false (indicating that the map should not attempt any
             * further modification).  The effects of returning @c true
             * after modifying the map from within this method are unspecified.
             *
             * <p>This implementation merely returns @c false (so that this
             * map acts like a normal map - the eldest element is never removed).
             *
             * @param    eldest The least recently inserted entry in the map, or if
             *           this is an access-ordered map, the least recently accessed
             *           entry.  This is the entry that will be removed if this
             *           method returns @c true.  If the map was empty prior
             *           to the @c put or @c putAll invocation resulting
             *           in this invocation, this will be the entry that was just
             *           inserted; in other words, if the map contains a single
             *           entry, the eldest entry is also the newest.
             * @return   @c true if the eldest entry should be removed
             *           from the map; @c false if it should be retained.
             */
            gbool deleteLastNode(Entry& eldest) { return false; }

        private:
            // internal utilities

            // link at the start of list
            void linkFirst(LINKEDNODE x) {
                LINKEDNODE first = head;
                head = x;
                if (first == null)
                    tail = x;
                else {
                    x->after = first;
                    first->before = x;
                }
            }

            // link at the end of list
            void linkLast(LINKEDNODE x) {
                LINKEDNODE last = tail;
                tail = x;
                if (last == null)
                    head = x;
                else {
                    x->before = last;
                    last->after = x;
                }
            }

            NODE newNode(gint hash, const K& key, const V& value, NODE next) override {
                K& k = UNSAFE::copyInstance(key);
                V& v = UNSAFE::copyInstance(value);
                LINKEDNODE x =
                        &UNSAFE::newReplaceableInstance<LinkedNode>(Class<TreeNode>::size(), hash, k, v, next);
                linkLast(x);
                return x;
            }

            NODE convert2Node(NODE x, NODE next) override {
                // oldLinks
                LINKEDNODE old = HashMap::toLinkedNode(x);
                LINKEDNODE after = HashMap::afterOf(old);
                LINKEDNODE before = HashMap::beforeOf(old);
                LINKEDNODE repl = new(x) LinkedNode(hashOf(x), keyOf(x), valueOf(x), next);
                // transfer all links to replacement node.
                repl->after = after;
                repl->before = before;
                if (HashMap::beforeOf(repl) == null)
                    head = repl;
                else
                    HashMap::beforeOf(repl)->after = repl;
                if (HashMap::afterOf(repl) == null)
                    tail = repl;
                else
                    HashMap::afterOf(repl)->before = repl;
                // end of operations
                return repl;
            }

            TREENODE newTreeNode(gint hash, const K& key, const V& value, NODE next) override {
                K& k = UNSAFE::copyInstance(key);
                V& v = UNSAFE::copyInstance(value);
                TREENODE x = &UNSAFE::newInstance<TreeNode>(hash, k, v, next);
                if (mode == INSERT_FIRST)
                    linkFirst(x);
                else
                    linkLast(x);
                return x;
            }

            TREENODE convert2TreeNode(NODE x, NODE next) override {
                // oldLinks
                LINKEDNODE old = CORE_DCAST(LINKEDNODE, x);
                LINKEDNODE after = HashMap::afterOf(old);
                LINKEDNODE before = HashMap::beforeOf(old);
                TREENODE repl = new(x) TreeNode(hashOf(x), keyOf(x), valueOf(x), next);
                // transfer all links to replacement node.
                repl->after = after;
                repl->before = before;
                if (HashMap::beforeOf(repl) == null)
                    head = repl;
                else
                    HashMap::beforeOf(repl)->after = repl;
                if (HashMap::afterOf(repl) == null)
                    tail = repl;
                else
                    HashMap::afterOf(repl)->before = repl;
                // end of operations
                return repl;
            }

            void afterAccess(NODE x) override {
                LINKEDNODE last = null, first = null;
                if ((mode == INSERT_LAST || (mode == INSERT_DEFAULT && accessOrder == ACCESS)) && (last = tail) != x) {
                    // move node to last
                    LINKEDNODE lx = HashMap::toLinkedNode(x);
                    LINKEDNODE b = HashMap::beforeOf(lx);
                    LINKEDNODE a = HashMap::afterOf(lx);
                    lx->after = null;
                    if (b == null)
                        head = a;
                    else
                        b->after = a;
                    if (a != null)
                        a->before = b;
                    else
                        last = b;
                    if (last == null)
                        head = lx;
                    else {
                        lx->before = last;
                        last->after = lx;
                    }
                    tail = lx;
                    ++HashMap::modCount;
                } else {
                    // move node to first
                    LINKEDNODE lx = HashMap::toLinkedNode(x);
                    LINKEDNODE b = HashMap::beforeOf(lx);
                    LINKEDNODE a = HashMap::afterOf(lx);
                    lx->before = null;
                    if (a == null)
                        tail = b;
                    else
                        a->before = b;
                    if (b != null)
                        b->after = a;
                    else
                        first = a;
                    if (first == null)
                        tail = lx;
                    else {
                        lx->after = first;
                        first->before = lx;
                    }
                    head = lx;
                    ++HashMap::modCount;
                }
            }

            void afterInsertion(gbool evict) override {
                LINKEDNODE first = null;
                if (evict && (first = head) != null && deleteLastNode(*first))
                    deleteNode(hashKey(keyOf(first)), keyOf(first), null, false, true);
            }

            void afterDeletion(NODE x) override {
                LINKEDNODE lx = HashMap::toLinkedNode(x);
                LINKEDNODE b = HashMap::beforeOf(lx);
                LINKEDNODE a = HashMap::afterOf(lx);
                lx->before = lx->after = null;
                if (b == null)
                    head = a;
                else
                    b->after = a;
                if (a == null)
                    tail = b;
                else
                    a->before = b;
            }

            enum InsertionMode {
                INSERT_DEFAULT,
                INSERT_FIRST,
                INSERT_LAST,
            };

            InsertionMode mode = INSERT_DEFAULT;

            void initialize(HashMap& m2) override {
                HashMap::initialize(m2);
                if (Class<LinkedHashMap>::hasInstance(m2)) {
                    LinkedHashMap& m3 = CORE_XCAST(LinkedHashMap, m2);
                    UNSAFE::swapValues(head, m3.head);
                    UNSAFE::swapValues(tail, m3.tail);
                    UNSAFE::swapValues(accessOrder, m3.accessOrder);
                    UNSAFE::swapValues(mode, m3.mode);
                }
            }

            static LINKEDNODE nextOf(LINKEDNODE node) { return node->after; }

            static LINKEDNODE prevOf(LINKEDNODE node) { return node->before; }

            static K& keyOf(NODE node) { return node->getKey(); }

            static V& valueOf(NODE node) { return node->getValue(); }

            static gint hashOf(NODE node) { return node->hash; }

            CORE_IMPORT_FIELD_OR_METHOD(HashMap, deleteNode);
            CORE_IMPORT_FIELD_OR_METHOD(HashMap, hashKey);

            // ::::::::::::::::::::: LinkedHashMap views Fields ::::::::::::::::::::::::::::::

            /**
             * The keys view field
             */
            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);

            /**
             * The values view field
             */
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);

            /**
             * The entries view field
             */
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);

            /**
             * The reverse view field
             */
            CORE_IMPORT_FIELD_OR_METHOD(SequencedMap, reverse);

            // ::::::::::::::::::::: LinkedHashMap views Destructors Utility :::::::::::::::::::::::::::::
            static void detachKeys(LinkedHashMap& map, gbool reverseOrder) {
                if (!reverseOrder)
                    Map::detachKeys(map);
                else
                    Map::detachKeys(map.reversed());
            }

            static void detachVals(LinkedHashMap& map, gbool reverseOrder) {
                if (!reverseOrder)
                    Map::detachVals(map);
                else
                    Map::detachVals(map.reversed());
            }

            static void detachEntries(LinkedHashMap const& map, gbool reverseOrder) {
                if (!reverseOrder)
                    Map::detachEntries(map);
                else
                    Map::detachEntries(map.reversed());
            }

            static void detachReverse(LinkedHashMap const& map) { SequencedMap::detachReverse(map); }

            // ::::::::::::::::::::: LinkedHashMap views Classes :::::::::::::::::::::::::::::

            /**
             * The keys view class
             */
            class KeySet;
            /**
             * The values view class
             */
            class Values;
            /**
             * The entries view class
             */
            class EntrySet;
            /**
             * The reverse view class
             */
            class Reverse;
        };

        template <class K, class V>
        class LinkedHashMap<K, V>::KeySet final : public SequencedSet<K> {
            LinkedHashMap<K, V>& map;
            gbool reverseOrder = false;

        public:
            KeySet(LinkedHashMap<K, V> const& map) : map((LinkedHashMap&) map) {}

            KeySet(LinkedHashMap<K, V>& map, gbool reverseOrder) :
                map(map), reverseOrder(reverseOrder) {}

            gint size() const override { return map.size(); }

            gbool isEmpty() const override { return map.isEmpty(); }

            Iterator<K>& iterator() override {
                class Itr final : public Iterator<K> {
                    LinkedHashMap<K, V>& map;
                    LINKEDNODE cursor = null;
                    LINKEDNODE last = null;
                    gint modCount = 0;
                    gbool reverseOrder;

                public:
                    Itr(LinkedHashMap& map, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? map.head : map.tail;
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    K&next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = !reverseOrder ? nextOf(cursor) : prevOf(cursor);
                        return keyOf(last);
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        LINKEDNODE node = last;
                        last = null;
                        map.deleteNode(hashOf(node), keyOf(node), null, false, false);
                        modCount = map.modCount;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && reverseOrder == itr.reverseOrder && cursor == itr.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(map, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& iterator() const override {
                class Itr final : public Iterator2<K> {
                    LinkedHashMap<K, V> const& map;
                    LINKEDNODE cursor = null;
                    LINKEDNODE last = null;
                    gint modCount = 0;
                    gbool reverseOrder;

                public:
                    Itr(LinkedHashMap<K, V> const& map, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? map.head : map.tail;
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    K const&next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = !reverseOrder ? nextOf(cursor) : prevOf(cursor);
                        return keyOf(last);
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && reverseOrder == itr.reverseOrder && cursor == itr.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(map, reverseOrder); //
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<K> const& reversed() const override {
                try {
                    return (reverseOrder) ? map.sequencedKeySet() : map.reversed().sequencedKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<K>& reversed() override {
                try {
                    return (reverseOrder) ? map.sequencedKeySet() : map.reversed().sequencedKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<K&> const& action) override {
                gint modCount = map.modCount;
                if (reverseOrder) {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        K& key = keyOf(node);
                        action.accept(key);
                    }
                } else {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        K& key = keyOf(node);
                        action.accept(key);
                    }
                }
            }

            void forEach(function::Consumer<K> const& action) const override {
                gint modCount = map.modCount;
                if (reverseOrder) {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        K& key = keyOf(node);
                        action.accept(key);
                    }
                } else {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        K& key = keyOf(node);
                        action.accept(key);
                    }
                }
            }

            void addFirst(K const& val) override { UnsupportedOperationException().throws($ftrace()); }

            void addLast(K const& val) override { UnsupportedOperationException().throws($ftrace()); }

            K& firstElement() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return !reverseOrder ? keyOf(map.head) : keyOf(map.tail);
            }

            K const& firstElement() const override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return !reverseOrder ? keyOf(map.head) : keyOf(map.tail);
            }

            K& lastElement() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return reverseOrder ? keyOf(map.head) : keyOf(map.tail);
            }

            K const& lastElement() const override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return reverseOrder ? keyOf(map.head) : keyOf(map.tail);
            }

            K& removeFirst() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return (!reverseOrder ? map.pollFirstEntry() : map.pollLastEntry()).getKey();
            }

            K& removeLast() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return (reverseOrder ? map.pollFirstEntry() : map.pollLastEntry()).getKey();
            }

            gbool contains(K const& val) const override { return map.containsKey(val); }

            Array<K> toArray() const override {
                gint len = map.size();
                CORE_IGNORE_DEPRECATIONS(Array<K> a = Array<K>(len);)
                gint modCount = map.modCount;
                gint i = 0;
                if (reverseOrder) {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        K& key = keyOf(node);
                        Arrays::fastSet(a, i++, key);
                    }
                } else {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        K& key = keyOf(node);
                        Arrays::fastSet(a, i++, key);
                    }
                }
                return a;
            }

            gbool add(K const& val) override { UnsupportedOperationException().throws($ftrace()); }

            gbool remove(K const& val) override {
                gint size = map.size();
                map.remove(val);
                return map.size() < size;
            }

            gbool addAll(Collection<K> const& c) override { UnsupportedOperationException().throws($ftrace()); }

            void clear() override { map.clear(); }

            ~KeySet() override { detachKeys(map, reverseOrder); }
        };

        template <class K, class V>
        class LinkedHashMap<K, V>::Values final : public SequencedCollection<V> {
            LinkedHashMap<K, V>& map;
            gbool reverseOrder = false;

        public:
            Values(LinkedHashMap<K, V> const& map) :
                map((LinkedHashMap&) map) {}

            Values(LinkedHashMap<K, V>& map, gbool reverseOrder) :
                map(map),
                reverseOrder(reverseOrder) {}

            gint size() const override { return map.size(); }

            gbool isEmpty() const override { return map.isEmpty(); }

            Iterator<V>& iterator() override {
                class Itr final : public Iterator<V> {
                    LinkedHashMap<K, V>& map;
                    LINKEDNODE cursor = null;
                    LINKEDNODE last = null;
                    gint modCount = 0;
                    gbool reverseOrder;

                public:
                    Itr(LinkedHashMap<K, V>& map, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? map.head : map.tail;
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    V&next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = !reverseOrder ? nextOf(cursor) : prevOf(cursor);
                        return valueOf(last);
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        LINKEDNODE node = last;
                        last = null;
                        map.deleteNode(hashOf(node), keyOf(node), null, false, false);
                        modCount = map.modCount;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && reverseOrder == itr.reverseOrder && cursor == itr.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(map, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& iterator() const override {
                class Itr final : public Iterator2<V> {
                    LinkedHashMap<K, V>& map;
                    LINKEDNODE cursor = null;
                    LINKEDNODE last = null;
                    gint modCount = 0;
                    gbool reverseOrder;

                public:
                    Itr(LinkedHashMap<K, V>& map, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? map.head : map.tail;
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    V const&next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = !reverseOrder ? nextOf(cursor) : prevOf(cursor);
                        return valueOf(last);
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && reverseOrder == itr.reverseOrder && cursor == itr.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(map, reverseOrder);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<V> const& reversed() const override {
                try {
                    return !reverseOrder ? map.reversed().sequencedValues() : map.sequencedValues();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<V>& reversed() override {
                try {
                    return !reverseOrder ? map.reversed().sequencedValues() : map.sequencedValues();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<V&> const& action) override {
                gint modCount = map.modCount;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        action.accept(value);
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        action.accept(value);
                    }
                }
            }

            void forEach(function::Consumer<V> const& action) const override {
                gint modCount = map.modCount;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        action.accept(value);
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        action.accept(value);
                    }
                }
            }

            gbool contains(V const& val) const override {
                gint modCount = map.modCount;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        if (val == value)
                            return true;
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        if (val == value)
                            return true;
                    }
                }
                return false;
            }

            Array<V> toArray() const override {
                gint len = map.size();
                CORE_IGNORE_DEPRECATIONS(Array<V> a = Array<V>(len);)
                gint modCount = map.modCount;
                gint i = 0;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        V& value = valueOf(node);
                        Arrays::fastSet(a, i++, value);
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        V& value = valueOf(node);
                        Arrays::fastSet(a, i++, value);
                    }
                }
                return a;
            }

            gbool add(V const& val) override { UnsupportedOperationException().throws($ftrace()); }

            void clear() override { map.clear(); }

            void addFirst(V const& val) override { UnsupportedOperationException().throws($ftrace()); }

            void addLast(V const& val) override { UnsupportedOperationException().throws($ftrace()); }

            V& firstElement() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return !reverseOrder ? valueOf(map.head) : valueOf(map.tail);
            }

            V const& firstElement() const override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return !reverseOrder ? valueOf(map.head) : valueOf(map.tail);
            }

            V& lastElement() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return reverseOrder ? valueOf(map.head) : valueOf(map.tail);
            }

            V const& lastElement() const override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return reverseOrder ? valueOf(map.head) : valueOf(map.tail);
            }

            V& removeFirst() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return (!reverseOrder ? map.pollFirstEntry() : map.pollLastEntry()).getValue();
            }

            V& removeLast() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return (reverseOrder ? map.pollFirstEntry() : map.pollLastEntry()).getValue();
            }

            gbool remove(V const& val) override {
                gint modCount = map.modCount;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        if (val == value) {
                            map.deleteNode(hashOf(node), keyOf(node), null, false, false);
                            return true;
                        }
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        if (val == value) {
                            map.deleteNode(hashOf(node), keyOf(node), null, false, false);
                            return true;
                        }
                    }
                }
                return false;
            }

            ~Values() override { detachVals(map, reverseOrder); }
        };

        template <class K, class V>
        class LinkedHashMap<K, V>::EntrySet final : public SequencedSet<Entry> {
            LinkedHashMap<K, V>& map;
            gbool reverseOrder = false;

        public:
            EntrySet(LinkedHashMap<K, V> const& map, gbool reverseOrder)
                : map((LinkedHashMap&) map), reverseOrder(reverseOrder) {}

            gint size() const override { return map.size(); }

            Iterator<Entry>& iterator() override {
                class Itr final : public Iterator<Entry> {
                    LinkedHashMap<K, V>& map;
                    LINKEDNODE cursor = null;
                    LINKEDNODE last = null;
                    gint modCount = 0;
                    gbool reverseOrder = false;

                public:
                    Itr(LinkedHashMap<K, V>& map, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? map.head : map.tail;
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    Entry&next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = !reverseOrder ? nextOf(cursor) : prevOf(cursor);
                        return *last;
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        LINKEDNODE node = last;
                        last = null;
                        map.deleteNode(hashOf(node), keyOf(node), null, false, false);
                        modCount = map.modCount;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && reverseOrder == itr.reverseOrder && cursor == itr.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(map, reverseOrder); //
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry>& iterator() const override {
                class Itr final : public Iterator2<Entry> {
                    LinkedHashMap<K, V> const& map;
                    LINKEDNODE cursor = null;
                    LINKEDNODE last = null;
                    gint modCount = 0;
                    gbool reverseOrder = false;

                public:
                    Itr(LinkedHashMap<K, V> const& map, gbool reverseOrder) :
                        map(map),
                        reverseOrder(reverseOrder) {
                        cursor = !reverseOrder ? map.head : map.tail;
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    Entry const&next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = !reverseOrder ? nextOf(cursor) : prevOf(cursor);
                        return *last;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && reverseOrder == itr.reverseOrder && cursor == itr.cursor;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(map, reverseOrder); //
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<Entry> const& reversed() const override {
                try {
                    return !reverseOrder
                               ? map.reversed().sequencedEntrySet()
                               : map.sequencedEntrySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<Entry>& reversed() override {
                try {
                    return !reverseOrder
                               ? map.reversed().sequencedEntrySet()
                               : map.sequencedEntrySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<Entry&> const& action) override {
                gint modCount = map.modCount;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        action.accept(entry);
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        action.accept(entry);
                    }
                }
            }

            void forEach(function::Consumer<Entry> const& action) const override {
                gint modCount = map.modCount;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        action.accept(entry);
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        action.accept(entry);
                    }
                }
            }

            void addFirst(Entry const& val) override { UnsupportedOperationException().throws($ftrace()); }

            void addLast(Entry const& val) override { UnsupportedOperationException().throws($ftrace()); }

            Entry& firstElement() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return !reverseOrder ? map.firstEntry() : map.lastEntry();
            }

            Entry const& firstElement() const override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return !reverseOrder ? map.firstEntry() : map.lastEntry();
            }

            Entry& lastElement() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return reverseOrder ? map.firstEntry() : map.lastEntry();
            }

            Entry const& lastElement() const override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return reverseOrder ? map.firstEntry() : map.lastEntry();
            }

            Entry& removeFirst() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return !reverseOrder ? map.pollFirstEntry() : map.pollLastEntry();
            }

            Entry& removeLast() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return reverseOrder ? map.pollFirstEntry() : map.pollLastEntry();
            }

            gbool isEmpty() const override { return map.isEmpty(); }

            gbool contains(Entry const& val) const override {
                gint modCount = map.modCount;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        if (val == entry)
                            return true;
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        if (val == entry)
                            return true;
                    }
                }
                return false;
            }

            Array<Entry> toArray() const override {
                gint len = map.size();
                gint modCount = map.modCount;
                CORE_IGNORE_DEPRECATIONS(Array<Entry> a = Array<Entry>(len);)
                gint i = 0;
                if (!reverseOrder) {
                    for (LINKEDNODE node = map.head; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        Arrays::fastSet(a, i++, entry);
                    }
                } else {
                    for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        Arrays::fastSet(a, i++, entry);
                    }
                }
                return a;
            }

            gbool add(Entry const& val) override { UnsupportedOperationException().throws($ftrace()); }

            gbool remove(Entry const& val) override { return map.remove(val.getKey(), val.getValue()); }

            void clear() override { map.clear(); }

            ~EntrySet() override { detachEntries(map, reverseOrder); }
        };

        template <class K, class V>
        class LinkedHashMap<K, V>::Reverse final : public SequencedMap {
            LinkedHashMap<K, V>& map;

        public:
            Reverse(LinkedHashMap<K, V> const& map) : map((LinkedHashMap&) map) {}

            gint size() const override { return map.size(); }

            gbool isEmpty() const override { return map.isEmpty(); }

            gbool containsKey(K const& key) const override {
                try {
                    return map.containsKey(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsValue(V const& value) const override {
                gint modCount = map.modCount;
                for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                    if (modCount != map.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    V& v = valueOf(node);
                    if (value == v)
                        return true;
                }
                return false;
            }

            V& get(K const& key) override {
                try { return map.get(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V const& get(K const& key) const override {
                try { return map.get(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& put(K const& key, V const& value) override {
                try { return map.put(key, value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& remove(K const& key) override {
                try { return map.remove(key); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override { try { map.clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); } }

            Set<K>& keySet() override {
                try { return sequencedKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Set<K> const& keySet() const override {
                try { return sequencedKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Collection<V>& values() override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Collection<V> const& values() const override {
                try { return sequencedValues(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Set<Entry>& entrySet() override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Set<Entry> const& entrySet() const override {
                try { return sequencedEntrySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
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

            void foreach(function::BiConsumer<K&, V&> const& action) override {
                gint modCount = map.modCount;
                for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                    if (modCount != map.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    K& key = keyOf(node);
                    V& value = valueOf(node);
                    action.accept(key, value);
                }
            }

            void foreach(function::BiConsumer<K, V> const& action) const override {
                gint modCount = map.modCount;
                for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                    if (modCount != map.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    K& key = keyOf(node);
                    V& value = valueOf(node);
                    action.accept(key, value);
                }
            }

            void replaceAll(function::BiFunction<K&, V&, V> const& function) override {
                gint modCount = map.modCount;
                for (LINKEDNODE node = map.tail; node != null; node = prevOf(node)) {
                    if (modCount != map.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    K& key = keyOf(node);
                    V& value = valueOf(node);
                    try {
                        node->setValue(UNSAFE::copyInstance(function.apply(key, value)));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            }

            Object& putIfAbsent(K const& key, V const& value) override {
                try { return map.putIfAbsent(key, value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(K const& key, V const& value) override {
                try { return map.remove(key, value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool replace(K const& key, V const& oldValue, V const& newValue) override {
                try {
                    return map.replace(key, oldValue, newValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& replace(K const& key, V const& value) override {
                try { return map.replace(key, value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedMap& reversed() override { return map; }

            SequencedMap const& reversed() const override { return map; }

            Entry& firstEntry() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return map.lastEntry();
            }

            Entry const& firstEntry() const override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return map.lastEntry();
            }

            Entry& lastEntry() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return map.firstEntry();
            }

            Entry const& lastEntry() const override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return map.firstEntry();
            }

            Entry& pollFirstEntry() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return map.pollLastEntry();
            }

            Entry& pollLastEntry() override {
                if (map.isEmpty())
                    NoSuchElementException().throws($ftrace());
                return map.pollFirstEntry();
            }

            Object& putFirst(K const& k, V const& v) override {
                try { return map.putLast(k, v); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& putLast(K const& k, V const& v) override {
                try { return map.putFirst(k, v); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<K>& sequencedKeySet() override {
                SequencedKeys view = CORE_DCAST(SequencedKeys, keys);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<KeySet>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    keys = view;
                }
                return *view;
            }

            SequencedSet<K> const& sequencedKeySet() const override {
                SequencedKeys view = CORE_DCAST(SequencedKeys, keys);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<KeySet>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    keys = view;
                }
                return *view;
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

            SequencedCollection<V> const& sequencedValues() const override {
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

            SequencedSet<Entry> const& sequencedEntrySet() const override {
                SequencedEntries view = CORE_DCAST(SequencedEntries, entries);
                if (!view) {
                    try {
                        view = &UNSAFE::newInstance<EntrySet>(map, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    entries = view;
                }
                return *view;
            }

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
            // ::::::::::::::::::::: Reverse views Fields :::::::::::::::::::::::::::::

            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);
            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
        };
    } // namespace util
} // namespace core

#endif // CORE24_LINKEDHASHMAP_H
