//
// Created by brunshweeck on 6 sept. 2024.
//

#include <meta/charset/Surrogate.h>

namespace core {
    namespace charset {
        gbool Surrogate::isHigh(gint c) { return (MIN_HIGH <= c) && (c <= MAX_HIGH); }

        gbool Surrogate::isLow(gint c) { return (MIN_LOW <= c) && (c <= MAX_LOW); }

        gbool Surrogate::is(gint c) { return (MIN <= c) && (c <= MAX); }

        gbool Surrogate::neededFor(gint uc) { return Character::isSupplementary(uc); }

        gchar Surrogate::high(gint uc) {
            CORE_ASSERT(Character::isSupplementary(uc));
            return Character::highSurrogate(uc);
        }

        gchar Surrogate::low(gint uc) {
            CORE_ASSERT(Character::isSupplementary(uc));
            return Character::lowSurrogate(uc);
        }

        gint Surrogate::toUCS4(gchar c, gchar d) {
            CORE_ASSERT(Character::isHighSurrogate(c) && Character::isLowSurrogate(d));
            return Character::toCodePoint(c, d);
        }

        gint Surrogate::Parser::character() const {
            CORE_ASSERT(err == CoderResult::UNDEFINED);
            return chr;
        }

        gbool Surrogate::Parser::isPair() const {
            CORE_ASSERT(err == CoderResult::UNDEFINED);
            return isUPair;
        }

        gint Surrogate::Parser::increment() const {
            CORE_ASSERT(err == CoderResult::UNDEFINED);
            return isUPair ? 2 : 1;
        }

        CoderResult Surrogate::Parser::error() {
            CORE_ASSERT(err != CoderResult::UNDEFINED);
            return err;
        }

        CoderResult Surrogate::Parser::unmappableResult() const {
            CORE_ASSERT(err == CoderResult::UNDEFINED);
            return CoderResult::unmappableForLength(isUPair ? 2 : 1);
        }

        gint Surrogate::Parser::parse(gchar c, io::CharBuffer &in) {
            if (Character::isHighSurrogate(c)) {
                if (!in.hasRemaining()) {
                    err = CoderResult::UNDERFLOW;
                    return -1;
                }
                gchar d = in.get();
                if (Character::isLowSurrogate(d)) {
                    chr = Character::toCodePoint(c, d);
                    isUPair = true;
                    err = CoderResult::UNDEFINED;
                    return chr;
                }
                err = CoderResult::malformedForLength(1);
                return -1;
            }
            if (Character::isLowSurrogate(c)) {
                err = CoderResult::malformedForLength(1);
                return -1;
            }
            chr = c;
            isUPair = false;
            err = CoderResult::UNDEFINED;
            return chr;
        }

        gint Surrogate::Parser::parse(gchar c, CharArray const &ia, gint ip, gint il) {
            CORE_ASSERT(ia[ip] == c);
            if (Character::isHighSurrogate(c)) {
                if (il - ip < 2) {
                    err = CoderResult::UNDERFLOW;
                    return -1;
                }
                gchar d = ia[ip + 1];
                if (Character::isLowSurrogate(d)) {
                    chr = Character::toCodePoint(c, d);
                    isUPair = true;
                    err = CoderResult::UNDEFINED;
                    return chr;
                }
                err = CoderResult::malformedForLength(1);
                return -1;
            }
            if (Character::isLowSurrogate(c)) {
                err = CoderResult::malformedForLength(1);
                return -1;
            }
            chr = c;
            isUPair = false;
            err = CoderResult::UNDEFINED;
            return chr;
        }

        CoderResult Surrogate::Generator::error() {
            CORE_ASSERT(err != CoderResult::UNDEFINED);
            return err;
        }

        gint Surrogate::Generator::generate(gint uc, gint len, io::CharBuffer &dst) {
            if (Character::isBmpCodePoint(uc)) {
                gchar c = CORE_CAST(gchar, uc);
                if (Character::isSurrogate(c)) {
                    err = CoderResult::malformedForLength(len);
                    return -1;
                }
                if (dst.remaining() < 1) {
                    err = CoderResult::OVERFLOW;
                    return -1;
                }
                dst.put(c);
                err = CoderResult::UNDEFINED;
                return 1;
            } else if (Character::isValidCodePoint(uc)) {
                if (dst.remaining() < 2) {
                    err = CoderResult::OVERFLOW;
                    return -1;
                }
                dst.put(Character::highSurrogate(uc));
                dst.put(Character::lowSurrogate(uc));
                err = CoderResult::UNDEFINED;
                return 2;
            } else {
                err = CoderResult::unmappableForLength(len);
                return -1;
            }
        }

        gint Surrogate::Generator::generate(gint uc, gint len, CharArray &da, gint dp, gint dl) {
            if (Character::isBmpCodePoint(uc)) {
                gchar c = CORE_CAST(gchar, uc);
                if (Character::isSurrogate(c)) {
                    err = CoderResult::malformedForLength(len);
                    return -1;
                }
                if (dl - dp < 1) {
                    err = CoderResult::OVERFLOW;
                    return -1;
                }
                da[dp] = c;
                err = CoderResult::UNDEFINED;
                return 1;
            } else if (Character::isValidCodePoint(uc)) {
                if (dl - dp < 2) {
                    err = CoderResult::OVERFLOW;
                    return -1;
                }
                da[dp] = Character::highSurrogate(uc);
                da[dp + 1] = Character::lowSurrogate(uc);
                err = CoderResult::UNDEFINED;
                return 2;
            } else {
                err = CoderResult::unmappableForLength(len);
                return -1;
            }
        }
    } // charset
} // core
