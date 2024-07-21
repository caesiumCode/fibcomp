#include "fibonacci_rec.hpp"

FibonacciRec::FibonacciRec()
: Fibonacci("recursif-naif", true)
{
    
}

uintinf_t FibonacciRec::run(uint64_t n)
{
    if      (n <= 1) return n;
    else if (n <= 4) return n-1;
    else             return run(n-1)+run(n-2);
}
