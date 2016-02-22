#ifndef CONSTSIZE2DARRAY_H_INCLUDED
#define CONSTSIZE2DARRAY_H_INCLUDED

#include "CConstSize1DArray.h"

namespace Collections
{
    template<size_t rowSize, size_t rowCount, class T>
    class CConstSize2DArray
    {
        public:
            using TRowType = CConstSize1DArray < rowSize, T > ;

            TRowType& operator[](size_t index)
            {
                return const_cast<TRowType&>(static_cast<const CConstSize2DArray*>(this)->operator[](index));
            }
            const TRowType& operator[](size_t index) const
            {
                if (index > rowCount)
                {
                    throw std::exception();
                }
                else
                {
                    return m_rows[index];
                }
            }


        private:
            TRowType m_rows[rowCount];
    };
}

#endif /*CONSTSIZE1DARRAY_H_INCLUDED*/