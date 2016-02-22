#ifndef GENERATEIMAGEDATABYSHADER_H_INCLUDED
#define GENERATEIMAGEDATABYSHADER_H_INCLUDED

#include "Collections\CConstSize2DArray.h"

namespace bmp
{
    template<size_t imageWidth, size_t imageHeight, class T>
    void GenerateImageDataByShader(
        Collections::CConstSize2DArray<imageWidth, imageHeight, T>& bitMap,
        T(*pixelShader)(size_t x, size_t y))
    {
        for (size_t x = 0; x < imageWidth; ++x)
        {
            for (size_t y = 0; y < imageHeight; ++y)
            {
                bitMap[y][x] = pixelShader(x, y);
            }
        }
    }
}

#endif /*GENERATEIMAGEDATABYSHADER_H_INCLUDED*/