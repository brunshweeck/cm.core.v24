//
// Created by brunshweeck on 14 juin 2024.
//

#ifndef CORE24_FUNCTIONAL_H
#define CORE24_FUNCTIONAL_H

#include <core/Class.h>

namespace core {
    namespace function {
        class Functional : public virtual Object {
            template<class T>
            CORE_ALIAS(PRIME, typename Class<T>::Prime);

            template<class T>
            CORE_ALIAS(CONST, typename Class<T>::Const);

            template<class T>
            CORE_ALIAS(UNREF, typename Class<T>::NRef);

            template<class T>
            CORE_ALIAS(REF, typename Class<T>::Ref);

            template<class T>
            CORE_ALIAS(MOVE, typename Class<T>::Rvalue);

            template<class T>
            CORE_ALIAS(CREF, REF<CONST<T>>);

            template<class T>
            CORE_ALIAS(CMOVE, MOVE<CONST<T>>);

            template<class T1, gbool Condition, class T2>
            CORE_ALIAS(IFELSE, typename Class<T1>::template IfElse<Condition, T2>);

            template<class T, class P = PRIME<T>, class U = UNREF<T>, class R = REF<U>, class CR = CREF<U> >
            CORE_ALIAS(PRT,)
            IFELSE<P,
                Class<Void>::isSame<U>() ||
                !Class<T>::isReference() && (Class<P>::isPrimitive() || Class<U>::isFinal() && !Class<U>::isAbstract()),
                IFELSE<CR, !Class<T>::isReference(), R> >;

            template<class T, class P = PRIME<T>, class U = UNREF<T>, class R = REF<U>, class CR = CREF<U> >
            CORE_ALIAS(RT,)
            IFELSE<U,
                Class<Void>::isSame<U>() ||
                !Class<T>::isReference() && (Class<P>::isPrimitive() || Class<U>::isFinal() && !Class<U>::isAbstract()),
                IFELSE<CR, !Class<T>::isReference(), R> >;

            template<class T, class U = UNREF<T>, class CR = CREF<U> >
            CORE_ALIAS(AT,) IFELSE<T, Class<T>::isReference(), CR>;

            template<class C, class R, class U = UNREF<C> >
            CORE_ALIAS(S00,)IFELSE<R(), Class<C>::template isConvertible<R()>(), U>;

            template<class C, class A, class R, class U = UNREF<C> >
            CORE_ALIAS(S11,)IFELSE<R(A), Class<U>::template isConvertible<R(A)>(), U>;

            template<class C, class A, class R, class U = UNREF<C>, class UA = UNREF<A> >
            CORE_ALIAS(S12,)IFELSE<R(UA), Class<U>::template isConvertible<R(UA)>(), S11<C, A, R> >;

            template<class C, class A, class R, class U = UNREF<C>, class UA = UNREF<A>, class CUA= CONST<UA> >
            CORE_ALIAS(S13,)IFELSE<R(CUA), Class<U>::template isConvertible<R(CUA)>(), S12<C, A, R> >;

            template<class C, class A, class R, class U = UNREF<C>, class UA = UNREF<A>, class CRA= CREF<UA> >
            CORE_ALIAS(S14,)IFELSE<R(CRA), Class<U>::template isConvertible<R(CRA)>(), S13<C, A, R> >;

            template<class C, class A, class R, class U = UNREF<C>, class UA = UNREF<A>, class RA= REF<UA> >
            CORE_ALIAS(S15,)IFELSE<R(RA), Class<U>::template isConvertible<R(RA)>(), S14<C, A, R> >;

            template<class C, class A, class R, class U = UNREF<C>, class UA = UNREF<A>, class CMA= CMOVE<UA> >
            CORE_ALIAS(S16, IFELSE<R(CMA), Class<U>::template isConvertible<R(CMA)>(), S15<C, A, R> >);

            template<class C, class A, class R, class U = UNREF<C>, class UA = UNREF<A>, class MA= MOVE<UA> >
            CORE_ALIAS(S17,)IFELSE<R(MA), Class<U>::template isConvertible<R(MA)>(), S16<C, A, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C> >
            CORE_ALIAS(S21,)IFELSE<R(A, B), Class<U>::template isConvertible<R(A, B)>(), U>;

            // ---

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B> >
            CORE_ALIAS(S22,) IFELSE<R(UA, UB), Class<U>::template isConvertible<R(UA, UB)>(), S21<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUB = CONST<UB> >
            CORE_ALIAS(S23,) IFELSE<R(UA, CUB), Class<U>::template isConvertible<R(UA, CUB)>(), S22<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CRB = CREF<UB> >
            CORE_ALIAS(S24,) IFELSE<R(UA, CRB), Class<U>::template isConvertible<R(UA, CRB)>(), S23<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CMB = CMOVE<UB> >
            CORE_ALIAS(S25,) IFELSE<R(UA, CMB), Class<U>::template isConvertible<R(UA, CMB)>(), S24<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class MB = MOVE<UB> >
            CORE_ALIAS(S26,) IFELSE<R(UA, MB), Class<U>::template isConvertible<R(UA, MB)>(), S25<C, A, B, R> >;

            // ---

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUA = CONST<UA> >
            CORE_ALIAS(S27,) IFELSE<R(CUA, UB), Class<U>::template isConvertible<R(CUA, UB)>(), S26<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUA = CONST<UA>, class CUB = CONST<UB> >
            CORE_ALIAS(S28,) IFELSE<R(CUA, CUB), Class<U>::template isConvertible<R(CUA, CUB)>(), S27<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUA = CONST<UA>, class CRB = CREF<UB> >
            CORE_ALIAS(S29,) IFELSE<R(CUA, CRB), Class<U>::template isConvertible<R(CUA, CRB)>(), S28<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUA = CONST<UA>, class CMB = CMOVE<UB> >
            CORE_ALIAS(S210,)IFELSE<R(CUA, CMB), Class<U>::template isConvertible<R(CUA, CMB)>(), S29<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUA = CONST<UA>, class MB = MOVE<UB> >
            CORE_ALIAS(S211,) IFELSE<R(CUA, MB), Class<U>::template isConvertible<R(CUA, MB)>(), S210<C, A, B, R> >;

            // ---

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CRA = CREF<UA> >
            CORE_ALIAS(S212,) IFELSE<R(CRA, UB), Class<U>::template isConvertible<R(CRA, UB)>(), S211<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUB = CONST<UB>, class CRA = CREF<UA> >
            CORE_ALIAS(S213,) IFELSE<R(CRA, CUB), Class<U>::template isConvertible<R(CRA, CUB)>(), S212<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CRA = CREF<UA>, class CRB = CREF<UB> >
            CORE_ALIAS(S214,) IFELSE<R(CRA, CRB), Class<U>::template isConvertible<R(CRA, CRB)>(), S213<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CRA = CREF<UA>, class CMB = CMOVE<UB> >
            CORE_ALIAS(S215,) IFELSE<R(CRA, CMB), Class<U>::template isConvertible<R(CRA, CMB)>(), S214<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class MB = MOVE<UB>, class CRA = CREF<UA> >
            CORE_ALIAS(S216,) IFELSE<R(CRA, MB), Class<U>::template isConvertible<R(CRA, MB)>(), S215<C, A, B, R> >;

            // ---

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class RA = REF<UA> >
            CORE_ALIAS(S217,) IFELSE<R(RA, UB), Class<U>::template isConvertible<R(RA, UB)>(), S216<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUB = CONST<UB>, class RA = REF<UA> >
            CORE_ALIAS(S218,) IFELSE<R(RA, CUB), Class<U>::template isConvertible<R(RA, CUB)>(), S217<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class RA = REF<UA>, class CRB = CREF<UB> >
            CORE_ALIAS(S219,) IFELSE<R(RA, CRB), Class<U>::template isConvertible<R(RA, CRB)>(), S218<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class RA = REF<UA>, class CMB = CMOVE<UB> >
            CORE_ALIAS(S220,) IFELSE<R(RA, CMB), Class<U>::template isConvertible<R(RA, CMB)>(), S219<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class RA = REF<UA>, class MB = MOVE<UB> >
            CORE_ALIAS(S221,) IFELSE<R(RA, MB), Class<U>::template isConvertible<R(RA, MB)>(), S220<C, A, B, R> >;

            // ---

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CMA = CMOVE<UA> >
            CORE_ALIAS(S222,) IFELSE<R(CMA, UB), Class<U>::template isConvertible<R(CMA, UB)>(), S221<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUB = CONST<UB>, class CMA = CMOVE<UA> >
            CORE_ALIAS(S223,) IFELSE<R(CMA, CUB), Class<U>::template isConvertible<R(CMA, CUB)>(), S222<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CRB = CREF<UB>, class CMA = CMOVE<UA> >
            CORE_ALIAS(S224,) IFELSE<R(CMA, CRB), Class<U>::template isConvertible<R(CMA, CRB)>(), S223<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CMA = CMOVE<UA>, class CMB = CMOVE<UB> >
            CORE_ALIAS(S225,) IFELSE<R(CMA, CMB), Class<U>::template isConvertible<R(CMA, CMB)>(), S224<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class MB = MOVE<UB>, class CMA = CMOVE<UA> >
            CORE_ALIAS(S226,) IFELSE<R(CMA, MB), Class<U>::template isConvertible<R(CMA, MB)>(), S225<C, A, B, R> >;

            // ---

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class MA = MOVE<UA> >
            CORE_ALIAS(S227,) IFELSE<R(MA, UB), Class<U>::template isConvertible<R(MA, UB)>(), S226<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class CUB = CONST<UB>, class MA = MOVE<UA> >
            CORE_ALIAS(S228,) IFELSE<R(MA, CUB), Class<U>::template isConvertible<R(MA, CUB)>(), S227<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class MA = MOVE<UA>, class CRB = CREF<UB> >
            CORE_ALIAS(S229,) IFELSE<R(MA, CRB), Class<U>::template isConvertible<R(MA, CRB)>(), S228<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class MA = MOVE<UA>, class CMB = CMOVE<UB> >
            CORE_ALIAS(S230,)IFELSE<R(MA, CMB), Class<U>::template isConvertible<R(MA, CMB)>(), S229<C, A, B, R> >;

            template<class C, class A, class B, class R, class U = UNREF<C>, class UA = UNREF<A>, class UB = UNREF<B>,
                class MA = MOVE<UA>, class MB = MOVE<UB> >
            CORE_ALIAS(S231,)IFELSE<R(MA, MB), Class<U>::template isConvertible<R(MA, MB)>(), S230<C, A, B, R> >;

        protected:
            /**
             * Obtains the valid return type (<em>primitive if it's possible</em>) from type @c T . such as:
             * @code
             *  using R1 = Functional::PrimeReturn<Integer>; // R1 = gint
             *  using R2 = Functional::PrimeReturn<Integer&>; // R2 = Integer&
             *  using R3 = Functional::PrimeReturn<Number>; // R3 = Number const&
             *  using R4 = Functional::PrimeReturn<Number&>; // R4 = Number&
             * @endcode
             *
             * @tparam T the target type
             */
            template<class T>
            CORE_ALIAS(PrimeReturn,) PRT<T>;

            /**
             * Obtains the valid return type (<em>class only</em>) from type @c T . such as:
             * @code
             *  using R1 = Functional::Return<Integer>; // R1 = Integer
             *  using R2 = Functional::Return<Integer&>; // R2 = Integer&
             *  using R3 = Functional::Return<Number>; // R3 = Number const&
             *  using R4 = Functional::Return<Number&>; // R4 = Number&
             * @endcode
             *
             * @tparam T the target type
             */
            template<class T>
            CORE_ALIAS(Return,) RT<T>;

            /**
             * Obtains the valid argument type from type @c T. such as:
             * @code
             *  using R1 = Functional::Arg<Integer>; // R1 = Integer const&
             *  using R2 = Functional::Arg<Integer&>; // R2 = Integer&
             *  using R3 = Functional::Arg<Object>; // R3 = Object const&
             *  using R4 = Functional::Arg<Number>; // R4 = Number const&
             * @endcode
             *
             * @tparam T the target type
             */
            template<class T>
            CORE_ALIAS(Arg,) AT<T>;

            /**
             * Obtains the simple signature of zero argument,
             * from callable type @c C.
             * @code
             *  auto a = []() { ... };
             *  using S = Functional::SIGN0<ClassOf(a)::Target, ClassOf(a)::Return<>>; // S = void()
             * @endcode
             *
             * @tparam C the callable type
             * @tparam R the return type of callable of type @c C
             */
            template<class C, class R>
            CORE_ALIAS(SIGN0,) S00<C, R>;

            /**
             * Obtains the simple signature of unary callable of
             * type @c C.
             * @code
             *  auto a = [](String s) { ... }; // 'a' is lambda
             *  using C = Functional::SIGN1<ClassOf(a)::Target,
             *                              String,
             *                              ClassOf(a)::Return<String>>; // C = void(String)
             * @endcode
             *
             * @tparam C the callable type
             * @tparam A the argument type for callable of type @c C
             * @tparam R the return type of callable of type @c C
             */
            template<class C, class A, class R>
            CORE_ALIAS(USIGN,) S17<C, A, R>;

            /**
             * Obtains the simple signature of binary callable of
             * type @c C.
             * @code
             *  auto a = [](String s, Integer i) { ... }; // 'a' is lambda
             *  using C = Functional::SIGN1<ClassOf(a)::Target,
             *                              String, Integer,
             *                              ClassOf(a)::Return<String, Integer>>; // C = void(String)
             * @endcode
             *
             * @tparam C the callable type
             * @tparam A the first argument type for callable of type @c C
             * @tparam B the second argument type for callable of type @c C
             * @tparam R the return type of callable of type @c C
             */
            template<class C, class A, class B, class R>
            CORE_ALIAS(BSIGN,) S231<C, A, B, R>;

        private:
            template<class C,
                gbool isFunctionHandle = Class<C>::isPointer(),
                gbool supportEQ = Class<C>::supportEQ()>
            class CallableTester final : public Object {
            protected:
                CORE_IMPLICIT CallableTester() = default;

            public:
                static gbool isValid(C c) { return c != null; }

                static gbool isEquals(C c1, C c2) { return c1 == c2; }
            };

            template<class C>
            class CallableTester<C, false, true> final : public Object {
            protected:
                CallableTester() = default;

            public:
                static gbool isValid(C const &c) { return true; }

                static gbool isEquals(C const &c1, C const &c2) { return c1 == c2; }
            };

            template<class C>
            class CallableTester<C, false, false> final : public Object {
            protected:
                CallableTester() = default;

            public:
                static gbool isValid(C const &c) { return true; }

                static gbool isEquals(C const &c1, C const &c2) { return &c1 == &c2; }
            };

        protected:
            /**
             * Internal utility for check validity for
             * callable of specified type.
             *
             * @tparam C the callable type
             */
            template<class C>
            CORE_ALIAS(Checker,) CallableTester<C>;

            /**
             * The type of @c Functional handle base
             */
            enum Status {
                // all functions pointers (R(*)(A...) or R(A...))
                FUNCTION_REFERENCE = 1 << 0,
                // lambda functions and callable objects
                OBJECT_REFERENCE = 1 << 1,
                // functions members (methods)
                METHOD_REFERENCE = 1 << 2,
                // other object that extends core::function::Functional
                FUNCTIONAL_OBJECT = 1 << 3,
            };

            template <class C>
            static CORE_FAST Status findStatus() {
                return Class<C>::isFunctionMember() ? METHOD_REFERENCE:
                       Class<C>::isPointer()? FUNCTION_REFERENCE : OBJECT_REFERENCE;
            }

            // the shared pointer.
            class SharedHandle : public Object {
                gint volatile counter = 0;

            public:
                gint state() const;
                gbool acquire();
                gint release();
            };
        };
    } // function
} // core

#endif //CORE24_FUNCTIONAL_H
