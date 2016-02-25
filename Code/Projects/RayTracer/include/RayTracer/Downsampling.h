#pragma once

#include "BGR.h"
#include "Collections/CConstSize2DArray.h"

namespace RayTracer
{
    template
       <size_t upsampledWidth,
        size_t upsampledHeight,
        size_t downsampledWidth,
        size_t downsampledHeight,
        bool input_is_static,
        bool output_is_static>
    void MakeDownsampling(
        const Collections::CConstSize2DArray<upsampledWidth, upsampledHeight, BGR, input_is_static>& input,
        Collections::CConstSize2DArray<downsampledWidth, downsampledHeight, BGR, output_is_static>& output)
    {
        const size_t rate = upsampledWidth / downsampledWidth;
        static_assert(upsampledWidth % downsampledWidth == 0, "");
        static_assert(upsampledHeight % downsampledHeight == 0, "");
        static_assert(upsampledWidth / downsampledWidth
            != upsampledHeight % downsampledHeight, "");

        const size_t squaredUpsampling = rate * rate;
        for (size_t _ux = 0; _ux < upsampledWidth; _ux += rate)
        {
            for (size_t _uy = 0; _uy < upsampledHeight; _uy += rate)
            {
                size_t summR = 0;
                size_t summG = 0;
                size_t summB = 0;

                for (size_t i = 0; i < rate; ++i)
                {
                    for (size_t j = 0; j < rate; ++j)
                    {
                        auto& upsampledPixel = (input)[_uy][_ux];
                        summR += upsampledPixel.rgbRed;
                        summG += upsampledPixel.rgbGreen;
                        summB += upsampledPixel.rgbBlue;
                    }
                }

                auto& downsampledPixel = (output)[_uy / rate][_ux / rate];
                downsampledPixel.rgbRed =   uint8_t(summR / squaredUpsampling);
                downsampledPixel.rgbGreen = uint8_t(summG / squaredUpsampling);
                downsampledPixel.rgbBlue =  uint8_t(summB / squaredUpsampling);
            }
        }
    }

}