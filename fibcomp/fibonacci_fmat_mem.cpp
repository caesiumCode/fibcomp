#include "fibonacci_fmat_mem.hpp"

FibonacciFmatMem::FibonacciFmatMem()
: Fibonacci("flatmatrice-mem", false)
{
    
}

uintinf_t FibonacciFmatMem::run(uint64_t n)
{
    std::unordered_map<uint64_t, uintinf_t> cache;
    cache[0] = uintinf_t(0);
    cache[1] = uintinf_t(1);
    cache[2] = uintinf_t(1);
    cache[3] = uintinf_t(2);
    cache[4] = uintinf_t(3);
    cache[5] = uintinf_t(5);
    cache[6] = uintinf_t(8);
    cache[7] = uintinf_t(13);
    cache[8] = uintinf_t(21);
    cache[9] = uintinf_t(34);
    cache[10] = uintinf_t(55);
    cache[11] = uintinf_t(89);
    cache[12] = uintinf_t(144);
    cache[13] = uintinf_t(233);
    cache[14] = uintinf_t(377);
    cache[15] = uintinf_t(610);
    
    return run(n, cache);
}


uintinf_t FibonacciFmatMem::run(uint64_t n, std::unordered_map<uint64_t, uintinf_t>& cache)
{
    auto iter = cache.find(n);
    
    if (iter != cache.end()) 
        return iter->second;
    else if (n%2) 
        return cache[n] = square(run((n+1)/2, cache)) + square(run((n-1)/2, cache));
    else
    {
        uintinf_t Fn = run(n/2, cache);
        return cache[n] = (twice(run(n/2 - 1, cache)) + Fn) * Fn;
    }
}
