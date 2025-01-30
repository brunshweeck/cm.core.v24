//
// Created by brunshweeck on 24 juin 2024.
//

#ifndef CORE24_INTFUNCTION_H
#define CORE24_INTFUNCTION_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a function that accepts a int-valued argument and produces a
         * result.  This is the @c int-consuming primitive specialization for
         * @c Function.
         *
         * <p>
         *  This is a functional interface whose functional method is @c IntFunction::apply.
         * </p>
         *
         * @tparam R the type of the result of the function
         */
        template<class R>
        class IntFunction final : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

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
            CORE_IMPLICIT IntFunction() = default;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be:
             * @li a <em> binary static function </em>;
             *     @code
             *       String myFunction(gint);
             *
             *       class MyClass {
             *         public:
             *             static String myStaticMethod(gint);
             *       };
             *
             *       IntFunction<String> f1 = myFunction;
             *       IntFunction<String> f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> binary lambda function </em>;
             *      @code
             *        IntFunction<String> f1 = [](gint) -> gint {...}
             *        IntFunction<String> f2 = [&](gint) -> gint {...}
             *        IntFunction<String> f3 = [=](gint) -> gint {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          String operator() (gint);
             *        };
             *
             *        IntFunction<String> f = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Integer>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIfAll<Class<RT<C, Integer> >::template isConvertible<Re>()> CaptureRet = 1>
            CORE_IMPLICIT IntFunction(C &&callable) {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Function with class method.
             * @code
             * class MyClass {
             *  public:
             *      String myMethod(gint);
             * };
             *
             * IntFunction<String> f = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Integer>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIfOne<Class<RT<M, O, Integer> >::template isConvertible<Re>()> CaptureRet = 1>
            CORE_IMPLICIT IntFunction(O &&object, M method): IntFunction() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    Re invoke(gint arg) const override { return CORE_CAST(object.*method, arg); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new Function with another consumer.
             *
             * @param other the function.
             */
            CORE_IMPLICIT IntFunction(const IntFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
            }

            ~IntFunction() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            IntFunction(IntFunction &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            IntFunction &operator=(const IntFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            IntFunction &operator=(IntFunction &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Applies this function to the given argument.
             *
             * @param value the function argument
             * @return the function result
             */
            Re apply(gint value) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(value);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<IntFunction>::hasInstance(obj) &&
                       launcher == CORE_XCAST(IntFunction const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<IntFunction>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual Re invoke(gint value) const = 0;
            };

            template<class T, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                Re invoke(gint value) const override { return f(value); }
            };

            template<class T>
            class Callable<T, OBJECT_REFERENCE> final : public Handle {
                T obj;

            public:
                CORE_EXPLICIT Callable(T obj): obj(obj) {
                }

                Re invoke(gint value) const override { return obj(value); }
            };
        };
    } // function
} // core

#endif //CORE24_INTFUNCTION_H
