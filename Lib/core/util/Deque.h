//
// Created by brunshweeck on 12 juil. 2024.
//

#ifndef CORE24_DEQUE_H
#define CORE24_DEQUE_H

#include <core/util/Arrays.h>
#include <core/util/Queue.h>
#include <core/util/SequencedCollection.h>

namespace core {
    namespace util {
        /**
         * A linear collection that supports element insertion and removal at
         * both ends.  The name <i>deque</i> is short for "double ended queue"
         * and is usually pronounced "deck".  Most @c Deque
         * implementations place no fixed limits on the number of elements
         * they may contain, but this interface supports capacity-restricted
         * deques as well as those with no fixed size limit.
         *
         * <p>
         * This interface defines methods to access the elements at both
         * ends of the deque.  Methods are provided to insert, remove, and
         * examine the element.  Each of these methods exists in two forms:
         * one throws an exception if the operation fails, the other returns a
         * special value (either @c null or @c false, depending on
         * the operation).  The latter form of the insert operation is
         * designed specifically for use with capacity-restricted
         * @c Deque implementations; in most implementations, insert
         * operations cannot fail.
         * </p>
         * <p>
         * The twelve methods described above are summarized in the
         * following table: <br/>
         * 1- Summary of Deque methods
         *    - First Element (Head)
         *      - Throws Exception:
         *        - @c addFirst(e)
         *        - @c removeFirst()
         *        - @c firstElement()
         *      - Return special Value:
         *        - @c offerFirst(e)
         *        - @c pollFirst()
         *        - @c peekFirst()
         *    - Last Element (Tail)
         *      - Throws Exception:
         *        - @c addLast(e)
         *        - @c removeLast()
         *        - @c lastElement()
         *      - Return special Value:
         *        - @c offerLast(e)
         *        - @c pollLast()
         *        - @c peekLast()
         * </p>
         * <p>
         * This interface extends the @c Queue interface.  When a deque is
         * used as a queue, FIFO (First-In-First-Out) behavior results.  Elements are
         * added at the end of the deque and removed from the beginning.  The methods
         * inherited from the @c Queue interface are precisely equivalent to
         * @c Deque methods as indicated in the following table: <br/>
         * 2- Comparison of Queue and Deque methods
         *    - @a Queue Method
         *      - @c add(e)
         *      - @c offer(e)
         *      - @c remove()
         *      - @c poll()
         *      - @c head()
         *      - @c peek()
         *    - Equivalent @a Deque Method
         *      - @c addLast(e)
         *      - @c offerLast(e)
         *      - @c removeFirst()
         *      - @c pollFirst()
         *      - @c firstElement()
         *      - @c peekFirst()
         * </p>
         * <p>
         * Deques can also be used as LIFO (Last-In-First-Out) stacks.  This
         * interface should be used in preference to the legacy @c Stack class.
         * When a deque is used as a stack, elements are pushed and popped from the
         * beginning of the deque.  Stack methods are equivalent to @c Deque
         * methods as indicated in the table below:
         * 3- Comparison of Stack and Deque methods
         *    - @a Stack Method
         *      - @c push(e)
         *      - @c pop()
         *      - @c peek()
         *    - Equivalent @a Deque Method
         *      - @c addFirst(e)
         *      - @c removeFirst()
         *      - @c firstElement()
         * </p>
         * <p>
         * Note that the @c peek method works equally well when
         * a deque is used as a queue or a stack; in either case, elements are
         * drawn from the beginning of the deque.
         * </p>
         * <p>
         * This interface provides two methods to remove interior
         * elements, @c removeFirstOccurrence and @c removeLastOccurrence.
         * </p>
         * <p>
         * Unlike the @c List interface, this interface does not
         * provide support for indexed access to elements.
         * </p>
         * <p>
         * While @c Deque implementations are not strictly required
         * to prohibit the insertion of null elements, they are strongly
         * encouraged to do so.  Users of any @c Deque implementations
         * that do allow null elements are strongly encouraged <i>not</i> to
         * take advantage of the ability to insert nulls.  This is so because
         * @c null is used as a special return value by various methods
         * to indicate that the deque is empty.
         * </p>
         * <p>
         * @c Deque implementations generally do not define
         * element-based versions of the @c equals and @c hash
         * methods, but instead inherit the identity-based versions from class
         * @c Object.
         * </p>
         *
         * @tparam E the type of elements held in this deque
         */
        template<class E>
        class Deque : public virtual Queue<E>, public virtual SequencedCollection<E> {
            CORE_ALIAS(DequeView, typename Class<Deque>::Pointer);
            CORE_ALIAS(UNSAFE, Object::UNSAFE);

        public:
            /**
             * Inserts the specified element at the front of this deque if it is
             * possible to do so immediately without violating capacity restrictions,
             * throwing an @c IllegalStateException if no space is currently
             * available.  When using a capacity-restricted deque, it is generally
             * preferable to use method @c offerFirst.
             *
             * @param val the element to add
             * @throws IllegalStateException if the element cannot be added at this
             *         time due to capacity restrictions
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this deque
             */
            void addFirst(const E &val) override = 0;

            /**
             * Inserts the specified element at the end of this deque if it is
             * possible to do so immediately without violating capacity restrictions,
             * throwing an @c IllegalStateException if no space is currently
             * available.  When using a capacity-restricted deque, it is generally
             * preferable to use method @c offerLast.
             *
             * <p>
             * This method is equivalent to @c add.
             * </p>
             *
             * @param val the element to add
             * @throws IllegalStateException if the element cannot be added at this
             *         time due to capacity restrictions
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this deque
             */
            void addLast(const E &val) override = 0;

            /**
             * Inserts the specified element at the front of this deque unless it would
             * violate capacity restrictions.  When using a capacity-restricted deque,
             * this method is generally preferable to the @c addFirst method,
             * which can fail to insert an element only by throwing an exception.
             *
             * @param val the element to add
             * @return @c true if the element was added to this deque, else
             *         @c false
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this deque
             */
            virtual gbool offerFirst(E const &val) = 0;

            /**
             * Inserts the specified element at the end of this deque unless it would
             * violate capacity restrictions.  When using a capacity-restricted deque,
             * this method is generally preferable to the @c addLast method,
             * which can fail to insert an element only by throwing an exception.
             *
             * @param val the element to add
             * @return @c true if the element was added to this deque, else
             *         @c false
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this deque
             */
            virtual gbool offerLast(E const &val) = 0;

            /**
             * Retrieves and removes the first element of this deque.  This method
             * differs from @c pollFirst only in that it throws an
             * exception if this deque is empty.
             *
             * @return the head of this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &removeFirst() override = 0;

            /**
             * Retrieves and removes the last element of this deque.  This method
             * differs from @c pollLast only in that it throws an
             * exception if this deque is empty.
             *
             * @return the tail of this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &removeLast() override = 0;

            /**
             * Retrieves and removes the first element of this deque,
             * or returns @c null if this deque is empty.
             *
             * @return the head of this deque, or @c null if this deque is empty
             */
            virtual Object &pollFirst() = 0;

            /**
             * Retrieves and removes the last element of this deque,
             * or returns @c null if this deque is empty.
             *
             * @return the tail of this deque, or @c null if this deque is empty
             */
            virtual Object &pollLast() = 0;

            /**
             * Retrieves, but does not remove, the first element of this deque.
             *
             * This method differs from @c peekFirst only in that it
             * throws an exception if this deque is empty.
             *
             * @return the head of this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &firstElement() override = 0;

            /**
             * Retrieves, but does not remove, the first element of this deque.
             *
             * This method differs from @c peekFirst only in that it
             * throws an exception if this deque is empty.
             *
             * @return the head of this deque
             * @throws NoSuchElementException if this deque is empty
             */
            const E &firstElement() const override = 0;

            /**
             * Retrieves, but does not remove, the last element of this deque.
             * This method differs from @c peekLast only in that it
             * throws an exception if this deque is empty.
             *
             * @return the tail of this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &lastElement() override = 0;

            /**
             * Retrieves, but does not remove, the last element of this deque.
             * This method differs from @c peekLast only in that it
             * throws an exception if this deque is empty.
             *
             * @return the tail of this deque
             * @throws NoSuchElementException if this deque is empty
             */
            const E &lastElement() const override = 0;

            /**
             * Retrieves, but does not remove, the first element of this deque,
             * or returns @c null if this deque is empty.
             *
             * @return the head of this deque, or @c null if this deque is empty
             */
            virtual Object &peekFirst() = 0;

            /**
             * Retrieves, but does not remove, the first element of this deque,
             * or returns @c null if this deque is empty.
             *
             * @return the head of this deque, or @c null if this deque is empty
             */
            virtual Object const &peekFirst() const = 0;

            /**
             * Retrieves, but does not remove, the last element of this deque,
             * or returns @c null if this deque is empty.
             *
             * @return the tail of this deque, or @c null if this deque is empty
             */
            virtual Object &peekLast() = 0;

            /**
             * Retrieves, but does not remove, the last element of this deque,
             * or returns @c null if this deque is empty.
             *
             * @return the tail of this deque, or @c null if this deque is empty
             */
            virtual Object const &peekLast() const = 0;

            /**
             * Removes the first occurrence of the specified element from this deque.
             * If the deque does not contain the element, it is unchanged.
             * More formally, removes the first element @c e such that
             * @c Object::equals(o, e) (if such an element exists).
             * Returns @c true if this deque contained the specified element
             * (or equivalently, if this deque changed as a result of the call).
             *
             * @param val element to be removed from this deque, if present
             * @return @c true if an element was removed as a result of this call
             */
            virtual gbool removeFirstOccurrence(E const &val) = 0;

            /**
             * Removes the last occurrence of the specified element from this deque.
             * If the deque does not contain the element, it is unchanged.
             * More formally, removes the last element @c e such that
             * @c Object::equals(o, e) (if such an element exists).
             * Returns @c true if this deque contained the specified element
             * (or equivalently, if this deque changed as a result of the call).
             *
             * @param val element to be removed from this deque, if present
             * @return @c true if an element was removed as a result of this call (@a optional)
             */
            virtual gbool removeLastOccurrence(E const &val) = 0;

            // Queue Methods

            /**
             * Inserts the specified element into the queue represented by this deque
             * (in other words, at the tail of this deque) if it is possible to do so
             * immediately without violating capacity restrictions, returning
             * @c true upon success and throwing an
             * @c IllegalStateException if no space is currently available.
             * When using a capacity-restricted deque, it is generally preferable to
             * use @c offer.
             *
             * <p>
             * This method is equivalent to @c addLast.
             * </p>
             *
             * @param val the element to add
             * @return @c true (as specified by @c Collection::add)
             * @throws IllegalStateException if the element cannot be added at this
             *         time due to capacity restrictions
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this deque
             */
            gbool add(const E &val) override {
                try { return Queue<E>::add(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts the specified element into the queue represented by this deque
             * (in other words, at the tail of this deque) if it is possible to do so
             * immediately without violating capacity restrictions, returning
             * @c true upon success and @c false if no space is currently
             * available.  When using a capacity-restricted deque, this method is
             * generally preferable to the @c add method, which can fail to
             * insert an element only by throwing an exception.
             *
             * <p>
             * This method is equivalent to @c offerLast.
             * </p>
             *
             * @param val the element to add
             * @return @c true if the element was added to this deque, else
             *         @c false
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this deque
             */
            gbool offer(const E &val) override = 0;

            /**
             * Retrieves and removes the head of the queue represented by this deque
             * (in other words, the first element of this deque).
             * This method differs from @c poll() only in that it
             * throws an exception if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c removeFirst().
             * </p>
             *
             * @return the head of the queue represented by this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &remove() override {
                try { return Queue<E>::remove(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Retrieves and removes the head of the queue represented by this deque
             * (in other words, the first element of this deque), or returns
             * @c null if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c pollFirst().
             * </p>
             *
             * @return the first element of this deque, or @c null if
             *         this deque is empty
             */
            Object &poll() override = 0;

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque (in other words, the first element of this deque).
             * This method differs from @c peek only in that it throws an
             * exception if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c firstElement().
             * </p>
             *
             * @return the head of the queue represented by this deque
             * @throws NoSuchElementException if this deque is empty
             */
            E &element() override {
                try { return Queue<E>::element(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque (in other words, the first element of this deque).
             * This method differs from @c peek only in that it throws an
             * exception if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c firstElement().
             * </p>
             *
             * @return the head of the queue represented by this deque
             * @throws NoSuchElementException if this deque is empty
             */
            const E &element() const override {
                try { return Queue<E>::element(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque (in other words, the first element of this deque), or
             * returns @c null if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c peekFirst().
             * </p>
             *
             * @return the head of the queue represented by this deque, or
             *         @c null if this deque is empty
             */
            Object &peek() override = 0;

            /**
             * Retrieves, but does not remove, the head of the queue represented by
             * this deque (in other words, the first element of this deque), or
             * returns @c null if this deque is empty.
             *
             * <p>
             * This method is equivalent to @c peekFirst().
             * </p>
             *
             * @return the head of the queue represented by this deque, or
             *         @c null if this deque is empty
             */
            const Object &peek() const override = 0;

            /**
             * Adds all the elements in the specified collection at the end
             * of this deque, as if by calling @c addLast on each one,
             * in the order that they are returned by the collection's iterator.
             *
             * <p>
             * When using a capacity-restricted deque, it is generally preferable
             * to call @c offer separately on each element.
             * </p>
             *
             * <p>
             * An exception encountered while trying to add an element may result
             * in only some of the elements having been successfully added when
             * the associated exception is thrown.
             * </p>
             *
             * @param c the elements to be inserted into this deque
             * @return @c true if this deque changed as a result of the call
             * @throws IllegalStateException if not all the elements can be added at
             *         this time due to insertion restrictions
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this deque
             */
            gbool addAll(const Collection<E> &c) override {
                try { return Queue<E>::addAll(c); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // Stack Methods

            /**
             * Pushes an element onto the stack represented by this deque (in other
             * words, at the head of this deque) if it is possible to do so
             * immediately without violating capacity restrictions, throwing an
             * @c IllegalStateException if no space is currently available.
             *
             * <p>
             * This method is equivalent to @c addFirst.
             * </p>
             *
             * @param val the element to push
             * @throws IllegalStateException if the element cannot be added at this
             *         time due to capacity restrictions
             * @throws IllegalArgumentException if some property of the specified
             *         element prevents it from being added to this deque
             */
            virtual void push(E const &val) = 0;

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
             * @throws NoSuchElementException if this deque is empty
             */
            virtual E &pop() = 0;

            // Collection Methods

            /**
             * Removes the first occurrence of the specified element from this deque.
             * If the deque does not contain the element, it is unchanged.
             * More formally, removes the first element @c e such that
             * @c Object::equals(o, e) (if such an element exists).
             * Returns @c true if this deque contained the specified element
             * (or equivalently, if this deque changed as a result of the call).
             *
             * <p>
             * This method is equivalent to @c removeFirstOccurrence(Object).
             * </p>
             *
             * @param val element to be removed from this deque, if present
             * @return @c true if an element was removed as a result of this call
             */
            gbool remove(const E &val) override = 0;

            /**
             * Returns @c true if this deque contains the specified element.
             * More formally, returns @c true if and only if this deque contains
             * at least one element @c e such that @c Object::equals(o, e).
             *
             * @param val element whose presence in this deque is to be tested
             * @return @c true if this deque contains the specified element
             */
            gbool contains(const E &val) const override = 0;

            /**
             * Returns the number of elements in this deque.
             *
             * @return the number of elements in this deque
             */
            gint size() const override = 0;

            /**
             * Returns an iterator over the elements in this deque in proper sequence.
             * The elements will be returned in order from first (head) to last (tail).
             *
             * @return an iterator over the elements in this deque in proper sequence
             */
            Iterator<E> &iterator() override = 0;

            /**
             * Returns an iterator over the elements in this deque in proper sequence.
             * The elements will be returned in order from first (head) to last (tail).
             *
             * @return an iterator over the elements in this deque in proper sequence
             */
            Iterator2<E> &iterator() const override = 0;

            /**
             * Returns an iterator over the elements in this deque in reverse
             * sequential order.  The elements will be returned in order from
             * last (tail) to first (head).
             *
             * @return an iterator over the elements in this deque in reverse
             * sequence
             */
            virtual Iterator<E> &descendingIterator() = 0;

            /**
             * Returns an iterator over the elements in this deque in reverse
             * sequential order.  The elements will be returned in order from
             * last (tail) to first (head).
             *
             * @return an iterator over the elements in this deque in reverse
             * sequence
             */
            virtual Iterator2<E> &descendingIterator() const = 0;

            /**
             * Returns a reverse-ordered @em view of this collection.
             * The encounter order of elements in the returned view is the inverse of the encounter
             * order of elements in this collection. The reverse ordering affects all order-sensitive
             * operations, including those on the view collections of the returned view. If the collection
             * implementation permits modifications to this view, the modifications "write through" to the
             * underlying collection. Changes to the underlying collection might or might not be visible
             * in this reversed view, depending upon the implementation.
             *
             * @note
             * The implementation in this interface returns a reverse-ordered Deque
             * view. The @c reversed() method of the view returns a reference
             * to this Deque. Other operations on the view are implemented via calls to
             * public methods on this Deque. The exact relationship between calls on the
             * view and calls on this Deque is unspecified. However, order-sensitive
             * operations generally behave as if they delegate to the appropriate method
             * with the opposite orientation. For example, calling @c firstElement on
             * the view might result in a call to @c lastElement on this Deque.
             *
             * @return a reverse-ordered view of this collection, as a @c Deque
             */
            Deque const &reversed() const override {
                try {
                    if (!reverse)
                        return UNSAFE::newInstance<Reverse>(CORE_CAST(Deque<E> &, *this));

                    return *reverse;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a reverse-ordered @em view of this collection.
             * The encounter order of elements in the returned view is the inverse of the encounter
             * order of elements in this collection. The reverse ordering affects all order-sensitive
             * operations, including those on the view collections of the returned view. If the collection
             * implementation permits modifications to this view, the modifications "write through" to the
             * underlying collection. Changes to the underlying collection might or might not be visible
             * in this reversed view, depending upon the implementation.
             *
             * @note
             * The implementation in this interface returns a reverse-ordered Deque
             * view. The @c reversed() method of the view returns a reference
             * to this Deque. Other operations on the view are implemented via calls to
             * public methods on this Deque. The exact relationship between calls on the
             * view and calls on this Deque is unspecified. However, order-sensitive
             * operations generally behave as if they delegate to the appropriate method
             * with the opposite orientation. For example, calling @c firstElement on
             * the view might result in a call to @c lastElement on this Deque.
             *
             * @return a reverse-ordered view of this collection, as a @c Deque
             */
            Deque &reversed() override {
                try {
                    if (!reverse)
                        return UNSAFE::newInstance<Reverse>(*this);

                    return *reverse;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            ~Deque() override {
                DequeView v = reverse;
                reverse = null;
                if (v)
                    UNSAFE::deleteInstance(*v);
            }

        private:
            // :::::::::::::::::::::: Deque Views Fields :::::::::::::::::::::::::::::
            /**
             * The reversed view of this deque
             */
            DequeView reverse = {};

            // :::::::::::::::::::::: Deque Views Class ::::::::::::::::::::::::::::::
            /**
             * The Reversed view of Deque
             */
            class Reverse;
        };

        template<class E>
        class Deque<E>::Reverse final : public Deque<E> {
            Deque<E> &deque;

        public:
            CORE_EXPLICIT Reverse(Deque<E> &deque) : deque(deque) {
                deque.reverse = this;
            }

            void addFirst(const E &val) override {
                try { deque.addLast(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void addLast(const E &val) override {
                try { deque.addFirst(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool offerFirst(const E &val) override {
                try { return deque.offerLast(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool offerLast(const E &val) override {
                try { return deque.offerFirst(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &removeFirst() override {
                try { return deque.removeLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &removeLast() override {
                try { return deque.removeFirst(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Object &pollFirst() override {
                try { return deque.pollLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Object &pollLast() override {
                try { return deque.pollFirst(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &firstElement() override {
                try { return deque.lastElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &firstElement() const override {
                try { return deque.lastElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &lastElement() override {
                try { return deque.firstElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &lastElement() const override {
                try { return deque.firstElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Object &peekFirst() override {
                try { return deque.peekLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Object &peekFirst() const override {
                try { return deque.peekLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Object &peekLast() override {
                try { return deque.peekFirst(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Object &peekLast() const override {
                try { return deque.peekFirst(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool removeFirstOccurrence(const E &val) override {
                try { return deque.removeLastOccurrence(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool removeLastOccurrence(const E &val) override {
                try { return deque.removeFirstOccurrence(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool offer(const E &val) override {
                try { return deque.offerFirst(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Object &poll() override {
                try { return deque.pollLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Object &peek() override {
                try { return deque.peekLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Object &peek() const override {
                try { return deque.peekLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void push(const E &val) override {
                try { deque.addLast(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &pop() override {
                try { return deque.removeLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool remove(const E &val) override {
                try { return deque.removeLastOccurrence(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool contains(const E &val) const override {
                try { return deque.contains(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gint size() const override {
                try { return deque.size(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try { return deque.isEmpty(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator<E> &iterator() override {
                try { return deque.descendingIterator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &iterator() const override {
                try {
                    return CORE_XCAST(Deque<E> const, deque).descendingIterator();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator<E> &descendingIterator() override {
                try { return deque.iterator(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Iterator2<E> &descendingIterator() const override {
                try {
                    return CORE_XCAST(Deque<E> const, deque).iterator();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    return Arrays::reverse(deque.toArray());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try { deque.clear(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            gbool add(const E &val) override {
                try { deque.addFirst(val); } catch (Throwable const &ex) { ex.throws($ftrace()); }
                return true;
            }

            E &remove() override {
                try { return deque.removeLast(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            E &element() override {
                try { return deque.lastElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const E &element() const override {
                try { return deque.lastElement(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            const Deque<E> &reversed() const override { return deque; }

            Deque<E> &reversed() override { return deque; }
        };
    } // util
} // core

#endif //CORE24_DEQUE_H
