import os
import subprocess
import sys
import random

random.seed(1e9 + 7)

# algos = ['DLS', 'HEFT', 'LA', 'OPT']
# algos = ['DLS', 'DLS_last', 'OPT']
# algos = ['HEFT', 'HEFT_median', 'HEFT_sqrt', 'OPT']
# algos = ['LA', 'LA_all', 'OPT']

algos = ['DLS', 'DLS_last', 'HEFT', 'HEFT_median', 'HEFT_sqrt', 'LA', 'LA_all', 'OPT']

sums = {}

for algo in algos:
    sums[algo] = 0


def gen_str():
    res = ''
    for i in range(10):
        res += chr(ord('a') + random.randint(0, 25))
    return res


for i in range(1000):
    print('Test:', i)
    f = open('./temp_test', 'w')
    subprocess.call(["./test_gen", '2', gen_str()], stdout=f)
    f.close()

    res = {}
    for algo in algos:
        to = open('./temp_out', 'w')
        fr = open('./temp_test', 'r')
        try:
            subprocess.call(['./' + algo], stdin=fr, stdout=to, timeout=100)
        except subprocess.TimeoutExpired:
            to.write("0")
        fr.close()
        to.close()

        f = open('./temp_out')
        res[algo] = float(f.readline())
        f.close()
    if res['OPT'] != 0:
        for algo in algos:
            sums[algo] += res[algo] / res['OPT']
    print('result: ', sums)
