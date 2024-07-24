#include "fibonacci_mat.hpp"

FibonacciMat::FibonacciMat(std::string name, bool slow)
: Fibonacci(name, slow)
{
    
}

FibMat operator*(FibMat A, const FibMat& B)
{
    uintinf_t p_01_01 = A.e01 * B.e01;
    
    A.e11 = A.e11 * B.e11 + p_01_01;
    A.e01 = A.e01 * B.e11 + A.e00*B.e01;
    A.e00 = A.e00 * B.e00 + p_01_01;
    
    return A;
}

FibMat square(FibMat A)
{
    uintinf_t p_01_01 = A.e01 * A.e01;
    
    A.e01 = A.e01 * ( A.e00 + A.e11 );
    A.e00 = A.e00 * A.e00 + p_01_01;
    A.e11 = A.e11 * A.e11 + p_01_01;
    
    return A;
}

uintinf_t FibonacciMat::run(uint64_t n)
{
    if (n == 0) return uintinf_t(0);
    
    FibMat A = power({1, 1, 0}, n-1);
    
    return A.e00;
}
