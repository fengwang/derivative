#ifndef _OSCILLATE_FUNCTION_HPP_INCLUDED_DFS98Y4KLJHASFDKNV98Y4KLASJFDVCNMKJDSFF
#define _OSCILLATE_FUNCTION_HPP_INCLUDED_DFS98Y4KLJHASFDKNV98Y4KLASJFDVCNMKJDSFF

#include <derivative/type_at.hpp>

#include <cstddef>
#include <functional>

namespace numeric
{

    namespace oscillate_function_private_fdspojiasldkjasasfdioj4asfd4d
    {

        template<typename R, std::size_t Bn, std::size_t N>
        struct oscillate_backward_impl
        {
            template<typename F, typename Type, typename... Types>
            R impl( F F_, Type t, Types... vts ) const
            {
                return oscillate_backward_impl < R, Bn + 1, N > ().impl( F_, vts..., t );
            }
        };

        template<typename R, std::size_t N>
        struct oscillate_backward_impl<R, N, N>
        {
            template<typename F, typename... Types>
            R impl( F F_, Types... vts ) const
            {
                return F_( vts... );
            }
        };

        template<typename R, std::size_t Fn, std::size_t Bn>
        struct oscillate_forward_impl
        {
            template<typename F, typename f, typename Type, typename... Types>
            R impl( F F_, f f_, Type t, Types... vts ) const
            {
                return oscillate_forward_impl < R, Fn - 1, Bn > ().impl( F_, f_, vts..., t );
            }
        };

        template<typename R, std::size_t Bn>
        struct oscillate_forward_impl<R, 0, Bn>
        {
            template<typename F, typename f, typename Type, typename... Types>
            R impl( F F_, f f_, Type t, Types... vts ) const
            {
                return oscillate_backward_impl<R, 0, Bn>().impl( F_, vts..., f_( t ) );
            }
        };

    }//namespace oscillate_function_private_fdspojiasldkjasasfdioj4asfd4d

    template<std::size_t N, typename R, typename... Types>
    struct oscillate_function
    {
        typedef R return_type;
        typedef typename type_at<N, Types...>::result_type oscillate_type;
        typedef std::function<R( Types... )> function_type;
        typedef std::function<oscillate_type( oscillate_type )> oscillate_function_type;

        static_assert( N < sizeof...( Types ), "dim size exceeds limitation." );

        function_type F;
        oscillate_function_type f;

        oscillate_function( const function_type& F_, const oscillate_function_type& f_ ) : F( F_ ), f( f_ ) {}

        return_type operator()( Types... vts ) const
        {
            using namespace oscillate_function_private_fdspojiasldkjasasfdioj4asfd4d;
            return_type const ans = oscillate_forward_impl < R, N, sizeof...( vts ) - N - 1 > ().impl( F, f, vts... );
            return ans;
        }
    };

    template<std::size_t M, typename Type>
    struct oscillate_trait
    {
        typedef oscillate_trait<M, decltype( &Type::operator() )>   trait_type;
        typedef typename trait_type::oscillate_type                oscillate_type;
        typedef typename oscillate_type::return_type               return_type;
    };

    template<std::size_t M, typename Return, typename... Types>
    struct oscillate_trait<M, Return( Types... )>
    {
        typedef Return                                              return_type;
        typedef oscillate_function<M, Return, Types...>                    oscillate_type;
    };

    template<std::size_t M, typename Return, typename... Types>
    struct oscillate_trait<M, Return(* )( Types... )>
    {
        typedef Return                                              return_type;
        typedef oscillate_function<M, Return, Types...>                    oscillate_type;
    };

    template<std::size_t M, typename Class, typename Return, typename... Types>
    struct oscillate_trait<M, Return( Class::* )( Types... ) const>
    {
        typedef Return                                              return_type;
        typedef oscillate_function<M, Return, Types...>                    oscillate_type;
    };

    template<std::size_t M, typename Function, typename Oscillate_Function>
    typename oscillate_trait<M, Function>::oscillate_type make_oscillate_function( const Function& F, const Oscillate_Function& of )
    {
        typedef typename oscillate_trait<M, Function>::oscillate_type oscillate_type;
        return oscillate_type( F, of );
    }

    template<std::size_t N, std::size_t M, typename R, typename... Types>
    struct double_oscillate_function
    {
        typedef R return_type;
        typedef typename type_at<N, Types...>::result_type n_oscillate_type;
        typedef typename type_at<M, Types...>::result_type m_oscillate_type;
        typedef oscillate_function<N, R, Types...>         n_oscillate_function;
        typedef oscillate_function<M, R, Types...>         m_oscillate_function;
        typedef std::function<R( Types... )> function_type;
        typedef std::function<n_oscillate_type( n_oscillate_type )> n_oscillate_function_type;
        typedef std::function<m_oscillate_type( m_oscillate_type )> m_oscillate_function_type;

        static_assert( N < sizeof...( Types ), "dim size 1 exceeds limitation." );
        static_assert( M < sizeof...( Types ), "dim size 2 exceeds limitation." );

        function_type F;
        n_oscillate_function_type n_f;
        m_oscillate_function_type m_f;

        double_oscillate_function( const function_type& F_, const n_oscillate_function_type& n_f_, const m_oscillate_function_type& m_f_ ) : F( F_ ), n_f( n_f_ ), m_f( m_f_ ) {}

        return_type operator()( Types... vts ) const
        {
            n_oscillate_function const oscillate_f_n( F, n_f );
            m_oscillate_function const oscillate_f_m( oscillate_f_n, m_f );
            return oscillate_f_m( vts... );
        }
    };

    template<std::size_t M, std::size_t N, typename Type>
    struct double_oscillate_trait
    {
        typedef double_oscillate_trait<M, N, decltype( &Type::operator() )>  trait_type;
        typedef typename trait_type::double_oscillate_type                  double_oscillate_type;
        typedef typename double_oscillate_type::return_type                 return_type;
    };

    template<std::size_t M, std::size_t N, typename Return, typename... Types>
    struct double_oscillate_trait<M, N, Return( Types... )>
    {
        typedef Return                                                      return_type;
        typedef double_oscillate_function<M, N, Return, Types...>         double_oscillate_type;
    };

    template<std::size_t M, std::size_t N, typename Return, typename... Types>
    struct double_oscillate_trait<M, N, Return(* )( Types... )>
    {
        typedef Return                                                      return_type;
        typedef double_oscillate_function<M, N, Return, Types...>         double_oscillate_type;
    };

    template<std::size_t M, std::size_t N, typename Class, typename Return, typename... Types>
    struct double_oscillate_trait<M, N, Return( Class::* )( Types... ) const>
    {
        typedef Return                                                      return_type;
        typedef double_oscillate_function<M, N, Return, Types...>         double_oscillate_type;
    };

    template<std::size_t M, std::size_t N, typename Function, typename M_Oscillate_Function, typename N_Oscillate_Function>
    typename double_oscillate_trait<M, N, Function>::double_oscillate_type make_double_oscillate_function( const Function& F, const M_Oscillate_Function& m_of, const N_Oscillate_Function& n_of )
    {
        typedef typename double_oscillate_trait<M, N, Function>::double_oscillate_type double_oscillate_type;
        return double_oscillate_type( F, m_of, n_of );
    }

}//namespace numeric

#endif//_OSCILLATE_FUNCTION_HPP_INCLUDED_DFS98Y4KLJHASFDKNV98Y4KLASJFDVCNMKJDSFF

