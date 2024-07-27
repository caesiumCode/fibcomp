#ifndef tomcook_hpp
#define tomcook_hpp

#include "karatsuba.hpp"

#define MULT_TOMCOOK_CUTOFF 2

namespace tomcook
{

std::vector<uint64_t> mult(const std::vector<uint64_t>&, const std::vector<uint64_t>&);

}

#endif /* tomcook_hpp */
