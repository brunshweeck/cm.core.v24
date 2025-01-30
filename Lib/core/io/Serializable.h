//
// Created by bruns on 01/10/2024.
//

#ifndef CORE24_SERIALIZABLE_H
#define CORE24_SERIALIZABLE_H

#include <core/Object.h>

namespace core {
    namespace io {
        /**
         * Serializability of a class is enabled by the class implementing the
         * Serializable interface.
         *
         * <p>
         * <b>Warning: Deserialization of untrusted data is inherently dangerous
         * and should be avoided.</b>
         * </p>
         *
         * Classes that do not implement this
         * interface will not have any of their state serialized or
         * deserialized.  All subtypes of a serializable class are themselves
         * serializable.  The serialization interface has no methods or fields
         * and serves only to identify the semantics of being serializable. <p>
         *
         * It is possible for subtypes of non-serializable classes to be serialized
         * and deserialized. During serialization, no data will be written for the
         * fields of non-serializable superclasses. During deserialization, the fields of non-serializable
         * superclasses will be initialized using the no-arg constructor of the first (bottommost)
         * non-serializable superclass. This constructor must be accessible to the subclass that is being
         * deserialized. It is an error to declare a class Serializable if this is not
         * the case; the error will be detected at runtime. A serializable subtype may
         * assume responsibility for saving and restoring the state of a non-serializable
         * supertype's public, protected, and (if accessible) package-access fields.
         * </p>
         * <p>
         * When traversing a graph, an object may be encountered that does not
         * support the Serializable interface. In this case the
         * NotSerializableException will be thrown and will identify the class
         * of the non-serializable object.
         * </p>
         * <p>
         * Classes that require special handling during the serialization and
         * deserialization process must implement special methods with these exact
         * signatures:
         *
         * @code
         * private:
         *      void writeObject(java.io.ObjectOutputStream out) override;
         * private:
         *     void readObject(java.io.ObjectInputStream in) override;
         * @endcode
         * </p>
         * <p>
         * The writeObject method is responsible for writing the state of the
         * object for its particular class so that the corresponding
         * readObject method can restore it.  The default mechanism for saving
         * the Object's fields can be invoked by calling
         * out.defaultWriteObject. The method does not need to concern
         * itself with the state belonging to its superclasses or subclasses.
         * State is saved by writing the individual fields to the
         * ObjectOutputStream using the writeObject method or by using the
         * methods for primitive data types supported by DataOutput.
         * </p>
         * <p>
         * The readObject method is responsible for reading from the stream and
         * restoring the classes fields. It may call in.defaultReadObject to invoke
         * the default mechanism for restoring the object's non-static and
         * non-transient fields.  The defaultReadObject method uses information in
         * the stream to assign the fields of the object saved in the stream with the
         * correspondingly named fields in the current object.  This handles the case
         * when the class has evolved to add new fields. The method does not need to
         * concern itself with the state belonging to its superclasses or subclasses.
         * State is restored by reading data from the ObjectInputStream for
         * the individual fields and making assignments to the appropriate fields
         * of the object. Reading primitive data types is supported by DataInput.
         * </p>
         * <p>
         * This readResolve method follows the same invocation rules and
         * accessibility rules as writeReplace.
         * </p>
         * <p>
         * Enum types are all serializable. Any declarations of the special
         * handling methods discussed above are ignored for enum types.
         * </p>
         *
         * @see ObjectWriter
         * @see ObjectReader
         */
        class Serializable : public virtual Object {
        protected:
            CORE_IMPLICIT Serializable() = default;

        private:
            virtual void readObject(ObjectReader &reader) const = 0;

            virtual void writeObject(ObjectWriter &writer) const = 0;
        };
    } // io
} // core

#endif //CORE24_SERIALIZABLE_H
