//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_ATOMICLONG_H
#define CORE24_ATOMICLONG_H

#include <core/Long.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace atomic {
        /**
         * A @c glong value that may be updated atomically.
         * An @c AtomicLong is used in applications such as atomically incremented
         * sequence numbers, and cannot be used as a replacement for a @c Long.
         * However, this class does extend @c Number to allow uniform access by tools and
         * utilities that deal with numerically-based classes.
         */
        class AtomicLong final : public virtual Number {
            CORE_ADD_GLOBAL_FRIENDS();

            volatile glong value = 0;

        public:
            /**
             * Creates a new AtomicLong with the given initial value.
             *
             * @param initialValue the initial value
             */
            AtomicLong(glong initialValue);

            /**
             * Creates a new AtomicLong with initial value @c 0.
             */
            AtomicLong() CORE_NOTHROW;

            /**
             * Returns the current value,
             * with memory semantics of reading as if the variable was declared @c volatile.
             *
             * @return the current value
             */
            glong get() const;

            /**
             * Sets the value to @c newValue,
             * with memory semantics of reading as if the variable was declared @c volatile.
             *
             * @param newValue the new value
             */
            void set(glong newValue);

            /**
             * Sets the value to @c newValue,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void lazySet(glong newValue);

            /**
             * Atomically sets the value to @c newValue and returns the old value,
             * with the memory semantics of @c setVolatile method and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile method
             *
             * @param newValue the new value
             * @return the previous value
             */
            glong getAndSet(glong newValue);

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
            gbool compareAndSet(glong expectedValue, glong newValue);

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
            gbool weakCompareAndSet(glong expectedValue, glong newValue);

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
            gbool weakCompareAndSetPlain(glong expectedValue, glong newValue);

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
            glong getAndIncrement();

            /**
             * Atomically decrements the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             * <p>
             *  Equivalent to @c getAndAdd(-1).
             * </p>
             * @return the previous value
             */
            glong getAndDecrement();

            /**
             * Atomically adds the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             *
             * @param delta the value to add.
             * @return the previous value
             */
            glong getAndAdd(glong delta);

            /**
             * Atomically increments the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             * <p>
             * Equivalent to @c addAndGet(1).
             * </p>
             * @return the updated value
             */
            glong incrementAndGet();

            /**
             * Atomically decrements the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             * <p>
             *  Equivalent to @c addAndGet(-1).
             * </p>
             * @return the updated value
             */
            glong decrementAndGet();

            /**
             * Atomically adds the current value,
             * with the memory semantics of @c setVolatile, and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile.
             *
             * @param delta the value to add
             * @return the updated value
             */
            glong addAndGet(glong delta);

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
            glong getAndUpdate(function::LongUnaryOperator const &updateFunction);

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
            glong updateAndGet(function::LongUnaryOperator const &updateFunction);

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
            glong getAndAccumulate(glong x, function::LongBinaryOperator const &accumulatorFunction);

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
            glong accumulateAndGet(glong x, function::LongBinaryOperator const &accumulatorFunction);

            /**
             * Returns the String representation of the current value.
             * @return the String representation of the current value
             */
            String toString() const override;

            /**
             * Returns the current value of this @c AtomicLong as a @c gint
             * after a narrowing primitive conversion, with memory  semantics of reading
             * as if the variable was declared @c volatile.
             */
            gint intValue() const override;

            /**
             * Returns the current value of this @c AtomicLong as a @c glong,
             * with memory  semantics of reading as if the variable was declared @c volatile.
             * Equivalent to @c #get().
             */
            glong longValue() const override;

            /**
             * Returns the current value of this @c AtomicLong as a @c gfloat
             * after a widening primitive conversion, with memory  semantics of reading
             * as if the variable was declared @c volatile.
             */
            gfloat floatValue() const override;

            /**
             * Returns the current value of this @c AtomicLong as a @c gdouble
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
            glong getPlain() const;

            /**
             * Sets the value to @c newValue, with memory semantics
             * of setting as if the variable was declared @c non-volatile
             * and non- @c constexpr.
             *
             * @param newValue the new value
             */
            void setPlain(glong newValue);

            /**
             * Returns the current value, accessed in program order, but with no
             * assurance of memory ordering effects with respect to other threads.
             *
             * @return the value
             */
            glong getOpaque() const;

            /**
             * Sets the value to @c newValue, in program order,
             * but with no assurance of memory ordering effects with respect to other
             * threads.
             *
             * @param newValue the new value
             */
            void setOpaque(glong newValue);

            /**
             * Returns the current value, and ensures that subsequent loads and
             * stores are not reordered before this access.
             *
             * @return the value
             */
            glong getAcquire() const;

            /**
             * Sets the value to @c newValue,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void setRelease(glong newValue);

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
            glong compareAndExchange(glong expectedValue, glong newValue);

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
            glong compareAndExchangeAcquire(glong expectedValue, glong newValue);

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
            glong compareAndExchangeRelease(glong expectedValue, glong newValue);

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
            gbool weakCompareAndSetVolatile(glong expectedValue, glong newValue);

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
            gbool weakCompareAndSetAcquire(glong expectedValue, glong newValue);

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
            gbool weakCompareAndSetRelease(glong expectedValue, glong newValue);

            gbool equals(const Object &o) const override;

            gint hash() const override;

            Object & clone() const override;
        };
    } // atomic
} // core

#endif //CORE24_ATOMICLONG_H
