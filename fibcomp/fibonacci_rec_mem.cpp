#include "fibonacci_rec_mem.hpp"

uintinf_t FibonacciRecMem::run(uint64_t n)
{
    std::vector<uintinf_t> cache(n+1, 0);
    cache[1] = uintinf_t(1);
    
    return run(n, cache);
}

uintinf_t FibonacciRecMem::run(uint64_t n, std::vector<uintinf_t>& cache)
{
    if (n == 0) return uintinf_t(0);
    else if (cache[n].is_zero()) cache[n] = run(n-1, cache) + run(n-2, cache);
    
    return cache[n];
}
