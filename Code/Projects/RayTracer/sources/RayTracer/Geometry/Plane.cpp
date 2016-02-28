#include <climits>
#include "RayTracer/Ray.h"
#include "RayTracer/Geometry/Plane.h"

namespace RayTracer
{
    namespace Geometry
    {
        const Type Plane::defaultUVScale = 30;

        Plane::Plane(const Vector3& position,
            const Vector3& normal,
            const Color& diffuse,
            const Type& refractionCoeff,
            const Type& refraction,
            const Type& reflection,
            const Type& shininessCoeff,
            const Type& specularCoeff,
            const Type& diffuseCoeff,
            const Bitmap& texture,
            const Bitmap& normalTexture,
            const Type& uvScale) :
            Geometry(position, diffuse, refractionCoeff, refraction,
                reflection, shininessCoeff, specularCoeff,
                diffuseCoeff, texture, normalTexture),
            m_UVScale(uvScale),
            m_normal(normal)
        {}

        Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
            : Geometry(p1),
            m_normal(p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z())
        {
            m_normal = m_normal.cross(Vector3(p1.x() - p3.x(), p1.y() - p3.y(), p1.z() - p3.z()));
        }

        Vector3 Plane::GetNormalAtPoint(const Vector3& point) const
        {
            return GetNormalAtPointFromMap(point, m_normal);
        }

        Vector2 Plane::GetUV(const Vector3& point) const
        {
            Vector3 u = Vector3(m_normal.y(), -m_normal.x(), 0);
            u.normalize();

            const Vector3 v = m_normal.cross(u);

            Type u_coord = u.dot(point);
            Type v_coord = v.dot(point);

            return PrepareUV(u_coord / m_UVScale, v_coord / m_UVScale);
        }

        Type Plane::Intersects(const Ray& ray) const
        {
            Type denom = ray.Direction().normalized().dot(m_normal);
            if (std::abs(denom) >= std::numeric_limits<Type>::epsilon())
            {
                const Vector3 pl = m_position - ray.Origin();
                const Type t = pl.dot(m_normal) / denom;

                if (t >= 0)
                {
                    return t;
                }
            }

            return 0;
        }
    }
}