#include "cmp.hpp"

bool cmp::less_than(const std::vector<uint64_t> & x, const std::vector<uint64_t> & y)
{
    if (x.size() != y.size()) return x.size() < y.size();
    
    for (int i = (int)x.size()-1; i >= 0; i--) if (x[i] != y[i]) return x[i] < y[i];
    
    return false;
}
