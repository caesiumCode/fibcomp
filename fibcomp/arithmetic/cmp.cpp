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
    if (x_len >= y_len)
    {
        int i;
        for (i = (int)x_len-1; i >= y_len; i--) if (x[i] > 0)     return false;
        for (                ; i >= 0;     i--) if (x[i] != y[i]) return x[i] < y[i];
            
        return false;
    }
    else
    {
        int i;
        for (i = (int)y_len-1; i >= x_len; i--) if (y[i] > 0)     return true;
        for (                ; i >= 0;     i--) if (x[i] != y[i]) return x[i] < y[i];
            
        return false;
    }
}

bool cmp::less_than(const uint64_t* x, const std::size_t x_len, const uint64_t x_top, const uint64_t* y, const std::size_t y_len, const uint64_t y_top)
{
    if (x_len == y_len)
    {
        if (x_top != y_top) return x_top < y_top;
        
        int i;
        for (i = (int)x_len-1; i >= 0; i--) if (x[i] != y[i]) return x[i] < y[i];
            
        return false;
    }
    else if (x_len > y_len)
    {
        if (x_top > 0) return false;
        
        int i;
        for (i = (int)x_len-1; i > y_len; i--) if (x[i] > 0)    return false;
        if (x[i] != y_top)                                      return x[i] < y_top;
        i--;
        for (; i >= 0; i--) if (x[i] != y[i])                   return x[i] < y[i];
            
        return false;
    }
    else
    {
        if (y_top > 0) return true;
        
        int i;
        for (i = (int)y_len-1; i > x_len; i--) if (y[i] > 0)    return true;
        if (y[i] != x_top)                                      return x_top < y[i];
        i--;
        for (; i >= 0; i--) if (x[i] != y[i])                   return x[i] < y[i];
            
        return false;
    }
}
