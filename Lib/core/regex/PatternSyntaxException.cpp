//
// Created by bruns on 13/10/2024.
//

#include "PatternSyntaxException.h"

#include <core/lang/XString.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace regex {
        PatternSyntaxException::PatternSyntaxException(const String& desc, const String& regex, gint index)
            : desc(desc), regex(regex), index(index) {}

        gint PatternSyntaxException::getIndex() const {
            return index;
        }

        String PatternSyntaxException::description() const {
            return desc;
        }

        String PatternSyntaxException::pattern() const {
            return regex;
        }

        String PatternSyntaxException::message() const {
            XString result;
            result.append(desc);
            if (index >= 0) {
                result.append(" near index "_S);
                result.append(index);
            }
            result.append('\n');
            result.append(regex);
            if (index >= 0 && index < regex.length()) {
                result.append('\n');
                for (int i = 0; i < index; i++) {
                    result.append((regex.charAt(i) == '\t') ? '\t' : ' ');
                }
                result.append('^');
            }
            return result.toString();
        }

        Object& PatternSyntaxException::clone() const {
            try {
                return UNSAFE::newInstance<PatternSyntaxException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void PatternSyntaxException::selfThrow() const {
            throw PatternSyntaxException(*this);
        }
    } // regex
} // core
