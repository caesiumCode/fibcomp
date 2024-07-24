#ifndef grade_school_hpp
#define grade_school_hpp

#include <stdint.h>
#include <vector>

namespace gschool
{

std::vector<uint64_t>& add_r(std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t>& sub_r(std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t>  add  (std::vector<uint64_t>,  const std::vector<uint64_t>&);
std::vector<uint64_t>  sub  (std::vector<uint64_t>,  const std::vector<uint64_t>&);

std::vector<uint64_t>  mult_s(uint64_t, std::vector<uint64_t>);
std::vector<uint64_t>  mult  (const std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t>  square(const std::vector<uint64_t>&);


/** WARNING **/
/** Assumes destination has enough space (even for overflow digit) and is filled with zeroes in the case of a pure destination  **/
void add_r (      uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len                );
void sub_r (      uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len                );
void add   (const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest);
void mult_s(const uint64_t scalar,                      const uint64_t* y, const std::size_t y_len, uint64_t* dest);
void square(const uint64_t* x, const std::size_t x_len,                                             uint64_t* dest);
void mult  (const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest);

}

#endif /* grade_school_hpp */
