//
// Created by brunshweeck on 23 juil. 2024.
//

#ifndef CORE24_MAP_H
#define CORE24_MAP_H

#include <core/lang/OutOfMemoryError.h>
#include <core/util/ConcurrentModificationException.h>
#include <core/util/Entry.h>
#include <core/util/MissingKeyException.h>
#include <core/util/Set.h>
#include <core/util/StringJoiner.h>
#include <core/function/BiConsumer.h>
#include <core/function/BiFunction.h>

namespace core {
    namespace util {
        /**
         * An object that maps keys to values.  A map cannot contain duplicate keys;
         * each key can map to at most one value.
         *
         * <p>
         * The @c Map interface provides three <i>collection views</i>, which
         * allow a map's contents to be viewed as a set of keys, collection of values,
         * or set of key-value mappings.  The <i>order</i> of a map is defined as
         * the order in which the iterators on the map's collection views return their
         * elements.  Some map implementations, like the @c TreeMap class, make
         * specific guarantees as to their encounter order; others, like the
         * @c HashMap class, do not. Maps with a defined encounter order
         * are generally subtypes of the @c SequencedMap interface.
         * </p>
         * <p>
         * Note: great care must be exercised if mutable objects are used as map
         * keys.  The behavior of a map is not specified if the value of an object is
         * changed in a manner that affects @c equals comparisons while the
         * object is a key in the map.  A special case of this prohibition is that it
         * is not permissible for a map to contain itself as a key.  While it is
         * permissible for a map to contain itself as a value, extreme caution is
         * advised: the @c equals and @c hash methods are no longer well-defined on such a map.
         * </p>
         * <p>
         * All general-purpose map implementation classes should provide two
         * "standard" constructors: a void (no arguments) constructor which creates an
         * empty map, and a constructor with a single argument of type @c Map,
         * which creates a new map with the same key-value mappings as its argument.
         * In effect, the latter constructor allows the user to copy any map,
         * producing an equivalent map of the desired class.
         * </p>
         * <p>
         * The "destructive" methods contained in this interface, that is, the
         * methods that modify the map on which they operate, are specified to throw
         * @c UnsupportedOperationException if this map does not support the
         * operation.  If this is the case, these methods may, but are not required
         * to, throw an @c UnsupportedOperationException if the invocation would
         * have no effect on the map.  For example, invoking the @c putAll(Map)
         * method on an unmodifiable map may, but is not required to, throw the
         * exception if the map whose mappings are to be "superimposed" is empty.
         * </p>
         * <p>
         * Some map implementations have restrictions on the keys and values they
         * may contain.  For example, some implementations prohibit null keys and
         * values, and some have restrictions on the types of their keys.  Attempting
         * to insert an ineligible key or value throws an unchecked exception,
         * typically @c IllegalArgumentException or @c ClassCastException.
         * Attempting to query the presence of an ineligible key or value may throw an
         * exception, or it may simply return false; some implementations will exhibit
         * the former behavior and some will exhibit the latter.  More generally,
         * attempting an operation on an ineligible key or value whose completion
         * would not result in the insertion of an ineligible element into the map may
         * throw an exception, or it may succeed, at the option of the implementation.
         * Such exceptions are marked as "optional" in the specification for this
         * interface.
         * <p>
         * <p>
         * Many methods in Collections Framework interfaces are defined
         * in terms of the @c equal method.  For
         * example, the specification for the @c containsKey(Object) method says:
         * "returns @c true if and only if this map contains a mapping for a key @c k such that
         * @c key.equals(k))." This specification should
         * <i>not</i> be construed to imply that invoking @c Map::containsKey
         * with a non-null argument @c key will cause @c key.equals(k) to
         * be invoked for any key @c k.  Implementations are free to
         * implement optimizations whereby the @c equals invocation is avoided,
         * for example, by first comparing the hash codes of the two keys.  (The
         * @c Object::hash() specification guarantees that two objects with
         * unequal hash codes cannot be equal.)  More generally, implementations of
         * the various Collections Framework interfaces are free to take advantage of
         * the specified behavior of underlying @c Object methods wherever the
         * implementor deems it appropriate.
         * </p>
         * <p>
         * Some map operations which perform recursive traversal of the map may fail
         * with an exception for self-referential instances where the map directly or
         * indirectly contains itself. This includes the @c clone(),
         * @c equals(), @c hash() and @c toString() methods.
         * Implementations may optionally handle the self-referential scenario, however
         * most current implementations do not do so.
         * </p>
         * <h2>Unmodifiable Maps</h2>
         * <p>
         * The @c Map::of, @c Map::ofEntries, and @c Map::copyOf
         * static factory methods provide a convenient way to create unmodifiable maps.
         * The @c Map instances created by these methods have the following characteristics:
         * </p>
         * - They are <i>unmodifiable</i>. Keys and values cannot be added, removed, or updated.
         * Calling any mutator method on the Map will always cause @c UnsupportedOperationException to be thrown.
         * However, if the contained keys or values are themselves mutable, this may cause the
         * Map to behave inconsistently or its contents to appear to change.
         * - They disallow @c null keys and values. Attempts to create them with
         * @c null keys or values result in @c IllegalArgumentException.
         * - They are serializable if all keys and values are serializable.
         * - They reject duplicate keys at creation time. Duplicate keys
         * passed to a static factory method result in @c IllegalArgumentException.
         * - The iteration order of mappings is unspecified and is subject to change.
         * - They are <i>value-based</i>.
         * Programmers should treat instances that are @c equal
         * as interchangeable and should not use them for synchronization, or
         * unpredictable behavior may occur. For example, in a future release,
         * synchronization may fail. Callers should make no assumptions
         * about the identity of the returned instances. Factories are free to
         * create new instances or reuse existing ones.
         * - They are serialized as specified on the Serialized Form page.
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         *
         * @see HashMap
         * @see TreeMap
         * @see Hashtable
         * @see SortedMap
         * @see Collection
         * @see Set
         */
        template <class K, class V>
        class Map : public virtual Object {
        protected:
            CORE_ALIAS(Keys, typename Class<Set<K>>::Pointer);
            CORE_ALIAS(Vals, typename Class<Collection<V>>::Pointer);
            CORE_ALIAS(Entry, util::Entry<K, V>);
            CORE_ALIAS(Entries, typename Class<Set<Entry>>::Pointer);
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            CORE_ALIAS(KEY, typename Class<K>::Pointer);
            CORE_ALIAS(VALUE, typename Class<V>::Pointer);
            CORE_ALIAS(MAP, typename Class<Map>::Pointer);

        public:
            // Query Operations

            /**
             * Returns the number of key-value mappings in this map.  If the
             * map contains more than @c Integer::MAX_VALUE elements, returns
             * @c Integer::MAX_VALUE.
             *
             * @return the number of key-value mappings in this map
             */
            virtual gint size() const {
                try { return entrySet().size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this map contains no key-value mappings.
             *
             * @return @c true if this map contains no key-value mappings
             */
            virtual gbool isEmpty() const {
                try { return size() == 0; } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this map contains a mapping for the specified
             * key.  More formally, returns @c true if and only if
             * this map contains a mapping for a key @c k such that
             * @code Objects.equals(key, k) @endcode.  (There can be
             * at most one such mapping.)
             *
             * @param key key whose presence in this map is to be tested
             * @return @c true if this map contains a mapping for the specified
             *         key
             * @throws ClassCastException if the key is of an inappropriate type for
             *         this map (@a optional)
             * @throws IllegalArgumentException if the specified key is null and this map
             *         does not permit null keys (@a optional)
             */
            virtual gbool containsKey(K const& key) const {
                try {
                    for (Entry const& entry : entrySet())
                        if (key == entry.getKey())
                            return true;

                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this map maps one or more keys to the
             * specified value.  More formally, returns @c true if and only if
             * this map contains at least one mapping to a value @c v such that
             * @code Objects::equals(value, v) @endcode .  This operation
             * will probably require time linear in the map size for most
             * implementations of the @c Map interface.
             *
             * @param value value whose presence in this map is to be tested
             * @return @c true if this map maps one or more keys to the
             *         specified value
             * @throws ClassCastException if the value is of an inappropriate type for
             *         this map (@a optional)
             * @throws IllegalArgumentException if the specified value is null and this
             *         map does not permit null values (@a optional)
             */
            virtual gbool containsValue(V const& value) const {
                try {
                    for (Entry const& entry : entrySet())
                        if (value == entry.getValue())
                            return true;

                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or @c null if this map contains no mapping for the key.
             *
             * <p>
             * More formally, if this map contains a mapping from a key
             * @c k to a value @c v such that
             * @code Objects.equals(key, k) @endcode,
             * then this method returns @c v; otherwise
             * it returns @c null.  (There can be at most one such mapping.)
             * </p>
             * <p>
             * If this map permits null values, then a return value of
             * @c null does not <i>necessarily</i> indicate that the map
             * contains no mapping for the key; it's also possible that the map
             * explicitly maps the key to @c null.  The @c containsKey
             * operation may be used to distinguish these two cases.
             * </p>
             * @param key the key whose associated value is to be returned
             * @return the value to which the specified key is mapped, or
             *         @c null if this map contains no mapping for the key
             * @throws ClassCastException if the key is of an inappropriate type for
             *         this map (@a optional)
             * @throws IllegalArgumentException if the specified key is null and this map
             *         does not permit null keys (@a optional)
             */
            virtual V& get(K const& key) {
                try {
                    for (Entry& entry : entrySet())
                        if (key == entry.getKey())
                            return entry.getValue();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                MissingKeyException(key).throws($ftrace());
            }

            /**
             * Returns the value to which the specified key is mapped,
             * or @c null if this map contains no mapping for the key.
             *
             * <p>
             * More formally, if this map contains a mapping from a key
             * @c k to a value @c v such that @code Objects.equals(key, k) @endcode,
             * then this method returns @c v; otherwise
             * it returns @c null.  (There can be at most one such mapping.)
             * </p>
             * <p>
             * If this map permits null values, then a return value of
             * @c null does not <i>necessarily</i> indicate that the map
             * contains no mapping for the key; it's also possible that the map
             * explicitly maps the key to @c null.  The @c containsKey
             * operation may be used to distinguish these two cases.
             * </p>
             * @param key the key whose associated value is to be returned
             * @return the value to which the specified key is mapped, or
             *         @c null if this map contains no mapping for the key
             * @throws ClassCastException if the key is of an inappropriate type for
             *         this map (@a optional)
             * @throws IllegalArgumentException if the specified key is null and this map
             *         does not permit null keys (@a optional)
             */
            virtual V const& get(K const& key) const {
                try {
                    for (Entry const& entry : entrySet())
                        if (key == entry.getKey())
                            return entry.getValue();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                MissingKeyException(key).throws($ftrace());
            }

            // Modification Operations

            /**
             * Associates the specified value with the specified key in this map
             * (optional operation).  If the map previously contained a mapping for
             * the key, the old value is replaced by the specified value.  (A map
             * @c m is said to contain a mapping for a key @c k if and only
             * if @c m.containsKey(k) would return @c true.)
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with @c key, or
             *         @c null if there was no mapping for @c key.
             *         (A @c null return can also indicate that the map
             *         previously associated @c null with @c key,
             *         if the implementation supports @c null values.)
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by this map
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             */
            virtual Object& put(K const& key, V const& value) {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Removes the mapping for a key from this map if it is present
             * (optional operation).   More formally, if this map contains a mapping
             * from key @c k to value @c v such that
             * @code Objects.equals(key, k) @endcode, that mapping
             * is removed.  (The map can contain at most one such mapping.)
             *
             * <p>
             * Returns the value to which this map previously associated the key,
             * or @c null if the map contained no mapping for the key.
             * </p>
             * <p>
             * If this map permits null values, then a return value of
             * @c null does not <i>necessarily</i> indicate that the map
             * contained no mapping for the key; it's also possible that the map
             * explicitly mapped the key to @c null.
             * </p>
             * <p>
             * The map will not contain a mapping for the specified key once the
             * call returns.
             * </p>
             * @param key key whose mapping is to be removed from the map
             * @return the previous value associated with @c key, or
             *         @c null if there was no mapping for @c key.
             * @throws UnsupportedOperationException if the @c remove operation
             *         is not supported by this map
             * @throws ClassCastException if the key is of an inappropriate type for
             *         this map (@a optional)
             * @throws IllegalArgumentException if the specified key is null and this
             *         map does not permit null keys (@a optional)
             */
            virtual V& remove(K const& key) {
                try {
                    Iterator<Entry>& it = entrySet().iterator();
                    while (it.hasNext()) {
                        Entry& entry = it.next();
                        if (key == entry.getKey()) {
                            V& oldValue = entry.getValue();
                            it.remove();
                            return oldValue;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                MissingKeyException(key).throws($ftrace());
            }


            // Bulk Operations

            /**
             * Copies all the mappings from the specified map to this map
             * (optional operation).  The effect of this call is equivalent to that
             * of calling @c put(k, v) on this map once
             * for each mapping from key @c k to value @c v in the
             * specified map.  The behavior of this operation is undefined if the specified map
             * is modified while the operation is in progress. If the specified map has a defined
             * <em>encounter order</em>, processing of its mappings generally occurs in that order.
             *
             * @param m mappings to be stored in this map
             * @throws UnsupportedOperationException if the @c putAll operation
             *         is not supported by this map
             * @throws ClassCastException if the class of a key or value in the
             *         specified map prevents it from being stored in this map
             * @throws IllegalArgumentException if the specified map is null, or if
             *         this map does not permit null keys or values, and the
             *         specified map contains null keys or values
             * @throws IllegalArgumentException if some property of a key or value in
             *         the specified map prevents it from being stored in this map
             */
            virtual void putAll(Map const& m) {
                for (Entry const& entry : m.entrySet())
                    put(entry.getKey(), entry.getValue());
            }

            /**
             * Removes all the mappings from this map (optional operation).
             * The map will be empty after this call returns.
             *
             * @throws UnsupportedOperationException if the @c clear operation
             *         is not supported by this map
             */
            virtual void clear() {
                try { entrySet().clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            // Views

            /**
             * Returns a @c Set view of the keys contained in this map.
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
            virtual Set<K>& keySet() {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            /**
             * Returns an Immutable @c Set view of the keys contained in this map.
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
            virtual Set<K> const& keySet() const {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            /**
             * Returns a @c Collection view of the values contained in this map.
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
             * @return a collection view of the values contained in this map
             */
            virtual Collection<V>& values() {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            /**
             * Returns an Immutable @c Collection view of the values contained in this map.
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
             * @return a collection view of the values contained in this map
             */
            virtual Collection<V> const& values() const {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            /**
             * Returns a @c Set view of the mappings contained in this map.
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
            virtual Set<Entry>& entrySet() = 0;

            /**
             * Returns a @c Set view of the mappings contained in this map.
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
            virtual Set<Entry> const& entrySet() const = 0;

            // Comparison and hashing

            /**
             * Compares the specified object with this map for equality.  Returns
             * @c true if the given object is also a map and the two maps
             * represent the same mappings.  More formally, two maps @c m1 and
             * @c m2 represent the same mappings if @c m1.entrySet().equals(m2.entrySet()).
             * This ensures that the @c equals method works properly across different implementations
             * of the @c Map interface.
             *
             * @param o object to be compared for equality with this map
             * @return @c true if the specified object is equal to this map
             */
            gbool equals(const Object& o) const override {
                if (this == &o)
                    return true;

                if (!Class<Map>::hasInstance(o))
                    return false;

                Map const& map = CORE_XCAST(Map const, o);
                if (map.size() != size())
                    return false;
                try {
                    for (Entry const& entry : entrySet()) {
                        if (!map.containsKey(entry.getKey()))
                            return false;

                        if (entry.getValue() != map.get(entry.getKey()))
                            return false;
                    }
                } catch (MissingKeyException const&) {
                    // Map<?, ?>::get(K) failed.
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                return true;
            }

            /**
             * Returns the hash code value for this map.  The hash code of a map is
             * defined to be the sum of the hash codes of each entry in the map's
             * @c entrySet() view.  This ensures that @c m1.equals(m2)
             * implies that @code m1.hash() == m2.hash() @endcode for any two maps
             * @c m1 and @c m2, as required by the general contract of @c Object::hash.
             *
             * @return the hash code value for this map
             * @see Entry::hash()
             * @see Object::equals(Object)
             * @see equals(Object)
             */
            gint hash() const override {
                gint hash = 0;
                for (Entry const& entry : entrySet())
                    hash = hash * 31 + entry.hash();

                return hash;
            }

            /**
             * Returns a string representation of this map.  The string representation
             * consists of a list of key-value mappings in the order returned by the
             * map's @c entrySet view's iterator, enclosed in braces
             * (@c "{}").  Adjacent mappings are separated by the characters
             * @c ", " (comma and space).  Each key-value mapping is rendered as
             * the key followed by two points (@c ":") followed by the
             * associated value.  Keys and values are converted to strings as by
             * @c String::valueOf(Object).
             *
             * @return a string representation of this map
             */
            String toString() const override {
                if (isEmpty())
                    return "{}";

                try {
                    StringJoiner sj = StringJoiner(", "_S, "{"_S, "}"_S);
                    for (Entry const& entry : entrySet()) {
                        StringJoiner sj2 = StringJoiner("="_S);
                        Object const& key = entry.getKey();
                        Object const& val = entry.getValue();

                        if (isSameObject(*this, key))
                            sj2.add("{..}"_S);
                        else
                            sj2.add(String::valueOf(key));

                        if (isSameObject(*this, val))
                            sj2.add("{..}"_S);
                        else
                            sj2.add(String::valueOf(val));

                        sj.merge(sj2);
                    }

                    return sj.toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            // Default-able methods

            /**
             * Returns the value to which the specified key is mapped, or
             * @c defaultValue if this map contains no mapping for the key.
             *
             * @note
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key the key whose associated value is to be returned
             * @param defaultValue the default mapping of the key
             * @return the value to which the specified key is mapped, or
             * @c defaultValue if this map contains no mapping for the key
             * @throws ClassCastException if the key is of an inappropriate type for
             * this map (@a optional)
             * @throws IllegalArgumentException if the specified key is null and this map
             * does not permit null keys (@a optional)
             */
            virtual V& getOrDefault(K const& key, V const& defaultValue) {
                try {
                    for (Entry& entry : entrySet())
                        if (key == entry.getKey())
                            return entry.getValue();

                    return UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the value to which the specified key is mapped, or
             * @c defaultValue if this map contains no mapping for the key.
             *
             * @note
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key the key whose associated value is to be returned
             * @param defaultValue the default mapping of the key
             * @return the value to which the specified key is mapped, or
             * @c defaultValue if this map contains no mapping for the key
             * @throws ClassCastException if the key is of an inappropriate type for
             * this map (@a optional)
             * @throws IllegalArgumentException if the specified key is null and this map
             * does not permit null keys (@a optional)
             */
            virtual V const& getOrDefault(K const& key, V const& defaultValue) const {
                try {
                    for (Entry const& entry : entrySet())
                        if (key == entry.getKey())
                            return entry.getValue();

                    return UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            virtual Object& getOrNull(K const& key) {
                try {
                    for (Entry& entry : entrySet())
                        if (key == entry.getKey())
                            return entry.getValue();

                    return null;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            virtual Object const& getOrNull(K const& key) const {
                try {
                    for (Entry const& entry : entrySet())
                        if (key == entry.getKey())
                            return entry.getValue();

                    return null;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Performs the given action for each entry in this map until all entries
             * have been processed or the action throws an exception.   Unless
             * otherwise specified by the implementing class, actions are performed in
             * the order of entry set iteration (if an iteration order is specified.)
             * Exceptions thrown by the action are relayed to the caller.
             *
             * @note
             * The default implementation is equivalent to, for this @c map:
             * @code
             * for (Entry &entry : map.entrySet())
             *     action.accept(entry.getKey(), entry.getValue());
             * @endcode
             *
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param action The action to be performed for each entry
             * @throws IllegalArgumentException if the specified action is null
             * @throws ConcurrentModificationException if an entry is found to be
             * removed during iteration
             */
            virtual void forEach(function::BiConsumer<K&, V&> const& action) {
                KEY k = null;
                VALUE v = null;
                for (Entry& entry : entrySet()) {
                    try {
                        k = &entry.getKey();
                        v = &entry.getValue();
                    } catch (IllegalStateException const& ex) {
                        // this usually means the entry is no longer in the map.
                        ConcurrentModificationException(ex).throws($ftrace());
                    }

                    try {
                        action.accept(*k, *v);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            }

            /**
             * Performs the given action for each entry in this map until all entries
             * have been processed or the action throws an exception.   Unless
             * otherwise specified by the implementing class, actions are performed in
             * the order of entry set iteration (if an iteration order is specified.)
             * Exceptions thrown by the action are relayed to the caller.
             *
             * @note
             * The default implementation is equivalent to, for this @c map:
             * @code
             * for (Entry &entry : map.entrySet())
             *     action.accept(entry.getKey(), entry.getValue());
             * @endcode
             *
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param action The action to be performed for each entry
             * @throws IllegalArgumentException if the specified action is null
             * @throws ConcurrentModificationException if an entry is found to be
             * removed during iteration
             */
            virtual void forEach(function::BiConsumer<K, V> const& action) const {
                KEY k = null;
                VALUE v = null;
                for (Entry const& entry : entrySet()) {
                    try {
                        k = CORE_CAST(KEY, &entry.getKey());
                        v = CORE_CAST(VALUE, &entry.getValue());
                    } catch (IllegalStateException const& ex) {
                        // this usually means the entry is no longer in the map.
                        ConcurrentModificationException(ex).throws($ftrace());
                    }

                    try {
                        action.accept(*k, *v);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            }

            /**
             * Replaces each entry's value with the result of invoking the given
             * function on that entry until all entries have been processed or the
             * function throws an exception (optional operation). Exceptions thrown
             * by the function are relayed to the caller.
             *
             * @note
             * <p>The default implementation is equivalent to, for this @c map:
             * @code
             * for (Entry &entry : map.entrySet())
             *     entry.setValue(function.apply(entry.getKey(), entry.getValue()));
             * @endcode
             * </p>
             * <p>The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             * </p>
             * @param function the function to apply to each entry
             * @throws UnsupportedOperationException if the @c replaceAll operation
             *         is not supported by this map
             *         (@a optional)
             * @throws ClassCastException if the class of a replacement value
             *         prevents it from being stored in this map
             *         (@a optional)
             * @throws IllegalArgumentException if the specified function is null, or if a
             *         replacement value is null and this map does not permit null values
             *         (@a optional)
             * @throws IllegalArgumentException if some property of a replacement value
             *         prevents it from being stored in this map
             *         (@a optional)
             * @throws ConcurrentModificationException if an entry is found to be
             *         removed during iteration
             */
            virtual void replaceAll(function::BiFunction<K&, V&, V> const& function) {
                KEY k = null;
                VALUE v = null;
                for (Entry& entry : entrySet()) {
                    try {
                        k = &entry.getKey();
                        v = &entry.getValue();
                    } catch (IllegalStateException const& ex) {
                        // this usually means the entry is no longer in the map.
                        ConcurrentModificationException(ex).throws($ftrace());
                    }
                    // ise thrown from function is not a cme.
                    v = &UNSAFE::copyInstance(function.apply(*k, *v));
                    try {
                        entry.setValue(*v);
                    } catch (IllegalStateException const& ex) {
                        // this usually means the entry is no longer in the map.
                        ConcurrentModificationException(ex).throws($ftrace());
                    }
                }
            }

            /**
             * If the specified key is not already associated with a value (or is mapped
             * to @c null) associates it with the given value and returns
             * @c null, else returns the current value (optional operation).
             *
             * @note
             * The default implementation is equivalent to, for this @c map:
             *
             * @code
             *      auto &v = map.getOrNull(key);
             *      if (v == null)
             *          return map.put(key, value);
             *      return v;
             * @endcode
             *
             * <p>
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             * </p>
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with the specified key, or
             *         @c null if there was no mapping for the key.
             *         (A @c null return can also indicate that the map
             *         previously associated @c null with the key,
             *         if the implementation supports null values.)
             * @throws UnsupportedOperationException if the @c putIfAbsent operation
             *         is not supported by this map
             *         (@a optional)
             * @throws ClassCastException if the key or value is of an inappropriate
             *         type for this map (@a optional)
             * @throws IllegalArgumentException if the specified key or value is null,
             *         and this map does not permit null keys or values
             *         (@a optional)
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             *         (@a optional)
             */
            virtual Object& putIfAbsent(K const& key, V const& value) {
                try {
                    for (Entry& entry : entrySet())
                        if (key == entry.getKey())
                            return entry.getValue();

                    return put(key, value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the entry for the specified key only if it is currently
             * mapped to the specified value (optional operation).
             *
             * @note
             * The default implementation is equivalent to, for this @c map:
             *
             * @code
             * if (map.containsKey(key) && Objects.equals(map.get(key), value)) {
             *     map.remove(key);
             *     return true;
             * } else
             *     return false;
             * @endcode
             *
             * <p>The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key key with which the specified value is associated
             * @param value value expected to be associated with the specified key
             * @return @c true if the value was removed
             * @throws UnsupportedOperationException if the @c remove operation
             *         is not supported by this map
             *         (@a optional)
             * @throws ClassCastException if the key or value is of an inappropriate
             *         type for this map
             *         (@a optional)
             * @throws IllegalArgumentException if the specified key or value is null,
             *         and this map does not permit null keys or values
             *         (@a optional)
             */
            virtual gbool remove(K const& key, V const& value) {
                try {
                    Iterator<Entry>& it = entrySet().iterator();

                    while (it.hasNext()) {
                        Entry& entry = it.next();

                        if (key == entry.getKey()) {
                            if (value != entry.getValue())
                                break;

                            it.remove();
                            return true;
                        }
                    }

                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Replaces the entry for the specified key only if currently
             * mapped to the specified value (optional operation).
             *
             * @note
             * The default implementation is equivalent to, for this @c map:
             *
             * @code
             * if (map.containsKey(key) && Objects.equals(map.get(key), oldValue)) {
             *     map.put(key, newValue);
             *     return true;
             * } else
             *     return false;
             * @endcode
             *
             * The default implementation does not throw IllegalArgumentException
             * for maps that do not support null values if oldValue is null unless
             * newValue is also null.
             *
             * <p>The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key key with which the specified value is associated
             * @param oldValue value expected to be associated with the specified key
             * @param newValue value to be associated with the specified key
             * @return @c true if the value was replaced
             * @throws UnsupportedOperationException if the @c replace operation
             *         is not supported by this map
             *         (@a optional)
             * @throws ClassCastException if the class of a specified key or value
             *         prevents it from being stored in this map
             * @throws IllegalArgumentException if a specified key or newValue is null,
             *         and this map does not permit null keys or values
             * @throws IllegalArgumentException if oldValue is null and this map does not
             *         permit null values (@a optional)
             * @throws IllegalArgumentException if some property of a specified key
             *         or value prevents it from being stored in this map
             */
            virtual gbool replace(K const& key, V const& oldValue, V const& newValue) {
                try {
                    for (Entry const& entry : entrySet()) {
                        if (key == entry.getValue()) {
                            if (oldValue != entry.getValue())
                                break;

                            return oldValue == put(key, newValue);
                        }
                    }

                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Replaces the entry for the specified key only if it is
             * currently mapped to some value (optional operation).
             *
             * @note
             * The default implementation is equivalent to, for this @c map:
             *
             * @code
             * if (map.containsKey(key)) {
             *     return map.put(key, value);
             * } else
             *     return null;
             * @endcode
             *
             * <p>The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties.
             *
             * @param key key with which the specified value is associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with the specified key, or
             *         @c null if there was no mapping for the key.
             *         (A @c null return can also indicate that the map
             *         previously associated @c null with the key,
             *         if the implementation supports null values.)
             * @throws UnsupportedOperationException if the @c replace operation
             *         is not supported by this map
             *         (@a optional)
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map
             *         (@a optional)
             * @throws IllegalArgumentException if the specified key or value is null,
             *         and this map does not permit null keys or values
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             */
            virtual Object& replace(K const& key, V const& value) {
                try {
                    for (Entry& entry : entrySet()) {
                        if (key == entry.getValue()) {
                            if (value == entry.getValue())
                                return entry.getValue();

                            return put(key, value);
                        }
                    }

                    return null;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * If the specified key is not already associated with a value (or is mapped
             * to @c null), attempts to compute its value using the given mapping
             * function and enters it into this map unless @c null.
             *
             * <p>
             * If the mapping function returns @c null, no mapping is recorded.
             * If the mapping function itself throws an (unchecked) exception, the
             * exception is rethrown, and no mapping is recorded.  The most
             * common usage is to construct a new object serving as an initial
             * mapped value or memoized result, as in:
             *
             * @code
             * map.computeIfAbsent(key, [](auto& k) { return Value(f(k)); });
             * @endcode
             * </p>
             * <p>
             * Or to implement a multi-value map, @c Map<K,Collection<V>>,
             * supporting multiple values per key:
             *
             * @code
             * map.computeIfAbsent(key, [](auto& k) { return HashSet<V>(); }).add(v);
             * @endcode
             * </p>
             * <p>
             * The mapping function should not modify this map during computation.
             * </p>
             * @note
             * The default implementation is equivalent to the following steps for this
             * @c map, then returning the current value or @c null if now
             * absent:
             *
             * @code
             * if (map.getOrNull(key) == null) {
             *     auto& newValue = mappingFunction.apply(key);
             *     if (newValue != null)
             *         map.put(key, newValue);
             * }
             * @endcode
             *
             * <p>
             * The default implementation makes no guarantees about detecting if the
             * mapping function modifies this map during computation and, if
             * appropriate, reporting an error. Non-concurrent implementations should
             * override this method and, on a best-effort basis, throw a
             * @c ConcurrentModificationException if it is detected that the
             * mapping function modifies this map during computation. Concurrent
             * implementations should override this method and, on a best-effort basis,
             * throw an @c IllegalStateException if it is detected that the
             * mapping function modifies this map during computation and as a result
             * computation would never complete.
             * </p>
             * <p>
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties. In particular, all implementations of
             * sub-interface @b concurrent::ConcurrentMap must document
             * whether the mapping function is applied once atomically only if the value
             * is not present.
             * </p>
             * @param key key with which the specified value is to be associated
             * @param mappingFunction the mapping function to compute a value
             * @return the current (existing or computed) value associated with
             *         the specified key, or null if the computed value is null
             * @throws NullPointerException if the specified key is null and
             *         this map does not support null keys, or the mappingFunction
             *         is null
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by this map (@em optional )
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map (@em optional )
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map (@em optional )
             */
            virtual Object& computeIfAbsent(K const& key, function::Function<K, V&> const& mappingFunction) {
                Object& v = getOrNull(key);
                if (v == null) {
                    try {
                        Object& newValue = UNSAFE::copyInstance(mappingFunction.apply(key));
                        CORE_ASSERT(newValue == null || Class<V>::hasInstance(newValue));
                        if (newValue != null) {
                            put(key, CORE_XCAST(V, newValue));
                            return newValue;
                        }
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
                return v;
            }

            /**
             * If the value for the specified key is present and non-null, attempts to
             * compute a new mapping given the key and its current mapped value.
             *
             * <p>
             * If the remapping function returns @c null, the mapping is removed.
             * If the remapping function itself throws an (unchecked) exception, the
             * exception is rethrown, and the current mapping is left unchanged.
             * </p>
             * <p>
             * The remapping function should not modify this map during computation.
             * </p>
             * @note
             * The default implementation is equivalent to performing the following
             * steps for this @c map, then returning the current value or
             * @c null if now absent:
             *
             * @code
             * if (map.get(key) != null) {
             *     auto& oldValue = map.getOrNull(key);
             *     auto& newValue = remappingFunction.apply(key, oldValue);
             *     if (newValue != null)
             *         map.put(key, newValue);
             *     else
             *         map.remove(key);
             * }
             * @endcode
             *
             * <p>
             * The default implementation makes no guarantees about detecting if the
             * remapping function modifies this map during computation and, if
             * appropriate, reporting an error. Non-concurrent implementations should
             * override this method and, on a best-effort basis, throw a
             * @c ConcurrentModificationException if it is detected that the
             * remapping function modifies this map during computation. Concurrent
             * implementations should override this method and, on a best-effort basis,
             * throw an @c IllegalStateException if it is detected that the
             * remapping function modifies this map during computation and as a result
             * computation would never complete.
             * </p>
             * <p>
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties. In particular, all implementations of
             * sub-interface @b concurrent::ConcurrentMap must document
             * whether the remapping function is applied once atomically only if the
             * value is not present.
             * </p>
             * @param key key with which the specified value is to be associated
             * @param remappingFunction the remapping function to compute a value
             * @return the new value associated with the specified key, or null if none
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by this map (@em optional )
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map (@em optional )
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map (@em optional )
             * @since 1.8
             */
            virtual Object& computeIfPresent(K const& key, function::BiFunction<K, V&, V> const& remappingFunction) {
                Object& oldValue = getOrNull(key);
                if (oldValue != null) {
                    V& value = CORE_XCAST(V, oldValue);
                    Object& newValue = UNSAFE::copyInstance(remappingFunction.apply(key, value));
                    if (newValue != null) {
                        put(key, CORE_XCAST(V, newValue));
                        return newValue;
                    }
                    remove(key);
                }
                return null;
            }

            /**
             * Attempts to compute a mapping for the specified key and its current
             * mapped value (or @c null if there is no current mapping). For
             * example, to either create or append a @c String msg to a value
             * mapping:
             *
             * @code
             * map.compute(key, [](auto &k, auto &v) { return (v == null) ? msg : v.concat(msg); })
             * @endcode
             * (Method @b merge() is often simpler to use for such purposes.)
             *
             * <p>
             * If the remapping function returns @c null, the mapping is removed
             * (or remains absent if initially absent).  If the remapping function
             * itself throws an (unchecked) exception, the exception is rethrown, and
             * the current mapping is left unchanged.
             * </p>
             * <p>
             * The remapping function should not modify this map during computation.
             * </p>
             * @note
             * The default implementation is equivalent to performing the following
             * steps for this @c map:
             *
             * @code
             * auto& oldValue = map.getOrNull(key);
             * auto& newValue = remappingFunction.apply(key, oldValue);
             * if (newValue != null) {
             *     map.put(key, newValue);
             * } else if (oldValue != null || map.containsKey(key)) {
             *     map.remove(key);
             * }
             * return newValue;
             * @endcode
             *
             * <p>
             * The default implementation makes no guarantees about detecting if the
             * remapping function modifies this map during computation and, if
             * appropriate, reporting an error. Non-concurrent implementations should
             * override this method and, on a best-effort basis, throw a
             * @c ConcurrentModificationException if it is detected that the
             * remapping function modifies this map during computation. Concurrent
             * implementations should override this method and, on a best-effort basis,
             * throw an @c IllegalStateException if it is detected that the
             * remapping function modifies this map during computation and as a result
             * computation would never complete.
             * </p>
             * <p>
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties. In particular, all implementations of
             * subinterface @b concurrent::ConcurrentMap must document
             * whether the remapping function is applied once atomically only if the
             * value is not present.
             * </p>
             * @param key key with which the specified value is to be associated
             * @param remappingFunction the remapping function to compute a value
             * @return the new value associated with the specified key, or null if none
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by this map (@em optional )
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map (@em optional )
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map (@em optional )
             */
            virtual Object& compute(K const& key, function::BiFunction<K, V&, V> const& remappingFunction) {
                Object& oldValue = getOrNull(key);
                Object& newValue = oldValue == null && !Class<V>::hasInstance(oldValue)
                                       ? (Object &)null
                                       : UNSAFE::copyInstance(remappingFunction.apply(key, CORE_XCAST(V, oldValue)));
                if (newValue == null) {
                    // delete mapping
                    if (oldValue != null || containsKey(key)) {
                        // something to remove
                        remove(key);
                        return null;
                    } else {
                        // nothing to do. Leave things as they were.
                        return null;
                    }
                } else {
                    // add or replace old mapping
                    put(key, CORE_XCAST(V, newValue));
                    return newValue;
                }
            }

            /**
             * If the specified key is not already associated with a value or is
             * associated with null, associates it with the given non-null value.
             * Otherwise, replaces the associated value with the results of the given
             * remapping function, or removes if the result is @c null. This
             * method may be of use when combining multiple mapped values for a key.
             * For example, to either create or append a @c String msg to a
             * value mapping:
             *
             * @code
             * map.merge(key, msg, {&String::concat})
             * @endcode
             *
             * <p>
             * If the remapping function returns @c null, the mapping is removed.
             * If the remapping function itself throws an (unchecked) exception, the
             * exception is rethrown, and the current mapping is left unchanged.
             * </p>
             * <p>
             * The remapping function should not modify this map during computation.
             * </p>
             * @note
             * The default implementation is equivalent to performing the following
             * steps for this @c map, then returning the current value or
             * @c null if absent:
             *
             * @code
             * auto& oldValue = map.getOrNull(key);
             * auto& newValue = (oldValue == null) ? value :
             *              remappingFunction.apply(oldValue, value);
             * if (newValue == null)
             *     map.remove(key);
             * else
             *     map.put(key, newValue);
             * @endcode
             *
             * <p>
             * The default implementation makes no guarantees about detecting if the
             * remapping function modifies this map during computation and, if
             * appropriate, reporting an error. Non-concurrent implementations should
             * override this method and, on a best-effort basis, throw a
             * @c ConcurrentModificationException if it is detected that the
             * remapping function modifies this map during computation. Concurrent
             * implementations should override this method and, on a best-effort basis,
             * throw an @c IllegalStateException if it is detected that the
             * remapping function modifies this map during computation and as a result
             * computation would never complete.
             * </p>
             * <p>
             * The default implementation makes no guarantees about synchronization
             * or atomicity properties of this method. Any implementation providing
             * atomicity guarantees must override this method and document its
             * concurrency properties. In particular, all implementations of
             * sub-interface @b concurrent::ConcurrentMap must document
             * whether the remapping function is applied once atomically only if the
             * value is not present.
             * </p>
             * @param key key with which the resulting value is to be associated
             * @param value the non-null value to be merged with the existing value
             *        associated with the key or, if no existing value or a null value
             *        is associated with the key, to be associated with the key
             * @param remappingFunction the remapping function to recompute a value if
             *        present
             * @return the new value associated with the specified key, or null if no
             *         value is associated with the key
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by this map (@em optional )
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map (@em optional )
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map (@em optional )
             */
            virtual Object& merge(K const& key, V const& value,
                                  function::BiFunction<V, V, V> const& remappingFunction) {
                Object& oldValue = getOrNull(key);
                V const& newValue = (oldValue == null)
                                        ? value
                                        : UNSAFE::copyInstance(remappingFunction.apply(CORE_XCAST(V, oldValue), value));
                if (newValue == null) {
                    remove(key);
                } else {
                    V& v = UNSAFE::copyInstance(newValue);
                    put(key, v);
                    return v;
                }
                return UNSAFE::copyInstance(newValue);
            }

            /**
             * Returns an unmodifiable map containing zero mappings.
             * See <em>Unmodifiable Maps</em> for details.
             *
             * @tparam K the @c Map's key type
             * @tparam V the @c Map's value type
             * @return an empty @c Map
             */
            static Map& of() {
                try { return emptyMap(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an unmodifiable map containing specified mappings.
             * See <em>Unmodifiable Maps</em> for details.
             * For Example:
             * @code
             *      auto& map = Map<Integer, String>::of(
             *          1, "a",
             *          2, "b",
             *          3, "c",
             *          ...
             *          26, "z"
             *      );
             * @endcode
             *
             * @note The number of given arguments must be always multiple of two.
             *
             * @param entries the inline key, value pairs.
             * @return a @c Map containing the specified mappings
             * @throw AssertionError If the number of argument is not pair.
             * @throws IllegalArgumentException if there are any duplicate keys
             */
            template <class... Args>
            static Map& of(Args&&... entries) {
                // assert sizeof...(Args) % 2 == 0
                CORE_FAST_XASSERT(VarArgs<Args...>::check(), "Illegal Arguments");

                CORE_FAST gint len = (sizeof...(Args) >> 1) * 2;
                if (len < 0)
                    OutOfMemoryError("Number of mapping is too large.").throws($ftrace());

                Array<> a = Array<>(len);
                BooleanArray b = BooleanArray(len);

                // Store inline entries
                VarArgs<Args...>::init(0, a, b, UNSAFE::forwardInstance<Args>(entries)...);

                // Organize entries
                Array<Entry> table = organize(a, b);

                switch (table.length()) {
                    case 0: return emptyMap();
                    case 1: return UNSAFE::newInstance<Singleton>(table[0]);
                    default: return UNSAFE::newInstance<MapN>(table);
                }
            }

            /**
             * Returns an unmodifiable empty map.
             * See <em>Unmodifiable Maps</em> for details.
             *
             * @return an empty map
             *
             * @see Map::entry()
             */
            static Map& ofEntries() {
                try {
                    return emptyMap();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an unmodifiable map containing keys and values extracted from the given entries.
             * The entries themselves are not stored in the map.
             * See <em>Unmodifiable Maps</em> for details.
             *
             * @note
             * It is convenient to create the map entries using the @c Map::entry() method.
             * For example,
             *
             * @code
             *     auto& map = Map<>::ofEntries(
             *         Map<>::newEntry(1, "a"),
             *         Map<>::newEntry(2, "b"),
             *         Map<>::newEntry(3, "c"),
             *         ...
             *         Map<>::newEntry(26, "z")
             *     );
             * @endcode
             *
             * @param entries the entries.
             * @return a @c Map containing the specified mappings
             *
             * @see Map::entry()
            */
            template <class... X, class... Y>
            static Map& ofEntries(util::Entry<X, Y> const&... entries) {
                CORE_FAST_ASSERT(Class<>::allIsTrue<Class<K>::template isSuper<X>()...>());
                CORE_FAST_ASSERT(Class<>::allIsTrue<Class<V>::template isSuper<Y>()...>());
                try {
                    Array<Entry> table = Array<Entry>::of(toUnmodifiable(entries)...);
                    return ofEntries(table);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an unmodifiable map containing keys and values extracted from the given entries.
             * The entries themselves are not stored in the map.
             * See <em>Unmodifiable Maps</em> for details.
             *
             * @param table @em Entries containing the keys and values from which the map is populated
             * @return a @c Map containing the specified mappings
             *
             * @see Map::entry()
            */
            template <class X, class Y,
                      ClassOf(1)::OnlyIf<Class<K>::template isSuper<X>()> CaptureKey = 1,
                      ClassOf(1)::OnlyIf<Class<V>::template isSuper<Y>()> CaptureValue = 1>
            static Map& ofEntries(Array<util::Entry<X, Y>> const& table) {
                try {
                    switch (table.length()) {
                        case 0: return emptyMap();
                        case 1: return UNSAFE::newInstance<Singleton>(organize(table)[0]);
                        default: return UNSAFE::newInstance<MapN>(organize(table));
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an <em>unmodifiable Map</em> containing the entries
             * of the given Map. The given Map must not be null, and it must not contain any
             * null keys or values. If the given Map is subsequently modified, the returned
             * Map will not reflect such modifications.
             *
             * @note
             * If the given Map is an <em>unmodifiable Map</em>,
             * calling copyOf will generally not create a copy.
             *
             * @tparam K the @c Map's key type
             * @tparam V the @c Map's value type
             * @param map a @c Map from which entries are drawn, must be non-null
             * @return a @c Map containing the entries of the given @c Map
             */
            static Map<K, V>& copyOf(Map<K, V> const& map) {
                try {
                    if (Class<UnmodifiableMap>::hasInstance(map))
                        return CORE_CAST(Map &, map);

                    return ofEntries(map.entrySet().toArray());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an unmodifiable @c Entry containing the given key and value.
             * These entries are suitable for populating @c Map instances using the
             * @c Map::ofEntries() method.
             * <p>
             * The @c Entry instances created by this method have the following characteristics:
             *
             * @li They are unmodifiable. Calls to @c Entry::setValue()
             * on a returned @c Entry result in @c UnsupportedOperationException.
             * @li They are not serializable.
             * @li They are <i>value-based</i>.
             * </p>
             * Programmers should treat instances that are @a equal
             * as interchangeable and should not use them for synchronization, or
             * unpredictable behavior may occur. For example, in a future release,
             * synchronization may fail. Callers should make no assumptions
             * about the identity of the returned instances. This method is free to
             * create new instances or reuse existing ones.
             *
             * @tparam Ek the key's type
             * @tparam Ev the value's type
             * @param key the key
             * @param value the value
             * @return an @c Entry containing the specified key and value
             * @throws IllegalArgumentException if the key or value is @c null
             *
             * @see Map::ofEntries()
             */
            template <class Ek, class Ev,
                      ClassOf(1)::OnlyIf<Class<K>::template accept<Ek>()> CaptureKey = 1,
                      ClassOf(1)::OnlyIf<Class<V>::template accept<Ev>()> CaptureValue = 1>
            static Entry& newEntry(Ek&& key, Ev&& value) {
                try {
                    // We'll use Array<?>::of method to construct key and value
                    Array<K> ak = Array<K>::of(UNSAFE::forwardInstance<Ek>(key));
                    Array<V> av = Array<V>::of(UNSAFE::forwardInstance<Ev>(value));

                    /// assert ak.length() == 1
                    /// assert av.length() == 1

                    return UNSAFE::newInstance<UnmodifiableEntry>(ak[0], av[0]);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an empty map (immutable).  This map is serializable.
             *
             * <p>
             * This example illustrates the type-safe way to obtain an empty map:
             * @code
             *     Map<String, Date> s = Collections::emptyMap();
             * @endcode
             * </p>
             * @note Implementations of this method need not create a separate
             * @c Map object for each call.  Using this method is likely to have
             * comparable cost to using the like-named field.  (Unlike this method, the
             * field does not provide type safety.)
             *
             * @tparam K the class of the map keys
             * @tparam V the class of the map values
             * @return an empty map
             */
            static Map& emptyMap() {
                try {
                    return UNSAFE::newInstance<EmptyMap>();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Destroy this map, and him dependant views such as
             * keys and values views
             */
            ~Map() override {
                if (keys != null) {
                    UNSAFE::deleteRegInstance(*keys);
                    keys = null;
                }

                if (vals != null) {
                    UNSAFE::deleteRegInstance(*vals);
                    vals = null;
                }
            }

            // ::::::::::::::::::::: Unmodifiable Map Base class :::::::::::::::::::::::::::::

            /**
             * The Unmodifiable version of Map
             */
            class UnmodifiableMap;

        protected:
            // ::::::::::::::::::::: Views Fields :::::::::::::::::::::::::::::

            Keys mutable keys = null;
            Vals mutable vals = null;
            Entries mutable entries = null; // Unused directly by this class

            // ::::::::::::::::::::: views Destructor Utilities :::::::::::::::::::::::::::::

            static void detachVals(Map const& map) { map.vals = null; }

            static void detachKeys(Map const& map) { map.keys = null; }

            static void detachEntries(Map const& map) { map.entries = null; }

        private:
            // ::::::::::::::::::::: Views Class :::::::::::::::::::::::::::::

            /**
             * The keys view
             */
            class KeySet;

            /**
             * The values view
             */
            class Values;

            // ::::::::::::::::::::: Unmodifiable Maps Class :::::::::::::::::::::::::::::

            /**
             * The Unmodifiable entry
             */
            class UnmodifiableEntry;

            /**
             * The immutable empty map
             */
            class EmptyMap;

            /**
             * The immutable map with single entry
             */
            class Singleton;

            /**
             * The immutable map with multiple entries.
             */
            class MapN;

            // ::::::::::::::::::::: Utility Maps class/methods :::::::::::::::::::::::::::::

            template <class... Entries>
            class VarArgs;

            /**
             * VarArgs with one argument (key only).
             *
             * @tparam Ek The entry key type
             * @throws AssertionError always, because the value is missing.
             */
            template <class Ek>
            class VarArgs<Ek> final : public Object {
            public:
                /**
                 * Check Compatibility of all entries with
                 * this map template.
                 */
                static CORE_FAST gbool check() { return false; }

                /**
                 * Collect all entries key and value
                 */
                static gbool init(gint, Array<>&, BooleanArray&, Ek) { return false; }
            };

            /**
             * VarArgs with two arguments.
             *
             * @tparam Ek the key type
             * @tparam Ev the entry value type
             */
            template <class Ek, class Ev>
            class VarArgs<Ek, Ev> final : public Object {
            public:
                /**
                 * Check Compatibility of all entries with
                 * this map template.
                 */
                static CORE_FAST gbool check() {
                    return Class<K>::template accept<Ek>() && Class<V>::template accept<Ev>();
                }

                /**
                 * Collect all entries key and value
                 */
                static void init(gint index, Array<>& a, BooleanArray& b, Ek key, Ev value) {
                    try {
                        Array<K> ak = Array<K>::of(key);
                        Array<V> av = Array<V>::of(value);

                        Arrays::fastSet(a, index + 0, ak[0]);
                        Arrays::fastSet(a, index + 1, av[0]);
                        b[index + 0] = isSameObject(key, ak[0]);
                        b[index + 1] = isSameObject(key, av[0]);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                /**
                 * Test if two object have same memory address
                 */
                template <class E1, class E2>
                static gbool isSameObject(E1 const& e1, E2 const& e2) {
                    return CORE_CAST(glong, &e1) == CORE_CAST(glong, &e2);
                }
            };

            template <class Ek, class Ev, class... Entries>
            class VarArgs<Ek, Ev, Entries...> final : public Object {
                CORE_FAST_XASSERT(sizeof...(Entries) % 2 == 0, "Number of arguments is not sufficient.");

            public:
                /**
                 * Check Compatibility of all entries with
                 * this map template.
                 */
                static CORE_FAST gbool check() {
                    return VarArgs<Ek, Ev>::check() && VarArgs<Entries...>::check();
                }

                /**
                 * Collect all entries key and value
                 */
                static void init(gint index, Array<>& a, BooleanArray& b,
                                 Ek key, Ev value, Entries... entries) {
                    try {
                        VarArgs<Ek, Ev>::init(index, a, b, key, value);
                        VarArgs<Entries...>::init(index + 2, a, b, entries...);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            };

            static Array<Entry> organize(Array<>& table, BooleanArray const& status) {
                class Dummy final : public Object {
                public:
                    Object&clone() const override { return CORE_CAST(Dummy &, *this); }
                };
                // Dummy value
                Dummy DUMMY;
                gint len = table.length() >> 1;
                // Result array
                Array<> a = Array<>(len, DUMMY);
                // number of element present
                gint present = 0;

                try {
                    // Convert all inline entries array to non-inline entries hashed array
                    IntArray indexes = IntArray(len, -1);
                    for (gint i = 0; i < (len << 1); i += 2) {
                        K& tk = CORE_XCAST(K, table[i]);
                        gint index = Math::floorMod(tk.hash(), len);

                        // Find index of new Entry
                        while (true) {
                            Object& e = a[index];
                            // Unused place
                            if (DUMMY == e) {
                                V& tv = CORE_XCAST(V, table[i + 1]);
                                Entry& entry = newEntry(tk, tv);
                                Arrays::fastSet(a, index, entry);
                                indexes[index] = i;
                                present++;
                                break;
                            }

                            Entry& oldEntry = CORE_XCAST(UnmodifiableEntry, e);
                            // Key Already exist (Replace entry)
                            if (tk == oldEntry.getKey()) {
                                // new key is newly created key (Replace value only)
                                if (!status[i]) {
                                    UNSAFE::deleteInstance(tk);

                                    V& tv = CORE_XCAST(V, table[i + 1]);
                                    Entry& entry = newEntry(oldEntry.getKey(), tv);
                                    Arrays::fastSet(a, index, entry);

                                    // Destroy if possible the old value
                                    gint j = indexes[index + 1];
                                    if (j != -1)
                                        UNSAFE::deleteInstance(oldEntry.getValue());
                                    UNSAFE::deleteInstance(oldEntry);
                                    break;
                                }

                                // new key is not newly created key
                                // old key is newly created key (Replace key and value)
                                if (!status[index]) {
                                    UNSAFE::deleteInstance(oldEntry.getKey());
                                    V& tv = CORE_XCAST(V, table[i + 1]);
                                    Entry& entry = newEntry(tk, tv);
                                    Arrays::fastSet(a, index, entry);
                                    // Destroy if possible the old value
                                    gint j = indexes[index + 1];
                                    if (j != -1)
                                        UNSAFE::deleteInstance(oldEntry.getValue());
                                    UNSAFE::deleteInstance(oldEntry);
                                    break;
                                }

                                // new key is not newly created key
                                // old key is not newly created key (Replace value only)
                                {
                                    V& tv = CORE_XCAST(V, table[i + 1]);
                                    Entry& entry = newEntry(oldEntry.getKey(), tv);
                                    Arrays::fastSet(a, index, entry); // Destroy if possible the old value
                                    gint j = indexes[index + 1];
                                    if (j != -1)
                                        UNSAFE::deleteInstance(oldEntry.getValue());
                                    UNSAFE::deleteInstance(oldEntry);
                                    break;
                                }
                            }

                            // Place Already used by another entry (find new place)
                            if (++index == len)
                                index = 0;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                try {
                    // Remove all Dummy place on result
                    if (present < len) {
                        gint offset = 0;
                        for (int i = 0; i < len; i++) {
                            Object& obj = a[i];
                            if (DUMMY == obj) {
                                offset--;
                                continue;
                            }

                            Arrays::swap(a, i, i + offset);
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                try {
                    // Convert result to Array<Entry<?, ?>>
                    return Arrays::copyOf<Entry>(a, present);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            template <class X, class Y>
            static Array<Entry> organize(Array<util::Entry<X, Y>> const& table) {
                class Dummy final : public Object {
                public:
                    Object&clone() const override { return CORE_CAST(Dummy &, *this); }
                };
                Dummy DUMMY;
                gint len = table.length();
                Array<> a = Array<>(len, DUMMY);
                gint present = 0;

                try {
                    BooleanArray ste = BooleanArray(len, true);
                    BooleanArray stk = BooleanArray(len, true);
                    BooleanArray stv = BooleanArray(len, true);
                    gint i = -1;
                    for (util::Entry<X, Y> const& te : table) {
                        i += 1;
                        X const& tk = te.getKey();

                        gint index = Math::floorMod(tk.hash(), len);

                        // Find index
                        while (true) {
                            Object& e = a[index];

                            // Empty place
                            if (DUMMY == e) {
                                // Convert Entry to Unmodifiable entry
                                if (Class<UnmodifiableEntry>::hasInstance(te))
                                    Arrays::fastSet(a, index, CORE_CAST($(Entry &), te));
                                else {
                                    Y const& tv = te.getValue();
                                    Entry& entry = toUnmodifiable(te);
                                    Arrays::fastSet(a, index, entry);

                                    stk[index] = &tk == &entry.getKey();
                                    stv[index] = &tv == &entry.getValue();
                                    ste[index] = false;
                                }

                                present++;
                                break;
                            }

                            Entry& oldEntry = toUnmodifiable(CORE_XCAST($(Entry), e));
                            // Key already exist (Replace current entry)
                            if (tk == oldEntry.getKey()) {
                                gbool sk = true;
                                gbool sv = true;
                                gbool se = true;
                                if (Class<UnmodifiableEntry>::hasInstance(te)) {
                                    Arrays::fastSet(a, index, toUnmodifiable(te));
                                    ste[index] = true;
                                } else {
                                    Y const& tv = te.getValue();
                                    Entry& entry = toUnmodifiable(te);
                                    Arrays::fastSet(a, index, entry);

                                    sk = &tk == &entry.getKey();
                                    sv = &tv == &entry.getValue();
                                    se = false;
                                }

                                if (!stk[index])
                                    UNSAFE::deleteInstance(oldEntry.getKey());
                                if (!stv[index])
                                    UNSAFE::deleteInstance(oldEntry.getValue());
                                if (!ste[index])
                                    UNSAFE::deleteInstance(oldEntry);
                                stk[index] = sk;
                                stv[index] = sv;
                                ste[index] = se;

                                break;
                            }

                            // Place is already used by another entry
                            if (++index == len)
                                index = 0;
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                try {
                    // Remove all Dummy place on result
                    if (present < len) {
                        gint offset = 0;
                        for (int i = 0; i < len; i++) {
                            Object& obj = a[i];
                            if (DUMMY == obj) {
                                offset -= 1;
                                continue;
                            }

                            Arrays::swap(a, i, i + offset);
                        }
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                try {
                    return Arrays::copyOf<Entry>(a, present);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            static UnmodifiableEntry& toUnmodifiable(Entry const& entry) {
                if (Class<UnmodifiableEntry>::hasInstance(entry))
                    return (UnmodifiableEntry&) entry;
                return (UnmodifiableEntry&) newEntry(entry.getKey(), entry.getValue());
            }

            template <class X, class Y>
            static UnmodifiableEntry& toUnmodifiable(util::Entry<X, Y> const& entry) {
                return (UnmodifiableEntry&) newEntry(entry.getKey(), entry.getValue());
            }

            static gbool isSameObject(Object const& o1, Object const& o2) { return &o1 == &o2; }
        };

        template <class K, class V>
        class Map<K, V>::KeySet final : public Set<K> {
            Map& m;

        public:
            CORE_EXPLICIT KeySet(Map const& m) : m((Map&) m) {}

            gint size() const override {
                try { return m.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return m.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const K& val) const override {
                try { return m.containsKey(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<K>& iterator() override {
                class Itr final : public Iterator<K> {
                    Iterator<Entry>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry>& it) : it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    K&next() override {
                        try { return it.next().getKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        try { it.remove(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }
                };

                try {
                    Iterator<Entry>& it = m.entrySet().iterator();
                    return UNSAFE::newInstance<Itr>(it);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<K>& iterator() const override {
                class Itr final : public Iterator2<K> {
                    Iterator<Entry>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry>& it) : it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    K const&next() override {
                        try { return it.next().getKey(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }
                };

                try {
                    Iterator<Entry>& it = m.entrySet().iterator();
                    return UNSAFE::newInstance<Itr>(it);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const K&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool remove(const K& val) override {
                try {
                    m.remove(val);
                    return true;
                } catch (MissingKeyException const&) {
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try { m.clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class K, class V>
        class Map<K, V>::Values final : public Collection<V> {
            Map<K, V>& m;

        public:
            CORE_EXPLICIT Values(Map const& m) : m((Map&) m) {}

            gint size() const override {
                try { return m.size(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return m.isEmpty(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(const V& val) const override {
                try { return m.containsValue(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<V>& iterator() override {
                class Itr final : public Iterator<V> {
                    Iterator<Entry>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry>& it) : it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    V&next() override {
                        try { return it.next().getValue(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        try { it.remove(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }
                };

                try {
                    Iterator<Entry>& it = m.entrySet().iterator();
                    return UNSAFE::newInstance<Itr>(it);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<V>& iterator() const override {
                class Itr final : public Iterator2<V> {
                    Iterator<Entry>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry>& it) : it(it) {}

                    gbool hasNext() const override {
                        try { return it.hasNext(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    V const&next() override {
                        try { return it.next().getValue(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }
                };

                try {
                    Iterator<Entry>& it = m.entrySet().iterator();
                    return UNSAFE::newInstance<Itr>(it);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(const V&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool addAll(const Collection<V>&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                try { return m.clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };

        template <class K, class V>
        class Map<K, V>::UnmodifiableEntry final : public Entry {
        public:
            K& key;
            V& value;

            CORE_EXPLICIT UnmodifiableEntry(K& key, V& value) : key(key), value(value) {}

            K& getKey() override { return key; }

            const K& getKey() const override { return key; }

            V& getValue() override { return value; }

            const V& getValue() const override { return value; }

            V& setValue(const V&) override {
                UnsupportedOperationException().throws($ftrace());
            }
        };

        template <class K, class V>
        class Map<K, V>::UnmodifiableMap : public virtual Map<K, V> {
        public:
            Object& put(const K&, const V&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            V& remove(const K&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void putAll(const Map<K, V>&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                UnsupportedOperationException().throws($ftrace());
            }

            void replaceAll(const function::BiFunction<K&, V&, V>&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool replace(const K&, const V&, const V&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            Object& replace(const K&, const V&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            Object& putIfAbsent(const K&, const V&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool remove(const K&, const V&) override {
                UnsupportedOperationException().throws($ftrace());
            }
        };

        template <class K, class V>
        class Map<K, V>::EmptyMap final : public UnmodifiableMap {
            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);

        public:
            gint size() const override { return 0; }

            gbool isEmpty() const override { return true; }

            gbool containsKey(const K&) const override { return false; }

            gbool containsValue(const V&) const override { return false; }

            V& get(const K& key) override {
                MissingKeyException(key).throws($ftrace());
            }

            const V& get(const K& key) const override {
                MissingKeyException(key).throws($ftrace());
            }

            Set<K>& keySet() override {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            const Set<K>& keySet() const override {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            Collection<V>& values() override {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            const Collection<V>& values() const override {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            Set<Entry>& entrySet() override {
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(*this);
                return *entries;
            }

            const Set<Entry>& entrySet() const override {
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(*this);
                return *entries;
            }

            gint hash() const override { return 0; }

            V& getOrDefault(const K&, const V& defaultValue) override {
                try { return UNSAFE::copyInstance(defaultValue); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& getOrDefault(const K&, const V& defaultValue) const override {
                try { return UNSAFE::copyInstance(defaultValue); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& getOrNull(const K&) override { return null; }

            const Object& getOrNull(const K&) const override { return null; }

            void forEach(const function::BiConsumer<K&, V&>&) override {}

            void forEach(const function::BiConsumer<K, V>&) const override {}

            ~EmptyMap() override {
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

        private:
            class KeySet final : public Set<K> {
                EmptyMap const& m;

            public:
                CORE_EXPLICIT KeySet(const EmptyMap& m) : m(m) {}

                gint size() const override { return 0; }

                gbool isEmpty() const override { return true; }

                gbool contains(const K& val) const override { return false; }

                gbool add(const K& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool remove(const K& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool addAll(const Collection<K>& c) override { UnsupportedOperationException().throws($ftrace()); }

                gbool containsAll(const Collection<K>& c) const override { return false; }

                gbool retainAll(const Collection<K>& c) override { UnsupportedOperationException().throws($ftrace()); }

                gbool removeAll(const Collection<K>& c) override { UnsupportedOperationException().throws($ftrace()); }

                gbool removeIf(const function::Predicate<K>& filter) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override { UnsupportedOperationException().throws($ftrace()); }

                gbool equals(const Object& o) const override {
                    return this == &o || Class<Set<K>>::hasInstance(o) && CORE_XCAST(Set<K> const, o).size() == 0;
                }

                gint hash() const override { return 0; }

                String toString() const override { return "[]"_Sl; }

                Array<K> toArray() const override { return Array<K>(); }

                Iterator<K>& iterator() override { return Collection<K>::emptyIterator(); }

                Iterator2<K>& iterator() const override { return Collection<K>::emptyIterator2(); }

                Spliterator<K>& spliterator() override { return Spliterators::emptySpliterator<K>(); }

                Spliterator2<K>& spliterator() const override { return Spliterators::emptySpliterator2<K>(); }

                void forEach(const function::Consumer<K&>& action) override {}

                void forEach(const function::Consumer<K>& action) const override {}

                ~KeySet() override { detachKeys(m); }
            };

            class Values final : public Collection<V> {
                EmptyMap const& m;

            public:
                CORE_EXPLICIT Values(const EmptyMap& m) : m(m) {}

                gint size() const override { return 0; }

                gbool isEmpty() const override { return true; }

                gbool contains(const V& val) const override { return false; }

                Iterator<V>& iterator() override { return Collection<V>::emptyIterator(); }

                Iterator2<V>& iterator() const override { return Collection<V>::emptyIterator2(); }

                Array<V> toArray() const override { return Array<V>(); }

                gbool add(const V& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool remove(const V& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool containsAll(const Collection<V>& c) const override { return false; }

                gbool addAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeIf(const function::Predicate<V>& filter) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool retainAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override { UnsupportedOperationException().throws($ftrace()); }

                gbool equals(const Object& o) const override {
                    return this == &o ||
                            Class<Collection<V>>::hasInstance(o) && CORE_XCAST(Collection<V> const, o).size() == 0;
                }

                gint hash() const override { return 0; }

                Spliterator<V>& spliterator() override { return Spliterators::emptySpliterator<V>(); }

                Spliterator2<V>& spliterator() const override { return Spliterators::emptySpliterator2<V>(); }

                String toString() const override { return "[]"; }

                void forEach(const function::Consumer<V&>& action) override {}

                void forEach(const function::Consumer<V>& action) const override {}

                ~Values() override { detachVals(m); }
            };

            class EntrySet final : public Set<Entry> {
                EmptyMap const& m;

            public:
                CORE_EXPLICIT EntrySet(const EmptyMap& m) : m(m) {}

                gint size() const override { return 0; }

                gbool isEmpty() const override { return true; }

                gbool contains(const Entry& val) const override { return false; }

                Iterator<Entry>& iterator() override { return Collection<Entry>::emptyIterator(); }

                Iterator2<Entry>& iterator() const override { return Collection<Entry>::emptyIterator2(); }

                Array<Entry> toArray() const override { return Array<Entry>(); }

                gbool add(const Entry& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool remove(const Entry& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool containsAll(const Collection<Entry>& c) const override { return false; }

                gbool addAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool retainAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeIf(const function::Predicate<Entry>& filter) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override { UnsupportedOperationException().throws($ftrace()); }

                gbool equals(const Object& o) const override {
                    return this == &o ||
                            Class<Set<Entry>>::hasInstance(o) && CORE_XCAST(Set<Entry> const, o).size() == 0;
                }

                gint hash() const override { return 0; }

                Spliterator<Entry>& spliterator() override {
                    return Spliterators::emptySpliterator<Entry>();
                }

                Spliterator2<Entry>& spliterator() const override {
                    return Spliterators::emptySpliterator2<Entry>();
                }

                String toString() const override { return "[]"_Sl; }

                void forEach(const function::Consumer<Entry&>& action) override {}

                void forEach(const function::Consumer<Entry>& action) const override {}

                ~EntrySet() override { detachEntries(m); }
            };
        };

        template <class K, class V>
        class Map<K, V>::Singleton final : public UnmodifiableMap {
            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);

            UnmodifiableEntry& e;

        public:
            CORE_EXPLICIT Singleton(Entry& e) : e(toUnmodifiable(e)) {}

            gint size() const override { return 1; }

            gbool isEmpty() const override { return false; }

            gbool containsKey(const K& key) const override { return key == e.key; }

            gbool containsValue(const V& value) const override { return value == e.value; }

            V& get(const K& key) override {
                if (key == e.key) return e.value;

                MissingKeyException(key).throws($ftrace());
            }

            const V& get(const K& key) const override {
                if (key == e.key) return e.value;

                MissingKeyException(key).throws($ftrace());
            }

            void putAll(const Map&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            Set<K>& keySet() override {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            const Set<K>& keySet() const override {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            Collection<V>& values() override {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            const Collection<V>& values() const override {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            Set<Entry>& entrySet() override {
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(*this);
                return *entries;
            }

            const Set<Entry>& entrySet() const override {
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(*this);
                return *entries;
            }

            gint hash() const override {
                try { return e.hash(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& getOrDefault(const K& key, const V& defaultValue) override {
                try {
                    if (key == e.key)
                        return e.value;

                    return UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& getOrDefault(const K& key, const V& defaultValue) const override {
                try {
                    if (key == e.key)
                        return e.value;

                    return UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& getOrNull(const K& key) override {
                try {
                    if (key == e.key)
                        return e.value;

                    return null;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Object& getOrNull(const K& key) const override {
                try {
                    if (key == e.key)
                        return e.value;

                    return null;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::BiConsumer<K&, V&>& action) override {
                try {
                    action.accept(e.key, e.value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::BiConsumer<K, V>& action) const override {
                try {
                    action.accept(e.key, e.value);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        private:
            class KeySet final : public Set<K> {
                Singleton const& m;

            public:
                CORE_EXPLICIT KeySet(const Singleton& m) : m(m) {}

                gint size() const override { return 1; }

                gbool isEmpty() const override { return false; }

                gbool contains(const K& val) const override { return val == m.e.key; }

                Iterator<K>& iterator() override {
                    class Itr final : public Iterator<K> {
                        Singleton const& m;
                        gbool isEnd = false;

                    public:
                        CORE_EXPLICIT Itr(const Singleton& m) : m(m) {}

                        gbool hasNext() const override { return !isEnd; }

                        K&next() override {
                            if (isEnd) NoSuchElementException().throws($ftrace());
                            isEnd = true;
                            return m.e.key;
                        }

                        void remove() override { UnsupportedOperationException().throws($ftrace()); }

                        void forEach(const function::Consumer<K&>& action) override {
                            try {
                                action.accept(m.e.key);
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (!Class<Itr>::hasInstance(o))
                                return false;
                            Itr const& it = CORE_XCAST(Itr const, o);
                            return &m == &it.m && isEnd == it.isEnd;
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Iterator2<K>& iterator() const override {
                    class Itr final : public Iterator2<K> {
                        Singleton const& m;
                        gbool isEnd = false;

                    public:
                        CORE_EXPLICIT Itr(const Singleton& m) : m(m) {}

                        gbool hasNext() const override { return !isEnd; }

                        K const&next() override {
                            if (isEnd) NoSuchElementException().throws($ftrace());
                            isEnd = true;
                            return m.e.key;
                        }

                        void remove() override { UnsupportedOperationException().throws($ftrace()); }

                        void forEach(const function::Consumer<K>& action) override {
                            try {
                                action.accept(m.e.key);
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (!Class<Itr>::hasInstance(o))
                                return false;
                            Itr const& it = CORE_XCAST(Itr const, o);
                            return &m == &it.m && isEnd == it.isEnd;
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Array<K> toArray() const override { return Array<K>(1, m.e.key); }

                gbool add(const K& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool remove(const K& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool containsAll(const Collection<K>& c) const override {
                    for (K const& elt : c) {
                        if (!m.containsKey(elt))
                            return false;
                    }
                    return true;
                }

                gbool addAll(const Collection<K>& c) override { UnsupportedOperationException().throws($ftrace()); }

                gbool retainAll(const Collection<K>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeAll(const Collection<K>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override { UnsupportedOperationException().throws($ftrace()); }

                gbool equals(const Object& o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Set<K>>::hasInstance(o))
                        return false;
                    Set<K> const& ks = CORE_XCAST(Set<K> const, o);
                    return ks.size() == 1 && ks.contains(m.e.key);
                }

                gint hash() const override {
                    try {
                        return m.e.key.hash();
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                void forEach(const function::Consumer<K&>& action) override {
                    try {
                        return action.accept(m.e.key);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                void forEach(const function::Consumer<K>& action) const override {
                    try {
                        return action.accept(m.e.key);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                ~KeySet() override { detachKeys(m); }
            };

            class Values final : public Collection<V> {
                Singleton const& m;

            public:
                CORE_EXPLICIT Values(const Singleton& m) : m(m) {}

                gint size() const override { return 1; }

                gbool isEmpty() const override { return false; }

                gbool contains(const V& val) const override { return val == m.e.value; }

                Iterator<V>& iterator() override {
                    class Itr final : public Iterator<V> {
                        Singleton const& m;
                        gbool isEnd = false;

                    public:
                        CORE_EXPLICIT Itr(const Singleton& m) : m(m) {}

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (Class<Itr>::hasInstance(o)) {
                                Itr const& it = CORE_XCAST(Itr const, o);
                                return &m == &it.m && isEnd == it.isEnd;
                            }
                            return false;
                        }

                        gbool hasNext() const override { return !isEnd; }

                        V&next() override {
                            if (isEnd) NoSuchElementException().throws($ftrace());
                            isEnd = true;
                            return m.e.value;
                        }

                        void remove() override { UnsupportedOperationException().throws($ftrace()); }

                        void forEach(const function::Consumer<V&>& action) override {
                            try {
                                if (!isEnd) action.accept(m.e.value);
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Iterator2<V>& iterator() const override {
                    class Itr final : public Iterator2<V> {
                        Singleton const& m;
                        gbool isEnd = false;

                    public:
                        CORE_EXPLICIT Itr(const Singleton& m) : m(m) {}

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (Class<Itr>::hasInstance(o)) {
                                Itr const& it = CORE_XCAST(Itr const, o);
                                return &m == &it.m && isEnd == it.isEnd;
                            }
                            return false;
                        }

                        gbool hasNext() const override { return !isEnd; }

                        V const&next() override {
                            if (isEnd) NoSuchElementException().throws($ftrace());
                            isEnd = true;
                            return m.e.value;
                        }

                        void remove() override { UnsupportedOperationException().throws($ftrace()); }

                        void forEach(const function::Consumer<V>& action) override {
                            try {
                                if (!isEnd) action.accept(m.e.value);
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Array<V> toArray() const override { return Array<V>(1, m.e.value); }

                gbool add(const V& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool remove(const V& val) override { UnsupportedOperationException().throws($ftrace()); }

                gbool containsAll(const Collection<V>& c) const override {
                    for (Object const& obj : c) {
                        if (obj != m.e.value)
                            return false;
                    }
                    return true;
                }

                gbool addAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeIf(const function::Predicate<V>& filter) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool retainAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override { UnsupportedOperationException().throws($ftrace()); }

                gbool equals(const Object& other) const override {
                    if (this == &other)
                        return true;
                    if (Class<Collection<V>>::hasInstance(other)) {
                        Collection<V> const& c = CORE_XCAST(Collection<V> const, other);
                        return c.size() == 1 && c.contains(m.e.value);
                    }
                    return false;
                }

                gint hash() const override {
                    return m.e.value.hash();
                }

                Spliterator<V>& spliterator() override {
                    return Spliterators::spliterator<V>(toArray(), 0);
                }

                Spliterator2<V>& spliterator() const override {
                    return Spliterators::spliterator2<V>(toArray(), 0);
                }

                void forEach(const function::Consumer<V&>& action) override {
                    try {
                        action.accept(m.e.value);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                void forEach(const function::Consumer<V>& action) const override {
                    try {
                        action.accept(m.e.value);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                ~Values() override { detachVals(m); }
            };

            class EntrySet final : public Set<Entry> {
                Singleton const& m;

            public:
                CORE_EXPLICIT EntrySet(const Singleton& m) : m(m) {}

                gint size() const override { return 1; }

                gbool isEmpty() const override { return false; }

                gbool contains(const Entry& val) const override { return val == m.e; }

                Iterator<Entry>& iterator() override {
                    class Itr final : public Iterator<Entry> {
                        Singleton const& m;
                        gbool isEnd = false;

                    public:
                        CORE_EXPLICIT Itr(const Singleton& m) : m(m) {}

                        gbool hasNext() const override { return !isEnd; }

                        Entry&next() override {
                            if (isEnd) NoSuchElementException().throws($ftrace());
                            isEnd = true;
                            return m.e;
                        }

                        void remove() override { UnsupportedOperationException().throws($ftrace()); }

                        void forEach(const function::Consumer<Entry&>& action) override {
                            try {
                                action.accept(m.e);
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (!Class<Itr>::hasInstance(o))
                                return false;
                            Itr const& it = CORE_XCAST(Itr const, o);
                            return &m == &it.m && isEnd == it.isEnd;
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Iterator2<Entry>& iterator() const override {
                    class Itr final : public Iterator2<Entry> {
                        Singleton const& m;
                        gbool isEnd = false;

                    public:
                        CORE_EXPLICIT Itr(const Singleton& m) : m(m) {}

                        gbool hasNext() const override { return !isEnd; }

                        Entry const&next() override {
                            if (isEnd) NoSuchElementException().throws($ftrace());
                            isEnd = true;
                            return m.e;
                        }

                        void remove() override { UnsupportedOperationException().throws($ftrace()); }

                        void forEach(const function::Consumer<Entry>& action) override {
                            try {
                                action.accept(m.e);
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (!Class<Itr>::hasInstance(o))
                                return false;
                            Itr const& it = CORE_XCAST(Itr const, o);
                            return &m == &it.m && isEnd == it.isEnd;
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Array<Entry> toArray() const override { return Array<Entry>(1, m.e); }

                gbool add(const Entry& val) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool remove(const Entry& val) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool containsAll(const Collection<Entry>& c) const override {
                    for (Object const& obj : c)
                        if (obj != m.e)
                            return false;
                    return true;
                }

                gbool addAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool retainAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool equals(const Object& o) const override {
                    if (this == &o)
                        return true;
                    if (Class<Set<Entry>>::hasInstance(o)) {
                        Set<Entry> const& s = CORE_XCAST(Set<Entry> const, o);
                        return s.size() == 1 && s.contains(m.e);
                    }
                    return false;
                }

                gint hash() const override {
                    return m.e.hash();
                }

                void forEach(const function::Consumer<Entry&>& action) override {
                    action.accept(m.e);
                }

                void forEach(const function::Consumer<Entry>& action) const override {
                    action.accept(m.e);
                }

                gbool removeIf(const function::Predicate<Entry>& filter) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                Spliterator<Entry>& spliterator() override {
                    return Spliterators::spliterator<Entry>(toArray(), 0);
                }

                Spliterator2<Entry>& spliterator() const override {
                    return Spliterators::spliterator2<Entry>(toArray(), 0);
                }

                ~EntrySet() override { detachEntries(m); }
            };
        };

        template <class K, class V>
        class Map<K, V>::MapN final : public UnmodifiableMap {
            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);

            Array<Entry> table;

        public:
            CORE_EXPLICIT MapN(Array<Entry> const& table) : table(table) {}

            gint size() const override { return table.length(); }

            gbool isEmpty() const override { return false; }

            gbool containsKey(const K& key) const override { return indexOf(key) >= 0; }

            gbool containsValue(const V& value) const override {
                for (Entry const& entry : table)
                    if (value == entry.getValue())
                        return true;

                return false;
            }

            V& get(const K& key) override {
                gint index = indexOf(key);
                if (index >= 0)
                    return table[index].getValue();

                MissingKeyException(key).throws($ftrace());
            }

            const V& get(const K& key) const override {
                gint index = indexOf(key);
                if (index >= 0)
                    return table[index].getValue();

                MissingKeyException(key).throws($ftrace());
            }

            void putAll(const Map&) override {
                UnsupportedOperationException().throws($ftrace());
            }

            Set<K>& keySet() override {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            const Set<K>& keySet() const override {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *keys;
            }

            Collection<V>& values() override {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            const Collection<V>& values() const override {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *vals;
            }

            Set<Entry>& entrySet() override {
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(*this);
                return *entries;
            }

            const Set<Entry>& entrySet() const override {
                if (entries == null)
                    entries = &UNSAFE::newInstance<EntrySet>(*this);
                return *entries;
            }

            gint hash() const override {
                try { return table.hash(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& getOrDefault(const K& key, const V& defaultValue) override {
                try {
                    gint index = indexOf(key);
                    if (index >= 0)
                        return table[index].getValue();

                    return UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& getOrDefault(const K& key, const V& defaultValue) const override {
                try {
                    gint index = indexOf(key);
                    if (index >= 0)
                        return table[index].getValue();

                    return UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Object& getOrNull(const K& key) override {
                try {
                    gint index = indexOf(key);
                    if (index >= 0)
                        return table[index].getValue();

                    return null;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Object& getOrNull(const K& key) const override {
                try {
                    gint index = indexOf(key);
                    if (index >= 0)
                        return table[index].getValue();

                    return null;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::BiConsumer<K&, V&>& action) override {
                try {
                    for (Entry& entry : table)
                        action.accept(entry.getKey(), entry.getValue());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::BiConsumer<K, V>& action) const override {
                try {
                    for (Entry const& entry : table)
                        action.accept(entry.getKey(), entry.getValue());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        private:
            // ::::::::::::::::::::: Utility ::::::::::::::::::::::::::::
            gint indexOf(K const& key) const {
                gint length = table.length();
                if (length == 0)
                    return -1;

                gint index = Math::floorMod(key.hash(), length);
                gint n = 0;
                while (n < length) {
                    Entry const& entry = table[index];
                    if (key == entry.getKey())
                        return index;

                    if (++index == length)
                        index = 0;

                    n += 1;
                }

                return -1;
            }

            // ::::::::::::::::::::: Views ::::::::::::::::::::::::::::
            class KeySet final : public Set<K> {
                MapN& m;

            public:
                CORE_EXPLICIT KeySet(MapN const& m) : m((MapN&) m) {}

                Iterator<K>& iterator() override {
                    class Itr final : public Iterator<K> {
                        MapN& m;
                        gint cursor = 0;

                    public:
                        CORE_EXPLICIT Itr(MapN& m) : m(m) {}

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (Class<Itr>::hasInstance(o)) {
                                Itr const& it = CORE_XCAST(Itr const, o);
                                return &m == &it.m && cursor == it.cursor;
                            }
                            return false;
                        }

                        gbool hasNext() const override {
                            return cursor < m.table.length();
                        }

                        K&next() override {
                            gint i = cursor++;
                            if (i >= m.table.length())
                                NoSuchElementException().throws($ftrace());
                            return m.table[i].getKey();
                        }

                        void remove() override {
                            UnsupportedOperationException().throws($ftrace());
                        }

                        void forEach(const function::Consumer<K&>& action) override {
                            for (gint i = cursor; cursor < m.table.length(); cursor++)
                                action.accept(m.table[i++].getKey());
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Iterator2<K>& iterator() const override {
                    class Itr final : public Iterator2<K> {
                        MapN& m;
                        gint cursor = 0;

                    public:
                        CORE_EXPLICIT Itr(MapN& m) : m(m) {}

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (Class<Itr>::hasInstance(o)) {
                                Itr const& it = CORE_XCAST(Itr const, o);
                                return &m == &it.m && cursor == it.cursor;
                            }
                            return false;
                        }

                        gbool hasNext() const override {
                            return cursor < m.table.length();
                        }

                        K const&next() override {
                            gint i = cursor++;
                            if (i >= m.table.length())
                                NoSuchElementException().throws($ftrace());
                            return m.table[i].getKey();
                        }

                        void remove() override {
                            UnsupportedOperationException().throws($ftrace());
                        }

                        void forEach(const function::Consumer<K>& action) override {
                            for (gint i = cursor; cursor < m.table.length(); cursor++)
                                action.accept(m.table[i++].getKey());
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                void forEach(const function::Consumer<K&>& action) override {
                    for (Entry& e : m.table)
                        action.accept(e.getKey());
                }

                void forEach(const function::Consumer<K>& action) const override {
                    for (Entry const& e : m.table)
                        action.accept(e.getKey());
                }

                ~KeySet() override { detachKeys(m); }

                gbool addAll(const Collection<K>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeAll(const Collection<K>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeIf(const function::Predicate<K>& filter) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool retainAll(const Collection<K>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gint size() const override {
                    return m.table.length();
                }

                gbool isEmpty() const override {
                    return m.table.isEmpty();
                }

                gbool contains(const K& val) const override { return m.containsKey(val); }

                gbool add(const K& val) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool remove(const K& val) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override {
                    UnsupportedOperationException().throws($ftrace());
                }
            };

            class Values final : public Collection<V> {
                MapN& m;

            public:
                CORE_EXPLICIT Values(MapN const& m) : m((MapN&) m) {}

                Iterator<V>& iterator() override {
                    class Itr final : public Iterator<V> {
                        MapN& m;
                        gint cursor = 0;

                    public:
                        CORE_EXPLICIT Itr(MapN& m) : m(m) {}

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (Class<Itr>::hasInstance(o)) {
                                Itr const& it = CORE_XCAST(Itr const, o);
                                return &m == &it.m && cursor == it.cursor;
                            }
                            return false;
                        }

                        gbool hasNext() const override {
                            return cursor < m.table.length();
                        }

                        V&next() override {
                            gint i = cursor++;
                            if (i >= m.table.length())
                                NoSuchElementException().throws($ftrace());
                            return m.table[i].getValue();
                        }

                        void remove() override {
                            UnsupportedOperationException().throws($ftrace());
                        }

                        void forEach(const function::Consumer<V&>& action) override {
                            for (gint i = cursor; cursor < m.table.length(); cursor++)
                                action.accept(m.table[i++].getValue());
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Iterator2<V>& iterator() const override {
                    class Itr final : public Iterator2<V> {
                        MapN& m;
                        gint cursor = 0;

                    public:
                        CORE_EXPLICIT Itr(MapN& m) : m(m) {}

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (Class<Itr>::hasInstance(o)) {
                                Itr const& it = CORE_XCAST(Itr const, o);
                                return &m == &it.m && cursor == it.cursor;
                            }
                            return false;
                        }

                        gbool hasNext() const override {
                            return cursor < m.table.length();
                        }

                        V const&next() override {
                            gint i = cursor++;
                            if (i >= m.table.length())
                                NoSuchElementException().throws($ftrace());
                            return m.table[i].getValue();
                        }

                        void remove() override {
                            UnsupportedOperationException().throws($ftrace());
                        }

                        void forEach(const function::Consumer<V>& action) override {
                            for (gint i = cursor; cursor < m.table.length(); cursor++)
                                action.accept(m.table[i++].getValue());
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                void forEach(const function::Consumer<V&>& action) override {
                    for (Entry& e : m.table)
                        action.accept(e.getValue());
                }

                void forEach(const function::Consumer<V>& action) const override {
                    for (Entry& e : m.table)
                        action.accept(e.getValue());
                }

                ~Values() override { detachVals(m); }

                gint size() const override { return m.table.length(); }

                gbool isEmpty() const override { return m.table.isEmpty(); }

                gbool contains(const V& val) const override { return m.containsValue(val); }

                gbool add(const V& val) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool remove(const V& val) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool addAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeIf(const function::Predicate<V>& filter) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool retainAll(const Collection<V>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override {
                    UnsupportedOperationException().throws($ftrace());
                }
            };

            class EntrySet final : public Set<Entry> {
                MapN& m;

            public:
                CORE_EXPLICIT EntrySet(MapN const& m) : m((MapN&) m) {}

                Iterator<Entry>& iterator() override {
                    class Itr final : public Iterator<Entry> {
                        MapN& m;
                        gint cursor = 0;

                    public:
                        CORE_EXPLICIT Itr(MapN& m) : m(m) {}

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (Class<Itr>::hasInstance(o)) {
                                Itr const& it = CORE_XCAST(Itr const, o);
                                return &m == &it.m && cursor == it.cursor;
                            }
                            return false;
                        }

                        gbool hasNext() const override {
                            return cursor < m.table.length();
                        }

                        Entry&next() override {
                            gint i = cursor++;
                            if (i >= m.table.length())
                                NoSuchElementException().throws($ftrace());
                            return m.table[i];
                        }

                        void remove() override {
                            UnsupportedOperationException().throws($ftrace());
                        }

                        void forEach(const function::Consumer<Entry&>& action) override {
                            for (gint i = cursor; cursor < m.table.length(); cursor++)
                                action.accept(m.table[i++]);
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                Iterator2<Entry>& iterator() const override {
                    class Itr final : public Iterator2<Entry> {
                        MapN& m;
                        gint cursor = 0;

                    public:
                        CORE_EXPLICIT Itr(MapN& m) : m(m) {}

                        gbool equals(const Object& o) const override {
                            if (this == &o)
                                return true;
                            if (Class<Itr>::hasInstance(o)) {
                                Itr const& it = CORE_XCAST(Itr const, o);
                                return &m == &it.m && cursor == it.cursor;
                            }
                            return false;
                        }

                        gbool hasNext() const override {
                            return cursor < m.table.length();
                        }

                        Entry const&next() override {
                            gint i = cursor++;
                            if (i >= m.table.length())
                                NoSuchElementException().throws($ftrace());
                            return m.table[i];
                        }

                        void remove() override {
                            UnsupportedOperationException().throws($ftrace());
                        }

                        void forEach(const function::Consumer<Entry>& action) override {
                            for (gint i = cursor; cursor < m.table.length(); cursor++)
                                action.accept(m.table[i++]);
                        }
                    };

                    return UNSAFE::newInstance<Itr>(m);
                }

                void forEach(const function::Consumer<Entry&>& action) override {
                    for (Entry& e : m.table)
                        action.accept(e);
                }

                void forEach(const function::Consumer<Entry>& action) const override {
                    for (Entry& e : m.table)
                        action.accept(e);
                }

                ~EntrySet() override { detachEntries(m); }

                gbool addAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool removeIf(const function::Predicate<Entry>& filter) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool retainAll(const Collection<Entry>& c) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gint size() const override {
                    return m.table.length();
                }

                gbool isEmpty() const override {
                    return m.table.isEmpty();
                }

                gbool contains(const Entry& val) const override {
                    for (Entry const& e : m.table)
                        if (val == e)
                            return true;
                    return false;
                }

                Array<Entry> toArray() const override { return m.table; }

                gbool add(const Entry& val) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                gbool remove(const Entry& val) override {
                    UnsupportedOperationException().throws($ftrace());
                }

                void clear() override {
                    UnsupportedOperationException().throws($ftrace());
                }
            };
        };
    } // util
} // core

#endif //CORE24_MAP_H
