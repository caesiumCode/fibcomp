#include "fibonacci_fmat.hpp"

FibonacciFmat::FibonacciFmat()
: Fibonacci("flatmatrice-naif", false)
{
    
}

uintinf_t FibonacciFmat::run(uint64_t n)
{
    if      (n <= 93) return run_small(n);
    else if (n%2)    
    {
        uintinf_t F1 = run((n+1)/2);
        uintinf_t F2 = run((n-1)/2);
        return square(F1) + square(F2);
    }
    else
    {
        uintinf_t Fn = run(n/2);
        return (twice(run(n/2 - 1)) + Fn) * Fn;
    }
}


uint64_t FibonacciFmat::run_small(uint64_t n)
{
    if      (n <= 1) return n;
    else if (n <= 4) return n-1;
    else if (n%2)    
    {
        uint64_t F1 = run_small((n+1)/2);
        F1 *= F1;
        uint64_t F2 = run_small((n-1)/2);
        F2 *= F2;
        
        return F1 + F2;
    }
    else
    {
        uint64_t Fn = run_small(n/2);
        
        return (2*(run_small(n/2 - 1)) + Fn) * Fn;
    }
}
