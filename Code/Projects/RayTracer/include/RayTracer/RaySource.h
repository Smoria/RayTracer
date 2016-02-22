#pragma once

#include "Color.h"

namespace RayTracer
{
    class RaySource
    {
        public:
            RaySource(const Color& color, const Vector3& position,
                const Type& intensity = 1) :
                m_color(color),
                m_position(position),
                m_intensity(intensity)
            {}

            const Color& GetColor() const { return m_color; }
            const Type& Intensity() const { return m_intensity; }
            const Vector3& Position() const { return m_position; }
    
        protected:
            Color m_color;
            Vector3 m_position;
            Type m_intensity;
    };
}