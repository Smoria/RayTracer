#pragma once

#include <string>
#include "ColorsUnion.h"

class Bitmap
{
    public:

        const Bitmap& operator=(Bitmap&& uref);
        const Bitmap& operator=(const Bitmap& ref);

        Bitmap();
        Bitmap(const std::string& filename);
        Bitmap(const Bitmap& ref);
        Bitmap(Bitmap&& uref);
        ~Bitmap();

        bool Empty() const { return m_pData == nullptr; }
        size_t GetWidth() const { return m_width; }
        size_t GetHeight() const { return m_height; }
        const ColorsUnion& GetPixel(size_t width, size_t height) const;

    private:
        size_t m_width;
        size_t m_height;
        ColorsUnion* m_pData;
};