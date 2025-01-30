//
// Created by brunshweeck on 12 juil. 2024.
//

#ifndef CORE24_LINKEDLIST_H
#define CORE24_LINKEDLIST_H

#include <core/util/List.h>

namespace core {
    namespace util {
        /**
         * Doubly-linked list implementation of the @c List
         * interfaces.  Implements all optional list operations, and permits all
         * elements.
         *
         * <p>
         * All the operations perform as could be expected for a doubly-linked
         * list.  Operations that index into the list will traverse the list from
         * the beginning or the end, whichever is closer to the specified index.
         * </p>
         * <p>
         * <strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a linked list concurrently, and at least
         * one of the threads modifies the list structurally, it <i>must</i> be
         * synchronized externally.  (A structural modification is any operation
         * that adds or deletes one or more elements; merely setting the value of
         * an element is not a structural modification.)  This is typically
         * accomplished by synchronizing on some object that naturally
         * encapsulates the list.
         *
         * If no such object exists, the list should be "wrapped" using the
         * @c Collections::synchronizedList method.  This is best done at creation
         * time, to prevent accidental unsynchronized access to the list:
         * @code
         *   auto& list = Collections::synchronizedList(Unsafe::newInstance<LinkedList<>>(...));
         * @endcode
         * </p>
         * <p>
         * The iterators returned by this class's @c iterator and
         * @c listIterator methods are <i>fail-fast</i>: if the list is
         * structurally modified at any time after the iterator is created, in
         * any way except through the Iterator's own @c remove or
         * @c add methods, the iterator will throw a @c ConcurrentModificationException.
         * Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than
         * risking arbitrary, non-deterministic behavior at an undetermined
         * time in the future.
         * </p>
         * <p>
         * Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw @c ConcurrentModificationException on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:   <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         * </p>
         *
         * @see     List
         * @see     ArrayList
         *
         * @tparam E the type of elements held in this collection
         */
        template<class E>
        class LinkedList : public virtual List<E> {
            CORE_ALIAS(ITEM, typename Class<E>::Pointer);

            class Node;

            CORE_ALIAS(LINK, typename Class<Node>::Pointer);
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            /**
             * Number of nodes created.
             */
            gint count = 0;

            /**
             * Pointer to first node.
             */
            LINK first = null;

            /**
             * Pointer to last node.
             */
            LINK last = null;

            CORE_IMPORT_FIELD_OR_METHOD(List<E>, modCount);

        public:
            /**
             * Constructs an empty list.
             */
            LinkedList() CORE_NOTHROW {
                CORE_IGNORE(this);
            }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param  c the collection whose elements are to be placed into this list
             */
            CORE_EXPLICIT LinkedList(Collection<E> const &c): LinkedList() {
                try {
                    LinkedList::addAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            CORE_IMPLICIT LinkedList(LinkedList const &c) : LinkedList() {
                try {
                    addAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            CORE_IMPLICIT LinkedList(LinkedList &&c) CORE_NOTHROW : LinkedList() {
                try {
                    UNSAFE::swapValues(first, c.first);
                    UNSAFE::swapValues(last, c.last);
                    UNSAFE::swapValues(count, c.count);
                    UNSAFE::swapValues(modCount, c.modCount);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            LinkedList &operator=(LinkedList const &c) {
                if (this != &c) {
                    LinkedList copy = c;
                    *this = UNSAFE::moveInstance(copy);
                }
                return *this;
            }

            LinkedList &operator=(LinkedList &&c) CORE_NOTHROW {
                if (this != &c) {
                    try {
                        UNSAFE::swapValues(first, c.first);
                        UNSAFE::swapValues(last, c.last);
                        UNSAFE::swapValues(count, c.count);
                        UNSAFE::swapValues(modCount, c.modCount);
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                }
                return *this;
            }

            ~LinkedList() CORE_NOTHROW override {
                LinkedList::clear();
            }

            /**
             * Returns the first element in this list.
             *
             * @return the first element in this list
             * @throws NoSuchElementException if this list is empty
             */
            E &firstElement() override {
                LINK const f = first;
                if (f == null)
                    NoSuchElementException().throws($ftrace());
                return *f->item;
            }

            /**
             * Returns the first element in this list.
             *
             * @return the first element in this list
             * @throws NoSuchElementException if this list is empty
             */
            const E &firstElement() const override {
                LINK const f = first;
                if (f == null)
                    NoSuchElementException().throws($ftrace());
                return *f->item;
            }

            /**
             * Returns the last element in this list.
             *
             * @return the last element in this list
             * @throws NoSuchElementException if this list is empty
             */
            E &lastElement() override {
                LINK const l = last;
                if (l == null)
                    NoSuchElementException().throws($ftrace());
                return *l->item;
            }

            /**
             * Returns the last element in this list.
             *
             * @return the last element in this list
             * @throws NoSuchElementException if this list is empty
             */
            const E &lastElement() const override {
                LINK const l = last;
                if (l == null)
                    NoSuchElementException().throws($ftrace());
                return *l->item;
            }

            /**
             * Removes and returns the first element from this list.
             *
             * @return the first element from this list
             * @throws NoSuchElementException if this list is empty
             */
            E &removeFirst() override {
                LINK const f = first;
                if (f == null)
                    NoSuchElementException().throws($ftrace());
                return unlinkFirst(f);
            }

            /**
             * Removes and returns the last element from this list.
             *
             * @return the last element from this list
             * @throws NoSuchElementException if this list is empty
             */
            E &removeLast() override {
                LINK const l = last;
                if (l == null)
                    NoSuchElementException().throws($ftrace());
                return unlinkLast(l);
            }

            /**
             * Inserts the specified element at the beginning of this list.
             *
             * @param val the element to add
             */
            void addFirst(const E &val) override {
                try {
                    E &newVal = UNSAFE::copyInstance(val);
                    linkFirst(newVal);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Appends the specified element to the end of this list.
             *
             * <p>This method is equivalent to @c add.
             *
             * @param val the element to add
             */
            void addLast(const E &val) override {
                try {
                    E &newVal = UNSAFE::copyInstance(val);
                    linkLast(newVal);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this list contains the specified element.
             * More formally, returns @c true if and only if this list contains
             * at least one element @c e such that @c Object::equals(o, e).
             *
             * @param val element whose presence in this list is to be tested
             * @return @c true if this list contains the specified element
             */
            gbool contains(const E &val) const override {
                return indexOf(val) >= 0;
            }

            /**
             * Returns the number of elements in this list.
             *
             * @return the number of elements in this list
             */
            gint size() const override {
                return count;
            }

            /**
             * Appends the specified element to the end of this list.
             *
             * <p>This method is equivalent to @c addLast.
             *
             * @param val element to be appended to this list
             * @return @c true (as specified by @c Collection::add)
             */
            gbool add(const E &val) override {
                try {
                    E &newVal = UNSAFE::copyInstance(val);
                    linkLast(newVal);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return true;
            }

            /**
             * Removes the first occurrence of the specified element from this list,
             * if it is present.  If this list does not contain the element, it is
             * unchanged.  More formally, removes the element with the lowest index
             * @c i such that @c Objects.equals(o, get(i))
             * (if such an element exists).  Returns @c true if this list
             * contained the specified element (or equivalently, if this list
             * changed as a result of the call).
             *
             * @param val element to be removed from this list, if present
             * @return @c true if this list contained the specified element
             */
            gbool remove(const E &val) override {
                for (LINK x = first; x->next != null; x = x->next)
                    if (val == *x->item) {
                        unlink(x);
                        return true;
                    }
                return false;
            }

            /**
             * Appends all the elements in the specified collection to the end of
             * this list, in the order that they are returned by the specified
             * collection's iterator.  The behavior of this operation is undefined if
             * the specified collection is modified while the operation is in
             * progress.  (Note that this will occur if the specified collection is
             * this list, and it's nonempty.)
             *
             * @param c collection containing elements to be added to this list
             * @return @c true if this list changed as a result of the call
             */
            gbool addAll(const Collection<E> &c) override {
                try {
                    return addAll(count, c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts all the elements in the specified collection into this
             * list, starting at the specified position.  Shifts the element
             * currently at that position (if any) and any subsequent elements to
             * the right (increases their indices).  The new elements will appear
             * in the list in the order that they are returned by the
             * specified collection's iterator.
             *
             * @param index index at which to insert the first element
             *              from the specified collection
             * @param c collection containing elements to be added to this list
             * @return @c true if this list changed as a result of the call
             * @throws IndexOutOfBoundsException
             */
            gbool addAll(gint index, const Collection<E> &c) override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    Array<E> a = c.toArray();
                    gint const size = a.length();
                    if (size == 0)
                        return false;
                    LINK pred = null;
                    LINK succ = null;
                    if (index == size) {
                        succ = null;
                        pred = last;
                    } else {
                        succ = nodeAt(index);
                        pred = succ->prev;
                    }
                    for (E &val: a) {
                        LINK newNode = new Node(pred, val, succ);
                        if (pred == null) { first = newNode; } else { pred->next = newNode; }
                        pred = newNode;
                    }
                    if (succ == null)
                        last = pred;
                    else {
                        pred->next = last;
                        succ->prev = pred;
                    }
                    count += size;
                    modCount += 1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return true;
            }

            /**
             * Removes all the elements from this list.
             * The list will be empty after this call returns.
             */
            void clear() override {
                // Clearing all the links between nodes is "unnecessary", but:
                // - helps a generational GC if the discarded nodes inhabit
                //   more than one generation
                // - is sure to free memory even if there is a reachable Iterator

                try {
                    for (LINK x = first; x != null;) {
                        LINK next = x->next;
                        x->item = null;
                        x->next = null;
                        x->prev = null;
                        delete x;
                        x = next;
                    }
                    first = last = null;
                    count = 0;
                    modCount += 1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // Positional Access Operations

            /**
             * Returns the element at the specified position in this list.
             *
             * @param index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexOutOfBoundsException
             */
            E &get(gint index) override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    return *nodeAt(index)->item;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the element at the specified position in this list.
             *
             * @param index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexOutOfBoundsException
             */
            const E &get(gint index) const override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    return *nodeAt(index)->item;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Replaces the element at the specified position in this list with the
             * specified element.
             *
             * @param index index of the element to replace
             * @param newValue element to be stored at the specified position
             * @return the element previously at the specified position
             * @throws IndexOutOfBoundsException
             */
            E &set(gint index, const E &newValue) override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    LINK const x = nodeAt(index);
                    ITEM item = x->item;
                    E &newVal = UNSAFE::copyInstance(newValue);
                    x->item = &newVal;
                    return *item;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts the specified element at the specified position in this list.
             * Shifts the element currently at that position (if any) and any
             * subsequent elements to the right (adds one to their indices).
             *
             * @param index index at which the specified element is to be inserted
             * @param val element to be inserted
             * @throws IndexOutOfBoundsException
             */
            void add(gint index, const E &val) override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    LINK const x = nodeAt(index);
                    E &newVal = UNSAFE::copyInstance(val);
                    if (index == count) linkLast(newVal);
                    else linkBefore(newVal, x);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the element at the specified position in this list.  Shifts any
             * subsequent elements to the left (subtracts one from their indices).
             * Returns the element that was removed from the list.
             *
             * @param index the index of the element to be removed
             * @return the element previously at the specified position
             * @throws IndexOutOfBoundsException
             */
            E &remove(gint index) override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    return unlink(nodeAt(index));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // Search Operations

            /**
             * Returns the index of the first occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the lowest index @c i such that
             * @c Objects.equals(o, get(i)),
             * or -1 if there is no such index.
             *
             * @param val element to search for
             * @return the index of the first occurrence of the specified element in
             *         this list, or -1 if this list does not contain the element
             */
            gint indexOf(const E &val) const override {
                gint index = 0;
                for (LINK x = first; x != null; x = x->next) {
                    if (val == *x->item)
                        return index;
                    index += 1;
                }
                return -1;
            }

            gint lastIndexOf(const E &val) const override {
                gint index = count;
                for (LINK x = last; x != null; x = x->prev) {
                    index -= 1;
                    if (val == *x->item)
                        return index;
                }
                return -1;
            }

            /**
             * Returns a list-iterator of the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * Obeys the general contract of @c List::listIterator(int).<p>
             *
             * The list-iterator is <i>fail-fast</i>: if the list is structurally
             * modified at any time after the Iterator is created, in any way except
             * through the list-iterator's own @c remove or @c add
             * methods, the list-iterator will throw a
             * @c ConcurrentModificationException.  Thus, in the face of
             * concurrent modification, the iterator fails quickly and cleanly, rather
             * than risking arbitrary, non-deterministic behavior at an undetermined
             * time in the future.
             *
             * @param index index of the first element to be returned from the
             *              list-iterator (by a call to @c next)
             * @return a ListIterator of the elements in this list (in proper
             *         sequence), starting at the specified position in the list
             * @throws IndexOutOfBoundsException
             * @see List::listIterator(int)
             */
            ListIterator<E> &listIterator(gint index) override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    return UNSAFE::newInstance<AscListItr>(*this, index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a list-iterator of the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * Obeys the general contract of @c List::listIterator(int).<p>
             *
             * The list-iterator is <i>fail-fast</i>: if the list is structurally
             * modified at any time after the Iterator is created, in any way except
             * through the list-iterator's own @c remove or @c add
             * methods, the list-iterator will throw a
             * @c ConcurrentModificationException.  Thus, in the face of
             * concurrent modification, the iterator fails quickly and cleanly, rather
             * than risking arbitrary, non-deterministic behavior at an undetermined
             * time in the future.
             *
             * @param index the index of the first element to be returned from the
             *              list-iterator (by a call to @c next)
             * @return a ListIterator of the elements in this list (in proper
             *         sequence), starting at the specified position in the list
             * @throws IndexOutOfBoundsException
             * @see List::listIterator(int)
             */
            ListIterator2<E> &listIterator(gint index) const override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    return UNSAFE::newInstance<AscListItr2>(*this, index);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a shallow copy of this @c LinkedList. (The elements
             * themselves are not cloned.)
             *
             * @return a shallow copy of this @c LinkedList instance
             */
            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<LinkedList>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an array containing all the elements in this list
             * in proper sequence (from first to last element).
             *
             * <p>The returned array will be "safe" in that no references to it are
             * maintained by this list.  (In other words, this method must allocate
             * a new array).  The caller is thus free to modify the returned array.
             *
             * <p>This method acts as bridge between array-based and collection-based
             * APIs.
             *
             * @return an array containing all the elements in this list
             *         in proper sequence
             */
            Array<E> toArray() const override {
                try {
                    if (count == 0)
                        return {};

                    Array<E> a = Array<E>(count);
                    gint i = 0;
                    for (LINK x = first; x != null; x = x->next)
                        a.set(i++, *x->item);
                    return a;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator<E> &iterator() override {
                return listIterator();
            }

            Iterator2<E> &iterator() const override {
                return listIterator();
            }

            ListIterator<E> &listIterator() override {
                return listIterator(0);
            }

            ListIterator2<E> &listIterator() const override {
                return listIterator(0);
            }

        private:
            class Node final {
            public:
                ITEM item;
                LINK next;
                LINK prev;

                CORE_EXPLICIT Node(LINK prev, E &item, LINK next): item(&item), next(next), prev(prev) {
                }
            };

            /**
             * Links e as first element.
             */
            void linkFirst(E &val) {
                try {
                    LINK const f = first;
                    LINK const newNode = new Node(null, val, f);
                    first = newNode;
                    if (f == null) { last = newNode; } else { f->prev = newNode; }
                    count += 1;
                    modCount += 1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Links e as last element.
             */
            void linkLast(E &val) {
                try {
                    LINK const l = last;
                    LINK const newNode = new Node(l, val, null);
                    last = newNode;
                    if (l == null) { first = newNode; } else { l->next = newNode; }
                    count += 1;
                    modCount += 1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts element e before non-null Node succ.
             */
            void linkBefore(E &val, LINK succ) {
                CORE_ASSERT(succ != null);

                try {
                    LINK const pred = succ->prev;
                    LINK newNode = new Node(pred, val, succ);
                    succ->prev = newNode;
                    if (pred == null) { first = newNode; } else { pred->next = newNode; }
                    count += 1;
                    modCount += 1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Unlinks non-null first node f.
             */
            E &unlinkFirst(LINK f) {
                CORE_ASSERT(f == first && f != null);

                try {
                    ITEM const item = f->item;
                    LINK const next = f->next;
                    f->item = null;
                    f->next = null;
                    first = next;
                    if (next == null) { last = null; } else { next->prev = null; }
                    count -= 1;
                    modCount += 1;
                    delete f;
                    return *item;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Unlinks non-null last node l.
             */
            E &unlinkLast(LINK l) {
                CORE_ASSERT(l == last && l != null);

                try {
                    ITEM const item = l->item;
                    LINK const prev = l->prev;
                    l->item = null;
                    l->prev = null;
                    if (prev == null) { first = null; } else { prev->next = null; }
                    count -= 1;
                    modCount += 1;
                    delete l;
                    return *item;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Unlinks non-null node x.
             */
            E &unlink(LINK x) {
                CORE_ASSERT(x != null);

                try {
                    ITEM const item = x->item;
                    LINK const prev = x->prev;
                    LINK const next = x->next;
                    x->item = null;
                    x->prev = null;
                    x->next = null;
                    if (prev == null) { first = next; } else { prev->next = next; }
                    if (next == null) { last = prev; } else { next->prev = prev; }
                    count -= 1;
                    modCount += 1;
                    delete x;
                    return *item;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the (non-null) Node at the specified element index.
             */
            LINK nodeAt(gint index) const {
                if (index < 0 || index >= count)
                    return null;
                LINK x;
                if (index < (count >> 1)) {
                    x = first;
                    for (int i = 0; i < index; ++i)
                        x = x->next;
                } else {
                    x = last;
                    for (int i = count - 1; i > index; --i)
                        x = x->prev;
                }
                return x;
            }

            class AscListItr final : public ListIterator<E> {
                LinkedList &list;
                gint cursor = 0;
                LINK nextLink = null;
                LINK lastLink = null;
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT AscListItr(LinkedList &list, gint index) : list(list),
                                                                         cursor(index),
                                                                         nextLink(list.nodeAt(index)),
                                                                         lastModCount(list.modCount) {
                }

                gbool hasNext() const override {
                    return cursor < list.count;
                }

                E &next() override {
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().
                                throws($ftrace());
                    if (!hasNext())
                        NoSuchElementException().throws($ftrace());
                    lastLink = nextLink;
                    nextLink = nextLink->next;
                    cursor += 1;
                    return *lastLink->item;
                }

                gbool hasPrevious() const override {
                    return cursor > 0;
                }

                E &previous() override {
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().
                                throws($ftrace());
                    if (!hasPrevious())
                        NoSuchElementException().throws($ftrace());
                    lastLink = nextLink = (nextLink == null) ? list.last : nextLink->prev;
                    cursor -= 1;
                    return *lastLink->item;
                }

                gint nextIndex() const override {
                    return cursor;
                }

                gint previousIndex() const override {
                    return cursor - 1;
                }

                void remove() override {
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().
                                throws($ftrace());
                    if (lastLink == null)
                        IllegalStateException().throws($ftrace());
                    LINK const x = lastLink->next;
                    list.unlink(lastLink);
                    if (nextLink == lastLink)
                        nextLink = x;
                    else
                        cursor -= 1;
                    lastLink = null;
                    lastModCount += 1;
                }

                void set(const E &e) override {
                    if (lastLink == null)
                        IllegalStateException().throws($ftrace());
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().
                                throws($ftrace());
                    E &newVal = UNSAFE::copyInstance(e);
                    lastLink->item = &newVal;
                }

                void add(const E &e) override {
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().
                                throws($ftrace());
                    lastLink = null;
                    E &newVal = UNSAFE::copyInstance(e);
                    if (nextLink == null)
                        list.linkLast(newVal);
                    else
                        list.linkBefore(newVal, nextLink);
                    cursor += 1;
                    lastModCount += 1;
                }

                void forEach(const function::Consumer<E &> &action) override {
                    try {
                        while (lastModCount == list.modCount && cursor < list.count) {
                            action.accept(*nextLink->item);
                            lastLink = nextLink;
                            nextLink = nextLink->next;
                            cursor += 1;
                        }
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().
                                throws($ftrace());
                }

                gbool equals(const Object &obj) const override {
                    if (this == &obj)
                        return true;
                    if (Class<AscListItr>::hasInstance(obj)) {
                        AscListItr const &it = CORE_XCAST(AscListItr const, obj);
                        return &list == &it.list && cursor == it.cursor;
                    }
                    return false;
                }

                ~AscListItr() override {
                    cursor = 0;
                    nextLink = lastLink = null;
                }
            };

            class AscListItr2 final : public ListIterator2<E> {
                LinkedList const &list;
                gint cursor = 0;
                LINK nextLink = null;
                LINK lastLink = null;
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT AscListItr2(LinkedList const &list, gint index) : list(list),
                    cursor(index),
                    nextLink(list.nodeAt(index)),
                    lastModCount(list.modCount) {
                }

                gbool hasNext() const override {
                    return cursor < list.count;
                }

                E const &next() override {
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().throws(
                            $ftrace());
                    if (!hasNext())
                        NoSuchElementException().throws($ftrace());
                    lastLink = nextLink;
                    nextLink = nextLink->next;
                    cursor += 1;
                    return *lastLink->item;
                }

                gbool hasPrevious() const override {
                    return cursor > 0;
                }

                E const &previous() override {
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().throws(
                            $ftrace());
                    if (!hasPrevious())
                        NoSuchElementException().throws($ftrace());
                    lastLink = nextLink = (nextLink == null) ? list.last : nextLink->prev;
                    cursor -= 1;
                    return *lastLink->item;
                }

                gint nextIndex() const override {
                    return cursor;
                }

                gint previousIndex() const override {
                    return cursor - 1;
                }

                void forEach(const function::Consumer<E> &action) override {
                    try {
                        while (lastModCount == list.modCount && cursor < list.count) {
                            action.accept(*nextLink->item);
                            lastLink = nextLink;
                            nextLink = nextLink->next;
                            cursor += 1;
                        }
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    if (lastModCount != list.modCount)
                        ConcurrentModificationException().throws(
                            $ftrace());
                }

                gbool equals(const Object &obj) const override {
                    if (this == &obj)
                        return true;
                    if (Class<AscListItr2>::hasInstance(obj)) {
                        AscListItr2 const &it = CORE_XCAST(AscListItr2 const, obj);
                        return &list == &it.list && cursor == it.cursor;
                    }
                    return false;
                }

                ~AscListItr2() override {
                    cursor = 0;
                    nextLink = lastLink = null;
                }
            };
        };
    } // util
} // core


#endif //CORE24_LINKEDLIST_H
