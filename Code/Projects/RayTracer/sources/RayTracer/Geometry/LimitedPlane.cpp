#include "RayTracer/Geometry/LimitedPlane.h"

namespace RayTracer
{
    namespace Geometry
    {
        static Vector3 CalculateNormal(const std::vector<Vector3>& limitedPoints)
        {
            if (limitedPoints.size() < 3)
            {
                throw std::exception();
            }

            const Vector3& p1 = limitedPoints[0];
            const Vector3& p2 = limitedPoints[1];
            const Vector3& p3 = limitedPoints[2];

            const Vector3 p12 = p1 - p2;
            const Vector3 p23 = p3 - p2;

            Vector3 result(p12.cross(p23));
            result.normalize();
            return result;
        }

        LimitedPlane::LimitedPlane(const Vector3& position,
            const std::vector<Vector3>& limitedPoint) :
            LimitedPlane(position, limitedPoint, CalculateNormal(limitedPoint))
        {}

        LimitedPlane::LimitedPlane(const Vector3& position,
            const std::vector<Vector3>& limitedPoint,
            const Vector3& normal) :
            Plane(position, normal),
            m_limitPoints(limitedPoint)
        {
            if (limitedPoint.size() < 3)
            {
                throw std::exception();
            }
        }

        Type LimitedPlane::Intersects(const Ray& ray) const
        {
            Type inter = Base::Intersects(ray);

            if (inter > 0)
            {
                const Vector3 point = GetIntersectPoint(ray, inter);
                if (!Contains(point))
                {
                    return 0;
                }
            }
            return inter;
        }

        bool LimitedPlane::Contains(const Vector3& point) const
        {
            int balance = 0;

            for (int idx1 = 0; idx1 < m_limitPoints.size(); idx1++)
            {
                int idx2 = idx1 + 1;
                if (idx2 >= m_limitPoints.size())
                {
                    idx2 = 0;
                }

                const Vector3& p1 = m_limitPoints[idx1];
                const Vector3& p2 = m_limitPoints[idx2];
                const Vector3 p3 = point - m_position;

                Type d = ((p2 - p1).cross(p3 - p1)).dot(m_normal);

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