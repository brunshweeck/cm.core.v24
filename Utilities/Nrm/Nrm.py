from Sample import prop

file = open(file='./Sample.txt', mode='w+')

for p in prop:
    l: list = p
    l.pop(4)
    l.pop(4)
    l.pop(7)
    l.pop(12)
    file.write(str(l) + '\n')
    pass

if __name__ == '__main__':
    pass