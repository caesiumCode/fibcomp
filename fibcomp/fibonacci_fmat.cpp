#include "fibonacci_fmat.hpp"

FibonacciFmat::FibonacciFmat()
: Fibonacci("flatmatrice-naif", false)
{
    
}

uintinf_t FibonacciFmat::run(uint64_t n)
{
    if      (n <= 1) return uintinf_t(n);
    else if (n <= 4) return uintinf_t(n-1);
    else if (n%2)    return square(run((n+1)/2)) + square(run((n-1)/2));
    else
    {
        uintinf_t Fn = run(n/2);
        return (doubling(run(n/2 - 1)) + Fn) * Fn;
    }
}
