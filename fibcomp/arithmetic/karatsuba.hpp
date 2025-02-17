#ifndef karatsuba_hpp
#define karatsuba_hpp

#include <iostream>

#include "grade_school.hpp"

#define MULT_KARATSUBA_CUTOFF 50
//#define MULT_KARATSUBA_CUTOFF 4 // WARNING - 4 is the minimum legal value
#define SQR_KARATSUBA_CUTOFF 75
//#define SQR_KARATSUBA_CUTOFF 4 // WARNING - 4 is the minimum legal value

namespace karatsuba_old
{

std::vector<uint64_t>  mult  (const std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t>  square(const std::vector<uint64_t>&);

}

namespace karatsuba
{

std::vector<uint64_t>  mult  (const std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t>  square(const std::vector<uint64_t>&);

/** WARNING **/
/** Assumes destination has enough space (even for overflow digit) and is filled with zeroes in the case of a pure destination  **/
void square(const uint64_t* x, const std::size_t x_len,                                             uint64_t* dest);
void mult  (const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest);

}

#endif /* karatsuba_hpp */
