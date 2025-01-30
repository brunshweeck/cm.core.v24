//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_HASHMAP_H
#define CORE24_HASHMAP_H

#include <core/util/Map.h>

namespace core {
    namespace util {
        /**
         * Hash table based implementation of the @c Map interface.  This
         * implementation provides all the optional map operations, and permits
         * @c null values and the @c null key. This class makes no guarantees as to
         * the order of the map; in particular, it does not guarantee that the order
         * will remain constant over time.
         *
         * <p>
         * This implementation provides constant-time performance for the basic
         * operations (@c get and @c put), assuming the hash function
         * disperses the elements properly among the buckets.  Iteration over
         * collection views requires time proportional to the "capacity" of the
         * @c HashMap instance (the number of buckets) plus its size (the number
         * of key-value mappings).  Thus, it's very important not to set the initial
         * capacity too high (or the load factor too low) if iteration performance is
         * important.
         * </p>
         * <p>
         * An instance of @c HashMap has two parameters that affect its
         * performance: <i>initial capacity</i> and <i>load factor</i>.  The
         * <i>capacity</i> is the number of buckets in the hash table, and the initial
         * capacity is simply the capacity at the time the hash table is created.  The
         * <i>load factor</i> is a measure of how full the hash table is allowed to
         * get before its capacity is automatically increased.  When the number of
         * entries in the hash table exceeds the product of the load factor and the
         * current capacity, the hash table is <i>rehashed</i> (that is, internal data
         * structures are rebuilt) so that the hash table has approximately twice the
         * number of buckets.
         * </p>
         * <p>
         * As a general rule, the default load factor (.75) offers a good
         * tradeoff between time and space costs.  Higher values decrease the
         * space overhead but increase the lookup cost (reflected in most of
         * the operations of the @c HashMap class, including
         * @c get and @c put).  The expected number of entries in
         * the map and its load factor should be taken into account when
         * setting its initial capacity, to minimize the number of
         * rehash operations.  If the initial capacity is greater than the
         * maximum number of entries divided by the load factor, no rehash
         * operations will ever occur.
         * </p>
         * <p>
         * If many mappings are to be stored in a @c HashMap
         * instance, creating it with a sufficiently large capacity will allow
         * the mappings to be stored more efficiently than letting it perform
         * automatic rehashing as needed to grow the table.  Note that using
         * many keys with the same @c hash() is a sure way to slow
         * down performance of any hash table. To ameliorate impact, when keys
         * are @b Comparable, this class may use comparison order among
         * keys to help break ties.
         * </p>
         * <p>
         * <b>Note that this implementation is not synchronized.</b>
         * If multiple threads access a hash map concurrently, and at least one of
         * the threads modifies the map structurally, it <i>must</i> be
         * synchronized externally.  (A structural modification is any operation
         * that adds or deletes one or more mappings; merely changing the value
         * associated with a key that an instance already contains is not a
         * structural modification.)  This is typically accomplished by
         * synchronizing on some object that naturally encapsulates the map.
         *
         * If no such object exists, the map should be "wrapped" using the
         * @b Collections.synchronizedMap method.
         * This is best done at creation time, to prevent accidental
         * unsynchronized access to the map:
         * @code
         *   Map m = Collections.synchronizedMap(UNSAFE::newInstance<HashMap>(...));
         * @endcode
         * </p>
         * <p>
         * The iterators returned by all of this class's "collection view methods"
         * are <i>fail-fast</i>: if the map is structurally modified at any time after
         * the iterator is created, in any way except through the iterator's own
         * @b remove method, the iterator will throw a
         * @b ConcurrentModificationException.  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the
         * future.
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
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         *
         * @see     Object::hash()
         * @see     Collection
         * @see     Map
         * @see     TreeMap
         * @see     Hashtable
         */
        template <class K, class V>
        class HashMap : public virtual Map<K, V> {
        protected:
            CORE_ALIAS(Entry, util::Entry<K, V>);
            CORE_ALIAS(Map, util::Map<K, V>);
            CORE_ALIAS(SequencedMap, util::SequencedMap<K, V>);

            CORE_ALIAS(KEY, typename Class<K>::Pointer);
            CORE_ALIAS(VALUE, typename Class<V>::Pointer);
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(Keys, typename Class<Set<K>>::Pointer);
            CORE_ALIAS(Vals, typename Class<Collection<V>>::Pointer);
            CORE_ALIAS(Entries, typename Class<Set<Entry>>::Pointer);
            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_TEMPLATES_AS_FRIEND(LinkedHashMap, Key, class Value);

            class Node;
            class LinkedNode;
            class TreeNode;

            CORE_ALIAS(NODE, typename Class<Node>::Pointer);
            CORE_ALIAS(LINKEDNODE, typename Class<LinkedNode>::Pointer);
            CORE_ALIAS(TREENODE, typename Class<TreeNode>::Pointer);
            CORE_ALIAS(TABLE, typename Class<NODE>::Pointer);

        private:
            /**
             * Implementation notes.
             * <p>
             * This map usually acts as a binned (bucketed) hash table, but
             * when bins get too large, they are transformed into bins of
             * TreeNodes, each structured similarly to those in
             * @b TreeMap. Most methods try to use normal bins, but
             * relay to TreeNode methods when applicable (simply by checking
             * instanceof a node).  Bins of TreeNodes may be traversed and
             * used like any others, but additionally support faster lookup
             * when overpopulated. However, since the vast majority of bins in
             * normal use are not overpopulated, checking for existence of
             * tree bins may be delayed in the course of table methods.
             * </p>
             * <p>
             * Tree bins (i.e., bins whose elements are all TreeNodes) are
             * ordered primarily by hashCode, but in the case of ties, if two
             * elements are of the same "class C implements Comparable<C>",
             * type then their compareTo method is used for ordering. (We
             * conservatively check generic types via reflection to validate
             * this -- see method @c compare).  The added complexity
             * of tree bins is worthwhile in providing worst-case O(log n)
             * operations when keys either have distinct hashes or are
             * orderable, Thus, performance degrades gracefully under
             * accidental or malicious usages in which hashCode() methods
             * return values that are poorly distributed, as well as those in
             * which many keys share a hashCode, so long as they are also
             * Comparable. (If neither of these apply, we may waste about a
             * factor of two in time and space compared to taking no
             * precautions. But the only known cases stem from poor user
             * programming practices that are already so slow that this makes
             * little difference.)
             * </p>
             * <p>
             * Because TreeNodes are about twice the size of regular nodes, we
             * use them only when bins contain enough nodes to warrant use
             * (see TREEIFY_THRESHOLD). And when they become too small (due to
             * removal or resizing) they are converted back to plain bins.  In
             * usages with well-distributed user hashCodes, tree bins are
             * rarely used.  Ideally, under random hashCodes, the frequency of
             * nodes in bins follows a Poisson distribution
             * (http://en.wikipedia.org/wiki/Poisson_distribution) with a
             * parameter of about 0.5 on average for the default resizing
             * threshold of 0.75, although with a large variance because of
             * resizing granularity. Ignoring variance, the expected
             * occurrences of list size k are (exp(-0.5) * pow(0.5, k) /
             * factorial(k)). The first values are:
             * </p>
             * <p>
             * 0:    0.60653066 <br/>
             * 1:    0.30326533 <br/>
             * 2:    0.07581633 <br/>
             * 3:    0.01263606 <br/>
             * 4:    0.00157952 <br/>
             * 5:    0.00015795 <br/>
             * 6:    0.00001316 <br/>
             * 7:    0.00000094 <br/>
             * 8:    0.00000006 <br/>
             * more: less than 1 in ten million
             * </p>
             * <p>
             * The root of a tree bin is normally its first node.  However,
             * sometimes (currently only upon Iterator::remove), the root might
             * be elsewhere, but can be recovered following parent links
             * (method TreeNode::root()).
             * </p>
             * <p>
             * All applicable internal methods accept a hash code as an
             * argument (as normally supplied from a public method), allowing
             * them to call each other without recomputing user hashCodes.
             * Most internal methods also accept a "tab" argument, that is
             * normally the current table, but may be a new or old one when
             * resizing or converting.
             * </p>
             * <p>
             * When bin lists are treeified, split, or untreeified, we keep
             * them in the same relative access/traversal order (i.e., field
             * Node::next) to better preserve locality, and to slightly
             * simplify handling of splits and traversals that invoke
             * iterator.remove. When using comparators on insertion, to keep a
             * total ordering (or as close as is required here) across
             * rebalancings, we compare classes and identityHashCodes as
             * tie-breakers.
             * </p>
             * <p>
             * The use and transitions among plain vs tree modes is
             * complicated by the existence of subclass LinkedHashMap. See
             * below for hook methods defined to be invoked upon insertion,
             * removal and access that allow LinkedHashMap internals to
             * otherwise remain independent of these mechanics. (This also
             * requires that a map instance be passed to some utility methods
             * that may create new nodes.)
             * </p>
             * <p>
             * The concurrent-programming-like SSA-based coding style helps
             * avoid aliasing errors amid all the twisty pointer operations.
             * </p>
             */

            //
            /**
             * The default initial capacity - MUST be a power of two.
             */
            static CORE_FAST gint DEFAULT_INITIAL_CAPACITY = 1 << 4;

            /**
             * The maximum capacity, used if a higher value is implicitly specified
             * by either of the constructors with arguments.
             * Must be a power of two <= 1 << 30.
             */
            static CORE_FAST gint MAXIMUM_CAPACITY = 1 << 30;

            /**
             * The load factor used when none specified in constructor.
             */
            static CORE_FAST gfloat DEFAULT_LOAD_FACTOR = 0.75F;

            /**
             * The bin count threshold for using a tree rather than list for a
             * bin.  Bins are converted to trees when adding an element to a
             * bin with at least this many nodes. The value must be greater
             * than 2 and should be at least 8 to mesh with assumptions in
             * tree removal about conversion back to plain bins upon
             * shrinkage.
             */
            static CORE_FAST gint TREEIFY_THRESHOLD = 8;

            /**
             * The bin count threshold for untreeifying a (split) bin during a
             * resize operation. Should be less than TREEIFY_THRESHOLD, and at
             * most 6 to mesh with shrinkage detection under removal.
             */
            static CORE_FAST gint UNTREEIFY_THRESHOLD = 6;

            /**
             * The smallest table capacity for which bins may be treeified.
             * (Otherwise the table is resized if too many nodes in a bin.)
             * Should be at least 4 * TREEIFY_THRESHOLD to avoid conflicts
             * between resizing and treeification thresholds.
             */
            static CORE_FAST gint MIN_TREEIFY_CAPACITY = 64;

            /* ---------------- Fields -------------- */

            /**
             * The table, initialized on first use, and resized as
             * necessary. When allocated, length is always a power of two.
             * (We also tolerate length zero in some operations to allow
             * bootstrapping mechanics that are currently not needed.)
             */
            Array<Node> table;

            /**
             * The number of key-value mappings contained in this map.
             */
            gint count = 0;

        protected:
            /**
             * The number of times this HashMap has been structurally modified
             * Structural modifications are those that change the number of mappings in
             * the HashMap or otherwise modify its internal structure (e.g.,
             * rehash).  This field is used to make iterators on Collection-views of
             * the HashMap fail-fast.  (See ConcurrentModificationException).
             */
            gint mutable modCount = 0;

        private:
            /**
             * The next size value at which to resize (capacity * load factor).
             */
            gint threshold = 0;

            /**
             * The load factor for the hash table.
             */
            gfloat loadFactor = 0.0F;

        public:
            /* ---------------- Public operations -------------- */

            /**
             * Constructs an empty @c HashMap with the specified initial
             * capacity and load factor.
             *
             * @param  initialCapacity the initial capacity
             * @param  loadFactor      the load factor
             * @throws IllegalArgumentException if the initial capacity is negative
             *         or the load factor is non-positive
             */
            CORE_EXPLICIT HashMap(gint initialCapacity, gfloat loadFactor) {
                if (initialCapacity < 0)
                    IllegalArgumentException("Illegal initial capacity: "_S + initialCapacity)
                            .throws($ftrace());
                if (initialCapacity > MAXIMUM_CAPACITY)
                    initialCapacity = MAXIMUM_CAPACITY;
                if (loadFactor <= 0 || Float::isNaN(loadFactor))
                    IllegalArgumentException("Illegal load factor: "_S + loadFactor)
                            .throws($ftrace());
                HashMap::loadFactor = loadFactor;
                HashMap::threshold = tableSizeFor(initialCapacity);
            }

            /**
             * Constructs an empty @c HashMap with the specified initial
             * capacity and the default load factor (0.75).
             *
             * @param  initialCapacity the initial capacity.
             * @throws IllegalArgumentException if the initial capacity is negative.
             */
            CORE_EXPLICIT HashMap(gint initialCapacity) : HashMap(initialCapacity, DEFAULT_INITIAL_CAPACITY) {}

            /**
             * Constructs an empty @c HashMap with the default initial capacity
             * (16) and the default load factor (0.75).
             */
            CORE_IMPLICIT HashMap() {
                loadFactor = DEFAULT_LOAD_FACTOR;
            }

            /**
             * Constructs a new @c HashMap with the same mappings as the
             * specified @c Map.  The @c HashMap is created with
             * default load factor (0.75) and an initial capacity sufficient to
             * hold the mappings in the specified @c Map.
             *
             * @param m the map whose mappings are to be placed in this map
             */
            CORE_EXPLICIT HashMap(Map const& m) {
                loadFactor = DEFAULT_LOAD_FACTOR;
                putNodes(m, false);
            }

            template <class X, class Y,
                      ClassOf(1)::OnlyIf<Class<K>::template isSuper<X>() && Class<V>::template isSuper<Y>()>  = 1>
            CORE_EXPLICIT HashMap(util::Map<X, Y> const& m) {
                loadFactor = DEFAULT_LOAD_FACTOR;
                putNodes(m, false);
            }

            HashMap(HashMap const& other)
                : table(other.table), count(other.count), threshold(other.threshold), loadFactor(other.loadFactor) {}

            HashMap(HashMap&& other) CORE_NOTHROW {
                other.modCount += 1;
                UNSAFE::swapValues(loadFactor, other.loadFactor);
                UNSAFE::swapValues(threshold, other.threshold);
                table = UNSAFE::moveInstance(other.table);
                UNSAFE::swapValues(count, other.count);
            }

            HashMap& operator=(HashMap const& other) {
                if (this != &other) {
                    modCount += 1;
                    HashMap tmp(other);
                    try { initialize(tmp); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
                return *this;
            }

            HashMap& operator=(HashMap&& other) CORE_NOTHROW {
                if (this != &other) {
                    modCount += 1;
                    other.modCount -= 1;
                    initialize(other);
                }
                return *this;
            }

            ~HashMap() override {
                HashMap::clear();

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

            /**
             * Returns the number of key-value mappings in this map.
             *
             * @return the number of key-value mappings in this map
             */
            gint size() const override {
                return count;
            }

            /**
             * Returns @c true if this map contains no key-value mappings.
             *
             * @return @c true if this map contains no key-value mappings
             */
            gbool isEmpty() const override {
                return count == 0;
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or thrown @c MissingKeyException if this map contains no mapping for the key.
             *
             * <p>
             * More formally, if this map contains a mapping from a key
             * @c k to a value @c v such that @c key.equals(k),
             * then this method returns @c v; otherwise it throws
             * @c MissingKeyException.  (There can be at most one such mapping.)
             * </p>
             *
             * @see put(Object, Object)
             */
            V& get(K const& key) override {
                NODE x = exactNode(key);
                if (x)
                    return *x->value;
                MissingKeyException(key).throws($ftrace());
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or thrown @c MissingKeyException if this map contains no mapping for the key.
             *
             * <p>
             * More formally, if this map contains a mapping from a key
             * @c k to a value @c v such that @c key.equals(k),
             * then this method returns @c v; otherwise it throws
             * @c MissingKeyException.  (There can be at most one such mapping.)
             * </p>
             *
             * @see put(Object, Object)
             */
            V const& get(K const& key) const override {
                NODE x = exactNode(key);
                if (x)
                    return *x->value;
                MissingKeyException(key).throws($ftrace());
            }

            /**
             * Returns @c true if this map contains a mapping for the
             * specified key.
             *
             * @param   key   The key whose presence in this map is to be tested
             * @return @c true if this map contains a mapping for the specified
             * key.
             */
            gbool containsKey(K const& key) const override {
                return exactNode(key);
            }

            /**
             * Associates the specified value with the specified key in this map.
             * If the map previously contained a mapping for the key, the old
             * value is replaced.
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with @c key, or
             *         @c null if there was no mapping for @c key.
             *         (A @c null return can also indicate that the map
             *         previously associated @c null with @c key.)
             */
            Object& put(K const& key, V const& value) override {
                return putNode(hashKey(key), key, value, false, true);
            }

            /**
             * Copies all the mappings from the specified map to this map.
             * These mappings will replace any mappings that this map had for
             * any of the keys currently in the specified map.
             *
             * @param m mappings to be stored in this map
             */
            void putAll(const Map& m) override {
                putNodes(m, true);
            }

            /**
             * Removes the mapping for the specified key from this map if present.
             *
             * @param  key key whose mapping is to be removed from the map
             * @return the previous value associated with @c key, or
             *         thrown @c MissingKeyException if there was no mapping for @c key.
             */
            V& remove(K const& key) override {
                NODE x = deleteNode(hashKey(key), key, null, false, true);
                if (!x)
                    return valueOf(x);
                MissingKeyException(key).throws($ftrace());
            }

            /**
             * Removes all the mappings from this map.
             * The map will be empty after this call returns.
             */
            void clear() override {
                count = 0;
                modCount += 1;

                gint len = table.length();
                gint index = 0;
                gint modCount2 = modCount;
                for (TABLE t = tableOf(table); index < len; index++) {
                    if (modCount2 != modCount)
                        ConcurrentModificationException().throws($ftrace());

                    t[index] = null;
                }
            }

            /**
             * Returns @c true if this map maps one or more keys to the
             * specified value.
             *
             * @param value value whose presence in this map is to be tested
             * @return @c true if this map maps one or more keys to the
             *         specified value
             */
            gbool containsValue(V const& value) const override {
                gint modCount2 = modCount;
                gint len = table.length();
                gint index = 0;
                for (TABLE t = tableOf(table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        if (modCount2 != modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& v = valueOf(node);
                        if (value == v)
                            return true;
                    }
                }
                return false;
            }

            /**
             * Returns a @b Set view of the keys contained in this map.
             * The set is backed by the map, so changes to the map are
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
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            /**
             * Returns a @b Set view of the keys contained in this map.
             * The set is backed by the map, so changes to the map are
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
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            /**
             * Returns a @b Collection view of the values contained in this map.
             * The collection is backed by the map, so changes to the map are
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
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            /**
             * Returns a @b Collection view of the values contained in this map.
             * The collection is backed by the map, so changes to the map are
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
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            /**
             * Returns a @b Set view of the mappings contained in this map.
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
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(*this);
                return *entries;
            }

            /**
             * Returns a @b Set view of the mappings contained in this map.
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
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(*this);
                return *entries;
            }

            V& getOrDefault(K const& key, V const& defaultValue) override {
                NODE x = exactNode(key);
                if (x)
                    return valueOf(x);
                try { return UNSAFE::copyInstance(defaultValue); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V const& getOrDefault(K const& key, V const& defaultValue) const override {
                NODE x = exactNode(key);
                if (x)
                    return valueOf(x);

                try { return UNSAFE::copyInstance(defaultValue); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& getOrNull(K const& key) override {
                NODE x = exactNode(key);
                if (x)
                    return valueOf(x);

                return null;
            }

            const Object& getOrNull(K const& key) const override {
                NODE x = exactNode(key);
                if (x)
                    return valueOf(x);

                return null;
            }

            Object& putIfAbsent(K const& key, V const& value) override {
                try {
                    return putNode(hashKey(key), key, value, true, true);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(K const& key, V const& value) override {
                try {
                    return deleteNode(hashKey(key), key, value, true, true);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool replace(K const& key, V const& oldValue, V const& newValue) override {
                NODE x = exactNode(key);
                if (x && oldValue == valueOf(x)) {
                    try {
                        x->value = &UNSAFE::copyInstance(newValue);
                        afterAccess(x);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    return true;
                }
                return false;
            }

            Object& replace(K const& key, V const& value) override {
                NODE x = exactNode(key);
                if (x) {
                    V& oldValue = valueOf(x);
                    try {
                        x->value = &UNSAFE::copyInstance(value);
                        afterAccess(x);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    return oldValue;
                }
                return null;
            }

            void forEach(const function::BiConsumer<K&, V&>& action) override {
                gint modCount2 = modCount;
                gint len = table.length();
                gint index = 0;

                for (TABLE t = tableOf(table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        if (modCount2 != modCount)
                            ConcurrentModificationException().throws($ftrace());

                        K& key = keyOf(node);
                        V& value = valueOf(node);
                        action.accept(key, value);
                    }
                }
            }

            void forEach(const function::BiConsumer<K, V>& action) const override {
                gint modCount2 = modCount;
                gint len = table.length();
                gint index = 0;

                for (TABLE t = tableOf(table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        if (modCount2 != modCount)
                            ConcurrentModificationException().throws($ftrace());

                        K& key = keyOf(node);
                        V& value = valueOf(node);
                        action.accept(key, value);
                    }
                }
            }

            void replaceAll(const function::BiFunction<K&, V&, V>& function) override {
                gint modCount2 = modCount;
                gint len = table.length();
                gint index = 0;

                for (TABLE t = tableOf(table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        if (modCount2 != modCount)
                            ConcurrentModificationException().throws($ftrace());

                        K& key = keyOf(node);
                        V& value = valueOf(node);
                        V& repl = UNSAFE::copyInstance(function.apply(key, value));
                        node->value = &repl;
                    }
                }
            }

            /* ------------------------------------------------------------ */
            // Cloning and serialization

            /**
             * Returns a shallow copy of this @c HashMap instance: the keys and
             * values themselves are not cloned.
             *
             * @return a shallow copy of this map
             */
            Object& clone() const override {
                try {
                    return UNSAFE::newInstance<HashMap>(*this);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        protected:
            // ::::::::::::::::::::::::::: HashMap Views Fields ::::::::::::::::::::::::::::::::::::::

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

            // ::::::::::::::::::::::::::: HashMap Utility ::::::::::::::::::::::::::::::::::::::

            /**
             * Computes key.hashCode() and spreads (XORs) higher bits of hash
             * to lower.  Because the table uses power-of-two masking, sets of
             * hashes that vary only in bits above the current mask will
             * always collide. (Among known examples are sets of Float keys
             * holding consecutive whole numbers in small tables.)  So we
             * apply a transform that spreads the impact of higher bits
             * downward. There is a tradeoff between speed, utility, and
             * quality of bit-spreading. Because many common sets of hashes
             * are already reasonably distributed (so don't benefit from
             * spreading), and because we use trees to handle large sets of
             * collisions in bins, we just XOR some shifted bits in the
             * cheapest possible way to reduce systematic loss-age, as well as
             * to incorporate impact of the highest bits that would otherwise
             * never be used in index calculations because of table bounds.
             */
            static gint hashKey(Object const& key) {
                if (key == null)
                    return 0;
                gint h = key.hash();
                return h ^ CORE_CAST(gint, Integer::toUnsignedLong(h) >> 16);
            }

            /**
             * Returns k.compareTo(x) if x matches kc (k's screened comparable
             * class), else 0.
             */
            static gint compare(Object const& k, Object const& x) {
                if (x == null || !Class<Comparable<K>>::hasInstance(x))
                    return 0;

                return CORE_XCAST(Comparable<K> const, k).compareTo(CORE_XCAST(K const, x));
            }

            /**
             * Returns a power of two size for the given target capacity.
             */
            static gint tableSizeFor(gint c) {
                const int n = CORE_CAST(gint, (-1 + 0U) >> Integer::numberOfLeadingZeros(c - 1));
                return (n < 0) ? 1 : Math::min(n + 1, MAXIMUM_CAPACITY);
            }

            /**
             * Implements Map::get and related methods.
             *
             * @param key the key
             * @return the node, or null if none
             */
            NODE exactNode(K const& key) const {
                TABLE tab = { };
                NODE first = { };
                NODE x = { };
                gint n = 0;
                gint hash = 0;

                if (((tab = tableOf(table))) && (n = table.length()) > 0 &&
                    ((first = tab[(n - 1) & (hash = hashKey(key))]))) {
                    if (hash == hashOf(first) && key == keyOf(first))
                        return first;
                    if ((x = nextOf(first))) {
                        if (isTreeNode(x))
                            return toTreeNode(x)->exactTreeNode(hash, key);
                        do {
                            if (hash == hashOf(x) && key == keyOf(x))
                                return x;
                        } while ((x = nextOf(x)));
                    }
                }
                return null;
            }

            static gint hashOf(NODE x) { return (!x) ? 0 : x->hash; }

            static NODE nextOf(NODE x) { return (!x) ? x : x->next; }

            static K& keyOf(NODE x) { return x->getKey(); }

            static V& valueOf(NODE x) { return x->getValue(); }

            static TABLE tableOf(Array<Node> const& tab) { return CORE_CAST(TABLE, Arrays::array2DirectAccess(tab)); }

            static gbool isTreeNode(NODE x) { return CORE_DCAST(TREENODE, x); }

            static TREENODE toTreeNode(NODE x) { return CORE_DCAST(TREENODE, x); }

            static LINKEDNODE toLinkedNode(NODE x) { return CORE_DCAST(LINKEDNODE, x); }

            static TREENODE rightOf(TREENODE x) { return (!x) ? x : x->right; }

            static TREENODE leftOf(TREENODE x) { return (!x) ? x : x->left; }

            static TREENODE parentOf(TREENODE x) { return (!x) ? x : x->parent; }

            static TREENODE prevOf(TREENODE x) { return (!x) ? x : x->prev; }

            static LINKEDNODE afterOf(LINKEDNODE x) { return (!x) ? x : x->after; }

            static LINKEDNODE beforeOf(LINKEDNODE x) { return (!x) ? x : x->before; }

            /**
             * Implements Map.putAll and Map constructor.
             *
             * @param m the map
             * @param evict false when initially constructing this map, else
             * true (relayed to method afterNodeInsertion).
             */
            template <class X, class Y>
            void putNodes(util::Map<X, Y> const& m, gbool evict) {
                const gint size = m.size();
                if (size > 0) {
                    if (table.isEmpty()) {
                        const gdouble dt = Math::ceil(size / CORE_CAST(gdouble, loadFactor));
                        const gint t = ((dt < CORE_CAST(gdouble, MAXIMUM_CAPACITY))
                                            ? CORE_CAST(gint, dt)
                                            : MAXIMUM_CAPACITY);
                        if (t > threshold)
                            threshold = tableSizeFor(t);
                    } else {
                        // Because of linked-list bucket constraints, we cannot
                        // expand all at once, but can reduce total resize
                        // effort by repeated doubling now vs later
                        while (size > threshold && table.length() < MAXIMUM_CAPACITY)
                            resize();
                    }

                    try {
                        for (util::Entry<X, Y> const& e : m.entrySet()) {
                            K const& key = e.getKey();
                            V const& value = e.getValue();
                            putNode(hashKey(key), key, value, false, evict);
                        }
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            }

            /**
             * Implements Map::put and related methods.
             *
             * @param hash hash for key
             * @param key the key
             * @param value the value to put
             * @param onlyIfAbsent if true, don't change existing value
             * @param evict if false, the table is in creation mode.
             * @return previous value, or null if none
             */
            Object& putNode(gint hash, K const& key, V const& value, gbool onlyIfAbsent, gbool evict) {
                TABLE tab = { };
                NODE p = { };
                gint n = 0;
                gint i = 0;
                if (!((tab = tableOf(table))) || (n = table.length()) == 0) {
                    tab = tableOf(resize());
                    n = table.length();
                }
                if (!((p = tab[i = (n - 1) & hash])))
                    tab[i] = newNode(hash, key, value, null);
                else {
                    NODE x = { };
                    if (hash == hashOf(p) && key == keyOf(p))
                        x = p;
                    else if (isTreeNode(p))
                        x = toTreeNode(p)->putTreeNode(*this, table, hash, key, value);
                    else {
                        for (gint binCount = 0;; ++binCount) {
                            if (!((x = nextOf(p)))) {
                                p->next = newNode(hash, key, value, null);
                                if (binCount >= TREEIFY_THRESHOLD - 1) // -1 for 1st
                                    treeifyBin(table, hash);
                                break;
                            }
                            if (hash == hashOf(x) && key == keyOf(x))
                                break;
                            p = x;
                        }
                    }
                    if (x) {
                        // existing mapping for key
                        V& oldValue = valueOf(x);
                        if (!onlyIfAbsent)
                            x->value = &UNSAFE::copyInstance(value);
                        afterAccess(x);
                        return oldValue;
                    }
                }
                modCount += 1;
                if (++count > threshold)
                    resize();
                afterInsertion(evict);
                return null;
            }

            /**
             * Implements Map::remove and related methods.
             *
             * @param hash hash for key
             * @param key the key
             * @param value the value to match if matchValue, else ignored
             * @param matchValue if true only remove if value is equal
             * @param movable if false do not move other nodes while removing
             * @return the node, or null if none
             */
            NODE deleteNode(gint hash, K const& key, Object const& value, gbool matchValue, gbool movable) {
                TABLE tab = { };
                NODE p;
                int n;
                int index;
                if (((tab = tableOf(table))) && (n = table.length()) > 0 &&
                    ((p = tab[index = (n - 1) & hash]))) {
                    NODE node = { };
                    NODE e = { };
                    if (hash == hashOf(p) && key == keyOf(p))
                        node = p;
                    else if ((e = nextOf(p))) {
                        if (isTreeNode(p))
                            node = toTreeNode(p)->exactTreeNode(hash, key);
                        else {
                            do {
                                if (hash == hashOf(e) && key == keyOf(e)) {
                                    node = e;
                                    break;
                                }
                                p = e;
                            } while ((e = nextOf(e)));
                        }
                    }
                    if (node && (!matchValue || value == valueOf(node))) {
                        if (isTreeNode(node))
                            toTreeNode(node)->deleteTreeNode(*this, table, movable);
                        else if (node == p)
                            tab[index] = nextOf(node);
                        else
                            p->next = nextOf(node);
                        ++modCount;
                        --count;
                        afterDeletion(node);
                        return node;
                    }
                }
                return null;
            }

        private:
            /**
             * Initializes or doubles table size.  If null, allocates in
             * accord with initial capacity target held in field threshold.
             * Otherwise, because we are using power-of-two expansion, the
             * elements from each bin must either stay at same index, or move
             * with a power of two offset in the new table.
             *
             * @return the table
             */
            Array<Node>& resize() {
                Array<Node>& tab = table;
                gint oldCap = tab.length();
                const gint oldThr = threshold;
                gint newCap = 0;
                gint newThr = 0;
                if (oldCap > 0) {
                    if (oldCap >= MAXIMUM_CAPACITY) {
                        threshold = Integer::MAX_VALUE;
                        return tab;
                    }
                    if ((newCap = oldCap << 1) < MAXIMUM_CAPACITY && oldCap > DEFAULT_INITIAL_CAPACITY)
                        newThr = oldThr << 1; // double threshold.
                } else if (oldThr > 0) {
                    // initial capacity was placed in threshold
                    newCap = oldThr;
                } else {
                    // zero initial threshold signifies using defaults
                    newCap = DEFAULT_INITIAL_CAPACITY;
                    newThr = CORE_CAST(gint, DEFAULT_LOAD_FACTOR * DEFAULT_INITIAL_CAPACITY);
                }
                if (newThr == 0) {
                    gfloat ft = CORE_CAST(gfloat, newCap) * loadFactor;
                    newThr = newCap < MAXIMUM_CAPACITY && ft < CORE_CAST(gfloat, MAXIMUM_CAPACITY)
                                 ? CORE_CAST(gint, ft)
                                 : Integer::MAX_VALUE;
                }
                threshold = newThr;
                CORE_IGNORE_DEPRECATIONS(Array<Node> newTable = Array<Node>(newCap);)
                TABLE oldTab = tableOf(tab);
                TABLE newTab = tableOf(newTable);
                if (oldTab) {
                    for (gint j = 0; j < oldCap; j++) {
                        NODE x = { };
                        if ((x = oldTab[j])) {
                            oldTab[j] = { };
                            if (!nextOf(x))
                                newTab[hashOf(x) & (newCap - 1)] = x;
                            else if (isTreeNode(x))
                                toTreeNode(x)->split(*this, newTable, j, oldCap);
                            else {
                                // Preserve order
                                NODE loHead = { };
                                NODE loTail = { };
                                NODE hiHead = { };
                                NODE hiTail = { };
                                NODE next = { };
                                do {
                                    next = nextOf(x);
                                    if ((hashOf(x) & oldCap) == 0) {
                                        (!loTail ? loHead : loTail->next) = x;
                                        loTail = x;
                                    } else {
                                        (!hiTail ? hiHead : hiTail->next) = x;
                                        hiTail = x;
                                    }
                                } while ((x = next));
                                if (loTail) {
                                    loTail->next = { };
                                    newTab[j] = loHead;
                                }
                                if (hiTail) {
                                    hiTail->next = { };
                                    newTab[j + oldCap] = hiHead;
                                }
                            }
                        }
                    }
                }
                return (table = UNSAFE::moveInstance(newTable));
            }

            /**
             * Replaces all linked nodes in bin at index for given hash unless
             * table is too small, in which case resizes instead.
             */
            void treeifyBin(Array<Node>& tab, gint hash) {
                gint n = 0;
                gint index = 0;
                NODE x = { };
                TABLE t = tableOf(tab);
                if ((!t) || (n = tab.length()) < MIN_TREEIFY_CAPACITY)
                    resize();
                else if ((x = t[index = (n - 1) & hash])) {
                    TREENODE hd = { };
                    TREENODE tl = { };
                    do {
                        TREENODE p = convert2TreeNode(x, null);
                        if (!tl)
                            hd = p;
                        else {
                            p->prev = tl;
                            tl->next = p;
                        }
                        tl = p;
                    } while ((x = nextOf(x)));
                    if ((t[index] = hd))
                        hd->treeify(tab);
                }
            }

        protected:
            /**
             * Basic hash bin node, used for most entries.  (See below for TreeNode subclass.)
             */
            class Node : public Entry {
            public:
                gint hash;
                KEY key;
                VALUE value;
                NODE next;

                CORE_EXPLICIT Node(gint hash, K& key, V& value, NODE next)
                    : hash(hash), key(&key), value(&value), next(next) {}

                K& getKey() override {
                    return *key;
                }

                K const& getKey() const override {
                    return *key;
                }

                V& getValue() override {
                    return *value;
                }

                V const& getValue() const override {
                    return *value;
                }

                V& setValue(V const& newValue) override {
                    VALUE oldValue = value;
                    value = &UNSAFE::copyInstance(newValue);
                    return *oldValue;
                }
            };

            /**
             * HashMap.Node subclass for normal LinkedHashMap entries.
             */
            class LinkedNode : public Node {
            public:
                LINKEDNODE before = { };
                LINKEDNODE after = { };

                CORE_EXPLICIT LinkedNode(gint hash, K& key, V& value, NODE next) : Node(hash, key, value, next) {}
            };

            enum class Color: gbool { BLACK, RED };

            /* ------------------------------------------------------------ */
            // Tree bins

            /**
             * Entry for Tree bins. Extends LinkedHashMap.Entry (which in turn
             * extends Node) so can be used as extension of either regular or
             * linked node.
             */
            class TreeNode final : public LinkedNode {
            public:
                TREENODE left = { };
                TREENODE right = { };
                TREENODE parent = { }; // red-black tree links
                TREENODE prev = { }; // needed to unlink next upon deletion
                Color color = Color::BLACK;

                CORE_EXPLICIT TreeNode(gint hash, K& key, V& value, NODE next) : LinkedNode(hash, key, value, next) {}

                /**
                 * Returns root of tree containing this node.
                 */
                TREENODE root() {
                    for (TREENODE x = this, p;;) {
                        if (!((p = parentOf(x))))
                            return x;
                        x = p;
                    }
                }

                /**
                 * Ensures that the given root is the first node of its bin.
                 */
                static void root2Front(Array<Node>& table, TREENODE root) {
                    TABLE tab = { };
                    gint n = 0;
                    if (root && ((tab = tableOf(table))) && (n = table.length()) > 0) {
                        gint index = (n - 1) & hashOf(root);
                        TREENODE first = toTreeNode(tab[index]);
                        if (root != first) {
                            NODE rn = { };
                            tab[index] = root;
                            TREENODE rp = prevOf(root);
                            if ((rn = nextOf(root)))
                                toTreeNode(rn)->prev = rp;
                            if (rp)
                                rp->next = rn;
                            if (first)
                                first->prev = root;
                            root->next = first;
                            root->prev = { };
                        }
                    }
                }

                /**
                 * Finds the node starting at root p with the given hash and key.
                 * The kc argument caches comparableClassFor(key) upon first use
                 * comparing keys.
                 */
                TREENODE search(gint h, K const& key) {
                    TREENODE x = this;
                    do {
                        gint xh = 0;
                        gint dir = 0;
                        TREENODE xl = leftOf(x);
                        TREENODE xr = rightOf(x);
                        TREENODE y = { };
                        if ((xh = hashOf(x)) > h)
                            x = xl;
                        else if (xh < h)
                            x = xr;
                        else if (key == keyOf(x))
                            return x;
                        else if (!xl)
                            x = xr;
                        else if (!xr)
                            x = xl;
                        else if ((dir = compare(key, keyOf(x))) != 0)
                            x = (dir < 0) ? xl : xr;
                        else if ((y = xr->search(h, key)))
                            return y;
                        else
                            x = xl;
                    } while (x);
                    return null;
                }

                /**
                 * Calls search for root node.
                 */
                TREENODE exactTreeNode(gint h, K const key) {
                    return (parent ? root() : this)->search(h, key);
                }

                /**
                 * Tie-breaking utility for ordering insertions when equal
                 * hashCodes and non-comparable. We don't require a total
                 * order, just a consistent insertion rule to maintain
                 * equivalence across rebalancings. Tie-breaking further than
                 * necessary simplifies testing a bit.
                 */
                static gint tieBreakOrder(Object const& a, Object const& b) {
                    gint d = 0;
                    if (a == null ||
                        b == null ||
                        (d = Object::typeName(a).compareTo(Object::typeName(b))) == 0)
                        d = (Object::identityHash(a) < Object::identityHash(b)) ? 1 : -1;
                    return d;
                }

                /**
                 * Forms tree of the nodes linked from this node.
                 */
                void treeify(Array<Node>& t) {
                    TREENODE root = { };
                    for (TREENODE x = this, next; x; x = next) {
                        next = toTreeNode(nextOf(x));
                        x->left = x->right = { };
                        if (!root) {
                            x->parent = { };
                            setColor(x, Color::BLACK);
                            root = x;
                        } else {
                            gint h = hashOf(x);
                            for (TREENODE y = root;;) {
                                gint dir = 0;
                                gint yh = 0;
                                if ((yh = hashOf(y)) > h)
                                    dir = -1;
                                else if (yh < h)
                                    dir = 1;
                                else if ((dir = compare(keyOf(x), keyOf(y))) == 0)
                                    dir = tieBreakOrder(keyOf(x), keyOf(y));
                                TREENODE xp = y;
                                if (!((x = (dir <= 0) ? leftOf(x) : rightOf(x)))) {
                                    x->parent = xp;
                                    (dir <= 0 ? xp->left : xp->right) = x;
                                    root = afterInsertion(root, x);
                                    break;
                                }
                            }
                        }
                    }
                    root2Front(t, root);
                }

                /**
                 * Returns a list of non-TreeNodes replacing those linked from
                 * this node.
                 */
                NODE untreeify(HashMap& m) {
                    NODE hd = { };
                    NODE tl = { };
                    for (NODE x = this; x; x = nextOf(x)) {
                        NODE y = m.convert2Node(x, null);
                        if (!tl)
                            hd = y;
                        else
                            tl->next = y;
                        tl = x;
                    }
                    return hd;
                }

                /**
                 * Tree version of putNode.
                 */
                TREENODE putTreeNode(HashMap& m, Array<Node>& tab, gint h, K const& k, V const& v) {
                    gbool searched = false;
                    TREENODE root = parent ? TreeNode::root() : this;
                    for (TREENODE x = root;;) {
                        gint dir = 0;
                        gint xh = 0;
                        if ((xh = hashOf(x)) > h)
                            dir = -1;
                        else if (xh < h)
                            dir = 1;
                        else if (k == keyOf(x))
                            return x;
                        else if ((dir = compare(k, keyOf(x))) == 0) {
                            if (!searched) {
                                TREENODE y = { };
                                TREENODE ch = { };
                                searched = true;
                                if (((ch = leftOf(x))) && ((y = ch->search(h, k))) ||
                                    ((ch = rightOf(x))) && ((y = ch->search(h, k))))
                                    return y;
                            }
                            dir = tieBreakOrder(k, keyOf(x));
                        }

                        TREENODE xp = x;
                        if (!((x = (dir <= 0) ? leftOf(x) : rightOf(x)))) {
                            NODE xpn = nextOf(xp);
                            TREENODE z = m.newTreeNode(h, k, v, xpn);
                            (dir <= 0 ? xp->left : xp->right) = z;
                            xp->next = z;
                            z->parent = z->prev = xp;
                            if (xpn)
                                toTreeNode(xpn)->prev = z;
                            root2Front(tab, afterInsertion(root, z));
                            return null;
                        }
                    }
                }

                /**
                 * Removes the given node, that must be present before this call.
                 * This is messier than typical red-black deletion code because we
                 * cannot swap the contents of an interior node with a leaf
                 * successor that is pinned by "next" pointers that are accessible
                 * independently during traversal. So instead we swap the tree
                 * linkages. If the current tree appears to have too few nodes,
                 * the bin is converted back to a plain bin. (The test triggers
                 * somewhere between 2 and 6 nodes, depending on tree structure).
                 */
                void deleteTreeNode(HashMap& map, Array<Node>& t, gbool movable) {
                    gint n = 0;
                    TABLE tab = { };
                    if (!((tab = tableOf(t))) || (n = t.length()) == 0)
                        return;
                    gint index = (n - 1) & hashOf(this);
                    TREENODE first = toTreeNode(tab[index]);
                    TREENODE root = first;
                    TREENODE rl = { };
                    TREENODE succ = toTreeNode(nextOf(this));
                    TREENODE pred = prev;
                    if (!pred)
                        tab[index] = first = succ;
                    else
                        pred->next = succ;
                    if (succ)
                        succ->prev = pred;
                    if (!first)
                        return;
                    if (parentOf(root))
                        root = root->root();
                    if ((!root) || (movable && (!rightOf(root) || !((rl = leftOf(root))) || !leftOf(rl)))) {
                        tab[index] = first->untreeify(map); // too small
                        return;
                    }
                    TREENODE p = this;
                    TREENODE pl = left;
                    TREENODE pr = right;
                    TREENODE repl = { };
                    if (pl && pr) {
                        TREENODE s = pr;
                        TREENODE sl = { };
                        while ((sl = leftOf(s))) // find successor
                            s = sl;
                        Color c = colorOf(s);
                        setColor(s, colorOf(p));
                        setColor(p, c); // swap colors
                        TREENODE sr = rightOf(s);
                        TREENODE pp = parentOf(p);
                        if (s == pr) {
                            // p was s's direct parent
                            p->parent = s;
                            s->right = p;
                        } else {
                            TREENODE sp = parentOf(s);
                            if ((p->parent = sp)) {
                                if (s == leftOf(sp))
                                    sp->left = p;
                                else
                                    sp->right = p;
                            }
                            if ((s->right = pr))
                                pr->parent = s;
                        }
                        p->left = { };
                        if ((p->right = sr))
                            sr->parent = p;
                        if ((s->left = pl))
                            pl->parent = s;
                        if (!((s->parent = pp)))
                            root = s;
                        else if (p == pp->left)
                            pp->left = s;
                        else
                            pp->right = s;
                        if (sr)
                            repl = sr;
                        else
                            repl = p;
                    } else if (pl)
                        repl = pl;
                    else if (pr)
                        repl = pr;
                    else
                        repl = p;
                    if (repl != p) {
                        TREENODE pp = repl->parent = parentOf(p);
                        if (!pp)
                            setColor(root = repl, Color::BLACK);
                        else if (p == leftOf(pp))
                            pp->left = repl;
                        else
                            pp->right = repl;
                        p->left = p->right = p->parent = { };
                    }

                    TREENODE r = colorOf(p) == Color::RED ? root : afterDeletion(root, repl);

                    if (repl == p) {
                        // detach
                        TREENODE pp = parentOf(p);
                        p->parent = { };
                        if (pp) {
                            if (p == leftOf(pp))
                                pp->left = { };
                            else if (p == rightOf(pp))
                                pp->right = { };
                        }
                    }
                    if (movable)
                        root2Front(t, r);
                }

                /**
                 * Splits nodes in a tree bin into lower and upper tree bins,
                 * or untreeifies if now too small. Called only from resize;
                 * see above discussion about split bits and indices.
                 *
                 * @param map the map
                 * @param t the table for recording bin heads
                 * @param index the index of the table being split
                 * @param bit the bit of hash to split on
                 */
                void split(HashMap& map, Array<Node>& t, gint index, gint bit) {
                    TREENODE b = this;
                    // Relink into lo and hi lists, preserving order
                    TREENODE loHead = { };
                    TREENODE loTail = { };
                    TREENODE hiHead = { };
                    TREENODE hiTail = { };
                    int lc = 0;
                    int hc = 0;
                    for (TREENODE e = b, next; e; e = next) {
                        next = toTreeNode(nextOf(e));
                        e->next = { };
                        if ((hashOf(e) & bit) == 0) {
                            if (!((e->prev = loTail)))
                                loHead = e;
                            else
                                loTail->next = e;
                            loTail = e;
                            ++lc;
                        } else {
                            if (!((e->prev = hiTail)))
                                hiHead = e;
                            else
                                hiTail->next = e;
                            hiTail = e;
                            ++hc;
                        }
                    }
                    TABLE tab = tableOf(t);
                    if (loHead) {
                        if (lc <= UNTREEIFY_THRESHOLD)
                            tab[index] = loHead->untreeify(map);
                        else {
                            tab[index] = loHead;
                            if (hiHead) // (else is already treeified)
                                loHead->treeify(t);
                        }
                    }
                    if (hiHead) {
                        if (hc <= UNTREEIFY_THRESHOLD)
                            tab[index + bit] = hiHead->untreeify(map);
                        else {
                            tab[index + bit] = hiHead;
                            if (loHead)
                                hiHead->treeify(t);
                        }
                    }
                }

                /* ------------------------------------------------------------ */
                // Red-black tree methods, all adapted from CLR

                static TREENODE rotateLeft(TREENODE root, TREENODE x) {
                    TREENODE r = { };
                    TREENODE xp = { };
                    TREENODE rl = { };
                    if (x && ((r = rightOf(x)))) {
                        if ((rl = x->right = leftOf(x)))
                            rl->parent = x;
                        if (!((xp = r->parent = parentOf(x))))
                            setColor(root = r, Color::BLACK);
                        else if (leftOf(xp) == x)
                            xp->left = r;
                        else
                            xp->right = r;
                        r->left = x;
                        x->parent = r;
                    }
                    return root;
                }

                static TREENODE rotateRight(TREENODE root, TREENODE x) {
                    TREENODE l = { };
                    TREENODE xp = { };
                    TREENODE lr = { };
                    if (x && ((l = leftOf(x)))) {
                        if ((lr = x->left = rightOf(l)))
                            lr->parent = x;
                        if (!((xp = l->parent = parentOf(x))))
                            setColor(root = l, Color::BLACK);
                        else if (rightOf(xp) == x)
                            xp->right = l;
                        else
                            xp->left = l;
                        l->right = x;
                        x->parent = l;
                    }
                    return root;
                }

                static TREENODE afterInsertion(TREENODE root, TREENODE x) {
                    setColor(x, Color::RED);
                    for (TREENODE xp = { }, xpp = { }, xppl = { }, xppr = { };;) {
                        if (!((xp = parentOf(x)))) {
                            setColor(x, Color::BLACK);
                            return x;
                        }
                        if (colorOf(xp) == Color::RED || !((xpp = parentOf(xp))))
                            return root;
                        if (xp == (xppl = leftOf(xpp))) {
                            if (((xppr = rightOf(xpp))) && colorOf(xppr) == Color::RED) {
                                setColor(xppr, Color::BLACK);
                                setColor(xp, Color::BLACK);
                                setColor(xpp, Color::RED);
                                x = xpp;
                            } else {
                                if (x == rightOf(xp)) {
                                    root = rotateLeft(root, x = xp);
                                    xpp = parentOf(xp = parentOf(x));
                                }
                                if (xp) {
                                    setColor(xp, Color::BLACK);
                                    if (xpp) {
                                        setColor(xpp, Color::RED);
                                        root = rotateRight(root, xpp);
                                    }
                                }
                            }
                        } else {
                            if (xppl && colorOf(xppl) == Color::RED) {
                                setColor(xppl, Color::BLACK);
                                setColor(xp, Color::BLACK);
                                setColor(xpp, Color::RED);
                                x = xpp;
                            } else {
                                if (x == leftOf(xp)) {
                                    root = rotateRight(root, x = xp);
                                    xpp = parentOf(xp = parentOf(x));
                                }
                                if (xp) {
                                    setColor(xp, Color::BLACK);
                                    if (xpp) {
                                        setColor(xpp, Color::RED);
                                        root = rotateLeft(root, xpp);
                                    }
                                }
                            }
                        }
                    }
                }

                static TREENODE afterDeletion(TREENODE root, TREENODE x) {
                    for (TREENODE xp = { }, xpl = { }, xpr = { };;) {
                        if ((!x) || x == root)
                            return root;
                        if (!((xp = parentOf(x)))) {
                            setColor(x, Color::BLACK);
                            return x;
                        }
                        if (colorOf(x) == Color::RED) {
                            setColor(x, Color::BLACK);
                            return root;
                        }
                        if ((xpl = leftOf(xp)) == x) {
                            if (((xpr = rightOf(xp))) && colorOf(xp) == Color::RED) {
                                setColor(xpr, Color::BLACK);
                                setColor(xp, Color::RED);
                                root = rotateLeft(root, xp);
                                xpr = rightOf(xp = parentOf(x));
                            }
                            if (!xpr)
                                x = xp;
                            else {
                                TREENODE sl = leftOf(xpr);
                                TREENODE sr = rightOf(xpr);
                                if (((!sr) || colorOf(sr) == Color::BLACK) && ((!sl) || colorOf(sl) == Color::BLACK)) {
                                    setColor(xpr, Color::RED);
                                    x = xp;
                                } else {
                                    if ((!sr) || colorOf(sr) == Color::BLACK) {
                                        setColor(sl, Color::BLACK);
                                        setColor(xpr, Color::RED);
                                        root = rotateRight(root, xpr);
                                        xpr = rightOf(xp = parentOf(x));
                                    }
                                    if (xpr) {
                                        setColor(xpr, colorOf(xp));
                                        if (!((sr = rightOf(xpr))))
                                            setColor(sr, Color::BLACK);
                                    }
                                    if (xp) {
                                        setColor(xp, Color::BLACK);
                                        root = rotateLeft(root, xp);
                                    }
                                    x = root;
                                }
                            }
                        } else {
                            // symmetric
                            if (xpl && colorOf(xpl) == Color::RED) {
                                setColor(xpl, Color::BLACK);
                                setColor(xp, Color::RED);
                                root = rotateRight(root, xp);
                                xpl = leftOf(xp = parentOf(x));
                            }
                            if (!xpl)
                                x = xp;
                            else {
                                TREENODE sl = leftOf(xpl);
                                TREENODE sr = rightOf(xpl);
                                if (((!sl) || sl->color == Color::BLACK) && (!sr || colorOf(sr) == Color::BLACK)) {
                                    setColor(xpl, Color::RED);
                                    x = xp;
                                } else {
                                    if ((!sl) || sl->color == Color::BLACK) {
                                        setColor(sr, Color::BLACK);
                                        setColor(xpl, Color::BLACK);
                                        root = rotateLeft(root, xpl);
                                        xpl = leftOf(xp = parentOf(x));
                                    }
                                    if (xpl) {
                                        setColor(xpl, colorOf(xp));
                                        if ((sl = leftOf(xpl)))
                                            setColor(sl, Color::BLACK);
                                    }
                                    if (xp) {
                                        setColor(xp, Color::BLACK);
                                        root = rotateRight(root, xp);
                                    }
                                    x = root;
                                }
                            }
                        }
                    }
                }

                /**
                 * Recursive invariant check
                 */
                static gbool checkInvariants(TREENODE t) {
                    TREENODE tp = parentOf(t);
                    TREENODE tl = leftOf(t);
                    TREENODE tr = rightOf(t);
                    TREENODE tb = prevOf(t);
                    TREENODE tn = toTreeNode(nextOf(t));
                    if (tb && nextOf(tb) != t)
                        return false;
                    if (tn && prevOf(tn) != t)
                        return false;
                    if (tp && t != leftOf(tp) && t != rightOf(tp))
                        return false;
                    if (tl && (parentOf(tl) != t || hashOf(tl) > hashOf(t)))
                        return false;
                    if (tr && (parentOf(tr) != t || hashOf(tr) < hashOf(t)))
                        return false;
                    if (colorOf(t) == Color::RED && tl && colorOf(tl) == Color::RED && tr && colorOf(tr) == Color::RED)
                        return false;
                    if (tl && !checkInvariants(tl))
                        return false;
                    if (tr && !checkInvariants(tr))
                        return false;
                    return true;
                }
            };

            static Color colorOf(TREENODE x) { return (!x) ? Color::BLACK : x->color; }

            static Color setColor(TREENODE x, Color c) { return (!x) ? Color::BLACK : (x->color = c); }

            /**
             * Calculate initial capacity for HashMap based classes, from expected size and default load factor (0.75).
             *
             * @param numMappings the expected number of mappings
             * @return initial capacity for HashMap based classes.
             */
            static gint hashCapacity(gint numMappings) {
                return CORE_CAST(gint, Math::ceil(numMappings / CORE_CAST(gdouble, DEFAULT_LOAD_FACTOR)));
            }

        private:
            // ::::::::::::::::::::::::::: LinkedHashMap Support ::::::::::::::::::::::::::::::::::::::


            /*
             * The following package-protected methods are designed to be
             * overridden by LinkedHashMap, but not by any other subclass.
             * Nearly all other internal methods are also package-protected
             * but are declared final, so can be used by LinkedHashMap, view
             * classes, and HashSet.
             */

            // Create a regular (non-tree) node
            virtual NODE newNode(gint hash, K const& key, V const& value, NODE next) {
                K& k = UNSAFE::copyInstance(key);
                V& v = UNSAFE::copyInstance(value);
                return &UNSAFE::newReplaceableInstance<Node>(Class<TreeNode>::size(), hash, k, v, next);
            }

            // For conversion from TreeNodes to plain nodes
            virtual NODE convert2Node(NODE x, NODE next) {
                return new(x) Node(hashOf(x), keyOf(x), valueOf(x), next);
            }

            // Create a tree bin node
            virtual TREENODE newTreeNode(gint hash, K const& key, V const& value, NODE next) {
                K& k = UNSAFE::copyInstance(key);
                V& v = UNSAFE::copyInstance(value);
                return &UNSAFE::newInstance<TreeNode>(hash, k, v, next);
            }

            // For treeifyBin
            virtual TREENODE convert2TreeNode(NODE x, NODE next) {
                return new(x) TreeNode(hashOf(x), keyOf(x), valueOf(x), next);
            }

            // Callbacks to allow LinkedHashMap post-actions
            virtual void afterAccess(NODE x) {
                // Do Nothing
            }

            virtual void afterInsertion(gbool evict) {
                // Do Nothing
            }

            virtual void afterDeletion(NODE x) {
                // Do Nothing
            }

            // ::::::::::::::::::::::::::: HashMap Ctors/Assignment Utility ::::::::::::::::::::::::::::::::::::::

            virtual void initialize(HashMap& m2) {
                table = UNSAFE::moveInstance(m2.table);
                UNSAFE::swapValues(count, m2.count);
                UNSAFE::swapValues(loadFactor, m2.loadFactor);
                UNSAFE::swapValues(threshold, m2.threshold);
            }

            static void detachKeys(HashMap const& map) { Map::detachKeys(map); }

            static void detachVals(HashMap const& map) { Map::detachVals(map); }

            static void detachEntries(HashMap const& map) { Map::detachEntries(map); }

            // ::::::::::::::::::::::::::: HashMap Views Classes ::::::::::::::::::::::::::::::::::::::

            /**
             * The keys view class
             */
            class KeySet;

            /**
             * The values view class
             */
            class Values;

            /**
             * The entries view classes
             */
            class EntrySet;
        };

        template <class K, class V>
        class HashMap<K, V>::KeySet final : public Set<K> {
            HashMap<K, V>& map;

        public:
            CORE_EXPLICIT KeySet(HashMap<K, V> const& map) : map((HashMap&) map) {}

            gint size() const override { return map.size(); }

            gbool isEmpty() const override { return map.isEmpty(); }

            Iterator<K>& iterator() override {
                class Itr final : public Iterator<K> {
                    HashMap<K, V>& map;
                    gint index = 0;
                    NODE cursor = null;
                    NODE last = null;
                    gint modCount = 0;

                public:
                    Itr(HashMap<K, V>& map) : map(map) {
                        gint len = map.table.length();
                        for (TABLE t = tableOf(map.table); index < len; index++) {
                            cursor = t[index];
                            if (cursor != null)
                                break;
                        }
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    K& next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = nextOf(cursor);
                        if (cursor == null) {
                            gint len = map.table.length();
                            index += 1;
                            for (TABLE t = tableOf(map.table); index < len; index++) {
                                cursor = t[index];
                                if (cursor != null)
                                    break;
                            }
                        }
                        return keyOf(last);
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && index == itr.index && cursor == itr.cursor;
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        NODE node = last;
                        last = null;
                        map.deleteNode(hashOf(node), keyOf(node), null, false, false);
                        modCount = map.modCount;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(map); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& iterator() const override {
                class Itr final : public Iterator2<K> {
                    HashMap<K, V> const& map;
                    gint index = 0;
                    NODE cursor = null;
                    NODE last = null;
                    gint modCount = 0;

                public:
                    Itr(HashMap<K, V> const& map) : map(map) {
                        gint len = map.table.length();
                        for (TABLE t = tableOf(map.table); index < len; index++) {
                            cursor = t[index];
                            if (cursor != null)
                                break;
                        }
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    K const& next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = nextOf(cursor);
                        if (cursor == null) {
                            gint len = map.table.length();
                            index += 1;
                            for (TABLE t = tableOf(map.table); index < len; index++) {
                                cursor = t[index];
                                if (cursor != null)
                                    break;
                            }
                        }
                        return keyOf(last);
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && index == itr.index && cursor == itr.cursor;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(map); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<K&> const& action) override {
                gint index = 0;
                gint len = map.table.length();
                gint modCount = map.modCount;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        K& key = keyOf(node);

                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        action.accept(key);
                    }
                }
            }

            void forEach(function::Consumer<K> const& action) const override {
                gint index = 0;
                gint len = map.table.length();
                gint modCount = map.modCount;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        K& key = keyOf(node);

                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        action.accept(key);
                    }
                }
            }

            gbool contains(K const& val) const override {
                return map.containsKey(val);
            }

            Array<K> toArray() const override {
                gint len = map.table.length();
                gint index = 0;
                gint modCount = 0;
                CORE_IGNORE_DEPRECATIONS(Array<K> a = Array<K>(len);)
                gint i = 0;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        K& key = keyOf(node);
                        Arrays::fastSet(a, i++, key);
                    }
                }
                return a;
            }

            gbool add(K const& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool remove(K const& val) override {
                gint size = map.size();
                try { map.remove(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return map.size() < size;
            }

            gbool addAll(Collection<K> const& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                map.clear();
            }

            ~KeySet() override {
                detachKeys(map);
            }
        };

        template <class K, class V>
        class HashMap<K, V>::Values final : public Collection<V> {
            HashMap<K, V>& map;

        public:
            CORE_EXPLICIT Values(HashMap<K, V> const& map) : map((HashMap&) map) {}

            gint size() const override { return map.size(); }

            gbool isEmpty() const override { return map.isEmpty(); }

            Iterator<V>& iterator() override {
                class Itr final : public Iterator<V> {
                    HashMap<K, V>& map;
                    gint index = 0;
                    NODE cursor = null;
                    NODE last = null;
                    gint modCount = 0;

                public:
                    Itr(HashMap<K, V>& map) : map(map) {
                        gint len = map.table.length();
                        for (TABLE t = tableOf(map.table); index < len; index++) {
                            cursor = t[index];
                            if (cursor != null)
                                break;
                        }
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    V& next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = nextOf(cursor);
                        if (cursor == null) {
                            gint len = map.table.length();
                            index += 1;
                            for (TABLE t = tableOf(map.table); index < len; index++) {
                                cursor = t[index];
                                if (cursor != null)
                                    break;
                            }
                        }
                        return valueOf(last);
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        NODE node = last;
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
                        return &map == &itr.map && index == itr.index && cursor == itr.cursor;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(map); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& iterator() const override {
                class Itr final : public Iterator2<V> {
                    HashMap<K, V> const& map;
                    gint index = 0;
                    NODE cursor = null;
                    NODE last = null;
                    gint modCount = 0;

                public:
                    Itr(HashMap<K, V> const& map) : map(map) {
                        gint len = map.table.length();
                        for (TABLE t = tableOf(map.table); index < len; index++) {
                            cursor = t[index];
                            if (cursor != null)
                                break;
                        }
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    V const& next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = nextOf(cursor);
                        if (cursor == null) {
                            gint len = map.table.length();
                            index += 1;
                            for (TABLE t = tableOf(map.table); index < len; index++) {
                                cursor = t[index];
                                if (cursor != null)
                                    break;
                            }
                        }
                        return valueOf(last);
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && index == itr.index && cursor == itr.cursor;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(map); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<V&> const& action) override {
                gint index = 0;
                gint len = map.table.length();
                gint modCount = map.modCount;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        V& value = valueOf(node);

                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        action.accept(value);
                    }
                }
            }

            void forEach(function::Consumer<V> const& action) const override {
                gint index = 0;
                gint len = map.table.length();
                gint modCount = map.modCount;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        V& value = valueOf(node);

                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        action.accept(value);
                    }
                }
            }

            gbool contains(V const& val) const override {
                return map.containsValue(val);
            }

            Array<V> toArray() const override {
                gint len = map.table.length();
                gint index = 0;
                gint modCount = 0;
                CORE_IGNORE_DEPRECATIONS(Array<V> a = Array<V>(len);)
                gint i = 0;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        V& value = valueOf(node);
                        Arrays::fastSet(a, i++, value);
                    }
                }
                return a;
            }

            gbool add(V const& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool addAll(Collection<V> const& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                map.clear();
            }

            ~Values() override {
                detachVals(map);
            }
        };

        template <class K, class V>
        class HashMap<K, V>::EntrySet final : public Set<Entry> {
            HashMap<K, V>& map;

        public:
            CORE_EXPLICIT EntrySet(HashMap const& map)
                : map((HashMap&) map) {}

            gint size() const override { return map.size(); }

            gbool isEmpty() const override { return map.isEmpty(); }

            Iterator<Entry>& iterator() override {
                class Itr final : public Iterator<Entry> {
                    HashMap<K, V>& map;
                    gint index = 0;
                    NODE cursor = null;
                    NODE last = null;
                    gint modCount = 0;

                public:
                    Itr(HashMap<K, V>& map) : map(map) {
                        gint len = map.table.length();
                        for (TABLE t = tableOf(map.table); index < len; index++) {
                            cursor = t[index];
                            if (cursor != null)
                                break;
                        }
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    Entry& next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = nextOf(cursor);
                        if (cursor == null) {
                            gint len = map.table.length();
                            index += 1;
                            for (TABLE t = tableOf(map.table); index < len; index++) {
                                cursor = t[index];
                                if (cursor != null)
                                    break;
                            }
                        }
                        return *last;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && index == itr.index && cursor == itr.cursor;
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        NODE node = last;
                        last = null;
                        map.deleteNode(hashOf(node), keyOf(node), null, false, false);
                        modCount = map.modCount;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(map); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry>& iterator() const override {
                class Itr final : public Iterator2<Entry> {
                    HashMap<K, V> const& map;
                    gint index = 0;
                    NODE cursor = null;
                    NODE last = null;
                    gint modCount = 0;

                public:
                    Itr(HashMap<K, V> const& map) : map(map) {
                        gint len = map.table.length();
                        for (TABLE t = tableOf(map.table); index < len; index++) {
                            cursor = t[index];
                            if (cursor != null)
                                break;
                        }
                        modCount = map.modCount;
                    }

                    gbool hasNext() const override { return cursor != null; }

                    Entry const& next() override {
                        if (!cursor)
                            NoSuchElementException().throws($ftrace());
                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        last = cursor;
                        cursor = nextOf(cursor);
                        if (cursor == null) {
                            gint len = map.table.length();
                            index += 1;
                            for (TABLE t = tableOf(map.table); index < len; index++) {
                                cursor = t[index];
                                if (cursor != null)
                                    break;
                            }
                        }
                        return *last;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& itr = CORE_XCAST(Itr const, other);
                        return &map == &itr.map && index == itr.index && cursor == itr.cursor;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(map); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<Entry&> const& action) override {
                gint index = 0;
                gint len = map.table.length();
                gint modCount = map.modCount;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        Entry& entry = *node;

                        if (modCount != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        action.accept(entry);
                    }
                }
            }

            void forEach(function::Consumer<Entry> const& action) const override {
                gint index = 0;
                gint len = map.table.length();
                gint mc = map.modCount;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        Entry& entry = *node;

                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        action.accept(entry);
                    }
                }
            }

            Array<Entry> toArray() const override {
                gint len = map.table.length();
                gint index = 0;
                gint mc = 0;
                CORE_IGNORE_DEPRECATIONS(Array<Entry> a = Array<Entry>(len);)
                gint i = 0;
                for (TABLE t = tableOf(map.table); index < len; index++) {
                    for (NODE node = t[index]; node != null; node = nextOf(node)) {
                        if (mc != map.modCount)
                            ConcurrentModificationException().throws($ftrace());

                        Entry& entry = *node;
                        Arrays::fastSet(a, i++, entry);
                    }
                }
                return a;
            }

            gbool add(Entry const& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool remove(Entry const& val) override {
                gint size = map.size();
                try { map.remove(val.getKey(), val.getValue()); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return map.size() < size;
            }

            gbool addAll(Collection<Entry> const& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                map.clear();
            }

            ~EntrySet() override { detachEntries(map); }
        };
    } // util
} // core

#endif //CORE24_HASHMAP_H
