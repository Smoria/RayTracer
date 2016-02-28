#include "RayTracer/Geometry/Geometry.h"
#include "RayTracer/VectorOperations.h"
#include "RayTracer/Ray.h"
#include "UnusedVar.h"
#include <iostream>

namespace RayTracer
{
    namespace Geometry
    {
        const Vector3f Geometry::defaultDiffuse(0, 0, 0);
        const Float Geometry::defaultRefractionCoeff = 1;
        const Float Geometry::defaultRefraction = 0;
        const Float Geometry::defaultReflection = 0;
        const Float Geometry::defaultShininessCoeff = 5;
        const Float Geometry::defaultSpecularCoeff = 0.5;
        const Float Geometry::defaultDiffuseCoeff = 1;
        const Bitmap Geometry::emptyTexture = Bitmap();

        Geometry::Geometry(const Vector3f& position, const Color& diffuse,
            const Float& refractionCoeff, const Float& refraction,
            const Float& reflection, const Float& shininessCoeff,
            const Float& specularCoeff, const Float& diffuseCoeff,
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

        Vector3f Geometry::GetNormalAtPointFromMap(const Vector3f& point,
            const Vector3f& normal) const
        {
            if (m_normalTexture.Empty())
            {
                return normal;
            }

            return GetNormalAtPointFromMap(point,
                normal, GetUV(point));
        }

        Vector3f Geometry::GetNormalAtPointFromMap(const Vector3f& point,
            const Vector3f& normal, const Vector2f& uv) const
        {
            if (!m_normalTexture.Empty())
            {
                const size_t x = static_cast<size_t>((m_normalTexture.GetWidth() - 1) * uv.x());
                const size_t y = static_cast<size_t>((m_normalTexture.GetHeight() - 1) * uv.y());
                const Color normalCol = Color::FromColorsUnion(m_normalTexture.GetPixel(x, y));

                return (Vector3f(normalCol.GetRed() - 0.5, normalCol.GetGreen() - 0.5,
                    normalCol.GetBlue() - 0.5) * 2 + normal).normalized();
            }

            return normal;
        }

        Vector3f Geometry::GetIntersectPoint(const Ray& ray, Float length)
        {
            return ray.Origin() + length * ray.Direction().normalized();
        }

        Color Geometry::Shade(const Ray& ray, const Vector3f& hitPoint, Color& color,
            const Vector3f* normal, Color lightColor, Float lightIntensity)
        {
            double l = 1;
            double specular = 0;
            const Vector2f uv = GetUV(hitPoint);

            Color col = m_diffuse;

            if (m_diffuseCoeff > 0)
            {
                if (normal != nullptr)
                {
                    const Vector3f n = GetNormalAtPointFromMap(hitPoint, *normal, uv);
                    const Vector3f dir = ray.Direction().normalized();
                    const Vector3f refl = Reflect(dir, n);
                    // Lambertian
                    
                    l = lightIntensity * m_diffuseCoeff * std::max<Float>(0, n.dot(dir));

                    // Blinn-Phong
                    Vector3f surfaceToCamera = (hitPoint - ray.Origin());
                    surfaceToCamera.normalize();
                    const Float cosAngle = std::max<Float>(0, surfaceToCamera.dot(refl));
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

        Vector2f Geometry::PrepareUV(double u, double v)
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

            return Vector2f(u, v);
        }
    }
}