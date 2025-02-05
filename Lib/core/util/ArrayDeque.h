//
// Created by brunshweeck on 15 juil. 2024.
//

#ifndef CORE24_ARRAYDEQUE_H
#define CORE24_ARRAYDEQUE_H

#include <core/util/Arrays.h>
#include <core/util/ConcurrentModificationException.h>
#include <core/util/Deque.h>

namespace core {
    namespace util {
        /**
         * Resizable-array implementation of the @c Deque interface.  Array
         * deques have no capacity restrictions; they grow as necessary to support
         * usage.  They are not thread-safe; in the absence of external
         * synchronization, they do not support concurrent access by multiple threads.
         * Null elements are prohibited.  This class is likely to be faster than
         * @c Stack when used as a stack, and faster than @c LinkedDeque
         * when used as a queue.
         *
         * <p>
         * Most @c ArrayDeque operations run in amortized constant time.
         * Exceptions include @c remove, @c removeFirstOccurrence, @c removeLastOccurrence,
         * @c contains, @c iterator.remove(),
         * and the bulk operations, all of which run in linear time.
         * </p>
         *
         * <p>
         * The iterators returned by this class's @c iterator()
         * method are <em>fail-fast</em>: If the deque is modified at any time after
         * the iterator is created, in any way except through the iterator's own
         * @c remove method, the iterator will generally throw a
         * @c ConcurrentModificationException.  Thus, in the face of concurrent
         * modification, the iterator fails quickly and cleanly, rather than risking
         * arbitrary, non-deterministic behavior at an undetermined time in the
         * future.
         * </p>
         *
         * <p>
         * Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw @c ConcurrentModificationException on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness: <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         * </p>
         *
         * <p>
         * This class and its iterator implement all the <em>optional</em> methods of the
         * @c Collection, @c SequencedCollection, and @c Iterator interfaces.
         * </p>
         *
         * @tparam E the type of elements held in this deque
         */
        template <class E>
        class ArrayDeque final : public virtual Deque<E> {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            /**
             * The array in which the elements of the deque are stored.
             * All array cells not holding deque elements are always null.
             * The array always has at least one null slot (at tail).
             */
            Array<> /* mutable */ elements;

            /**
             * The index of the element at the head of the deque (which is the
             * element that would be removed by remove() or pop()); or an
             * arbitrary number 0 <= head < elements.length() equal to tail if
             * the deque is empty.
             */
            gint head = 0;

            /**
             * The index at which the next element would be added to the tail
             * of the deque (via addLast(E), add(E), or push(E));
             * elements[tail] is always null.
             */
            gint tail = 0;

            /**
             * The maximum size of array to allocate.
             * Some VMs reserve some header words in an array.
             * Attempts to allocate larger arrays may result in
             * OutOfMemoryError: Requested array size exceeds VM limit
             */
            static CORE_FAST gint MAX_ARRAY_SIZE = Arrays::SOFT_MAX_ARRAY_LENGTH;

            /**
             * Increases the capacity of this deque by at least the given amount.
             *
             * @param needed the required minimum extra capacity; must be positive
             */
            void resize(gint needed) {
                try {
                    // overflow-conscious code
                    gint const oldCapacity = elements.length();
                    gint newCapacity;
                    // Double capacity if small; else grow by 50%
                    gint const jump = (oldCapacity < 64) ? (oldCapacity + 2) : (oldCapacity >> 1);
                    if ((jump < needed) || (newCapacity = (oldCapacity + jump)) > MAX_ARRAY_SIZE)
                        newCapacity = ArrayDeque::newCapacity(needed, jump);
                    Array<>& es = elements = Arrays::copyOf(elements, newCapacity);
                    // Exceptionally, here tail == head needs to be disambiguated
                    if (tail < head || (tail == head && es[head] != null)) {
                        // wrap around; slide first leg forward to end of array
                        const gint newSpace = newCapacity - oldCapacity;
                        Arrays::copy(es, head,
                                     es, head + newSpace,
                                     oldCapacity - head);
                        Arrays::fill(es, head, head += newSpace, null);
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /** Capacity calculation for edge conditions, especially overflow. */
            gint newCapacity(gint needed, gint jump) const {
                gint const oldCapacity = elements.length();
                gint minCapacity;
                if ((minCapacity = oldCapacity + needed) > MAX_ARRAY_SIZE) {
                    if (minCapacity < 0)
                        IllegalStateException("Sorry, deque too big"_S).throws($ftrace());
                    return Integer::MAX_VALUE;
                }
                if (needed > jump)
                    return minCapacity;
                return (oldCapacity + jump - MAX_ARRAY_SIZE < 0)
                           ? oldCapacity + jump
                           : MAX_ARRAY_SIZE;
            }

        public:
            /**
             * Constructs an empty array deque with an initial capacity
             * sufficient to hold 16 elements.
             */
            CORE_IMPLICIT ArrayDeque() {
                try {
                    elements = Array<>(16 + 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs an empty array deque with an initial capacity
             * sufficient to hold the specified number of elements.
             *
             * @param numElements lower bound on initial capacity of the deque
             */
            CORE_EXPLICIT ArrayDeque(gint numElements) {
                try {
                    elements = Array<>((numElements < 1)
                                           ? 1
                                           : (numElements == Integer::MAX_VALUE)
                                                 ? Integer::MAX_VALUE
                                                 : numElements + 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a deque containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.  (The first element returned by the collection's
             * iterator becomes the first element, or <i>front</i> of the
             * deque.)
             *
             * @param c the collection whose elements are to be placed into the deque
             */
            CORE_EXPLICIT ArrayDeque(Collection<E> const& c) : ArrayDeque(c.size()) {}

        private:
            /**
             * Circularly increments i, mod modulus.
             * Precondition and postCondition: 0 <= i < modulus.
             */
            static gint nextPosition(gint i, gint modulus) {
                i += 1;
                if (i >= modulus)
                    i = 0;
                return i;
            }

            /**
             * Circularly adds the given distance to index i, mod modulus.
             * Precondition: 0 <= i < modulus, 0 <= distance <= modulus.
             * @return index 0 <= i < modulus
             */
            static gint nextPosition(gint i, gint distance, gint modulus) {
                i += distance;
                if (i >= modulus)
                    i -= modulus;
                return i;
            }

            /**
             * Circularly decrements i, mod modulus.
             * Precondition and post-condition: 0 <= i < modulus.
             */
            static gint prevPosition(gint i, gint modulus) {
                i -= 1;
                if (i < 0)
                    i = modulus - 1;
                return i;
            }

            /**
             * Subtracts j from i, mod modulus.
             * Index i must be logically ahead of index j.
             * Precondition: 0 <= i < modulus, 0 <= j < modulus.
             * @return the "circular distance" from j to i; corner case i == j
             * is disambiguated to "empty", returning 0.
             */
            static gint sub(gint i, gint j, gint modulus) {
                i -= j;
                if (i < 0)
                    i += modulus;
                return i;
            }

            /**
             * Returns element at array index i.
             */
            static E& elementAt(Array<>& es, gint i) {
                return CORE_XCAST(E, es[i]);
            }

            /**
             * Returns element at array index i.
             */
            static E const& elementAt(Array<> const& es, gint i) {
                return CORE_XCAST(E const, es[i]);
            }

        public:
            // The main insertion and extraction methods are addFirst,
            // addLast, pollFirst, pollLast. The other methods are defined in
            // terms of these.

            /**
             * Inserts the specified element at the front of this deque.
             *
             * @param val the element to add
             * @throws IllegalArgumentException if the specified element is null
             */
            void addFirst(const E& val) override {
                if (val == null)
                    IllegalArgumentException().throws($ftrace());

                try {
                    Array<>& es = elements;
                    es.set(head = prevPosition(head, elements.length()), val);
                    if (head == tail) resize(1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts the specified element at the end of this deque.
             *
             * <p>
             * This method is equivalent to @c add.
             * </p>
             *
             * @param val the element to add
             * @throws IllegalArgumentException if the specified element is null
             */
            void addLast(const E& val) override {
                if (val == null)
                    IllegalArgumentException().throws($ftrace());

                try {
                    Array<>& es = elements;
                    es.set(tail, val);
                    if (head == (tail = nextPosition(tail, es.length()))) resize(1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Adds all the elements in the specified collection at the end
             * of this deque, as if by calling @c addLast on each one,
             * in the order that they are returned by the collection's iterator.
             *
             * @param c the elements to be inserted into this deque
             * @return @c true if this deque changed as a result of the call
             * @throws IllegalArgumentException if any of the specified collection elements are null
             */
            gbool addAll(const Collection<E>& c) override {
                try {
                    gint s = 0, needed = 0;
                    if ((needed = (s = size()) + c.size() + 1 - elements.length()) > 0) resize(needed);
                    copyElements(c);
                    return size() > s;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts the specified element at the front of this deque.
             *
             * @param val the element to add
             * @return @c true (as specified by @c Deque::offerFirst)
             * @throws IllegalArgumentException if the specified element is null
             */
            gbool offerFirst(const E& val) override {
                try {
                    addFirst(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return true;
            }

            /**
             * Inserts the specified element at the end of this deque.
             *
             * @param val the element to add
             * @return @c true (as specified by @c Deque::offerLast)
             * @throws IllegalArgumentException if the specified element is null
             */
            gbool offerLast(const E& val) override {
                try {
                    addLast(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return true;
            }

            E& removeFirst() override {
                Object& val = pollFirst();
                if (val == null)
                    NoSuchElementException().throws($ftrace());
                return CORE_XCAST(E, val);
            }

            E& removeLast() override {
                Object& val = pollLast();
                if (val == null)
                    NoSuchElementException().throws($ftrace());
                return CORE_XCAST(E, val);
            }

            Object& pollFirst() override {
                Array<>& es = elements;
                gint h = head;
                E& val = elementAt(es, h);
                if (val != null) {
                    es.set(h, null);
                    head = nextPosition(h, es.length());
                }
                return val;
            }

            Object& pollLast() override {
                Array<>& es = elements;
                gint t = prevPosition(tail, es.length());
                E& val = elementAt(es, t);
                if (val != null)
                    es.set(tail = t, null);
                return val;
            }

            E& firstElement() override {
                Array<>& es = elements;
                E& val = elementAt(es, head);
                if (val == null)
                    NoSuchElementException().throws($ftrace());
                return val;
            }

            const E& firstElement() const override {
                Array<> const& es = elements;
                E const& val = elementAt(es, head);
                if (val == null)
                    NoSuchElementException().throws($ftrace());
                return val;
            }

            E& lastElement() override {
                Array<>& es = elements;
                E& val = elementAt(es, prevPosition(tail, es.length()));
                if (val == null)
                    NoSuchElementException().throws($ftrace());
                return val;
            }

            const E& lastElement() const override {
                Array<> const& es = elements;
                E const& val = elementAt(es, prevPosition(tail, es.length()));
                if (val == null)
                    NoSuchElementException().throws($ftrace());
                return val;
            }

            Object& peekFirst() override {
                Array<>& es = elements;
                return elementAt(es, head);
            }

            const Object& peekFirst() const override {
                Array<> const& es = elements;
                return elementAt(es, head);
            }

            Object& peekLast() override {
                Array<>& es = elements;
                return elementAt(es, prevPosition(tail, es.length()));
            }

            const Object& peekLast() const override {
                Array<> const& es = elements;
                return elementAt(es, prevPosition(tail, es.length()));
            }

            /**
             * Removes the first occurrence of the specified element in this
             * deque (when traversing the deque from head to tail).
             * If the deque does not contain the element, it is unchanged.
             * More formally, removes the first element @c e such that
             * @c o.equals(e) (if such an element exists).
             * Returns @c true if this deque contained the specified element
             * (or equivalently, if this deque changed as a result of the call).
             *
             * @param val element to be removed from this deque, if present
             * @return @c true if the deque contained the specified element
             */
            gbool removeFirstOccurrence(const E& val) override {
                if (val != null) {
                    Array<>& es = elements;
                    for (gint i = head, end = tail, to = (i <= end) ? end : es.length();; i = 0, to = end) {
                        for (; i < to; ++i)
                            if (val == es[i]) {
                                deleteAt(i);
                                return true;
                            }
                        if (to == end)
                            break;
                    }
                }
                return false;
            }

            /**
             * Removes the last occurrence of the specified element in this
             * deque (when traversing the deque from head to tail).
             * If the deque does not contain the element, it is unchanged.
             * More formally, removes the last element @c e such that
             * @c o.equals(e) (if such an element exists).
             * Returns @c true if this deque contained the specified element
             * (or equivalently, if this deque changed as a result of the call).
             *
             * @param val element to be removed from this deque, if present
             * @return @c true if the deque contained the specified element
             */
            gbool removeLastOccurrence(const E& val) override {
                if (val != null) {
                    Array<>& es = elements;
                    for (gint i = tail, end = head, to = (i >= end) ? end : 0;; i = es.length(), to = end) {
                        for (i -= 1; i > to; i -= 1)
                            if (val == es[i]) {
                                deleteAt(i);
                                return true;
                            }
                        if (to == end)
                            break;
                    }
                }
                return false;
            }

            // *** Queue methods ***

            /**
             * Inserts the specified element at the end of this deque.
             *
             * <p>
             * This method is equivalent to @c addLast.
             * </p>
             *
             * @param val the element to add
             * @return @c true (as specified by @c Collection::add)
             * @throws IllegalArgumentException if the specified element is null
             */
            gbool add(const E& val) override {
                try {
                    addLast(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return true;
            }

            /**
             * Inserts the specified element at the end of this deque.
             *
             * <p>
             * This method is equivalent to @c #offerLast.
             * </p>
             *
             * @param val the element to add
             * @return @c true (as specified by @c Queue::offer)
             * @throws IllegalArgumentException if the specified element is null
             */
            gbool offer(const E& val) override {
                try {
                    return offerLast(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Retrieves and removes the head of the queue represented by this deque.
             *
             * This method differs from @c poll() only in that it
             * throws an exception if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c removeFirst.
             * </p>
             *
             * @return the head of the queue represented by this deque
             * @throws NoSuchElementException
             */
            E& remove() override {
                try {
                    return removeFirst();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque.  This method differs from @c peek only in
             * that it throws an exception if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c firstElement.
             * </p>
             *
             * @return the head of the queue represented by this deque
             * @throws NoSuchElementException
             */
            Object& poll() override {
                try {
                    return pollFirst();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque.  This method differs from @c peek only in
             * that it throws an exception if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c firstElement.
             * </p>
             *
             * @return the head of the queue represented by this deque
             * @throws NoSuchElementException
             */
            E& element() override {
                if (head == tail)
                    NoSuchElementException().throws($ftrace());
                return elementAt(elements, head);
            }

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque.  This method differs from @c peek only in
             * that it throws an exception if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c firstElement.
             * </p>
             *
             * @return the head of the queue represented by this deque
             * @throws NoSuchElementException
             */
            const E& element() const override {
                if (head == tail)
                    NoSuchElementException().throws($ftrace());
                return elementAt(elements, head);
            }

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque, or returns @c null if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c peekFirst.
             * </p>
             *
             * @return the head of the queue represented by this deque, or
             *         @c null if this deque is empty
             */
            Object& peek() override {
                try {
                    return peekFirst();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque, or returns @c null if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c peekFirst.
             * </p>
             *
             * @return the head of the queue represented by this deque, or
             *         @c null if this deque is empty
             */
            const Object& peek() const override {
                try {
                    return peekFirst();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            // *** Stack methods ***

            /**
             * Pushes an element onto the stack represented by this deque.  In other
             * words, inserts the element at the front of this deque.
             *
             * <p>
             * This method is equivalent to @c addFirst.
             * </p>
             *
             * @param val the element to push
             * @throws IllegalArgumentException if the specified element is null
             */
            void push(const E& val) override {
                try {
                    addFirst(val);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Pops an element from the stack represented by this deque.  In other
             * words, removes and returns the first element of this deque.
             *
             * <p>
             * This method is equivalent to @c removeFirst().
             * </p>
             *
             * @return the element at the front of this deque (which is the top
             *         of the stack represented by this deque)
             * @throws NoSuchElementException
             */
            E& pop() override {
                try {
                    return removeFirst();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            // *** Collection Methods ***

            /**
             * Returns the number of elements in this deque.
             *
             * @return the number of elements in this deque
             */
            gint size() const override {
                return sub(tail, head, elements.length());
            }

            /**
             * Returns @c true if this deque contains no elements.
             *
             * @return @c true if this deque contains no elements
             */
            gbool isEmpty() const override {
                return head == tail;
            }

            /**
             * Returns an iterator over the elements in this deque.  The elements
             * will be ordered from first (head) to last (tail).  This is the same
             * order that elements would be dequeued (via successive calls to
             * @c remove) or popped (via successive calls to @c pop).
             *
             * @return an iterator over the elements in this deque
             */
            Iterator<E>& iterator() override {
                try {
                    return UNSAFE::newInstance<AscendingIterator>(*this);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this deque.  The elements
             * will be ordered from first (head) to last (tail).  This is the same
             * order that elements would be dequeued (via successive calls to
             * @c remove) or popped (via successive calls to @c pop).
             *
             * @return an iterator over the elements in this deque
             */
            Iterator2<E>& iterator() const override {
                try {
                    return UNSAFE::newInstance<AscendingIterator2>(*this);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& descendingIterator() override {
                try {
                    return UNSAFE::newInstance<DescendingIterator>(*this);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& descendingIterator() const override {
                try {
                    return UNSAFE::newInstance<DescendingIterator2>(*this);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<E&> const& action) override {
                Array<>& es = elements;
                for (gint i = head, end = tail, to = (i < end) ? end : es.length();; i = 0, to = end) {
                    try {
                        for (; i < to; i++)
                            action.accept(elementAt(es, i));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    if (to == end) {
                        if (end != tail)
                            ConcurrentModificationException().throws($ftrace());
                        break;
                    }
                }
            }

            void forEach(function::Consumer<E> const& action) const override {
                Array<> const& es = elements;
                for (gint i = head, end = tail, to = (i < end) ? end : es.length();; i = 0, to = end) {
                    try {
                        for (; i < to; i++)
                            action.accept(elementAt(es, i));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    if (to == end) {
                        if ((end != tail))
                            ConcurrentModificationException().throws($ftrace());
                        break;
                    }
                }
            }

            gbool removeIf(function::Predicate<E> const& filter) override {
                try {
                    return deleteAll(filter);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(Collection<E> const& c) override {
                return deleteAll([&](E const& val) -> gbool { return c.contains(val); });
            }

            gbool retainAll(Collection<E> const& c) override {
                return deleteAll([&](E const& val) -> gbool { return !c.contains(val); });
            }

            /**
             * Returns @c true if this deque contains the specified element.
             * More formally, returns @c true if and only if this deque contains
             * at least one element @c e such that @c o.equals(e).
             *
             * @param val object to be checked for containment in this deque
             * @return @c true if this deque contains the specified element
             */
            gbool contains(E const& val) const override {
                if (val != null) {
                    Array<> const& es = elements;
                    for (gint i = head, end = tail, to = (i <= end) ? end : es.length();; i = 0, to = end) {
                        for (; i < to; i += 1)
                            if (val == es[i])
                                return true;
                        if (to == end)
                            break;
                    }
                }
                return false;
            }

            /**
             * Removes a single instance of the specified element from this deque.
             * If the deque does not contain the element, it is unchanged.
             * More formally, removes the first element @c e such that
             * @c o.equals(e) (if such an element exists).
             * Returns @c true if this deque contained the specified element
             * (or equivalently, if this deque changed as a result of the call).
             *
             * <p>
             * This method is equivalent to @c #removeFirstOccurrence(Object).
             * </p>
             *
             * @param val element to be removed from this deque, if present
             * @return @c true if this deque contained the specified element
             */
            gbool remove(E const& val) override {
                return removeFirstOccurrence(val);
            }

            /**
             * Returns an array containing all the elements in this deque
             * in proper sequence (from first to last element).
             *
             * <p>
             * The returned array will be "safe" in that no references to it are
             * maintained by this deque.  (In other words, this method must allocate
             * a new array).  The caller is thus free to modify the returned array.
             * </p>
             *
             * <p>
             * This method acts as bridge between array-based and collection-based
             * APIs.
             * </p>
             *
             * @return an array containing all the elements in this deque
             */
            Array<E> toArray() const override {
                Array<> const& es = elements;
                Array<E> a;
                gint count = size();
                gint end = es.length();
                if (head > tail) {
                    CORE_IGNORE_DEPRECATIONS(a = Array<E>(count);)
                    Arrays::copy(es, head, a, 0, end - head);
                    Arrays::copy(es, 0, a, end - head, tail);
                } else
                    a = Arrays::copyOfRange<E>(es, head, tail);
                return a;
            }

            // *** Object methods ***

            /**
             * Returns a copy of this deque.
             *
             * @return a copy of this deque
             */
            Object& clone() const override {
                try {
                    return UNSAFE::newInstance<ArrayDeque>(*this);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        private:
            void copyElements(Collection<E> const& c) {
                try {
                    c.forEach({*this, &ArrayDeque::addLast});
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the element at the specified position in the elements array.
             * This can result in forward or backwards motion of array elements.
             * We optimize for least element motion.
             *
             * <p>
             * This method is called delete rather than remove to emphasize
             * that its semantics differ from those of @c List::remove(int).
             * </p>
             *
             * @return true if elements near tail moved backwards
             */
            gbool deleteAt(gint i) {
                Array<>& es = elements;
                gint const capacity = es.length();
                gint h, t;
                // number of elements before to-be-deleted elt
                gint const front = sub(i, h = head, capacity);
                // number of elements after to-be-deleted elt
                gint const back = sub(t = tail, i, capacity) - 1;
                if (front < back) {
                    // move front elements forwards
                    if (h <= i)
                        Arrays::copy(es, h, es, h + 1, front);
                    else {
                        Arrays::copy(es, 0, es, 1, i);
                        es.set(0, es[capacity - 1]);
                        Arrays::copy(es, h, es, h + 1, front - (i + 1));
                    }
                    es.set(h, null);
                    head = nextPosition(h, capacity);
                    return false;
                }
                // move back elements backwards
                tail = prevPosition(t, capacity);
                if (i <= tail)
                    Arrays::copy(es, i + 1, es, i, back);
                else {
                    Arrays::copy(es, h + 1, es, h, capacity - (i + 1));
                    es.set(capacity - 1, es[0]);
                    Arrays::copy(es, 1, es, 0, t - 1);
                }
                es.set(tail, null);
                return true;
            }

            gbool deleteAll(function::Predicate<E> const& condition) {
                Array<>& es = elements;
                // Number of suppressed elements
                gint count = 0;
                // Optimize for initial run of survivors
                for (gint i = head, end = tail, to = (i <= end) ? end : es.length();; i = 0, to = end) {
                    for (; i < to; i += 1) {
                        if ((es[i] == null))
                            ConcurrentModificationException().throws($ftrace());
                        if (condition.test(elementAt(es, i))) {
                            count += 1;
                            es.set(i, null);
                            continue;
                        }
                        if (count > 0)
                            Arrays::swap(es, i, sub(i, count, es.length()));
                    }

                    if (to == end) {
                        if ((end != tail))
                            ConcurrentModificationException().throws($ftrace());
                        tail = sub(tail, count, es.length());
                        break;
                    }
                }
                return count > 0;
            }

            class AscendingIterator final : public Iterator<E> {
                ArrayDeque& deque;

                /** Index of element to be returned by subsequent call to next. */
                gint cursor = 0;

                /** Number of elements yet to be returned. */
                gint remaining;

                /**
                 * Index of element returned by most recent call to next.
                 * Reset to -1 if element is deleted by a call to remove.
                 */
                gint last = -1;

            public:
                CORE_EXPLICIT AscendingIterator(ArrayDeque& deque) : deque(deque), cursor(deque.head),
                                                                     remaining(deque.size()) {}

                gbool hasNext() const override {
                    return remaining > 0;
                }

                E& next() override {
                    if (remaining <= 0)
                        NoSuchElementException().throws($ftrace());
                    Array<>& es = deque.elements;
                    E& val = elementAt(es, cursor);
                    cursor = nextPosition(last = cursor, es.length());
                    remaining -= 1;
                    return val;
                }

                void remove() override {
                    if (last < 0)
                        IllegalStateException().throws($ftrace());
                    const Array<>& es = deque.elements;
                    if (deque.deleteAt(last))
                        cursor = prevPosition(cursor, es.length());
                    last = -1;
                }

                void forEach(const function::Consumer<E&>& action) override {
                    gint r;
                    if ((r = remaining) <= 0)
                        return;
                    remaining = 0;
                    Array<>& es = deque.elements;
                    if ((es[cursor] == null || sub(deque.tail, cursor, es.length()) != r))
                        ConcurrentModificationException().throws($ftrace());
                    for (gint i = cursor, end = deque.tail, to = (i <= end) ? end : es.length();; i = 0, to = end) {
                        try {
                            for (; i < to; i += 1)
                                action.accept(elementAt(es, i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (end == to) {
                            if ((end != deque.tail))
                                ConcurrentModificationException().throws($ftrace());
                            last = prevPosition(end, es.length());
                            break;
                        }
                    }
                }

                gbool equals(const Object& o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<AscendingIterator>::hasInstance(o))
                        return false;
                    AscendingIterator const& it = CORE_XCAST(AscendingIterator const, o);
                    return &deque == &it.deque && cursor == it.cursor;
                }
            };

            class AscendingIterator2 final : public Iterator2<E> {
                ArrayDeque const& deque;

                /** Index of element to be returned by subsequent call to next. */
                gint cursor = 0;

                /** Number of elements yet to be returned. */
                gint remaining;

                /**
                 * Index of element returned by most recent call to next.
                 * Reset to -1 if element is deleted by a call to remove.
                 */
                gint last = -1;

            public:
                CORE_EXPLICIT AscendingIterator2(ArrayDeque const& deque) : deque(deque), cursor(deque.head),
                                                                            remaining(deque.size()) {}

                gbool hasNext() const override {
                    return remaining > 0;
                }

                E const& next() override {
                    if (remaining <= 0)
                        NoSuchElementException().throws($ftrace());
                    Array<> const& es = deque.elements;
                    E const& val = elementAt(es, cursor);
                    cursor = nextPosition(last = cursor, es.length());
                    remaining -= 1;
                    return val;
                }

                void forEach(const function::Consumer<E>& action) override {
                    gint r;
                    if ((r = remaining) <= 0)
                        return;
                    remaining = 0;
                    Array<> const& es = deque.elements;
                    if ((es[cursor] == null || sub(deque.tail, cursor, es.length()) != r))
                        ConcurrentModificationException().throws($ftrace());
                    for (gint i = cursor, end = deque.tail, to = (i <= end) ? end : es.length();; i = 0, to = end) {
                        try {
                            for (; i < to; i += 1)
                                action.accept(elementAt(es, i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (end == to) {
                            if ((end != deque.tail))
                                ConcurrentModificationException().throws($ftrace());
                            last = prevPosition(end, es.length());
                            break;
                        }
                    }
                }

                gbool equals(const Object& o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<AscendingIterator2>::hasInstance(o))
                        return false;
                    AscendingIterator2 const& it = CORE_XCAST(AscendingIterator2 const, o);
                    return &deque == &it.deque && cursor == it.cursor;
                }
            };

            class DescendingIterator final : public Iterator<E> {
                ArrayDeque& deque;

                /** Index of element to be returned by subsequent call to next. */
                gint cursor = 0;

                /** Number of elements yet to be returned. */
                gint remaining;

                /**
                 * Index of element returned by most recent call to next.
                 * Reset to -1 if element is deleted by a call to remove.
                 */
                gint last = -1;

            public:
                CORE_EXPLICIT DescendingIterator(ArrayDeque& deque) : deque(deque), remaining(deque.size()) {
                    cursor = prevPosition(deque.tail, deque.elements.length());
                }

                gbool hasNext() const override {
                    return remaining > 0;
                }

                E& next() override {
                    if (remaining <= 0)
                        NoSuchElementException().throws($ftrace());
                    Array<>& es = deque.elements;
                    Object& val = es[cursor];
                    if ((val == null))
                        ConcurrentModificationException().throws($ftrace());
                    cursor = prevPosition(last = cursor, es.length());
                    remaining -= 1;
                    return CORE_XCAST(E, val);
                }

                void remove() override {
                    if (last < 0)
                        IllegalStateException().throws($ftrace());
                    Array<>& es = deque.elements;
                    if (deque.deleteAt(last))
                        cursor = prevPosition(cursor, es.length());
                    last = -1;
                }

                void forEach(const function::Consumer<E&>& action) override {
                    gint r;
                    if ((r = remaining) <= 0)
                        return;
                    remaining = 0;
                    Array<>& es = deque.elements;
                    if ((es[cursor] == null || sub(cursor, deque.head, es.length()) + 1 != r))
                        ConcurrentModificationException().throws($ftrace());
                    for (gint i = cursor, end = deque.head, to = (i >= end) ? end : 0;; i = es.length() - 1, to = end) {
                        try {
                            for (; i > to - 1; i -= 1)
                                action.accept(elementAt(es, i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (end == to) {
                            if ((end != deque.tail))
                                ConcurrentModificationException().throws($ftrace());
                            last = end;
                            break;
                        }
                    }
                }

                gbool equals(const Object& o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<DescendingIterator>::hasInstance(o))
                        return false;
                    DescendingIterator const& it = CORE_XCAST(DescendingIterator const, o);
                    return &deque == &it.deque && cursor == it.cursor;
                }
            };

            class DescendingIterator2 final : public Iterator2<E> {
                ArrayDeque const& deque;

                /** Index of element to be returned by subsequent call to next. */
                gint cursor = 0;

                /** Number of elements yet to be returned. */
                gint remaining;

                /**
                 * Index of element returned by most recent call to next.
                 * Reset to -1 if element is deleted by a call to remove.
                 */
                gint last = -1;

            public:
                CORE_EXPLICIT DescendingIterator2(ArrayDeque const& deque) : deque(deque), remaining(deque.size()) {
                    cursor = prevPosition(deque.tail, deque.elements.length());
                }

                gbool hasNext() const override {
                    return remaining > 0;
                }

                E const& next() override {
                    if (remaining <= 0)
                        NoSuchElementException().throws($ftrace());
                    Array<> const& es = deque.elements;
                    Object const& val = es[cursor];
                    if ((val == null))
                        ConcurrentModificationException().throws($ftrace());
                    cursor = prevPosition(last = cursor, es.length());
                    remaining -= 1;
                    return CORE_XCAST(E const, val);
                }

                void forEach(const function::Consumer<E>& action) override {
                    gint r;
                    if ((r = remaining) <= 0)
                        return;
                    remaining = 0;
                    Array<> const& es = deque.elements;
                    if ((es[cursor] == null || sub(cursor, deque.head, es.length()) + 1 != r))
                        ConcurrentModificationException().throws($ftrace());
                    for (gint i = cursor, end = deque.head, to = (i >= end) ? end : 0;; i = es.length() - 1, to = end) {
                        try {
                            for (; i > to - 1; i -= 1)
                                action.accept(elementAt(es, i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (end == to) {
                            if ((end != deque.tail))
                                ConcurrentModificationException().throws($ftrace());
                            last = end;
                            break;
                        }
                    }
                }

                gbool equals(const Object& o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<DescendingIterator2>::hasInstance(o))
                        return false;
                    DescendingIterator2 const& it = CORE_XCAST(DescendingIterator2 const, o);
                    return &deque == &it.deque && cursor == it.cursor;
                }
            };
        };


#if __cpp_deduction_guides >= 201711L
        ArrayDeque() -> ArrayDeque<>;
        ArrayDeque(...) -> ArrayDeque<>;
        template <class E> ArrayDeque(Collection<E> const&) -> ArrayDeque<E>;
#endif


    } // util
} // core

#endif //CORE24_ARRAYDEQUE_H
