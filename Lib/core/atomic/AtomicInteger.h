//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_ATOMICINTEGER_H
#define CORE24_ATOMICINTEGER_H

#include <core/lang/Integer.h>

namespace core {
    namespace atomic {
        /**
         * A @c integer value that may be updated atomically.
         * An @c AtomicInteger is used in applications such as atomically incremented
         * sequence numbers, and cannot be used as a replacement for an @c Integer.
         * However, this class does extend @c Number to allow uniform access by tools and
         * utilities that deal with numerically-based classes.
         */
        class AtomicInteger final : public virtual Number {
            CORE_ADD_GLOBAL_FRIENDS();

            volatile gint value = 0;

        public:
            /**
             * Creates a new AtomicInteger with the given initial value.
             *
             * @param initialValue the initial value
             */
            AtomicInteger(gint initialValue);

            /**
             * Creates a new AtomicInteger with initial value @c 0.
             */
            AtomicInteger() CORE_NOTHROW;

            /**
             * Returns the current value,
             * with memory semantics of reading as if the variable was declared @c volatile.
             *
             * @return the current value
             */
            gint get() const;

            /**
             * Sets the value to @c newValue,
             * with memory semantics of reading as if the variable was declared @c volatile.
             *
             * @param newValue the new value
             */
            void set(gint newValue);

            /**
             * Sets the value to @c newValue,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void lazySet(gint newValue);

            /**
             * Atomically sets the value to @c newValue and returns the old value,
             * with the memory semantics of @c setVolatile method and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile method
             *
             * @param newValue the new value
             * @return the previous value
             */
            gint getAndSet(gint newValue);

            /**
             * Atomically sets the value to @c newValue
             * with the memory semantics of @c setVolatile if the variable's current value,
             * referred to as the <em>witness value</em>, @c == the @c expectedValue,
             * as accessed with the memory semantics of @c getVolatile.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return @c true if successful. False return indicates that
             * the actual value was not equal to the expected value.
             */
            gbool compareAndSet(gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the value to @c newValue
             * with the memory semantics of @c setVolatile if the variable's
             * current value, referred to as the <em>witness value</em>, @c == the
             * @c expectedValue, as accessed with the memory semantics of @c getVolatile.
             *
             * @deprecated This method has plain memory effects but the method
             * name implies volatile memory effects (see methods such as
             * @c compareAndExchange and @c compareAndSet).  To avoid
             * confusion over plain or volatile memory effects it is recommended that
             * the method @c weakCompareAndSetPlain be used instead.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return @c true if successful
             * @see #weakCompareAndSetPlain
             */
            gbool weakCompareAndSet(gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the value to @c newValue
             * with the semantics of @c set if the variable's current value,
             * referred to as the <em>witness value</em>, @c == the
             * @c expectedValue, as accessed with the memory semantics of @c get.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return @c true if successful
             */
            gbool weakCompareAndSetPlain(gint expectedValue, gint newValue);

            /**
             * Atomically increments the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             * <p>
             *  Equivalent to @c getAndAdd(1).
             * </p>
             *
             * @return the previous value
             */
            gint getAndIncrement();

            /**
             * Atomically decrements the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             * <p>
             *  Equivalent to @c getAndAdd(-1).
             * </p>
             * @return the previous value
             */
            gint getAndDecrement();

            /**
             * Atomically adds the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             *
             * @param delta the value to add.
             * @return the previous value
             */
            gint getAndAdd(gint delta);

            /**
             * Atomically increments the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             * <p>
             * Equivalent to @c addAndGet(1).
             * </p>
             * @return the updated value
             */
            gint incrementAndGet();

            /**
             * Atomically decrements the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             * <p>
             *  Equivalent to @c addAndGet(-1).
             * </p>
             * @return the updated value
             */
            gint decrementAndGet();

            /**
             * Atomically adds the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             *
             * @param delta the value to add
             * @return the updated value
             */
            gint addAndGet(gint delta);

            /**
             * Atomically updates (with memory effects as specified by @c compareAndSet)
             * the current value with the results of
             * applying the given function, returning the previous value. The
             * function should be side-effect-free, since it may be re-applied
             * when attempted updates fail due to contention among threads.
             *
             * @param updateFunction a side-effect-free function
             * @return the previous value
             */
            gint getAndUpdate(function::IntUnaryOperator const &updateFunction);

            /**
             * Atomically updates (with memory effects as specified by @c compareAndSet)
             * the current value with the results of
             * applying the given function, returning the updated value. The
             * function should be side-effect-free, since it may be re-applied
             * when attempted updates fail due to contention among threads.
             *
             * @param updateFunction a side-effect-free function
             * @return the updated value
             */
            gint updateAndGet(function::IntUnaryOperator const &updateFunction);

            /**
             * Atomically updates (with memory effects as specified by @c compareAndSet)
             * the current value with the results of
             * applying the given function to the current and given values,
             * returning the previous value. The function should be
             * side-effect-free, since it may be re-applied when attempted
             * updates fail due to contention among threads.  The function is
             * applied with the current value as its first argument, and the
             * given update as the second argument.
             *
             * @param x the update value
             * @param accumulatorFunction a side-effect-free function of two arguments
             * @return the previous value
             */
            gint getAndAccumulate(gint x, function::IntBinaryOperator const &accumulatorFunction);

            /**
             * Atomically updates (with memory effects as specified by @c compareAndSet)
             * the current value with the results of
             * applying the given function to the current and given values,
             * returning the updated value. The function should be
             * side-effect-free, since it may be re-applied when attempted
             * updates fail due to contention among threads.  The function is
             * applied with the current value as its first argument, and the
             * given update as the second argument.
             *
             * @param x the update value
             * @param accumulatorFunction a side-effect-free function of two arguments
             * @return the updated value
             */
            gint accumulateAndGet(gint x, function::IntBinaryOperator const &accumulatorFunction);

            /**
             * Returns the String representation of the current value.
             * @return the String representation of the current value
             */
            String toString() const override;

            /**
             * Returns the current value of this @c AtomicInteger as a @c gint
             * after a narrowing primitive conversion, with memory  semantics of reading
             * as if the variable was declared @c volatile.
             */
            gint intValue() const override;

            /**
             * Returns the current value of this @c AtomicInteger as a @c glong,
             * with memory  semantics of reading as if the variable was declared @c volatile.
             * Equivalent to @c #get().
             */
            glong longValue() const override;

            /**
             * Returns the current value of this @c AtomicInteger as a @c gfloat
             * after a widening primitive conversion, with memory  semantics of reading
             * as if the variable was declared @c volatile.
             */
            gfloat floatValue() const override;

            /**
             * Returns the current value of this @c AtomicInteger as a @c gdouble
             * after a widening primitive conversion, with memory  semantics of reading
             * as if the variable was declared @c volatile.
             */
            gdouble doubleValue() const override;

            /**
             * Returns the current value, with memory semantics of reading as if the
             * variable was declared @c non-volatile.
             *
             * @return the value
             */
            gint getPlain() const;

            /**
             * Sets the value to @c newValue, with memory semantics
             * of setting as if the variable was declared @c non-volatile
             * and non- @c constexpr.
             *
             * @param newValue the new value
             */
            void setPlain(gint newValue);

            /**
             * Returns the current value, accessed in program order, but with no
             * assurance of memory ordering effects with respect to other threads.
             *
             * @return the value
             */
            gint getOpaque() const;

            /**
             * Sets the value to @c newValue, in program order,
             * but with no assurance of memory ordering effects with respect to other
             * threads.
             *
             * @param newValue the new value
             */
            void setOpaque(gint newValue);

            /**
             * Returns the current value, and ensures that subsequent loads and
             * stores are not reordered before this access.
             *
             * @return the value
             */
            gint getAcquire() const;

            /**
             * Sets the value to @c newValue,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void setRelease(gint newValue);

            /**
             * Atomically sets the value of a variable to the @c newValue with the
             * memory semantics of @c setVolatile if the variable's current value,
             * referred to as the <em>witness value</em>, @c == the
             * @c expectedValue, as accessed with the memory semantics of
             * @c getVolatile.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the <em>witness value</em>, which will be the same as the
             * expected value if successful
             */
            gint compareAndExchange(gint expectedValue, gint newValue);

            /**
             * Atomically sets the value of a variable to the @c newValue with the
             * memory semantics of @c set if the variable's current value,
             * referred to as the <em>witness value</em>, @c == the
             * @c expectedValue, as accessed with the memory semantics of
             * @c getAcquire.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the <em>witness value</em>, which will be the same as the
             * expected value if successful
             */
            gint compareAndExchangeAcquire(gint expectedValue, gint newValue);

            /**
             * Atomically sets the value of a variable to the @c newValue with the
             * memory semantics of @c setRelease if the variable's current value,
             * referred to as the <em>witness value</em>, @c == the
             * @c expectedValue, as accessed with the memory semantics of @c get.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return the <em>witness value</em>, which will be the same as the
             * expected value if successful
             */
            gint compareAndExchangeRelease(gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the value of a variable to the @c newValue
             * with the memory semantics of @c setVolatile if the variable's
             * current value, referred to as the <em>witness value</em>, @c == the
             * @c expectedValue, as accessed with the memory semantics of
             * @c getVolatile.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return @c true if successful
             */
            gbool weakCompareAndSetVolatile(gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the value of a variable to the @c newValue
             * with the semantics of @c set if the variable's current value,
             * referred to as the <em>witness value</em>, @c == the
             * @c expectedValue, as accessed with the memory semantics of
             * @c getAcquire.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return @c true if successful
             */
            gbool weakCompareAndSetAcquire(gint expectedValue, gint newValue);

            /**
             * Possibly atomically sets the value of a variable to the @c newValue
             * with the semantics of @c setRelease if the variable's current
             * value, referred to as the <em>witness value</em>, @c == the
             * @c expectedValue, as accessed with the memory semantics of
             * @c get.
             *
             * @param expectedValue the expected value
             * @param newValue the new value
             * @return @c true if successful
             */
            gbool weakCompareAndSetRelease(gint expectedValue, gint newValue);

            gbool equals(const Object &o) const override;

            gint hash() const override;

            Object &clone() const override;
        };
    } // atomic
} // core

#endif //CORE24_ATOMICINTEGER_H
