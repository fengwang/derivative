#ifndef _PRINT_VALUE_HPP_INCLUDED_SEDPFOINASDLFKJN4OIUANJDFSLKJAMNDFSIUHAJSFDOIU
#define _PRINT_VALUE_HPP_INCLUDED_SEDPFOINASDLFKJN4OIUANJDFSLKJAMNDFSIUHAJSFDOIU

#include <derivative/type_at.hpp>

#include <cstddef>
#include <iostream>

namespace numeric
{

    struct print_value
    {
        template<typename Type>
        std::ostream& operator()( std::ostream& lhs, Type const& rhs ) const
        {
            return lhs << rhs;
        }

        template<typename Type, typename... Types>
        std::ostream& operator()( std::ostream& lhs, Type const& rhs, Types const& ... rhss ) const
        {
            lhs << rhs << ", ";
            return print_value()( lhs, rhss... );
        }
    };

}//namespace numeric

#endif//_PRINT_VALUE_HPP_INCLUDED_SEDPFOINASDLFKJN4OIUANJDFSLKJAMNDFSIUHAJSFDOIU 

