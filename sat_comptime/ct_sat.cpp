#include <array>
#include <cstdio>
#include <cstdlib>

import CNF;

using namespace SAT;
using namespace SAT::Test;

// Generated via testgen.py
//
#include "autogen_tests.h"

template <typename CNFType, typename Array>
consteval bool is_cnf_sat(const CNFType &cnf, Array &solution, size_t idx)
{
  if (idx == solution.size())
  {
    return cnf.eval(solution.begin());
  }
  solution[idx] = ValueEncoding::FALSE;
  bool res = is_cnf_sat(cnf, solution, idx + 1);
  if (!res)
  {
    solution[idx] = ValueEncoding::TRUE;
    res = is_cnf_sat(cnf, solution, idx + 1);
  }
  return res;
}

consteval bool run_tests()
{
  bool all_ok = true;
  constexpr size_t num_vars = autogen_tests.front().cnf.get_num_vars();
  for (auto &&test : autogen_tests)
  {
    std::array<ValueEncoding, num_vars> solution;
    all_ok = all_ok && (is_cnf_sat(test.cnf, solution, 0) == test.is_sat);
    break;
  }
  return all_ok;
}

int main()
{
  constexpr bool success = run_tests();
  if (!success)
  {
    printf("Tests ERROR\n");
    return EXIT_FAILURE;
  }
  printf("Tests OK\n");
  return EXIT_SUCCESS;
}