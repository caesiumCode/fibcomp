#ifndef fibonacci_fmat_hpp
#define fibonacci_fmat_hpp

#include "fibonacci.hpp"


class FibonacciFmat : public Fibonacci
{
public:
    FibonacciFmat();
    
    uintinf_t run(uint64_t);
};

#endif /* fibonacci_fmat_hpp */
