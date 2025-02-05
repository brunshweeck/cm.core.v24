# Documentation

***

## CORE 24

*core 24* represents an optimized version of the standard system
linked to _C++ programming language_ in a vision, hierachizing all
object classes into types having a common base, thus producing better
traceability.

***

## Requirement

The use _CORE 24 C++ Library_, it required to have:

- C++ version >= 11
- Support of exception

## Namespaces

- [core](core.md)
- [core::charset](core.charset.md)
- [core::concurrent](core.concurrent.md)
- [core::function](core.function.md)
- [core::io](core.io.md)
- [core::misc](core.misc.md)
- [core::net](core.net.md)
- [core::random](core.random.md)
- [core::regex](core.regex.md)
- [core::stream](core.stream.md)
- [core::text](core.text.md)
- [core::time](core.time.md)
- [core::util](core.util.md)

## Correspondence with standard C++

For evolution the standard and primitive type are restricted to
stable sized type (_generic_ type).

### Primitive types

| New Types | Standard Types               | size (bytes) | Category  |
|:----------|:-----------------------------|--------------|:----------|
| _gbool_   | bool                         | 1            | boolean   |
| _gbyte_   | signed char                  | 1            | integer   |
| _gshort_  | signed short                 | 2            | integer   |
| _gint_    | signed int                   | 4            | integer   |
| _glong_   | signed long/signed long long | 8            | integer   |
| _gfloat_  | float                        | 4            | floating  |
| _gdouble_ | double/long double           | 8            | floating  |
| _gchar_   | char16_t                     | 2            | character |

### Equivalence of types

| Primitive Types | Object Types                   |
|:----------------|:-------------------------------|
| _gbool_         | [Boolean](core/boolean.md)     |
| _gbyte_         | [Byte](core/byte.md)           |
| _short_         | [Short](core/short.md)         |
| _gint_          | [Integer](core/integer.md)     |
| _glong_         | [Long](core/long.md)           |
| _gfloat_        | [Float](core/float.md)         |
| _gdouble_       | [Double](core/double.md)       |
| _gchar_         | [Character](core/character.md) |

## Literals

_CORE 24_ Add new literal expressions for fast code writing.

### Complex Numbers

For complex numbers, to create easily imaginary part developers have
possibility to uses one of all following suffix:

```c++
    // Require CORE_OVERRIDE_COMPLEX_LITERALS != 0
    Complex operator""i(...);
    Complex operator""j(...);
    
    // No Requirement
    Complex operator""_i(...);
    Complex operator""_j(...);
```

#### Samples

```c++
    auto z1 = 1 + 2i;
    auto z2 = 1 + 2j;
    auto z2 = 1 + 2_i;
    auto z2 = 1 + 2_j;
```

### Strings Literals

For string numbers, to create easily and freely the latin-1 or Unicode string, developers have
possibility to uses one of all following suffix:

```c++
    // For Auto deduction string encoding.
    // For example
    // On MSVC: it can be equivalent to operator""_Sl(...) 
    // for type const char* if CORE_UNICODE_MSVC is not defined
    // Otherwise: it equivalent to operator""_Su(...)
    String operator""_S(...);
    
    // Specific of Unicode String
    String operator""_Su(...);
    
    // Specific of Latin-1 String
    String operator""_Sl(...);

```

#### Samples

```c++
  
  // From Utf-8
  auto s1 = "ブラン背ヴィク"_S; // "ブラン背ヴィク"
  auto s2 = "ブラン背ヴィク"_Su; // "ブラン背ヴィク"
  auto s3 = "ブラン背ヴィク"_Sl; // "ÖéóÌô£¯"
  auto s4 = "Hello World!"_Sl; // "Hello World!"

  // From Utf-16
  auto s1 = u"ブラン背ヴィク"_S; // "ブラン背ヴィク"
  auto s2 = u"ブラン背ヴィク"_Su; // "ブラン背ヴィク"
  auto s3 = u"ブラン背ヴィク"_Sl; // "ÖéóÌô£¯"
  auto s4 = u"Hello World!"_Sl; // "Hello World!"

  // From Utf-32
  auto s1 = U"ブラン背ヴィク"_S; // "ブラン背ヴィク"
  auto s2 = U"ブラン背ヴィク"_Su; // "ブラン背ヴィク"
  auto s3 = U"ブラン背ヴィク"_Sl; // "ÖéóÌô£¯"
  auto s4 = U"Hello World!"_Sl; // "Hello World!"

  // For Wide String
  auto s1 = L"ブラン背ヴィク"_S; // "ブラン背ヴィク"
  auto s2 = L"ブラン背ヴィク"_Su; // "ブラン背ヴィク"
  auto s3 = L"ブラン背ヴィク"_Sl; // "ÖéóÌô£¯"
  auto s4 = L"Hello World!"_Sl; // "Hello World!"

```

### Null Pointer

For pointer, it exists `nullptr` to define null pointer. the developers
have possibility to use instead `null` for compatibility with _CORE 24_

#### Samples

```c++
 void *ptr = null; // equivalent to ptr = nullptr
```
