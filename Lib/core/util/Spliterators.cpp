//
// Created by brunshweeck on 16/11/24.
//

#include "Spliterators.h"

#include <core/DoubleArray.h>
#include <core/LongArray.h>
#include <core/util/Arrays.h>
#include <core/util/Comparator.h>
#include <core/util/Iterator.h>

namespace core {
    namespace util {
        IntSpliterator &Spliterators::emptyIntSpliterator() {
            class Spliterator final : public IntSpliterator {
            public:
                glong estimateSize() const override { return 0; }

                gint characteristics() const override { return SIZED; }

                const Comparator<Integer> & comparator() const override {
                    IllegalStateException().throws($ftrace());
                }

                IntSpliterator & trySplit() override { return *this; }

                const IntSpliterator & trySplit() const override { return *this; }

                gbool tryAdvance(const function::IntConsumer &action) override { return false; }

                void forEach(const function::IntConsumer &action) override {
                }

                glong getExactSizeIfKnown() const override { return 0; }

                gbool tryAdvance(const function::Consumer<Integer &> &action) override { return false; }

                void forEach(const function::Consumer<Integer &> &action) override {
                }

                gbool equals(const Object &o) const override {
                    return this == &o || Class<Spliterator>::hasInstance(o);
                }
            };

            try { return UNSAFE::newInstance<Spliterator>(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        LongSpliterator &Spliterators::emptyLongSpliterator() {
            class Spliterator final : public LongSpliterator {
            public:
                glong estimateSize() const override { return 0; }

                gint characteristics() const override { return SIZED; }

                const Comparator<Long> & comparator() const override {
                    IllegalStateException().throws($ftrace());
                }

                LongSpliterator & trySplit() override { return *this; }

                const LongSpliterator & trySplit() const override { return *this; }

                gbool tryAdvance(const function::LongConsumer &action) override { return false; }

                void forEach(const function::LongConsumer &action) override {
                }

                glong getExactSizeIfKnown() const override { return 0; }

                gbool tryAdvance(const function::Consumer<Long &> &action) override { return false; }

                void forEach(const function::Consumer<Long &> &action) override {
                }

                gbool equals(const Object &o) const override {
                    return this == &o || Class<Spliterator>::hasInstance(o);
                }
            };

            try { return UNSAFE::newInstance<Spliterator>(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        DoubleSpliterator &Spliterators::emptyDoubleSpliterator() {
            class Spliterator final : public DoubleSpliterator {
            public:
                glong estimateSize() const override { return 0; }

                gint characteristics() const override { return SIZED; }

                const Comparator<Double> & comparator() const override {
                    IllegalStateException().throws($ftrace());
                }

                DoubleSpliterator & trySplit() override { return *this; }

                const DoubleSpliterator & trySplit() const override { return *this; }

                gbool tryAdvance(const function::DoubleConsumer &action) override { return false; }

                void forEach(const function::DoubleConsumer &action) override {
                }

                glong getExactSizeIfKnown() const override { return 0; }

                gbool tryAdvance(const function::Consumer<Double &> &action) override { return false; }

                void forEach(const function::Consumer<Double &> &action) override {
                }

                gbool equals(const Object &o) const override {
                    return this == &o || Class<Spliterator>::hasInstance(o);
                }
            };

            try { return UNSAFE::newInstance<Spliterator>(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        IntSpliterator &Spliterators::spliterator(IntArray array, gint additionalCharacteristics) {
            try {
                gint len = array.length();
                return spliterator(UNSAFE::moveInstance(array), 0, len, additionalCharacteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        IntSpliterator &Spliterators::spliterator(IntArray array, gint fromIndex, gint toIndex,
                                                  gint additionalCharacteristics) {
            /**
             * A IntSpliterator designed for use by sources that traverse and split
             * elements maintained in an unmodifiable IntArray array.
             */
            class Spliterator final : public IntSpliterator {
                IntArray array;
                gint mutable offset;
                gint limit;
                gint additionalCharacteristics;

            public:
                /**
                 * Creates a spliterator covering the given array and range
                 * @param a the array, assumed to be unmodified during use
                 * @param fromIndex the least index (inclusive) to cover
                 * @param toIndex one past the greatest index to cover
                 * @param additionalCharacteristics Additional spliterator characteristics
                 *        of this spliterator's source or elements beyond @c SIZED and
                 *        @c SUBSIZED which are always reported
                 */
                CORE_EXPLICIT Spliterator(IntArray a, gint fromIndex, gint toIndex, gint characteristics) {
                    array = UNSAFE::moveInstance(a);
                    offset = fromIndex;
                    limit = toIndex;
                    additionalCharacteristics = characteristics | SIZED | SUBSIZED;
                }

                glong estimateSize() const override { return limit - offset; }

                gint characteristics() const override { return additionalCharacteristics; }

                const Comparator<Integer> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Integer>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                IntSpliterator & trySplit() override {
                    gint mid = (offset + limit) >> 1;
                    if (offset < mid) {
                        IntArray subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator(subArray, 0, mid, additionalCharacteristics);
                    }
                    return *this;
                }

                const IntSpliterator & trySplit() const override {
                    gint mid = (offset + limit) >> 1;
                    if (offset < mid) {
                        IntArray subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator(subArray, 0, mid, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::IntConsumer &action) override {
                    if (offset < 0 || offset >= limit)
                        return false;
                    action.accept(array[offset++]);
                    return true;
                }

                void forEach(const function::IntConsumer &action) override {
                    if (offset < 0)
                        return;
                    while (offset < limit)
                        action.accept(array[offset++]);
                }

                glong getExactSizeIfKnown() const override { return estimateSize(); }

                gbool tryAdvance(const function::Consumer<Integer &> &action) override {
                    if (offset < 0 || offset >= limit)
                        return false;
                    Integer i = array[offset++];
                    action.accept(i);
                    return true;
                }

                void forEach(const function::Consumer<Integer &> &action) override {
                    if (offset < 0)
                        return;
                    while (offset < limit) {
                        Integer i = array[offset++];
                        action.accept(i);
                    }
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Spliterator>::hasInstance(o))
                        return false;
                    Spliterator const &spliterator = CORE_XCAST(Spliterator const, o);
                    if (offset != spliterator.offset)
                        return false;
                    if (limit != spliterator.limit)
                        return false;
                    if (additionalCharacteristics != spliterator.additionalCharacteristics)
                        return false;
                    return array == spliterator.array;
                }
            };

            try {
                misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, array.length());
                return UNSAFE::newInstance<Spliterator>(
                    UNSAFE::moveInstance(array),
                    fromIndex,
                    toIndex,
                    additionalCharacteristics
                );
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        LongSpliterator &Spliterators::spliterator(LongArray array, gint additionalCharacteristics) {
            try {
                gint len = array.length();
                return spliterator(UNSAFE::moveInstance(array), 0, len, additionalCharacteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        LongSpliterator &Spliterators::spliterator(LongArray array, gint fromIndex, gint toIndex,
                                                   gint additionalCharacteristics) {
            /**
             * A LongSpliterator designed for use by sources that traverse and split
             * elements maintained in an unmodifiable LongArray array.
             */
            class Spliterator final : public LongSpliterator {
                LongArray array;
                gint mutable offset;
                gint limit;
                gint additionalCharacteristics;

            public:
                /**
                 * Creates a spliterator covering the given array and range
                 * @param a the array, assumed to be unmodified during use
                 * @param fromIndex the least index (inclusive) to cover
                 * @param toIndex one past the greatest index to cover
                 * @param additionalCharacteristics Additional spliterator characteristics
                 *        of this spliterator's source or elements beyond @c SIZED and
                 *        @c SUBSIZED which are always reported
                 */
                CORE_EXPLICIT Spliterator(LongArray a, gint fromIndex, gint toIndex, gint characteristics) {
                    array = UNSAFE::moveInstance(a);
                    offset = fromIndex;
                    limit = toIndex - fromIndex;
                    additionalCharacteristics = characteristics | SIZED | SUBSIZED;
                }

                glong estimateSize() const override { return limit - offset; }

                gint characteristics() const override { return additionalCharacteristics; }

                const Comparator<Long> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Long>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                LongSpliterator & trySplit() override {
                    gint mid = (offset + limit) >> 1;
                    if (offset < mid) {
                        LongArray subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator(subArray, 0, mid, additionalCharacteristics);
                    }
                    return *this;
                }

                const LongSpliterator & trySplit() const override {
                    gint mid = (offset + limit) >> 1;
                    if (offset < mid) {
                        LongArray subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator(subArray, 0, mid, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::LongConsumer &action) override {
                    if (offset < 0 || offset >= limit)
                        return false;
                    action.accept(array[offset++]);
                    return true;
                }

                glong getExactSizeIfKnown() const override { return estimateSize(); }

                void forEach(const function::LongConsumer &action) override {
                    if (offset < 0)
                        return;
                    while (offset < limit)
                        action.accept(array[offset++]);
                }

                gbool tryAdvance(const function::Consumer<Long &> &action) override {
                    if (offset < 0 || offset >= limit)
                        return false;
                    Long i = array[offset++];
                    action.accept(i);
                    return true;
                }

                void forEach(const function::Consumer<Long &> &action) override {
                    if (offset < 0)
                        return;
                    while (offset < limit) {
                        Long i = array[offset++];
                        action.accept(i);
                    }
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Spliterator>::hasInstance(o))
                        return false;
                    Spliterator const &spliterator = CORE_XCAST(Spliterator const, o);
                    if (offset != spliterator.offset)
                        return false;
                    if (limit != spliterator.limit)
                        return false;
                    if (additionalCharacteristics != spliterator.additionalCharacteristics)
                        return false;
                    return array == spliterator.array;
                }
            };

            try {
                misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, array.length());
                return UNSAFE::newInstance<Spliterator>(
                    UNSAFE::moveInstance(array),
                    fromIndex,
                    toIndex,
                    additionalCharacteristics
                );
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        DoubleSpliterator &Spliterators::spliterator(DoubleArray array, gint additionalCharacteristics) {
            try {
                gint len = array.length();
                return spliterator(UNSAFE::moveInstance(array), 0, len, additionalCharacteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        DoubleSpliterator &Spliterators::spliterator(DoubleArray array, gint fromIndex, gint toIndex,
                                                     gint additionalCharacteristics) {
            /**
             * A DoubleSpliterator designed for use by sources that traverse and split
             * elements maintained in an unmodifiable DoubleArray array.
             */
            class Spliterator final : public DoubleSpliterator {
                DoubleArray array;
                gint mutable offset;
                gint limit;
                gint additionalCharacteristics;

            public:
                /**
                 * Creates a spliterator covering the given array and range
                 * @param a the array, assumed to be unmodified during use
                 * @param fromIndex the least index (inclusive) to cover
                 * @param toIndex one past the greatest index to cover
                 * @param additionalCharacteristics Additional spliterator characteristics
                 *        of this spliterator's source or elements beyond @c SIZED and
                 *        @c SUBSIZED which are always reported
                 */
                CORE_EXPLICIT Spliterator(DoubleArray a, gint fromIndex, gint toIndex, gint characteristics) {
                    array = UNSAFE::moveInstance(a);
                    offset = fromIndex;
                    limit = toIndex - fromIndex;
                    additionalCharacteristics = characteristics | SIZED | SUBSIZED;
                }

                glong estimateSize() const override { return limit - offset; }

                gint characteristics() const override { return additionalCharacteristics; }

                const Comparator<Double> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Double>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                DoubleSpliterator & trySplit() override {
                    gint mid = (offset + limit) >> 1;
                    if (offset < mid) {
                        DoubleArray subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator(subArray, 0, mid, additionalCharacteristics);
                    }
                    return *this;
                }

                const DoubleSpliterator & trySplit() const override {
                    gint mid = (offset + limit) >> 1;
                    if (offset < mid) {
                        DoubleArray subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator(subArray, 0, mid, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::DoubleConsumer &action) override {
                    if (offset < 0 || offset >= limit)
                        return false;
                    action.accept(array[offset++]);
                    return true;
                }

                glong getExactSizeIfKnown() const override { return estimateSize(); }

                void forEach(const function::DoubleConsumer &action) override {
                    if (offset < 0)
                        return;
                    while (offset < limit)
                        action.accept(array[offset++]);
                }

                gbool tryAdvance(const function::Consumer<Double &> &action) override {
                    if (offset < 0 || offset >= limit)
                        return false;
                    Double i = array[offset++];
                    action.accept(i);
                    return true;
                }

                void forEach(const function::Consumer<Double &> &action) override {
                    if (offset < 0)
                        return;
                    while (offset < limit) {
                        Double i = array[offset++];
                        action.accept(i);
                    }
                }

                gbool equals(const Object &o) const override {
                    if (this == &o)
                        return true;
                    if (!Class<Spliterator>::hasInstance(o))
                        return false;
                    Spliterator const &spliterator = CORE_XCAST(Spliterator const, o);
                    if (offset != spliterator.offset)
                        return false;
                    if (limit != spliterator.limit)
                        return false;
                    if (additionalCharacteristics != spliterator.additionalCharacteristics)
                        return false;
                    return array == spliterator.array;
                }
            };

            try {
                misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, array.length());
                return UNSAFE::newInstance<Spliterator>(
                    UNSAFE::moveInstance(array),
                    fromIndex,
                    toIndex,
                    additionalCharacteristics
                );
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        IntSpliterator &Spliterators::spliterator(IntIterator &iterator, glong size, gint characteristics) {
            static CORE_FAST gint BATCH_UNIT = 1 << 10; // batch array size increment
            static CORE_FAST gint MAX_BATCH = 1 << 25; // max batch array size;
            /**
             * A IntSpliterator using a given IntIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Spliterator final : public IntSpliterator {
                IntIterator &it;
                gint additionalCharacteristics;
                glong mutable estSize; // size estimate
                gint mutable batch = 0; // batch size for splits
            public:
                /**
                 * Creates a spliterator using the given iterator
                 * for traversal, and reporting the given initial size
                 * and characteristics.
                 *
                 * @param iterator the iterator for the source
                 * @param size the number of elements in the source
                 * @param characteristics properties of this spliterator's
                 * source or elements.
                 */
                CORE_EXPLICIT Spliterator(IntIterator &iterator, glong size, gint characteristics): it(iterator) {
                    additionalCharacteristics = (characteristics & CONCURRENT) == 0
                                                    ? characteristics | SIZED | SUBSIZED
                                                    : characteristics;
                    estSize = size;
                }

                glong estimateSize() const override {
                    return estSize;
                }

                gint characteristics() const override {
                    return additionalCharacteristics;
                }

                const Comparator<Integer> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Integer>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                IntSpliterator & trySplit() override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        IntArray a = IntArray(n);
                        gint j = 0;
                        do { a[j] = it.nextInt(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                const IntSpliterator & trySplit() const override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        IntArray a = IntArray(n);
                        gint j = 0;
                        do { a[j] = it.nextInt(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::IntConsumer &action) override {
                    if (it.hasNext()) {
                        action.accept(it.nextInt());
                        return true;
                    }
                    return false;
                }

                void forEach(const function::IntConsumer &action) override {
                    it.forEach(action);
                }

                gbool tryAdvance(const function::Consumer<Integer &> &action) override {
                    if (it.hasNext()) {
                        Integer i = it.nextInt();
                        action.accept(i);
                        return true;
                    }
                    return false;
                }

                void forEach(const function::Consumer<Integer &> &action) override {
                    if (it.hasNext()) {
                        Integer i = it.nextInt();
                        action.accept(i);
                    }
                }
            };

            try {
                return UNSAFE::newInstance<Spliterator>(iterator, size, characteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        IntSpliterator &Spliterators::spliterator(IntIterator &iterator, gint characteristics) {
            static CORE_FAST gint BATCH_UNIT = 1 << 10; // batch array size increment
            static CORE_FAST gint MAX_BATCH = 1 << 25; // max batch array size;
            /**
             * A IntSpliterator using a given IntIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Spliterator final : public IntSpliterator {
                IntIterator &it;
                gint additionalCharacteristics;
                glong mutable estSize; // size estimate
                gint mutable batch = 0; // batch size for splits
            public:
                /**
                 * Creates a spliterator using the given iterator for a
                 * source of unknown size, reporting the given
                 * characteristics.
                 *
                 * @param iterator the iterator for the source
                 * @param characteristics properties of this spliterator's
                 * source or elements.
                 */
                CORE_EXPLICIT Spliterator(IntIterator &iterator, gint characteristics): it(iterator) {
                    additionalCharacteristics = characteristics & ~(SIZED | SUBSIZED);
                    estSize = Long::MAX_VALUE;
                }

                glong estimateSize() const override {
                    return estSize;
                }

                gint characteristics() const override {
                    return additionalCharacteristics;
                }

                const Comparator<Integer> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Integer>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                IntSpliterator & trySplit() override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        IntArray a = IntArray(n);
                        gint j = 0;
                        do { a[j] = it.nextInt(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                const IntSpliterator & trySplit() const override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        IntArray a = IntArray(n);
                        gint j = 0;
                        do { a[j] = it.nextInt(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::IntConsumer &action) override {
                    if (it.hasNext()) {
                        action.accept(it.nextInt());
                        return true;
                    }
                    return false;
                }

                void forEach(const function::IntConsumer &action) override {
                    it.forEach(action);
                }

                gbool tryAdvance(const function::Consumer<Integer &> &action) override {
                    if (it.hasNext()) {
                        Integer i = it.nextInt();
                        action.accept(i);
                        return true;
                    }
                    return false;
                }

                void forEach(const function::Consumer<Integer &> &action) override {
                    if (it.hasNext()) {
                        Integer i = it.nextInt();
                        action.accept(i);
                    }
                }
            };

            try {
                return UNSAFE::newInstance<Spliterator>(iterator, characteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        LongSpliterator &Spliterators::spliterator(LongIterator &iterator, glong size, gint characteristics) {
            static CORE_FAST gint BATCH_UNIT = 1 << 10; // batch array size increment
            static CORE_FAST gint MAX_BATCH = 1 << 25; // max batch array size;
            /**
             * A LongSpliterator using a given LongIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Spliterator final : public LongSpliterator {
                LongIterator &it;
                gint additionalCharacteristics;
                glong mutable estSize; // size estimate
                gint mutable batch = 0; // batch size for splits
            public:
                /**
                 * Creates a spliterator using the given iterator
                 * for traversal, and reporting the given initial size
                 * and characteristics.
                 *
                 * @param iterator the iterator for the source
                 * @param size the number of elements in the source
                 * @param characteristics properties of this spliterator's
                 * source or elements.
                 */
                CORE_EXPLICIT Spliterator(LongIterator &iterator, glong size, gint characteristics): it(iterator) {
                    additionalCharacteristics = (characteristics & CONCURRENT) == 0
                                                    ? characteristics | SIZED | SUBSIZED
                                                    : characteristics;
                    estSize = size;
                }

                glong estimateSize() const override {
                    return estSize;
                }

                gint characteristics() const override {
                    return additionalCharacteristics;
                }

                const Comparator<Long> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Long>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                LongSpliterator & trySplit() override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        LongArray a = LongArray(n);
                        gint j = 0;
                        do { a[j] = it.nextLong(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                const LongSpliterator & trySplit() const override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        LongArray a = LongArray(n);
                        gint j = 0;
                        do { a[j] = it.nextLong(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::LongConsumer &action) override {
                    if (it.hasNext()) {
                        action.accept(it.nextLong());
                        return true;
                    }
                    return false;
                }

                void forEach(const function::LongConsumer &action) override {
                    it.forEach(action);
                }

                gbool tryAdvance(const function::Consumer<Long &> &action) override {
                    if (it.hasNext()) {
                        Long i = it.nextLong();
                        action.accept(i);
                        return true;
                    }
                    return false;
                }

                void forEach(const function::Consumer<Long &> &action) override {
                    if (it.hasNext()) {
                        Long i = it.nextLong();
                        action.accept(i);
                    }
                }
            };

            try {
                return UNSAFE::newInstance<Spliterator>(iterator, size, characteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        LongSpliterator &Spliterators::spliterator(LongIterator &iterator, gint characteristics) {
            static CORE_FAST gint BATCH_UNIT = 1 << 10; // batch array size increment
            static CORE_FAST gint MAX_BATCH = 1 << 25; // max batch array size;
            /**
             * A LongSpliterator using a given LongIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Spliterator final : public LongSpliterator {
                LongIterator &it;
                gint additionalCharacteristics;
                glong mutable estSize; // size estimate
                gint mutable batch = 0; // batch size for splits
            public:
                /**
                 * Creates a spliterator using the given iterator for a
                 * source of unknown size, reporting the given
                 * characteristics.
                 *
                 * @param iterator the iterator for the source
                 * @param characteristics properties of this spliterator's
                 * source or elements.
                 */
                CORE_EXPLICIT Spliterator(LongIterator &iterator, gint characteristics): it(iterator) {
                    additionalCharacteristics = characteristics & ~(SIZED | SUBSIZED);
                    estSize = Long::MAX_VALUE;
                }

                glong estimateSize() const override {
                    return estSize;
                }

                gint characteristics() const override {
                    return additionalCharacteristics;
                }

                const Comparator<Long> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Long>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                LongSpliterator & trySplit() override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        LongArray a = LongArray(n);
                        gint j = 0;
                        do { a[j] = it.nextLong(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                const LongSpliterator & trySplit() const override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        LongArray a = LongArray(n);
                        gint j = 0;
                        do { a[j] = it.nextLong(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::LongConsumer &action) override {
                    if (it.hasNext()) {
                        action.accept(it.nextLong());
                        return true;
                    }
                    return false;
                }

                void forEach(const function::LongConsumer &action) override {
                    it.forEach(action);
                }

                gbool tryAdvance(const function::Consumer<Long &> &action) override {
                    if (it.hasNext()) {
                        Long i = it.nextLong();
                        action.accept(i);
                        return true;
                    }
                    return false;
                }

                void forEach(const function::Consumer<Long &> &action) override {
                    if (it.hasNext()) {
                        Long i = it.nextLong();
                        action.accept(i);
                    }
                }
            };

            try {
                return UNSAFE::newInstance<Spliterator>(iterator, characteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        DoubleSpliterator &Spliterators::spliterator(DoubleIterator &iterator, glong size, gint characteristics) {
            static CORE_FAST gint BATCH_UNIT = 1 << 10; // batch array size increment
            static CORE_FAST gint MAX_BATCH = 1 << 25; // max batch array size;
            /**
             * A DoubleSpliterator using a given DoubleIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Spliterator final : public DoubleSpliterator {
                DoubleIterator &it;
                gint additionalCharacteristics;
                glong mutable estSize; // size estimate
                gint mutable batch = 0; // batch size for splits
            public:
                /**
                 * Creates a spliterator using the given iterator
                 * for traversal, and reporting the given initial size
                 * and characteristics.
                 *
                 * @param iterator the iterator for the source
                 * @param size the number of elements in the source
                 * @param characteristics properties of this spliterator's
                 * source or elements.
                 */
                CORE_EXPLICIT Spliterator(DoubleIterator &iterator, glong size, gint characteristics): it(iterator) {
                    additionalCharacteristics = (characteristics & CONCURRENT) == 0
                                                    ? characteristics | SIZED | SUBSIZED
                                                    : characteristics;
                    estSize = size;
                }

                glong estimateSize() const override {
                    return estSize;
                }

                gint characteristics() const override {
                    return additionalCharacteristics;
                }

                const Comparator<Double> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Double>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                DoubleSpliterator & trySplit() override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        DoubleArray a = DoubleArray(n);
                        gint j = 0;
                        do { a[j] = it.nextDouble(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                const DoubleSpliterator & trySplit() const override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        DoubleArray a = DoubleArray(n);
                        gint j = 0;
                        do { a[j] = it.nextDouble(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::DoubleConsumer &action) override {
                    if (it.hasNext()) {
                        action.accept(it.nextDouble());
                        return true;
                    }
                    return false;
                }

                void forEach(const function::DoubleConsumer &action) override {
                    it.forEach(action);
                }

                gbool tryAdvance(const function::Consumer<Double &> &action) override {
                    if (it.hasNext()) {
                        Double i = it.nextDouble();
                        action.accept(i);
                        return true;
                    }
                    return false;
                }

                void forEach(const function::Consumer<Double &> &action) override {
                    if (it.hasNext()) {
                        Double i = it.nextDouble();
                        action.accept(i);
                    }
                }
            };

            try {
                return UNSAFE::newInstance<Spliterator>(iterator, size, characteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        DoubleSpliterator &Spliterators::spliterator(DoubleIterator &iterator, gint characteristics) {
            static CORE_FAST gint BATCH_UNIT = 1 << 10; // batch array size increment
            static CORE_FAST gint MAX_BATCH = 1 << 25; // max batch array size;
            /**
             * A DoubleSpliterator using a given DoubleIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Spliterator final : public DoubleSpliterator {
                DoubleIterator &it;
                gint additionalCharacteristics;
                glong mutable estSize; // size estimate
                gint mutable batch = 0; // batch size for splits
            public:
                /**
                 * Creates a spliterator using the given iterator for a
                 * source of unknown size, reporting the given
                 * characteristics.
                 *
                 * @param iterator the iterator for the source
                 * @param characteristics properties of this spliterator's
                 * source or elements.
                 */
                CORE_EXPLICIT Spliterator(DoubleIterator &iterator, gint characteristics): it(iterator) {
                    additionalCharacteristics = characteristics & ~(SIZED | SUBSIZED);
                    estSize = Long::MAX_VALUE;
                }

                glong estimateSize() const override {
                    return estSize;
                }

                gint characteristics() const override {
                    return additionalCharacteristics;
                }

                const Comparator<Double> & comparator() const override {
                    if (hasCharacteristics(SORTED))
                        return Comparator<Double>::naturalOrder();
                    IllegalStateException().throws($ftrace());
                }

                DoubleSpliterator & trySplit() override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        DoubleArray a = DoubleArray(n);
                        gint j = 0;
                        do { a[j] = it.nextDouble(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                const DoubleSpliterator & trySplit() const override {
                    glong size = estSize;
                    if (size > 1 && it.hasNext()) {
                        gint n = batch + BATCH_UNIT;
                        if (n > size)
                            n = (gint) size;
                        if (n > MAX_BATCH)
                            n = MAX_BATCH;
                        DoubleArray a = DoubleArray(n);
                        gint j = 0;
                        do { a[j] = it.nextDouble(); } while (++j < n && it.hasNext());
                        batch = j;
                        if (estSize != Long::MAX_VALUE) {
                            estSize -= j;
                        }
                        return spliterator(UNSAFE::moveInstance(a), 0, j, additionalCharacteristics);
                    }
                    return *this;
                }

                gbool tryAdvance(const function::DoubleConsumer &action) override {
                    if (it.hasNext()) {
                        action.accept(it.nextDouble());
                        return true;
                    }
                    return false;
                }

                void forEach(const function::DoubleConsumer &action) override {
                    it.forEach(action);
                }

                gbool tryAdvance(const function::Consumer<Double &> &action) override {
                    if (it.hasNext()) {
                        Double i = it.nextDouble();
                        action.accept(i);
                        return true;
                    }
                    return false;
                }

                void forEach(const function::Consumer<Double &> &action) override {
                    if (it.hasNext()) {
                        Double i = it.nextDouble();
                        action.accept(i);
                    }
                }
            };

            try {
                return UNSAFE::newInstance<Spliterator>(iterator, characteristics);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        IntIterator &Spliterators::iterator(IntSpliterator &spliterator) {
            /**
             * A IntSpliterator using a given IntIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Iterator final : public virtual IntIterator, public virtual function::IntConsumer {
                gbool mutable isReady = false;
                gint mutable nextValue = 0;
                IntSpliterator &spliterator;

            public:
                CORE_EXPLICIT Iterator(IntSpliterator &spliterator): spliterator(spliterator) {
                }

                void accept(gint value) const override {
                    isReady = true;
                    nextValue = value;
                }

                gbool hasNext() const override {
                    if (!isReady)
                        spliterator.tryAdvance(*this);
                    return isReady;
                }

                gint nextInt() override {
                    if (!isReady && !hasNext())
                        NoSuchElementException().throws($ftrace());
                    isReady = false;
                    return nextValue;
                }

                void forEach(const IntConsumer &action) override {
                    if (isReady) {
                        action.accept(nextValue);
                        isReady = false;
                    }
                    spliterator.forEach(action);
                }

                gbool equals(const Object &obj) const override {
                    if (this == &obj)
                        return true;
                    if (!Class<Iterator>::hasInstance(obj))
                        return false;
                    Iterator const &it = CORE_XCAST(Iterator const, obj);
                    if (isReady)
                        return it.isReady && nextValue == it.nextValue && spliterator == it.spliterator;
                    return !it.isReady && spliterator == it.spliterator;
                }

                void forEach(const function::Consumer<Integer &> &action) override {
                    if (isReady) {
                        Integer next = nextValue;
                        action.accept(next);
                        isReady = false;
                    }
                    spliterator.forEach(action);
                }
            };

            try {
                return UNSAFE::newInstance<Iterator>(spliterator);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        LongIterator &Spliterators::iterator(LongSpliterator &spliterator) {
            /**
             * A LongSpliterator using a given LongIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Iterator final : public virtual LongIterator, public virtual function::LongConsumer {
                gbool mutable isReady = false;
                glong mutable nextValue = 0;
                LongSpliterator &spliterator;

            public:
                CORE_EXPLICIT Iterator(LongSpliterator &spliterator): spliterator(spliterator) {
                }

                void accept(glong value) const override {
                    isReady = true;
                    nextValue = value;
                }

                gbool hasNext() const override {
                    if (!isReady)
                        spliterator.tryAdvance(*this);
                    return isReady;
                }

                glong nextLong() override {
                    if (!isReady && !hasNext())
                        NoSuchElementException().throws($ftrace());
                    isReady = false;
                    return nextValue;
                }

                void forEach(const LongConsumer &action) override {
                    if (isReady) {
                        action.accept(nextValue);
                        isReady = false;
                    }
                    spliterator.forEach(action);
                }

                gbool equals(const Object &obj) const override {
                    if (this == &obj)
                        return true;
                    if (!Class<Iterator>::hasInstance(obj))
                        return false;
                    Iterator const &it = CORE_XCAST(Iterator const, obj);
                    if (isReady)
                        return it.isReady && nextValue == it.nextValue && spliterator == it.spliterator;
                    return !it.isReady && spliterator == it.spliterator;
                }

                void forEach(const function::Consumer<Long &> &action) override {
                    if (isReady) {
                        Long next = nextValue;
                        action.accept(next);
                        isReady = false;
                    }
                    spliterator.forEach(action);
                }
            };

            try {
                return UNSAFE::newInstance<Iterator>(spliterator);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        DoubleIterator &Spliterators::iterator(DoubleSpliterator &spliterator) {
            /**
             * A DoubleSpliterator using a given DoubleIterator for element
             * operations. The spliterator implements @c trySplit to
             * permit limited parallelism.
             */
            class Iterator final : public virtual DoubleIterator, public virtual function::DoubleConsumer {
                gbool mutable isReady = false;
                gdouble mutable nextValue = 0;
                DoubleSpliterator &spliterator;

            public:
                CORE_EXPLICIT Iterator(DoubleSpliterator &spliterator): spliterator(spliterator) {
                }

                void accept(gdouble value) const override {
                    isReady = true;
                    nextValue = value;
                }

                gbool hasNext() const override {
                    if (!isReady)
                        spliterator.tryAdvance(*this);
                    return isReady;
                }

                gdouble nextDouble() override {
                    if (!isReady && !hasNext())
                        NoSuchElementException().throws($ftrace());
                    isReady = false;
                    return nextValue;
                }

                void forEach(const DoubleConsumer &action) override {
                    if (isReady) {
                        action.accept(nextValue);
                        isReady = false;
                    }
                    spliterator.forEach(action);
                }

                gbool equals(const Object &obj) const override {
                    if (this == &obj)
                        return true;
                    if (!Class<Iterator>::hasInstance(obj))
                        return false;
                    Iterator const &it = CORE_XCAST(Iterator const, obj);
                    if (isReady)
                        return it.isReady && nextValue == it.nextValue && spliterator == it.spliterator;
                    return !it.isReady && spliterator == it.spliterator;
                }

                void forEach(const function::Consumer<Double &> &action) override {
                    if (isReady) {
                        Double next = nextValue;
                        action.accept(next);
                        isReady = false;
                    }
                    spliterator.forEach(action);
                }
            };

            try {
                return UNSAFE::newInstance<Iterator>(spliterator);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }
    } // util
} // core
