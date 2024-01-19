
#include <algorithm>
#include <array>
#include <iostream>
#include <ranges>

import CNF;
import SAT_hybrid;

using namespace SAT;
using namespace SAT::Test;

struct RecursionStatus
{
  bool is_sat;
  size_t recursion_ttl;
};

struct CompiletimeResult
{
  bool is_sat;
  bool is_finished;
  size_t steps_done;
};

template <typename CNFType, typename Array>
consteval RecursionStatus is_cnf_sat(const CNFType &cnf, Array solution,
                                     size_t recursion_ttl, size_t idx)
{
  if (recursion_ttl == 0)
  {
    return {false, 0};
  }
  if (idx == solution.size())
  {
    return {cnf.eval(solution.begin()), recursion_ttl};
  }
  recursion_ttl--;
  solution[idx] = ValueEncoding::FALSE;
  auto res = is_cnf_sat(cnf, solution, recursion_ttl, idx + 1);
  if (res.recursion_ttl == 0)
  {
    return res;
  }
  if (!res.is_sat)
  {
    solution[idx] = ValueEncoding::TRUE;
    res = is_cnf_sat(cnf, solution, res.recursion_ttl, idx + 1);
  }
  return res;
}

template <typename CNF> constexpr var_num_t get_max_var_num(CNF cnf)
{
  auto max_view = std::views::transform(cnf, [](const auto &conj) {
    auto num_range = std::views::transform(conj, get_num);
    return *std::ranges::max_element(num_range);
  });
  var_num_t bound = *std::ranges::max_element(max_view);
  return bound;
}

#include "tests.h"

constexpr auto &&ActiveTest = autogen_tests[0];

constexpr auto run_test()
{
  constexpr size_t max_var_num = get_max_var_num(ActiveTest.cnf) + 1;
  constexpr std::array<ValueEncoding, max_var_num> solution{};
  constexpr size_t recursion_ttl = 1 << 6;
  auto status = is_cnf_sat(ActiveTest.cnf, solution, recursion_ttl, 1);
  CompiletimeResult result = {status.is_sat, status.recursion_ttl != 0,
                              recursion_ttl - status.recursion_ttl};
  return result;
}

int main()
{
  std::cout << "Running tests\n";
  auto precalc_res = run_test();

  if (precalc_res.is_finished)
  {
    if (ActiveTest.is_sat != precalc_res.is_sat) {
      std::cout << "Error: wrong compiletime SAT result\n";
      return -1;
    }
    std::cout << "Answer " << precalc_res.is_sat << "\n";
    return 0;
  }

  // TODO: init solver in compiletime
  Hybrid::ResumableSATSolver solver;
  solver.set_cnf_task(ActiveTest.cnf, ActiveTest.cnf.get_num_vars());
  bool res = solver.resume_solving_from(precalc_res.steps_done);

  if (ActiveTest.is_sat != res) {
    std::cout << "Error: wrong runtime SAT result\n";
    return -1;
  }

  std::cout << "Answer " << res << "\n";

  return 0;
}