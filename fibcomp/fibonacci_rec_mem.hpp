#ifndef fibonacci_rec_mem_hpp
#define fibonacci_rec_mem_hpp

#include "fibonacci.hpp"

class FibonacciRecMem : public Fibonacci
{
public:
    FibonacciRecMem() = default;
    
    uintinf_t run(uint64_t);
    
private:
    uintinf_t run(uint64_t, std::vector<uintinf_t>&);
};

#endif /* fibonacci_rec_mem_hpp */
