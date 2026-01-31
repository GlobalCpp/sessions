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

#define N 50'000
#define INDEX( i,j ) (i)*N + (j)
    std::vector<double> square(static_cast<long>(N)*N);
    double tstart = omp_get_wtime();
#pragma omp parallel for
    for ( int i=1; i<N-1; i++ ) {
      for ( int j=1; j<N-1; j++ ) {
	square[ INDEX(i,j) ] = 
	  ( square[ INDEX(i-1,j-1) ] + square[ INDEX(i+1,j+1) ] )/2;
      }
    }
    double duration = omp_get_wtime()-tstart;
    std::println( "time: {}",duration );

    //  std::println( "{}",v );

  return 0;
}
