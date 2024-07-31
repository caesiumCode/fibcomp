#ifndef grade_school_hpp
#define grade_school_hpp

#include <iostream>

#include "bshift.hpp"
#include "cmp.hpp"

namespace gschool
{

std::vector<uint64_t>& add_r    (       std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t>& sub_r    (       std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t>  add      (       std::vector<uint64_t>,  const std::vector<uint64_t>&);
std::vector<uint64_t>  sub      (       std::vector<uint64_t>,  const std::vector<uint64_t>&);

std::vector<uint64_t>  sub_sgn  (const bool, const  std::vector<uint64_t>&, const bool, const std::vector<uint64_t>&, bool&);
std::vector<uint64_t>  add_sgn  (const bool, const  std::vector<uint64_t>&, const bool, const std::vector<uint64_t>&, bool&);

std::vector<uint64_t>  mult_s(uint64_t, std::vector<uint64_t>);
std::vector<uint64_t>  mult  (const std::vector<uint64_t>&, const std::vector<uint64_t>&);
std::vector<uint64_t>  square(const std::vector<uint64_t>&);

std::vector<uint64_t>& divide_by_3_r(std::vector<uint64_t>&);


/** WARNING **/
/** Assumes destination has enough space (even for overflow digit) and is filled with zeroes in the case of a pure destination  **/
void add_r (      uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len                );
void sub_r (      uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len                );
void sub_r2(const uint64_t* x, const std::size_t x_len,       uint64_t* y, const std::size_t y_len                );
void add   (const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest);
void sub   (const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest);

void add_r_sgn(bool&, uint64_t* x, const std::size_t x_len, const bool, const uint64_t* y, const std::size_t y_len);

void add_sgn(const bool, const uint64_t* x, const std::size_t x_len,
             const bool, const uint64_t* y, const std::size_t y_len,
                   bool&,      uint64_t* dest);

void mult_s(const uint64_t scalar,                      const uint64_t* y, const std::size_t y_len, uint64_t* dest);
void square(const uint64_t* x, const std::size_t x_len,                                             uint64_t* dest);
void mult  (const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest);

void divide_by_3_r(uint64_t* x, const std::size_t x_len);

/** specialisation for dealing with a splitted representation **/
void add(const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest, const std::size_t dest_len, uint64_t& c);
void sub(const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest, const std::size_t dest_len, uint64_t& c);
void add_sgn(const bool, const uint64_t* x,    const std::size_t x_len,
             const bool, const uint64_t* y,    const std::size_t y_len,
                   bool&,      uint64_t* dest, const std::size_t dest_len, uint64_t& c);

void add_r_sgn(     bool&,       uint64_t* x, const std::size_t x_len,      uint64_t& x_top,
               const bool, const uint64_t* y, const std::size_t y_len, const uint64_t y_top);
void add_r (      uint64_t* x, const std::size_t x_len,      uint64_t& x_top,
            const uint64_t* y, const std::size_t y_len, const uint64_t y_top);

void sub_r (      uint64_t* x, const std::size_t x_len,      uint64_t& x_top,
            const uint64_t* y, const std::size_t y_len, const uint64_t y_top);

void sub_r2(const uint64_t* x, const std::size_t x_len, const uint64_t x_top,
                  uint64_t* y, const std::size_t y_len,      uint64_t& y_top);
}

#endif /* grade_school_hpp */
