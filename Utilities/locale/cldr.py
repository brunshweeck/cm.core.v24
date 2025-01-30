import os
from pathlib import Path
from xml.dom import minidom
from xml.dom.minidom import Element

from External.locale.tzdb import TzRulesData as regionToRules  # (TimeZone Rules)

IGNORE_PY_DEPENDENCIES = False

print(os.name)

CLDR_PATH = "/home/admin/cldr-main"  # The path of cldr
MAX_COLS_PER_LINE = 8


class TzRule:
    From = ''
    To = ''
    Zone = ''

    def __init__(self, From, To, Zone):
        self.From = From
        self.To = To
        self.Zone = Zone


class TzEntry:
    generic: str
    region: str
    timezone: str

    def __init__(self, generic, region, timezone):
        super().__init__()
        self.generic = generic
        self.region = region
        self.timezone = timezone

    def __str__(self):
        return f"[({self.generic}, {self.region}) -> {self.timezone}]"


class TimeZoneMap:
    data: list[TzEntry] = []

    def __init__(self):
        super().__init__()

    def __setitem__(self, key, value: list[str]):
        entry = TzEntry(key, value[0], value[1])
        self.data.append(entry)

    def toLongZoneMap(self):
        out: dict[str, list[list[str]]] = {}
        for e in self.data:
            if e.generic not in out:
                out[e.generic] = []
            out[e.generic].append([e.region, e.timezone])
        return out

    def toTimeZoneMap(self):
        out: dict[str, str] = {}
        for e in self.data:
            out[e.timezone] = e.generic
        return out


# python files generator
class Cldr:
    L = set()  # the list of languages
    S = set()  # the list of scripts
    R = set()  # the list of regions
    V = set()  # the list of variants
    C = set()  # the list of currencies
    U = set()  # the list of units
    Tz = set()  # list of timezones Iana IDs
    TzRules = {}  # list of timezone rules
    TzIDs = set()  # list of zone Ids
    TzCodes = set()  # list of zone Ids (short)
    TzMap = {}  # list of timezone codes to IDs
    TzMap2 = TimeZoneMap()  # list of timezone IDs to Iana IDs
    Ca = set()  # list of calendar

    def __init__(self):
        self.path = Path(CLDR_PATH)
        self.LFile = ''
        self.RFile = ''
        self.SFile = ''
        self.VFile = ''
        self.CFile = ''
        self.UFile = ''

        print('Start of Analyze')
        if not IGNORE_PY_DEPENDENCIES:
            for path in self.path.joinpath('common/main').glob('*.xml'):
                print('Analyzing of file [%s].' % path)
                doc = minidom.parse(str(path))
                print("::::::::: Locales Codes")
                displayNamesTag = doc.getElementsByTagName('localeDisplayNames')
                if len(displayNamesTag) > 0:
                    parentTag = displayNamesTag[0].getElementsByTagName('languages')
                    if len(parentTag) == 1:
                        parentTag = parentTag[0]
                        Cldr.GenerateLanguages(parentTag)
                    parentTag = displayNamesTag[0].getElementsByTagName('scripts')
                    if len(parentTag) == 1:
                        parentTag = parentTag[0]
                        Cldr.GenerateScripts(parentTag)
                    parentTag = displayNamesTag[0].getElementsByTagName('territories')
                    if len(parentTag) == 1:
                        parentTag = parentTag[0]
                        Cldr.GenerateRegions(parentTag)
                    parentTag = displayNamesTag[0].getElementsByTagName('variants')
                    if len(parentTag) == 1:
                        parentTag = parentTag[0]
                        Cldr.GenerateVariants(parentTag)

                print("::::::::: Currencies Codes")
                numberTag = doc.getElementsByTagName('numbers')
                if len(numberTag) > 0:
                    parentTag = numberTag[0].getElementsByTagName('currencies')
                    if len(parentTag) == 1:
                        parentTag = parentTag[0]
                        Cldr.GenerateCurrencies(parentTag)

                print("::::::::: Units Names")
                unitTag = doc.getElementsByTagName('units')
                if len(unitTag) > 0:
                    parentTags = unitTag[0].getElementsByTagName('unitLength')
                    for parentTag in parentTags:
                        Cldr.GenerateUnits(parentTag)

        path = self.path.joinpath('common/supplemental/metaZones.xml')
        print('Analyzing File [%s].' % path)
        doc = minidom.parse(str(path))
        metaZones = doc.getElementsByTagName('metaZones')
        if len(metaZones) > 0:
            Cldr.GenerateZones(metaZones[0])
            Cldr.GenerateZonesIds(metaZones[0])
        print('All files Analyzed.')

    @classmethod
    def GenerateLanguages(cls, parent: Element):
        # parent = <languages> <language type="lang">display</language> ... </languages>
        children = parent.getElementsByTagName('language')
        for child in children:
            t = child.getAttribute('type')
            alt = child.getAttribute('alt')
            if len(alt) != 0 or len(t) < 2:
                continue
            lang = t.split('_')
            cls.L.add(lang[0])

    @classmethod
    def GenerateScripts(cls, parent: Element):
        # parent = <scripts> <script type="script">display</script> ... </scripts>
        children = parent.getElementsByTagName('script')
        for child in children:
            t = child.getAttribute('type')
            alt = child.getAttribute('alt')
            if len(alt) != 0 or len(t) != 4:
                continue
            script = t
            cls.S.add(script)

    @classmethod
    def GenerateRegions(cls, parent: Element):
        # parent = <territories> <territory type="region">display</territory> ... </territories>
        children = parent.getElementsByTagName('territory')
        for child in children:
            t = child.getAttribute('type')
            alt = child.getAttribute('alt')
            if len(alt) != 0 or (len(t) != 2 and len(t) != 3):
                continue
            region = t
            cls.R.add(region)

    @classmethod
    def GenerateVariants(cls, parent: Element):
        # parent = <variants> <variant type="variant">display</variant> ... </variants>
        children = parent.getElementsByTagName('variant')
        for child in children:
            t = child.getAttribute('type')
            alt = child.getAttribute('alt')
            if len(alt) != 0 or len(t) < 4:
                continue
            variant = t
            cls.V.add(variant)

    @classmethod
    def GenerateCurrencies(cls, parent: Element):
        # parent = <currencies> <currency type="currency">display</currency> ... </currencies>
        children = parent.getElementsByTagName('currency')
        for child in children:
            t = child.getAttribute('type')
            if len(t) != 3:
                continue
            currency = t
            cls.C.add(currency)

    @classmethod
    def GenerateUnits(cls, parent: Element):
        # parent = <units> <unit type="unit">display</unit> ... </units>
        children = parent.getElementsByTagName('unit')
        for child in children:
            t = child.getAttribute('type')
            if len(t) == 0:
                continue
            unit = t
            cls.U.add(unit)

    @classmethod
    def GenerateZones(cls, metaZone: Element):
        """
        Generate Zone and Corresponding Rules
        """
        metazoneInfo = metaZone.getElementsByTagName('metazoneInfo')
        if len(metazoneInfo) == 0:
            return
        timezone = metazoneInfo[0].getElementsByTagName('timezone')
        for tag in timezone:
            t = tag.getAttribute('type')
            if len(t) == 0:
                continue
            cls.Tz.add(t)
            rules = []
            usesMetazone = tag.getElementsByTagName('usesMetazone')
            for meta in usesMetazone:
                To = meta.getAttribute('to')
                From = meta.getAttribute('from')
                Zone = meta.getAttribute('mzone')
                rules.append(TzRule(From, To, Zone))
            cls.TzRules[t] = rules

    @classmethod
    def GenerateZonesIds(cls, metaZone: Element):
        """
        Generate Zone and Corresponding Rules
        """
        metazoneIds = metaZone.getElementsByTagName('metazoneIds')
        if len(metazoneIds) == 0:
            return
        metazoneId = metazoneIds[0].getElementsByTagName('metazoneId')
        for meta in metazoneId:
            shortID = meta.getAttribute('shortId')
            longID = meta.getAttribute('longId')
            if len(shortID) == 0 or len(longID) == 0:
                continue
            cls.TzIDs.add(longID)
            cls.TzCodes.add(shortID.upper())
            cls.TzMap[shortID.upper()] = longID

        mapTimezones = metaZone.getElementsByTagName('mapTimezones')
        if len(mapTimezones) == 0:
            return
        mapZones = mapTimezones[0].getElementsByTagName('mapZone')

        for mapZone in mapZones:
            ID = mapZone.getAttribute('other')
            region = mapZone.getAttribute('territory')
            IanaID = mapZone.getAttribute('type')
            if len(ID) == 0 or len(IanaID) == 0:
                continue
            cls.TzMap2[ID] = [region, IanaID]

    def WriteLanguagesAsArray(self):
        self.LFile = "L.py"
        f = open(self.LFile, mode='w', encoding='US-ASCII')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('## This file has been Generated by cldr.py \n')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('\n')
        f.write('# The list of supported language code.\n')
        f.write('\n')
        f.write('L = [')
        i = 0
        for language in sorted(Cldr.L):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % language)
            i += 1

        f.write('\n]\n')
        f.close()

    def WriteScriptsAsArray(self):
        self.SFile = "S.py"
        f = open(self.SFile, mode='w', encoding='US-ASCII')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('## This file has been Generated by cldr.py \n')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('\n')
        f.write('# The list of supported script code.\n')
        f.write('\n')
        f.write('S = [')
        i = 0
        for script in sorted(Cldr.S):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % script)
            i += 1

        f.write('\n]\n')
        f.close()

    def WriteRegionsAsArray(self):
        self.RFile = "R.py"
        f = open(self.RFile, mode='w', encoding='US-ASCII')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('## This file has been Generated by cldr.py \n')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('\n')
        f.write('# The list of supported country/region code.\n')
        f.write('\n')
        f.write('R = [')
        i = 0
        for region in sorted(Cldr.R):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % region)
            i += 1

        f.write('\n]\n')
        f.close()

    def WriteVariantsAsArray(self):
        self.VFile = "V.py"
        f = open(self.VFile, mode='w', encoding='US-ASCII')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('## This file has been Generated by cldr.py \n')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('\n')
        f.write('# The list of supported variant code.\n')
        f.write('\n')
        f.write('V = [')
        i = 0
        for variant in sorted(Cldr.V):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % variant)
            i += 1

        f.write('\n]\n')
        f.close()

    def WriteCurrenciesAsArray(self):
        self.CFile = "C.py"
        f = open(self.CFile, mode='w', encoding='US-ASCII')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('## This file has been Generated by cldr.py \n')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('\n')
        f.write('# The list of supported currency code.\n')
        f.write('\n')
        f.write('C = [')
        i = 0
        for currency in sorted(Cldr.C):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % currency)
            i += 1

        f.write('\n]\n')
        f.close()

    def WriteUnitsAsArray(self):
        self.UFile = "U.py"
        f = open(self.UFile, mode='w', encoding='US-ASCII')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('## This file has been Generated by cldr.py \n')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('\n')
        f.write('# The list of supported unit (type-name).\n')
        f.write('\n')
        f.write('U = [')
        i = 0
        for unit in sorted(Cldr.U):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % unit)
            i += 1

        f.write('\n]\n')
        f.close()

    def WriteTimeZoneAsArray(self):
        for region in regionToRules:
            Cldr.Tz.add(region)

        self.UFile = "Tz.py"
        f = open(self.UFile, mode='w', encoding='US-ASCII')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('## This file has been Generated by cldr.py \n')
        f.write('## ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n')
        f.write('\n')
        f.write('# The list of supported timezone.\n')
        f.write('\n')
        f.write('Tz = [')
        i = 0
        for tz in sorted(Cldr.Tz):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % tz)
            i += 1

        f.write('\n]\n')

        # add other short id
        for short in displayShortToLong:
            if short not in Cldr.TzCodes:
                Cldr.TzCodes.add(short)

        f.write('\n')
        f.write('# The list of supported timezone IDs (short).\n')
        f.write('\n')
        f.write('TzIds = [')
        i = 0
        for tz in sorted(Cldr.TzCodes):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % tz)
            i += 1

        f.write('\n]\n')

        f.write('\n')
        f.write('# The list of supported timezone IDs (long).\n')
        f.write('\n')
        f.write('TzIDs = [')
        i = 0
        for tz in sorted(Cldr.TzIDs):
            if i == 0 or i >= MAX_COLS_PER_LINE:
                f.write('\n\t')
                i = 0
            f.write("'%s', " % tz)
            i += 1

        f.write('\n]\n')
        f.close()


displayShortToLong = {
    "ACT": "Australia_Central",
    "ACWT": "Australia_CentralWestern",
    "AET": "Australia_Eastern",
    "AFT": "Afghanistan",
    "AKST": "Gambier",
    "AKT": "Alaska",
    "ALMT": "Alma_Ata",
    "AMT": "Amazon",
    "ANAT": "Anadyr",
    "AQTT": "Aqtobe",
    "ART": "Argentina",
    "AT": "Atlantic",
    "ATT": "Troll",
    "AWT": "Australia_Western",
    "AZOT": "Azores",
    "AZT": "Azerbaijan",
    "BDT": "Bangladesh",
    "BNT": "Brunei",
    "BOT": "Bolivia",
    "BRT": "Brasilia",
    "BT": "British",
    "BTT": "Bhutan",
    "CAT": "Africa_Central",
    "CCT": "Cocos",
    "CET": "Europe_Central",
    "CHAT": "Chatham",
    "CHOT": "Ulaanbaatar",
    "CHUT": "Chuuk",
    "CIT": "Christmas",
    "CKT": "Cook",
    "CLT": "Chile",
    "COT": "Colombia",
    "CT": "America_Central",
    "CVT": "Cape_Verde",
    "ChT": "Chamorro",
    "DAVT": "Davis",
    "DDUT": "Dumont_Urville",
    "EAST": "Easter",
    "EAT": "Africa_Eastern",
    "ECT": "Ecuador",
    "EET": "Europe_Eastern",
    "EGT": "Greenland_Eastern",
    "ET": "America_Eastern",
    "FJT": "Fiji",
    "FKT": "Falkland",
    "FNT": "Noronha",
    "GALT": "Galapagos",
    "GAMT": "Gambier",
    "GET": "Georgia",
    "GFT": "French_Guiana",
    "GILT": "Gilbert_Islands",
    "GMT": "GMT",
    # "GMT+01:00": "GMT+01:00",
    # "GMT+02:00": "GMT+02:00",
    # "GMT+03:00": "GMT+03:00",
    # "GMT+04:00": "Ulyanovsk",
    # "GMT+05:00": "Atyrau Time",
    # "GMT+06:00": "GMT+06:00",
    # "GMT+07:00": "Barnaul Time",
    # "GMT+08:00": "GMT+08:00",
    # "GMT+09:00": "GMT+09:00",
    # "GMT+10:00": "GMT+10:00",
    # "GMT+11:00": "GMT+11:00",
    # "GMT+12:00": "GMT+12:00",
    # "GMT+13:00": "GMT+13:00",
    # "GMT+14:00": "GMT+14:00",
    # "GMT-01:00": "GMT-01:00",
    # "GMT-02:00": "GMT-02:00",
    # "GMT-03:00": "GMT-03:00",
    # "GMT-04:00": "GMT-04:00",
    # "GMT-05:00": "GMT-05:00",
    # "GMT-06:00": "GMT-06:00",
    # "GMT-07:00": "GMT-07:00",
    # "GMT-08:00": "GMT-08:00",
    # "GMT-09:00": "GMT-09:00",
    # "GMT-10:00": "GMT-10:00",
    # "GMT-11:00": "GMT-11:00",
    # "GMT-12:00": "GMT-12:00",
    "GT": "Gulf",
    "GYT": "Guyana",
    "HAT": "Hawaii_Aleutian",
    "HKT": "Hong_Kong",
    "HOVT": "Hovd",
    "HST": "Hawaii_Aleutian",
    "ICT": "Indochina",
    "IOT": "Indian_Ocean",
    "IRKT": "Irkutsk",
    "IRT": "Iran",
    "IT": "India",
    "JT": "Japan",
    "KGT": "Kirgizstan",
    "KOST": "Kosrae",
    "KRAT": "Krasnoyarsk",
    "KT": "Korean",
    "LHT": "Lord Howe",
    "LINT": "Line_Islands",
    "MAGT": "Magadan",
    "MART": "Marquesas",
    "MAWT": "Mawson",
    "MET": "Middle_Europe",
    "MHT": "Marshall_Islands",
    "MMT": "Myanmar",
    "MT": "Mountain",
    "MUT": "Mauritius",
    "MVT": "Maldives",
    "MYT": "Malaysia",
    "NCT": "New_Caledonia",
    "NFT": "Norfolk",
    "NOVT": "Novosibirsk",
    "NPT": "Nepal",
    "NRT": "Nauru",
    "NT": "Newfoundland",
    "NUT": "Niue",
    "NZT": "New_Zealand",
    "OMST": "Omsk",
    "ORAT": "Oral",
    "PET": "Peru",
    "PETT": "Kamchatka",
    "PGT": "Papua_New_Guinea",
    "PHOT": "Phoenix_Islands",
    "PKT": "Pakistan",
    "PMT": "Pierre_Miquelon",
    "PONT": "Ponape",
    "PST": "Pitcairn",
    "PT": "Pacific",
    "PWT": "Palau",
    "PYT": "Paraguay",
    "QOST": "Kostanay",
    "QYZT": "Qyzylorda",
    "RET": "Reunion",
    "ROTT": "Rothera",
    "SAKT": "Sakhalin",
    "SAMT": "Samara",
    "SAT": "Africa_Southern",
    "SBT": "Solomon",
    "SCT": "Seychelles",
    "SGT": "Singapore",
    "SRET": "Srednekolymsk",
    "SRT": "Suriname",
    "ST": "Samoa",
    "SYOT": "Syowa",
    "TAHT": "Tahiti",
    "TFT": "French_Southern",
    "TJT": "Tajikistan",
    "TKT": "Tokelau",
    "TLT": "Timor_Leste",
    "TMT": "Turkmenistan",
    "TOT": "Tonga",
    "TRT": "Turkey",
    "TVT": "Tuvalu",
    "ULAT": "Ulaanbaatar",
    "UTC": "Etc/UTC",
    "UYT": "Uruguay",
    "UZT": "Uzbekistan",
    "VET": "Venezuela",
    "VLAT": "Vladivostok",
    "VOST": "Vostok",
    "VUT": "Vanuatu",
    "WAKT": "Wake",
    "WAT": "Africa_Western",
    "WET": "Europe_Western",
    "WFT": "Wallis",
    "WGT": "Greenland_Western",
    "WIB": "Indonesia_Western",
    "WIT": "Indonesia_Eastern",
    "WITA": "Indonesia_Central",
    "WST": "Apia",
    "XJT": "Xinjiang",
    "YAKT": "Yakutsk",
    "YEKT": "Yekaterinburg"
}
displayLongToShort = {}
for short in displayShortToLong:
    long = displayShortToLong[short]
    if long in displayLongToShort:
        continue
    displayLongToShort[long] = short

