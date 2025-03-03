//
// Created by brunshweeck on 4 août 2024.
//

#ifndef CORE24_SORTEDMAP_H
#define CORE24_SORTEDMAP_H

#include <core/util/SequencedMap.h>

namespace core {
    namespace util {
        /**
         * A @b Map that further provides a <em>total ordering</em> on its keys.
         * The map is ordered according to the <em>natural ordering</em>
         * of its keys, or by a @b Comparator typically
         * provided at sorted map creation time.  This order is reflected when
         * iterating over the sorted map's collection views (returned by the
         * @c entrySet, @c keySet and @c values methods).
         * Several additional operations are provided to take advantage of the
         * ordering.  (This interface is the map analogue of @c SortedSet.)
         * <p>
         * All keys inserted into a sorted map must implement the @c Comparable
         * interface (or be accepted by the specified comparator).  Furthermore, all
         * such keys must be <em>mutually comparable</em>: @c k1.compareTo(k2) (or
         * @code comparator.compare(k1, k2) @endcode ) must not throw a
         * @c ClassCastException for any keys @c k1 and @c k2 in
         * the sorted map.  Attempts to violate this restriction will cause the
         * offending method or constructor invocation to throw a
         * @c ClassCastException.
         * </p>
         * <p>
         * Note that the ordering maintained by a sorted map (whether an
         * explicit comparator is provided) must be <em>consistent with equals</em> if
         * the sorted map is to correctly implement the @c Map interface.  (See
         * the @c Comparable interface or @c Comparator interface for a
         * precise definition of <em>consistent with equals</em>.)  This is so because
         * the @c Map interface is defined in terms of the @c equals
         * operation, but a sorted map performs all key comparisons using its
         * @c compareTo (or @c compare) method, so two keys that are
         * deemed equal by this method are, from the standpoint of the sorted map,
         * equal.  The behavior of a tree map <em>is</em> well-defined even if its
         * ordering is inconsistent with equals; it just fails to obey the general
         * contract of the @c Map interface.
         * </p>
         * <p>
         * All general-purpose sorted map implementation classes should provide four
         * "standard" constructors. It is not possible to enforce this recommendation
         * though as required constructors cannot be specified by interfaces. The
         * expected "standard" constructors for all sorted map implementations are:
         * <br/>
         *   - A void (no arguments) constructor, which creates an empty sorted map
         *   sorted according to the natural ordering of its keys.<br/>
         *   - A constructor with a single argument of type @c Comparator, which
         *   creates an empty sorted map sorted according to the specified comparator.<br/>
         *   - A constructor with a single argument of type @c Map, which creates
         *   a new map with the same key-value mappings as its argument, sorted
         *   according to the keys' natural ordering.<br/>
         *   - A constructor with a single argument of type @c SortedMap, which
         *   creates a new sorted map with the same key-value mappings and the same
         *   ordering as the input sorted map.<br/>
         * </p>
         * <p>
         * @b Note: several methods return submaps with restricted key
         * ranges. Such ranges are <em>half-open</em>, that is, they include their low
         * endpoint but not their high endpoint (where applicable).  If you need a
         * <em>closed range</em> (which includes both endpoints), and the key type
         * allows for calculation of the successor of a given key, merely request
         * the subrange from @c lowEndpoint to @c successor(highEndpoint).
         * For example, suppose that @c m is a map whose keys are strings.
         * The following idiom obtains a view containing all the key-value mappings
         * in @c m whose keys are between @c low and @c high, inclusive:
         * @code
         *   SortedMap<String, ?> sub = m.subMap(low, high+"\0");
         * @endcode
         *
         * A similar technique can be used to generate an <em>open range</em>
         * (which contains neither endpoint).  The following idiom obtains a
         * view containing all the key-value mappings in @c m whose keys
         * are between @c low and @c high, exclusive:
         * @code
         *   SortedMap<String, ?> sub = m.subMap(low+"\0", high);
         * @endcode
         * </p>
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         *
         * @see Map
         * @see TreeMap
         * @see SortedSet
         * @see Comparator
         * @see Comparable
         * @see Collection
         * @see ClassCastException
         */
        template <class K, class V>
        class SortedMap : public virtual SequencedMap<K, V> {
        protected:
            CORE_ALIAS(UNSAFE, Object::UNSAFE);
            CORE_ALIAS(Entry, util::Entry<K, V>);
            CORE_ALIAS(Map, util::Map<K, V>);
            CORE_ALIAS(SequencedMap, util::SequencedMap<K, V>);

            CORE_ALIAS(SortedView, typename Class<SortedMap>::Pointer);
            CORE_ALIAS(Entries, typename Map::Entries);

        public:
            /**
             * Returns the comparator used to order the keys in this map.
             *
             * @return the comparator used to order the keys in this map.
             */
            virtual Comparator<K> const& comparator() const = 0;

            /**
             * Returns a view of the portion of this map whose keys range from
             * @c fromKey, inclusive, to @c toKey, exclusive.  (If
             * @c fromKey and @c toKey are equal, the returned map
             * is empty.)  The returned map is backed by this map, so changes
             * in the returned map are reflected in this map, and vice versa.
             * The returned map supports all optional map operations that this
             * map supports.
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param fromKey low endpoint (inclusive) of the keys in the returned map
             * @param toKey high endpoint (exclusive) of the keys in the returned map
             * @return a view of the portion of this map whose keys range from
             *         @c fromKey, inclusive, to @c toKey, exclusive
             * @throws ClassCastException if @c fromKey and @c toKey
             *         cannot be compared to one another using this map's comparator
             *         (or, if the map has no comparator, using natural ordering).
             *         Implementations may, but are not required to, throw this
             *         exception if @c fromKey or @c toKey
             *         cannot be compared to keys currently in the map.
             * @throws IllegalArgumentException if @c fromKey is greater than
             *         @c toKey; or if this map itself has a restricted
             *         range, and @c fromKey or @c toKey lies
             *         outside the bounds of the range
             */
            virtual SortedMap const& subMap(K const& fromKey, K const& toKey) const = 0;

            /**
             * Returns a view of the portion of this map whose keys range from
             * @c fromKey, inclusive, to @c toKey, exclusive.  (If
             * @c fromKey and @c toKey are equal, the returned map
             * is empty.)  The returned map is backed by this map, so changes
             * in the returned map are reflected in this map, and vice versa.
             * The returned map supports all optional map operations that this
             * map supports.
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param fromKey low endpoint (inclusive) of the keys in the returned map
             * @param toKey high endpoint (exclusive) of the keys in the returned map
             * @return a view of the portion of this map whose keys range from
             *         @c fromKey, inclusive, to @c toKey, exclusive
             * @throws ClassCastException if @c fromKey and @c toKey
             *         cannot be compared to one another using this map's comparator
             *         (or, if the map has no comparator, using natural ordering).
             *         Implementations may, but are not required to, throw this
             *         exception if @c fromKey or @c toKey
             *         cannot be compared to keys currently in the map.
             * @throws IllegalArgumentException if @c fromKey is greater than
             *         @c toKey; or if this map itself has a restricted
             *         range, and @c fromKey or @c toKey lies
             *         outside the bounds of the range
             */
            virtual SortedMap& subMap(K const& fromKey, K const& toKey) = 0;

            /**
             * Returns a view of the portion of this map whose keys are
             * strictly less than @c toKey.  The returned map is backed
             * by this map, so changes in the returned map are reflected in
             * this map, and vice versa.  The returned map supports all
             * optional map operations that this map supports.
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param toKey high endpoint (exclusive) of the keys in the returned map
             * @return a view of the portion of this map whose keys are strictly
             *         less than @c toKey
             * @throws ClassCastException if @c toKey is not compatible
             *         with this map's comparator (or, if the map has no comparator,
             *         if @c toKey does not implement @b Comparable).
             *         Implementations may, but are not required to, throw this
             *         exception if @c toKey cannot be compared to keys
             *         currently in the map.
             * @throws IllegalArgumentException if this map itself has a
             *         restricted range, and @c toKey lies outside the
             *         bounds of the range
             */
            virtual SortedMap const& headMap(K const& toKey) const = 0;

            /**
             * Returns a view of the portion of this map whose keys are
             * strictly less than @c toKey.  The returned map is backed
             * by this map, so changes in the returned map are reflected in
             * this map, and vice versa.  The returned map supports all
             * optional map operations that this map supports.
             *
             * <p>The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             *
             * @param toKey high endpoint (exclusive) of the keys in the returned map
             * @return a view of the portion of this map whose keys are strictly
             *         less than @c toKey
             * @throws ClassCastException if @c toKey is not compatible
             *         with this map's comparator (or, if the map has no comparator,
             *         if @c toKey does not implement @b Comparable).
             *         Implementations may, but are not required to, throw this
             *         exception if @c toKey cannot be compared to keys
             *         currently in the map.
             * @throws IllegalArgumentException if this map itself has a
             *         restricted range, and @c toKey lies outside the
             *         bounds of the range
             */
            virtual SortedMap& headMap(K const& toKey) = 0;

            /**
             * Returns a view of the portion of this map whose keys are
             * greater than or equal to @c fromKey.  The returned map is
             * backed by this map, so changes in the returned map are
             * reflected in this map, and vice versa.  The returned map
             * supports all optional map operations that this map supports.
             *
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param fromKey low endpoint (inclusive) of the keys in the returned map
             * @return a view of the portion of this map whose keys are greater
             *         than or equal to @c fromKey
             * @throws ClassCastException if @c fromKey is not compatible
             *         with this map's comparator (or, if the map has no comparator,
             *         if @c fromKey does not implement @b Comparable).
             *         Implementations may, but are not required to, throw this
             *         exception if @c fromKey cannot be compared to keys
             *         currently in the map.
             * @throws IllegalArgumentException if this map itself has a
             *         restricted range, and @c fromKey lies outside the
             *         bounds of the range
             */
            virtual SortedMap const& tailMap(K const& fromKey) const = 0;

            /**
             * Returns a view of the portion of this map whose keys are
             * greater than or equal to @c fromKey.  The returned map is
             * backed by this map, so changes in the returned map are
             * reflected in this map, and vice versa.  The returned map
             * supports all optional map operations that this map supports.
             *
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param fromKey low endpoint (inclusive) of the keys in the returned map
             * @return a view of the portion of this map whose keys are greater
             *         than or equal to @c fromKey
             * @throws ClassCastException if @c fromKey is not compatible
             *         with this map's comparator (or, if the map has no comparator,
             *         if @c fromKey does not implement @b Comparable).
             *         Implementations may, but are not required to, throw this
             *         exception if @c fromKey cannot be compared to keys
             *         currently in the map.
             * @throws IllegalArgumentException if this map itself has a
             *         restricted range, and @c fromKey lies outside the
             *         bounds of the range
             */
            virtual SortedMap& tailMap(K const& fromKey) = 0;

            /**
             * Returns the first key-value mapping in this map,
             * or thrown NoSuchElementException if the map is empty.
             *
             * @note
             * The implementation in this interface obtains the iterator of this map's entrySet.
             * If the iterator has an element, it returns an unmodifiable copy of that element.
             * Otherwise, it returns null.
             *
             * @return the first key-value mapping
             * @throws NoSuchElementException If this map is empty
             */
            Entry const& firstEntry() const override {
                try {
                    return SequencedMap::firstEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the first key-value mapping in this map,
             * or thrown NoSuchElementException if the map is empty.
             *
             * @note
             * The implementation in this interface obtains the iterator of this map's entrySet.
             * If the iterator has an element, it returns an unmodifiable copy of that element.
             * Otherwise, it returns null.
             *
             * @return the first key-value mapping
             * @throws NoSuchElementException If this map is empty
             */
            Entry& firstEntry() override {
                try { return SequencedMap::firstEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the last key-value mapping in this map,
             * or thrown NoSuchElementException if the map is empty.
             *
             * @note
             * The implementation in this interface obtains the iterator of the entrySet of this map's
             * reversed view. If the iterator has an element, it returns an unmodifiable copy of
             * that element. Otherwise, it returns null.
             *
             * @return the last key-value mapping
             * @throws NoSuchElementException If this map is empty
             */
            Entry const& lastEntry() const override {
                try { return SequencedMap::lastEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the last key-value mapping in this map,
             * or thrown NoSuchElementException if the map is empty.
             *
             * @note
             * The implementation in this interface obtains the iterator of the entrySet of this map's
             * reversed view. If the iterator has an element, it returns an unmodifiable copy of
             * that element. Otherwise, it returns null.
             *
             * @return the last key-value mapping
             * @throws NoSuchElementException If this map is empty
             */
            Entry& lastEntry() override {
                try { return SequencedMap::lastEntry(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the first (lowest) key currently in this map.
             *
             * @return the first (lowest) key currently in this map
             * @throws NoSuchElementException if this map is empty
             */
            virtual K const& firstKey() const {
                try { return firstEntry().getKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the first (lowest) key currently in this map.
             *
             * @return the first (lowest) key currently in this map
             * @throws NoSuchElementException if this map is empty
             */
            virtual K& firstKey() {
                try { return firstEntry().getKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the last (highest) key currently in this map.
             *
             * @return the last (highest) key currently in this map
             * @throws NoSuchElementException if this map is empty
             */
            virtual K const& lastKey() const {
                try { return lastEntry().getKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the last (highest) key currently in this map.
             *
             * @return the last (highest) key currently in this map
             * @throws NoSuchElementException if this map is empty
             */
            virtual K& lastKey() {
                try { return lastEntry().getKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a @b Set view of the keys contained in this map.
             * The set's iterator returns the keys in ascending order.
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
             * @return a set view of the keys contained in this map, sorted in
             *         ascending order
             */
            Set<K>& keySet() override {
                return sequencedKeySet();
            }

            /**
             * Returns a @b Set view of the keys contained in this map.
             * The set's iterator returns the keys in ascending order.
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
             * @return a set view of the keys contained in this map, sorted in
             *         ascending order
             */
            const Set<K>& keySet() const override {
                return sequencedKeySet();
            }

            /**
             * Returns a @b Collection view of the values contained in this map.
             * The collection's iterator returns the values in ascending order
             * of the corresponding keys.
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
             * @return a collection view of the values contained in this map,
             *         sorted in ascending key order
             */
            Collection<V>& values() override {
                return sequencedValues();
            }

            /**
             * Returns a @b Collection view of the values contained in this map.
             * The collection's iterator returns the values in ascending order
             * of the corresponding keys.
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
             * @return a collection view of the values contained in this map,
             *         sorted in ascending key order
             */
            const Collection<V>& values() const override {
                return sequencedValues();
            }

            /**
             * Returns a @b Set view of the mappings contained in this map.
             * The set's iterator returns the entries in ascending key order.
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
             * @return a set view of the mappings contained in this map,
             *         sorted in ascending key order
             */
            Set<Entry>& entrySet() override {
                return sequencedEntrySet();
            }

            /**
             * Returns a @b Set view of the mappings contained in this map.
             * The set's iterator returns the entries in ascending key order.
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
             * @return a set view of the mappings contained in this map,
             *         sorted in ascending key order
             */
            const Set<Entry>& entrySet() const override {
                return sequencedEntrySet();
            }

            /**
             * Returns a reverse-ordered @b view of this map.
             * The encounter order of mappings in the returned view is the inverse of the encounter
             * order of mappings in this map. The reverse ordering affects all order-sensitive operations,
             * including those on the view collections of the returned view. If the implementation permits
             * modifications to this view, the modifications "write through" to the underlying map.
             * Changes to the underlying map might or might not be visible in this reversed view,
             * depending upon the implementation.
             *
             * @return a reverse-ordered view of this map
             */
            SortedMap const& reversed() const override {
                if (reverse == null)
                    reverse = &UNSAFE::newInstance<Reverse>(*this);
                return *CORE_DCAST(SortedView, reverse);
            }

            /**
             * Returns a reverse-ordered @b view of this map.
             * The encounter order of mappings in the returned view is the inverse of the encounter
             * order of mappings in this map. The reverse ordering affects all order-sensitive operations,
             * including those on the view collections of the returned view. If the implementation permits
             * modifications to this view, the modifications "write through" to the underlying map.
             * Changes to the underlying map might or might not be visible in this reversed view,
             * depending upon the implementation.
             *
             * @return a reverse-ordered view of this map
             */
            SortedMap& reversed() override {
                if (reverse == null)
                    reverse = &UNSAFE::newInstance<Reverse>(*this);
                return *CORE_DCAST(SortedView, reverse);
            }

            SequencedSet<K>& sequencedKeySet() override {
                return SequencedMap::sequencedKeySet();
            }

            const SequencedSet<K>& sequencedKeySet() const override {
                return SequencedMap::sequencedKeySet();
            }

            SequencedCollection<V>& sequencedValues() override {
                return SequencedMap::sequencedValues();
            }

            const SequencedCollection<V>& sequencedValues() const override {
                return SequencedMap::sequencedValues();
            }

            SequencedSet<Entry>& sequencedEntrySet() override = 0;

            const SequencedSet<Entry>& sequencedEntrySet() const override = 0;

            /**
             * Destroy this sorted map, and him dependant views such as
             * reversed order view.
             */
            ~SortedMap() override {
                if (reverse != null) {
                    UNSAFE::deleteRegInstance(*reverse);
                    reverse = null;
                }
            }

        protected:
            // :::::::::::::::::::: Map Views Fields :::::::::::::::::::::::::::
            CORE_IMPORT_FIELD_OR_METHOD(SequencedMap, reverse);

            // ::::::::::::::::::::: SortedMap views Destructor Utilities :::::::::::::::::::::::::::::

            static void detachKeys(SortedMap const& map) { SequencedMap::detachKeys(map); }

            static void detachVals(SortedMap const& map) { SequencedMap::detachVals(map); }

            static void detachEntries(SortedMap const& map) { SequencedMap::detachEntries(map); }

            static void detachReverse(SortedMap const& map) { SequencedMap::detachReverse(map); }

        private:
            // :::::::::::::::::::: Map Views Class :::::::::::::::::::::::::::
            /**
             * The Reverse Order view
             */
            class Reverse;

            /**
             * The sub map base view
             */
            class SubMap;

            /**
             * The majored sub map view
             */
            class HeadMap;

            /**
             * The minored sub map view
             */
            class TailMap;

            /**
             * The bounded sub map view
             */
            class BoundedMap;
        };

        template <class K, class V>
        class SortedMap<K, V>::Reverse final : public SortedMap<K, V> {
            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);

            SortedMap<K, V>& m;

        public:
            CORE_EXPLICIT Reverse(SortedMap const& m) : m((SortedMap&) m) {}

            gint size() const override {
                try {
                    return m.size();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    return m.isEmpty();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsKey(const K& key) const override {
                try {
                    return m.containsKey(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsValue(const V& value) const override {
                try {
                    return m.containsValue(value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& get(const K& key) override {
                try {
                    return m.get(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& get(const K& key) const override {
                try {
                    return m.get(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& put(const K& key, const V& value) override {
                try {
                    return m.put(key, value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& remove(const K& key) override {
                try {
                    return m.remove(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    return m.clear();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void putAll(const Map& map) override {
                try {
                    return m.putAll(map);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& getOrDefault(const K& key, const V& defaultValue) override {
                try {
                    return m.getOrDefault(key, defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& getOrDefault(const K& key, const V& defaultValue) const override {
                try {
                    return m.getOrDefault(key, defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& getOrNull(const K& key) override {
                try {
                    return m.getOrNull(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Object& getOrNull(const K& key) const override {
                try {
                    return m.getOrNull(key);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& putIfAbsent(const K& key, const V& value) override {
                try {
                    return m.putIfAbsent(key, value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const K& key, const V& value) override {
                try {
                    return m.remove(key, value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool replace(const K& key, const V& oldValue, const V& newValue) override {
                try {
                    return m.replace(key, oldValue, newValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& replace(const K& key, const V& value) override {
                try {
                    return m.replace(key, value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& pollFirstEntry() override {
                try {
                    return m.pollLastEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& pollLastEntry() override {
                try {
                    return m.pollFirstEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& putFirst(const K& k, const V& v) override {
                try {
                    return m.putLast(k, v);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& putLast(const K& k, const V& v) override {
                try {
                    return m.putFirst(k, v);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Comparator<K>& comparator() const override {
                try {
                    return m.comparator().reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& firstEntry() const override {
                try {
                    return m.lastEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& firstEntry() override {
                try {
                    return m.lastEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& lastEntry() const override {
                try {
                    return m.firstEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& lastEntry() override {
                try {
                    return m.firstEntry();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& firstKey() const override {
                try {
                    return m.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& firstKey() override {
                try {
                    return m.lastKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& lastKey() const override {
                try {
                    return m.firstKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& lastKey() override {
                try {
                    return m.firstKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedMap<K, V>& reversed() const override {
                return m;
            }

            SortedMap<K, V>& reversed() override {
                return m;
            }

            gbool equals(const Object& o) const override {
                if (this == &o)
                    return true;

                try {
                    return m.equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Set<K>& keySet() override {
                return sequencedKeySet();
            }

            const Set<K>& keySet() const override {
                return sequencedKeySet();
            }

            Collection<V>& values() override {
                return sequencedValues();
            }

            const Collection<V>& values() const override {
                return sequencedValues();
            }

            Set<Entry>& entrySet() override {
                return sequencedEntrySet();
            }

            const Set<Entry>& entrySet() const override {
                return sequencedEntrySet();
            }

            SequencedSet<K>& sequencedKeySet() override {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(m);
                return CORE_XCAST(SequencedSet<K>, *keys);
            }

            const SequencedSet<K>& sequencedKeySet() const override {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(m);
                return CORE_XCAST(SequencedSet<K>, *keys);
            }

            SequencedCollection<V>& sequencedValues() override {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(m);
                return CORE_XCAST(SequencedCollection<V>, *vals);
            }

            const SequencedCollection<V>& sequencedValues() const override {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(m);
                return CORE_XCAST(SequencedCollection<V>, *vals);
            }

            SequencedSet<Entry>& sequencedEntrySet() override {
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(m);
                return CORE_XCAST(SequencedSet<Entry>, *entries);
            }

            const SequencedSet<Entry>& sequencedEntrySet() const override {
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(m);
                return CORE_XCAST(SequencedSet<Entry>, *entries);
            }

            const SortedMap<K, V>& subMap(const K& fromKey, const K& toKey) const override {
                try {
                    return m.subMap(toKey, fromKey).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedMap<K, V>& subMap(const K& fromKey, const K& toKey) override {
                try {
                    return m.subMap(toKey, fromKey).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedMap<K, V>& headMap(const K& toKey) const override {
                try {
                    return m.tailMap(toKey).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedMap<K, V>& headMap(const K& toKey) override {
                try {
                    return m.tailMap(toKey).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SortedMap<K, V>& tailMap(const K& fromKey) const override {
                try {
                    return m.headMap(fromKey).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SortedMap<K, V>& tailMap(const K& fromKey) override {
                try {
                    return m.headMap(fromKey).reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Destroy this reversed view and detach him from
             * base map.
             */
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

                detachReverse(m);
            }

        private:
            // ::::::::::::::::: Map Views class :::::::::::::::::::::::

            /**
             * The keys view
             */
            class KeySet;

            /**
             * The values view
             */
            class Values;

            /**
             * The entries view
             */
            class EntrySet;
        };

        template <class K, class V>
        class SortedMap<K, V>::Reverse::KeySet final : public SequencedSet<K> {
            SortedMap<K, V>& m;

        public:
            CORE_EXPLICIT KeySet(SortedMap const& m) : m((SortedMap&) m) {}

            gint size() const override {
                try { return m.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return m.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const K& val) const override {
                try { return m.keySet().contains(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const K& val) override {
                try { return m.keySet().remove(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try { return m.keySet().clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<K>& iterator() override {
                CORE_ALIAS(KEY, typename Class<K>::Pointer);
                class Itr final : public Iterator<K> {
                    SortedMap<K, V>& m;
                    SortedView rem = null;
                    KEY last = null;

                public:
                    CORE_EXPLICIT Itr(SortedMap<K, V>& m) : m(m), rem(&m) {}

                    gbool hasNext() const override {
                        try { return !rem->isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    K& next() override {
                        if (rem->isEmpty())
                            NoSuchElementException().throws($ftrace());
                        try {
                            K& lastKey = rem->lastKey();
                            SortedView view = &rem->headMap(lastKey);

                            UNSAFE::swapValues(rem, view);
                            if (view && view != &m)
                                UNSAFE::deleteInstance(*view);

                            last = &lastKey;

                            return lastKey;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());
                        try {
                            K& lastKey = *last;
                            last = null;
                            m.remove(lastKey);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &m == &it.m && rem->size() == it.rem->size();
                    }

                    ~Itr() override {
                        if (rem && rem != &m)
                            UNSAFE::deleteInstance(*rem);
                        rem = null;
                        last = null;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(m);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& iterator() const override {
                CORE_ALIAS(KEY, typename Class<K>::Pointer);
                class Itr final : public Iterator2<K> {
                    SortedMap<K, V>& m;
                    SortedView rem = null;
                    KEY last = null;

                public:
                    CORE_EXPLICIT Itr(SortedMap<K, V>& m) : m(m), rem(&m) {}

                    gbool hasNext() const override {
                        try { return !rem->isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    K const& next() override {
                        if (rem->isEmpty())
                            NoSuchElementException().throws($ftrace());
                        try {
                            K& lastKey = rem->lastKey();
                            SortedView view = &rem->headMap(lastKey);

                            UNSAFE::swapValues(rem, view);
                            if (view && view != &m)
                                UNSAFE::deleteInstance(*view);

                            last = &lastKey;

                            return lastKey;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &m == &it.m && rem->size() == it.rem->size();
                    }

                    ~Itr() override {
                        if (rem && rem != &m)
                            UNSAFE::deleteInstance(*rem);
                        rem = null;
                        last = null;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(m); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedSet<K>& reversed() const override {
                return m.sequencedKeySet();
            }

            SequencedSet<K>& reversed() override {
                return m.sequencedKeySet();
            }

            K& firstElement() override {
                try {
                    return m.sequencedKeySet().lastElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& firstElement() const override {
                try {
                    return m.sequencedKeySet().lastElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& lastElement() override {
                try {
                    return m.sequencedKeySet().firstElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& lastElement() const override {
                try {
                    return m.sequencedKeySet().firstElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& removeFirst() override {
                try {
                    return m.sequencedKeySet().removeLast();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& removeLast() override {
                try {
                    return m.sequencedKeySet().removeFirst();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~KeySet() override {
                detachKeys(m.reversed());
            }
        };

        template <class K, class V>
        class SortedMap<K, V>::Reverse::Values final : public SequencedCollection<V> {
            SortedMap<K, V>& m;

        public:
            CORE_EXPLICIT Values(SortedMap<K, V> const& m) : m((SortedMap&) m) {}

            gint size() const override {
                try { return m.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return m.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const V& val) const override {
                try { return m.values().contains(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const V& val) override {
                try { return m.values().remove(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try { return m.values().clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<V>& iterator() override {
                class Itr final : public Iterator<V> {
                    SortedMap<K, V>& m;
                    Iterator<K>& kit; // The key iterator

                public:
                    CORE_EXPLICIT Itr(SortedMap<K, V>& m, Iterator<K>& kit): m(m), kit(kit) {}

                    gbool hasNext() const override {
                        try { return kit.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    V& next() override {
                        try { return m.get(kit.next()); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        try { return kit.remove(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;
                        Itr const& it = CORE_XCAST(Itr const, o);
                        try { return kit == it.kit; } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(kit);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(m, m.reversed().keySet().iterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& iterator() const override {
                class Itr final : public Iterator2<V> {
                    SortedMap<K, V>& m;
                    Iterator<K>& kit;

                public:
                    CORE_EXPLICIT Itr(SortedMap<K, V>& m, Iterator<K>& kit): m(m), kit(kit) {}

                    gbool hasNext() const override {
                        try { return kit.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    V const& next() override {
                        try { return m.get(kit.next()); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;
                        Itr const& it = CORE_XCAST(Itr const, o);
                        try { return kit == it.kit; } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(m, m.reversed().keySet().iterator());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedCollection<V>& reversed() const override {
                return m.sequencedValues();
            }

            SequencedCollection<V>& reversed() override {
                return m.sequencedValues();
            }

            V& firstElement() override {
                try {
                    return m.sequencedValues().lastElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& firstElement() const override {
                try {
                    return m.sequencedValues().lastElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& lastElement() override {
                try {
                    return m.sequencedValues().firstElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& lastElement() const override {
                try {
                    return m.sequencedValues().firstElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& removeFirst() override {
                try {
                    return m.sequencedValues().removeLast();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& removeLast() override {
                try {
                    return m.sequencedValues().removeFirst();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~Values() override {
                detachVals(m.reversed());
            }
        };

        template <class K, class V>
        class SortedMap<K, V>::Reverse::EntrySet final : public SequencedSet<Entry> {
            SortedMap<K, V>& m;

        public:
            CORE_EXPLICIT EntrySet(SortedMap const& m) : m((SortedMap&) m) {}

            gint size() const override {
                try { return m.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return m.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const Entry& val) const override {
                try { return m.entrySet().contains(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(const Entry& val) override {
                try { return m.entrySet().remove(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try { return m.entrySet().clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<Entry>& iterator() override {
                CORE_ALIAS(ENTRY, typename Class<Entry>::Pointer);
                class Itr final : public Iterator<Entry> {
                    SortedMap<K, V>& m;
                    SortedView rem = null;
                    ENTRY last = null;

                public:
                    CORE_EXPLICIT Itr(SortedMap<K, V>& m) : m(m), rem(&m) {}

                    gbool hasNext() const override {
                        try { return !rem->isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Entry& next() override {
                        if (rem->isEmpty())
                            NoSuchElementException().throws($ftrace());
                        try {
                            Entry& lastEntry = rem->lastEntry();
                            SortedView view = &rem->headMap(lastEntry.getKey());

                            UNSAFE::swapValues(rem, view);
                            if (view && view != &m)
                                UNSAFE::deleteInstance(*view);

                            last = &lastEntry;

                            return lastEntry;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        if (!last)
                            IllegalStateException().throws($ftrace());
                        try {
                            Entry& lastEntry = *last;
                            last = null;
                            m.remove(lastEntry.getKey());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &m == &it.m && rem->size() == it.rem->size();
                    }

                    ~Itr() override {
                        if (rem && rem != &m)
                            UNSAFE::deleteInstance(*rem);
                        rem = null;
                        last = null;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(m); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<Entry>& iterator() const override {
                CORE_ALIAS(ENTRY, typename Class<Entry>::Pointer);
                class Itr final : public Iterator2<Entry> {
                    SortedMap<K, V>& m;
                    SortedView rem = null;
                    ENTRY last = null;

                public:
                    CORE_EXPLICIT Itr(SortedMap<K, V>& m) : m(m), rem(&m) {}

                    gbool hasNext() const override {
                        try { return !rem->isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Entry const& next() override {
                        if (rem->isEmpty())
                            NoSuchElementException().throws($ftrace());
                        try {
                            Entry& lastEntry = rem->lastEntry();
                            SortedView view = &rem->headMap(lastEntry.getKey());

                            UNSAFE::swapValues(rem, view);
                            if (view && view != &m)
                                UNSAFE::deleteInstance(*view);

                            last = &lastEntry;

                            return lastEntry;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const& it = CORE_XCAST(Itr const, o);
                        return &m == &it.m && rem->size() == it.rem->size();
                    }

                    ~Itr() override {
                        if (rem && rem != &m)
                            UNSAFE::deleteInstance(*rem);
                        rem = null;
                        last = null;
                    }
                };

                try { return UNSAFE::newInstance<Itr>(m); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedSet<Entry>& reversed() const override {
                return m.sequencedEntrySet();
            }

            SequencedSet<Entry>& reversed() override {
                return m.sequencedEntrySet();
            }

            Entry& firstElement() override {
                try {
                    return m.sequencedEntrySet().lastElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& firstElement() const override {
                try {
                    return m.sequencedEntrySet().lastElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& lastElement() override {
                try {
                    return m.sequencedEntrySet().firstElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Entry& lastElement() const override {
                try {
                    return m.sequencedEntrySet().firstElement();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& removeFirst() override {
                try {
                    return m.sequencedEntrySet().removeLast();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Entry& removeLast() override {
                try {
                    return m.sequencedEntrySet().removeFirst();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~EntrySet() override {
                detachEntries(m.reversed());
            }
        };
    } // util
} // core

#endif //CORE24_SORTEDMAP_H
