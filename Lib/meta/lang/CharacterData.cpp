//
// Created by bruns on 19/03/2024.
//

#include <core/lang/CharArray.h>
#include "CharacterData.h"
#include "CharacterData00.h"
#include "CharacterData01.h"
#include "CharacterData02.h"
#include "CharacterData03.h"
#include "CharacterData0E.h"
#include "CharacterDataLatin1.h"
#include "CharacterDataPrivateUse.h"
#include "CharacterDataUndefined.h"

namespace core
{
    CharacterData::CharacterData() = default;

    gint CharacterData::toUpperCaseEx(gint ch)
    {
        return toUpperCase(ch);
    }

    CharArray CharacterData::toUpperCaseCharArray(gint ch)
    {
        return Character::toChars(ch);
    }

    gbool CharacterData::isOtherAlphabetic(gint ch)
    {
        return false;
    }

    gbool CharacterData::isIdeographic(gint ch)
    {
        return false;
    }

    // Character <= 0xff (basic latin) is handled by internal fast-path
    // to avoid initializing large tables.
    // Note: performance of this "fast-path" code may be sub-optimal
    // in negative cases for some accessors due to complicated ranges.
    // Should revisit after optimization of table initialization.
    CharacterData& CharacterData::of(gint ch)
    {
        if ((ch + 0u) >> 8 == 0)
        {
            return CharacterDataLatin1::instance;
        }
        switch ((ch + 0u) >> 16)
        {
        //plane 00-16
        case 0: return CharacterData00::instance;
        case 1: return CharacterData01::instance;
        case 2: return CharacterData02::instance;
        case 3: return CharacterData03::instance;
        case 14: return CharacterData0E::instance;
        case 15:
        case 16: return CharacterDataPrivateUse::instance;
        default: return CharacterDataUndefined::instance;
        }
    }
} // core
