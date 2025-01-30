//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_TOLONGFUNCTION_H
#define CORE24_TOLONGFUNCTION_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a function that produces a long-valued result.
         * This is the @c long - producing primitive specialization
         * for @c Function.
         * <p>
         * This is a functional interface whose functional method is @c ToLongFunction::apply.
         * </p>
         *
         * @tparam T the type of the input to the function
         */
        template<class T>
        class ToLongFunction final : public virtual Functional {
            CORE_FULL_CHECK_TYPE(T);

            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());

            CORE_ADD_GLOBAL_FRIENDS();

            /**
             * The type of argument value accepted by this @c Function type
             */
            CORE_ALIAS(A, Arg<T>);

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, typename Class<Handle>::Pointer);

            Launcher launcher = null;

        protected:
            /**
             * Construct new Function that do nothing
             * @note Unlike consumer this constructor is only accessible
             * by derived classes
             */
            CORE_IMPLICIT ToLongFunction() = default;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       glong myFunction(String, gint);
             *
             *       class MyClass {
             *         public:
             *             static glong myStaticMethod(String, gint);
             *       };
             *
             *       ToLongFunction<String, Integer> f1 = myFunction;
             *       ToLongFunction<String, Integer> f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        ToLongFunction<String, Integer> f1 = [](String, glong) -> gint {...}
             *        ToLongFunction<String, Integer> f2 = [&](String, glong) -> gint {...}
             *        ToLongFunction<String, Integer> f3 = [=](String, glong) -> gint {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          glong operator() (String, gint);
             *        };
             *
             *        ToLongFunction<String, Integer> f = MyCallableClass(...);
             *      @endcode
             *
             * @note the given callable object must be callable with @c T
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Long>::accept<RT<C, A> >()> CaptureReturn = 1>
            CORE_IMPLICIT ToLongFunction(C &&callable) {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Function with class method.
             * @code
             * class MyClass {
             *  public:
             *      glong myMethod(String, gint);
             * };
             *
             * Function<String, Integer> bc1 = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<O>::isClass()> CaptureObject = 1,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Long>::accept<RT<M, O, A> >()> CaptureReturn = 1>
            CORE_IMPLICIT ToLongFunction(O &&object, M method) {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    glong invoke(A a) const override { return CORE_CAST(object.*method, a); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c ToLongFunction with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT ToLongFunction(ToLongFunction const &other) {
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
            ~ToLongFunction() override {
                if (launcher && launcher->release() == 0) delete launcher;
            }

            ToLongFunction(ToLongFunction &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            ToLongFunction &operator=(const ToLongFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            ToLongFunction &operator=(ToLongFunction &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given arguments
             * values.
             *
             * @param a the input argument.
             * @return the function result
             */
            glong apply(A a) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(a);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<ToLongFunction>::hasInstance(obj) &&
                       launcher == CORE_XCAST(ToLongFunction const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<ToLongFunction>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual glong invoke(A a) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                glong invoke(A a) const override { return f(a); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                glong invoke(A a) const override { return obj(a); }
            };
        };
    } // function
} // core

#endif //CORE24_TOLONGFUNCTION_H
