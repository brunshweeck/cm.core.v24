//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_UNARYOPERATOR_H
#define CORE24_UNARYOPERATOR_H
#include <core/function/Function.h>

namespace core {
    namespace function {
        /**
         * Represents an operation on a single operand that produces a result of the
         * same type as its operand.  This is a specialization of @c Function for
         * the case where the operand and result are of the same type.
         * <p>
         * This is a functional interface whose functional method is @c UnaryOperator::apply.
         * </p>
         *
         * @tparam T the type of the operand and result of the operator
         */
        template<class T>
        class UnaryOperator final : public virtual Function<T, T> {
            CORE_FULL_CHECK_TYPE(T);

            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());

            CORE_ADD_GLOBAL_FRIENDS();

            /**
             * The type of first and second argument value accepted by this @c Predicate type
             */
            CORE_ALIAS(A, Functional::Arg<T>);

            /**
             * The type of value returned by @c Function instance of type T.
             * @note it's always void.
             */
            CORE_ALIAS(Re, Functional::Return<T>);

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            CORE_ALIAS(UNSAFE, Object::UNSAFE);

        protected:
            /**
             * Construct new Operator that do nothing
             * @note Unlike consumer this constructor is only accessible
             * by derived classes
             */
            CORE_IMPLICIT UnaryOperator() = default;

        public:
            /**
             * Construct new Operator with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       gint myFunction(String);
             *
             *       class MyClass {
             *         public:
             *             static String myStaticMethod(String);
             *       };
             *
             *       UnaryOperator<String> f1 = myFunction;
             *       UnaryOperator<String> f2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        UnaryOperator<String> f1 = [](String) -> String {...}
             *        UnaryOperator<String> f2 = [&](String) -> String {...}
             *        UnaryOperator<String> f3 = [=](String) -> String {...}
             *      @endcode
             * @li an <em>object that implement call operator with two arguments </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          String operator() (String);
             *        };
             *
             *        UnaryOperator<String> f = MyCallableClass(...);
             *      @endcode
             *
             * @note the given callable object must be callable with @c T
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<C, A> >::template isConvertible<Re>()> CaptureReturn = 1>
            CORE_IMPLICIT UnaryOperator(C &&callable)
                : Function<T, T>(UNSAFE::forwardInstance<C>(callable)) {
            }

            /**
             * Construct new Function with class method.
             * @code
             * class MyClass {
             *  public:
             *      String myMethod(String);
             * };
             *
             * UnaryOperator<String> op1 = { MyClass(), &MyClass::myMethod };
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
                ClassOf(1)::OnlyIf<Class<RT<M, O, A> >::template isConvertible<Re>()> CaptureReturn = 1>
            CORE_IMPLICIT UnaryOperator(O &&object, M method)
                : Function<T, T>(UNSAFE::forwardInstance<O>(object), method) {
            }

            UnaryOperator(const UnaryOperator &other)
                : Function<T, T>(other) {
            }

            UnaryOperator(UnaryOperator &&other) CORE_NOTHROW
                : Function<T, T>(UNSAFE::moveInstance(other)) {
            }

            UnaryOperator & operator=(const UnaryOperator &other) {
                if (this != &other)
                    Function<T, T>::operator =(other);
                return *this;
            }

            UnaryOperator & operator=(UnaryOperator &&other) CORE_NOTHROW {
                if (this != &other)
                    Function<T, T>::operator =(UNSAFE::moveInstance(other));
                return *this;
            }

            /**
             * Returns a unary operator that always returns its input argument.
             *
             * @return a unary operator that always returns its input argument
             */
            static UnaryOperator identity() {
                return [&](A arg) -> Re { return arg; };
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<UnaryOperator>::hasInstance(obj) && Function<T, T>::equals(obj);
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<UnaryOperator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };
    } // function
} // core

#endif //CORE24_UNARYOPERATOR_H
