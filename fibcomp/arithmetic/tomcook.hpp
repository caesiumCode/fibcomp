#ifndef tomcook_hpp
#define tomcook_hpp

#include <iostream>

#include "uintinf_t.hpp"
#include "karatsuba.hpp"
#include "cmp.hpp"

#define MULT_TOMCOOK_CUTOFF 750
//#define MULT_TOMCOOK_CUTOFF 5
#define SQR_TOMCOOK_CUTOFF 750

namespace tomcook_old
{

std::vector<uint64_t> mult(const std::vector<uint64_t>&, const std::vector<uint64_t>&);

}

namespace tomcook
{

std::vector<uint64_t> mult(const std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t> square(const std::vector<uint64_t>&);

void mult(const uint64_t*, const std::size_t, const uint64_t*, const std::size_t, uint64_t*);
void square(const uint64_t*, const std::size_t, uint64_t*);

}

#endif /* tomcook_hpp */
