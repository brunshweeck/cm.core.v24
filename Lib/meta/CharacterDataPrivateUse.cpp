//
// Created by bruns on 19/03/2024.
//

#include <meta/CharacterDataPrivateUse.h>

namespace core {
    gint CharacterDataPrivateUse::properties(gint ch) {
        return 0;
    }

    gint CharacterDataPrivateUse::type(gint ch) {
        return (ch & 0xFFFE) == 0xFFFE
                   ? (gint) Character::Category::UNASSIGNED
                   : (gint) Character::Category::PRIVATE_USE;
    }

    gint CharacterDataPrivateUse::toLowerCase(gint ch) {
        return ch;
    }

    gint CharacterDataPrivateUse::toUpperCase(gint ch) {
        return ch;
    }

    gint CharacterDataPrivateUse::toTitleCase(gint ch) {
        return ch;
    }

    gint CharacterDataPrivateUse::numericValue(gint ch) {
        return -1;
    }

    gbool CharacterDataPrivateUse::isDigit(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isLowerCase(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isUpperCase(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isWhitespace(gint ch) {
        return false;
    }

    gbyte CharacterDataPrivateUse::directionality(gint ch) {
        return (ch & 0xFFFE) == 0xFFFE
                   ? (gint) Character::Directionality::DIRECTIONALITY_UNDEFINED
                   : (gint) Character::Directionality::DIRECTIONALITY_LEFT_TO_RIGHT;
    }

    gbool CharacterDataPrivateUse::isMirrored(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isEmoji(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isEmojiPresentation(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isEmojiModifier(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isEmojiModifierBase(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isEmojiComponent(gint ch) {
        return false;
    }

    gbool CharacterDataPrivateUse::isExtendedPictographic(gint ch) {
        return false;
    }

    gint CharacterDataPrivateUse::digit(gint ch, gint radix) {
        return -1;
    }

    CharacterDataPrivateUse::CharacterDataPrivateUse() = default;

    CharacterDataPrivateUse CharacterDataPrivateUse::instance{};
} // core
