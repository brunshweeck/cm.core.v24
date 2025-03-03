//
// Created by admin on 26/01/25.
//

#ifndef CORE24_CONCURRENTLINKEDDEQUE_H
#define CORE24_CONCURRENTLINKEDDEQUE_H

#include <core/lang/StringArray.h>
#include <core/util/Deque.h>
#include <core/util/Objects.h>

namespace core {
  namespace concurrent {
    CORE_WARNING_PUSH
    CORE_WARNING_DISABLE_DEPRECATED
    CORE_WARNING_DISABLE_INVALID_OFFSETOF

    /**
     * An unbounded concurrent @em deque based on linked nodes.
     * Concurrent insertion, removal, and access operations execute safely
     * across multiple threads.
     * A @c ConcurrentLinkedDeque is an appropriate choice when
     * many threads will share access to a common collection.
     * Like most other concurrent collection implementations, this class
     * does not permit the use of @c null elements.
     *
     * <p>
     * Iterators and spliterators are <i>weakly consistent</i>.
     * </p>
     * <p>
     * Beware that, unlike in most collections, the @c size method
     * is <em>NOT</em> a constant-time operation. Because of the
     * asynchronous nature of these deques, determining the current number
     * of elements requires a traversal of the elements, and so may report
     * inaccurate results if this collection is modified during traversal.
     * </p>
     * <p>
     * Bulk operations that add, remove, or examine multiple elements,
     * such as @b addAll, @b removeIf or @b forEach,
     * are <em>not</em> guaranteed to be performed atomically.
     * For example, a @c forEach traversal concurrent with an
     * @c addAll operation might observe only some of the added elements.
     * </p>
     * <p>
     * This class and its iterator implement all the <em>optional</em>
     * methods of the @b Deque and @b Iterator interfaces.
     * </p>
     * <p>
     * Memory consistency effects: As with other concurrent collections,
     * actions in a thread prior to placing an object into a
     * @c ConcurrentLinkedDeque <i>happen-before</i>
     * actions after the access or removal of that element from
     * the @c ConcurrentLinkedDeque in another thread.
     * </p>
     *
     * @tparam E the type of elements held in this deque
     */
    template <class E>
    class ConcurrentLinkedDeque final : public util::Deque<E> {
      /*
       * This is an implementation of a concurrent lock-free deque
       * supporting interior removes but not interior insertions, as
       * required to support the entire Deque interface.
       *
       * We extend the techniques developed for ConcurrentLinkedQueue and
       * LinkedTransferQueue (see the internal docs for those classes).
       * Understanding the ConcurrentLinkedQueue implementation is a
       * prerequisite for understanding the implementation of this class.
       *
       * The data structure is a symmetrical doubly-linked "GC-robust"
       * linked list of nodes.  We minimize the number of volatile writes
       * using two techniques: advancing multiple hops with a single CAS
       * and mixing volatile and non-volatile writes of the same memory
       * locations.
       *
       * A node contains the expected E ("item") and links to predecessor
       * ("prev") and successor ("next") nodes:
       *
       * class Node<E> { volatile Node<E> prev, next; volatile E item; }
       *
       * A node p is considered "live" if it contains a non-null item
       * (p->item != null).  When an item is CASed to null, the item is
       * atomically logically deleted from the collection.
       *
       * At any time, there is precisely one "first" node with a null
       * prev reference that terminates any chain of prev references
       * starting at a live node.  Similarly, there is precisely one
       * "last" node terminating any chain of next references starting at
       * a live node.  The "first" and "last" nodes may or may not be live.
       * The "first" and "last" nodes are always mutually reachable.
       *
       * A new element is added atomically by CASing the null prev or
       * next reference in the first or last node to a fresh node
       * containing the element.  The element's node atomically becomes
       * "live" at that point.
       *
       * A node is considered "active" if it is a live node, or the
       * first or last node.  Active nodes cannot be unlinked.
       *
       * A "self-link" is a next or prev reference that is the same node:
       *   p->prev == p  or  p->next == p
       * Self-links are used in the node unlinking process.  Active nodes
       * never have self-links.
       *
       * A node p is active if and only if:
       *
       * p->item != null ||
       * (p->prev == null && p->next != p) ||
       * (p->next == null && p->prev != p)
       *
       * The deque object has two node references, "head" and "tail".
       * The head and tail are only approximations to the first and last
       * nodes of the deque.  The first node can always be found by
       * following prev pointers from head; likewise for tail.  However,
       * it is permissible for head and tail to be referring to deleted
       * nodes that have been unlinked and so may not be reachable from
       * any live node.
       *
       * There are 3 stages of node deletion;
       * "logical deletion", "unlinking", and "gc-unlinking".
       *
       * 1. "logical deletion" by CASing item to null atomically removes
       * the element from the collection, and makes the containing node
       * eligible for unlinking.
       *
       * 2. "unlinking" makes a deleted node unreachable from active
       * nodes, and thus eventually reclaimable by GC.  Unlinked nodes
       * may remain reachable indefinitely from an iterator.
       *
       * Physical node unlinking is merely an optimization (albeit a
       * critical one), and so can be performed at our convenience.  At
       * any time, the set of live nodes maintained by prev and next
       * links are identical, that is, the live nodes found via next
       * links from the first node is equal to the elements found via
       * prev links from the last node.  However, this is not true for
       * nodes that have already been logically deleted - such nodes may
       * be reachable in one direction only.
       *
       * 3. "gc-unlinking" takes unlinking further by making active
       * nodes unreachable from deleted nodes, making it easier for the
       * GC to reclaim future deleted nodes.  This step makes the data
       * structure "gc-robust", as first described in detail by Boehm
       * (http://portal.acm.org/citation.cfm?doid=503272.503282).
       *
       * GC-unlinked nodes may remain reachable indefinitely from an
       * iterator, but unlike unlinked nodes, are never reachable from
       * head or tail.
       *
       * Making the data structure GC-robust will eliminate the risk of
       * unbounded memory retention with conservative GCs and is likely
       * to improve performance with generational GCs.
       *
       * When a node is dequeued at either end, e.g. via poll(), we would
       * like to break any references from the node to active nodes.  We
       * develop further the use of self-links that was very effective in
       * other concurrent collection classes.  The idea is to replace
       * prev and next pointers with special values that are interpreted
       * to mean off-the-list-at-one-end.  These are approximations, but
       * good enough to preserve the properties we want in our
       * traversals, e.g. we guarantee that a traversal will never visit
       * the same element twice, but we don't guarantee whether a
       * traversal that runs out of elements will be able to see more
       * elements later after enqueues at that end.  Doing gc-unlinking
       * safely is particularly tricky, since any node can be in use
       * indefinitely (for example by an iterator).  We must ensure that
       * the nodes pointed at by head/tail never get gc-unlinked, since
       * head/tail are needed to get "back on track" by other nodes that
       * are gc-unlinked.  gc-unlinking accounts for much of the
       * implementation complexity.
       *
       * Since neither unlinking nor gc-unlinking are necessary for
       * correctness, there are many implementation choices regarding
       * frequency (eagerness) of these operations.  Since volatile
       * reads are likely to be much cheaper than CASes, saving CASes by
       * unlinking multiple adjacent nodes at a time may be a win.
       * gc-unlinking can be performed rarely and still be effective,
       * since it is most important that long chains of deleted nodes
       * are occasionally broken.
       *
       * The actual representation we use is that p->next == p means to
       * goto the first node (which in turn is reached by following prev
       * pointers from head), and p->next == null && p->prev == p means
       * that the iteration is at an end and that p is a (static final)
       * dummy node, NEXT_TERMINATOR, and not the last active node.
       * Finishing the iteration when encountering such a TERMINATOR is
       * good enough for read-only traversals, so such traversals can use
       * p->next == null as the termination condition.  When we need to
       * find the last (active) node, for enqueueing a new node, we need
       * to check whether we have reached a TERMINATOR node; if so,
       * restart traversal from tail.
       *
       * The implementation is completely directionally symmetrical,
       * except that most public methods that iterate through the list
       * follow next pointers, in the "forward" direction.
       *
       * We believe (without full proof) that all single-element Deque
       * operations that operate directly at the two ends of the Deque
       * (e.g., addFirst, peekLast, pollLast) are linearizable (see
       * Herlihy and Shavit's book).  However, some combinations of
       * operations are known not to be linearizable.  In particular,
       * when an addFirst(A) is racing with pollFirst() removing B, it
       * is possible for an observer iterating over the elements to
       * observe first [A B C] and then [A C], even though no interior
       * removes are ever performed.  Nevertheless, iterators behave
       * reasonably, providing the "weakly consistent" guarantees.
       *
       * Empirically, micro-benchmarks suggest that this class adds about
       * 40% overhead relative to ConcurrentLinkedQueue, which feels as
       * good as we can hope for.
       */

      /**
       *
       */
      class Node;

      CORE_ALIAS(NODE, typename Class<Node>::Pointer);
      CORE_ALIAS(ITEM, typename Class<E>::Pointer);
      CORE_ALIAS(UNSAFE, Object::UNSAFE);

      /**
       * A node from which the first node on list (that is, the unique node p
       * with p->prev == null && p->next != p) can be reached in O(1) time.
       * Invariants:
       * - the first node is always O(1) reachable from head via prev links
       * - all live nodes are reachable from the first node via succ()
       * - head != null
       * - (tmp = head)->next != tmp || tmp != head
       * - head is never gc-unlinked (but may be unlinked)
       * Non-invariants:
       * - head->item may or may not be null
       * - head may not be reachable from the first or last node, or from tail
       */
      NODE volatile mutable head = { };

      /**
       * A node from which the last node on list (that is, the unique node p
       * with p->next == null && p->prev != p) can be reached in O(1) time.
       * Invariants:
       * - the last node is always O(1) reachable from tail via next links
       * - all live nodes are reachable from the last node via pred()
       * - tail != null
       * - tail is never gc-unlinked (but may be unlinked)
       * Non-invariants:
       * - tail->item may or may not be null
       * - tail may not be reachable from the first or last node, or from head
       */
      NODE volatile mutable tail = { };

    public:
      /**
       * Constructs an empty deque.
       */
      ConcurrentLinkedDeque() {
        head = tail = new Node();
      }

      /**
       * Constructs a deque initially containing the elements of
       * the given collection, added in traversal order of the
       * collection's iterator.
       *
       * @param c the collection of elements to initially contain
       * @throws IllegalArgumentException if the of specified collection elements are null
       */
      CORE_EXPLICIT ConcurrentLinkedDeque(util::Collection<E> const& c) {
        // Copy c into a private chain of Nodes
        static CORE_FAST glong NEXT = nextFieldOffset();
        static CORE_FAST glong PREV = prevFieldOffset();

        NODE h = null, t = null;
        try {
          for (E const& e : c) {
            NODE node = newNode(util::Objects::requireNonNull(e));
            if (h == null)
              h = t = node;
            else {
              UNSAFE::putReference(toObject(t), NEXT, toObject(node));
              UNSAFE::putReference(toObject(node), PREV, toObject(t));
              t = node;
            }
          }
        } catch (Throwable const& ex) {
          // Initialization failed
          for (NODE x = h, y = null; x != null; x = y) {
            y = x->next;
            delete x;
          }
          h = t = null;
          ex.throws($ftrace());
        }
        initHeadTail(h, t);
      }

      /**
       * Inserts the specified element at the front of this deque.
       * As the deque is unbounded, this method will never throw
       * @b IllegalStateException.
       *
       * @throws IllegalArgumentException if the specified element is null
       */
      void addFirst(const E& val) override {
        try {
          linkFirst(val);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * Inserts the specified element at the end of this deque.
       * As the deque is unbounded, this method will never throw
       * @b IllegalStateException.
       *
       * <p>This method is equivalent to @b add.
       *
       * @throws IllegalArgumentException if the specified element is null
       */
      void addLast(const E& val) override {
        try {
          linkLast(val);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * Inserts the specified element at the front of this deque.
       * As the deque is unbounded, this method will never return @c false.
       *
       * @return @c true (as specified by @b Deque::offerFirst)
       * @throws IllegalArgumentException if the specified element is null
       */
      gbool offerFirst(const E& val) override {
        try {
          linkFirst(val);
          return true;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * Inserts the specified element at the end of this deque.
       * As the deque is unbounded, this method will never return @c false.
       *
       * <p>This method is equivalent to @b add.
       *
       * @return @c true (as specified by @b Deque::offerLast)
       * @throws IllegalArgumentException if the specified element is null
       */
      gbool offerLast(const E& val) override {
        try {
          linkLast(val);
          return true;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      Object& peekFirst() override {
      RESTART:
        for (;;) {
          ITEM item = null;
          NODE f = first(), p = first();
          while ((item = p->item) == null) {
            if (p == (p = p->next))
              goto RESTART;
            if (p == null)
              break;
          }
          // recheck for linearizability
          if (f->prev != null)
            goto RESTART;
          return toObject(item);
        }
      }

      const Object& peekFirst() const override {
      RESTART:
        for (;;) {
          ITEM item = null;
          NODE f = first(), p = first();
          while ((item = p->item) == null) {
            if (p == (p = p->next))
              goto RESTART;
            if (p == null)
              break;
          }
          // recheck for linearizability
          if (f->prev != null)
            goto RESTART;
          return toObject(item);
        }
      }

      Object& peekLast() override {
      RESTART:
        for (;;) {
          ITEM item = null;
          NODE l = last(), p = l;
          while ((item = p->item) == null) {
            if (p == (p = p->prev))
              goto RESTART;
            if (p == null)
              break;
          }
          // recheck for linearizability
          if (l->next != null)
            goto RESTART;
          return toObject(item);
        }
      }

      const Object& peekLast() const override {
      RESTART:
        for (;;) {
          ITEM item = null;
          NODE l = last(), p = l;
          while ((item = p->item) == null) {
            if (p == (p = p->prev))
              goto RESTART;
            if (p == null)
              break;
          }
          // recheck for linearizability
          if (l->next != null)
            goto RESTART;
          return toObject(item);
        }
      }

      /**
       * @throws NoSuchElementException
       */
      E& firstElement() override {
        try {
          return CORE_XCAST(E, util::Objects::requireNonNull(peekFirst()));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * @throws NoSuchElementException
       */
      const E& firstElement() const override {
        try {
          return CORE_XCAST(E const, util::Objects::requireNonNull(peekFirst()));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * @throws NoSuchElementException
       */
      E& lastElement() override {
        try {
          return CORE_XCAST(E, util::Objects::requireNonNull(peekLast()));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * @throws NoSuchElementException
       */
      const E& lastElement() const override {
        try {
          return CORE_XCAST(E const, util::Objects::requireNonNull(peekLast()));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      Object& pollFirst() override {
        static CORE_FAST glong IT = itemFieldOffset();
      RESTART:
        for (;;) {
          for (NODE f = first(), p = f;;) {
            ITEM item = null;
            if ((item = p->item) != null) {
              // recheck for linearizability
              if (f->prev != null)
                goto RESTART;
              if (UNSAFE::compareAndSetReference(toObject(p), IT, toObject(item), null)) {
                unlink(p);
                return toObject(item);
              }
            }
            if (p == (p = p->next))
              goto RESTART;
            if (p == null) {
              if (f->prev != null)
                goto RESTART;
              return null;
            }
          }
        }
      }

      Object& pollLast() override {
        static CORE_FAST glong IT = itemFieldOffset();
      RESTART:
        for (;;) {
          for (NODE l = last(), p = l;;) {
            ITEM item = null;
            if ((item = p->item) != null) {
              // recheck for linearizability
              if (l->next != null)
                goto RESTART;
              if (UNSAFE::compareAndSetReference(toObject(p), IT, toObject(item), null)) {
                unlink(p);
                return toObject(item);
              }
            }
            if (p == (p = p->prev))
              goto RESTART;
            if (p == null) {
              if (l->next != null)
                goto RESTART;
              return null;
            }
          }
        }
      }

      /**
       * @throws NoSuchElementException
       */
      E& removeFirst() override {
        try {
          return CORE_XCAST(E, util::Objects::requireNonNull(pollFirst()));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      E& removeLast() override {
        try {
          return CORE_XCAST(E, util::Objects::requireNonNull(pollLast()));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      // *** Queue and stack methods ***

      gbool add(const E& val) override {
        try {
          return offerLast(val);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      gbool offer(const E& val) override {
        try {
          return offerLast(val);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      Object& poll() override {
        try {
          return pollFirst();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      Object& peek() override {
        try {
          return peekFirst();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      const Object& peek() const override {
        try {
          return peekFirst();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      E& remove() override {
        try {
          return removeFirst();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      E& element() override {
        try {
          return firstElement();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      const E& element() const override {
        try {
          return firstElement();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      void push(const E& val) override {
        try {
          addFirst(val);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      E& pop() override {
        try {
          return removeFirst();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * Removes the first occurrence of the specified element from this deque.
       * If the deque does not contain the element, it is unchanged.
       * More formally, removes the first element @c e such that
       * @c o.equals(e) (if such an element exists).
       * Returns @c true if this deque contained the specified element
       * (or equivalently, if this deque changed as a result of the call).
       *
       * @param o element to be removed from this deque, if present
       * @return @c true if the deque contained the specified element
       * @throws IllegalArgumentException if the specified element is null
       */
      gbool removeFirstOccurrence(const E& o) override {
        static CORE_FAST glong IT = itemFieldOffset();
        try {
          util::Objects::requireNonNull(o);
          for (NODE p = first(); p != null; p = succ(p)) {
            ITEM item = null;
            if ((item = p->item) != null
              && o.equals(toObject(item))
              && UNSAFE::compareAndSetReference(toObject(p), IT, toObject(item), null)) {
              unlink(p);
              return true;
            }
          }
          return false;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * Removes the last occurrence of the specified element from this deque.
       * If the deque does not contain the element, it is unchanged.
       * More formally, removes the last element @c e such that
       * @c o.equals(e) (if such an element exists).
       * Returns @c true if this deque contained the specified element
       * (or equivalently, if this deque changed as a result of the call).
       *
       * @param o element to be removed from this deque, if present
       * @return @c true if the deque contained the specified element
       * @throws IllegalArgumentException if the specified element is null
       */
      gbool removeLastOccurrence(const E& o) override {
        static CORE_FAST glong IT = itemFieldOffset();
        try {
          util::Objects::requireNonNull(o);
          for (NODE p = last(); p != null; p = pred(p)) {
            ITEM item = null;
            if ((item = p->item) != null
              && o.equals(toObject(item))
              && UNSAFE::compareAndSetReference(toObject(p), IT, toObject(item), null)) {
              unlink(p);
              return true;
            }
          }
          return false;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * Returns @c true if this deque contains the specified element.
       * More formally, returns @c true if and only if this deque contains
       * at least one element @c e such that @c o.equals(e).
       *
       * @param o element whose presence in this deque is to be tested
       * @return @c true if this deque contains the specified element
       */
      gbool contains(const E& o) const override {
        if (o != null) {
          for (NODE p = first(); p != null; p = succ(p)) {
            ITEM item = null;
            if ((item = p->item) != null && o.equals(toObject(item)))
              return true;
          }
        }
        return false;
      }

      /**
       * Returns @c true if this collection contains no elements.
       *
       * @return @c true if this collection contains no elements
       */
      gbool isEmpty() const override {
        return peekFirst() == null;
      }

      /**
       * Returns the number of elements in this deque.  If this deque
       * contains more than @c Integer::MAX_VALUE elements, it
       * returns @c Integer::MAX_VALUE.
       *
       * <p>Beware that, unlike in most collections, this method is
       * <em>NOT</em> a constant-time operation. Because of the
       * asynchronous nature of these deques, determining the current
       * number of elements requires traversing them all to count them.
       * Additionally, it is possible for the size to change during
       * execution of this method, in which case the returned result
       * will be inaccurate. Thus, this method is typically not very
       * useful in concurrent applications.
       *
       * @return the number of elements in this deque
       */
      gint size() const override {
      RESTART:
        for (;;) {
          int count = 0;
          for (NODE p = first(); p != null;) {
            if (p->item != null)
              if (++count == Integer::MAX_VALUE)
                break; // @see Collection::size()
            if (p == (p = p->next))
              goto RESTART;
          }
          return count;
        }
      }

      /**
       * Removes the first occurrence of the specified element from this deque.
       * If the deque does not contain the element, it is unchanged.
       * More formally, removes the first element @c e such that
       * @c o.equals(e) (if such an element exists).
       * Returns @c true if this deque contained the specified element
       * (or equivalently, if this deque changed as a result of the call).
       *
       * <p>This method is equivalent to @b removeFirstOccurrence(Object).
       *
       * @param o element to be removed from this deque, if present
       * @return @c true if the deque contained the specified element
       * @throws IllegalArgumentException if the specified element is null
       */
      gbool remove(const E& val) override {
        return removeFirstOccurrence(val);
      }

      /**
       * Appends all of the elements in the specified collection to the end of
       * this deque, in the order that they are returned by the specified
       * collection's iterator.  Attempts to @c addAll of a deque to
       * itself result in @c IllegalArgumentException.
       *
       * @param c the elements to be inserted into this deque
       * @return @c true if this deque changed as a result of the call
       * @throws IllegalArgumentException if the specified collection or any
       *         of its elements are null
       * @throws IllegalArgumentException if the collection is this deque
       */
      gbool addAll(const util::Collection<E>& c) override {
        static CORE_FAST glong NEXT = nextFieldOffset();
        static CORE_FAST glong PREV = prevFieldOffset();
        static CORE_FAST glong TAIL = tailFieldOffset();

        if (&c == this)
          IllegalArgumentException().throws($ftrace());

        // Copy c into a private chain of Nodes
        NODE beginningOfTheEnd = null, last = null;
        try {
          for (E const& e : c) {
            NODE node = newNode(util::Objects::requireNonNull(e));
            if (beginningOfTheEnd == null)
              beginningOfTheEnd = last = node;
            else {
              UNSAFE::putReference(toObject(last), NEXT, toObject(node));
              UNSAFE::putReference(toObject(node), PREV, toObject(last));
              last = node;
            }
          }
        } catch (Throwable const& ex) {
          for (NODE x = beginningOfTheEnd, y = null; x != null; x = y) {
            y = x->next;
            delete x;
          }
          beginningOfTheEnd = last = null;
          ex.throws($ftrace());
        }
        if (beginningOfTheEnd == null)
          return false;

        // Atomically append the chain at the tail of this collection
      RESTART_FROM_TAIL:
        for (;;) {
          for (NODE t = tail, p = t, q = null;;) {
            if ((q = p->next) != null && (q = (p = q)->next) != null)
              // Check for tail updates every other hop.
              // If p == q, we are sure to follow tail instead.
              p = (t != (t = tail)) ? t : q;
            else if (p->prev == p) // NEXT_TERMINATOR
              goto RESTART_FROM_TAIL;
            else {
              // p is last node
              UNSAFE::putReference(toObject(beginningOfTheEnd), PREV, toObject(p)); // CAS piggyback
              if (UNSAFE::compareAndSetReference(toObject(p), NEXT, null, toObject(beginningOfTheEnd))) {
                // Successful CAS is the linearization point
                // for all elements to be added to this deque.
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
            }
          }
        }
      }

      /**
       * Removes all of the elements from this deque.
       */
      void clear() override {
        while (pollFirst() != null) {}
      }

      String toString() const override {
        StringArray a;
      RESTART:
        for (;;) {
          int charLength = 0;
          int size = 0;
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
              goto RESTART;
          }

          if (size == 0)
            return "[]";

          return toStringImpl(a, size, charLength);
        }
      }

      /**
       * Returns an array containing all of the elements in this deque, in
       * proper sequence (from first to last element).
       *
       * <p>The returned array will be "safe" in that no references to it are
       * maintained by this deque.  (In other words, this method must allocate
       * a new array).  The caller is thus free to modify the returned array.
       *
       * <p>This method acts as bridge between array-based and collection-based
       * APIs.
       *
       * @return an array containing all of the elements in this deque
       */
      Array<E> toArray() const override {
        try {
          return (Array<E>) toArrayImpl();
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      /**
       * Returns an iterator over the elements in this deque in proper sequence.
       * The elements will be returned in order from first (head) to last (tail).
       *
       * <p>The returned iterator is <i>weakly consistent</i>.
       *
       * @return an iterator over the elements in this deque in proper sequence
       */
      util::Iterator<E>& iterator() override {
        class Itr final : public util::Iterator<E> {
          ConcurrentLinkedDeque& self;
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
           * Node returned by most recent call to next. Needed by remove.
           * Reset to null if this element is deleted by a call to remove.
           */
          NODE last = { };

        public:
          CORE_EXPLICIT Itr(ConcurrentLinkedDeque& self) : self(self) {
            advance();
          }

          gbool equals(const Object& other) const override {
            if (this == &other)
              return true;
            if (!Class<Itr>::hasInstance(other))
              return false;
            Itr const& it = CORE_XCAST(Itr const, other);
            return &self == &it.self && nextNode == it.nextNode && nextItem == it.nextItem;
          }

          gbool hasNext() const override {
            return nextItem != null;
          }

          E&next() override {
            ITEM item = nextItem;
            if (item == null)
              util::NoSuchElementException().throws($ftrace());
            advance();
            return *item;
          }

          void remove() override {
            NODE l = last;
            if (l == null)
              IllegalStateException().throws($ftrace());
            l->item = null;
            self.unlink(l);
            last = null;
          }

        private:
          /**
           * Sets nextNode and nextItem to next valid node, or to null
           * if no such.
           */
          void advance() {
            last = nextNode;

            NODE p = (nextNode == null) ? self.first() : self.succ(nextNode);
            for (;; p = self.succ(p)) {
              if (p == null) {
                // might be at active end or TERMINATOR node; both are OK
                nextNode = null;
                nextItem = null;
                break;
              }
              ITEM item = null;
              if ((item = p->item) != null) {
                nextNode = p;
                nextItem = item;
                break;
              }
            }
          }
        };

        return UNSAFE::newInstance<Itr>(*this);
      }

      /**
       * Returns an iterator over the elements in this deque in proper sequence.
       * The elements will be returned in order from first (head) to last (tail).
       *
       * <p>The returned iterator is <i>weakly consistent</i>.
       *
       * @return an iterator over the elements in this deque in proper sequence
       */
      util::Iterator2<E>& iterator() const override {
        class Itr final : public util::Iterator2<E> {
          ConcurrentLinkedDeque const& self;
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
           * Node returned by most recent call to next. Needed by remove.
           * Reset to null if this element is deleted by a call to remove.
           */
          NODE last = { };

        public:
          CORE_EXPLICIT Itr(ConcurrentLinkedDeque const& self) : self(self) {
            advance();
          }

          gbool equals(const Object& other) const override {
            if (this == &other)
              return true;
            if (!Class<Itr>::hasInstance(other))
              return false;
            Itr const& it = CORE_XCAST(Itr const, other);
            return &self == &it.self && nextNode == it.nextNode && nextItem == it.nextItem;
          }

          gbool hasNext() const override {
            return nextItem != null;
          }

          E const&next() override {
            ITEM item = nextItem;
            if (item == null)
              util::NoSuchElementException().throws($ftrace());
            advance();
            return *item;
          }

        private:
          /**
           * Sets nextNode and nextItem to next valid node, or to null
           * if no such.
           */
          void advance() {
            last = nextNode;

            NODE p = (nextNode == null) ? self.first() : self.succ(nextNode);
            for (;; p = self.succ(p)) {
              if (p == null) {
                // might be at active end or TERMINATOR node; both are OK
                nextNode = null;
                nextItem = null;
                break;
              }
              ITEM item = null;
              if ((item = p->item) != null) {
                nextNode = p;
                nextItem = item;
                break;
              }
            }
          }
        };

        return UNSAFE::newInstance<Itr>(*this);
      }

      /**
       * Returns an iterator over the elements in this deque in reverse
       * sequential order.  The elements will be returned in order from
       * last (tail) to first (head).
       *
       * <p>The returned iterator is <i>weakly consistent</i>.
       *
       * @return an iterator over the elements in this deque in reverse order
       */
      util::Iterator<E>& descendingIterator() override {
        class Itr final : public util::Iterator<E> {
          ConcurrentLinkedDeque& self;
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
           * Node returned by most recent call to next. Needed by remove.
           * Reset to null if this element is deleted by a call to remove.
           */
          NODE last = { };

        public:
          CORE_EXPLICIT Itr(ConcurrentLinkedDeque& self) : self(self) {
            advance();
          }

          gbool equals(const Object& other) const override {
            if (this == &other)
              return true;
            if (!Class<Itr>::hasInstance(other))
              return false;
            Itr const& it = CORE_XCAST(Itr const, other);
            return &self == &it.self && nextNode == it.nextNode && nextItem == it.nextItem;
          }

          gbool hasNext() const override {
            return nextItem != null;
          }

          E&next() override {
            ITEM item = nextItem;
            if (item == null)
              util::NoSuchElementException().throws($ftrace());
            advance();
            return *item;
          }

          void remove() override {
            NODE l = last;
            if (l == null)
              IllegalStateException().throws($ftrace());
            l->item = null;
            self.unlink(l);
            last = null;
          }

        private:
          /**
           * Sets nextNode and nextItem to next valid node, or to null
           * if no such.
           */
          void advance() {
            last = nextNode;

            NODE p = (nextNode == null) ? self.last() : self.pred(nextNode);
            for (;; p = self.pred(p)) {
              if (p == null) {
                // might be at active end or TERMINATOR node; both are OK
                nextNode = null;
                nextItem = null;
                break;
              }
              ITEM item = null;
              if ((item = p->item) != null) {
                nextNode = p;
                nextItem = item;
                break;
              }
            }
          }
        };

        return UNSAFE::newInstance<Itr>(*this);
      }

      /**
       * Returns an iterator over the elements in this deque in reverse
       * sequential order.  The elements will be returned in order from
       * last (tail) to first (head).
       *
       * <p>The returned iterator is <i>weakly consistent</i>.
       *
       * @return an iterator over the elements in this deque in reverse order
       */
      util::Iterator2<E>& descendingIterator() const override {
        class Itr final : public util::Iterator2<E> {
          ConcurrentLinkedDeque const& self;
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
           * Node returned by most recent call to next. Needed by remove.
           * Reset to null if this element is deleted by a call to remove.
           */
          NODE last = { };

        public:
          CORE_EXPLICIT Itr(ConcurrentLinkedDeque const& self) : self(self) {
            advance();
          }

          gbool equals(const Object& other) const override {
            if (this == &other)
              return true;
            if (!Class<Itr>::hasInstance(other))
              return false;
            Itr const& it = CORE_XCAST(Itr const, other);
            return &self == &it.self && nextNode == it.nextNode && nextItem == it.nextItem;
          }

          gbool hasNext() const override {
            return nextItem != null;
          }

          E&next() override {
            ITEM item = nextItem;
            if (item == null)
              util::NoSuchElementException().throws($ftrace());
            advance();
            return *item;
          }

        private:
          /**
           * Sets nextNode and nextItem to next valid node, or to null
           * if no such.
           */
          void advance() {
            last = nextNode;

            NODE p = (nextNode == null) ? self.last() : self.pred(nextNode);
            for (;; p = self.pred(p)) {
              if (p == null) {
                // might be at active end or TERMINATOR node; both are OK
                nextNode = null;
                nextItem = null;
                break;
              }
              ITEM item = null;
              if ((item = p->item) != null) {
                nextNode = p;
                nextItem = item;
                break;
              }
            }
          }
        };

        return UNSAFE::newInstance<Itr>(*this);
      }

      /**
       * Returns a @b Spliterator over the elements in this deque.
       *
       * <p>The returned spliterator is <i>weakly consistent</i>.
       *
       * <p>The @c Spliterator reports @b Spliterator::CONCURRENT,
       * @b Spliterator::ORDERED, and @b Spliterator::NONNULL.
       *
       * @note
       * The @c Spliterator implements @c trySplit to permit limited
       * parallelism.
       *
       * @return a @c Spliterator over the elements in this deque
       */
      static CORE_FAST int MAX_BATCH = 1 << 25; // max batch array size;
      util::Spliterator<E>& spliterator() override {
        class Spl final : public util::Spliterator<E> {
          ConcurrentLinkedDeque& self;
          NODE mutable current = { }; // current node; null until initialized
          int mutable batch = 0; // batch size for splits
          gbool mutable exhausted = false; // true when no more nodes

        public:
          CORE_EXPLICIT Spl(ConcurrentLinkedDeque& self) : self(self) {}

          util::Spliterator<E>&trySplit() override {
            NODE p = null, q = null;
            if ((p = getCurrent()) == null || (q = p->next) == null)
              return *this;
            int i = 0, n = batch = Math::min(batch + 1, MAX_BATCH);
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
              return util::Spliterators::spliterator<E>(a, 0, i, (util::Spliterator<>::ORDERED |
                                                          util::Spliterator<>::NONNULL |
                                                          util::Spliterator<>::CONCURRENT));
          }

          const util::Spliterator<E>&trySplit() const override {
            NODE p = null, q = null;
            if ((p = getCurrent()) == null || (q = p->next) == null)
              return *this;
            int i = 0, n = batch = Math::min(batch + 1, MAX_BATCH);
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
              return util::Spliterators::spliterator<E>(a, 0, i, (util::Spliterator<>::ORDERED |
                                                          util::Spliterator<>::NONNULL |
                                                          util::Spliterator<>::CONCURRENT));
          }

          void forEach(const function::Consumer<E&>& action) override {
            NODE p = null;
            if ((p = getCurrent()) != null) {
              current = null;
              exhausted = true;
              do {
                ITEM e = null;
                try {
                  if ((e = p->item) != null)
                    action.accept(*e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (p == (p = p->next))
                  p = self.first();
              } while (p != null);
            }
          }

          gbool tryAdvance(const function::Consumer<E&>& action) override {
            NODE p;
            if ((p = getCurrent()) != null) {
              ITEM e = null;
              do {
                e = p->item;
                if (p == (p = p->next))
                  p = self.first();
              } while (e == null && p != null);
              setCurrent(p);
              if (e != null) {
                try {
                  action.accept(*e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
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

          gbool equals(const Object& other) const override {
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
       * Returns a @b Spliterator over the elements in this deque.
       *
       * <p>The returned spliterator is <i>weakly consistent</i>.
       *
       * <p>The @c Spliterator reports @b Spliterator::CONCURRENT,
       * @b Spliterator::ORDERED, and @b Spliterator::NONNULL.
       *
       * @note
       * The @c Spliterator implements @c trySplit to permit limited
       * parallelism.
       *
       * @return a @c Spliterator over the elements in this deque
       */
      util::Spliterator2<E>& spliterator() const override {
        class Spl final : public util::Spliterator2<E> {
          ConcurrentLinkedDeque const& self;
          NODE mutable current = { }; // current node; null until initialized
          int mutable batch = 0; // batch size for splits
          gbool mutable exhausted = false; // true when no more nodes

        public:
          CORE_EXPLICIT Spl(ConcurrentLinkedDeque const& self) : self(self) {}

          util::Spliterator2<E>&trySplit() override {
            NODE p = null, q = null;
            if ((p = getCurrent()) == null || (q = p->next) == null)
              return *this;
            int i = 0, n = batch = Math::min(batch + 1, MAX_BATCH);
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
              return util::Spliterators::spliterator2<E>(a, 0, i, (util::Spliterator<>::ORDERED |
                                                           util::Spliterator<>::NONNULL |
                                                           util::Spliterator<>::CONCURRENT));
          }

          const util::Spliterator2<E>&trySplit() const override {
            NODE p = null, q = null;
            if ((p = getCurrent()) == null || (q = p->next) == null)
              return *this;
            int i = 0, n = batch = Math::min(batch + 1, MAX_BATCH);
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
              return util::Spliterators::spliterator2<E>(a, 0, i, (util::Spliterator<>::ORDERED |
                                                           util::Spliterator<>::NONNULL |
                                                           util::Spliterator<>::CONCURRENT));
          }

          void forEach(const function::Consumer<E>& action) override {
            NODE p = null;
            if ((p = getCurrent()) != null) {
              current = null;
              exhausted = true;
              do {
                ITEM e = null;
                try {
                  if ((e = p->item) != null)
                    action.accept(*e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (p == (p = p->next))
                  p = self.first();
              } while (p != null);
            }
          }

          gbool tryAdvance(const function::Consumer<E>& action) override {
            NODE p;
            if ((p = getCurrent()) != null) {
              ITEM e = null;
              do {
                e = p->item;
                if (p == (p = p->next))
                  p = self.first();
              } while (e == null && p != null);
              setCurrent(p);
              if (e != null) {
                try {
                  action.accept(*e);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
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

          gbool equals(const Object& other) const override {
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

      gbool removeIf(const function::Predicate<E>& filter) override {
        try {
          return bulkRemove(filter);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      gbool removeAll(const util::Collection<E>& c) override {
        try {
          return bulkRemove([&](E const& e) -> gbool { CORE_TRY_RETHROW(return c.contains(e)) });
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      gbool retainAll(const util::Collection<E>& c) override {
        try {
          return bulkRemove([&](E const& e) -> gbool { CORE_TRY_RETHROW(return !c.contains(e)) });
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

      void forEach(const function::Consumer<E&>& action) override {
        ITEM item = null;
        for (NODE p = first(); p != null; p = succ(p))
          if ((item = p->item) != null)
            CORE_TRY_RETHROW(action.accept(*item))
      }

      void forEach(const function::Consumer<E>& action) const override {
        ITEM item = null;
        for (NODE p = first(); p != null; p = succ(p))
          if ((item = p->item) != null)
            CORE_TRY_RETHROW(action.accept(*item))
      }

    private:
      NODE prevTerminator() const {
        static NODE PREV_TERMINATOR = { };
        NODE p = null;
        if ((p = PREV_TERMINATOR) == null) {
          p = new Node();
          p->next = p;
          PREV_TERMINATOR = p;
        }
        return p;
      }

      NODE nextTerminator() const {
        static NODE NEXT_TERMINATOR = { };
        NODE p = null;
        if ((p = NEXT_TERMINATOR) == null) {
          p = new Node();
          p->prev = p;
          NEXT_TERMINATOR = p;
        }
        return p;
      }

      /**
       * Returns a new node holding item.  Uses relaxed write because item
       * can only be seen after piggy-backing publication via CAS.
       */
      static NODE newNode(E const& item) {
        static CORE_FAST glong ITEM = itemFieldOffset();
        NODE node = new Node();
        UNSAFE::putReference(toObject(node), ITEM, (E&) item);
        return node;
      }

      /**
       * Links e as first element.
       */
      void linkFirst(E const& e) {
        static CORE_FAST glong HEAD = headFieldOffset();
        static CORE_FAST glong NEXT = nextFieldOffset();
        static CORE_FAST glong PREV = prevFieldOffset();
        NODE node = null;
        try {
          node = newNode(UNSAFE::copyInstance(util::Objects::requireNonNull(e)));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      RESTART_FROM_HEAD:
        for (;;) {
          for (NODE h = head, p = h, q = null;;) {
            if ((q = p->prev) != null && (q = (p = q)->prev) != null)
              // Check for head updates every other hop.
              // If p == q, we are sure to follow head instead.
              p = (h != (h = head)) ? h : q;
            else if (p->next == p) // PREV_TERMINATOR
              goto RESTART_FROM_HEAD;
            else {
              // p is first node
              UNSAFE::putReference(toObject(node), NEXT, toObject(p));
              if (UNSAFE::compareAndSetReference(toObject(p), PREV, null, toObject(node))) {
                // Successful CAS is the linearization point
                // for e to become an element of this deque,
                // and for newNode to become "live".
                if (p != h) // hop two nodes at a time; failure is OK
                  UNSAFE::weakCompareAndSetReference(toObject(this), HEAD, toObject(h),
                                                     toObject(node));
                return;
              }
              // Lost CAS race to another thread; re-read prev
            }
          }
        }
      }

      /**
       * Links e as last element.
       */
      void linkLast(E const& e) {
        static CORE_FAST glong TAIL = tailFieldOffset();
        static CORE_FAST glong NEXT = nextFieldOffset();
        static CORE_FAST glong PREV = prevFieldOffset();
        NODE node = null;
        try {
          node = newNode(UNSAFE::copyInstance(util::Objects::requireNonNull(e)));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      RESTART_FROM_TAIL:
        for (;;) {
          for (NODE t = tail, p = t, q = null;;) {
            if ((q = p->next) != null && (q = (p = q)->next) != null)
              // Check for tail updates every other hop.
              // If p == q, we are sure to follow tail instead.
              p = (t != (t = tail)) ? t : q;
            else if (p->prev == p) // NEXT_TERMINATOR
              goto RESTART_FROM_TAIL;
            else {
              // p is last node
              UNSAFE::putReference(toObject(node), PREV, toObject(p));
              if (UNSAFE::compareAndSetReference(toObject(p), NEXT, null, toObject(node))) {
                // Successful CAS is the linearization point
                // for e to become an element of this deque,
                // and for newNode to become "live".
                if (p != t) // hop two nodes at a time; failure is OK
                  UNSAFE::weakCompareAndSetReference(toObject(this), TAIL, toObject(t),
                                                     toObject(node));
                return;
              }
              // Lost CAS race to another thread; re-read next
            }
          }
        }
      }

      static CORE_FAST gint HOPS = 2;

      /**
       * Unlinks non-null node x.
       */
      void unlink(NODE x) {
        // CORE_ASSERT(x != null);
        // CORE_ASSERT(x->item == null);
        // CORE_ASSERT(x != PREV_TERMINATOR);
        // CORE_ASSERT(x != NEXT_TERMINATOR);
        static CORE_FAST glong NEXT = nextFieldOffset();
        static CORE_FAST glong PREV = prevFieldOffset();

        NODE prev = x->prev;
        NODE next = x->next;
        if (prev == null)
          unlinkFirst(x, next);
        else if (next == null)
          unlinkLast(x, prev);
        else {
          // Unlink interior node.
          //
          // This is the common case, since a series of polls at the
          // same end will be "interior" removes, except perhaps for
          // the first one, since end nodes cannot be unlinked.
          //
          // At any time, all active nodes are mutually reachable by
          // following a sequence of either next or prev pointers.
          //
          // Our strategy is to find the unique active predecessor
          // and successor of x.  Try to fix up their links so that
          // they point to each other, leaving x unreachable from
          // active nodes.  If successful, and if x has no live
          // predecessor/successor, we additionally try to gc-unlink,
          // leaving active nodes unreachable from x, by rechecking
          // that the status of predecessor and successor are
          // unchanged and ensuring that x is not reachable from
          // tail/head, before setting x's prev/next links to their
          // logical approximate replacements, self/TERMINATOR.
          NODE activePred = null, activeSucc = null;
          gbool isFirst = false, isLast = false;
          gint hops = 1;

          // Find active predecessor
          for (NODE p = prev;; ++hops) {
            if (p->item != null) {
              activePred = p;
              isFirst = false;
              break;
            }
            NODE q = p->prev;
            if (q == null) {
              if (p->next == p)
                return;
              activePred = p;
              isFirst = true;
              break;
            }
            if (p == q)
              return;
            else
              p = q;
          }

          // Find active successor
          for (NODE p = next;; ++hops) {
            if (p->item != null) {
              activeSucc = p;
              isLast = false;
              break;
            }
            NODE q = p->next;
            if (q == null) {
              if (p->prev == p)
                return;
              activeSucc = p;
              isLast = true;
              break;
            } else if (p == q)
              return;
            else
              p = q;
          }

          // TODO: better HOP heuristics
          if (hops < HOPS
            // always squeeze out interior deleted nodes
            && (isFirst | isLast))
            return;

          // Squeeze out deleted nodes between activePred and
          // activeSucc, including x.
          skipDeletedSuccessors(activePred);
          skipDeletedPredecessors(activeSucc);

          // Try to gc-unlink, if possible
          if ((isFirst | isLast) &&

            // Recheck expected state of predecessor and successor
            (activePred->next == activeSucc) &&
            (activeSucc->prev == activePred) &&
            (isFirst ? activePred->prev == null : activePred->item != null) &&
            (isLast ? activeSucc->next == null : activeSucc->item != null)) {
            updateHead(); // Ensure x is not reachable from head
            updateTail(); // Ensure x is not reachable from tail

            // Finally, actually gc-unlink
            UNSAFE::putReferenceRelease(toObject(x), PREV, toObject(isFirst ? prevTerminator() : x));
            UNSAFE::putReferenceRelease(toObject(x), NEXT, toObject(isLast ? prevTerminator() : x));
          }
        }
      }

      /**
       * Unlinks non-null first node.
       */
      void unlinkFirst(NODE first, NODE next) {
        // CORE_ASSERT(first != null);
        // CORE_ASSERT(next != null);
        // CORE_ASSERT(first->item == null);
        static CORE_FAST glong NEXT = nextFieldOffset();
        static CORE_FAST glong PREV = prevFieldOffset();
        for (NODE o = null, p = next, q = null;;) {
          if (p->item != null || (q = p->next) == null) {
            if (o != null && p->prev != p &&
              UNSAFE::compareAndSetReference(toObject(first), NEXT, toObject(next), toObject(p))) {
              skipDeletedPredecessors(p);
              if (first->prev == null &&
                (p->next == null || p->item != null) &&
                p->prev == first) {
                updateHead(); // Ensure o is not reachable from head
                updateTail(); // Ensure o is not reachable from tail

                // Finally, actually gc-unlink
                UNSAFE::putReferenceRelease(toObject(o), NEXT, toObject(o));
                UNSAFE::putReferenceRelease(toObject(o), PREV, toObject(prevTerminator()));
              }
            }
            return;
          } else if (p == q)
            return;
          else {
            o = p;
            p = q;
          }
        }
      }

      /**
       * Unlinks non-null last node.
       */
      void unlinkLast(NODE last, NODE prev) {
        // CORE_ASSERT(last != null);
        // CORE_ASSERT(prev != null);
        // CORE_ASSERT(last->item == null);
        static CORE_FAST glong NEXT = nextFieldOffset();
        static CORE_FAST glong PREV = prevFieldOffset();
        for (NODE o = null, p = prev, q = null;;) {
          if (p->item != null || (q = p->prev) == null) {
            if (o != null && p->next != p &&
              UNSAFE::compareAndSetReference(toObject(last), PREV, toObject(prev), toObject(p))) {
              skipDeletedSuccessors(p);
              if (last->next == null &&
                (p->prev == null || p->item != null) &&
                p->next == last) {
                updateHead(); // Ensure o is not reachable from head
                updateTail(); // Ensure o is not reachable from tail

                // Finally, actually gc-unlink
                UNSAFE::putReferenceRelease(toObject(o), PREV, toObject(o));
                UNSAFE::putReferenceRelease(toObject(o), NEXT, toObject(nextTerminator()));
              }
            }
            return;
          } else if (p == q)
            return;
          else {
            o = p;
            p = q;
          }
        }
      }

      /**
       * Guarantees that any node which was unlinked before a call to
       * this method will be unreachable from head after it returns.
       * Does not guarantee to eliminate slack, only that head will
       * point to a node that was active while this method was running.
       */
      void updateHead() const {
        // Either head already points to an active node, or we keep
        // trying to cas it to the first node until it does.
        static CORE_FAST glong HEAD = headFieldOffset();
        NODE h = null, p = null, q = null;
      RESTART_FROM_HEAD:
        while ((h = head)->item == null && (p = h->prev) != null) {
          for (;;) {
            if ((q = p->prev) == null ||
              (q = (p = q)->prev) == null) {
              // It is possible that p is PREV_TERMINATOR,
              // but if so, the CAS is guaranteed to fail.
              if (UNSAFE::compareAndSetReference(toObject(this), HEAD, toObject(h), toObject(p)))
                return;
              else
                goto RESTART_FROM_HEAD;
            } else if (h != head)
              goto RESTART_FROM_HEAD;
            else
              p = q;
          }
        }
      }

      /**
       * Guarantees that any node which was unlinked before a call to
       * this method will be unreachable from tail after it returns.
       * Does not guarantee to eliminate slack, only that tail will
       * point to a node that was active while this method was running.
       */
      void updateTail() const {
        // Either tail already points to an active node, or we keep
        // trying to cas it to the last node until it does.
        static CORE_FAST glong TAIL = tailFieldOffset();
        NODE t = null, p = null, q = null;
      RESTART_FROM_TAIL:
        while ((t = tail)->item == null && (p = t->next) != null) {
          for (;;) {
            if ((q = p->next) == null ||
              (q = (p = q)->next) == null) {
              // It is possible that p is NEXT_TERMINATOR,
              // but if so, the CAS is guaranteed to fail.
              if (UNSAFE::compareAndSetReference(toObject(this), TAIL, toObject(t), toObject(p)))
                return;
              else
                goto RESTART_FROM_TAIL;
            } else if (t != tail)
              goto RESTART_FROM_TAIL;
            else
              p = q;
          }
        }
      }

      void skipDeletedPredecessors(NODE x) {
        static CORE_FAST glong PREV = prevFieldOffset();
      WHILE_ACTIVE:
        do {
          NODE prev = x->prev;
          // assert prev != null;
          // assert x != NEXT_TERMINATOR;
          // assert x != PREV_TERMINATOR;
          NODE p = prev;
          // FIND_ACTIVE:
          for (;;) {
            if (p->item != null)
              break; // FIND_ACTIVE
            NODE q = p->prev;
            if (q == null) {
              if (p->next == p)
                goto WHILE_ACTIVE;
              break; // FIND_ACTIVE
            } else if (p == q)
              goto WHILE_ACTIVE;
            else
              p = q;
          }

          // found active CAS target
          if (prev == p || UNSAFE::compareAndSetReference(toObject(x), PREV, toObject(prev), toObject(p)))
            return;
        } while (x->item != null || x->next == null);
      }

      void skipDeletedSuccessors(NODE x) {
        static CORE_FAST glong NEXT = nextFieldOffset();
      WHILE_ACTIVE:
        do {
          NODE next = x->next;
          // assert next != null;
          // assert x != NEXT_TERMINATOR;
          // assert x != PREV_TERMINATOR;
          NODE p = next;
          // FIND_ACTIVE:
          for (;;) {
            if (p->item != null)
              break; // FIND_ACTIVE;
            NODE q = p->next;
            if (q == null) {
              if (p->prev == p)
                goto WHILE_ACTIVE;
              break; // FIND_ACTIVE;
            } else if (p == q)
              goto WHILE_ACTIVE;
            else
              p = q;
          }

          // found active CAS target
          if (next == p || UNSAFE::compareAndSetReference(toObject(x), NEXT, toObject(next), toObject(p)))
            return;
        } while (x->item != null || x->prev == null);
      }

      /**
       * Implementation of bulk remove methods.
       */
      gbool bulkRemove(function::Predicate<E> const& filter) {
        static CORE_FAST glong IT = itemFieldOffset();

        gbool removed = false;
        for (NODE p = first(), next = null; p != null; p = next) {
          next = succ(p);
          ITEM item = null;
          if ((item = p->item) != null && filter.test(*item) &&
            UNSAFE::compareAndSetReference(toObject(p), IT, toObject(item), null)) {
            unlink(p);
            removed = true;
          }
        }
        return removed;
      }

      template <class T, class TRef = typename Class<T>::Slim, class Ref = typename Class<TRef>::NoPointer>
      static Object& toObject(T ref) {
        if (ref == null)
          return null;
        return (Ref&) *ref;
      }

      /**
       * Returns the successor of p, or the first node if p->next has been
       * linked to self, which will only be true if traversing with a
       * stale pointer that is now off the list.
       */
      NODE succ(NODE p) const {
        // TODO: should we skip deleted nodes here?
        if (p == (p = p->next))
          p = first();
        return p;
      }

      /**
       * Returns the predecessor of p, or the last node if p->prev has been
       * linked to self, which will only be true if traversing with a
       * stale pointer that is now off the list.
       */
      NODE pred(NODE p) const {
        if (p == (p = p->prev))
          p = last();
        return p;
      }

      /**
       * Returns the first node, the unique node p for which:
       *     p->prev == null && p->next != p
       * The returned node may or may not be logically deleted.
       * Guarantees that head is set to the returned node.
       */
      NODE first() const {
        static CORE_FAST glong HEAD = headFieldOffset();
      RESTART_FROM_HEAD:
        for (;;)
          for (NODE h = head, p = h, q = null;;) {
            if ((q = p->prev) != null &&
              (q = (p = q)->prev) != null)
              // Check for head updates every other hop.
              // If p == q, we are sure to follow head instead.
              p = (h != (h = head)) ? h : q;
            else if (p == h
              // It is possible that p is PREV_TERMINATOR,
              // but if so, the CAS is guaranteed to fail.
              || UNSAFE::compareAndSetReference(toObject(this), HEAD, toObject(h), toObject(p)))
              return p;
            else
              goto RESTART_FROM_HEAD;
          }
      }

      /**
       * Returns the last node, the unique node p for which:
       *     p->next == null && p->prev != p
       * The returned node may or may not be logically deleted.
       * Guarantees that tail is set to the returned node.
       */
      NODE last() const {
        static CORE_FAST glong TAIL = tailFieldOffset();
      RESTART_FROM_TAIL:
        for (;;)
          for (NODE t = tail, p = t, q = null;;) {
            if ((q = p->next) != null &&
              (q = (p = q)->next) != null)
              // Check for tail updates every other hop.
              // If p == q, we are sure to follow tail instead.
              p = (t != (t = tail)) ? t : q;
            else if (p == t
              // It is possible that p is NEXT_TERMINATOR,
              // but if so, the CAS is guaranteed to fail.
              || UNSAFE::compareAndSetReference(toObject(this), TAIL, toObject(t), toObject(p)))
              return p;
            else
              goto RESTART_FROM_TAIL;
          }
      }

      /**
       * Initializes head and tail, ensuring invariants hold.
       */
      void initHeadTail(NODE h, NODE t) {
        static CORE_FAST glong NEXT = nextFieldOffset();
        static CORE_FAST glong PREV = prevFieldOffset();
        if (h == t) {
          if (h == null)
            h = t = new Node();
          else {
            // Avoid edge case of a single Node with non-null item.
            NODE newNode = new Node();
            UNSAFE::putReference(toObject(t), NEXT, toObject(newNode));
            UNSAFE::putReference(toObject(newNode), PREV, toObject(t));
            t = newNode;
          }
        }
        head = h;
        tail = t;
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
      RESTART:
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
              goto RESTART;
          }
          return (size == x.length()) ? x : util::Arrays::copyOf(x, size);
        }
      }

      template <class T>
      static glong toOffset(T& ref) {
        if (ref == null)
          return 0L;
        return (glong) &ref;
      }

      static CORE_FAST glong headFieldOffset() { return CORE_FIELD_OFFSET(ConcurrentLinkedDeque, head); }

      static CORE_FAST glong tailFieldOffset() { return CORE_FIELD_OFFSET(ConcurrentLinkedDeque, tail); }

      static CORE_FAST glong itemFieldOffset() { return CORE_FIELD_OFFSET(Node, item); }

      static CORE_FAST glong nextFieldOffset() { return CORE_FIELD_OFFSET(Node, next); }

      static CORE_FAST glong prevFieldOffset() { return CORE_FIELD_OFFSET(Node, prev); }
    };

    template <class E>
    class ConcurrentLinkedDeque<E>::Node final : public Object {
    public:
      NODE volatile prev = { };
      ITEM item = { };
      NODE next = { };

      ~Node() override {
        prev = next = null;
        item = null;
      }
    };

    CORE_WARNING_POP
  } // concurrent
} // core

#endif //CORE24_CONCURRENTLINKEDDEQUE_H
