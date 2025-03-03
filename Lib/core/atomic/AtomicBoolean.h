//
// Created by brunshweeck on 29 juin 2024.
//

#ifndef CORE24_ATOMICBOOLEAN_H
#define CORE24_ATOMICBOOLEAN_H

#include <core/lang/Boolean.h>

namespace core {
    namespace atomic {
        /**
         * A @c boolean value that may be updated atomically.
         * An @c AtomicBoolean is used in applications such as atomically
         * updated flags, and cannot be used as a replacement for a @c Boolean.
         */
        class AtomicBoolean final : public virtual Object {
            CORE_ADD_GLOBAL_FRIENDS();

            volatile gint value = 0;

        public:
            /**
             * Creates a new AtomicBoolean with the given initial value.
             *
             * @param initialValue the initial value
             */
            AtomicBoolean(gbool initialValue);

            /**
             * Creates a new AtomicBoolean with initial value @c 0.
             */
            AtomicBoolean() CORE_NOTHROW;

            /**
             * Returns the current value,
             * with memory semantics of reading as if the variable was declared @c volatile.
             *
             * @return the current value
             */
            gbool get() const;

            /**
             * Sets the value to @c newValue,
             * with memory semantics of reading as if the variable was declared @c volatile.
             *
             * @param newValue the new value
             */
            void set(gbool newValue);

            /**
             * Sets the value to @c newValue,
             * and ensures that prior loads and stores are not reordered after this access.
             *
             * @param newValue the new value
             */
            void lazySet(gbool newValue);

            /**
             * Atomically sets the value to @c newValue and returns the old value,
             * with the memory semantics of @c setVolatile method and returns the variable's
             * previous value, as accessed with the memory semantics of @c getVolatile method
             *
             * @param newValue the new value
             * @return the previous value
             */
            gbool getAndSet(gbool newValue);

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
            gbool compareAndSet(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSet(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSetPlain(gbool expectedValue, gbool newValue);

            /**
             * Returns the String representation of the current value.
             * @return the String representation of the current value
             */
            String toString() const override;

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
            gbool compareAndExchange(gbool expectedValue, gbool newValue);

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
            gbool compareAndExchangeAcquire(gbool expectedValue, gbool newValue);

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
            gbool compareAndExchangeRelease(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSetVolatile(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSetAcquire(gbool expectedValue, gbool newValue);

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
            gbool weakCompareAndSetRelease(gbool expectedValue, gbool newValue);

            gbool equals(const Object &o) const override;

            gint hash() const override;

            Object & clone() const override;
        };
    } // atomic
} // core

#endif //CORE24_ATOMICBOOLEAN_H
