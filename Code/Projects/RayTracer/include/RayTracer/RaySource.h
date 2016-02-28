#pragma once

#include "Color.h"

namespace RayTracer
{
    class RaySource
    {
        public:
            RaySource(const Color& color, const Vector3f& position,
                const Float& intensity = 1) :
                m_color(color),
                m_position(position),
                m_intensity(intensity)
            {}

            const Color& GetColor() const { return m_color; }
            const Float& Intensity() const { return m_intensity; }
            const Vector3f& Position() const { return m_position; }
    
        protected:
            Color m_color;
            Vector3f m_position;
            Float m_intensity;
    };
}