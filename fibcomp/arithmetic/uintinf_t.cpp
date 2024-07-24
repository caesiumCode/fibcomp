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
    return uintinf_t::kara_mult(x.m_digits, y.m_digits);
}
#else
uintinf_t operator*(const uintinf_t& x, const uintinf_t& y)
{
    return gschool::mult(x.m_digits, y.m_digits);
}
#endif

uintinf_t operator*(uint64_t scalar, uintinf_t x)
{
    return gschool::mult_s(scalar, x.m_digits);
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
    return gschool::square(x.m_digits);
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

void uintinf_t::kara_mult(const uint64_t *x, const std::size_t x_len, const uint64_t *y, const std::size_t y_len, uint64_t *dest)
{
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return;
    else if (x_len == 1) gschool::mult_s(x[0], y, y_len, dest);
    else if (y_len == 1) gschool::mult_s(y[0], x, x_len, dest);
    else if (x_len <= MULT_KARATSUBA_CUTOFF && y_len <= MULT_KARATSUBA_CUTOFF) gschool::mult(x, x_len, y, y_len, dest);
    else
    {
        std::size_t split = std::max(x_len, y_len) >> 1;
        std::size_t x_lim = std::min(split, x_len);
        std::size_t y_lim = std::min(split, y_len);
        
        uint64_t* z_lo = dest;
        uint64_t* z_hi = dest + (2*split);
        
        std::size_t z_lo_len = x_lim + y_lim;
        std::size_t z_hi_len = (x_len == x_lim || y_len == y_lim) ? 1 : x_len - x_lim + y_len - y_lim;
        
        kara_mult(x,        x_lim,          y,          y_lim,       z_lo); // z_lo = x_lo * y_lo
        kara_mult(x+x_lim,  x_len-x_lim,    y+y_lim,    y_len-y_lim, z_hi); // z_hi = x_hi * y_hi
        
        while (z_lo_len > 1 && z_lo[z_lo_len-1] == 0) z_lo_len--;
        while (z_hi_len > 1 && z_lo[z_hi_len-1] == 0) z_hi_len--;
        
        std::vector<uint64_t> x_plus(std::max(x_lim, x_len-x_lim) + 1, 0);
        std::vector<uint64_t> y_plus(std::max(y_lim, y_len-y_lim) + 1, 0);
        
        gschool::add(x, x_lim, x+x_lim, x_len-x_lim, x_plus.data()); // x_lo + x_hi
        gschool::add(y, y_lim, y+y_lim, y_len-y_lim, y_plus.data()); // y_lo + y_hi
        
        if (x_plus.back() == 0) x_plus.resize(x_plus.size()-1);
        if (y_plus.back() == 0) y_plus.resize(y_plus.size()-1);
        
        std::vector<uint64_t> z_mi(x_plus.size() + y_plus.size(), 0);
        kara_mult(x_plus.data(), x_plus.size(), y_plus.data(), y_plus.size(), z_mi.data()); // (x_lo + x_hi) * (y_lo + y_hi)
        
        gschool::sub_r(z_mi.data(), z_mi.size(), z_lo, z_lo_len); // (x_lo + x_hi) * (y_lo + y_hi) - z_lo
        gschool::sub_r(z_mi.data(), z_mi.size(), z_hi, z_hi_len); // (x_lo + x_hi) * (y_lo + y_hi) - z_lo - z_hi
        std::size_t z_mi_len = z_mi.size();
        
                              gschool::add_r(z_mi.data(), z_mi_len, dest        + split, split);
        if (z_mi_len > split) gschool::add_r(z_hi,        z_hi_len, z_mi.data() + split, z_mi_len - split);
        
        
        //while (z_mi[z_mi_len-1] == 0 && z_mi_len > 1) z_mi_len--;
        //z_mi.resize(z_mi_len);
                
        std::copy(z_mi.data(), z_mi.data() + std::min(split, z_mi_len), dest + split);
    }
}

std::vector<uint64_t> uintinf_t::kara_mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
#if defined(MULT_MEM_OPT)
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return {0};
    if (x_len == 1) return gschool::mult_s(x[0], y);
    if (y_len == 1) return gschool::mult_s(y[0], x);
    if (x_len <= MULT_KARATSUBA_CUTOFF && y_len <= MULT_KARATSUBA_CUTOFF) return gschool::mult(x, y);
    
    
    std::vector<uint64_t> z(x.size() + y.size(), 0);
    
    kara_mult(x.data(), x.size(), y.data(), y.size(), z.data());
    
    if (z.back() == 0) z.resize(z.size()-1);
    
    return z;
# else
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return {0};
    if (x_len == 1) return gschool::mult_s(x[0], y);
    if (y_len == 1) return gschool::mult_s(y[0], x);
    if (x_len <= MULT_KARATSUBA_CUTOFF && y_len <= MULT_KARATSUBA_CUTOFF) return gschool::mult(x, y);
    
    std::size_t split = std::max(x_len, y_len) >> 1;
    std::size_t x_lim = std::min(split, x_len);
    std::size_t y_lim = std::min(split, y_len);
    
    std::vector<uint64_t> x_lo(x.begin()        , x.begin() + x_lim);
    std::vector<uint64_t> x_hi(x.begin() + x_lim, x.end()          );
    std::vector<uint64_t> y_lo(y.begin()        , y.begin() + y_lim);
    std::vector<uint64_t> y_hi(y.begin() + y_lim, y.end()          );
    
    std::vector<uint64_t> z_lo = kara_mult(x_lo, y_lo);
    std::vector<uint64_t> z_hi = kara_mult(x_hi, y_hi);
    std::vector<uint64_t> z_mi = kara_mult(gschool::add(x_hi, x_lo), gschool::add(y_hi, y_lo));
    
    gschool::sub_r(z_mi, z_lo);
    gschool::sub_r(z_mi, z_hi);
    
    gschool::add_r(z_mi, std::vector<uint64_t>(z_lo.begin() + std::min(split, z_lo.size()), z_lo.end()));
    gschool::add_r(z_hi, std::vector<uint64_t>(z_mi.begin() + std::min(split, z_mi.size()), z_mi.end()));
        
    std::vector<uint64_t> z(2 * split + z_hi.size(), 0);
    std::copy(z_lo.begin(), z_lo.begin() + std::min(split, z_lo.size()), z.begin());
    std::copy(z_mi.begin(), z_mi.begin() + std::min(split, z_mi.size()), z.begin() + split);
    std::copy(z_hi.begin(), z_hi.end()                                 , z.begin() + (2*split));
    
    std::size_t len = z.size();
    while (len > 1 && z[len-1] == 0) len--;
    z.resize(len);
    
    return z;
#endif
}

void uintinf_t::kara_square(const uint64_t *x, const std::size_t x_len, uint64_t *dest)
{
    if (x_len == 1 && x[0] == 0) return;
    else if (x_len <= MULT_KARATSUBA_CUTOFF) gschool::square(x, x_len, dest);
    else
    {
        std::size_t x_lim = x_len >> 1;
        
        uint64_t* z_lo = dest;
        uint64_t* z_hi = dest + (2*x_lim);
        
        std::size_t z_lo_len = 2*x_lim;
        std::size_t z_hi_len = 2*(x_len - x_lim);
        
        kara_square(x,        x_lim,       z_lo); // z_lo = x_lo * x_lo
        kara_square(x+x_lim,  x_len-x_lim, z_hi); // z_hi = x_hi * x_hi
        
        z_lo_len -= (z_lo[z_lo_len-1] == 0);
        z_hi_len -= (z_hi[z_hi_len-1] == 0);
        
        std::vector<uint64_t> z_mi(x_len, 0);
        kara_mult(x, x_lim, x+x_lim, x_len-x_lim, z_mi.data()); // x_lo * x_hi
        if (z_mi.back() == 0 && z_mi.size() > 1) z_mi.resize(z_mi.size()-1);
        relative_doubling(z_mi); // z_mi = 2 * x_lo * x_hi
        std::size_t z_mi_len = z_mi.size();
        
                              gschool::add_r(z_mi.data(), z_mi_len, dest        + x_lim, x_lim);
        if (z_mi_len > x_lim) gschool::add_r(z_hi,        z_hi_len, z_mi.data() + x_lim, z_mi_len - x_lim);
        
        std::copy(z_mi.data(), z_mi.data() + std::min(x_lim, z_mi_len), dest + x_lim);
    }
}

std::vector<uint64_t> uintinf_t::kara_square(const std::vector<uint64_t>& x)
{
#if defined(SQUARE_MEM_OPT)
    std::size_t x_len = x.size();
    
    if (x_len == 1 && x[0] == 0) return {0};
    if (x_len <= MULT_KARATSUBA_CUTOFF) return gschool::square(x);
    
    
    std::vector<uint64_t> z(2*x.size(), 0);
    
    kara_square(x.data(), x.size(), z.data());
    
    if (z.back() == 0) z.resize(z.size()-1);
    
    return z;
#else
    std::size_t x_len = x.size();
    
    if (x[0] == 0) return {0};
    if (x_len <= MULT_KARATSUBA_CUTOFF) return gschool::square(x);
    
    std::size_t split = x_len >> 1;
    
    std::vector<uint64_t> x_lo(std::vector<uint64_t>(x.begin()        , x.begin() + split));
    std::vector<uint64_t> x_hi(std::vector<uint64_t>(x.begin() + split, x.end()          ));
    
    std::vector<uint64_t> z_hi = kara_square(x_hi);
    std::vector<uint64_t> z_lo = kara_square(x_lo);
    std::vector<uint64_t> z_mi = kara_mult(x_lo, x_hi);
    relative_doubling(z_mi);

    gschool::add_r(z_mi, std::vector<uint64_t>(z_lo.begin() + std::min(split, z_lo.size()), z_lo.end()));
    gschool::add_r(z_hi, std::vector<uint64_t>(z_mi.begin() + std::min(split, z_mi.size()), z_mi.end()));
    
    std::vector<uint64_t> z(2 * split + z_hi.size(), 0);
    std::copy(z_lo.begin(), z_lo.begin() + std::min(split, z_lo.size()), z.begin());
    std::copy(z_mi.begin(), z_mi.begin() + std::min(split, z_mi.size()), z.begin() + split);
    std::copy(z_hi.begin(), z_hi.end()                                 , z.begin() + (2*split));
    
    std::size_t len = z.size();
    while (z[len-1] == 0) len--;
    z.resize(len);
    
    return z;
#endif
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
