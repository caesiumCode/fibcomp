#ifndef uintinf_t_hpp
#define uintinf_t_hpp

#include <stdint.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

class uintinf_t
{
public:
    uintinf_t();
    uintinf_t(uint64_t);
    uintinf_t(const std::vector<uint64_t>&);
    
    bool is_zero();
    
    uintinf_t& operator+=(const uintinf_t&);
    uintinf_t& operator-=(const uintinf_t&);
    friend uintinf_t operator+(      uintinf_t,  const uintinf_t&);
    friend uintinf_t operator-(      uintinf_t,  const uintinf_t&);
    friend uintinf_t operator*(const uintinf_t&, const uintinf_t&);
    friend uintinf_t operator*(      uint64_t,         uintinf_t);
    
    void quadrupling();
    void doubling();
    void halving();
    void fourthing();
    
    friend uintinf_t twice(uintinf_t);
    friend uintinf_t half(uintinf_t);
    friend uintinf_t square(const uintinf_t&);
    
    friend void swap(uintinf_t&, uintinf_t&);
    
    std::string to_string() const;
    uint64_t    to_uint64() const;
    
public:
    static const uint64_t max_digit = 18446744073709551615ul;
    static const uint8_t  log2_base = 64;
            
private:
    std::vector<uint64_t> m_digits;
    
private:
    // Assume destination is big enough to store overflow
    // Assume pure destination is filled with zeros
    static void relative_long_add(      uint64_t* x, const std::size_t x_len,
                                  const uint64_t* y, const std::size_t y_len);
    static void relative_long_sub(      uint64_t* x, const std::size_t x_len,
                                  const uint64_t* y, const std::size_t y_len);
    
    static void long_add(const  uint64_t* x, const std::size_t x_len,
                         const  uint64_t* y, const std::size_t y_len,
                         uint64_t* dest);
    
    static void long_mult(const uint64_t* x, const std::size_t x_len,
                          const uint64_t* y, const std::size_t y_len,
                          uint64_t* dest);
    static void kara_mult(const uint64_t* x, const std::size_t x_len,
                          const uint64_t* y, const std::size_t y_len,
                          uint64_t* dest);
    static void long_square(const uint64_t* x, const std::size_t x_len,
                            uint64_t* dest);
    static void kara_square(const uint64_t* x, const std::size_t x_len,
                            uint64_t* dest);
    static void scalar_mult(const uint64_t scalar,
                            const uint64_t* x, const std::size_t x_len,
                            uint64_t* dest);

    
    static void relative_quadrupling(std::vector<uint64_t>&);
    static void relative_doubling(std::vector<uint64_t>&);
    static void relative_halving(std::vector<uint64_t>&);
    static void relative_fourthing(std::vector<uint64_t>&);

    static std::vector<uint64_t>& relative_long_add(std::vector<uint64_t>&, const std::vector<uint64_t>&);
    static std::vector<uint64_t>& relative_long_sub(std::vector<uint64_t>&, const std::vector<uint64_t>&);

    static std::vector<uint64_t> long_add  (std::vector<uint64_t>, const std::vector<uint64_t>&);
    static std::vector<uint64_t> long_sub  (std::vector<uint64_t>, const std::vector<uint64_t>&);

    static std::vector<uint64_t> kara_mult  (const std::vector<uint64_t>&, const std::vector<uint64_t>&);
    static std::vector<uint64_t> kara_square(const std::vector<uint64_t>&);
    static std::vector<uint64_t> scalar_mult(uint64_t, std::vector<uint64_t>);
    static std::vector<uint64_t> long_mult  (const std::vector<uint64_t>&, const std::vector<uint64_t>&);
    static std::vector<uint64_t> long_square(const std::vector<uint64_t>&);
};

#endif /* uintinf_t_hpp */
