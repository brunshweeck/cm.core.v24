//
// Created by admin on 03/12/24.
//

#ifndef DECIMALFORMATTER_H
#define DECIMALFORMATTER_H

#include <core/lang/String.h>

namespace core {
    namespace ext {
        class DecimalFormatter final : public Object {
        public:
            static CORE_FAST gchar SCIENTIFIC = 'e';
            static CORE_FAST gchar PLAIN = 'f';
            static CORE_FAST gchar GENERAL = 'g';

            glong f = 0L;
            gint e = 0; // normalized to 0 when f = 0
            gint n = 0;
            CharArray digits; // ... and often the decimal separator as well
            CharArray exp; // [+-][e]ee, that is, sign and minimum 2 digits

            DecimalFormatter() = default;

            static DecimalFormatter valueOf(gdouble v, gint prec, gchar form);

            void set(glong f, gint e, gint n);

            CharArray getExponent() const;

            CharArray getMantissa() const;

            gint getExponentRounded() const;

        private:
            DecimalFormatter plain(gint prec);

            DecimalFormatter& plainChars();

            void plainCharsPureInteger();

            void plainCharsMixed();

            void plainCharsPureFraction();

            DecimalFormatter scientific(gint prec);

            DecimalFormatter scientificChars(gint prec);

            void scientificCharsWithFraction();

            void scientificCharsNoFraction();

            DecimalFormatter general(gint prec);

            void expChars();

            void round(glong pp);

            /*
             * Fills the digits section with indices in [from, to) with the lower
             * to - from digits of x (as chars), while stripping them away from x.
             * Returns the stripped x.
             */
            glong fillWithDigits(glong x, gint from, gint to);

            /*
             * Fills the digits section with indices in [from, to) with '0'.
             */
            void fillWithZeros(gint from, gint to);

            static gchar toDigit(glong d);

            static gchar toDigit(gint d);
        };
    } // ext
} // core

#endif //DECIMALFORMATTER_H
