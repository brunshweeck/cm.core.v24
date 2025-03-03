//
// Created by brunshweeck on 16/11/24.
//

#ifndef CORE24_SPLITERATORS_H
#define CORE24_SPLITERATORS_H

#include <core/util/Arrays.h>
#include <core/util/Spliterator.h>

namespace core {
    namespace util {
        /**
         * Static classes and methods for operating on or creating instances of
         * @b Spliterator and its primitive specializations
         * @b IntSpliterator, @b Spliterator, and @b DoubleSpliterator.
         *
         * @see Spliterator
         */
        class Spliterators final : public Object {
            // Suppresses default constructor, ensuring non-instantiability.
            Spliterators() = default;

        public:
            // Empty spliterators

            /**
             * Creates an empty @c Spliterator
             *
             * <p>
             * The empty spliterator reports @b Spliterator::SIZED and
             * @b Spliterator::SUBSIZED.  Calls to
             * @b Spliterator::trySplit() always return @c itself.
             * </p>
             * @tparam T Type of elements
             * @return An empty spliterator
             */
            template <class T>
            static Spliterator<T>& emptySpliterator() {
                class Spliterator final : public util::Spliterator<T> {
                public:
                    gbool tryAdvance(const function::Consumer<T&>& action) override {
                        return false;
                    }

                    util::Spliterator<T>& trySplit() override {
                        return *this;
                    }

                    const util::Spliterator<T>& trySplit() const override {
                        return *this;
                    }

                    glong estimateSize() const override {
                        return 0;
                    }

                    gint characteristics() const override {
                        return util::Spliterator<T>::SIZED | util::Spliterator<T>::SUBSIZED;
                    }

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Spliterator>::hasInstance(o);
                    }

                    void forEach(const function::Consumer<T&>& action) override {}
                };

                try {
                    return UNSAFE::newInstance<Spliterator>();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates an empty @c Spliterator
             *
             * <p>
             * The empty spliterator reports @b Spliterator::SIZED and
             * @b Spliterator::SUBSIZED.  Calls to
             * @b Spliterator::trySplit() always return @c itself.
             * </p>
             * @tparam T Type of elements
             * @return An empty spliterator
             */
            template <class T>
            static Spliterator2<T>& emptySpliterator2() {
                class Spliterator final : public Spliterator2<T> {
                public:
                    gbool tryAdvance(const function::Consumer<T>& action) override { return false; }

                    Spliterator2<T>& trySplit() override {
                        return *this;
                    }

                    const Spliterator2<T>& trySplit() const override {
                        return *this;
                    }

                    glong estimateSize() const override {
                        return 0;
                    }

                    gint characteristics() const override {
                        return util::Spliterator<T>::SIZED | util::Spliterator<T>::SUBSIZED;
                    }

                    gbool equals(const Object& o) const override {
                        return this == &o || Class<Spliterator>::hasInstance(o);
                    }

                    void forEach(const function::Consumer<T>& action) override {}
                };

                try {
                    return UNSAFE::newInstance<Spliterator>();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates an empty @c IntSpliterator
             *
             * <p>
             * The empty spliterator reports @b Spliterator::SIZED and
             * @b Spliterator::SUBSIZED.  Calls to
             * @b Spliterator::trySplit() always return @c itself.
             * </p>
             * @return An empty spliterator
             */
            static IntSpliterator& emptyIntSpliterator();

            /**
             * Creates an empty @c LongSpliterator
             *
             * <p>
             * The empty spliterator reports @b Spliterator::SIZED and
             * @b Spliterator::SUBSIZED.  Calls to
             * @b Spliterator::trySplit() always return @c itself.
             * </p>
             * @return An empty spliterator
             */
            static LongSpliterator& emptyLongSpliterator();

            /**
             * Creates an empty @c DoubleSpliterator
             *
             * <p>
             * The empty spliterator reports @b Spliterator::SIZED and
             * @b Spliterator::SUBSIZED.  Calls to
             * @b Spliterator::trySplit() always return @c itself.
             * </p>
             * @return An empty spliterator
             */
            static DoubleSpliterator& emptyDoubleSpliterator();

            // Array-based spliterators

            /**
             * Creates a @c Spliterator covering the elements of a given array,
             * using a customized set of spliterator characteristics.
             *
             * <p>
             * This method is provided as an implementation convenience for
             * Spliterators which store portions of their elements in arrays, and need
             * fine control over Spliterator characteristics.  Most other situations in
             * which a Spliterator for an array is needed should use
             * @b Arrays::spliterator(Array<T>).
             * </p>
             * <p>
             * The returned spliterator always reports the characteristics
             * @c SIZED and @c SUBSIZED.  The caller may provide additional
             * characteristics for the spliterator to report; it is common to
             * additionally specify @c IMMUTABLE and @c ORDERED.
             * </p>
             * @tparam T Type of elements
             * @param array The array, assumed to be unmodified during use
             * @param additionalCharacteristics Additional spliterator characteristics
             *        of this spliterator's source or elements beyond @c SIZED and
             *        @c SUBSIZED which are always reported
             * @return A spliterator for an array
             * @see Arrays::spliterator(Array<T>)
             */
            template <class T = Object, class X = T,
                      ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Spliterator<T>& spliterator(Array<X> array, gint additionalCharacteristics) {
                try {
                    gint len = array.length();
                    return spliterator<T>(UNSAFE::moveInstance(array),
                                          0, len,
                                          additionalCharacteristics);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            template <class T = Object, class X = T,
                      ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Spliterator2<T>& spliterator2(Array<X> array, gint additionalCharacteristics) {
                try {
                    gint len = array.length();
                    return spliterator2<T>(UNSAFE::moveInstance(array),
                                           0,
                                           len,
                                           additionalCharacteristics);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates a @c Spliterator covering a range of elements of a given
             * array, using a customized set of spliterator characteristics.
             *
             * <p>
             * This method is provided as an implementation convenience for
             * Spliterators which store portions of their elements in arrays, and need
             * fine control over Spliterator characteristics.  Most other situations in
             * which a Spliterator for an array is needed should use
             * @b Arrays::spliterator(Array<T>).
             * </p>
             * <p>
             * The returned spliterator always reports the characteristics
             * @c SIZED and @c SUBSIZED.  The caller may provide additional
             * characteristics for the spliterator to report; it is common to
             * additionally specify @c IMMUTABLE and @c ORDERED.
             * </p>
             * @tparam T Type of elements
             * @param array The array, assumed to be unmodified during use
             * @param fromIndex The least index (inclusive) to cover
             * @param toIndex One past the greatest index to cover
             * @param additionalCharacteristics Additional spliterator characteristics
             *        of this spliterator's source or elements beyond @c SIZED and
             *        @c SUBSIZED which are always reported
             * @return A spliterator for an array
             * @throws IndexOutOfBoundsException if @c fromIndex is negative,
             *         @c toIndex is less than @c fromIndex, or
             *         @c toIndex is greater than the array size
             * @see Arrays::spliterator(Array<T>, int, int)
             */
            template <class T, class X, ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Spliterator<T>& spliterator(Array<X> array, gint fromIndex, gint toIndex,
                                               gint additionalCharacteristics) {
                class Spliterator final : public util::Spliterator<T> {
                    Array<X> array;
                    gint mutable offset;
                    gint limit;
                    gint additionalCharacteristics;

                public:
                    CORE_EXPLICIT Spliterator(Array<X> a, gint fromIndex, gint toIndex, gint characteristics) {
                        array = UNSAFE::moveInstance(a);
                        offset = fromIndex;
                        limit = toIndex;
                        additionalCharacteristics =
                                characteristics | util::Spliterator<T>::SIZED | util::Spliterator<T>::SUBSIZED;
                    }

                    gbool tryAdvance(const function::Consumer<T&>& action) override {
                        if (offset < limit) {
                            action.accept(array[offset++]);
                            return true;
                        }
                        return false;
                    }

                    util::Spliterator<T>& trySplit() override {
                        gint mid = (offset + limit) / 2;
                        if (mid <= offset)
                            return *this;
                        Array<X> subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator<T>(subArray, additionalCharacteristics);
                    }

                    const util::Spliterator<T>& trySplit() const override {
                        gint mid = (offset + limit) / 2;
                        if (mid <= offset)
                            return *this;
                        Array<X> subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator<T>(subArray, additionalCharacteristics);
                    }

                    glong estimateSize() const override {
                        return limit - offset;
                    }

                    gint characteristics() const override {
                        return additionalCharacteristics;
                    }

                    const Comparator<T>& comparator() const override {
                        if (this->hasCharacteristics(util::Spliterator<T>::SORTED))
                            return Comparator<T>::naturalOrder();
                        IllegalStateException().throws($ftrace());
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Spliterator>::hasInstance(o))
                            return false;
                        Spliterator const& spliterator = CORE_XCAST(Spliterator const, o);
                        return offset == spliterator.offset &&
                                limit == spliterator.limit &&
                                additionalCharacteristics == spliterator.additionalCharacteristics &&
                                array == spliterator.array;
                    }
                };

                try {
                    return UNSAFE::newInstance<Spliterator>(UNSAFE::moveInstance(array),
                                                            fromIndex,
                                                            toIndex,
                                                            additionalCharacteristics);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            template <class T, class X, ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Spliterator2<T>& spliterator2(Array<X> array, gint fromIndex, gint toIndex,
                                                 gint additionalCharacteristics) {
                class Spliterator final : public Spliterator2<T> {
                    Array<X> array;
                    gint mutable offset;
                    gint limit;
                    gint additionalCharacteristics;

                public:
                    CORE_EXPLICIT Spliterator(Array<X> a, gint fromIndex, gint toIndex, gint characteristics) {
                        array = UNSAFE::moveInstance(a);
                        offset = fromIndex;
                        limit = toIndex;
                        additionalCharacteristics =
                                characteristics | util::Spliterator<T>::SIZED | util::Spliterator<T>::SUBSIZED;
                    }

                    gbool tryAdvance(const function::Consumer<T>& action) override {
                        if (offset < limit) {
                            action.accept(array[offset++]);
                            return true;
                        }
                        return false;
                    }

                    Spliterator2<T>& trySplit() override {
                        gint mid = (offset + limit) / 2;
                        if (mid <= offset)
                            return *this;
                        Array<X> subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator2<T>(subArray, additionalCharacteristics);
                    }

                    const Spliterator2<T>& trySplit() const override {
                        gint mid = (offset + limit) / 2;
                        if (mid <= offset)
                            return *this;
                        Array<X> subArray = Arrays::copyOfRange(array, offset, mid);
                        offset = mid;
                        return spliterator2<T>(subArray, additionalCharacteristics);
                    }

                    glong estimateSize() const override {
                        return limit - offset;
                    }

                    gint characteristics() const override {
                        return additionalCharacteristics;
                    }

                    const Comparator<T>& comparator() const override {
                        if (this->hasCharacteristics(util::Spliterator<T>::SORTED))
                            return Comparator<T>::naturalOrder();
                        IllegalStateException().throws($ftrace());
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;
                        if (!Class<Spliterator>::hasInstance(o))
                            return false;
                        Spliterator const& spliterator = CORE_XCAST(Spliterator const, o);
                        return offset == spliterator.offset &&
                                limit == spliterator.limit &&
                                additionalCharacteristics == spliterator.additionalCharacteristics &&
                                array == spliterator.array;
                    }
                };

                try {
                    return UNSAFE::newInstance<Spliterator>(
                        UNSAFE::moveInstance(array),
                        fromIndex,
                        toIndex,
                        additionalCharacteristics);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates a @c IntSpliterator covering the elements of a given array,
             * using a customized set of spliterator characteristics.
             *
             * <p>
             * This method is provided as an implementation convenience for
             * Spliterators which store portions of their elements in arrays, and need
             * fine control over Spliterator characteristics.  Most other situations in
             * which a Spliterator for an array is needed should use
             * @b Arrays::spliterator(IntArray).
             * </p>
             * <p>
             * The returned spliterator always reports the characteristics
             * @c SIZED and @c SUBSIZED.  The caller may provide additional
             * characteristics for the spliterator to report; it is common to
             * additionally specify @c IMMUTABLE and @c ORDERED.
             * </p>
             * @param array The array, assumed to be unmodified during use
             * @param additionalCharacteristics Additional spliterator characteristics
             *        of this spliterator's source or elements beyond @c SIZED and
             *        @c SUBSIZED which are always reported
             * @return A spliterator for an array
             * @see Arrays::spliterator(IntArray)
             */
            static IntSpliterator& spliterator(IntArray array, gint additionalCharacteristics);

            /**
             * Creates a @c IntSpliterator covering a range of elements of a
             * given array, using a customized set of spliterator characteristics.
             *
             * <p>
             * This method is provided as an implementation convenience for
             * Spliterators which store portions of their elements in arrays, and need
             * fine control over Spliterator characteristics.  Most other situations in
             * which a Spliterator for an array is needed should use
             * @b Arrays::spliterator(IntArray, int, int).
             * </p>
             * <p>
             * The returned spliterator always reports the characteristics
             * @c SIZED and @c SUBSIZED.  The caller may provide additional
             * characteristics for the spliterator to report; it is common to
             * additionally specify @c IMMUTABLE and @c ORDERED.
             * </p>
             * @param array The array, assumed to be unmodified during use
             * @param fromIndex The least index (inclusive) to cover
             * @param toIndex One past the greatest index to cover
             * @param additionalCharacteristics Additional spliterator characteristics
             *        of this spliterator's source or elements beyond @c SIZED and
             *        @c SUBSIZED which are always reported
             * @return A spliterator for an array
             * @throws IndexOutOfBoundsException if @c fromIndex is negative,
             *         @c toIndex is less than @c fromIndex, or
             *         @c toIndex is greater than the array size
             * @see Arrays::spliterator(IntArray, int, int)
             */
            static IntSpliterator& spliterator(IntArray array, gint fromIndex, gint toIndex,
                                               gint additionalCharacteristics);

            /**
             * Creates a @c LongSpliterator covering the elements of a given array,
             * using a customized set of spliterator characteristics.
             *
             * <p>
             * This method is provided as an implementation convenience for
             * Spliterators which store portions of their elements in arrays, and need
             * fine control over Spliterator characteristics.  Most other situations in
             * which a Spliterator for an array is needed should use
             * @b Arrays::spliterator(LongArray).
             * </p>
             * <p>
             * The returned spliterator always reports the characteristics
             * @c SIZED and @c SUBSIZED.  The caller may provide additional
             * characteristics for the spliterator to report; it is common to
             * additionally specify @c IMMUTABLE and @c ORDERED.
             * </p>
             * @param array The array, assumed to be unmodified during use
             * @param additionalCharacteristics Additional spliterator characteristics
             *        of this spliterator's source or elements beyond @c SIZED and
             *        @c SUBSIZED which are always reported
             * @return A spliterator for an array
             * @see Arrays::spliterator(LongArray)
             */
            static LongSpliterator& spliterator(LongArray array, gint additionalCharacteristics);

            /**
             * Creates a @c LongSpliterator covering a range of elements of a
             * given array, using a customized set of spliterator characteristics.
             *
             * <p>
             * This method is provided as an implementation convenience for
             * Spliterators which store portions of their elements in arrays, and need
             * fine control over Spliterator characteristics.  Most other situations in
             * which a Spliterator for an array is needed should use
             * @b Arrays::spliterator(LongArray, int, int).
             * </p>
             * <p>
             * The returned spliterator always reports the characteristics
             * @c SIZED and @c SUBSIZED.  The caller may provide additional
             * characteristics for the spliterator to report.  (For example, if it is
             * known the array will not be further modified, specify @c IMMUTABLE;
             * if the array data is considered to have an encounter order, specify
             * @c ORDERED).  The method @b Arrays::spliterator(LongArray, int, int) can
             * often be used instead, which returns a spliterator that reports
             * @c SIZED, @c SUBSIZED, @c IMMUTABLE, and @c ORDERED.
             * </p>
             * @param array The array, assumed to be unmodified during use
             * @param fromIndex The least index (inclusive) to cover
             * @param toIndex One past the greatest index to cover
             * @param additionalCharacteristics Additional spliterator characteristics
             *        of this spliterator's source or elements beyond @c SIZED and
             *        @c SUBSIZED which are always reported
             * @return A spliterator for an array
             * @throws IndexOutOfBoundsException if @c fromIndex is negative,
             *         @c toIndex is less than @c fromIndex, or
             *         @c toIndex is greater than the array size
             * @see Arrays::spliterator(LongArray, int, int)
             */
            static LongSpliterator& spliterator(LongArray array, gint fromIndex, gint toIndex,
                                                gint additionalCharacteristics);

            /**
             * Creates a @c DoubleSpliterator covering the elements of a given array,
             * using a customized set of spliterator characteristics.
             *
             * <p>
             * This method is provided as an implementation convenience for
             * Spliterators which store portions of their elements in arrays, and need
             * fine control over Spliterator characteristics.  Most other situations in
             * which a Spliterator for an array is needed should use
             * @b Arrays::spliterator(DoubleArray).
             * </p>
             * <p>
             * The returned spliterator always reports the characteristics
             * @c SIZED and @c SUBSIZED.  The caller may provide additional
             * characteristics for the spliterator to report; it is common to
             * additionally specify @c IMMUTABLE and @c ORDERED.
             * </p>
             * @param array The array, assumed to be unmodified during use
             * @param additionalCharacteristics Additional spliterator characteristics
             *        of this spliterator's source or elements beyond @c SIZED and
             *        @c SUBSIZED which are always reported
             * @return A spliterator for an array
             * @see Arrays::spliterator(DoubleArray)
             */
            static DoubleSpliterator& spliterator(DoubleArray array, gint additionalCharacteristics);

            /**
             * Creates a @c DoubleSpliterator covering a range of elements of a
             * given array, using a customized set of spliterator characteristics.
             *
             * <p>
             * This method is provided as an implementation convenience for
             * Spliterators which store portions of their elements in arrays, and need
             * fine control over Spliterator characteristics.  Most other situations in
             * which a Spliterator for an array is needed should use
             * @b Arrays::spliterator(DoubleArray, int, int).
             * </p>
             * <p>
             * The returned spliterator always reports the characteristics
             * @c SIZED and @c SUBSIZED.  The caller may provide additional
             * characteristics for the spliterator to report.  (For example, if it is
             * known the array will not be further modified, specify @c IMMUTABLE;
             * if the array data is considered to have an encounter order, specify
             * @c ORDERED).  The method @b Arrays::spliterator(LongArray, int, int) can
             * often be used instead, which returns a spliterator that reports
             * @c SIZED, @c SUBSIZED, @c IMMUTABLE, and @c ORDERED.
             * </p>
             * @param array The array, assumed to be unmodified during use
             * @param fromIndex The least index (inclusive) to cover
             * @param toIndex One past the greatest index to cover
             * @param additionalCharacteristics Additional spliterator characteristics
             *        of this spliterator's source or elements beyond @c SIZED and
             *        @c SUBSIZED which are always reported
             * @return A spliterator for an array
             * @throws IndexOutOfBoundsException if @c fromIndex is negative,
             *         @c toIndex is less than @c fromIndex, or
             *         @c toIndex is greater than the array size
             * @see Arrays::spliterator(DoubleArray, int, int)
             */
            static DoubleSpliterator& spliterator(DoubleArray array, gint fromIndex, gint toIndex,
                                                  gint additionalCharacteristics);

            // Iterator-based spliterators

            /**
             * Creates a @c Spliterator using a given @c Iterator
             * as the source of elements, and with a given initially reported size.
             *
             * <p>
             * The spliterator is not <em>late-binding</em>, inherits
             * the <em>fail-fast</em> properties of the iterator, and implements
             * @c trySplit to permit limited parallelism.
             * </p>
             * <p>
             * Traversal of elements should be accomplished through the spliterator.
             * The behaviour of splitting and traversal is undefined if the iterator is
             * operated on after the spliterator is returned, or the initially reported
             * size is not equal to the actual number of elements in the source.
             * </p>
             * @tparam T Type of elements
             * @param iterator The iterator for the source
             * @param size The number of elements in the source, to be reported as
             *        initial @c estimateSize
             * @param characteristics Characteristics of this spliterator's source or
             *        elements.  The characteristics @c SIZED and @c SUBSIZED
             *        are additionally reported unless @c CONCURRENT is supplied.
             * @return A spliterator from an iterator
             */
            template <class T, class X = T,
                      ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Spliterator<T>& spliterator(Iterator<X>& iterator, glong size, gint characteristics) {
                class Spliterator final : public util::Spliterator<T> {
                    Iterator<X>& iterator;
                    glong mutable estSize;
                    gint mutable batch = 0;
                    gint additionalCharacteristics;

                public:
                    CORE_EXPLICIT Spliterator(Iterator<X>& it, glong size, gint characteristics)
                        : iterator(it) {
                        estSize = size;
                        additionalCharacteristics = (characteristics & util::Spliterator<T>::CONCURRENT) == 0
                                                        ? characteristics | util::Spliterator<T>::SIZED |
                                                        util::Spliterator<T>::SUBSIZED
                                                        : characteristics;
                    }

                    gbool tryAdvance(const function::Consumer<T&>& action) override {
                        if (iterator.hasNext()) {
                            action.accept(iterator.next());
                            return true;
                        }
                        return false;
                    }

                    util::Spliterator<T>& trySplit() override {
                        /*
                         * Split into arrays of arithmetically increasing batch
                         * sizes.  This will only improve parallel performance if
                         * per-element Consumer actions are more costly than
                         * transferring them into an array.  The use of an
                         * arithmetic progression in split sizes provides overhead
                         * vs parallelism bounds that do not particularly favor or
                         * penalize cases of lightweight vs heavyweight element
                         * operations, across combinations of #elements vs #cores,
                         * whether either are known.  We generate
                         * O(sqrt(#elements)) splits, allowing O(sqrt(#cores))
                         * potential speedup.
                         */
                        static CORE_FAST gint BATCH_UNIT = 1 << 10;
                        static CORE_FAST gint MAX_BATCH = 1 << 25;
                        glong s = estSize;
                        if (s > 1 && iterator.hasNext()) {
                            gint n = batch + BATCH_UNIT;
                            if (n > s)
                                n = (gint) s;
                            if (n > MAX_BATCH)
                                n = MAX_BATCH;
                            CORE_IGNORE_DEPRECATIONS(Array<T> a = Array<T>(n);)
                            gint j = 0;
                            do { Arrays::fastSet(a, j, iterator.next()); } while (++j < n && iterator.hasNext());
                            batch = j;
                            if (estSize != Long::MAX_VALUE)
                                estSize -= j;
                            if (j < n)
                                return spliterator<T>(Arrays::copyOf(a, j), 0, j, additionalCharacteristics);
                            return spliterator<T>(a, 0, j, additionalCharacteristics);
                        }
                        return *this;
                    }

                    const util::Spliterator<T>& trySplit() const override {
                        /*
                         * Split into arrays of arithmetically increasing batch
                         * sizes.  This will only improve parallel performance if
                         * per-element Consumer actions are more costly than
                         * transferring them into an array.  The use of an
                         * arithmetic progression in split sizes provides overhead
                         * vs parallelism bounds that do not particularly favor or
                         * penalize cases of lightweight vs heavyweight element
                         * operations, across combinations of #elements vs #cores,
                         * whether either are known.  We generate
                         * O(sqrt(#elements)) splits, allowing O(sqrt(#cores))
                         * potential speedup.
                         */
                        static CORE_FAST gint BATCH_UNIT = 1 << 10;
                        static CORE_FAST gint MAX_BATCH = 1 << 25;
                        glong s = estSize;
                        if (s > 1 && iterator.hasNext()) {
                            gint n = batch + BATCH_UNIT;
                            if (n > s)
                                n = (gint) s;
                            if (n > MAX_BATCH)
                                n = MAX_BATCH;
                            CORE_IGNORE_DEPRECATIONS(Array<T> a = Array<T>(n);)
                            gint j = 0;
                            do { Arrays::fastSet(a, j, iterator.next()); } while (++j < n && iterator.hasNext());
                            batch = j;
                            if (estSize != Long::MAX_VALUE)
                                estSize -= j;
                            if (j < n)
                                return spliterator<T>(Arrays::copyOf(a, j), 0, j, additionalCharacteristics);
                            return spliterator<T>(a, 0, j, additionalCharacteristics);
                        }
                        return *this;
                    }

                    glong estimateSize() const override {
                        return estSize;
                    }

                    gint characteristics() const override {
                        return additionalCharacteristics;
                    }

                    void forEach(const function::Consumer<T&>& action) override {
                        iterator.forEach(action);
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Spliterator>::hasInstance(o))
                            return false;

                        Spliterator const& spliterator = CORE_XCAST(Spliterator const, o);
                        return additionalCharacteristics == spliterator.additionalCharacteristics &&
                                batch == spliterator.batch &&
                                estSize == spliterator.estSize &&
                                iterator == spliterator.iterator;
                    }
                };

                if (size < 0)
                    IllegalArgumentException().throws($ftrace());

                try {
                    return UNSAFE::newInstance<Spliterator>(iterator, size, characteristics);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            template <class T, class X = T,
                      ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Spliterator2<T>& spliterator(Iterator2<X>& iterator, glong size, gint characteristics) {
                class Spliterator final : public Spliterator2<T> {
                    Iterator2<X>& iterator;
                    glong mutable estSize;
                    gint mutable batch = 0;
                    gint additionalCharacteristics;

                public:
                    CORE_EXPLICIT Spliterator(Iterator2<X>& it, glong size, gint characteristics)
                        : iterator(it) {
                        estSize = size;
                        additionalCharacteristics = (characteristics & util::Spliterator<T>::CONCURRENT) == 0
                                                        ? characteristics | util::Spliterator<T>::SIZED |
                                                        util::Spliterator<T>::SUBSIZED
                                                        : characteristics;
                    }

                    gbool tryAdvance(const function::Consumer<T>& action) override {
                        if (iterator.hasNext()) {
                            action.accept(iterator.next());
                            return true;
                        }
                        return false;
                    }

                    Spliterator2<T>& trySplit() override {
                        /*
                         * Split into arrays of arithmetically increasing batch
                         * sizes.  This will only improve parallel performance if
                         * per-element Consumer actions are more costly than
                         * transferring them into an array.  The use of an
                         * arithmetic progression in split sizes provides overhead
                         * vs parallelism bounds that do not particularly favor or
                         * penalize cases of lightweight vs heavyweight element
                         * operations, across combinations of #elements vs #cores,
                         * whether either are known.  We generate
                         * O(sqrt(#elements)) splits, allowing O(sqrt(#cores))
                         * potential speedup.
                         */
                        static CORE_FAST gint BATCH_UNIT = 1 << 10;
                        static CORE_FAST gint MAX_BATCH = 1 << 25;
                        glong s = estSize;
                        if (s > 1 && iterator.hasNext()) {
                            gint n = batch + BATCH_UNIT;
                            if (n > s)
                                n = (gint) s;
                            if (n > MAX_BATCH)
                                n = MAX_BATCH;
                            CORE_IGNORE_DEPRECATIONS(Array<X> a = Array<X>(n);)
                            gint j = 0;
                            do { Arrays::fastSet(a, j, (X&) iterator.next()); } while (++j < n && iterator.hasNext());
                            batch = j;
                            if (estSize != Long::MAX_VALUE)
                                estSize -= j;
                            if (j < n)
                                return spliterator2<T>(Arrays::copyOf(a, j), 0, j, additionalCharacteristics);
                            return spliterator2<T>(a, 0, j, additionalCharacteristics);
                        }
                        return *this;
                    }

                    const Spliterator2<T>& trySplit() const override {
                        /*
                         * Split into arrays of arithmetically increasing batch
                         * sizes.  This will only improve parallel performance if
                         * per-element Consumer actions are more costly than
                         * transferring them into an array.  The use of an
                         * arithmetic progression in split sizes provides overhead
                         * vs parallelism bounds that do not particularly favor or
                         * penalize cases of lightweight vs heavyweight element
                         * operations, across combinations of #elements vs #cores,
                         * whether either are known.  We generate
                         * O(sqrt(#elements)) splits, allowing O(sqrt(#cores))
                         * potential speedup.
                         */
                        static CORE_FAST gint BATCH_UNIT = 1 << 10;
                        static CORE_FAST gint MAX_BATCH = 1 << 25;
                        glong s = estSize;
                        if (s > 1 && iterator.hasNext()) {
                            gint n = batch + BATCH_UNIT;
                            if (n > s)
                                n = (gint) s;
                            if (n > MAX_BATCH)
                                n = MAX_BATCH;
                            CORE_IGNORE_DEPRECATIONS(Array<X> a = Array<X>(n);)
                            gint j = 0;
                            do { Arrays::fastSet(a, j, (X&) iterator.next()); } while (++j < n && iterator.hasNext());
                            batch = j;
                            if (estSize != Long::MAX_VALUE)
                                estSize -= j;
                            if (j < n)
                                return spliterator2<T>(Arrays::copyOf(a, j), 0, j, additionalCharacteristics);
                            return spliterator2<T>(a, 0, j, additionalCharacteristics);
                        }
                        return *this;
                    }

                    glong estimateSize() const override {
                        return estSize;
                    }

                    gint characteristics() const override {
                        return additionalCharacteristics;
                    }

                    void forEach(const function::Consumer<T>& action) override {
                        iterator.forEach(action);
                    }

                    gbool equals(const Object& o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Spliterator>::hasInstance(o))
                            return false;

                        Spliterator const& spliterator = CORE_XCAST(Spliterator const, o);
                        return additionalCharacteristics == spliterator.additionalCharacteristics &&
                                batch == spliterator.batch &&
                                estSize == spliterator.estSize &&
                                iterator == spliterator.iterator;
                    }
                };

                if (size < 0)
                    IllegalArgumentException().throws($ftrace());

                try {
                    return UNSAFE::newInstance<Spliterator>(iterator, size, characteristics);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates a @c Spliterator using a given @c Iterator
             * as the source of elements, with no initial size estimate.
             *
             * <p>
             * The spliterator is not <em>late-binding</em>, inherits
             * the <em>fail-fast</em> properties of the iterator, and implements
             * @c trySplit to permit limited parallelism.
             * </p>
             * <p>
             * Traversal of elements should be accomplished through the spliterator.
             * The behaviour of splitting and traversal is undefined if the iterator is
             * operated on after the spliterator is returned.
             * </p>
             * @tparam T Type of elements
             * @param iterator The iterator for the source
             * @param characteristics Characteristics of this spliterator's source
             *        or elements (@c SIZED and @c SUBSIZED, if supplied, are
             *        ignored and are not reported.)
             * @return A spliterator from an iterator
             */
            template <class T, class X = T,
                      ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Spliterator<T>& spliterator(Iterator<X>& iterator, gint characteristics) {
                try {
                    return spliterator<T>(iterator, Long::MAX_VALUE, characteristics);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            template <class T, class X = T,
                      ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Spliterator2<T>& spliterator(Iterator2<X>& iterator, gint characteristics) {
                try {
                    return spliterator<T>(iterator, Long::MAX_VALUE, characteristics);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates a @c IntSpliterator using a given
             * @c IntIterator as the source of elements, and with a given
             * initially reported size.
             *
             * <p>
             * The spliterator is not <em>late-binding</em>, inherits
             * the <em>fail-fast</em> properties of the iterator, and implements
             * @c trySplit to permit limited parallelism.
             * </p>
             * <p>
             * Traversal of elements should be accomplished through the spliterator.
             * The behaviour of splitting and traversal is undefined if the iterator is
             * operated on after the spliterator is returned, or the initially reported
             * size is not equal to the actual number of elements in the source.
             * </p>
             * @param iterator The iterator for the source
             * @param size The number of elements in the source, to be reported as
             *        initial @c estimateSize.
             * @param characteristics Characteristics of this spliterator's source or
             *        elements.  The characteristics @c SIZED and @c SUBSIZED
             *        are additionally reported unless @c CONCURRENT is supplied.
             * @return A spliterator from an iterator
             */
            static IntSpliterator& spliterator(IntIterator& iterator, glong size, gint characteristics);

            /**
             * Creates a @c IntSpliterator using a given
             * @c IntIterator as the source of elements, with no initial
             * size estimate.
             *
             * <p>
             * The spliterator is not <em>late-binding</em>, inherits
             * the <em>fail-fast</em> properties of the iterator, and implements
             * @c trySplit to permit limited parallelism.
             * </p>
             * <p>
             * Traversal of elements should be accomplished through the spliterator.
             * The behaviour of splitting and traversal is undefined if the iterator is
             * operated on after the spliterator is returned.
             * </p>
             * @param iterator The iterator for the source
             * @param characteristics Characteristics of this spliterator's source
             *        or elements (@c SIZED and @c SUBSIZED, if supplied, are
             *        ignored and are not reported.)
             * @return A spliterator from an iterator
             */
            static IntSpliterator& spliterator(IntIterator& iterator, gint characteristics);

            /**
             * Creates a @c LongSpliterator using a given
             * @c LongIterator as the source of elements, and with a
             * given initially reported size.
             *
             * <p>
             * The spliterator is not <em>late-binding</em>, inherits
             * the <em>fail-fast</em> properties of the iterator, and implements
             * @c trySplit to permit limited parallelism.
             * </p>
             * <p>
             * Traversal of elements should be accomplished through the spliterator.
             * The behaviour of splitting and traversal is undefined if the iterator is
             * operated on after the spliterator is returned, or the initially reported
             * size is not equal to the actual number of elements in the source.
             * </p>
             * @param iterator The iterator for the source
             * @param size The number of elements in the source, to be reported as
             *        initial @c estimateSize.
             * @param characteristics Characteristics of this spliterator's source or
             *        elements.  The characteristics @c SIZED and @c SUBSIZED
             *        are additionally reported unless @c CONCURRENT is supplied.
             * @return A spliterator from an iterator
             */
            static LongSpliterator& spliterator(LongIterator& iterator, glong size, gint characteristics);

            /**
             * Creates a @c LongSpliterator using a given
             * @c LongIterator as the source of elements, with no
             * initial size estimate.
             *
             * <p>
             * The spliterator is not <em>late-binding</em>, inherits
             * the <em>fail-fast</em> properties of the iterator, and implements
             * @c trySplit to permit limited parallelism.
             * </p>
             * <p>
             * Traversal of elements should be accomplished through the spliterator.
             * The behaviour of splitting and traversal is undefined if the iterator is
             * operated on after the spliterator is returned.
             * </p>
             * @param iterator The iterator for the source
             * @param characteristics Characteristics of this spliterator's source
             *        or elements (@c SIZED and @c SUBSIZED, if supplied, are
             *        ignored and are not reported.)
             * @return A spliterator from an iterator
             */
            static LongSpliterator& spliterator(LongIterator& iterator, gint characteristics);

            /**
             * Creates a @c DoubleSpliterator using a given
             * @c DoubleIterator as the source of elements, and with a
             * given initially reported size.
             *
             * <p>
             * The spliterator is not <em>late-binding</em>, inherits
             * the <em>fail-fast</em> properties of the iterator, and implements
             * @c trySplit to permit limited parallelism.
             * </p>
             * <p>
             * Traversal of elements should be accomplished through the spliterator.
             * The behaviour of splitting and traversal is undefined if the iterator is
             * operated on after the spliterator is returned, or the initially reported
             * size is not equal to the actual number of elements in the source.
             * </p>
             * @param iterator The iterator for the source
             * @param size The number of elements in the source, to be reported as
             *        initial @c estimateSize
             * @param characteristics Characteristics of this spliterator's source or
             *        elements.  The characteristics @c SIZED and @c SUBSIZED
             *        are additionally reported unless @c CONCURRENT is supplied.
             * @return A spliterator from an iterator
             */
            static DoubleSpliterator& spliterator(DoubleIterator& iterator, glong size, gint characteristics);

            /**
             * Creates a @c DoubleSpliterator using a given
             * @c DoubleIterator as the source of elements, with no
             * initial size estimate.
             *
             * <p>
             * The spliterator is not <em>late-binding</em>, inherits
             * the <em>fail-fast</em> properties of the iterator, and implements
             * @c trySplit to permit limited parallelism.
             * </p>
             * <p>
             * Traversal of elements should be accomplished through the spliterator.
             * The behaviour of splitting and traversal is undefined if the iterator is
             * operated on after the spliterator is returned.
             * </p>
             * @param iterator The iterator for the source
             * @param characteristics Characteristics of this spliterator's source
             *        or elements (@c SIZED and @c SUBSIZED, if supplied, are
             *        ignored and are not reported.)
             * @return A spliterator from an iterator
             */
            static DoubleSpliterator& spliterator(DoubleIterator& iterator, gint characteristics);

            // Iterators from Spliterators

            /**
             * Creates an @c Iterator from a @c Spliterator.
             *
             * <p>
             * Traversal of elements should be accomplished through the iterator.
             * The behaviour of traversal is undefined if the spliterator is operated
             * after the iterator is returned.
             * </p>
             * @tparam T Type of elements
             * @param spliterator The spliterator
             * @return An iterator
             */
            template <class T, class X, ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Iterator<T>& iterator(Spliterator<X>& spliterator) {
                CORE_ALIAS(VALUE, typename Class<T>::Pointer);
                class Iterator final : public virtual util::Iterator<T> {
                    Spliterator<X>& spliterator;
                    gbool mutable isReady = false;
                    VALUE mutable nextValue = null;
                    function::Consumer<T&> consumer;

                public:
                    CORE_EXPLICIT Iterator(Spliterator<X>& spliterator)
                        : spliterator(spliterator) {
                        consumer = [this](T& t) -> void { nextValue = &t; };
                    }

                    gbool hasNext() const override {
                        if (!isReady) {
                            spliterator.tryAdvance(consumer);
                            isReady = true;
                            return true;
                        }
                        return false;
                    }

                    T& next() override {
                        if (!isReady && !hasNext())
                            NoSuchElementException().throws($ftrace());
                        isReady = false;
                        return *nextValue;
                    }

                    void forEach(const function::Consumer<T&>& action) override {
                        if (isReady) {
                            isReady = false;
                            action.accept(*nextValue);
                        }
                        spliterator.forEach(action);
                    }
                };

                try {
                    return UNSAFE::newInstance<Iterator>(spliterator);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            template <class T, class X, ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            static Iterator2<T>& iterator(Spliterator2<T>& spliterator) {
                CORE_ALIAS(VALUE, typename Class<T>::Pointer);
                class Iterator final : public virtual Iterator2<T> {
                    Spliterator2<X>& spliterator;
                    gbool mutable isReady = false;
                    VALUE mutable nextValue = null;
                    function::Consumer<T&> consumer;

                public:
                    CORE_EXPLICIT Iterator(Spliterator<X>& spliterator)
                        : spliterator(spliterator) {
                        consumer = [this](T& t) -> void { nextValue = &t; };
                    }

                    gbool hasNext() const override {
                        if (!isReady) {
                            spliterator.tryAdvance(consumer);
                            isReady = true;
                            return true;
                        }
                        return false;
                    }

                    T const& next() override {
                        if (!isReady && !hasNext())
                            NoSuchElementException().throws($ftrace());
                        isReady = false;
                        return *nextValue;
                    }

                    void forEach(const function::Consumer<T&>& action) override {
                        if (isReady) {
                            isReady = false;
                            action.accept(*nextValue);
                        }
                        spliterator.forEach(action);
                    }
                };

                try {
                    return UNSAFE::newInstance<Iterator>(spliterator);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates an @c IntIterator from a @c IntSpliterator.
             *
             * <p>
             * Traversal of elements should be accomplished through the iterator.
             * The behaviour of traversal is undefined if the spliterator is operated
             * after the iterator is returned.
             * </p>
             * @param spliterator The spliterator
             * @return An iterator
             */
            static IntIterator& iterator(IntSpliterator& spliterator);

            /**
             * Creates an @c LongIterator from a @c LongSpliterator.
             *
             * <p>
             * Traversal of elements should be accomplished through the iterator.
             * The behaviour of traversal is undefined if the spliterator is operated
             * after the iterator is returned.
             * </p>
             * @param spliterator The spliterator
             * @return An iterator
             */
            static LongIterator& iterator(LongSpliterator& spliterator);

            /**
             * Creates an @c DoubleIterator from a @c DoubleSpliterator.
             *
             * <p>
             * Traversal of elements should be accomplished through the iterator.
             * The behaviour of traversal is undefined if the spliterator is operated
             * after the iterator is returned.
             * </p>
             * @param spliterator The spliterator
             * @return An iterator
             */
            static DoubleIterator& iterator(DoubleSpliterator& spliterator);
        };
    } // util
} // core

#endif //CORE24_SPLITERATORS_H
