#include <cstdio>
#include "Bitmap.h"
#include "Macroses.h"

const Bitmap& Bitmap::operator=(Bitmap&& uref)
{
    SAFE_ARRAY_DELETE(m_pData);
    m_width = uref.m_width;
    m_height = uref.m_height;
    m_pData = uref.m_pData;
    uref.m_pData = nullptr;
    return *this;
}

const Bitmap& Bitmap::operator=(const Bitmap& ref)
{
    if (this != &ref)
    {
        SAFE_ARRAY_DELETE(m_pData);
        m_width = ref.m_width;
        m_height = ref.m_height;

        const size_t bytesCount = m_width * m_height * sizeof(ColorsUnion);

        if (bytesCount > 0)
        {
            m_pData = static_cast<ColorsUnion*>(malloc(bytesCount));
            memcpy(m_pData, ref.m_pData, bytesCount);
        }
    }

    return *this;
}


Bitmap::Bitmap(const std::string&)
{
    throw std::exception();
}

Bitmap::Bitmap() :
    m_width(0),
    m_height(0),
    m_pData(nullptr)
{}

Bitmap::Bitmap(const Bitmap& ref) :
    m_width(ref.m_width),
    m_height(ref.m_height),
    m_pData(ref.m_width * ref.m_height > 0 ? new ColorsUnion[ref.m_width*ref.m_height] : nullptr)
{}

Bitmap::Bitmap(Bitmap&& uref) :
    m_width(uref.m_width),
    m_height(uref.m_height),
    m_pData(uref.m_pData)
{
    uref.m_pData = nullptr;
}

Bitmap::~Bitmap()
{
    SAFE_ARRAY_DELETE(m_pData);
}

const ColorsUnion& Bitmap::GetPixel(size_t x, size_t y) const
{
    if (x < m_width && y < m_height)
    {
        return m_pData[(y * m_width) + x];
    }
    else
    {
        throw std::exception();
    }
}