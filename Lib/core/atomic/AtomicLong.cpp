//
// Created by brunshweeck on 27 juin 2024.
//

#include <core/atomic/AtomicLong.h>
#include <core/function/LongBinaryOperator.h>
#include <core/function/LongUnaryOperator.h>

namespace core {
    namespace atomic {
        AtomicLong::AtomicLong(glong initialValue) {
            value = initialValue;
        }

        AtomicLong::AtomicLong() noexcept {
            CORE_IGNORE(this);
        }

        glong AtomicLong::get() const {
            return value;
        }

        void AtomicLong::set(glong newValue) {
            UNSAFE::putLongVolatile(null, (glong) &value, newValue);
        }

        void AtomicLong::lazySet(glong newValue) {
            UNSAFE::putLongRelease(null, (glong) &value, newValue);
        }

        glong AtomicLong::getAndSet(glong newValue) {
            return UNSAFE::getAndSetLong(null, (glong) &value, newValue);
        }

        gbool AtomicLong::compareAndSet(glong expectedValue, glong newValue) {
            return UNSAFE::compareAndSetLong(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSet(glong expectedValue, glong newValue) {
            return UNSAFE::weakCompareAndSetLongPlain(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetPlain(glong expectedValue, glong newValue) {
            return UNSAFE::weakCompareAndSetLongPlain(null, (glong) &value, expectedValue, newValue);
        }

        glong AtomicLong::getAndIncrement() {
            return UNSAFE::getAndAddLong(null, (glong) &value, 1L);
        }

        glong AtomicLong::getAndDecrement() {
            return UNSAFE::getAndAddLong(null, (glong) &value, -1L);
        }

        glong AtomicLong::getAndAdd(glong delta) {
            return UNSAFE::getAndAddLong(null, (glong) &value, delta);
        }

        glong AtomicLong::incrementAndGet() {
            return UNSAFE::getAndAddLong(null, (glong) &value, 1L) + 1L;
        }

        glong AtomicLong::decrementAndGet() {
            return UNSAFE::getAndAddLong(null, (glong) &value, -1L) - 1L;
        }

        glong AtomicLong::addAndGet(glong delta) {
            return UNSAFE::getAndAddLong(null, (glong) &value, delta) + delta;
        }

        glong AtomicLong::getAndUpdate(function::LongUnaryOperator const &updateFunction) {
            glong prev = get(), next = 0L;
            for (gbool haveNext = false;;) {
                if (!haveNext)
                    next = updateFunction.apply(prev);
                if (weakCompareAndSetVolatile(prev, next))
                    return prev;
                haveNext = (prev == (prev = get()));
            }
        }

        glong AtomicLong::updateAndGet(function::LongUnaryOperator const &updateFunction) {
            glong prev = get(), next = 0L;
            for (gbool haveNext = false;;) {
                if (!haveNext)
                    next = updateFunction.apply(prev);
                if (weakCompareAndSetVolatile(prev, next))
                    return next;
                haveNext = (prev == (prev = get()));
            }
        }

        glong AtomicLong::getAndAccumulate(glong x, function::LongBinaryOperator const &accumulatorFunction) {
            glong prev = get(), next = 0L;
            for (gbool haveNext = false;;) {
                if (!haveNext)
                    next = accumulatorFunction.apply(prev, x);
                if (weakCompareAndSetVolatile(prev, next))
                    return prev;
                haveNext = (prev == (prev = get()));
            }
        }

        glong AtomicLong::accumulateAndGet(glong x, function::LongBinaryOperator const &accumulatorFunction) {
            glong prev = get(), next = 0L;
            for (gbool haveNext = false;;) {
                if (!haveNext)
                    next = accumulatorFunction.apply(prev, x);
                if (weakCompareAndSetVolatile(prev, next))
                    return next;
                haveNext = (prev == (prev = get()));
            }
        }

        String AtomicLong::toString() const {
            return Long::toString(get());
        }

        gint AtomicLong::intValue() const {
            return CORE_CAST(gint, get());
        }

        glong AtomicLong::longValue() const {
            return get();
        }

        gfloat AtomicLong::floatValue() const {
            return CORE_CAST(gfloat, get());
        }

        gdouble AtomicLong::doubleValue() const {
            return CORE_CAST(gdouble, get());
        }

        glong AtomicLong::getPlain() const {
            return UNSAFE::getLong(null, (glong) &value);
        }

        void AtomicLong::setPlain(glong newValue) {
            UNSAFE::putLong(null, (glong) &value, newValue);
        }

        glong AtomicLong::getOpaque() const {
            return UNSAFE::getLongOpaque(null, (glong) &value);
        }

        void AtomicLong::setOpaque(glong newValue) {
            UNSAFE::putLongOpaque(null, (glong) &value, newValue);
        }

        glong AtomicLong::getAcquire() const {
            return UNSAFE::getLongAcquire(null, (glong) &value);
        }

        void AtomicLong::setRelease(glong newValue) {
            UNSAFE::putLongRelease(null, (glong) &value, newValue);
        }

        glong AtomicLong::compareAndExchange(glong expectedValue, glong newValue) {
            return UNSAFE::compareAndExchangeLong(null, (glong) &value, expectedValue, newValue);
        }

        glong AtomicLong::compareAndExchangeAcquire(glong expectedValue, glong newValue) {
            return UNSAFE::compareAndExchangeLongAcquire(null, (glong) &value, expectedValue, newValue);
        }

        glong AtomicLong::compareAndExchangeRelease(glong expectedValue, glong newValue) {
            return UNSAFE::compareAndExchangeLongRelease(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetVolatile(glong expectedValue, glong newValue) {
            return UNSAFE::weakCompareAndSetLong(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetAcquire(glong expectedValue, glong newValue) {
            return UNSAFE::weakCompareAndSetLongAcquire(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicLong::weakCompareAndSetRelease(glong expectedValue, glong newValue) {
            return UNSAFE::weakCompareAndSetLongRelease(null, (glong) &value, expectedValue, newValue);
        }

        gbool AtomicLong::equals(const Object &o) const {
            return this == &o ||
                   Class<AtomicLong>::hasInstance(o) && get() == CORE_XCAST(AtomicLong const, o).get();
        }

        gint AtomicLong::hash() const {
            return Long::hash(get());
        }

        Object &AtomicLong::clone() const {
            try {
                return UNSAFE::newInstance<AtomicLong>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }
    } // atomic
} // core
