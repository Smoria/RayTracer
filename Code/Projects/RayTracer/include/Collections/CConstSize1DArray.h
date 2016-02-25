#ifndef CONSTSIZE1DARRAY_H_INCLUDED
#define CONSTSIZE1DARRAY_H_INCLUDED

namespace Collections
{
    template<size_t size, class T, bool is_static = true>
    class CConstSize1DArray {};

    template<size_t size, class T>
    class CConstSize1DArray<size, T, true>
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

    template<size_t size, class T>
    class CConstSize1DArray<size, T, false>
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
                return m_pData[index];
            }
        }

        CConstSize1DArray() :
            m_pData(new T[size])
        {
            memset(m_pData, 0, sizeof(T) * size);
        }

        ~CConstSize1DArray()
        {
            if (m_pData != nullptr)
            {
                delete[]m_pData;
                m_pData = nullptr;
            }
        }

    private:
        T* m_pData;
    };
}

#endif CONSTSIZE1DARRAY_H_INCLUDED