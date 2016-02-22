#ifndef MINIMALVALUES_H_DEFINED
#define MINIMALVALUES_H_DEFINED

#include <climits>

template<class T>
T MinimalValue()
{
    return std::numeric_limits<T>::min();
}

#endif /*MINIMALVALUES_H_DEFINED*/