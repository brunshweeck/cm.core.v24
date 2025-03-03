//
// Created by brunshweeck on 23 juil. 2024.
//

#ifndef CORE24_ENTRY_H
#define CORE24_ENTRY_H

#include <core/lang/UnsupportedOperationException.h>
#include <core/util/Comparator.h>

namespace core {
    namespace util {
        /**
         * A map entry (key-value pair). The Entry may be unmodifiable, or the
         * value may be modifiable if the optional @c setValue method is
         * implemented. The Entry may be independent of any map, or it may represent
         * an entry of the entry-set view of a map.
         * <p>
         * An Entry maintains a connection to its underlying map if the Entry was obtained by
         * iterating the <i>view of a map</i>, either explicitly by using an
         * @c Iterator or implicitly via the enhanced @c for statement. This connection
         * to the backing map is valid <i>only</i> during iteration of the entry-set view. During
         * the iteration, if supported by the backing map, a change to an Entry's value via
         * the @c setValue method will be visible in the backing map.
         * The behavior of such an Entry is undefined outside of iteration of the map's entry-set
         * view. It is also undefined if the backing map has been modified after the Entry was
         * returned by the iterator, except through the @c setValue method. In addition,
         * a change to the value of a mapping in the backing map might or might not be
         * visible in the corresponding Entry of the entry-set view.
         * </p>
         * <p>
         * An Entry may also be obtained from a map's entry-set view by other means, for
         * example, using the @c toArray overloads,
         * or by copying the entry-set view into another collection. It is unspecified whether
         * the obtained Entry instances are connected to the underlying map, whether changes
         * to such an Entry will affect the underlying the map and vice versa, and whether
         * such an Entry supports the optional @c setValue method.
         * </p>
         * <p>
         * In addition, an Entry may be obtained directly from a map, for example via calls
         * to methods directly on the @c NavigableMap interface. An entry thus obtained
         * is generally not connected to the map and is an unmodifiable snapshot of the mapping
         * as of the time of the call. Such an Entry also does not generally support the
         * @c setValue method.
         * </p>
         *
         * @note
         * The exact behavior of Entry instances obtained from a map's entry-set view other than
         * via iteration varies across different map implementations; some are connected to the
         * backing map, and some are not. To guarantee that an Entry is disconnected from its
         * backing map, use the @c Entry::copyOf method. For example, the following
         * creates a snapshot of a map's entries that is guaranteed not to change even if the
         * original map is modified:
         * @code
         * auto &entries = map.entrySet()
         *                 .stream()
         *                 .map(Entry<>::copyOf)
         *                 .toList()
         * @endcode
         *
         * @tparam K the type of the key
         * @tparam V the type of the value
         *
         * @see Map::entrySet()
         */
        template<class K, class V>
        class Entry : public virtual Object {
        public:
            /**
             * Returns the key corresponding to this entry.
             *
             * @return the key corresponding to this entry
             * @throws IllegalStateException implementations may, but are not
             *         required to, throw this exception if the entry has been
             *         removed from the backing map.
             */
            virtual K &getKey() = 0;

            /**
             * Returns the key corresponding to this entry.
             *
             * @return the key corresponding to this entry
             * @throws IllegalStateException implementations may, but are not
             *         required to, throw this exception if the entry has been
             *         removed from the backing map.
             */
            virtual K const &getKey() const = 0;

            /**
             * Returns the value corresponding to this entry.  If the mapping
             * has been removed from the backing map (by the iterator's
             * @c remove operation), the results of this call are undefined.
             *
             * @return the value corresponding to this entry
             * @throws IllegalStateException implementations may, but are not
             *         required to, throw this exception if the entry has been
             *         removed from the backing map.
             */
            virtual V &getValue() = 0;

            /**
             * Returns the value corresponding to this entry.  If the mapping
             * has been removed from the backing map (by the iterator's
             * @c remove operation), the results of this call are undefined.
             *
             * @return the value corresponding to this entry
             * @throws IllegalStateException implementations may, but are not
             *         required to, throw this exception if the entry has been
             *         removed from the backing map.
             */
            virtual V const &getValue() const = 0;

            /**
             * Replaces the value corresponding to this entry with the specified
             * value (optional operation).  (Writes through to the map.)  The
             * behavior of this call is undefined if the mapping has already been
             * removed from the map (by the iterator's @c remove operation).
             *
             * @param value new value to be stored in this entry
             * @return old value corresponding to the entry
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by the backing map
             * @throws IllegalArgumentException if some property of this value
             *         prevents it from being stored in the backing map
             * @throws IllegalStateException implementations may, but are not
             *         required to, throw this exception if the entry has been
             *         removed from the backing map.
             */
            virtual V &setValue(V const &value) = 0;

            /**
             * Compares the specified object with this entry for equality.
             * Returns @c true if the given object is also a map entry and
             * the two entries represent the same mapping.  More formally, two
             * entries @c e1 and @c e2 represent the same mapping
             * if<pre>
             *     e1.getKey().equals(e2.getKey())  &&
             *     e1.getValue().equals(e2.getValue())
             * </pre>
             * This ensures that the @c equals method works properly across
             * different implementations of the @c util::Entry interface.
             *
             * @param other object to be compared for equality with this map entry
             * @return @c true if the specified object is equal to this map
             *         entry
             */
            gbool equals(Object const &other) const override {
                try {
                    if (this == &other)
                        return true;

                    if (!Class<Entry>::hasInstance(other))
                        return false;

                    Entry const &entry = CORE_XCAST(Entry const, other);
                    return getKey() == entry.getKey() && getValue() == entry.getValue();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the hash code value for this map entry.  The hash code
             * of a map entry @c e is defined to be: <pre>
             *     e.getKey().hash() ^
             *       e.getValue().hash()
             * </pre>
             * This ensures that @c e1.equals(e2) implies that
             * @code e1.hash() == e2.hash() @endcode for any two Entries
             * @c e1 and @c e2, as required by the general
             * contract of @c Object::hash.
             *
             * @return the hash code value for this map entry
             * @see Object::hash()
             * @see Object::equals(Object)
             * @see equals(Object)
             */
            gint hash() const override {
                try {
                    return getKey().hash() ^ getValue().hash();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a comparator that compares @c util::Entry in natural order on key.
             *
             * <p>
             * The returned comparator is serializable and throws @c IllegalArgumentException
             * when comparing an entry with a null key.
             * </p>
             *
             * @tparam K the @c Comparable type of then map keys
             * @tparam V the type of the map values
             * @return a comparator that compares @c util::Entry in natural order on key.
             * @see Comparable
             */
            static Comparator<Entry> &comparingByKey() {
                class Comparator final : public util::Comparator<Entry> {
                public:
                    gint compare(const Entry &t1, const Entry &t2) const CORE_NOTHROW override {
                        Comparable<K> const &t = CORE_XCAST(Comparable<K> const, t1.getKey());
                        return t.compareTo(t2.getKey());
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<Comparator>::hasInstance(o);
                    }
                };

                try {
                    if (Class<Comparable<K> >::template isSuper<K>())
                        return UNSAFE::newInstance<Comparator>();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                UnsupportedOperationException("natural ordering."_S).throws($ftrace());
            }

            /**
             * Returns a comparator that compares @c util::Entry in natural order on value.
             *
             * <p>
             * The returned comparator is serializable and throws @c IllegalArgumentException
             * when comparing an entry with null values.
             * </p>
             *
             * @tparam K the type of the map keys
             * @tparam V the @c Comparable type of the map values
             * @return a comparator that compares @c util::Entry in natural order on value.
             * @see Comparable
             */
            static Comparator<Entry> &comparingByValue() {
                class Comparator final : public util::Comparator<Entry> {
                public:
                    gint compare(const Entry &t1, const Entry &t2) const CORE_NOTHROW override {
                        Comparable<V> const &t = CORE_XCAST(Comparable<V> const, t1.getValue());
                        return t.compareTo(t2.getValue());
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<Comparator>::hasInstance(o);
                    }
                };

                try {
                    if (Class<Comparable<V> >::template isSuper<K>())
                        return UNSAFE::newInstance<Comparator>();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                UnsupportedOperationException("natural ordering."_S).throws($ftrace());
            }

            /**
             * Returns a comparator that compares @c util::Entry by key using the given
             * @c Comparator.
             *
             * <p>
             * The returned comparator is serializable if the specified comparator
             * is also serializable.
             * </p>
             *
             * @param  comparator the key @c Comparator
             * @return a comparator that compares @c util::Entry by the key.
             */
            template<class T = K, ClassOf(1)::OnlyIf<Class<T>::template isSuper<K>()> CaptureKey = 1>
            static Comparator<Entry> &comparingByKey(Comparator<T> const &comparator) {
                class Comparator final : public util::Comparator<Entry> {
                    util::Comparator<T> const &comparator;

                public:
                    CORE_EXPLICIT Comparator(util::Comparator<T> const &comparator) : comparator(comparator) {
                    }

                    gint compare(const Entry &t1, const Entry &t2) const CORE_NOTHROW override {
                        return comparator.compare(t1.getKey(), t2.getKey());
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<Comparator>::hasInstance(o);
                    }
                };

                try {
                    return UNSAFE::newInstance<Comparator>(comparator);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a comparator that compares @c util::Entry by value using the given
             * @c Comparator.
             *
             * <p>
             * The returned comparator is serializable if the specified comparator
             * is also serializable.
             * </p>
             *
             * @param  comparator the value @c Comparator
             * @return a comparator that compares @c util::Entry by the value.
             */
            template<class T = V, ClassOf(1)::OnlyIf<Class<T>::template isSuper<V>()> CaptureValue = 1>
            static Comparator<Entry> &comparingByValue(Comparator<T> const &comparator) {
                class Comparator final : public util::Comparator<Entry> {
                    util::Comparator<T> const &comparator;

                public:
                    CORE_EXPLICIT Comparator(util::Comparator<T> const &comparator) : comparator(comparator) {
                    }

                    gint compare(const Entry &t1, const Entry &t2) const CORE_NOTHROW override {
                        return comparator.compare(t1.getValue(), t2.getValue());
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<Comparator>::hasInstance(o);
                    }
                };

                try {
                    return UNSAFE::newInstance<Comparator>(comparator);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Return String representation of this entry in pair format:
             * @code (key, value) @endcode
             *
             * @return The string representation of this entry
             */
            String toString() const override {
                try {
                    XString string(50);
                    string.append('(');
                    Object const &key = getKey();
                    if (this == &key)
                        string.append('{').append('.').append('.').append('}');
                    else
                        string.append(key);
                    string.append(',').append(' ');
                    Object const &value = getValue();
                    if (this == &value)
                        string.append('{').append('.').append('.').append('}');
                    else
                        string.append(value);
                    string.append(')');
                    return string.toString();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };
    } // util
} // core

#endif //CORE24_ENTRY_H
