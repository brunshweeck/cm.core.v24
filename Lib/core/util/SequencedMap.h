//
// Created by bruns on 24/10/2024.
//

#ifndef CORE24_SEQUENCEDMAP_H
#define CORE24_SEQUENCEDMAP_H

#include <core/util/Map.h>
#include <core/util/SequencedCollection.h>
#include <core/util/SequencedSet.h>

namespace core {
    namespace util {
        /**
         * A Map that has a well-defined encounter order, that supports operations at both ends, and
         * that is reversible. The <a href="SequencedCollection.html#encounter">encounter order</a>
         * of a @c SequencedMap is similar to that of the elements of a @b SequencedCollection,
         * but the ordering applies to mappings instead of individual elements.
         * <p>
         * The bulk operations on this map, including the @b forEach and the
         * @b replaceAll methods, operate on this map's mappings in
         * encounter order.
         * </p>
         * <p>
         * The view collections provided by the @b keySet, @b values, @b entrySet,
         * @b sequencedKeySet, @b sequencedValues, and
         * @b sequencedEntrySet methods all reflect the encounter order
         * of this map. Even though the return values of the @c keySet, @c values, and
         * @c entrySet methods are not sequenced <i>types</i>, the elements
         * in those view collections do reflect the encounter order of this map. Thus, the
         * iterators returned by the statements
         * @code
         *     auto& it1 = sequencedMap.entrySet().iterator();
         *     auto& it2 = sequencedMap.sequencedEntrySet().iterator();
         * @endcode
         * both provide the mappings of @c sequencedMap in that map's encounter order.
         * </p>
         * <p>
         * This interface provides methods to add mappings, to retrieve mappings, and to remove
         * mappings at either end of the map's encounter order.
         * </p>
         * <p>
         * This interface also defines the @b reversed method, which provides a
         * reverse-ordered @a view of this map.
         * In the reverse-ordered view, the concepts of first and last are inverted, as
         * are the concepts of successor and predecessor. The first mapping of this map
         * is the last mapping of the reverse-ordered view, and vice-versa. The successor of some
         * mapping in this map is its predecessor in the reversed view, and vice-versa. All
         * methods that respect the encounter order of the map operate as if the encounter order
         * is inverted. For instance, the @b forEach method of the reversed view reports
         * the mappings in order from the last mapping of this map to the first. In addition, all of
         * the view collections of the reversed view also reflect the inverse of this map's
         * encounter order. For example,
         * @code
         *     auto& itr = sequencedMap.reversed().entrySet().iterator();
         * @endcode
         * provides the mappings of this map in the inverse of the encounter order, that is, from
         * the last mapping to the first mapping. The availability of the @c reversed method,
         * and its impact on the ordering semantics of all applicable methods and views, allow convenient
         * iteration, searching, copying, and streaming of this map's mappings in either forward order or
         * reverse order.
         * </p>
         * <p>
         * A map's reverse-ordered view is generally not serializable, even if the original
         * map is serializable.
         * </p>
         * <p>
         * The @b Entry instances obtained by iterating the @b entrySet view, the
         * @b sequencedEntrySet view, and its reverse-ordered view, maintain a connection to the
         * underlying map. This connection is guaranteed only during the iteration. It is unspecified
         * whether the connection is maintained outside of the iteration. If the underlying map permits
         * it, calling an Entry's @b setValue method will modify the value of the
         * underlying mapping. It is, however, unspecified whether modifications to the value in the
         * underlying mapping are visible in the @c Entry instance.
         * </p>
         * <p>
         * The methods
         * @b firstEntry,
         * @b lastEntry,
         * @b pollFirstEntry, and
         * @b pollLastEntry
         * return @b Entry instances that represent snapshots of mappings as
         * of the time of the call. They do <em>not</em> support mutation of the
         * underlying map via the optional @b setValue method.
         * </p>
         * <p>
         * Depending upon the implementation, the @c Entry instances returned by other
         * means might or might not be connected to the underlying map. For example, consider
         * an @c Entry obtained in the following manner:
         * @code
         *     var entry = sequencedMap.sequencedEntrySet().getFirst();
         * @endcode
         * It is not specified by this interface whether the @c setValue method of the
         * @c Entry thus obtained will update a mapping in the underlying map, or whether
         * it will throw an exception, or whether changes to the underlying map are visible in
         * that @c Entry.
         * </p>
         * <p>
         * This interface has the same requirements on the @c equals and @c hash
         * methods as defined by @b Map::equals and @b Map::hash.
         * Thus, a @c Map and a @c SequencedMap will compare equals if and only
         * if they have equal mappings, irrespective of ordering.
         * </p>
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         */
        template <class K, class V>
        class SequencedMap : public virtual Map<K, V> {
        protected:
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(Map, util::Map<K, V>);
            CORE_ALIAS(Entry, util::Entry<K, V>);

            CORE_ALIAS(SequencedView, typename Class<SequencedMap>::Pointer);
            CORE_ALIAS(SequencedKeys, typename Class<SequencedSet<K>>::Pointer);
            CORE_ALIAS(SequencedVals, typename Class<SequencedCollection<V>>::Pointer);
            CORE_ALIAS(SequencedEntries, typename Class<SequencedSet<Entry>>::Pointer);

        public:
            /**
             * Returns a reverse-ordered @a view of this map.
             * The encounter order of mappings in the returned view is the inverse of the encounter
             * order of mappings in this map. The reverse ordering affects all order-sensitive operations,
             * including those on the view collections of the returned view. If the implementation permits
             * modifications to this view, the modifications "write through" to the underlying map.
             * Changes to the underlying map might or might not be visible in this reversed view,
             * depending upon the implementation.
             *
             * @return a reverse-ordered view of this map
             */
            virtual SequencedMap<K, V>& reversed() = 0;

            /**
             * Returns a reverse-ordered @a view of this map.
             * The encounter order of mappings in the returned view is the inverse of the encounter
             * order of mappings in this map. The reverse ordering affects all order-sensitive operations,
             * including those on the view collections of the returned view. If the implementation permits
             * modifications to this view, the modifications "write through" to the underlying map.
             * Changes to the underlying map might or might not be visible in this reversed view,
             * depending upon the implementation.
             *
             * @return a reverse-ordered view of this map
             */
            virtual SequencedMap<K, V> const& reversed() const = 0;

            /**
             * Returns the first key-value mapping in this map,
             * or thrown @c NoSuchElementException if the map is empty.
             *
             * @note
             * The implementation in this interface obtains the iterator of this map's entrySet.
             * If the iterator has an element, it returns an unmodifiable copy of that element.
             * Otherwise, it thrown @c NoSuchElementException.
             *
             * @return the first key-value mapping
             * @throws NoSuchElementException  if the map is empty.
             */
            virtual Entry& firstEntry() {
                try {
                    for (Entry& e : entrySet())
                        return e;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            /**
             * Returns the first key-value mapping in this map,
             * or thrown @c NoSuchElementException if the map is empty.
             *
             * @note
             * The implementation in this interface obtains the iterator of this map's entrySet.
             * If the iterator has an element, it returns an unmodifiable copy of that element.
             * Otherwise, it thrown @c NoSuchElementException.
             *
             * @return the first key-value mapping.
             * @throws NoSuchElementException  if the map is empty.
             */
            virtual Entry const& firstEntry() const {
                try {
                    for (Entry const& e : entrySet())
                        return e;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            /**
             * Returns the last key-value mapping in this map,
             * or thrown @c NoSuchElementException if the map is empty.
             *
             * @note
             * The implementation in this interface obtains the iterator of the entrySet of this map's
             * reversed view. If the iterator has an element, it returns an unmodifiable copy of
             * that element. Otherwise, it thrown @c NoSuchElementException.
             *
             * @return the last key-value mapping
             * @throws NoSuchElementException  if the map is empty.
             */
            virtual Entry& lastEntry() {
                try {
                    for (Entry& e : reversed().entrySet())
                        return e;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            /**
             * Returns the last key-value mapping in this map,
             * or thrown @c NoSuchElementException if the map is empty.
             *
             * @note
             * The implementation in this interface obtains the iterator of the entrySet of this map's
             * reversed view. If the iterator has an element, it returns an unmodifiable copy of
             * that element. Otherwise, it thrown @c NoSuchElementException.
             *
             * @return the last key-value mapping
             * @throws NoSuchElementException  if the map is empty.
             */
            virtual Entry const& lastEntry() const {
                try {
                    for (Entry const& e : reversed().entrySet())
                        return e;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            /**
             * Removes and returns the first key-value mapping in this map,
             * or thrown @c NoSuchElementException if the map is empty (optional operation).
             *
             * @note
             * The implementation in this interface obtains the iterator of this map's entrySet.
             * If the iterator has an element, it calls @c remove on the iterator and
             * then returns an unmodifiable copy of that element. Otherwise, it thrown @c NoSuchElementException.
             *
             * @return the removed first entry of this map
             * @throws UnsupportedOperationException if this collection implementation does not
             *         support this operation
             * @throws NoSuchElementException  if the map is empty.
             */
            virtual Object& pollFirstEntry() {
                try {
                    Iterator<Entry>& it = this->entrySet().iterator();
                    if (it.hasNext()) {
                        Entry& entry = it.next();
                        it.remove();
                        return entry;
                    }
                    UNSAFE::deleteRegInstance(it);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                return null;
            }

            /**
             * Removes and returns the last key-value mapping in this map,
             * or thrown @c NoSuchElementException if the map is empty (optional operation).
             *
             * @note
             * The implementation in this interface obtains the iterator of the entrySet of this map's
             * reversed view. If the iterator has an element, it calls @c remove on the iterator
             * and then returns an unmodifiable copy of that element. Otherwise, it thrown @c NoSuchElementException.
             *
             * @return the removed last entry of this map
             * @throws UnsupportedOperationException if this collection implementation does not
             *         support this operation
             * @throws NoSuchElementException  if the map is empty.
             */
            virtual Object& pollLastEntry() {
                try {
                    Iterator<Entry>& it = reversed().entrySet().iterator();
                    if (it.hasNext()) {
                        Entry& entry = it.next();
                        it.remove();
                        return entry;
                    }
                    UNSAFE::deleteRegInstance(it);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return null;
            }

            /**
             * Inserts the given mapping into the map if it is not already present, or replaces the
             * value of a mapping if it is already present (optional operation). After this operation
             * completes normally, the given mapping will be present in this map, and it will be the
             * first mapping in this map's encounter order.
             *
             * @note The implementation in this interface always throws
             * @c UnsupportedOperationException.
             *
             * @param k the key
             * @param v the value
             * @return the value previously associated with k, or null if none
             * @throws UnsupportedOperationException if this collection implementation does not
             *         support this operation
             * @throws NoSuchElementException  if the map is empty.
             */
            virtual Object& putFirst(K const& k, V const& v) {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Inserts the given mapping into the map if it is not already present, or replaces the
             * value of a mapping if it is already present (optional operation). After this operation
             * completes normally, the given mapping will be present in this map, and it will be the
             * last mapping in this map's encounter order.
             *
             * @note The implementation in this interface always throws
             * @c UnsupportedOperationException.
             *
             * @param k the key
             * @param v the value
             * @return the value previously associated with k, or null if none
             * @throws UnsupportedOperationException if this collection implementation does not
             *         support this operation
             */
            virtual Object& putLast(K const& k, V const& v) {
                UnsupportedOperationException().throws($ftrace());
            }

            Set<K>& keySet() override {
                try {
                    return sequencedKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Set<K>& keySet() const override {
                try {
                    return sequencedKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Collection<V>& values() override {
                try {
                    return sequencedValues();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Collection<V>& values() const override {
                try {
                    return sequencedValues();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Set<Entry>& entrySet() override {
                try {
                    return sequencedEntrySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const Set<Entry>& entrySet() const override {
                try {
                    return sequencedEntrySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a @c SequencedSet view of this map's @b keySet.
             *
             * @note
             * The implementation in this interface returns a @c SequencedSet instance
             * that behaves as follows. Its @b add and
             * @b addAll methods throw @b UnsupportedOperationException.
             * Its @b reversed method returns the @b sequencedKeySet view of the @b reversed view of
             * this map. Each of its other methods calls the corresponding method of the @b keySet view of this map.
             *
             * @return a @c SequencedSet view of this map's @c keySet
             */
            virtual SequencedSet<K>& sequencedKeySet() {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *CORE_DCAST(SequencedKeys, keys);
            }

            /**
             * Returns a @c SequencedSet view of this map's @b keySet.
             *
             * @note
             * The implementation in this interface returns a @c SequencedSet instance
             * that behaves as follows. Its @b add and
             * @b addAll methods throw @b UnsupportedOperationException.
             * Its @b reversed method returns the @b sequencedKeySet view of the @b reversed view of
             * this map. Each of its other methods calls the corresponding method of the @b keySet view of this map.
             *
             * @return a @c SequencedSet view of this map's @c keySet
             */
            virtual SequencedSet<K> const& sequencedKeySet() const {
                if (keys == null)
                    keys = &UNSAFE::newInstance<KeySet>(*this);
                return *CORE_DCAST(SequencedKeys, keys);
            }

            /**
             * Returns a @c SequencedCollection view of this map's @b values collection.
             *
             * @note
             * The implementation in this interface returns a @c SequencedCollection instance
             * that behaves as follows. Its @b add and @b addAll methods throw @b UnsupportedOperationException.
             * Its @b reversed method returns the @b sequencedValues view of the @b reversed view of
             * this map. Its @b equals and @b hash methods
             * are inherited from @b Object. Each of its other methods calls the corresponding
             * method of the @b values view of this map.
             *
             * @return a @c SequencedCollection view of this map's @c values collection
             */
            virtual SequencedCollection<V>& sequencedValues() {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *CORE_DCAST(SequencedVals, vals);
            }

            /**
             * Returns a @c SequencedCollection view of this map's @b values collection.
             *
             * @note
             * The implementation in this interface returns a @c SequencedCollection instance
             * that behaves as follows. Its @b add and @b addAll methods throw @b UnsupportedOperationException.
             * Its @b reversed method returns the @b sequencedValues view of the @b reversed view of
             * this map. Its @b equals and @b hash methods
             * are inherited from @b Object. Each of its other methods calls the corresponding
             * method of the @b values view of this map.
             *
             * @return a @c SequencedCollection view of this map's @c values collection
             */
            virtual SequencedCollection<V> const& sequencedValues() const {
                if (vals == null)
                    vals = &UNSAFE::newInstance<Values>(*this);
                return *CORE_DCAST(SequencedVals, vals);
            }

            /**
             * Returns a @c SequencedSet view of this map's @b entrySet.
             *
             * @note
             * The implementation in this interface returns a @c SequencedSet instance
             * that behaves as follows. Its @b add and
             * @b addAll methods throw @b UnsupportedOperationException.
             * Its @b reversed method returns the @b sequencedEntrySet view of the @b reversed view of
             * this map. Each of its other methods calls the corresponding method of the
             * @b entrySet view of this map.
             *
             * @return a @c SequencedSet view of this map's @c entrySet
             */
            virtual SequencedSet<Entry>& sequencedEntrySet() = 0;

            /**
             * Returns a @c SequencedSet view of this map's @b entrySet.
             *
             * @note
             * The implementation in this interface returns a @c SequencedSet instance
             * that behaves as follows. Its @b add and
             * @b addAll methods throw @b UnsupportedOperationException.
             * Its @b reversed method returns the @b sequencedEntrySet view of the @b reversed view of
             * this map. Each of its other methods calls the corresponding method of the
             * @b entrySet view of this map.
             *
             * @return a @c SequencedSet view of this map's @c entrySet
             */
            virtual SequencedSet<Entry> const& sequencedEntrySet() const = 0;

            /**
             * Destroy this sequenced map, and him dependant view such as
             * sequenced keys, values, and entries views.
             */
            ~SequencedMap() override {
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

        protected:
            // ::::::::::::::::::::: Sequenced views Fields :::::::::::::::::::::::::::::

            CORE_IMPORT_FIELD_OR_METHOD(Map, keys);
            CORE_IMPORT_FIELD_OR_METHOD(Map, vals);
            CORE_IMPORT_FIELD_OR_METHOD(Map, entries);
            SequencedView mutable reverse = null;

            // ::::::::::::::::::::: Sequenced views Destructor Utilities :::::::::::::::::::::::::::::

            static void detachKeys(SequencedMap const& map) { Map::detachKeys(map); }

            static void detachVals(SequencedMap const& map) { Map::detachVals(map); }

            static void detachEntries(SequencedMap const& map) { Map::detachEntries(map); }

            static void detachReverse(SequencedMap const& map) { map.reverse = null; }

        private:
            // ::::::::::::::::::::: Sequenced views Class :::::::::::::::::::::::::::::

            /**
             * The keys view
             */
            class KeySet;

            /**
             * The values view
             */
            class Values;
        };

        template <class K, class V>
        class SequencedMap<K, V>::KeySet final : public SequencedSet<K> {
            SequencedMap<K, V>& m;

        public:
            CORE_EXPLICIT KeySet(SequencedMap const& m) : m((SequencedMap&) m) {}

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

            gbool contains(const K& val) const override {
                try {
                    return m.containsKey(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<K>& iterator() override {
                class Itr final : public Iterator<K> {
                    Iterator<Entry>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry>& it) : it(it) {}

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    gbool hasNext() const override {
                        try {
                            return it.hasNext();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    K&next() override {
                        try {
                            return it.next().getKey();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        try {
                            it.remove();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                };

                return UNSAFE::newInstance<Itr>(m.entrySet().iterator());
            }

            Iterator2<K>& iterator() const override {
                class Itr final : public Iterator2<K> {
                    Iterator<Entry>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry>& it) : it(it) {}

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    gbool hasNext() const override {
                        try {
                            return it.hasNext();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    K const&next() override {
                        try {
                            return it.next().getKey();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                };

                return UNSAFE::newInstance<Itr>(m.entrySet().iterator());
            }

            gbool add(const K& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool addAll(const Collection<K>& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                try { m.clear(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedSet<K>& reversed() const override {
                try {
                    return m.reversed().sequencedKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedSet<K>& reversed() override {
                try {
                    return m.reversed().sequencedKeySet();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& firstElement() override {
                try {
                    return m.firstEntry().getKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& firstElement() const override {
                try {
                    return m.firstEntry().getKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            K& lastElement() override {
                try {
                    return m.lastEntry().getKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const K& lastElement() const override {
                try {
                    return m.lastEntry().getKey();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~KeySet() override { detachKeys(m); }
        };

        template <class K, class V>
        class SequencedMap<K, V>::Values final : public SequencedCollection<V> {
            SequencedMap<K, V>& m;

        public:
            CORE_EXPLICIT Values(SequencedMap const& m) : m((SequencedMap&) m) {}

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

            gbool contains(const V& val) const override {
                try {
                    return m.containsValue(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<V>& iterator() override {
                class Itr final : public Iterator<V> {
                    Iterator<Entry>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry>& it) : it(it) {}

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    gbool hasNext() const override {
                        try {
                            return it.hasNext();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    V&next() override {
                        try {
                            return it.next().getValue();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void remove() override {
                        try {
                            it.remove();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                };

                return UNSAFE::newInstance<Itr>(m.entrySet().iterator());
            }

            Iterator2<V>& iterator() const override {
                class Itr final : public Iterator2<V> {
                    Iterator<Entry>& it;

                public:
                    CORE_EXPLICIT Itr(Iterator<Entry>& it) : it(it) {}

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Itr>::hasInstance(o) && it == CORE_XCAST(Itr const, o).it;
                    }

                    gbool hasNext() const override {
                        try {
                            return it.hasNext();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    V const&next() override {
                        try {
                            return it.next().getValue();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                };

                return UNSAFE::newInstance<Itr>(m.entrySet().iterator());
            }

            gbool add(const V& val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool addAll(const Collection<V>& c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                try {
                    m.clear();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const SequencedCollection<V>& reversed() const override {
                try {
                    return m.reversed().sequencedValues();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            SequencedCollection<V>& reversed() override {
                try {
                    return m.reversed().sequencedValues();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& firstElement() override {
                try {
                    return m.firstEntry().getValue();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& firstElement() const override {
                try {
                    return m.firstEntry().getValue();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            V& lastElement() override {
                try {
                    return m.lastEntry().getValue();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            const V& lastElement() const override {
                try {
                    return m.lastEntry().getValue();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~Values() override { detachVals(m); }
        };
    } // util
} // core

#endif //CORE24_SEQUENCEDMAP_H
