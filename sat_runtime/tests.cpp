import CNF;
import SAT_rt;

#include <array>
#include <iostream>

using namespace SAT;
using namespace SAT::Test;

template <typename Test>
void test(size_t test_i, SAT::Runtime::SATSolver &solver, Test &test)
{
  std::cout << "Solving " << test.cnf.get_conj_len() << "SAT with "
            << test.cnf.get_num_vars() << " variables in "
            << test.cnf.get_conj_num() << " conjuncts\n";

  solver.set_cnf_task(test.cnf, test.cnf.get_num_vars());
  auto res = solver.solve();

  if (res != test.is_sat)
  {
    std::cerr << "Test " << test_i << " fail: Wrong SAT value. Expected "
              << test.is_sat << " got " << res << "\n";
    abort();
  }

  bool eval_res = test.cnf.eval(solver.answer_begin());

  if (eval_res != test.is_sat)
  {
    std::cerr << "Test " << test_i << " fail: evaluation failure\n";
    std::for_each(solver.answer_begin(), solver.answer_end(),
                  [](int x) { std::cerr << x << " "; });
    std::cerr << "\n";
    abort();
  }

  std::cout << "Result = " << eval_res << "\n";
}

// Generated via testgen.py
//
#include "tests.h"

int main()
{
  SAT::Runtime::SATSolver solver;

  std::cout << "Running tests\n";

  for (size_t i = 0; i < autogen_tests.size(); ++i)
  {
    test(i, solver, autogen_tests[i]);
  }

  std::cout << "Finished\n";
  return 0;
}