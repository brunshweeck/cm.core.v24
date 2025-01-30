//
// Created by brunshweeck on 15/11/24.
//

#ifndef CORE24_SPLITERATOR_H
#define CORE24_SPLITERATOR_H

#include <core/function/Consumer.h>
#include <core/function/DoubleConsumer.h>
#include <core/function/IntConsumer.h>
#include <core/function/LongConsumer.h>

namespace core {
    namespace util {
        /**
         * An object for traversing and partitioning elements of a source.  The source
         * of elements covered by a Spliterator could be, for example, an array, a
         * @b Collection, an IO channel, or a generator function.
         *
         * <p>
         * A Spliterator may traverse elements individually
         * (@b tryAdvance()) or sequentially in bulk (@b forEach()).
         * </p>
         * <p>
         * A Spliterator may also partition off some of its elements
         * (using @b trySplit) as another Spliterator, to be used in
         * possibly-parallel operations.  Operations using a Spliterator that
         * cannot split, or does so in a highly imbalanced or inefficient
         * manner, are unlikely to benefit from parallelism.  Traversal
         * and splitting exhaust elements; each Spliterator is useful for only a single
         * bulk computation.
         * </p>
         * <p>
         * A Spliterator also reports a set of @b characteristics() of its
         * structure, source, and elements from among @b ORDERED,
         * @b DISTINCT, @b SORTED, @b SIZED, @b NONNULL,
         * @b IMMUTABLE, @b CONCURRENT, and @b SUBSIZED. These may
         * be employed by Spliterator clients to control, specialize or simplify
         * computation.  For example, a Spliterator for a @b Collection would
         * report @c SIZED, a Spliterator for a @b Set would report
         * @c DISTINCT, and a Spliterator for a @b SortedSet would also
         * report @c SORTED.  Characteristics are reported as a simple unioned bit
         * set.
         *
         * Some characteristics additionally constrain method behavior; for example if
         * @c ORDERED, traversal methods must conform to their documented ordering.
         * New characteristics may be defined in the future, so implementors should not
         * assign meanings to unlisted values.
         * </p>
         * <p>
         * A Spliterator that does not report @c IMMUTABLE or
         * @c CONCURRENT is expected to have a documented policy concerning:
         * when the spliterator <em>binds</em> to the element source; and detection of
         * structural interference of the element source detected after binding.  A
         * <em>late-binding</em> Spliterator binds to the source of elements at the
         * point of first traversal, first split, or first query for estimated size,
         * rather than at the time the Spliterator is created.  A Spliterator that is
         * not <em>late-binding</em> binds to the source of elements at the point of
         * construction or first invocation of any method.  Modifications made to the
         * source prior to binding are reflected when the Spliterator is traversed.
         * After binding a Spliterator should, on a best-effort basis, throw
         * @b ConcurrentModificationException if structural interference is
         * detected.  Spliterators that do this are called <em>fail-fast</em>.  The
         * bulk traversal method (@b forEach()) of a
         * Spliterator may optimize traversal and check for structural interference
         * after all elements have been traversed, rather than checking per-element and
         * failing immediately.
         * </p>
         * <p>
         * Spliterators can provide an estimate of the number of remaining elements
         * via the @b estimateSize method.  Ideally, as reflected in characteristic
         * @b SIZED, this value corresponds exactly to the number of elements
         * that would be encountered in a successful traversal.  However, even when not
         * exactly known, an estimated value may still be useful to operations
         * being performed on the source, such as helping to determine whether it is
         * preferable to split further or traverse the remaining elements sequentially.
         * </p>
         * <p>
         * Despite their obvious utility in parallel algorithms, spliterators are not
         * expected to be thread-safe; instead, implementations of parallel algorithms
         * using spliterators should ensure that the spliterator is only used by one
         * thread at a time.  This is generally easy to attain via <em>serial
         * thread-confinement</em>, which often is a natural consequence of typical
         * parallel algorithms that work by recursive decomposition.  A thread calling
         * @b trySplit() may hand over the returned Spliterator to another thread,
         * which in turn may traverse or further split that Spliterator.  The behaviour
         * of splitting and traversal is undefined if two or more threads operate
         * concurrently on the same spliterator.  If the original thread hands a
         * spliterator off to another thread for processing, it is best if that handoff
         * occurs before any elements are consumed with
         * @b tryAdvance(), as certain guarantees (such as the accuracy of
         * @b estimateSize() for @c SIZED spliterators) are only valid before
         * traversal has begun.
         * </p>
         * <p>
         * Primitive subtype specializations of @c Spliterator are provided for
         * @b int, @b long, and @b double values.
         * The subtype default implementations of @b Spliterator::tryAdvance(function::Consumer)
         * and @b Spliterator::forEach(function::Consumer) box
         * primitive values to instances of their corresponding wrapper class.  Such
         * boxing may undermine any performance advantages gained by using the primitive
         * specializations.  To avoid boxing, the corresponding primitive-based methods
         * should be used.  For example, @b IntSpliterator::tryAdvance(function::IntConsumer)
         * and @b IntSpliterator::forEach(function::IntConsumer) should be used in preference to
         * @b IntSpliterator::tryAdvance(function::Consumer) and
         * @b IntSpliterator::forEach(function::Consumer).
         * Traversal of primitive values using boxing-based methods
         * @b tryAdvance() and @b forEach()
         * does not affect the order in which the values, transformed to boxed values,
         * are encountered.
         * </p>
         * @note
         * <p>
         * Spliterators, like @c Iterators, are for traversing the elements of
         * a source.  The @c Spliterator API was designed to support efficient
         * parallel traversal in addition to sequential traversal, by supporting
         * decomposition as well as single-element iteration.  In addition, the
         * protocol for accessing elements via a Spliterator is designed to impose
         * smaller per-element overhead than @c Iterator, and to avoid the inherent
         * race involved in having separate methods for @c hasNext() and
         * @c next().
         * </p>
         * <p>
         * For mutable sources, arbitrary and non-deterministic behavior may occur if
         * the source is structurally interfered with (elements added, replaced, or
         * removed) between the time that the Spliterator binds to its data source and
         * the end of traversal.  For example, such interference will produce arbitrary,
         * non-deterministic results when using the @c stream framework.
         * </p>
         * <p>
         * Structural interference of a source can be managed in the following ways
         * (in approximate order of decreasing desirability):
         *
         * @li The source cannot be structurally interfered with.
         * <br>For example, an instance of
         * @b concurrent::CopyOnWriteArrayList is an immutable source.
         * A Spliterator created from the source reports a characteristic of
         * @c IMMUTABLE.
         * @li The source manages concurrent modifications.
         * <br>For example, a key set of a @b concurrent::ConcurrentHashMap
         * is a concurrent source.  A Spliterator created from the source reports a
         * characteristic of @c CONCURRENT.
         * @li The mutable source provides a late-binding and fail-fast Spliterator.
         * <br>Late binding narrows the window during which interference can affect
         * the calculation; fail-fast detects, on a best-effort basis, that structural
         * interference has occurred after traversal has commenced and throws
         * @b ConcurrentModificationException.  For example, @b ArrayList,
         * and many other non-concurrent @c Collection classes, provide
         * a late-binding, fail-fast spliterator.
         * @li The mutable source provides a non-late-binding but fail-fast Spliterator.
         * <br>The source increases the likelihood of throwing
         * @c ConcurrentModificationException since the window of potential
         * interference is larger.
         * @li The mutable source provides a late-binding and non-fail-fast Spliterator.
         * <br>The source risks arbitrary, non-deterministic behavior after traversal
         * has commenced since interference is not detected.
         *
         * @li The mutable source provides a non-late-binding and non-fail-fast
         * Spliterator.
         * <br>The source increases the risk of arbitrary, non-deterministic behavior
         * since non-detected interference may occur after construction.
         * </p>
         *
         * <p>
         * <b>Example.</b> Here is a class (not a very useful one, except
         * for illustration) that maintains an array in which the actual data
         * are held in even locations, and unrelated tag data are held in odd
         * locations. Its Spliterator ignores the tags.
         *
         * @code
         * template <class T>
         * class TaggedArray {
         *  private:
         *    Array<> elements; // immutable after construction
         *
         *    TaggedArray(Array<T> data, Array<> tags) {
         *      int size = data.length();
         *      if (tags.length() != size) throw new IllegalArgumentException();
         *      this->elements = Array<>(2 * size);
         *      for (int i = 0, j = 0; i < size; ++i) {
         *        elements.set(j++, data[i]);
         *        elements.set(j++, tag[i]);
         *      }
         *    }
         *
         *  public:
         *    Spliterator<T> &spliterator() {
         *      return *new TaggedArraySpliterator<>(elements, 0, elements.length());
         *    }
         *
         *   class TaggedArraySpliterator<T> : public Spliterator<T> {
         *     private:
         *        Array<> array;
         *        int origin; // current index, advanced on split or traversal
         *        int fence; // one past the greatest index
         *     public:
         *       TaggedArraySpliterator(Array<> array, int origin, int fence) {
         *         this->array = array; this->origin = origin; this->fence = fence;
         *       }
         *
         *       void forEach(Consumer<T &> action) {
         *         for (; origin < fence; origin += 2)
         *           action.accept(CORE_XCAST(T, array[origin]));
         *       }
         *
         *       gbool tryAdvance(Consumer<T &> action) {
         *         if (origin < fence) {
         *           action.accept((T) array[origin]);
         *           origin += 2;
         *           return true;
         *         }
         *         else // cannot advance
         *           return false;
         *       }
         *
         *       Spliterator<T> &trySplit() {
         *         int lo = origin; // divide range in half
         *         int mid = ((lo + fence) >> 1) & ~1; // force midpoint to be even
         *         if (lo < mid) { // split out left half
         *           origin = mid; // reset this Spliterator's origin
         *           return new TaggedArraySpliterator<>(array, lo, mid);
         *         }
         *         else       // too small to split
         *           return *this;
         *       }
         *
         *       glong estimateSize() {
         *         return (long)((fence - origin) / 2);
         *       }
         *
         *       gint characteristics() {
         *         return ORDERED | SIZED | IMMUTABLE | SUBSIZED;
         *       }
         *   };
         * };
         * @endcode
         * </p>
         *
         * @tparam E the type of elements returned by this Spliterator
         *
         * @see Collection
         */
        template <class E>
        class Spliterator : public virtual Object {
        public:
            /**
             * If a remaining element exists: performs the given action on it,
             * returning @c true; else returns @c false.  If this
             * Spliterator is @b ORDERED the action is performed on the
             * next element in encounter order.  Exceptions thrown by the
             * action are relayed to the caller.
             * <p>
             * Subsequent behavior of a spliterator is unspecified if the action throws
             * an exception.
             * </p>
             * @param action The action whose operation is performed at-most once
             * @return @c false if no remaining elements existed
             * upon entry to this method, else @c true.
             */
            virtual gbool tryAdvance(function::Consumer<E&> const& action) = 0;

            /**
             * Performs the given action for each remaining element, sequentially in
             * the current thread, until all elements have been processed or the action
             * throws an exception.  If this Spliterator is @b ORDERED, actions
             * are performed in encounter order.  Exceptions thrown by the action
             * are relayed to the caller.
             * <p>
             * Subsequent behavior of a spliterator is unspecified if the action throws
             * an exception.
             * </p>
             * @note
             * The default implementation repeatedly invokes @b tryAdvance until
             * it returns @c false.  It should be overridden whenever possible.
             *
             * @param action The action
             */
            virtual void forEach(function::Consumer<E&> const& action) {
                do { $() } while (tryAdvance(action));
            }

            /**
             * If this spliterator can be partitioned, returns a Spliterator
             * covering elements, that will, upon return from this method, not
             * be covered by this Spliterator.
             *
             * <p>
             * If this Spliterator is @b ORDERED, the returned Spliterator
             * must cover a strict prefix of the elements.
             * </p>
             * <p>
             * Unless this Spliterator covers an infinite number of elements,
             * repeated calls to @c trySplit() must eventually return @c null.
             * Upon non-null return:
             *
             * @li the value reported for @c estimateSize() before splitting,
             * must, after splitting, be greater than or equal to @c estimateSize()
             * for this and the returned Spliterator; and
             * @li if this Spliterator is @c SUBSIZED, then @c estimateSize()
             * for this spliterator before splitting must be equal to the sum of
             * @c estimateSize() for this and the returned Spliterator after
             * splitting.
             * </p>
             *
             * <p>
             * This method may return @c null for any reason,
             * including emptiness, inability to split after traversal has
             * commenced, data structure constraints, and efficiency
             * considerations.
             * </p>
             * @note
             * An ideal @c trySplit method efficiently (without
             * traversal) divides its elements exactly in half, allowing
             * balanced parallel computation.  Many departures from this ideal
             * remain highly effective; for example, only approximately
             * splitting an approximately balanced tree, or for a tree in
             * which leaf nodes may contain either one or two elements,
             * failing to further split these nodes.  However, large
             * deviations in balance and/or overly inefficient @c trySplit
             * mechanics typically result in poor parallel performance.
             *
             * @return a @c Spliterator covering some portion of the
             * elements, or @c itself if this spliterator cannot be split
             */
            virtual Spliterator<E>& trySplit() = 0;

            virtual Spliterator<E> const& trySplit() const = 0;

            /**
             * Returns an estimate of the number of elements that would be
             * encountered by a @b forEach traversal, or returns
             * @b Long::MAX_VALUE if infinite, unknown, or too expensive to compute.
             *
             * <p>
             * If this Spliterator is @b SIZED and has not yet been partially
             * traversed or split, or this Spliterator is @b SUBSIZED and has
             * not yet been partially traversed, this estimate must be an accurate
             * count of elements that would be encountered by a complete traversal.
             * Otherwise, this estimate may be arbitrarily inaccurate, but must decrease
             * as specified across invocations of @b trySplit.
             * </p>
             * @note
             * Even an inexact estimate is often useful and inexpensive to compute.
             * For example, a sub-spliterator of an approximately balanced binary tree
             * may return a value that estimates the number of elements to be half of
             * that of its parent; if the root Spliterator does not maintain an
             * accurate count, it could estimate size to be the power of two
             * corresponding to its maximum depth.
             *
             * @return the estimated size, or @c Long::MAX_VALUE if infinite,
             *         unknown, or too expensive to compute.
             */
            virtual glong estimateSize() const = 0;

            /**
             * Convenience method that returns @b estimateSize() if this
             * Spliterator is @b SIZED, else @c -1.
             * @note
             * The default implementation returns the result of @c estimateSize()
             * if the Spliterator reports a characteristic of @c SIZED, and
             * @c -1 otherwise.
             *
             * @return the exact size, if known, else @c -1.
             */
            virtual glong getExactSizeIfKnown() const {
                return hasCharacteristics(SIZED) ? estimateSize() : -1;
            }

            /**
             * Returns a set of characteristics of this Spliterator and its
             * elements. The result is represented as ORed values from
             * @b ORDERED, @b DISTINCT, @b SORTED, @b SIZED,
             * @b NONNULL, @b IMMUTABLE, @b CONCURRENT,
             * @b SUBSIZED.  Repeated calls to @c characteristics() on
             * a given spliterator, prior to or in-between calls to @c trySplit,
             * should always return the same result.
             *
             * <p>
             * If a Spliterator reports an inconsistent set of
             * characteristics (either those returned from a single invocation
             * or across multiple invocations), no guarantees can be made
             * about any computation using this Spliterator.
             * </p>
             * @note The characteristics of a given spliterator before splitting
             * may differ from the characteristics after splitting.  For specific
             * examples see the characteristic values @b SIZED, @b SUBSIZED
             * and @b CONCURRENT.
             *
             * @return a representation of characteristics
             */
            virtual gint characteristics() const = 0;

            /**
             * Returns @c true if this Spliterator's
             * @b characteristics contain all the given characteristics.
             *
             * @note
             * The default implementation returns true if the corresponding bits
             * of the given characteristics are set.
             *
             * @param characteristics the characteristics to check for
             * @return @c true if all the specified characteristics are present,
             * else @c false
             */
            virtual gbool hasCharacteristics(gint characteristics) const {
                return (this->characteristics() & characteristics) == characteristics;
            }

            /**
             * If this Spliterator's source is @b SORTED by a @b Comparator,
             * returns that @c Comparator. If the source is @c SORTED in
             * <em>natural order</em>, returns @c null.  Otherwise,
             * if the source is not @c SORTED, throws @b IllegalStateException.
             *
             * @note
             * The default implementation always throws @b IllegalStateException.
             *
             * @return a Comparator, or @c null if the elements are sorted in the
             * natural order.
             * @throws IllegalStateException if the spliterator does not report
             *         a characteristic of @c SORTED.
             */
            virtual Comparator<E> const& comparator() const {
                IllegalStateException().throws($ftrace());
            }

            enum Characteristic {
                /**
                 * Characteristic value signifying that an encounter order is defined for
                 * elements. If so, this Spliterator guarantees that method
                 * @b trySplit splits a strict prefix of elements, that method
                 * @b tryAdvance steps by one element in prefix order, and that
                 * @b forEach performs actions in encounter order.
                 *
                 * <p>
                 * A @b Collection has an encounter order if the corresponding
                 * @b Collection::iterator documents an order. If so, the encounter
                 * order is the same as the documented order. Otherwise, a collection does
                 * not have an encounter order.
                 * </p>
                 * @note Encounter order is guaranteed to be ascending index order for
                 * any @b List. But no order is guaranteed for hash-based collections
                 * such as @b HashSet. Clients of a Spliterator that reports
                 * @c ORDERED are expected to preserve ordering constraints in
                 * non-commutative parallel computations.
                 */
                ORDERED = 0x00000010,

                /**
                 * Characteristic value signifying that, for each pair of
                 * encountered elements @code x, y @endcode , @c !x.equals(y). This
                 * applies for example, to a Spliterator based on a @b Set.
                 */
                DISTINCT = 0x00000001,

                /**
                 * Characteristic value signifying that encounter order follows a defined
                 * sort order. If so, method @b comparator() returns the associated
                 * Comparator, or @c null if all elements are @b Comparable and
                 * are sorted by their natural ordering.
                 *
                 * <p>
                 * A Spliterator that reports @c SORTED must also report
                 * @c ORDERED.
                 * </p>
                 * @note The spliterators for @c Collection classes that
                 * implement @b NavigableSet or @b SortedSet report @c SORTED.
                 */
                SORTED = 0x00000004,

                /**
                 * Characteristic value signifying that the value returned from
                 * @c estimateSize() prior to traversal or splitting represents a
                 * finite size that, in the absence of structural source modification,
                 * represents an exact count of the number of elements that would be
                 * encountered by a complete traversal.
                 *
                 * @note Most Spliterators for Collections, that cover all elements of a
                 * @c Collection report this characteristic. Sub-spliterators, such as
                 * those for @b HashSet, that cover a sub-set of elements and
                 * approximate their reported size do not.
                 */
                SIZED = 0x00000040,

                /**
                 * Characteristic value signifying that the source guarantees that
                 * encountered elements will not be @c null. (This applies,
                 * for example, to most concurrent collections, queues, and maps.)
                 */
                NONNULL = 0x00000100,

                /**
                 * Characteristic value signifying that the element source cannot be
                 * structurally modified; that is, elements cannot be added, replaced, or
                 * removed, so such changes cannot occur during traversal. A Spliterator
                 * that does not report @c IMMUTABLE or @c CONCURRENT is expected
                 * to have a documented policy (for example throwing
                 * @b ConcurrentModificationException) concerning structural
                 * interference detected during traversal.
                 */
                IMMUTABLE = 0x00000400,

                /**
                 * Characteristic value signifying that the element source may be safely
                 * concurrently modified (allowing additions, replacements, and/or removals)
                 * by multiple threads without external synchronization. If so, the
                 * Spliterator is expected to have a documented policy concerning the impact
                 * of modifications during traversal.
                 *
                 * <p>A top-level Spliterator should not report both @c CONCURRENT and
                 * @c SIZED, since the finite size, if known, may change if the source
                 * is concurrently modified during traversal. Such a Spliterator is
                 * inconsistent and no guarantees can be made about any computation using
                 * that Spliterator. Sub-spliterators may report @c SIZED if the
                 * sub-split size is known and additions or removals to the source are not
                 * reflected when traversing.
                 *
                 * <p>A top-level Spliterator should not report both @c CONCURRENT and
                 * @c IMMUTABLE, since they are mutually exclusive. Such a Spliterator
                 * is inconsistent and no guarantees can be made about any computation using
                 * that Spliterator. Sub-spliterators may report @c IMMUTABLE if
                 * additions or removals to the source are not reflected when traversing.
                 *
                 * @note Most concurrent collections maintain a consistency policy
                 * guaranteeing accuracy with respect to elements present at the point of
                 * Spliterator construction, but possibly not reflecting subsequent
                 * additions or removals.
                 */
                CONCURRENT = 0x00001000,

                /**
                 * Characteristic value signifying that all Spliterators resulting from
                 * @c trySplit() will be both @b SIZED and @b SUBSIZED.
                 * (This means that all child Spliterators, whether direct or indirect, will
                 * be @c SIZED.)
                 *
                 * <p>
                 * A Spliterator that does not report @c SIZED as required by
                 * @c SUBSIZED is inconsistent and no guarantees can be made about any
                 * computation using that Spliterator.
                 * </p>
                 * @note Some spliterators, such as the top-level spliterator for an
                 * approximately balanced binary tree, will report @c SIZED but not
                 * @c SUBSIZED, since it is common to know the size of the entire tree
                 * but not the exact sizes of subtrees.
                 */
                SUBSIZED = 0x00004000
            };

            /**
             * A Spliterator specialized for primitive values.
             *
             * @tparam T the type of elements returned by this Spliterator.  The
             * type must be a wrapper type for a primitive type, such as @c Integer
             * for the primitive @c int type.
             * @tparam T_CONS the type of primitive consumer.  The type must be a
             * primitive specialization of @b function::Consumer for
             * @c T, such as @b function::IntConsumer for @c Integer.
             * @tparam T_SPLITR the type of primitive Spliterator.  The type must be
             * a primitive specialization of Spliterator for @c T, such as
             * @b IntSpliterator for @c Integer.
             *
             * @see IntSpliterator
             * @see LongSpliterator
             * @see DoubleSpliterator
             */
            template <class T, class T_CONS, class T_SPLITR>
            class OfPrimitive;
        };

        template <class E>
        class Spliterator2 : public virtual Object {
        public:
            /**
             * If a remaining element exists: performs the given action on it,
             * returning @c true; else returns @c false.  If this
             * Spliterator is @b ORDERED the action is performed on the
             * next element in encounter order.  Exceptions thrown by the
             * action are relayed to the caller.
             * <p>
             * Subsequent behavior of a spliterator is unspecified if the action throws
             * an exception.
             * </p>
             * @param action The action whose operation is performed at-most once
             * @return @c false if no remaining elements existed
             * upon entry to this method, else @c true.
             */
            virtual gbool tryAdvance(function::Consumer<E> const& action) = 0;

            /**
             * Performs the given action for each remaining element, sequentially in
             * the current thread, until all elements have been processed or the action
             * throws an exception.  If this Spliterator is @b ORDERED, actions
             * are performed in encounter order.  Exceptions thrown by the action
             * are relayed to the caller.
             * <p>
             * Subsequent behavior of a spliterator is unspecified if the action throws
             * an exception.
             * </p>
             * @note
             * The default implementation repeatedly invokes @b tryAdvance until
             * it returns @c false.  It should be overridden whenever possible.
             *
             * @param action The action
             */
            virtual void forEach(function::Consumer<E> const& action) {
                do { $() } while (tryAdvance(action));
            }

            /**
             * If this spliterator can be partitioned, returns a Spliterator
             * covering elements, that will, upon return from this method, not
             * be covered by this Spliterator.
             *
             * <p>
             * If this Spliterator is @b ORDERED, the returned Spliterator
             * must cover a strict prefix of the elements.
             * </p>
             * <p>
             * Unless this Spliterator covers an infinite number of elements,
             * repeated calls to @c trySplit() must eventually return @c null.
             * Upon non-null return:
             *
             * @li the value reported for @c estimateSize() before splitting,
             * must, after splitting, be greater than or equal to @c estimateSize()
             * for this and the returned Spliterator; and
             * @li if this Spliterator is @c SUBSIZED, then @c estimateSize()
             * for this spliterator before splitting must be equal to the sum of
             * @c estimateSize() for this and the returned Spliterator after
             * splitting.
             * </p>
             *
             * <p>
             * This method may return @c null for any reason,
             * including emptiness, inability to split after traversal has
             * commenced, data structure constraints, and efficiency
             * considerations.
             * </p>
             * @note
             * An ideal @c trySplit method efficiently (without
             * traversal) divides its elements exactly in half, allowing
             * balanced parallel computation.  Many departures from this ideal
             * remain highly effective; for example, only approximately
             * splitting an approximately balanced tree, or for a tree in
             * which leaf nodes may contain either one or two elements,
             * failing to further split these nodes.  However, large
             * deviations in balance and/or overly inefficient @c trySplit
             * mechanics typically result in poor parallel performance.
             *
             * @return a @c Spliterator covering some portion of the
             * elements, or @c itself if this spliterator cannot be split
             */
            virtual Spliterator2<E>& trySplit() = 0;

            virtual Spliterator2<E> const& trySplit() const = 0;

            /**
             * Returns an estimate of the number of elements that would be
             * encountered by a @b forEach traversal, or returns
             * @b Long::MAX_VALUE if infinite, unknown, or too expensive to compute.
             *
             * <p>
             * If this Spliterator is @b SIZED and has not yet been partially
             * traversed or split, or this Spliterator is @b SUBSIZED and has
             * not yet been partially traversed, this estimate must be an accurate
             * count of elements that would be encountered by a complete traversal.
             * Otherwise, this estimate may be arbitrarily inaccurate, but must decrease
             * as specified across invocations of @b trySplit.
             * </p>
             * @note
             * Even an inexact estimate is often useful and inexpensive to compute.
             * For example, a sub-spliterator of an approximately balanced binary tree
             * may return a value that estimates the number of elements to be half of
             * that of its parent; if the root Spliterator does not maintain an
             * accurate count, it could estimate size to be the power of two
             * corresponding to its maximum depth.
             *
             * @return the estimated size, or @c Long::MAX_VALUE if infinite,
             *         unknown, or too expensive to compute.
             */
            virtual glong estimateSize() const = 0;

            /**
             * Convenience method that returns @b estimateSize() if this
             * Spliterator is @b SIZED, else @c -1.
             * @note
             * The default implementation returns the result of @c estimateSize()
             * if the Spliterator reports a characteristic of @c SIZED, and
             * @c -1 otherwise.
             *
             * @return the exact size, if known, else @c -1.
             */
            virtual glong getExactSizeIfKnown() const {
                return hasCharacteristics(SIZED) ? estimateSize() : -1;
            }

            /**
             * Returns a set of characteristics of this Spliterator and its
             * elements. The result is represented as ORed values from
             * @b ORDERED, @b DISTINCT, @b SORTED, @b SIZED,
             * @b NONNULL, @b IMMUTABLE, @b CONCURRENT,
             * @b SUBSIZED.  Repeated calls to @c characteristics() on
             * a given spliterator, prior to or in-between calls to @c trySplit,
             * should always return the same result.
             *
             * <p>
             * If a Spliterator reports an inconsistent set of
             * characteristics (either those returned from a single invocation
             * or across multiple invocations), no guarantees can be made
             * about any computation using this Spliterator.
             * </p>
             * @note The characteristics of a given spliterator before splitting
             * may differ from the characteristics after splitting.  For specific
             * examples see the characteristic values @b SIZED, @b SUBSIZED
             * and @b CONCURRENT.
             *
             * @return a representation of characteristics
             */
            virtual gint characteristics() const = 0;

            /**
             * Returns @c true if this Spliterator's
             * @b characteristics contain all the given characteristics.
             *
             * @note
             * The default implementation returns true if the corresponding bits
             * of the given characteristics are set.
             *
             * @param characteristics the characteristics to check for
             * @return @c true if all the specified characteristics are present,
             * else @c false
             */
            virtual gbool hasCharacteristics(gint characteristics) const {
                return (this->characteristics() & characteristics) == characteristics;
            }

            /**
             * If this Spliterator's source is @b SORTED by a @b Comparator,
             * returns that @c Comparator. If the source is @c SORTED in
             * <em>natural order</em>, returns @c null.  Otherwise,
             * if the source is not @c SORTED, throws @b IllegalStateException.
             *
             * @note
             * The default implementation always throws @b IllegalStateException.
             *
             * @return a Comparator, or @c null if the elements are sorted in the
             * natural order.
             * @throws IllegalStateException if the spliterator does not report
             *         a characteristic of @c SORTED.
             */
            virtual Comparator<E> const& comparator() const {
                IllegalStateException().throws($ftrace());
            }

            enum Characteristic {
                /**
                 * Characteristic value signifying that an encounter order is defined for
                 * elements. If so, this Spliterator guarantees that method
                 * @b trySplit splits a strict prefix of elements, that method
                 * @b tryAdvance steps by one element in prefix order, and that
                 * @b forEach performs actions in encounter order.
                 *
                 * <p>
                 * A @b Collection has an encounter order if the corresponding
                 * @b Collection::iterator documents an order. If so, the encounter
                 * order is the same as the documented order. Otherwise, a collection does
                 * not have an encounter order.
                 * </p>
                 * @note Encounter order is guaranteed to be ascending index order for
                 * any @b List. But no order is guaranteed for hash-based collections
                 * such as @b HashSet. Clients of a Spliterator that reports
                 * @c ORDERED are expected to preserve ordering constraints in
                 * non-commutative parallel computations.
                 */
                ORDERED = Spliterator<E>::ORDERED,

                /**
                 * Characteristic value signifying that, for each pair of
                 * encountered elements @code x, y @endcode , @c !x.equals(y). This
                 * applies for example, to a Spliterator based on a @b Set.
                 */
                DISTINCT = Spliterator<E>::DISTINCT,

                /**
                 * Characteristic value signifying that encounter order follows a defined
                 * sort order. If so, method @b comparator() returns the associated
                 * Comparator, or @c null if all elements are @b Comparable and
                 * are sorted by their natural ordering.
                 *
                 * <p>
                 * A Spliterator that reports @c SORTED must also report
                 * @c ORDERED.
                 * </p>
                 * @note The spliterators for @c Collection classes that
                 * implement @b NavigableSet or @b SortedSet report @c SORTED.
                 */
                SORTED = Spliterator<E>::SORTED,

                /**
                 * Characteristic value signifying that the value returned from
                 * @c estimateSize() prior to traversal or splitting represents a
                 * finite size that, in the absence of structural source modification,
                 * represents an exact count of the number of elements that would be
                 * encountered by a complete traversal.
                 *
                 * @note Most Spliterators for Collections, that cover all elements of a
                 * @c Collection report this characteristic. Sub-spliterators, such as
                 * those for @b HashSet, that cover a sub-set of elements and
                 * approximate their reported size do not.
                 */
                SIZED = Spliterator<E>::SIZED,

                /**
                 * Characteristic value signifying that the source guarantees that
                 * encountered elements will not be @c null. (This applies,
                 * for example, to most concurrent collections, queues, and maps.)
                 */
                NONNULL = Spliterator<E>::NONNULL,

                /**
                 * Characteristic value signifying that the element source cannot be
                 * structurally modified; that is, elements cannot be added, replaced, or
                 * removed, so such changes cannot occur during traversal. A Spliterator
                 * that does not report @c IMMUTABLE or @c CONCURRENT is expected
                 * to have a documented policy (for example throwing
                 * @b ConcurrentModificationException) concerning structural
                 * interference detected during traversal.
                 */
                IMMUTABLE = Spliterator<E>::IMMUTABLE,

                /**
                 * Characteristic value signifying that the element source may be safely
                 * concurrently modified (allowing additions, replacements, and/or removals)
                 * by multiple threads without external synchronization. If so, the
                 * Spliterator is expected to have a documented policy concerning the impact
                 * of modifications during traversal.
                 *
                 * <p>
                 * A top-level Spliterator should not report both @c CONCURRENT and
                 * @c SIZED, since the finite size, if known, may change if the source
                 * is concurrently modified during traversal. Such a Spliterator is
                 * inconsistent and no guarantees can be made about any computation using
                 * that Spliterator. Sub-spliterators may report @c SIZED if the
                 * sub-split size is known and additions or removals to the source are not
                 * reflected when traversing.
                 * </p>
                 * <p>
                 * A top-level Spliterator should not report both @c CONCURRENT and
                 * @c IMMUTABLE, since they are mutually exclusive. Such a Spliterator
                 * is inconsistent and no guarantees can be made about any computation using
                 * that Spliterator. Sub-spliterators may report @c IMMUTABLE if
                 * additions or removals to the source are not reflected when traversing.
                 * </p>
                 * @note Most concurrent collections maintain a consistency policy
                 * guaranteeing accuracy with respect to elements present at the point of
                 * Spliterator construction, but possibly not reflecting subsequent
                 * additions or removals.
                 */
                CONCURRENT = Spliterator<E>::CONCURRENT,

                /**
                 * Characteristic value signifying that all Spliterators resulting from
                 * @c trySplit() will be both @b SIZED and @b SUBSIZED.
                 * (This means that all child Spliterators, whether direct or indirect, will
                 * be @c SIZED.)
                 *
                 * <p>
                 * A Spliterator that does not report @c SIZED as required by
                 * @c SUBSIZED is inconsistent and no guarantees can be made about any
                 * computation using that Spliterator.
                 * </p>
                 * @note Some spliterators, such as the top-level spliterator for an
                 * approximately balanced binary tree, will report @c SIZED but not
                 * @c SUBSIZED, since it is common to know the size of the entire tree
                 * but not the exact sizes of subtrees.
                 */
                SUBSIZED = Spliterator<E>::SUBSIZED
            };
        };

        template <class E>
        template <class T, class T_CONS, class T_SPLITR>
        class Spliterator<E>::OfPrimitive : public virtual Spliterator<T> {
        public:
            // T_SPLITR &trySplit() override = 0;
            //
            // T_SPLITR const &trySplit() const override = 0;

            /**
             * If a remaining element exists, performs the given action on it,
             * returning @c true; else returns @c false.  If this
             * Spliterator is @b ORDERED the action is performed on the
             * next element in encounter order.  Exceptions thrown by the
             * action are relayed to the caller.
             * <p>
             * Subsequent behavior of a spliterator is unspecified if the action throws
             * an exception.
             *
             * @param action The action
             * @return @c false if no remaining elements existed
             * upon entry to this method, else @c true.
             */
            virtual gbool tryAdvance(const T_CONS& action) = 0;

            /**
             * Performs the given action for each remaining element, sequentially in
             * the current thread, until all elements have been processed or the
             * action throws an exception.  If this Spliterator is @b ORDERED,
             * actions are performed in encounter order.  Exceptions thrown by the
             * action are relayed to the caller.
             * <p>
             * Subsequent behavior of a spliterator is unspecified if the action throws
             * an exception.
             * </p>
             * @note
             * The default implementation repeatedly invokes @b tryAdvance
             * until it returns @c false.  It should be overridden whenever
             * possible.
             *
             * @param action The action
             */
            virtual void forEach(const T_CONS& action) {
                do { $() } while (tryAdvance(action));
            }
        };

        /**
         * A Spliterator specialized for @c int values.
         */
        class IntSpliterator :
                public virtual Spliterator<>::OfPrimitive<Integer, function::IntConsumer, IntSpliterator> {
        public:
            IntSpliterator& trySplit() override = 0;

            const IntSpliterator& trySplit() const override = 0;

            gbool tryAdvance(const function::IntConsumer& action) override = 0;

            void forEach(const function::IntConsumer& action) override = 0;

            /**
             * @note
             * If the action is an instance of @c IntConsumer then it is cast
             * to @c IntConsumer and passed to
             * @b tryAdvance(function::IntConsumer); otherwise
             * the action is adapted to an instance of @c IntConsumer, by
             * boxing the argument of @c IntConsumer, and then passed to
             * @b tryAdvance(function::IntConsumer).
             */
            gbool tryAdvance(const function::Consumer<Integer&>& action) override {
                return tryAdvance((function::IntConsumer) [action](Integer i) -> void { action.accept(i); });
            }

            /**
             * @note
             * If the action is an instance of @c IntConsumer then it is cast
             * to @c IntConsumer and passed to
             * @b forEach(function::IntConsumer); otherwise
             * the action is adapted to an instance of @c IntConsumer, by
             * boxing the argument of @c IntConsumer, and then passed to
             * @b forEach(function::IntConsumer).
             */
            void forEach(const function::Consumer<Integer&>& action) override {
                return forEach((function::IntConsumer) [action](Integer i) -> void { action.accept(i); });
            }
        };

        /**
         * A Spliterator specialized for @c long values.
         */
        class LongSpliterator :
                public virtual Spliterator<>::OfPrimitive<Long, function::LongConsumer, LongSpliterator> {
        public:
            LongSpliterator& trySplit() override = 0;

            const LongSpliterator& trySplit() const override = 0;

            gbool tryAdvance(const function::LongConsumer& action) override = 0;

            void forEach(const function::LongConsumer& action) override = 0;

            /**
             * @note
             * If the action is an instance of @c LongConsumer then it is cast
             * to @c LongConsumer and passed to
             * @b tryAdvance(function::LongConsumer); otherwise
             * the action is adapted to an instance of @c LongConsumer, by
             * boxing the argument of @c LongConsumer, and then passed to
             * @b tryAdvance(function::LongConsumer).
             */
            gbool tryAdvance(const function::Consumer<Long&>& action) override {
                return tryAdvance((function::LongConsumer) [action](Long i) -> void { action.accept(i); });
            }

            /**
             * @note
             * If the action is an instance of @c LongConsumer then it is cast
             * to @c LongConsumer and passed to
             * @b forEach(function::LongConsumer); otherwise
             * the action is adapted to an instance of @c LongConsumer, by
             * boxing the argument of @c LongConsumer, and then passed to
             * @b forEach(function::LongConsumer).
             */
            void forEach(const function::Consumer<Long&>& action) override {
                return forEach((function::LongConsumer) [action](Long i) -> void { action.accept(i); });
            }
        };

        /**
         * A Spliterator specialized for @c double values.
         */
        class DoubleSpliterator :
                public virtual Spliterator<>::OfPrimitive<Double, function::DoubleConsumer, DoubleSpliterator> {
        public:
            DoubleSpliterator& trySplit() override = 0;

            const DoubleSpliterator& trySplit() const override = 0;

            gbool tryAdvance(const function::DoubleConsumer& action) override = 0;

            void forEach(const function::DoubleConsumer& action) override = 0;

            /**
             * @note
             * If the action is an instance of @c DoubleConsumer then it is cast
             * to @c DoubleConsumer and passed to
             * @b tryAdvance(function::DoubleConsumer); otherwise
             * the action is adapted to an instance of @c DoubleConsumer, by
             * boxing the argument of @c DoubleConsumer, and then passed to
             * @b tryAdvance(function::DoubleConsumer).
             */
            gbool tryAdvance(const function::Consumer<Double&>& action) override {
                return tryAdvance((function::DoubleConsumer) [action](Double i) -> void { action.accept(i); });
            }

            /**
             * @note
             * If the action is an instance of @c DoubleConsumer then it is cast
             * to @c DoubleConsumer and passed to
             * @b forEach(function::DoubleConsumer); otherwise
             * the action is adapted to an instance of @c DoubleConsumer, by
             * boxing the argument of @c DoubleConsumer, and then passed to
             * @b forEach(function::DoubleConsumer).
             */
            void forEach(const function::Consumer<Double&>& action) override {
                return forEach((function::DoubleConsumer) [action](Double i) -> void { action.accept(i); });
            }
        };
    } // util
} // core

#endif //CORE24_SPLITERATOR_H
