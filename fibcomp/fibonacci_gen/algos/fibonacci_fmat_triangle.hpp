#ifndef fibonacci_fmat_triangle_hpp
#define fibonacci_fmat_triangle_hpp

#include <array>
#include <iostream>

#include "fibonacci.hpp"


class FibonacciFmatTriangle : public Fibonacci
{
public:
    FibonacciFmatTriangle();
    
    uintinf_t run(uint64_t);
    
private:
    std::array<uint64_t, 94> cache_small;
    
private:
    uint64_t rho_left (uint64_t);
    uint64_t rho_right(uint64_t);
};

#endif /* fibonacci_fmat_triangle_hpp */
