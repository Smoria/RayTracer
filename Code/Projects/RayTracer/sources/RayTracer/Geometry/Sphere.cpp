#include "RayTracer/Geometry/Sphere.h"
#include "RayTracer/Ray.h"
#define _USE_MATH_DEFINES
#include <cmath>

namespace RayTracer
{
    namespace Geometry
    {
        Sphere::Sphere(const Vector3f& position,
            const Float& radius,
            const Color& diffuse,
            const Float& refractionCoeff,
            const Float& refraction,
            const Float& reflection,
            const Float& shininessCoeff,
            const Float& specularCoeff,
            const Float& diffuseCoeff,
            const Bitmap& texture,
            const Bitmap& normalTexture) :
            Geometry(position, diffuse, refractionCoeff,
                refraction, reflection, shininessCoeff,
                specularCoeff, diffuseCoeff, texture,
                normalTexture),
            m_radius(radius)
        {}

        Vector2f Sphere::GetUV(const Vector3f& point) const
        {
            const Vector3f pos = (point - m_position).normalized();
            const Float u = 0.5 + std::atan2(pos.z(), pos.x()) / (M_PI * 2);
            const Float v = 0.5 - std::asin(pos.y()) / M_PI;

            return PrepareUV(u, v);
        }
        Vector3f Sphere::GetNormalAtPoint(const Vector3f& point) const
        {
            return GetNormalAtPointFromMap(point, (point - m_position).normalized());
        }
        double Sphere::Intersects(const Ray& ray) const
        {
            const Vector3f L = m_position - ray.Origin();
            const Float tca = L.dot(ray.Direction());
            if (tca < 0)
                return 0;

            const Float d2 = L.dot(L) - tca * tca;
            const Float squareRad = m_radius * m_radius;
            if (d2 > squareRad)
                return 0;

            const Float thc = std::sqrt(squareRad - d2);
            Float t0 = tca - thc;
            Float t1 = tca + thc;

            if (t0 > t1)
            {
                std::swap(t0, t1);
            }

            if (t0 < 0)
            {
                t0 = t1;
                if (t0 < 0)
                {
                    return 0;
                }
            }

            return t0;
        }
    }
}