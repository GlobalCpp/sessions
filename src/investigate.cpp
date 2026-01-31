#include <print>
#include <vector>

// include openmp library
#include <omp.h>

int main() {

  auto nprocs = omp_get_num_procs();
  std::println( "{}",nprocs );

#pragma omp parallel
  {
    auto nthreads = omp_get_num_threads(),
      tnum = omp_get_thread_num();
    if (tnum==0) 
      std::println( "{}",nthreads );
  }
  
  std::vector<double> v(10,2);
#pragma omp parallel for 
    for ( auto& x : v )
      x = x*x;

  std::println( "{}",v );

  return 0;
}
