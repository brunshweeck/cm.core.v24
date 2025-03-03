import singleByte
from GB18030TableGen import Limit


def generateString(db, f):
    i = 0
    cnt = 0
    val = 0x80
    for ch in db:
        if i >= Limit:
            i = 0
            cnt += 1
            f.write('  // 0x%.2X - 0x%.2X\n' % (val, val + Limit-1))
            val = (val + Limit) % 0x100
        if i == 0:
            f.write('\t')
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
    f.write('  // 0x%.2X - 0x%.2X\n' % (val, val + Limit-1))
    return cnt


def generate(b2cTable='', charset=''):
    f = open(f'../../Lib/meta/charset/{charset}.db', mode='w')
    generateString(b2cTable, f)
    f.close()


if __name__ == '__main__':
    generate(singleByte.IBM437, 'IBM437')
    generate(singleByte.IBM737, 'IBM737')
    generate(singleByte.IBM775, 'IBM775')
    generate(singleByte.IBM850, 'IBM850')
    generate(singleByte.IBM852, 'IBM852')
    generate(singleByte.IBM855, 'IBM855')
    generate(singleByte.IBM857, 'IBM857')
    generate(singleByte.IBM858, 'IBM858')
    generate(singleByte.IBM862, 'IBM862')
    generate(singleByte.IBM866, 'IBM866')
    generate(singleByte.IBM874, 'IBM874')
    generate(singleByte.ISO_8859_2, 'ISO_8859_2')
    generate(singleByte.ISO_8859_4, 'ISO_8859_4')
    generate(singleByte.ISO_8859_5, 'ISO_8859_5')
    generate(singleByte.ISO_8859_7, 'ISO_8859_7')
    generate(singleByte.ISO_8859_9, 'ISO_8859_9')
    generate(singleByte.ISO_8859_13, 'ISO_8859_13')
    generate(singleByte.ISO_8859_15, 'ISO_8859_15')
    generate(singleByte.ISO_8859_16, 'ISO_8859_16')
    generate(singleByte.JIS_X_0201, 'JIS_X_0201')
    generate(singleByte.KOI8_R, 'KOI8_R')
    generate(singleByte.KOI8_U, 'KOI8_U')
    generate(singleByte.MS874, 'MS874')
    generate(singleByte.MS1250, 'MS1250')
    generate(singleByte.MS1251, 'MS1251')
    generate(singleByte.MS1252, 'MS1252')
    generate(singleByte.MS1253, 'MS1253')
    generate(singleByte.MS1254, 'MS1254')
    generate(singleByte.MS1255, 'MS1255')
    generate(singleByte.MS1256, 'MS1256')
    generate(singleByte.MS1257, 'MS1257')
    generate(singleByte.MS1258, 'MS1258')
