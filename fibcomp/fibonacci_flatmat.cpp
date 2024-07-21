#include "fibonacci_flatmat.hpp"

FibonacciFlatmat::FibonacciFlatmat()
: Fibonacci("flatmatrice", false)
{
    
}

uintinf_t FibonacciFlatmat::run(uint64_t n)
{
    if      (n == 0) return 0;
    else if (n == 1) return 1;
    else if (n%2)    return square(run((n+1)/2)) + square(run((n+1)/2 - 1));
    else
    {
        uintinf_t Fn = run(n/2);
        return ( uintinf_t(2) * run(n/2 - 1) + Fn) * Fn;
    }
}
