#include "RayTracer/Geometry/Sphere.h"
#include "RayTracer/Ray.h"
#define _USE_MATH_DEFINES
#include <cmath>

namespace RayTracer
{
    namespace Geometry
    {
        Sphere::Sphere(const Vector3& position,
            const Type& radius,
            const Color& diffuse,
            const Type& refractionCoeff,
            const Type& refraction,
            const Type& reflection,
            const Type& shininessCoeff,
            const Type& specularCoeff,
            const Type& diffuseCoeff,
            const Bitmap& texture,
            const Bitmap& normalTexture) :
            Geometry(position, diffuse, refractionCoeff,
                refraction, reflection, shininessCoeff,
                specularCoeff, diffuseCoeff, texture,
                normalTexture),
            m_radius(radius)
        {}

        Vector2 Sphere::GetUV(const Vector3& point) const
        {
            const Vector3 pos = (point - m_position).normalized();
            const Type u = 0.5 + std::atan2(pos.z(), pos.x()) / (M_PI * 2);
            const Type v = 0.5 - std::asin(pos.y()) / M_PI;

            return PrepareUV(u, v);
        }
        Vector3 Sphere::GetNormalAtPoint(const Vector3& point) const
        {
            return GetNormalAtPointFromMap(point, (point - m_position).normalized());
        }
        double Sphere::Intersects(const Ray& ray) const
        {
            const Vector3 L = m_position - ray.Origin();
            const Type tca = L.dot(ray.Direction());
            if (tca < 0)
                return 0;

            const Type d2 = L.dot(L) - tca * tca;
            const Type squareRad = m_radius * m_radius;
            if (d2 > squareRad)
                return 0;

            const Type thc = std::sqrt(squareRad - d2);
            Type t0 = tca - thc;
            Type t1 = tca + thc;

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