//
// Created by admin on 25/01/25.
//

#ifndef CORE24_CONCURRENTLINKEDQUEUE_H
#define CORE24_CONCURRENTLINKEDQUEUE_H

#include <core/StringArray.h>
#include <core/util/Objects.h>
#include <core/util/Queue.h>


namespace core {
    namespace concurrent {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED
        CORE_WARNING_DISABLE_INVALID_OFFSETOF

        /**
         * An unbounded thread-safe @em queue based on linked nodes.
         * This queue orders elements FIFO (first-in-first-out).
         * The <em>head</em> of the queue is that element that has been on the
         * queue the longest time.
         * The <em>tail</em> of the queue is that element that has been on the
         * queue the shortest time. New elements
         * are inserted at the tail of the queue, and the queue retrieval
         * operations obtain elements at the head of the queue.
         * A @c ConcurrentLinkedQueue is an appropriate choice when
         * many threads will share access to a common collection.
         * Like most other concurrent collection implementations, this class
         * does not permit the use of @c null elements.
         *
         * <p>
         * This implementation employs an efficient <em>non-blocking</em>
         * algorithm based on one described in
         * <a href="http://www.cs.rochester.edu/~scott/papers/1996_PODC_queues.pdf">
         * Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue
         * Algorithms</a> by Maged M. Michael and Michael L. Scott.
         * </p>
         * <p>
         * Iterators are <i>weakly consistent</i>, returning elements
         * reflecting the state of the queue at some point at or since the
         * creation of the iterator.  They do <em>not</em> throw
         * @b core::util::ConcurrentModificationException, and may proceed concurrently
         * with other operations.  Elements contained in the queue since the creation
         * of the iterator will be returned exactly once.
         * </p>
         * <p>
         * Beware that, unlike in most collections, the @c size method
         * is <em>NOT</em> a constant-time operation. Because of the
         * asynchronous nature of these queues, determining the current number
         * of elements requires a traversal of the elements, and so may report
         * inaccurate results if this collection is modified during traversal.
         * </p>
         * <p>
         * Bulk operations that add, remove, or examine multiple elements,
         * such as @b addAll, @b removeIf or @b forEach,
         * are <em>not</em> guaranteed to be performed atomically.
         * For example, a @c forEach traversal concurrent with an @c addAll
         * operation might observe only some of the added elements.
         * </p>
         * <p>
         * This class and its iterator implement all the <em>optional</em>
         * methods of the @b Queue and @b Iterator interfaces.
         * </p>
         * <p>
         * Memory consistency effects: As with other concurrent
         * collections, actions in a thread prior to placing an object into a
         * @c ConcurrentLinkedQueue <i>happen-before</i>
         * actions after the access or removal of that element from
         * the @c ConcurrentLinkedQueue in another thread.
         * </p>
         *
         * @tparam E the type of elements held in this queue
         */
        template <class E>
        class ConcurrentLinkedQueue final : public util::Queue<E> {
            /*
             * This is a modification of the Michael & Scott algorithm,
             * adapted for a garbage-collected environment, with support for
             * interior node deletion (to support e.g. remove(Object)).  For
             * explanation, read the paper.
             *
             * Note that like most non-blocking algorithms in this package,
             * this implementation relies on the fact that in garbage
             * collected systems, there is no possibility of ABA problems due
             * to recycled nodes, so there is no need to use "counted
             * pointers" or related techniques seen in versions used in
             * non-GC'ed settings.
             *
             * The fundamental invariants are:
             * - There is exactly one (last) Node with a null next reference,
             *   which is CASed when enqueueing.  This last Node can be
             *   reached in O(1) time from tail, but tail is merely an
             *   optimization - it can always be reached in O(N) time from
             *   head as well.
             * - The elements contained in the queue are the non-null items in
             *   Nodes that are reachable from head.  CASing the item
             *   reference of a Node to null atomically removes it from the
             *   queue.  Reachability of all elements from head must remain
             *   true even in the case of concurrent modifications that cause
             *   head to advance.  A dequeued Node may remain in use
             *   indefinitely due to creation of an Iterator or simply a
             *   poll() that has lost its time slice.
             *
             * The above might appear to imply that all Nodes are GC-reachable
             * from a predecessor dequeued Node.  That would cause two problems:
             * - allow a rogue Iterator to cause unbounded memory retention
             * - cause cross-generational linking of old Nodes to new Nodes if
             *   a Node was tenured while live, which generational GCs have a
             *   hard time dealing with, causing repeated major collections.
             * However, only non-deleted Nodes need to be reachable from
             * dequeued Nodes, and reachability does not necessarily have to
             * be of the kind understood by the GC.  We use the trick of
             * linking a Node that has just been dequeued to itself.  Such a
             * self-link implicitly means to advance to head.
             *
             * Both head and tail are permitted to lag.  In fact, failing to
             * update them every time one could is a significant optimization
             * (fewer CASes). As with LinkedTransferQueue (see the internal
             * documentation for that class), we use a slack threshold of two;
             * that is, we update head/tail when the current pointer appears
             * to be two or more steps away from the first/last node.
             *
             * Since head and tail are updated concurrently and independently,
             * it is possible for tail to lag behind head (why not)?
             *
             * CASing a Node's item reference to null atomically removes the
             * element from the queue, leaving a "dead" node that should later
             * be unlinked (but unlinking is merely an optimization).
             * Interior element removal methods (other than Iterator.remove())
             * keep track of the predecessor node during traversal so that the
             * node can be CAS-unlinked.  Some traversal methods try to unlink
             * any deleted nodes encountered during traversal.  See comments
             * in bulkRemove.
             *
             * When constructing a Node (before enqueuing it) we avoid paying
             * for a volatile write to item.  This allows the cost of enqueue
             * to be "one-and-a-half" CASes.
             *
             * Both head and tail may or may not point to a Node with a
             * non-null item.  If the queue is empty, all items must of course
             * be null.  Upon creation, both head and tail refer to a dummy
             * Node with null item.  Both head and tail are only updated using
             * CAS, so they never regress, although again this is merely an
             * optimization.
             */

            class Node;

            CORE_ALIAS(NODE, typename Class<Node>::Pointer);
            CORE_ALIAS(ITEM, typename Class<E>::Pointer);
            CORE_ALIAS(UNSAFE, Object::UNSAFE);

            /**
             * A node from which the first live (non-deleted) node (if any)
             * can be reached in O(1) time.
             * Invariants: <br/>
             * - all live nodes are reachable from head via succ() <br/>
             * - head != null <br/>
             * - (tmp = head)->next != tmp || tmp != head <br/>
             * Non-invariants: <br/>
             * - head->item may or may not be null. <br/>
             * - it is permitted for tail to lag behind head, that is, for tail
             *   to not be reachable from head!
             */
            NODE volatile mutable head = { };

            /**
             * A node from which the last node on list (that is, the unique
             * node with node->next == null) can be reached in O(1) time.
             * Invariants: <br/>
             * - the last node is always reachable from tail via succ() <br/>
             * - tail != null <br/>
             * Non-invariants: <br/>
             * - tail->item may or may not be null. <br/>
             * - it is permitted for tail to lag behind head, that is, for tail <br/>
             *   to not be reachable from head! <br/>
             * - tail->next may or may not be self-linked.
             */
            NODE volatile mutable tail = { };

        public:
            /**
             * Creates a @c ConcurrentLinkedQueue that is initially empty.
             */
            ConcurrentLinkedQueue() {
                try {
                    head = tail = new Node();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates a @c ConcurrentLinkedQueue
             * initially containing the elements of the given collection,
             * added in traversal order of the collection's iterator.
             *
             * @param c the collection of elements to initially contain
             */
            CORE_EXPLICIT ConcurrentLinkedQueue(util::Collection<E> const& c) {
                NODE h = null, t = null;
                try {
                    for (E const& e : c) {
                        NODE newNode = new Node((E&) util::Objects::requireNonNull(e));
                        if (h == null)
                            h = t = newNode;
                        else
                            t->appendRelaxed(t = newNode);
                    }
                } catch (Throwable const& cme) {
                    // we will delete all allocated node
                    for (NODE x = h, y = null; x != null; x = y) {
                        y = x->next;
                        delete x;
                    }
                    h = t = null;
                    cme.throws($ftrace());
                }
                if (h == null)
                    h = t = new Node();
                head = h;
                tail = t;
            }

            /**
             * Inserts the specified element at the tail of this queue.
             * As the queue is unbounded, this method will never throw
             * @b IllegalStateException or return @c false.
             *
             * @return @c true (as specified by @b Collection::add)
             */
            gbool add(E const& e) override {
                return offer(e);
            }

            /**
             * Inserts the specified element at the tail of this queue.
             * As the queue is unbounded, this method will never return @c false.
             *
             * @return @c true (as specified by @b Queue::offer)
             */
            gbool offer(E const& e) override {
                static CORE_FAST glong TAIL = tailFieldOffset();
                static CORE_FAST glong NEXT = nextFieldOffset();
                NODE newNode = null;
                try {
                    E& val = UNSAFE::copyInstance(util::Objects::requireNonNull(e));
                    newNode = new Node(val);
                } catch (Throwable const& cnse) { cnse.throws($ftrace()); }
                for (NODE t = tail, p = t;;) {
                    NODE q = p->next;
                    if (q == null) {
                        // p is last node
                        if (UNSAFE::compareAndSetReference(toObject(p), NEXT, null, toObject(newNode))) {
                            // Successful CAS is the linearization point
                            // for e to become an element of this queue,
                            // and for newNode to become "live".
                            if (p != t) // hop two nodes at a time; failure is OK
                                UNSAFE::weakCompareAndSetReference(toObject(this), TAIL, toObject(t),
                                                                   toObject(newNode));
                            return true;
                        }
                        // Lost CAS race to another thread; re-read next
                    } else if (q == p)
                        // We have fallen off list.  If tail is unchanged, it
                        // will also be off-list, in which case we need to
                        // jump to head, from which all live nodes are always
                        // reachable.  Else the new tail is a better bet.
                        p = (t != (t = tail)) ? t : head;
                    else
                        // Check for tail updates after two hops.
                        p = (p != t && t != (t = tail)) ? t : q;
                }
            }

            Object& poll() override {
            RESTART_FROM_HEAD:
                for (;;) {
                    for (NODE h = head, p = h, q = null;; p = q) {
                        ITEM item = null;
                        if ((item = p->item) != null && p->casItem(toObject(item), null)) {
                            // Successful CAS is the linearization point
                            // for item to be removed from this queue.
                            if (p != h) // hop two nodes at a time
                                updateHead(h, ((q = p->next) != null) ? q : p);
                            return *item;
                        } else if ((q = p->next) == null) {
                            updateHead(h, p);
                            return null;
                        } else if (p == q)
                            goto RESTART_FROM_HEAD;
                    }
                }
            }

            Object& peek() override {
            RESTART_FROM_HEAD:
                for (;;) {
                    for (NODE h = head, p = h, q = null;; p = q) {
                        ITEM item = null;
                        if ((item = p->item) != null && (q = p->next) == null) {
                            updateHead(h, p);
                            return *item;
                        } else if (q == p)
                            goto RESTART_FROM_HEAD;
                    }
                }
            }

            Object const& peek() const override {
            RESTART_FROM_HEAD:
                for (;;) {
                    for (NODE h = head, p = h, q = null;; p = q) {
                        ITEM item = null;
                        if ((item = p->item) != null && (q = p->next) == null) {
                            updateHead(h, p);
                            return *item;
                        } else if (q == p)
                            goto RESTART_FROM_HEAD;
                    }
                }
            }

            /**
             * Returns @c true if this queue contains no elements.
             *
             * @return @c true if this queue contains no elements
             */
            gbool isEmpty() const override {
                return first() == null;
            }

            /**
             * Returns the number of elements in this queue.  If this queue
             * contains more than @c Integer::MAX_VALUE elements, returns
             * @c Integer::MAX_VALUE.
             *
             * <p>Beware that, unlike in most collections, this method is
             * <em>NOT</em> a constant-time operation. Because of the
             * asynchronous nature of these queues, determining the current
             * number of elements requires an O(n) traversal.
             * Additionally, if elements are added or removed during execution
             * of this method, the returned result may be inaccurate.  Thus,
             * this method is typically not very useful in concurrent
             * applications.
             *
             * @return the number of elements in this queue
             */
            gint size() const override {
            RESTART_FROM_HEAD:
                for (;;) {
                    gint count = 0;
                    for (NODE p = first(); p != null;) {
                        if (p->item != null) {
                            if (++count == Integer::MAX_VALUE)
                                break; // @see Collection.size()
                        } else if (p == (p = p->next))
                            goto RESTART_FROM_HEAD;
                    }
                    return count;
                }
            }

            /**
             * Returns @c true if this queue contains the specified element.
             * More formally, returns @c true if and only if this queue contains
             * at least one element @c e such that @c o.equals(e).
             *
             * @param val object to be checked for containment in this queue
             * @return @c true if this queue contains the specified element
             */
            gbool contains(E const& val) const override {
                if (val == null) return false;
            RESTART_FROM_HEAD:
                for (;;) {
                    for (NODE p = head, pred = null; p != null;) {
                        NODE q = p->next;
                        ITEM item = null;
                        if ((item = p->item) != null) {
                            if (val.equals(toObject(item)))
                                return true;
                            pred = p;
                            p = q;
                            continue;
                        }
                        for (NODE c = p;; q = p->next) {
                            if (q == null || q->item != null) {
                                pred = skipDeadNodes(pred, c, p, q);
                                p = q;
                                break;
                            }
                            if (p == (p = q))
                                goto RESTART_FROM_HEAD;
                        }
                    }
                    return false;
                }
            }

            /**
             * Removes a single instance of the specified element from this queue,
             * if it is present.  More formally, removes an element @c e such
             * that @c o.equals(e), if this queue contains one or more such
             * elements.
             * Returns @c true if this queue contained the specified element
             * (or equivalently, if this queue changed as a result of the call).
             *
             * @param o element to be removed from this queue, if present
             * @return @c true if this queue changed as a result of the call
             */
            gbool remove(E const& o) override {
                if (o == null)
                    return false;
            RESTART_FROM_HEAD:
                for (;;) {
                    for (NODE p = head, pred = null; p != null;) {
                        NODE q = p->next;
                        ITEM item = null;
                        if ((item = p->item) != null) {
                            if (o.equals(toObject(item)) && p->casItem(toObject(item), null)) {
                                skipDeadNodes(pred, p, p, q);
                                return true;
                            }
                            pred = p;
                            p = q;
                            continue;
                        }
                        for (NODE c = p;; q = p->next) {
                            if (q == null || q->item != null) {
                                pred = skipDeadNodes(pred, c, p, q);
                                p = q;
                                break;
                            }
                            if (p == (p = q))
                                goto RESTART_FROM_HEAD;
                        }
                    }
                    return false;
                }
            }

            /**
             * Appends all the elements in the specified collection to the end of
             * this queue, in the order that they are returned by the specified
             * collection's iterator.  Attempts to @c addAll of a queue to
             * itself result in @c IllegalArgumentException.
             *
             * @param c the elements to be inserted into this queue
             * @return @c true if this queue changed as a result of the call
             * @throws IllegalArgumentException if the collection is this queue
             */
            gbool addAll(util::Collection<E> const& c) override {
                static CORE_FAST glong TAIL = tailFieldOffset();
                static CORE_FAST glong NEXT = nextFieldOffset();
                if (this == &c)
                    IllegalArgumentException().throws($ftrace());

                // Copy c into a private chain of Nodes
                NODE beginningOfTheEnd = null, last = null;
                try {
                    for (E const& e : c) {
                        NODE newNode = new Node(UNSAFE::copyInstance(util::Objects::requireNonNull(e)));
                        if (beginningOfTheEnd == null)
                            beginningOfTheEnd = last = newNode;
                        else
                            last->appendRelaxed(last = newNode);
                    }
                } catch (Throwable const& cnse) {
                    for (NODE x = beginningOfTheEnd, y = null; x != null; x = y) {
                        y = x->next;
                        delete x;
                    }
                    cnse.throws($ftrace());
                }
                if (beginningOfTheEnd == null)
                    return false;

                // Atomically append the chain at the tail of this collection
                for (NODE t = tail, p = t;;) {
                    NODE q = p->next;
                    if (q == null) {
                        // p is last node
                        if (UNSAFE::compareAndSetReference(toObject(p), NEXT, null, toObject(beginningOfTheEnd))) {
                            // Successful CAS is the linearization point
                            // for all elements to be added to this queue.
                            if (!UNSAFE::weakCompareAndSetReference(toObject(this), TAIL, toObject(t),
                                                                    toObject(last))) {
                                // Try a little harder to update tail,
                                // since we may be adding many elements.
                                t = tail;
                                if (last->next == null)
                                    UNSAFE::weakCompareAndSetReference(toObject(this), TAIL, toObject(t),
                                                                       toObject(last));
                            }
                            return true;
                        }
                        // Lost CAS race to another thread; re-read next
                    } else if (p == q)
                        // We have fallen off list.  If tail is unchanged, it
                        // will also be off-list, in which case we need to
                        // jump to head, from which all live nodes are always
                        // reachable.  Else the new tail is a better bet.
                        p = (t != (t = tail)) ? t : head;
                    else
                        // Check for tail updates after two hops.
                        p = (p != t && t != (t = tail)) ? t : q;
                }
            }

            String toString() const override {
                StringArray a;
            RESTART_FROM_HEAD:
                for (;;) {
                    gint charLength = 0;
                    gint size = 0;
                    for (NODE p = first(); p != null;) {
                        ITEM item = null;
                        if ((item = p->item) != null) {
                            if (a.isEmpty())
                                a = StringArray(4);
                            else if (size == a.length())
                                a = util::Arrays::copyOf(a, 2 * size);
                            String s = item->toString();
                            a[size++] = s;
                            charLength += s.length();
                        }
                        if (p == (p = p->next))
                            goto RESTART_FROM_HEAD;
                    }

                    if (size == 0)
                        return "[]";

                    return toStringImpl(a, size, charLength);
                }
            }

            /**
             * Returns an array containing all of the elements in this queue, in
             * proper sequence.
             *
             * <p>The returned array will be "safe" in that no references to it are
             * maintained by this queue.  (In other words, this method must allocate
             * a new array).  The caller is thus free to modify the returned array.
             *
             * <p>This method acts as bridge between array-based and collection-based
             * APIs.
             *
             * @return an array containing all of the elements in this queue
             */
            Array<E> toArray() const override {
                return (Array<E>) toArrayImpl();
            }

            /**
             * Returns an iterator over the elements in this queue in proper sequence.
             * The elements will be returned in order from first (head) to last (tail).
             *
             * <p>The returned iterator is
             * <a href="package-summary.html#Weakly"><i>weakly consistent</i></a>.
             *
             * @return an iterator over the elements in this queue in proper sequence
             */
            util::Iterator<E>& iterator() override {
                static CORE_FAST glong NEXT = nextFieldOffset();
                class Itr final : public util::Iterator<E> {
                    ConcurrentLinkedQueue& self;
                    /**
                     * Next node to return item for.
                     */
                    NODE nextNode = { };

                    /**
                     * nextItem holds on to item fields because once we claim
                     * that an element exists in hasNext(), we must return it in
                     * the following next() call even if it was in the process of
                     * being removed when hasNext() was called.
                     */
                    ITEM nextItem = { };

                    /**
                     * Node of the last returned item, to support remove.
                     */
                    NODE last = { };

                public:
                    CORE_EXPLICIT Itr(ConcurrentLinkedQueue& self): self(self) {
                    RESTART_FROM_HEAD:
                        for (;;) {
                            NODE h = null, p = null, q = null;
                            for (p = h = self.head;; p = q) {
                                ITEM item = null;
                                if ((item = p->item) != null) {
                                    nextNode = p;
                                    nextItem = item;
                                    break;
                                } else if ((q = p->next) == null)
                                    break;
                                else if (p == q)
                                    goto RESTART_FROM_HEAD;
                            }
                            self.updateHead(h, p);
                            return;
                        }
                    }

                    gbool hasNext() const override {
                        return nextItem != null;
                    }

                    E& next() override {
                        NODE pred = nextNode;
                        if (pred == null)
                            util::NoSuchElementException().throws($ftrace());
                        // assert nextItem != null;
                        last = pred;
                        ITEM item = null;

                        for (NODE p = self.succ(pred), q;; p = q) {
                            if (p == null || (item = p->item) != null) {
                                nextNode = p;
                                ITEM x = nextItem;
                                nextItem = item;
                                return *x;
                            }
                            // unlink deleted nodes
                            if ((q = self.succ(p)) != null)
                                UNSAFE::compareAndSetReference(toObject(pred), NEXT, toObject(p), toObject(q));
                        }
                    }

                    // Default implementation of forEach is "good enough".

                    void remove() override {
                        NODE l = last;
                        if (l == null)
                            IllegalStateException().throws($ftrace());
                        // rely on a future traversal to relink.
                        l->item = null;
                        last = null;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &self == &it.self && nextNode == it.nextNode;
                    }
                };

                return UNSAFE::newInstance<Itr>(*this);
            }

            /**
             * Returns an iterator over the elements in this queue in proper sequence.
             * The elements will be returned in order from first (head) to last (tail).
             *
             * <p>The returned iterator is
             * <a href="package-summary.html#Weakly"><i>weakly consistent</i></a>.
             *
             * @return an iterator over the elements in this queue in proper sequence
             */
            util::Iterator2<E>& iterator() const override {
                static CORE_FAST glong NEXT = nextFieldOffset();
                class Itr final : public util::Iterator2<E> {
                    ConcurrentLinkedQueue const& self;
                    /**
                     * Next node to return item for.
                     */
                    NODE nextNode = { };

                    /**
                     * nextItem holds on to item fields because once we claim
                     * that an element exists in hasNext(), we must return it in
                     * the following next() call even if it was in the process of
                     * being removed when hasNext() was called.
                     */
                    ITEM nextItem = { };

                    /**
                     * Node of the last returned item, to support remove.
                     */
                    NODE last = { };

                public:
                    CORE_EXPLICIT Itr(ConcurrentLinkedQueue const& self): self(self) {
                    RESTART_FROM_HEAD:
                        for (;;) {
                            NODE h = null, p = null, q = null;
                            for (p = h = self.head;; p = q) {
                                ITEM item = null;
                                if ((item = p->item) != null) {
                                    nextNode = p;
                                    nextItem = item;
                                    break;
                                } else if ((q = p->next) == null)
                                    break;
                                else if (p == q)
                                    goto RESTART_FROM_HEAD;
                            }
                            self.updateHead(h, p);
                            return;
                        }
                    }

                    gbool hasNext() const override {
                        return nextItem != null;
                    }

                    E& next() override {
                        NODE pred = nextNode;
                        if (pred == null)
                            util::NoSuchElementException().throws($ftrace());
                        // assert nextItem != null;
                        last = pred;
                        ITEM item = null;

                        for (NODE p = self.succ(pred), q;; p = q) {
                            if (p == null || (item = p->item) != null) {
                                nextNode = p;
                                ITEM x = nextItem;
                                nextItem = item;
                                return *x;
                            }
                            // unlink deleted nodes
                            if ((q = self.succ(p)) != null)
                                UNSAFE::compareAndSetReference(toObject(pred), NEXT, toObject(p), toObject(q));
                        }
                    }

                    // Default implementation of forEach is "good enough".

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Itr>::hasInstance(other))
                            return false;
                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &self == &it.self && nextNode == it.nextNode;
                    }
                };

                return UNSAFE::newInstance<Itr>(*this);
            }

            /**
             * Returns a @b Spliterator over the elements in this queue.
             *
             * <p>
             * The returned spliterator is <i>weakly consistent</i>.
             * </p>
             * <p>
             * The @c Spliterator reports @b Spliterator::CONCURRENT,
             * @b Spliterator::ORDERED, and @b Spliterator::NONNULL.
             * </p>
             * @note
             * The @c Spliterator implements @c trySplit to permit limited
             * parallelism.
             *
             * @return a @c Spliterator over the elements in this queue
             */
            util::Spliterator<E>& spliterator() override {
                static CORE_FAST gint MAX_BATCH = 1 << 25; // max batch array size;
                class Spl final : public util::Spliterator<E> {
                    ConcurrentLinkedQueue& self;
                    NODE mutable current = { }; // current node; null until initialized
                    gint mutable batch = 0; // batch size for splits
                    gbool mutable exhausted = false; // true when no more nodes

                public:
                    CORE_EXPLICIT Spl(ConcurrentLinkedQueue& self): self(self) {}

                    util::Spliterator<E>& trySplit() override {
                        NODE p = null, q = null;
                        if ((p = getCurrent()) == null || (q = p->next) == null)
                            return *this;
                        gint i = 0, n = batch = Math::min(batch + 1, MAX_BATCH);
                        Array<E> a;
                        do {
                            ITEM e = null;
                            if ((e = p->item) != null) {
                                if (a.isEmpty())
                                    a = Array<E>(n);
                                util::Arrays::fastSet(a, i++, *e);
                            }
                            if (p == (p = q))
                                p = self.first();
                        } while (p != null && (q = p->next) != null && i < n);
                        setCurrent(p);
                        if (i == 0)
                            return *this;
                        else
                            return util::Spliterators::spliterator<E>(a, 0, i, util::Spliterator<>::ORDERED |
                                                                      util::Spliterator<>::NONNULL |
                                                                      util::Spliterator<>::CONCURRENT);
                    }

                    util::Spliterator<E> const& trySplit() const override {
                        NODE p = null, q = null;
                        if ((p = getCurrent()) == null || (q = p->next) == null)
                            return *this;
                        gint i = 0, n = batch = Math::min(batch + 1, MAX_BATCH);
                        Array<E> a;
                        do {
                            ITEM e = null;
                            if ((e = p->item) != null) {
                                if (a.isEmpty())
                                    a = Array<E>(n);
                                util::Arrays::fastSet(a, i++, *e);
                            }
                            if (p == (p = q))
                                p = self.first();
                        } while (p != null && (q = p->next) != null && i < n);
                        setCurrent(p);
                        if (i == 0)
                            return *this;
                        else
                            return util::Spliterators::spliterator<E>(a, 0, i, util::Spliterator<>::ORDERED |
                                                                      util::Spliterator<>::NONNULL |
                                                                      util::Spliterator<>::CONCURRENT);
                    }

                    void forEach(function::Consumer<E&> const& action) override {
                        NODE p = null;
                        if ((p = getCurrent()) != null) {
                            current = null;
                            exhausted = true;
                            try {
                                self.forEachImpl(action, p);
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }
                    }

                    gbool tryAdvance(function::Consumer<E&> const& action) override {
                        NODE p = null;
                        if ((p = getCurrent()) != null) {
                            ITEM e = null;
                            do {
                                e = p->item;
                                if (p == (p = p->next))
                                    p = self.first();
                            } while (e == null && p != null);
                            setCurrent(p);
                            if (e != null) {
                                action.accept(*e);
                                return true;
                            }
                        }
                        return false;
                    }

                    glong estimateSize() const override {
                        return Long::MAX_VALUE;
                    }

                    gint characteristics() const override {
                        return util::Spliterator<>::ORDERED |
                                util::Spliterator<>::NONNULL |
                                util::Spliterator<>::CONCURRENT;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Spl>::hasInstance(other))
                            return false;
                        Spl const& spl = CORE_XCAST(Spl const, other);
                        return &self == &spl.self && current == spl.current;
                    }

                private:
                    NODE getCurrent() const {
                        NODE p = null;
                        if ((p = current) == null && !exhausted)
                            setCurrent(p = self.first());
                        return p;
                    }

                    void setCurrent(NODE p) const {
                        if ((current = p) == null)
                            exhausted = true;
                    }
                };

                return UNSAFE::newInstance<Spl>(*this);
            }

            /**
             * Returns a @b Spliterator over the elements in this queue.
             *
             * <p>
             * The returned spliterator is <i>weakly consistent</i>.
             * </p>
             * <p>
             * The @c Spliterator reports @b Spliterator::CONCURRENT,
             * @b Spliterator::ORDERED, and @b Spliterator::NONNULL.
             * </p>
             * @note
             * The @c Spliterator implements @c trySplit to permit limited
             * parallelism.
             *
             * @return a @c Spliterator over the elements in this queue
             */
            util::Spliterator2<E>& spliterator() const override {
                static CORE_FAST gint MAX_BATCH = 1 << 25; // max batch array size;
                class Spl final : public util::Spliterator2<E> {
                    ConcurrentLinkedQueue const& self;
                    NODE mutable current = { }; // current node; null until initialized
                    gint mutable batch = 0; // batch size for splits
                    gbool mutable exhausted = false; // true when no more nodes

                public:
                    CORE_EXPLICIT Spl(ConcurrentLinkedQueue const& self): self(self) {}

                    util::Spliterator2<E>& trySplit() override {
                        NODE p = null, q = null;
                        if ((p = getCurrent()) == null || (q = p->next) == null)
                            return *this;
                        gint i = 0, n = batch = Math::min(batch + 1, MAX_BATCH);
                        Array<E> a;
                        do {
                            ITEM e = null;
                            if ((e = p->item) != null) {
                                if (a.isEmpty())
                                    a = Array<E>(n);
                                util::Arrays::fastSet(a, i++, *e);
                            }
                            if (p == (p = q))
                                p = self.first();
                        } while (p != null && (q = p->next) != null && i < n);
                        setCurrent(p);
                        if (i == 0)
                            return *this;
                        else
                            return util::Spliterators::spliterator2<E>(a, 0, i, util::Spliterator<>::ORDERED |
                                                                       util::Spliterator<>::NONNULL |
                                                                       util::Spliterator<>::CONCURRENT);
                    }

                    util::Spliterator2<E> const& trySplit() const override {
                        NODE p = null, q = null;
                        if ((p = getCurrent()) == null || (q = p->next) == null)
                            return *this;
                        gint i = 0, n = batch = Math::min(batch + 1, MAX_BATCH);
                        Array<E> a;
                        do {
                            ITEM e = null;
                            if ((e = p->item) != null) {
                                if (a.isEmpty())
                                    a = Array<E>(n);
                                util::Arrays::fastSet(a, i++, *e);
                            }
                            if (p == (p = q))
                                p = self.first();
                        } while (p != null && (q = p->next) != null && i < n);
                        setCurrent(p);
                        if (i == 0)
                            return *this;
                        else
                            return util::Spliterators::spliterator2<E>(a, 0, i, util::Spliterator<>::ORDERED |
                                                                       util::Spliterator<>::NONNULL |
                                                                       util::Spliterator<>::CONCURRENT);
                    }

                    void forEach(function::Consumer<E> const& action) override {
                        NODE p = null;
                        if ((p = getCurrent()) != null) {
                            current = null;
                            exhausted = true;
                            try {
                                self.forEachImpl(action, p);
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }
                    }

                    gbool tryAdvance(function::Consumer<E> const& action) override {
                        NODE p = null;
                        if ((p = getCurrent()) != null) {
                            ITEM e = null;
                            do {
                                e = p->item;
                                if (p == (p = p->next))
                                    p = self.first();
                            } while (e == null && p != null);
                            setCurrent(p);
                            if (e != null) {
                                action.accept(*e);
                                return true;
                            }
                        }
                        return false;
                    }

                    glong estimateSize() const override {
                        return Long::MAX_VALUE;
                    }

                    gint characteristics() const override {
                        return util::Spliterator<>::ORDERED |
                                util::Spliterator<>::NONNULL |
                                util::Spliterator<>::CONCURRENT;
                    }

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        if (!Class<Spl>::hasInstance(other))
                            return false;
                        Spl const& spl = CORE_XCAST(Spl const, other);
                        return &self == &spl.self && current == spl.current;
                    }

                private:
                    NODE getCurrent() const {
                        NODE p = null;
                        if ((p = current) == null && !exhausted)
                            setCurrent(p = self.first());
                        return p;
                    }

                    void setCurrent(NODE p) const {
                        if ((current = p) == null)
                            exhausted = true;
                    }
                };

                return UNSAFE::newInstance<Spl>(*this);
            }

            gbool removeIf(function::Predicate<E> const& filter) override {
                try {
                    return bulkRemove(filter);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeAll(util::Collection<E> const& c) override {
                try {
                    return bulkRemove([&](E const& e) -> gbool { return c.contains(e); });
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(util::Collection<E> const& c) override {
                try {
                    return bulkRemove([&](E const& e) -> gbool { return !c.contains(e); });
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    bulkRemove([&](E const& e) -> gbool { return true; });
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<E&> const& action) override {
                try {
                    forEachImpl(action, head);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<E> const& action) const override {
                try {
                    forEachImpl(action, head);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~ConcurrentLinkedQueue() override {
                static CORE_FAST glong HEAD = headFieldOffset();
                static CORE_FAST glong TAIL = tailFieldOffset();
                NODE h = null;
                do {
                    h = head;
                    UNSAFE::putReferenceRelease(toObject(this), TAIL, null);
                } while (h != null && !UNSAFE::compareAndSetReference(toObject(this), HEAD, toObject(h), null));
                for (NODE x = h, y = null; x != null; x = y) {
                    y = x->next;
                    delete x;
                }
            }

        private:
            /**
             * Tries to CAS head to p. If successful, repoint old head to itself
             * as sentinel for succ(), below.
             */
            void updateHead(NODE h, NODE p) const {
                // CORE_ASSERT(h != null && p != null && (h == p || h->item == null));
                static CORE_FAST glong HEAD = headFieldOffset();
                static CORE_FAST glong NEXT = nextFieldOffset();
                if (h != p && UNSAFE::compareAndSetReference(toObject(this), HEAD, toObject(h), toObject(p)))
                    UNSAFE::putReferenceRelease(toObject(head), NEXT, toObject(h));
            }

            /**
             * Returns the successor of p, or the head node if p->next has been
             * linked to self, which will only be true if traversing with a
             * stale pointer that is now off the list.
             */
            NODE succ(NODE p) const {
                if (p == (p = p->next))
                    p = head;
                return p;
            }

            /**
             * Returns the first live (non-deleted) node on list, or null if none.
             * This is yet another variant of poll/peek; here returning the
             * first node, not element.  We could make peek() a wrapper around
             * first(), but that would cost an extra volatile read of item,
             * and the need to add a retry loop to deal with the possibility
             * of losing a race to a concurrent poll().
             */
            NODE first() const {
            RESTART_FROM_HEAD:
                for (;;) {
                    for (NODE h = head, p = h, q = null;; p = q) {
                        gbool hasItem = p->item != null;
                        if (hasItem || (q = p->next) == null) {
                            updateHead(h, p);
                            return hasItem ? p : null;
                        } else if (p == q)
                            goto RESTART_FROM_HEAD;
                    }
                }
            }

            /**
             * Tries to CAS pred->next (or head, if pred is null) from c to p.
             * Caller must ensure that we're not unlinking the trailing node.
             */
            gbool tryCasSuccessor(NODE pred, NODE c, NODE p) const {
                // CORE_ASSERT(p != null);
                // CORE_ASSERT(c->item == null);
                // CORE_ASSERT(c != p);
                static CORE_FAST glong HEAD = headFieldOffset();
                static CORE_FAST glong NEXT = nextFieldOffset();
                if (pred != null)
                    return UNSAFE::compareAndSetReference(toObject(pred), NEXT, toObject(c), toObject(p));
                if (UNSAFE::compareAndSetReference(toObject(this), HEAD, toObject(c), toObject(p))) {
                    UNSAFE::putReferenceRelease(toObject(c), NEXT, toObject(c));
                    return true;
                }
                return false;
            }

            /**
             * Collapse dead nodes between pred and q.
             * @param pred the last known live node, or null if none
             * @param c the first dead node
             * @param p the last dead node
             * @param q p->next: the next live node, or null if at end
             * @return either old pred or p if pred dead or CAS failed
             */
            NODE skipDeadNodes(NODE pred, NODE c, NODE p, NODE q) const {
                // CORE_ASSERT(pred != c);
                // CORE_ASSERT(p != q);
                // CORE_ASSERT(c->item == null);
                // CORE_ASSERT(p->item == null);
                static CORE_FAST glong ITEM = itemFieldOffset();
                if (q == null) {
                    // Never unlink trailing node.
                    if (c == p)
                        return pred;
                    q = p;
                }
                return (tryCasSuccessor(pred, c, p) &&
                           (pred == null || UNSAFE::getReference(toObject(pred), ITEM) != null))
                           ? pred
                           : p;
            }

            /**
             * Like Arrays::toString(), but caller guarantees that size > 0,
             * each element with index 0 <= i < size is a non-null String,
             * and charLength is the sum of the lengths of the input Strings.
             */
            static String toStringImpl(StringArray const& a, gint size, gint charLength) {
                CORE_ASSERT(size > 0);

                // Copy each string into a perfectly sized char[]
                // Length of [ , , , ] == 2 * size
                CharArray chars = CharArray(charLength + 2 * size);
                chars[0] = '[';
                gint j = 1;
                for (gint i = 0; i < size; i++) {
                    if (i > 0) {
                        chars[j++] = ',';
                        chars[j++] = ' ';
                    }
                    String const& s = a[i];
                    gint len = s.length();
                    s.toChars(0, len, chars, j);
                    j += len;
                }
                chars[j] = ']';
                // CORE_ASSERT(j == chars.length() - 1);
                return String(chars);
            }

            Array<> toArrayImpl() const {
                Array<> x;
            RESTART_FROM_HEAD:
                for (;;) {
                    gint size = 0;
                    for (NODE p = first(); p != null;) {
                        ITEM item = null;
                        if ((item = p->item) != null) {
                            if (x.isEmpty())
                                x = Array<>(4);
                            else if (size == x.length())
                                x = util::Arrays::copyOf(x, 2 * (size + 4));
                            util::Arrays::fastSet(x, size++, toObject(item));
                        }
                        if (p == (p = p->next))
                            goto RESTART_FROM_HEAD;
                    }
                    return (size == x.length()) ? x : util::Arrays::copyOf(x, size);
                }
            }

            /**
             * Tolerate this many consecutive dead nodes before CAS-collapsing.
             * Amortized cost of clear() is (1 + 1/MAX_HOPS) CASes per element.
             */
            static CORE_FAST gint MAX_HOPS = 8;

            /** Implementation of bulk remove methods. */
            gbool bulkRemove(function::Predicate<E> const& filter) {
                gbool removed = false;
            RESTART_FROM_HEAD:
                for (;;) {
                    int hops = MAX_HOPS;
                    // c will be CASed to collapse intervening dead nodes between
                    // pred (or head if null) and p.
                    for (NODE p = head, c = p, pred = null, q = null; p != null; p = q) {
                        q = p->next;
                        ITEM item = null;
                        gbool pAlive = false;
                        if ((pAlive = ((item = p->item) != null)) == true) {
                            try {
                                if (filter.test(*item)) {
                                    if (p->casItem(toObject(item), null))
                                        removed = true;
                                    pAlive = false;
                                }
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }
                        if (pAlive || q == null || --hops == 0) {
                            // p might already be self-linked here, but if so:
                            // - CASing head will surely fail
                            // - CASing pred's next will be useless but harmless.
                            if ((c != p && !tryCasSuccessor(pred, c, c = p)) || pAlive) {
                                // if CAS failed or alive, abandon old pred
                                hops = MAX_HOPS;
                                pred = p;
                                c = q;
                            }
                        } else if (p == q)
                            goto RESTART_FROM_HEAD;
                    }
                    return removed;
                }
            }

            /**
             * Runs action on each element found during a traversal starting at p.
             * If p is null, the action is not run.
             */
            template <class T>
            void forEachImpl(function::Consumer<T> const& action, NODE p) const {
                for (NODE pred = null; p != null;) {
                    NODE q = p->next;
                    ITEM item = null;
                    if ((item = p->item) != null) {
                        try {
                            action.accept(*item);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        pred = p;
                        p = q;
                        continue;
                    }
                    for (NODE c = p;; q = p->next) {
                        if (q == null || q->item != null) {
                            pred = skipDeadNodes(pred, c, p, q);
                            p = q;
                            break;
                        }
                        if (p == (p = q)) {
                            pred = null;
                            p = head;
                            break;
                        }
                    }
                }
            }

            template <class T, class TRef = typename Class<T>::Slim, class Ref = typename Class<TRef>::NoPointer>
            static Object& toObject(T ref) {
                if (ref == null)
                    return null;
                return (Ref&) *ref;
            }

            static CORE_FAST glong headFieldOffset() { return CORE_FIELD_OFFSET(ConcurrentLinkedQueue, head); }

            static CORE_FAST glong tailFieldOffset() { return CORE_FIELD_OFFSET(ConcurrentLinkedQueue, tail); }

            static CORE_FAST glong itemFieldOffset() { return CORE_FIELD_OFFSET(Node, item); }

            static CORE_FAST glong nextFieldOffset() { return CORE_FIELD_OFFSET(Node, next); }
        };

        template <class E>
        class ConcurrentLinkedQueue<E>::Node final : public Object {
        public: // for offset checking
            ITEM volatile item = { };
            NODE volatile next = { };

            /**
             * Constructs a node holding item.  Uses relaxed write because
             * item can only be seen after piggybacking publication via CAS.
             */
            CORE_EXPLICIT Node(E& item) {
                static CORE_FAST glong ITEM = itemFieldOffset();
                UNSAFE::putReference(toObject(this), ITEM, item);
            }

            /**
             * Constructs a dead dummy node.
             *
             */
            Node() = default;

            void appendRelaxed(NODE next) {
                // CORE_ASSERT(next != null);
                // CORE_ASSERT(Node::next == null);
                static CORE_FAST glong NEXT = nextFieldOffset();
                UNSAFE::putReference(toObject(this), NEXT, toObject(next));
            }

            gbool casItem(Object const& cmp, Object& val) {
                // CORE_ASSERT(item == null || &cmp == item);
                // CORE_ASSERT(cmp != null);
                // CORE_ASSERT(cmp == val);
                static CORE_FAST glong ITEM = itemFieldOffset();
                return UNSAFE::compareAndSetReference(toObject(this), ITEM, cmp, val);
            }

            ~Node() override {
                item = null;
                next = null;
            }
        };

        CORE_WARNING_POP
    } // concurrent
} // core

#endif //CORE24_CONCURRENTLINKEDQUEUE_H
