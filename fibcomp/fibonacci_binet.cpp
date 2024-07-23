#include "fibonacci_binet.hpp"

FibonacciBinet::FibonacciBinet()
: Fibonacci("binet", false)
{
    
}

BinetCouple::BinetCouple(BinetCoupleSmall binet)
: a(binet.a)
, b(binet.b)
{
    
}

void BinetCouple::halving()
{
    a.halving();
    b.halving();
}

void BinetCouple::special_halving()
{
    uintinf_t tmp = a + b;
    tmp.fourthing();
    
    a = tmp + b;
    b = tmp;
}

BinetCouple operator*(BinetCouple A, const BinetCouple& B)
{
    uintinf_t AbBb = A.b*B.b;
    uintinf_t z    = A.a*B.a;
    z += AbBb;
    
    A.b  = (A.a + A.b)*(B.a + B.b);
    A.b -= z;
    
    AbBb.quadrupling();
    
    A.a = z + AbBb;
    
    return A;
}

BinetCouple square(BinetCouple A)
{
    uintinf_t b2 = square(A.b);
    uintinf_t z = square(A.a);
    z += b2;
    
    A.b  = square(A.a + A.b);
    A.b -= z;
    
    b2.quadrupling();
    
    A.a = z + b2;
    
    return A;
}

BinetCoupleSmall binet_power_small(uint64_t n)
{
    if      (n == 0) return {1, 0};
    else if (n == 1) return {1, 1};
    else if (n == 2) return {3, 1};
    else
    {
        BinetCoupleSmall A = binet_power_small(n/2);
        A = {A.a*A.a + 5*A.b*A.b, 2*A.a*A.b};
        
        if (n%2)
        {
            uint64_t tmp = (A.a + A.b) >> 2;
            
            A.a = tmp + A.b;
            A.b = tmp;
        }
        else
        {
            A.a >>= 1;
            A.b >>= 1;
        }
        
        return A;
    }
}

BinetCouple binet_power(uint64_t n)
{
    if (n <= 89) return binet_power_small(n);
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
