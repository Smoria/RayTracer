#include "RayTracer/Color.h"

namespace RayTracer
{
    const size_t Color::m_sRedIndex = 0;
    const size_t Color::m_sGreenIndex = 1;
    const size_t Color::m_sBlueIndex = 2;

    Color::Color(const Vector3f& ref) :
        Color(ref.x(), ref.y(), ref.z())
    {
    }

    Color::Color(const Float& r, const Float& g, const Float& b) :
        m_rgb(Clamp(r), Clamp(g), Clamp(b))
    {
    }

    Color Color::FromColorsUnion(const ColorsUnion& rgba)
    {
        return Color(
            rgba.colors[m_sRedIndex] / 255.0,
            rgba.colors[m_sGreenIndex] / 255.0,
            rgba.colors[m_sBlueIndex] / 255.0);
    }
}