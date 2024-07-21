#ifndef fibonacci_mat_rec_hpp
#define fibonacci_mat_rec_hpp

#include "fibonacci_mat.hpp"

class FibonacciMatRec : public FibonacciMat
{
public:
    FibonacciMatRec();
        
    FibMat power(const FibMat&, uint64_t);
};

#endif /* fibonacci_mat_rec_hpp */
