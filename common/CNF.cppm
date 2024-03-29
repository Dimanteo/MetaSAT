module;

#include <array>
#include <cstdlib>
#include <stddef.h>
#include <concepts>

export module CNF;

// Don't work for some reason
// import <array>;

namespace SAT
{

// Variable encoding type. Negative value means negation
//
export using var_t = long int;
export using var_num_t = unsigned long int;
export constexpr var_num_t get_num(var_t v) { return v >= 0 ? v : -v; }

export enum ValueEncoding : int
{
  TRUE = 1,
  FALSE = 0,
  ANY = -1
};

export template <size_t ConjNum, size_t ConjLen, size_t NumVars> class CNF
{
public:
  using Conjunct = std::array<var_t, ConjLen>;
  using CNFType = std::array<Conjunct, ConjLen>;

  using iterator = CNFType::iterator;
  using const_iterator = CNFType::const_iterator;

private:
  std::array<Conjunct, ConjNum> m_conjuncts;

public:
  constexpr CNF(const std::array<var_t, ConjNum * ConjLen> &cnf)
  {
    size_t idx = 0;
    for (auto &&var : cnf)
    {
      m_conjuncts[idx / ConjLen][idx % ConjLen] = var;
      idx++;
    }
  }

  constexpr static size_t get_num_vars() { return NumVars; }
  constexpr static size_t get_conj_len() { return ConjLen; }
  constexpr static size_t get_conj_num() { return ConjNum; }
  constexpr iterator begin() { return m_conjuncts.begin(); }
  constexpr iterator end() { return m_conjuncts.end(); }
  constexpr const_iterator begin() const { return m_conjuncts.begin(); }
  constexpr const_iterator end() const { return m_conjuncts.end(); }

  template <std::random_access_iterator SolutionIt>
  requires requires (SolutionIt x) { {*x} -> std::convertible_to<ValueEncoding>; }
  constexpr bool eval(SolutionIt solution_begin) const
  {
    bool result = true;
    for (auto &&conj : m_conjuncts)
    {
      bool conj_value = false;
      for (var_t disjunct : conj)
      {
        ValueEncoding sol_val = *(solution_begin + get_num(disjunct) - 1);
        bool value = false;
        if (sol_val != ValueEncoding::ANY)
        {
          value = static_cast<bool>(sol_val);
          if (disjunct < 0)
          {
            value = !value;
          }
        }
        conj_value = conj_value || value;
      }
      result = result && conj_value;
    }

    return result;
  }
};

namespace Test
{

export template <typename CNF> struct SATTest
{
  CNF cnf;
  bool is_sat;

  template <size_t ArraySize>
  constexpr SATTest(bool is_sat_, std::array<var_t, ArraySize> arr)
      : cnf(arr), is_sat(is_sat_)
  {}
};

} // namespace Test

} // namespace SAT