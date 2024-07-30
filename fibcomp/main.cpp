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
        
    while (high - low > 1 + low/10000ULL)
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
void    test_mult_algo(int n);
void    test_square_algo(int n);
double  running_time_mult(int n);
double  running_time_square(int n);

int main(int argc, const char * argv[]) 
{
    /*
    std::vector<uint64_t> x = {1699028698ULL, 485256320ULL, 3855046228ULL, 18446744071392036848ULL, 18446744072149751478ULL, 18446744072113964571ULL};
    std::vector<uint64_t> y = {5971734733ULL, 12887332767ULL, 18446744067320795577ULL, 18446744072240093012ULL, 3166167172ULL, 18446744067786812737ULL};
    
    std::vector<uint64_t> z_test = tomcook_new::mult(x, y);
    std::vector<uint64_t> z_ref = gschool::mult(x, y);
    
    std::cout << uintinf_t(z_test).to_string() << std::endl << std::endl;
    std::cout << uintinf_t(z_ref).to_string() << std::endl;
    */
     
    /*
    std::vector<uint64_t> x = {18446744072410102229ULL, 62842680ULL, 18446744071197053212ULL, 18446744072119826548ULL, 481772383ULL, 1135108698ULL};
    std::vector<uint64_t> y = {1123474284ULL, 1556066764ULL, 758249382ULL, 18446744072919408941ULL, 110934322ULL, 457361065ULL};
    
    std::vector<uint64_t> z_test = tomcook::mult(x, y);
    std::vector<uint64_t> z_ref = gschool::mult(x, y);
    
    std::cout << uintinf_t(z_test).to_string() << std::endl << std::endl;
    std::cout << uintinf_t(z_ref).to_string() << std::endl;
*/
    
    //std::cout << running_time_mult(100) << std::endl;
    test_mult_algo(1000);
    
    test_square_algo(1000);
    //std::cout << running_time_square(100) << std::endl;
    
    /*
    for (int n = 0; n <= 10; n++)
    {
        std::cout << MULT_KARATSUBA_CUTOFF << "," << running_time_square(10000) << std::endl;
    }*/
    
    find_thresholds(1);
    
    //compare_to_ref();
    
    //FibonacciFmatTriangle algo;
    //std::cout << running_time_high_precision(algo, 20000000, 1) << "s" << std::endl;
    
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
    
    bool ONE_ERROR = false;
    
    for (int i = 0; i < 1000; i++)
    {
        uint64_t n = rand() % 300000;
        
        uintinf_t ref = algo_ref.run(n);
        
        std::cout << std::setw(10) << n;
        for (std::shared_ptr<Fibonacci> algo : algos)
        {
            bool good = algo->run(n).is_equal(ref);
            std::cout << std::setw(21) << (good ? "GOOD" : "X");
            
            ONE_ERROR = good ? ONE_ERROR : true;
        }
        
        std::cout << std::endl;
    }
    
    std::cout << (ONE_ERROR ? "ONE ERROR" : "ALL GOOD") << std::endl;
}

double running_time_high_precision(Fibonacci& algo, uint64_t n, uint64_t N)
{
    std::vector<double> data;
    
    for (std::size_t i = 0; i < N; i++) data.push_back(running_time(algo, n));
    
    std::sort(data.begin(), data.end());
    
    return data[N/2];
}

void test_mult_algo(int n)
{
    bool all_check = true;
    srand((unsigned)time(nullptr));
    for (int i = 0; i < n; i++)
    {
        std::size_t x_len = 6 + rand()%1000;
        std::size_t y_len = 6 + rand()%1000;
        
        std::vector<uint64_t> x(x_len);
        std::vector<uint64_t> y(y_len);
        
        for (std::size_t i = 0; i < x_len; i++) x[i] = rand();
        for (std::size_t i = 0; i < y_len; i++) y[i] = rand();
        
        std::vector<uint64_t> z_test = tomcook::mult(x, y);
        std::vector<uint64_t> z_ref = gschool::mult(x, y);
        
        bool check = uintinf_t(z_test).is_equal(z_ref);
        
        all_check = check && all_check;
        
        if (!check) std::cout << "x=" << uintinf_t(x).to_string() << std::endl << "y=" << uintinf_t(y).to_string() << std::endl << std::endl;
    }
    
    std::cout << (all_check ? "GOOD" : "BAD") << std::endl;
}

void test_square_algo(int n)
{
    bool all_check = true;
    srand((unsigned)time(nullptr));
    for (int i = 0; i < n; i++)
    {
        std::size_t x_len = 6 + rand()%1000;
        
        std::vector<uint64_t> x(x_len);
        
        for (std::size_t i = 0; i < x_len; i++) x[i] = rand();
        
        std::vector<uint64_t> z_test = tomcook::square(x);
        std::vector<uint64_t> z_ref = gschool::square(x);
        
        bool check = uintinf_t(z_test).is_equal(z_ref);
        
        all_check = check && all_check;
        
        if (!check) std::cout << "x=" << uintinf_t(x).to_string() << std::endl << std::endl;
    }
    
    std::cout << (all_check ? "GOOD" : "BAD") << std::endl;
}


double running_time_mult(int n)
{
    std::size_t x_len = 100000;
    std::size_t y_len = 100000;
    
    std::vector<uint64_t> x(x_len);
    std::vector<uint64_t> y(y_len);
    srand(0);
    
    auto T0 = std::chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
    {
        for (std::size_t i = 0; i < x_len; i++) x[i] = rand();
        for (std::size_t i = 0; i < y_len; i++) y[i] = rand();
        
        std::vector<uint64_t> z_test = tomcook::mult(x, y);
    }
    auto T1 = std::chrono::steady_clock::now();
    
    return std::chrono::duration<double>(T1 - T0).count() / double(n);
}

double running_time_square(int n)
{
    std::size_t x_len = 100000;
    
    std::vector<uint64_t> x(x_len);
    srand(0);
    
    auto T0 = std::chrono::steady_clock::now();
    for (int i = 0; i < n; i++)
    {
        for (std::size_t i = 0; i < x_len; i++) x[i] = rand();
        
        std::vector<uint64_t> z_test = tomcook::square(x);
    }
    auto T1 = std::chrono::steady_clock::now();
    
    return std::chrono::duration<double>(T1 - T0).count() / double(n);;
}
