//
// Created by brunshweeck on 5 juil. 2024.
//

#ifndef CORE24_SET_H
#define CORE24_SET_H

#include <core/BooleanArray.h>
#include <core/util/Arrays.h>
#include <core/util/Collection.h>

namespace core {
    namespace util {
        /**
         * A collection that contains no duplicate elements.  More formally, sets
         * contain no pair of elements @c e1 and @c e2 such that @c e1.equals(e2),
         * and at most one null element.  As implied by its name, this interface
         * models the mathematical <i>set</i> abstraction.
         * <p>
         * The @c Set interface places additional stipulations, beyond those
         * inherited from the @c Collection interface, on the contracts of all
         * constructors and on the contracts of the @c add, @c equals and
         * @c hash methods.  Declarations for other inherited methods are
         * also included here for convenience.  (The specifications accompanying these
         * declarations have been tailored to the @c Set interface, but they do
         * not contain any additional stipulations.)
         * </p>
         * <p>
         * The additional stipulation on constructors is, not surprisingly,
         * that all constructors must create a set that contains no duplicate elements
         * (as defined above).
         * </p>
         * <p>
         * Note: Great care must be exercised if mutable objects are used as set
         * elements.  The behavior of a set is not specified if the value of an object
         * is changed in a manner that affects @c equals comparisons while the
         * object is an element in the set.  A special case of this prohibition is
         * that it is not permissible for a set to contain itself as an element.
         * </p>
         * <p>
         * Some set implementations have restrictions on the elements that
         * they may contain.  For example, some implementations prohibit null elements,
         * and some have restrictions on the types of their elements.  Attempting
         * to query the presence of an ineligible element may throw an exception,
         * or it may simply return false; some implementations will exhibit the former
         * behavior and some will exhibit the latter.  More generally, attempting an
         * operation on an ineligible element whose completion would not result in
         * the insertion of an ineligible element into the set may throw an
         * exception, or it may succeed, at the option of the implementation.
         * Such exceptions are marked as "optional" in the specification for this
         * interface.
         * </p>
         * <h2>Unmodifiable Sets</h2>
         * <p>
         * The @c Set::of and @c Set::copyOf static factory methods
         * provide a convenient way to create unmodifiable sets. The @c Set
         * instances created by these methods have the following characteristics:
         *
         * - They are <i>unmodifiable</i>. Elements cannot be added or removed.
         * Calling any mutator method on the Set will always cause @c UnsupportedOperationException to be thrown.
         * However, if the contained elements are themselves mutable, this may cause the
         * Set to behave inconsistently or its contents to appear to change.
         * - They are serializable if all elements are serializable.
         * - They reject duplicate elements at creation time. Duplicate elements
         * passed to a static factory method result in @c IllegalArgumentException.
         * - The iteration order of set elements is unspecified and is subject to change.
         * - They are <i>value-based</i>.
         * Programmers should treat instances that are @c equal as interchangeable and should not use
         * them for synchronization, or unpredictable behavior may occur. Callers should make no assumptions
         * about the identity of the returned instances. Factories are free to create new instances or reuse existing ones.
         * - They are serialized as specified on the <i>Serialized Form</i> page.
         * </p>
         *
         * @tparam E the type of elements maintained by this set
         *
         * @see Collection
         * @see List
         * @see SortedSet
         * @see HashSet
         * @see TreeSet
         */
        template<class E>
        class Set : public virtual Collection<E> {
        protected:
            CORE_ALIAS(UNSAFE, misc::Unsafe);

        public:
            // Query Operations

            /**
             * Returns the number of elements in this set (its cardinality).  If this
             * set contains more than @c Integer::MAX_VALUE elements, returns
             * @c Integer::MAX_VALUE.
             *
             * @return the number of elements in this set (its cardinality)
             */
            gint size() const override = 0;

            /**
             * Returns @c true if this set contains no elements.
             *
             * @return @c true if this set contains no elements
             */
            gbool isEmpty() const override { return size() == 0; }

            /**
             * Returns @c true if this set contains the specified element.
             * More formally, returns @c true if and only if this set
             * contains an element @c e such that
             * @code Objects.equals(o, e) @endcode.
             *
             * @param val element whose presence in this set is to be tested
             * @return @c true if this set contains the specified element
             */
            gbool contains(E const &val) const override {
                try {
                    return Collection<E>::contains(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this set.  The elements are
             * returned in no particular order (unless this set is an instance of some
             * class that provides a guarantee).
             *
             * @return an iterator over the elements in this set
             */
            Iterator<E> &iterator() override = 0;

            /**
             * Returns an iterator over the elements in this set.  The elements are
             * returned in no particular order (unless this set is an instance of some
             * class that provides a guarantee).
             *
             * @return an iterator over the elements in this set
             */
            Iterator2<E> &iterator() const override = 0;

            /**
             * Returns an array containing all the elements in this set.
             * If this set makes any guarantees as to what order its elements
             * are returned by its iterator, this method must return the
             * elements in the same order.
             *
             * <p>The returned array will be "safe" in that no references to it
             * are maintained by this set.  (In other words, this method must
             * allocate a new array even if this set is backed by an array).
             * The caller is thus free to modify the returned array.
             *
             * <p>This method acts as bridge between array-based and collection-based
             * APIs.
             *
             * @return an array containing all the elements in this set
             */
            Array<E> toArray() const override {
                try {
                    return Collection<E>::toArray();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }


            // Modification Operations

            /**
             * Adds the specified element to this set if it is not already present
             * (optional operation).  More formally, adds the specified element
             * @c e to this set if the set contains no element @c e2
             * such that @c e.equals(e2).
             * If this set already contains the element, the call leaves the set
             * unchanged and returns @c false.  In combination with the
             * restriction on constructors, this ensures that sets never contain
             * duplicate elements.
             *
             * @param val element to be added to this set
             * @return @c true if this set did not already contain the specified
             *         element
             * @throws UnsupportedOperationException if the @c add operation
             *         is not supported by this set
             * @throws IllegalArgumentException if some property of the specified element
             *         prevents it from being added to this set
             */
            gbool add(E const &val) override {
                UnsupportedOperationException().throws($ftrace());
            }


            /**
             * Removes the specified element from this set if it is present
             * (optional operation).  More formally, removes an element @c e
             * such that
             * @code Objects.equals(o, e) @endcode, if
             * this set contains such an element.  Returns @c true if this set
             * contained the element (or equivalently, if this set changed as a
             * result of the call).  (This set will not contain the element once the
             * call returns.)
             *
             * @param val object to be removed from this set, if present
             * @return @c true if this set contained the specified element
             * @throws UnsupportedOperationException if the @c remove operation
             *         is not supported by this set
             */
            gbool remove(E const &val) override {
                UnsupportedOperationException().throws($ftrace());
            }


            // Bulk Operations

            /**
             * Returns @c true if this set contains all the elements of the
             * specified collection.  If the specified collection is also a set, this
             * method returns @c true if it is a <i>subset</i> of this set.
             *
             * @param  c collection to be checked for containment in this set
             * @return @c true if this set contains all the elements of the
             *         specified collection
             */
            gbool containsAll(Collection<E> const &c) const override {
                try {
                    return Collection<E>::containsAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Adds all the elements in the specified collection to this set if
             * they're not already present (optional operation).  If the specified
             * collection is also a set, the @c addAll operation effectively
             * modifies this set so that its value is the <i>union</i> of the two
             * sets.  The behavior of this operation is undefined if the specified
             * collection is modified while the operation is in progress.
             *
             * @param  c collection containing elements to be added to this set
             * @return @c true if this set changed as a result of the call
             *
             * @throws UnsupportedOperationException if the @c addAll operation
             *         is not supported by this set
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this set
             */
            gbool addAll(Collection<E> const &c) override {
                try {
                    return Collection<E>::addAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Retains only the elements in this set that are contained in the
             * specified collection (optional operation).  In other words, removes
             * from this set all of its elements that are not contained in the
             * specified collection.  If the specified collection is also a set, this
             * operation effectively modifies this set so that its value is the
             * <i>intersection</i> of the two sets.
             *
             * @param  c collection containing elements to be retained in this set
             * @return @c true if this set changed as a result of the call
             * @throws UnsupportedOperationException if the @c retainAll operation
             *         is not supported by this set
             */
            gbool retainAll(const Collection<E> &c) override {
                try {
                    return Collection<E>::retainAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes from this set all of its elements that are contained in the
             * specified collection (optional operation).  If the specified
             * collection is also a set, this operation effectively modifies this
             * set so that its value is the <i>asymmetric set difference</i> of
             * the two sets.
             *
             * @param  c collection containing elements to be removed from this set
             * @return @c true if this set changed as a result of the call
             * @throws UnsupportedOperationException if the @c removeAll operation
             *         is not supported by this set
             */
            gbool removeAll(const Collection<E> &c) override {
                try {
                    gbool modified = false;
                    if (size() > c.size()) {
                        for (E const &val: c) {
                            modified |= remove(val);
                        }
                    } else {
                        for (Iterator<E> &it = iterator(); it.hasNext();) {
                            if (c.contains(it.next())) {
                                it.remove();
                                modified = true;
                            }
                        }
                    }
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all the elements from this set (optional operation).
             * The set will be empty after this call returns.
             *
             * @throws UnsupportedOperationException if the @c clear method
             *         is not supported by this set
             */
            void clear() override {
                try {
                    return Collection<E>::clear();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }


            // Comparison and hashing

            /**
             * Compares the specified object with this set for equality.  Returns
             * @c true if the specified object is also a set, the two sets
             * have the same size, and every member of the specified set is
             * contained in this set (or equivalently, every member of this set is
             * contained in the specified set).  This definition ensures that the
             * equals method works properly across different implementations of the
             * set interface.
             *
             * @param o object to be compared for equality with this set
             * @return @c true if the specified object is equal to this set
             */
            gbool equals(const Object &o) const override {
                if (this == &o) {
                    return true;
                }
                if (Class<Set>::hasInstance(o)) {
                    Collection<E> const &c = CORE_XCAST(Set const, o);
                    if (size() != c.size())
                        return false;

                    try {
                        return containsAll(c);
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                }
                return false;
            }

            /**
             * Returns the hash code value for this set.  The hash code of a set is
             * defined to be the sum of the hash codes of the elements in the set,
             * where the hash code of a @c null element is defined to be zero.
             * This ensures that @c s1.equals(s2) implies that
             * @code s1.hash() == s2.hash() @endcode for any two sets @c s1
             * and @c s2, as required by the general contract of
             * @c Object::hash.
             *
             * @return the hash code value for this set
             */
            gint hash() const override {
                try {
                    Iterator2<E> &it = iterator();
                    gint h = 0;
                    while (it.hasNext()) {
                        Object const &o = it.next();
                        h += o.hash();
                    }
                    UNSAFE::deleteInstance(it);
                    return h;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // ::::::::::::::::::::::: Statics Set Methods :::::::::::::::::::::::::::::::::

            /**
             * Returns an unmodifiable set containing an arbitrary number of elements.
             * See <i>Unmodifiable Sets</i> for details.
             *
             * @param args The elements to be contained in the set
             * @return a @c Set containing the specified elements
             */
            template<class... T>
            static Set &of(T &&... args) {
                try {
                    gint i = sizeof...(T);
                    Array<E> a = Array<E>::of(UNSAFE::forwardInstance<T>(args)...);
                    BooleanArray s = BooleanArray::of(isAlreadyExist(i, args, a)...);
                    Array<E> elts = organize(a, s);
                    switch (elts.length()) {
                        case 0: return emptySet();
                        case 1: return UNSAFE::newInstance<Singleton>(elts[0]);
                        default: return UNSAFE::newInstance<SetN>(elts);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an unmodifiable set containing an arbitrary number of elements.
             * See <i>Unmodifiable Sets</i> for details.
             *
             * @param args The elements to be contained in the set
             * @return a @c Set containing the specified elements
             */
            template<class T = E, ClassOf(1)::OnlyIf<Class<E>::template isSuper<T>()> CaptureArray = 1>
            static Set &of(Array<T> args) {
                try {
                    Array<T> a = Arrays::copyOf<E>(args, args.length());
                    Array<E> elts = organize(a, BooleanArray());
                    switch (elts.length()) {
                        case 0: return emptySet();
                        case 1: return UNSAFE::newInstance<Singleton>(elts[0]);
                        default: return UNSAFE::newInstance<SetN>(elts);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an <i>unmodifiable Set</i> containing the elements
             * of the given Collection. The given Collection must not be null, and it must not
             * contain any null elements. If the given Collection contains duplicate elements,
             * an arbitrary element of the duplicates is preserved. If the given Collection is
             * subsequently modified, the returned Set will not reflect such modifications.
             *
             * @implNote
             * If the given Collection is an <i>unmodifiable Set</i>,
             * calling copyOf will generally not create a copy.
             *
             * @tparam T the @c Collection's element type
             * @param c a @c Collection from which elements are drawn, must be non-null
             * @return a @c Set containing the elements of the given @c Collection
             */
            template<class T = E, ClassOf(1)::OnlyIf<Class<E>::template isSuper<T>()> CaptureCollection = 1>
            static Set &copyOf(Collection<T> const &c) {
                try {
                    Array<T> ca = c.toArray();
                    Array<E> a = Arrays::copyOf<E>(ca, ca.length());
                    Array<E> elts = organize(a, BooleanArray());
                    switch (elts.length()) {
                        case 0: return emptySet();
                        case 1: return UNSAFE::newInstance<Singleton>(elts[0]);
                        default: return UNSAFE::newInstance<SetN>(elts);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an empty set (immutable).  This set is serializable.
             * Unlike the like-named field, this method is parameterized.
             *
             * <p>
             * This example illustrates the type-safe way to obtain an empty set:
             * @code
             *     auto& s = Set<String>::emptySet();
             * @endcode
             * </p>
             * @note Implementations of this method need not create a separate
             * @c Set object for each call.  Using this method is likely to have
             * comparable cost to using the like-named field.  (Unlike this method, the
             * field does not provide type safety.)
             *
             * @return the empty set
             */
            static Set &emptySet() {
                try { return UNSAFE::newInstance<EmptySet>(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            // ::::::::::::::::::::::: Set Utility Method :::::::::::::::::::::::::::::::::::::
            /**
             * Organize and Remove all duplication of element in given array.
             *
             * @param elts The array to be organized in new array
             * @param status The construction status of all elements in input array
             * @return the optimized array containing the unique key.
             */
            static Array<E> organize(Array<E> &elts, BooleanArray const &status) {
                class Dummy final : public Object {
                public:
                    Object &clone() const override { return CORE_CAST(Dummy &, *this); }
                };

                // Dummy value
                Dummy DUMMY;
                gint len = elts.length();
                // Result array
                Array<> a = Array<>(len, DUMMY);
                // number of element present
                gint present = 0;

                // 1.Convert array to hashed array
                try {
                    IntArray indexes = IntArray(len, -1);
                    gint i = 0;
                    for (E &elt: elts) {
                        gint index = Math::floorMod(elt.hash(), len);
                        // Find index of elt in result array
                        while (true) {
                            Object &old = a[index];
                            // Unused place
                            if (DUMMY == old) {
                                indexes[present] = i;
                                Arrays::fastSet(a, index, elt);
                                present++;
                                break;
                            }

                            // Key Already exist (Replace element)
                            if (old == elt) {
                                if (!status.isEmpty()) {
                                    // elt is new created instance
                                    if (!status[i]) {
                                        // Destroy elt and conserve old element.
                                        UNSAFE::deleteInstance(elt);
                                        break;
                                    }
                                    // elt is not newly created
                                    gint j = indexes[index];
                                    if (j != -1) {
                                        // old is newly created instance, we'll replace it
                                        if (!status[j]) {
                                            UNSAFE::deleteInstance(old);
                                            Arrays::fastSet(a, index, elt);
                                            indexes[index] = i;
                                            break;
                                        }
                                    }
                                }

                                // elt and old are not newly created instances (Replacing old by elt).
                                if (true) {
                                    Arrays::fastSet(a, index, elt);
                                    indexes[index] = i;
                                }
                            }

                            // Place Already used by another element (find new place)
                            if (++index == len)
                                index = 0;
                        }
                        i++;
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                // 2.Remove all Dummy place on result
                try {
                    if (present < len) {
                        gint offset = 0;
                        for (int i = 0; i < len; ++i) {
                            Object &elt = a[i];
                            if (elt == DUMMY) {
                                offset--;
                                continue;
                            }
                            Arrays::swap(a, i, i + offset);
                        }
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                // 3.Convert the result array to Array restricted by this set element type (Array<?>)
                try {
                    return Arrays::copyOf<E>(a, present);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Check and Return true if the given element already exist in specified array.
             */
            template<class T>
            static gbool isAlreadyExist(gint &i, T &&elt, Array<E> const &elts) {
                return CORE_CAST(glong, &elt) == CORE_CAST(glong, &elts[--i]);
            }

            // ::::::::::::::::::::::: Unmodifiable Set Class :::::::::::::::::::::::::::::::::

            /**
             * The unmodifiable set base class
             */
            class UnmodifiableSet;

            /**
             * The unmodifiable empty set
             */
            class EmptySet;

            /**
             * The unmodifiable set of single element.
             */
            class Singleton;

            /**
             * The unmodifiable set of arbitrary number of elements.
             */
            class SetN;
        };

        template<class E>
        class Set<E>::UnmodifiableSet : public Set {
        public:
            gbool add(const E &val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool remove(const E &val) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool addAll(const Collection<E> &c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool removeAll(const Collection<E> &c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool removeIf(const function::Predicate<E> &filter) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool retainAll(const Collection<E> &c) override {
                UnsupportedOperationException().throws($ftrace());
            }

            void clear() override {
                UnsupportedOperationException().throws($ftrace());
            }
        };

        template<class E>
        class Set<E>::EmptySet final : public UnmodifiableSet {
        public:
            gint size() const override { return 0; }

            Iterator<E> &iterator() override { return Collection<E>::emptyIterator(); }

            Iterator2<E> &iterator() const override { return Collection<E>::emptyIterator2(); }

            void forEach(const function::Consumer<E &> &) override {
            }

            void forEach(const function::Consumer<E> &) const override {
            }

            String toString() const override { return "[]"_S; }

            gbool isEmpty() const override { return true; }

            gbool contains(const E &) const override { return false; }

            Array<E> toArray() const override { return {}; }

            gbool containsAll(const Collection<E> &c) const override { return c.isEmpty(); }

            gbool equals(const Object &o) const override {
                return this == &o || Class<Set>::hasInstance(o) && CORE_XCAST(Set const, o).isEmpty();
            }

            gint hash() const override { return 0; }
        };

        template<class E>
        class Set<E>::Singleton final : public UnmodifiableSet {
            E &value;

        public:
            CORE_EXPLICIT Singleton(E &val): value(val) {
            }

            gint size() const override { return 1; }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    Singleton &set;
                    gint cursor = 0;

                public:
                    CORE_EXPLICIT Itr(Singleton &set) : set(set) {
                    }

                    gbool hasNext() const override { return cursor < 1; }

                    E &next() override {
                        if (cursor >= 1)
                            NoSuchElementException().throws($ftrace());

                        cursor++;
                        return set.value;
                    }

                    gbool equals(const Object &obj) const override {
                        if (this == &obj)
                            return true;

                        if (Class<Itr>::hasInstance(obj)) {
                            Itr const &it = CORE_XCAST(Itr const, obj);
                            return &set == &it.set && cursor == it.cursor;
                        }
                        return false;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                class Itr final : public Iterator2<E> {
                    Singleton const &set;
                    gint cursor = 0;

                public:
                    CORE_EXPLICIT Itr(Singleton const &set) : set(set) {
                    }

                    gbool hasNext() const override { return cursor < 1; }

                    E const &next() override {
                        if (cursor >= 1)
                            NoSuchElementException().throws($ftrace());

                        cursor++;
                        return set.value;
                    }

                    gbool equals(const Object &obj) const override {
                        if (this == &obj)
                            return true;

                        if (Class<Itr>::hasInstance(obj)) {
                            Itr const &it = CORE_XCAST(Itr const, obj);
                            return &set == &it.set && cursor == it.cursor;
                        }
                        return false;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E &> &action) override {
                try { action.accept(value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E> &action) const override {
                try { action.accept(value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            String toString() const override { return "["_S + String::valueOf(value) + "]"_S; }

            gbool isEmpty() const override { return false; }

            gbool contains(const E &val) const override { return val == value; }

            Array<E> toArray() const override {
                try { return Array<E>::of(value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gint hash() const override { return value.hash(); }
        };

        template<class E>
        class Set<E>::SetN final : public UnmodifiableSet {
            Array<E> elts;

        public:
            CORE_EXPLICIT SetN(Array<E> elts): elts(UNSAFE::moveInstance(elts)) {
            }

            gint size() const override { return elts.length(); }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    SetN &set;
                    gint cursor = 0;

                public:
                    CORE_EXPLICIT Itr(SetN &set) : set(set) {
                    }

                    gbool hasNext() const override { return cursor < set.size(); }

                    E &next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        return set.elts[cursor++];
                    }

                    gbool equals(const Object &obj) const override {
                        if (this == &obj)
                            return true;

                        if (Class<Itr>::hasInstance(obj)) {
                            Itr const &it = CORE_XCAST(Itr const, obj);
                            return &set == &it.set && cursor == it.cursor;
                        }

                        return false;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                class Itr final : public Iterator2<E> {
                    SetN const &set;
                    gint cursor = 0;

                public:
                    CORE_EXPLICIT Itr(SetN const &set) : set(set) {
                    }

                    gbool hasNext() const override { return cursor < set.size(); }

                    E const &next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        return set.elts[cursor++];
                    }

                    gbool equals(const Object &obj) const override {
                        if (this == &obj)
                            return true;

                        if (Class<Itr>::hasInstance(obj)) {
                            Itr const &it = CORE_XCAST(Itr const, obj);
                            return &set == &it.set && cursor == it.cursor;
                        }

                        return false;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E &> &action) override {
                try {
                    for (E &elt: elts) action.accept(elt);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E> &action) const override {
                try {
                    for (E const &elt: elts) action.accept(elt);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                return elts.toString();
            }

            gbool isEmpty() const override { return elts.isEmpty(); }

            gbool contains(const E &val) const override {
                for (E const &elt: elts)
                    if (val == elt)
                        return true;

                return false;
            }

            Array<E> toArray() const override { return elts; }

            gint hash() const override { return elts.hash(); }
        };
    } // util
} // core

#endif //CORE24_SET_H
