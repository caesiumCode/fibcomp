#include "karatsuba.hpp"


std::vector<uint64_t> karatsuba_old::mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
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
    
    std::vector<uint64_t> z_lo = karatsuba_old::mult(x_lo, y_lo);
    std::vector<uint64_t> z_hi = karatsuba_old::mult(x_hi, y_hi);
    std::vector<uint64_t> z_mi = karatsuba_old::mult(gschool::add(x_hi, x_lo), gschool::add(y_hi, y_lo));
    
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
}

std::vector<uint64_t> karatsuba_old::square(const std::vector<uint64_t>& x)
{
    std::size_t x_len = x.size();
    
    if (x_len == 0 || (x_len == 1 && x[0] == 0)) return {0};
    if (x_len <= MULT_KARATSUBA_CUTOFF) return gschool::square(x);
    
    std::size_t split = x_len >> 1;
    
    std::vector<uint64_t> x_lo(std::vector<uint64_t>(x.begin()        , x.begin() + split));
    std::vector<uint64_t> x_hi(std::vector<uint64_t>(x.begin() + split, x.end()          ));
    
    std::vector<uint64_t> z_hi = karatsuba_old::square(x_hi);
    std::vector<uint64_t> z_lo = karatsuba_old::square(x_lo);
    std::vector<uint64_t> z_mi = karatsuba_old::mult(x_lo, x_hi);
    bshift::times2_r(z_mi);

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
}


void karatsuba::mult(const uint64_t *x, const std::size_t x_len,
                     const uint64_t *y, const std::size_t y_len,
                     uint64_t *dest, const std::size_t dest_len)
{
    if      (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return;
    else if (x_len == 1) gschool::mult_s(x[0], y, y_len, dest);
    else if (y_len == 1) gschool::mult_s(y[0], x, x_len, dest);
    else if (x_len <= MULT_KARATSUBA_CUTOFF && y_len <= MULT_KARATSUBA_CUTOFF) gschool::mult(x, x_len, y, y_len, dest);
    else
    {
        std::size_t split = std::max(x_len, y_len) >> 1;
        std::size_t x_lo_len = std::min(split, x_len);
        std::size_t y_lo_len = std::min(split, y_len);
        std::size_t x_hi_len = x_len - x_lo_len;
        std::size_t y_hi_len = y_len - y_lo_len;
        
        uint64_t* z_lo = dest;
        uint64_t* z_hi = dest + (2*split);
        
        mult(x,          x_lo_len, y,          y_lo_len, z_lo, dest_len);             // z_lo = x_lo * y_lo
        mult(x+x_lo_len, x_hi_len, y+y_lo_len, y_hi_len, z_hi, dest_len - (2*split)); // z_hi = x_hi * y_hi
        
        std::size_t x_plus_len = std::max(x_lo_len, x_hi_len) + 1;
        std::size_t y_plus_len = std::max(y_lo_len, y_hi_len) + 1;
        std::vector<uint64_t> x_plus(x_plus_len, 0);
        std::vector<uint64_t> y_plus(y_plus_len, 0);
        
        gschool::add(x, x_lo_len, x+x_lo_len, x_hi_len, x_plus.data()); // x_lo + x_hi
        gschool::add(y, y_lo_len, y+y_lo_len, y_hi_len, y_plus.data()); // y_lo + y_hi
        
        if (x_plus_len > 0 && x_plus[x_plus_len-1] == 0) x_plus_len--;
        if (y_plus_len > 0 && y_plus[y_plus_len-1] == 0) y_plus_len--;
        
        std::size_t z_mi_len = x_plus_len + y_plus_len;
        std::vector<uint64_t> z_mi(z_mi_len, 0);
        mult(x_plus.data(), x_plus_len, y_plus.data(), y_plus_len, z_mi.data(), z_mi_len); // (x_lo + x_hi) * (y_lo + y_hi)
        
        gschool::sub_r(z_mi.data(), z_mi_len, z_lo, 2*split);               // (x_lo + x_hi) * (y_lo + y_hi) - z_lo
        gschool::sub_r(z_mi.data(), z_mi_len, z_hi, dest_len - (2*split));  // (x_lo + x_hi) * (y_lo + y_hi) - z_lo - z_hi
                
        
        /*
           |....B^(3*split)|....B^(2*split)|........B^split|............B^0|
           |...........................z_hi|...........................z_lo|
           |...............|...........................z_mi|...............|
         */
        
        // |...............|...................z_mi+z_lo_hi|...............|
            gschool::add_r(z_mi.data(), z_mi_len,             dest        + split, split);
        // |...................z_hi+z_mi'_hi|...........................z_lo|
        if (z_mi_len > split) 
            gschool::add_r(z_hi,        dest_len - (2*split), z_mi.data() + split, z_mi_len - split);
        
        // z_hi*B^(2*split) + z_mi*B^split + z_lo
        std::copy(z_mi.data(), z_mi.data() + split, dest + split);
    }
}

std::vector<uint64_t> karatsuba::mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return {0};
    if (x_len == 1) return gschool::mult_s(x[0], y);
    if (y_len == 1) return gschool::mult_s(y[0], x);
    if (x_len <= MULT_KARATSUBA_CUTOFF && y_len <= MULT_KARATSUBA_CUTOFF) return gschool::mult(x, y);
    
    std::vector<uint64_t> z(x.size() + y.size(), 0);
    
    mult(x.data(), x.size(), y.data(), y.size(), z.data(), z.size());
    
    if (z.back() == 0) z.resize(z.size()-1);
    
    return z;
}

void karatsuba::square(const uint64_t *x, const std::size_t x_len, uint64_t *dest, const std::size_t dest_len)
{
    if      (x_len == 0 || (x_len == 1 && x[0] == 0)) return;
    else if (x_len <= MULT_KARATSUBA_CUTOFF) gschool::square(x, x_len, dest);
    else
    {
        std::size_t split = x_len >> 1;
        std::size_t x_lo_len = std::min(split, x_len);
        std::size_t x_hi_len = x_len - x_lo_len;
        
        uint64_t* z_lo = dest;
        uint64_t* z_hi = dest + (2*split);
        
        square(x,          x_lo_len, z_lo, dest_len);             // z_lo = x_lo ^ 2
        square(x+x_lo_len, x_hi_len, z_hi, dest_len - (2*split)); // z_hi = x_hi ^ 2
                
        std::vector<uint64_t> z_mi(x_lo_len + x_hi_len + 1, 0);             // +1 for the carry digit in the next bitshift
        mult(x, x_lo_len, x+x_lo_len, x_hi_len, z_mi.data(), z_mi.size());  // z_mi =     x_lo * x_hi
        bshift::times2_r(z_mi);                                             // z_mi = 2 * x_lo * x_hi
        std::size_t z_mi_len = z_mi.size();
        
        /*
           |....B^(3*split)|....B^(2*split)|........B^split|............B^0|
           |...........................z_hi|...........................z_lo|
           |...............|...........................z_mi|...............|
         */
        
        // |...............|...................z_mi+z_lo_hi|...............|
            gschool::add_r(z_mi.data(), z_mi_len,             dest        + split, split);
        // |...................z_hi+z_mi'_hi|...........................z_lo|
        if (z_mi_len > split)
            gschool::add_r(z_hi,        dest_len - (2*split), z_mi.data() + split, z_mi_len - split);
        
        // z_hi*B^(2*split) + z_mi*B^split + z_lo
        std::copy(z_mi.data(), z_mi.data() + split, dest + split);
    }
}

std::vector<uint64_t> karatsuba::square(const std::vector<uint64_t>& x)
{
    std::size_t x_len = x.size();
    
    if (x_len == 0 || (x_len == 1 && x[0] == 0)) return {0};
    if (x_len <= MULT_KARATSUBA_CUTOFF) return gschool::square(x);
    
    
    std::vector<uint64_t> z(2*x.size(), 0);
    
    square(x.data(), x.size(), z.data(), z.size());
    
    if (z.back() == 0) z.resize(z.size()-1);
    
    return z;
}
