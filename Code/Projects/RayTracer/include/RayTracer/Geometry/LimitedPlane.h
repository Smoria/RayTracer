#pragma once

#include <vector>
#include "Plane.h"

namespace RayTracer
{
    namespace Geometry
    {
        class LimitedPlane :
            public Plane
        {
            public:
                using Base = Plane;

                LimitedPlane(const Vector3& position,
                    const std::vector<Vector3>& limitedPoint);

                LimitedPlane(const Vector3& position,
                    const std::vector<Vector3>& limitedPoint,
                    const Vector3& normal);

                Type Intersects(const Ray& ray) const override;

                bool Contains(const Vector3& point) const;

            protected:
                std::vector<Vector3> m_limitPoints;
        };
    }
}