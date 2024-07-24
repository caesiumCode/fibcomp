#ifndef fibonacci_mat_hpp
#define fibonacci_mat_hpp

#include "fibonacci.hpp"

struct FibMat;

class FibonacciMat : public Fibonacci
{
public:
    FibonacciMat(std::string, bool);
    
    uintinf_t run(uint64_t);
    
    virtual FibMat power(const FibMat&, uint64_t) = 0;
};

struct FibMat
{
    uintinf_t e00 = 0;
    uintinf_t e01 = 0;
    uintinf_t e11 = 0;
    
    friend FibMat operator*(FibMat, const FibMat&);
    friend FibMat square(FibMat);
};

#endif /* fibonacci_mat_hpp */
