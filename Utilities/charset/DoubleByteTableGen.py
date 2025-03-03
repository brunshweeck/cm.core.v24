from External.charset import doubleByte
from External.charset.GB18030TableGen import Limit


def generateString(db='', f=None):
    i = 0
    cnt = 0
    for ch in db:
        if i >= Limit:
            i = 0
            cnt += 1
            f.write('\n')
        if i == 0:
            f.write('\t\t')
        c = ord(ch)
        if c < 0:
            raise "error"
        if c < 0x10000:
            f.write('0x%.4X, ' % c)
            i += 1
        else:
            f.write('0x%.4X, 0x%.4X, ' % (
                (c >> 10) + (0xD800 - (0x10000 >> 10)),
                (c & 0x3ff) + 0xDC00
            ))
            i += 2
    return cnt


def generateStrings(db: list[str], f=None):
    for s in db:
        if len(s) == 0:
            f.write('\t{},\n')
        else:
            f.write('\tString(CharArray{\n')
            generateString(s, f)
            f.write('}),\n')


def generate(b2cStr: list[str] = ..., b2cSBStr='', charset='?', dir='../../Lib/meta/charset/%s.db'):
    f = open(dir % charset, mode='w')
    f.write('b2cSBStr = String(CharArray{\n')
    generateString(b2cSBStr, f)
    f.write('\n});\n')
    f.write('b2cStr = {\n')
    generateStrings(b2cStr, f)
    f.write('\n};\n')
    f.close()


if __name__ == '__main__':
    generate(doubleByte.Johab_b2cStr, doubleByte.Johab_b2cSBStr, 'Johab')
    generate(doubleByte.MS932_b2cStr, doubleByte.MS932_b2cSBStr, 'MS932')
    generate(doubleByte.MS936_b2cStr, doubleByte.MS936_b2cSBStr, 'MS936')
    generate(doubleByte.MS949_b2cStr, doubleByte.MS949_b2cSBStr, 'MS949')
    generate(doubleByte.MS950_b2cStr, doubleByte.MS950_b2cSBStr, 'MS950')
    generate(doubleByte.SJIS_b2cStr, doubleByte.SJIS_b2cSBStr, 'SJIS')
