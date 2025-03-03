//
// Created by bruns on 14/10/2024.
//

#include <meta/regex/Pattern.ASCII.h>
#include "Pattern.ASCII.h"

namespace core {
    namespace regex {
        gint Pattern::ASCII::ctype[256] = {
            CNTRL, /* 00 (NUL) */
            CNTRL, /* 01 (SOH) */
            CNTRL, /* 02 (STX) */
            CNTRL, /* 03 (ETX) */
            CNTRL, /* 04 (EOT) */
            CNTRL, /* 05 (ENQ) */
            CNTRL, /* 06 (ACK) */
            CNTRL, /* 07 (BEL) */
            CNTRL, /* 08 (BS)  */
            SPACE + CNTRL + BLANK, /* 09 (HT)  */
            SPACE + CNTRL, /* 0A (LF)  */
            SPACE + CNTRL, /* 0B (VT)  */
            SPACE + CNTRL, /* 0C (FF)  */
            SPACE + CNTRL, /* 0D (CR)  */
            CNTRL, /* 0E (SI)  */
            CNTRL, /* 0F (SO)  */
            CNTRL, /* 10 (DLE) */
            CNTRL, /* 11 (DC1) */
            CNTRL, /* 12 (DC2) */
            CNTRL, /* 13 (DC3) */
            CNTRL, /* 14 (DC4) */
            CNTRL, /* 15 (NAK) */
            CNTRL, /* 16 (SYN) */
            CNTRL, /* 17 (ETB) */
            CNTRL, /* 18 (CAN) */
            CNTRL, /* 19 (EM)  */
            CNTRL, /* 1A (SUB) */
            CNTRL, /* 1B (ESC) */
            CNTRL, /* 1C (FS)  */
            CNTRL, /* 1D (GS)  */
            CNTRL, /* 1E (RS)  */
            CNTRL, /* 1F (US)  */
            SPACE + BLANK, /* 20 SPACE */
            PUNCT, /* 21 !     */
            PUNCT, /* 22 "     */
            PUNCT, /* 23 #     */
            PUNCT, /* 24 $     */
            PUNCT, /* 25 %     */
            PUNCT, /* 26 &     */
            PUNCT, /* 27 '     */
            PUNCT, /* 28 (     */
            PUNCT, /* 29 )     */
            PUNCT, /* 2A *     */
            PUNCT, /* 2B +     */
            PUNCT, /* 2C ,     */
            PUNCT, /* 2D -     */
            PUNCT, /* 2E .     */
            PUNCT, /* 2F /     */
            DIGIT + HEX + 0, /* 30 0     */
            DIGIT + HEX + 1, /* 31 1     */
            DIGIT + HEX + 2, /* 32 2     */
            DIGIT + HEX + 3, /* 33 3     */
            DIGIT + HEX + 4, /* 34 4     */
            DIGIT + HEX + 5, /* 35 5     */
            DIGIT + HEX + 6, /* 36 6     */
            DIGIT + HEX + 7, /* 37 7     */
            DIGIT + HEX + 8, /* 38 8     */
            DIGIT + HEX + 9, /* 39 9     */
            PUNCT, /* 3A :     */
            PUNCT, /* 3B ;     */
            PUNCT, /* 3C <     */
            PUNCT, /* 3D =     */
            PUNCT, /* 3E >     */
            PUNCT, /* 3F ?     */
            PUNCT, /* 40 @     */
            UPPER + HEX + 10, /* 41 A     */
            UPPER + HEX + 11, /* 42 B     */
            UPPER + HEX + 12, /* 43 C     */
            UPPER + HEX + 13, /* 44 D     */
            UPPER + HEX + 14, /* 45 E     */
            UPPER + HEX + 15, /* 46 F     */
            UPPER + 16, /* 47 G     */
            UPPER + 17, /* 48 H     */
            UPPER + 18, /* 49 I     */
            UPPER + 19, /* 4A J     */
            UPPER + 20, /* 4B K     */
            UPPER + 21, /* 4C L     */
            UPPER + 22, /* 4D M     */
            UPPER + 23, /* 4E N     */
            UPPER + 24, /* 4F O     */
            UPPER + 25, /* 50 P     */
            UPPER + 26, /* 51 Q     */
            UPPER + 27, /* 52 R     */
            UPPER + 28, /* 53 S     */
            UPPER + 29, /* 54 T     */
            UPPER + 30, /* 55 U     */
            UPPER + 31, /* 56 V     */
            UPPER + 32, /* 57 W     */
            UPPER + 33, /* 58 X     */
            UPPER + 34, /* 59 Y     */
            UPPER + 35, /* 5A Z     */
            PUNCT, /* 5B [     */
            PUNCT, /* 5C \     */
            PUNCT, /* 5D ]     */
            PUNCT, /* 5E ^     */
            PUNCT | UNDER, /* 5F _     */
            PUNCT, /* 60 `     */
            LOWER + HEX + 10, /* 61 a     */
            LOWER + HEX + 11, /* 62 b     */
            LOWER + HEX + 12, /* 63 c     */
            LOWER + HEX + 13, /* 64 d     */
            LOWER + HEX + 14, /* 65 e     */
            LOWER + HEX + 15, /* 66 f     */
            LOWER + 16, /* 67 g     */
            LOWER + 17, /* 68 h     */
            LOWER + 18, /* 69 i     */
            LOWER + 19, /* 6A j     */
            LOWER + 20, /* 6B k     */
            LOWER + 21, /* 6C l     */
            LOWER + 22, /* 6D m     */
            LOWER + 23, /* 6E n     */
            LOWER + 24, /* 6F o     */
            LOWER + 25, /* 70 p     */
            LOWER + 26, /* 71 q     */
            LOWER + 27, /* 72 r     */
            LOWER + 28, /* 73 s     */
            LOWER + 29, /* 74 t     */
            LOWER + 30, /* 75 u     */
            LOWER + 31, /* 76 v     */
            LOWER + 32, /* 77 w     */
            LOWER + 33, /* 78 x     */
            LOWER + 34, /* 79 y     */
            LOWER + 35, /* 7A z     */
            PUNCT, /* 7B {     */
            PUNCT, /* 7C |     */
            PUNCT, /* 7D }     */
            PUNCT, /* 7E ~     */
            CNTRL, /* 7F (DEL) */
        };

        gint Pattern::ASCII::getType(gint ch) {
            return ((ch & 0xFFFFFF80) == 0 ? ctype[ch] : 0);
        }

        gbool Pattern::ASCII::isType(gint ch, gint type) {
            return (getType(ch) & type) != 0;
        }

        gbool Pattern::ASCII::isAscii(gint ch) {
            return ch < 128 && ((ch & 0xFFFFFF80) == 0);
        }

        gbool Pattern::ASCII::isAlpha(gint ch) {
            return ch < 128 && isType(ch, ALPHA);
        }

        gbool Pattern::ASCII::isDigit(gint ch) {
            return ((ch - '0') | ('9' - ch)) >= 0;
        }

        gbool Pattern::ASCII::isAlnum(gint ch) {
            return ch < 128 && isType(ch, ALNUM);
        }

        gbool Pattern::ASCII::isGraph(gint ch) {
            return ch < 128 && isType(ch, GRAPH);
        }

        gbool Pattern::ASCII::isPrint(gint ch) {
            return ((ch - 0x20) | (0x7E - ch)) >= 0;
        }

        gbool Pattern::ASCII::isPunct(gint ch) {
            return ch < 128 && isType(ch, PUNCT);
        }

        gbool Pattern::ASCII::isSpace(gint ch) {
            return ch < 128 && isType(ch, SPACE);
        }

        gbool Pattern::ASCII::isHexDigit(gint ch) {
            return ch < 128 && isType(ch, HEX);
        }

        gbool Pattern::ASCII::isOctDigit(gint ch) {
            return ((ch-'0')|('7'-ch)) >= 0;
        }

        gbool Pattern::ASCII::isCntrl(gint ch) {
            return ch < 128 && isType(ch, CNTRL);
        }

        gbool Pattern::ASCII::isLower(gint ch) {
            return ((ch-'a')|('z'-ch)) >= 0;
        }

        gbool Pattern::ASCII::isUpper(gint ch) {
            return ((ch-'A')|('Z'-ch)) >= 0;
        }

        gbool Pattern::ASCII::isWord(gint ch) {
            return ch < 128 && isType(ch, WORD);
        }

        gbool Pattern::ASCII::isBlank(gint ch){
            return ch < 128 && isType(ch, BLANK);
        }
        gint Pattern::ASCII::toDigit(gint ch)
        {
            return (ctype[ch & 0x7F] & 0x3F);
        }

        gint Pattern::ASCII::toLower(gint ch) {
            return isUpper(ch) ? (ch + 0x20) : ch;
        }

        gint Pattern::ASCII::toUpper(gint ch) {
            return isLower(ch) ? (ch - 0x20) : ch;
        }
    } // regex
} // core
