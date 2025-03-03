//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_TOINTFUNCTION_H
#define CORE24_TOINTFUNCTION_H

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a function that produces a int-valued result.
         * This is the @c int - producing primitive specialization
         * for @c Function.
         * <p>
         * This is a functional interface whose functional method is @c ToIntFunction::apply.
         * </p>
         *
         * @tparam T the type of the input to the function
         */
        template<class T>
        class ToIntFunction final : public virtual Functional {
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
            CORE_IMPLICIT ToIntFunction() = default;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       gint myFunction(String, gint);
             *
             *       class MyClass {
             *         public:
             *             static gint myStaticMethod(String, gint);
             *       };
             *
             *       ToIntFunction<String, Integer> f1 = myFunction;
             *       ToIntFunction<String, Integer> f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        ToIntFunction<String, Integer> f1 = [](String, gint) -> gint {...}
             *        ToIntFunction<String, Integer> f2 = [&](String, gint) -> gint {...}
             *        ToIntFunction<String, Integer> f3 = [=](String, gint) -> gint {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gint operator() (String, gint);
             *        };
             *
             *        ToIntFunction<String, Integer> f = MyCallableClass(...);
             *      @endcode
             *
             * @note the given callable object must be callable with @c T
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<C, A> >()> CaptureReturn = 1>
            CORE_IMPLICIT ToIntFunction(C &&callable) {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Function with class method.
             * @code
             * class MyClass {
             *  public:
             *      gint myMethod(String, gint);
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
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<M, O, A> >()> CaptureReturn = 1>
            CORE_IMPLICIT ToIntFunction(O &&object, M method) {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gint invoke(A a) const override { return CORE_CAST(object.*method, a); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c ToIntFunction with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT ToIntFunction(ToIntFunction const &other) {
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
            ~ToIntFunction() override {
                if (launcher && launcher->release() == 0) delete launcher;
            }

            ToIntFunction(ToIntFunction &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            ToIntFunction &operator=(const ToIntFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            ToIntFunction &operator=(ToIntFunction &&other) CORE_NOTHROW {
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
            gint apply(A a) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(a);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<ToIntFunction>::hasInstance(obj) &&
                       launcher == CORE_XCAST(ToIntFunction const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<ToIntFunction>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual gint invoke(A a) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gint invoke(A a) const override { return f(a); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gint invoke(A a) const override { return obj(a); }
            };
        };
    } // function
} // core

#endif //CORE24_TOINTFUNCTION_H
