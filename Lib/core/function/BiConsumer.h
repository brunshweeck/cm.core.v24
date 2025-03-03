//
// Created by brunshweeck on 17 juin 2024.
//

#ifndef CORE24_BICONSUMER_H
#define CORE24_BICONSUMER_H

#include <core/function/Consumer.h>

namespace core {
  namespace function {
    /**
     * Represents an operation that accepts two input arguments and returns no
     * result.  This is the two-arity specialization of @c Consumer.
     * Unlike most other functional interfaces, @c BiConsumer is expected
     * to operate via side effects.
     * <p>
     *  This is a functional interface whose functional method is @em BiConsumer::accept.
     * </p>
     *
     * @tparam T the type of the first argument to the operation
     * @tparam U the type of the second argument to the operation
     */
    template <class T, class U>
    class BiConsumer final : public virtual Functional {
      CORE_FULL_CHECK_TYPE(T);
      CORE_FULL_CHECK_TYPE(U);

      CORE_FAST_ASSERT(!Class<Void>::isSame<T>());
      CORE_FAST_ASSERT(!Class<Void>::isSame<U>());

      CORE_ADD_GLOBAL_FRIENDS();

      /**
       * The type of first argument value accepted by this @c Consumer type
       */
      CORE_ALIAS(A, Arg<T>);

      /**
       * The type of second argument value accepted by this @c Consumer type.
       */
      CORE_ALIAS(B, Arg<U>);

      class Handle;

      CORE_ALIAS(Launcher, typename Class<Handle>::Pointer);

      Launcher launcher = { };

    public:
      /**
       * Construct new Consumer instance that Do Nothing
       */
      CORE_IMPLICIT BiConsumer() = default;

      /**
       * Construct new Consumer with specified callable
       * object. The given callable may be: <br/>
       * - a <em> binary static function </em>;
       *     @code
       *       void myFunction(gint, String);
       *
       *       class MyClass {
       *         public:
       *             static void myStaticMethod(gint, String);
       *       };
       *
       *       BiConsumer<Integer, String> bc1 = myFunction;
       *       BiConsumer<Integer, String> bc2 = MyClass::myStaticMethod;
       *     @endcode
       *    <br/>
       * - a <em> binary lambda function </em>;
       *      @code
       *        BiConsumer<Integer, String> bc1 = [](gint, String) {...}
       *        BiConsumer<Integer, String> bc2 = [&](gint, String) {...}
       *        BiConsumer<Integer, String> bc3 = [=](gint, String) {...}
       *      @endcode
       *    <br/>
       * - an <em>object that implement call operator with two arguments </em>.
       *      @code
       *        class MyCallableClass {
       *        public:
       *          void operator() (gint, String);
       *        };
       *
       *        BiConsumer<Integer, String> bc = MyCallableClass(...);
       *      @endcode
       *    <br/>
       * @note the given callable object must be callable with @c T
       *
       * @param callable the callable object.
       */
      template <class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable =
                    1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A, B>()> CaptureArgs = 1>
      CORE_IMPLICIT BiConsumer(C&& callable): BiConsumer() {
        if (!Checker<C>::isValid(callable))
          IllegalArgumentException().throws($ftrace());

        launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
      }

      /**
       * Construct new Consumer with class method.
       * @code
       * class MyClass {
       *  public:
       *      virtual void myMethod(gint, String);
       * };
       *
       * class MyDerivedClass: public MyClass {
       *  public:
       *      void myMethod(gint, String) override;
       *      void myMethod(gint);
       * };
       *
       * BiConsumer<Integer, String> bc1 = { MyClass(), &MyClass::myMethod };
       * // ambiguous method detection
       * BiConsumer<Integer, String> bc2 = { MyDerivedClass(), &MyDerivedClass::myMethod };
       * // solution
       * BiConsumer<Integer, String> bc2 = { MyDerivedClass(), &MyClass::myMethod };
       * @endcode
       *
       * @tparam O the class supporting specified non-static method reference.
       * @tparam M the type of method signature.
       * @param object the instance used call specified method
       * @param method the reference of method support by class @c O
       */
      template <class O, class M,
                ClassOf(1)::OnlyIf<Class<O>::isClass()> CaptureObject = 1,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureMethod = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A, B>()> CaptureArgs = 1>
      CORE_IMPLICIT BiConsumer(O&& object, M method): BiConsumer() {
        class MethodHandle final : public Handle {
          O object;
          M method;

        public:
          CORE_EXPLICIT MethodHandle(O object, M method) : object(object), method(method) {}

          void invoke(A a, B b) const override {
            CORE_CAST(object.*method, a, b);
          }
        };

        launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
      }

      /**
       * Construct new instance of @c BiConsumer with another consumer.
       *
       * @param other the consumer.
       */
      CORE_IMPLICIT BiConsumer(BiConsumer const& other) CORE_NOTHROW : BiConsumer() {
        if (this != &other && other.launcher) {
          if (launcher && launcher->release() == 0)
            delete launcher;
          launcher = other.launcher;
          launcher->acquire();
        }
      }

      /**
       * Destroy this instance.
       */
      ~BiConsumer() CORE_NOTHROW override {
        if (launcher && launcher->release() == 0) delete launcher;
      }

      BiConsumer(BiConsumer&& other) CORE_NOTHROW {
        UNSAFE::swapValues(launcher, other.launcher);
      }

      BiConsumer& operator=(const BiConsumer& other) {
        if (this != &other && other.launcher) {
          if (launcher && launcher->release() == 0)
            delete launcher;
          launcher = other.launcher;
          launcher->acquire();
        }
        return *this;
      }

      BiConsumer& operator=(BiConsumer&& other) noexcept {
        if (this != &other)
          UNSAFE::swapValues(launcher, other.launcher);
        return *this;
      }

      /**
       * Performs this operation on the given argument
       * value.
       *
       * @param a the first input argument.
       * @param b the second input argument.
       */
      void accept(A a, B b) const CORE_NOTHROW {
        if (launcher) launcher->invoke(a, b);
      }

      /**
       * Obtains the composed @c Consumer that performs, in sequence,
       * this operation followed by @c after operation.
       * If performing either operation throws an exception, it is
       * relayed to the caller of the composed operation.
       * If performing this operation throws an exception, the
       * @c after operation will not be performed.
       *
       * @tparam X the first argument type of specified consumer.
       * @tparam Y the second argument type of specified consumer.
       *
       * @param after the operation to performs after this operation.
       * @return A composed @c Consumer that performs in sequence this
       *          operation followed by the @c after operation.
       */
      template <class X, class Y,
                ClassOf(1)::OnlyIf<Class<X>::template isSuper<T>()> CaptureX = 1,
                ClassOf(1)::OnlyIf<Class<Y>::template isSuper<U>()> CaptureY = 1>
      BiConsumer& andThen(BiConsumer<X, Y> const& after) const {
        return [&](A a, B b)-> void {
          accept(a, b);
          after.accept(a, b);
        };
      }

      gbool equals(const Object& obj) const override {
        return this == &obj ||
            Class<BiConsumer>::hasInstance(obj) && launcher == CORE_XCAST(BiConsumer const, obj).launcher;
      }

      Object& clone() const override {
        try {
          return UNSAFE::newInstance<BiConsumer>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

    private:
      class Handle : public SharedHandle {
      public:
        virtual void invoke(A a, B b) const = 0;
      };

      template <class C, Status status>
      class Callable;

      template <class Fn>
      class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
        Fn f;

      public:
        CORE_EXPLICIT Callable(Fn f): f(f) {}

        void invoke(A a, B b) const override {
          f(a, b);
        }
      };

      template <class O>
      class Callable<O, OBJECT_REFERENCE> final : public Handle {
        O obj;

      public:
        CORE_EXPLICIT Callable(O obj): obj(obj) {}

        void invoke(A a, B b) const CORE_NOTHROW override { obj(a, b); }
      };
    };
  } // function
} // core

#endif //CORE24_BICONSUMER_H
