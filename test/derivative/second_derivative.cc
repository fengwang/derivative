#include <derivative/derivative.hpp>
#include <derivative/second_derivative.hpp>

#include <iostream>
#include <cmath>
#include <functional>
#include <iomanip>
#include <cassert>

using namespace numeric;

double ff( double* x )
{
    return std::sin(x[0]) * std::cos(x[1]) * x[2] * x[2];
}

double ff0_pr( double* x )
{
    return std::cos(x[0]) * std::cos(x[1]) * x[2] * x[2];
}

double f00( double* x )
{
    return - std::sin(x[0]) * std::cos(x[1]) * x[2] * x[2];
}

double f01( double* x )
{
    return - std::cos(x[0]) * std::sin(x[1]) * x[2] * x[2];
}

double f02( double* x )
{
    return 2.0 * std::cos(x[0])*std::cos(x[1])*x[2];
}

double fx( double* x )
{
    return x[0] * x[0];
}

double f_x( double* x )
{
    return 2.0 * x[0];
}

double f__x( double* )
{
    return 2.0;
}

void test1()
{
    auto const& fx_0 = make_derivative( fx, 0 );
    auto const& fx_00_ = [&fx_0]( double* x )
    { 
        auto const ans = fx_0(x); 
        return ans;
    };
    auto const& fx_00 = make_derivative( fx_00_, 0 );

    double x = 3.0;
    
    std::cout << "\nfor f(x) = x^2\n";
    std::cout << "\nf''(x) at (3.0) is " <<  fx_00(&x) << "\n";
    std::cout << "\nf''(x) at (3.0) is " <<  make_second_derivative(fx, 0, 0)(&x) << "\n";

}

double t2_fxyz( double x, double y, double z )
{
    return x*y*z;
}

double t2_fxyz_xx( double, double, double )
{
    return 0;
}

double t2_fxyz_yy( double, double, double )
{
    return 0;
}

double t2_fxyz_zz( double, double, double )
{
    return 0;
}

double t2_fxyz_xy( double, double, double z )
{
    return z;
}

double t2_fxyz_xz( double x, double y, double z )
{
    return y;
}

double t2_fxyz_yz( double x, double y, double z )
{
    return x;
}

void test2()
{
    second_derivative<0,0, double(double, double, double)> const fxx( t2_fxyz ); 
    second_derivative<0,1, double(double, double, double)> const fxy( t2_fxyz ); 
    second_derivative<0,2, double(double, double, double)> const fxz( t2_fxyz ); 

    second_derivative<1,0, double(double, double, double)> const fyx( t2_fxyz ); 
    second_derivative<1,1, double(double, double, double)> const fyy( t2_fxyz ); 
    second_derivative<1,2, double(double, double, double)> const fyz( t2_fxyz ); 

    second_derivative<2,0, double(double, double, double)> const fzx( t2_fxyz ); 
    second_derivative<2,1, double(double, double, double)> const fzy( t2_fxyz ); 
    //second_derivative<2,2, double(double, double, double)> const fzz( t2_fxyz ); 

    auto const& fzz = make_second_derivative<2,2>( t2_fxyz );

    std::cout.precision( 15 );

    std::cout << "\nfxx=" << fxx( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_xx( 1.0, 2.0, 3.0 ) << "\n";
    std::cout << "\nfxy=" << fxy( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_xy( 1.0, 2.0, 3.0 ) << "\n";
    std::cout << "\nfxz=" << fxz( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_xz( 1.0, 2.0, 3.0 ) << "\n\n";
    std::cout << "\nfyx=" << fyx( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_xy( 1.0, 2.0, 3.0 ) << "\n";
    std::cout << "\nfyy=" << fyy( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_yy( 1.0, 2.0, 3.0 ) << "\n";
    std::cout << "\nfyz=" << fyz( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_yz( 1.0, 2.0, 3.0 ) << "\n\n";
    std::cout << "\nfzx=" << fzx( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_xz( 1.0, 2.0, 3.0 ) << "\n";
    std::cout << "\nfzy=" << fzy( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_yz( 1.0, 2.0, 3.0 ) << "\n";
    std::cout << "\nfzz=" << fzz( 1.0, 2.0, 3.0 ) << "\t" << t2_fxyz_zz( 1.0, 2.0, 3.0 ) << "\n\n";
}

double t3_fxyz( double* x )
{
    return x[0] * x[1] * x[2];
}


double t3_fxyz_xx( double* x )
{
    return 0;
}

double t3_fxyz_xy( double* x )
{
    return x[2];
}

double t3_fxyz_xz( double* x )
{
    return x[1];
}


double t3_fxyz_yx( double* x )
{
    return x[2];
}

double t3_fxyz_yy( double* x )
{
    return 0;
}

double t3_fxyz_yz( double* x )
{
    return x[0];
}


double t3_fxyz_zx( double* x )
{
    return x[1];
}

double t3_fxyz_zy( double* x )
{
    return x[0];
}

double t3_fxyz_zz( double* x )
{
    return 0;
}


void test3()
{
    double x[] = { 2.0, 3.0, 4.0 };
    typedef rural_second_derivative<double> rd_type;

    rd_type const fxx( t3_fxyz, 0,  0 );
    rd_type const fxy( t3_fxyz, 0,  1 );
    rd_type const fxz( t3_fxyz, 0,  2 );
    rd_type const fyx( t3_fxyz, 1,  0 );
    rd_type const fyy( t3_fxyz, 1,  1 );
    rd_type const fyz( t3_fxyz, 1,  2 );
    rd_type const fzx( t3_fxyz, 2,  0 );
    rd_type const fzy( t3_fxyz, 2,  1 );
    rd_type const fzz( t3_fxyz, 2,  2 );

    std::cout.precision( 15 );
    std::cout << "\n" << fxx( x ) << "\t" << t3_fxyz_xx( x ) << "\n";
    std::cout << "\n" << fxy( x ) << "\t" << t3_fxyz_xy( x ) << "\n";
    std::cout << "\n" << fxz( x ) << "\t" << t3_fxyz_xz( x ) << "\n";
    std::cout << "\n" << fyx( x ) << "\t" << t3_fxyz_yx( x ) << "\n";
    std::cout << "\n" << fyy( x ) << "\t" << t3_fxyz_yy( x ) << "\n";
    std::cout << "\n" << fyz( x ) << "\t" << t3_fxyz_yz( x ) << "\n";
    std::cout << "\n" << fzx( x ) << "\t" << t3_fxyz_zx( x ) << "\n";
    std::cout << "\n" << fzy( x ) << "\t" << t3_fxyz_zy( x ) << "\n";
    std::cout << "\n" << fzz( x ) << "\t" << t3_fxyz_zz( x ) << "\n";

}

int main()
{
    std::cout << "\nTEST --1 \n\n";
    test1();

    std::cout << "\nTEST -- 2 \n\n";

    test2();


    std::cout << "\nTest -- 3 \n\n";

    test3();

    return 0;
}

