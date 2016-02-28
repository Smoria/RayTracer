#pragma once

namespace RayTracer
{
    struct Refraction
    {
        static const double air;
        static const double helium;
    };

    const double Refraction::air = 1.000293;
    const double Refraction::helium = 1.000036;
}