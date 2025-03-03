//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_TOINTBIFUNCTION_H
#define CORE24_TOINTBIFUNCTION_H

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a function that accepts two arguments and produces a int-valued
         * result.  This is the @c int - producing primitive specialization for
         * @c BiFunction.
         * <p>
         * This is a functional interface whose functional method is @c ToIntBiFunction::apply.
         * </p>
         *
         * @tparam T the type of the first argument to the function
         * @tparam U the type of the second argument to the function
         */
        template<class T, class U>
        class ToIntBiFunction final : public virtual Functional {
            CORE_FULL_CHECK_TYPE(T);
            CORE_FULL_CHECK_TYPE(U);

            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());
            CORE_FAST_ASSERT(!Class<Void>::isSame<U>());

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_TEMPLATES_AS_FRIEND(BinaryOperator,);

            /**
             * The type of first argument value accepted by this @c Function type
             */
            CORE_ALIAS(A, Arg<T>);

            /**
             * The type of second argument value accepted by this @c Function type.
             */
            CORE_ALIAS(B, Arg<U>);

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
            CORE_IMPLICIT ToIntBiFunction() = default;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be:
             * @li a <em> binary static function </em>;
             *     @code
             *       gint myFunction(String, gint);
             *
             *       class MyClass {
             *         public:
             *             static gint myStaticMethod(String, gint);
             *       };
             *
             *       ToIntBiFunction<String, Integer> f1 = myFunction;
             *       ToIntBiFunction<String, Integer> f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> binary lambda function </em>;
             *      @code
             *        ToIntBiFunction<String, Integer> f1 = [](String, gint) -> gint {...}
             *        ToIntBiFunction<String, Integer> f2 = [&](String, gint) -> gint {...}
             *        ToIntBiFunction<String, Integer> f3 = [=](String, gint) -> gint {...}
             *      @endcode
             * @li an <em>object that implement call operator with two arguments </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gint operator() (String, gint);
             *        };
             *
             *        ToIntBiFunction<String, Integer> f = MyCallableClass(...);
             *      @endcode
             *
             * @note the given callable object must be callable with @c T
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A, B>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<C, A, B> >()> CaptureReturn = 1>
            CORE_IMPLICIT ToIntBiFunction(C &&callable) {
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
             * BiFunction<String, Integer> bc1 = { MyClass(), &MyClass::myMethod };
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
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A, B>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<M, O, A, B> >()> CaptureReturn = 1>
            CORE_IMPLICIT ToIntBiFunction(O &&object, M method) {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gint invoke(A a, B b) const override { return CORE_CAST(object.*method, a, b); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c ToIntBiFunction with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT ToIntBiFunction(ToIntBiFunction const &other) {
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
            ~ToIntBiFunction() override {
                if (launcher && launcher->release() == 0) delete launcher;
            }

            ToIntBiFunction(ToIntBiFunction &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            ToIntBiFunction &operator=(const ToIntBiFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            ToIntBiFunction &operator=(ToIntBiFunction &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given arguments
             * values.
             *
             * @param a the first input argument.
             * @param b the second input argument.
             * @return the function result
             */
            gint apply(A a, B b) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(a, b);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<ToIntBiFunction>::hasInstance(obj) &&
                       launcher == CORE_XCAST(ToIntBiFunction const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<BiFunction>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual gint invoke(A a, B b) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gint invoke(A a, B b) const override { return f(a, b); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gint invoke(A a, B b) const override { return obj(a, b); }
            };
        };
    } // function
} // core

#endif //CORE24_TOINTBIFUNCTION_H
