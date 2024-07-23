#include "uintinf_t.hpp"

#define MULT_KARATSUBA
#define MULT_KARATSUBA_CUTOFF 70 // same as in CPython (https://github.com/python/cpython/blob/main/Objects/longobject.c#L73)

uintinf_t::uintinf_t()
: m_digits({0})
{
}

uintinf_t::uintinf_t(uint64_t x)
: m_digits({x})
{
}

uintinf_t::uintinf_t(const std::vector<uint64_t>& digits)
: m_digits(digits.empty() ? std::vector<uint64_t>(1, 0) : digits)
{
}

bool uintinf_t::is_zero()
{
    return m_digits.size() == 1 && m_digits[0] == 0;
}

uintinf_t& uintinf_t::operator+=(const uintinf_t& y)
{
    relative_long_add(this->m_digits, y.m_digits);
    return *this;
}

uintinf_t operator+(uintinf_t x, const uintinf_t& y)
{
    return uintinf_t::long_add(x.m_digits, y.m_digits);
}

uintinf_t& uintinf_t::operator-=(const uintinf_t& y)
{
    relative_long_sub(this->m_digits, y.m_digits);
    return *this;
}

uintinf_t operator-(uintinf_t x, const uintinf_t& y)
{
    return uintinf_t::long_sub(x.m_digits, y.m_digits);
}

#if defined(MULT_KARATSUBA)
uintinf_t operator*(const uintinf_t& x, const uintinf_t& y)
{
    return uintinf_t::kara_mult(x.m_digits, y.m_digits);
}
#else
uintinf_t operator*(const uintinf_t& x, const uintinf_t& y)
{
    return uintinf_t::long_mult(x.m_digits, y.m_digits);
}
#endif

uintinf_t operator*(uint64_t scalar, uintinf_t x)
{
    return uintinf_t::scalar_mult(scalar, x.m_digits);
}

void uintinf_t::quadrupling()
{
    relative_quadrupling(m_digits);
}

void uintinf_t::doubling()
{
    relative_doubling(m_digits);
}

void uintinf_t::halving()
{
    relative_halving(m_digits);
}

void uintinf_t::fourthing()
{
    relative_fourthing(m_digits);
}

uintinf_t twice(uintinf_t x)
{
    x.doubling();
    
    return x;
}

uintinf_t half(uintinf_t x)
{
    x.halving();
    
    return x;
}

#if defined(MULT_KARATSUBA)
uintinf_t square(const uintinf_t& x)
{
    return uintinf_t::kara_square(x.m_digits);
}
#else
uintinf_t square(const uintinf_t& x)
{
    return uintinf_t::long_square(x.m_digits);
}
#endif

void swap(uintinf_t& x, uintinf_t& y)
{
    std::swap(x.m_digits, y.m_digits);
}

std::string uintinf_t::to_string() const
{
    std::string s;
    for (uint64_t d : m_digits) s += "[" + std::to_string(d) + "]";
    return s;
}

uint64_t uintinf_t::to_uint64() const
{
    return (m_digits.empty() ? 0 : m_digits[0]);
}


std::vector<uint64_t>& uintinf_t::relative_long_add(std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
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
    
    for (; i < x_len; i++)
    {
        x[i] += carry;
        
        carry = (x[i] < carry);
    }
    
    x[i] = carry;
    if (x[i] == 0) x.resize(len-1);
    
    return x;
}

std::vector<uint64_t> uintinf_t::long_add(std::vector<uint64_t> x, const std::vector<uint64_t>& y)
{
    return relative_long_add(x, y);
}

std::vector<uint64_t>& uintinf_t::relative_long_sub(std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
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
        
        borrow = borrow && (x[i] == uintinf_t::max_digit);
    }
    
    x.resize(last_idx + 1);
    
    return x;
}

std::vector<uint64_t> uintinf_t::long_sub(std::vector<uint64_t> x, const std::vector<uint64_t>& y)
{
    return relative_long_sub(x, y);
}

std::vector<uint64_t> uintinf_t::scalar_mult(uint64_t scalar, std::vector<uint64_t> x)
{
    if (scalar == 0) x.resize(1);
    
    uint64_t carry = 0;
    for (std::size_t i = 0; i < x.size(); i++)
    {
        __uint128_t overflow = __uint128_t(scalar) * __uint128_t(x[i]) + __uint128_t(carry);
        
        x[i]  = (overflow &  uintinf_t::max_digit);
        carry = (overflow >> uintinf_t::log2_base);
    }
    
    if (carry) x.push_back(carry);
    
    return x;
}

std::vector<uint64_t> uintinf_t::long_mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    if (x[0] == 0 || y[0] == 0) return {0};
    
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    std::vector<uint64_t> z(x_len + y_len, 0);
    
    uint64_t carry = 0;
    for (std::size_t j = 0; j < y_len; j++)
    {
        carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(z[i+j]) + __uint128_t(x[i]) * __uint128_t(y[j]) + __uint128_t(carry);
            
            z[i+j] = (overflow &  uintinf_t::max_digit);
            carry  = (overflow >> uintinf_t::log2_base);
        }
        z[x_len+j] = carry;
    }
    
    if (carry == 0) z.resize(x_len + y_len - 1);
    
    return z;
}

std::vector<uint64_t> uintinf_t::kara_mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    if (x[0] == 0 || y[0] == 0) return {0};
    if (x_len == 1) return scalar_mult(x[0], y);
    if (y_len == 1) return scalar_mult(y[0], x);
    if (x_len <= MULT_KARATSUBA_CUTOFF && y_len <= MULT_KARATSUBA_CUTOFF) return long_mult(x, y);
    
    std::size_t split = std::max(x_len, y_len) >> 1;
    std::size_t x_lim = std::min(split, x_len);
    std::size_t y_lim = std::min(split, y_len);
    
    std::vector<uint64_t> x_lo(x.begin()        , x.begin() + x_lim);
    std::vector<uint64_t> x_hi(x.begin() + x_lim, x.end()          );
    std::vector<uint64_t> y_lo(y.begin()        , y.begin() + y_lim);
    std::vector<uint64_t> y_hi(y.begin() + y_lim, y.end()          );
    
    std::vector<uint64_t> z_hi = kara_mult(x_hi, y_hi);
    std::vector<uint64_t> z_lo = kara_mult(x_lo, y_lo);
    std::vector<uint64_t> z_mi = long_sub(long_sub(kara_mult(long_add(x_hi, x_lo), long_add(y_hi, y_lo)), z_hi), z_lo);
    
    relative_long_add(z_mi, std::vector<uint64_t>(z_lo.begin() + std::min(split, z_lo.size()), z_lo.end()));
    relative_long_add(z_hi, std::vector<uint64_t>(z_mi.begin() + std::min(split, z_mi.size()), z_mi.end()));
    
    std::vector<uint64_t> z(2 * split + z_hi.size(), 0);
    std::copy(z_lo.begin(), z_lo.begin() + std::min(split, z_lo.size()), z.begin());
    std::copy(z_mi.begin(), z_mi.begin() + std::min(split, z_mi.size()), z.begin() + split);
    std::copy(z_hi.begin(), z_hi.end()                                 , z.begin() + (2*split));
    
    std::size_t len = z.size();
    while (z[len-1] == 0) len--;
    z.resize(len);
    
    return z;
}

std::vector<uint64_t> uintinf_t::kara_square(const std::vector<uint64_t>& x)
{
    std::size_t x_len = x.size();
    
    if (x[0] == 0) return {0};
    if (x_len <= MULT_KARATSUBA_CUTOFF) return long_square(x);
    
    std::size_t split = x_len >> 1;
    
    std::vector<uint64_t> x_lo(std::vector<uint64_t>(x.begin()        , x.begin() + split));
    std::vector<uint64_t> x_hi(std::vector<uint64_t>(x.begin() + split, x.end()          ));
    
    std::vector<uint64_t> z_hi = kara_square(x_hi);
    std::vector<uint64_t> z_lo = kara_square(x_lo);
    std::vector<uint64_t> z_mi = kara_mult(x_lo, x_hi);
    relative_doubling(z_mi);
    
    relative_long_add(z_mi, std::vector<uint64_t>(z_lo.begin() + std::min(split, z_lo.size()), z_lo.end()));
    relative_long_add(z_hi, std::vector<uint64_t>(z_mi.begin() + std::min(split, z_mi.size()), z_mi.end()));
    
    std::vector<uint64_t> z(2 * split + z_hi.size(), 0);
    std::copy(z_lo.begin(), z_lo.begin() + std::min(split, z_lo.size()), z.begin());
    std::copy(z_mi.begin(), z_mi.begin() + std::min(split, z_mi.size()), z.begin() + split);
    std::copy(z_hi.begin(), z_hi.end()                                 , z.begin() + (2*split));
    
    std::size_t len = z.size();
    while (z[len-1] == 0) len--;
    z.resize(len);
    
    return z;
}

std::vector<uint64_t> uintinf_t::long_square(const std::vector<uint64_t>& x)
{
    std::size_t x_len = x.size();
    
    std::vector<uint64_t> z(2 * x_len, 0);
    
    uint64_t carry = 0;
    for (std::size_t j = 0; j < x_len; j++)
    {
        carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(z[i+j]) + __uint128_t(x[i]) * __uint128_t(x[j]) + __uint128_t(carry);
            
            z[i+j] = (overflow &  uintinf_t::max_digit);
            carry  = (overflow >> uintinf_t::log2_base);
        }
        z[x_len+j] += carry;
    }
    
    if (carry == 0) z.resize(2 * x_len - 1);
    
    return z;
}

void uintinf_t::relative_quadrupling(std::vector<uint64_t>& x)
{
    uint64_t mask = uint64_t(3) << (uintinf_t::log2_base-2);
    
    if (x.back() & mask) x.push_back(x.back() >> (uintinf_t::log2_base-2));
    
    for (std::size_t i = x.size()-1; i > 0; i--)
    {
        x[i] <<= 2;
        x[i] |= (x[i-1] >> (uintinf_t::log2_base-2));
    }
    
    x[0] <<= 2;
}

void uintinf_t::relative_doubling(std::vector<uint64_t>& x)
{
    uint64_t mask = uint64_t(1) << (uintinf_t::log2_base-1);
    
    if (x.back() & mask) x.push_back(1);
    
    for (std::size_t i = x.size()-1; i > 0; i--)
    {
        x[i] <<= 1;
        x[i] |= (x[i-1] >> (uintinf_t::log2_base-1));
    }
    
    x[0] <<= 1;
}

void uintinf_t::relative_halving(std::vector<uint64_t>& x)
{
    std::size_t len = x.size();
    
    for (std::size_t i = 0; i < len-1; i++)
    {
        x[i] >>= 1;
        x[i] |= (x[i+1] << (uintinf_t::log2_base-1));
    }
    
    x.back() >>= 1;
    if (x.back() == 0 && len > 1) x.resize(len-1);
}

void uintinf_t::relative_fourthing(std::vector<uint64_t>& x)
{
    std::size_t len = x.size();
    
    for (std::size_t i = 0; i < len-1; i++)
    {
        x[i] >>= 2;
        x[i] |= (x[i+1] << (uintinf_t::log2_base-2));
    }
    
    x.back() >>= 2;
    if (x.back() == 0 && len > 1) x.resize(len-1);
}
