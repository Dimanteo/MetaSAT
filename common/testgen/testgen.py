from random import randint, seed
from sys import argv

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


# def gen_unsat(num_vars : int, num_conj : int, conj_len : int):
#     vars = get_rand_ints(num_vars + 1, 0, 1)
#     # x & ~x = 0
#     # (1 | 2 | 3) & (-1 | | ) & (-2 | | ) & (-3 | | ) = 0
#     assert(num_conj >= conj_len + 1)
#     contrs = list(range(1, conj_len + 1))
#     unsat = [contrs]
#     for neg_x in contrs:
#         idxs = get_rand_ints(conj_len - 1, conj_len + 1, num_vars)
#         conj_gen = map(lambda x : x * (1 if randint(0, 1) else -1), idxs)
#         conj = list(conj_gen)
#         conj.append(-neg_x)
#         unsat.append(conj)
#     for _ in range(num_conj - len(unsat)):
#         idxs = get_rand_ints(conj_len, 1, num_vars)
#         conj_gen = map(lambda x : x * (1 if randint(0, 1) else -1), idxs)
#         unsat.append(list(conj_gen))

#     return unsat


def gen_test(is_sat : bool, cnf : list[list], num_vars : int, name):
    conj_num = len(cnf)
    conj_len = len(cnf[0])
    type = f"SATTest<CNF<{conj_num}, {conj_len}, {num_vars}>>"

    disjuncts = []
    for conj in cnf:
        str_gen = map(lambda x : str(x), conj)
        disjuncts.extend(str_gen)
    init_list = "{" + ', '.join(disjuncts) + "}"
    array = f"std::array<var_t, {conj_len * conj_num}> {init_list}"

    answer = 'true' if is_sat else 'false'

    test = f"{type} {{{answer}, {array}}},"

    return test


def main():
    seed(42)

    num_tests = 20
    conj_len = 3
    num_conj = 10
    num_vars = 10
    cppcode = "/*** Auto-generated file. Change testgen.py script to modify. ***/\n\n"

    if len(argv) < 2:
        print("Testgen failure: enter output filename")
        exit(42)

    test_type = f"SATTest<CNF<{num_conj}, {conj_len}, {num_vars}>>"
    cppcode += f"std::array<{test_type}, {num_tests}> autogen_tests {{\n"

    for i in range(num_tests):
        sat = gen_sat(  num_conj=num_conj,
                        num_vars=num_vars,
                        conj_len=conj_len)
        test = gen_test(True, sat, num_vars, f"sat_test_{i}")
        cppcode += '  ' + test + '\n'

    # TODO: unsat generation
    # for i in range(num_tests):
    #     unsat = gen_unsat(  num_conj=num_conj,
    #                     num_vars=num_vars,
    #                     conj_len=conj_len)
    #     test = gen_test(False, unsat, num_vars, f"unsat_test_{i}")
    #     cppcode += '  ' + test + '\n'

    cppcode += "};"

    with open(argv[1], "w") as file:
        file.write(cppcode)


if __name__ == '__main__':
    main()
