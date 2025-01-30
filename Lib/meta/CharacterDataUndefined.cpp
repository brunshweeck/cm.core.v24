//
// Created by bruns on 19/03/2024.
//

#include <meta/CharacterDataUndefined.h>

namespace core {
    gint CharacterDataUndefined::properties(gint ch) {
        return 0;
    }

    gint CharacterDataUndefined::type(gint ch) {
        return CORE_CAST(gint, Character::Category::UNASSIGNED);
    }

    gint CharacterDataUndefined::toLowerCase(gint ch) {
        return ch;
    }

    gint CharacterDataUndefined::toUpperCase(gint ch) {
        return ch;
    }

    gint CharacterDataUndefined::toTitleCase(gint ch) {
        return ch;
    }

    gint CharacterDataUndefined::digit(gint ch, gint radix) {
        return -1;
    }

    gint CharacterDataUndefined::numericValue(gint ch) {
        return -1;
    }

    gbool CharacterDataUndefined::isDigit(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isLowerCase(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isUpperCase(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isWhitespace(gint ch) {
        return false;
    }

    gbyte CharacterDataUndefined::directionality(gint ch) {
        return (gint) Character::Directionality::DIRECTIONALITY_UNDEFINED;
    }

    gbool CharacterDataUndefined::isMirrored(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isEmoji(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isEmojiPresentation(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isEmojiModifier(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isEmojiModifierBase(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isEmojiComponent(gint ch) {
        return false;
    }

    gbool CharacterDataUndefined::isExtendedPictographic(gint ch) {
        return false;
    }

    CharacterDataUndefined::CharacterDataUndefined() = default;

    CharacterDataUndefined CharacterDataUndefined::instance{};
} // core
