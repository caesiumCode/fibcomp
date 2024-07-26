#include "uintinf_t.hpp"

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

bool uintinf_t::is_equal(const uintinf_t & y)
{
    if (y.m_digits.size() != m_digits.size()) return false;
    
    for (std::size_t i = 0; i < m_digits.size(); i++) if (m_digits[i] != y.m_digits[i]) return false;
    
    return true;
}

uintinf_t& uintinf_t::operator+=(const uintinf_t& y)
{
    gschool::add_r(this->m_digits, y.m_digits);
    return *this;
}

uintinf_t operator+(uintinf_t x, const uintinf_t& y)
{
    return gschool::add(x.m_digits, y.m_digits);
}

uintinf_t& uintinf_t::operator-=(const uintinf_t& y)
{
    gschool::sub_r(this->m_digits, y.m_digits);
    return *this;
}

uintinf_t operator-(uintinf_t x, const uintinf_t& y)
{
    return gschool::sub(x.m_digits, y.m_digits);
}

#if defined(MULT_KARATSUBA)
uintinf_t operator*(const uintinf_t& x, const uintinf_t& y)
{
    return karatsuba::mult(x.m_digits, y.m_digits);
}
#else
uintinf_t operator*(const uintinf_t& x, const uintinf_t& y)
{
    return gschool::mult(x.m_digits, y.m_digits);
}
#endif

#if defined(MULT_KARATSUBA)
uintinf_t square(const uintinf_t& x)
{
    return karatsuba::square(x.m_digits);
}
#else
uintinf_t square(const uintinf_t& x)
{
    return gschool::square(x.m_digits);
}
#endif

uintinf_t operator*(uint64_t scalar, uintinf_t x)
{
    return gschool::mult_s(scalar, x.m_digits);
}

void uintinf_t::quadrupling()
{
    bshift::times4_r(m_digits);
}

void uintinf_t::doubling()
{
    bshift::times2_r(m_digits);
}

void uintinf_t::halving()
{
    bshift::divide2_r(m_digits);
}

void uintinf_t::fourthing()
{
    bshift::divide4_r(m_digits);
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

