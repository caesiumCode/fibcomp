#include <iostream>
#include <iomanip>
#include <chrono>
#include <memory>

#include "fibonacci_rec.hpp"
#include "fibonacci_rec_mem.hpp"
#include "fibonacci_lin.hpp"
#include "fibonacci_mat_rec.hpp"
#include "fibonacci_mat_loop.hpp"
#include "fibonacci_fmat.hpp"
#include "fibonacci_fmat_mem.hpp"
#include "fibonacci_binet.hpp"

BinetCouple binet_power(uint64_t);

double running_time(Fibonacci& algo, uint64_t n)
{
    const auto start = std::chrono::steady_clock::now();
    const uintinf_t F =  algo.run(n);
    const auto end = std::chrono::steady_clock::now();
    
    return std::chrono::duration<double>(end - start).count();
}

uint64_t search_threshold(Fibonacci& algo, double time_limit, bool slow = false)
{
    uint64_t low  = 0;
    uint64_t high = 1;
    
    while (running_time(algo, high) < time_limit)
    {
        low  = high;
        high = (slow ? high+2 : high << 1);
    }
        
    while (high - low > 1)
    {
        uint64_t mid = (high + low) / 2;
        double t = running_time(algo, mid);
        
        if (t < time_limit) low  = mid;
        else                high = mid;        
    }
    
    return low;
}

void disp_search(Fibonacci& algo, double time_limit)
{
    std::cout << std::setw(20) << algo.name << " |" << std::setw(10) << search_threshold(algo, time_limit, algo.slow) << std::endl;
}

int main(int argc, const char * argv[]) 
{
    double time_limit = 1;
    
    std::vector<std::shared_ptr<Fibonacci>> algos;
    
    //algos.push_back(std::make_shared<FibonacciRec>());
    //algos.push_back(std::make_shared<FibonacciRecMem>());
    //algos.push_back(std::make_shared<FibonacciLin>());
    //algos.push_back(std::make_shared<FibonacciMatRec>());
    algos.push_back(std::make_shared<FibonacciMatLoop>());
    //algos.push_back(std::make_shared<FibonacciFmat>());
    algos.push_back(std::make_shared<FibonacciFmatMem>());
    algos.push_back(std::make_shared<FibonacciBinet>());
    
    std::cout << std::setw(20) << "ALGORITHME" << " |" << std::setw(10) << "MAX N" << std::endl;
    std::cout << std::string(32, '-') << std::endl;
    for (std::shared_ptr<Fibonacci> algo : algos) disp_search(*algo.get(), time_limit);

    
    /*
    FibonacciLin algo;
    std::cout << algo.run(1000).to_string() << std::endl;
    FibonacciBinet algo2;
    std::cout << algo2.run(1000).to_string() << std::endl;
    */
    
    /*
    uintinf_t x({15936873492946612855ul,9774062212524773964ul,17647631051235622973ul});
    uintinf_t y({917629945586240594ul,11697812875968936183ul,6675791983397549555ul});
    
    std::cout << x.to_string() << " - " << y.to_string() << " = " << (x-y).to_string() << std::endl;
     */
    /*
    uintinf_t x({14187903849909755555ul,12700963192829829547ul,13618500700654901798ul,2178911929463441851ul,9309698343727878080ul,8721246518062843745ul,1318384419796813901ul,14101981235888899220ul,7601339296691187348ul,7587641507671754210ul,525820222474136895ul,6073407550217382419ul,4049358921189102811ul,3986792779959091855ul,11067510043033632972ul});
    uintinf_t y({3577628452150881636ul,244686181740636949ul,9255254066655978353ul,12306101048976605471ul,1421504085468624812ul});
    
    std::cout << x.to_string() << " x " << y.to_string() << " = " << (x*y).to_string() << std::endl;
     */
    
    
    return EXIT_SUCCESS;
}
