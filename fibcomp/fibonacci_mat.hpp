#ifndef fibonacci_mat_hpp
#define fibonacci_mat_hpp

#include "fibonacci.hpp"

class FibonacciMat : public Fibonacci
{
public:
    FibonacciMat() = default;
    
    uintinf_t run(uint64_t);
};

struct FibMat
{
    uintinf_t e00 = 0;
    uintinf_t e01 = 0;
    uintinf_t e11 = 0;
    
    friend FibMat operator*(FibMat, const FibMat&);
    friend FibMat square(const FibMat&);
    friend FibMat power(const FibMat&, uint64_t);
};

#endif /* fibonacci_mat_hpp */
