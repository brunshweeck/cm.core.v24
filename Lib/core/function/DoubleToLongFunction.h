//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_DOUBLETOLONGFUNCTION_H
#define CORE24_DOUBLETOLONGFUNCTION_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a function that accepts a double-valued argument and produces a
         * long-valued result.  This is the @c double - to - @c long primitive
         * specialization for @c Function.
         * <p>
         * This is a functional interface whose functional method is @c DoubleToLongFunction::apply.
         * </p>
         */
        class DoubleToLongFunction final : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            CORE_IMPLICIT DoubleToLongFunction() CORE_NOTHROW;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       glong myFunction(gdouble);
             *
             *       class MyClass {
             *         public:
             *             static glong myStaticMethod(gdouble);
             *       };
             *
             *       DoubleToLongFunction f1 = myFunction;
             *       DoubleToLongFunction f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        DoubleToLongFunction f1 = [](gdouble) -> glong {...}
             *        DoubleToLongFunction f2 = [&](gdouble) -> glong {...}
             *        DoubleToLongFunction f3 = [=](gdouble) -> glong {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          glong operator() (gdouble);
             *        };
             *
             *        DoubleToLongFunction f = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Double>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Long>::accept<RT<C, Double> >()> CaptureReturn = 1>
            CORE_IMPLICIT DoubleToLongFunction(C &&callable): DoubleToLongFunction() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Operator with class method.
             * @code
             * class MyClass {
             *  public:
             *      glong myMethod(gdouble);
             * };
             *
             * DoubleToLongFunction f = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Double>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Long>::accept<RT<M, O, Double> >()> CaptureReturn = 1>
            CORE_IMPLICIT DoubleToLongFunction(O &&object, M method): DoubleToLongFunction() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    glong invoke(gdouble arg) const CORE_NOTHROW override {
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
            CORE_IMPLICIT DoubleToLongFunction(DoubleToLongFunction const &other) {
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
            ~DoubleToLongFunction() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            DoubleToLongFunction(DoubleToLongFunction &&other) CORE_NOTHROW {
            UNSAFE::swapValues(launcher, other.launcher);
            }

            DoubleToLongFunction & operator=(const DoubleToLongFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            DoubleToLongFunction & operator=(DoubleToLongFunction &&other) CORE_NOTHROW {
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
            glong apply(gdouble arg) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(arg);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<DoubleToLongFunction>::hasInstance(obj) &&
                       launcher == CORE_XCAST(DoubleToLongFunction const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<DoubleToLongFunction>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:

            class Handle: public SharedHandle {
            public:
                virtual glong invoke(gdouble arg) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                glong invoke(gdouble arg) const  override { return f(arg); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                glong invoke(gdouble arg) const  override { return obj(arg); }
            };
        };
    } // function
} // core

#endif //CORE24_DOUBLETOLONGFUNCTION_H
