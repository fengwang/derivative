#include <derivative/derivative.hpp>

#include <iostream>
#include <cmath>
#include <functional>
#include <iomanip>
#include <cassert>

double ds( double x )
{
    return std::sin(x);
}

double fxy( double x, double y )
{
    return std::sin(x) * std::cos(y);
}

double gxy( double x, double y )
{
    return x * x * std::log(y);
}


int main()
{
    std::cout << "f(x,y) = sin(x) cos(y)\n";

    using namespace numeric;
    auto const dfx = make_derivative<0>( fxy ); //-sin(x) sin(y)
    auto const dfx_1 = []( double x, double y ) { return std::cos(x) * std::cos(y); };
    auto const dfy = make_derivative<1>( fxy ); //-sin(x) sin(y)
    auto const dfy_1 = []( double x, double y ) { return -std::sin(x) * std::sin(y); };
    auto const dfxx = make_derivative<0>( dfx ); //-sin(x) cos(y)
    //auto const dfxx_1 = []( double x, double y ) { return -std::sin(x) * std::cos(y); };

    std::cout.precision(15);
    std::cout << "df / dx at ( 1, 2 ) is " << dfx(1, 2 ) << "\n";
    std::cout << "it is assumed to be " << dfx_1( 1, 2 ) << std::endl;
    std::cout << "df / dy at ( 1, 2 ) is " << dfy(1, 2 ) << "\n";
    std::cout << "it is assumed to be " << dfy_1(1, 2)  << "\n";

    return 0;
}

