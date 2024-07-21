#ifndef fibonacci_hpp
#define fibonacci_hpp

#include "uintinf_t.hpp"

class Fibonacci
{
public:
    Fibonacci(std::string, bool);
    
    virtual uintinf_t run(uint64_t) = 0;
    
public:
    const std::string   name;
    const bool          slow;
};

#endif /* fibonacci_hpp */
