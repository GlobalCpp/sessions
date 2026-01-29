#include <omp.h>
#include <print>

int main() {
  std::println( "{}",omp_get_num_procs() );
  return 0;
}
