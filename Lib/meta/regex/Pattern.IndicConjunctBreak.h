//
// Created by bruns on 16/10/2024.
//

#ifndef INDICCONJUNCTBREAK_H
#define INDICCONJUNCTBREAK_H

#include <core/regex/Pattern.h>

namespace core {
    namespace regex {
        /**
         * Helper class for supporting the GB9c rule in Unicode Text Segmentation TR29
         *
         * <p>
         * GB9c Do not break within certain combinations with Indic_Conjunct_Break (InCB)=Linker.
         *
         * \p{InCB=Consonant} [ \p{InCB=Extend} \p{InCB=Linker} ]* \p{InCB=Linker} [ \p{InCB=Extend} \p{InCB=Linker} ]* x \p{InCB=Consonant}*
         * </p>
         *
         * Code point conditions included in this class are derived from the "Derived Property: Indic_Conjunct_Break"
         * section in DerivedCoreProperties.txt of the Unicode Character Database.
         */
        class Pattern::IndicConjunctBreak final : public Object {
        public:
            static gbool isLinker(gint cp);

            static gbool isExtend(gint cp);

            static gbool isConsonant(gint cp);
        };
    } // regex
} // core

#endif //INDICCONJUNCTBREAK_H
