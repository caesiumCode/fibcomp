#include "tomcook.hpp"

std::vector<uint64_t> tomcook::mult(const std::vector<uint64_t>& x, const std::vector<uint64_t>& y)
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
