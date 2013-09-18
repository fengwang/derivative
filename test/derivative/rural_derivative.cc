#include <derivative/derivative.hpp>

#include <iostream>
#include <cmath>
#include <functional>
#include <iomanip>
#include <cassert>

#include <complex>

double ff( double* x )
{
    return std::sin(x[0]) * std::cos(x[1]);
}

double df_dx( double* x )
{
    return std::cos(x[0]) * std::cos(x[1]);
}

double df_dy( double* x )
{
    return -std::sin(x[0]) * std::sin(x[1]);
}

int main()
{
    using namespace numeric;
    std::cout << "f(x,y) = sin(x) cos(y)\n";
    std::cout.precision( 15 );

    rural_derivative<double> dfy( ff, 1 );
    double x[2] = { 1.0, 2.0 };
    std::cout << "\nf(x,y) = sin(x) cos(y)\n";
    std::cout << "\ndf/dy at (1.0, 2.0) is " << dfy(x) << "\n";
    std::cout << "\nreal value of df/dy is " << df_dy(x) << "\n";

    rural_derivative<double> dfx( ff, 0 );
    std::cout << "\nf(x,y) = sin(x) cos(y)\n";
    std::cout << "\ndf/dx at (1.0, 2.0) is " << dfx(x) << "\n";
    std::cout << "\nreal value of df/dx is " << df_dx(x) << "\n";

    auto rdfx = make_derivative( ff, 0 );
    std::cout << "\ntesting make_derivative:\n";
    std::cout << "\nf(x,y) = sin(x) cos(y)\n";
    std::cout << "\ndf/dx at (1.0, 2.0) is " << rdfx(x) << "\n";
    std::cout << "\nreal value of df/dx is " << df_dx(x) << "\n";

    auto rdfy = make_derivative( ff, 1 );
    std::cout << "\ntesting make_derivative:\n";
    std::cout << "\nf(x,y) = sin(x) cos(y)\n";
    std::cout << "\ndf/dy at (1.0, 2.0) is " << rdfy(x) << "\n";
    std::cout << "\nreal value of df/dy is " << df_dy(x) << "\n";

    return 0;
}

