#ifndef bshift_hpp
#define bshift_hpp

#include <stdint.h>
#include <vector>

namespace bshift
{

void times4_r (std::vector<uint64_t>&);
void times2_r (std::vector<uint64_t>&);
void divide2_r(std::vector<uint64_t>&);
void divide4_r(std::vector<uint64_t>&);

std::vector<uint64_t> times2(std::vector<uint64_t>);

void multiply_by_2_r(uint64_t*, const std::size_t);
void divide_by_2_r(uint64_t*, const std::size_t);

}

#endif /* bshift_hpp */
