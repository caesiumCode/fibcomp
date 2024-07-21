#include "fibonacci_mat_loop.hpp"

FibonacciMatLoop::FibonacciMatLoop()
: FibonacciMat("matrice-loop", false)
{
    
}

FibMat FibonacciMatLoop::power(const FibMat& A, uint64_t n)
{
    if      (n == 0) return {1, 0, 1};
    else if (n == 1) return A;
    else
    {
        uint64_t mask = uint64_t(1) << 63;
        while ((n & mask) == 0) mask >>= 1;
        mask >>= 1;
        
        FibMat B = A;
        while (mask > 0)
        {
            B = (n & mask) ? square(B) * A : square(B);
            
            mask >>= 1;
        }
        
        return B;
    }
}
