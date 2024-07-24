#include "fibonacci_fmat_mem_hit.hpp"

FibonacciFmatMemHit::FibonacciFmatMemHit()
: Fibonacci("flatmatrice-mem-hit", false)
{
    cache_small[0] = 0;
    cache_small[1] = 1;
    for (std::size_t i = 2; i < 94; i++) cache_small[i] = cache_small[i-1] + cache_small[i-2];
}

uintinf_t FibonacciFmatMemHit::run(uint64_t n)
{
    std::unordered_map<uint64_t, uintinf_t> cache;
    
    return run(n, cache, 0);;
}


uintinf_t FibonacciFmatMemHit::run(uint64_t n, std::unordered_map<uint64_t, uintinf_t>& cache, int pos)
{
    if (n <= 93) return cache_small[n];
    
    auto iter = cache.find(n);
    
    if (iter != cache.end())
        return iter->second;
    else if (n%2)
        return cache[n] = square(run((n-1)/2, cache, pos)) + square(run((n+1)/2, cache, pos+1));
    else if (pos >= 0)
    {
        uintinf_t Fn = run(n/2, cache, pos);
        return cache[n] = (twice(run((n-1)/2, cache, pos-1)) + Fn) * Fn;
    }
    else
    {
        uintinf_t Fn = run(n/2, cache, pos);
        return cache[n] = (twice(run((n+1)/2, cache, pos+1)) - Fn) * Fn;
    }
}
