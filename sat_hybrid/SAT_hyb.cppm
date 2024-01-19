module;

#include <ranges>

export module SAT_hybrid;

import CNF;
import SAT_rt;

namespace SAT
{

namespace Hybrid
{

export class ResumableSATSolver final : public SAT::Runtime::SATSolver
{

  // size_t solutions_prechecked = 0;

  // bool solve()
  // {
  //   auto try_solve_with = [this](var_num_t var_code, bool var_val) {
  //     define_var(var_code, var_val);
  //     // propagate_var(var_code, var_val);
  //     return solve();
  //   };

  //   // Get first undefined variable index
  //   auto first_undef_it =
  //       std::find_if(m_defined_map.cbegin(), m_defined_map.cend(),
  //                    [](bool var_is_def) { return !var_is_def; });

  //   if (first_undef_it == m_defined_map.cend())
  //   {
  //     return check_answer();
  //   }

  //   var_t disjunct = m_cnf[first_undef_it - m_defined_map.begin()];

  //   // Define variable
  //   bool value = disjunct > 0;
  //   var_num_t variable = get_num(disjunct);

  //   bool is_sat = try_solve_with(variable, value);

  //   if (!is_sat)
  //   {
  //     undefine_var(variable);
  //     is_sat = try_solve_with(variable, !value);
  //   }

  //   return is_sat;
  // }

public:
  bool resume_solving_from(size_t steps_done)
  {
    size_t solutions_prechecked = steps_done / m_var_uses.size();
    for (size_t pos = m_var_uses.size() - 1; pos > 0; --pos)
    {
      bool var_val = solutions_prechecked & 1UL;
      define_var(pos, var_val);
      solutions_prechecked = solutions_prechecked >> 1;
    }
    return solve();
  }
};

} // namespace Hybrid

} // namespace SAT