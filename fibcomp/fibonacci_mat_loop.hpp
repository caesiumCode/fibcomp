#ifndef fibonacci_mat_loop_hpp
#define fibonacci_mat_loop_hpp

#include "fibonacci_mat.hpp"

class FibonacciMatLoop : public FibonacciMat
{
public:
    FibonacciMatLoop();
        
    FibMat power(const FibMat&, uint64_t);
};

#endif /* fibonacci_mat_loop_hpp */
