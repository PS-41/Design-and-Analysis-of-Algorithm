#include "guting.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>

int main(int argc, char const *argv[])
{
    Guting guting;
    guting.read();
    clock_t t0 = clock();
    guting.compute();
    std::cout << std::setprecision(15) << "Measure (Area) = " << guting.measure << " Square Units\nContour (Perimeter) = " << guting.contour << " Units" << std::endl;
    clock_t t1 = clock();
    std::cout << "\nRunning time (input):               " << int(1000 * t0 / CLOCKS_PER_SEC) << " ms";
    std::cout << "\nRunning time (algo):                " << int(1000 * (t1 - t0) / CLOCKS_PER_SEC) << " ms\n";
    return 0;
}