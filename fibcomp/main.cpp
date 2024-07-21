#include <iostream>
#include <iomanip>
#include <chrono>

#include "fibonacci_rec.hpp"
#include "fibonacci_rec_mem.hpp"
#include "fibonacci_lin.hpp"
#include "fibonacci_mat.hpp"

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

int main(int argc, const char * argv[]) 
{
    double time_limit = 1;
    
    std::cout << std::setw(20) << "ALGORITHME" << " |" << std::setw(10) << "MAX N" << std::endl;
    std::cout << std::string(32, '-') << std::endl;
    /*
    FibonacciRec algo_rec;
    std::cout << std::setw(20) << "recursif-naif" << " |" << std::setw(10) << search_threshold(algo_rec, time_limit, true) << std::endl;
    
    //FibonacciRecMem algo_rec_mem;
    //std::cout << std::setw(20) << "recursif-memo" << " |" << std::setw(10) << search_threshold(algo_rec_mem, time_limit, true) << std::endl;
    
    FibonacciLin algo_lin;
    std::cout << std::setw(20) << "lineaire" << " |" << std::setw(10) << search_threshold(algo_lin, time_limit) << std::endl;
     */
    
    FibonacciMat algo_mat;
    std::cout << std::setw(20) << "matrice" << " |" << std::setw(10) << search_threshold(algo_mat, time_limit) << std::endl;
    //std::cout << algo_mat.run(100).to_string() << std::endl;
    
    return EXIT_SUCCESS;
}
