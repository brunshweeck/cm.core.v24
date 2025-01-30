//
// Created by brunshweeck on 20 juil. 2024.
//

#ifndef CORE24_LINKEDDEQUE_H
#define CORE24_LINKEDDEQUE_H

#include <core/util/Deque.h>
#include <core/util/ConcurrentModificationException.h>

namespace core {
    namespace util {
        /**
         * Doubly-linked list implementation of the @c Deque interfaces.
         * Implements all optional deque operations, and permits all elements.
         *
         * <p>
         * All the operations perform as could be expected for a doubly-linked
         * list.
         * </p>
         * <p>
         * <strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access a linked deque concurrently, and at least
         * one of the threads modifies the deque structurally, it <i>must</i> be
         * synchronized externally.  (A structural modification is any operation
         * that adds or deletes one or more elements; merely setting the value of
         * an element is not a structural modification.)  This is typically
         * accomplished by synchronizing on some object that naturally
         * encapsulates the deque.
         *
         * If no such object exists, the deque should be "wrapped" using the
         * @c Collections::synchronizedDeque method.  This is best done at creation
         * time, to prevent accidental unsynchronized access to the deque:
         * @code
         *   auto& deque = Collections::synchronizedDeque(Unsafe::newInstance<LinkedDeque<>>(...));
         * @endcode
         * </p>
         * <p>
         * The iterators returned by this class's @c iterator method is <i>fail-fast</i>:
         * if the deque is structurally modified at any time after the iterator is created, in
         * any way except through the Iterator's own @c remove method, the iterator will throw
         * a @c ConcurrentModificationException.
         * Thus, in the face of concurrent modification, the iterator fails quickly and cleanly,
         * rather than risking arbitrary, non-deterministic behavior at an undetermined
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
         * @see     Deque
         * @see     ArrayDeque
         *
         * @tparam E the type of elements held in this collection
         */
        template<class E>
        class LinkedDeque : public virtual Deque<E> {
            CORE_ALIAS(ITEM, typename Class<E>::Pointer);

            class Node;

            CORE_ALIAS(LINK, Node *);
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            /**
             * Number of nodes created.
             */
            gint count = 0;

            /**
             * Pointer to first node.
             */
            LINK head = null;

            /**
             * Pointer to last node.
             */
            LINK tail = null;

            gint modCount = 0;

        public:
            /**
             * Construct empty deque
             */
            CORE_IMPLICIT LinkedDeque() = default;

            /**
             * Constructs a deque containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param  c the collection whose elements are to be placed into this deque
             */
            CORE_EXPLICIT LinkedDeque(Collection<E> const &c): LinkedDeque() {
                try {
                    LinkedDeque::addAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            CORE_IMPLICIT LinkedDeque(LinkedDeque const &c) : LinkedDeque() {
                try {
                    addAll(c);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            CORE_IMPLICIT LinkedDeque(LinkedDeque &&c) CORE_NOTHROW : LinkedDeque() {
                UNSAFE::swapValues(head, c.head);
                UNSAFE::swapValues(tail, c.tail);
                UNSAFE::swapValues(count, c.count);
                UNSAFE::swapValues(modCount, c.modCount);
            }

            LinkedDeque &operator=(LinkedDeque const &c) {
                try {
                    if (this != &c) {
                        LinkedDeque copy = c;
                        *this = UNSAFE::moveInstance(copy);
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return *this;
            }

            LinkedDeque &operator=(LinkedDeque &&c) CORE_NOTHROW {
                if (this != &c) {
                    UNSAFE::swapValues(head, c.head);
                    UNSAFE::swapValues(tail, c.tail);
                    UNSAFE::swapValues(count, c.count);
                    UNSAFE::swapValues(modCount, c.modCount);
                }
                return *this;
            }

            ~LinkedDeque() CORE_NOTHROW override {
                try {
                    LinkedDeque::clear();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the first element in this deque.
             *
             * @return the first element in this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &firstElement() override {
                LINK f = head;
                if (f == null)
                    NoSuchElementException().throws($ftrace());

                return *f->item;
            }

            /**
             * Returns the first element in this deque.
             *
             * @return the first element in this deque
             * @throws NoSuchElementException if this deque is empty
             */
            const E &firstElement() const override {
                LINK f = head;
                if (f == null)
                    NoSuchElementException().throws($ftrace());

                return *f->item;
            }

            /**
             * Returns the last element in this deque.
             *
             * @return the last element in this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &lastElement() override {
                LINK l = tail;
                if (l == null)
                    NoSuchElementException().throws($ftrace());

                return *l->item;
            }

            /**
             * Returns the last element in this deque.
             *
             * @return the last element in this deque
             * @throws NoSuchElementException if this deque is empty
             */
            const E &lastElement() const override {
                LINK l = tail;
                if (l == null)
                    NoSuchElementException().throws($ftrace());

                return *l->item;
            }

            /**
             * Removes and returns the first element from this deque.
             *
             * @return the first element from this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &removeFirst() override {
                LINK f = head;
                if (f == null)
                    NoSuchElementException().throws($ftrace());

                return unlinkFirst(f);
            }

            /**
             * Removes and returns the last element from this deque.
             *
             * @return the last element from this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &removeLast() override {
                LINK l = tail;
                if (l == null)
                    NoSuchElementException().throws($ftrace());

                return unlinkLast(l);
            }

            /**
             * Inserts the specified element at the beginning of this deque.
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
             * Appends the specified element to the end of this deque.
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
                try {
                    for (LINK x = head; x != null; x = x->next)
                        if (val == *x->item)
                            return true;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return false;
            }

            /**
             * Returns the number of elements in this list.
             *
             * @return the number of elements in this list
             */
            gint size() const override { return count; }

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
                for (LINK x = head; x->next != null; x = x->next)
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
                    Array<E> a = c.toArray();
                    for (auto &val: a) linkLast(val);
                    return a.length() > 0;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
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
                    for (LINK x = head; x != null;) {
                        LINK next = x->next;
                        x->item = null;
                        x->next = null;
                        x->prev = null;
                        delete x;
                        x = next;
                    }

                    head = tail = null;
                    count = 0;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // Queue operations.

            /**
             * Retrieves, but does not remove, the head (first element) of this list.
             *
             * @return the head of this list, or @c null if this list is empty
             */
            Object &peek() override {
                LINK f = head;
                return (f == null) ? CORE_XCAST(Object, null) : *f->item;
            }

            /**
             * Retrieves, but does not remove, the head (first element) of this list.
             *
             * @return the head of this list, or @c null if this list is empty
             */
            Object const &peek() const override {
                LINK f = head;
                return (f == null) ? CORE_XCAST(Object, null) : *f->item;
            }

            /**
             * Retrieves, but does not remove, the head (first element) of this list.
             *
             * @return the head of this list
             * @throws NoSuchElementException if this list is empty
             */
            E &element() override {
                LINK f = head;
                if (f == null)
                    NoSuchElementException().throws($ftrace());

                return *f->item;
            }

            /**
             * Retrieves, but does not remove, the head (first element) of this list.
             *
             * @return the head of this list
             * @throws NoSuchElementException if this list is empty
             */
            E const &element() const override {
                LINK f = head;
                if (f == null)
                    NoSuchElementException().throws($ftrace());

                return *f->item;
            }

            /**
             * Retrieves and removes the head (first element) of this list.
             *
             * @return the head of this list, or @c null if this list is empty
             */
            Object &poll() override {
                LINK f = head;
                return (f == null) ? CORE_XCAST(Object, null) : unlinkFirst(f);
            }

            /**
             * Retrieves and removes the head (first element) of this list.
             *
             * @return the head of this list
             * @throws NoSuchElementException if this list is empty
             */
            E &remove() override {
                try {
                    return removeFirst();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Adds the specified element as the tail (last element) of this list.
             *
             * @param val the element to add
             * @return @c true (as specified by @c Queue::offer)
             */
            gbool offer(E const &val) override {
                try {
                    return add(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // Deque operations

            /**
             * Inserts the specified element at the front of this list.
             *
             * @param val the element to insert
             * @return @c true (as specified by @c Deque::offerFirst)
             */
            gbool offerFirst(E const &val) override {
                try {
                    addFirst(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return true;
            }

            /**
             * Inserts the specified element at the end of this list.
             *
             * @param val the element to insert
             * @return @c true (as specified by @c Deque::offerLast)
             */
            gbool offerLast(E const &val) override {
                try {
                    addLast(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return true;
            }

            /**
             * Retrieves, but does not remove, the first element of this list,
             * or returns @c null if this list is empty.
             *
             * @return the first element of this list, or @c null
             *         if this list is empty
             */
            Object &peekFirst() override {
                LINK f = head;
                return (f == null) ? CORE_XCAST(Object, null) : *f->item;
            }

            /**
             * Retrieves, but does not remove, the first element of this list,
             * or returns @c null if this list is empty.
             *
             * @return the first element of this list, or @c null
             *         if this list is empty
             */
            Object const &peekFirst() const override {
                LINK f = head;
                return (f == null) ? CORE_XCAST(Object, null) : *f->item;
            }

            /**
             * Retrieves, but does not remove, the last element of this list,
             * or returns @c null if this list is empty.
             *
             * @return the last element of this list, or @c null
             *         if this list is empty
             */
            Object &peekLast() override {
                LINK l = tail;
                return (l == null) ? CORE_XCAST(Object, null) : *l->item;
            }

            /**
             * Retrieves, but does not remove, the last element of this list,
             * or returns @c null if this list is empty.
             *
             * @return the last element of this list, or @c null
             *         if this list is empty
             */
            Object const &peekLast() const override {
                LINK l = tail;
                return (l == null) ? CORE_XCAST(Object, null) : *l->item;
            }

            /**
             * Retrieves and removes the first element of this list,
             * or returns @c null if this list is empty.
             *
             * @return the first element of this list, or @c null if
             *     this list is empty
             */
            Object &pollFirst() override {
                LINK f = head;
                return (f == null) ? CORE_XCAST(Object, null) : unlinkFirst(f);
            }

            /**
             * Retrieves and removes the last element of this list,
             * or returns @c null if this list is empty.
             *
             * @return the last element of this list, or @c null if
             *     this list is empty
             */
            Object &pollLast() override {
                LINK l = tail;
                return (l == null) ? CORE_XCAST(Object, null) : unlinkLast(l);
            }

            /**
             * Pushes an element onto the stack represented by this list.  In other
             * words, inserts the element at the front of this list.
             *
             * <p>This method is equivalent to @c addFirst.
             *
             * @param val the element to push
             */
            void push(const E &val) override {
                try {
                    addFirst(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Pops an element from the stack represented by this list.  In other
             * words, removes and returns the first element of this list.
             *
             * <p>This method is equivalent to @c removeFirst().
             *
             * @return the element at the front of this list (which is the top
             *         of the stack represented by this list)
             * @throws NoSuchElementException if this list is empty
             */
            E &pop() override {
                try {
                    return removeFirst();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the first occurrence of the specified element in this
             * list (when traversing the list from head to tail).  If the list
             * does not contain the element, it is unchanged.
             *
             * @param val element to be removed from this list, if present
             * @return @c true if the list contained the specified element
             */
            gbool removeFirstOccurrence(E const &val) override {
                try {
                    return remove(val);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the last occurrence of the specified element in this
             * list (when traversing the list from head to tail).  If the list
             * does not contain the element, it is unchanged.
             *
             * @param val element to be removed from this list, if present
             * @return @c true if the list contained the specified element
             */
            gbool removeLastOccurrence(E const &val) override {
                try {
                    for (LINK x = tail; x != null; x = x->prev)
                        if (val == *x->item) {
                            unlink(x);
                            return true;
                        }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return false;
            }

            Iterator<E> &iterator() override {
                try {
                    return UNSAFE::newInstance<AscendingIterator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                try {
                    return UNSAFE::newInstance<AscendingIterator2>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator<E> &descendingIterator() override {
                try {
                    return UNSAFE::newInstance<DescendingIterator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &descendingIterator() const override {
                try {
                    return UNSAFE::newInstance<DescendingIterator2>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a shallow copy of this @c LinkedDeque. (The elements
             * themselves are not cloned.)
             *
             * @return a shallow copy of this @c LinkedDeque instance
             */
            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<LinkedDeque>(*this);
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
                    for (LINK x = head; x != null; x = x->next)
                        Arrays::fastSet(a, i++, *x->item);

                    return a;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void forEach(const function::Consumer<E &> &action) override {
                gint lastModCount = modCount;
                try {
                    for (LINK x = head; x != null && lastModCount == modCount; x = x->next)
                        action.accept(*x->item);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                if (lastModCount != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void forEach(const function::Consumer<E> &action) const override {
                gint lastModCount = modCount;
                try {
                    for (LINK x = head; x != null && lastModCount == modCount; x = x->next)
                        action.accept(*x->item);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                if (lastModCount != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            gbool equals(const Object &o) const override {
                if (this == &o)
                    return true;

                if (!Class<Deque<E> >::hasInstance(o))
                    return false;

                Deque<E> const &deque = CORE_XCAST(Deque<E> const, o);
                if (deque.size() != count)
                    return false;

                LINK x = head;
                gint lastModCount = modCount;
                for (E const &val: deque) {
                    if (val != *x->item)
                        return false;

                    if (lastModCount != modCount)
                        ConcurrentModificationException().throws($ftrace());

                    x = x->next;
                }
                return true;
            }

            gbool removeAll(const Collection<E> &c) override {
                try {
                    gbool modified = false;
                    for (LINK x = head; x != null; x = x->next)
                        if (c.contains(*x->item)) {
                            unlink(x);
                            modified = true;
                        }
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(const function::Predicate<E> &filter) override {
                try {
                    gbool modified = false;
                    for (LINK x = head; x != null; x = x->next)
                        if (filter.test(*x->item)) {
                            unlink(x);
                            modified = true;
                        }
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(const Collection<E> &c) override {
                try {
                    gbool modified = false;
                    for (LINK x = head; x != null; x = x->next)
                        if (!c.contains(*x->item)) {
                            unlink(x);
                            modified = true;
                        }
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Node final : public Object {
            public:
                ITEM item = {};
                LINK next = {};
                LINK prev = {};

                CORE_EXPLICIT Node(LINK prev, E &item, LINK next)
                    : item(&item), next(next), prev(prev) {
                }
            };

            /**
             * Links e as first element.
             */
            void linkFirst(E &val) {
                try {
                    LINK f = head;
                    LINK newNode = new Node(null, val, f);
                    head = newNode;
                    if (f == null)
                        tail = newNode;
                    else
                        f->prev = newNode;

                    count += 1;
                    modCount += 1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Links e as last element.
             */
            void linkLast(E &val) {
                try {
                    LINK l = tail;
                    LINK newNode = new Node(l, val, null);
                    tail = newNode;
                    if (l == null)
                        head = newNode;
                    else
                        l->next = newNode;

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
                    LINK pred = succ->prev;
                    LINK newNode = new Node(pred, val, succ);
                    succ->prev = newNode;
                    if (pred == null)
                        head = newNode;
                    else
                        pred->next = newNode;

                    count += 1;
                    modCount += 1;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Unlinks non-null first node f.
             */
            E &unlinkFirst(LINK f) {
                CORE_ASSERT(f == head && f != null);
                try {
                    ITEM const item = f->item;
                    LINK next = f->next;
                    f->item = null;
                    f->next = null;
                    head = next;
                    if (next == null)
                        tail = null;
                    else
                        next->prev = null;

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
                CORE_ASSERT(l == tail && l != null);
                try {
                    ITEM const item = l->item;
                    LINK prev = l->prev;
                    l->item = null;
                    l->prev = null;
                    if (prev == null)
                        head = null;
                    else
                        prev->next = null;

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
                    LINK prev = x->prev;
                    LINK next = x->next;
                    x->item = null;
                    x->prev = null;
                    x->next = null;
                    if (prev == null)
                        head = next;
                    else
                        prev->next = next;

                    if (next == null)
                        tail = prev;
                    else
                        next->prev = prev;

                    count -= 1;
                    modCount += 1;
                    delete x;
                    return *item;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            class AscendingIterator final : public Iterator<E> {
                LinkedDeque &deque;
                LINK nextLink = null;
                LINK lastLink = null;
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT AscendingIterator(LinkedDeque &deque): deque(deque), nextLink(deque.head),
                                                                     lastModCount(deque.modCount) {
                }

                ~AscendingIterator() override { nextLink = lastLink = null; }

                gbool hasNext() const override { return nextLink != null; }

                E &next() override {
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    if (nextLink == null)
                        NoSuchElementException().throws($ftrace());
                    lastLink = nextLink;
                    nextLink = nextLink->next;
                    return *lastLink->item;
                }

                void remove() override {
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    if (lastLink == null)
                        IllegalStateException().throws($ftrace());
                    LINK x = lastLink;
                    lastLink = null;
                    if (nextLink == x)
                        nextLink = x->next;
                    deque.unlink(x);
                    lastModCount += 1;
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (Class<AscendingIterator>::hasInstance(o)) {
                        AscendingIterator const &it = CORE_XCAST(AscendingIterator const, o);
                        return &deque == &it.deque && nextLink == it.nextLink;
                    }
                    return false;
                }

                void forEach(const function::Consumer<E &> &action) override {
                    try {
                        for (; nextLink != null && lastModCount == deque.modCount; nextLink = nextLink->next)
                            action.accept(*(lastLink = nextLink)->item);
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                }
            };

            class AscendingIterator2 final : public Iterator2<E> {
                LinkedDeque const &deque;
                LINK nextLink = null;
                LINK lastLink = null;
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT AscendingIterator2(LinkedDeque const &deque): deque(deque), nextLink(deque.head),
                                                                            lastModCount(deque.modCount) {
                }

                ~AscendingIterator2() override { nextLink = lastLink = null; }

                gbool hasNext() const override { return nextLink != null; }

                E &next() override {
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());

                    if (nextLink == null)
                        NoSuchElementException().throws($ftrace());

                    lastLink = nextLink;
                    nextLink = nextLink->next;
                    return *lastLink->item;
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (Class<AscendingIterator2>::hasInstance(o)) {
                        AscendingIterator2 const &it = CORE_XCAST(AscendingIterator2 const, o);
                        return &deque == &it.deque && nextLink == it.nextLink;
                    }
                    return false;
                }

                void forEach(const function::Consumer<E> &action) override {
                    try {
                        for (; nextLink != null && lastModCount == deque.modCount; nextLink = nextLink->next)
                            action.accept(*(lastLink = nextLink)->item);
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                }
            };

            class DescendingIterator final : public Iterator<E> {
                LinkedDeque &deque;
                LINK prevLink = null;
                LINK lastLink = null;
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT DescendingIterator(LinkedDeque &deque): deque(deque), prevLink(deque.tail),
                                                                      lastModCount(deque.modCount) {
                }

                ~DescendingIterator() override {
                    prevLink = lastLink = null;
                }

                gbool hasNext() const override {
                    return prevLink != null;
                }

                E &next() override {
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    if (prevLink == null)
                        NoSuchElementException().throws($ftrace());
                    lastLink = prevLink;
                    prevLink = prevLink->prev;
                    return *lastLink->item;
                }

                void remove() override {
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    if (lastLink == null)
                        IllegalStateException().throws($ftrace());
                    LINK x = lastLink;
                    lastLink = null;
                    if (prevLink == x)
                        prevLink = x->next;
                    deque.unlink(x);
                    lastModCount += 1;
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (Class<DescendingIterator>::hasInstance(o)) {
                        DescendingIterator const &it = CORE_XCAST(DescendingIterator const, o);
                        return &deque == &it.deque && prevLink == it.prevLink;
                    }
                    return false;
                }

                void forEach(const function::Consumer<E &> &action) override {
                    try {
                        for (; prevLink != null && lastModCount == deque.modCount; prevLink = prevLink->prev)
                            action.accept(*(lastLink = prevLink)->item);
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                }
            };

            class DescendingIterator2 final : public Iterator2<E> {
                LinkedDeque const &deque;
                LINK prevLink = null;
                LINK lastLink = null;
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT DescendingIterator2(LinkedDeque const &deque): deque(deque), prevLink(deque.tail),
                                                                             lastModCount(deque.modCount) {
                }

                ~DescendingIterator2() override {
                    prevLink = lastLink = null;
                }

                gbool hasNext() const override {
                    return prevLink != null;
                }

                E &next() override {
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    if (prevLink == null)
                        NoSuchElementException().throws($ftrace());
                    lastLink = prevLink;
                    prevLink = prevLink->prev;
                    return *lastLink->item;
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (Class<DescendingIterator2>::hasInstance(o)) {
                        DescendingIterator2 const &it = CORE_XCAST(DescendingIterator2 const, o);
                        return &deque == &it.deque && prevLink == it.prevLink;
                    }
                    return false;
                }

                void forEach(const function::Consumer<E> &action) override {
                    try {
                        for (; prevLink != null && lastModCount == deque.modCount; prevLink = prevLink->prev)
                            action.accept(*(lastLink = prevLink)->item);
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    if (lastModCount != deque.modCount)
                        ConcurrentModificationException().throws($ftrace());
                }
            };
        };
    } // util
} // core

#endif //CORE24_LINKEDDEQUE_H
