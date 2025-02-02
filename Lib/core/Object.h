//
// Created by bruns on 08/05/2024.
//

#ifndef CORE24_OBJECT_H
#define CORE24_OBJECT_H

#include <core/misc/Literals.h>

namespace core {
    /**
     * The class @c Object is a root of class hierachy
     */
    class Object {
    protected:
        CORE_ADD_GLOBAL_FRIENDS();
        CORE_ALIAS(UNSAFE, misc::Unsafe);

        /**
         * The @c Object constructor is only accessible via inherited class
         */
        CORE_IMPLICIT CORE_FAST Object() = default;

    public:
        /**
         * Destroy this object
         */
        virtual ~Object() = default;

        /**
         * Indicates whether some other object is "equal to" this one.
         *
         * The @c equals method implements an equivalence relation
         * on non-null object references:
         * @li It is @a reflexive : For any reference value @c x,
         *     @c x.equals(x) should return @c true.
         * @li It is @a symetric : For any reference values @c x and @c y,
         *     @c x.equals(y) should return @c true if and only if @c y.equals(x)
         *     return @c true.
         * @li It is @a transitive : For any reference values @c x, @c y and @c z,
         *     if @c x.equals(y) return @c true, and @c y.equals(z) return @c true,
         *     then @c x.equals(z) should be return @c true
         * @li It is @a consistant : For any reference value @c x and @c y,
         *     mutliple invocation of @c x.equals(y) consistently return @c true,
         *     or consistently return @c false provided no information used in
         *     @c equals  comparisons on the objects is modified.
         *
         * @param other The object to be compared
         * @return @c true if object instance equals to given object.
         */
        virtual gbool equals(Object const &other) const;

        /**
         * Returns a hash code value for the object. This method is
         * supported for the benefit of hash tables such as those provided by
         * @c core::util::HashTable.
         *
         * The general contract of @c hash is:
         * @li Whenever it is invoked on the same object more than once during
         *     an execution of a application, the @c hash method
         *     must consistently return the same integer, provided no information
         *     used in @c equals comparisons on the object is modified.
         *     This integer need not remain consistent from one execution of an
         *     application to another execution of the same application.
         * @li If two objects are equal according to the @c equals
         *     method, then calling the @c hash
         *     method on each of the two objects must produce the
         *     same integer result.
         * @li It is @a not required that if two objects are unequal
         *     according to the @c equals method, then calling the
         *     @c hash method on each of the two objects must produce
         *     distinct integer results.  However, the programmer
         *     should be aware that producing distinct integer results for
         *     unequal objects may improve the performance of hash tables.
         *
         * @note
         *     As far as is reasonably practical, the @c hash method defined
         *     by class @c Object returns distinct integers for distinct objects.
         *
         * @return The hash code value for this object
         */
        virtual gint hash() const;

        /**
         * Returns a string representation of the object.
         *
         * In general, the @c toString method returns a string that
         * "textually represents" this object. The result should
         * be a concise but informative representation that is easy for a
         * person to read.
         *
         * @note It is recommended that all subclasses override this method.
         *
         * The @c toString method for class @c Object returns a string consisting
         * of the name of the class of which the object is an instance, the at-sign
         * character @c '@', and the unsigned hexadecimal representation of the hash
         * code of the object. In other words, this method returns a string equal to the
         * value of: @c typeInfo().name() + @c "@" + @c Integer::toHexString(hash())
         *
         * @return  a string representation of the object.
         */
        virtual String toString() const;

        /**
         * Create and return the shadow and dynamic copy of this object.
         * The precise meaning @em copy may depend on the class of object.
         * The General intent is that, for any object @c x, the expression:
         * @code &x.clone() != &x @endcode will be @c true and that the
         * expression @code x.clone().classInfo() == x.classInfo() @endcode
         * will be @c true, but these are not absolute requirement.
         * While it is typically the case that: @code x.clone().equals(x) @endcode
         * or @code x.clone() == x @endcode will be @c true, this is not absolute
         * requirement.
         *
         * <p>
         * By convention, the returned object should be independent for this
         * object (which is being cloned)
         * </p>
         *
         * @note The method @c clone for class @c Object performs a
         * specific cloning operation. First, if the class of this object does
         * support this operation, then a @c CloneNotSupportedException is thrown.
         * Otherwise, this method creates a new instance of the class of this
         * object and initializes all its fields with exactly the contents of
         * the corresponding fields of this object, as if by assignment; the
         * contents of the fields are not themselves cloned. Thus, this method
         * performs a "shallow copy" of this object, not a "deep copy" operation.
         *
         * <p>
         * The class @c Object does not itself support the clone operation,
         * so calling the @c clone method on an object whose class is
         * @c Object will result in throwing an exception at run time.
         * </p>
         *
         * @return the shadow copy of this object.
         * @throws CloneNotSupportedException If the object's class does not
         *               support the @c clone operation. Subclasses
         *               that override the @c clone method can also
         *               throw this exception to indicate that an instance cannot
         *               be cloned.
         */
        virtual Object &clone() const;

        /**
         * Return the string representing the typename of
         * specified object.
         *
         * @return the typename of this class
         */
        static String typeName(Object const &obj);

        /**
         * Returns the same hash code for the given object as
         * would be returned by the default method hash(),
         * whether the given object's class overrides hash().
         * The hash code for the null reference is zero.
         *
         * @param x object for which the hash is to be calculated
         * @return  the hash code
         * @see Object::hash
         */
        static gint identityHash(Object const &x);
    };
} // core

#endif // CORE24_OBJECT_H
