#ifndef CONSTSIZE1DARRAY_H_INCLUDED
#define CONSTSIZE1DARRAY_H_INCLUDED

namespace Collections
{
    template<size_t size, class T>
    class CConstSize1DArray
    {
        public:
            T& operator[](size_t index)
            {
                return const_cast<T&>(static_cast<const CConstSize1DArray*>(this)->operator[](index));
            }
            const T& operator[](size_t index) const
            {
                if (index >= size)
                {
                    throw std::exception();
                }
                else
                {
                    return m_data[index];
                }
            }

            CConstSize1DArray()
            {
                memset(m_data, 0, sizeof(m_data));
            }

        private:
            T m_data[size];
    };
}

#endif CONSTSIZE1DARRAY_H_INCLUDED