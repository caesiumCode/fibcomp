#ifndef fibonacci_fmat_mem_hpp
#define fibonacci_fmat_mem_hpp

#include <unordered_map>

#include "fibonacci.hpp"


class FibonacciFmatMem : public Fibonacci
{
public:
    FibonacciFmatMem();
    
    uintinf_t run(uint64_t);
    
private:
    uintinf_t run(uint64_t, std::unordered_map<uint64_t, uintinf_t>&);
};

#endif /* fibonacci_fmat_mem_hpp */
