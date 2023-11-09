import CNF;
import SAT_rt;

#include <cassert>
#include <iostream>

int main()
{
  SAT::Runtime::SATSolver solver;
  solver.set_cnf_task(SAT::Test::sat_test.cnf,
                      SAT::Test::sat_test.cnf.get_num_vars());
  auto res = solver.solve();

  std::for_each(solver.answer_begin(), solver.answer_end(),
                [](int val) { std::cout << val << " "; });

  assert(res == SAT::Test::sat_test.is_sat && "Wrong SAT value");

  bool eval_res = SAT::Test::sat_test.cnf.eval(solver.answer_begin());

  assert(eval_res == SAT::Test::sat_test.is_sat && "Test evaluation failed");

  return 0;
}