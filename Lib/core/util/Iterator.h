//
// Created by brunshweeck on 21 juin 2024.
//

#ifndef CORE24_ITERATOR_H
#define CORE24_ITERATOR_H

#include <core/UnsupportedOperationException.h>
#include <core/function/Consumer.h>
#include <core/function/DoubleConsumer.h>
#include <core/function/IntConsumer.h>
#include <core/function/LongConsumer.h>

namespace core {
    namespace util {
        /**
         * An iterator over a collection.
         *
         * @tparam E the type of elements returned by this iterator.
         */
        template<class E>
        class Iterator : public virtual Object {
            CORE_FULL_CHECK_SLIMMED_TYPE(E);

        public:
            /**
             * Returns @c true if the iteration has more elements.
             * In other words, returns @c true if @c Iterator::next would
             * return an element rather than throwing an exception.
             *
             * @return @c true if the iteration has more elements.
             */
            virtual gbool hasNext() const = 0;

            /**
             * Returns the next element in the iteration.
             *
             * @return the next element in the iteration.
             * @throws NoSuchElementException if the iteration has no more elements
             */
            virtual E &next() = 0;

            /**
             * Removes from the underlying collection the last element returned
             * by this iterator (optional operation).  This method can be called
             * only once per call to @c Iterator::next.
             * <p>
             * The behavior of an iterator is unspecified if the underlying collection
             * is modified while the iteration is in progress in any way other than by
             * calling this method, unless an overriding class has specified a
             * concurrent modification policy.
             * </p>
             * <p>
             * The behavior of an iterator is unspecified if this method is called
             * after a call to the @c Iterator::forEach method.
             * </p>
             *
             * @note The default implementation throws an instance of
             *         @c UnsupportedOperationException and performs no other action.
             * @throws UnsupportedOperationException if the @c Iterator::remove
             *          operation is not supported by this iterator
             * @throws IllegalStateException if the @c Iterator::next method has not
             *          yet been called, or the @c Iterator::remove method has already
             *          been called after the last call to the @c Iterator::next method
             */
            virtual void remove() {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Performs the given action for each remaining element until all elements
             * have been processed or the action throws an exception.  Actions are
             * performed in the order of iteration, if that order is specified.
             * Exceptions thrown by the action are relayed to the caller.
             * <p>
             * The behavior of an iterator is unspecified if the action modifies the
             * collection in any way (even by calling the @c Iterator::remove method
             * or other mutator methods of @c Iterator subtypes), unless an overriding
             * class has specified a concurrent modification policy.
             * </p>
             * <p>
             * Subsequent behavior of an iterator is unspecified if the action throws an
             * exception.
             * </p>
             * @note The default implementation behaves as if:
             *      @code
             *      while(hasNext()) {
             *          action.accept(next());
             *      }
             *      @endcode
             * @param action The action to be performed for each element.
             */
            virtual void forEach(function::Consumer<E &> const &action) {
                try {
                    while (hasNext()) action.accept(next());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * A base type for primitive specializations of @c Iterator.  Specialized
             * subtypes are provided for @b int, @b long, and @b double values.
             *
             * <p>
             * The specialized subtype default implementations of @b Iterator::next
             * and @b Iterator::forEach(function::Consumer) box
             * primitive values to instances of their corresponding wrapper class.  Such
             * boxing may offset any advantages gained when using the primitive
             * specializations.  To avoid boxing, the corresponding primitive-based methods
             * should be used.  For example, @b IntIterator::nextInt() and
             * @b IntIterator::forEach(function::IntConsumer)
             * should be used in preference to @b IntIterator::next() and
             * @b IntIterator::forEach(function::Consumer).
             * </p>
             * <p>
             * Iteration of primitive values using boxing-based methods
             * @b next() and @b forEach(),
             * does not affect the order in which the values, transformed to boxed values,
             * are encountered.
             * </p>
             *
             * @tparam T the type of elements returned by this PrimitiveIterator.  The
             *        type must be a wrapper type for a primitive type, such as
             *        @c Integer for the primitive @c int type.
             * @tparam T_CONS the type of primitive consumer.  The type must be a
             *        primitive specialization of @b function::Consumer for
             *        @c T, such as @b function::IntConsumer for
             *        @c Integer.
             */
            template<class T, class T_CONS>
            class OfPrimitive;
        };

        /**
         * An iterator over a collection.
         *
         * @note the element returned by this iterator is not modifiable.
         *      It's mostly used for Sets structures.
         * @tparam E the type of elements returned by this iterator.
         */
        template<class E>
        class Iterator2 : public virtual Object {
            CORE_FULL_CHECK_SLIMMED_TYPE(E);

        public:
            /**
             * Returns @c true if the iteration has more elements.
             * In other words, returns @c true if @c Iterator2::next would
             * return an element rather than throwing an exception.
             *
             * @return @c true if the iteration has more elements.
             */
            virtual gbool hasNext() const = 0;

            /**
             * Returns the next element in the iteration.
             *
             * @return the next element in the iteration.
             * @throws NoSuchElementException if the iteration has no more elements
             */
            virtual E const &next() = 0;

            /**
             * Removes from the underlying collection the last element returned
             * by this iterator (optional operation).  This method can be called
             * only once per call to @c Iterator2::next.
             * <p>
             * The behavior of an iterator is unspecified if the underlying collection
             * is modified while the iteration is in progress in any way other than by
             * calling this method, unless an overriding class has specified a
             * concurrent modification policy.
             * </p>
             * <p>
             * The behavior of an iterator is unspecified if this method is called
             * after a call to the @c Iterator2::forEach method.
             * </p>
             *
             * @note The default implementation throws an instance of
             *         @c UnsupportedOperationException and performs no other action.
             * @throws UnsupportedOperationException if the @c Iterator2::remove
             *          operation is not supported by this iterator
             * @throws IllegalStateException if the @c Iterator2::next method has not
             *          yet been called, or the @c Iterator2::remove method has already
             *          been called after the last call to the @c Iterator2::next method
             */
            virtual void remove() {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Performs the given action for each remaining element until all elements
             * have been processed or the action throws an exception.  Actions are
             * performed in the order of iteration, if that order is specified.
             * Exceptions thrown by the action are relayed to the caller.
             * <p>
             * The behavior of an iterator is unspecified if the action modifies the
             * collection in any way (even by calling the @c Iterator::remove method
             * or other mutator methods of @c Iterator subtypes), unless an overriding
             * class has specified a concurrent modification policy.
             * </p>
             * <p>
             * Subsequent behavior of an iterator is unspecified if the action throws an
             * exception.
             * </p>
             * @note The default implementation behaves as if:
             *      @code
             *      while(hasNext()) {
             *          action.accept(next());
             *      }
             *      @endcode
             * @param action The action to be performed for each element.
             */
            virtual void forEach(function::Consumer<E> const &action) {
                try {
                    while (hasNext()) action.accept(next());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };

        template<class E>
        template<class T, class T_CONS>
        class Iterator<E>::OfPrimitive : public virtual Iterator<T> {
        public:
            /**
             * Performs the given action for each remaining element until all elements
             * have been processed or the action throws an exception.  Actions are
             * performed in the order of iteration, if that order is specified.
             * Exceptions thrown by the action are relayed to the caller.
             * <p>
             * The behavior of an iterator is unspecified if the action modifies the
             * source of elements in any way (even by calling the @b remove
             * method or other mutator methods of @c Iterator subtypes),
             * unless an overriding class has specified a concurrent modification policy.
             * </p>
             * <p>
             * Subsequent behavior of an iterator is unspecified if the action throws an
             * exception.
             * </p>
             *
             * @param action The action to be performed for each element
             */
            virtual void forEach(T_CONS const &action) = 0;
        };

        /**
         * An Iterator specialized for @c int values.
         */
        class IntIterator : public virtual Iterator<>::OfPrimitive<Integer, function::IntConsumer> {
        public:
            /**
             * Returns the next @c int element in the iteration.
             *
             * @return the next @c int element in the iteration
             * @throws NoSuchElementException if the iteration has no more elements
             */
            virtual gint nextInt() = 0;

            /**
             * @note
             * <p>
             * The default implementation behaves as if:
             * @code
             *     while (hasNext())
             *         action.accept(nextInt());
             * @endcode
             * </p>
             */
            void forEach(const function::IntConsumer &action) override {
                try {
                    while (hasNext()) action.accept(nextInt());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @note
             * The default implementation boxes the result of calling
             * @b nextInt(), and returns that boxed result.
             */
            Integer &next() override {
                try {
                    return UNSAFE::newInstance<Integer>(nextInt());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @note
             * If the action is an instance of @c IntConsumer then it is cast
             * to @c IntConsumer and passed to @b forEach;
             * otherwise the action is adapted to an instance of
             * @c IntConsumer, by boxing the argument of @c IntConsumer,
             * and then passed to @b forEach.
             */
            void forEach(const function::Consumer<Integer &> &action) override {
                try {
                    return forEach((function::IntConsumer) [action](Integer i) -> void { action.accept(i); });
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };

        /**
         * An Iterator specialized for @c long values.
         */
        class LongIterator : public virtual Iterator<>::OfPrimitive<Long, function::LongConsumer> {
        public:
            /**
             * Returns the next @c int element in the iteration.
             *
             * @return the next @c int element in the iteration
             * @throws NoSuchElementException if the iteration has no more elements
             */
            virtual glong nextLong() = 0;

            /**
             * @note
             * <p>
             * The default implementation behaves as if:
             * @code
             *     while (hasNext())
             *         action.accept(nextLong());
             * @endcode
             * </p>
             */
            void forEach(const function::LongConsumer &action) override {
                try {
                    while (hasNext()) action.accept(nextLong());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @note
             * The default implementation boxes the result of calling
             * @b nextLong(), and returns that boxed result.
             */
            Long &next() override {
                try {
                    return UNSAFE::newInstance<Long>(nextLong());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @note
             * If the action is an instance of @c LongConsumer then it is cast
             * to @c LongConsumer and passed to @b forEach;
             * otherwise the action is adapted to an instance of
             * @c LongConsumer, by boxing the argument of @c LongConsumer,
             * and then passed to @b forEach.
             */
            void forEach(const function::Consumer<Long &> &action) override {
                try {
                    return forEach((function::LongConsumer) [action](Long i) -> void { action.accept(i); });
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };

        /**
         * An Iterator specialized for @c double values.
         */
        class DoubleIterator : public virtual Iterator<>::OfPrimitive<Double, function::DoubleConsumer> {
        public:
            /**
             * Returns the next @c int element in the iteration.
             *
             * @return the next @c int element in the iteration
             * @throws NoSuchElementException if the iteration has no more elements
             */
            virtual gdouble nextDouble() = 0;

            /**
             * @note
             * <p>
             * The default implementation behaves as if:
             * @code
             *     while (hasNext())
             *         action.accept(nextDouble());
             * @endcode
             * </p>
             */
            void forEach(const function::DoubleConsumer &action) override {
                try {
                    while (hasNext()) action.accept(nextDouble());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @note
             * The default implementation boxes the result of calling
             * @b nextDouble(), and returns that boxed result.
             */
            Double &next() override {
                try {
                    return UNSAFE::newInstance<Double>(nextDouble());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * @note
             * If the action is an instance of @c DoubleConsumer then it is cast
             * to @c DoubleConsumer and passed to @b forEach;
             * otherwise the action is adapted to an instance of
             * @c DoubleConsumer, by boxing the argument of @c DoubleConsumer,
             * and then passed to @b forEach.
             */
            void forEach(const function::Consumer<Double &> &action) override {
                try {
                    return forEach((function::DoubleConsumer) [action](Double i) -> void { action.accept(i); });
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };

        //
    }
}

#endif // CORE24_ITERATOR_H
