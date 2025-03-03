//
// Created by brunshweeck on 30 juin 2024.
//

#ifndef CORE24_LISTITERATOR_H
#define CORE24_LISTITERATOR_H

#include <core/util/Iterator.h>

namespace core {
    namespace util {
        /**
         * An iterator for lists that allows the programmer to traverse the list in either direction, modify
         * the list during iteration, and obtain the iterator's current position in the list. A @c ListIterator
         * has no current element; its <I>cursor position</I> always lies between the element that would be returned
         * by a call to @c previous() and the element that would be returned by a call to @c next().
         * An iterator for a list of length @c n has @c n+1 possible cursor positions, as illustrated by the carets
         * (@c ^) below:
         * <pre>
         *          Element(0)   Element(1)   Element(2)   ... Element(n-1)
         * cursor: ^            ^            ^          ^                  ^
         * </pre>
         * Note that the @c ListIterator::remove and @c ListIterator::set methods are <i>not</i> defined in terms
         * of the cursor position;  they are defined to operate on the last element returned by a call to
         * @c ListIterator::next or @c ListIterator::previous.
         *
         * @tparam E the type of elements returned by this list iterator
         *
         * @see Collection
         * @see List
         * @see Iterator
         * @see List::listIterator
         */
        template<class E>
        class ListIterator : public virtual Iterator<E> {
        public:
            // Query Operations

            /**
             * Returns @c true if this list iterator has more elements when
             * traversing the list in the forward direction. (In other words,
             * returns @c true if @c ListIterator::next would return an element rather
             * than throwing an exception.)
             *
             * @return @c true if the list iterator has more elements when
             *         traversing the list in the forward direction
             */
            gbool hasNext() const override = 0;

            /**
             * Returns the next element in the list and advances the cursor position.
             * This method may be called repeatedly to iterate through the list,
             * or intermixed with calls to @c ListIterator::previous to go back and forth.
             * (Note that alternating calls to @c ListIterator::next and @c ListIterator::previous
             * will return the same element repeatedly.)
             *
             * @return the next element in the list
             * @throws NoSuchElementException if the iteration has no next element
             */
            E &next() override = 0;

            /**
             * Returns @c true if this list iterator has more elements when
             * traversing the list in the reverse direction.  (In other words,
             * returns @c true if @c ListIterator::previous would return an element
             * rather than throwing an exception.)
             *
             * @return @c true if the list iterator has more elements when
             *         traversing the list in the reverse direction
             */
            virtual gbool hasPrevious() const = 0;

            /**
             * Returns the previous element in the list and moves the cursor
             * position backwards.  This method may be called repeatedly to
             * iterate through the list backwards, or intermixed with calls to
             * @c ListIterator::next to go back and forth.  (Note that alternating calls
             * to @c ListIterator::next and @c ListIterator::previous will return the same
             * element repeatedly.)
             *
             * @return the previous element in the list
             * @throws NoSuchElementException if the iteration has no previous
             *         element
             */
            virtual E &previous() = 0;

            /**
             * Returns the index of the element that would be returned by a
             * subsequent call to @c ListIterator::next. (Returns list size if the list
             * iterator is at the end of the list.)
             *
             * @return the index of the element that would be returned by a
             *         subsequent call to @c ListIterator::next, or list size if the list
             *         iterator is at the end of the list
             */
            virtual gint nextIndex() const =0;

            /**
             * Returns the index of the element that would be returned by a
             * subsequent call to @c ListIterator::previous. (Returns -1 if the list
             * iterator is at the beginning of the list.)
             *
             * @return the index of the element that would be returned by a
             *         subsequent call to @c ListIterator::previous, or -1 if the list
             *         iterator is at the beginning of the list
             */
            virtual gint previousIndex() const =0;


            // Modification Operations

            /**
             * Removes from the list the last element that was returned by @c ListIterator::next or
             * @c ListIterator::previous (optional operation).  This call can
             * only be made once per call to @c ListIterator::next or @c ListIterator::previous.
             * It can be made only if @c ListIterator::add has not been
             * called after the last call to @c ListIterator::next or @c ListIterator::previous.
             *
             * @throws UnsupportedOperationException if the @c ListIterator::remove
             *         operation is not supported by this list iterator
             * @throws IllegalStateException if neither @c ListIterator::next nor
             *         @c ListIterator::previous have been called, or @c ListIterator::remove or
             *         @c ListIterator::add have been called after the last call to
             *         @c ListIterator::next or @c previous
             */
            void remove() override {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Replaces the last element returned by @c ListIterator::next or
             * @c ListIterator::previous with the specified element (optional operation).
             * This call can be made only if neither @c ListIterator::remove nor
             * @c ListIterator::add have been called after the last call to
             * @c ListIterator::next or @c previous.
             *
             * @param e the element with which to replace the last element returned by
             *          @c ListIterator::next or @c previous
             * @throws UnsupportedOperationException if the @c ListIterator::set operation
             *         is not supported by this list iterator
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some aspect of the specified
             *         element prevents it from being added to this list
             * @throws IllegalStateException if neither @c ListIterator::next nor
             *         @c previous have been called, or @c ListIterator::remove or
             *         @c ListIterator::add have been called after the last call to
             *         @c ListIterator::next or @c previous
             */
            virtual void set(E const &e) {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Inserts the specified element into the list (optional operation).
             * The element is inserted immediately before the element that
             * would be returned by @c ListIterator::next, if any, and after the element
             * that would be returned by @c ListIterator::previous, if any.  (If the
             * list contains no elements, the new element becomes the sole element
             * on the list.)  The new element is inserted before the implicit
             * cursor: a subsequent call to @c ListIterator::next would be unaffected, and a
             * subsequent call to @c previous would return the new element.
             * (This call increases by one the value that would be returned by a
             * call to @c ListIterator::nextIndex or @c ListIterator::previousIndex.)
             *
             * @param e the element to insert
             * @throws UnsupportedOperationException if the @c ListIterator::add method is
             *         not supported by this list iterator
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some aspect of this element
             *         prevents it from being added to this list
             */
            virtual void add(E const &e) {
                CORE_IGNORE(e);
                UnsupportedOperationException().throws($ftrace());
            }
        };

        /**
         * An iterator for lists that allows the programmer to traverse the list in either direction, modify
         * the list during iteration, and obtain the iterator's current position in the list. A @c ListIterator2
         * has no current element; its <I>cursor position</I> always lies between the element that would be returned
         * by a call to @c previous() and the element that would be returned by a call to @c next().
         * An iterator for a list of length @c n has @c n+1 possible cursor positions, as illustrated by the carets
         * (@c ^) below:
         * <pre>
         *          Element(0)   Element(1)   Element(2)   ... Element(n-1)
         * cursor: ^            ^            ^          ^                  ^
         * </pre>
         * Note that the @c ListIterator2::remove and @c ListIterator2::set methods are <i>not</i> defined in terms
         * of the cursor position;  they are defined to operate on the last element returned by a call to
         * @c ListIterator2::next or @c ListIterator2::previous.
         *
         * @tparam E the type of elements returned by this list iterator
         *
         * @see Collection
         * @see List
         * @see Iterator
         * @see List::listIterator
         */
        template<class E>
        class ListIterator2 : public virtual Iterator2<E> {
        public:
            // Query Operations

            /**
             * Returns @c true if this list iterator has more elements when
             * traversing the list in the forward direction. (In other words,
             * returns @c true if @c ListIterator2::next would return an element rather
             * than throwing an exception.)
             *
             * @return @c true if the list iterator has more elements when
             *         traversing the list in the forward direction
             */
            gbool hasNext() const override = 0;

            /**
             * Returns the next element in the list and advances the cursor position.
             * This method may be called repeatedly to iterate through the list,
             * or intermixed with calls to @c ListIterator2::previous to go back and forth.
             * (Note that alternating calls to @c ListIterator2::next and @c ListIterator2::previous
             * will return the same element repeatedly.)
             *
             * @return the next element in the list
             * @throws NoSuchElementException if the iteration has no next element
             */
            E const &next() override = 0;

            /**
             * Returns @c true if this list iterator has more elements when
             * traversing the list in the reverse direction.  (In other words,
             * returns @c true if @c ListIterator2::previous would return an element
             * rather than throwing an exception.)
             *
             * @return @c true if the list iterator has more elements when
             *         traversing the list in the reverse direction
             */
            virtual gbool hasPrevious() const = 0;

            /**
             * Returns the previous element in the list and moves the cursor
             * position backwards.  This method may be called repeatedly to
             * iterate through the list backwards, or intermixed with calls to
             * @c ListIterator2::next to go back and forth.  (Note that alternating calls
             * to @c ListIterator2::next and @c ListIterator2::previous will return the same
             * element repeatedly.)
             *
             * @return the previous element in the list
             * @throws NoSuchElementException if the iteration has no previous
             *         element
             */
            virtual E const &previous() = 0;

            /**
             * Returns the index of the element that would be returned by a
             * subsequent call to @c ListIterator2::next. (Returns list size if the list
             * iterator is at the end of the list.)
             *
             * @return the index of the element that would be returned by a
             *         subsequent call to @c ListIterator2::next, or list size if the list
             *         iterator is at the end of the list
             */
            virtual gint nextIndex() const =0;

            /**
             * Returns the index of the element that would be returned by a
             * subsequent call to @c ListIterator2::previous. (Returns -1 if the list
             * iterator is at the beginning of the list.)
             *
             * @return the index of the element that would be returned by a
             *         subsequent call to @c ListIterator2::previous, or -1 if the list
             *         iterator is at the beginning of the list
             */
            virtual gint previousIndex() const =0;


            // Modification Operations

            /**
             * Removes from the list the last element that was returned by @c ListIterator2::next or
             * @c ListIterator2::previous (optional operation).  This call can
             * only be made once per call to @c ListIterator2::next or @c ListIterator2::previous.
             * It can be made only if @c ListIterator2::add has not been
             * called after the last call to @c ListIterator2::next or @c ListIterator2::previous.
             *
             * @throws UnsupportedOperationException if the @c ListIterator2::remove
             *         operation is not supported by this list iterator
             * @throws IllegalStateException if neither @c ListIterator2::next nor
             *         @c ListIterator2::previous have been called, or @c ListIterator2::remove or
             *         @c ListIterator2::add have been called after the last call to
             *         @c ListIterator2::next or @c previous
             */
            void remove() override {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Replaces the last element returned by @c ListIterator2::next or
             * @c ListIterator2::previous with the specified element (optional operation).
             * This call can be made only if neither @c ListIterator2::remove nor
             * @c ListIterator2::add have been called after the last call to
             * @c ListIterator2::next or @c previous.
             *
             * @param e the element with which to replace the last element returned by
             *          @c ListIterator2::next or @c previous
             * @throws UnsupportedOperationException if the @c ListIterator2::set operation
             *         is not supported by this list iterator
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some aspect of the specified
             *         element prevents it from being added to this list
             * @throws IllegalStateException if neither @c ListIterator2::next nor
             *         @c previous have been called, or @c ListIterator2::remove or
             *         @c ListIterator2::add have been called after the last call to
             *         @c ListIterator2::next or @c previous
             */
            virtual void set(E const &e) {
                CORE_IGNORE(e);
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Inserts the specified element into the list (optional operation).
             * The element is inserted immediately before the element that
             * would be returned by @c ListIterator2::next, if any, and after the element
             * that would be returned by @c ListIterator2::previous, if any.  (If the
             * list contains no elements, the new element becomes the sole element
             * on the list.)  The new element is inserted before the implicit
             * cursor: a subsequent call to @c ListIterator2::next would be unaffected, and a
             * subsequent call to @c previous would return the new element.
             * (This call increases by one the value that would be returned by a
             * call to @c ListIterator2::nextIndex or @c ListIterator2::previousIndex.)
             *
             * @param e the element to insert
             * @throws UnsupportedOperationException if the @c ListIterator2::add method is
             *         not supported by this list iterator
             * @throws ClassCastException if the class of the specified element
             *         prevents it from being added to this list
             * @throws IllegalArgumentException if some aspect of this element
             *         prevents it from being added to this list
             */
            virtual void add(E const &e) {
                CORE_IGNORE(e);
                UnsupportedOperationException().throws($ftrace());
            }
        };
    } // util
} // core

#endif //CORE24_LISTITERATOR_H
