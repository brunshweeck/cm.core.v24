//
// Created by brunshweeck on 21 juin 2024.
//

#ifndef CORE24_ITERABLE_H
#define CORE24_ITERABLE_H

#include <core/util/Iterator.h>
#include <core/util/Spliterators.h>

namespace core {
  /**
   * Implementing this interface allows an object to be the target of the enhanced
   * @c for statement (sometimes called the "for-each loop" statement).
   *
   * @tparam T the type of elements returned by the iterator.
   */
  template <class T>
  class Iterable : public virtual Object {
    CORE_FULL_CHECK_SLIMMED_TYPE(T);
    CORE_ADD_GLOBAL_FRIENDS();

  public:
    /**
     * Returns an iterator over elements of type @c T.
     *
     * @return an Iterator.
     */
    virtual util::Iterator<T>& iterator() = 0;

    /**
     * Returns an iterator over elements of type @c T.
     *
     * @return an Iterator.
     */
    virtual util::Iterator2<T>& iterator() const = 0;

    /**
     * Performs the given action for each element of the @c Iterable
     * until all elements have been processed or the action throws an
     * exception.  Actions are performed in the order of iteration, if that
     * order is specified.  Exceptions thrown by the action are relayed to the caller.
     * <p>
     * The behavior of this method is unspecified if the action performs
     * side effects that modify the underlying source of elements, unless an
     * overriding class has specified a concurrent modification policy.
     * </p>
     *
     * @note The default implementation behaves as if:
     *        <pre> for (T &t: *this) { action.accept(t); } </pre>
     *
     * @param action The action to be performed for each element.
     */
    virtual void forEach(function::Consumer<T&> const& action) {
      try {
        util::Iterator<T>& it = iterator();
        while (it.hasNext())
          action.accept(it.next());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    /**
     * Performs the given action for each element of the @c Iterable
     * until all elements have been processed or the action throws an
     * exception.  Actions are performed in the order of iteration, if that
     * order is specified.  Exceptions thrown by the action are relayed to the caller.
     * <p>
     * The behavior of this method is unspecified if the action performs
     * side effects that modify the underlying source of elements, unless an
     * overriding class has specified a concurrent modification policy.
     * </p>
     *
     * @note The default implementation behaves as if:
     *        <pre> for (T const &t: *this) { action.accept(t); } </pre>
     *
     * @param action The action to be performed for each element.
     */
    virtual void forEach(function::Consumer<T> const& action) const {
      try {
        util::Iterator2<T>& it = iterator();
        while (it.hasNext())
          action.accept(it.next());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    /**
     * Creates a @b Spliterator over the elements described by this
     * @c Iterable.
     *
     * @note
     * The default implementation creates an <em>early-binding</em>
     * spliterator from the iterable's @c Iterator.  The spliterator
     * inherits the <em>fail-fast</em> properties of the iterable's iterator.
     *
     * @note
     * The default implementation should usually be overridden.  The
     * spliterator returned by the default implementation has poor splitting
     * capabilities, is unsized, and does not report any spliterator
     * characteristics. Implementing classes can nearly always provide a
     * better implementation.
     *
     * @return a @c Spliterator over the elements described by this
     * @c Iterable.
     */
    virtual util::Spliterator<T>& spliterator() {
      try {
        return util::Spliterators::spliterator<T>(iterator(), 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    /**
     * Creates a @b Spliterator over the elements described by this
     * @c Iterable.
     *
     * @note
     * The default implementation creates an <em>early-binding</em>
     * spliterator from the iterable's @c Iterator.  The spliterator
     * inherits the <em>fail-fast</em> properties of the iterable's iterator.
     *
     * @note
     * The default implementation should usually be overridden.  The
     * spliterator returned by the default implementation has poor splitting
     * capabilities, is unsized, and does not report any spliterator
     * characteristics. Implementing classes can nearly always provide a
     * better implementation.
     *
     * @return a @c Spliterator over the elements described by this
     * @c Iterable.
     */
    virtual util::Spliterator2<T>& spliterator() const {
      try {
        return util::Spliterators::spliterator<T>(iterator(), 0);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

  private:
    CORE_ALIAS(ITER, typename Class<util::Iterator<T>>::Pointer);
    CORE_ALIAS(ITER2, typename Class<util::Iterator2<T>>::Pointer);

    CORE_ALIAS(NEXT, typename Class<T>::Pointer);
    CORE_ALIAS(NEXT2, typename Class<T const>::Pointer);

    /**
     * The point of iteration
     */
    class ForEach final : public Object {
      ITER iter = null; // always null for end point.
      NEXT next = null;
      gbool isFinishByError = false;
      gint index = -1;

    public:
      CORE_EXPLICIT ForEach() {
        CORE_IGNORE(this);
      }

      CORE_EXPLICIT ForEach(Iterable& it) {
        iter = &it.iterator();
        ++(*this);
      }

      ForEach& operator++() {
        next = null;
        if (!isFinishByError && iter->hasNext()) {
          CORE_TRY_RETHROW(next = &iter->next())
          isFinishByError = (next == null);
          if (!isFinishByError)
            index += 1;
        }
        return *this;
      }

      T& operator*() {
        CORE_ASSERT(next != null && iter != null && !isFinishByError);
        return *next;
      }

      gbool equals(Object const& o) const override {
        if (this == &o)
          return true;
        if (Class<ForEach>::hasInstance(o)) {
          ForEach const& f = CORE_XCAST(ForEach const, o);
          if (iter == f.iter)
            return true;
          if (iter == null) // end point is me
            return f.isFinishByError || f.next == null;
          if (f.iter == null)
            return isFinishByError || next == null;
          return *iter == *f.iter;
        }
        return false;
      }

      ~ForEach() override {
        if (iter)
          UNSAFE::deleteInstance(*iter);
      }
    };

    class ForEach2 final : public Object {
      ITER2 iter = null; // always null for end point.
      NEXT2 next = null;
      gbool isFinishByError = false;
      gint index = -1;

    public:
      CORE_EXPLICIT ForEach2() {
        CORE_IGNORE(this);
      }

      CORE_EXPLICIT ForEach2(Iterable const& it) {
        iter = &it.iterator();
        ++(*this);
      }

      ForEach2& operator++() {
        next = null;
        if (!isFinishByError && iter->hasNext()) {
          CORE_TRY_ONLY(next = &iter->next())
          isFinishByError = (next == null);
          if (!isFinishByError)
            index += 1;
        }
        return *this;
      }

      T const& operator*() {
        CORE_ASSERT(next && iter && !isFinishByError);
        return *next;
      }

      gbool equals(Object const& o) const override {
        if (this == &o)
          return true;
        if (Class<ForEach2>::hasInstance(o)) {
          ForEach2 const& f = CORE_XCAST(ForEach2 const, o);
          if (iter == f.iter)
            return true;
          if (iter == null) // end point is me
            return f.isFinishByError || f.next == null;
          if (f.iter == null)
            return isFinishByError || next == null;
          return *iter == *f.iter;
        }
        return false;
      }

      ~ForEach2() override {
        if (iter)
          UNSAFE::deleteInstance(*iter);
      }
    };

  public:
    ForEach begin() { return ForEach(*this); }

    ForEach2 begin() const { return ForEach2(*this); }

    ForEach end() { return ForEach(); }

    ForEach2 end() const { return ForEach2(); }
  };
} // core

#endif //CORE24_ITERABLE_H
