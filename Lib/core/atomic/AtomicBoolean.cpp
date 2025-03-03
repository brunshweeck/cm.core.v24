//
// Created by brunshweeck on 27 juin 2024.
//

#include <core/atomic/AtomicBoolean.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace atomic {
        AtomicBoolean::AtomicBoolean(gbool initialValue) {
            value = initialValue;
        }

        AtomicBoolean::AtomicBoolean() noexcept {
            CORE_IGNORE(this);
        }

        gbool AtomicBoolean::get() const {
            return value != 0;
        }

        void AtomicBoolean::set(gbool newValue) {
            value = newValue ? 1 : 0;
        }

        void AtomicBoolean::lazySet(gbool newValue) {
            UNSAFE::putIntRelease(null, CORE_CAST(glong, &value), newValue ? 1 : 0);
        }

        gbool AtomicBoolean::getAndSet(gbool newValue) {
            return UNSAFE::getAndSetInt(null, CORE_CAST(glong, &value), newValue ? 1 : 0);
        }

        gbool AtomicBoolean::compareAndSet(gbool expectedValue, gbool newValue) {
            return UNSAFE::compareAndSetInt(null, CORE_CAST(glong, &value), expectedValue ? 1 : 0, newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSet(gbool expectedValue, gbool newValue) {
            return UNSAFE::weakCompareAndSetIntPlain(null, CORE_CAST(glong, &value),
                                                     expectedValue ? 1 : 0,
                                                     newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSetPlain(gbool expectedValue, gbool newValue) {
            return UNSAFE::weakCompareAndSetIntPlain(null, CORE_CAST(glong, &value),
                                                     expectedValue ? 1 : 0,
                                                     newValue ? 1 : 0);
        }

        String AtomicBoolean::toString() const {
            return Boolean::toString(get());
        }

        gint AtomicBoolean::getPlain() const {
            return UNSAFE::getInt(null, CORE_CAST(glong, &value));
        }

        void AtomicBoolean::setPlain(gint newValue) {
            UNSAFE::putInt(null, CORE_CAST(glong, &value), newValue);
        }

        gint AtomicBoolean::getOpaque() const {
            return UNSAFE::getIntOpaque(null, CORE_CAST(glong, &value));
        }

        void AtomicBoolean::setOpaque(gint newValue) {
            UNSAFE::putIntOpaque(null, CORE_CAST(glong, &value), newValue);
        }

        gint AtomicBoolean::getAcquire() const {
            return UNSAFE::getIntAcquire(null, CORE_CAST(glong, &value));
        }

        void AtomicBoolean::setRelease(gint newValue) {
            UNSAFE::putIntRelease(null, CORE_CAST(glong, &value), newValue);
        }

        gbool AtomicBoolean::compareAndExchange(gbool expectedValue, gbool newValue) {
            return UNSAFE::compareAndExchangeInt(null, CORE_CAST(glong, &value),
                                                 expectedValue ? 1 : 0,
                                                 newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::compareAndExchangeAcquire(gbool expectedValue, gbool newValue) {
            return UNSAFE::compareAndExchangeIntAcquire(null, CORE_CAST(glong, &value),
                                                        expectedValue ? 1 : 0,
                                                        newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::compareAndExchangeRelease(gbool expectedValue, gbool newValue) {
            return UNSAFE::compareAndExchangeIntRelease(null, CORE_CAST(glong, &value),
                                                        expectedValue ? 1 : 0,
                                                        newValue ? 1 : 0) != 0;
        }

        gbool AtomicBoolean::weakCompareAndSetVolatile(gbool expectedValue, gbool newValue) {
            return UNSAFE::weakCompareAndSetInt(null, CORE_CAST(glong, &value),
                                                expectedValue ? 1 : 0,
                                                newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSetAcquire(gbool expectedValue, gbool newValue) {
            return UNSAFE::weakCompareAndSetIntAcquire(null, CORE_CAST(glong, &value),
                                                       expectedValue ? 1 : 0,
                                                       newValue ? 1 : 0);
        }

        gbool AtomicBoolean::weakCompareAndSetRelease(gbool expectedValue, gbool newValue) {
            return UNSAFE::weakCompareAndSetIntRelease(null, CORE_CAST(glong, &value),
                                                       expectedValue ? 1 : 0,
                                                       newValue ? 1 : 0);
        }

        gbool AtomicBoolean::equals(const Object &o) const {
            return this == &o ||
                   Class<AtomicBoolean>::hasInstance(o) && get() == CORE_XCAST(AtomicBoolean const, o).get();
        }

        gint AtomicBoolean::hash() const {
            return Boolean::hash(get());
        }

        Object &AtomicBoolean::clone() const {
            try {
                return UNSAFE::newInstance<AtomicBoolean>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }
    } // atomic
} // core
