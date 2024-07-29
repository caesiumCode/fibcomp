#include "bshift.hpp"

void bshift::times4_r(std::vector<uint64_t>& x)
{
    const std::size_t x_len = x.size();
    const uint64_t    mask  = 3ULL << 62;
        
    if (x.back() & mask) x.push_back(x.back() >> 62);
    
    for (std::size_t i = x_len-1; i > 0; i--)
    {
        x[i] <<= 2;
        x[i] |= (x[i-1] >> 62);
    }
    
    x[0] <<= 2;
}

void bshift::times2_r(std::vector<uint64_t>& x)
{
    const std::size_t x_len = x.size();
    const uint64_t    mask  = 1ULL << 63;
    
    if (x.back() & mask) x.push_back(1);
    
    for (std::size_t i = x_len-1; i > 0; i--)
    {
        x[i] <<= 1;
        x[i] |= (x[i-1] >> 63);
    }
    
    x[0] <<= 1;
}

void bshift::divide2_r(std::vector<uint64_t>& x)
{
    const std::size_t len = x.size();
    
    for (std::size_t i = 0; i < len-1; i++)
    {
        x[i] >>= 1;
        x[i] |= (x[i+1] << 63);
    }
    
    x.back() >>= 1;
    if (x.back() == 0 && len > 1) x.pop_back();
}

void bshift::divide4_r(std::vector<uint64_t>& x)
{
    const std::size_t len = x.size();
    
    for (std::size_t i = 0; i < len-1; i++)
    {
        x[i] >>= 2;
        x[i] |= (x[i+1] << 62);
    }
    
    x.back() >>= 2;
    if (x.back() == 0 && len > 1) x.pop_back();
}


std::vector<uint64_t> bshift::times2(std::vector<uint64_t> x)
{
    times2_r(x);
    return x;
}

void bshift::multiply_by_2_r(uint64_t* x, const std::size_t x_len)
{
    for (std::size_t i = x_len-1; i > 0; i--)
    {
        x[i] <<= 1;
        x[i] |= (x[i-1] >> 63);
    }
    
    x[0] <<= 1;
}

void bshift::divide_by_2_r(uint64_t* x, const std::size_t x_len)
{
    std::size_t i;
    for (i = 0; i < x_len-1; i++)
    {
        x[i] >>= 1;
        x[i] |= (x[i+1] << 63);
    }
    
    x[i] >>= 1;
}
