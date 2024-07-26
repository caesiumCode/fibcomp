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

void find_thresholds(double time_limit = 1)
{
    std::vector<std::shared_ptr<Fibonacci>> algos;
    
    algos.push_back(std::make_shared<FibonacciRec>());
    algos.push_back(std::make_shared<FibonacciLin>());
    algos.push_back(std::make_shared<FibonacciMatRec>());
    //algos.push_back(std::make_shared<FibonacciMatLoop>());
    //algos.push_back(std::make_shared<FibonacciFmat>());
    algos.push_back(std::make_shared<FibonacciFmatMem>());
    algos.push_back(std::make_shared<FibonacciFmatTriangle>());
    algos.push_back(std::make_shared<FibonacciBinet>());
    
    std::cout << std::setw(20) << "ALGORITHME" << " |" << std::setw(10) << "MAX N" << std::endl;
    std::cout << std::string(32, '-') << std::endl;
    for (std::shared_ptr<Fibonacci> algo : algos) disp_search(*algo.get(), time_limit);
}

void    compare_to_ref();
double  running_time_high_precision(Fibonacci&, uint64_t, uint64_t = 10);

int main(int argc, const char * argv[]) 
{
    find_thresholds();
    
    //compare_to_ref();
    
    //FibonacciFmatTriangle algo;
    //std::cout << running_time_high_precision(algo, 15000000, 100) << "s" << std::endl;;
    
    return EXIT_SUCCESS;
}

void compare_to_ref()
{
    FibonacciLin algo_ref;
    
    std::vector<std::shared_ptr<Fibonacci>> algos;
    algos.push_back(std::make_shared<FibonacciMatRec>());
    algos.push_back(std::make_shared<FibonacciMatLoop>());
    algos.push_back(std::make_shared<FibonacciFmat>());
    algos.push_back(std::make_shared<FibonacciFmatMem>());
    algos.push_back(std::make_shared<FibonacciFmatTriangle>());
    algos.push_back(std::make_shared<FibonacciBinet>());
    
    std::cout << std::setw(10) << "n";
    for (std::shared_ptr<Fibonacci> algo : algos)
    {
        std::cout << std::setw(21) << algo->name;
    }
    std::cout << std::endl;
    
    for (int i = 0; i < 100; i++)
    {
        uint64_t n = rand() % 300000;
        
        uintinf_t ref = algo_ref.run(n);
        
        std::cout << std::setw(10) << n;
        for (std::shared_ptr<Fibonacci> algo : algos)
        {
            bool good = algo->run(n).is_equal(ref);
            std::cout << std::setw(21) << (good ? "GOOD" : "X");
        }
        
        std::cout << std::endl;
    }
}

double running_time_high_precision(Fibonacci& algo, uint64_t n, uint64_t N)
{
    double T = 0;
    for (std::size_t i = 0; i < N; i++) T += running_time(algo, n);
    
    return T / double(N);
}
