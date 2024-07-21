#include "fibonacci_mat_rec.hpp"

FibonacciMatRec::FibonacciMatRec()
: FibonacciMat("matrice-rec", false)
{
    
}

FibMat FibonacciMatRec::power(const FibMat& A, uint64_t n)
{
    if      (n == 0) return {1, 0, 1};
    else if (n == 1) return A;
    else             return (n & 1) ? square(power(A, n >> 1)) * A : square(power(A, n >> 1));
}
