module;

#include <array>
#include <stddef.h>

export module CNF;

// Don't work for some reason
// import <array>;

namespace SAT
{

using Conjuct = std::array<int, 3>;

template <size_t NumConj> struct CNF
{
  using type = std::array<Conjuct, NumConj>;
};

template <size_t NumConj> struct SATTest
{
  using cnf_type = typename CNF<NumConj>::type;

  cnf_type cnf;
  bool is_sat;

  consteval SATTest(cnf_type cnf_, bool is_sat_) : cnf(cnf_), is_sat(is_sat_) {}
};

namespace Test {

export SATTest<2> sat_test{{{{3, 3, 2}, {2, -1, 2}}}, true};

export SATTest<4> unsat_test{
    {{{1, 2, 3}, {9, -6, -1}, {-9, -8, -2}, {-4, -6, -3}}}, false};

}

} // namespace SAT