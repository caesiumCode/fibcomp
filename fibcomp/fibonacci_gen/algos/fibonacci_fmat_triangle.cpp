#include "fibonacci_fmat_triangle.hpp"

FibonacciFmatTriangle::FibonacciFmatTriangle()
: Fibonacci("flatmatrice-triangle", false)
{
    cache_small[0] = 0;
    cache_small[1] = 1;
    for (std::size_t i = 2; i < 94; i++) cache_small[i] = cache_small[i-1] + cache_small[i-2];
}

uintinf_t FibonacciFmatTriangle::run(uint64_t n)
{
    if (n <= 93) return cache_small[n];
    
    std::vector<uint64_t> left  = {n};
    std::vector<uint64_t> right = {n};
    
    //compute layer intervals
    std::size_t idx = 0;
    while (left.back() >= 93)
    {
        left.push_back(rho_left(left.back()));
        right.push_back(rho_right(right.back()));
        
        idx++;
    }
    
    std::vector<uintinf_t> layer_top(right.back() - left.back() + 1, 0);
    std::vector<uintinf_t> layer    (right.back() - left.back() + 1, 0);
    
    //compute bottom layer
    layer[0] = cache_small[left.back()];
    layer[1] = cache_small[left.back()+1];
    for (std::size_t i = 2; i < layer.size(); i++) layer[i] = layer[i-1] + layer[i-2];
    idx--;
    
    //compute mid layers
    while (idx > 0)
    {
        uint64_t n_ref_top = left[idx];
        uint64_t offset    = left[idx+1];
        if (n_ref_top%2)
        {
            layer_top[0] = square(layer[(n_ref_top-1)/2 - offset]) + square(layer[(n_ref_top+1)/2 - offset]);
            
            n_ref_top++;
            uintinf_t F = layer[n_ref_top/2 - offset];
            layer_top[1] = (twice(layer[n_ref_top/2 - 1 - offset]) + F) * F;
        }
        else
        {
            uintinf_t F = layer[n_ref_top/2 - offset];
            layer_top[0] = (twice(layer[n_ref_top/2 - 1 - offset]) + F) * F;
            
            n_ref_top++;
            layer_top[1] = square(layer[(n_ref_top-1)/2 - offset]) + square(layer[(n_ref_top+1)/2 - offset]);
        }
        
        const uint64_t top_len = right[idx] - left[idx] + 1;
        for (std::size_t i = 2; i < top_len; i++) layer_top[i] = layer_top[i-1] + layer_top[i-2];
        
        std::swap(layer, layer_top);
        idx--;
    }
    
    //compute top layer
    uint64_t offset = left[idx+1];
    if (n%2)
    {
        return square(layer[(n-1)/2 - offset]) + square(layer[(n+1)/2 - offset]);
    }
    else
    {
        uintinf_t F = layer[n/2 - offset];
        return (twice(layer[n/2 - 1 - offset]) + F) * F;
    }
}

uint64_t FibonacciFmatTriangle::rho_left(uint64_t n)
{
    return (n%2) ? (n-1)/2 : n/2-1;
}

uint64_t FibonacciFmatTriangle::rho_right(uint64_t n)
{
    return (n%2) ? (n+1)/2 : n/2;
}
