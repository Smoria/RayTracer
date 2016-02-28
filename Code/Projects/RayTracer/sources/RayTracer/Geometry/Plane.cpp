#include <climits>
#include "RayTracer/Ray.h"
#include "RayTracer/Geometry/Plane.h"

namespace RayTracer
{
    namespace Geometry
    {
        const Float Plane::defaultUVScale = 30;

        Plane::Plane(const Vector3f& position,
            const Vector3f& normal,
            const Color& diffuse,
            const Float& refractionCoeff,
            const Float& refraction,
            const Float& reflection,
            const Float& shininessCoeff,
            const Float& specularCoeff,
            const Float& diffuseCoeff,
            const Bitmap& texture,
            const Bitmap& normalTexture,
            const Float& uvScale) :
            Geometry(position, diffuse, refractionCoeff, refraction,
                reflection, shininessCoeff, specularCoeff,
                diffuseCoeff, texture, normalTexture),
            m_UVScale(uvScale),
            m_normal(normal)
        {}

        Plane::Plane(const Vector3f& p1, const Vector3f& p2, const Vector3f& p3)
            : Geometry(p1),
            m_normal(p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z())
        {
            m_normal = m_normal.cross(Vector3f(p1.x() - p3.x(), p1.y() - p3.y(), p1.z() - p3.z()));
        }

        Vector3f Plane::GetNormalAtPoint(const Vector3f& point) const
        {
            return GetNormalAtPointFromMap(point, m_normal);
        }

        Vector2f Plane::GetUV(const Vector3f& point) const
        {
            Vector3f u = Vector3f(m_normal.y(), -m_normal.x(), 0);
            u.normalize();

            const Vector3f v = m_normal.cross(u);

            Float u_coord = u.dot(point);
            Float v_coord = v.dot(point);

            return PrepareUV(u_coord / m_UVScale, v_coord / m_UVScale);
        }

        Float Plane::Intersects(const Ray& ray) const
        {
            Float denom = ray.Direction().normalized().dot(m_normal);
            if (std::abs(denom) >= std::numeric_limits<Float>::epsilon())
            {
                const Vector3f pl = m_position - ray.Origin();
                const Float t = pl.dot(m_normal) / denom;

                if (t >= 0)
                {
                    return t;
                }
            }

            return 0;
        }
    }
}