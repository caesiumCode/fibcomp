#ifndef fibonacci_hpp
#define fibonacci_hpp

#include "uintinf_t.hpp"

class Fibonacci
{
public:
    Fibonacci() = default;
    
    virtual uintinf_t run(uint64_t) = 0;
};

#endif /* fibonacci_hpp */
