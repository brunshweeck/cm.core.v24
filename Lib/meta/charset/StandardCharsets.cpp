//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/util/HashMap.h>
#include <meta/charset/CESU_8.h>
#include <meta/charset/GB18030.h>
#include <meta/charset/GBK.h>
#include <meta/charset/IBM437.h>
#include <meta/charset/IBM737.h>
#include <meta/charset/IBM775.h>
#include <meta/charset/IBM850.h>
#include <meta/charset/IBM852.h>
#include <meta/charset/IBM855.h>
#include <meta/charset/IBM857.h>
#include <meta/charset/IBM858.h>
#include <meta/charset/IBM862.h>
#include <meta/charset/IBM866.h>
#include <meta/charset/IBM874.h>
#include <meta/charset/ISO_8859_13.h>
#include <meta/charset/ISO_8859_15.h>
#include <meta/charset/ISO_8859_16.h>
#include <meta/charset/ISO_8859_2.h>
#include <meta/charset/ISO_8859_4.h>
#include <meta/charset/ISO_8859_5.h>
#include <meta/charset/ISO_8859_7.h>
#include <meta/charset/ISO_8859_9.h>
#include <meta/charset/JIS_X_0201.h>
#include <meta/charset/Johab.h>
#include <meta/charset/KOI8_R.h>
#include <meta/charset/KOI8_U.h>
#include <meta/charset/MS1250.h>
#include <meta/charset/MS1251.h>
#include <meta/charset/MS1252.h>
#include <meta/charset/MS1253.h>
#include <meta/charset/MS1254.h>
#include <meta/charset/MS1255.h>
#include <meta/charset/MS1256.h>
#include <meta/charset/MS1257.h>
#include <meta/charset/MS1258.h>
#include <meta/charset/MS874.h>
#include <meta/charset/MS932.h>
#include <meta/charset/MS936.h>
#include <meta/charset/MS949.h>
#include <meta/charset/MS950_HKSCS.h>
#include <meta/charset/MS950_HKSCS_XP.h>
#include <meta/charset/SJIS.h>
#include <meta/charset/StandardCharsets.h>
#include <meta/charset/UTF_16LE_BOM.h>
#include <meta/charset/UTF_32BE_BOM.h>
#include <meta/charset/UTF_32LE_BOM.h>

namespace core {
    namespace charset {
        util::HashMap<String, String> StandardCharsets::aliases = util::HashMap<String, String>();
        util::HashMap<String, Charset> StandardCharsets::cache = util::HashMap<String, Charset>();
        gint StandardCharsets::level = 0;

        void StandardCharsets::initialize() {
            initializeAliases();
            initializeCache();
        }

        void StandardCharsets::initializeAliases() {
            aliases.put("csisolatin0"_S, "iso-8859-15"_S);
            aliases.put("csisolatin1"_S, "iso-8859-1"_S);
            aliases.put("csisolatin2"_S, "iso-8859-2"_S);
            aliases.put("csisolatin4"_S, "iso-8859-4"_S);
            aliases.put("csisolatin5"_S, "iso-8859-9"_S);
            aliases.put("csisolatin9"_S, "iso-8859-15"_S);
            aliases.put("unicodelittle"_S, "x-utf-16le-bom"_S);
            aliases.put("iso646-us"_S, "us-ascii"_S);
            aliases.put("iso_8859-7:1987"_S, "iso-8859-7"_S);
            aliases.put("912"_S, "iso-8859-2"_S);
            aliases.put("914"_S, "iso-8859-4"_S);
            aliases.put("915"_S, "iso-8859-5"_S);
            aliases.put("latin10"_S, "iso-8859-16"_S);
            aliases.put("ksc5601-1992"_S, "x-johab"_S);
            aliases.put("ibm-1252"_S, "windows-1252"_S);
            aliases.put("920"_S, "iso-8859-9"_S);
            aliases.put("923"_S, "iso-8859-15"_S);
            aliases.put("csisolatincyrillic"_S, "iso-8859-5"_S);
            aliases.put("8859_1"_S, "iso-8859-1"_S);
            aliases.put("8859_2"_S, "iso-8859-2"_S);
            aliases.put("8859_4"_S, "iso-8859-4"_S);
            aliases.put("813"_S, "iso-8859-7"_S);
            aliases.put("8859_5"_S, "iso-8859-5"_S);
            aliases.put("8859_7"_S, "iso-8859-7"_S);
            aliases.put("8859_9"_S, "iso-8859-9"_S);
            aliases.put("iso_8859-1:1987"_S, "iso-8859-1"_S);
            aliases.put("819"_S, "iso-8859-1"_S);
            aliases.put("unicode-1-1-utf-8"_S, "utf-8"_S);
            aliases.put("ms-874"_S, "x-windows-874"_S);
            aliases.put("x-utf-16le"_S, "utf-16le"_S);
            aliases.put("ecma-118"_S, "iso-8859-7"_S);
            aliases.put("koi8_r"_S, "koi8-r"_S);
            aliases.put("koi8_u"_S, "koi8-u"_S);
            aliases.put("cp912"_S, "iso-8859-2"_S);
            aliases.put("cp914"_S, "iso-8859-4"_S);
            aliases.put("cp915"_S, "iso-8859-5"_S);
            aliases.put("jis0201"_S, "jis_x0201"_S);
            aliases.put("cp920"_S, "iso-8859-9"_S);
            aliases.put("cp923"_S, "iso-8859-15"_S);
            aliases.put("utf_32le_bom"_S, "x-utf-32le-bom"_S);
            aliases.put("utf_16be"_S, "utf-16be"_S);
            aliases.put("cspc8codepage437"_S, "ibm437"_S);
            aliases.put("ansi-1251"_S, "windows-1251"_S);
            aliases.put("cp813"_S, "iso-8859-7"_S);
            aliases.put("cp936"_S, "gbk"_S);
            aliases.put("850"_S, "ibm850"_S);
            aliases.put("cp819"_S, "iso-8859-1"_S);
            aliases.put("852"_S, "ibm852"_S);
            aliases.put("855"_S, "ibm855"_S);
            aliases.put("cswindows31j"_S, "windows-31j"_S);
            aliases.put("857"_S, "ibm857"_S);
            aliases.put("iso-ir-6"_S, "us-ascii"_S);
            aliases.put("858"_S, "ibm00858"_S);
            aliases.put("737"_S, "x-ibm737"_S);
            aliases.put("csascii"_S, "us-ascii"_S);
            aliases.put("ms932"_S, "windows-31j"_S);
            aliases.put("ibm1252"_S, "windows-1252"_S);
            aliases.put("862"_S, "ibm862"_S);
            aliases.put("ms936"_S, "x-mswin-936"_S);
            aliases.put("866"_S, "ibm866"_S);
            aliases.put("x-utf-32be"_S, "utf-32be"_S);
            aliases.put("iso_8859-2:1987"_S, "iso-8859-2"_S);
            aliases.put("unicodebig"_S, "utf-16"_S);
            aliases.put("iso8859_15_fdis"_S, "iso-8859-15"_S);
            aliases.put("874"_S, "x-ibm874"_S);
            aliases.put("ms949"_S, "x-windows-949"_S);
            aliases.put("unicodelittleunmarked"_S, "utf-16le"_S);
            aliases.put("iso8859_1"_S, "iso-8859-1"_S);
            aliases.put("iso8859_2"_S, "iso-8859-2"_S);
            aliases.put("csiso885915"_S, "iso-8859-15"_S);
            aliases.put("csiso885916"_S, "iso-8859-16"_S);
            aliases.put("iso8859_4"_S, "iso-8859-4"_S);
            aliases.put("iso8859_5"_S, "iso-8859-5"_S);
            aliases.put("iso8859_7"_S, "iso-8859-7"_S);
            aliases.put("iso8859_9"_S, "iso-8859-9"_S);
            aliases.put("ibm912"_S, "iso-8859-2"_S);
            aliases.put("ibm914"_S, "iso-8859-4"_S);
            aliases.put("ibm915"_S, "iso-8859-5"_S);
            aliases.put("ms950"_S, "x-windows-950"_S);
            aliases.put("iso_8859-13"_S, "iso-8859-13"_S);
            aliases.put("iso_8859-15"_S, "iso-8859-15"_S);
            aliases.put("iso_8859-16"_S, "iso-8859-16"_S);
            aliases.put("greek8"_S, "iso-8859-7"_S);
            aliases.put("646"_S, "us-ascii"_S);
            aliases.put("ms_kanji"_S, "shift_jis"_S);
            aliases.put("ibm-912"_S, "iso-8859-2"_S);
            aliases.put("ibm920"_S, "iso-8859-9"_S);
            aliases.put("ibm-914"_S, "iso-8859-4"_S);
            aliases.put("ibm-915"_S, "iso-8859-5"_S);
            aliases.put("l1"_S, "iso-8859-1"_S);
            aliases.put("cp850"_S, "ibm850"_S);
            aliases.put("ibm923"_S, "iso-8859-15"_S);
            aliases.put("l2"_S, "iso-8859-2"_S);
            aliases.put("cyrillic"_S, "iso-8859-5"_S);
            aliases.put("cp852"_S, "ibm852"_S);
            aliases.put("l4"_S, "iso-8859-4"_S);
            aliases.put("l5"_S, "iso-8859-9"_S);
            aliases.put("cp855"_S, "ibm855"_S);
            aliases.put("cp857"_S, "ibm857"_S);
            aliases.put("l9"_S, "iso-8859-15"_S);
            aliases.put("cp858"_S, "ibm00858"_S);
            aliases.put("cp737"_S, "x-ibm737"_S);
            aliases.put("iso_8859_1"_S, "iso-8859-1"_S);
            aliases.put("koi8"_S, "koi8-r"_S);
            aliases.put("775"_S, "ibm775"_S);
            aliases.put("iso_8859-9:1989"_S, "iso-8859-9"_S);
            aliases.put("ibm-920"_S, "iso-8859-9"_S);
            aliases.put("ibm-923"_S, "iso-8859-15"_S);
            aliases.put("ibm813"_S, "iso-8859-7"_S);
            aliases.put("cp862"_S, "ibm862"_S);
            aliases.put("cp866"_S, "ibm866"_S);
            aliases.put("ibm819"_S, "iso-8859-1"_S);
            aliases.put("ansi_x3.4-1968"_S, "us-ascii"_S);
            aliases.put("ibm-813"_S, "iso-8859-7"_S);
            aliases.put("ibm-819"_S, "iso-8859-1"_S);
            aliases.put("cp874"_S, "x-ibm874"_S);
            aliases.put("iso-ir-100"_S, "iso-8859-1"_S);
            aliases.put("iso-ir-101"_S, "iso-8859-2"_S);
            aliases.put("437"_S, "ibm437"_S);
            aliases.put("iso-ir-226"_S, "iso-8859-16"_S);
            aliases.put("iso-8859-15"_S, "iso-8859-15"_S);
            aliases.put("latin0"_S, "iso-8859-15"_S);
            aliases.put("latin1"_S, "iso-8859-1"_S);
            aliases.put("latin2"_S, "iso-8859-2"_S);
            aliases.put("ms874"_S, "x-windows-874"_S);
            aliases.put("latin4"_S, "iso-8859-4"_S);
            aliases.put("latin5"_S, "iso-8859-9"_S);
            aliases.put("iso-ir-110"_S, "iso-8859-4"_S);
            aliases.put("latin9"_S, "iso-8859-15"_S);
            aliases.put("ansi_x3.4-1986"_S, "us-ascii"_S);
            aliases.put("utf-32be-bom"_S, "x-utf-32be-bom"_S);
            aliases.put("sjis"_S, "shift_jis"_S);
            aliases.put("cp775"_S, "ibm775"_S);
            aliases.put("shift_jis"_S, "shift_jis"_S);
            aliases.put("iso-ir-126"_S, "iso-8859-7"_S);
            aliases.put("ibm850"_S, "ibm850"_S);
            aliases.put("ibm852"_S, "ibm852"_S);
            aliases.put("ibm855"_S, "ibm855"_S);
            aliases.put("ibm857"_S, "ibm857"_S);
            aliases.put("ibm858"_S, "ibm00858"_S);
            aliases.put("ibm737"_S, "x-ibm737"_S);
            aliases.put("ms950_hkscs"_S, "x-ms950-hkscs"_S);
            aliases.put("x-sjis"_S, "shift_jis"_S);
            aliases.put("utf_16le"_S, "utf-16le"_S);
            aliases.put("ibm-850"_S, "ibm850"_S);
            aliases.put("ibm-852"_S, "ibm852"_S);
            aliases.put("ibm-855"_S, "ibm855"_S);
            aliases.put("ibm862"_S, "ibm862"_S);
            aliases.put("ibm-857"_S, "ibm857"_S);
            aliases.put("ibm-858"_S, "ibm00858"_S);
            aliases.put("ibm-737"_S, "x-ibm737"_S);
            aliases.put("ibm866"_S, "ibm866"_S);
            aliases.put("unicodebigunmarked"_S, "utf-16be"_S);
            aliases.put("cp437"_S, "ibm437"_S);
            aliases.put("utf16"_S, "utf-16"_S);
            aliases.put("windows-932"_S, "windows-31j"_S);
            aliases.put("windows-936"_S, "gbk"_S);
            aliases.put("iso-ir-144"_S, "iso-8859-5"_S);
            aliases.put("iso-ir-148"_S, "iso-8859-9"_S);
            aliases.put("ibm-862"_S, "ibm862"_S);
            aliases.put("ibm-866"_S, "ibm866"_S);
            aliases.put("ibm874"_S, "x-ibm874"_S);
            aliases.put("x-utf-32le"_S, "utf-32le"_S);
            aliases.put("windows-949"_S, "x-windows-949"_S);
            aliases.put("ibm-874"_S, "x-ibm874"_S);
            aliases.put("iso_8859-4:1988"_S, "iso-8859-4"_S);
            aliases.put("ms_936"_S, "x-mswin-936"_S);
            aliases.put("utf32"_S, "utf-32"_S);
            aliases.put("pc-multilingual-850+euro"_S, "ibm00858"_S);
            aliases.put("windows-950"_S, "x-windows-950"_S);
            aliases.put("elot_928"_S, "iso-8859-7"_S);
            aliases.put("cshalfwidthkatakana"_S, "jis_x0201"_S);
            aliases.put("csisolatingreek"_S, "iso-8859-7"_S);
            aliases.put("csibm857"_S, "ibm857"_S);
            aliases.put("ibm775"_S, "ibm775"_S);
            aliases.put("ms_949"_S, "x-windows-949"_S);
            aliases.put("cp1250"_S, "windows-1250"_S);
            aliases.put("cp1251"_S, "windows-1251"_S);
            aliases.put("cp1252"_S, "windows-1252"_S);
            aliases.put("cp1253"_S, "windows-1253"_S);
            aliases.put("cp1254"_S, "windows-1254"_S);
            aliases.put("cp1255"_S, "windows-1255"_S);
            aliases.put("cp1256"_S, "windows-1256"_S);
            aliases.put("csibm862"_S, "ibm862"_S);
            aliases.put("cp1257"_S, "windows-1257"_S);
            aliases.put("cp1258"_S, "windows-1258"_S);
            aliases.put("csibm866"_S, "ibm866"_S);
            aliases.put("cesu8"_S, "cesu-8"_S);
            aliases.put("iso8859_13"_S, "iso-8859-13"_S);
            aliases.put("iso8859_15"_S, "iso-8859-15"_S);
            aliases.put("utf_32be"_S, "utf-32be"_S);
            aliases.put("iso8859_16"_S, "iso-8859-16"_S);
            aliases.put("utf_32be_bom"_S, "x-utf-32be-bom"_S);
            aliases.put("ibm-775"_S, "ibm775"_S);
            aliases.put("cp00858"_S, "ibm00858"_S);
            aliases.put("8859_13"_S, "iso-8859-13"_S);
            aliases.put("us"_S, "us-ascii"_S);
            aliases.put("8859_15"_S, "iso-8859-15"_S);
            aliases.put("ibm437"_S, "ibm437"_S);
            aliases.put("cp367"_S, "us-ascii"_S);
            aliases.put("iso-10646-ucs-2"_S, "utf-16be"_S);
            aliases.put("ibm-437"_S, "ibm437"_S);
            aliases.put("iso8859-13"_S, "iso-8859-13"_S);
            aliases.put("iso8859-15"_S, "iso-8859-15"_S);
            aliases.put("windows-874"_S, "x-windows-874"_S);
            aliases.put("iso_8859-5:1988"_S, "iso-8859-5"_S);
            aliases.put("unicode"_S, "utf-16"_S);
            aliases.put("greek"_S, "iso-8859-7"_S);
            aliases.put("ms1361"_S, "x-johab"_S);
            aliases.put("ascii7"_S, "us-ascii"_S);
            aliases.put("iso8859-1"_S, "iso-8859-1"_S);
            aliases.put("iso8859-2"_S, "iso-8859-2"_S);
            aliases.put("cskoi8r"_S, "koi8-r"_S);
            aliases.put("jis_x0201"_S, "jis_x0201"_S);
            aliases.put("iso8859-4"_S, "iso-8859-4"_S);
            aliases.put("iso8859-5"_S, "iso-8859-5"_S);
            aliases.put("iso8859-7"_S, "iso-8859-7"_S);
            aliases.put("iso8859-9"_S, "iso-8859-9"_S);
            aliases.put("windows949"_S, "x-windows-949"_S);
            aliases.put("johab"_S, "x-johab"_S);
            aliases.put("ccsid00858"_S, "ibm00858"_S);
            aliases.put("cspc862latinhebrew"_S, "ibm862"_S);
            aliases.put("ibm367"_S, "us-ascii"_S);
            aliases.put("iso_8859-1"_S, "iso-8859-1"_S);
            aliases.put("iso_8859-2"_S, "iso-8859-2"_S);
            aliases.put("x-utf-16be"_S, "utf-16be"_S);
            aliases.put("sun_eu_greek"_S, "iso-8859-7"_S);
            aliases.put("iso_8859-16:2001"_S, "iso-8859-16"_S);
            aliases.put("iso_8859-4"_S, "iso-8859-4"_S);
            aliases.put("iso_8859-5"_S, "iso-8859-5"_S);
            aliases.put("ms950_hkscs_xp"_S, "x-ms950-hkscs-xp"_S);
            aliases.put("cspcp852"_S, "ibm852"_S);
            aliases.put("iso_8859-7"_S, "iso-8859-7"_S);
            aliases.put("shift-jis"_S, "shift_jis"_S);
            aliases.put("iso_8859-9"_S, "iso-8859-9"_S);
            aliases.put("cspcp855"_S, "ibm855"_S);
            aliases.put("windows-437"_S, "ibm437"_S);
            aliases.put("ascii"_S, "us-ascii"_S);
            aliases.put("cscesu-8"_S, "cesu-8"_S);
            aliases.put("ksc5601_1992"_S, "x-johab"_S);
            aliases.put("utf8"_S, "utf-8"_S);
            aliases.put("iso_646.irv:1983"_S, "us-ascii"_S);
            aliases.put("l10"_S, "iso-8859-16"_S);
            aliases.put("cp5346"_S, "windows-1250"_S);
            aliases.put("cp5347"_S, "windows-1251"_S);
            aliases.put("cp5348"_S, "windows-1252"_S);
            aliases.put("cp5349"_S, "windows-1253"_S);
            aliases.put("iso_646.irv:1991"_S, "us-ascii"_S);
            aliases.put("cp5350"_S, "windows-1254"_S);
            aliases.put("cp5353"_S, "windows-1257"_S);
            aliases.put("latin-9"_S, "iso-8859-15"_S);
            aliases.put("utf_32le"_S, "utf-32le"_S);
            aliases.put("utf_16"_S, "utf-16"_S);
            aliases.put("csshiftjis"_S, "shift_jis"_S);
            aliases.put("cspc850multilingual"_S, "ibm850"_S);
            aliases.put("utf-32le-bom"_S, "x-utf-32le-bom"_S);
            aliases.put("utf_32"_S, "utf-32"_S);
            aliases.put("x0201"_S, "jis_x0201"_S);

            level = 12345;
        }

        void StandardCharsets::initializeCache() {
            cache.put("ibm862"_S, UNSAFE::newInstance<IBM862>());
            cache.put("windows-31j"_S, UNSAFE::newInstance<MS932>());
            cache.put("ibm866"_S, UNSAFE::newInstance<IBM866>());
            cache.put("utf-16le"_S, Charset::UTF_16LE);
            cache.put("utf-32"_S, Charset::UTF_32);
            cache.put("windows-1250"_S, UNSAFE::newInstance<MS1250>());
            cache.put("windows-1251"_S, UNSAFE::newInstance<MS1251>());
            cache.put("windows-1252"_S, UNSAFE::newInstance<MS1252>());
            cache.put("utf-32be"_S, Charset::UTF_32BE);
            cache.put("windows-1253"_S, UNSAFE::newInstance<MS1253>());
            cache.put("windows-1254"_S, UNSAFE::newInstance<MS1254>());
            cache.put("windows-1255"_S, UNSAFE::newInstance<MS1255>());
            cache.put("gb18030"_S, UNSAFE::newInstance<GB18030>());
            cache.put("utf-16"_S, Charset::UTF_16);
            cache.put("windows-1256"_S, UNSAFE::newInstance<MS1256>());
            cache.put("x-ms950-hkscs"_S, UNSAFE::newInstance<MS950_HKSCS>());
            cache.put("windows-1257"_S, UNSAFE::newInstance<MS1257>());
            cache.put("windows-1258"_S, UNSAFE::newInstance<MS1258>());
            cache.put("utf-16be"_S, Charset::UTF_16BE);
            cache.put("x-johab"_S, UNSAFE::newInstance<Johab>());
            cache.put("jis_x0201"_S, UNSAFE::newInstance<JIS_X_0201>());
            cache.put("iso-8859-1"_S, Charset::ISO_8859_1);
            cache.put("iso-8859-2"_S, UNSAFE::newInstance<ISO_8859_2>());
            cache.put("utf-8"_S, Charset::UTF_8);
            cache.put("iso-8859-4"_S, UNSAFE::newInstance<ISO_8859_4>());
            cache.put("iso-8859-5"_S, UNSAFE::newInstance<ISO_8859_5>());
            cache.put("x-ibm874"_S, UNSAFE::newInstance<IBM874>());
            cache.put("iso-8859-7"_S, UNSAFE::newInstance<ISO_8859_7>());
            cache.put("shift_jis"_S, UNSAFE::newInstance<SJIS>());
            cache.put("iso-8859-9"_S, UNSAFE::newInstance<ISO_8859_9>());
            cache.put("x-ms950-hkscs-xp"_S, UNSAFE::newInstance<MS950_HKSCS_XP>());
            cache.put("x-ibm737"_S, UNSAFE::newInstance<IBM737>());
            cache.put("x-windows-949"_S, UNSAFE::newInstance<MS949>());
            cache.put("ibm850"_S, UNSAFE::newInstance<IBM850>());
            cache.put("ibm852"_S, UNSAFE::newInstance<IBM852>());
            cache.put("ibm775"_S, UNSAFE::newInstance<IBM775>());
            cache.put("us-ascii"_S, Charset::US_ASCII);
            cache.put("iso-8859-13"_S, UNSAFE::newInstance<ISO_8859_13>());
            cache.put("ibm855"_S, UNSAFE::newInstance<IBM855>());
            cache.put("ibm437"_S, UNSAFE::newInstance<IBM437>());
            cache.put("iso-8859-15"_S, UNSAFE::newInstance<ISO_8859_15>());
            cache.put("ibm857"_S, UNSAFE::newInstance<IBM857>());
            cache.put("iso-8859-16"_S, UNSAFE::newInstance<ISO_8859_16>());
            cache.put("x-utf-32le-bom"_S, UNSAFE::newInstance<UTF_32LE_BOM>());
            cache.put("ibm00858"_S, UNSAFE::newInstance<IBM858>());
            cache.put("x-utf-16le-bom"_S, UNSAFE::newInstance<UTF_16LE_BOM>());
            cache.put("cesu-8"_S, UNSAFE::newInstance<CESU_8>());
            cache.put("x-mswin-936"_S, UNSAFE::newInstance<MS936>());
            cache.put("gbk"_S, UNSAFE::newInstance<GBK>());
            cache.put("x-utf-32be-bom"_S, UNSAFE::newInstance<UTF_32BE_BOM>());
            cache.put("x-windows-950"_S, UNSAFE::newInstance<MS950>());
            cache.put("x-windows-874"_S, UNSAFE::newInstance<MS874>());
            cache.put("koi8-r"_S, UNSAFE::newInstance<KOI8_R>());
            cache.put("koi8-u"_S, UNSAFE::newInstance<KOI8_U>());
            cache.put("utf-32le"_S, Charset::UTF_32LE);

            level = 1234567890;
        }

        String StandardCharsets::canonicalize(const String& csn) {
            if (csn.startsWith("gb18030-"_S)) {
                return csn.equals("gb18030-2022"_S) && !GB18030::IS_2000 ||
                       csn.equals("gb18030-2020"_S) && GB18030::IS_2000
                           ? "gb18030"_S
                           : csn;
            }

            Object& acn = aliases.getOrNull(csn);
            return acn != null ? CORE_XCAST(String, acn) : csn;
        }

        String StandardCharsets::toLower(String const& s) {
            int n = s.length();
            gbool allLower = true;
            for (int i = 0; i < n; i++) {
                int c = s.charAt(i);
                if (((c - 'A') | ('Z' - c)) >= 0) {
                    allLower = false;
                    break;
                }
            }
            if (allLower)
                return s;

            XString xs = XString(s.length());
            for (int i = 0; i < n; i++) {
                int c = s.charAt(i);
                if (((c - 'A') | ('Z' - c)) >= 0)
                    xs.append((gchar)(c + 0x20));
                else
                    xs.append((gchar)c);
            }
            return xs.toString();
        }

        Object& StandardCharsets::lookup(const String& csn) {
            CORE_ASSERT(level == 0 || level == 12345 || level == 1234567890);
            if (level == 0)
                initialize();
            else if (level == 12345)
                initializeCache();

            // By checking these built-ins we can avoid initializing Aliases,
            // and Cache eagerly during bootstrap.
            //
            // Initialization of StandardCharsets should be
            // avoided here to minimize time spent in System, as it
            // may delay initialization of performance critical subsystems.
            if (csn.equals("UTF-8"_S))
                return Charset::UTF_8;
            if (csn.equals("US_ASCII"_S))
                return Charset::US_ASCII;
            if (csn.equals("ISO-8859-1"_S))
                return Charset::ISO_8859_1;

            String cn = canonicalize(toLower(csn));
            return cache.getOrNull(cn);
        }

        util::Iterator<Charset>& StandardCharsets::charsets() {
            static util::Set<String>& charsetNames = cache.keySet();

            class CharsetIterator : public util::Iterator<Charset> {
                Iterator<String>& it = charsetNames.iterator();

            public:
                gbool hasNext() const override {
                    return it.hasNext();
                }

                Charset& next() override {
                    String csn = it.next();
                    Object& cs = lookup(csn);
                    if (cs == null)
                        return next();
                    return CORE_XCAST(Charset, cs);
                }

                gbool equals(const Object& o) const override {
                    return this == &o ||
                        Class<CharsetIterator>::hasInstance(o) && it == CORE_XCAST(CharsetIterator const, o);
                }

                ~CharsetIterator() override {
                    UNSAFE::deleteInstance(it);
                }
            };

            return UNSAFE::newInstance<CharsetIterator>();
        }
    } // charset
} // core
