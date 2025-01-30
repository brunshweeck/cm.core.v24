//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_INTTODOUBLEFUNCTION_H
#define CORE24_INTTODOUBLEFUNCTION_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a function that accepts a int-valued argument and produces a
         * double-valued result.  This is the @c int - to - @c double primitive
         * specialization for @c Function.
         * <p>
         * This is a functional interface whose functional method is @c IntToDoubleFunction::apply.
         * </p>
         */
        class IntToDoubleFunction final : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            CORE_IMPLICIT IntToDoubleFunction() CORE_NOTHROW;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       gdouble myFunction(gint);
             *
             *       class MyClass {
             *         public:
             *             static gdouble myStaticMethod(gint);
             *       };
             *
             *       IntToDoubleFunction f1 = myFunction;
             *       IntToDoubleFunction f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        IntToDoubleFunction f1 = [](gint) -> gdouble {...}
             *        IntToDoubleFunction f2 = [&](gint) -> gdouble {...}
             *        IntToDoubleFunction f3 = [=](gint) -> gdouble {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gdouble operator() (gint);
             *        };
             *
             *        IntToDoubleFunction f = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Integer>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Double>::accept<RT<C, Integer> >()> CaptureReturn = 1>
            CORE_IMPLICIT IntToDoubleFunction(C &&callable): IntToDoubleFunction() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Operator with class method.
             * @code
             * class MyClass {
             *  public:
             *      gdouble myMethod(gint);
             * };
             *
             * IntToDoubleFunction f = { MyClass(), &MyClass::myMethod };
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
                ClassOf(1)::OnlyIf<Class<Double>::accept<RT<M, O, Integer> >()> CaptureReturn = 1>
            CORE_IMPLICIT IntToDoubleFunction(O &&object, M method): IntToDoubleFunction() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gdouble invoke(gint arg) const CORE_NOTHROW override {
                        return CORE_CAST(object.*method, arg);
                    }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new Function with another.
             *
             * @param other the function.
             */
            CORE_IMPLICIT IntToDoubleFunction(IntToDoubleFunction const &other) {
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
            ~IntToDoubleFunction() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            IntToDoubleFunction(IntToDoubleFunction &&other) CORE_NOTHROW {
            UNSAFE::swapValues(launcher, other.launcher);
            }

            IntToDoubleFunction & operator=(const IntToDoubleFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            IntToDoubleFunction & operator=(IntToDoubleFunction &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given arguments
             * values.
             *
             * @param arg the input argument.
             * @return the function result
             */
            gdouble apply(gint arg) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(arg);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<IntToDoubleFunction>::hasInstance(obj) &&
                       launcher == CORE_XCAST(IntToDoubleFunction const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<IntToDoubleFunction>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:

            class Handle: public SharedHandle {
            public:
                virtual gdouble invoke(gint arg) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gdouble invoke(gint arg) const  override { return f(arg); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gdouble invoke(gint arg) const  override { return obj(arg); }
            };
        };
    } // function
} // core

#endif //CORE24_INTTODOUBLEFUNCTION_H
