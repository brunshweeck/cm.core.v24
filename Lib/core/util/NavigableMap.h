//
// Created by brunshweeck on 5 août 2024.
//

#ifndef CORE24_NAVIGABLEMAP_H
#define CORE24_NAVIGABLEMAP_H

#include <core/util/SortedMap.h>

namespace core {
    namespace util {
        /**
         * A @b SortedMap extended with navigation methods returning the
         * closest matches for given search targets. Methods
         * @b lowerEntry, @b floorEntry, @b ceilingEntry,
         * and @b higherEntry return @c Map::Entry objects
         * associated with keys respectively less than, less than or equal,
         * greater than or equal, and greater than a given key, throwing
         * @c NoSuchElementException if there is no such key.  Similarly, methods
         * @b lowerKey, @b floorKey, @b ceilingKey, and
         * @b higherKey return only the associated keys. All of these
         * methods are designed for locating, not traversing entries.
         * <p>
         * A @c NavigableMap may be accessed and traversed in either
         * ascending or descending key order.  The @b descendingMap
         * method returns a view of the map with the senses of all relational
         * and directional methods inverted. The performance of ascending
         * operations and views is likely to be faster than that of descending
         * ones.  Methods
         * <b> subMap(K, gbool, K, gbool)</b>,
         * <b> headMap(K, gbool)</b>, and
         * <b> tailMap(K, gbool)</b>
         * differ from the like-named @c SortedMap methods in accepting
         * additional arguments describing whether lower and upper bounds are
         * inclusive versus exclusive.  Sub-maps of any @c NavigableMap
         * must implement the @c NavigableMap interface.
         * </p>
         * <p>
         * This interface additionally defines methods @b firstEntry,
         * @b pollFirstEntry, @b lastEntry, and
         * @b pollLastEntry that return and/or remove the least and
         * greatest mappings, if any exist, else returning @c null or throwing
         * @c NoSuchElementException.
         * </p>
         * <p>
         * The methods @b ceilingEntry, @b firstEntry, @b floorEntry, @b higherEntry,
         * @b lastEntry, @b lowerEntry, @b pollFirstEntry, and @b pollLastEntry
         * return @c Map::Entry instances that represent snapshots of mappings as
         * of the time of the call. They do <em>not</em> support mutation of the
         * underlying map via the optional @b setValue method.
         * </p>
         * <p>
         * Methods <b> subMap(K, K) </b>, <b> headMap(K) </b>, and
         * <b> tailMap(K)</b>
         * are specified to return @c SortedMap to allow existing
         * implementations of @c SortedMap to be compatibly retrofitted to
         * implement @c NavigableMap, but extensions and implementations
         * of this interface are encouraged to override these methods to return
         * @c NavigableMap.  Similarly,
         * @b keySet() can be overridden to return @b NavigableSet.
         * </p>
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         */
        template <class K, class V>
        class NavigableMap : public virtual SortedMap<K, V> {
            CORE_ALIAS(Entry, util::Entry<K, V>);
            CORE_ALIAS(Map, util::Map<K, V>);
            CORE_ALIAS(SequencedMap, util::SequencedMap<K, V>);
            CORE_ALIAS(SortedMap, util::SortedMap<K, V>);

            CORE_ALIAS(Entries, typename Map::Entries);

        public:
            /**
             * Returns a key-value mapping associated with the greatest key
             * strictly less than the given key, or thrown @c MissingKeyException if there is
             * no such key.
             *
             * @param key the key
             * @return an entry with the greatest key less than @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual Entry const& lowerEntry(K const& key) const = 0;

            /**
             * Returns a key-value mapping associated with the greatest key
             * strictly less than the given key, or thrown @c MissingKeyException if there is
             * no such key.
             *
             * @param key the key
             * @return an entry with the greatest key less than @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual Entry& lowerEntry(K const& key) = 0;

            /**
             * Returns the greatest key strictly less than the given key, or
             * thrown @c MissingKeyException if there is no such key.
             *
             * @param key the key
             * @return the greatest key less than @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual K const& lowerKey(K const& key) const = 0;

            /**
             * Returns the greatest key strictly less than the given key, or
             * thrown @c MissingKeyException if there is no such key.
             *
             * @param key the key
             * @return the greatest key less than @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual K& lowerKey(K const& key) = 0;

            /**
             * Returns a key-value mapping associated with the greatest key
             * less than or equal to the given key, or thrown @c MissingKeyException if there
             * is no such key.
             *
             * @param key the key
             * @return an entry with the greatest key less than or equal to
             *         @c key, or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual Entry const& floorEntry(K const& key) const = 0;

            /**
             * Returns a key-value mapping associated with the greatest key
             * less than or equal to the given key, or thrown @c MissingKeyException if there
             * is no such key.
             *
             * @param key the key
             * @return an entry with the greatest key less than or equal to
             *         @c key, or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual Entry& floorEntry(K const& key) = 0;

            /**
             * Returns the greatest key less than or equal to the given key,
             * or thrown @c MissingKeyException if there is no such key.
             *
             * @param key the key
             * @return the greatest key less than or equal to @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual K const& floorKey(K const& key) const = 0;

            /**
             * Returns the greatest key less than or equal to the given key,
             * or thrown @c MissingKeyException if there is no such key.
             *
             * @param key the key
             * @return the greatest key less than or equal to @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual K& floorKey(K const& key) = 0;

            /**
             * Returns a key-value mapping associated with the least key
             * greater than or equal to the given key, or thrown @c MissingKeyException if
             * there is no such key.
             *
             * @param key the key
             * @return an entry with the least key greater than or equal to
             *         @c key, or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual Entry const& ceilingEntry(K const& key) const = 0;

            /**
             * Returns a key-value mapping associated with the least key
             * greater than or equal to the given key, or thrown @c MissingKeyException if
             * there is no such key.
             *
             * @param key the key
             * @return an entry with the least key greater than or equal to
             *         @c key, or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual Entry& ceilingEntry(K const& key) = 0;

            /**
             * Returns the least key greater than or equal to the given key,
             * or thrown @c MissingKeyException if there is no such key.
             *
             * @param key the key
             * @return the least key greater than or equal to @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual K const& ceilingKey(K const& key) const = 0;

            /**
             * Returns the least key greater than or equal to the given key,
             * or thrown @c MissingKeyException if there is no such key.
             *
             * @param key the key
             * @return the least key greater than or equal to @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual K& ceilingKey(K const& key) = 0;

            /**
             * Returns a key-value mapping associated with the least key
             * strictly greater than the given key, or thrown @c MissingKeyException if there
             * is no such key.
             *
             * @param key the key
             * @return an entry with the least key greater than @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual Entry const& higherEntry(K const& key) const = 0;

            /**
             * Returns a key-value mapping associated with the least key
             * strictly greater than the given key, or thrown @c MissingKeyException if there
             * is no such key.
             *
             * @param key the key
             * @return an entry with the least key greater than @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual Entry& higherEntry(K const& key) = 0;

            /**
             * Returns the least key strictly greater than the given key, or
             * thrown @c MissingKeyException if there is no such key.
             *
             * @param key the key
             * @return the least key greater than @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual K const& higherKey(K const& key) const = 0;

            /**
             * Returns the least key strictly greater than the given key, or
             * thrown @c MissingKeyException if there is no such key.
             *
             * @param key the key
             * @return the least key greater than @c key,
             *         or thrown @c MissingKeyException if there is no such key
             * @throws ClassCastException if the specified key cannot be compared
             *         with the keys currently in the map
             * @throws IllegalArgumentException if the specified key is null
             *         and this map does not permit null keys
             */
            virtual K& higherKey(K const& key) = 0;

            /**
             * Returns a key-value mapping associated with the least
             * key in this map, or thrown NoSuchElementException if the map is empty.
             *
             * @return an entry with the least key,
             *         or thrown NoSuchElementException if this map is empty
             */
            Entry const& firstEntry() const override = 0;

            /**
             * Returns a key-value mapping associated with the least
             * key in this map, or thrown NoSuchElementException if the map is empty.
             *
             * @return an entry with the least key,
             *         or thrown NoSuchElementException if this map is empty
             */
            Entry& firstEntry() override = 0;

            /**
             * Returns a key-value mapping associated with the greatest
             * key in this map, or thrown NoSuchElementException if the map is empty.
             *
             * @return an entry with the greatest key,
             *         or thrown NoSuchElementException if this map is empty
             */
            Entry const& lastEntry() const override = 0;

            /**
             * Returns a key-value mapping associated with the greatest
             * key in this map, or thrown NoSuchElementException if the map is empty.
             *
             * @return an entry with the greatest key,
             *         or thrown NoSuchElementException if this map is empty
             */
            Entry& lastEntry() override = 0;

            /**
             * Returns a reverse order view of the mappings contained in this map.
             * The descending map is backed by this map, so changes to the map are
             * reflected in the descending map, and vice versa.  If either map is
             * modified while an iteration over a collection view of either map
             * is in progress (except through the iterator's own @c remove
             * operation), the results of the iteration are undefined.
             *
             * <p>The returned map has an ordering equivalent to
             * @b Collections.reverseOrder @c (comparator()).
             * The expression @c m.descendingMap().descendingMap() returns a
             * view of @c m essentially equivalent to @c m.
             *
             * @return a reverse order view of this map
             */
            virtual NavigableMap const& descendingMap() const = 0;

            /**
             * Returns a reverse order view of the mappings contained in this map.
             * The descending map is backed by this map, so changes to the map are
             * reflected in the descending map, and vice versa.  If either map is
             * modified while an iteration over a collection view of either map
             * is in progress (except through the iterator's own @c remove
             * operation), the results of the iteration are undefined.
             *
             * <p>The returned map has an ordering equivalent to
             * @b Collections.reverseOrder @c (comparator()).
             * The expression @c m.descendingMap().descendingMap() returns a
             * view of @c m essentially equivalent to @c m.
             *
             * @return a reverse order view of this map
             */
            virtual NavigableMap<K, V>& descendingMap() = 0;

            /**
             * Returns a @b NavigableSet view of the keys contained in this map.
             * The set's iterator returns the keys in ascending order.
             * The set is backed by the map, so changes to the map are reflected in
             * the set, and vice versa.  If the map is modified while an iteration
             * over the set is in progress (except through the iterator's own @c
             * remove operation), the results of the iteration are undefined.  The
             * set supports element removal, which removes the corresponding mapping
             * from the map, via the @c Iterator::remove, @c Set::remove,
             * @c removeAll, @c retainAll, and @c clear operations.
             * It does not support the @c add or @c addAll operations.
             *
             * @return a navigable set view of the keys in this map
             */
            virtual NavigableSet<K> const& navigableKeySet() const = 0;

            /**
             * Returns a @b NavigableSet view of the keys contained in this map.
             * The set's iterator returns the keys in ascending order.
             * The set is backed by the map, so changes to the map are reflected in
             * the set, and vice versa.  If the map is modified while an iteration
             * over the set is in progress (except through the iterator's own @c
             * remove operation), the results of the iteration are undefined.  The
             * set supports element removal, which removes the corresponding mapping
             * from the map, via the @c Iterator::remove, @c Set::remove,
             * @c removeAll, @c retainAll, and @c clear operations.
             * It does not support the @c add or @c addAll operations.
             *
             * @return a navigable set view of the keys in this map
             */
            virtual NavigableSet<K>& navigableKeySet() = 0;

            /**
             * Returns a reverse order @b NavigableSet view of the keys contained in this map.
             * The set's iterator returns the keys in descending order.
             * The set is backed by the map, so changes to the map are reflected in
             * the set, and vice versa.  If the map is modified while an iteration
             * over the set is in progress (except through the iterator's own @c
             * remove operation), the results of the iteration are undefined.  The
             * set supports element removal, which removes the corresponding mapping
             * from the map, via the @c Iterator::remove, @c Set::remove,
             * @c removeAll, @c retainAll, and @c clear operations.
             * It does not support the @c add or @c addAll operations.
             *
             * @return a reverse order navigable set view of the keys in this map
             */
            virtual NavigableSet<K> const& descendingKeySet() const {
                try { return descendingMap().navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a reverse order @b NavigableSet view of the keys contained in this map.
             * The set's iterator returns the keys in descending order.
             * The set is backed by the map, so changes to the map are reflected in
             * the set, and vice versa.  If the map is modified while an iteration
             * over the set is in progress (except through the iterator's own @c
             * remove operation), the results of the iteration are undefined.  The
             * set supports element removal, which removes the corresponding mapping
             * from the map, via the @c Iterator::remove, @c Set::remove,
             * @c removeAll, @c retainAll, and @c clear operations.
             * It does not support the @c add or @c addAll operations.
             *
             * @return a reverse order navigable set view of the keys in this map
             */
            virtual NavigableSet<K>& descendingKeySet() {
                try { return descendingMap().navigableKeySet(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this map whose keys range from
             * @c fromKey to @c toKey.  If @c fromKey and
             * @c toKey are equal, the returned map is empty unless
             * @c fromInclusive and @c toInclusive are both true.  The
             * returned map is backed by this map, so changes in the returned map are
             * reflected in this map, and vice-versa.  The returned map supports all
             * optional map operations that this map supports.
             *
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range, or to construct a
             * submap either of whose endpoints lie outside its range.
             * </p>
             * @param fromKey low endpoint of the keys in the returned map
             * @param fromInclusive @c true if the low endpoint
             *        is to be included in the returned view
             * @param toKey high endpoint of the keys in the returned map
             * @param toInclusive @c true if the high endpoint
             *        is to be included in the returned view
             * @return a view of the portion of this map whose keys range from
             *         @c fromKey to @c toKey
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
            virtual NavigableMap const& subMap(K const& fromKey, gbool fromInclusive,
                                               K const& toKey, gbool toInclusive) const = 0;

            /**
             * Returns a view of the portion of this map whose keys range from
             * @c fromKey to @c toKey.  If @c fromKey and
             * @c toKey are equal, the returned map is empty unless
             * @c fromInclusive and @c toInclusive are both true.  The
             * returned map is backed by this map, so changes in the returned map are
             * reflected in this map, and vice-versa.  The returned map supports all
             * optional map operations that this map supports.
             *
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range, or to construct a
             * submap either of whose endpoints lie outside its range.
             * </p>
             * @param fromKey low endpoint of the keys in the returned map
             * @param fromInclusive @c true if the low endpoint
             *        is to be included in the returned view
             * @param toKey high endpoint of the keys in the returned map
             * @param toInclusive @c true if the high endpoint
             *        is to be included in the returned view
             * @return a view of the portion of this map whose keys range from
             *         @c fromKey to @c toKey
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
            virtual NavigableMap& subMap(K const& fromKey, gbool fromInclusive, K const& toKey, gbool toInclusive) = 0;

            /**
             * Returns a view of the portion of this map whose keys are less than (or
             * equal to, if @c inclusive is true) @c toKey.  The returned
             * map is backed by this map, so changes in the returned map are reflected
             * in this map, and vice-versa.  The returned map supports all optional
             * map operations that this map supports.
             *
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param toKey high endpoint of the keys in the returned map
             * @param inclusive @c true if the high endpoint
             *        is to be included in the returned view
             * @return a view of the portion of this map whose keys are less than
             *         (or equal to, if @c inclusive is true) @c toKey
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
            virtual NavigableMap const& headMap(K const& toKey, gbool inclusive) const = 0;

            /**
             * Returns a view of the portion of this map whose keys are less than (or
             * equal to, if @c inclusive is true) @c toKey.  The returned
             * map is backed by this map, so changes in the returned map are reflected
             * in this map, and vice-versa.  The returned map supports all optional
             * map operations that this map supports.
             *
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param toKey high endpoint of the keys in the returned map
             * @param inclusive @c true if the high endpoint
             *        is to be included in the returned view
             * @return a view of the portion of this map whose keys are less than
             *         (or equal to, if @c inclusive is true) @c toKey
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
            virtual NavigableMap& headMap(K const& toKey, gbool inclusive) = 0;

            /**
             * Returns a view of the portion of this map whose keys are greater than (or
             * equal to, if @c inclusive is true) @c fromKey.  The returned
             * map is backed by this map, so changes in the returned map are reflected
             * in this map, and vice-versa.  The returned map supports all optional
             * map operations that this map supports.
             *
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param fromKey low endpoint of the keys in the returned map
             * @param inclusive @c true if the low endpoint
             *        is to be included in the returned view
             * @return a view of the portion of this map whose keys are greater than
             *         (or equal to, if @c inclusive is true) @c fromKey
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
            virtual NavigableMap const& tailMap(K const& fromKey, gbool inclusive) const = 0;

            /**
             * Returns a view of the portion of this map whose keys are greater than (or
             * equal to, if @c inclusive is true) @c fromKey.  The returned
             * map is backed by this map, so changes in the returned map are reflected
             * in this map, and vice-versa.  The returned map supports all optional
             * map operations that this map supports.
             *
             * <p>
             * The returned map will throw an @c IllegalArgumentException
             * on an attempt to insert a key outside its range.
             * </p>
             * @param fromKey low endpoint of the keys in the returned map
             * @param inclusive @c true if the low endpoint
             *        is to be included in the returned view
             * @return a view of the portion of this map whose keys are greater than
             *         (or equal to, if @c inclusive is true) @c fromKey
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
            virtual NavigableMap& tailMap(K const& fromKey, gbool inclusive) = 0;

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
             * <p>
             * Equivalent to @code subMap(fromKey, true, toKey, false) @endcode.
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
            SortedMap const& subMap(K const& fromKey, K const& toKey) const override {
                try { return subMap(fromKey, true, toKey, false); } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

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
             * <p>
             * Equivalent to @code subMap(fromKey, true, toKey, false) @endcode.
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
            SortedMap& subMap(K const& fromKey, K const& toKey) override {
                try { return subMap(fromKey, true, toKey, false); } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

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
             * <p>
             * Equivalent to @code headMap(toKey, false) @endcode .
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
            SortedMap const& headMap(K const& toKey) const override {
                try { return headMap(toKey, false); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

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
             * <p>
             * Equivalent to @code headMap(toKey, false) @endcode .
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
            SortedMap& headMap(K const& toKey) override {
                try { return headMap(toKey, false); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

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
             * <p>
             * Equivalent to @code tailMap(fromKey, true) @endcode .
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
            SortedMap const& tailMap(K const& fromKey) const override {
                try { return tailMap(fromKey, true); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

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
             * <p>
             * Equivalent to @code tailMap(fromKey, true) @endcode .
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
            SortedMap& tailMap(K const& fromKey) override {
                try { return tailMap(fromKey, true); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a reverse-ordered @b view of this map.
             * The encounter order of mappings in the returned view is the inverse of the encounter
             * order of mappings in this map. The reverse ordering affects all order-sensitive operations,
             * including those on the view collections of the returned view. If the implementation permits
             * modifications to this view, the modifications "write through" to the underlying map.
             * Changes to the underlying map might or might not be visible in this reversed view,
             * depending upon the implementation.
             * <p>
             * This method is equivalent to @b descendingMap.
             * </p>
             * @note
             * The implementation in this interface returns the result of calling the
             * @c descendingMap method.
             *
             * @return a reverse-ordered view of this map, as a @c NavigableMap
             */
            NavigableMap<K, V> const& reversed() const override {
                try { return descendingMap(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a reverse-ordered @b view of this map.
             * The encounter order of mappings in the returned view is the inverse of the encounter
             * order of mappings in this map. The reverse ordering affects all order-sensitive operations,
             * including those on the view collections of the returned view. If the implementation permits
             * modifications to this view, the modifications "write through" to the underlying map.
             * Changes to the underlying map might or might not be visible in this reversed view,
             * depending upon the implementation.
             * <p>
             * This method is equivalent to @b descendingMap.
             * </p>
             * @note
             * The implementation in this interface returns the result of calling the
             * @c descendingMap method.
             *
             * @return a reverse-ordered view of this map, as a @c NavigableMap
             */
            NavigableMap<K, V>& reversed() override {
                try { return descendingMap(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };
    } // util
} // core

#endif //CORE24_NAVIGABLEMAP_H
