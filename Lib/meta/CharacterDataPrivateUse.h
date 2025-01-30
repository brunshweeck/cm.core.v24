//
// Created by bruns on 19/03/2024.
//

#ifndef CORE24_CHARACTERDATAPRIVATEUSE_H
#define CORE24_CHARACTERDATAPRIVATEUSE_H

#include <meta/CharacterData.h>

namespace core {

    class CharacterDataPrivateUse final: public virtual CharacterData {
    public:
        gint properties(gint ch) override;

        gint type(gint ch) override;

        gint toLowerCase(gint ch) override;

        gint toUpperCase(gint ch) override;

        gint toTitleCase(gint ch) override;

        gint numericValue(gint ch) override;

        gbool isDigit(gint ch) override;

        gbool isLowerCase(gint ch) override;

        gbool isUpperCase(gint ch) override;

        gbool isWhitespace(gint ch) override;

        gbyte directionality(gint ch) override;

        gbool isMirrored(gint ch) override;

        gbool isEmoji(gint ch) override;

        gbool isEmojiPresentation(gint ch) override;

        gbool isEmojiModifier(gint ch) override;

        gbool isEmojiModifierBase(gint ch) override;

        gbool isEmojiComponent(gint ch) override;

        gbool isExtendedPictographic(gint ch) override;

        gint digit(gint ch, gint radix) override;

        static CharacterDataPrivateUse instance;

    private:
        CharacterDataPrivateUse();
    };

} // core

#endif //CORE24_CHARACTERDATAPRIVATEUSE_H
