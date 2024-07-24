#ifndef fibonacci_binet_hpp
#define fibonacci_binet_hpp

#include "fibonacci.hpp"

struct BinetCouple;

class FibonacciBinet : public Fibonacci
{
public:
    FibonacciBinet();
    
    uintinf_t run(uint64_t);
};

struct BinetCoupleSmall
{
    uint64_t a;
    uint64_t b;
    
    friend BinetCoupleSmall binet_power_small(uint64_t);
};

struct BinetCouple
{
    BinetCouple(BinetCoupleSmall);
    
    uintinf_t a;
    uintinf_t b;
    
    void halving();
    void special_halving();
    
    friend BinetCouple operator*(BinetCouple, const BinetCouple&);
    friend BinetCouple square(BinetCouple);
    friend BinetCouple binet_power(uint64_t);
};


#endif /* fibonacci_binet_hpp */
