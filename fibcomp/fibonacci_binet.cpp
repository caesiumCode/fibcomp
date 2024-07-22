#include "fibonacci_binet.hpp"

FibonacciBinet::FibonacciBinet()
: Fibonacci("binet", false)
{
    
}

void BinetCouple::halving()
{
    a.halving();
    b.halving();
}

void BinetCouple::special_halving()
{
    uintinf_t tmp = half(half(a + b));
    
    a = tmp + b;
    b = tmp;
}

BinetCouple operator*(const BinetCouple& A, const BinetCouple& B)
{
    return {A.a*B.a + (uint64_t(5)*A.b)*B.b, A.a*B.b + A.b*B.a};
}

BinetCouple square(const BinetCouple& A)
{
    return {square(A.a) + uint64_t(5) * square(A.b), twice(A.a*A.b)};
}

BinetCouple binet_power(uint64_t n)
{
    if      (n == 0) return {1, 0};
    else if (n == 1) return {1, 1};
    else if (n == 2) return {3, 1};
    else
    {
        BinetCouple A = square(binet_power(n/2));
        
        if (n%2) A.special_halving();
        else     A.halving();
        
        return A;
    }
}

uintinf_t FibonacciBinet::run(uint64_t n)
{
    return binet_power(n).b;
}
