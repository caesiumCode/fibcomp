#include "tomcook.hpp"

std::vector<uint64_t> tomcook_old::mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return {0};
    else if (x_len == 1) return gschool::mult_s(x[0], y);
    else if (y_len == 1) return gschool::mult_s(y[0], x);
    else if (x_len <= MULT_KARATSUBA_CUTOFF  && y_len <= MULT_KARATSUBA_CUTOFF)  return gschool::mult(x, y);
    else if (x_len <= MULT_TOMCOOK_CUTOFF    && y_len <= MULT_TOMCOOK_CUTOFF)    return karatsuba::mult(x, y);
    else if (x_len >= 3*y_len)
    {
        std::size_t split = (std::max(x_len, y_len) + 2) / 3;
        
        // p(t) = x_2 t^2 + x_1 t + x_0
        std::size_t x0_len = std::min(split, x_len);
        std::size_t x1_len = std::min(split, x_len - x0_len);
        
        std::vector<uint64_t> x0(x.begin(),                     x.begin() + x0_len);
        std::vector<uint64_t> x1(x.begin() + x0_len,            x.begin() + x0_len + x1_len);
        std::vector<uint64_t> x2(x.begin() + x0_len + x1_len,   x.end());
        
        std::vector<uint64_t> z0 = mult(x0, y);
        std::vector<uint64_t> z1 = mult(x1, y);
        std::vector<uint64_t> z2 = mult(x2, y);
        
        std::size_t z_len = x_len + y_len;
        z0.resize(z_len, 0);
        gschool::add_r(z0.data() + split,   z_len - split,   z1.data(), z1.size());
        gschool::add_r(z0.data() + 2*split, z_len - 2*split, z2.data(), z2.size());
        
        if (z0.back() == 0) z0.pop_back();
        
        return z0;
    }
    else if (y_len >= 3*x_len) return mult(y, x);
    
    std::size_t split = (std::max(x_len, y_len) + 2) / 3;
    
    // p(t) = x_2 t^2 + x_1 t + x_0
    std::size_t x0_len = std::min(split, x_len);
    std::size_t x1_len = std::min(split, x_len - x0_len);
    
    std::vector<uint64_t> x0(x.begin(),                     x.begin() + x0_len);
    std::vector<uint64_t> x1(x.begin() + x0_len,            x.begin() + x0_len + x1_len);
    std::vector<uint64_t> x2(x.begin() + x0_len + x1_len,   x.end());
    
    // p(0), p(1), p(-1), p(-2), p(+inf)
    std::vector<uint64_t> tmp  = gschool::add(x0, x2);
    std::vector<uint64_t> p0   = x0;
    std::vector<uint64_t> p1   = gschool::add(tmp, x1);
    bool pm1_sgn;
    std::vector<uint64_t> pm1 = gschool::sub_sgn(false, tmp, false, x1, pm1_sgn);
    bool pm2_sgn = false;
    std::vector<uint64_t> pm2 = gschool::add_sgn(pm1_sgn, pm1, false, x2, pm2_sgn); // p(−2) = p(−1) + x2
    bshift::times2_r(pm2); // 2(x2 + p(-1))
    pm2 = gschool::sub_sgn(pm2_sgn, pm2, false, x0, pm2_sgn); // p(−2) = 2(p(−1) + x2) − x0
    
    std::vector<uint64_t> pinf = x2;
    
    // q(t) = y_2 t^2 + y_1 t + y_0
    std::size_t y0_len = std::min(split, y_len);
    std::size_t y1_len = std::min(split, y_len - y0_len);
    
    std::vector<uint64_t> y0(y.begin(),                     y.begin() + y0_len);
    std::vector<uint64_t> y1(y.begin() + y0_len,            y.begin() + y0_len + y1_len);
    std::vector<uint64_t> y2(y.begin() + y0_len + y1_len,   y.end());
    
    // q(0), q(1), q(-1), q(-2), q(+inf)
    std::vector<uint64_t> tmq  = gschool::add(y0, y2);
    std::vector<uint64_t> q0   = y0;
    std::vector<uint64_t> q1   = gschool::add(tmq, y1);
    bool qm1_sgn;
    std::vector<uint64_t> qm1 = gschool::sub_sgn(false, tmq, false, y1, qm1_sgn);
    bool qm2_sgn = false;
    std::vector<uint64_t> qm2 = gschool::add_sgn(qm1_sgn, qm1, false, y2, qm2_sgn); // p(−1) + y2
    bshift::times2_r(qm2); // 2(x2 + p(-1))
    qm2 = gschool::sub_sgn(qm2_sgn, qm2, false, y0, qm2_sgn); // 2(y2 + p(-1)) - y0
    
    std::vector<uint64_t> qinf = y2;
    
    // r(0), r(1), r(-1), r(-2), r(+inf)
    std::vector<uint64_t> r0   = mult(p0,   q0);
    std::vector<uint64_t> r1   = mult(p1,   q1);
    std::vector<uint64_t> rm1  = mult(pm1,  qm1);
    std::vector<uint64_t> rm2  = mult(pm2,  qm2);
    std::vector<uint64_t> rinf = mult(pinf, qinf);
    
    bool rm1_sgn = (pm1_sgn != qm1_sgn);
    bool rm2_sgn = (pm2_sgn != qm2_sgn);
    
    
    // interpolation
    std::vector<uint64_t> z0, z1, z2, z3, z4;
    bool z1_sgn, z2_sgn, z3_sgn;
    
    z0 = r0;
    z4 = rinf;
    z3 = gschool::sub_sgn(rm2_sgn, rm2, false, r1, z3_sgn);
    gschool::divide_by_3_r(z3);
    
    z1 = gschool::sub_sgn(false, r1, rm1_sgn, rm1, z1_sgn);
    bshift::divide2_r(z1);
    
    z2 = gschool::sub_sgn(rm1_sgn, rm1, false, r0, z2_sgn);
    
    z3 = gschool::sub_sgn(z2_sgn, z2, z3_sgn, z3, z3_sgn);
    bshift::divide2_r(z3);
    bshift::times2_r(rinf);
    z3 = gschool::add_sgn(z3_sgn, z3, false, rinf, z3_sgn);
    
    z2 = gschool::add_sgn(z2_sgn, z2, z1_sgn, z1, z2_sgn);
    z2 = gschool::sub_sgn(z2_sgn, z2, false, z4, z2_sgn);
    
    gschool::sub_r(z1, z3);
    
    // Recomposition
    
    std::size_t z_len = x_len + y_len;
    z0.resize(z_len, 0);
    gschool::add_r(z0.data() + split,   z_len - split,   z1.data(), z1.size());
    gschool::add_r(z0.data() + 2*split, z_len - 2*split, z2.data(), z2.size());
    gschool::add_r(z0.data() + 3*split, z_len - 3*split, z3.data(), z3.size());
    gschool::add_r(z0.data() + 4*split, z_len - 4*split, z4.data(), z4.size());
    
    if (z0.back() == 0) z0.pop_back();
    
    return z0;
}



std::vector<uint64_t> tomcook::mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
{
    std::size_t x_len = x.size();
    std::size_t y_len = y.size();
    
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return {0};
    else if (x_len == 1) return gschool::mult_s(x[0], y);
    else if (y_len == 1) return gschool::mult_s(y[0], x);
    else if (x_len <= MULT_KARATSUBA_CUTOFF && y_len <= MULT_KARATSUBA_CUTOFF) return gschool::mult(x, y);
    else if (x_len <= MULT_TOMCOOK_CUTOFF   && y_len <= MULT_TOMCOOK_CUTOFF)   return karatsuba::mult(x, y);
    
    std::vector<uint64_t> z(x_len + y_len, 0);
    
    mult(x.data(), x_len, y.data(), y_len, z.data());
    
    if (z.back() == 0) z.pop_back();
    
    return z;
}


void tomcook::mult(const uint64_t* x, const std::size_t x_len, const uint64_t* y, const std::size_t y_len, uint64_t* dest)
{
    if (x_len == 0 || y_len == 0 || (x_len == 1 && x[0] == 0) || (y_len == 1 && y[0] == 0)) return;
    else if (x_len == 1) gschool::mult_s(x[0], y, y_len, dest);
    else if (y_len == 1) gschool::mult_s(y[0], x, x_len, dest);
    else if (x_len <= MULT_KARATSUBA_CUTOFF  && y_len <= MULT_KARATSUBA_CUTOFF) gschool::mult(x, x_len, y, y_len, dest);
    else if (x_len <= MULT_TOMCOOK_CUTOFF    && y_len <= MULT_TOMCOOK_CUTOFF)   karatsuba::mult(x, x_len, y, y_len, dest);
    else if (x_len >= 3*y_len)
    {
        std::size_t z_len = x_len + y_len;
        std::size_t split = (std::max(x_len, y_len) + 2) / 3;
        
        std::size_t x0_len = std::min(split, x_len);
        std::size_t x1_len = std::min(split, x_len - x0_len);
        std::size_t x2_len = x_len - x0_len - x1_len;
        
        const uint64_t* x0 = x;
        const uint64_t* x1 = x0 + x0_len;
        const uint64_t* x2 = x1 + x1_len;
        
        mult(x0, x0_len, y, y_len, dest);
        mult(x2, x2_len, y, y_len, dest + (2*split));
        
        std::vector<uint64_t> z1(x1_len + y_len, 0);
        mult(x1, x1_len, y, y_len, z1.data());
        
        gschool::add_r(dest + split, z_len - split, z1.data(), z1.size());
    }
    else if (y_len >= 3*x_len) return mult(y, y_len, x, x_len, dest);
    else
    {
        std::size_t z_len = x_len + y_len;
        std::size_t split = (std::max(x_len, y_len) + 2) / 3;
        
        // p(t) = x_2 t^2 + x_1 t + x_0
        std::size_t x0_len = std::min(split, x_len);
        std::size_t x1_len = std::min(split, x_len - x0_len);
        std::size_t x2_len = x_len - x0_len - x1_len;
        
        const uint64_t* x0 = x;
        const uint64_t* x1 = x0 + x0_len;
        const uint64_t* x2 = x1 + x1_len;
        
        
        // p(0), p(1), p(-1), p(-2), p(+inf)
        std::vector<uint64_t> tmp(x0_len + 1, 0);
        gschool::add(x0, x0_len, x2, x2_len, tmp.data());
        
        std::vector<uint64_t> p1(tmp.size() + 1, 0);
        gschool::add(tmp.data(), tmp.size(), x1, x1_len, p1.data());
        
        bool pm1_sgn;
        std::vector<uint64_t> pm1(tmp.size(), 0);
        gschool::sub_sgn(false, tmp.data(), tmp.size(), false, x1, x1_len, pm1_sgn, pm1.data());
        
        bool pm2_sgn;
        std::vector<uint64_t> pm2(pm1.size() + 2, 0);
        gschool::add_sgn(pm1_sgn, pm1.data(), pm1.size(), false, x2, x2_len, pm2_sgn, pm2.data());
        bshift::multiply_by_2_r(pm2.data(), pm2.size());
        gschool::sub_sgn(pm2_sgn, pm2.data(), pm2.size(), false, x0, x0_len, pm2_sgn, pm2.data());
        
        // q(t) = y_2 t^2 + y_1 t + y_0
        std::size_t y0_len = std::min(split, y_len);
        std::size_t y1_len = std::min(split, y_len - y0_len);
        std::size_t y2_len = y_len - y0_len - y1_len;
        
        const uint64_t* y0 = y;
        const uint64_t* y1 = y0 + y0_len;
        const uint64_t* y2 = y1 + y1_len;
        
        
        // q(0), q(1), q(-1), q(-2), q(+inf)
        std::vector<uint64_t> tmq(y0_len + 1, 0);
        gschool::add(y0, y0_len, y2, y2_len, tmq.data());
        
        std::vector<uint64_t> q1(tmq.size() + 1, 0);
        gschool::add(tmq.data(), tmq.size(), y1, y1_len, q1.data());
        
        bool qm1_sgn;
        std::vector<uint64_t> qm1(tmq.size(), 0);
        gschool::sub_sgn(false, tmq.data(), tmq.size(), false, y1, y1_len, qm1_sgn, qm1.data());
         
        bool qm2_sgn;
        std::vector<uint64_t> qm2(qm1.size() + 2, 0);
        gschool::add_sgn(qm1_sgn, qm1.data(), qm1.size(), false, y2, y2_len, qm2_sgn, qm2.data());
        bshift::multiply_by_2_r(qm2.data(), qm2.size());
        gschool::sub_sgn(qm2_sgn, qm2.data(), qm2.size(), false, y0, y0_len, qm2_sgn, qm2.data());
        
        // r(0), r(1), r(-1), r(-2), r(+inf)
        std::size_t rinf_len = (4*split <= z_len) ? z_len - (4*split) : 0;
        uint64_t* r0   = dest;
        uint64_t* rinf = dest + (4*split);
        mult(x0, x0_len, y0, y0_len, r0);
        mult(x2, x2_len, y2, y2_len, rinf);
        
        std::vector<uint64_t> r1(p1.size() + q1.size(), 0);
        std::vector<uint64_t> rm1(pm1.size() + qm1.size(), 0);
        std::vector<uint64_t> rm2(pm2.size() + qm2.size(), 0);
        
        mult(p1.data(), p1.size(), q1.data(), q1.size(), r1.data());
        mult(pm1.data(), pm1.size(), qm1.data(), qm1.size(), rm1.data());
        mult(pm2.data(), pm2.size(), qm2.data(), qm2.size(), rm2.data());
        
        cmp::trim(r1);
        cmp::trim(rm1);
        cmp::trim(rm2);
        
        bool rm1_sgn = (pm1_sgn != qm1_sgn);
        bool rm2_sgn = (pm2_sgn != qm2_sgn);
        
        
        // interpolation
        std::vector<uint64_t> z1(2*split + 1, 0);
        std::vector<uint64_t> z2(2*split + 2, 0);
        std::vector<uint64_t> z3(2*split + 1, 0);
        bool z1_sgn, z2_sgn, z3_sgn;
        
        gschool::sub_sgn(rm2_sgn, rm2.data(), rm2.size(), false, r1.data(), r1.size(), z3_sgn, z3.data());
        gschool::divide_by_3_r(z3.data(), z3.size());
        
        gschool::sub_sgn(false, r1.data(), r1.size(), rm1_sgn, rm1.data(), rm1.size(), z1_sgn, z1.data());
        bshift::divide_by_2_r(z1.data(), z1.size());
        
        gschool::sub_sgn(rm1_sgn, rm1.data(), rm1.size(), false, r0, x0_len + y0_len, z2_sgn, z2.data());
        
        gschool::sub_sgn(z2_sgn, z2.data(), z2.size(), z3_sgn, z3.data(), z3.size(), z3_sgn, z3.data());
        bshift::divide_by_2_r(z3.data(), z3.size());
        
        gschool::add_sgn(z3_sgn, z3.data(), z3.size(), false, rinf, rinf_len, z3_sgn, z3.data());
        gschool::add_sgn(z3_sgn, z3.data(), z3.size(), false, rinf, rinf_len, z3_sgn, z3.data());
        
        gschool::add_sgn(z2_sgn, z2.data(), z2.size(), z1_sgn, z1.data(), z1.size(), z2_sgn, z2.data());
        gschool::sub_sgn(z2_sgn, z2.data(), z2.size(), false, rinf, rinf_len, z2_sgn, z2.data());
        
        gschool::sub_r(z1.data(), z1.size(), z3.data(), z3.size());
        
        // Recomposition
        gschool::add_r(dest + split,   z_len - split,   z1.data(), z1.size());
        gschool::add_r(dest + 2*split, z_len - 2*split, z2.data(), z2.size());
        gschool::add_r(dest + 3*split, z_len - 3*split, z3.data(), z3.size());
    }
}

std::vector<uint64_t> tomcook::square(const std::vector<uint64_t>& x)
{
    std::size_t x_len = x.size();
    
    if (x_len == 0 || (x_len == 1 && x[0] == 0)) return {0};
    else if (x_len <= SQR_KARATSUBA_CUTOFF) return gschool::square(x);
    else if (x_len <= SQR_TOMCOOK_CUTOFF)   return karatsuba::square(x);
    
    std::vector<uint64_t> z(2*x_len, 0);
    
    square(x.data(), x_len, z.data());
    
    if (z.back() == 0) z.pop_back();
    
    return z;
}


void tomcook::square(const uint64_t* x, const std::size_t x_len, uint64_t* dest)
{
    if (x_len == 0 || (x_len == 1 && x[0] == 0)) return;
    else if (x_len <= SQR_KARATSUBA_CUTOFF) gschool::square(x, x_len, dest);
    else if (x_len <= SQR_TOMCOOK_CUTOFF)   karatsuba::square(x, x_len, dest);
    else
    {
        std::size_t z_len = 2*x_len;
        std::size_t split = (x_len + 2) / 3;
        
        // p(t) = x_2 t^2 + x_1 t + x_0
        std::size_t x0_len = std::min(split, x_len);
        std::size_t x1_len = std::min(split, x_len - x0_len);
        std::size_t x2_len = x_len - x0_len - x1_len;
        
        const uint64_t* x0 = x;
        const uint64_t* x1 = x0 + x0_len;
        const uint64_t* x2 = x1 + x1_len;
        
        // p(0), p(1), p(-1), p(-2), p(+inf)
        std::vector<uint64_t> tmp(x0_len + 1, 0);
        gschool::add(x0, x0_len, x2, x2_len, tmp.data());
        
        std::vector<uint64_t> p1(tmp.size() + 1, 0);
        gschool::add(tmp.data(), tmp.size(), x1, x1_len, p1.data());
        
        bool pm1_sgn;
        std::vector<uint64_t> pm1(tmp.size(), 0);
        gschool::sub_sgn(false, tmp.data(), tmp.size(), false, x1, x1_len, pm1_sgn, pm1.data());
        
        bool pm2_sgn;
        std::vector<uint64_t> pm2(tmp.size() + 2, 0);
        gschool::add_sgn(pm1_sgn, pm1.data(), pm1.size(), false, x2, x2_len, pm2_sgn, pm2.data());
        bshift::multiply_by_2_r(pm2.data(), pm2.size());
        gschool::sub_sgn(pm2_sgn, pm2.data(), pm2.size(), false, x0, x0_len, pm2_sgn, pm2.data());
        
        // r(0), r(1), r(-1), r(-2), r(+inf)
        std::size_t rinf_len = (4*split <= z_len) ? z_len - (4*split) : 0;
        uint64_t* r0   = dest;
        uint64_t* rinf = dest + (4*split);
        square(x0, x0_len, r0);
        square(x2, x2_len, rinf);
        
        std::vector<uint64_t> r1(2*p1.size(), 0);
        std::vector<uint64_t> rm1(2*pm1.size(), 0);
        std::vector<uint64_t> rm2(2*pm2.size(), 0);
        
        square(p1.data(), p1.size(), r1.data());
        square(pm1.data(), pm1.size(), rm1.data());
        square(pm2.data(), pm2.size(), rm2.data());
        
        cmp::trim(r1);
        cmp::trim(rm1);
        cmp::trim(rm2);
        
        // interpolation
        std::vector<uint64_t> z1(2*split + 1, 0);
        std::vector<uint64_t> z2(2*split + 2, 0);
        std::vector<uint64_t> z3(2*split + 1, 0);
        bool z1_sgn, z2_sgn, z3_sgn;
        
        gschool::sub_sgn(false, rm2.data(), rm2.size(), false, r1.data(), r1.size(), z3_sgn, z3.data());
        gschool::divide_by_3_r(z3.data(), z3.size());
        
        gschool::sub_sgn(false, r1.data(), r1.size(), false, rm1.data(), rm1.size(), z1_sgn, z1.data());
        bshift::divide_by_2_r(z1.data(), z1.size());
        
        gschool::sub_sgn(false, rm1.data(), rm1.size(), false, r0, 2*split, z2_sgn, z2.data());
        
        gschool::sub_sgn(z2_sgn, z2.data(), z2.size(), z3_sgn, z3.data(), z3.size(), z3_sgn, z3.data());
        bshift::divide_by_2_r(z3.data(), z3.size());
        
        gschool::add_sgn(z3_sgn, z3.data(), z3.size(), false, rinf, rinf_len, z3_sgn, z3.data());
        gschool::add_sgn(z3_sgn, z3.data(), z3.size(), false, rinf, rinf_len, z3_sgn, z3.data());
        
        gschool::add_sgn(z2_sgn, z2.data(), z2.size(), z1_sgn, z1.data(), z1.size(), z2_sgn, z2.data());
        gschool::sub_sgn(z2_sgn, z2.data(), z2.size(), false, rinf, rinf_len, z2_sgn, z2.data());
        
        gschool::sub_r(z1.data(), z1.size(), z3.data(), z3.size());
        
        // Recomposition
        gschool::add_r(dest + split,   z_len - split,   z1.data(), z1.size());
        gschool::add_r(dest + 2*split, z_len - 2*split, z2.data(), z2.size());
        gschool::add_r(dest + 3*split, z_len - 3*split, z3.data(), z3.size());
    }
}
