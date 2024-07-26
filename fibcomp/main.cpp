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
#include "fibonacci_fmat_triangle.hpp"

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
    
    algos.push_back(std::make_shared<FibonacciRec>());
    //algos.push_back(std::make_shared<FibonacciRecMem>());
    algos.push_back(std::make_shared<FibonacciLin>());
    algos.push_back(std::make_shared<FibonacciMatRec>());
    //algos.push_back(std::make_shared<FibonacciMatLoop>());
    //algos.push_back(std::make_shared<FibonacciFmat>());
    //algos.push_back(std::make_shared<FibonacciFmatMem>());
    algos.push_back(std::make_shared<FibonacciFmatTriangle>());
    algos.push_back(std::make_shared<FibonacciBinet>());
    
    std::cout << std::setw(20) << "ALGORITHME" << " |" << std::setw(10) << "MAX N" << std::endl;
    std::cout << std::string(32, '-') << std::endl;
    for (std::shared_ptr<Fibonacci> algo : algos) disp_search(*algo.get(), time_limit);
    
    /*
    uint64_t n = 100000;
    FibonacciLin algo;
    uintinf_t ref = algo.run(n);
    for (std::shared_ptr<Fibonacci> algo : algos)
    {
        bool good = algo->run(n).is_equal(ref);
        std::cout << std::setw(20) << algo->name << std::setw(12) << (good ? "GOOD" : "BAD") << std::endl;
    }
     */
    
    //std::cout << res_1.to_string() << std::endl;
    //std::cout << res_2.to_string() << std::endl;
    
    /*
    FibonacciFmatMem algo;
    uint64_t n = 15000000;
    double T = 0;
    int N = 20;
    for (int i = 0; i < N; i++)
    {
        double t = running_time(algo, n);
        std::cout << t << std::endl;
        T += t;
    }
    std::cout << "avg : " << T / double(N) << std::endl;
    */
    
    /*
    FibonacciFmatMem algo;
    std::cout << running_time(algo, 13000000) << std::endl;
     */
    
    
    return EXIT_SUCCESS;
}
