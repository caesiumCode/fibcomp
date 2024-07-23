#ifndef fibonacci_fmat_mem_hit_hpp
#define fibonacci_fmat_mem_hit_hpp

#include <unordered_map>
#include <array>

#include "fibonacci.hpp"


class FibonacciFmatMemHit : public Fibonacci
{
public:
    FibonacciFmatMemHit();
    
    uintinf_t run(uint64_t);
    
private:
    std::array<uint64_t, 94> cache_small;
    
private:
    uintinf_t run(uint64_t, std::unordered_map<uint64_t, uintinf_t>&, int);
};

#endif /* fibonacci_fmat_mem_hit_hpp */
