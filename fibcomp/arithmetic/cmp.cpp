#include "cmp.hpp"

void cmp::trim(std::vector<uint64_t>& x)
{
    std::size_t len = x.size();
    while (len > 1 && x[len-1] == 0) len--;
    
    x.resize(len, 0);
}

bool cmp::less_than(const std::vector<uint64_t> & x, const std::vector<uint64_t> & y)
{
    if (x.size() != y.size()) return x.size() < y.size();
    
    for (int i = (int)x.size()-1; i >= 0; i--) if (x[i] != y[i]) return x[i] < y[i];
    
    return false;
}

bool cmp::less_than(const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len)
{
    bool x_ref = (x_len >= y_len);
    
    const uint64_t* l = x_ref ? x : y;
    const uint64_t* s = x_ref ? y : x;
    
    std::size_t l_len = x_ref ? x_len : y_len;
    std::size_t s_len = x_ref ? y_len : x_len;
    
    int i;
    for (i = (int)l_len-1; i >= s_len; i--) if (l[i] > 0)     return !x_ref;
    for (                ; i >= 0;     i--) if (l[i] != s[i]) return x_ref ? l[i] < s[i] : l[i] > s[i];
        
    return false;
}
