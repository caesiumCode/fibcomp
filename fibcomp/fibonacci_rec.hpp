#ifndef fibonacci_rec_hpp
#define fibonacci_rec_hpp

#include "fibonacci.hpp"

class FibonacciRec : public Fibonacci
{
public:
    FibonacciRec();
    
    uintinf_t run(uint64_t);
};

#endif /* fibonacci_rec_hpp */
