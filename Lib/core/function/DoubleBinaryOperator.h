//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_DOUBLEBINARYOPERATOR_H
#define CORE24_DOUBLEBINARYOPERATOR_H

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents an operation upon two @c double - valued operands and producing a
         * @c double - valued result.   This is the primitive type specialization of
         * @c BinaryOperator for @c gdouble.
         * <p>
         * This is a functional interface whose functional method is @c DoubleBinaryOperator::apply.
         * </p>
         */
        class DoubleBinaryOperator final : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            /**
             * Construct new Operator that do nothing
             * @note Unlike consumer this constructor is only accessible
             * by derived classes
             */
            CORE_IMPLICIT DoubleBinaryOperator() = default;

        public:
            /**
             * Construct new Operator with specified callable
             * object. The given callable may be:
             * @li a <em> binary static function </em>;
             *     @code
             *       gdouble myFunction(gdouble, gdouble);
             *
             *       class MyClass {
             *         public:
             *             static gdouble myStaticMethod(gdouble, gdouble);
             *       };
             *
             *       DoubleBinaryOperator op1 = myFunction;
             *       DoubleBinaryOperator op2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> binary lambda function </em>;
             *      @code
             *        DoubleBinaryOperator op1 = [](gdouble, gdouble) -> gdouble {...}
             *        DoubleBinaryOperator op2 = [&](gdouble, gdouble) -> gdouble {...}
             *        DoubleBinaryOperator op3 = [=](gdouble, gdouble) -> gdouble {...}
             *      @endcode
             * @li an <em>object that implement call operator with two arguments </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gdouble operator() (gdouble, gdouble);
             *        };
             *
             *        DoubleBinaryOperator op = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Double, Double>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Double>::accept<RT<C, Double, Double> >()> CaptureReturn = 1>
            CORE_IMPLICIT DoubleBinaryOperator(C &&callable): DoubleBinaryOperator() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Operator with class method.
             * @code
             * class MyClass {
             *  public:
             *      gdouble myMethod(gdouble, gdouble);
             * };
             *
             * DoubleBinaryOperator op = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Double, Double>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Double>::accept<RT<M, O, Double, Double> >()> CaptureReturn = 1>
            CORE_IMPLICIT DoubleBinaryOperator(O &&object, M method): DoubleBinaryOperator() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gdouble invoke(gdouble a, gdouble b) const override { return CORE_CAST(object.*method, a, b); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new Operator with another consumer.
             *
             * @param other the function.
             */
            CORE_IMPLICIT DoubleBinaryOperator(DoubleBinaryOperator const &other) {
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
            ~DoubleBinaryOperator() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            DoubleBinaryOperator(DoubleBinaryOperator &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            DoubleBinaryOperator &operator=(const DoubleBinaryOperator &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            DoubleBinaryOperator &operator=(DoubleBinaryOperator &&other) CORE_NOTHROW {
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
            gdouble apply(gdouble a, gdouble b) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(a, b);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<DoubleBinaryOperator>::hasInstance(obj) &&
                       launcher == CORE_XCAST(DoubleBinaryOperator const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<DoubleBinaryOperator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual gdouble invoke(gdouble a, gdouble b) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gdouble invoke(gdouble a, gdouble b) const override { return f(a, b); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gdouble invoke(gdouble a, gdouble b) const override { return obj(a, b); }
            };
        };
    } // function
} // core

#endif //CORE24_DOUBLEBINARYOPERATOR_H
