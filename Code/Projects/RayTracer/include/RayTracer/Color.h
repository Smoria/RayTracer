#pragma once

#include "Definitions.h"
#include "ColorsUnion.h"
#include <ostream>

namespace RayTracer
{
    class Color
    {
        public:

            friend std::ostream& operator << (std::ostream& stream, const Color& color)
            {
                return stream
                    << "RGB("
                    << color.m_rgb[m_sRedIndex] << ','
                    << color.m_rgb[m_sGreenIndex] << ','
                    << color.m_rgb[m_sBlueIndex] << ")" << std::endl;
            }

            Color operator+(const Color& rhs) const
            {
                return Color(rhs.m_rgb + m_rgb);
            }
            Color& operator+=(const Color& rhs)
            {
                m_rgb.x() = Clamp(m_rgb.x() + rhs.m_rgb.x());
                m_rgb.y() = Clamp(m_rgb.y() + rhs.m_rgb.y());
                m_rgb.z() = Clamp(m_rgb.z() + rhs.m_rgb.z());
                return *this;
            }
            Color operator*(const Color& rhs) const
            {
                return Color(
                    m_rgb.x() * rhs.m_rgb.x(),
                    m_rgb.y() * rhs.m_rgb.y(),
                    m_rgb.z() * rhs.m_rgb.z());
            }
            Color operator*(const Type& value) const
            {
                return Color(m_rgb*value);
            }
            Color& operator*=(const Color& rhs)
            {
                m_rgb[0] = Clamp(m_rgb.x() * rhs.m_rgb.x());
                m_rgb[1] = Clamp(m_rgb.y() * rhs.m_rgb.y());
                m_rgb[2] = Clamp(m_rgb.z() * rhs.m_rgb.z());
                return *this;
            }
            Color& operator*=(const Type& value)
            {
                m_rgb *= value;
                m_rgb[0] = Clamp(m_rgb[0]);
                m_rgb[1] = Clamp(m_rgb[1]);
                m_rgb[2] = Clamp(m_rgb[2]);
                return *this;
            }
            Color operator+(const Type& value) const
            {
                return Color(m_rgb + Vector3(value, value, value));
            }
            Color operator/(const Type& value) const
            {
                return Color(
                    Vector3(
                        m_rgb.x() / value,
                        m_rgb.y() / value,
                        m_rgb.z() / value
                        ));
            }

            Color() = delete;
            Color(const Vector3& ref);
            Color(const Type& r, const Type& g, const Type& b);
            Color(const Color&) = default;

            const Type& GetRed() const { return m_rgb[m_sRedIndex]; }
            const Type& GetGreen() const { return m_rgb[m_sGreenIndex]; }
            const Type& GetBlue() const { return m_rgb[m_sBlueIndex]; }

            void SetRed(const Type& value) { m_rgb[m_sRedIndex] = Clamp(value); }
            void SetGreen(const Type& value) { m_rgb[m_sGreenIndex] = Clamp(value); }
            void SetBlue(const Type& value) { m_rgb[m_sBlueIndex] = Clamp(value); }

            static Color FromColorsUnion(const ColorsUnion& rgba);

        private:
            Vector3 m_rgb;

            static const size_t m_sRedIndex;
            static const size_t m_sGreenIndex;
            static const size_t m_sBlueIndex;

            static Type Clamp(const Type& value)
            {
                return value > 1 ? 1 : value;
            }
    };
};
