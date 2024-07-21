#include "fibonacci_lin.hpp"

FibonacciLin::FibonacciLin()
: Fibonacci("lineaire", false)
{
    
}

uintinf_t FibonacciLin::run(uint64_t n)
{
    uintinf_t F0(0);
    uintinf_t F1(1);
    
    for (uint64_t k = 1; k < n; k++)
    {
        swap(F0, F1);
        F1 += F0;
    }
    
    return (n == 0 ? uintinf_t(0) : F1);
}
