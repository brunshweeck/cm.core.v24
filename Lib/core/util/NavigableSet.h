//
// Created by brunshweeck on 7 août 2024.
//

#ifndef CORE24_NAVIGABLESET_H
#define CORE24_NAVIGABLESET_H
#include <core/util/SortedSet.h>

namespace core {
    namespace util {
        /**
         * A @b SortedSet extended with navigation methods reporting
         * the closest matches for given search targets. Methods @b lower,
         * @b floor, @b ceiling, and @b higher return elements
         * respectively less than, less than or equal, greater than or equal,
         * and greater than a given element.
         *
         * <p>
         * A @c NavigableSet may be accessed and traversed in either
         * ascending or descending order.  The @b descendingSet method
         * returns a view of the set with the senses of all relational and
         * directional methods inverted. The performance of ascending
         * operations and views is likely to be faster than that of descending
         * ones.  This interface additionally defines methods
         * @b pollFirst and @b pollLast that return and remove the lowest
         * and highest element, if one exists, else returning @c null.
         * Methods
         * @b subSet(E, boolean, E, boolean),
         * @b headSet(E, boolean), and
         * @b tailSet(E, boolean)
         * differ from the like-named @c SortedSet methods in accepting
         * additional arguments describing whether lower and upper bounds are
         * inclusive versus exclusive.  Subsets of any @c NavigableSet
         * must implement the @c NavigableSet interface.
         * </p>
         * <p>
         * Methods
         * <b> subSet(E, E) </b>,
         * <b> headSet(E) </b>, and
         * <b> tailSet(E) </b>
         * are specified to return @c SortedSet to allow existing
         * implementations of @c SortedSet to be compatibly retrofitted to
         * implement @c NavigableSet, but extensions and implementations
         * of this interface are encouraged to override these methods to return
         * @c NavigableSet.
         *
         * @tparam E the type of elements maintained by this set
         */
        template<class E>
        class NavigableSet : public virtual SortedSet<E> {
        protected:
            CORE_ALIAS(UNSAFE, misc::Unsafe);

        public:
            /**
             * Returns the greatest element in this set strictly less than the
             * given element, or @c null if there is no such element.
             *
             * @param e the value to match
             * @return the greatest element less than @c e,
             *         or @c null if there is no such element
             * @throws ClassCastException if the specified element cannot be
             *         compared with the elements currently in the set
             */
            virtual E const &lower(E const &e) const = 0;

            /**
             * Returns the greatest element in this set strictly less than the
             * given element, or @c null if there is no such element.
             *
             * @param e the value to match
             * @return the greatest element less than @c e,
             *         or @c null if there is no such element
             * @throws ClassCastException if the specified element cannot be
             *         compared with the elements currently in the set
             */
            virtual E &lower(E const &e) = 0;

            /**
             * Returns the greatest element in this set less than or equal to
             * the given element, or @c null if there is no such element.
             *
             * @param e the value to match
             * @return the greatest element less than or equal to @c e
             * @throws ClassCastException if the specified element cannot be
             *         compared with the elements currently in the set
             */
            virtual E const &floor(E const &e) const = 0;

            /**
             * Returns the greatest element in this set less than or equal to
             * the given element, or @c null if there is no such element.
             *
             * @param e the value to match
             * @return the greatest element less than or equal to @c e
             * @throws ClassCastException if the specified element cannot be
             *         compared with the elements currently in the set
             */
            virtual E &floor(E const &e) = 0;

            /**
             * Returns the least element in this set greater than or equal to
             * the given element, or @c null if there is no such element.
             *
             * @param e the value to match
             * @return the least element greater than or equal to @c e
             * @throws ClassCastException if the specified element cannot be
             *         compared with the elements currently in the set
             */
            virtual E const &ceiling(E const &e) const = 0;

            /**
             * Returns the least element in this set greater than or equal to
             * the given element, or @c null if there is no such element.
             *
             * @param e the value to match
             * @return the least element greater than or equal to @c e
             * @throws ClassCastException if the specified element cannot be
             *         compared with the elements currently in the set
             */
            virtual E &ceiling(E const &e) = 0;

            /**
             * Returns the least element in this set strictly greater than the
             * given element, or @c null if there is no such element.
             *
             * @param e the value to match
             * @return the least element greater than @c e
             * @throws ClassCastException if the specified element cannot be
             *         compared with the elements currently in the set
             */
            virtual E const &higher(E const &e) const = 0;

            /**
             * Returns the least element in this set strictly greater than the
             * given element, or @c null if there is no such element.
             *
             * @param e the value to match
             * @return the least element greater than @c e
             * @throws ClassCastException if the specified element cannot be
             *         compared with the elements currently in the set
             */
            virtual E &higher(E const &e) = 0;

            /**
             * Returns an iterator over the elements in this set, in ascending order.
             *
             * @return an iterator over the elements in this set, in ascending order
             */
            Iterator<E> &iterator() override = 0;

            /**
             * Returns an iterator over the elements in this set, in ascending order.
             *
             * @return an iterator over the elements in this set, in ascending order
             */
            Iterator2<E> &iterator() const override = 0;

            /**
             * Returns a reverse order view of the elements contained in this set.
             * The descending set is backed by this set, so changes to the set are
             * reflected in the descending set, and vice-versa.  If either set is
             * modified while an iteration over either set is in progress (except
             * through the iterator's own @c remove operation), the results of
             * the iteration are undefined.
             *
             * @return a reverse order view of this set
             */
            virtual NavigableSet const &descendingSet() const {
                try {
                    return UNSAFE::newInstance<ReversedOrderView>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a reverse order view of the elements contained in this set.
             * The descending set is backed by this set, so changes to the set are
             * reflected in the descending set, and vice-versa.  If either set is
             * modified while an iteration over either set is in progress (except
             * through the iterator's own @c remove operation), the results of
             * the iteration are undefined.
             *
             * @return a reverse order view of this set
             */
            virtual NavigableSet &descendingSet() {
                try {
                    return UNSAFE::newInstance<ReversedOrderView>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this set, in descending order.
             * Equivalent in effect to @code descendingSet().iterator() @endcode.
             *
             * @return an iterator over the elements in this set, in descending order
             */
            virtual Iterator<E> &descendingIterator() {
                try {
                    return descendingSet().iterator();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this set, in descending order.
             * Equivalent in effect to @code descendingSet().iterator() @endcode.
             *
             * @return an iterator over the elements in this set, in descending order
             */
            virtual Iterator2<E> &descendingIterator() const {
                try {
                    return descendingSet().iterator();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this set whose elements range from
             * @c fromElement to @c toElement.  If @c fromElement and
             * @c toElement are equal, the returned set is empty unless
             * @c fromInclusive and @c toInclusive are both true.  The returned set
             * is backed by this set, so changes in the returned set are reflected in
             * this set, and vice-versa.  The returned set supports all optional set
             * operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param fromElement low endpoint of the returned set
             * @param fromInclusive @c true if the low endpoint
             *        is to be included in the returned view
             * @param toElement high endpoint of the returned set
             * @param toInclusive @c true if the high endpoint
             *        is to be included in the returned view
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
             *         @c toElement lies outside the bounds of the range.
             */
            virtual NavigableSet const &subSet(E const &fromElement, gbool fromInclusive,
                                               E const &toElement, gbool toInclusive) const {
                try {
                    E &from = UNSAFE::copyInstance(fromElement);
                    E &to = UNSAFE::copyInstance(toElement);

                    return UNSAFE::newInstance<SubSet>(*this, from, fromInclusive, to, toInclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this set whose elements range from
             * @c fromElement to @c toElement.  If @c fromElement and
             * @c toElement are equal, the returned set is empty unless
             * @c fromInclusive and @c toInclusive are both true.  The returned set
             * is backed by this set, so changes in the returned set are reflected in
             * this set, and vice-versa.  The returned set supports all optional set
             * operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param fromElement low endpoint of the returned set
             * @param fromInclusive @c true if the low endpoint
             *        is to be included in the returned view
             * @param toElement high endpoint of the returned set
             * @param toInclusive @c true if the high endpoint
             *        is to be included in the returned view
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
             *         @c toElement lies outside the bounds of the range.
             */
            virtual NavigableSet &subSet(E const &fromElement, gbool fromInclusive,
                                         E const &toElement, gbool toInclusive) {
                try {
                    E &from = UNSAFE::copyInstance(fromElement);
                    E &to = UNSAFE::copyInstance(toElement);

                    return UNSAFE::newInstance<SubSet>(*this, from, fromInclusive, to, toInclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this set whose elements are less than
             * (or equal to, if @c inclusive is true) @c toElement.  The
             * returned set is backed by this set, so changes in the returned set are
             * reflected in this set, and vice-versa.  The returned set supports all
             * optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param toElement high endpoint of the returned set
             * @param inclusive @c true if the high endpoint
             *        is to be included in the returned view
             * @return a view of the portion of this set whose elements are less than
             *         (or equal to, if @c inclusive is true) @c toElement
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
            virtual NavigableSet const &headSet(E const &toElement, gbool inclusive) const {
                try {
                    E &to = UNSAFE::copyInstance(toElement);

                    return UNSAFE::newInstance<HeadSet>(*this, to, inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this set whose elements are less than
             * (or equal to, if @c inclusive is true) @c toElement.  The
             * returned set is backed by this set, so changes in the returned set are
             * reflected in this set, and vice-versa.  The returned set supports all
             * optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param toElement high endpoint of the returned set
             * @param inclusive @c true if the high endpoint
             *        is to be included in the returned view
             * @return a view of the portion of this set whose elements are less than
             *         (or equal to, if @c inclusive is true) @c toElement
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
            virtual NavigableSet &headSet(E const &toElement, gbool inclusive) {
                try {
                    E &to = UNSAFE::copyInstance(toElement);

                    return UNSAFE::newInstance<HeadSet>(*this, to, inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this set whose elements are greater
             * than (or equal to, if @c inclusive is true) @c fromElement.
             * The returned set is backed by this set, so changes in the returned set
             * are reflected in this set, and vice-versa.  The returned set supports
             * all optional set operations that this set supports.
             *
             * <p>
             * The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             * </p>
             * @param fromElement low endpoint of the returned set
             * @param inclusive @c true if the low endpoint
             *        is to be included in the returned view
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
            virtual NavigableSet const &tailSet(E const &fromElement, gbool inclusive) const {
                try {
                    E &from = UNSAFE::copyInstance(fromElement);

                    return UNSAFE::newInstance<HeadSet>(*this, from, inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this set whose elements are greater
             * than (or equal to, if @c inclusive is true) @c fromElement.
             * The returned set is backed by this set, so changes in the returned set
             * are reflected in this set, and vice-versa.  The returned set supports
             * all optional set operations that this set supports.
             *
             * <p>The returned set will throw an @c IllegalArgumentException
             * on an attempt to insert an element outside its range.
             *
             * @param fromElement low endpoint of the returned set
             * @param inclusive @c true if the low endpoint
             *        is to be included in the returned view
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
            virtual NavigableSet &tailSet(E const &fromElement, gbool inclusive) {
                try {
                    E &from = UNSAFE::copyInstance(fromElement);

                    return UNSAFE::newInstance<HeadSet>(*this, from, inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * <p>
             * Equivalent to @code subSet(fromElement, true, toElement, false) @endcode .
             * </p>
             *
             * @throws ClassCastException
             * @throws NullPointerException
             * @throws IllegalArgumentException
             */
            const SortedSet<E> &subSet(const E &fromElement, const E &toElement) const override {
                try {
                    return subSet(fromElement, true, toElement, false);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * <p>
             * Equivalent to @c subSet(fromElement, true, toElement, false).
             * </p>
             *
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            SortedSet<E> &subSet(const E &fromElement, const E &toElement) override {
                try {
                    return subSet(fromElement, true, toElement, false);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * <p>
             * Equivalent to @code headSet(toElement, false) @endcode
             * </p>.
             *
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            const SortedSet<E> &headSet(const E &toElement) const override {
                try {
                    return headSet(toElement, false);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * <p>
             * Equivalent to @code headSet(toElement, false) @endcode
             * </p>.
             *
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            SortedSet<E> &headSet(const E &toElement) override {
                try {
                    return headSet(toElement, false);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * <p>
             * Equivalent to @code tailSet(fromElement, true) @endcode
             * </p>.
             *
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            const SortedSet<E> &tailSet(const E &fromElement) const override {
                try {
                    return tailSet(fromElement, true);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             *
             * <p>
             * Equivalent to @code tailSet(fromElement, true) @endcode
             * </p>.
             *
             * @throws ClassCastException
             * @throws IllegalArgumentException
             */
            SortedSet<E> &tailSet(const E &fromElement) override {
                try {
                    return tailSet(fromElement, true);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             * <p>
             * This method is equivalent to @b descendingSet.
             * </p>
             * @note
             * The implementation in this interface returns the result of calling the
             * @c descendingSet method.
             * @return a reverse-ordered view of this collection, as a @c NavigableSet
             *
             */
            const NavigableSet &reversed() const override {
                try {
                    return descendingSet();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             *
             * <p>
             * This method is equivalent to @b descendingSet.
             * </p>
             * @note
             * The implementation in this interface returns the result of calling the
             * @c descendingSet method.
             *
             * @return a reverse-ordered view of this collection, as a @c NavigableSet
             *
             */
            NavigableSet &reversed() override {
                try {
                    return descendingSet();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Retrieves and removes the first (lowest) element,
             * or returns @c null if this set is empty.
             *
             * @return the first element, or @c null if this set is empty
             */
             virtual Object &pollFirst() {
                 try {
                     if (isEmpty())
                         return null;
                     return removeFirst();
                 } catch (Throwable const &ex) { ex.throws($ftrace()); }
             }

            /**
             * Retrieves and removes the last (highest) element,
             * or returns @c null if this set is empty.
             *
             * @return the last element, or @c null if this set is empty
             */
             virtual Object &pollLast() {
                try {
                    if (isEmpty())
                        return null;
                    return removeLast();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
             }

            CORE_IMPORT_FIELD_OR_METHOD(SortedSet<E>, isEmpty);
            CORE_IMPORT_FIELD_OR_METHOD(SortedSet<E>, removeFirst);
            CORE_IMPORT_FIELD_OR_METHOD(SortedSet<E>, removeLast);

        private:
            class ReversedOrderView;
            class HeadSet;
            class TailSet;
            class SubSet;
        };


        template<class E>
        class NavigableSet<E>::HeadSet final : public NavigableSet {
            CORE_ALIAS(ITR_CACHE, Class<Object>::Pointer);
            CORE_ALIAS(ITR2_CACHE, Class<Object const>::Pointer);

            NavigableSet &view;
            E &tail;
            gbool inclusive;

            gbool inRange(E const &val) const {
                gint r = view.comparator().compare(val, tail);
                return inclusive ? r <= 0 : r < 0;
            }

        public:
            CORE_EXPLICIT HeadSet(NavigableSet const &view, E &toElement, gbool inclusive)
                : view(CORE_CAST(NavigableSet &, view)), tail(toElement), inclusive(inclusive) {
            }

            gint size() const override {
                try {
                    gint n = 0;
                    for (E const &_: *this)
                        n += 1;

                    return n;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Comparator<E> &comparator() const override {
                return view.comparator();
            }

            const E &lower(const E &e) const override {
                try {
                    if (inRange(e))
                        return view.lower(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &lower(const E &e) override {
                try {
                    if (inRange(e))
                        return view.lower(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &floor(const E &e) const override {
                try {
                    if (inRange(e))
                        return view.floor(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &floor(const E &e) override {
                try {
                    if (inRange(e))
                        return view.floor(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &ceiling(const E &e) const override {
                try {
                    if (inRange(e))
                        return view.ceiling(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &ceiling(const E &e) override {
                try {
                    if (inRange(e))
                        return view.ceiling(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &higher(const E &e) const override {
                try {
                    if (inRange(e))
                        return view.higher(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &higher(const E &e) override {
                try {
                    if (inRange(e))
                        return view.higher(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    HeadSet &headView;
                    Iterator<E> &it;

                    ITR_CACHE mutable cache = null;
                    gbool mutable isEnd = false;

                public:
                    CORE_EXPLICIT Itr(HeadSet &headView, Iterator<E> &it): headView(headView), it(it) {
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

                    void remove() override {
                        try {
                            return it.remove();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
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
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, view.iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                class Itr final : public Iterator2<E> {
                    HeadSet const &headView;
                    Iterator2<E> &it;

                    ITR2_CACHE mutable cache = null;
                    gbool mutable isEnd = false;

                public:
                    CORE_EXPLICIT Itr(HeadSet const &headView, Iterator2<E> &it): headView(headView), it(it) {
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

                    void remove() override {
                        try {
                            return it.remove();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
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
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(NavigableSet const &, view).iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const NavigableSet &subSet(const E &fromElement, gbool fromInclusive,
                                       const E &toElement, gbool toInclusive) const override {
                try {
                    if (inRange(fromElement)) {
                        if (inRange(toElement))
                            return view.subSet(fromElement, fromInclusive, toElement, toInclusive);

                        if (!toInclusive && !inclusive && toElement == tail)
                            return view.subSet(fromElement, fromInclusive, tail, inclusive);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &subSet(const E &fromElement, gbool fromInclusive,
                                 const E &toElement, gbool toInclusive) override {
                try {
                    if (inRange(fromElement)) {
                        if (inRange(toElement))
                            return view.subSet(fromElement, fromInclusive, toElement, toInclusive);

                        if (!toInclusive && !inclusive && toElement == tail)
                            return view.subSet(fromElement, fromInclusive, tail, inclusive);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const NavigableSet &headSet(const E &toElement, gbool inclusive) const override {
                try {
                    if (inRange(toElement))
                        return view.headSet(toElement, inclusive);

                    if (!inclusive && !HeadSet::inclusive && toElement == tail)
                        return *this;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &headSet(const E &toElement, gbool inclusive) override {
                try {
                    if (inRange(toElement))
                        return view.headSet(toElement, inclusive);

                    if (!inclusive && !HeadSet::inclusive && toElement == tail)
                        return *this;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const NavigableSet &tailSet(const E &fromElement, gbool inclusive) const override {
                try {
                    if (inRange(fromElement))
                        return view.subSet(fromElement, inclusive, tail, HeadSet::inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &tailSet(const E &fromElement, gbool inclusive) override {
                try {
                    if (inRange(fromElement))
                        return view.subSet(fromElement, inclusive, tail, HeadSet::inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }
        };

        template<class E>
        class NavigableSet<E>::TailSet final : public NavigableSet {
            CORE_ALIAS(ITR_CACHE, Class<Object>::Pointer);
            CORE_ALIAS(ITR2_CACHE, Class<Object const>::Pointer);

            NavigableSet &view;
            E &head;
            gbool inclusive;

            gbool inRange(E const &val) const {
                gint r = view.comparator().compare(val, head);
                return inclusive ? r >= 0 : r > 0;
            }

        public:
            CORE_EXPLICIT TailSet(NavigableSet &view, E &fromElement, gbool inclusive)
                : view(view), head(fromElement), inclusive(inclusive) {
            }

            gint size() const override {
                try {
                    gint n = 0;
                    for (E const &_: *this)
                        n += 1;

                    return n;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Comparator<E> &comparator() const override {
                return view.comparator();
            }

            const E &lower(const E &e) const override {
                try {
                    if (inRange(e))
                        return view.lower(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &lower(const E &e) override {
                try {
                    if (inRange(e))
                        return view.lower(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &floor(const E &e) const override {
                try {
                    if (inRange(e))
                        return view.floor(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &floor(const E &e) override {
                try {
                    if (inRange(e))
                        return view.floor(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &ceiling(const E &e) const override {
                try {
                    if (inRange(e))
                        return view.ceiling(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &ceiling(const E &e) override {
                try {
                    if (inRange(e))
                        return view.ceiling(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &higher(const E &e) const override {
                try {
                    if (inRange(e))
                        return view.higher(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &higher(const E &e) override {
                try {
                    if (inRange(e))
                        return view.higher(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    TailSet &headView;
                    Iterator<E> &it;

                    ITR_CACHE mutable cache = null;

                public:
                    CORE_EXPLICIT Itr(TailSet &headView, Iterator<E> &it): headView(headView), it(it) {
                    }

                    gbool hasNext() const override {
                        if (cache != null)
                            return true;

                        while (it.hasNext()) {
                            E &e = it.next();

                            if (!headView.inRange(e))
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

                    void remove() override {
                        try {
                            return it.remove();
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Itr>::hasInstance(o))
                            return false;

                        Itr const &other = CORE_XCAST(Itr const, o);
                        if (&headView != &other.headView)
                            return false;

                        return it == other.it;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, view.iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                class Itr final : public Iterator2<E> {
                    TailSet const &headView;
                    Iterator2<E> &it;

                    ITR2_CACHE mutable cache = null;

                public:
                    CORE_EXPLICIT Itr(TailSet const &headView, Iterator2<E> &it): headView(headView), it(it) {
                    }

                    gbool hasNext() const override {
                        if (cache != null)
                            return true;

                        while (it.hasNext()) {
                            E &e = it.next();

                            if (!headView.inRange(e))
                                continue;

                            cache = &e;
                            return true;
                        }

                        return false;
                    }

                    E const & next() override {
                        if (!hasNext())
                            NoSuchElementException().throws($ftrace());

                        E &e = CORE_XCAST(E const, *cache);
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

                        return it == other.it;
                    }
                };

                try {
                    return UNSAFE::newInstance<Itr>(*this, view.iterator());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const NavigableSet &subSet(const E &fromElement, gbool fromInclusive,
                                       const E &toElement, gbool toInclusive) const override {
                try {
                    if (inRange(toElement)) {
                        if (inRange(fromElement))
                            return view.subSet(fromElement, fromInclusive, toElement, toInclusive);

                        if (!fromInclusive && !inclusive && fromElement == head)
                            return view.subSet(head, inclusive, toElement, toInclusive);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &subSet(const E &fromElement, gbool fromInclusive,
                                 const E &toElement, gbool toInclusive) override {
                try {
                    if (inRange(toElement)) {
                        if (inRange(fromElement))
                            return view.subSet(fromElement, fromInclusive, toElement, toInclusive);

                        if (!fromInclusive && !inclusive && fromElement == head)
                            return view.subSet(head, inclusive, toElement, toInclusive);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const NavigableSet &headSet(const E &toElement, gbool inclusive) const override {
                try {
                    if (inRange(toElement))
                        return view.subSet(head, TailSet::inclusive, toElement, inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &headSet(const E &toElement, gbool inclusive) override {
                try {
                    if (inRange(toElement))
                        return view.subSet(head, TailSet::inclusive, toElement, inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const NavigableSet &tailSet(const E &fromElement, gbool inclusive) const override {
                try {
                    if (inRange(fromElement))
                        return view.tailSet(fromElement, inclusive);

                    if (!inclusive && !TailSet::inclusive && fromElement == head)
                        return *this;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &tailSet(const E &fromElement, gbool inclusive) override {
                try {
                    if (inRange(fromElement))
                        return view.tailSet(fromElement, inclusive);

                    if (!inclusive && !TailSet::inclusive && fromElement == head)
                        return *this;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }
        };

        template<class E>
        class NavigableSet<E>::SubSet final : public NavigableSet {
            CORE_ALIAS(ITR_CACHE, Class<Object>::Pointer);
            CORE_ALIAS(ITR2_CACHE, Class<Object const>::Pointer);

            NavigableSet &view;
            E &head;
            E &tail;
            gbool headInclusive;
            gbool tailInclusive;

            gbool aboveHead(E const &val) const {
                gint r = view.comparator().compare(val, head);
                return headInclusive ? r >= 0 : r > 0;
            }

            gbool bellowTail(E const &val) const {
                gint r = view.comparator().compare(val, tail);
                return tailInclusive ? r <= 0 : r > 0;
            }

        public:
            CORE_EXPLICIT SubSet(NavigableSet const &view,
                                 E &fromElement, gbool fromInclusive,
                                 E &toElement, gbool toInclusive)
                : view(CORE_CAST(NavigableSet&, view)),
                  head(fromElement), tail(toElement),
                  headInclusive(fromInclusive), tailInclusive(toInclusive) {
            }

            gint size() const override {
                try {
                    gint n = 0;
                    for (E const &_: *this)
                        n += 1;

                    return n;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Comparator<E> &comparator() const override {
                return view.comparator();
            }

            const E &lower(const E &e) const override {
                try {
                    if (aboveHead(e) && bellowTail(e))
                        return view.lower(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &lower(const E &e) override {
                try {
                    if (aboveHead(e) && bellowTail(e))
                        return view.lower(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &floor(const E &e) const override {
                try {
                    if (aboveHead(e) && bellowTail(e))
                        return view.floor(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &floor(const E &e) override {
                try {
                    if (aboveHead(e) && bellowTail(e))
                        return view.floor(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &ceiling(const E &e) const override {
                try {
                    if (aboveHead(e) && bellowTail(e))
                        return view.ceiling(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &ceiling(const E &e) override {
                try {
                    if (aboveHead(e) && bellowTail(e))
                        return view.ceiling(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            const E &higher(const E &e) const override {
                try {
                    if (aboveHead(e) && bellowTail(e))
                        return view.higher(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
            }

            E &higher(const E &e) override {
                try {
                    if (aboveHead(e) && bellowTail(e))
                        return view.higher(e);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                NoSuchElementException().throws($ftrace());
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
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(NavigableSet&, view).iterator());
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
                    return UNSAFE::newInstance<Itr>(*this, CORE_CAST(NavigableSet const&, view).iterator());
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

            const NavigableSet &subSet(const E &fromElement, gbool fromInclusive,
                                       const E &toElement, gbool toInclusive) const override {
                try {
                    if (aboveHead(fromElement) && aboveHead(toElement) &&
                        bellowTail(fromElement) && bellowTail(toElement) &&
                        comparator().compare(fromElement, toElement) <= 0)
                        return view.subSet(fromElement, fromInclusive, toElement, toInclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &subSet(const E &fromElement, gbool fromInclusive,
                                 const E &toElement, gbool toInclusive) override {
                try {
                    if (aboveHead(fromElement) && aboveHead(toElement) &&
                        bellowTail(fromElement) && bellowTail(toElement) &&
                        comparator().compare(fromElement, toElement) <= 0)
                        return view.subSet(fromElement, fromInclusive, toElement, toInclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const NavigableSet &headSet(const E &toElement, gbool inclusive) const override {
                try {
                    if (aboveHead(toElement) && bellowTail(toElement))
                        return view.subSet(head, headInclusive, toElement, inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &headSet(const E &toElement, gbool inclusive) override {
                try {
                    if (aboveHead(toElement) && bellowTail(toElement))
                        return view.subSet(head, headInclusive, toElement, inclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            const NavigableSet &tailSet(const E &fromElement, gbool inclusive) const override {
                try {
                    if (aboveHead(fromElement) && bellowTail(fromElement))
                        return view.subSet(fromElement, inclusive, tail, tailInclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }

            NavigableSet &tailSet(const E &fromElement, gbool inclusive) override {
                try {
                    if (aboveHead(fromElement) && bellowTail(fromElement))
                        return view.subSet(fromElement, inclusive, tail, tailInclusive);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                IllegalArgumentException().throws($ftrace());
            }
        };

        template<class E>
        class NavigableSet<E>::ReversedOrderView final : public NavigableSet {
            CORE_ALIAS(VIEW_CACHE, typename Class<NavigableSet>::Pointer);
            CORE_ALIAS(VIEW2_CACHE, typename Class<NavigableSet const>::Pointer);
            CORE_ALIAS(ITR_CACHE, Class<Object>::Pointer);
            CORE_ALIAS(ITR2_CACHE, Class<Object const>::Pointer);
            CORE_ALIAS(CPR, typename Class<Comparator<E>>::Pointer);

            NavigableSet &base;
            CPR mutable cpr = {};

        public:
            CORE_EXPLICIT ReversedOrderView(NavigableSet const &base): base(CORE_CAST(NavigableSet &, base)) {
            }

            gint size() const override {
                return base.size();
            }

            const Comparator<E> &comparator() const override {
                if (cpr->reversed() != base.comparator()) {
                    UNSAFE::deleteInstance(*cpr);
                    cpr = &base.comparator().reversed();
                }
                return *cpr;
            }

            const E &lower(const E &e) const override {
                return base.higher(e);
            }

            E &lower(const E &e) override {
                return base.higher(e);
            }

            const E &floor(const E &e) const override {
                return base.ceiling(e);
            }

            E &floor(const E &e) override {
                return base.ceiling(e);
            }

            const E &ceiling(const E &e) const override {
                return base.floor(e);
            }

            E &ceiling(const E &e) override {
                return base.floor(e);
            }

            const E &higher(const E &e) const override {
                return base.lower(e);
            }

            E &higher(const E &e) override {
                return base.lower(e);
            }

            Iterator<E> &iterator() override {
                class Itr final : public Iterator<E> {
                    NavigableSet &set;

                    VIEW_CACHE view = null;
                    ITR_CACHE cache = null;

                public:
                    CORE_EXPLICIT Itr(NavigableSet &set) : set(set) {
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
                            view = &set.headSet(e, false);
                            if (v != null) {
                                UNSAFE::deleteInstance(*v);
                            }

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
                    NavigableSet const &set;

                    VIEW2_CACHE view = null;
                    ITR2_CACHE cache = null;

                public:
                    CORE_EXPLICIT Itr(NavigableSet const &set) : set(set) {
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
                            view = &set.headSet(e, false);
                            if (v != null) {
                                UNSAFE::deleteInstance(*v);
                            }

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

            const E &first() const override {
                return base.last();
            }

            E &first() override {
                return base.last();
            }

            const E &last() const override {
                return base.first();
            }

            E &last() override {
                return base.first();
            }

            E &removeFirst() override {
                return base.removeLast();
            }

            E &removeLast() override {
                return base.removeFirst();
            }

            gbool add(const E &val) override {
                return base.add(val);
            }

            gbool remove(const E &val) override {
                return base.remove(val);
            }

            gbool addAll(const Collection<E> &c) override {
                return base.addAll(c);
            }

            gbool retainAll(const Collection<E> &c) override {
                return base.retainAll(c);
            }

            gbool removeAll(const Collection<E> &c) override {
                return base.removeAll(c);
            }

            void clear() override {
                return base.clear();
            }
        };
    } // util
} // core

#endif //CORE24_NAVIGABLESET_H
