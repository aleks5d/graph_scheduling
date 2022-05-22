import random

random.seed(1e9 + 7)


def randStr():
    s = ""
    for i in range(10):
        s += chr(ord('a') + random.randint(0, 25))
    return s


f = open('list', 'a')
for i in range(100):
    f.write("2 " + randStr() + '\n')
