#include "fibonacci_mat.hpp"

FibMat operator*(FibMat A, const FibMat& B)
{
    uintinf_t p_01_01 = A.e01 * B.e01;
    
    A.e11 = A.e11 * B.e11;
    A.e11 += p_01_01;
    
    A.e01 = A.e01 * B.e11;
    A.e01 += A.e00*B.e01;
    
    A.e00 = A.e00 * B.e00;
    A.e00 += p_01_01;
    
    return A;
}

FibMat square(const FibMat& A)
{
    FibMat B;
    uintinf_t p_01_01 = A.e01 * A.e01;
    
    B.e00 = A.e00*A.e00 + p_01_01;
    B.e01 = A.e00*A.e01 + A.e01*A.e11;
    B.e11 = p_01_01     + A.e11*A.e11;
    return B;
}

FibMat power(const FibMat& A, uint64_t n)
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

uintinf_t FibonacciMat::run(uint64_t n)
{
    if (n == 0) return uintinf_t(0);
    
    FibMat A = power({1, 1, 0}, n-1);
    
    return A.e00;
}
