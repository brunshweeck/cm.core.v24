Year = '' # replace with '2000' for gb18030-2000 data
IS_2000 = Year == '2000'
Limit = 8

from External.charset import gb18030


def generateString(db, f):
    i = 0
    cnt = 0
    for ch in db:
        if i >= Limit:
            i = 0
            cnt += 1
            f.write('\n')
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
    return cnt


def generateArray(db, f):
    i = 0
    cnt = 0
    for c in db:
        if i >= Limit:
            i = 0
            cnt += 1
            f.write('\n')
        if i == 0:
            f.write('\t')
        f.write('0x%.4X, ' % c)
        i += 1
    return cnt


if __name__ == '__main__':
    f = open('../../Lib/meta/charset/GB18030DecoderIndex%s.db' % Year, mode='w')
    f.write('CharArray innerDecoderIndex0 = {\n')
    generateString(gb18030.innerDecoderIndex0, f)
    f.write('\n};\n')
    f.write('CharArray innerDecoderIndex1 = {\n')
    generateString(gb18030.innerDecoderIndex1, f)
    f.write('\n};\n')
    f.write('CharArray innerDecoderIndex2 = {\n')
    generateString(gb18030.innerDecoderIndex2, f)
    f.write('\n};\n')
    f.write('CharArray innerDecoderIndex3 = {\n')
    generateString(gb18030.innerDecoderIndex3, f)
    f.write('\n};\n')
    f.write('CharArray innerDecoderIndex4 = {\n')
    generateString(gb18030.innerDecoderIndex4, f)
    f.write('\n};\n')
    f.write('CharArray innerDecoderIndex5 = {\n')
    generateString(gb18030.innerDecoderIndex5, f)
    f.write('\n};\n')
    f.write('\n\n')
    f.write('decoderIndex1 = {\n')
    generateArray(gb18030.decoderIndex1, f)
    f.write('\n};\n')
    f.write('\n\n')
    f.write('decoderIndex2 = {\n')
    f.write('\tString(innerDecoderIndex0),\n')
    f.write('\tString(innerDecoderIndex1),\n')
    f.write('\tString(innerDecoderIndex2),\n')
    f.write('\tString(innerDecoderIndex3),\n')
    f.write('\tString(innerDecoderIndex4),\n')
    f.write('\tString(innerDecoderIndex5),\n')
    f.write('};\n')
    f.close()

    f = open('../../Lib/meta/charset/GB18030EncoderIndex%s.db' % Year, mode='w')
    f.write('CharArray innerEncoderIndex0 = {\n')
    generateString(gb18030.innerEncoderIndex0, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex1 = {\n')
    generateString(gb18030.innerEncoderIndex1, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex2 = {\n')
    generateString(gb18030.innerEncoderIndex2, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex3 = {\n')
    generateString(gb18030.innerEncoderIndex3, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex4 = {\n')
    generateString(gb18030.innerEncoderIndex4, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex5 = {\n')
    generateString(gb18030.innerEncoderIndex5, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex6 = {\n')
    generateString(gb18030.innerEncoderIndex6, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex7 = {\n')
    generateString(gb18030.innerEncoderIndex7, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex8 = {\n')
    generateString(gb18030.innerEncoderIndex8, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex9 = {\n')
    generateString(gb18030.innerEncoderIndex9, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex10 = {\n')
    generateString(gb18030.innerEncoderIndex10, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex11 = {\n')
    generateString(gb18030.innerEncoderIndex11, f)
    f.write('\n};\n')
    f.write('CharArray innerEncoderIndex12 = {\n')
    generateString(gb18030.innerEncoderIndex12, f)
    f.write('\n};\n')
    f.write('\n\n')
    f.write('encoderIndex1 = {\n')
    generateArray(gb18030.encoderIndex1, f)
    f.write('\n};\n')
    f.write('\n\n')
    f.write('encoderIndex2 = {\n')
    f.write('\tString(innerEncoderIndex0),\n')
    f.write('\tString(innerEncoderIndex1),\n')
    f.write('\tString(innerEncoderIndex2),\n')
    f.write('\tString(innerEncoderIndex3),\n')
    f.write('\tString(innerEncoderIndex4),\n')
    f.write('\tString(innerEncoderIndex5),\n')
    f.write('\tString(innerEncoderIndex6),\n')
    f.write('\tString(innerEncoderIndex7),\n')
    f.write('\tString(innerEncoderIndex8),\n')
    f.write('\tString(innerEncoderIndex9),\n')
    f.write('\tString(innerEncoderIndex10),\n')
    f.write('\tString(innerEncoderIndex11),\n')
    f.write('\tString(innerEncoderIndex12),\n')
    f.write('};\n')
    f.close()

    f = open('../../Lib/meta/charset/GB18030Index%s.db' % Year, mode='w')
    f.write('CharArray innerIndex0 = {\n')
    generateString(gb18030.innerIndex0, f)
    f.write('\n};\n')
    f.write('CharArray innerIndex1 = {\n')
    generateString(gb18030.innerIndex1, f)
    f.write('\n};\n')
    f.write('CharArray innerIndex2 = {\n')
    generateString(gb18030.innerIndex2, f)
    f.write('\n};\n')
    f.write('CharArray innerIndex3 = {\n')
    generateString(gb18030.innerIndex3, f)
    f.write('\n};\n')
    f.write('CharArray innerIndex4 = {\n')
    generateString(gb18030.innerIndex4, f)
    f.write('\n};\n')
    f.write('CharArray innerIndex5 = {\n')
    generateString(gb18030.innerIndex5, f)
    f.write('\n};\n')
    f.write('CharArray innerIndex6 = {\n')
    generateString(gb18030.innerIndex6, f)
    f.write('\n};\n')
    f.write('CharArray innerIndex7 = {\n')
    generateString(gb18030.innerIndex7, f)
    f.write('\n};\n')
    f.write('\n\n')
    f.write('index1 = {\n')
    generateArray(gb18030.index1, f)
    f.write('\n};\n')
    f.write('\n\n')
    f.write('index2 = {\n')
    f.write('\tString(innerIndex0),\n')
    f.write('\tString(innerIndex1),\n')
    f.write('\tString(innerIndex2),\n')
    f.write('\tString(innerIndex3),\n')
    f.write('\tString(innerIndex4),\n')
    f.write('\tString(innerIndex5),\n')
    f.write('\tString(innerIndex6),\n')
    f.write('\tString(innerIndex7),\n')
    f.write('};\n')
    f.close()
