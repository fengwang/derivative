#ifndef AHKOYFIPKAEMBKRQCSDDCNEVCQUGPSXXOOXYPUWJTJBLAWGJDFFASBUCSVPAOMRAFTLOKUJSP
#define AHKOYFIPKAEMBKRQCSDDCNEVCQUGPSXXOOXYPUWJTJBLAWGJDFFASBUCSVPAOMRAFTLOKUJSP

#include <derivative/oscillate_function.hpp>
#include <derivative/type_at.hpp>
#include <derivative/value_at.hpp>

#include <cstddef>
#include <complex>
#include <functional>
#include <limits>
#include <cassert>

namespace numeric
{
    template<typename T>
    struct stepper_value_type
    {
        typedef T value_type;
    };

    template<typename T>
    struct stepper_value_type<std::complex<T>>
    {
        typedef stepper_value_type<T> original_type;
        typedef typename original_type::value_type value_type;
    };

    /*
       Proposed usage:

        auto ff = []( double x, double y, double z ) { return std::sin(x)*cos(y)*tan(z); };

        stepper< 0, double, double, double, double > st( ff );

        const double h =  st( 1.0, 2.0, 3.0 );
     */

    template<std::size_t M, typename R, typename... Types>
    struct stepper
    {
        typedef typename type_at<M, Types...>::result_type return_type;
        typedef typename stepper_value_type<return_type>::value_type denorm_type;

        typedef std::function<R( Types... )> function_type;
        function_type ff;

        static_assert( M < sizeof...( Types ), "dim size exceeds limitation" );

        template<typename Function>
        stepper( const Function& f_ ) : ff( f_ ) {}

        return_type operator()( Types... vts ) const
        {
            return_type h = std::numeric_limits<denorm_type>::denorm_min();
            // h != 0;
            assert( h != h + h ); //make sure h != 0;
            // x-h != x+h
            return_type const x =  value_at<M, Types...>()( vts... );
            for ( ;; )
            {
                const return_type volatile x_l = x - h;
                const return_type volatile x_r = x + h;
                const return_type pi = 3.14159265358979323846;
                h *= pi;
                if ( x_l != x_r ) break;
            }
            /*
            // f(x-h) != f(x+h); //but this is not necessarily always true
            for (;;)
            {
                return_type const dh = h;
                auto const& local_f_l = [dh]( return_type x ) { return x - dh; };
                auto const& local_f_r = [dh]( return_type x ) { return x + dh; };
                oscillate_function<M, return_type, Types...> const of_l( ff, local_f_l );
                oscillate_function<M, return_type, Types...> const of_r( ff, local_f_r );

                if ( of_l( vts... ) != of_r( vts... ) ) break;

                h = h+h;
            }
            */
            return h;
        }

    };

    template<typename T>
    T step( const T x )
    {
        typedef typename stepper_value_type<T>::value_type denorm_type;
        typedef T return_type;
        return_type h = std::numeric_limits<denorm_type>::denorm_min();
        assert( h != h + h );
        for ( ;; )
        {
            const return_type volatile x_l = x - h;
            const return_type volatile x_r = x + h;
            if ( x_l != x_r ) break;
            const return_type pi = 3.14159265358979323846;
            h *= pi;
        }
        return h;
    }

}//namespace numeric

#endif//AHKOYFIPKAEMBKRQCSDDCNEVCQUGPSXXOOXYPUWJTJBLAWGJDFFASBUCSVPAOMRAFTLOKUJSP

