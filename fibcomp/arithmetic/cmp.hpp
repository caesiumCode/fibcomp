#ifndef cmp_hpp
#define cmp_hpp

#include <stdint.h>
#include <vector>

namespace cmp
{

void trim(std::vector<uint64_t>&);

bool less_than(const std::vector<uint64_t>&, const std::vector<uint64_t>&);

bool less_than(const uint64_t*, const std::size_t, const uint64_t*, const std::size_t);
bool less_than(const uint64_t*, const std::size_t, const uint64_t, const uint64_t*, const std::size_t, const uint64_t);

}

#endif /* cmp_hpp */
