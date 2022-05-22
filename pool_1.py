f = open('list', 'a')
for size in range(2, 5):
    for proc in range(2, 6):
        for lex in range(2 ** (size * (size - 1) // 2)):
            f.write(
                "0 "  # gen number
                + str(size)
                + " "  # size
                + str(lex)
                + " "  # lex number
                + str(proc)
                + " "  # proc count
                + "1 10 "  # data min max
                + "1 10 "  # W min max
                + "1 10 "  # B min max
                + "1 10\n"  # L min max
            )
