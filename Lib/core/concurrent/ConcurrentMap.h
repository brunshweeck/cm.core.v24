//
// Created by brunshweeck-tazeussong on 30/01/25.
//

#ifndef CORE24_CONCURRENTMAP_H
#define CORE24_CONCURRENTMAP_H

#include <core/util/Map.h>

namespace core {
    namespace concurrent {
        /**
         * A @b Map providing thread safety and atomicity guarantees.
         *
         * <p>
         * To maintain the specified guarantees, default implementations of
         * methods including @b putIfAbsent inherited from @b Map
         * must be overridden by implementations of this interface. Similarly,
         * implementations of the collections returned by methods
         * @b keySet, @b values, and @b entrySet must override
         * methods such as @c removeIf when necessary to
         * preserve atomicity guarantees.
         * </p>
         * <p>
         * Memory consistency effects: As with other concurrent
         * collections, actions in a thread prior to placing an object into a
         * @c ConcurrentMap as a key or value <i>happen-before</i>
         * actions after the access or removal of that object from
         * the @c ConcurrentMap in another thread.
         * </p>
         *
         * @tparam K the type of keys maintained by this map
         * @tparam V the type of mapped values
         */
        template <class K, class V>
        class ConcurrentMap : public virtual util::Map<K, V> {
        public:
            CORE_ALIAS(UNSAFE, Object::UNSAFE);
            CORE_ALIAS(KEY, typename Class<K>::Pointer);
            CORE_ALIAS(VALUE, typename Class<V>::Pointer);
            CORE_ALIAS(ANY, Class<>::Pointer);

            /**
             *
             * @note This implementation assumes that the ConcurrentMap cannot
             * contain null values and @c getOrNull() returning null unambiguously means
             * the key is absent. Implementations which support null values
             * <b>must</b> override this default implementation.
             *
             * @throws ClassCastException
             */
            V& getOrDefault(K const& key, V const& defaultValue) override {
                try {
                    ANY v = null;
                    return (v = toAny(this->getrOrNull(key)) != null) ? toValue(v) : UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @note This implementation assumes that the ConcurrentMap cannot
             * contain null values and @c getOrNull() returning null unambiguously means
             * the key is absent. Implementations which support null values
             * <b>must</b> override this default implementation.
             *
             * @throws ClassCastException
             */
            V const& getOrDefault(K const& key, V const& defaultValue) const override {
                try {
                    ANY v = null;
                    return (v = toAny(this->getrOrNull(key)) != null) ? toValue(v) : UNSAFE::copyInstance(defaultValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @details The default implementation is equivalent to, for this
             * @c map:
             * @code
             * for (Entry<K, V> &entry : map.entrySet()) {
             *   action.accept(entry.getKey(), entry.getValue());
             * }
             * @endcode
             *
             * @note The default implementation assumes that
             * @c IllegalStateException thrown by @c getKey() or
             * @c getValue() indicates that the entry has been removed and cannot
             * be processed. Operation continues for subsequent entries.
             */
            void forEach(function::BiConsumer<K&, V&> const& action) override {
                for (util::Entry<K, V>& entry : this->entrySet()) {
                    ANY key = null;
                    ANY value = null;
                    try {
                        key = toAny(entry.getKey());
                        value = toAny(entry.getValue());
                    } catch (IllegalStateException const& ise) {
                        // this usually means the entry is no longer in the map.
                        continue;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    action.accept(toKey(key), toValue(value));
                }
            }

            /**
             *
             *
             * @details The default implementation is equivalent to, for this
             * @c map:
             * @code
             * for (Entry<K, V> entry : map.entrySet()) {
             *   action.accept(entry.getKey(), entry.getValue());
             * }
             * @endcode
             *
             * @note The default implementation assumes that
             * @c IllegalStateException thrown by @c getKey() or
             * @c getValue() indicates that the entry has been removed and cannot
             * be processed. Operation continues for subsequent entries.
             */
            void forEach(function::BiConsumer<K, V> const& action) const override {
                for (util::Entry<K, V>& entry : this->entrySet()) {
                    ANY key = null;
                    ANY value = null;
                    try {
                        key = toAny(entry.getKey());
                        value = toAny(entry.getValue());
                    } catch (IllegalStateException const& ise) {
                        // this usually means the entry is no longer in the map.
                        continue;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    action.accept(toKey(key), toValue(value));
                }
            }

            /**
             * If the specified key is not already associated
             * with a value, associates it with the given value.
             * This is equivalent to, for this @c map:
             * @code
             * if (!map.containsKey(key))
             *   return map.put(key, value);
             * else
             *   return map.get(key);
             * @endcode
             *
             * except that the action is performed atomically.
             *
             * @implNote This implementation intentionally re-abstracts the
             * inappropriate default provided in @c Map.
             *
             * @param key key with which the specified value is to be associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with the specified key, or
             *         @c null if there was no mapping for the key.
             *         (A @c null return can also indicate that the map
             *         previously associated @c null with the key,
             *         if the implementation supports null values.)
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by this map
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map
             * @throws IllegalArgumentException if the specified key or value is null,
             *         and this map does not permit null keys or values
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             */
            Object& putIfAbsent(K const& key, V const& value) override = 0;

            /**
             * Removes the entry for a key only if currently mapped to a given value.
             * This is equivalent to, for this @c map:
             * @code
             * if (map.containsKey(key) && Objects::equals(map.get(key), value)) {
             *   map.remove(key);
             *   return true;
             * } else {
             *   return false;
             * }
             * @endcode
             *
             * except that the action is performed atomically.
             *
             * @note This implementation intentionally re-abstracts the
             * inappropriate default provided in @c Map.
             *
             * @param key key with which the specified value is associated
             * @param value value expected to be associated with the specified key
             * @return @c true if the value was removed
             * @throws UnsupportedOperationException if the @c remove operation
             *         is not supported by this map
             * @throws ClassCastException if the key or value is of an inappropriate
             *         type for this map (optional)
             * @throws IllegalArgumentException if the specified key or value is null,
             *         and this map does not permit null keys or values (optional)
             */
            gbool remove(K const& key, V const& value) override = 0;

            /**
             * Replaces the entry for a key only if currently mapped to a given value.
             * This is equivalent to, for this @c map:
             * @code
             * if (map.containsKey(key) && Objects::equals(map.get(key), oldValue)) {
             *   map.put(key, newValue);
             *   return true;
             * } else {
             *   return false;
             * }
             * @endcode
             *
             * except that the action is performed atomically.
             *
             * @note This implementation intentionally re-abstracts the
             * inappropriate default provided in @c Map.
             *
             * @param key key with which the specified value is associated
             * @param oldValue value expected to be associated with the specified key
             * @param newValue value to be associated with the specified key
             * @return @c true if the value was replaced
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by this map
             * @throws ClassCastException if the class of a specified key or value
             *         prevents it from being stored in this map
             * @throws IllegalArgumentException if a specified key or value is null,
             *         and this map does not permit null keys or values
             * @throws IllegalArgumentException if some property of a specified key
             *         or value prevents it from being stored in this map
             */
            gbool replace(K const& key, V const& oldValue, V const& newValue) override = 0;

            /**
             * Replaces the entry for a key only if currently mapped to some value.
             * This is equivalent to, for this @c map:
             * @code
             * if (map.containsKey(key))
             *   return map.put(key, value);
             * else
             *   return null;
             * @endcode
             *
             * except that the action is performed atomically.
             *
             * @implNote This implementation intentionally re-abstracts the
             * inappropriate default provided in @c Map.
             *
             * @param key key with which the specified value is associated
             * @param value value to be associated with the specified key
             * @return the previous value associated with the specified key, or
             *         @c null if there was no mapping for the key.
             *         (A @c null return can also indicate that the map
             *         previously associated @c null with the key,
             *         if the implementation supports null values.)
             * @throws UnsupportedOperationException if the @c put operation
             *         is not supported by this map
             * @throws ClassCastException if the class of the specified key or value
             *         prevents it from being stored in this map
             * @throws IllegalArgumentException if the specified key or value is null,
             *         and this map does not permit null keys or values
             * @throws IllegalArgumentException if some property of the specified key
             *         or value prevents it from being stored in this map
             */
            Object& replace(K const& key, V const& value) override = 0;

            /**
             *
             *
             * @details
             * <p>
             * The default implementation is equivalent to, for this @c map:
             * @code
             * for (Entry<K,V> &entry : map.entrySet()) {
             *   K k;
             *   V v;
             *   do {
             *     k = entry.getKey();
             *     v = entry.getValue();
             *   } while (!map.replace(k, v, function.apply(k, v)));
             * }
             * @endcode
             *
             * The default implementation may retry these steps when multiple
             * threads attempt updates including potentially calling the function
             * repeatedly for a given key.
             * </p>
             * <p>
             * This implementation assumes that the ConcurrentMap cannot contain null
             * values and @c get() returning null unambiguously means the key is
             * absent. Implementations which support null values <strong>must</strong>
             * override this default implementation.
             * </p>
             * @throws UnsupportedOperationException
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            void replaceAll(function::BiFunction<K&, V&, V> const& function) override {
                try {
                    forEach([&](K& key, V& value) {
                        ANY v = toAny(value);
                        while (!replace(key, toValue(v), function.apply(key, value))) {
                            // v changed or k is gone
                            if ((v = toAny(this->getOrNull(key))) == null)
                                break;
                        }
                    });
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @details
             * The default implementation is equivalent to the following steps for this
             * @c map:
             * @code
             * V oldValue, newValue;
             * return ((oldValue = map.get(key)) == null
             *         && (newValue = mappingFunction.apply(key)) != null
             *         && (oldValue = map.putIfAbsent(key, newValue)) == null)
             *   ? newValue
             *   : oldValue;
             * @endcode
             *
             * <p>
             * This implementation assumes that the ConcurrentMap cannot contain null
             * values and @c get() returning null unambiguously means the key is
             * absent. Implementations which support null values <strong>must</strong>
             * override this default implementation.
             * </p>
             * @throws UnsupportedOperationException
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            Object& computeIfAbsent(K const& key, function::Function<K, V&> const& mappingFunction) override {
                ANY oldValue = null, newValue = null;
                try {
                    return ((oldValue = toAny(this->getOrNull(key)) == null)
                               && (newValue = toAny(UNSAFE::copyInstance(mappingFunction.apply(key)))) != null
                               && (oldValue = toAny(putIfAbsent(key, toValue(newValue)))) == null)
                               ? toValue(newValue)
                               : toValue(oldValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @details
             * The default implementation is equivalent to performing the following
             * steps for this @c map:
             *
             * @code
             * for (V oldValue; (oldValue = map.get(key)) != null; ) {
             *   V newValue = remappingFunction.apply(key, oldValue);
             *   if ((newValue == null)
             *       ? map.remove(key, oldValue)
             *       : map.replace(key, oldValue, newValue))
             *     return newValue;
             * }
             * return null;
             * @endcode
             * When multiple threads attempt updates, map operations and the
             * remapping function may be called multiple times.
             *
             * <p>
             * This implementation assumes that the ConcurrentMap cannot contain null
             * values and @c get() returning null unambiguously means the key is
             * absent. Implementations which support null values <strong>must</strong>
             * override this default implementation.
             * </p>
             * @throws UnsupportedOperationException
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            Object& computeIfPresent(K const& key, function::BiFunction<K, V&, V> const& remappingFunction) override {
                for (ANY oldValue = null, newValue = null; (oldValue = toAny(this->getOrNull(key))) != null;) {
                    try {
                        newValue = toAny(UNSAFE::copyInstance(remappingFunction.apply(key, toValue(oldValue))));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    if ((newValue == null)
                            ? remove(key, toValue(oldValue))
                            : replace(key, toValue(oldValue), toValue(newValue)))
                        return toValue(newValue);
                }
                return null;
            }

            /**
             *
             *
             * @details
             * The default implementation is equivalent to performing the following
             * steps for this @c map:
             * @code
             * for (;;) {
             *   V oldValue = map.get(key);
             *   V newValue = remappingFunction.apply(key, oldValue);
             *   if (newValue != null) {
             *     if ((oldValue != null)
             *       ? map.replace(key, oldValue, newValue)
             *       : map.putIfAbsent(key, newValue) == null)
             *       return newValue;
             *   } else if (oldValue == null || map.remove(key, oldValue)) {
             *     return null;
             *   }
             * }
             * @endcode
             * When multiple threads attempt updates, map operations and the
             * remapping function may be called multiple times.
             *
             * <p>
             * This implementation assumes that the ConcurrentMap cannot contain null
             * values and @c get() returning null unambiguously means the key is
             * absent. Implementations which support null values <strong>must</strong>
             * override this default implementation.
             * </p>
             * @throws UnsupportedOperationException
             * @throws ClassCastException
             * @throws IllegalArgumentException
            */
            Object& compute(K const& key, function::BiFunction<K, V&, V> const& remappingFunction) override {
            RESTART:
                for (;;) {
                    ANY old = toAny(getOrNull(key));
                    // if putIfAbsent fails, opportunistically use its return value
                HAVE_OLD:
                    for (;;) {
                        ANY newValue = (old == null)
                                           ? old
                                           : toAny(UNSAFE::copyInstance(remappingFunction.apply(key, toValue(old))));
                        if (newValue != null) {
                            if (old != null) {
                                if (replace(key, toValue(old), toValue(newValue)))
                                    return toValue(newValue);
                            }
                            if ((old = toAny(putIfAbsent(key, toValue(newValue)))) == null)
                                return toValue(newValue);
                            goto HAVE_OLD;
                        } else if (old == null || remove(key, toValue(old)))
                            return null;
                        goto RESTART;
                    }
                }
            }

            /**
             *
             *
             * @details
             * The default implementation is equivalent to performing the following
             * steps for this @c map:
             * @code
             * for (;;) {
             *   V oldValue = map.get(key);
             *   if (oldValue != null) {
             *     V newValue = remappingFunction.apply(oldValue, value);
             *     if (newValue != null) {
             *       if (map.replace(key, oldValue, newValue))
             *         return newValue;
             *     } else if (map.remove(key, oldValue)) {
             *       return null;
             *     }
             *   } else if (map.putIfAbsent(key, value) == null) {
             *     return value;
             *   }
             * }
             * @endcode
             * When multiple threads attempt updates, map operations and the
             * remapping function may be called multiple times.
             *
             * <p>
             * This implementation assumes that the ConcurrentMap cannot contain null
             * values and @c get() returning null unambiguously means the key is
             * absent. Implementations which support null values <strong>must</strong>
             * override this default implementation.
             * </p>
             * @throws UnsupportedOperationException
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            Object& merge(K const& key, V const& value,
                          function::BiFunction<V, V, V> const& remappingFunction) override {
            RESTART:
                for (;;) {
                    ANY old = toAny(getOrNull(key));
                    // if putIfAbsent fails, opportunistically use its return value
                HAVE_OLD:
                    for (;;) {
                        if (old != null) {
                            ANY new_ = (old == null)
                                           ? old
                                           : toAny(UNSAFE::copyInstance(remappingFunction.apply(key, toValue(old))));
                            if (new_ != null) {
                                if (old != null) {
                                    if (replace(key, toValue(old), toValue(new_)))
                                        return toValue(new_);
                                }
                            } else if (remove(key, toValue(old)))
                                return null;
                            goto RESTART;
                        }
                        V& val = UNSAFE::copyInstance(value);
                        if ((old = toAny(putIfAbsent(key, val))) == null)
                            return val;
                        if (toAny(val) != toAny(value)) UNSAFE::deleteRegInstance(val);
                        goto RESTART;
                    }
                }
            }

        private:
            static ANY toAny(Object const& obj) {
                if (obj == null)
                    return null;
                return (ANY) &obj;
            }

            static K& toKey(ANY const& any) {
                return *CORE_DCAST(KEY, any);
            }

            static V& toValue(ANY const& any) {
                return *CORE_DCAST(VALUE, any);
            }
        };
    } // concurrent
} // core

#endif //CORE24_CONCURRENTMAP_H
