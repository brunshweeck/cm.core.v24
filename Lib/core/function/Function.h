//
// Created by brunshweeck on 15 juin 2024.
//

#ifndef CORE24_FUNCTION_H
#define CORE24_FUNCTION_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a function that accepts one argument and produces a result.
         *
         * @tparam T the type of the input to the function.
         * @tparam R the type of the result of the function.
         */
        template<class T, class R>
        class Function : public virtual Functional {
            CORE_FULL_CHECK_TYPE(T);
            CORE_FULL_CHECK_TYPE(R);

            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_TEMPLATES_AS_FRIEND(UnaryOperator);

            /**
             * The type of argument value accepted by
             * @c Function instance of type T.
             */
            CORE_ALIAS(A, Arg<T>);

            /**
             * The type of value returned by @c Function
             * instance of type T.
             * @note it's always void.
             */
            CORE_ALIAS(Re, Return<R>);

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, typename Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            /**
             * Construct new Function that do nothing
             * @note Unlike consumer this constructor is only accessible
             * by derived classes
             */
            CORE_IMPLICIT Function() = default;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       gint myFunction(String);
             *
             *       class MyClass {
             *         public:
             *             static gint myStaticMethod(String);
             *       };
             *
             *       Function<String, Integer> f1 = myFunction;
             *       Function<String, Integer> f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        Function<String, Integer> f1 = [](String) -> gint {...}
             *        Function<String, Integer> f2 = [&](String) -> gint {...}
             *        Function<String, Integer> f3 = [=](String) -> gint {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gint operator() (String);
             *        };
             *
             *        Function<String, Integer> f = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<C, A> >::template isConvertible<Re>()> CaptureReturn = 1>
            CORE_IMPLICIT Function(C &&callable) {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Function with class method.
             * @code
             * class MyClass {
             *  public:
             *      gint myMethod(String);
             * };
             *
             * Function<String, Integer> f = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<M, O, A> >::template isConvertible<Re>()> CaptureReturn = 1>
            CORE_IMPLICIT Function(O &&object, M method) {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    Re invoke(A arg) const override { return CORE_CAST(object.*method, arg); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c Function with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT Function(Function const &other)  {
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
            ~Function() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            Function(Function &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            Function &operator=(const Function &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            Function &operator=(Function &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given argument
             * value.
             *
             * @param arg the input argument.
             */
            virtual Re apply(A arg) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(arg);
            }

            /**
             * Obtains a composed function that first applies the @c before
             * function to its input, and then applies this function to the result.
             * If evaluation of either function throws an exception, it is relayed to
             * the caller of the composed function.
             *
             * @tparam X the type of input to the @c before function, and to the composed function.
             * @tparam Y the type of return value that will be reused as argument for this function.
             * @tparam CaptureY the value used to ensure that the result of given function is
             *                  convertible implicitly to input type of this function.
             * @param before the function to apply before this function is applied
             * @return a composed function that first applies the @c before
             *         function and then applies this function.
             */
            template<class X, class Y,
                ClassOf(1)::OnlyIf<Class<T>::template accept<Y>()> CaptureY = 1>
            Function<X, R> compose(Function<X, Y> const &before) const {
                return [&](Arg<X> arg) -> Re { return apply(before.apply(arg)); };
            }

            /**
             * Obtains a composed function that first applies the @c after
             * function to its input, and then applies this function to the result.
             * If evaluation of either function throws an exception, it is relayed to
             * the caller of the composed function.
             *
             * @tparam X the type of input to the @c after function.
             * @tparam Y the type of return type of @c after function and composed function.
             * @tparam CaptureX the value used to ensure that the result of this function is
             *              convertible to input type of specified function.
             * @param after the function to apply after this function is applied
             * @return a composed function that first applies this function and then
             *          applies the @c after function
             */
            template<class X, class Y,
                ClassOf(1)::OnlyIf<Class<X>::template accept<R>()> CaptureX = 1>
            Function<T, Y> andThen(Function<X, Y> const &after) const {
                return [&](A arg) -> Return<Y> { return after.apply(apply(arg)); };
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                    Class<Function>::hasInstance(obj) && launcher == CORE_XCAST(Function const, obj).launcher;
            }

            Object & clone() const override {
                try {
                    return UNSAFE::newInstance<Function>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:

            class Handle : public SharedHandle {
            public:
                virtual Re invoke(A arg) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                Re invoke(A arg) const CORE_NOTHROW override {
                    return f(arg);
                }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                Re invoke(A arg) const CORE_NOTHROW override {
                    return obj(arg);
                }
            };
        };

    } // function
} // core

#endif //CORE24_FUNCTION_H
