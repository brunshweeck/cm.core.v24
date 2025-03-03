//
// Created by bruns on 05/05/2024.
//

#ifndef CORE24_CLASS_H
#define CORE24_CLASS_H

#include <core/misc/Literals.h>
#include <core/misc/Nil.h>

namespace core {
  /**
   * @c Class is utility that provide multiples static methods and aliases
   * for respectively tests and modify. @c Class provide the conditional
   * aliases (simple with single conditions and multiple with multiple
   * conditions).
   *
   * @tparam T the type used for all defined Test and Aliases
   */
  template <class T>
  class Class final : public Object {
    CORE_ALIAS(REQUEST, misc::REQUEST);
    template <gint R, class... C>
    CORE_ALIAS(TEST, misc::TEST<R, C...>);
    template <gint R, class C, glong N = -1>
    CORE_ALIAS(TRANSFORM, misc::TRANSFORM<R, C, N>);
    template <class C, gbool Cnd>
    CORE_ALIAS(ONLY_IF, misc::ONLY_IF<C, Cnd>);
    template <class C1, class C2, gbool Cnd>
    CORE_ALIAS(IF_OR_ELSE, misc::IF_OR_ELSE<C1, C2, Cnd>);
    template <class C>
    CORE_ALIAS(IGNORED, misc::IGNORED<C>);

    CORE_IMPLICIT Class() = default;

  public:
    // ======================= [ type native versions ] =======================

    // Constant version of T

    /**
     * Obtains the const version of T
     * @code
     *  gint a = 23;
     *  using C = ClassOf(a)::Constant; // C = const gint
     * @endcode
     */
    CORE_ALIAS(Constant, $(typename TRANSFORM<REQUEST::CONST_QLF, T>::T));

    // Mutable version of T

    /**
     * Obtains the non-const version of T
     * @code
     *  const gint a = 23;
     *  using C = ClassOf(a)::Mutable; // C = gint
     * @endcode
     */
    CORE_ALIAS(Mutable, $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::REMOVE_QLF, T>::T));

    // Constant version of T

    /**
     * @copydoc Class::Constant
     */
    CORE_ALIAS(Const, Constant);

    // Mutable version of T

    /**
     * @copydoc Class::Mutable
     */
    CORE_ALIAS(NoConstant, Mutable);

    // Mutable version of T

    /**
     * @copydoc Class::NoConstant
     */
    CORE_ALIAS(NConst, NoConstant);

    // Volatile version of T

    /**
     * Obtains the volatile version of T
     * @code
     *  gint a = 23;
     *  using C = ClassOf(a)::Volatile; // C = volatile gint
     * @endcode
     */
    CORE_ALIAS(Volatile, $(typename TRANSFORM<REQUEST::VOL_QLF, T>::T));

    // Non-Volatile version of T

    /**
     * Obtains the non-volatile version of T
     * @code
     *  volatile gint a = 23;
     *  using C = ClassOf(a)::NoVolatile; // C = gint
     * @endcode
     */
    CORE_ALIAS(NoVolatile, $(typename TRANSFORM<REQUEST::VOL_QLF | REQUEST::REMOVE_QLF, T>::T));

    // Constant and Volatile version of T

    /**
     * Obtains the const volatile version of T
     * @code
     *  gint a = 23;
     *  using C = ClassOf(a)::ConstVolatile; // C = const volatile gint
     * @endcode
     */
    CORE_ALIAS(ConstVolatile, $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF, T>::T));

    // Mutable and Non-Volatile version of T

    /**
     * Obtains the non-const non-volatile version of T
     * @code
     *  const volatile gint a = 23;
     *  const gint b = 23;
     *  volatile gint c = 23;
     *  using C1 = ClassOf(a)::NoConstVolatile; // C1 = gint
     *  using C2 = ClassOf(b)::NoConstVolatile; // C2 = gint
     *  using C3 = ClassOf(c)::NoConstVolatile; // C3 = gint
     * @endcode
     */
    CORE_ALIAS(NoConstVolatile,
               $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REMOVE_QLF, T>::T));

    // Volatile version of T

    /**
     * @copydoc Class::Volatile
     */
    CORE_ALIAS(Vol, Volatile);

    // Constant and Volatile version of T

    /**
     * @copydoc Class::ConstVolatile
     */
    CORE_ALIAS(CVol, Volatile);

    // Non-Volatile version of T

    /**
     * @copydoc Class::NoVolatile
     */
    CORE_ALIAS(NVol, NoVolatile);

    // Mutable and Non-Volatile version of T

    /**
     * @copydoc Class::NoConstVolatile
     */
    CORE_ALIAS(NConstVol, NoConstVolatile);

    // Mutable and Non-Volatile version of T

    /**
     * @copydoc Class::NoConstVolatile
     */
    CORE_ALIAS(NCVol, NConstVol);

    // Pointer version of T

    /**
     * Obtains the pointer type of T
     * @code
     *  gint a = 23;
     *  using P = ClassOf(a)::Pointer; // P = gint*
     * @endcode
     */
    CORE_ALIAS(Pointer, $(typename TRANSFORM<REQUEST::PTR_QLF, T>::T));

    // Pointer Target of Pointer type T

    /**
     * Obtains the target of pointer type T
     * @code
     *  gint *a = null;
     *  using T = ClassOf(a)::PointerTarget; // T = gint
     * @endcode
     */
    CORE_ALIAS(PointerTarget, $(typename TRANSFORM<REQUEST::PTR_QLF | REQUEST::REMOVE_QLF, T>::T));

    // Non-Pointer version of T

    /**
     * @copydoc Class::PointerTarget
     */
    CORE_ALIAS(NoPointer, PointerTarget);

    // Pointer version of T

    /**
     * @copydoc Class::Pointer
     */
    CORE_ALIAS(Ptr, Pointer);

    // Non-Pointer version of T

    /**
     * @copydoc Class::NoPointer
     */
    CORE_ALIAS(NPtr, NoPointer);

    /**
     * The @c PointerLv represent the multidimensional pointer.
     *
     * Example:
     * @code
     *  Class<String>::PointerLv<2> => String**
     * @endcode
     *
     * @tparam N the pointer dimension
     */
    template <gint N>
    CORE_ALIAS(PointerLv, $(typename misc::TRANSFORM_PTR_LV<T, N>::T));

    // Reference version of T (Left value)

    /**
     * Obtains the lvalue reference type of T
     * @code
     *  gint a = 23;
     *  using C = ClassOf(a)::Reference; // C = gint&
     * @endcode
     */
    CORE_ALIAS(Reference, $(typename TRANSFORM<REQUEST::REF_QLF, T>::T));

    // Constant Reference version of T
    CORE_ALIAS(ConstReference, $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF, T>::T));

    // Volatile Reference version of T
    CORE_ALIAS(VolReference, $(typename TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF, T>::T));

    // Constant and Volatile Reference version of T
    CORE_ALIAS(ConstVolReference,
               $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REF_QLF, T>::T));

    // Reference version of T (Right value)
    CORE_ALIAS(Rvalue, $(typename TRANSFORM<REQUEST::RVAL_QLF, T>::T));

    // Non-Reference version of T
    CORE_ALIAS(NoReference, $(typename TRANSFORM<REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T>::T));

    // Mutable and Non-Reference version of T
    CORE_ALIAS(NoConstReference,
               $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T>::T));

    // Non-Volatile and Non-Reference version of T
    CORE_ALIAS(NoVolReference,
               $(typename TRANSFORM<REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::REMOVE_QLF, T>::T));

    // Mutable, Non-Volatile and Non-Reference version of T
    CORE_ALIAS(NoConstVolReference,
               $(typename TRANSFORM<REQUEST::CONST_QLF | REQUEST::VOL_QLF | REQUEST::REF_QLF | REQUEST::
                 REMOVE_QLF, T>::T
               ));

    // Reference version of T
    CORE_ALIAS(Ref, Reference);

    // Constant Reference version of T
    CORE_ALIAS(CRef, ConstReference);

    // Volatile Reference version of T
    CORE_ALIAS(VRef, VolReference);

    // Constant Volatile Reference version of T
    CORE_ALIAS(CVRef, ConstVolReference);

    // Non-Reference version of T
    CORE_ALIAS(NRef, NoReference);

    // Mutable and Non-Reference version of T
    CORE_ALIAS(NConstRef, NoConstReference);

    // Non-Volatile version of T
    CORE_ALIAS(NVolRef, NoVolReference);

    // Mutable Non-Volatile and Non-Reference version of T
    CORE_ALIAS(NConstVolRef, NoConstVolReference);

    // Mutable and Non-Reference version of T
    CORE_ALIAS(NCRef, NConstRef);

    // Mutable and Non-Reference version of T
    CORE_ALIAS(NVRef, NVolRef);

    // Mutable Non-Volatile and Non-Reference version of T
    CORE_ALIAS(NCVRef, NConstVolRef);

    // Arrays version of T

    /**
     * Obtains the array of T with given number of values.
     * @code
     *  using A = ClassOf(1)::Array<5>;
     *  A a = {1, 2, 3}; //ok (A = gint[5])
     * @endcode
     *
     * @note T may be complete, non-abstact and public default constructible type to evict error.
     * @code
     *  using A = ClassOf(null)::Array<5>; // error
     * @endcode
     *
     * @tparam N the length of array (maybe only positive value between 0 and Integer::MAX_VALUE)
     *
     * @see Class::isComplete()
     * @see Class::isAbstract()
     * @see Class::isConstructible()
     */
    template <glong N>
    CORE_ALIAS(Array, $(typename TRANSFORM<REQUEST::ARRAY_QLF, NRef, N>::T));

    // Array Element type of Array type @c T

    /**
     * Obtain the type of value that contains T.
     * @code
     *  gint a[] = {1, 2, 3}
     *  using AE = ClassOf(a)::ArrayElement;
     *  AE ae = a[2]; // ok (AE = gint)
     * @endcode
     *
     * @note T may be an array type. otherwise this alias
     * has no effect, such as:
     * @code
     *  auto a  = IntArray::of(1, 2, 3);
     *  using AE = ClassOf(a)::ArrayElement;
     *  AE b = a[2]; // error (AE = IntArray)
     * @endcode
     *
     */
    CORE_ALIAS(ArrayElement, $(typename TRANSFORM<REQUEST::ARRAY_QLF | REQUEST::REMOVE_QLF, NCVRef>::T));

    // Mutli-densionnal array

    /**
     * Obtains the matrix of T with given number of place by dimension.
     * @code
     *  using M5x4 = ClassOf(1)::Matrix<5, 4>;
     *  M5x4 m = {
     *      {0, 1, 2, 3},
     *      {4, 5, 6, 7},
     *      {8, 9, 0, 1},
     *      {2, 3, 4, 5},
     *      {6, 7, 8, 9}
     *  }; // ok (M5x4 = gint[5][4])
     * @endcode
     *
     * @note T may be complete, non-abstact and public default constructible type to evict error.
     * @code
     *  using M = ClassOf(null)::Matrix<5, 4>; // error
     * @endcode
     *
     * @tparam N the length array respectively in all dimensions
     */
    template <gint ... N>
    CORE_ALIAS(Matrix, $(typename misc::TRANSFORM_ARRAY_LV<NRef, N...>::T));

    /**
     * Obtains the Slimmed version of T.
     *
     * This version is recursive @c Mutable, recursive @c NoVolatile,
     * and recursive @c NoReference version of @c T
     */
    CORE_ALIAS(Slim, $(typename TRANSFORM<REQUEST::SLIM_FLAG, NCVRef>::T));

    // ======================= [ type conditional test ] =======================

    // Irreversible conditions
    /**
     * Enable the template type (@c T), if and only if
     * the condition result is @c true.
     * @code
     *  if (condition)
     *  {
     *      ...
     *  }
     *
     *  template <class T>
     *  using AlwaysNumber = Class<T>::OnlyIf<Class<Number>::isSuper<T>()>;
     *
     *  AlwaysNumber<Double> // Double
     *  AlwaysNumber<String> // error
     * @endcode
     *
     * @param Condition the condition result
     */
    template <gbool Condition>
    CORE_ALIAS(OnlyIf, $(typename ONLY_IF<T, Condition>::T));

    // Reversible conditions
    /**
     * Enable the template type (@c T), if and only if
     * the condition result is @c true.
     * @code
     *  if (condition) ... else ...
     *
     *  template <class T>
     *  using AlwaysNumber = Class<T>::IfElse<Class<Number>::isSuper<T>(), Integer>;
     *
     *  AlwaysNumber<Double> // Double
     *  AlwaysNumber<String> // Integer
     * @endcode
     *
     * @param Condition the condition result
     */
    template <gbool Condition, class U>
    CORE_ALIAS(IfElse, $(typename IF_OR_ELSE<T, U, Condition>::T));

    // ======================= [ type status ] =======================

    /**
     * Test if type @c T is same to specified type @c To
     *
     * @tparam To the type to be tested.
     */
    template <class To>
    static CORE_FAST gbool isSame() { return TEST<REQUEST::SAME_FLAG, NCVRef, typename Class<To>::NCVRef>::V != 0; }

    template <class... To>
    static CORE_FAST gbool isSameToOne() { return oneIsTrue<isSame<To>()...>(); }

    /**
     * Test if type @c T is constant. in other word,
     * this method test if type @c T have qualifier
     * @c const.
     */
    static CORE_FAST gbool isConstant() { return TEST<REQUEST::CONST_QLF, NVRef>::V != 0; }

    /**
     * Test if type @c T is volatile. in other word,
     * this method test if type @c T have qualifier
     * @c volatile.
     */
    static CORE_FAST gbool isVolatile() { return TEST<REQUEST::VOL_QLF, NCRef>::V != 0; }

    /**
     * Test if type @c T is pointer. in other word,
     * this method test if type @c T have qualifier
     * @c *.
     */
    static CORE_FAST gbool isPointer() { return TEST<REQUEST::PTR_QLF, NCVRef>::V != 0; }

    /**
     * Test if type @c T is reference. in other word,
     * this method test if type @c T have qualifier
     * @c & or qualifier @c &&.
     */
    static CORE_FAST gbool isReference() { return TEST<REQUEST::REF_QLF, NCVol>::V != 0; }

    /**
     * Test if type @c T is reference (lvalue). in other word,
     * this method test if type @c T have qualifier
     * @c &.
     */
    static CORE_FAST gbool isLvalue() { return isReference() && !isRvalue(); }

    /**
     * Test if type @c T is reference (rvalue). in other word,
     * this method test if type @c T have qualifier
     * @c &&.
     */
    static CORE_FAST gbool isRvalue() { return TEST<REQUEST::RVAL_QLF, NCVol>::V != 0; }

    /**
     * Test if type @c T is array. in other word,
     * this method test if type @c T have qualifier
     * @c [N] where N is length of @c T.
     */
    static CORE_FAST gbool isArray() { return TEST<REQUEST::ARRAY_QLF, NCVRef>::V != 0; }

    /**
     * Test if type @c T is complete type.
     * The complete type is type that definition
     * found or type that have size.
     */
    static CORE_FAST gbool isComplete() {
      return (TEST<REQUEST::COMPLET_FLAG, NCVRef>::V
        | TEST<REQUEST::COMPLET_FLAG | REQUEST::ARRAY_QLF, NCVRef>::V) != 0;
    }

    /**
     * Test if type @c T is template. in other word,
     * this method test if type @c T is defined like
     * this : template <...> class T;
     */
    static CORE_FAST gbool isTemplate() { return TEST<REQUEST::TEMPL_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is function. in other word,
     * this method test if type @c T have signature
     * @c R(*)(A...) or @c R(*)(A...) where R is return
     * type and A is arguments types or @c T is lambda
     * function without capture list.
     */
    static CORE_FAST gbool isFunction() { return TEST<REQUEST::FUNCTION_QLF, NCVRef>::V != 0; }

    /**
     * Test if type @c T is class member.
     */
    static CORE_FAST gbool isMember() { return TEST<REQUEST::MEMBER_FLAG, NRef>::V != 0; }

    /**
     * Test if type @c T is function member. in other word,
     * this method test if type @c T is non-static method.
     */
    static CORE_FAST gbool isFunctionMember() { return TEST<REQUEST::METH_QLF, NCVRef>::V != 0; }

    /**
     * Test if type @c T is abstract class. in other
     * words, one or more methods of class T is pure
     * virtual
     */
    static CORE_FAST gbool isAbstract() { return TEST<REQUEST::ABSTRACT_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is enum type.
     */
    static CORE_FAST gbool isEnum() { return TEST<REQUEST::ENUM_QLF, NCVRef>::V != 0; }

    /**
     * Test if type @c T is class or union type.
     */
    static CORE_FAST gbool isClass() { return TEST<REQUEST::CLASS_QLF, NCVRef>::V != 0 || __is_union(T); }

    /**
     * Test if type @c T is able to content others values.
     */
    static CORE_FAST gbool isAggregate() { return isClass() || isArray(); }

    /**
     * Test if type @c T is destructible. in other word,
     * this method test if type @c T define destructor
     * method.
     */
    static CORE_FAST gbool isDestructible() { return TEST<REQUEST::DTOR_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is not inheritable.
     */
    static CORE_FAST gbool isFinal() { return TEST<REQUEST::FINAL_QLF, NCVRef>::V != 0; }

    /**
     * Test if type @c T is empty. in other word,
     * this method test if type @c T have not content
     * (fields, methods, etc...)
     */
    static CORE_FAST gbool isEmpty() { return TEST<REQUEST::EMPTY_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is literal type.
     */
    static CORE_FAST gbool isLiteral() { return TEST<REQUEST::LITERAL_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is polymorphic type.
     */
    static CORE_FAST gbool isPolymorphic() { return TEST<REQUEST::POLY_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is trivial type.
     */
    static CORE_FAST gbool isTrivial() { return TEST<REQUEST::TRIVIAL_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is integer number type.
     */
    static CORE_FAST gbool isInteger() {
      return (TEST<REQUEST::INT_FLAG, NCVRef>::V
        | TEST<REQUEST::SAME_FLAG, NCVRef, int>::V
        | TEST<REQUEST::SAME_FLAG, NCVRef, long>::V
        | TEST<REQUEST::SAME_FLAG, NCVRef, long long>::V) != 0;
    }

    /**
     * Test if type @c T is floating/decimal number type.
     */
    static CORE_FAST gbool isFloating() { return TEST<REQUEST::FLT_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is integer or floating number type.
     */
    static CORE_FAST gbool isNumber() { return isInteger() || isFloating(); }

    /**
     * Test if type @c T is integral type
     * (integer number, enumeration type, characters, boolean).
     */
    static CORE_FAST gbool isIntegral() { return isInteger() || isEnum() || isCharacter() || isBoolean(); }

    /**
     * Test if type @c T is boolean type.
     */
    static CORE_FAST gbool isBoolean() { return TEST<REQUEST::BOOLEAN_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is void type.
     */
    static CORE_FAST gbool isVoid() { return TEST<REQUEST::VOID_FLAG, NCVRef>::V != 0; }

    /**
     * Test if type @c T is primitive type.
     */
    static CORE_FAST gbool isPrimitive() {
      return isVoid() || isString() || isCharacter() || isBoolean() || isNumber() || isArray() || isPointer();
    }

    /**
     * Test if type @c T is character type.
     */
    static CORE_FAST gbool isCharacter() {
      return (TEST<REQUEST::CHR_FLAG, NCVRef>::V | TEST<REQUEST::SAME_FLAG, NCVRef, char>::V) != 0;
    }

    /**
     * Test if type @c T is string type.
     */
    static CORE_FAST gbool isString() {
      return TEST<REQUEST::STR_FLAG, NCVRef>::V != 0 ||
          (isArray() && Class<ArrayElement>::isCharacter()) ||
          (isPointer() && Class<PointerTarget>::isCharacter());
    }

    /**
     * Check if this template type (@c T) has no qualifiers: @c const, @c &, @c && and @c volatile
     * @return @c true if @c T has no qualifiers: @c const, @c &, @c && and @c volatile
     */
    static CORE_FAST gbool isSlimmed() { return TEST<REQUEST::SLIM_FLAG, T>::V != 0; }

    /**
     * Test if type @c T support operator equals.
     */
    static CORE_FAST gbool supportEQ() { return TEST<REQUEST::EQ_FLAG, NRef>::V != 0; }

    /**
     * Test if type @c T support operator less than.
     */
    static CORE_FAST gbool supportLT() { return TEST<REQUEST::LT_FLAG, NRef>::V != 0; }

    /**
     * Test if type @c T is default constructible.
     */
    static CORE_FAST gbool isConstructible() { return TEST<REQUEST::CTOR_FLAG, NCVRef, IGNORED<T>>::V != 0; }

    /**
     * Test if type @c T is constructible with specified arguments types.
     *
     * @tparam Args the arguments type
     */
    template <class... Args>
    static CORE_FAST gbool isConstructible() { return TEST<REQUEST::CTOR_FLAG, NCVRef, Args...>::V != 0; }

    /**
     * Test if type @c T is assignable to specified type.
     *
     * @tparam To the expected type.
     */
    template <class To>
    static CORE_FAST gbool isAssignable() { return TEST<REQUEST::ASSIGN_FLAG, T, To>::V != 0; }

    /**
     * Test if type @c T is base class of specified type @c ExtendsClass.
     *
     * @tparam ExtendsClass the derived class
     */
    template <class ExtendsClass>
    static CORE_FAST gbool isSuper() {
      CORE_ALIAS(Super, NCVRef);
      CORE_ALIAS(Extends, typename Class<ExtendsClass>::NCVRef);
      return TEST<REQUEST::SUPER_FLAG, Super, Extends>::V != 0;
    }

    /**
     * Test if type @c T is derived class of specified type @c SuperClass.
     *
     * @tparam SuperClass the base class
     */
    template <class SuperClass>
    static CORE_FAST gbool isExtends() {
      CORE_ALIAS(Super, typename Class<SuperClass>::NCVRef);
      CORE_ALIAS(Extends, NCVRef);
      return TEST<REQUEST::SUPER_FLAG, Super, Extends>::V != 0;
    }

    /**
     * Test if type @c T is implicitly convertible to specified type @c ExtendsClass.
     *
     * @tparam To the target class
     */
    template <class To>
    static CORE_FAST gbool isConvertible() {
      return TEST<REQUEST::CONVERT_FLAG, T, To>::V != 0
          || (Class<To>::isVoid() && (isArray() || isVoid() || isFunction()))
          || (isSame<To>() && !isArray());
    }

    /**
     * Test if type @c T is callable without argument
     */
    static CORE_FAST gbool isCallable() { return TEST<REQUEST::CALL_FLAG, T, IGNORED<T>>::V != 0; }

    /**
     * Test if type @c T is callable with specified argument types
     *
     * @tparam Args the argument type list
     */
    template <class... Args>
    static CORE_FAST gbool isCallable() { return TEST<REQUEST::CALL_FLAG, T, Args...>::V != 0; }

    /**
     * Test if specified value of type @c ExtendsClass is instance of type @c T
     *
     * @tparam E the expected value type
     *
     * @param x the value to be tested
     */
    template <class E>
    static CORE_FAST gbool hasInstance(E const& x) {
      CORE_ALIAS(CHECKER,
                 typename TEST<REQUEST::SUPER_FLAG, NCVRef>::template
                 IMPL<isClass() && Class<E>::isClass()>);
      CORE_ALIAS(CHECKER2,
                 typename TEST<REQUEST::SUPER_FLAG, NCVRef>::template
                 IMPL2<isPointer() && Class<E>::isPointer(), isClass() && Class<E>::isClass()>);

      return isSuper<E>() ||
          Class<Slim>::template isSame<typename Class<E>::Slim>() ||
          CHECKER::checkInstance(x) || CHECKER2::checkInstance(x);
    }

    /**
     * Test if all given conditions is true
     *
     * @tparam Conditions the booleans values to be tested
     * @return @c true if all given conditions is @c true
     */
    template <gbool ... Conditions>
    static CORE_FAST gbool allIsTrue() { return misc::ALL_IS_TRUE<Conditions...>::V != 0; }

    /**
     * Test and return @c true if one of given conditions
     * is true.
     *
     * @tparam Conditions the booleans values to be tested
     * @return @c true if one of given conditions is true
     */
    template <gbool ... Conditions>
    static CORE_FAST gbool oneIsTrue() { return misc::ONE_IS_TRUE<Conditions...>::V != 0; }

    /**
     * Return the memory size of an instance of this template parameter.
     *
     * @return the memory size of an instance of this template parameter.
     */
    static CORE_FAST gint size() { return TEST<REQUEST::COMPLET_FLAG, NCVRef>::SIZE; }

    /**
     * Return the number element supported by an instance of this template parameter
     * is it's array. in other case the value returned is 1.
     *
     * @return the memory size of an instance of this template parameter.
     */
    static CORE_FAST gint count() { return size() ? size() / Class<ArrayElement>::size() : 1; }

    /**
     * @c Class::OnlyIfAll alias represent the variant of @c Class::OnlyIf alias
     * that support multiple conditions.
     *
     * <p>
     * This alias combines @c Class::allIsTrue() method and @c Class::OnlyIf.
     * In otherword this allias will evalute logical or between given
     * conditions to product one condition via @c Class::allIsTrue(), and
     * Tested the resulted condition via @c Class::OnlyIf alias to
     * product T
     * </p>
     *
     * @tparam Conditions the conditions to be tested
     *
     * @see Class::OnlyIf
     */
    template <gbool ... Conditions>
    CORE_ALIAS(OnlyIfAll, $(typename ONLY_IF<T, misc::ALL_IS_TRUE<Conditions...>::V>::T));

    template <gbool ... Conditions>
    CORE_ALIAS(OnlyIfOne, $(typename ONLY_IF<T, misc::ONE_IS_TRUE<Conditions...>::V>::T));

  private:
    template <class... A>
    CORE_ALIAS(Rtype, $(typename TEST<REQUEST::CALL_FLAG, T, A...>::T));

  public:
    /**
     * Obtains the return type of any instance of target type
     * @c T if it's called with instances of specified types
     * as arguments.
     *
     * @note @c T may be the callable object (functions, lambda).
     *  If @c T represent the function member (non-static method)
     *  the fist argument should be the type of instance of object
     *  supporting this methods.
     *  @code
     *   class MyClass
     *   {
     *   public:
     *     String myMethod(gint, glong);
     *   };
     *
     *   class MySubClass : public virtual MyClass
     *   {
     *   };
     *
     *   class MyClass2
     *   {
     *   };
     *
     *   using R1 = ClassOf(&MyClass::myMethod)::Return<MyClass, gint, glong>; // R1 = String
     *   using R2 = ClassOf(&MyClass::myMethod)::Return<MySubClass, gint, glong>; // R2 = String
     *   using R3 = ClassOf(&MyClass::myMethod)::Return<MyClass2, gint, glong>; // error (MyClass2 has not this member)
     *   using R4 = ClassOf(&MyClass::myMethod)::Return<gint, glong>; // error (class instance type not found)
     *  @endcode
     *
     * @tparam Args the types of arguments values
     */
    template <class... Args>
    CORE_ALIAS(Return, $(typename Rtype<Args...>::T));

    // ======================= [ type compatibility ] =======================

    /**
     * The primitive version of T
     * @code
     *  using P = Class<Integer>::Prime;
     *  P a = 23; // ok (P = gint)
     * @endcode
     */
    CORE_ALIAS(Prime, $(typename TRANSFORM<REQUEST::PRIME_QLF, NCVRef>::T));

    /**
     * The object version of T
     * @code
     *  using P = ClassOf(1)::Object;
     *  P a = 23; // ok (P = Integer)
     * @endcode
     */
    CORE_ALIAS(Object, typename IF_OR_ELSE<Enum<NCVRef>,
                       typename TRANSFORM<REQUEST::CLASS_QLF, NCVRef>::T, isEnum()>::T);

    /**
     * Obtains the target type T
     * @code
     *  using A = ClassOf(2)::Target; // A = gint
     *  using B = ClassOf(null)::Target; // B = core::misc::NIL&
     *  using C = ClassOf(1+1i)::Target; // C = core::Complex
     * @endcode
     */
    CORE_ALIAS(Target, T);

    /**
     * Test If this type T can accept reference of specified
     * type for construction (unlikely Class::isConstructible() method, this
     * method return true if @c Arg extends publicly T, or T is constructible
     * with value of primitive/class version of @c Arg).
     *
     * <p> For Example
     * @code
     *      Class<Number>::accept<Integer>() // Return true
     *      Class<Number>::isConstructible<Integer>() // Return false
     * @endcode
     * </p>
     */
    template <class Arg>
    static CORE_FAST gbool accept() {
      CORE_ALIAS(Clazz, typename Class<Arg>::Object);
      CORE_ALIAS(Prim, typename Class<Arg>::Prime);
      return isPrimitive()
               ? isConstructible<Prim>() ||
               Class<Prime>::template isConvertible<T>()
               : isSuper<Clazz>() ||
               isConstructible<Clazz>() ||
               (isConstructible<Prim>() && Class<Prime>::template isConvertible<Clazz>()) ||
               Class<Arg>::template isConvertible<T>();
    }
  };


#ifndef ClassOf

#if defined(typeof) || __has_builtin(typeof) || __has_feature(typeof)
  /**
   * Retrieve template from any valid expression or type
   */
#define ClassOf(...) Class<typeof(__VA_ARGS__)>
#elif defined(__typeof) || __has_builtin(__typeof) || __has_feature(__typeof)
    /**
     * Retrieve template from any valid expression or type
     */
#define ClassOf(...) Class<__typeof(__VA_ARGS__)>
#else
    /**
     * Retrieve template from any valid expression
     */
#define ClassOf(...) Class<decltype(__VA_ARGS__)>
#endif

#endif
} // core

#endif // CORE24_CLASS_H
