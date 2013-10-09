#if 0
    derivative -- a numerical auto derivative library
    Copyright (C) 2013 Feng Wang feng.wang@uni-ulm.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#endif 

#ifndef _SECONDARY_DERIVATIVE_HPP_INCLUDED_SDFPIS39P8YUASFLKJHASFDJKLNASFDKLJBAN
#define _SECONDARY_DERIVATIVE_HPP_INCLUDED_SDFPIS39P8YUASFLKJHASFDJKLNASFDKLJBAN

#include <derivative/oscillate_function.hpp>
#include <derivative/type_at.hpp>
#include <derivative/value_at.hpp>
#include <derivative/stepper.hpp>

#include <cassert>
#include <cstddef>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <vector>

namespace numeric
{

    template<std::size_t M, std::size_t N, typename Dummy_Type> struct second_derivative;

    template<std::size_t M, typename R, typename... Types>
    struct second_derivative<M, M, R( Types... )>
    {
        typedef R                                               return_type;
        typedef std::function<R( Types... )>                    function_type;
        typedef typename type_at<M, Types...>::result_type      result_type;
        typedef std::size_t                                     size_type;
        typedef result_type                                     value_type;

        function_type ff;

        template<typename Function>
        second_derivative( const Function& ff_ ) : ff( ff_ ) {}

    result_type operator()( Types... vts ) const
    {
        const value_type decrease_step   = 1.6180339887498948482;
        const value_type decrease_step_2 = decrease_step * decrease_step;
        const value_type safe_boundary   = 2.6180339887498948482;
        const value_type start_step      = decrease_step * 1.0e-2;
        const result_type x              = value_at<M, Types...>()( vts... );
        const result_type s              = step( x );
        const size_type iter_depth       = 32;
        const value_type middle_fx       = ff( vts... );
        result_type error                = std::numeric_limits<value_type>::max();
        result_type ans                  = std::numeric_limits<value_type>::quiet_NaN();
        value_type step                  = start_step;
        auto const& lhs_f_3              = [&step]( result_type x ) { return x - step - step - step; };
        auto const& lhs_f_2              = [&step]( result_type x ) { return x - step - step; };
        auto const& lhs_f                = [&step]( result_type x ) { return x - step; };
        auto const& rhs_f                = [&step]( result_type x ) { return x + step; };
        auto const& rhs_f_2              = [&step]( result_type x ) { return x + step + step; };
        auto const& rhs_f_3              = [&step]( result_type x ) { return x + step + step + step; };
        oscillate_function<M, return_type, Types...> const lhs_of_3( ff, lhs_f_3 );
        oscillate_function<M, return_type, Types...> const lhs_of_2( ff, lhs_f_2 );
        oscillate_function<M, return_type, Types...> const lhs_of( ff, lhs_f );
        oscillate_function<M, return_type, Types...> const rhs_of( ff, rhs_f );
        oscillate_function<M, return_type, Types...> const rhs_of_2( ff, rhs_f_2 );
        oscillate_function<M, return_type, Types...> const rhs_of_3( ff, rhs_f_3 );
        value_type a[iter_depth][iter_depth];
        a[0][0] = ( value_type(2) * lhs_of_3( vts... ) - value_type(27) * lhs_of_2( vts... ) + value_type(270) * lhs_of( vts... ) - value_type(490) * middle_fx +
                    value_type(2) * rhs_of_3( vts... ) - value_type(27) * rhs_of_2( vts... ) + value_type(270) * rhs_of( vts... ) ) / ( value_type(180) * step * step );
        //a[0][0] = ( lhs_of( vts... ) - rhs_of( vts... ) ) / ( step * step * 2 );
        for ( size_type i = 1; i != iter_depth; ++i )
        {
            step /= decrease_step;
            a[i][0] = ( value_type(2) * lhs_of_3( vts... ) - value_type(27) * lhs_of_2( vts... ) + value_type(270) * lhs_of( vts... ) - value_type(490) * middle_fx +
                        value_type(2) * rhs_of_3( vts... ) - value_type(27) * rhs_of_2( vts... ) + value_type(270) * rhs_of( vts... ) ) / ( value_type(180) * step * step );
            //a[i][0] = ( lhs_of( vts... ) - rhs_of( vts... ) ) / ( step * step * 2 );
            result_type factor = decrease_step_2;
            for ( size_type j = 1; j <= i; ++j )
            {
                const result_type factor_1 = factor - result_type( 1 );
                a[i][j] = ( a[i][j - 1] * factor - a[i - 1][j - 1] ) / factor_1;
                factor *= decrease_step_2;
                const result_type error_so_far = std::max( std::abs( a[i][j] - a[i][j - 1] ), std::abs( a[i][j] - a[i - 1][j - 1] ) );
                if ( error > error_so_far )
                {
                    error = error_so_far;
                    ans = a[i][j];
                }
            }
            if ( std::abs( a[i][i] - a[i - 1][i - 1] ) >=  safe_boundary * error ) break;
            if ( s > step ) break;
        }
        return ans;
    }

    };//second_derivative<M, M>

    template<std::size_t M, std::size_t N, typename R, typename... Types>
    struct second_derivative<M, N, R( Types... )>
    {
        typedef R                                               return_type;
        typedef std::function<R( Types... )>                    function_type;
        typedef typename type_at<M, Types...>::result_type      result_type;
        typedef std::size_t                                     size_type;
        typedef result_type                                     value_type;

        function_type F;

        template<typename Function>
        second_derivative( const Function& F_ ) : F( F_ ) {}

    result_type operator()( Types... vts ) const
    {
        const value_type decrease_step   = 1.6180339887498948482;
        const value_type decrease_step_2 = decrease_step * decrease_step;
        const value_type safe_boundary   = 2.6180339887498948482;
        const value_type start_step      = decrease_step * 1.0e-1;
        const result_type x              = value_at<M, Types...>()( vts... );
        const result_type s              = step( x );
        const size_type iter_depth       = 32;
        result_type error                = std::numeric_limits<value_type>::max();
        result_type ans                  = std::numeric_limits<value_type>::quiet_NaN();
        value_type step                  = start_step;
        //make oscillate functions
        auto const& osl = [&step]( value_type x ) { return x - step; };
        auto const& osr = [&step]( value_type x ) { return x + step; };
        auto const& frr = make_double_oscillate_function<M, N>( F, osr, osr );
        auto const& frl = make_double_oscillate_function<M, N>( F, osr, osl );
        auto const& flr = make_double_oscillate_function<M, N>( F, osl, osr );
        auto const& fll = make_double_oscillate_function<M, N>( F, osl, osl );
        value_type a[iter_depth][iter_depth];
        a[0][0] = ( frr( vts... ) + fll( vts... ) - frl( vts... ) - flr( vts... ) ) / ( value_type( 4 ) * step * step );
        for ( size_type i = 1; i != iter_depth; ++i )
        {
            step /= decrease_step;
            a[i][0] = ( frr( vts... ) + fll( vts... ) - frl( vts... ) - flr( vts... ) ) / ( value_type( 4 ) * step * step );
            result_type factor = decrease_step_2;
            for ( size_type j = 1; j <= i; ++j )
            {
                const result_type factor_1 = factor - result_type( 1 );
                a[i][j] = ( a[i][j - 1] * factor - a[i - 1][j - 1] ) / factor_1;
                factor *= decrease_step_2;
                const result_type error_so_far = std::max( std::abs( a[i][j] - a[i][j - 1] ), std::abs( a[i][j] - a[i - 1][j - 1] ) );
                if ( error > error_so_far )
                {
                    error = error_so_far;
                    ans = a[i][j];
                }
            }
            if ( std::abs( a[i][i] - a[i - 1][i - 1] ) >=  safe_boundary * error ) break;
            if ( s > step ) break;
        }
        return ans;
    }

    };//second_derivative<M, N>


    template<typename T>
    struct rural_second_derivative
    {
        typedef T                                           value_type;
        typedef value_type*                                 pointer;
        typedef const value_type*                           const_pointer;
        typedef std::vector<value_type>                     array_type;
        typedef std::function<value_type( value_type )>     unary_function_type;
        typedef std::function<value_type( pointer )>        function_type;
        typedef std::function<value_type( const_pointer )>  const_function_type;
        typedef std::size_t                                 size_type;

        function_type   func;
        size_type       n;
        size_type       m;

        // f_ ---- the function to get derivatvie
        // n_ ---- the index of derivatation
        template<typename F>
        rural_second_derivative( F const& f_, size_type const n_, size_type const m_ ) : func( f_ ), n( n_ ), m( m_ ) {}

        // just in case when x_ is not contex free
        value_type operator()( pointer const& x_, size_type const length_of_x_ ) const
        {
            assert( length_of_x_ > n );
            assert( length_of_x_ > m );
            array_type arr( x_, x_ + length_of_x_ );
            return ( *this )( &arr[0] );
        }

        value_type operator()( pointer x_ ) const
        {
            if ( m == n )
            {
                value_type const orig_xn      = *( x_ + n );
                size_type const     cn        = n;
                pointer             cx        = x_;
                unary_function_type const& uf = [ cn, cx, this ]( value_type x ) { cx[cn] = x; return ( ( *this ).func )( cx ); };
                value_type const x            = *( x_ + n );
                second_derivative<0, 0, value_type( value_type )> const df( uf );
                value_type const ans = df( x );
                *( x_ + n ) = orig_xn;
                return ans;
            }
            value_type const orig_xn      = *( x_ + n );
            value_type const orig_xm      = *( x_ + m );
            size_type const     cn        = n;
            size_type const     cm        = m;
            pointer             cx        = x_;
            auto const& uf = [ cn, cm, cx, this ]( value_type x, value_type y ) { cx[cn] = x; cx[cm] = y; return ( ( *this ).func )( cx ); };
            value_type const x            = *( x_ + n );
            value_type const y            = *( x_ + m );
            second_derivative<0, 1, value_type( value_type, value_type )> const df( uf );
            value_type const ans = df( x, y );
            *( x_ + n ) = orig_xn;
            *( x_ + m ) = orig_xm;
            return ans;
        }

    };


    template<std::size_t M, std::size_t N, typename Type>
    struct second_derivative_trait
    {
        typedef second_derivative_trait<M, N, decltype( &Type::operator() )>    trait_type;
        typedef typename trait_type::second_derivative_type                     second_derivative_type;
        typedef typename second_derivative_type::return_type                    return_type;
        typedef rural_second_derivative<return_type>                            rural_second_derivative_type;
    };

    template<std::size_t M, std::size_t N, typename Return, typename... Types>
    struct second_derivative_trait<M, N, Return( Types... )>
    {
        typedef Return                                                      return_type;
        typedef second_derivative<M, N, Return( Types... )>                 second_derivative_type;
        typedef rural_second_derivative<return_type>                        rural_second_derivative_type;
    };

    template<std::size_t M, std::size_t N, typename Return, typename... Types>
    struct second_derivative_trait<M, N, Return(* )( Types... )>
    {
        typedef Return                                                      return_type;
        typedef second_derivative<M, N, Return( Types... )>                 second_derivative_type;
        typedef rural_second_derivative<return_type>                        rural_second_derivative_type;
    };

    template<std::size_t M, std::size_t N, typename Class, typename Return, typename... Types>
    struct second_derivative_trait<M, N, Return( Class::* )( Types... ) const>
    {
        typedef Return                                                      return_type;
        typedef second_derivative<M, N, Return( Types... )>                 second_derivative_type;
        typedef rural_second_derivative<return_type>                        rural_second_derivative_type;
    };

    template<std::size_t M, std::size_t N, typename Lambda>
    typename second_derivative_trait<M, N, Lambda>::second_derivative_type make_second_derivative( const Lambda& f )
    {
        return typename second_derivative_trait<M, N, Lambda>::second_derivative_type( f );
    }

    template<typename Lambda>
    typename second_derivative_trait<0, 0, Lambda>::rural_second_derivative_type make_second_derivative( const Lambda& f, const std::size_t n, const std::size_t m )
    {
        return typename second_derivative_trait<0, 0, Lambda>::rural_second_derivative_type( f, n, m );
    }

}//namespace numeric

#endif//_SECONDARY_DERIVATIVE_HPP_INCLUDED_SDFPIS39P8YUASFLKJHASFDJKLNASFDKLJBAN

