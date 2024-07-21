#ifndef uintinf_t_hpp
#define uintinf_t_hpp

#include <stdint.h>
#include <vector>
#include <string>

class uintinf_t
{
public:
    uintinf_t();
    uintinf_t(uint64_t);
    uintinf_t(const std::vector<uint64_t>&);
    
    bool is_zero();
    
    uintinf_t& operator+=(const uintinf_t&);
    //uintinf_t& operator*=(const uintinf_t&);
    friend uintinf_t operator+(      uintinf_t,  const uintinf_t&);
    friend uintinf_t operator*(const uintinf_t&, const uintinf_t&);
    
    friend uintinf_t doubling(uintinf_t);
    friend uintinf_t square  (const uintinf_t&);
    
    friend void swap(uintinf_t&, uintinf_t&);
    
    std::string to_string() const;
    uint64_t    to_uint64() const;
    
public:
    static const uint64_t max_digit = 18446744073709551615ul;
    static const uint8_t  log2_base = 64;
            
private:
    std::vector<uint64_t> m_digits;
};

#endif /* uintinf_t_hpp */
