//
// Created by bruns on 05/05/2024.
//

#ifndef CORE24_TEMPLATES_H
#define CORE24_TEMPLATES_H


#include <core/misc/Types.h>


namespace core {
    namespace misc {
        class ALWAYS_TRUE {
        public:
            static CORE_FAST gint V = 1;
        };

        class ALWAYS_FALSE {
        public:
            static CORE_FAST gint V = 0;
        };

        template<class X>
        class ALWAYS {
        public:
            CORE_ALIAS(T, X);
        };

        enum REQUEST: glong {
            REMOVE_QLF   = 0x01,
            CONST_QLF    = 0x02,
            VOL_QLF      = 0x04,
            REF_QLF      = 0x08,
            PTR_QLF      = 0x10,
            RVAL_QLF     = 0x20,
            ARRAY_QLF    = 0x40,
            FUNCTION_QLF = 0x80,
            CLASS_QLF, METH_QLF, PRIME_QLF, OBJECT_QLF, SAME_FLAG, TEMPL_FLAG, MEMBER_FLAG, ENUM_QLF,
            ABSTRACT_FLAG, CTOR_FLAG, ASSIGN_FLAG, SUPER_FLAG, CONVERT_FLAG, COMPLET_FLAG,
            INT_FLAG, FLT_FLAG, CHR_FLAG, STR_FLAG, BOOLEAN_FLAG, CALL_FLAG, EQ_FLAG, LT_FLAG, DTOR_FLAG,
            FINAL_QLF, EMPTY_FLAG, UNION_QLF, POLY_FLAG, LITERAL_FLAG, TRIVIAL_FLAG, VOID_FLAG,
            SLIM_FLAG, SIZE_FLAG
        };

        template<class...>
        CORE_ALIAS(IGNORED, void);

        template<gbool>
        class CONSTANT : public ALWAYS_TRUE {
        };

        template<>
        class CONSTANT<false> : public ALWAYS_FALSE {
        };

        template<gint R, class... T>
        class TEST : public ALWAYS_FALSE {
        };

        template<gint R, class T, glong N = -1>
        class TRANSFORM : public ALWAYS<T> {
        };

        // ============================ [ Simalarity ] =======================================
        template<class T>
        class TEST<REQUEST::SAME_FLAG, T, T> : public ALWAYS_TRUE {
        };

        // ============================ [ Constant   ] =======================================
        template<class T>
        class TEST<REQUEST::CONST_QLF, T const> : public ALWAYS_TRUE {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF, T> : public ALWAYS<T const> {
        };

        template<class R, class... Args>
        class TRANSFORM<REQUEST::CONST_QLF, R(Args...)> : public ALWAYS<R(Args...)> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REMOVE_QLF, T const> : public ALWAYS<T> {
        };

        // ============================ [ Volatile ] =======================================
        template<class T>
        class TEST<REQUEST::VOL_QLF, T volatile> : public ALWAYS_TRUE {
        };

        template<class T>
        class TRANSFORM<REQUEST::VOL_QLF, T> : public ALWAYS<T volatile> {
        };

        template<class R, class... Args>
        class TRANSFORM<REQUEST::VOL_QLF, R(Args...)> : public ALWAYS<R(Args...)> {
        };

        template<class T>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REMOVE_QLF, T volatile> : public ALWAYS<T> {
        };

        // ============================ [ References ] =======================================
        template<class T>
        class TEST<REQUEST::REF_QLF, T &> : public ALWAYS_TRUE {
        };

        template<class T>
        class TEST<REQUEST::REF_QLF, T &&> : public ALWAYS_TRUE {
        };

        template<class T>
        class TEST<REQUEST::RVAL_QLF, T &&> : public ALWAYS_TRUE {
        };

        template<class T>
        class TRANSFORM<REQUEST::REF_QLF, T> : public ALWAYS<T &> {
        };

        template<class T>
        class TRANSFORM<REQUEST::RVAL_QLF, T> : public ALWAYS<T &&> {
        };

        template<>
        class TRANSFORM<REQUEST::REF_QLF, IGNORED<> > : public ALWAYS<IGNORED<> > {
        };

        template<>
        class TRANSFORM<REQUEST::RVAL_QLF, IGNORED<> > : public ALWAYS<IGNORED<> > {
        };

        template<class T>
        class TRANSFORM<REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T &&> : public ALWAYS<T> {
        };

        // ============================ [ Const-Vol ] =======================================
        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF, T> : public ALWAYS<T const volatile> {
        };

        template<class R, class... Args>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF, R(Args...)> : public ALWAYS<R(Args...)> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REMOVE_QLF, T const> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REMOVE_QLF, T volatile> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REMOVE_QLF, T const volatile> : public ALWAYS<T> {
        };

        // ============================ [ Const-Ref ] =======================================
        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF, T> : public ALWAYS<T const &> {
        };

        template<class R, class... Args>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF, R(Args...)> : public TRANSFORM<REQUEST::REF_QLF, R(Args...)> {
        };

        template<>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF, IGNORED<> > : public ALWAYS<IGNORED<> > {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T const> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T &&> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T const &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T const &&> : public ALWAYS<T> {
        };

        // ============================ [ Vol-Ref ] =======================================
        template<class T>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF, T> : public ALWAYS<T volatile &> {
        };

        template<class R, class... Args>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF, R(Args...)> : public TRANSFORM<REQUEST::REF_QLF, R(Args...)> {
        };

        template<>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF, IGNORED<> > : public ALWAYS<IGNORED<> > {
        };

        template<class T>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T volatile> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T &&> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T volatile &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T volatile &&> : public ALWAYS<T> {
        };

        // ============================ [ Const-Vol-Ref ] =======================================
        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF, T> : public ALWAYS<T const volatile &> {
        };

        template<class R, class... Args>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF, R(Args...)> : public TRANSFORM<REF_QLF, R(Args...)> {
        };

        template<>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF, IGNORED<> > : public ALWAYS<IGNORED<> > {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T const> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T volatile> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T &&> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T const &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T volatile &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T const &&> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T volatile &&> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T const volatile> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T const volatile &> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM<CONST_QLF | VOL_QLF | REF_QLF | REMOVE_QLF, T const volatile &&> : public ALWAYS<T> {
        };

        // ============================ [ Pointers ] =======================================
        template<class T>
        class TEST<REQUEST::PTR_QLF, T *> : public ALWAYS_TRUE {
        };

        template<class T>
        class TEST<REQUEST::PTR_QLF, T[]> : public ALWAYS_FALSE {
        };

        template<class T>
        class TEST<REQUEST::PTR_QLF, T[0]> : public ALWAYS_FALSE {
        };

        template<class T, glong N>
        class TEST<REQUEST::PTR_QLF, T[N]> : public ALWAYS_FALSE {
        };

        template<class T>
        class TRANSFORM<REQUEST::PTR_QLF, T> : public ALWAYS<T *> {
        };

        template<class T>
        class TRANSFORM<REQUEST::PTR_QLF, T &> : public ALWAYS<T *> {
        };

        template<class T>
        class TRANSFORM<REQUEST::PTR_QLF, T &&> : public ALWAYS<T *> {
        };

        template<class T>
        class TRANSFORM<REQUEST::PTR_QLF | REQUEST::REMOVE_QLF, T *> : public ALWAYS<T> {
        };

        // ============================ [ Level: N Dimension ] =======================================
        template<class T, gint N, gbool = (N > 0)>
        class TRANSFORM_PTR_LV : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM_PTR_LV<T, 0> : public ALWAYS<T> {
        };

        template<class T>
        class TRANSFORM_PTR_LV<T, 1> : public TRANSFORM<REQUEST::PTR_QLF, T> {
        };

        template<class T, gint N>
        class TRANSFORM_PTR_LV<T, N, true> : public TRANSFORM<REQUEST::PTR_QLF, typename TRANSFORM_PTR_LV<T, N - 1>::T> {
        };

        template<class T, gint ... N>
        class TRANSFORM_ARRAY_LV : public ALWAYS<T> {
        };

        template<class T, gint N>
        class TRANSFORM_ARRAY_LV<T, N> : public TRANSFORM<REQUEST::ARRAY_QLF, T, N> {
        };

        template<class T, gint N1, gint ... N>
        class TRANSFORM_ARRAY_LV<
                    T, N1, N...> : public TRANSFORM_ARRAY_LV<typename TRANSFORM<REQUEST::ARRAY_QLF, T, N1>::T, N...> {
        };

        // ============================ [ Array ] =======================================
        template<class T>
        class TEST<REQUEST::ARRAY_QLF, T[]> : public ALWAYS_TRUE {
        };

        template<class T, glong N>
        class TEST<REQUEST::ARRAY_QLF, T[N]> : public ALWAYS_TRUE {
        };

        template<class T>
        class TRANSFORM<REQUEST::ARRAY_QLF, T> : public ALWAYS<T[]> {
        };

        template<class T>
        class TRANSFORM<REQUEST::ARRAY_QLF, T, 0> : public ALWAYS<T[0]> {
        };

        template<class T, glong N>
        class TRANSFORM<REQUEST::ARRAY_QLF, T, N> : public ALWAYS<T[N]> {
            CORE_FAST_ASSERT(N >= 0);
        };

        template<class T>
        class TRANSFORM<REQUEST::ARRAY_QLF | REQUEST::REMOVE_QLF, T[]> : public ALWAYS<T> {
        };

        template<class T, glong N>
        class TRANSFORM<REQUEST::ARRAY_QLF | REQUEST::REMOVE_QLF, T[N]> : public ALWAYS<T> {
        };

        // ============================ [ Templates ] =======================================
        template<template <class...> class T, class... P>
        class TEST<REQUEST::TEMPL_FLAG, T<P...> > : public ALWAYS_TRUE {
        };

        // ============================ [ Statics Functions/ Statics Methods ] =======================================
        template<class T>
        class TEST<REQUEST::FUNCTION_QLF, T> : public TEST<REQUEST::CONST_QLF | REQUEST::REMOVE_QLF, T const> {
        };

        template<class R>
        class TEST<REQUEST::FUNCTION_QLF, R()> : public ALWAYS_TRUE {
        };

        template<class R>
        class TEST<REQUEST::FUNCTION_QLF, R(...)> : public ALWAYS_TRUE {
        };

        template<class R, class... T>
        class TEST<REQUEST::FUNCTION_QLF, R(T...)> : public ALWAYS_TRUE {
        };

        template<class R, class... T>
        class TEST<REQUEST::FUNCTION_QLF, R(T..., ...)> : public ALWAYS_TRUE {
        };

        // ============================ [ class Member ] =======================================
        template<class T, class C>
        class TEST<REQUEST::MEMBER_FLAG, T C::*> : public ALWAYS_TRUE {
        };

#ifdef CORE_COMPILER_CLANG
        template <class T>
        class TEST<REQUEST::MEMBER_FLAG, T> : public CONSTANT<__is_member_pointer(T)>
        {
        };
#endif


        // ============================ [ Non-Statics Methods ] =======================================


#ifdef CORE_COMPILER_CLANG
        template <class T>
        class TEST<REQUEST::METH_QLF, T> : public CONSTANT<__is_member_function_pointer(T)>
        {
        };

#else

        template<class T, class C>
        class TEST<REQUEST::METH_QLF, T (C::*)()> : public ALWAYS_TRUE {
        };

        template<class T, class C>
        class TEST<REQUEST::METH_QLF, T (C::*)(...)> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...)> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A..., ...)> : public ALWAYS_TRUE {
        };


#ifdef CORE_COMPILER_MSVC

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) const> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) volatile> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) const volatile> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) &> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) const &> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) volatile &> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) const volatile &> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) &&> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) const &&> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) volatile &&> : public ALWAYS_TRUE {
        };

        template<class T, class C, class... A>
        class TEST<REQUEST::METH_QLF, T (C::*)(A...) const volatile &&> : public ALWAYS_TRUE {
        };

#endif

#endif

        // ============================ [ Abstracts Class ] =======================================
        template<class T>
        class TEST<REQUEST::ABSTRACT_FLAG, T> : public CONSTANT<__is_abstract(T)> {
        };

        template<>
        class TEST<REQUEST::ABSTRACT_FLAG, Void> : public ALWAYS_FALSE {
        };

        // ============================ [ Enumerations ] =======================================
        template<class T>
        class TEST<REQUEST::ENUM_QLF, T> : public CONSTANT<__is_enum(T)> {
        };

        // ============================ [ Class ] =======================================
        template<class T>
        class TEST<REQUEST::CLASS_QLF, T> : public CONSTANT<__is_class(T)> {
        };

        // ============================ [ Constructors ] =======================================
        template<class T, class... A>
        class TEST<REQUEST::CTOR_FLAG, T, A...> {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_GCC("-Wctor-dtor-privacy")
            template<class C, class... P, gint = sizeof(C)>
            static CONSTANT<__is_constructible(C, P...)> test(gint) { return {}; }
        CORE_WARNING_POP

            template<class...>
            static ALWAYS_FALSE test(...) { return {}; }

        public:
            static CORE_FAST gint V = decltype(test<T, A...>(0))::V;
        };


        template<class T>
        class TEST<REQUEST::CTOR_FLAG, T, IGNORED<T> > : public TEST<REQUEST::CTOR_FLAG, T> {
        };

        // ============================ [ Assignable ] =======================================
        template<class T, class V>
        class TEST<REQUEST::ASSIGN_FLAG, T, V> : public CONSTANT<__is_assignable(T, V)> {
        };

        // ============================ [ Super class ] =======================================
        template<class S, class T>
        class TEST<SUPER_FLAG, S, T> : public CONSTANT<__is_base_of(S, T)> {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Boolean> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Byte> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Short> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Character> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Integer> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Long> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Float> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Double> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, Complex> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<SUPER_FLAG, Object, String> : public ALWAYS_TRUE {
        };

        // ============================ [ False Declaration ] =======================================
        template<class T, class R = T &&>
        static R declImpl(gint);

        template<class T>
        static T declImpl(...);

        template<class T>
        static decltype(declImpl<T>(0)) decl();

        // ============================ [ Implicitly Convertible ] =======================================
#ifdef CORE_COMPILER_MSVC
        template<class F, class T>
        class TEST<REQUEST::CONVERT_FLAG, F, T> : public CONSTANT<__is_convertible_to(F, T)> {
        };

        template<class T>
        class TEST<REQUEST::CONVERT_FLAG, core::Void, T> : public ALWAYS_FALSE {
        };

        template<class F>
        class TEST<REQUEST::CONVERT_FLAG, F, core::Void> : public ALWAYS_FALSE {
        };

#else
        template <class F, class T>
        class TEST<REQUEST::CONVERT_FLAG, F, T>
        {
            CORE_WARNING_PUSH
            CORE_WARNING_DISABLE_GCC("-Wctor-dtor-privacy")

            template <class X>
            static void call(X) {
                //
            }

            template <class A, class B, class = decltype(call<B>(decl<A>()))>
            static ALWAYS_TRUE test(gint) { return {}; }

            template <class...>
            static ALWAYS_FALSE test(...) { return {}; }

            CORE_WARNING_POP

        public:
            static CORE_FAST gint V = decltype(test<F, T>(0))::V;
        };
#endif


        // ============================ [ Completes type (Sizable type) ] =======================================

        template<class T>
        struct ALWAYS_SIZABLE : public ALWAYS_TRUE {
            static CORE_FAST gint SIZE = sizeof(T);
        };

        template<gint S>
        struct ALWAYS_SIZABLE2 : public ALWAYS_TRUE {
            static CORE_FAST gint SIZE = S;
        };

        struct NEVER_SIZABLE : public ALWAYS_FALSE {
            static CORE_FAST gint SIZE = 0;
        };

        template<class T>
        class TEST<REQUEST::COMPLET_FLAG, T> {
            template<class C, gint S = sizeof(C)>
            static ALWAYS_SIZABLE2<S> test(gint) { return {}; }

            template<class...>
            static NEVER_SIZABLE test(...) { return {}; }

            CORE_ALIAS(TEST_RESULT, decltype(test<T>(0)));

        public:
            static CORE_FAST gint V = TEST_RESULT::V;
            static CORE_FAST gint SIZE = TEST_RESULT::SIZE;
        };

        template<>
        class TEST<REQUEST::COMPLET_FLAG, void> : public NEVER_SIZABLE {
        };

        template<class T>
        class TEST<REQUEST::COMPLET_FLAG | REQUEST::ARRAY_QLF, Array<T> > : public ALWAYS_SIZABLE<Array<T> > {
        };

        template<class T>
        class TEST<REQUEST::COMPLET_FLAG, T[]> : public NEVER_SIZABLE {
        };

        template<class T>
        class TEST<REQUEST::COMPLET_FLAG, T *> : public ALWAYS_SIZABLE<T *> {
        };

        template<class T>
        class TEST<REQUEST::COMPLET_FLAG, T[0]> : public ALWAYS_SIZABLE<T[0]> {
        };

        template<class T, gint N>
        class TEST<REQUEST::COMPLET_FLAG, T[N]> : public ALWAYS_SIZABLE<T[N]> {
        };

        // ============================ [ Integers ] =======================================
        template<>
        class TEST<REQUEST::INT_FLAG, __int8_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::INT_FLAG, __uint8_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::INT_FLAG, __int16_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::INT_FLAG, __uint16_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::INT_FLAG, __int32_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::INT_FLAG, __uint32_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::INT_FLAG, __int64_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::INT_FLAG, __uint64_t> : public ALWAYS_TRUE {
        };

#ifdef __INT128__
        template <>
        class TEST<REQUEST::INT_FLAG, __int128> : public ALWAYS_TRUE
        {
        };

        template <>
        class TEST<REQUEST::INT_FLAG, unsigned __int128> : public ALWAYS_TRUE
        {
        };

#elif defined(__SIZEOF_INT128__)
        template <>
        class TEST<REQUEST::INT_FLAG, __int128> : public ALWAYS_TRUE
        {
        };

        template <>
        class TEST<REQUEST::INT_FLAG, unsigned __int128> : public ALWAYS_TRUE
        {
        };

#elif defined(__GLIBCXX_TYPE_INT_N_0)
        template <>
        class TEST<REQUEST::INT_FLAG, __GLIBCXX_TYPE_INT_N_0> : public ALWAYS_TRUE
        {
        };

        template <>
        class TEST<REQUEST::INT_FLAG, unsigned __GLIBCXX_TYPE_INT_N_0> : public ALWAYS_TRUE
        {
        };

#elif defined(__GLIBCXX_TYPE_INT_N_1)
        template <>
        class TEST<REQUEST::INT_FLAG, __GLIBCXX_TYPE_INT_N_1> : public ALWAYS_TRUE
        {
        };

        template <>
        class TEST<REQUEST::INT_FLAG, unsigned __GLIBCXX_TYPE_INT_N_1> : public ALWAYS_TRUE
        {
        };

#elif defined(__GLIBCXX_TYPE_INT_N_2)
        template <>
        class TEST<REQUEST::INT_FLAG, __GLIBCXX_TYPE_INT_N_2> : public ALWAYS_TRUE
        {
        };

        template <>
        class TEST<REQUEST::INT_FLAG, unsigned __GLIBCXX_TYPE_INT_N_2> : public ALWAYS_TRUE
        {
        };

#elif defined(__GLIBCXX_TYPE_INT_N_3)
        template <>
        class TEST<REQUEST::INT_FLAG, __GLIBCXX_TYPE_INT_N_3> : public ALWAYS_TRUE
        {
        };

        template <>
        class TEST<REQUEST::INT_FLAG, unsigned __GLIBCXX_TYPE_INT_N_3> : public ALWAYS_TRUE
        {
        };

#endif


        // ============================ [ Floating ] =======================================
        template<>
        class TEST<REQUEST::FLT_FLAG, __float32_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::FLT_FLAG, __float64_t> : public ALWAYS_TRUE {
        };

#if defined(__FLOAT128__) || defined(__SIZEOF_FLOAT128__) || !defined(__STRICT_ANSI__) && defined(_GLIBCXX_USE_FLOAT128)

        template <>
        class TEST<REQUEST::FLT_FLAG, __float128> : public ALWAYS_TRUE
        {
        };

#endif


        // ============================ [ Characters ] =======================================
        template<>
        class TEST<REQUEST::CHR_FLAG, __int8_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::CHR_FLAG, __uint8_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::CHR_FLAG, __ucs2_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::CHR_FLAG, __ucs4_t> : public ALWAYS_TRUE {
        };

        template<>
        class TEST<REQUEST::CHR_FLAG, wchar_t> : public ALWAYS_TRUE {
        };

#if defined(__cpp_lib_char8_t)
        template <>
        class TEST<REQUEST::CHR_FLAG, char8_t> : public ALWAYS_TRUE
        {
        };

#endif
        // ============================ [ Booleans ] =======================================
        template<>
        class TEST<REQUEST::BOOLEAN_FLAG, __bool_t> : public ALWAYS_TRUE {
        };

        // ============================ [ Condition ] =======================================
        template<class T, class, gbool>
        class IF_OR_ELSE : public ALWAYS<T> {
        };

        template<class T, class U>
        class IF_OR_ELSE<T, U, false> : public ALWAYS<U> {
        };

        template<class T, gbool Condition>
        class ONLY_IF : public ALWAYS_FALSE {
        };

        template<class T>
        class ONLY_IF<T, true> : public ALWAYS<T> {
        };

        // ============================ [ Callable ] =======================================
        template<class R>
        class TEST<REQUEST::CALL_FLAG, R()> : public ALWAYS_TRUE, public virtual ALWAYS<R> {
        };

        template<class R>
        class TEST<REQUEST::CALL_FLAG, R(), void> : public ALWAYS_TRUE, public virtual ALWAYS<R> {
        };

        template<class R>
        class TEST<REQUEST::CALL_FLAG, R(...)> : public ALWAYS_TRUE, public virtual ALWAYS<R> {
        };

        template<class R>
        class TEST<REQUEST::CALL_FLAG, R(...), void> : public ALWAYS_TRUE, public virtual ALWAYS<R> {
        };

        template<class R, class... A>
        class TEST<REQUEST::CALL_FLAG, R(A...), A...> : public ALWAYS_TRUE, public virtual ALWAYS<R> {
        };

        template<class R, class... A>
        class TEST<REQUEST::CALL_FLAG, R(A..., ...), A...> : public ALWAYS_TRUE, public virtual ALWAYS<R> {
        };


        template<class C, class... A>
        class TEST<REQUEST::CALL_FLAG, C, A...> {
            template<class T, class... P>
            static ALWAYS<decltype(CORE_FCAST(decl<T>(), decl<P>()...))> callTest(gint) { return {}; }

            template<class...>
            static ALWAYS_FALSE callTest(...) { return {}; }

        public:
            CORE_ALIAS(T,) decltype(callTest<C, A...>(0));
            static CORE_FAST gint V = ~TEST<SAME_FLAG, T, ALWAYS_FALSE>::V;
        };


        template<class R, class C, class I, class... A>
        class TEST<REQUEST::CALL_FLAG, R C::*, I, A...> {
            template<class Cl, class Rt, class... P>
            static ALWAYS<decltype((decl<Rt>().*decl<Cl>())(decl<P>()...))> callTest1(gint) { return {}; }

            template<class...>
            static ALWAYS_FALSE callTest1(...) { return {}; }

            template<class Cl, class Rt, class... P>
            static ALWAYS<decltype(((*decl<Rt>()).*decl<Cl>())(decl<P>()...))> callTest2(gint) { return {}; }

            template<class...>
            static ALWAYS_FALSE callTest2(...) { return {}; }

            CORE_ALIAS(Obj, $(typename TRANSFORM<REQUEST::REF_QLF | REQUEST::REMOVE_QLF, I>::T)) ;

            CORE_ALIAS(T1, decltype(callTest1<R C::*, Obj, A...>(0)));
            CORE_ALIAS(T2, decltype(callTest2<R C::*, Obj, A...>(0)));

        public:
            CORE_ALIAS(T, $(typename IF_OR_ELSE<T1, T2, TEST<SAME_FLAG, T2, ALWAYS_FALSE>::V != 0>::T)) ;
            static CORE_FAST gint V = ~TEST<SAME_FLAG, T, ALWAYS_FALSE>::V;
        };


        template<class T>
        class TEST<REQUEST::CALL_FLAG, T, IGNORED<T> > : public TEST<REQUEST::CALL_FLAG, T> {
        };

        // ============================ [ String ] =======================================
        template<class T>
        class TEST<REQUEST::STR_FLAG, T *> : public TEST<REQUEST::CHR_FLAG, T> {
        };

        template<class T, glong N>
        class TEST<REQUEST::STR_FLAG, T[N]> : public TEST<REQUEST::CHR_FLAG, T> {
        };

        template<class T>
        class TEST<REQUEST::STR_FLAG, T[0]> : public TEST<REQUEST::CHR_FLAG, T> {
        };

        template<class T>
        class TEST<REQUEST::STR_FLAG, T[]> : public ALWAYS_FALSE {
        };

        // ============================ [ EQUALITY ] =======================================
        template<class T>
        class TEST<REQUEST::EQ_FLAG, T> {
            template<class A, class = decltype(decl<A>() == decl<A>())>
            static ALWAYS_TRUE test(gint) { return {}; }

            template<class...>
            static ALWAYS_FALSE test(...) { return {}; }

        public:
            static CORE_FAST gbool V = decltype(test<T>(0))::V;
        };

        // ============================ [ LESS-THAN ] =======================================
        template<class T>
        class TEST<LT_FLAG, T> {
            template<class A, class = decltype(decl<A>() < decl<A>())>
            static ALWAYS_TRUE test(gint) { return {}; }

            template<class...>
            static ALWAYS_FALSE test(...) { return {}; }

        public:
            static CORE_FAST gbool V = decltype(test<T>(0))::V;
        };

        // ============================ [ DESTRUCTIBLE ] =======================================
#ifdef CORE_COMPILER_MSVC
        template<class T>
        class TEST<DTOR_FLAG, T> : public CONSTANT<__is_destructible(T)> {
        };

#else
        template <class T>
        class TEST<DTOR_FLAG, T>
        {
            template <class A, class = decltype(decl<A>().~A())>
            static ALWAYS_TRUE test(gint) { return {}; }

            template <class...>
            static ALWAYS_FALSE test(...) { return {}; }

        public:
            static CORE_FAST gbool V = decltype(test<T>(0))::V;
        };
#endif

        // ============================ [ Prim to Class Models ] =======================================
        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __bool_t> : public ALWAYS<Boolean> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __int8_t> : public ALWAYS<Byte> {
        };

#ifdef CORE_COMPILER_MSVC
        // For another compiler, char and signed char is not distincts types
        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, char> : public ALWAYS<Byte> {
        };
#endif // CORE_COMPILER_MSVC

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint8_t> : public ALWAYS<Byte> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __int16_t> : public ALWAYS<Short> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint16_t> : public ALWAYS<Short> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __int32_t> : public ALWAYS<Integer> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint32_t> : public ALWAYS<Integer> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __int64_t> : public ALWAYS<Long> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint64_t> : public ALWAYS<Long> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __float32_t> : public ALWAYS<Float> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __float64_t> : public ALWAYS<Double> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __ucs2_t> : public ALWAYS<Character> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __ucs4_t> : public ALWAYS<Integer> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, void> : public ALWAYS<Void> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __bool_t[N]> : public ALWAYS<BooleanArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __int8_t[N]> : public ALWAYS<ByteArray> {
        };

#ifndef CORE_COMPILER_MSVC
        // For another compiler, char and signed char is not distincts types
        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, char[N]> : public ALWAYS<String> {
        };
        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, char*> : public ALWAYS<String> {
        };
#endif // CORE_COMPILER_MSVC

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint8_t[N]> : public ALWAYS<String> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint8_t*> : public ALWAYS<String> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __int16_t[N]> : public ALWAYS<ShortArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint16_t[N]> : public ALWAYS<ShortArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __int32_t[N]> : public ALWAYS<IntArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint32_t[N]> : public ALWAYS<IntArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __int64_t[N]> : public ALWAYS<LongArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __uint64_t[N]> : public ALWAYS<LongArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __float32_t[N]> : public ALWAYS<FloatArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __float64_t[N]> : public ALWAYS<DoubleArray> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __ucs2_t[N]> : public ALWAYS<String> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __ucs2_t*> : public ALWAYS<String> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, __ucs4_t[N]> : public ALWAYS<String> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __ucs4_t*> : public ALWAYS<String> {
        };

        template<glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, wchar_t[N]> : public ALWAYS<String> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, wchar_t*> : public ALWAYS<String> {
        };

#ifdef CORE_COMPILER_MSVC
        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, _Fcomplex> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, _Dcomplex> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, _Lcomplex> : public ALWAYS<Complex> {
        };
#else
        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ char> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ unsigned char> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ short> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ unsigned short> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ int> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ unsigned> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ long> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ unsigned long> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ long long> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, __complex__ unsigned long long> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, _Fcomplex> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, _Dcomplex> : public ALWAYS<Complex> {
        };

        template<>
        class TRANSFORM<REQUEST::CLASS_QLF, _Lcomplex> : public ALWAYS<Complex> {
        };
#endif

        template<class T>
        class TRANSFORM<REQUEST::CLASS_QLF, T const> : public TRANSFORM<REQUEST::CLASS_QLF, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CLASS_QLF, T &> : public TRANSFORM<REQUEST::CLASS_QLF, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CLASS_QLF, T &&> : public TRANSFORM<REQUEST::CLASS_QLF, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CLASS_QLF, T volatile> : public TRANSFORM<REQUEST::CLASS_QLF, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CLASS_QLF, T const volatile> : public TRANSFORM<REQUEST::CLASS_QLF, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CLASS_QLF, T const*> : public TRANSFORM<REQUEST::CLASS_QLF, T*> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CLASS_QLF, T volatile*> : public TRANSFORM<REQUEST::CLASS_QLF, T*> {
        };

        template<class T>
        class TRANSFORM<REQUEST::CLASS_QLF, T const volatile*> : public TRANSFORM<REQUEST::CLASS_QLF, T*> {
        };

        template<class T, glong N>
        class TRANSFORM<REQUEST::CLASS_QLF, T[N]> : public ALWAYS<Array<typename TRANSFORM<REQUEST::CLASS_QLF, T>::T> > {
        };

        // ============================ [ Class to Prim Models ] =======================================
        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Boolean> : public ALWAYS<gbool> {
        };

        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Byte> : public ALWAYS<gbyte> {
        };

        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Short> : public ALWAYS<gshort> {
        };

        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Character> : public ALWAYS<gchar> {
        };

        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Integer> : public ALWAYS<gint> {
        };

        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Float> : public ALWAYS<gfloat> {
        };

        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Long> : public ALWAYS<glong> {
        };

        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Double> : public ALWAYS<gdouble> {
        };

        template<>
        class TRANSFORM<REQUEST::PRIME_QLF, Void> : public ALWAYS<void> {
        };

        template<class E>
        class TRANSFORM<REQUEST::PRIME_QLF, Enum<E> > : public ALWAYS<E> {
        };

        // ============================ [ All Of ... Is True ] =======================================
        template<gbool...>
        class ALL_IS_TRUE;

        template<>
        class ALL_IS_TRUE<> : public ALWAYS_TRUE {
        };

        template<>
        class ALL_IS_TRUE<true> : public ALWAYS_TRUE {
        };

        template<gbool ... bs>
        class ALL_IS_TRUE<true, bs...> : public ALL_IS_TRUE<bs...> {
        };

        template<>
        class ALL_IS_TRUE<false> : public ALWAYS_FALSE {
        };

        template<gbool ... bs>
        class ALL_IS_TRUE<false, bs...> : public ALWAYS_FALSE {
        };

        // ============================ [ One of ... Is True ] =======================================
        template<gbool...>
        class ONE_IS_TRUE;

        template<>
        class ONE_IS_TRUE<> : public ALWAYS_TRUE {
        };

        template<>
        class ONE_IS_TRUE<true> : public ALWAYS_TRUE {
        };

        template<gbool ... bs>
        class ONE_IS_TRUE<false, bs...> : public ONE_IS_TRUE<bs...> {
        };

        template<>
        class ONE_IS_TRUE<false> : public ALWAYS_FALSE {
        };

        template<gbool ... bs>
        class ONE_IS_TRUE<true, bs...> : public ALWAYS_TRUE {
        };

        // ============================ [ FINAL ] =======================================
        template<class T>
        class TEST<REQUEST::FINAL_QLF, T> : public CONSTANT<__is_final(T)> {
        };

        template<>
        class TEST<REQUEST::FINAL_QLF, Void> : public ALWAYS_TRUE {
        };

        // ============================ [ UNION ] =======================================
        template<class T>
        class TEST<REQUEST::UNION_QLF, T> : public CONSTANT<__is_union(T)> {
        };

        // ============================ [ EMPTY ] =======================================
        template<class T>
        class TEST<REQUEST::EMPTY_FLAG, T> : public CONSTANT<__is_empty(T)> {
        };

        // ============================ [ POLYMORPHIC ] =======================================
        template<class T>
        class TEST<REQUEST::POLY_FLAG, T> : public CONSTANT<__is_polymorphic(T)> {
        };

        // ============================ [ LITERAL ] =======================================
        template<class T>
        class TEST<REQUEST::LITERAL_FLAG, T> : public CONSTANT<__is_literal_type(T)> {
        };

        // ============================ [ TRIVIAL ] =======================================
        template<class T>
        class TEST<REQUEST::TRIVIAL_FLAG, T> : public CONSTANT<__is_trivial(T)> {
        };

        // ============================ [ VOID ] =======================================
        template<class T>
        class TEST<REQUEST::VOID_FLAG, T> : public TEST<SAME_FLAG, T, IGNORED<T> > {
        };

        // ============================ [ SLIM Style ] =======================================
        template<class T>
        class TEST<REQUEST::SLIM_FLAG, T const> : public ALWAYS_FALSE {
        };

        template<class T>
        class TEST<REQUEST::SLIM_FLAG, T volatile> : public ALWAYS_FALSE {
        };

        template<class T>
        class TEST<REQUEST::SLIM_FLAG, T *> : public TEST<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TEST<REQUEST::SLIM_FLAG, T const volatile> : public ALWAYS_FALSE {
        };

        template<class T>
        class TEST<REQUEST::SLIM_FLAG, T &> : public ALWAYS_FALSE {
        };

        template<class T>
        class TEST<REQUEST::SLIM_FLAG, T &&> : public ALWAYS_FALSE {
        };

        template<class T, glong N>
        class TEST<REQUEST::SLIM_FLAG, T[N]> : public TEST<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TEST<REQUEST::SLIM_FLAG, T[0]> : public TEST<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TEST<REQUEST::SLIM_FLAG, T[]> : public TEST<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, T const> : public TRANSFORM<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, T volatile> : public TRANSFORM<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, T const volatile> : public TRANSFORM<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, T &> : public TRANSFORM<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, T &&> : public TRANSFORM<REQUEST::SLIM_FLAG, T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, T *> : public TRANSFORM<REQUEST::PTR_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, T[]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, const T
                    []> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, volatile T
                    []> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, const volatile T
                    []> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, T[0]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T, 0> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, const T[
                    0]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T, 0> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, volatile T[
                    0]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T, 0> {
        };

        template<class T>
        class TRANSFORM<REQUEST::SLIM_FLAG, const volatile T[
                    0]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T, 0> {
        };

        template<class T, glong N>
        class TRANSFORM<REQUEST::SLIM_FLAG, T[N]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T, N> {
        };

        template<class T, glong N>
        class TRANSFORM<REQUEST::SLIM_FLAG, const T[
                    N]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T, N> {
        };

        template<class T, glong N>
        class TRANSFORM<REQUEST::SLIM_FLAG, volatile T[
                    N]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T, N> {
        };

        template<class T, glong N>
        class TRANSFORM<REQUEST::SLIM_FLAG, const volatile T[
                    N]> : public TRANSFORM<REQUEST::ARRAY_QLF, typename TRANSFORM<REQUEST::SLIM_FLAG, T>::T, N> {
        };

        // ============================ [ InstanceOf ] =======================================
        template<class T>
        class TEST<SUPER_FLAG, T> : public ALWAYS_FALSE {
        public:
            template<gbool isClass, class S = T>
            class IMPL {
            public:
                template<class E>
                static CORE_FAST gbool checkInstance(E const &e) {
                    CORE_ALIAS(T1, $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, S>::T)) ;
                    CORE_ALIAS(T2, $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, E>::T)) ;

                    CORE_ALIAS(Super, $(typename TRANSFORM<REQUEST::PTR_QLF, T2>::T)) ;
                    CORE_ALIAS(Derived, $(typename TRANSFORM<REQUEST::PTR_QLF, T1>::T)) ;

                    return CORE_DCAST(Derived, CORE_FCAST(Super, &e)) != 0;
                }
            };

            template<class S>
            class IMPL<false, S> {
            public:
                template<class E>
                static CORE_FAST gbool checkInstance(E const &) {
                    return TEST<SAME_FLAG, S, E>::V != 0;
                }
            };

            template<gbool isPointer, gbool isClass, class S = T>
            class IMPL2 {
            public:
                template<class E>
                static CORE_FAST gbool checkInstance(E const &) {
                    return isClass && isPointer;
                }
            };

            template<class S>
            class IMPL2<true, true, S> {
            public:
                template<class E>
                static CORE_FAST gbool checkInstance(E const &e) {
                    CORE_ALIAS(Derived, $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, S>::T)) ;
                    CORE_ALIAS(Super, $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, E>::T)) ;

                    CORE_ALIAS(DerivedRef, $(typename TRANSFORM<REQUEST::PTR_QLF | REQUEST::REMOVE_QLF, S>::T)) ;
                    CORE_ALIAS(SuperRef, $(typename TRANSFORM<REQUEST::PTR_QLF | REQUEST::REMOVE_QLF, E>::T)) ;

                    return CORE_DCAST(Derived, CORE_FCAST(Super, e)) != 0
                           || e == 0 && TEST<SUPER_FLAG, SuperRef, DerivedRef>::V != 0;
                }
            };

            template<class E>
            static CORE_FAST gbool checkValue(E &&) {
                return (TEST<REQUEST::CTOR_FLAG, T, E>::V | TEST<REQUEST::CONVERT_FLAG, E, T>::V) != 0;
            }
        };

        // ============================ [ SIZE ] =======================================
        template<class T, gbool Condition = TEST<REQUEST::COMPLET_FLAG, T>::V != 0>
        class MEMORY_SIZE {
        public:
            static CORE_FAST gint V = sizeof(T);
        };

        template<class T>
        class MEMORY_SIZE<T, false> {
        public:
            static CORE_FAST gint V = 0;
        };
    }
} // core

#endif // CORE24_TEMPLATES_H
