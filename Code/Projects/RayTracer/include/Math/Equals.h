#pragma once

#include <limits>

namespace Math
{
    template<class Type>
    bool Equals(Type value1, Type value2)
    {
        return std::numeric_limits<Type>::epsilon() > std::abs(value1 - value2);
    }
}