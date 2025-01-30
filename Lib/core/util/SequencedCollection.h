//
// Created by brunshweeck on 30 juin 2024.
//

#ifndef CORE24_SEQUENCEDCOLLECTION_H
#define CORE24_SEQUENCEDCOLLECTION_H

#include "Collection.h"

#include <core/UnsupportedOperationException.h>

namespace core {
    namespace util {
        /**
         * A collection that has a well-defined encounter order, that supports operations at both ends,
         * and that is reversible. The elements of a sequenced collection have an <a id="encounter">
         * <i>encounter order</i></a>, where conceptually the elements have a linear arrangement
         * from the first element to the last element. Given any two elements, one element is
         * either before (closer to the first element) or after (closer to the last element)
         * the other element.
         * <p>
         * (Note that this definition does not imply anything about physical positioning
         * of elements, such as their locations in a computer's memory.)
         * </p>
         * <p>
         * Several methods inherited from the @c Collection interface are required to operate
         * on elements according to this collection's encounter order. For instance, the
         * @em iterator method provides elements starting from the first element,
         * proceeding through successive elements, until the last element. Other methods that are
         * required to operate on elements in encounter order include the following:
         * @em forEach, @em parallelStream, @em spliterator, @em stream,
         * and all overloads of the @c Collection::toArray method.
         * </p>
         * <p>
         * This interface provides methods to add, retrieve, and remove elements at either end
         * of the collection.
         * </p>
         * <p>
         * This interface also defines the @c SequencedCollection::reversed method, which provides
         * a reverse-ordered @em view< of this collection.
         * In the reverse-ordered view, the concepts of first and last are inverted, as are
         * the concepts of successor and predecessor. The first element of this collection is
         * the last element of the reverse-ordered view, and vice versa. The successor of some
         * element in this collection is its predecessor in the reversed view, and vice versa. All
         * methods that respect the encounter order of the collection operate as if the encounter order
         * is inverted. For instance, the @c SequencedCollection::iterator method of the reversed view reports the
         * elements in order from the last element of this collection to the first. The availability of
         * the @c SequencedCollection::reversed method, and its impact on the ordering semantics of all applicable
         * methods, allow convenient iteration, searching, copying, and streaming of the elements of
         * this collection in either forward order or reverse order.
         * </p>
         * @note
         * This interface does not impose any requirements on the @c equals and @c hash
         * methods, because requirements imposed by sub-interfaces @c List and @c SequencedSet
         * (which inherits requirements from @c Set) would be in conflict. See the specifications for
         * @c Collection::equals and @c Collection::hash for further information.
         *
         * @tparam E the type of elements in this collection
         */
        template<class E>
        class SequencedCollection : public virtual Collection<E> {
            CORE_CHECK_SLIMMED_TYPE(E);
            CORE_ALIAS(UNSAFE, Object::UNSAFE);

        public:
            /**
             * Returns a reverse-ordered @em view of this collection.
             * The encounter order of elements in the returned view is the inverse of the encounter
             * order of elements in this collection. The reverse ordering affects all order-sensitive
             * operations, including those on the view collections of the returned view. If the collection
             * implementation permits modifications to this view, the modifications "write through" to the
             * underlying collection. Changes to the underlying collection might or might not be visible
             * in this reversed view, depending upon the implementation.
             *
             * @return a reverse-ordered view of this collection
             */
            virtual SequencedCollection const &reversed() const = 0;

            /**
             * Returns a reverse-ordered @em view of this collection.
             * The encounter order of elements in the returned view is the inverse of the encounter
             * order of elements in this collection. The reverse ordering affects all order-sensitive
             * operations, including those on the view collections of the returned view. If the collection
             * implementation permits modifications to this view, the modifications "write through" to the
             * underlying collection. Changes to the underlying collection might or might not be visible
             * in this reversed view, depending upon the implementation.
             *
             * @return a reverse-ordered view of this collection
             */
            virtual SequencedCollection &reversed() = 0;

            /**
             * Adds an element as the first element of this collection (optional operation).
             * After this operation completes normally, the given element will be a member of
             * this collection, and it will be the first element in encounter order.
             *
             * @note
             * The implementation in this interface always throws @c UnsupportedOperationException.
             *
             * @param val the element to be added
             * @throws UnsupportedOperationException if this collection implementation
             *         does not support this operation
             */
            virtual void addFirst(E const &val) {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Adds an element as the last element of this collection (optional operation).
             * After this operation completes normally, the given element will be a member of
             * this collection, and it will be the last element in encounter order.
             *
             * @note
             * The implementation in this interface always throws @c UnsupportedOperationException.
             *
             * @param val the element to be added.
             * @throws UnsupportedOperationException if this collection implementation
             *         does not support this operation
             */
            virtual void addLast(E const &val) {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Gets the first element of this collection.
             *
             * @note
             * The implementation in this interface obtains an iterator of this collection, and
             * then it obtains an element by calling the iterator's @c next method. Any
             * @c NoSuchElementException thrown is propagated. Otherwise, it returns
             * the element.
             *
             * @return the retrieved element
             * @throws NoSuchElementException if this collection is empty
             */
            virtual E &firstElement() {
                try {
                    Iterator<E> &it = (*this).iterator();
                    E &first = it.next();
                    UNSAFE::deleteInstance(it);
                    return first;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Gets the first element of this collection.
             *
             * @note
             * The implementation in this interface obtains an iterator of this collection, and
             * then it obtains an element by calling the iterator's @c next method. Any
             * @c NoSuchElementException thrown is propagated. Otherwise, it returns
             * the element.
             *
             * @return the retrieved element
             * @throws NoSuchElementException if this collection is empty
             */
            virtual E const &firstElement() const {
                try {
                    Iterator2<E> &it = (*this).iterator();
                    E const &first = it.next();
                    UNSAFE::deleteInstance(it);
                    return first;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Gets the last element of this collection.
             *
             * @note
             * The implementation in this interface obtains an iterator of the reversed view
             * of this collection, and then it obtains an element by calling the iterator's
             * @c next method. Any @c NoSuchElementException thrown is propagated.
             * Otherwise, it returns the element.
             *
             * @return the retrieved element
             * @throws NoSuchElementException if this collection is empty
             */
            virtual E &lastElement() {
                try {
                    Iterator<E> &it = reversed().iterator();
                    E &last = it.next();
                    UNSAFE::deleteInstance(it);
                    return last;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Gets the last element of this collection.
             *
             * @note
             * The implementation in this interface obtains an iterator of the reversed view
             * of this collection, and then it obtains an element by calling the iterator's
             * @c next method. Any @c NoSuchElementException thrown is propagated.
             * Otherwise, it returns the element.
             *
             * @return the retrieved element
             * @throws NoSuchElementException if this collection is empty
             */
            virtual E const &lastElement() const {
                try {
                    Iterator2<E> &it = reversed().iterator();
                    E const &last = it.next();
                    UNSAFE::deleteInstance(it);
                    return last;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes and returns the first element of this collection (optional operation).
             *
             * @note
             * The implementation in this interface obtains an iterator of this collection, and then
             * it obtains an element by calling the iterator's @c next method. Any
             * @c NoSuchElementException thrown is propagated. It then calls the iterator's
             * @c remove method. Any @c UnsupportedOperationException thrown is propagated.
             * Then, it returns the element.
             *
             * @return the removed element
             * @throws NoSuchElementException if this collection is empty
             * @throws UnsupportedOperationException if this collection implementation
             *         does not support this operation
             */
            virtual E &removeFirst() {
                try {
                    Iterator<E> &it = (*this).iterator();
                    E &first = it.next();
                    it.remove();
                    UNSAFE::deleteInstance(it);
                    return first;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes and returns the last element of this collection (optional operation).
             *
             * @note
             * The implementation in this interface obtains an iterator of the reversed view of this
             * collection, and then it obtains an element by calling the iterator's @c next method.
             * Any @c NoSuchElementException thrown is propagated. It then calls the iterator's
             * @c remove method. Any @c UnsupportedOperationException thrown is propagated.
             * Then, it returns the element.
             *
             * @return the removed element
             * @throws NoSuchElementException if this collection is empty
             * @throws UnsupportedOperationException if this collection implementation
             *         does not support this operation
             */
            virtual E &removeLast() {
                try {
                    Iterator<E> &it = reversed().iterator();
                    E &first = it.next();
                    it.remove();
                    UNSAFE::deleteInstance(it);
                    return first;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };
    } // util
} // core

#endif //CORE24_SEQUENCEDCOLLECTION_H
