from random import randint, random, seed

def get_rand_ints(num, a, b):
    idxs = [*map(lambda x : randint(a, b), range(num))]
    return idxs


def gen_sat(num_vars : int, num_conj : int, conj_len : int):
    vars = get_rand_ints(num_vars + 1, 0, 1)
    sat = []
    for _ in range(num_conj):
        idxs = get_rand_ints(conj_len, 1, num_vars)
        conj = map(lambda x : x if vars[x] else -x, idxs)
        sat.append(list(conj))

    return sat


def gen_unsat(num_vars : int, num_conj : int, conj_len : int):
    vars = get_rand_ints(num_vars + 1, 0, 1)
    # x & ~x = 0
    # (1 | 2 | 3) & (-1 | | ) & (-2 | | ) & (-3 | | ) = 0
    assert(num_conj >= conj_len + 1)
    contrs = list(range(1, conj_len + 1))
    unsat = [contrs]
    for neg_x in contrs:
        idxs = get_rand_ints(conj_len - 1, conj_len + 1, num_vars)
        conj_gen = map(lambda x : x * (1 if randint(0, 1) else -1), idxs)
        conj = list(conj_gen)
        conj.append(-neg_x)
        unsat.append(conj)
    for _ in range(num_conj - len(unsat)):
        idxs = get_rand_ints(conj_len, 1, num_vars)
        conj_gen = map(lambda x : x * (1 if randint(0, 1) else -1), idxs)
        unsat.append(list(conj_gen))

    return unsat


def gen_test(is_sat : bool, cnf, name):
    test = "SATTest<{}> {} {{".format(len(cnf), name)
    conj_strs = []
    for conj in cnf:
        str_gen = map(lambda x : str(x), conj)
        conj_strs.append("{" + ",".join(str_gen) + "}")
    test += "{{" + ",".join(conj_strs) + "}}"
    test += ", {}}};".format('true' if is_sat else 'false')
    return test


if __name__ == '__main__':
    sat = gen_sat(3, 2, 3)
    unsat = gen_unsat(12, 4, 3)
    test = gen_test(True, sat, "sat_test")
    test += '\n'
    test += gen_test(False, unsat, "unsat_test")
    with open("tests.h", "w") as file:
        file.write(test)
