# Namespaces

## core

***
This namespace is kernel of _[CORE 24 Library](index.md#core-24)_.

Provides classes that are fundamental to the design of the
Core-24 programming framework. The most important classes are
`Object`, which is the root of the class hierarchy, and
`Class`, instances of which represent classes at run time.

Frequently it is necessary to represent a value of primitive
type as if it were an object. The wrapper classes `Boolean`,
`Character`, `Integer`, `Long`, `Float`,
and `Double` serve this purpose. An object of type
`Double`, for example, contains a field whose type is double,
representing that value in such a way that a reference to it can be
stored in a variable of reference type. These classes also provide
a number of methods for converting among primitive values, as well
as supporting such standard methods as equals and hash. The
`Void` class is a non-instantiable class that holds a
reference to a `Class` object representing the type void.

The class `Math` provides commonly used mathematical
functions such as sine, cosine, and square root. The classes
`String` and `XString` similarly
provide commonly used operations on character strings.

Class `Throwable` encompasses objects that may be thrown
by the `throw` statement. Subclasses of `Throwable`
represent errors and exceptions.

### Character Encodings

The specification of the [Charset](core/charset/charset.md)
class describes the naming conventions
for character encodings as well as the set of standard encodings
that must be supported by every implementation of the Java
platform.

***

## Classes
- [Appendable](core/appendable.md)
- [ArithmeticException](core/exceptions.md#ArithmeticException)
- [Array](core/array.md)
- [AssertionError](core/exceptions.md#AssertionError)
- [AutoClosable](core/auto_closable.md)
- [Boolean](core/boolean.md)
- [BooleanArray](core/boolean_array.md#BooleanArray)
- [BooleanArray2D](core/boolean_array.md#BooleanArray2D)
- [BooleanArray3D](core/boolean_array.md#BooleanArray3D)
- [Byte](core/byte.md)
- [ByteArray](core/byte_array.md#ByteArray)
- [ByteArray2D](core/byte_array.md#ByteArray2D)
- [ByteArray3D](core/byte_array.md#ByteArray3D)
- [Character](core/character.md)
- [CharArray](core/char_array.md#CharArray)
- [CharArray2D](core/char_array.md#CharArray2D)
- [CharArray3D](core/char_array.md#CharArray3D)
- [CharSequence](core/char_sequence.md)
- [Class](core/class.md)
- [ClassCastException](core/exceptions.md#ClassCastException)
- [CloneNotSupportedException](core/exceptions.md#CloneNotSupportedException)
- [Comparable](core/comparable.md)
- [Complex](core/complex.md)
- [Double](core/double.md)
- [DoubleArray](core/double_array.md#DoubleArray)
- [DoubleArray2D](core/double_array.md#DoubleArray2D)
- [DoubleArray3D](core/double_array.md#DoubleArray3D)
- [Enum](core/enum.md)
- [Error](core/error.md)
- [Exception](core/exception.md)
- [Float](core/float.md)
- [FloatArray](core/float_array.md#FloatArray)
- [FloatArray2D](core/float_array.md#FloatArray2D)
- [FloatArray3D](core/float_array.md#FloatArray3D)
- [IllegalArgumentException](core/exceptions.md#IllegalArgumentException)
- [IllegalStateException](core/exceptions.md#IllegalStateException)
- [IllegalThreadStateException](core/exceptions.md#IllegalThreadStateException)
- [IndexOutOfBoundsException](core/exceptions.md#IndexOutOfBoundsException)
- [IntArray](core/int_array.md#IntArray)
- [IntArray2D](core/int_array.md#IntArray2D)
- [IntArray3D](core/int_array.md#IntArray3D)
- [Integer](core/integer.md)
- [InterruptedException](core/exceptions.md#InterruptedException)
- [Iterable](core/iterable.md)
- [Long](core/long.md)
- [LongArray](core/long_array.md#LongArray)
- [LongArray2D](core/long_array.md#LongArray2D)
- [LongArray3D](core/long_array.md#LongArray3D)
- [Math](core/math.md)
- [Number](core/number.md)
- [NumberFormatException](core/exceptions.md#NumberFormatException)
- [Object](core/object.md)
- [OutOfMemoryError](core/exceptions.md#OutOfMemoryError)
- [Runnable](core/runnable.md)
- [RuntimeException](core/exceptions.md#RuntimeException)
- [SecurityException](core/exceptions.md#SecurityException)
- [Short](core/short.md)
- [ShortArray](core/short_array.md#ShortArray)
- [ShortArray2D](core/short_array.md#ShortArray2D)
- [ShortArray3D](core/short_array.md#ShortArray3D)
- [String](core/string.md)
- [StringArray](core/string_array.md)
- [Thread](core/thread.md)
- [Throwable](core/throwable.md)
- [Trace](core/trace.md)
- [UnsupportedOperationException](core/exceptions.md#UnsupportedOperationException)
- [Void](core/void.md)
- [XString](core/x_string.md)


