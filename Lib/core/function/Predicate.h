//
// Created by brunshweeck on 15 juin 2024.
//

#ifndef CORE24_PREDICATE_H
#define CORE24_PREDICATE_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a predicate (boolean-valued function) of one argument.
         *
         * <p>This is a functional interface whose functional method is @c Predicate::test.
         *
         * @tparam T the type of the input to the predicate
         */
        template <class T>
        class Predicate : public virtual Functional {
            CORE_FULL_CHECK_TYPE(T);

            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());

            /**
             * The type of argument value accepted by
             * @c Predicate instance of type T.
             */
            CORE_ALIAS(A, Arg<T>);

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle: public SharedHandle {
            public:
                virtual gbool invoke(A arg) const = 0;
            };

            CORE_ALIAS(Launcher, typename Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            /**
             * Construct new Predicate instance that Do Nothing.
             * @note Unlike Consumers classes that not return value
             *       This Constructor is only accessible by derived classes.
             */
            CORE_IMPLICIT Predicate() = default;

        public:
            /**
             * Construct new Predicate with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       void myFunction(String);
             *
             *       class MyClass {
             *         public:
             *             static gbool myStaticMethod(String);
             *       };
             *
             *       Predicate<String> p1 = myFunction;
             *       Predicate<String> p2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        Predicate<String> p1 = [](String) -> gbool {...}
             *        Predicate<String> p2 = [&](String) -> gbool {...}
             *        Predicate<String> p3 = [=](String) -> gbool {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gbool operator() (String);
             *        };
             *
             *        Predicate<String> p = MyCallableClass(...);
             *      @endcode
             *
             * @note the given callable object must be callable with @c T
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Boolean>::accept<RT<C, A> >()> CaptureReturn = 1>
            CORE_IMPLICIT Predicate(C&& callable): Predicate() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Predicate with class method.
             * @code
             *    class MyClass {
             *     public:
             *         gbool myMethod(String);
             *    };
             *
             *    Predicate<String> p = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<O>::isClass()> CaptureObject = 1,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureMethod = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Boolean>::accept<RT<M, O, A> >()> CaptureReturn = 1>
            CORE_IMPLICIT Predicate(O&& object, M method): Predicate() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gbool invoke(A arg) const override { return CORE_CAST(object.*method, arg); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c Predicate with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT Predicate(Predicate const& other) CORE_NOTHROW : Predicate() {
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
            ~Predicate() CORE_NOTHROW override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            Predicate(Predicate &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            Predicate & operator=(const Predicate &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            Predicate & operator=(Predicate &&other) CORE_NOTHROW {
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
            virtual gbool test(A arg) const CORE_NOTHROW {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(arg);
            }

            /**
             * Obtains the composed @c Predicate that represents a short-circuiting
             * logical AND of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is @c false, then the @c other predicate
             * is not evaluated.
             * Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller. If evaluation of this predicate throws an exception, the
             * @c other predicate will not be evaluated.
             *
             * @tparam T2 the argument type of specified predicate.
             * @tparam Capture the value used to ensure that the given consumer is compatible.
             * @param other a predicate that will be logically-ANDed with this predicate
             * @return new predicate
             */
            template <class T2, ClassOf(1)::OnlyIfAll<Class<A>::template isConvertible<Arg<T2>>()> Capture = 1>
            Predicate& logicalAnd(Predicate<T2> const& other) const {
                return [&](A arg) -> gbool { return test(arg) && other.test(arg); };
            }

            /**
             * Obtains the composed @c Predicate that represents a short-circuiting
             * logical OR of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is @c false, then the @c other predicate
             *  is not evaluated.
             * Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller. If evaluation of this predicate throws an exception, the
             * @c other predicate will not be evaluated.
             *
             * @tparam T2 the argument type of specified predicate.
             * @tparam Capture the value used to ensure that the given consumer is compatible.
             * @param other a predicate that will be logically-ANDed with this predicate
             * @return new predicate
             */
            template <class T2, ClassOf(1)::OnlyIfAll<Class<A>::template isConvertible<Arg<T2>>()> Capture = 1>
            Predicate& logicalOr(Predicate<T2> const& other) const {
                return [&](A arg) -> gbool { return test(arg) || other.test(arg); };
            }

            /**
             * Returns a predicate that represents the logical negation of this
             * predicate.
             *
             * @return A new predicate
             */
            Predicate& negate() const {
                return [&](A arg) -> gbool { return !test(arg); };
            }

            /**
             * Obtains the predicate that tests if two arguments are equal according
             * to @link Object::equals @endlink
             *
             * @param object the object reference with which to compare for equality.
             * @return new predicate
             */
            static Predicate& equalTo(Object const& object) {
                return [&](A arg) -> gbool { return arg == object; };
            }

            gbool equals(const Object& obj) const override {
                return this == &obj ||
                       Class<Predicate>::hasInstance(obj) && launcher == CORE_XCAST(Predicate const, obj).launcher;
            }

            Object& clone() const override {
                try {
                    return UNSAFE::newInstance<Predicate>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            template<class C, Status status>
            class Callable;

            template <class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gbool invoke(A arg) const override {
                    return f(arg);
                }
            };

            template <class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gbool invoke(A arg) const override { return obj(arg); }
            };
        };
    } // function
} // core

#endif //CORE24_PREDICATE_H
