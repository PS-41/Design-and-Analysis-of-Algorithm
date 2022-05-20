#include "sls.hpp"
#include <chrono>
#include <iostream>

int main(int argc, char* argv[])
{
    SLS sls;
    sls.read();
    clock_t t0 = clock();
    sls.compute();
    std::cout << "Minimum Penalty = " << sls.min_penalty << std::endl;
    std::cout << "Number of segments = " << sls.segments << std::endl;
    std::cout << "Multiplier C = " << sls.C << std::endl;
    clock_t t1 = clock();
    std::cout << "\nRunning time (input):               " << int(1000 * t0 / CLOCKS_PER_SEC) << " ms";
    std::cout << "\nRunning time (algo):                " << int(1000 * (t1 - t0) / CLOCKS_PER_SEC) << " ms\n";
    return 0;
}