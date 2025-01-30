//
// Created by brunshweeck on 12 juil. 2024.
//

#ifndef CORE24_QUEUE_H
#define CORE24_QUEUE_H

#include <core/util/Collection.h>

namespace core {
    namespace util {
        /**
         * A collection designed for holding elements prior to processing.
         * Besides basic @c Collection operations, queues provide
         * additional insertion, extraction, and inspection operations.
         * Each of these methods exists in two forms: one throws an exception
         * if the operation fails, the other returns a special value (either
         * @c null or @c false, depending on the operation).  The
         * latter form of the insert operation is designed specifically for
         * use with capacity-restricted @c Queue implementations; in most
         * implementations, insert operations cannot fail.
         * <p>
         * Summary of Queue methods:
         *  - Insert:  @c add(e),  @c offer(e)
         *  - Remove:  @c remove(e),  @c poll(e)
         *  - Examine:  @c head(),  @c peek(e)
         * </p>
         * <p>
         * Queues typically, but do not necessarily, order elements in a
         * FIFO (first-in-first-out) manner.  Among the exceptions are
         * priority queues, which order elements according to a supplied
         * comparator, or the elements' natural ordering, and LIFO queues (or
         * stacks) which order the elements LIFO (last-in-first-out).
         * Whatever the ordering used, the <em>head</em> of the queue is that
         * element which would be removed by a call to @c remove() or
         * @c poll().  In a FIFO queue, all new elements are inserted at
         * the <em>tail</em> of the queue. Other kinds of queues may use
         * different placement rules.  Every @c Queue implementation
         * must specify its ordering properties.
         * </p>
         * <p>
         * The @c offer method inserts an element if possible,
         * otherwise returning @c false.  This differs from the
         * @c Collection::add method, which can fail to
         * add an element only by throwing an unchecked exception.  The
         * @c offer method is designed for use when failure is a normal,
         * rather than exceptional occurrence, for example, in fixed-capacity
         * (or 'bounded') queues.
         * </p>
         * <p>
         * The @c remove() and @c poll() methods remove and
         * return the head of the queue.
         * Exactly which element is removed from the queue is a
         * function of the queue's ordering policy, which differs from
         * implementation to implementation. The @c remove() and
         * @c poll() methods differ only in their behavior when the
         * queue is empty: the @c remove() method throws an exception,
         * while the @c poll() method returns @c null.
         * </p>
         * <p>
         * The @c head() and @c peek() methods return, but do
         * not remove, the head of the queue.
         * </p>
         * <p>
         * The @c Queue interface does not define the <i>blocking queue
         * methods</i>, which are common in concurrent programming.  These methods,
         * which wait for elements to appear or for space to become available, are
         * defined in the @c concurrent::BlockingQueue interface, which
         * extends this interface.
         * </p>
         * <p>
         * @c Queue implementations generally do not allow insertion
         * of @c null elements, although some implementations, such as
         * @c LinkedList, do not prohibit insertion of @c null.
         * Even in the implementations that permit it, @c null should
         * not be inserted into a @c Queue, as @c null is also
         * used as a special return value by the @c poll method to
         * indicate that the queue contains no elements.
         * </p>
         * <p>
         * @c Queue implementations generally do not define
         * element-based versions of methods @c equals and
         * @c hash but instead inherit the identity based versions
         * from class @c Object, because element-based equality is not
         * always well-defined for queues with the same elements but different
         * ordering properties.
         * </p>
         *
         * @tparam E the type of elements held in this queue
         */
        template<class E>
        class Queue : public virtual Collection<E> {
        public:
            /**
             * Inserts the specified element into this queue if it is possible to do so
             * immediately without violating capacity restrictions, returning
             * @c true upon success and throwing an @c IllegalStateException
             * if no space is currently available.
             *
             * @param val the element to add
             * @return @c true (as specified by @c Collection::add)
             * @throws IllegalStateException if the element cannot be added at this
             *         time due to capacity restrictions
             * @throws IllegalArgumentException if some property of this element
             *         prevents it from being added to this queue
             */
            gbool add(E const &val) override {
                if (offer(val))
                    return true;
                IllegalStateException().throws($ftrace());
            }

            /**
             * Inserts the specified element into this queue if it is possible to do
             * so immediately without violating capacity restrictions.
             * When using a capacity-restricted queue, this method is generally
             * preferable to @c add, which can fail to insert an element only
             * by throwing an exception.
             *
             * @param val the element to add
             * @return @c true if the element was added to this queue, else
             *         @c false
             * @throws IllegalArgumentException if some property of this element
             *         prevents it from being added to this queue
             */
            virtual gbool offer(E const &val) = 0;

            /**
             * Retrieves and removes the head of this queue.  This method differs
             * from @c poll() only in that it throws an exception if
             * this queue is empty.
             *
             * @return the head of this queue
             * @throws NoSuchElementException if this queue is empty
             */
            virtual E &remove() {
                Object &x = poll();
                if (x != null)
                    return CORE_XCAST(E, x);
                NoSuchElementException().throws($ftrace());
            }

            /**
             * Retrieves and removes the head of this queue,
             * or returns @c null if this queue is empty.
             *
             * @return the head of this queue, or @c null if this queue is empty
             */
            virtual Object &poll() = 0;

            /**
             * Retrieves, but does not remove, the head of this queue.  This method
             * differs from @c peek only in that it throws an exception
             * if this queue is empty.
             *
             * @return the head of this queue
             * @throws NoSuchElementException if this queue is empty
             */
            virtual E &element() {
                Object &x = peek();
                if (x != null)
                    return CORE_XCAST(E, x);
                NoSuchElementException().throws($ftrace());
            }

            /**
             * Retrieves, but does not remove, the head of this queue.  This method
             * differs from @c peek only in that it throws an exception
             * if this queue is empty.
             *
             * @return the head of this queue
             * @throws NoSuchElementException if this queue is empty
             */
            virtual E const &element() const {
                Object const &x = peek();
                if (x != null)
                    return CORE_XCAST(E const, x);
                NoSuchElementException().throws($ftrace());
            }

            /**
             * Retrieves, but does not remove, the head of this queue,
             * or returns @c null if this queue is empty.
             *
             * @return the head of this queue, or @c null if this queue is empty
             */
            virtual Object &peek() = 0;

            /**
             * Retrieves, but does not remove, the head of this queue,
             * or returns @c null if this queue is empty.
             *
             * @return the head of this queue, or @c null if this queue is empty
             */
            virtual Object const &peek() const = 0;

            /**
             * Removes all of the elements from this queue.
             * The queue will be empty after this call returns.
             *
             * <p>This implementation repeatedly invokes @b poll until it
             * returns @c null.
             */
            void clear() override {
                while (poll() != null) {
                    //
                }
            }

            /**
             * Adds all the elements in the specified collection to this
             * queue.  Attempts to addAll of a queue to itself result in
             * @c IllegalArgumentException. Further, the behavior of
             * this operation is undefined if the specified collection is
             * modified while the operation is in progress.
             *
             * <p>
             * This implementation iterates over the specified collection,
             * and adds each element returned by the iterator to this
             * queue, in turn.  A runtime exception encountered while
             * trying to add an element (including, in particular, a
             * @c null element) may result in only some of the elements
             * having been successfully added when the associated exception is
             * thrown.
             * </p>
             *
             * @param c collection containing elements to be added to this queue
             * @return @c true if this queue changed as a result of the call
             * @throws ClassCastException if the class of an element of the specified
             *         collection prevents it from being added to this queue
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this
             *         queue, or if the specified collection is this queue
             * @throws IllegalStateException if not all the elements can be added at
             *         this time due to insertion restrictions
             * @see add(Object)
             */
            gbool addAll(const Collection<E> &c) override {
                if (this == &c)
                    IllegalArgumentException().throws($ftrace());
                gbool modified = false;
                for (E const &val: c)
                    modified |= add(val);
                return modified;
            }
        };
    } // util
} // core

#endif //CORE24_QUEUE_H
