#include "RayTracer/Geometry/LimitedPlane.h"

namespace RayTracer
{
    namespace Geometry
    {
        static Vector3f CalculateNormal(const std::vector<Vector3f>& limitedPoints)
        {
            if (limitedPoints.size() < 3)
            {
                throw std::exception();
            }

            const Vector3f& p1 = limitedPoints[0];
            const Vector3f& p2 = limitedPoints[1];
            const Vector3f& p3 = limitedPoints[2];

            const Vector3f p12 = p1 - p2;
            const Vector3f p23 = p3 - p2;

            Vector3f result(p12.cross(p23));
            result.normalize();
            return result;
        }

        LimitedPlane::LimitedPlane(const Vector3f& position,
            const std::vector<Vector3f>& limitedPoints,
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
            Plane(position, CalculateNormal(limitedPoints),
                diffuse, refractionCoeff, refraction,
                reflection, shininessCoeff, specularCoeff,
                diffuseCoeff, texture, normalTexture),
            m_limitPoints(limitedPoints)
        {}

        LimitedPlane::LimitedPlane(const Vector3f& position,
            const std::vector<Vector3f>& limitedPoints,
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
            Plane(position, normal, diffuse, refractionCoeff,
                refraction, reflection, shininessCoeff,
                specularCoeff, diffuseCoeff, texture, normalTexture),
            m_limitPoints(limitedPoints)
        {
            if (limitedPoints.size() < 3)
            {
                throw std::exception();
            }
        }

        Float LimitedPlane::Intersects(const Ray& ray) const
        {
            Float inter = Base::Intersects(ray);

            if (inter > 0)
            {
                const Vector3f point = GetIntersectPoint(ray, inter);
                if (!Contains(point))
                {
                    return 0;
                }
            }
            return inter;
        }

        bool LimitedPlane::Contains(const Vector3f& point) const
        {
            int balance = 0;

            for (int idx1 = 0; idx1 < m_limitPoints.size(); idx1++)
            {
                int idx2 = idx1 + 1;
                if (idx2 >= m_limitPoints.size())
                {
                    idx2 = 0;
                }

                const Vector3f& p1 = m_limitPoints[idx1];
                const Vector3f& p2 = m_limitPoints[idx2];
                const Vector3f p3 = point - m_position;

                Float d = ((p2 - p1).cross(p3 - p1)).dot(m_normal);

                if (d < 0)
                {
                    ++balance;
                }
                else if (d < 0)
                {
                    --balance;
                }
            }

            return balance == m_limitPoints.size();
        }
    }
}