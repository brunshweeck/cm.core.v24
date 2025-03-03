//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_PRIORITYQUEUE_H
#define CORE24_PRIORITYQUEUE_H

#include <core/util/ArrayDeque.h>
#include <core/util/Arrays.h>
#include <core/util/Queue.h>

namespace core {
    namespace util {
        /**
         * An unbounded priority @a queue based on a priority heap.
         * The elements of the priority queue are ordered according to their
         * <em>natural ordering</em>, or by a @em Comparator
         * provided at queue construction time, depending on which constructor is
         * used. A priority queue relying on natural ordering also does not permit
         * insertion of non-comparable objects (doing so may result in
         * @c ClassCastException).
         *
         * <p>
         * The <em>head</em> of this queue is the <em>least</em> element
         * with respect to the specified ordering.  If multiple elements are
         * tied for least value, the head is one of those elements -- ties are
         * broken arbitrarily.  The queue retrieval operations @c poll,
         * @c remove, @c peek, and @c element access the
         * element at the head of the queue.
         * </p>
         * <p>
         * A priority queue is unbounded, but has an internal
         * <i>capacity</i> governing the size of an array used to store the
         * elements on the queue.  It is always at least as large as the queue
         * size.  As elements are added to a priority queue, its capacity
         * grows automatically.  The details of the growth policy are not
         * specified.
         * </p>
         * <p>
         * This class and its iterator implement all the
         * <em>optional</em> methods of the @em Collection and @em Iterator
         * interfaces.  The Iterator provided in method @em iterator()
         * is <em>not</em> guaranteed to traverse the elements of
         * the priority queue in any particular order. If you need ordered
         * traversal, consider using @code Arrays.sort(pq.toArray()) @endcode .
         * </p>
         * <p>
         * <b>Note that this implementation is not synchronized.</b>
         * Multiple threads should not access a @c PriorityQueue
         * instance concurrently if any of the threads modifies the queue.
         * </p>
         * <p>
         * Implementation note: this implementation provides
         * O(log(n)) time for the enqueuing and dequeuing methods
         * (@c offer, @c poll, @c remove() and @c add);
         * linear time for the @c remove(Object) and @c contains(Object)
         * methods; and constant time for the retrieval methods
         * (@c peek, @c element, and @c size).
         * </p>
         *
         * @tparam E the type of elements held in this queue
         */
        template<class E>
        class PriorityQueue : public virtual Queue<E> {
            CORE_ALIAS(U, misc::Unsafe);

            /**
             * Default initial capacity.
             */
            static CORE_FAST gint DEFAULT_INITIAL_CAPACITY = 11;

            /**
             * Priority queue represented as a balanced binary heap: the two
             * children of queue[n] are queue[2*n+1] and queue[2*(n+1)].  The
             * priority queue is ordered by comparator, or by the elements'
             * natural ordering, if comparator is null: For each node n in the
             * heap and each descendant d of n, n <= d.  The element with the
             * lowest value is in queue[0], assuming the queue is nonempty.
             */
            Array<> queue;

            /**
             * The number of elements in the priority queue.
             */
            gint count = 0;

            /**
             * The comparator, or null if priority queue uses elements'
             * natural ordering.
             */
            Comparator<E> &cpr;

            /**
             * The number of times this priority queue has been
             * <i>structurally modified</i>.
             */
            gint modCount = 0;

        public:
            /**
             * Creates a @c PriorityQueue with the default initial
             * capacity (11) that orders its elements according to their
             * <em>natural ordering</em>.
             */
            PriorityQueue()
                : PriorityQueue(DEFAULT_INITIAL_CAPACITY) {
            }

            /**
             * Creates a @c PriorityQueue with the specified initial
             * capacity that orders its elements according to their
             * <em>natural ordering</em>.
             *
             * @param initialCapacity the initial capacity for this priority queue
             * @throws IllegalArgumentException if @c initialCapacity is less
             *         than 1
             */
            CORE_EXPLICIT PriorityQueue(gint initialCapacity)
                : PriorityQueue(initialCapacity, Comparator<E>::naturalOrder()) {
            }

            /**
             * Creates a @c PriorityQueue with the default initial capacity and
             * whose elements are ordered according to the specified comparator.
             *
             * @param  comparator the comparator that will be used to order this
             *         priority queue.
             */
            CORE_EXPLICIT PriorityQueue(Comparator<E> const &comparator)
                : PriorityQueue(DEFAULT_INITIAL_CAPACITY, comparator) {
            }

            /**
             * Creates a @c PriorityQueue with the specified initial capacity
             * that orders its elements according to the specified comparator.
             *
             * @param  initialCapacity the initial capacity for this priority queue
             * @param  comparator the comparator that will be used to order this
             *         priority queue.
             * @throws IllegalArgumentException if @c initialCapacity is less than 1
             */
            CORE_EXPLICIT PriorityQueue(gint initialCapacity, Comparator<E> const &comparator)
                : cpr(U::copyInstance(comparator)) {
                // Note: This restriction of at least one is not actually needed
                if (initialCapacity < 1)
                    IllegalArgumentException().throws($ftrace());
                queue = Array<>(initialCapacity, null);
            }

            /**
             * Creates a @c PriorityQueue containing the elements in the
             * specified collection.  If the specified collection is an instance of
             * a @c SortedSet or is another @c PriorityQueue, this
             * priority queue will be ordered according to the same ordering.
             * Otherwise, this priority queue will be ordered according to the
             * <em>natural ordering</em> of its elements.
             *
             * @param  c the collection whose elements are to be placed
             *         into this priority queue
             * @throws ClassCastException if elements of the specified collection
             *         cannot be compared to one another according to the priority
             *         queue's ordering
             */
            CORE_EXPLICIT PriorityQueue(Collection<E> const &c): cpr(detectComparator(c)) {
                if (Class<PriorityQueue>::hasInstance(c)) {
                    PriorityQueue const &pq = CORE_XCAST(PriorityQueue const, c);
                    queue = Arrays::copyOf(queue, pq.count);
                    count = pq.count;
                } else {
                    Array<E> es = c.toArray();
                    gint n = es.length();
                    for (Object &e: es)
                        if (e == null)
                            IllegalArgumentException().throws($ftrace());
                    queue = Arrays::copyOf(es, n);
                    count = n;
                    heapify();
                }
            }

            /**
             * Creates a @c PriorityQueue containing the elements in the
             * specified priority queue.  This priority queue will be
             * ordered according to the same ordering as the given priority
             * queue.
             *
             * @param  pq the priority queue whose elements are to be placed
             *         into this priority queue
             * @throws ClassCastException if elements of @c pq cannot be
             *         compared to one another according to @c pq is ordering
             */
            CORE_IMPLICIT PriorityQueue(PriorityQueue const &pq)
                : queue(pq.queue), count(pq.count), cpr(pq.cpr) {
                //
            }

            /**
             * Creates a @c PriorityQueue containing the elements in the
             * specified priority queue.  This priority queue will be
             * ordered according to the same ordering as the given priority
             * queue.
             *
             * @param  pq the priority queue whose elements are to be placed
             *         into this priority queue
             * @throws ClassCastException if elements of @c pq cannot be
             *         compared to one another according to @c pq is ordering
             */
            CORE_IMPLICIT PriorityQueue(PriorityQueue &&pq) CORE_NOTHROW
                : queue(U::moveInstance(pq.queue)), count(pq.count), cpr(pq.cpr) {
            }

            /**
             * Destroy this queue.
             */
            ~PriorityQueue() override {
                modCount += 1;
                queue.~Array();
                count = 0;
            }

            /**
             * Inserts the specified element into this priority queue.
             *
             * @return @c true (as specified by @c Collection::add)
             * @throws ClassCastException if the specified element cannot be
             *         compared with elements currently in this priority queue
             *         according to the priority queue's ordering
             * @throws IllegalArgumentException if the specified element is null
             */
            gbool add(const E &val) override {
                return offer(val);
            }

            /**
             * Inserts the specified element into this priority queue.
             *
             * @return @c true (as specified by @c Queue::offer)
             * @throws ClassCastException if the specified element cannot be
             *         compared with elements currently in this priority queue
             *         according to the priority queue's ordering
             * @throws IllegalArgumentException if the specified element is null
             */
            gbool offer(const E &val) override {
                if (val == null)
                    IllegalArgumentException().throws($ftrace());
                modCount += 1;
                gint const i = count;
                if (i >= queue.length())
                    resize(i + 1);
                siftUp(i, val);
                count += 1;
                return true;
            }

            /**
             * Retrieves, but does not remove, the head of this queue,
             * or returns @c null if this queue is empty.
             *
             * @return the head of this queue, or @c null if this queue is empty
             */
            Object &peek() override {
                return queue[0];
            }

            /**
             * Retrieves, but does not remove, the head of this queue,
             * or returns @c null if this queue is empty.
             *
             * @return the head of this queue, or @c null if this queue is empty
             */
            const Object &peek() const override {
                return queue[0];
            }

            /**
             * Removes a single instance of the specified element from this queue,
             * if it is present.  More formally, removes an element @c e such
             * that @code o.equals(e) @endcode, if this queue contains one or more such
             * elements.  Returns @c true if and only if this queue contained
             * the specified element (or equivalently, if this queue changed as a
             * result of the call).
             *
             * @param val element to be removed from this queue, if present
             * @return @c true if this queue changed as a result of the call
             */
            gbool remove(const E &val) override {
                gint i = indexOf(val);
                if (i < 0)
                    return false;
                removeAt(i);
                return true;
            }

            /**
             * Returns @c true if this queue contains the specified element.
             * More formally, returns @c true if and only if this queue contains
             * at least one element @c e such that @code o.equals(e) @endcode.
             *
             * @param val object to be checked for containment in this queue
             * @return @c true if this queue contains the specified element
             */
            gbool contains(const E &val) const override {
                return indexOf(val) >= 0;
            }

            /**
             * Returns an array containing all the elements in this queue.
             * The elements are in no particular order.
             *
             * <p>
             * The returned array will be "safe" in that no references to it are
             * maintained by this queue.  (In other words, this method must allocate
             * a new array).  The caller is thus free to modify the returned array.
             * </p>
             * <p>
             * This method acts as bridge between array-based and collection-based
             * APIs.
             * </p>
             * @return an array containing all the elements in this queue
             */
            Array<E> toArray() const override {
                return Arrays::copyOf<E>(queue, count);
            }

            /**
             * Returns an iterator over the elements in this queue. The iterator
             * does not return the elements in any particular order.
             *
             * @return an iterator over the elements in this queue
             */
            Iterator<E> &iterator() override {
                return U::newInstance<PQIterator>(*this);
            }

            /**
             * Returns an iterator over the elements in this queue. The iterator
             * does not return the elements in any particular order.
             *
             * @return an iterator over the elements in this queue
             */
            Iterator2<E> &iterator() const override {
                return U::newInstance<PQIterator2>(*this);
            }

            /**
             * Returns the number of elements in this collection.  If this collection
             * contains more than @c Integer::MAX_VALUE elements, returns @c Integer::MAX_VALUE.
             *
             * @return the number of elements in this collection.
             */
            gint size() const override {
                return count;
            }

            /**
             * Removes all the elements from this priority queue.
             * The queue will be empty after this call returns.
             */
            void clear() override {
                modCount += 1;
                count = 0;
                Arrays::fill(queue, null);
            }

            /**
             * Retrieves and removes the head of this queue,
             * or returns @c null if this queue is empty.
             *
             * @return the head of this queue, or @c null if this queue is empty
             */
            Object &poll() override {
                Array<> &es = queue;
                Object &result = queue[0];
                if (result != null) {
                    modCount += 1;
                    gint const n = --count;
                    const E &x = CORE_XCAST(E, es[n]);
                    es.set(n, null);
                    if (n > 0) {
                        Class<Comparable<E> >::template isSuper<E>() && cpr == Comparator<E>::naturalOrder()
                            ? siftDown(0, x, es, n)
                            : siftDown(0, x, es, n, cpr);
                    }
                }
                return result;
            }

            /**
             * Returns the comparator used to order the elements in this
             * queue.
             *
             * @return the comparator used to order this queue.
             */
            Comparator<E> &comparator() const {
                return cpr;
            }

            gbool removeAll(const Collection<E> &c) override {
                CORE_TRY_RETHROW_AT(
                    core::util::PriorityQueue,
                    return removeAllIf([&](E const &x) -> gbool { return c.contains(x); })
                )
            }

            gbool removeIf(const function::Predicate<E> &filter) override {
                CORE_TRY_RETHROW_AT(core::util::PriorityQueue, return removeAllIf(filter))
            }

            gbool retainAll(const Collection<E> &c) override {
                CORE_TRY_RETHROW_AT(
                    core::util::PriorityQueue,
                    return removeAllIf([&](E const &x) -> gbool { return !c.contains(x); })
                )
            }

            void forEach(const function::Consumer<E &> &action) override {
                gint const lastModCount = modCount;
                Array<> &es = queue;
                try {
                    for (int i = 0, n = count; i < n && lastModCount == modCount; ++i)
                        action.accept(CORE_XCAST(E, es[i]));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                if (lastModCount != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void forEach(const function::Consumer<E> &action) const override {
                gint const lastModCount = modCount;
                Array<> const &es = queue;
                try {
                    for (int i = 0, n = count; i < n && lastModCount == modCount; ++i)
                        action.accept(CORE_XCAST(E const, es[i]));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
                if (lastModCount != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            gbool equals(const Object &o) const override {
                if (this == &o)
                    return true;
                if (!Class<PriorityQueue>::hasInstance(o))
                    return false;
                PriorityQueue const &pq = CORE_XCAST(PriorityQueue const, o);
                if (count != pq.count)
                    return false;
                gint const lastModCount1 = modCount;
                gint const lastModCount2 = pq.modCount;
                gint i = 0;
                gint n = count;
                for (; i < n && (lastModCount1 == modCount) && (lastModCount2 == pq.modCount); ++i)
                    if (queue[i] == pq.queue[i])
                        return false;
                if (i < n)
                    ConcurrentModificationException().throws($ftrace());
                return false;
            }

        private:
            Comparator<E> &detectComparator(Collection<E> const &c) {
                if (Class<PriorityQueue>::hasInstance(c))
                    return CORE_CAST(Comparator<E>&, CORE_XCAST(PriorityQueue const, c).comparator());
                return Comparator<E>::naturalOrder();
            }

            /**
             * Increases the capacity of the array.
             *
             * @param minCapacity the desired minimum capacity
             */
            void resize(gint minCapacity) {
                gint const oldCapacity = queue.length();
                // Double size if small; else grow by 50%
                gint const newCapacity = Arrays::newLength(oldCapacity,
                                                           minCapacity - oldCapacity,
                                                           oldCapacity < 64 ? oldCapacity + 2 : oldCapacity >> 1);
                queue = Arrays::copyOf(queue, newCapacity);
            }

            /**
             * Inserts item x at position k, maintaining heap invariant by
             * promoting x up the tree until it is greater than or equal to
             * its parent, or is the root.
             *
             * To simplify and speed up coercions and comparisons, the
             * Comparable and Comparator versions are separated into different
             * methods that are otherwise identical. (Similarly for siftDown.)
             *
             * @param k the position to fill
             * @param x the item to insert
             */
            void siftUp(gint k, E const &x) {
                // assert k >= 0 && val != null
                if (Class<Comparable<E> >::template isSuper<E>() && cpr == Comparator<E>::naturalOrder())
                    siftUp(k, x, queue);
                else
                    siftUp(k, x, queue, cpr);
            }

            static void siftUp(gint k, E const &x, Array<> &es) {
                Comparable<E> const &key = CORE_XCAST(Comparable<E> const, x);
                while (k > 0) {
                    gint const parent = (k - 1) >> 1;
                    Object &e = es[parent];
                    if (key.compareTo(CORE_XCAST(E, e)) >= 0)
                        break;
                    es.set(k, e);
                    k = parent;
                }
                es.set(k, x);
            }

            static void siftUp(gint k, E const &x, Array<> &es, Comparator<E> const &cmp) {
                while (k > 0) {
                    gint const parent = (k - 1) >> 1;
                    Object &e = es[parent];
                    if (cmp.compare(x, CORE_XCAST(E, e)) >= 0)
                        break;
                    es.set(k, e);
                    k = parent;
                }
                es.set(k, x);
            }

            /**
             * Inserts item x at position k, maintaining heap invariant by
             * demoting x down the tree repeatedly until it is less than or
             * equal to its children or is a leaf.
             *
             * @param k the position to fill
             * @param x the item to insert
             */
            void siftDown(gint k, E const &x) {
                // assert k >= 0 && val != null
                if (Class<Comparable<E> >::template isSuper<E>() && cpr == Comparator<E>::naturalOrder())
                    siftDown(k, x, queue, count);
                else
                    siftDown(k, x, queue, count, cpr);
            }

            static void siftDown(gint k, E const &x, Array<> &es, gint n) {
                // assert n > 0
                Comparable<E> const &key = CORE_XCAST(Comparable<E> const, x);
                gint half = n >> 1;
                // loop while a non-leaf
                while (k < half) {
                    // assume left child is least
                    gint child = (k << 1) + 1;
                    Object &c = es[child];
                    gint const right = child + 1;
                    if ((right < n) && CORE_XCAST(Comparable<E>, c).compareTo(CORE_XCAST(E, es[right])) > 0)
                        child = right;
                    Object &e = es[child];
                    if (key.compareTo(CORE_XCAST(E, e)))
                        break;
                    es.set(k, e);
                    k = child;
                }
                es.set(k, x);
            }

            static void siftDown(gint k, E const &x, Array<> &es, gint n, Comparator<E> const &cmp) {
                // assert n > 0
                gint const half = n >> 1;
                // loop while a non-leaf
                while (k < half) {
                    // assume left child is least
                    gint child = (k << 1) + 1;
                    Object &c = es[child];
                    gint const right = child + 1;
                    if ((right < n) && cmp.compare(CORE_XCAST(E, c), CORE_XCAST(E, es[right])) > 0)
                        child = right;
                    Object &e = es[child];
                    if (cmp.compare(x, CORE_XCAST(E, e)))
                        break;
                    es.set(k, e);
                    k = child;
                }
                es.set(k, x);
            }

            /**
             * Establishes the heap invariant (described above) in the entire tree,
             * assuming nothing about the order of the elements prior to the call.
             * This classic algorithm due to Floyd (1964) is known to be O(size).
             */
            void heapify() {
                Array<> &es = queue;
                gint n = count, i = (n >> 1) - 1;
                if (Class<Comparable<E> >::template isSuper<E>() && cpr == Comparator<E>::naturalOrder())
                    for (; i >= 0; i -= 1)
                        siftDown(i, CORE_XCAST(E, es[i]), es, n);
                else
                    for (; i >= 0; i -= 1)
                        siftDown(i, CORE_XCAST(E, es[i]), es, n, cpr);
            }

            gint indexOf(Object const &o) const {
                if (o != null) {
                    Array<> const &es = queue;
                    for (int i = 0, n = count; i < n; ++i)
                        if (o == es[i])
                            return i;
                }
                return -1;
            }

            /**
             * Removes the ith element from queue.
             *
             * Normally this method leaves the elements at up to i-1,
             * inclusive, untouched.  Under these circumstances, it returns
             * null.  Occasionally, in order to maintain the heap invariant,
             * it must swap a later element of the list with one earlier than
             * i.  Under these circumstances, this method returns the element
             * that was previously at the end of the list and is now at some
             * position before 'i'. This fact is used by iterator. Remove to
             * avoid missing traversing elements.
             */
            Object &removeAt(gint i) {
                // assert i >= 0 && i < count
                Array<> &es = queue;
                modCount += 1;
                gint s = --count;
                if (s == i)
                    es.set(i, null);
                else {
                    Object &moved = es[s];
                    es.set(i, null);
                    siftDown(s, CORE_XCAST(E, moved));
                    if (es[i] == moved) {
                        siftUp(i, CORE_XCAST(E, moved));
                        if (es[i] != moved)
                            return moved;
                    }
                }
                return null;
            }

            gbool removeAllIf(function::Predicate<E> const &condition) {
                gint lastModCount = ++modCount;
                Array<> &es = queue;
                gint n = count;
                gint i = 0;
                for (; (i < n) && (lastModCount == modCount) && !condition.test(CORE_XCAST(E, es[i])); i += 1) {
                    //
                }
                if (i >= n) {
                    if (lastModCount != modCount)
                        ConcurrentModificationException().throws($ftrace());
                    return false;
                }
                i += 1;
                gint nbRemoved = 1;
                for (; i < n; i += 1)
                    if (condition.test(CORE_XCAST(E, es[i])))
                        nbRemoved += 1;
                    else
                        Arrays::swap(es, i, i - nbRemoved);
                count = n - nbRemoved;
                Arrays::fill(es, i, n, null);
                heapify();
                return true;
            }

            class PQIterator final : public Iterator<E> {
                CORE_ALIAS(ITEM, Class<Object>::Pointer);

                PriorityQueue &pq;

                /**
                 * Index (into queue array) of element to be returned by
                 * subsequent call to next.
                 */
                gint cursor = 0;

                /**
                 * Index of element returned by most recent call to next,
                 * unless that element came from the forgetMeNot list.
                 * Set to -1 if element is deleted by a call to remove.
                 */
                gint last = -1;

                /**
                 * A queue of elements that were moved from the unvisited portion of
                 * the heap into the visited portion as a result of "unlucky" element
                 * removals during the iteration.  (Unlucky element removals are those
                 * that require a siftup instead of a siftdown.)  We must visit all
                 * the elements in this list to complete the iteration.  We do this
                 * after we've completed the "normal" iteration.
                 *
                 * We expect that most iterations, even those involving removals,
                 * will not need to store elements in this field.
                 */
                ArrayDeque<> forgetMeNot;

                /**
                 * Element returned by the most recent call to next iff that
                 * element was drawn from the forgetMeNot list.
                 */
                ITEM lastItem = null;

                /**
                 * The modCount value that the iterator believes that the backing
                 * Queue should have.  If this expectation is violated, the iterator
                 * has detected concurrent modification.
                 */
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT PQIterator(PriorityQueue &pq): pq(pq), forgetMeNot(pq.count) {
                }

                gbool hasNext() const override {
                    return cursor < pq.count || !forgetMeNot.isEmpty();
                }

                E &next() override {
                    if (lastModCount != pq.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    if (cursor < pq.count)
                        return CORE_XCAST(E, pq.queue[last = cursor++]);
                    if (!forgetMeNot.isEmpty()) {
                        last = -1;
                        lastItem = &forgetMeNot.poll();
                        if (lastItem != null && *lastItem != null)
                            return CORE_XCAST(E, *lastItem);
                    }
                    NoSuchElementException().throws($ftrace());
                }

                void remove() override {
                    if (lastModCount != pq.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    if (last != -1) {
                        Object &moved = pq.removeAt(last);
                        last = -1;
                        if (moved == null)
                            cursor -= 1;
                        else
                            forgetMeNot.add(moved);
                    } else if (lastItem != null && *lastItem != null) {
                        Array<> &es = pq.queue;
                        for (int i = 0, n = pq.count; i < n; ++i)
                            if (*lastItem == es[i]) {
                                pq.removeAt(i);
                                break;
                            }
                        lastItem = null;
                    } else
                        IllegalStateException().throws($ftrace());
                    lastModCount = pq.modCount;
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<PQIterator>::hasInstance(o))
                        return false;
                    PQIterator const &it = CORE_XCAST(PQIterator const, o);
                    return &pq == &it.pq && cursor == it.cursor;
                }
            };

            class PQIterator2 final : public Iterator2<E> {
                CORE_ALIAS(ITEM, Class<Object>::Pointer);

                PriorityQueue const &pq;

                /**
                 * Index (into queue array) of element to be returned by
                 * subsequent call to next.
                 */
                gint cursor = 0;

                /**
                 * Index of element returned by most recent call to next,
                 * unless that element came from the forgetMeNot list.
                 * Set to -1 if element is deleted by a call to remove.
                 */
                gint last = -1;

                /**
                 * Element returned by the most recent call to next iff that
                 * element was drawn from the forgetMeNot list.
                 */
                ITEM lastItem = null;

                /**
                 * The modCount value that the iterator believes that the backing
                 * Queue should have.  If this expectation is violated, the iterator
                 * has detected concurrent modification.
                 */
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT PQIterator2(PriorityQueue const &pq): pq(pq) {
                }

                gbool hasNext() const override {
                    return cursor < pq.count;
                }

                E const &next() override {
                    if (lastModCount != pq.modCount)
                        ConcurrentModificationException().throws($ftrace());
                    if (cursor < pq.count)
                        return CORE_XCAST(E const, pq.queue[last = cursor++]);
                    NoSuchElementException().throws($ftrace());
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<PQIterator2>::hasInstance(o))
                        return false;
                    PQIterator2 const &it = CORE_XCAST(PQIterator2 const, o);
                    return &pq == &it.pq && cursor == it.cursor;
                }
            };
        };
    } // util
} // core

#endif //CORE24_PRIORITYQUEUE_H
