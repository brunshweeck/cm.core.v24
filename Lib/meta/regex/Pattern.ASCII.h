//
// Created by bruns on 14/10/2024.
//

#ifndef CORE24_ASCII_H
#define CORE24_ASCII_H

#include <core/regex/Pattern.h>

namespace core {
    namespace regex {
        class Pattern::ASCII final: public Object {
        public:
            static CORE_FAST gint UPPER   = 0x00000100;

            static CORE_FAST gint LOWER   = 0x00000200;

            static CORE_FAST gint DIGIT   = 0x00000400;

            static CORE_FAST gint SPACE   = 0x00000800;

            static CORE_FAST gint PUNCT   = 0x00001000;

            static CORE_FAST gint CNTRL   = 0x00002000;

            static CORE_FAST gint BLANK   = 0x00004000;

            static CORE_FAST gint HEX     = 0x00008000;

            static CORE_FAST gint UNDER   = 0x00010000;

            static CORE_FAST gint ASCII_  = 0x0000FF00;

            static CORE_FAST gint ALPHA   = (UPPER|LOWER);

            static CORE_FAST gint ALNUM   = (UPPER|LOWER|DIGIT);

            static CORE_FAST gint GRAPH   = (PUNCT|UPPER|LOWER|DIGIT);

            static CORE_FAST gint WORD    = (UPPER|LOWER|UNDER|DIGIT);

            static CORE_FAST gint XDIGIT  = (HEX);

            static gint getType(gint ch);

            static gbool isType(gint ch, gint type);

            static gbool isAscii(gint ch);

            static gbool isAlpha(gint ch);

            static gbool isDigit(gint ch);

            static gbool isAlnum(gint ch);

            static gbool isGraph(gint ch);

            static gbool isPrint(gint ch);

            static gbool isPunct(gint ch);

            static gbool isSpace(gint ch);

            static gbool isHexDigit(gint ch);

            static gbool isOctDigit(gint ch);

            static gbool isCntrl(gint ch);

            static gbool isLower(gint ch);

            static gbool isUpper(gint ch);

            static gbool isWord(gint ch);

            static gbool isBlank(gint ch);

            static gint toDigit(gint ch);

            static gint toLower(gint ch);

            static gint toUpper(gint ch);

            static gint ctype[256];
        };
    } // regex
} // core

#endif //CORE24_ASCII_H
