//
// Created by brunshweeck on 3 juin 2024.
//

#include "Complex.h"

#include <core/lang/ArithmeticException.h>
#include <core/lang/NumberFormatException.h>
#include <core/util/ArrayList.h>

namespace core {
  using namespace util;

  Complex const Complex::I = Complex(0., 1.);

  Complex const Complex::INF = Complex(Math::INF, 0);

  Complex const Complex::NaN = Complex(Math::NaN, Math::NaN);

  Complex const Complex::INFi = Complex(0, Math::INF);

  Complex const Complex::ZERO = Complex(0., 0.);

  Complex const Complex::ONE = Complex(1., 0.);

  Complex::Complex(gdouble real, gdouble imag) : re(real), im(imag) {
    if (Double::isNaN(real) || Double::isNaN(imag))
      re = im = Double::NaN;
  }

  Complex Complex::negate() const {
    if (isNaN())
      return NaN;
    return Complex(-re, -im);
  }

  Complex::Complex(gdouble real) : Complex(real, 0.0) {}

  Complex::Complex(_Dcomplex const& c) : Complex(0.0, 0.0) {
    struct ComplexD {
      ClassOf(.0)::Array<2> val;
    };

    CORE_ALIAS(Complex64, Class< ComplexD >::Pointer);

    const Complex64 z = CORE_CAST(Complex64, &c);

    re = z->val[0];
    im = z->val[1];

    if (Double::isNaN(re) || Double::isNaN(im))
      re = im = Double::NaN;
  }

  gbool Complex::isNaN() const {
    return Double::isNaN(re) || Double::isNaN(im);
  }

  gbool Complex::isInfinite() const {
    return Double::isInfinite(re) || Double::isInfinite(im);
  }

  gbool Complex::isFinite() const {
    return Double::isFinite(re) && Double::isFinite(im);
  }

  gbool Complex::isReal() const {
    return im == 0.0;
  }

  gbool Complex::isImag() const {
    return re == 0.0;
  }

  Complex Complex::valueOf(String const& str0) {
    String str = str0.strip().toLowerCase();
    str = str.replace("*i"_S, "i"_S);
    str = str.replace("(nan)i"_S, "nani"_S);
    str = str.replace("(-nan)i"_S, "nani"_S);
    str = str.replace("(+nan)i"_S, "nani"_S);
    str = str.replace("infinity"_S, "inf"_S);
    str = str.replace("(inf)i"_S, "infi"_S);
    str = str.replace("(+inf)i"_S, "infi"_S);
    str = str.replace("(-inf)i"_S, "-infi"_S);
    // Insert (+)
    str = str.replace("-"_S, "+-"_S);

    while (str.indexOf("+ "_S) >= 0)
      str = str.replace("+ "_S, "+"_S);

    while (str.indexOf("- "_S) >= 0)
      str = str.replace("- "_S, "-"_S);

    while (str.indexOf("--"_S) >= 0)
      str = str.replace("--"_S, "+"_S);

    while (str.indexOf("++"_S) >= 0)
      str = str.replace("++"_S, "+"_S);

    // Splitting
    gint count = str.length();
    gint success = 0x000;
    Complex c;

    try {
      gint j = 0;
      gint i;
      while ((i = str.indexOf(u'+', j)) >= 0) {
        if (i == j) {
          j += 1;
          continue;
        }
        String s = str.subString(j, i);
        j = i + 1;
        s = s.strip();

        gint count2 = s.length();
        if (count2 == 0)
          continue;

        if (s.charAt(count2 - 1) == 'i') {
          // z = bi
          // remove last characters (i) and parse b
          String s2 = s.subString(0, count2 - 1);
          if (s2.length() == 0)
            // z = 1i
            c.im += 1.0;
          else if (s2.length() == 1 && s2.charAt(0) == '-')
            // z = -1i
            c.im -= 1.0;
          else
            c.im += Double::parseDouble(s2);
          success |= 0xF0;
        } else {
          // z = a
          c.re += Double::parseDouble(s);
          success |= 0x0F;
        }

        if (j >= count)
          success |= 0xF00;
      }

      // parse remaining characters
      if (j < count) {
        String s = str.subString(j);
        s = s.strip();
        if (!s.isEmpty()) {
          gint count2 = s.length();
          if (s.charAt(count2 - 1) == 'i') {
            // z = bi
            // remove last characters (i) and parse b
            String s2 = s.subString(0, count2 - 1);
            if (s2.length() == 0)
              // z = 1i
              c.im += 1.0;
            else if (s2.length() == 1 && s2.charAt(0) == '-')
              // z = -1i
              c.im -= 1.0;
            else
              c.im += Double::parseDouble(s2);
            success |= 0xFF0;
          } else {
            // z = a
            c.re += Double::parseDouble(s);
            success |= 0xF0F;
          }
        }
      }
    } catch (Throwable const& ex) { ex.throws($ftrace()); }

    if ((success & 0xF00) != 0xF00)
      NumberFormatException("For input: "_S + str0).throws($ftrace());

    return c;
  }

  Complex Complex::valueOf(gdouble real) { return Complex(real, 0); }

  String Complex::toString() const {
    String str;
    if (re == 0 && im == 0)
      return Double::toString(re);
    if (re == 0)
      if (isFinite())
        str = Double::toString(im) + "i"_S;
      else if (isNaN())
        str = "(NaN)i"_S;
      else
        str = im > 0 ? "+(Inf)i"_S : "-(Inf)i"_S;
    else {
      str = Double::toString(re);
      if (im != 0) {
        if (isFinite()) {
          if (re > 0)
            str += "+"_S;
          str += Double::toString(im) + "i"_S;
        } else if (isNaN())
          str += "(NaN)i"_S;
        else
          str += im > 0 ? "+(inf)i"_S : "-(inf)i"_S;
      }
    }
    return str;
  }

  gdouble Complex::real() const { return re; }

  gdouble Complex::imag() const { return im; }

  gint Complex::intValue() const { return CORE_CAST(gint, re); }

  glong Complex::longValue() const { return CORE_CAST(glong, re); }

  gfloat Complex::floatValue() const { return CORE_CAST(gfloat, re); }

  gdouble Complex::doubleValue() const { return re; }

  gbyte Complex::byteValue() const { return CORE_CAST(gbyte, re); }

  gshort Complex::shortValue() const { return CORE_CAST(gshort, re); }

  Complex Complex::reciprocal() const {
    if (isNaN())
      return NaN;
    if (re == 0.0 && im == 0.0)
      return INF + INFi;
    if (isInfinite())
      return ZERO;
    gdouble q, scale;
    if (Math::abs(re) < Math::abs(im)) {
      q = re / im;
      scale = 1. / (re * q + im);
      return Complex(scale * re, -scale);
    }
    q = im / re;
    scale = 1. / (re + im * q);
    return Complex(scale, -scale * q);
  }

  Complex Complex::reverse() const {
    return Complex(im, re);
  }

  Complex Complex::conjugate() const {
    if (isNaN())
      return NaN;
    return Complex(re, -im);
  }

  gbool Complex::equals(Object const& obj) const {
    return this == &obj || Class<Complex>::hasInstance(obj) && compareTo(CORE_XCAST(Complex const, obj)) == 0;
  }

  gint Complex::hash() const {
    if (isNaN())
      return Double::hash(Double::NaN);
    return Double::hash(re) * 31 + Double::hash(im);
  }

  gint Complex::compareTo(Complex const& anotherComplex) const {
    gdouble real1;
    gdouble real2;
    gdouble imag1;
    gdouble imag2;
    if (isNaN()) {
      real1 = Double::NaN;
      imag1 = Double::NaN;
    } else {
      real1 = re;
      imag1 = im;
    }
    if (anotherComplex.isNaN()) {
      real2 = Double::NaN;
      imag2 = Double::NaN;
    } else {
      real2 = anotherComplex.re;
      imag2 = anotherComplex.im;
    }
    gint r = Double::compare(real1, real2);
    if (r == 0)
      r = Double::compare(imag1, imag2);
    return r;
  }

  Complex Complex::plus(Complex const& addend) const {
    if (isNaN() || addend.isNaN())
      return NaN;
    gdouble real = re + addend.re;
    gdouble imag = im + addend.im;
    return Complex(real, imag);
  }

  Complex Complex::plus(gdouble addend) const {
    if (isNaN() || Double::isNaN(addend))
      return NaN;
    gdouble real = re + addend;
    gdouble imag = im;
    return Complex(real, imag);
  }

  Complex Complex::subtract(Complex const& subtrahend) const {
    if (isNaN() || subtrahend.isNaN())
      return NaN;
    gdouble real = re - subtrahend.re;
    gdouble imag = im;
    return Complex(real, imag);
  }

  Complex Complex::subtract(gdouble subtrahend) const {
    if (isNaN() || Double::isNaN(subtrahend))
      return NaN;
    gdouble real = re - subtrahend;
    gdouble imag = im;
    return Complex(real, imag);
  }

  Complex Complex::multiply(Complex const& factor) const {
    if (isNaN() || factor.isNaN())
      return NaN;

    gdouble real1 = re;
    gdouble real2 = factor.re;
    gdouble imag1 = im;
    gdouble imag2 = factor.im;

    return Complex(real1 * real1 - imag1 * imag2, real1 * imag2 + imag1 * real2);
  }

  Complex Complex::multiply(gdouble factor) const {
    if (isNaN() || Double::isNaN(factor))
      return NaN;

    gdouble real = re * factor;
    gdouble imag = im * factor;

    return Complex(real, imag);
  }

  Complex Complex::divide(Complex const& divisor) const {
    if (isNaN() || divisor.isNaN())
      return NaN;

    gdouble real = re;
    gdouble imag = im;
    gdouble real2 = divisor.re;
    gdouble imag2 = divisor.im;

    if (real2 == 0.0 && imag2 == 0.0)
      return NaN;

    if (Math::abs(real2) < Math::abs(imag2)) {
      gdouble q = real2 / imag2;
      gdouble denominator = real2 * q + imag2;
      return Complex((real * q + imag) / denominator, (imag * q - real) / denominator);
    }
    gdouble q = imag2 / real2;
    gdouble denominator = imag2 * q + real2;
    return Complex((imag * q + real) / denominator, (imag - real * q) / denominator);
  }

  Complex Complex::divide(gdouble divisor) const {
    if (isNaN() || Double::isNaN(divisor))
      return NaN;

    gdouble real = re / divisor;
    gdouble imag = im / divisor;

    return Complex(real, imag);
  }

  gdouble Complex::norm() const {
    if (isNaN())
      return Double::NaN;
    if (isInfinite())
      return Double::POSITIVE_INFINITY;
    gdouble real = re;
    gdouble imag = im;
    if (Math::abs(real) < Math::abs(imag)) {
      if (imag == 0.0) {
        return Math::abs(real);
      }
      gdouble q = real / imag;
      return Math::abs(imag) * Math::sqrt(1 + q * q);
    }
    if (real == 0.0) {
      return Math::abs(imag);
    }
    gdouble q = imag / real;
    return Math::abs(real) * Math::sqrt(1 + q * q);
  }

  gdouble Complex::phase() const {
    return argument();
  }

  gdouble Complex::argument() const {
    Complex __z = *this;

    return Math::atan2(__z.real(), __z.imag());
  }

  Array<Complex> Complex::nthRoot(gint n) const {
    if (n <= 0)
      ArithmeticException("cannot compute nth root for null or negative n: "_S + n).throws($ftrace());

    Complex __z = *this;

    ArrayList<Complex> roots;
    if (__z.isNaN()) {
      roots.add(NaN);
      return roots.toArray();
    }
    if (__z.isInfinite()) {
      roots.add(Complex(Double::POSITIVE_INFINITY, Double::POSITIVE_INFINITY));
      return roots.toArray();
    }

    // nth root of abs -- faster / more accurate to use a solver here?
    gdouble nthRootOfNorm = Math::pow(__z.norm(), 1. / n);

    // Compute nth roots of complex number with k = 0, 1, ... n-1
    gdouble nthPhi = __z.argument() / n;
    gdouble slice = 2 * Math::PI / n;
    gdouble innerPart = nthPhi;
    for (int i = 0; i < n; ++i) {
      // inner part
      gdouble real = nthRootOfNorm * Math::cos(innerPart);
      gdouble imag = nthRootOfNorm * Math::sin(innerPart);
      roots.add(Complex(real, imag));
      innerPart += slice;
    }

    return roots.toArray();
  }

  Complex Complex::cos() const {
    if (isNaN())
      return NaN;
    gdouble real = re;
    gdouble imag = im;

    return Complex(
      Math::cos(real) * Math::cosh(imag),
      -Math::sin(real) * Math::sinh(imag)
    );
  }

  Complex Complex::acos() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    CORE_FAST gdouble __pi_2 = 1.570796326794896619231321691639751442; // PI / 2
    __z = __z.asin();

    return Complex(__pi_2 - __z.real(), -__z.imag());
  }

  Complex Complex::cosh() const {
    if (isNaN())
      return NaN;

    gdouble real = re;
    gdouble imag = im;

    return Complex(Math::cosh(real) * Math::cos(imag), Math::sinh(real) * Math::sin(imag));
  }

  Complex Complex::acosh() const {
    if (isNaN())
      return NaN;
    // Kahan's formula.
    Complex __z = *this;
    Complex __z1 = __z + 1.0;
    Complex __z2 = __z - 1.0;

    __z1 *= 0.5;
    __z2 *= 0.5;

    __z1 = __z1.sqrt();
    __z2 = __z2.sqrt();

    __z = __z1 + __z2;
    return __z.log() * 2.0;
  }

  Complex Complex::sin() const {
    if (isNaN())
      return NaN;

    gdouble real = re;
    gdouble imag = im;

    return Complex(Math::sin(real) * Math::cosh(imag), Math::cos(real) * Math::sinh(imag));
  }

  Complex Complex::asin() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    Complex __t = Complex(-__z.imag(), __z.real());
    __z = __t.asinh();

    return Complex(__z.imag(), -__z.real());
  }

  Complex Complex::sinh() const {
    if (isNaN())
      return NaN;

    gdouble real = re;
    gdouble imag = im;

    return Complex(Math::sinh(real) * Math::cos(imag), Math::cosh(real) * Math::sin(imag));
  }

  Complex Complex::asinh() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    Complex __t = Complex(
      (__z.real() - __z.imag()) * (__z.real() + __z.imag()) + 1.0,
      2.0 * __z.real() * __z.imag()
    );

    __z += __t.sqrt();
    return __z.log();
  }

  Complex Complex::tan() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    return __z.sin() / __z.cos();
  }

  Complex Complex::atan() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    gdouble __r = __z.real() * __z.real();
    gdouble __i = __z.imag() * __z.imag();
    gdouble __x = 1.0 - __r - __i;
    gdouble __n = __z.imag() + 1.0;
    gdouble __d = __z.imag() - 1.0;

    __n = __r + __n * __n;
    __d = __r + __d * __d;

    return Complex(
      0.5 * Math::atan2(2.0 * __z.real(), __x),
      0.25 * Math::log(__n / __d)
    );
  }

  Complex Complex::tanh() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    return __z.sinh() / __z.cosh();
  }

  Complex Complex::atanh() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    gdouble __r = __z.real() * __z.real();
    gdouble __i = __z.imag() * __z.imag();
    gdouble __x = 1.0 - __i - __r;
    gdouble __n = 1.0 + __z.real();
    gdouble __d = 1.0 - __z.real();

    __n = __i + __n * __n;
    __d = __i + __d * __d;

    return Complex(
      0.25 * Math::log(__n / __d),
      0.5 * Math::atan2(2.0 * __z.imag(), __x)
    );
  }

  Complex Complex::exp() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    return Complex::polar(Math::exp(__z.real()), __z.imag());
  }

  Complex Complex::log() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;

    return Complex(Math::log(__z.norm()), __z.argument());
  }

  Complex Complex::pow(Complex const& x) const {
    if (isNaN() || x.isNaN())
      return NaN;

    Complex __z = *this;
    __z = __z.log();
    __z *= x;
    return __z.exp();
  }

  Complex Complex::pow(gdouble x) const {
    if (isNaN() || Double::isNaN(x))
      return NaN;

    Complex __z = *this;
    __z = __z.log();
    __z *= x;
    return __z.exp();
  }

  Complex Complex::sqrt() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    gdouble __r = __z.real();
    gdouble __i = __z.imag();

    if (__r == 0.0) {
      gdouble __t = Math::sqrt(Math::abs(__i));
      return Complex(__t, __i < 0.0 ? -__t : __t);
    } else {
      gdouble __t = Math::sqrt(2 * (__z.norm() + Math::abs(__r)));
      gdouble __u = __t / 2;
      return __r > 0.0
               ? Complex(__u, __i / __t)
               : Complex(Math::abs(__i) / __t, __i < 0.0 ? -__u : __u);
    }

    gdouble real = re;
    gdouble imag = im;

    if (real == 0.0 && imag == 0.0)
      return ZERO;

    double t = Math::sqrt((Math::abs(real) + norm()) / 2.0);
    if (real >= 0.0)
      return Complex(t, imag / (2.0 * t));
    return Complex(Math::abs(imag) / (2.0 * t), Math::copySign(1., imag) * t);
  }

  Complex Complex::sqrt1z() const {
    if (isNaN())
      return NaN;

    Complex __z = *this;
    __z = 1.0 - __z;
    return __z.sqrt();
    // return ONE.subtract(multiply(*this)).sqrt();
  }

  Complex Complex::polar(gdouble rho, gdouble phi) {
    if (Double::isNaN(rho) || Double::isNaN(phi) || Double::isInfinite(phi))
      return NaN;

    return Complex(
      rho * Math::cos(phi),
      rho * Math::sin(phi)
    );
  }

  Object& Complex::clone() const {
    try {
      return UNSAFE::newInstance<Complex>(*this);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  Complex Complex::initLiteral(glong literal, gint memSize, gbool isFloating) {
    switch (memSize) {
      case 2: {
        gbyte* ptr = (gbyte*) literal;
        return Complex(ptr[0], ptr[1]);
      }
      case 4: {
        gshort* ptr = (gshort*) literal;
        return Complex(ptr[0], ptr[1]);
      }
      case 8: {
        if (isFloating) {
          gfloat* ptr = (gfloat*) literal;
          return Complex(ptr[0], ptr[1]);
        } else {
          gint* ptr = (gint*) literal;
          return Complex(ptr[0], ptr[1]);
        }
      }
      case 16: {
        if (isFloating) {
          gdouble* ptr = (gdouble*) literal;
          return Complex(ptr[0], ptr[1]);
        } else {
          glong* ptr = (glong*) literal;
          return Complex(ptr[0], ptr[1]);
        }
      }
      default:
        break;
    }

    AssertionError("Illegal complex literal").throws($ftrace());
  }
} // core
