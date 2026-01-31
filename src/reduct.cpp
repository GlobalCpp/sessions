#include <print>
#include <vector>

#include "omp.h"

class Number {
private:
  double x;
public:
  Number() = default;
  Number( double x )
    : x(x) {};
  void operator+=( const Number& n ) {
    x += n.x; 
  };
  Number operator+( const Number& n ) {
    return Number( x+n.x );
  };
  void print() {
    std::println("{}",x); };
};

#pragma omp declare reduction(+: Number : omp_out += omp_in) \
                    initializer(omp_priv = Number{})

int main() {

  std::vector<Number> numbers(1000,1.);
  Number sum(0);

#pragma omp parallel for reduction(+:sum)
  for ( const auto& t : numbers )
    sum =  sum+t;

  sum.print();

  return 0;
}
