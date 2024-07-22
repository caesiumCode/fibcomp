#include "uintinf_t.hpp"

#define MULT_KARATSUBA
#define MULT_KARATSUBA_LIM 100

uintinf_t::uintinf_t()
{
    m_digits = {0};
}


uintinf_t::uintinf_t(uint64_t x)
{
    m_digits = {x};
}


uintinf_t::uintinf_t(const std::vector<uint64_t>& digits)
{
    m_digits = digits;
    
    if (m_digits.empty()) m_digits = {0};
}

bool uintinf_t::is_zero()
{
    return m_digits.empty() || (m_digits.size() == 1 && m_digits[0] == 0);
}

uintinf_t& uintinf_t::operator+=(const uintinf_t& y)
{
    const std::size_t x_len =   m_digits.size();
    const std::size_t y_len = y.m_digits.size();
    const std::size_t len   = std::max(x_len, y_len) + 1;
    
    m_digits.resize(len, 0);
    
    std::size_t i = 0;
    uint64_t carry = 0;
    for (i = 0; i < y_len; i++)
    {
        uint64_t plus = y.m_digits[i] + carry;
        m_digits[i] += plus;
        
        carry = (carry ? m_digits[i] <= y.m_digits[i] : m_digits[i] < y.m_digits[i]);
    }
    
    for (; i < x_len; i++)
    {
        m_digits[i] += carry;
        
        carry = (m_digits[i] < carry);
    }
    
    m_digits[i] = carry;
    if (m_digits[i] == 0) m_digits.resize(len-1);
    
    return *this;
}

uintinf_t& uintinf_t::operator-=(const uintinf_t& y)
{
    const std::size_t x_len =   m_digits.size();
    const std::size_t y_len = y.m_digits.size();
    std::size_t last_idx = 0;
    
    std::size_t i = 0;
    uint64_t borrow = 0;
    for (i = 0; i < y_len; i++)
    {
        uint64_t minus = y.m_digits[i] + borrow;
        borrow = (borrow ? m_digits[i] <= y.m_digits[i] : m_digits[i] < y.m_digits[i]);
        m_digits[i] -= minus;
        
        if (m_digits[i] != 0) last_idx = i;
    }
    
    for (; i < x_len; i++)
    {
        m_digits[i] -= borrow;
        
        if (m_digits[i] != 0) last_idx = i;
        
        borrow = borrow && (m_digits[i] == uintinf_t::max_digit);
    }
    
    m_digits.resize(last_idx + 1);
    
    return *this;
}

uintinf_t operator+(uintinf_t x, const uintinf_t& y)
{
    return x += y;
}

uintinf_t operator-(uintinf_t x, const uintinf_t& y)
{
    return x -= y;
}

uintinf_t long_multiplication(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    if (x[0] == 0 || y[0] == 0) return uintinf_t(0);
    
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    uintinf_t z(0);
    z.m_digits.resize(x_len + y_len, 0);
    
    uint64_t carry = 0;
    for (std::size_t j = 0; j < y_len; j++)
    {
        carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(z.m_digits[i+j]) + __uint128_t(x[i]) * __uint128_t(y[j]) + __uint128_t(carry);
            
            z.m_digits[i+j] = (overflow &  uintinf_t::max_digit);
            carry           = (overflow >> uintinf_t::log2_base);
        }
        z.m_digits[x_len+j] = carry;
    }
    
    if (carry == 0) z.m_digits.resize(x_len + y_len - 1);
    
    return z;
}

#if defined(MULT_KARATSUBA)
uintinf_t operator*(const uintinf_t& x, const uintinf_t& y)
{
    uintinf_t z(0);
    
    std::size_t x_len = x.m_digits.size();
    std::size_t y_len = y.m_digits.size();
    
    if (x.m_digits[0] == 0 || y.m_digits[0] == 0) return 0;
    if (x_len == 1) return x.m_digits[0] * y;
    if (y_len == 1) return y.m_digits[0] * x;
    if (x_len <= MULT_KARATSUBA_LIM && y_len <= MULT_KARATSUBA_LIM) return long_multiplication(x.m_digits, y.m_digits);
    
    std::size_t split = std::max(x_len, y_len) >> 1;
    std::size_t x_lim = std::min(split, x_len);
    std::size_t y_lim = std::min(split, y_len);
    
    uintinf_t x_lo(std::vector<uint64_t>(x.m_digits.begin()        , x.m_digits.begin() + x_lim));
    uintinf_t x_hi(std::vector<uint64_t>(x.m_digits.begin() + x_lim, x.m_digits.end()          ));
    uintinf_t y_lo(std::vector<uint64_t>(y.m_digits.begin()        , y.m_digits.begin() + y_lim));
    uintinf_t y_hi(std::vector<uint64_t>(y.m_digits.begin() + y_lim, y.m_digits.end()          ));
    
    uintinf_t z_hi = x_hi * y_hi;
    uintinf_t z_lo = x_lo * y_lo;
    uintinf_t z_mi = (x_hi + x_lo) * (y_hi + y_lo) - z_hi - z_lo;
    
    z_mi += uintinf_t(std::vector<uint64_t>(z_lo.m_digits.begin() + std::min(split, z_lo.m_digits.size()), z_lo.m_digits.end()));
    z_hi += uintinf_t(std::vector<uint64_t>(z_mi.m_digits.begin() + std::min(split, z_mi.m_digits.size()), z_mi.m_digits.end()));
    
    z.m_digits.resize(2 * split + z_hi.m_digits.size());
    std::copy(z_lo.m_digits.begin(), z_lo.m_digits.begin() + std::min(split, z_lo.m_digits.size()), z.m_digits.begin());
    std::copy(z_mi.m_digits.begin(), z_mi.m_digits.begin() + std::min(split, z_mi.m_digits.size()), z.m_digits.begin() + split);
    std::copy(z_hi.m_digits.begin(), z_hi.m_digits.end()                                          , z.m_digits.begin() + (2*split));
    
    std::size_t len = z.m_digits.size();
    while (z.m_digits[len-1] == 0) len--;
    z.m_digits.resize(len);
    
    return z;
}
#else
uintinf_t operator*(const uintinf_t& x, const uintinf_t& y)
{
    return long_multiplication(x.m_digits, y.m_digits);
}
#endif

uintinf_t operator*(uint64_t scalar, uintinf_t x)
{
    if (scalar == 0) x.m_digits.resize(1);
    
    uint64_t carry = 0;
    for (std::size_t i = 0; i < x.m_digits.size(); i++)
    {
        __uint128_t overflow = __uint128_t(scalar) * __uint128_t(x.m_digits[i]) + __uint128_t(carry);
        
        x.m_digits[i] = (overflow &  uintinf_t::max_digit);
        carry         = (overflow >> uintinf_t::log2_base);
    }
    
    if (carry) x.m_digits.push_back(carry);
    
    return x;
}

void uintinf_t::doubling()
{
    uint64_t mask = uint64_t(1) << 63;
    
    if (m_digits.back() & mask) m_digits.push_back(1);
    
    for (std::size_t i = m_digits.size()-1; i > 0; i--)
    {
        m_digits[i] <<= 1;
        m_digits[i] |= (m_digits[i-1] >> 63);
    }
    
    m_digits[0] <<= 1;
}

void uintinf_t::halving()
{
    std::size_t len = m_digits.size();
    
    for (std::size_t i = 0; i < len-1; i++)
    {
        m_digits[i] >>= 1;
        m_digits[i] |= (m_digits[i+1] << 63);
    }
    
    m_digits.back() >>= 1;
    if (m_digits.back() == 0 && len > 1) m_digits.resize(len-1);
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

uintinf_t long_squaring(const std::vector<uint64_t>& x)
{
    std::size_t x_len = x.size();
    
    uintinf_t z(0);
    z.m_digits.resize(2 * x_len, 0);
    
    uint64_t carry = 0;
    for (std::size_t j = 0; j < x_len; j++)
    {
        carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(z.m_digits[i+j]) + __uint128_t(x[i]) * __uint128_t(x[j]) + __uint128_t(carry);
            
            z.m_digits[i+j] = (overflow &  uintinf_t::max_digit);
            carry           = (overflow >> uintinf_t::log2_base);
        }
        z.m_digits[x_len+j] += carry;
    }
    
    if (carry == 0) z.m_digits.resize(2 * x_len - 1);
    
    return z;
}

#if defined(MULT_KARATSUBA)
uintinf_t square(const uintinf_t& x)
{
    uintinf_t z(0);
    
    std::size_t x_len = x.m_digits.size();
    
    if (x.m_digits[0] == 0) return 0;
    if (x_len <= MULT_KARATSUBA_LIM) return long_squaring(x.m_digits);
    
    std::size_t split = x_len >> 1;
    
    uintinf_t x_lo(std::vector<uint64_t>(x.m_digits.begin()        , x.m_digits.begin() + split));
    uintinf_t x_hi(std::vector<uint64_t>(x.m_digits.begin() + split, x.m_digits.end()          ));
    
    uintinf_t z_hi = square(x_hi);
    uintinf_t z_lo = square(x_lo);
    uintinf_t z_mi = x_lo * x_hi;
    z_mi.doubling();
    
    z_mi += uintinf_t(std::vector<uint64_t>(z_lo.m_digits.begin() + std::min(split, z_lo.m_digits.size()), z_lo.m_digits.end()));
    z_hi += uintinf_t(std::vector<uint64_t>(z_mi.m_digits.begin() + std::min(split, z_mi.m_digits.size()), z_mi.m_digits.end()));
    
    z.m_digits.resize(2 * split + z_hi.m_digits.size());
    std::copy(z_lo.m_digits.begin(), z_lo.m_digits.begin() + std::min(split, z_lo.m_digits.size()), z.m_digits.begin());
    std::copy(z_mi.m_digits.begin(), z_mi.m_digits.begin() + std::min(split, z_mi.m_digits.size()), z.m_digits.begin() + split);
    std::copy(z_hi.m_digits.begin(), z_hi.m_digits.end()                                          , z.m_digits.begin() + (2*split));
    
    std::size_t len = z.m_digits.size();
    while (z.m_digits[len-1] == 0) len--;
    z.m_digits.resize(len);
    
    return z;
}
#else
uintinf_t square(const uintinf_t& x)
{
    return long_squaring(x.m_digits);
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
