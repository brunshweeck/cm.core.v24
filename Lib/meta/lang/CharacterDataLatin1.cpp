//
// Created by bruns on 19/03/2024.
//

#include "CharacterDataLatin1.h"

namespace core {
    // Digit values for codePoints in the 0-255 range. Contents generated using:
    // for (char i = 0; i < 256; i++) {
    //     int v = -1;
    //     if (i >= '0' && i <= '9') { v = i - '0'; }
    //     else if (i >= 'A' && i <= 'Z') { v = i - 'A' + 10; }
    //     else if (i >= 'a' && i <= 'z') { v = i - 'a' + 10; }
    //     if (i % 20 == 0) System.out.println();
    //     System.out.printf("%2d, ", v);
    // }
    //
    // Analysis has shown that generating the whole array allows the JIT to generate
    // better code compared to a slimmed down array, such as one cutting off after 'z'
    static CORE_FAST gint DIGITS[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1,
        -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1, -1, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
        33, 34, 35, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };

    static CORE_FAST gint A[] = {
        0x4800100F, //   0   Cc, ignorable
        0x4800100F, //   1   Cc, ignorable
        0x4800100F, //   2   Cc, ignorable
        0x4800100F, //   3   Cc, ignorable
        0x4800100F, //   4   Cc, ignorable
        0x4800100F, //   5   Cc, ignorable
        0x4800100F, //   6   Cc, ignorable
        0x4800100F, //   7   Cc, ignorable
        0x4800100F, //   8   Cc, ignorable
        0x5800400F, //   9   Cc, S, whitespace
        0x5000400F, //  10   Cc, B, whitespace
        0x5800400F, //  11   Cc, S, whitespace
        0x6000400F, //  12   Cc, WS, whitespace
        0x5000400F, //  13   Cc, B, whitespace
        0x4800100F, //  14   Cc, ignorable
        0x4800100F, //  15   Cc, ignorable
        0x4800100F, //  16   Cc, ignorable
        0x4800100F, //  17   Cc, ignorable
        0x4800100F, //  18   Cc, ignorable
        0x4800100F, //  19   Cc, ignorable
        0x4800100F, //  20   Cc, ignorable
        0x4800100F, //  21   Cc, ignorable
        0x4800100F, //  22   Cc, ignorable
        0x4800100F, //  23   Cc, ignorable
        0x4800100F, //  24   Cc, ignorable
        0x4800100F, //  25   Cc, ignorable
        0x4800100F, //  26   Cc, ignorable
        0x4800100F, //  27   Cc, ignorable
        0x5000400F, //  28   Cc, B, whitespace
        0x5000400F, //  29   Cc, B, whitespace
        0x5000400F, //  30   Cc, B, whitespace
        0x5800400F, //  31   Cc, S, whitespace
        0x6000400C, //  32   Zs, WS, whitespace
        0x68000018, //  33   Po, ON
        0x68000018, //  34   Po, ON
        -0xD7FFFFE8,
        //  35   Mc, B, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31, emoji, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        0x2800601A, //  36   Sc, ET, currency
        0x28000018, //  37   Po, ET
        0x68000018, //  38   Po, ON
        0x68000018, //  39   Po, ON
        -0x17FFFFEB,
        //  40   No, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        -0x17FFFFEA,
        //  41   Nl, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        -0x97FFFFE8,
        //  42   Mc, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31, emoji, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        0x20000019, //  43   Sm, ES
        0x38000018, //  44   Po, CS
        0x20000014, //  45   Pd, ES
        0x38000018, //  46   Po, CS
        0x38000018, //  47   Po, CS
        -0xE7FFC9F7,
        //  48   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  49   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  50   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  51   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  52   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  53   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  54   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  55   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  56   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        -0xE7FFC9F7,
        //  57   Pc, WS, hasUpper (subtract 511), hasLower (add 511), hasTitle, whitespace, strange, IDContinue, emojiPresentation, emojiModifier, emojiModifierBase, extendedPictographic
        0x38000018, //  58   Po, CS
        0x68000018, //  59   Po, ON
        -0x17FFFFE7,
        //  60   Me, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        0x68000019, //  61   Sm, ON
        -0x17FFFFE7,
        //  62   Me, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        0x68000018, //  63   Po, ON
        0x68000018, //  64   Po, ON
        -0xFF7D801F,
        //  65   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  66   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  67   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  68   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  69   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  70   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  71   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  72   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  73   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  74   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  75   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  76   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  77   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  78   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  79   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  80   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  81   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  82   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  83   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  84   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  85   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  86   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  87   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  88   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  89   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D801F,
        //  90   , hasUpper (subtract 479), hasTitle, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0x17FFFFEB,
        //  91   No, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        0x68000018, //  92   Po, ON
        -0x17FFFFEA,
        //  93   Nl, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        0x6800001B, //  94   Sk, ON
        -0x97FFAFE9,
        //  95   Nd, hasUpper (subtract 511), hasLower (add 511), hasTitle, supradecimal 31, IDContinue, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        0x6800001B, //  96   Sk, ON
        -0xFF7E801E,
        //  97   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        //  98   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        //  99   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 100   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 101   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 102   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 103   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 104   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 105   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 106   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 107   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 108   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 109   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 110   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 111   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 112   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 113   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 114   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 115   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 116   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 117   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 118   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 119   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 120   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 121   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E801E,
        // 122   , hasLower (add 479), hasTitle, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0x17FFFFEB,
        // 123   No, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        0x68000019, // 124   Sm, ON
        -0x17FFFFEA,
        // 125   Nl, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        0x68000019, // 126   Sm, ON
        0x4800100F, // 127   Cc, ignorable
        0x4800100F, // 128   Cc, ignorable
        0x4800100F, // 129   Cc, ignorable
        0x4800100F, // 130   Cc, ignorable
        0x4800100F, // 131   Cc, ignorable
        0x4800100F, // 132   Cc, ignorable
        0x5000100F, // 133   Cc, B, ignorable
        0x4800100F, // 134   Cc, ignorable
        0x4800100F, // 135   Cc, ignorable
        0x4800100F, // 136   Cc, ignorable
        0x4800100F, // 137   Cc, ignorable
        0x4800100F, // 138   Cc, ignorable
        0x4800100F, // 139   Cc, ignorable
        0x4800100F, // 140   Cc, ignorable
        0x4800100F, // 141   Cc, ignorable
        0x4800100F, // 142   Cc, ignorable
        0x4800100F, // 143   Cc, ignorable
        0x4800100F, // 144   Cc, ignorable
        0x4800100F, // 145   Cc, ignorable
        0x4800100F, // 146   Cc, ignorable
        0x4800100F, // 147   Cc, ignorable
        0x4800100F, // 148   Cc, ignorable
        0x4800100F, // 149   Cc, ignorable
        0x4800100F, // 150   Cc, ignorable
        0x4800100F, // 151   Cc, ignorable
        0x4800100F, // 152   Cc, ignorable
        0x4800100F, // 153   Cc, ignorable
        0x4800100F, // 154   Cc, ignorable
        0x4800100F, // 155   Cc, ignorable
        0x4800100F, // 156   Cc, ignorable
        0x4800100F, // 157   Cc, ignorable
        0x4800100F, // 158   Cc, ignorable
        0x4800100F, // 159   Cc, ignorable
        0x3800000C, // 160   Zs, CS
        0x68000018, // 161   Po, ON
        0x2800601A, // 162   Sc, ET, currency
        0x2800601A, // 163   Sc, ET, currency
        0x2800601A, // 164   Sc, ET, currency
        0x2800601A, // 165   Sc, ET, currency
        0x6800001C, // 166   So, ON
        0x68000018, // 167   Po, ON
        0x6800001B, // 168   Sk, ON
        -0x97FFFFE4,
        // 169   Lm, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent
        -0xFFFF8FFB,
        // 170   Sk, hasUpper (subtract 511), hasLower (add 511), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0x17FFFFE3,
        // 171   Lt, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        0x68000019, // 172   Sm, ON
        0x48001010, // 173   Cf, ignorable
        -0x97FFFFE4,
        // 174   Lm, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent
        0x6800001B, // 175   Sk, ON
        0x2800001C, // 176   So, ET
        0x28000019, // 177   Sm, ET
        0x1800060B, // 178   No, EN, decimal 16
        0x1800060B, // 179   No, EN, decimal 16
        0x6800001B, // 180   Sk, ON
        -0xF8028FFE,
        // 181   , hasLower (add 0), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        0x68000018, // 182   Po, ON
        -0x97FFFFE8,
        // 183   Mc, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31, IDContinue, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        0x6800001B, // 184   Sk, ON
        0x1800050B, // 185   No, EN, decimal 8
        -0xFFFF8FFB,
        // 186   Sk, hasUpper (subtract 511), hasLower (add 511), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0x17FFFFE2,
        // 187   Ll, hasUpper (subtract 511), hasLower (add 511), hasTitle, identifier start, supradecimal 31
        0x6800080B, // 188   No, ON, strange
        0x6800080B, // 189   No, ON, strange
        0x6800080B, // 190   No, ON, strange
        0x68000018, // 191   Po, ON
        -0xFF7D8FFF,
        // 192   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 193   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 194   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 195   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 196   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 197   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 198   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 199   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 200   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 201   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 202   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 203   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 204   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 205   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 206   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 207   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 208   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 209   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 210   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 211   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 212   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 213   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 214   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        0x68000019, // 215   Sm, ON
        -0xFF7D8FFF,
        // 216   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 217   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 218   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 219   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 220   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 221   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7D8FFF,
        // 222   , hasUpper (subtract 479), hasTitle, supradecimal 31, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xF8028FFE,
        // 223   , hasLower (add 0), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 224   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 225   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 226   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 227   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 228   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 229   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 230   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 231   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 232   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 233   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 234   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 235   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 236   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 237   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 238   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 239   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 240   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 241   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 242   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 243   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 244   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 245   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 246   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        0x68000019, // 247   Sm, ON
        -0xFF7E8FFE,
        // 248   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 249   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 250   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 251   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 252   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 253   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xFF7E8FFE,
        // 254   , hasLower (add 479), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
        -0xF9E28FFE
        // 255   , hasLower (add 120), hasTitle, supradecimal 31, otherLowercase, otherUppercase, otherAlphabetic, ideographic, emoji, emojiPresentation, emojiModifier, emojiModifierBase, emojiComponent, extendedPictographic
    };

    static CORE_FAST gint B[] = {
        0x0000, //   0   unassigned, L
        0x0000, //   1   unassigned, L
        0x0000, //   2   unassigned, L
        0x0000, //   3   unassigned, L
        0x0000, //   4   unassigned, L
        0x0000, //   5   unassigned, L
        0x0000, //   6   unassigned, L
        0x0000, //   7   unassigned, L
        0x0000, //   8   unassigned, L
        0x0000, //   9   unassigned, L
        0x0000, //  10   unassigned, L
        0x0000, //  11   unassigned, L
        0x0000, //  12   unassigned, L
        0x0000, //  13   unassigned, L
        0x0000, //  14   unassigned, L
        0x0000, //  15   unassigned, L
        0x0000, //  16   unassigned, L
        0x0000, //  17   unassigned, L
        0x0000, //  18   unassigned, L
        0x0000, //  19   unassigned, L
        0x0000, //  20   unassigned, L
        0x0000, //  21   unassigned, L
        0x0000, //  22   unassigned, L
        0x0000, //  23   unassigned, L
        0x0000, //  24   unassigned, L
        0x0000, //  25   unassigned, L
        0x0000, //  26   unassigned, L
        0x0000, //  27   unassigned, L
        0x0000, //  28   unassigned, L
        0x0000, //  29   unassigned, L
        0x0000, //  30   unassigned, L
        0x0000, //  31   unassigned, L
        0x0000, //  32   unassigned, L
        0x0000, //  33   unassigned, L
        0x0000, //  34   unassigned, L
        0x0440, //  35   unassigned, L, emoji, emojiComponent
        0x0000, //  36   unassigned, L
        0x0000, //  37   unassigned, L
        0x0000, //  38   unassigned, L
        0x0000, //  39   unassigned, L
        0x0000, //  40   unassigned, L
        0x0000, //  41   unassigned, L
        0x0440, //  42   unassigned, L, emoji, emojiComponent
        0x0000, //  43   unassigned, L
        0x0000, //  44   unassigned, L
        0x0000, //  45   unassigned, L
        0x0000, //  46   unassigned, L
        0x0000, //  47   unassigned, L
        0x0460, //  48   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  49   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  50   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  51   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  52   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  53   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  54   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  55   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  56   unassigned, L, IDContinue, emoji, emojiComponent
        0x0460, //  57   unassigned, L, IDContinue, emoji, emojiComponent
        0x0000, //  58   unassigned, L
        0x0000, //  59   unassigned, L
        0x0000, //  60   unassigned, L
        0x0000, //  61   unassigned, L
        0x0000, //  62   unassigned, L
        0x0000, //  63   unassigned, L
        0x0000, //  64   unassigned, L
        0x0032, //  65   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  66   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  67   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  68   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  69   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  70   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  71   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  72   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  73   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  74   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  75   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  76   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  77   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  78   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  79   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  80   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  81   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  82   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  83   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  84   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  85   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  86   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  87   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  88   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  89   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, //  90   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0000, //  91   unassigned, L
        0x0000, //  92   unassigned, L
        0x0000, //  93   unassigned, L
        0x0000, //  94   unassigned, L
        0x0020, //  95   unassigned, L, IDContinue
        0x0000, //  96   unassigned, L
        0x0031, //  97   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, //  98   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, //  99   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 100   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 101   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 102   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 103   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 104   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 105   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 106   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 107   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 108   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 109   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 110   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 111   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 112   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 113   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 114   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 115   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 116   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 117   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 118   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 119   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 120   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 121   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 122   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0000, // 123   unassigned, L
        0x0000, // 124   unassigned, L
        0x0000, // 125   unassigned, L
        0x0000, // 126   unassigned, L
        0x0000, // 127   unassigned, L
        0x0000, // 128   unassigned, L
        0x0000, // 129   unassigned, L
        0x0000, // 130   unassigned, L
        0x0000, // 131   unassigned, L
        0x0000, // 132   unassigned, L
        0x0000, // 133   unassigned, L
        0x0000, // 134   unassigned, L
        0x0000, // 135   unassigned, L
        0x0000, // 136   unassigned, L
        0x0000, // 137   unassigned, L
        0x0000, // 138   unassigned, L
        0x0000, // 139   unassigned, L
        0x0000, // 140   unassigned, L
        0x0000, // 141   unassigned, L
        0x0000, // 142   unassigned, L
        0x0000, // 143   unassigned, L
        0x0000, // 144   unassigned, L
        0x0000, // 145   unassigned, L
        0x0000, // 146   unassigned, L
        0x0000, // 147   unassigned, L
        0x0000, // 148   unassigned, L
        0x0000, // 149   unassigned, L
        0x0000, // 150   unassigned, L
        0x0000, // 151   unassigned, L
        0x0000, // 152   unassigned, L
        0x0000, // 153   unassigned, L
        0x0000, // 154   unassigned, L
        0x0000, // 155   unassigned, L
        0x0000, // 156   unassigned, L
        0x0000, // 157   unassigned, L
        0x0000, // 158   unassigned, L
        0x0000, // 159   unassigned, L
        0x0000, // 160   unassigned, L
        0x0000, // 161   unassigned, L
        0x0000, // 162   unassigned, L
        0x0000, // 163   unassigned, L
        0x0000, // 164   unassigned, L
        0x0000, // 165   unassigned, L
        0x0000, // 166   unassigned, L
        0x0000, // 167   unassigned, L
        0x0000, // 168   unassigned, L
        0x0840, // 169   unassigned, L, emoji, extendedPictographic
        0x0031, // 170   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0000, // 171   unassigned, L
        0x0000, // 172   unassigned, L
        0x0000, // 173   unassigned, L
        0x0840, // 174   unassigned, L, emoji, extendedPictographic
        0x0000, // 175   unassigned, L
        0x0000, // 176   unassigned, L
        0x0000, // 177   unassigned, L
        0x0000, // 178   unassigned, L
        0x0000, // 179   unassigned, L
        0x0000, // 180   unassigned, L
        0x0031, // 181   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0000, // 182   unassigned, L
        0x0020, // 183   unassigned, L, IDContinue
        0x0000, // 184   unassigned, L
        0x0000, // 185   unassigned, L
        0x0031, // 186   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0000, // 187   unassigned, L
        0x0000, // 188   unassigned, L
        0x0000, // 189   unassigned, L
        0x0000, // 190   unassigned, L
        0x0000, // 191   unassigned, L
        0x0032, // 192   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 193   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 194   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 195   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 196   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 197   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 198   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 199   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 200   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 201   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 202   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 203   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 204   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 205   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 206   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 207   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 208   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 209   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 210   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 211   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 212   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 213   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 214   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0000, // 215   unassigned, L
        0x0032, // 216   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 217   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 218   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 219   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 220   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 221   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0032, // 222   unassigned, L, otherUppercase, IDStart, IDContinue
        0x0031, // 223   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 224   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 225   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 226   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 227   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 228   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 229   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 230   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 231   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 232   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 233   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 234   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 235   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 236   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 237   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 238   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 239   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 240   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 241   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 242   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 243   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 244   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 245   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 246   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0000, // 247   unassigned, L
        0x0031, // 248   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 249   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 250   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 251   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 252   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 253   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031, // 254   unassigned, L, otherLowercase, IDStart, IDContinue
        0x0031 // 255   unassigned, L, otherLowercase, IDStart, IDContinue
    };

    gint CharacterDataLatin1::properties(gint ch) {
        gchar offset = (gchar) ch;
        int props = A[offset];
        return props;
    }

    gint CharacterDataLatin1::propertiesEx(gint ch) {
        gchar offset = (gchar) ch;
        int props = B[offset];
        return props;
    }

    gbool CharacterDataLatin1::isDigit(gint ch) {
        return '0' <= ch && ch <= '9';
    }

    gbool CharacterDataLatin1::isLowerCase(gint ch) {
        return (propertiesEx(ch) & 0x0001) != 0;
    }

    gbool CharacterDataLatin1::isUpperCase(gint ch) {
        return (propertiesEx(ch) & 0x0002) != 0;
    }

    gbool CharacterDataLatin1::isOtherAlphabetic(gint ch) {
        return (propertiesEx(ch) & 0x004) != 0;
    }

    gbool CharacterDataLatin1::isIdeographic(gint ch) {
        return (propertiesEx(ch) & 0x008) != 0;
    }

    gint CharacterDataLatin1::type(gint ch) {
        return (properties(ch) & 0x1F) != 0;
    }

    gint CharacterDataLatin1::toLowerCase(gint ch) {
        int mapChar = ch;
        int val = properties(ch);

        if (((val & 0x00020000) != 0) &&
            ((val & 0x07FC0000) != 0x07FC0000)) {
            int offset = val << 5 >> (5 + 18);
            mapChar = ch + offset;
        }
        return mapChar;
    }

    gint CharacterDataLatin1::toUpperCase(gint ch) {
        int mapChar = ch;
        int val = properties(ch);

        if ((val & 0x00010000) != 0) {
            if ((val & 0x07FC0000) != 0x07FC0000) {
                int offset = val << 5 >> (5 + 18);
                mapChar = ch - offset;
            } else if (ch == 0x00B5) {
                mapChar = 0x039C;
            }
        }
        return mapChar;
    }

    gint CharacterDataLatin1::toTitleCase(gint ch) {
        return toUpperCase(ch);
    }

    gint CharacterDataLatin1::digit(gint ch, gint radix) {
        gint value = DIGITS[ch];
        return (value >= 0 && value < radix && radix >= Character::MIN_RADIX
                && radix <= Character::MAX_RADIX)
                   ? value
                   : -1;
    }

    gint CharacterDataLatin1::numericValue(gint ch) {
        int val = properties(ch);
        int retval = -1;

        switch (val & 0xC00) {
            default: // cannot occur
            case (0x00000000): // not numeric
                retval = -1;
                break;
            case (0x00000400): // simple numeric
                retval = ch + ((val & 0x3E0) >> 5) & 0x1F;
                break;
            case (0x00000800): // "strange" numeric
                retval = -2;
                break;
            case (0x00000C00): // Java supradecimal
                retval = (ch + ((val & 0x3E0) >> 5) & 0x1F) + 10;
                break;
        }
        return retval;
    }

    gbool CharacterDataLatin1::isWhitespace(gint ch) {
        gint props = properties(ch);
        return ((props & 0x00007000) == 0x00004000);
    }

    gbyte CharacterDataLatin1::directionality(gint ch) {
        int val = properties(ch);
        gbyte directionality = (gbyte) ((val & 0x78000000) >> 27);

        if (directionality == 0xF) {
            directionality = -1;
        }
        return directionality;
    }

    gbool CharacterDataLatin1::isMirrored(gint ch) {
        int props = properties(ch);
        return ((props & 0x80000000) != 0);
    }

    gbool CharacterDataLatin1::isEmoji(gint ch) {
        return (propertiesEx(ch) & 0x0040) != 0;
    }

    gbool CharacterDataLatin1::isEmojiPresentation(gint ch) {
        return (propertiesEx(ch) & 0x0080) != 0;
    }

    gbool CharacterDataLatin1::isEmojiModifier(gint ch) {
        return (propertiesEx(ch) & 0x0100) != 0;
    }

    gbool CharacterDataLatin1::isEmojiModifierBase(gint ch) {
        return (propertiesEx(ch) & 0x0200) != 0;
    }

    gbool CharacterDataLatin1::isEmojiComponent(gint ch) {
        return (propertiesEx(ch) & 0x0400) != 0;
    }

    gbool CharacterDataLatin1::isExtendedPictographic(gint ch) {
        return (propertiesEx(ch) & 0x0800) != 0;
    }

    gint CharacterDataLatin1::toUpperCaseEx(gint ch) {
        int mapChar = ch;
        int val = properties(ch);

        if ((val & 0x00010000) != 0) {
            if ((val & 0x07FC0000) != 0x07FC0000) {
                int offset = val << 5 >> (5 + 18);
                mapChar = ch - offset;
            } else {
                mapChar = ch == 0x00B5 ? 0x039C : -1;
            }
        }
        return mapChar;
    }

    static CORE_FAST gchar sharpsMap[] = {'S', 'S'};

    CharArray CharacterDataLatin1::toUpperCaseCharArray(gint ch) {
        if (ch == 0x00DF) {
            return CharArray::copyOf(sharpsMap);
        }
        return CharArray::of(ch);
    }

    CharacterDataLatin1::CharacterDataLatin1() {
        CORE_IGNORE(this);
    }

    CharacterDataLatin1 CharacterDataLatin1::instance{};
} // core
