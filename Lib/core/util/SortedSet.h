//
// Created by brunshweeck on 5 août 2024.
//

#ifndef CORE24_SORTEDSET_H
#define CORE24_SORTEDSET_H

#include <core/util/SequencedSet.h>

namespace core {
    namespace util {
        /**
         * A @b Set that further provides a <i>total ordering</i> on its elements.
         * The elements are ordered using their <em> natural
         * ordering</em>, or by a @b Comparator typically provided at sorted
         * set creation time.  The set's iterator will traverse the set in
         * ascending element order. Several additional operations are provided
         * to take advantage of the ordering.  (This interface is the set
         * analogue of @b SortedMap.)
         *
         * <p>
         * All elements inserted into a sorted set must implement the @c Comparable
         * interface (or be accepted by the specified comparator).  Furthermore, all
         * such elements must be <i>mutually comparable</i>: @c e1.compareTo(e2)
         * (or @c comparator.compare(e1, e2)) must not throw a
         * @c ClassCastException for any elements @c e1 and @c e2 in
         * the sorted set.  Attempts to violate this restriction will cause the
         * offending method or constructor invocation to throw a
         * @c ClassCastException.
         * </p>
         * <p>
         * Note that the ordering maintained by a sorted set (whether an
         * explicit comparator is provided) must be <i>consistent with equals</i> if
         * the sorted set is to correctly implement the @c Set interface.  (See
         * the @c Comparable interface or @c Comparator interface for a
         * precise definition of <i>consistent with equals</i>.)  This is so because
         * the @c Set interface is defined in terms of the @c equals
         * operation, but a sorted set performs all element comparisons using its
         * @c compareTo (or @c compare) method, so two elements that are
         * deemed equal by this method are, from the standpoint of the sorted set,
         * equal.  The behavior of a sorted set <i>is</i> well-defined even if its
         * ordering is inconsistent with equals; it just fails to obey the general
         * contract of the @c Set interface.
         * </p>
         * <p>
         * All general-purpose sorted set implementation classes should
         * provide four "standard" constructors: 1) A void (no arguments)
         * constructor, which creates an empty sorted set sorted according to
         * the natural ordering of its elements.  2) A constructor with a
         * single argument of type @c Comparator, which creates an empty
         * sorted set sorted according to the specified comparator.  3) A
         * constructor with a single argument of type @c Collection,
         * which creates a new sorted set with the same elements as its
         * argument, sorted according to the natural ordering of the elements.
         * 4) A constructor with a single argument of type @c SortedSet,
         * which creates a new sorted set with the same elements and the same
         * ordering as the input sorted set.  There is no way to enforce this
         * recommendation, as interfaces cannot contain constructors.
         * </p>
         * <p>
         * Note: several methods return subsets with restricted ranges.
         * Such ranges are <i>half-open</i>, that is, they include their low
         * endpoint but not their high endpoint (where applicable).
         * If you need a <i>closed range</i> (which includes both endpoints), and
         * the element type allows for calculation of the successor of a given
         * value, merely request the sub-range from @c lowEndpoint to
         * @c successor(highEndpoint).  For example, suppose that @c s
         * is a sorted set of strings.  The following idiom obtains a view
         * containing all the strings in @c s from @c low to
         * @c high, inclusive:
         * @code
         *   auto& sub = s.subSet(low, high+"\0");
         * @endcode
         *
         * A similar technique can be used to generate an <i>open range</i> (which
         * contains neither endpoint).  The following idiom obtains a view
         * containing all the Strings in @c s from @c low to
         * @c high, exclusive:
         * @code
         *   auto& sub = s.subSet(low+"\0", high);
         * @endcode
         * </p>
         * @param E the type of elements maintained by this set
         *
         * @see Set
         * @see TreeSet
         * @see SortedMap
         * @see Collection
         * @see Comparable
         * @see Comparator
         * @see ClassCastException
         */
        template<class E>
        class SortedSet : public virtual SequencedSet<E> {
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(CPR, typename Class<Comparator<E>>::Pointer);

        public:
            /**
             * Returns the comparator used to order the elements in this set.
             *
             * @return the comparator used to order the elements in this set.
             */
            virtual Comparator<E> const &comparator() const = 0;

            CORE_IMPORT_FIELD_OR_METHOD(Set<E>, contains);

            /**
             * Returns a view of the portion of this set whose elements range
             * from @c fromElement, inclusive, to @c toElement,
             * exclusive.  (If @c fromElement and @c toElement are
             * equal, the returned set is empty.)  The returned set is backed
             * by this set, so changes in the returned set are reflected in
             * this set, and vice-versa.  The returned set supports all
             * optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param fromElement low endpoint (inclusive) of the returned set
             * @param toElement high endpoint (exclusive) of the returned set
             * @return a view of the portion of this set whose elements range from
             *         @c fromElement, inclusive, to @c toElement, exclusive
             * @throws ClassCastException if @c fromElement and
             *         @c toElement cannot be compared to one another using this
             *         set's comparator (or, if the set has no comparator, using
             *         natural ordering).  Implementations may, but are not required
             *         to, throw this exception if @c fromElement or
             *         @c toElement cannot be compared to elements currently in
             *         the set.
             * @throws IllegalArgumentException if @c fromElement is
             *         greater than @c toElement; or if this set itself
             *         has a restricted range, and @c fromElement or
             *         @c toElement lies outside the bounds of the range
             */
            virtual SortedSet const &subSet(E const &fromElement, E const &toElement) const {
                try {
                    if (contains(fromElement)) {
                        E &from = UNSAFE::copyInstance(fromElement);
                        E &to = UNSAFE::copyInstance(toElement);

                        return UNSAFE::newInstance<SubSet>(*this, from, to);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            /**
             * Returns a view of the portion of this set whose elements range
             * from @c fromElement, inclusive, to @c toElement,
             * exclusive.  (If @c fromElement and @c toElement are
             * equal, the returned set is empty.)  The returned set is backed
             * by this set, so changes in the returned set are reflected in
             * this set, and vice-versa.  The returned set supports all
             * optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param fromElement low endpoint (inclusive) of the returned set
             * @param toElement high endpoint (exclusive) of the returned set
             * @return a view of the portion of this set whose elements range from
             *         @c fromElement, inclusive, to @c toElement, exclusive
             * @throws ClassCastException if @c fromElement and
             *         @c toElement cannot be compared to one another using this
             *         set's comparator (or, if the set has no comparator, using
             *         natural ordering).  Implementations may, but are not required
             *         to, throw this exception if @c fromElement or
             *         @c toElement cannot be compared to elements currently in
             *         the set.
             * @throws IllegalArgumentException if @c fromElement is
             *         greater than @c toElement; or if this set itself
             *         has a restricted range, and @c fromElement or
             *         @c toElement lies outside the bounds of the range
             */
            virtual SortedSet &subSet(E const &fromElement, E const &toElement) {
                try {
                    if (contains(fromElement)) {
                        E &from = UNSAFE::copyInstance(fromElement);
                        E &to = UNSAFE::copyInstance(toElement);

                        return UNSAFE::newInstance<SubSet>(*this, from, to);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            /**
             * Returns a view of the portion of this set whose elements are
             * strictly less than @c toElement.  The returned set is
             * backed by this set, so changes in the returned set are
             * reflected in this set, and vice-versa.  The returned set
             * supports all optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param toElement high endpoint (exclusive) of the returned set
             * @return a view of the portion of this set whose elements are strictly
             *         less than @c toElement
             * @throws ClassCastException if @c toElement is not compatible
             *         with this set's comparator (or, if the set has no comparator,
             *         if @c toElement does not implement @b Comparable).
             *         Implementations may, but are not required to, throw this
             *         exception if @c toElement cannot be compared to elements
             *         currently in the set.
             * @throws IllegalArgumentException if this set itself has a
             *         restricted range, and @c toElement lies outside the
             *         bounds of the range
             */
            virtual SortedSet const &headSet(E const &toElement) const {
                try {
                    E &to = UNSAFE::copyInstance(toElement);

                    return UNSAFE::newInstance<HeadSet>(*this, to);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this set whose elements are
             * strictly less than @c toElement.  The returned set is
             * backed by this set, so changes in the returned set are
             * reflected in this set, and vice-versa.  The returned set
             * supports all optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param toElement high endpoint (exclusive) of the returned set
             * @return a view of the portion of this set whose elements are strictly
             *         less than @c toElement
             * @throws ClassCastException if @c toElement is not compatible
             *         with this set's comparator (or, if the set has no comparator,
             *         if @c toElement does not implement @b Comparable).
             *         Implementations may, but are not required to, throw this
             *         exception if @c toElement cannot be compared to elements
             *         currently in the set.
             * @throws IllegalArgumentException if this set itself has a
             *         restricted range, and @c toElement lies outside the
             *         bounds of the range
             */
            virtual SortedSet &headSet(E const &toElement) {
                try {
                    E &to = UNSAFE::copyInstance(toElement);

                    return UNSAFE::newInstance<HeadSet>(*this, to);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this set whose elements are
             * greater than or equal to @c fromElement.  The returned
             * set is backed by this set, so changes in the returned set are
             * reflected in this set, and vice-versa.  The returned set
             * supports all optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param fromElement low endpoint (inclusive) of the returned set
             * @return a view of the portion of this set whose elements are greater
             *         than or equal to @c fromElement
             * @throws ClassCastException if @c fromElement is not compatible
             *         with this set's comparator (or, if the set has no comparator,
             *         if @c fromElement does not implement @b Comparable).
             *         Implementations may, but are not required to, throw this
             *         exception if @c fromElement cannot be compared to elements
             *         currently in the set.
             * @throws IllegalArgumentException if this set itself has a
             *         restricted range, and @c fromElement lies outside the
             *         bounds of the range
             */
            virtual SortedSet const &tailSet(E const &fromElement) const {
                try {
                    if (contains(fromElement)) {
                        E &from = UNSAFE::copyInstance(fromElement);

                        return UNSAFE::newInstance<TailSet>(*this, from);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            /**
             * Returns a view of the portion of this set whose elements are
             * greater than or equal to @c fromElement.  The returned
             * set is backed by this set, so changes in the returned set are
             * reflected in this set, and vice-versa.  The returned set
             * supports all optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param fromElement low endpoint (inclusive) of the returned set
             * @return a view of the portion of this set whose elements are greater
             *         than or equal to @c fromElement
             * @throws ClassCastException if @c fromElement is not compatible
             *         with this set's comparator (or, if the set has no comparator,
             *         if @c fromElement does not implement @b Comparable).
             *         Implementations may, but are not required to, throw this
             *         exception if @c fromElement cannot be compared to elements
             *         currently in the set.
             * @throws IllegalArgumentException if this set itself has a
             *         restricted range, and @c fromElement lies outside the
             *         bounds of the range
             */
            virtual SortedSet &tailSet(E const &fromElement) {
                try {
                    if (contains(fromElement)) {
                        E &from = UNSAFE::copyInstance(fromElement);

                        return UNSAFE::newInstance<TailSet>(*this, from);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            /**
             * Returns the first (lowest) element currently in this set.
             *
             * @return the first (lowest) element currently in this set
             * @throws NoSuchElementException if this set is empty
             */
            virtual E const &first() const {
                try {
                    for (E const &e: *this)
                        return e;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            /**
             * Returns the first (lowest) element currently in this set.
             *
             * @return the first (lowest) element currently in this set
             * @throws NoSuchElementException if this set is empty
             */
            virtual E &first() {
                try {
                    for (E &e: *this)
                        return e;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            /**
             * Returns the last (highest) element currently in this set.
             *
             * @return the last (highest) element currently in this set
             * @throws NoSuchElementException if this set is empty
             */
            virtual E const &last() const {
                try {
                    SortedSet const &r = reversed();
                    for (E const &e: r) {
                        UNSAFE::deleteInstance(r);
                        return e;
                    }

                    UNSAFE::deleteInstance(r);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            /**
             * Returns the last (highest) element currently in this set.
             *
             * @return the last (highest) element currently in this set
             * @throws NoSuchElementException if this set is empty
             */
            virtual E &last() {
                try {
                    SortedSet &r = reversed();
                    for (E &e: r) {
                        UNSAFE::deleteInstance(r);
                        return e;
                    }

                    UNSAFE::deleteInstance(r);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            // ========== SequencedCollection ==========

            /**
             * Throws @c UnsupportedOperationException. The encounter order induced by this
             * set's comparison method determines the position of elements, so explicit positioning
             * is not supported.
             *
             * @note
             * The implementation in this interface always throws @c UnsupportedOperationException.
             *
             * @throws UnsupportedOperationException always
             */
            void addFirst(E const &e) override {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Throws @c UnsupportedOperationException. The encounter order induced by this
             * set's comparison method determines the position of elements, so explicit positioning
             * is not supported.
             *
             * @note
             * The implementation in this interface always throws @c UnsupportedOperationException.
             *
             * @throws UnsupportedOperationException always
             */
            void addLast(E const &e) override {
                UnsupportedOperationException().throws($ftrace());
            }

            CORE_IMPORT_FIELD_OR_METHOD(Set<E>, remove);

            /**
             *
             *
             * @note
             * The implementation in this interface calls the @c first method to obtain the first
             * element, then it calls @c remove(element) to remove the element, and then it returns
             * the element.
             *
             * @throws NoSuchElementException
             * @throws UnsupportedOperationException
             *
             */
            E &removeFirst() override {
                try {
                    E &e = first();
                    remove(e);
                    return e;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @note
             * The implementation in this interface calls the @c last method to obtain the last
             * element, then it calls @c remove(element) to remove the element, and then it returns
             * the element.
             *
             * @throws NoSuchElementException
             * @throws UnsupportedOperationException
             *
             */
            E &removeLast() override {
                try {
                    E &e = first();
                    remove(e);
                    return e;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @note
             * The implementation in this interface returns a reverse-ordered SortedSet
             * view. The @c reversed() method of the view returns a reference
             * to this SortedSet. Other operations on the view are implemented via calls to
             * public methods on this SortedSet. The exact relationship between calls on the
             * view and calls on this SortedSet is unspecified. However, order-sensitive
             * operations generally behave as if they delegate to the appropriate method
             * with the opposite orientation. For example, calling @c first on the
             * view might result in a call to @c last on this SortedSet.
             *
             * @return a reverse-ordered view of this collection, as a @c SortedSet
             *
             */
            SortedSet const &reversed() const override {
                try {
                    if (Class<ReversedOrderView>::hasInstance(*this)) {
                        ReversedOrderView const &r = CORE_XCAST(ReversedOrderView const, *this);
                        return UNSAFE::newInstance<ReversedOrderView>(r.reversed());
                    }

                    return UNSAFE::newInstance<ReversedOrderView>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * @note
             * The implementation in this interface returns a reverse-ordered SortedSet
             * view. The @c reversed() method of the view returns a reference
             * to this SortedSet. Other operations on the view are implemented via calls to
             * public methods on this SortedSet. The exact relationship between calls on the
             * view and calls on this SortedSet is unspecified. However, order-sensitive
             * operations generally behave as if they delegate to the appropriate method
             * with the opposite orientation. For example, calling @c first on the
             * view might result in a call to @c last on this SortedSet.
             *
             * @return a reverse-ordered view of this collection, as a @c SortedSet
             *
             */
            SortedSet &reversed() override {
                try {
                    if (Class<ReversedOrderView>::hasInstance(*this)) {
                        ReversedOrderView &r = CORE_XCAST(ReversedOrderView, *this);
                        return UNSAFE::newInstance<ReversedOrderView>(r.reversed());
                    }

                    return UNSAFE::newInstance<ReversedOrderView>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            /**
             * Provides a reversed-ordered view of a SortedSet. Not serializable.
             */
            class ReversedOrderView;
            class HeadSet;
            class TailSet;
            class SubSet;
        };

        template<class E>
        class SortedSet<E>::HeadSet final : public SortedSet {
            CORE_ALIAS(ITR_CACHE, Class<Object>::Pointer);
            CORE_ALIAS(ITR2_CACHE, Class<Object const>::Pointer);
            SortedSet &view;
            E &tail; // Exclusive

            /**
             * Return whether e is below the tail, exclusive
             *
             * @param val expected value
             * @return whether e is below the tail, exclusive
             */
            gbool inRange(E const &val) const {
                try {
                    return view.comparator().compare(val, tail) < 0;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        public:
            CORE_EXPLICIT HeadSet(SortedSet const &view, E &toElement)
                : view(CORE_CAST(SortedSet&, view)), tail(toElement) {
            }

            gint size() const override {
                try {
                    gint n = 0;
                    for (E const &_: *this)
                        n += 1;

                    return n;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    HeadSet &headView;
                    Iterator<E> &it;

                    ITR_CACHE mutable cache = null;
                    gbool mutable isEnd = false;

                public:
                    CORE_EXPLICIT Itr(HeadSet &headView, Iterator<E> &it) : headView(headView), it(it) {
                    }

                    gbool hasNext() const override {
                        if (isEnd)
                            return false;

                        if (cache != null)
                            return true;

                        if (it.hasNext()) {
                            E &e = it.next();

                            if (!headView.inRange(e)) {
                                isEnd = true;
                                return false;
                            }

                            cache = &e;
                            return true;
                        }

                        return false;
                    }

                    E & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        E &e = CORE_XCAST(E, *cache);
                        return e;
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &other = CORE_XCAST(Itr const, o);
                        if (&headView != &other.headView)
                            return false;

                        if (isEnd)
                            return other.isEnd;

                        if (other.isEnd)
                            return false;

                        return it == other.it;
                    }

                    void remove() override {
                        try {
                            return it.remove();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(SortedSet&, view).iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                class Itr final : public Iterator2<E> {
                    HeadSet const &headView;
                    Iterator2<E> &it;

                    ITR2_CACHE mutable cache = null;
                    gbool mutable isEnd = false;

                public:
                    CORE_EXPLICIT Itr(HeadSet const &headView, Iterator2<E> &it)
                        : headView(headView), it(it) {
                    }

                    gbool hasNext() const override {
                        if (isEnd)
                            return false;

                        if (cache != null)
                            return true;

                        if (it.hasNext()) {
                            E const &e = it.next();

                            if (!headView.inRange(e)) {
                                isEnd = true;
                                return false;
                            }

                            cache = &e;
                            return true;
                        }

                        return false;
                    }

                    E const & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        E const &e = CORE_XCAST(E const, *cache);
                        return e;
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &other = CORE_XCAST(Itr const, o);
                        if (&headView != &other.headView)
                            return false;

                        if (isEnd)
                            return other.isEnd;

                        if (other.isEnd)
                            return false;

                        return it == other.it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(SortedSet const&, view).iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool add(const E &val) override {
                try {
                    if (inRange(val))
                        return view.add(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            gbool remove(const E &val) override {
                try {
                    if (inRange(val))
                        return view.remove(val);

                    return false;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Comparator<E> &comparator() const override {
                return view.comparator();
            }

            gbool contains(const E &val) const override {
                if (inRange(val))
                    return view.contains(val);

                return false;
            }

            const E &first() const override {
                try {
                    return view.first();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &first() override {
                try {
                    return view.first();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &last() const override {
                try {
                    ITR2_CACHE cache = null;
                    for (E const &e: *this)
                        cache = &e;

                    if (cache != null)
                        return CORE_XCAST(E const, *cache);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &last() override {
                try {
                    ITR_CACHE cache = null;
                    for (E &e: *this)
                        cache = &e;

                    if (cache != null)
                        return CORE_XCAST(E, *cache);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const SortedSet &subSet(const E &fromElement, const E &toElement) const override {
                try {
                    if (inRange(fromElement) && comparator().compare(fromElement, toElement) <= 0) {
                        if (inRange(toElement))
                            return view.subSet(fromElement, toElement);

                        if (toElement == tail)
                            return view.subSet(fromElement, tail);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &subSet(const E &fromElement, const E &toElement) override {
                try {
                    if (inRange(fromElement) && comparator().compare(fromElement, toElement) <= 0) {
                        if (inRange(toElement))
                            return view.subSet(fromElement, toElement);

                        if (toElement == tail)
                            return view.subSet(fromElement, tail);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const SortedSet &headSet(const E &toElement) const override {
                try {
                    if (inRange(toElement))
                        return view.headSet(toElement);

                    if (toElement == tail)
                        return *this;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &headSet(const E &toElement) override {
                try {
                    if (inRange(toElement))
                        return view.headSet(toElement);

                    if (toElement == tail)
                        return *this;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const SortedSet &tailSet(const E &fromElement) const override {
                try {
                    if (inRange(fromElement))
                        return view.subSet(fromElement, tail);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &tailSet(const E &fromElement) override {
                try {
                    if (inRange(fromElement))
                        return view.subSet(fromElement, tail);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<HeadSet>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };

        template<class E>
        class SortedSet<E>::TailSet final : public SortedSet {
            CORE_ALIAS(ITR_CACHE, Class<Object>::Pointer);
            CORE_ALIAS(ITR2_CACHE, Class<Object const>::Pointer);
            SortedSet &view;
            E &head; // Exclusive

            /**
             * Return whether e is below the tail, exclusive
             *
             * @param val expected value
             * @return whether e is below the tail, exclusive
             */
            gbool inRange(E const &val) const {
                try {
                    return view.comparator().compare(val, head) >= 0;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        public:
            CORE_EXPLICIT TailSet(SortedSet const &view, E &fromElement)
                : view(CORE_CAST(SortedSet&, view)), head(fromElement) {
            }

            gint size() const override {
                try {
                    gint n = 0;
                    for (E const &_: *this)
                        n += 1;

                    return n;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    TailSet &tailView;
                    Iterator<E> &it;

                    ITR_CACHE mutable cache = null;

                public:
                    CORE_EXPLICIT Itr(TailSet &tailView, Iterator<E> &it) : tailView(tailView), it(it) {
                    }

                    gbool hasNext() const override {
                        if (cache != null)
                            return true;

                        while (it.hasNext()) {
                            E &e = it.next();

                            if (!tailView.inRange(e))
                                continue;

                            cache = &e;
                            return true;
                        }

                        return false;
                    }

                    E & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        E &e = CORE_XCAST(E, *cache);
                        return e;
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &other = CORE_XCAST(Itr const, o);
                        if (&tailView != &other.tailView)
                            return false;

                        return it == other.it;
                    }

                    void remove() override {
                        try {
                            return it.remove();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(SortedSet&, view).iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                class Itr final : public Iterator2<E> {
                    TailSet const &tailView;
                    Iterator2<E> &it;

                    ITR2_CACHE mutable cache = null;

                public:
                    CORE_EXPLICIT Itr(TailSet const &tailView, Iterator2<E> &it)
                        : tailView(tailView), it(it) {
                    }

                    gbool hasNext() const override {
                        if (cache != null)
                            return true;

                        while (it.hasNext()) {
                            E const &e = it.next();

                            if (!tailView.inRange(e))
                                continue;

                            cache = &e;
                            return true;
                        }

                        return false;
                    }

                    E const & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        E const &e = CORE_XCAST(E const, *cache);
                        return e;
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &other = CORE_XCAST(Itr const, o);
                        if (&tailView != &other.tailView)
                            return false;

                        return it == other.it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(SortedSet const&, view).iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool add(const E &val) override {
                try {
                    if (inRange(val))
                        return view.add(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            gbool remove(const E &val) override {
                try {
                    if (inRange(val))
                        return view.remove(val);

                    return false;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Comparator<E> &comparator() const override {
                return view.comparator();
            }

            gbool contains(const E &val) const override {
                if (inRange(val))
                    return view.contains(val);

                return false;
            }

            const E &first() const override {
                try {
                    return view.first();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &first() override {
                try {
                    return view.first();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &last() const override {
                try {
                    ITR2_CACHE cache = null;
                    for (E const &e: *this)
                        cache = &e;

                    if (cache != null)
                        return CORE_XCAST(E const, *cache);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &last() override {
                try {
                    ITR_CACHE cache = null;
                    for (E &e: *this)
                        cache = &e;

                    if (cache != null)
                        return CORE_XCAST(E, *cache);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const SortedSet &subSet(const E &fromElement, const E &toElement) const override {
                try {
                    if (inRange(fromElement) && inRange(toElement) && comparator().compare(fromElement, toElement) <= 0)
                        return view.subSet(fromElement, toElement);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &subSet(const E &fromElement, const E &toElement) override {
                try {
                    if (inRange(fromElement) && inRange(toElement) && comparator().compare(fromElement, toElement) <= 0)
                        return view.subSet(fromElement, toElement);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const SortedSet &headSet(const E &toElement) const override {
                try {
                    if (inRange(toElement))
                        return view.subSet(head, toElement);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &headSet(const E &toElement) override {
                try {
                    if (inRange(toElement))
                        return view.subSet(head, toElement);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const SortedSet &tailSet(const E &fromElement) const override {
                try {
                    if (inRange(fromElement))
                        return view.tailSet(fromElement);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &tailSet(const E &fromElement) override {
                try {
                    if (inRange(fromElement))
                        return view.tailSet(fromElement);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<TailSet>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };

        template<class E>
        class SortedSet<E>::SubSet final : public SortedSet {
            CORE_ALIAS(ITR_CACHE, Class<Object>::Pointer);
            CORE_ALIAS(ITR2_CACHE, Class<Object const>::Pointer);
            SortedSet &view;
            E &head; // Inclusive
            E &tail; // Exclusive

            /**
             * Return whether e is below the tail, exclusive
             *
             * @param val expected value
             * @return whether e is below the tail, exclusive
             */
            gbool aboveHead(E const &val) const {
                try {
                    return view.comparator().compare(val, head) >= 0;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Return whether e is below the tail, exclusive
             *
             * @param val expected value
             * @return whether e is below the tail, exclusive
             */
            gbool bellowTail(E const &val) const {
                try {
                    return view.comparator().compare(val, tail) < 0;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        public:
            CORE_EXPLICIT SubSet(SortedSet const &view, E &fromElement, E &toElement)
                : view(CORE_CAST(SortedSet&, view)), head(fromElement), tail(toElement) {
            }

            gint size() const override {
                try {
                    gint n = 0;
                    for (E const &_: *this)
                        n += 1;

                    return n;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    SubSet &subView;
                    Iterator<E> &it;

                    ITR_CACHE mutable cache = null;
                    gbool mutable isEnd = false;

                public:
                    CORE_EXPLICIT Itr(SubSet &subView, Iterator<E> &it) : subView(subView), it(it) {
                    }

                    gbool hasNext() const override {
                        if (isEnd)
                            return false;

                        if (cache != null)
                            return true;

                        while (it.hasNext()) {
                            E &e = it.next();

                            if (!subView.bellowTail(e))
                                continue;

                            if (!subView.aboveHead(e)) {
                                isEnd = true;
                                return false;
                            }

                            cache = &e;
                            return true;
                        }

                        return false;
                    }

                    E & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        E &e = CORE_XCAST(E, *cache);
                        return e;
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &other = CORE_XCAST(Itr const, o);
                        if (&subView != &other.subView)
                            return false;

                        if (isEnd)
                            return other.isEnd;

                        if (other.isEnd)
                            return false;

                        return it == other.it;
                    }

                    void remove() override {
                        try {
                            return it.remove();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(SortedSet&, view).iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                class Itr final : public Iterator2<E> {
                    SubSet const &subView;
                    Iterator2<E> &it;

                    ITR2_CACHE mutable cache = null;
                    gbool mutable isEnd = false;

                public:
                    CORE_EXPLICIT Itr(SubSet const &subView, Iterator2<E> &it)
                        : subView(subView), it(it) {
                    }

                    gbool hasNext() const override {
                        if (isEnd)
                            return false;

                        if (cache != null)
                            return true;

                        while (it.hasNext()) {
                            E const &e = it.next();

                            if (!subView.bellowTail(e))
                                continue;

                            if (!subView.aboveHead(e)) {
                                isEnd = true;
                                return false;
                            }

                            cache = &e;
                            return true;
                        }

                        return false;
                    }

                    E const & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        E const &e = CORE_XCAST(E const, *cache);
                        return e;
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &other = CORE_XCAST(Itr const, o);
                        if (&subView != &other.subView)
                            return false;

                        if (isEnd)
                            return other.isEnd;

                        if (other.isEnd)
                            return false;

                        return it == other.it;
                    }

                    ~Itr() override {
                        UNSAFE::deleteInstance(it);
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(SortedSet const&, view).iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool add(const E &val) override {
                try {
                    if (aboveHead(val) && bellowTail(val))
                        return view.add(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            gbool remove(const E &val) override {
                try {
                    if (aboveHead(val) && bellowTail(val))
                        return view.remove(val);

                    return false;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Comparator<E> &comparator() const override {
                return view.comparator();
            }

            gbool contains(const E &val) const override {
                if (aboveHead(val) && bellowTail(val))
                    return view.contains(val);

                return false;
            }

            const E &first() const override {
                try {
                    for (const E &e: *this)
                        return e;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &first() override {
                try {
                    for (E &e: *this)
                        return e;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &last() const override {
                try {
                    ITR2_CACHE cache = null;
                    for (E const &e: *this)
                        cache = &e;

                    if (cache != null)
                        return CORE_XCAST(E const, *cache);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &last() override {
                try {
                    ITR_CACHE cache = null;
                    for (E &e: *this)
                        cache = &e;

                    if (cache != null)
                        return CORE_XCAST(E, *cache);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const SortedSet &subSet(const E &fromElement, const E &toElement) const override {
                try {
                    if (aboveHead(fromElement) && bellowTail(fromElement) &&
                        aboveHead(toElement) && comparator().compare(fromElement, toElement) <= 0) {
                        if (bellowTail(toElement))
                            return view.subSet(fromElement, toElement);

                        if (toElement == tail)
                            return view.subSet(fromElement, tail);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &subSet(const E &fromElement, const E &toElement) override {
                try {
                    if (aboveHead(fromElement) && bellowTail(fromElement) &&
                        aboveHead(toElement) && comparator().compare(fromElement, toElement) <= 0) {
                        if (bellowTail(toElement))
                            return view.subSet(fromElement, toElement);

                        if (toElement == tail)
                            return view.subSet(fromElement, tail);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const SortedSet &headSet(const E &toElement) const override {
                try {
                    if (aboveHead(toElement)) {
                        if (bellowTail(toElement))
                            return view.subSet(head, toElement);

                        if (toElement == tail)
                            return *this;
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &headSet(const E &toElement) override {
                try {
                    if (aboveHead(toElement)) {
                        if (bellowTail(toElement))
                            return view.subSet(head, toElement);

                        if (toElement == tail)
                            return *this;
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const SortedSet &tailSet(const E &fromElement) const override {
                try {
                    if (aboveHead(fromElement) && bellowTail(fromElement))
                        return view.subSet(fromElement, tail);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &tailSet(const E &fromElement) override {
                try {
                    if (aboveHead(fromElement) && bellowTail(fromElement))
                        return view.subSet(fromElement, tail);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<SubSet>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };

        template<class E>
        class SortedSet<E>::ReversedOrderView final : public SortedSet {
            CORE_ALIAS(VIEW_CACHE, typename Class<SortedSet>::Pointer);
            CORE_ALIAS(VIEW2_CACHE, typename Class<SortedSet const>::Pointer);
            CORE_ALIAS(ITR_CACHE, Class<Object>::Pointer);
            CORE_ALIAS(ITR2_CACHE, Class<Object const>::Pointer);

            SortedSet &base;
            CPR mutable cpr = {};

        public:
            CORE_EXPLICIT ReversedOrderView(SortedSet const &set)
                : base(CORE_CAST(SortedSet &, set)), cpr(&set.comparator().reversed()) {
            }

            gbool equals(const Object &o) const override {
                if (this == &o) return true;

                if (!Class<Set<E> >::hasInstance(o)) return false;

                Collection<E> const &other = CORE_XCAST(Collection<E> const, o);
                if (this->size() != other.size()) return false;

                return containsAll(other);
            }

            gint hash() const override {
                try {
                    Iterator2<E> &it = this->iterator();
                    gint h = 0;
                    while (it.hasNext()) {
                        Object const &o = it.next();
                        h += o.hash();
                    }
                    UNSAFE::deleteInstance(it);
                    return h;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    Iterator2<E> &it = this->iterator();
                    if (!it.hasNext()) {
                        UNSAFE::deleteInstance(it);
                        return "[]"_S;
                    }

                    XString str = XString();
                    str.append(u'[');
                    for (;;) {
                        Object const &val = it.next();
                        if (this == &val || &base == &val) {
                            // Replace by "{..}"
                            str.append(u'{').append(u'.').append(u'.').append(u'}');
                        } else
                            str.append(val);

                        if (!it.hasNext()) {
                            UNSAFE::deleteInstance(it);
                            return str.append(u']').toString();
                        }

                        str.append(u',').append(u' ');
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E &> &action) override {
                try {
                    for (E &e: *this) action.accept(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E> &action) const override {
                try {
                    for (E const &e: *this) action.accept(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool add(const E &val) override {
                try {
                    return base.add(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool addAll(const Collection<E> &c) override {
                try {
                    return base.addAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    base.clear();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool contains(const E &val) const override {
                try {
                    return base.contains(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(const Collection<E> &c) const override {
                try {
                    return base.containsAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    return base.isEmpty();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool remove(const E &val) override {
                try {
                    return base.remove(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(const Collection<E> &c) override {
                try {
                    return base.removeAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(const Collection<E> &c) override {
                try {
                    return base.retainAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gint size() const override { return base.size(); }

            Array<E> toArray() const override {
                try {
                    return Arrays::reverse(base.toArray());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Comparator<E> &comparator() const override {
                if (cpr->reversed() != base.comparator()) {
                    UNSAFE::deleteInstance(*cpr);
                    cpr = &base.comparator().reversed();
                }
                return *cpr;
            }

            const E &first() const override {
                try {
                    return base.last();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &first() override {
                try {
                    return base.last();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &last() const override {
                try {
                    return base.first();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &last() override {
                try {
                    return base.first();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const SortedSet &reversed() const override { return base; }

            SortedSet &reversed() override { return base; }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    SortedSet &set;

                    VIEW_CACHE view = null;
                    ITR_CACHE cache = null;

                public:
                    CORE_EXPLICIT Itr(SortedSet &set) : set(set) {
                        view = &set;
                    }

                    gbool hasNext() const override { return !view->isEmpty(); }

                    E & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        try {
                            E &e = view->last();
                            cache = &e;
                            VIEW_CACHE v = view;
                            view = &set.headSet(e);
                            if (v != null)
                                UNSAFE::deleteInstance(*v);

                            return e;
                        } catch (Throwable const &ex) {
                            ex.throws($ftrace());
                        }
                    }

                    void remove() override {
                        if (cache == null)
                            IllegalStateException().throws($ftrace());

                        try {
                            set.remove(CORE_XCAST(E const, *cache));
                            cache = null;
                        } catch (Throwable const &ex) {
                            ex.throws($ftrace());
                        }
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &it = CORE_XCAST(Itr const, o);

                        if (&set != &it.set)
                            return false;

                        VIEW_CACHE v1 = view;
                        VIEW_CACHE v2 = it.view;

                        return v1 != null && v2 != null && *v1 == *v2;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                class Itr final : public Iterator2<E> {
                    SortedSet const &set;

                    VIEW2_CACHE view = null;
                    ITR2_CACHE cache = null;

                public:
                    CORE_EXPLICIT Itr(SortedSet const &set) : set(set) {
                        view = &set;
                    }

                    gbool hasNext() const override { return !view->isEmpty(); }

                    E const & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        try {
                            E const &e = view->last();
                            cache = &e;
                            VIEW2_CACHE v = view;
                            view = &set.headSet(e);
                            if (v != null)
                                UNSAFE::deleteInstance(*v);

                            return e;
                        } catch (Throwable const &ex) {
                            ex.throws($ftrace());
                        }
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &it = CORE_XCAST(Itr const, o);

                        if (&set != &it.set)
                            return false;

                        VIEW2_CACHE v1 = view;
                        VIEW2_CACHE v2 = it.view;

                        return v1 != null && v2 != null && *v1 == *v2;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const SortedSet &subSet(const E &fromElement, const E &toElement) const override {
                try {
                    if (comparator().compare(fromElement, toElement) <= 0 && base.contains(fromElement)) {
                        E &from = UNSAFE::copyInstance(fromElement);
                        E &to = UNSAFE::copyInstance(toElement);

                        return UNSAFE::newInstance<SubSet>(*this, from, to);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &subSet(const E &fromElement, const E &toElement) override {
                try {
                    if (comparator().compare(fromElement, toElement) <= 0 && base.contains(fromElement)) {
                        E &from = UNSAFE::copyInstance(fromElement);
                        E &to = UNSAFE::copyInstance(toElement);

                        return UNSAFE::newInstance<SubSet>(*this, from, to);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const SortedSet &headSet(const E &toElement) const override {
                try {
                    E &to = UNSAFE::copyInstance(toElement);

                    return UNSAFE::newInstance<HeadSet>(*this, to);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            SortedSet &headSet(const E &toElement) override {
                try {
                    E &to = UNSAFE::copyInstance(toElement);

                    return UNSAFE::newInstance<HeadSet>(*this, to);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const SortedSet &tailSet(const E &fromElement) const override {
                try {
                    if (base.contains(fromElement)) {
                        E &from = UNSAFE::copyInstance(fromElement);

                        return UNSAFE::newInstance<TailSet>(*this, from);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            SortedSet &tailSet(const E &fromElement) override {
                try {
                    if (base.contains(fromElement)) {
                        E &from = UNSAFE::copyInstance(fromElement);

                        return UNSAFE::newInstance<TailSet>(*this, from);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }
        };
    } // util
} // core

#endif //CORE24_SORTEDSET_H
