#include "tomcook.hpp"

std::vector<uint64_t> tomcook::mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return {0};
    if (x_len == 1) return gschool::mult_s(x[0], y);
    if (y_len == 1) return gschool::mult_s(y[0], x);
    if (x_len <= MULT_KARATSUBA_CUTOFF  && y_len <= MULT_KARATSUBA_CUTOFF)  return gschool::mult(x, y);
    if (x_len <= MULT_TOMCOOK_CUTOFF    && y_len <= MULT_TOMCOOK_CUTOFF)    return karatsuba::mult(x, y);
    
    std::size_t split = std::max(x_len, y_len) / 3;
    
    // p(t) = x_2 t^2 + x_1 t + x_0
    std::size_t x0_len = std::min(split, x_len);
    std::size_t x1_len = std::min(split, x_len - x0_len);
    std::size_t x2_len = x_len - x0_len - x1_len;
    
    std::vector<uint64_t> x_0(x.begin(),            x.begin() + x0_len);
    std::vector<uint64_t> x_1(x.begin() + x0_len,   x.begin() + x1_len);
    std::vector<uint64_t> x_2(x.begin() + x1_len,   x.end());

    // q(t) = y_2 t^2 + y_1 t + y_0
    std::size_t y0_len = std::min(split, y_len);
    std::size_t y1_len = std::min(split, y_len - y0_len);
    std::size_t y2_len = y_len - y0_len - y1_len;
    
    std::vector<uint64_t> y_0(y.begin(),            y.begin() + y0_len);
    std::vector<uint64_t> y_1(y.begin() + y0_len,   y.begin() + y1_len);
    std::vector<uint64_t> y_2(y.begin() + y1_len,   y.end());
    
    return {0};
}
