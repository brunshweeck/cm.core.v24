//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_DOUBLETOINTFUNCTION_H
#define CORE24_DOUBLETOINTFUNCTION_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a function that accepts a double-valued argument and produces an
         * int-valued result.  This is the @c double - to - @c int primitive
         * specialization for @c Function.
         * <p>
         * This is a functional interface whose functional method is @c DoubleToIntFunction::apply.
         * </p>
         */
        class DoubleToIntFunction final : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            CORE_IMPLICIT DoubleToIntFunction() CORE_NOTHROW;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       gint myFunction(gdouble);
             *
             *       class MyClass {
             *         public:
             *             static gint myStaticMethod(gdouble);
             *       };
             *
             *       DoubleToIntFunction f1 = myFunction;
             *       DoubleToIntFunction f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        DoubleToIntFunction f1 = [](gdouble) -> gint {...}
             *        DoubleToIntFunction f2 = [&](gdouble) -> gint {...}
             *        DoubleToIntFunction f3 = [=](gdouble) -> gint {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gint operator() (gdouble);
             *        };
             *
             *        DoubleToIntFunction f = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Double>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<C, Double> >()> CaptureReturn = 1>
            CORE_IMPLICIT DoubleToIntFunction(C &&callable): DoubleToIntFunction() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Operator with class method.
             * @code
             * class MyClass {
             *  public:
             *      gint myMethod(gdouble);
             * };
             *
             * DoubleToIntFunction f = { MyClass(), &MyClass::myMethod };
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
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<M, O, Double> >()> CaptureReturn = 1>
            CORE_IMPLICIT DoubleToIntFunction(O &&object, M method): DoubleToIntFunction() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gint invoke(gdouble arg) const CORE_NOTHROW override {
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
            CORE_IMPLICIT DoubleToIntFunction(DoubleToIntFunction const &other) {
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
            ~DoubleToIntFunction() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            DoubleToIntFunction(DoubleToIntFunction &&other) CORE_NOTHROW {
            UNSAFE::swapValues(launcher, other.launcher);
            }

            DoubleToIntFunction & operator=(const DoubleToIntFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            DoubleToIntFunction & operator=(DoubleToIntFunction &&other) CORE_NOTHROW {
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
            gint apply(gdouble arg) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(arg);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<DoubleToIntFunction>::hasInstance(obj) &&
                       launcher == CORE_XCAST(DoubleToIntFunction const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<DoubleToIntFunction>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:

            class Handle: public SharedHandle {
            public:
                virtual gint invoke(gdouble arg) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gint invoke(gdouble arg) const  override { return f(arg); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gint invoke(gdouble arg) const  override { return obj(arg); }
            };
        };
    } // function
} // core

#endif //CORE24_DOUBLETOINTFUNCTION_H
