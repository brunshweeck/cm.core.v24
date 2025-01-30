//
// Created by bruns on 19/03/2024.
//

#ifndef CORE24_CHARACTERDATA01_H
#define CORE24_CHARACTERDATA01_H

#include <meta/CharacterData.h>

namespace core {

    /** The CharacterData class encapsulates the large tables once found in
     *  Character.
     */
    class CharacterData01 final : public CharacterData {
        /* The character properties are currently encoded into 32 bits in the following manner:
            1 bit   mirrored property
            4 bits  directionality property
            9 bits  signed offset used for converting case
            1 bit   if 1, adding the signed offset converts the character to lowercase
            1 bit   if 1, subtracting the signed offset converts the character to uppercase
            1 bit   if 1, this character has a titlecase equivalent (possibly itself)
            3 bits  0  may not be part of an identifier
                    1  ignorable control; may continue a Unicode identifier or Java identifier
                    2  may continue a Java identifier but not a Unicode identifier (unused)
                    3  may continue a Unicode identifier or Java identifier
                    4  is a Java whitespace character
                    5  may start or continue a Java identifier;
                       may continue but not start a Unicode identifier (underscores)
                    6  may start or continue a Java identifier but not a Unicode identifier ($)
                    7  may start or continue a Unicode identifier or Java identifier
                    Thus:
                       5, 6, 7 may start a Java identifier
                       1, 2, 3, 5, 6, 7 may continue a Java identifier
                       7 may start a Unicode identifier
                       1, 3, 5, 7 may continue a Unicode identifier
                       1 is ignorable within an identifier
                       4 is Java whitespace
            2 bits  0  this character has no numeric property
                    1  adding the digit offset to the character code and then
                       masking with 0x1F will produce the desired numeric value
                    2  this character has a "strange" numeric value
                    3  a Java supradecimal digit: adding the digit offset to the
                       character code, then masking with 0x1F, then adding 10
                       will produce the desired numeric value
            5 bits  digit offset
            5 bits  character type

            The encoding of character properties is subject to change at any time.
         */

        CharacterData01();
    public:

        gint properties(gint ch) override;

        gint propertiesEx(gint ch);

        gint type(gint ch) override;

        gbool isOtherAlphabetic(gint ch) override;

        gbool isIdeographic(gint ch) override;

        gint toLowerCase(gint ch) override;

        gint toUpperCase(gint ch) override;

        gint toTitleCase(gint ch) override;

        gint digit(gint ch, gint radix) override;

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

        static CharacterData01 instance;

    };

} // core

#endif //CORE24_CHARACTERDATA01_H
