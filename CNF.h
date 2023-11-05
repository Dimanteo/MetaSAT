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
