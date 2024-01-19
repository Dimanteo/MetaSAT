module;

#include <algorithm>
#include <stddef.h>
#include <unordered_set>
#include <vector>

export module SAT_rt;

import CNF;

namespace SAT
{

namespace Runtime
{

export class SATSolver
{
public:
  using answer_type = std::vector<ValueEncoding>;
  using iterator = answer_type::iterator;
  using const_iterator = answer_type::const_iterator;

private:
  size_t m_conj_len = 0;

  std::vector<var_t> m_cnf;

  //  1 - true
  //  0 - false
  // -1 - any
  //
  answer_type m_answer;

  // Encodes info about currently defined variables in CNF.
  // mask[i] = false -> variable on that position in cnf is undefined
  // mask[i] = true  -> variable is assigned or it's value doesn't matter
  //
  std::vector<bool> m_defined_map;

protected:
  // Mapping of variables to disjuncts.
  //
  std::vector<std::unordered_set<size_t>> m_var_uses;

public:
  SATSolver() = default;

  template <typename CNF>
  requires requires(CNF cnf)
  {
    cnf.get_conj_len();
    cnf.get_conj_num();
    cnf.begin();
    cnf.end();
  }
  void set_cnf_task(const CNF &cnf_task, size_t num_vars)
  {
    m_conj_len = cnf_task.get_conj_len();
    size_t conj_num = cnf_task.get_conj_num();

    m_cnf.clear();
    m_cnf.reserve(conj_num * m_conj_len);
    for (auto &&conj : cnf_task)
    {
      for (auto &&v : conj)
      {
        m_cnf.push_back(v);
      }
    }

    invalidate_task(num_vars);
  }

  bool solve()
  {
    // Get first undefined variable index
    auto first_undef_it =
        std::find_if(m_defined_map.cbegin(), m_defined_map.cend(),
                     [](bool var_is_def) { return !var_is_def; });

    if (first_undef_it == m_defined_map.cend())
    {
      return check_answer();
    }

    var_t disjunct = m_cnf[first_undef_it - m_defined_map.begin()];

    // Define variable
    bool value = disjunct > 0;
    var_num_t variable = get_num(disjunct);

    auto try_solve_with = [this](var_num_t var_code, bool var_val) {
      define_var(var_code, var_val);
      // propagate_var(var_code, var_val);
      return solve();
    };

    bool is_sat = try_solve_with(variable, value);

    if (!is_sat)
    {
      undefine_var(variable);
      is_sat = try_solve_with(variable, !value);
    }

    return is_sat;
  }

  // *begin is variable with number 1
  const_iterator answer_cbegin() { return std::next(m_answer.cbegin()); }
  const_iterator answer_cend() { return m_answer.cend(); }

  iterator answer_begin() { return std::next(m_answer.begin()); }
  iterator answer_end() { return m_answer.end(); }

private: // methods
  void invalidate_task(size_t num_vars)
  {
    m_defined_map.clear();
    m_defined_map.resize(m_cnf.size(), false);

    m_answer.clear();
    m_answer.resize(num_vars + 1, ValueEncoding::ANY);

    auto max_var = std::max_element(
        m_cnf.cbegin(), m_cnf.cend(),
        [](var_t lhs, var_t rhs) { return get_num(lhs) < get_num(rhs); });

    m_var_uses.clear();
    m_var_uses.resize(get_num(*max_var) + 1);

    for (size_t v_idx = 0; v_idx < m_cnf.size(); ++v_idx)
    {
      var_num_t var = get_num(m_cnf[v_idx]);
      m_var_uses[var].insert(v_idx);
    }
  }

  void set_all_uses(var_num_t var, bool value)
  {
    auto &var_idxs = m_var_uses[var];
    std::for_each(var_idxs.cbegin(), var_idxs.cend(),
                  [this, value](size_t idx) { m_defined_map[idx] = value; });
  }

  bool check_answer()
  {
    bool is_sat = true;
    bool conjunct = false;

    for (size_t dis_number = 0; dis_number < m_cnf.size(); dis_number++)
    {
      auto &&disjunct = m_cnf[dis_number];

      auto answer_val = m_answer[get_num(disjunct)];
      bool value = false;

      if (answer_val != ValueEncoding::ANY)
      {
        value = answer_val == ValueEncoding::TRUE ? true : false;
        if (disjunct < 0)
        {
          value = !value;
        }
      }

      conjunct = conjunct || value;

      if ((dis_number + 1) % m_conj_len == 0)
      {
        is_sat = is_sat && conjunct;
        conjunct = false;
      }
    }

    return is_sat;
  }

protected:
  void define_var(var_num_t variable, bool value)
  {
    m_answer[variable] = value ? ValueEncoding::TRUE : ValueEncoding::FALSE;
    set_all_uses(variable, true);
  }

  void undefine_var(var_num_t variable)
  {
    m_answer[variable] = ValueEncoding::ANY;
    set_all_uses(variable, false);
  }

  // TODO: exclude clauses that already satisfied
  // void propagate_var(var_num_t variable, bool value) {}
};

} // namespace Runtime
} // namespace SAT