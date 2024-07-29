#ifndef tomcook_hpp
#define tomcook_hpp

#include <iostream>

#include "uintinf_t.hpp"
#include "karatsuba.hpp"
#include "cmp.hpp"

#define MULT_TOMCOOK_CUTOFF 750
//#define MULT_TOMCOOK_CUTOFF 5

namespace tomcook
{

std::vector<uint64_t> mult(const std::vector<uint64_t>&, const std::vector<uint64_t>&);

}

#endif /* tomcook_hpp */
