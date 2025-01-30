//
// Created by brunshweeck on 6 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/Unicode.h>

namespace core {
    namespace charset {
        Unicode::Unicode(String canonicalName, StringArray aliases)
            : Charset(UNSAFE::moveInstance(canonicalName), UNSAFE::moveInstance(aliases)) {
        }

        gbool Unicode::contains(const Charset &cs) const {
            String name = cs.name();
            return (name.equals("US-ASCII"_S))
                   || (name.equals("UTF-8"_S))
                   || (name.equals("UTF-16"_S))
                   || (name.equals("UTF-16BE"_S))
                   || (name.equals("UTF-16LE"_S))
                   || (name.equals("UTF-16LE_BOM"_S))
                   || (name.equals("CESU-8"_S))
                   || (name.equals("UTF-32"_S))
                   || (name.equals("UTF-32BE"_S))
                   || (name.equals("UTF-32BE_BOM"_S))
                   || (name.equals("UTF-32LE"_S))
                   || (name.equals("UTF-32LE_BOM"_S))
                   || (name.equals("GBK"_S))
                   || (name.equals("GB18030"_S))
                   || (name.equals("ISO-8859-1"_S))
                   || (name.equals("ISO-8859-2"_S))
                   || (name.equals("ISO-8859-3"_S))
                   || (name.equals("ISO-8859-4"_S))
                   || (name.equals("ISO-8859-5"_S))
                   || (name.equals("ISO-8859-6"_S))
                   || (name.equals("ISO-8859-7"_S))
                   || (name.equals("ISO-8859-8"_S))
                   || (name.equals("ISO-8859-9"_S))
                   || (name.equals("ISO-8859-13"_S))
                   || (name.equals("ISO-8859-15"_S))
                   || (name.equals("ISO-8859-16"_S))
                   || (name.equals("JIS_X0201"_S))
                   || (name.equals("x-JIS0208"_S))
                   || (name.equals("JIS_X0212-1990"_S))
                   || (name.equals("GB2312"_S))
                   || (name.equals("EUC-KR"_S))
                   || (name.equals("x-EUC-TW"_S))
                   || (name.equals("EUC-JP"_S))
                   || (name.equals("x-euc-jp-linux"_S))
                   || (name.equals("KOI8-R"_S))
                   || (name.equals("TIS-620"_S))
                   || (name.equals("x-ISCII91"_S))
                   || (name.equals("windows-1251"_S))
                   || (name.equals("windows-1252"_S))
                   || (name.equals("windows-1253"_S))
                   || (name.equals("windows-1254"_S))
                   || (name.equals("windows-1255"_S))
                   || (name.equals("windows-1256"_S))
                   || (name.equals("windows-1257"_S))
                   || (name.equals("windows-1258"_S))
                   || (name.equals("windows-932"_S))
                   || (name.equals("x-mswin-936"_S))
                   || (name.equals("x-windows-949"_S))
                   || (name.equals("x-windows-950"_S))
                   || (name.equals("windows-31j"_S))
                   || (name.equals("Big5"_S))
                   || (name.equals("Big5-HKSCS"_S))
                   || (name.equals("x-MS950-HKSCS"_S))
                   || (name.equals("ISO-2022-JP"_S))
                   || (name.equals("ISO-2022-KR"_S))
                   || (name.equals("x-ISO-2022-CN-CNS"_S))
                   || (name.equals("x-ISO-2022-CN-GB"_S))
                   || (name.equals("x-Johab"_S))
                   || (name.equals("Shift_JIS"_S));
        }
    } // charset
} // core
