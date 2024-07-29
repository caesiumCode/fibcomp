#include "grade_school.hpp"

std::vector<uint64_t>& gschool::add_r(std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    const std::size_t x_len = x.size();
    const std::size_t y_len = y.size();
    const std::size_t len   = std::max(x_len, y_len) + 1;
    
    x.resize(len, 0);
    
    std::size_t i = 0;
    uint64_t carry = 0;
    for (i = 0; i < y_len; i++)
    {
        uint64_t plus = y[i] + carry;
        x[i] += plus;
        
        carry = (carry ? x[i] <= y[i] : x[i] < y[i]);
    }
    
    for (; i < x_len && carry; i++)
    {
        x[i] += carry;
        
        carry = (x[i] < carry);
    }
    
    x[i] += carry;
    if (x.back() == 0) x.pop_back();
    
    return x;
}

std::vector<uint64_t>& gschool::sub_r(std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    const std::size_t x_len = x.size();
    const std::size_t y_len = y.size();
    std::size_t last_idx = 0;
    
    std::size_t i = 0;
    uint64_t borrow = 0;
    for (i = 0; i < y_len; i++)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x[i] <= y[i] : x[i] < y[i]);
        x[i] -= minus;
        
        if (x[i] != 0) last_idx = i;
    }
    
    for (; i < x_len; i++)
    {
        x[i] -= borrow;
        
        if (x[i] != 0) last_idx = i;
        
        borrow = borrow && (x[i] == UINT64_MAX);
    }
    
    x.resize(last_idx + 1);
    
    return x;
}

std::vector<uint64_t> gschool::add(std::vector<uint64_t> x, const std::vector<uint64_t>& y)
{
    return gschool::add_r(x, y);
}

std::vector<uint64_t> gschool::sub(std::vector<uint64_t> x, const std::vector<uint64_t>& y)
{
    return gschool::sub_r(x, y);
}

std::vector<uint64_t> gschool::add_sgn(const bool x_sgn, const  std::vector<uint64_t>& x, const bool y_sgn, const std::vector<uint64_t>& y, bool& sgn)
{
    if (x_sgn == y_sgn) // x+y or -x-y=-(x+y)
    {
        sgn = x_sgn;
        return add(x, y);
    }
    else if (x_sgn && !y_sgn) // -x+y=y-x
    {
        sgn = cmp::less_than(y, x);
        if (sgn) return sub(x, y);
        else     return sub(y, x);
    }
    else // x-y
    {
        sgn = cmp::less_than(x, y);
        if (sgn) return sub(y, x);
        else     return sub(x, y);
    }
}

std::vector<uint64_t> gschool::sub_sgn(const bool x_sgn, const std::vector<uint64_t>& x, const bool y_sgn, const std::vector<uint64_t>& y, bool& sgn)
{
    if (x_sgn != y_sgn) // x-(-y)=x+y or -x-y=-(x+y)
    {
        sgn = x_sgn;
        return add(x, y);
    }
    else if (!x_sgn && !y_sgn) // x-y
    {
        sgn = cmp::less_than(x, y);
        if (sgn) return sub(y, x);
        else     return sub(x, y);
    }
    else // -x-(-y)=-x+y=y-x
    {
        sgn = cmp::less_than(y, x);
        if (sgn) return sub(x, y);
        else     return sub(y, x);
    }
}


std::vector<uint64_t> gschool::mult_s(uint64_t scalar, std::vector<uint64_t> x)
{
    const std::size_t x_len = x.size();
    
    if (scalar == 0 || x_len == 0 || (x_len == 1 && x[0] == 0)) return {0};
    
    uint64_t carry = 0;
    for (std::size_t i = 0; i < x_len; i++)
    {
        __uint128_t overflow = __uint128_t(scalar) * __uint128_t(x[i]) + __uint128_t(carry);
        
        x[i]  = (overflow &  UINT64_MAX);
        carry = (overflow >> 64);
    }
    
    if (carry) x.push_back(carry);
    
    return x;
}

std::vector<uint64_t> gschool::mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    const std::size_t x_len = x.size();
    const std::size_t y_len = y.size();
    
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return {0};
    
    std::vector<uint64_t> z(x_len + y_len, 0);
    
    uint64_t carry = 0;
    for (std::size_t j = 0; j < y_len; j++)
    {
        carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(z[i+j]) + __uint128_t(x[i]) * __uint128_t(y[j]) + __uint128_t(carry);
            
            z[i+j] = (overflow &  UINT64_MAX);
            carry  = (overflow >> 64);
        }
        z[x_len+j] = carry;
    }
    
    if (carry == 0) z.pop_back();
    
    return z;
}

std::vector<uint64_t> gschool::square(const std::vector<uint64_t>& x)
{
    const std::size_t x_len = x.size();
    
    if (x_len == 0 || (x_len == 1 && x[0] == 0)) return {0};
    
    std::size_t z_len = 2*x_len;
    std::vector<uint64_t> z(z_len, 0);
    
    // z = sum_{j < i} x_i x_j B^{i+j}
    __uint128_t carry = 0;
    for (std::size_t i = 0; i < x_len; i++)
    {
        carry = 0;
        for (std::size_t j = 0; j < i; j++)
        {
            __uint128_t overflow = __uint128_t(z[i+j]) + __uint128_t(x[i]) * __uint128_t(x[j]) + carry;
            
            z[i+j] = (overflow &  UINT64_MAX);
            carry  = (overflow >> 64);
        }
        z[2*i] = carry;
    }
    
    // z = 2 sum_{j < i} x_i x_j B^{i+j}
    for (std::size_t i = z_len-1; i > 0; i--)
    {
        z[i] <<= 1;
        z[i] |= (z[i-1] >> 63);
    }
    z[0] <<= 1;
    
    // z = sum_{i} x_i ^ 2 B^{2i} + 2 sum_{j < i} x_i x_j B^{i+j} = (sum_{i} x_i B^i)^2
    carry = 0;
    for (std::size_t i = 0; i < x_len ; i++)
    {
        __uint128_t overflow = __uint128_t(z[2*i]) + __uint128_t(x[i]) * __uint128_t(x[i]) + carry;
        z[2*i] = (overflow &  UINT64_MAX);
        carry  = (overflow >> 64);
        
        overflow = __uint128_t(z[2*i+1]) + carry;
        z[2*i+1] = (overflow &  UINT64_MAX);
        carry    = (overflow >> 64);
    }
    
    if (z.back() == 0) z.pop_back();
    return z;
}

// From GMP implementation (https://ftp.eecs.berkeley.edu/~bh/61a-pages/Scheme/source/src/stk/Gmp/mpn-diveby3.c)
#define INVERSE_3 ((UINT64_MAX / 3) * 2 + 1)
std::vector<uint64_t>& gschool::divide_by_3_r(std::vector<uint64_t>& x)
{
    uint64_t carry = 0;
    for (std::size_t i = 0; i < x.size(); i++)
    {
        uint64_t s = x[i];
        x[i] -= carry;
        carry = (x[i] > s);
        
        x[i] *= INVERSE_3;
        
        carry += (x[i] > UINT64_MAX/3);
        carry += (x[i] > (UINT64_MAX/3)*2);
    }
    
    if (x.back() == 0) x.pop_back();
    
    return x;
}

/** WARNING **/
/** Assumes destination has enough space (even for overflow digit) and is filled with zeroes in the case of a pure destination  **/

void gschool::add_r(uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len)
{
    std::size_t i = 0;
    uint64_t carry = 0;
    for (i = 0; i < y_len && i < x_len; i++)
    {
        uint64_t plus = y[i] + carry;
        x[i] += plus;
        
        carry = (carry ? x[i] <= y[i] : x[i] < y[i]);
    }
    
    for (; i < x_len && carry; i++)
    {
        x[i] += carry;
        
        carry = (x[i] < carry);
    }    
}

// Assumes x >= y
void gschool::sub_r(uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len)
{
    std::size_t i = 0;
    uint64_t borrow = 0;
    for (i = 0; i < y_len && i < x_len; i++)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x[i] <= y[i] : x[i] < y[i]);
        x[i] -= minus;
    }
    
    for (; i < x_len && borrow; i++)
    {
        x[i] -= borrow;
                
        borrow = borrow && (x[i] == UINT64_MAX);
    }
}

void gschool::add(const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest)
{
    if (x_len < y_len) add(y, y_len, x, x_len, dest);
    else
    {
        std::size_t i = 0;
        uint64_t carry = 0;
        for (i = 0; i < y_len; i++)
        {
            uint64_t plus = x[i] + y[i] + carry;
            dest[i] = plus;
            
            carry = (carry ? dest[i] <= y[i] : dest[i] < y[i]);
        }
        
        for (; i < x_len; i++)
        {
            dest[i] = x[i] + carry;
            
            carry = (dest[i] < carry);
        }
        
        dest[i] = carry;
    }
}

void gschool::mult_s(const uint64_t scalar, const uint64_t* x, const std::size_t x_len, uint64_t* dest)
{
    std::size_t i = 0;
    uint64_t carry = 0;
    for (i = 0; i < x_len; i++)
    {
        __uint128_t overflow = __uint128_t(scalar) * __uint128_t(x[i]) + __uint128_t(carry);
        
        dest[i] = (overflow &  UINT64_MAX);
        carry   = (overflow >> 64);
    }
    
    dest[i] = carry;
}

void gschool::square(const uint64_t* x, const std::size_t x_len, uint64_t* dest)
{
    // z = sum_{j < i} x_i x_j B^{i+j}
    __uint128_t carry = 0;
    for (std::size_t i = 0; i < x_len; i++)
    {
        carry = 0;
        for (std::size_t j = 0; j < i; j++)
        {
            __uint128_t overflow = __uint128_t(dest[i+j]) + __uint128_t(x[i]) * __uint128_t(x[j]) + carry;
            
            dest[i+j] = (overflow &  UINT64_MAX);
            carry     = (overflow >> 64);
        }
        dest[2*i] = carry;
    }
    
    // z = 2 sum_{j < i} x_i x_j B^{i+j}
    for (std::size_t i = 2*x_len-1; i > 0; i--)
    {
        dest[i] <<= 1;
        dest[i] |= (dest[i-1] >> 63);
    }
    dest[0] <<= 1;
    
    // z = sum_{i} x_i ^ 2 B^{2i} + 2 sum_{j < i} x_i x_j B^{i+j} = (sum_{i} x_i B^i)^2
    carry = 0;
    for (std::size_t i = 0; i < x_len ; i++)
    {
        __uint128_t overflow = __uint128_t(dest[2*i]) + __uint128_t(x[i]) * __uint128_t(x[i]) + carry;
        dest[2*i] = (overflow &  UINT64_MAX);
        carry     = (overflow >> 64);
        
        overflow    = __uint128_t(dest[2*i+1]) + carry;
        dest[2*i+1] = (overflow &  UINT64_MAX);
        carry       = (overflow >> 64);
    }
}

void gschool::mult(const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest)
{    
    uint64_t carry = 0;
    for (std::size_t j = 0; j < y_len; j++)
    {
        carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(dest[i+j]) + __uint128_t(x[i]) * __uint128_t(y[j]) + __uint128_t(carry);
            
            dest[i+j] = (overflow &  UINT64_MAX);
            carry     = (overflow >> 64);
        }
        dest[x_len+j] = carry;
    }
}
