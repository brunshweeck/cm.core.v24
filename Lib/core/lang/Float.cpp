//
// Created by brunshweeck on 25 mai 2024.
//

#include <core/lang/Character.h>
#include "Float.h"
#include <core/lang/Integer.h>
#include <core/lang/NumberFormatException.h>
#include <core/misc/Unsafe.h>
#include "meta/lang/FloatConsts.h"

namespace core {
  // gfloat const Float::NaN = Float::fromIntBits(0x7fc00000);

  String Float::toString(gfloat f) {
    if (isNaN(f))
      return "NaN"_S;

    if (isInfinite(f))
      return (f < 0.0F) ? "inf"_S : "-inf"_S;

    gint bits = toRawIntBits(f);
    if (bits == 0)
      return "0.0"_S;

    if (bits == FloatConsts::SIGN_BIT_MASK)
      return "-0.0"_S;

    // Find precision (such that f = a * 2^p).
    gint precision = 0;
    {
      gfloat abs = Math::abs(f);
      if (abs < 1.0F) {
        while (abs < 0.1F)
          abs *= 100.0F;
        while (abs < 1.0F)
          abs *= 10.0F;
        bits = toRawIntBits(abs);
        precision += 1;
      }
      precision +=
          FloatConsts::SIGNIFICAND_WIDTH -
          Integer::numberOfTrailingZeros(bits & FloatConsts::SIGNIF_BIT_MASK);
      precision = Math::abs(precision);
      bits = Float::toRawIntBits(f);
    }

    // Convert precision to decimal precision (such that f = a * 2^p = b * 10^dp)
    static CORE_FAST gint decimalPrecisions[] = {
      0, 0, 0, 0, // 2^00 ~ 2^03 = 10^0
      1, 1, 1, // 2^04 ~ 2^06 = 10^1
      2, 2, 2, // 2^07 ~ 2^09 = 10^2
      3, 3, 3, 3, // 2^10 ~ 2^13 = 10^3
      4, 4, 4, // 2^14 ~ 2^16 = 10^4
      5, 5, 5, // 2^17 ~ 2^19 = 10^5
      6, 6, 6, 6, // 2^20 ~ 2^23 = 10^6
      7, 7, 7, // 2^24 ~ 2^26 = 10^7 (max)
      8, 8, 8, // 2^27 ~ 2^29 = 10^8
      9, 9, 9, 9, // 2^30 ~ 2^33 = 10^9
    };
    // assert 0 <= precision <= 24
    gint decimalPrecision = decimalPrecisions[precision];

    // Adjust decimal precision
    static CORE_FAST gint powerOfTeen[] = {
      1,
      10,
      100,
      1000,
      10000,
      100000,
      1000000,
      10000000,
      100000000,
      1000000000
    };

    if (powerOfTeen[decimalPrecision] < (1 << precision))
      decimalPrecision += 1;

    CharArray ca = CharArray(20);
    gint next = 0;
    if ((bits & FloatConsts::SIGN_BIT_MASK) != 0) {
      ca[next++] = u'-';
      bits &= ~FloatConsts::SIGN_BIT_MASK;
    }

    // Skipping the first character after sign character for rounding.
    gint skipped = next++;
    ca[skipped] = '0';

    gfloat abs = fromIntBits(bits);
    gint exp = 0;
    gint digit = -1; // last digit

    gfloat f2 = abs;
    if (f2 > 1.0e+7F || f2 < 1.0e-3F) {
      // Scientific form (a * 10^exp)

      if (abs >= 10.0F) {
        while (abs >= 10000.0F) {
          exp += 4;
          abs *= 0.0001F;
        }
        while (abs >= 100.0F) {
          exp += 2;
          abs *= 0.01F;
        }
        while (abs >= 10.0F) {
          exp += 1;
          abs *= 0.1F;
        }
      }

      if (abs < 1.0F) {
        while (abs < 0.001F) {
          exp -= 4;
          abs *= 10000.0F;
        }
        while (abs < 0.1F) {
          exp -= 2;
          abs *= 100.0F;
        }
        while (abs < 1.0F) {
          exp -= 1;
          abs *= 10.0F;
        }
      }

      f2 = abs;
      digit = (gint) f2;
      ca[next++] = Character::forDigit(digit, 10);
      ca[next++] = '.';
      decimalPrecision -= (decimalPrecision > 7) ? 2 : 1;
      do {
        f2 = (f2 - (gfloat) digit) * 10.0f;
        digit = (gint) f2;
        ca[next++] = Character::forDigit(digit, 10);
        decimalPrecision -= 1;
      } while (decimalPrecision > 0);

      // round result and remove all trailing zeros
      f2 = (f2 - (gfloat) digit) * 10.0f;
      digit = (gint) f2;
      if (digit >= 5) {
        // round value and remove all trailing zeros
        gbool finished = false;
        while (!finished && next > 0) {
          next -= 1;
          if (ca[next] != '.') {
            digit = ca[next] - 48;
            ca[next] = Character::forDigit((digit + 1) % 10, 10);
            finished = digit != 9;
          }
        }
      } else if (ca[next - 1] == '0') {
        // remove all trailing zeros
        next -= 1;
        while (ca[next] == '0')
          next -= 1;
        if (ca[next] == '.')
          next -= 1;
      } else {
        // no problems found
        next -= 1;
      }

      next += 1;
      ca[next++] = 'e';
      ca[next++] = exp < 0 ? '-' : '+';
      exp = Math::abs(exp);

      if (exp >= 10)
        ca[next++] = Character::forDigit(exp / 10, 10);

      ca[next++] = Character::forDigit(exp % 10, 10);
      ca[next] = 0;
    } else {
      // Decimal format (unit.decimal)

      if (f2 < 1.0F) {
        ca[next++] = '0';
        ca[next++] = '.';
        digit = 0;
        f2 *= 10.0f;
        // decimalPrecision -= (decimalPrecision >= 7) ? 2 : 1;
        decimalPrecision -= (decimalPrecision >= 6) ? 2 : 1;
      } else {
        exp += 1;
        if (abs >= 10.0F) {
          while (abs >= 10000.0F) {
            exp += 4;
            abs *= 0.0001F;
          }
          while (abs >= 100.0F) {
            exp += 2;
            abs *= 0.01F;
          }
          while (abs >= 10.0F) {
            exp += 1;
            abs *= 0.1F;
          }
          f2 = abs;
        }

        // Write the Digits of Unit part
        do {
          digit = (gint) f2;
          f2 = (f2 - (gfloat) digit) * 10.0F;
          ca[next++] = Character::forDigit(digit, 10);
          decimalPrecision -= 1;
          exp -= 1;
        } while (exp > 0);
        ca[next++] = '.';
        if (decimalPrecision >= 5)
          decimalPrecision -= 1;
      }

      // Writing the Digits of Decimal part
      do {
        digit = (gint) f2;
        f2 = (f2 - (gfloat) digit) * 10.0F;
        ca[next++] = Character::forDigit(digit, 10);
        decimalPrecision -= 1;
      } while (decimalPrecision > 0);

      // Rounding
      digit = (gint) f2;
      if (digit == 4) {
        // Verifying the next digits while digits have value 4
        gint nextDigit = digit;
        do {
          f2 = (f2 - (gfloat) nextDigit) * 10.0F;
          nextDigit = (gint) f2;
        } while (nextDigit == 4);

        // Rounds digit at current position
        if (nextDigit >= 5)
          digit = 5;
      }
      if (digit >= 5) {
        gint i = next;
        gbool ok = false;
        while (!ok && next > 0) {
          next -= 1;
          if (ca[next] != '.') {
            digit = Character::digit(ca[next], 10);
            ca[next] = Character::forDigit((digit + 1) % 10, 10);
            ok = digit != 9;
          }
        }
        next = i;
      }

      // Remove all trailing zeros
      while (ca[next - 1] == '0')
        next -= 1;
      if (ca[next - 1] == '.') {
        next += 1;
        ca[next - 1] = '0';
      }
      next -= 1;
    }

    if (ca[next - 1] == '.')
      ca[next++] = '0';

    // Check if skipped index have been used
    if (ca[skipped] == '0') {
      if (skipped != 0) {
        ca[skipped] = ca[0];
        return String(ca, skipped, next - skipped);
      }
      return String(ca, 1, next - 1);
    }
    return String(ca, 0, next);
  }

  String Float::toHexString(gfloat f) {
    if (!isFinite(f))
      return toString(f);
    gint bit32 = toIntBits(f);
    CharArray digits{32};
    gint next = 0;
    gfloat const uVal = fromIntBits(bit32 & ~FloatConsts::SIGN_BIT_MASK);
    if ((bit32 & FloatConsts::SIGN_BIT_MASK) != 0)
      digits[next++] = '-';
    digits[next++] = '0';
    digits[next++] = 'x';
    digits[next++] = uVal < MIN_NORMAL ? '0' : '1';
    digits[next++] = '.';
    digits[next++] = Character::forDigit(((bit32 & FloatConsts::SIGNIF_BIT_MASK) >> 20) & 0xf, 16);
    digits[next++] = Character::forDigit(((bit32 & FloatConsts::SIGNIF_BIT_MASK) >> 16) & 0xf, 16);
    digits[next++] = Character::forDigit(((bit32 & FloatConsts::SIGNIF_BIT_MASK) >> 12) & 0xf, 16);
    digits[next++] = Character::forDigit(((bit32 & FloatConsts::SIGNIF_BIT_MASK) >> 8) & 0x0f, 16);
    digits[next++] = Character::forDigit(((bit32 & FloatConsts::SIGNIF_BIT_MASK) >> 4) & 0x0f, 16);
    digits[next++] = Character::forDigit(((bit32 & FloatConsts::SIGNIF_BIT_MASK) >> 0) & 0x0f, 16);
    // remove all trailing zero
    while (digits[next - 1] == '0' && digits[next - 2] != '.')
      next -= 1;
    digits[next++] = 'p';
    gint exponent = uVal < MIN_NORMAL
                      ? MIN_EXPONENT
                      : (gint) (((bit32 & FloatConsts::EXP_BIT_MASK) >> 23) -
                        FloatConsts::EXP_BIAS);
    digits[next++] = exponent < 0 ? '-' : '+';
    exponent = Math::abs(exponent);
    if (exponent >= 1000)
      digits[next++] = Character::forDigit(exponent / 1000, 10);
    if (exponent >= 100)
      digits[next++] = Character::forDigit(exponent % 1000 / 100, 10);
    if (exponent >= 10)
      digits[next++] = Character::forDigit(exponent % 100 / 10, 10);
    digits[next++] = Character::forDigit(exponent % 10, 10);
    digits[next] = 0;
    return String(digits, 0, next);
  }

  Float Float::valueOf(String const& s) {
    try {
      return valueOf(parseFloat(s));
    } catch (NumberFormatException const& ex) { ex.throws($ftrace()); }
  }

  Float Float::valueOf(gfloat f) {
    return f;
  }

  gfloat Float::parseFloat(String const& s) {
    if (s.isEmpty())
      NumberFormatException(R"(Invalid number format for input "".)"_S).throws($ftrace());
    gint len = s.length();
    gfloat sign = +1.0;
    gint base = 10;
    gint next = 0;
    gfloat retVal = 0;
    gchar ch = s.charAt(0);
    switch (ch) {
      case '-':
        sign = -1;
      case '+':
        next += 1;
        if (next == len)
          NumberFormatException(R"(Invalid number format for input " )"_S + s + R"(".)"_S)
              .throws($ftrace());
        ch = s.charAt(next);
      default:
        break;
    }
    switch (ch) {
      case '0':
        next += 1;
        if (next == len)
          // +0 or -0
          return 0.0F * (gfloat) sign;
        ch = s.charAt(next);
        if (ch == 'x' || ch == 'X') {
          // hex format
          next += 1;
          base = 16;
          if (next == len)
            goto throwIllegalFormat;
        } else if (ch == 'b' || ch == 'B') {
          // binary format 0b...
          next += 1;
          if (next == len)
            goto throwIllegalFormat;
          // check if is integer
          if (s.indexOf('.', next) >= 0)
            goto throwIllegalFormat;
          base = 2;
        } else {
          // check if is integer
          if (s.indexOf('.', next) < 0)
            base = 8;
        }
        break;
      case 'i':
      case 'I':
        // inf or infinity
        if (len - next == 3) {
          gchar const ch2 = s.charAt(next + 1);
          gchar const ch3 = s.charAt(next + 2);
          if ((ch2 == 'n' || ch2 == 'N') && (ch3 == 'f' || ch3 == 'F'))
            return (gfloat) (sign * POSITIVE_INFINITY);
        } else if (len - next == 8) {
          gchar const ch2 = s.charAt(next + 1);
          gchar const ch3 = s.charAt(next + 2);
          gchar const ch4 = s.charAt(next + 3);
          gchar const ch5 = s.charAt(next + 4);
          gchar const ch6 = s.charAt(next + 5);
          gchar const ch7 = s.charAt(next + 6);
          if ((ch2 == 'n' || ch2 == 'N') && (ch3 == 'f' || ch3 == 'F') && (ch4 == 'i' || ch4 == 'I') &&
            (ch5 == 'f' || ch5 == 'F') && (ch6 == 'i' || ch6 == 'I') && (ch7 == 'y' || ch7 == 'Y'))
            return (gfloat) (sign * POSITIVE_INFINITY);
        }
        goto throwIllegalFormat;
      case 'n':
      case 'N':
        // NaN
        if (len - next == 3) {
          gchar const ch2 = s.charAt(next + 1);
          gchar const ch3 = s.charAt(next + 2);
          if ((ch2 == 'a' || ch2 == 'A') && (ch3 == 'n' || ch3 == 'N'))
            return (gfloat) (sign * POSITIVE_INFINITY);
        }
        goto throwIllegalFormat;
      default:
        break;
    }
    switch (base) {
      case 2:
      case 8:
        // integral formats
        while (next < len) {
          ch = s.charAt(next);
          if (ch < '0' || ch > '9')
            goto throwIllegalFormat;
          gchar const digit = ch - '0';
          retVal = retVal * (gfloat) base + (gfloat) digit;
          next += 1;
        }
        return (gfloat) sign * retVal;
      case 10:
        // decimal format
        while (next < len) {
          ch = s.charAt(next);
          gchar digit = -1;
          if (ch >= '0' && ch <= '9')
            digit = ch - '0';
          else if (ch == 'e' || ch == 'E' || ch == '.') {
            next += 1;
            gint decimal = 0;
            gint exponent = 0;
            gint sign2 = +1;
            gint count = 0;
            if (ch == '.') {
              // decimal part
              gint maxCount = 9;
              while (next < len) {
                ch = s.charAt(next);
                if (ch == 'e' || ch == 'E')
                  break;
                if (ch >= '0' && ch <= '9')
                  digit = ch - '0';
                else
                  goto throwIllegalFormat;
                if (maxCount > 0) {
                  if (count < maxCount) {
                    decimal = decimal * 10 + digit;
                    count += 1;
                  } else if (count == maxCount) {
                    gint tmp = decimal * 10 + digit;
                    if (tmp > 0) {
                      // overflow
                      maxCount += 1;
                      decimal = tmp;
                      count += 1;
                    } else if (digit >= 5 && (tmp = decimal + 1) > decimal) {
                      decimal = tmp;
                      maxCount = 0;
                    }
                  }
                }
                next += 1;
              }
            }
            if (ch == 'e' || ch == 'E') {
              // exponent part
              next += 1;
              if (next == len)
                goto throwIllegalFormat;
              ch = s.charAt(next);
              switch (ch) {
                case '-':
                  sign2 = -1;
                case '+':
                  next += 1;
                  if (next == len)
                    goto throwIllegalFormat;
                default:
                  break;
              }
              while (next < len) {
                ch = s.charAt(next);
                if (ch >= '0' && ch <= '9')
                  digit = ch - '0';
                else
                  goto throwIllegalFormat;
                if (exponent <= 999999)
                  exponent = exponent * 10 + digit;
                next += 1;
              }
            }
            return (gfloat) ((gdouble) sign * ((gdouble) retVal + decimal * Math::pow(10, -count))
              * Math::pow(10, sign2 * exponent));
          } else
            goto throwIllegalFormat;
          retVal = retVal * 10 + (gfloat) digit;
          next += 1;
        }
        return sign * retVal;
      case 16: {
        gint bit32 = 0;
        gint shift = PRECISION - 1;
        gbool rounded = false;
        gbool sticky = false;
        // after 'p'
        gint decimalBitsLen = 0; // after '.'
        gint integerBitsLen = 0; // before '.'
        // hex format
        while (next < len) {
          ch = s.charAt(next);
          gchar digit = -1;
          if (ch >= '0' && ch <= '9')
            digit = ch - '0';
          else if (ch >= 'a' && ch <= 'f')
            digit = ch - 'a';
          else if (ch >= 'A' && ch <= 'F')
            digit = ch - 'A';
          else if (ch == 'p' || ch == 'P' || ch == '.')
            break;
          else
            goto throwIllegalFormat;
          if (shift == 23) {
            switch (digit) {
              case 1:
                bit32 |= (gint) digit << 23;
                shift -= 4;
                integerBitsLen = 0;
                break;
              case 2:
              case 3:
                bit32 |= (gint) digit << 22;
                shift -= 5;
                integerBitsLen = 1;
                break;
              case 4:
              case 5:
              case 6:
              case 7:
                bit32 |= (gint) digit << 21;
                shift -= 6;
                integerBitsLen = 2;
                break;
              default:
                bit32 |= (gint) digit << 20;
                shift -= 7;
                integerBitsLen = 3;
                break;
            }
          } else if (shift >= 0) {
            bit32 |= (gint) digit << shift;
            shift -= 4;
          } else if (shift >= -4) {
            switch (shift) {
              case -1:
                bit32 |= ((gint) digit & 0xE) >> 1;
                rounded = (digit & 0x1) != 0;
                break;
              case -2:
                bit32 |= ((gint) digit & 0xC) >> 2;
                rounded = (digit & 0x2) != 0;
                sticky = (digit & 0x1) != 0;
                break;
              case -3:
                bit32 |= ((gint) digit & 0x8) >> 3;
                rounded = (digit & 0x4) != 0;
                sticky = (digit & 0x3) != 0;
                break;
              case -4:
                rounded = (digit & 0x8) != 0;
                sticky = (digit & 0x7) != 0;
              default: break;
            }
            shift -= 4;
          } else if (!sticky)
            sticky |= digit != 0;
          decimalBitsLen += 1;
          next += 1;
        }
        {
          gint digit = 0;
          next += 1;
          gint exponent = 0;
          gint sign2 = +1;
          if (ch == '.') {
            // decimal part
            while (next < len) {
              ch = s.charAt(next);
              if (ch == 'p' || ch == 'P')
                break;
              if (ch >= '0' && ch <= '9')
                digit = ch - '0';
              else if (ch >= 'a' && ch <= 'z')
                digit = ch - 'a';
              else if (ch >= 'A' && ch <= 'Z')
                digit = ch - 'A';
              else
                goto throwIllegalFormat;
              if (shift >= 0) {
                bit32 |= (gint) digit << shift;
                shift -= 4;
              } else if (shift >= -4) {
                switch (shift) {
                  case -1:
                    bit32 |= ((gint) digit & 0xE) >> 1;
                    rounded = (digit & 0x1) != 0;
                    break;
                  case -2:
                    bit32 |= ((gint) digit & 0xC) >> 2;
                    rounded = (digit & 0x2) != 0;
                    sticky = (digit & 0x1) != 0;
                    break;
                  case -3:
                    bit32 |= ((gint) digit & 0x8) >> 3;
                    rounded = (digit & 0x4) != 0;
                    sticky = (digit & 0x3) != 0;
                    break;
                  case -4:
                    rounded = (digit & 0x8) != 0;
                    sticky = (digit & 0x7) != 0;
                  default: break;
                }
                shift -= 4;
              } else if (!sticky)
                sticky |= digit != 0;
              decimalBitsLen += 1;
              next += 1;
            }
          }
          if (ch == 'p' || ch == 'P') {
            // exponent part
            next += 1;
            if (next == len)
              goto throwIllegalFormat;
            ch = s.charAt(next);
            switch (ch) {
              case '-':
                sign2 = -1;
              case '+':
                next += 1;
                if (next == len)
                  goto throwIllegalFormat;
              default:
                break;
            }
            while (next < len) {
              ch = s.charAt(next);
              if (ch >= '0' && ch <= '9')
                digit = ch - '0';
              else
                goto throwIllegalFormat;
              if (exponent <= 999999)
                exponent = exponent * 10 + digit;
              next += 1;
            }
          }
          if (bit32 == 0) {
            retVal = 0.0F * (gfloat) sign;
            return retVal;
          } else {
            gint eBit32 = sign2 * exponent + integerBitsLen + (decimalBitsLen - 1) * 4;
            if (eBit32 > MAX_EXPONENT) {
              // overflow
              retVal = (gfloat) (sign * POSITIVE_INFINITY);
              return retVal;
            } else if (sign2 * exponent < 0 && (sign2 * exponent) > MIN_EXPONENT) {
              // normal
              eBit32 += FloatConsts::EXP_BIAS;
              eBit32 <<= 23;
              eBit32 &= FloatConsts::EXP_BIT_MASK;
            } else if (eBit32 < (MIN_EXPONENT - 23)) {
              sticky |= rounded;
              rounded = false;
              gint var = 24 - (eBit32 - (MIN_EXPONENT - 23) + 1);
              rounded = (bit32 & (1LL << (var - 1))) != 0;
              if (var > 1)
                sticky = sticky || (bit32 & ~((~0) << (var - 1))) != 0;
              bit32 = bit32 >> var;
              eBit32 = (gint) (MIN_EXPONENT - 1 + FloatConsts::EXP_BIAS) << 23;
            }
            if (((bit32 & 1) != 0 && rounded && sticky) || (((bit32 & 1) == 0) && rounded))
              bit32 += 1;
            if (sign < 0)
              bit32 |= FloatConsts::SIGN_BIT_MASK;
            bit32 |= eBit32;
            retVal = fromIntBits(bit32);
            return retVal;
          }
        }
      }
      default: break;
    }
  throwIllegalFormat:
    NumberFormatException(R"(Invalid number format for input " )"_S + s + R"(".)"_S).throws($ftrace());
  }

  gbool Float::isNaN(gfloat v) {
    return !isFinite(v) && !isInfinite(v);
  }

  gbool Float::isInfinite(gfloat v) {
    return Math::abs(v) > MAX_VALUE;
  }

  gbool Float::isFinite(gfloat f) {
    return Math::abs(f) <= MAX_VALUE;
  }

  Float::Float() CORE_NOTHROW {
    CORE_IGNORE(this);
  }

  Float::Float(gfloat value) : value(value) {}

  Float::Float(String const& s) {
    try {
      value = parseFloat(s);
    } catch (NumberFormatException const& ex) { ex.throws($ftrace()); }
  }

  gbool Float::isNaN() const {
    return isNaN(value);
  }

  gbool Float::isInfinite() const {
    return isInfinite(value);
  }

  String Float::toString() const {
    return toString(value);
  }

  gbyte Float::byteValue() const {
    return (gbyte) value;
  }

  gshort Float::shortValue() const {
    return (gshort) value;
  }

  gint Float::intValue() const {
    return (gint) value;
  }

  glong Float::longValue() const {
    return value;
  }

  gfloat Float::floatValue() const {
    return value;
  }

  gdouble Float::doubleValue() const {
    return value;
  }

  gint Float::hash() const {
    return hash(value);
  }

  gint Float::hash(gfloat value) {
    return toIntBits(value);
  }

  gbool Float::equals(Object const& obj) const {
    return this == &obj ||
        Class<Float>::hasInstance(obj) && compare(value, CORE_XCAST(Float const, obj).value);
  }

  gint Float::toIntBits(gfloat value) {
    return isNaN(value)
             ? 0x7fc00000
             : toRawIntBits(value);
  }

  gint Float::toRawIntBits(gfloat value) {
    return *(Class<gint>::Pointer) &value;
  }

  gfloat Float::fromIntBits(gint bits) {
    return *(Class<gfloat>::Pointer) &bits;
  }

  gfloat Float::fromFloat16(gshort floatBinary16) {
    /**
     * The binary16 format has 1 sign bit, 5 exponent bits, and 10
     * significand bits. The exponent bias is 15.
     */
    gint bin16arg = (gint) floatBinary16;
    gint bin16SignBit = 0x8000 & bin16arg;
    gint bin16ExpBits = 0x7c00 & bin16arg;
    gint bin16SignificandBits = 0x03ff & bin16arg;

    // Shift left difference in the number of significand bits in
    // the float and binary16 formats
    CORE_FAST gint SIGNIFICAND_SHIFT = (FloatConsts::SIGNIFICAND_WIDTH - 11);

    gfloat const sign = (bin16SignBit != 0) ? -1.0f : 1.0f;

    // Extract binary16 exponent, remove its bias, add in the bias
    // of a float exponent and shift to correct bit location
    // (significand width includes the implicit bit so shift one
    // less).
    gint bin16Exp = (bin16ExpBits >> 10) - 15;
    if (bin16Exp == -15) {
      // For subnormal binary16 values and 0, the numerical
      // value is 2^24 * the significand as an integer (no
      // implicit bit).
      return sign * (0x1p-24F * (gfloat) bin16SignificandBits);
    } else if (bin16Exp == 16) {
      return (bin16SignificandBits == 0)
               ? sign * POSITIVE_INFINITY
               : fromIntBits((bin16SignBit << 16) | 0x7f800000 |
                 // Preserve NaN significand bits
                 (bin16SignificandBits << SIGNIFICAND_SHIFT));
    }
    //        CORE_ASSERT(-15 < bin16Exp && bin16Exp < 16);
    gint floatExpBits = (bin16Exp + FloatConsts::EXP_BIAS) << (FloatConsts::SIGNIFICAND_WIDTH - 1);

    // Compute and combine result sign, exponent, and significand bits.
    return fromIntBits((bin16SignBit << 16) | floatExpBits | (bin16SignificandBits << SIGNIFICAND_SHIFT));
  }

  gshort Float::toFloat16(gfloat f) {
    gint bit32 = toIntBits(f);
    gshort const sign = (gshort) ((bit32 & FloatConsts::SIGN_BIT_MASK) >> 16);
    if (isNaN(f)) {
      // Preserve sign and attempt to preserve significand bits
      return (gshort) (sign | 0x7c00 | (bit32 & 0x007fe000) >> 13
        | (bit32 & 0x00001ff0) >> 4 | (bit32 & 0x0000000f));
    }
    gfloat const uVal = Math::abs(f);
    // The overflow threshold is binary16 MAX_VALUE + 1/2 ulp
    if (uVal >= (0x1.ffcp15F + 0x0.002p15F))
      return (gshort) (sign | 0x7c00); // Positive or negative infinity
    // Smallest magnitude nonzero representable binary16 value
    // is equal to 0x1.0p-24; half-way and smaller rounds to zero.
    if (uVal <= 0x1.0p-24F * 0.5F) {
      // Covers float zeros and sub-normals.
      return sign; // Positive or negative zero
    }
    // Dealing with finite values in exponent range of binary16
    // (when rounding is done, could still round up)
    gint exp = ((toIntBits(f) & FloatConsts::EXP_BIT_MASK) >> (FloatConsts::SIGNIFICAND_WIDTH - 1)) -
        FloatConsts::EXP_BIAS;
    //        CORE_ASSERT(-25 <= exp && exp <= 15);
    // For binary16 subNormals, beside forcing exp to -15, retain
    // the difference exponentDiff = E_min - exp.  This is the excess
    // shift value, in addition to 13, to be used in the
    // computations below.  Further the (hidden) msb with value 1
    // in f must be involved as well.
    gint exponentDiff = 0;
    gint msb = 0x00000000;
    if (exp < -14) {
      exponentDiff = -14 - exp;
      exp = -15;
      msb = 0x00800000;
    }
    gint fSignificandBits = bit32 & 0x007fffff | msb;

    // Significand bits as if using rounding to zero (truncation).
    gshort significandBits = (gshort) (fSignificandBits >> (13 + exponentDiff));

    // For round to nearest even, determining whether to
    // round up (in magnitude) is a function of the least
    // significant bit (LSB), the next bit position (the round
    // position), and the sticky bit (whether there are any
    // nonzero bits in the exact result to the right of the round
    // digit). An increment occurs in three cases:
    //
    // LSB  Round Sticky
    // 0    1     1
    // 1    1     0
    // 1    1     1
    // See "Computer Arithmetic Algorithms," Koren, Table 4.9
    gint lsb = fSignificandBits & (1 << (13 + exponentDiff));
    gint round = fSignificandBits & (1 << (12 + exponentDiff));
    gint sticky = fSignificandBits & ((1 << (12 + exponentDiff)) - 1);

    if (round != 0 && ((lsb | sticky) != 0))
      significandBits++;

    // No bits setValue in significand beyond the *first* exponent bit,
    // not just the significant; quantity is added to the exponent
    // to implement a carry-out from rounding the significand.
    //        CORE_ASSERT((0xf800 & significandBits) == 0x0);
    return (gshort) (sign | (exp + 15 << 10) + significandBits);
  }

  gint Float::compareTo(Float const& other) const {
    return compare(value, other.value);
  }

  gint Float::compare(gfloat f1, gfloat f2) {
    return (f1 < f2) ? -1 : ((f1 > f2) ? 1 : ((f1 == f2) ? 0 : (isNaN(f1) ? (isNaN(f2) ? 0 : +1) : -1)));
  }

  gfloat Float::sum(gfloat a, gfloat b) {
    return a + b;
  }

  gfloat Float::max(gfloat a, gfloat b) {
    return Math::max(a, b);
  }

  gfloat Float::min(gfloat a, gfloat b) {
    return Math::min(a, b);
  }

  Float::operator gfloat() const {
    return floatValue();
  }

  Float::operator gfloat&() {
    return value;
  }

  Object& Float::clone() const {
    try {
      return UNSAFE::newInstance<Float>(*this);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }
} // core
