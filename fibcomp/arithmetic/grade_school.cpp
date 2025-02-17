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

// Assumes x >= y
void gschool::sub_r2(const uint64_t* x, const std::size_t x_len, uint64_t* y, const std::size_t y_len)
{
    std::size_t i = 0;
    uint64_t borrow = 0;
    for (i = 0; i < y_len && i < x_len; i++)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x[i] <= y[i] : x[i] < y[i]);
        y[i] = x[i] - minus;
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
            carry = (carry ? plus <= y[i] : plus < y[i]);
            
            dest[i] = plus;
        }
        
        for (; i < x_len; i++)
        {
            dest[i] = x[i] + carry;
            
            carry = (dest[i] < carry);
        }
        
        dest[i] += carry;
    }
}

// Assumes x >= y
void gschool::sub(const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest)
{
    std::size_t i = 0;
    uint64_t borrow = 0;
    for (i = 0; i < y_len && i < x_len; i++)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x[i] <= y[i] : x[i] < y[i]);
        
        dest[i] = x[i] - minus;
    }
    
    for (; i < x_len; i++)
    {
        dest[i] = x[i] - borrow;
                
        borrow = borrow && (dest[i] == UINT64_MAX);
    }
}

void gschool::add_r_sgn(bool& x_sgn, uint64_t* x, const std::size_t x_len, const bool y_sgn, const uint64_t* y, const std::size_t y_len)
{
    if (x_sgn == y_sgn) // x+y or -x-y=-(x+y)
    {
        add_r(x, x_len, y, y_len);
    }
    else if (x_sgn && !y_sgn) // -x+y=y-x
    {
        x_sgn = cmp::less_than(y, y_len, x, x_len);
        if (x_sgn) sub_r (x, x_len, y, y_len); // -(x-y)
        else       sub_r2(y, y_len, x, x_len); //   y-x
    }
    else // x-y
    {
        x_sgn = cmp::less_than(x, x_len, y, y_len);
        if (x_sgn) sub_r2(y, y_len, x, x_len); // -(y-x)
        else       sub_r (x, x_len, y, y_len); //   x-y
    }
}

void gschool::add_sgn(const bool x_sgn, const uint64_t* x, const std::size_t x_len,
                      const bool y_sgn, const uint64_t* y, const std::size_t y_len,
                      bool& sgn,        uint64_t* dest)
{
    if (x_sgn == y_sgn) // x+y or -x-y=-(x+y)
    {
        sgn = x_sgn;
        add(x, x_len, y, y_len, dest);
    }
    else if (x_sgn && !y_sgn) // -x+y=y-x
    {
        sgn = cmp::less_than(y, y_len, x, x_len);
        if (sgn) sub(x, x_len, y, y_len, dest);
        else     sub(y, y_len, x, x_len, dest);
    }
    else // x-y
    {
        sgn = cmp::less_than(x, x_len, y, y_len);
        if (sgn) sub(y, y_len, x, x_len, dest);
        else     sub(x, x_len, y, y_len, dest);
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


void gschool::divide_by_3_r(uint64_t* x, const std::size_t x_len)
{
    uint64_t carry = 0;
    for (std::size_t i = 0; i < x_len; i++)
    {
        uint64_t s = x[i];
        x[i] -= carry;
        carry = (x[i] > s);
        
        x[i] *= INVERSE_3;
        
        carry += (x[i] > UINT64_MAX/3);
        carry += (x[i] > (UINT64_MAX/3)*2);
    }
}








void gschool::add(const uint64_t* x, const std::size_t x_len,
                  const uint64_t* y, const std::size_t y_len,
                  uint64_t* dest, const std::size_t dest_len, uint64_t& c)
{
    if (x_len < y_len) add(y, y_len, x, x_len, dest, dest_len, c);
    else
    {
        std::size_t i = 0;
        uint64_t carry = 0;
        for (i = 0; i < y_len && i < dest_len; i++)
        {
            uint64_t plus = x[i] + y[i] + carry;
            carry = (carry ? plus <= y[i] : plus < y[i]);
            
            dest[i] = plus;
        }
        
        for (; i < x_len && i < dest_len; i++)
        {
            dest[i] = x[i] + carry;
            
            carry = (dest[i] < carry);
        }
        
        if (i == dest_len) c = (i < x_len ? x[i] : 0) + carry;
    }
}

// Assumes x >= y
void gschool::sub(const uint64_t* x, const std::size_t x_len, 
                  const uint64_t* y, const std::size_t y_len,
                  uint64_t* dest, const std::size_t dest_len, uint64_t& dest_top)
{
    std::size_t i = 0;
    uint64_t borrow = 0;
    for (i = 0; i < y_len && i < x_len && i < dest_len; i++)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x[i] <= y[i] : x[i] < y[i]);
        
        dest[i] = x[i] - minus;
    }
    
    if (i == dest_len && i < y_len && i < x_len)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x[i] <= y[i] : x[i] < y[i]);
        
        dest_top = x[i] - minus;
        
        i++;
    }
    
    for (; i < x_len && i < dest_len; i++)
    {
        dest[i] = x[i] - borrow;
                
        borrow = borrow && (dest[i] == UINT64_MAX);
    }
    
    if (i == dest_len && i < x_len) dest_top = x[i] - borrow;
}

void gschool::add_sgn(const bool x_sgn, const uint64_t* x, const std::size_t x_len,
                      const bool y_sgn, const uint64_t* y, const std::size_t y_len,
                      bool& sgn,        uint64_t* dest, const std::size_t dest_len, uint64_t& c)
{
    if (x_sgn == y_sgn) // x+y or -x-y=-(x+y)
    {
        sgn = x_sgn;
        add(x, x_len, y, y_len, dest, dest_len, c);
    }
    else if (x_sgn && !y_sgn) // -x+y=y-x
    {
        sgn = cmp::less_than(y, y_len, x, x_len);
        if (sgn) sub(x, x_len, y, y_len, dest, dest_len, c);
        else     sub(y, y_len, x, x_len, dest, dest_len, c);
    }
    else // x-y
    {
        sgn = cmp::less_than(x, x_len, y, y_len);
        if (sgn) sub(y, y_len, x, x_len, dest, dest_len, c);
        else     sub(x, x_len, y, y_len, dest, dest_len, c);
    }
}

void gschool::add_r_sgn(      bool& x_sgn,      uint64_t* x, const std::size_t x_len,       uint64_t& x_top,
                        const bool y_sgn, const uint64_t* y, const std::size_t y_len, const uint64_t y_top)
{
    if (x_sgn == y_sgn) // x+y or -x-y=-(x+y)
    {
        add_r(x, x_len, x_top, y, y_len, y_top);
    }
    else if (x_sgn && !y_sgn) // -x+y=y-x
    {
        x_sgn = cmp::less_than(y, y_len, y_top, x, x_len, x_top);
        if (x_sgn) sub_r (x, x_len, x_top, y, y_len, y_top); // -(x-y)
        else       sub_r2(y, y_len, y_top, x, x_len, x_top); //   y-x
    }
    else // x-y
    {
        x_sgn = cmp::less_than(x, x_len, x_top, y, y_len, y_top);
        if (x_sgn) sub_r2(y, y_len, y_top, x, x_len, x_top); // -(y-x)
        else       sub_r (x, x_len, x_top, y, y_len, y_top); //   x-y
    }
}

void gschool::add_r(      uint64_t* x, const std::size_t x_len,       uint64_t& x_top,
                    const uint64_t* y, const std::size_t y_len, const uint64_t y_top)
{
    std::size_t i = 0;
    uint64_t carry = 0;
    for (i = 0; i < y_len && i < x_len; i++)
    {
        uint64_t plus = y[i] + carry;
        x[i] += plus;
        
        carry = (carry ? x[i] <= y[i] : x[i] < y[i]);
    }
    
    if (i == y_len && i < x_len)
    {
        uint64_t plus = y_top + carry;
        x[i] += plus;
        
        carry = (carry ? x[i] <= y_top : x[i] < y_top);
        
        i++;
    }
    else if (i < y_len && i == x_len)
    {
        uint64_t plus = y[i] + carry;
        x_top += plus;
        
        carry = (carry ? x_top <= y[i] : x_top < y[i]);
        
        i++;
    }
    else
    {
        uint64_t plus = y_top + carry;
        x_top += plus;
        
        carry = (carry ? x_top <= y_top : x_top < y_top);
        
        i++;
    }
    
    for (; i < x_len && carry; i++)
    {
        x[i] += carry;
        
        carry = (x[i] < carry);
    }
    
    if (i == x_len) x_top += carry;
}

// Assumes x >= y
void gschool::sub_r(      uint64_t* x, const std::size_t x_len,       uint64_t& x_top,
                    const uint64_t* y, const std::size_t y_len, const uint64_t y_top)
{
    std::size_t i = 0;
    uint64_t borrow = 0;
    for (i = 0; i < y_len && i < x_len; i++)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x[i] <= y[i] : x[i] < y[i]);
        x[i] -= minus;
    }
    
    if (i == y_len && i < x_len)
    {
        uint64_t minus = y_top + borrow;
        borrow = (borrow ? x[i] <= y_top : x[i] < y_top);
        x[i] -= minus;
        
        i++;
    }
    else if (i < y_len && i == x_len)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x_top <= y[i] : x_top < y[i]);
        x_top -= minus;
        
        i++;
    }
    else
    {
        uint64_t minus = y_top + borrow;
        borrow = (borrow ? x_top <= y_top : x_top < y_top);
        x_top -= minus;
        
        i++;
    }
    
    for (; i < x_len && borrow; i++)
    {
        x[i] -= borrow;
                
        borrow = borrow && (x[i] == UINT64_MAX);
    }
    
    if (i == x_len) x_top -= borrow;
}

// Assumes x >= y
void gschool::sub_r2(const uint64_t* x, const std::size_t x_len, const uint64_t x_top, uint64_t* y, const std::size_t y_len, uint64_t& y_top)
{
    std::size_t i = 0;
    uint64_t borrow = 0;
    for (i = 0; i < y_len && i < x_len; i++)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x[i] <= y[i] : x[i] < y[i]);
        y[i] = x[i] - minus;
    }
    
    if (i == y_len && i < x_len)
    {
        uint64_t minus = y_top + borrow;
        borrow = (borrow ? x[i] <= y_top : x[i] < y_top);
        y_top = x[i] - minus;
        
        i++;
    }
    else if (i < y_len && i == x_len)
    {
        uint64_t minus = y[i] + borrow;
        borrow = (borrow ? x_top <= y[i] : x_top < y[i]);
        y[i] = x_top - minus;
        
        i++;
    }
    else
    {
        uint64_t minus = y_top + borrow;
        borrow = (borrow ? x_top <= y_top : x_top < y_top);
        y_top = x_top - minus;
        
        i++;
    }
}
