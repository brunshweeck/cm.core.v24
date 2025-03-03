//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_BINARYOPERATOR_H
#define CORE24_BINARYOPERATOR_H

#include <core/function/BiFunction.h>
#include <core/util/Comparator.h>

namespace core {
  namespace function {
    /**
     * Represents an operation upon two operands of the same type, producing a result
     * of the same type as the operands.  This is a specialization of @c BiFunction
     * for the case where the operands and the result are all the same type.
     * <p>
     * This is a functional interface whose functional method is @c BinaryOperator::apply.
     * </p>
     *
     * @tparam T the type of the operands and result of the operator
     */
    template <class T>
    class BinaryOperator final : public virtual BiFunction<T, T, T> {
      CORE_FULL_CHECK_TYPE(T);

      CORE_FAST_ASSERT(!Class<Void>::isSame<T>());

      CORE_ADD_GLOBAL_FRIENDS();

      /**
       * The type of first and second argument value accepted by this @c Predicate type
       */
      CORE_ALIAS(A, Functional::Arg<T>);

      /**
       * The type of value returned by @c Function instance of type T.
       * @note it's always void.
       */
      CORE_ALIAS(Re, Functional::Return<T>);

      template <class C, class... A>
      CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

      CORE_ALIAS(UNSAFE, Object::UNSAFE);

    protected:
      /**
       * Construct new Operator that do nothing
       * @note Unlike consumer this constructor is only accessible
       * by derived classes
       */
      CORE_IMPLICIT BinaryOperator() = default;

    public:
      /**
       * Construct new Operator with specified callable
       * object. The given callable may be: <br/>
       * - a <em> binary static function </em>;
       *     @code
       *       gint myFunction(String, String);
       *
       *       class MyClass {
       *         public:
       *             static String myStaticMethod(String, String);
       *       };
       *
       *       BinaryOperator<String> f1 = myFunction;
       *       BinaryOperator<String> f2 = MyClass::myStaticMethod;
       *     @endcode
       * - a <em> binary lambda function </em>;
       *      @code
       *        BinaryOperator<String> f1 = [](String, String) -> String {...}
       *        BinaryOperator<String> f2 = [&](String, String) -> String {...}
       *        BinaryOperator<String> f3 = [=](String, String) -> String {...}
       *      @endcode
       * - an <em>object that implement call operator with two arguments </em>.
       *      @code
       *        class MyCallableClass {
       *        public:
       *          String operator() (String, String);
       *        };
       *
       *        BinaryOperator<String> f = MyCallableClass(...);
       *      @endcode
       *
       * @note the given callable object must be callable with @c T
       *
       * @param callable the callable object.
       */
      template <class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable =
                    1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A, A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<C, A, A>>::template isConvertible<Re>()> CaptureReturn = 1>
      CORE_IMPLICIT BinaryOperator(C&& callable)
        : BiFunction<T, T, T>(UNSAFE::forwardInstance<C>(callable)) {}

      /**
       * Construct new Function with class method.
       * @code
       * class MyClass {
       *  public:
       *      String myMethod(String, String);
       * };
       *
       * BinaryOperator op = { MyClass(), &MyClass::myMethod };
       * @endcode
       *
       * @tparam O the class supporting specified non-static method reference.
       * @tparam M the type of method signature.
       * @param object the instance used call specified method
       * @param method the reference of method support by class @c O
      */
      template <class O, class M,
                ClassOf(1)::OnlyIf<Class<O>::isClass()> CaptureObject = 1,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A, A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<M, O, A, A>>::template isConvertible<Re>()> CaptureReturn = 1>
      CORE_IMPLICIT BinaryOperator(O&& object, M method)
        : BiFunction<T, T, T>(UNSAFE::forwardInstance<O>(object), method) {}

      BinaryOperator(const BinaryOperator& other)
        : BiFunction<T, T, T>(other) {}

      BinaryOperator(BinaryOperator&& other) CORE_NOTHROW
        : BiFunction<T, T, T>(UNSAFE::moveInstance(other)) {}

      BinaryOperator& operator=(const BinaryOperator& other) {
        if (this != &other)
          BiFunction<T, T, T>::operator =(other);
        return *this;
      }

      BinaryOperator& operator=(BinaryOperator&& other) CORE_NOTHROW {
        if (this != &other)
          BiFunction<T, T, T>::operator =(UNSAFE::moveInstance(other));
        return *this;
      }

      /**
       * Returns a @c BinaryOperator which returns the lesser of two elements
       * according to the specified @c Comparator.
       *
       * @param comparator a @c Comparator for comparing the two values
       * @return a @c BinaryOperator which returns the lesser of its operands,
       *         according to the supplied @c Comparator
       */
      template <class X, ClassOf(1)::OnlyIf<Class<X>::template isSuper<T>()> CaptureX = 1>
      static BinaryOperator minBy(util::Comparator<X> const& comparator) {
        return [&](A a, A b) -> Re { return comparator.compare(a, b) <= 0 ? a : b; };
      }

      /**
       * Returns a @c BinaryOperator which returns the greater of two elements
       * according to the specified @c Comparator.
       *
       * @param comparator a @c Comparator for comparing the two values
       * @return a @c BinaryOperator which returns the greater of its operands,
       *         according to the supplied @c Comparator
       */
      template <class X, ClassOf(1)::OnlyIf<Class<X>::template isSuper<T>()> CaptureX = 1>
      static BinaryOperator maxBy(util::Comparator<X> const& comparator) {
        return [&](A a, A b) -> Re { return comparator.compare(a, b) >= 0 ? a : b; };
      }

      gbool equals(const Object& obj) const override {
        return this == &obj ||
            Class<BinaryOperator>::hasInstance(obj) && BiFunction<T, T, T>::equals(obj);
      }

      Object& clone() const override {
        try {
          return UNSAFE::newInstance<BinaryOperator>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
    };
  } // function
} // core

#endif //CORE24_BINARYOPERATOR_H
