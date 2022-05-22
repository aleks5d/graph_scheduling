import os
import subprocess
import sys

f = open('list')
tests = f.readlines()
f.close()

# algos = ['DLS', 'HEFT', 'LA', 'OPT']
# algos = ['DLS', 'DLS_last', 'OPT']
# algos = ['HEFT', 'HEFT_median', 'HEFT_sqrt', 'OPT']
algos = ['LA', 'LA_all', 'OPT']

for i in range(len(tests)):
    print('Test:', i, file=sys.stderr)
    print('Test:', i)
    print(tests[i].strip())
    path = "tests/" + str(i)
    if not os.path.exists(path):
        os.mkdir("tests/" + str(i))
    args = tests[i].split()
    f = open(path + "/test", 'w')
    subprocess.call(["./test_gen"] + args, stdout=f)
    f.close()
    res = {}
    file_latex = 0
    to_latex = ""
    if args[0] == '0':
        file_latex = open('latex/' + args[0] + '_' + args[1] + '_' + args[3], 'a')
        to_latex = args[2]
    else:
        file_latex = open('latex/' + args[0], 'a')
        to_latex = args[1]
    for algo in algos:
        print("Algo:", algo, end=" ")
        try:
            f = open(path + '/' + algo, 'r')
            assert float(f.readline()) != 0
            print('generated before', file=sys.stderr)
        except:
            to = open(path + '/' + algo, 'w')
            fr = open(path + '/test', 'r')
            try:
                subprocess.call(['./' + algo], stdin=fr, stdout=to)
            except subprocess.TimeoutExpired:
                to.write("0")
            fr.close()
            to.close()

        to = open(path + '/' + algo)
        result = float(to.readline())
        if len(to_latex):
            to_latex += " & "
        to_latex += str(result)
        print(result)
        to.close()
        res[algo] = result
    to_latex += " \\\\\n"
    file_latex.write(to_latex)
    mi = res['OPT']
    for key, value in res.items():
        if value < mi:
            print("ERROR")
            exit(1)
