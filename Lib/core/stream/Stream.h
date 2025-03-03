//
// Created by brunshweeck on 19/11/24.
//

#ifndef CORE24_STREAM_H
#define CORE24_STREAM_H

#include <core/lang/AutoCloseable.h>
#include <core/lang/Class.h>

namespace core {
  namespace stream {
    template <class T>
    class Stream : public virtual AutoCloseable {
    public:
      /**
       * Returns an iterator for the elements of this stream.
       *
       * <p>
       * This is a terminal operation.
       * </p>
       * @apiNote
       * This operation is provided as an "escape hatch" to enable
       * arbitrary client-controlled pipeline traversals in the event that the
       * existing operations are not sufficient to the task.
       *
       * @return the element iterator for this stream
       */
      virtual util::Iterator<T>& iterator() = 0;

      virtual util::Iterator2<T>& iterator() const = 0;

      /**
       * Returns a spliterator for the elements of this stream.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal
       * operation</a>.
       *
       * @apiNote
       * This operation is provided as an "escape hatch" to enable
       * arbitrary client-controlled pipeline traversals in the event that the
       * existing operations are not sufficient to the task.
       *
       * <p>
       * The returned spliterator should report the set of characteristics derived
       * from the stream pipeline (namely the characteristics derived from the
       * stream source spliterator and the intermediate operations).
       * Implementations may report a sub-set of those characteristics.  For
       * example, it may be too expensive to compute the entire set for some or
       * all possible stream pipelines.
       *
       * @return the element spliterator for this stream
       */
      virtual util::Spliterator<T>& spliterator() = 0;

      virtual util::Spliterator2<T>& spliterator() const = 0;

      /**
       * Returns whether this stream, if a terminal operation were to be executed,
       * would execute in parallel.  Calling this method after invoking an
       * terminal stream operation method may yield unpredictable results.
       *
       * @return {@code true} if this stream would execute in parallel if executed
       */
      virtual gbool isParallel() const = 0;

      /**
       * Returns an equivalent stream that is sequential.  May return
       * itself, either because the stream was already sequential, or because
       * the underlying stream state was modified to be sequential.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @return a sequential stream
       */
      virtual Stream& sequential() = 0;

      virtual Stream const& sequential() const = 0;

      /**
       * Returns an equivalent stream that is parallel.  May return
       * itself, either because the stream was already parallel, or because
       * the underlying stream state was modified to be parallel.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @return a parallel stream
       */
      virtual Stream& parallel() = 0;

      virtual Stream const& parallel() const = 0;

      /**
       * Returns an equivalent stream that is
       * <a href="package-summary.html#Ordering">unordered</a>.  May return
       * itself, either because the stream was already unordered, or because
       * the underlying stream state was modified to be unordered.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @return an unordered stream
       */
      virtual Stream& unordered() = 0;

      virtual Stream const& unordered() const = 0;

      /**
       * Returns an equivalent stream with an additional close handler.  Close
       * handlers are run when the {@link #close()} method
       * is called on the stream, and are executed in the order they were
       * added.  All close handlers are run, even if earlier close handlers throw
       * exceptions.  If any close handler throws an exception, the first
       * exception thrown will be relayed to the caller of {@code close()}, with
       * any remaining exceptions added to that exception as suppressed exceptions
       * (unless one of the remaining exceptions is the same exception as the
       * first exception, since an exception cannot suppress itself.)  May
       * return itself.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @param closeHandler A task to execute when the stream is closed
       * @return a stream with a handler that is run if the stream is closed
       */
      virtual Stream& onClose(Runnable const& closeHandler) = 0;

      /**
       * Closes this stream, causing all close handlers for this stream pipeline
       * to be called.
       *
       * @see AutoCloseable#close()
       */
      void close() override = 0;

      /**
       * Returns a stream consisting of the elements of this stream that match
       * the given predicate.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                  <a href="package-summary.html#Statelessness">stateless</a>
       *                  predicate to apply to each element to determine if it
       *                  should be included
       * @return the new stream
       */
      virtual Stream& filter(function::Predicate<T&> const& predicate) = 0;

      virtual Stream const& filter(function::Predicate<T> const& predicate) const = 0;

      /**
       * Returns a stream consisting of the results of applying the given
       * function to the elements of this stream.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @param <R> The element type of the new stream
       * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *               <a href="package-summary.html#Statelessness">stateless</a>
       *               function to apply to each element
       * @return the new stream
       */
      virtual Stream<>& map(function::Function<T&, Object> const& mapper) = 0;

      virtual Stream<> const& map(function::Function<T, Object> const& mapper) const = 0;

      template <class R, class X = T,
                ClassOf(1)::OnlyIf<Class<T>::template isSuper<X>()> CaptureX = 1>
      Stream<R>& map(function::Function<T&, R> const& mapper);

      template <class R, class X = T,
                ClassOf(1)::OnlyIf<Class<T>::template isSuper<X>()> CaptureX = 1>
      Stream<R> const& map(function::Function<T, R> const& mapper) const;

      /**
       * Returns an {@code IntStream} consisting of the results of applying the
       * given function to the elements of this stream.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">
       *     intermediate operation</a>.
       *
       * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *               <a href="package-summary.html#Statelessness">stateless</a>
       *               function to apply to each element
       * @return the new stream
       */
      virtual IntStream& mapToInt(function::ToIntFunction<T&> const& mapper) = 0;

      virtual IntStream const& mapToInt(function::ToIntFunction<T> const& mapper) const = 0;

      /**
       * Returns a {@code LongStream} consisting of the results of applying the
       * given function to the elements of this stream.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *               <a href="package-summary.html#Statelessness">stateless</a>
       *               function to apply to each element
       * @return the new stream
       */
      virtual LongStream& mapToLong(function::ToLongFunction<T&> const& mapper) = 0;

      virtual LongStream const& mapToLong(function::ToLongFunction<T> const& mapper) const = 0;

      /**
       * Returns a {@code DoubleStream} consisting of the results of applying the
       * given function to the elements of this stream.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *               <a href="package-summary.html#Statelessness">stateless</a>
       *               function to apply to each element
       * @return the new stream
       */
      virtual DoubleStream& mapToDouble(function::ToDoubleFunction<T&> const& mapper) = 0;

      virtual DoubleStream const& mapToDouble(function::ToDoubleFunction<T> const& mapper) const = 0;

      /**
       * Returns a stream consisting of the results of replacing each element of
       * this stream with the contents of a mapped stream produced by applying
       * the provided mapping function to each element.  Each mapped stream is
       * {@link java.util.stream.BaseStream#close() closed} after its contents
       * have been placed into this stream.  (If a mapped stream is {@code null}
       * an empty stream is used, instead.)
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @apiNote
       * The {@code flatMap()} operation has the effect of applying a one-to-many
       * transformation to the elements of the stream, and then flattening the
       * resulting elements into a new stream.
       *
       * <p><b>Examples.</b>
       *
       * <p>If {@code orders} is a stream of purchase orders, and each purchase
       * order contains a collection of line items, then the following produces a
       * stream containing all the line items in all the orders:
       * <pre>{@code
       *     orders.flatMap(order -> order.getLineItems().stream())...
       * }</pre>
       *
       * <p>If {@code path} is the path to a file, then the following produces a
       * stream of the {@code words} contained in that file:
       * <pre>{@code
       *     Stream<String> lines = Files.lines(path, StandardCharsets.UTF_8);
       *     Stream<String> words = lines.flatMap(line -> Stream.of(line.split(" +")));
       * }</pre>
       * The {@code mapper} function passed to {@code flatMap} splits a line,
       * using a simple regular expression, into an array of words, and then
       * creates a stream of words from that array.
       *
       * @param <R> The element type of the new stream
       * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *               <a href="package-summary.html#Statelessness">stateless</a>
       *               function to apply to each element which produces a stream
       *               of new values
       * @return the new stream
       * @see #mapMulti mapMulti
       */
      template <class R, class X = T, class S = Stream<R>,
                ClassOf(1)::OnlyIf<Class<T>::template isSuper<X>()> CaptureX = 1,
                ClassOf(1)::OnlyIf<Class<Stream<R>>::template isSuper<S>()> CaptureS = 1>
      Stream<R>& flatMap(function::Function<X&, S> const& mapper);

      template <class R, class X = T, class S = Stream<R>,
                ClassOf(1)::OnlyIf<Class<T>::template isSuper<X>()> CaptureX = 1,
                ClassOf(1)::OnlyIf<Class<Stream<R>>::template isSuper<S>()> CaptureS = 1>
      Stream<R>& flatMap(function::Function<X, S> const& mapper);

      virtual Stream<>& flatMap(function::Function<T&, Stream<>> const& mapper) = 0;

      virtual Stream<> const& flatMap(function::Function<T&, Stream<>> const& mapper) const = 0;

      /**
       * Returns an {@code IntStream} consisting of the results of replacing each
       * element of this stream with the contents of a mapped stream produced by
       * applying the provided mapping function to each element.  Each mapped
       * stream is {@link java.util.stream.BaseStream#close() closed} after its
       * contents have been placed into this stream.  (If a mapped stream is
       * {@code null} an empty stream is used, instead.)
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *               <a href="package-summary.html#Statelessness">stateless</a>
       *               function to apply to each element which produces a stream
       *               of new values
       * @return the new stream
       * @see #flatMap flatMap
       */
      virtual IntStream& flatMapToInt(function::Function<T&, IntStream> const& mapper) = 0;

      virtual IntStream const& flatMapToInt(function::Function<T, IntStream> const& mapper) const = 0;

      /**
       * Returns an {@code LongStream} consisting of the results of replacing each
       * element of this stream with the contents of a mapped stream produced by
       * applying the provided mapping function to each element.  Each mapped
       * stream is {@link java.util.stream.BaseStream#close() closed} after its
       * contents have been placed into this stream.  (If a mapped stream is
       * {@code null} an empty stream is used, instead.)
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *               <a href="package-summary.html#Statelessness">stateless</a>
       *               function to apply to each element which produces a stream
       *               of new values
       * @return the new stream
       * @see #flatMap flatMap
       */
      virtual LongStream& flatMapToLong(function::Function<T&, LongStream> const& mapper) = 0;

      virtual LongStream const& flatMapToLong(function::Function<T, LongStream> const& mapper) const = 0;

      /**
       * Returns an {@code DoubleStream} consisting of the results of replacing
       * each element of this stream with the contents of a mapped stream produced
       * by applying the provided mapping function to each element.  Each mapped
       * stream is {@link java.util.stream.BaseStream#close() closed} after its
       * contents have placed been into this stream.  (If a mapped stream is
       * {@code null} an empty stream is used, instead.)
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * @param mapper a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *               <a href="package-summary.html#Statelessness">stateless</a>
       *               function to apply to each element which produces a stream
       *               of new values
       * @return the new stream
       * @see #flatMap flatMap
       */
      virtual DoubleStream& flatMapToDouble(function::Function<T&, DoubleStream> const& mapper) = 0;

      virtual DoubleStream const& flatMapToDouble(function::Function<T, DoubleStream> const& mapper) const = 0;

      /**
       * Returns a stream consisting of the distinct elements (according to
       * {@link Object#equals(Object)}) of this stream.
       *
       * <p>For ordered streams, the selection of distinct elements is stable
       * (for duplicated elements, the element appearing first in the encounter
       * order is preserved.)  For unordered streams, no stability guarantees
       * are made.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">stateful
       * intermediate operation</a>.
       *
       * @apiNote
       * Preserving stability for {@code distinct()} in parallel pipelines is
       * relatively expensive (requires that the operation act as a full barrier,
       * with substantial buffering overhead), and stability is often not needed.
       * Using an unordered stream source (such as {@link #generate(Supplier)})
       * or removing the ordering constraint with {@link #unordered()} may result
       * in significantly more efficient execution for {@code distinct()} in parallel
       * pipelines, if the semantics of your situation permit.  If consistency
       * with encounter order is required, and you are experiencing poor performance
       * or memory utilization with {@code distinct()} in parallel pipelines,
       * switching to sequential execution with {@link #sequential()} may improve
       * performance.
       *
       * @return the new stream
       */
      virtual Stream& distinct() = 0;

      virtual Stream const& distinct() const = 0;

      /**
       * Returns a stream consisting of the elements of this stream, sorted
       * according to natural order.  If the elements of this stream are not
       * {@code Comparable}, a {@code java.lang.ClassCastException} may be thrown
       * when the terminal operation is executed.
       *
       * <p>For ordered streams, the sort is stable.  For unordered streams, no
       * stability guarantees are made.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">stateful
       * intermediate operation</a>.
       *
       * @return the new stream
       */
      virtual Stream& sorted() = 0;

      virtual Stream const& sorted() const = 0;

      /**
       * Returns a stream consisting of the elements of this stream, sorted
       * according to the provided {@code Comparator}.
       *
       * <p>For ordered streams, the sort is stable.  For unordered streams, no
       * stability guarantees are made.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">stateful
       * intermediate operation</a>.
       *
       * @param comparator a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                   <a href="package-summary.html#Statelessness">stateless</a>
       *                   {@code Comparator} to be used to compare stream elements
       * @return the new stream
       */
      virtual Stream& sorted(util::Comparator<T> const& comparator) = 0;

      virtual Stream const& sorted(util::Comparator<T> const& comparator) const = 0;

      /**
       * Returns a stream consisting of the elements of this stream, additionally
       * performing the provided action on each element as elements are consumed
       * from the resulting stream.
       *
       * <p>This is an <a href="package-summary.html#StreamOps">intermediate
       * operation</a>.
       *
       * <p>For parallel stream pipelines, the action may be called at
       * whatever time and in whatever thread the element is made available by the
       * upstream operation.  If the action modifies shared state,
       * it is responsible for providing the required synchronization.
       *
       * @apiNote This method exists mainly to support debugging, where you want
       * to see the elements as they flow past a certain point in a pipeline:
       * <pre>{@code
       *     Stream.of("one", "two", "three", "four")
       *         .filter(e -> e.length() > 3)
       *         .peek(e -> System.out.println("Filtered value: " + e))
       *         .map(String::toUpperCase)
       *         .peek(e -> System.out.println("Mapped value: " + e))
       *         .collect(Collectors.toList());
       * }</pre>
       *
       * <p>In cases where the stream implementation is able to optimize away the
       * production of some or all the elements (such as with short-circuiting
       * operations like {@code findFirst}, or in the example described in
       * {@link #count}), the action will not be invoked for those elements.
       *
       * @param action a <a href="package-summary.html#NonInterference">
       *                 non-interfering</a> action to perform on the elements as
       *                 they are consumed from the stream
       * @return the new stream
       */
      virtual Stream& peek(function::Consumer<T&> const& action) = 0;

      virtual Stream const& peek(function::Consumer<T> const& action) const = 0;

      /**
       * Returns a stream consisting of the elements of this stream, truncated
       * to be no longer than {@code maxSize} in length.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
       * stateful intermediate operation</a>.
       *
       * @apiNote
       * While {@code limit()} is generally a cheap operation on sequential
       * stream pipelines, it can be quite expensive on ordered parallel pipelines,
       * especially for large values of {@code maxSize}, since {@code limit(n)}
       * is constrained to return not just any <em>n</em> elements, but the
       * <em>first n</em> elements in the encounter order.  Using an unordered
       * stream source (such as {@link #generate(Supplier)}) or removing the
       * ordering constraint with {@link #unordered()} may result in significant
       * speedups of {@code limit()} in parallel pipelines, if the semantics of
       * your situation permit.  If consistency with encounter order is required,
       * and you are experiencing poor performance or memory utilization with
       * {@code limit()} in parallel pipelines, switching to sequential execution
       * with {@link #sequential()} may improve performance.
       *
       * @param maxSize the number of elements the stream should be limited to
       * @return the new stream
       * @throws IllegalArgumentException if {@code maxSize} is negative
       */
      virtual Stream& limit(glong maxSize) = 0;

      /**
       * Returns a stream consisting of the remaining elements of this stream
       * after discarding the first {@code n} elements of the stream.
       * If this stream contains fewer than {@code n} elements then an
       * empty stream will be returned.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">stateful
       * intermediate operation</a>.
       *
       * @apiNote
       * While {@code skip()} is generally a cheap operation on sequential
       * stream pipelines, it can be quite expensive on ordered parallel pipelines,
       * especially for large values of {@code n}, since {@code skip(n)}
       * is constrained to skip not just any <em>n</em> elements, but the
       * <em>first n</em> elements in the encounter order.  Using an unordered
       * stream source (such as {@link #generate(Supplier)}) or removing the
       * ordering constraint with {@link #unordered()} may result in significant
       * speedups of {@code skip()} in parallel pipelines, if the semantics of
       * your situation permit.  If consistency with encounter order is required,
       * and you are experiencing poor performance or memory utilization with
       * {@code skip()} in parallel pipelines, switching to sequential execution
       * with {@link #sequential()} may improve performance.
       *
       * @param n the number of leading elements to skip
       * @return the new stream
       * @throws IllegalArgumentException if {@code n} is negative
       */
      virtual Stream& skip(glong n) = 0;

      /**
       * Returns, if this stream is ordered, a stream consisting of the longest
       * prefix of elements taken from this stream that match the given predicate.
       * Otherwise returns, if this stream is unordered, a stream consisting of a
       * subset of elements taken from this stream that match the given predicate.
       *
       * <p>If this stream is ordered then the longest prefix is a contiguous
       * sequence of elements of this stream that match the given predicate.  The
       * first element of the sequence is the first element of this stream, and
       * the element immediately following the last element of the sequence does
       * not match the given predicate.
       *
       * <p>If this stream is unordered, and some (but not all) elements of this
       * stream match the given predicate, then the behavior of this operation is
       * nondeterministic; it is free to take any subset of matching elements
       * (which includes the empty set).
       *
       * <p>Independent of whether this stream is ordered or unordered if all
       * elements of this stream match the given predicate then this operation
       * takes all elements (the result is the same as the input), or if no
       * elements of the stream match the given predicate then no elements are
       * taken (the result is an empty stream).
       *
       * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
       * stateful intermediate operation</a>.
       *
       * @implSpec
       * The default implementation obtains the {@link #spliterator() spliterator}
       * of this stream, wraps that spliterator so as to support the semantics
       * of this operation on traversal, and returns a new stream associated with
       * the wrapped spliterator.  The returned stream preserves the execution
       * characteristics of this stream (namely parallel or sequential execution
       * as per {@link #isParallel()}) but the wrapped spliterator may choose to
       * not support splitting.  When the returned stream is closed, the close
       * handlers for both the returned and this stream are invoked.
       *
       * @apiNote
       * While {@code takeWhile()} is generally a cheap operation on sequential
       * stream pipelines, it can be quite expensive on ordered parallel
       * pipelines, since the operation is constrained to return not just any
       * valid prefix, but the longest prefix of elements in the encounter order.
       * Using an unordered stream source (such as {@link #generate(Supplier)}) or
       * removing the ordering constraint with {@link #unordered()} may result in
       * significant speedups of {@code takeWhile()} in parallel pipelines, if the
       * semantics of your situation permit.  If consistency with encounter order
       * is required, and you are experiencing poor performance or memory
       * utilization with {@code takeWhile()} in parallel pipelines, switching to
       * sequential execution with {@link #sequential()} may improve performance.
       *
       * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                  <a href="package-summary.html#Statelessness">stateless</a>
       *                  predicate to apply to elements to determine the longest
       *                  prefix of elements.
       * @return the new stream
       */
      virtual Stream& takeWhile(function::Predicate<T&> const& predicate) = 0;

      /**
       * Returns, if this stream is ordered, a stream consisting of the remaining
       * elements of this stream after dropping the longest prefix of elements
       * that match the given predicate.  Otherwise returns, if this stream is
       * unordered, a stream consisting of the remaining elements of this stream
       * after dropping a subset of elements that match the given predicate.
       *
       * <p>If this stream is ordered then the longest prefix is a contiguous
       * sequence of elements of this stream that match the given predicate.  The
       * first element of the sequence is the first element of this stream, and
       * the element immediately following the last element of the sequence does
       * not match the given predicate.
       *
       * <p>If this stream is unordered, and some (but not all) elements of this
       * stream match the given predicate, then the behavior of this operation is
       * nondeterministic; it is free to drop any subset of matching elements
       * (which includes the empty set).
       *
       * <p>Independent of whether this stream is ordered or unordered if all
       * elements of this stream match the given predicate then this operation
       * drops all elements (the result is an empty stream), or if no elements of
       * the stream match the given predicate then no elements are dropped (the
       * result is the same as the input).
       *
       * <p>This is a <a href="package-summary.html#StreamOps">stateful
       * intermediate operation</a>.
       *
       * @implSpec
       * The default implementation obtains the {@link #spliterator() spliterator}
       * of this stream, wraps that spliterator so as to support the semantics
       * of this operation on traversal, and returns a new stream associated with
       * the wrapped spliterator.  The returned stream preserves the execution
       * characteristics of this stream (namely parallel or sequential execution
       * as per {@link #isParallel()}) but the wrapped spliterator may choose to
       * not support splitting.  When the returned stream is closed, the close
       * handlers for both the returned and this stream are invoked.
       *
       * @apiNote
       * While {@code dropWhile()} is generally a cheap operation on sequential
       * stream pipelines, it can be quite expensive on ordered parallel
       * pipelines, since the operation is constrained to return not just any
       * valid prefix, but the longest prefix of elements in the encounter order.
       * Using an unordered stream source (such as {@link #generate(Supplier)}) or
       * removing the ordering constraint with {@link #unordered()} may result in
       * significant speedups of {@code dropWhile()} in parallel pipelines, if the
       * semantics of your situation permit.  If consistency with encounter order
       * is required, and you are experiencing poor performance or memory
       * utilization with {@code dropWhile()} in parallel pipelines, switching to
       * sequential execution with {@link #sequential()} may improve performance.
       *
       * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                  <a href="package-summary.html#Statelessness">stateless</a>
       *                  predicate to apply to elements to determine the longest
       *                  prefix of elements.
       * @return the new stream
       * @since 9
       */
      virtual Stream& dropWhile(function::Predicate<T&> const& predicate) = 0;

      /**
       * Performs an action for each element of this stream.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal
       * operation</a>.
       *
       * <p>The behavior of this operation is explicitly nondeterministic.
       * For parallel stream pipelines, this operation does <em>not</em>
       * guarantee to respect the encounter order of the stream, as doing so
       * would sacrifice the benefit of parallelism.  For any given element, the
       * action may be performed at whatever time and in whatever thread the
       * library chooses.  If the action accesses shared state, it is
       * responsible for providing the required synchronization.
       *
       * @param action a <a href="package-summary.html#NonInterference">
       *               non-interfering</a> action to perform on the elements
       */
      virtual void forEach(function::Consumer<T&> const& action) = 0;

      /**
       * Performs an action for each element of this stream, in the encounter
       * order of the stream if the stream has a defined encounter order.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal
       * operation</a>.
       *
       * <p>This operation processes the elements one at a time, in encounter
       * order if one exists.  Performing the action for one element
       * <a href="../concurrent/package-summary.html#MemoryVisibility"><i>happens-before</i></a>
       * performing the action for subsequent elements, but for any given element,
       * the action may be performed in whatever thread the library chooses.
       *
       * @param action a <a href="package-summary.html#NonInterference">
       *               non-interfering</a> action to perform on the elements
       * @see #forEach(Consumer)
       */
      virtual void forEachOrdered(function::Consumer<T&> const& action) = 0;

      /**
       * Returns an array containing the elements of this stream.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal
       * operation</a>.
       *
       * @return an array, whose {@linkplain Class#getComponentType runtime component
       * type} is {@code Object}, containing the elements of this stream
       */
      virtual Array<T> toArray() = 0;

      /**
       * Performs a <a href="package-summary.html#Reduction">reduction</a> on the
       * elements of this stream, using the provided identity value and an
       * <a href="package-summary.html#Associativity">associative</a>
       * accumulation function, and returns the reduced value.  This is equivalent
       * to:
       * <pre>{@code
       *     T result = identity;
       *     for (T element : this stream)
       *         result = accumulator.apply(result, element)
       *     return result;
       * }</pre>
       *
       * but is not constrained to execute sequentially.
       *
       * <p>The {@code identity} value must be an identity for the accumulator
       * function. This means that for all {@code t},
       * {@code accumulator.apply(identity, t)} is equal to {@code t}.
       * The {@code accumulator} function must be an
       * <a href="package-summary.html#Associativity">associative</a> function.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal
       * operation</a>.
       *
       * @apiNote Sum, min, max, average, and string concatenation are all special
       * cases of reduction. Summing a stream of numbers can be expressed as:
       *
       * <pre>{@code
       *     Integer sum = integers.reduce(0, (a, b) -> a+b);
       * }</pre>
       *
       * or:
       *
       * <pre>{@code
       *     Integer sum = integers.reduce(0, Integer::sum);
       * }</pre>
       *
       * <p>While this may seem a more roundabout way to perform an aggregation
       * compared to simply mutating a running total in a loop, reduction
       * operations parallelize more gracefully, without needing additional
       * synchronization and with greatly reduced risk of data races.
       *
       * @param identity the identity value for the accumulating function
       * @param accumulator an <a href="package-summary.html#Associativity">associative</a>,
       *                    <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                    <a href="package-summary.html#Statelessness">stateless</a>
       *                    function for combining two values
       * @return the result of the reduction
       */
      virtual T& reduce(T& identity, function::BinaryOperator<T> const& accumulator) = 0;

      /**
       * Performs a <a href="package-summary.html#Reduction">reduction</a> on the
       * elements of this stream, using an
       * <a href="package-summary.html#Associativity">associative</a> accumulation
       * function, and returns an {@code Optional} describing the reduced value,
       * if any. This is equivalent to:
       * <pre>{@code
       *     boolean foundAny = false;
       *     T result = null;
       *     for (T element : this stream) {
       *         if (!foundAny) {
       *             foundAny = true;
       *             result = element;
       *         }
       *         else
       *             result = accumulator.apply(result, element);
       *     }
       *     return foundAny ? Optional.of(result) : Optional.empty();
       * }</pre>
       *
       * but is not constrained to execute sequentially.
       *
       * <p>The {@code accumulator} function must be an
       * <a href="package-summary.html#Associativity">associative</a> function.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal
       * operation</a>.
       *
       * @param accumulator an <a href="package-summary.html#Associativity">associative</a>,
       *                    <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                    <a href="package-summary.html#Statelessness">stateless</a>
       *                    function for combining two values
       * @return an {@link Optional} describing the result of the reduction
       * @throws NullPointerException if the result of the reduction is null
       * @see #reduce(Object, BinaryOperator)
       * @see #min(Comparator)
       * @see #max(Comparator)
       */
      T& reduce(function::BinaryOperator<T> const& accumulator) const;

      /**
       * Performs a <a href="package-summary.html#MutableReduction">mutable
       * reduction</a> operation on the elements of this stream.  A mutable
       * reduction is one in which the reduced value is a mutable result container,
       * such as an {@code ArrayList}, and elements are incorporated by updating
       * the state of the result rather than by replacing the result.  This
       * produces a result equivalent to:
       * <pre>{@code
       *     R result = supplier.get();
       *     for (T element : this stream)
       *         accumulator.accept(result, element);
       *     return result;
       * }</pre>
       *
       * <p>Like {@link #reduce(Object, BinaryOperator)}, {@code collect} operations
       * can be parallelized without requiring additional synchronization.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal
       * operation</a>.
       *
       * @note There are many existing classes whose signatures are
       * well-suited for use with method references as arguments to {@code collect()}.
       * For example, the following will accumulate strings into an {@code ArrayList}:
       * <pre>{@code
       *     List<String> asList = stringStream.collect(ArrayList::new, ArrayList::add,
       *                                                ArrayList::addAll);
       * }</pre>
       *
       * <p>The following will take a stream of strings and concatenates them into a
       * single string:
       * <pre>{@code
       *     String concat = stringStream.collect(StringBuilder::new, StringBuilder::append,
       *                                          StringBuilder::append)
       *                                 .toString();
       * }</pre>
       *
       * @param <R> the type of the mutable result container
       * @param supplier a function that creates a new mutable result container.
       *                 For a parallel execution, this function may be called
       *                 multiple times and must return a fresh value each time.
       * @param accumulator an <a href="package-summary.html#Associativity">associative</a>,
       *                    <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                    <a href="package-summary.html#Statelessness">stateless</a>
       *                    function that must fold an element into a result
       *                    container.
       * @param combiner an <a href="package-summary.html#Associativity">associative</a>,
       *                    <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                    <a href="package-summary.html#Statelessness">stateless</a>
       *                    function that accepts two partial result containers
       *                    and merges them, which must be compatible with the
       *                    accumulator function.  The combiner function must fold
       *                    the elements from the second result container into the
       *                    first result container.
       * @return the result of the reduction
       */
      Object& collect(function::Supplier<> const& supplier,
                      function::BiConsumer<Object&, T> const& accumulator,
                      function::BiConsumer<> const& combiner);

      /**
       * Accumulates the elements of this stream into a {@code List}. The elements in
       * the list will be in this stream's encounter order, if one exists. The returned List
       * is unmodifiable; calls to any mutator method will always cause
       * {@code UnsupportedOperationException} to be thrown. There are no
       * guarantees on the implementation type or serializability of the returned List.
       *
       * <p>The returned instance may be <a href="{@docRoot}/java.base/java/lang/doc-files/ValueBased.html">value-based</a>.
       * Callers should make no assumptions about the identity of the returned instances.
       * Identity-sensitive operations on these instances (reference equality ({@code ==}),
       * identity hash code, and synchronization) are unreliable and should be avoided.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal operation</a>.
       *
       * @apiNote If more control over the returned object is required, use
       * {@link Collectors#toCollection(Supplier)}.
       *
       * @implSpec The implementation in this interface returns a List produced as if by the following:
       * <pre>{@code
       * Collections.unmodifiableList(new ArrayList<>(Arrays.asList(this.toArray())))
       * }</pre>
       *
       * @implNote Most instances of Stream will override this method and provide an implementation
       * that is highly optimized compared to the implementation in this interface.
       *
       * @return a List containing the stream elements
       */
      virtual util::List<T>& toList() = 0;

      /**
       * Returns the minimum element of this stream according to the provided
       * {@code Comparator}.  This is a special case of a
       * <a href="package-summary.html#Reduction">reduction</a>.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal operation</a>.
       *
       * @param comparator a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                   <a href="package-summary.html#Statelessness">stateless</a>
       *                   {@code Comparator} to compare elements of this stream
       * @return an {@code Optional} describing the minimum element of this stream,
       * or an empty {@code Optional} if the stream is empty
       * @throws NullPointerException if the minimum element is null
       */
      virtual T& min(util::Comparator<T> const& comparator) = 0;

      /**
       * Returns the maximum element of this stream according to the provided
       * {@code Comparator}.  This is a special case of a
       * <a href="package-summary.html#Reduction">reduction</a>.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal
       * operation</a>.
       *
       * @param comparator a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                   <a href="package-summary.html#Statelessness">stateless</a>
       *                   {@code Comparator} to compare elements of this stream
       * @return an {@code Optional} describing the maximum element of this stream,
       * or an empty {@code Optional} if the stream is empty
       * @throws NullPointerException if the maximum element is null
       */
      virtual T& max(util::Comparator<T> const& comparator) = 0;

      /**
       * Returns the count of elements in this stream.  This is a special case of
       * a <a href="package-summary.html#Reduction">reduction</a> and is
       * equivalent to:
       * <pre>{@code
       *     return mapToLong(e -> 1L).sum();
       * }</pre>
       *
       * <p>This is a <a href="package-summary.html#StreamOps">terminal operation</a>.
       *
       * @apiNote
       * An implementation may choose to not execute the stream pipeline (either
       * sequentially or in parallel) if it is capable of computing the count
       * directly from the stream source.  In such cases no source elements will
       * be traversed and no intermediate operations will be evaluated.
       * Behavioral parameters with side-effects, which are strongly discouraged
       * except for harmless cases such as debugging, may be affected.  For
       * example, consider the following stream:
       * <pre>{@code
       *     List<String> l = Arrays.asList("A", "B", "C", "D");
       *     long count = l.stream().peek(System.out::println).count();
       * }</pre>
       * The number of elements covered by the stream source, a {@code List}, is
       * known and the intermediate operation, {@code peek}, does not inject into
       * or remove elements from the stream (as may be the case for
       * {@code flatMap} or {@code filter} operations).  Thus the count is the
       * size of the {@code List} and there is no need to execute the pipeline
       * and, as a side-effect, print out the list elements.
       *
       * @return the count of elements in this stream
       */
      virtual glong count() = 0;

      /**
       * Returns whether any elements of this stream match the provided
       * predicate.  May not evaluate the predicate on all elements if not
       * necessary for determining the result.  If the stream is empty then
       * {@code false} is returned and the predicate is not evaluated.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
       * terminal operation</a>.
       *
       * @apiNote
       * This method evaluates the <em>existential quantification</em> of the
       * predicate over the elements of the stream (for some x P(x)).
       *
       * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                  <a href="package-summary.html#Statelessness">stateless</a>
       *                  predicate to apply to elements of this stream
       * @return {@code true} if any elements of the stream match the provided
       * predicate, otherwise {@code false}
       */
      virtual gbool anyMatch(function::Predicate<T> const& predicate) = 0;

      /**
       * Returns whether all elements of this stream match the provided predicate.
       * May not evaluate the predicate on all elements if not necessary for
       * determining the result.  If the stream is empty then {@code true} is
       * returned and the predicate is not evaluated.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
       * terminal operation</a>.
       *
       * @apiNote
       * This method evaluates the <em>universal quantification</em> of the
       * predicate over the elements of the stream (for all x P(x)).  If the
       * stream is empty, the quantification is said to be <em>vacuously
       * satisfied</em> and is always {@code true} (regardless of P(x)).
       *
       * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                  <a href="package-summary.html#Statelessness">stateless</a>
       *                  predicate to apply to elements of this stream
       * @return {@code true} if either all elements of the stream match the
       * provided predicate or the stream is empty, otherwise {@code false}
       */
      virtual gbool allMatch(function::Predicate<T> const& predicate) = 0;

      /**
       * Returns whether no elements of this stream match the provided predicate.
       * May not evaluate the predicate on all elements if not necessary for
       * determining the result.  If the stream is empty then {@code true} is
       * returned and the predicate is not evaluated.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
       * terminal operation</a>.
       *
       * @apiNote
       * This method evaluates the <em>universal quantification</em> of the
       * negated predicate over the elements of the stream (for all x ~P(x)).  If
       * the stream is empty, the quantification is said to be vacuously satisfied
       * and is always {@code true}, regardless of P(x).
       *
       * @param predicate a <a href="package-summary.html#NonInterference">non-interfering</a>,
       *                  <a href="package-summary.html#Statelessness">stateless</a>
       *                  predicate to apply to elements of this stream
       * @return {@code true} if either no elements of the stream match the
       * provided predicate or the stream is empty, otherwise {@code false}
       */
      virtual gbool noneMatch(function::Predicate<T> const& predicate) = 0;

      /**
       * Returns an {@link Optional} describing the first element of this stream,
       * or an empty {@code Optional} if the stream is empty.  If the stream has
       * no encounter order, then any element may be returned.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
       * terminal operation</a>.
       *
       * @return an {@code Optional} describing the first element of this stream,
       * or an empty {@code Optional} if the stream is empty
       * @throws NullPointerException if the element selected is null
       */
      virtual T& findFirst() = 0;

      /**
       * Returns an {@link Optional} describing some element of the stream, or an
       * empty {@code Optional} if the stream is empty.
       *
       * <p>This is a <a href="package-summary.html#StreamOps">short-circuiting
       * terminal operation</a>.
       *
       * <p>The behavior of this operation is explicitly nondeterministic; it is
       * free to select any element in the stream.  This is to allow for maximal
       * performance in parallel operations; the cost is that multiple invocations
       * on the same source may not return the same result.  (If a stable result
       * is desired, use {@link #findFirst()} instead.)
       *
       * @return an {@code Optional} describing some element of this stream, or an
       * empty {@code Optional} if the stream is empty
       * @throws NullPointerException if the element selected is null
       * @see #findFirst()
       */
      virtual T& findAny() = 0;

      /**
       * Returns an empty sequential {@code Stream}.
       *
       * @param <T> the type of stream elements
       * @return an empty sequential stream
       */
      static Stream& emptyStream();

      /**
       * Returns a sequential {@code Stream} containing a single element, if
       * non-null, otherwise returns an empty {@code Stream}.
       *
       * @param t the single element
       * @param <T> the type of stream elements
       * @return a stream with a single element if the specified element
       *         is non-null, otherwise an empty stream
       * @since 9
       */
      static Stream& of(T const& t);

      /**
       * Returns a sequential ordered stream whose elements are the specified values.
       *
       * @param <T> the type of stream elements
       * @param values the elements of the new stream
       * @return the new stream
       */
      template <class... Ts>
      static Stream& of(Ts const&... ts);

      /**
       * Returns an infinite sequential ordered {@code Stream} produced by iterative
       * application of a function {@code f} to an initial element {@code seed},
       * producing a {@code Stream} consisting of {@code seed}, {@code f(seed)},
       * {@code f(f(seed))}, etc.
       *
       * <p>The first element (position {@code 0}) in the {@code Stream} will be
       * the provided {@code seed}.  For {@code n > 0}, the element at position
       * {@code n}, will be the result of applying the function {@code f} to the
       * element at position {@code n - 1}.
       *
       * <p>The action of applying {@code f} for one element
       * <a href="../concurrent/package-summary.html#MemoryVisibility"><i>happens-before</i></a>
       * the action of applying {@code f} for subsequent elements.  For any given
       * element the action may be performed in whatever thread the library
       * chooses.
       *
       * @param <T> the type of stream elements
       * @param seed the initial element
       * @param f a function to be applied to the previous element to produce
       *          a new element
       * @return a new sequential {@code Stream}
       */
      static Stream& iterate(T const& seed, function::UnaryOperator<T> const& f);

      /**
       * Returns a sequential ordered {@code Stream} produced by iterative
       * application of the given {@code next} function to an initial element,
       * conditioned on satisfying the given {@code hasNext} predicate.  The
       * stream terminates as soon as the {@code hasNext} predicate returns false.
       *
       * <p>{@code Stream.iterate} should produce the same sequence of elements as
       * produced by the corresponding for-loop:
       * <pre>{@code
       *     for (T index=seed; hasNext.test(index); index = next.apply(index)) {
       *         ...
       *     }
       * }</pre>
       *
       * <p>The resulting sequence may be empty if the {@code hasNext} predicate
       * does not hold on the seed value.  Otherwise the first element will be the
       * supplied {@code seed} value, the next element (if present) will be the
       * result of applying the {@code next} function to the {@code seed} value,
       * and so on iteratively until the {@code hasNext} predicate indicates that
       * the stream should terminate.
       *
       * <p>The action of applying the {@code hasNext} predicate to an element
       * <a href="../concurrent/package-summary.html#MemoryVisibility"><i>happens-before</i></a>
       * the action of applying the {@code next} function to that element.  The
       * action of applying the {@code next} function for one element
       * <i>happens-before</i> the action of applying the {@code hasNext}
       * predicate for subsequent elements.  For any given element an action may
       * be performed in whatever thread the library chooses.
       *
       * @param <T> the type of stream elements
       * @param seed the initial element
       * @param hasNext a predicate to apply to elements to determine when the
       *                stream must terminate.
       * @param next a function to be applied to the previous element to produce
       *             a new element
       * @return a new sequential {@code Stream}
       */
      static Stream& iterate(T const& seed,
                             function::Predicate<T> const& hasNext,
                             function::UnaryOperator<T> const& next);

      /**
       * Returns an infinite sequential unordered stream where each element is
       * generated by the provided {@code Supplier}.  This is suitable for
       * generating constant streams, streams of random elements, etc.
       *
       * @param <T> the type of stream elements
       * @param s the {@code Supplier} of generated elements
       * @return a new infinite sequential unordered {@code Stream}
       */
      static Stream& generate(function::Supplier<T> const& s);
    };
  } // stream
} // core

#endif //CORE24_STREAM_H
