import CNF;

#include <iostream>

int main()
{
  for (auto &&conj : SAT::Test::sat_test.cnf)
  {
    std::cout << "(";
    for (auto &&var : conj)
    {
      std::cout << var << " ";
    }
    std::cout << ") ";
  }

  std::cout << "\n";

  return 0;
}