#ifndef fibonacci_lin_hpp
#define fibonacci_lin_hpp

#include "fibonacci.hpp"

class FibonacciLin : public Fibonacci
{
public:
    FibonacciLin() = default;
    
    uintinf_t run(uint64_t);
};

#endif /* fibonacci_lin_hpp */
