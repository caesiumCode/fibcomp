#include "uintinf_t.hpp"


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
}

bool uintinf_t::is_zero()
{
    return m_digits.empty() || (m_digits.size() == 1 && m_digits[0] == 0);
}

uintinf_t& uintinf_t::operator+=(const uintinf_t& y)
{
    std::size_t x_len =   m_digits.size();
    std::size_t y_len = y.m_digits.size();
    
    m_digits.reserve(std::max(x_len, y_len) + 1);
    
    std::size_t i = 0;
    uint64_t carry = 0;
    while (i < x_len)
    {
        uint64_t y_part = (i < y_len ? y.m_digits[i] : 0);
        m_digits[i] += y_part + carry;
        
        carry = (m_digits[i] <  y_part + carry);
        i++;
    }
    
    while (carry || i < y_len)
    {
        uint64_t y_part = (i < y_len ? y.m_digits[i] : 0);
        m_digits.push_back(y_part + carry);
        
        carry = (m_digits[i] <  y_part + carry);
        i++;
    }
    
    return *this;
}

/*
uintinf_t& uintinf_t::operator*=(const uintinf_t& y)
{
    std::size_t x_len =   m_digits.size();
    std::size_t y_len = y.m_digits.size();
    
    m_digits.resize(x_len + y_len, 0);
    
    for (std::size_t j = 0; j < y_len; j++)
    {
        uint64_t carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(m_digits[i+j]) + __uint128_t(m_digits[i]) * __uint128_t(y.m_digits[j]) + __uint128_t(carry);
            
            m_digits[i+j] = (overflow &  uintinf_t::max_digit);
            carry         = (overflow >> uintinf_t::log2_base);
        }
        m_digits[x_len+j] = carry;
    }
    
    return *this;
}*/

uintinf_t operator+(uintinf_t x, const uintinf_t& y)
{
    return x += y;
}


uintinf_t operator*(const uintinf_t& x, const uintinf_t& y)
{
    std::size_t x_len = x.m_digits.size();
    std::size_t y_len = y.m_digits.size();
    
    uintinf_t z(0);
    z.m_digits.resize(x_len + y_len, 0);
    
    uint64_t carry = 0;
    for (std::size_t j = 0; j < y_len; j++)
    {
        carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(z.m_digits[i+j]) + __uint128_t(x.m_digits[i]) * __uint128_t(y.m_digits[j]) + __uint128_t(carry);
            
            z.m_digits[i+j] = (overflow &  uintinf_t::max_digit);
            carry           = (overflow >> uintinf_t::log2_base);
        }
        z.m_digits[x_len+j] = carry;
    }
    
    if (carry == 0) z.m_digits.resize(x_len + y_len - 1);
    
    return z;
}

uintinf_t square(const uintinf_t& x)
{
    std::size_t x_len = x.m_digits.size();
    
    uintinf_t z(0);
    z.m_digits.resize(2 * x_len, 0);
    
    uint64_t carry = 0;
    for (std::size_t j = 0; j < x_len; j++)
    {
        carry = 0;
        for (std::size_t i = 0; i < x_len; i++)
        {
            __uint128_t overflow = __uint128_t(z.m_digits[i+j]) + __uint128_t(x.m_digits[i]) * __uint128_t(x.m_digits[j]) + __uint128_t(carry);
            
            z.m_digits[i+j] = (overflow &  uintinf_t::max_digit);
            carry           = (overflow >> uintinf_t::log2_base);
        }
        z.m_digits[x_len+j] += carry;
    }
    
    if (carry == 0) z.m_digits.resize(2 * x_len - 1);
    
    return z;
}


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
