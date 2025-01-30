import doubleByte2
from External.charset import DoubleByteTableGen


def generate(b2cStr: list[str], b2cSBStr='', charset=''):
    DoubleByteTableGen.generate(b2cStr, b2cSBStr, charset, '../../../Lib/meta/charset/ext/%s.db')
    ...


if __name__ == '__main__':
    generate(doubleByte2.Big5_b2cStr, doubleByte2.Big5_b2cSBStr, 'Big5')
    generate(doubleByte2.EUC_KR_b2cStr, doubleByte2.EUC_KR_b2cSBStr, 'EUC_KR')
    generate(doubleByte2.EUC_CN_b2cStr, doubleByte2.EUC_CN_b2cSBStr, 'GB2312')
    generate(doubleByte2.IBM300_b2cStr, doubleByte2.IBM300_b2cSBStr, 'IBM300')
    generate(doubleByte2.IBM930_b2cStr, doubleByte2.IBM930_b2cSBStr, 'IBM930')
    generate(doubleByte2.IBM933_b2cStr, doubleByte2.IBM933_b2cSBStr, 'IBM933')
    generate(doubleByte2.IBM935_b2cStr, doubleByte2.IBM935_b2cSBStr, 'IBM935')
    generate(doubleByte2.IBM937_b2cStr, doubleByte2.IBM937_b2cSBStr, 'IBM937')
    generate(doubleByte2.IBM939_b2cStr, doubleByte2.IBM939_b2cSBStr, 'IBM939')
    generate(doubleByte2.IBM942_b2cStr, doubleByte2.IBM942_b2cSBStr, 'IBM942')
    generate(doubleByte2.IBM943_b2cStr, doubleByte2.IBM943_b2cSBStr, 'IBM943')
    generate(doubleByte2.IBM948_b2cStr, doubleByte2.IBM948_b2cSBStr, 'IBM948')
    generate(doubleByte2.IBM949_b2cStr, doubleByte2.IBM949_b2cSBStr, 'IBM949')
    generate(doubleByte2.IBM950_b2cStr, doubleByte2.IBM950_b2cSBStr, 'IBM950')
    generate(doubleByte2.IBM970_b2cStr, doubleByte2.IBM970_b2cSBStr, 'IBM970')
    generate(doubleByte2.IBM1364_b2cStr, doubleByte2.IBM1364_b2cSBStr, 'IBM1364')
    generate(doubleByte2.IBM1381_b2cStr, doubleByte2.IBM1381_b2cSBStr, 'IBM1381')
    generate(doubleByte2.IBM1383_b2cStr, doubleByte2.IBM1383_b2cSBStr, 'IBM1383')
    generate(doubleByte2.JIS_X_0208_b2cStr, doubleByte2.JIS_X_0208_b2cSBStr, 'JIS_X_0208')
    generate(doubleByte2.JIS_X_0208_MS932_b2cStr, doubleByte2.JIS_X_0208_MS932_b2cSBStr, 'JIS_X_0208_MS932')
    generate(doubleByte2.JIS_X_0208_MS5022X_b2cStr, doubleByte2.JIS_X_0208_MS5022X_b2cSBStr, 'JIS_X_0208_MS5022X')
    generate(doubleByte2.JIS_X_0208_Solaris_b2cStr, doubleByte2.JIS_X_0208_Solaris_b2cSBStr, 'JIS_X_0208_Solaris')
    generate(doubleByte2.JIS_X_0212_b2cStr, doubleByte2.JIS_X_0212_b2cSBStr, 'JIS_X_0212')
    generate(doubleByte2.JIS_X_0212_MS5022X_b2cStr, doubleByte2.JIS_X_0212_MS5022X_b2cSBStr, 'JIS_X_0212_MS5022X')
    generate(doubleByte2.JIS_X_0212_Solaris_b2cStr, doubleByte2.JIS_X_0212_Solaris_b2cSBStr, 'JIS_X_0212_Solaris')
    generate(doubleByte2.PCK_b2cStr, doubleByte2.PCK_b2cSBStr, 'PCK')
