//
// Created by brunshweeck on 18 juin 2024.
//

#ifndef CORE24_SUPPLIER_H
#define CORE24_SUPPLIER_H

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace function {
    /**
     * Represents a supplier of results.
     *
     * <p>
     * There is no requirement that a new or distinct result be returned each
     * time the supplier is invoked.
     * </p>
     *
     * <p>
     * This is a functional interface whose functional method is @c Supplier::get.
     *
     * @tparam T the type of results supplied by this supplier
     */
    template <class T>
    class Supplier final : public virtual Functional {
      CORE_FULL_CHECK_TYPE(T);

      CORE_FAST_ASSERT(!Class<Void>::isSame<T>());

      /**
       * The type of value returned by @c Supplier of this type
       */
      CORE_ALIAS(Re, Return<T>);

      template <class C, class... A>
      CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

      class Handle : public SharedHandle {
      public:
        virtual Re invoke() const = 0;
      };

      CORE_ALIAS(Launcher, typename Class<Handle>::Pointer);

      Launcher launcher = { };

    protected:
      /**
       * Construct new Supplier that do nothing
       * @note Unlike consumer this constructor is only accessible
       * by derived classes
       */
      CORE_IMPLICIT Supplier() = default;

    public:
      /**
       * Construct new Supplier with specified callable
       * object. The given callable may be:
       * @li a <em> static function </em>;
       *     @code
       *       String myFunction();
       *
       *       class MyClass {
       *         public:
       *             static String myStaticMethod();
       *       };
       *
       *       Supplier<String> s1 = myFunction;
       *       Supplier<String> s2 = MyClass::myStaticMethod;
       *     @endcode
       * @li a <em> unary lambda function </em>;
       *      @code
       *        Supplier<String> s1 = []() -> gint {...}
       *        Supplier<String> s2 = [&]() -> gint {...}
       *        Supplier<String> s3 = [=]() -> gint {...}
       *      @endcode
       * @li an <em>object that implement call operator without argument </em>.
       *      @code
       *        class MyCallableClass {
       *        public:
       *          String operator()();
       *        };
       *
       *        Supplier<String> s = MyCallableClass(...);
       *      @endcode
       *
       * @param callable the callable object.
       */
      template <class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable =
                    1,
                ClassOf(1)::OnlyIf<Class<C>::isCallable()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<C>>::template isConvertible<Re>()> CaptureReturn = 1>
      CORE_IMPLICIT Supplier(C&& callable): Supplier() {
        if (!Checker<C>::isValid(callable))
          IllegalArgumentException().throws($ftrace());

        launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
      }

      /**
       * Construct new Supplier with class method.
       * @code
       *    class MyClass {
       *    public:
       *         String myMethod();
       *    };
       *
       *    Supplier<String> s = { MyClass(), &MyClass::myMethod };
       * @endcode
       *
       * @tparam O the class supporting specified non-static method reference.
       * @tparam M the type of method signature.
       * @param object the instance used call specified method
       * @param method the reference of method support by class @c O
       */
      template <class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<M, O>>::template isConvertible<Re>()> CaptureReturn = 1>
      CORE_IMPLICIT Supplier(O&& object, M method): Supplier() {
        class MethodHandle final : public Handle {
          O object;
          M method;

        public:
          MethodHandle(O object, M method) : object(object), method(method) {}

          Re invoke() const override { return CORE_CAST(object.*method,); }
        };

        launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
      }

      /**
       * Construct new instance of @c Supplier with another consumer.
       *
       * @param other the consumer.
       */
      CORE_IMPLICIT Supplier(Supplier const& other) : Supplier() {
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
      ~Supplier() override {
        if (launcher && launcher->release() == 0) delete launcher;
        launcher = null;
      }

      Supplier(Supplier&& other) CORE_NOTHROW {
        UNSAFE::swapValues(launcher, other.launcher);
      }

      Supplier& operator=(const Supplier& other) {
        if (this != &other && other.launcher) {
          if (launcher && launcher->release() == 0)
            delete launcher;
          launcher = other.launcher;
          launcher->acquire();
        }
        return *this;
      }

      Supplier& operator=(Supplier&& other) CORE_NOTHROW {
        if (this != &other)
          UNSAFE::swapValues(launcher, other.launcher);
        return *this;
      }

      /**
       * Gets a result.
       *
       * @return a result
       */
      Re get() const CORE_NOTHROW {
        if (launcher) IllegalStateException().throws($ftrace());
        return launcher->invoke();
      }

      gbool equals(const Object& obj) const override {
        return this == &obj ||
            Class<Supplier>::hasInstance(obj) && launcher == CORE_XCAST(Supplier const, obj).launcher;
      }

      Object& clone() const override {
        try {
          return UNSAFE::newInstance<Function>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }

    private:
      template <class C, Status status>
      class Callable;

      template <class Fn>
      class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
        Fn f;

      public:
        CORE_EXPLICIT Callable(Fn f): f(f) {}

        Re invoke() const override { return f(); }
      };

      template <class O>
      class Callable<O, OBJECT_REFERENCE> final : public Handle {
        O obj;

      public:
        CORE_EXPLICIT Callable(O obj): obj(obj) {}

        Re invoke() const override { return obj(); }
      };
    };
  } // function
} // core

#endif //CORE24_SUPPLIER_H
