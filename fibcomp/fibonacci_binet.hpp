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

struct BinetCouple
{
    uintinf_t a;
    uintinf_t b;
    
    void halving();
    void special_halving();
    
    friend BinetCouple operator*(const BinetCouple&, const BinetCouple&);
    friend BinetCouple square(const BinetCouple&);
    friend BinetCouple binet_power(uint64_t);
};


#endif /* fibonacci_binet_hpp */
