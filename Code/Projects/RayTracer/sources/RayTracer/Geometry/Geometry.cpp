#include "RayTracer/Geometry/Geometry.h"
#include "RayTracer/VectorOperations.h"
#include "RayTracer/Ray.h"
#include "UnusedVar.h"
#include <iostream>

namespace RayTracer
{
    namespace Geometry
    {
        Geometry::Geometry(const Vector3& position, const Color& diffuse,
            const Type& refractionCoeff, const Type& refraction,
            const Type& reflection, const Type& shininessCoeff,
            const Type& specularCoeff, const Type& diffuseCoeff,
            const Bitmap& texture, const Bitmap& normalTexture) :
                m_diffuse(diffuse),
                m_diffuseCoeff(diffuseCoeff),
                m_specularCoeff(specularCoeff),
                m_shininessCoeff(shininessCoeff),
                m_reflection(reflection),
                m_refraction(refraction),
                m_refractionCoeff(refractionCoeff),
                m_position(position),
                m_texture(texture),
                m_normalTexture(normalTexture)
        {}

        Vector3 Geometry::GetNormalAtPointFromMap(const Vector3& point,
            const Vector3& normal) const
        {
            if (m_normalTexture.Empty())
            {
                return normal;
            }

            return GetNormalAtPointFromMap(point,
                normal, GetUV(point));
        }

        Vector3 Geometry::GetNormalAtPointFromMap(const Vector3& point,
            const Vector3& normal, const Vector2& uv) const
        {
            if (!m_normalTexture.Empty())
            {
                const size_t x = static_cast<size_t>((m_normalTexture.GetWidth() - 1) * uv.x());
                const size_t y = static_cast<size_t>((m_normalTexture.GetHeight() - 1) * uv.y());
                const Color normalCol = Color::FromColorsUnion(m_normalTexture.GetPixel(x, y));

                return (Vector3(normalCol.GetRed() - 0.5, normalCol.GetGreen() - 0.5,
                    normalCol.GetBlue() - 0.5) * 2 + normal).normalized();
            }

            return normal;
        }

        Vector3 Geometry::GetIntersectPoint(const Ray& ray, Type length)
        {
            return ray.Origin() + length * ray.Direction().normalized();
        }

        Color Geometry::Shade(const Ray& ray, const Vector3& hitPoint, Color& color,
            const Vector3* normal, Color lightColor, Type lightIntensity)
        {
            double l = 1;
            double specular = 0;
            const Vector2 uv = GetUV(hitPoint);

            Color col = m_diffuse;

            if (m_diffuseCoeff > 0)
            {
                if (normal != nullptr)
                {
                    const Vector3 n = GetNormalAtPointFromMap(hitPoint, *normal, uv);
                    const Vector3 dir = ray.Direction().normalized();
                    const Vector3 refl = Reflect(dir, n);
                    // Lambertian
                    
                    l = lightIntensity * m_diffuseCoeff * std::max<Type>(0, n.dot(dir));

                    // Blinn-Phong
                    Vector3 surfaceToCamera = (hitPoint - ray.Origin());
                    surfaceToCamera.normalize();
                    const Type cosAngle = std::max<Type>(0, surfaceToCamera.dot(refl));
                    specular = lightIntensity * m_specularCoeff * std::pow(cosAngle, m_shininessCoeff);
                }
            }
            if (!m_texture.Empty())
            {
                const size_t x = static_cast<size_t>(uv.x() * (m_texture.GetWidth() - 1));
                const size_t y = static_cast<size_t>(uv.y() * (m_texture.GetHeight() - 1));
                col *= Color::FromColorsUnion(m_texture.GetPixel(x, y));
            }

            return color + lightColor * ((col * l) + specular);
        }

        Vector2 Geometry::PrepareUV(double u, double v)
        {
            u = u - std::trunc(u);
            v = v - std::trunc(v);

            if (u < 0)
            {
                u = 1 + u;
            }

            if (v < 0)
            {
                v = 1 + v;
            }

            return Vector2(u, v);
        }
    }
}