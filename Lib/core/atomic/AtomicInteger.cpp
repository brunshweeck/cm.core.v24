//
// Created by brunshweeck on 27 juin 2024.
//

#include <core/atomic/AtomicInteger.h>
#include <core/function/IntBinaryOperator.h>
#include <core/function/IntUnaryOperator.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace atomic {
        AtomicInteger::AtomicInteger(gint initialValue) {
            value = initialValue;
        }

        AtomicInteger::AtomicInteger() noexcept {
            CORE_IGNORE(this);
        }

        gint AtomicInteger::get() const {
            return value;
        }

        void AtomicInteger::set(gint newValue) {
            UNSAFE::putIntVolatile(null, (glong) &value, newValue);
        }

        void AtomicInteger::lazySet(gint newValue) {
            UNSAFE::putIntRelease(null, (glong) &value, newValue);
        }

        gint AtomicInteger::getAndSet(gint newValue) {
            return UNSAFE::getAndSetInt(null, (glong) &value, newValue);
        }

        gbool AtomicInteger::compareAndSet(gint expectedValue, gint newValue) {
            return UNSAFE::compareAndSetInt(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSet(gint expectedValue, gint newValue) {
            return UNSAFE::weakCompareAndSetIntPlain(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetPlain(gint expectedValue, gint newValue) {
            return UNSAFE::weakCompareAndSetIntPlain(null, (glong) &value, expectedValue, newValue);
        }

        gint AtomicInteger::getAndIncrement() {
            return UNSAFE::getAndAddInt(null, (glong) &value, 1L);
        }

        gint AtomicInteger::getAndDecrement() {
            return UNSAFE::getAndAddInt(null, (glong) &value, -1L);
        }

        gint AtomicInteger::getAndAdd(gint delta) {
            return UNSAFE::getAndAddInt(null, (glong) &value, delta);
        }

        gint AtomicInteger::incrementAndGet() {
            return UNSAFE::getAndAddInt(null, (glong) &value, 1L) + 1L;
        }

        gint AtomicInteger::decrementAndGet() {
            return UNSAFE::getAndAddInt(null, (glong) &value, -1L) - 1L;
        }

        gint AtomicInteger::addAndGet(gint delta) {
            return UNSAFE::getAndAddInt(null, (glong) &value, delta) + delta;
        }

        gint AtomicInteger::getAndUpdate(function::IntUnaryOperator const &updateFunction) {
            gint prev = get(), next = 0L;
            for (gbool haveNext = false;;) {
                if (!haveNext)
                    next = updateFunction.apply(prev);
                if (weakCompareAndSetVolatile(prev, next))
                    return prev;
                haveNext = (prev == (prev = get()));
            }
        }

        gint AtomicInteger::updateAndGet(function::IntUnaryOperator const &updateFunction) {
            gint prev = get(), next = 0L;
            for (gbool haveNext = false;;) {
                if (!haveNext)
                    next = updateFunction.apply(prev);
                if (weakCompareAndSetVolatile(prev, next))
                    return next;
                haveNext = (prev == (prev = get()));
            }
        }

        gint AtomicInteger::getAndAccumulate(gint x, function::IntBinaryOperator const &accumulatorFunction) {
            gint prev = get(), next = 0L;
            for (gbool haveNext = false;;) {
                if (!haveNext)
                    next = accumulatorFunction.apply(prev, x);
                if (weakCompareAndSetVolatile(prev, next))
                    return prev;
                haveNext = (prev == (prev = get()));
            }
        }

        gint AtomicInteger::accumulateAndGet(gint x, function::IntBinaryOperator const &accumulatorFunction) {
            gint prev = get(), next = 0L;
            for (gbool haveNext = false;;) {
                if (!haveNext)
                    next = accumulatorFunction.apply(prev, x);
                if (weakCompareAndSetVolatile(prev, next))
                    return next;
                haveNext = (prev == (prev = get()));
            }
        }

        String AtomicInteger::toString() const {
            return Integer::toString(get());
        }

        gint AtomicInteger::intValue() const {
            return CORE_CAST(gint, get());
        }

        glong AtomicInteger::longValue() const {
            return get();
        }

        gfloat AtomicInteger::floatValue() const {
            return CORE_CAST(gfloat, get());
        }

        gdouble AtomicInteger::doubleValue() const {
            return CORE_CAST(gdouble, get());
        }

        gint AtomicInteger::getPlain() const {
            return UNSAFE::getInt(null, (glong) &value);
        }

        void AtomicInteger::setPlain(gint newValue) {
            UNSAFE::putInt(null, (glong) &value, newValue);
        }

        gint AtomicInteger::getOpaque() const {
            return UNSAFE::getIntOpaque(null, (glong) &value);
        }

        void AtomicInteger::setOpaque(gint newValue) {
            UNSAFE::putIntOpaque(null, (glong) &value, newValue);
        }

        gint AtomicInteger::getAcquire() const {
            return UNSAFE::getIntAcquire(null, (glong) &value);
        }

        void AtomicInteger::setRelease(gint newValue) {
            UNSAFE::putIntRelease(null, (glong) &value, newValue);
        }

        gint AtomicInteger::compareAndExchange(gint expectedValue, gint newValue) {
            return UNSAFE::compareAndExchangeInt(null, (glong) &value, expectedValue, newValue);
        }

        gint AtomicInteger::compareAndExchangeAcquire(gint expectedValue, gint newValue) {
            return UNSAFE::compareAndExchangeIntAcquire(null, (glong) &value, expectedValue, newValue);
        }

        gint AtomicInteger::compareAndExchangeRelease(gint expectedValue, gint newValue) {
            return UNSAFE::compareAndExchangeIntRelease(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetVolatile(gint expectedValue, gint newValue) {
            return UNSAFE::weakCompareAndSetInt(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetAcquire(gint expectedValue, gint newValue) {
            return UNSAFE::weakCompareAndSetIntAcquire(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicInteger::weakCompareAndSetRelease(gint expectedValue, gint newValue) {
            return UNSAFE::weakCompareAndSetIntRelease(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicInteger::equals(const Object &o) const {
            return this == &o ||
                   Class<AtomicInteger>::hasInstance(o) && get() == CORE_XCAST(AtomicInteger const, o).get();
        }

        gint AtomicInteger::hash() const {
            return Integer::hash(get());
        }

        Object &AtomicInteger::clone() const {
            try {
                return UNSAFE::newInstance<AtomicInteger>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }
    } // atomic
} // core
