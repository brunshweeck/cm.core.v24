//
// Created by bruns on 19/03/2024.
//

#ifndef CORE24_CHARACTERDATA_H
#define CORE24_CHARACTERDATA_H

#include <core/lang/Character.h>

namespace core {

    class CharacterData: public virtual Object {
    protected:
        CharacterData();

    public:
        virtual gint properties(gint ch) = 0;
        virtual gint type(gint ch) = 0;
        virtual gbool isDigit(gint ch) = 0;
        virtual gbool isLowerCase(gint ch) = 0;
        virtual gbool isUpperCase(gint ch) = 0;
        virtual gbool isWhitespace(gint ch) = 0;
        virtual gbool isMirrored(gint ch) = 0;
        virtual gbool isEmoji(gint ch) = 0;
        virtual gbool isEmojiPresentation(gint ch) = 0;
        virtual gbool isEmojiModifier(gint ch) = 0;
        virtual gbool isEmojiModifierBase(gint ch) = 0;
        virtual gbool isEmojiComponent(gint ch) = 0;
        virtual gbool isExtendedPictographic(gint ch) = 0;
        virtual gint toLowerCase(gint ch) = 0;
        virtual gint toUpperCase(gint ch) = 0;
        virtual gint toTitleCase(gint ch) = 0;
        virtual gint digit(gint ch, gint radix) = 0;
        virtual gint numericValue(gint ch) = 0;
        virtual gbyte directionality(gint ch) = 0;

        virtual //need to implement for JSR204
        gint toUpperCaseEx(gint ch);

        virtual CharArray toUpperCaseCharArray(gint ch);

        virtual gbool isOtherAlphabetic(gint ch);

        virtual gbool isIdeographic(gint ch);

        // Character <= 0xff (basic latin) is handled by internal fast-path
        // to avoid initializing large tables.
        // Note: performance of this "fast-path" code may be sub-optimal
        // in negative cases for some accessors due to complicated ranges.
        // Should revisit after optimization of table initialization.

        static CharacterData &of(gint ch);
    };

} // core

#endif //CORE24_CHARACTERDATA_H
