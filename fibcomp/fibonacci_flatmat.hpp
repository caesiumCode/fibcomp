#ifndef fibonacci_flatmat_hpp
#define fibonacci_flatmat_hpp

#include "fibonacci.hpp"


class FibonacciFlatmat : public Fibonacci
{
public:
    FibonacciFlatmat();
    
    uintinf_t run(uint64_t);    
};

#endif /* fibonacci_flatmat_hpp */
